#include	"tonawindow3_pch.h"
#include	"tonawindow3_wbase.h"

#pragma comment(lib, "imm32.lib")

namespace NT3
{

// ****************************************************************
// コンストラクタ
// ================================================================
C_window_base::C_window_base()
{
	// パラメータを初期化
	init_param();
}

// ****************************************************************
// デストラクタ
// ================================================================
C_window_base::~C_window_base()
{
	if (h_large_icon)	{
		DestroyIcon(h_large_icon);
		h_large_icon = NULL;
	}
	if (h_small_icon)	{
		DestroyIcon(h_small_icon);
		h_small_icon = NULL;
	}
	unbind();
}

// ****************************************************************
// パラメータを初期化
// ================================================================
void C_window_base::init_param()
{
	// パラメータを初期化
	m_h_wnd = NULL;
	m_old_window_proc = NULL;
	m_is_dialog = false;
	m_is_limit_window_rect_min = false;
	m_is_attach_window = false;
	m_group_no = 0;
	h_large_icon = NULL;
	h_small_icon = NULL;
}

// ****************************************************************
// ウィンドウ登録
//		class_name : ウィンドウクラス名
//		icon_id : アイコンのリソースＩＤ（0 で無効）
//		menu_id : メニューのリソースＩＤ（0 で無効）
//		brush_type : 背景を塗りつぶすブラシ（-1 で無効）
// ================================================================
bool C_window_base::regist_class(CTSTR& class_name, int icon_id, int menu_id, int brush_type)
{
	// ウィンドウクラスの設定
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = window_proc_base;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = G_app.h_inst;
	wcex.hIcon = LoadIcon(G_app.h_inst, MAKEINTRESOURCE(icon_id));
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)GetStockObject(brush_type);
	wcex.lpszClassName = class_name.c_str();
	wcex.lpszMenuName = MAKEINTRESOURCE(menu_id);
	wcex.hIconSm = NULL;

	// ウィンドウの登録
	if (!RegisterClassEx(&wcex))	{
		set_last_error_win(_T("RegisterClassEx"));
		return false;
	}

	return true;
}

void C_window_base::unregist_class(CTSTR& class_name)
{
	UnregisterClass(class_name.c_str(), G_app.h_inst);
}

// ****************************************************************
// ウィンドウ作成
//		name : ウィンドウ名（ini 保存時に使用）
//		class_name : ウィンドウクラス名
//		caption : ウィンドウタイトル
//		x : ウィンドウの表示位置Ｘ（CW_USEDEFAULT : デフォルト、-1 : 中央）
//		y : ウィンドウの表示位置Ｙ（CW_USEDEFAULT : デフォルト、-1 : 中央）
//		client_w : ウィンドウのクライアント矩形の幅（CW_USEDEFAULT : デフォルト）
//		client_h : ウィンドウのクライアント矩形の高さ（CW_USEDEFAULT : デフォルト）
//		is_menu : メニューがある／ない
//		style : ウィンドウスタイル
//		style_ex : 拡張ウィンドウスタイル
//		is_limit_min : ウィンドウの最小矩形を制限
//		is_attach : 他のウィンドウにアタッチ
//		h_parent_wnd : 親ウィンドウハンドル
//		adjust_way : 親にくっつく方向（ﾃﾝｷｰ の方向で指定）
// ================================================================
bool C_window_base::create(CTSTR& name, CTSTR& class_name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD style, DWORD ex_style, bool is_limit_min, bool is_attach, HWND h_parent_wnd)
{
	if (m_h_wnd != NULL)
		return false;

	// ウィンドウの名前
	m_window_name = name;
	// ウィンドウの最小矩形を制限
	m_is_limit_window_rect_min = is_limit_min;
	// 他のウィンドウにアタッチ
	m_is_attach_window = is_attach;

	// ウィンドウの大きさの計算
	C_rect client_rect(0, 0, client_w, client_h);
	AdjustWindowRectEx(&client_rect, style, is_menu ? TRUE : FALSE, ex_style);

	// センタリング
	if (x == -1)	x = (GetSystemMetrics(SM_CXSCREEN) - client_rect.width()) / 2;
	if (y == -1)	y = (GetSystemMetrics(SM_CYSCREEN) - client_rect.height()) / 2;

	// ウィンドウの構築
	m_h_wnd = CreateWindowEx(ex_style,
		class_name.c_str(), caption.c_str(), style,
		x, y, client_rect.width(), client_rect.height(),
		h_parent_wnd, NULL, G_app.h_inst, this);

	// 構築に失敗
	if (m_h_wnd == NULL)	{
		set_last_error_win(_T("CreateWindowEx"));
		return false;
	}

	// 再びウィンドウハンドルとオブジェクトを結びつける
	// 既存のウィンドウクラス（"button" など）を直作成したときなどは、
	// この時点ではまだ結びついてないのです
	bind(m_h_wnd);

	// 他のウィンドウにアタッチ
	if (m_is_attach_window)
		m_attach.h_wnd_list.push_back(m_h_wnd);

	// IME を無効にする
//	ImmAssociateContext(h_wnd, NULL);

	return true;
}

// ****************************************************************
// ウィンドウを閉じる
// ================================================================
void C_window_base::close()
{
	if (m_h_wnd == NULL)
		return;

	send_message(WM_CLOSE, 0, 0);
}

// ****************************************************************
// ウィンドウを破壊する
// ================================================================
void C_window_base::destroy()
{
	if (m_h_wnd == NULL)
		return;

	DestroyWindow(m_h_wnd);
}

// ****************************************************************
// ウィンドウプロシージャ
// ================================================================
LRESULT CALLBACK C_window_base::window_proc_base(HWND h_wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// ウィンドウのプロパティリストから C_window_base オブジェクトへのポインタの取得を試みる
	C_window_base* p_target_wnd = (C_window_base *)GetProp(h_wnd, _T("this"));

	// C_window_base オブジェクトとウィンドウが結び付けられていない場合は
	// C_window_base オブジェクトとウィンドウを結び付ける
	if (!p_target_wnd)	{

		// ウィンドウの場合は CREATESTRUCT から Cwindow オブジェクトへのポインタを取得
		if (msg == WM_CREATE || msg == WM_NCCREATE)
			p_target_wnd = (C_window_base *)((CREATESTRUCT *)lp)->lpCreateParams;
		// ダイアログの場合
		else if (msg == WM_INITDIALOG)	{
			// プロパティシートかどうかを判定
			//		--- 判定方法が分からないので、先頭 DWORD を見て無理やり判断しています。
			DWORD top = *(DWORD *)lp;
			// 普通のダイアログの場合は lp からインスタンスを取得
			if (top != sizeof(PROPSHEETPAGE))
				p_target_wnd = (C_window_base *)lp;
			// プロパティシートの場合は psp 構造体からインスタンスを取得
			else
				p_target_wnd = (C_window_base *)((PROPSHEETPAGE *)lp)->lParam;
		}
		// ウィンドウハンドルとC_window_baseオブジェクトを結び付ける
		if (p_target_wnd)
			p_target_wnd->bind(h_wnd);
	}

	// p_target_wnd の取得に成功した場合は、C_window_base の window_proc を呼び出す
	if (p_target_wnd)	{

		p_target_wnd->window_proc_pre(msg, wp, lp);
		LRESULT result = p_target_wnd->window_proc(msg, wp, lp);
		// WM_DESTROY メッセージでウィンドウと C_window_base オブジェクトを切り離す
		if (msg == WM_DESTROY)
			p_target_wnd->unbind();

		return result;
	}

	// ダイアログの場合、FALSE を返す
	if (GetWindowLong(h_wnd, DWL_DLGPROC))
		return FALSE;

	// デフォルトのウィンドウプロシージャを呼び出す
	return DefWindowProc(h_wnd, msg, wp, lp);
}

// ****************************************************************
// 継承されるウィンドウプロシージャ（プレ）
// ================================================================
void C_window_base::window_proc_pre(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_CREATE:		on_create_pre();		break;
	}
}

// ****************************************************************
// ウィンドウが作られた（プレ）
// ================================================================
void C_window_base::on_create_pre()
{
	if (m_h_wnd == NULL)	{	return;	}
	// ウィンドウ矩形を取得
	m_def_window_rect = get_window_rect();
	// クライアント矩形を取得
	m_def_client_rect = get_client_rect();
	// ウィンドウを登録する
	G_app.window_list.push_back(m_h_wnd);
}

// ****************************************************************
// 継承されるウィンドウプロシージャ
// ================================================================
LRESULT C_window_base::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (!m_h_wnd)
		return FALSE;

	switch (msg)	{
		case WM_CLOSE:		if (!on_close())			return FALSE;	break;
		case WM_DESTROY:	if (!on_destroy())			return FALSE;	break;
		case WM_MOVING:		if (!on_moving(wp, lp))		return FALSE;	break;
		case WM_SIZING:		if (!on_sizing(wp, lp))		return FALSE;	break;

		case WM_SYSCOMMAND:
			// ウィンドウの移動／サイズ変更が開始された
			if ((wp & SC_MOVE) || (wp & SC_SIZE))	{
				attach_init();	// アタッチの情報を初期化
			}
			break;
	}

	// 古いプロシージャがある場合そのプロシージャを呼び出す
	if (m_old_window_proc)
		return CallWindowProc(m_old_window_proc, m_h_wnd, msg, wp, lp);

	// ダイアログの場合、FALSE を返す
	if (GetWindowLong(m_h_wnd, DWL_DLGPROC))
		return FALSE;

	// デフォルトのウィンドウプロシージャを呼び出す
	return DefWindowProc(m_h_wnd, msg, wp, lp);
}

// ****************************************************************
// ウィンドウハンドルとオブジェクトを結びつける
// ================================================================
bool C_window_base::bind(HWND h_wnd)
{
	if (!h_wnd)
		return true;

	// ハンドルの決定
	m_h_wnd = h_wnd;

	// ウィンドウハンドルと C_window_base オブジェクトを結びつける
	SetProp(m_h_wnd, _T("this"), (HANDLE)this);

	// 親からのウィンドウ位置を計算
	C_window_base* p_parent_wnd = (C_window_base *)GetProp(get_parent_handle(), _T("this"));
	if (p_parent_wnd)
		m_def_rltv_rect = p_parent_wnd->screen_to_client(get_window_rect());

	// ダイアログかウィンドウかを判定する
	m_is_dialog = GetWindowLongPtr(m_h_wnd, DWL_DLGPROC) != FALSE;

	// 既存のウィンドウをサブクラス化する場合は、
	// ウィンドウ（ダイアログ）プロシージャを window_proc_base に置き換える
	if (GetWindowLongPtr(m_h_wnd, m_is_dialog ? DWL_DLGPROC : GWL_WNDPROC) != (PTR)window_proc_base)
		m_old_window_proc = (WNDPROC)(PTR)SetWindowLongPtr(m_h_wnd, m_is_dialog ? DWL_DLGPROC : GWL_WNDPROC, (LONG)(PTR)window_proc_base);

	return true;
}

// ****************************************************************
// ウィンドウハンドルをオブジェクトから切り離す
// ================================================================
void C_window_base::unbind()
{
	if (!m_h_wnd)
		return;

	// ウィンドウがサブクラス化されている場合は、
	// ウィンドウ（ダイアログ）プロシージャを元に戻す
	if (m_old_window_proc)	{
		SetWindowLongPtr(m_h_wnd, (m_is_dialog ? DWL_DLGPROC : GWL_WNDPROC), (LONG)(PTR)m_old_window_proc);
		m_old_window_proc = NULL;
	}

	// ウィンドウハンドルを Cwindow オブジェクトから切り離す
	RemoveProp(m_h_wnd, _T("this"));

	// メインウィンドウの場合は app のハンドルを初期化
	if (m_h_wnd == G_app.h_wnd)
		G_app.h_wnd = NULL;
	
	// ハンドルを初期化
	m_h_wnd = NULL;
}

// ****************************************************************
// ウィンドウが閉じられた
// ================================================================
bool C_window_base::on_close()
{
	if (m_h_wnd == NULL)
		return true;

	destroy();
	return true;
}

// ****************************************************************
// ウィンドウが破壊された
// ================================================================
bool C_window_base::on_destroy()
{
	// 他のウィンドウにアタッチを解除
	if (m_is_attach_window)
		m_attach.h_wnd_list.erase(std::remove(m_attach.h_wnd_list.begin(), m_attach.h_wnd_list.end(), m_h_wnd), m_attach.h_wnd_list.end());
	// ウィンドウリストから削除
	G_app.window_list.erase(std::remove(G_app.window_list.begin(), G_app.window_list.end(), m_h_wnd), G_app.window_list.end());
	// メインウィンドウの場合アプリケーションを終了する
	if (m_h_wnd == G_app.h_wnd)
		PostQuitMessage(0);

	return true;
}

// ****************************************************************
// ウィンドウが動かされている
// ================================================================
bool C_window_base::on_moving(WPARAM wp, LPARAM lp)
{
	// 他のウィンドウにアタッチ
	if (m_is_attach_window)
		attach_on_moving(wp, lp);

	return true;
}

// ****************************************************************
// ウィンドウサイズが変更されている
// ================================================================
bool C_window_base::on_sizing(WPARAM wp, LPARAM lp)
{
	// 他のウィンドウにアタッチ
	if (m_is_attach_window)
		attach_on_sizing(wp, lp);

	// ウィンドウサイズを制限
	if (m_is_limit_window_rect_min)
		limit_window_size(wp, lp, m_def_window_rect);

	return true;
}

// ****************************************************************
// ウィンドウの最小矩形の制限処理
// ================================================================
void C_window_base::limit_window_size(WPARAM wp, LPARAM lp, C_rect min_rect)
{
	C_rect* rp = (C_rect *)lp;
	int width = std::max(min_rect.width(), rp->width());
	int height = std::max(min_rect.height(), rp->height());

	switch (wp)	{
		case WMSZ_BOTTOM:
			rp->bottom = rp->top + height;
			break;
		case WMSZ_BOTTOMLEFT:
			rp->left = rp->right - width;
			rp->bottom = rp->top + height;
			break;
		case WMSZ_BOTTOMRIGHT:
			rp->right = rp->left + width;
			rp->bottom = rp->top + height;
			break;
		case WMSZ_LEFT:
			rp->left = rp->right - width;
			break;
		case WMSZ_RIGHT:
			rp->right = rp->left + width;
			break;
		case WMSZ_TOP:
			rp->top = rp->bottom - height;
			break;
		case WMSZ_TOPLEFT:
			rp->top = rp->bottom - height;
			rp->left = rp->right - width;
			break;
		case WMSZ_TOPRIGHT:
			rp->top = rp->bottom - height;
			rp->right = rp->left + width;
			break;
	}
}

// ****************************************************************
// 親ウィンドウのサイズ変更に合わせて動く
// ================================================================
void C_window_base::adjust_to_parent(int dw, int dh)
{
	// 向きが 0 のときは移動しない
	if (m_adjust_way > 0)	{

		// 動く方向はテンキーの配置を参考に（fe. 7=左上）
		int adjust_h = (m_adjust_way - 1) % 3;	// 横の動き
		int adjust_v = (9 - m_adjust_way) / 3;	// 縦の動き

		// 新しいウィンドウレクトを計算
		C_rect new_rect = m_def_rltv_rect;
		if (adjust_h == 2)						new_rect.left += dw;
		if (adjust_v == 2)						new_rect.top += dh;
		if (adjust_h == 1 || adjust_h == 2)		new_rect.right += dw;
		if (adjust_v == 1 || adjust_v == 2)		new_rect.bottom += dh;

		// 移動して再描画
		set_window_rect(new_rect);
		invalidate_rect(NULL, false);
	}
}

// ****************************************************************
// 親ウィンドウのサイズ変更に合わせて動く
// ================================================================
void C_window_base::adjust_to_parent_4(int dl, int dt, int dw, int dh)
{
	C_rect new_rect = m_def_rltv_rect;
	new_rect.left += dl;
	new_rect.top += dt;
	new_rect.right += dl + dw;
	new_rect.bottom += dt + dh;

	// 移動して再描画
	set_window_rect(new_rect);
	invalidate_rect(NULL, false);
}


// ****************************************************************
// ウィンドウアタッチ
// ================================================================
WINDOW_ATTACH	C_window_base::m_attach;

// ****************************************************************
// ウィンドウアタッチ：初期化
// ================================================================
void C_window_base::attach_init()
{
	m_attach.mod = 0;
}

// ****************************************************************
// ウィンドウアタッチ（ウィンドウ移動時）
// ================================================================
void C_window_base::attach_on_moving(WPARAM wp, LPARAM lp)
{
	C_rect my_rect, target_rect;
	C_point point;
	WINDOW_ATTACH_PROC wap;

	my_rect = *(C_rect *)lp;
	GetCursorPos(&point);

	// 初期情報の記憶
	if (m_attach.h_wnd != m_h_wnd || m_attach.mod != 1){
		m_attach.h_wnd = m_h_wnd;
		m_attach.mod = 1;	// 1=移動時
		m_attach.left   = true;
		m_attach.right  = true;
		m_attach.top    = true;
		m_attach.bottom = true;
		m_attach.x = point.x - my_rect.left;
		m_attach.y = point.y - my_rect.top;
		m_attach.w = my_rect.width();
		m_attach.h = my_rect.height();
	}

	my_rect.left   = point.x - m_attach.x;
	my_rect.top    = point.y - m_attach.y;
	my_rect.right  = my_rect.left + m_attach.w;
	my_rect.bottom = my_rect.top  + m_attach.h;

	// 自分以外のウィンドウの縁
	wap.width_h_wnd  = NULL;
	wap.height_h_wnd = NULL;
	for (ARRAY<HWND>::iterator itr = m_attach.h_wnd_list.begin(); itr != m_attach.h_wnd_list.end(); ++itr)	{
		if (*itr != m_h_wnd)	{
			// 表示されているウィンドウに対してのみアタッチ
			if (GetWindowLongPtr(*itr, GWL_STYLE) & WS_VISIBLE)	{
				GetWindowRect(*itr, &target_rect);
				attach_check(*itr, &my_rect, &target_rect, &wap);
			}
		}
	}

	// デスクトップ全体の縁
	wap.desktop_width_flag  = false;
	wap.desktop_height_flag = false;
	get_my_desktop_work_area_rect(&target_rect);
	attach_check(NULL, &my_rect, &target_rect, &wap);

	// 補正
	attach_rep(&my_rect, &wap);

	*(C_rect *)lp = my_rect;
}

// ****************************************************************
// ウィンドウアタッチ（ウィンドウサイズ変更時）
// ================================================================
void C_window_base::attach_on_sizing(WPARAM wp, LPARAM lp)
{
	C_rect my_rect, target_rect;
	C_point point;
	WINDOW_ATTACH_PROC wap;

	my_rect = *(C_rect *)lp;
	GetCursorPos(&point);

	// 初期情報の記憶
	if (m_attach.h_wnd != m_h_wnd || m_attach.mod != 2)	{
		m_attach.h_wnd = m_h_wnd;
		m_attach.mod = 2;	// 2=サイズ変更時
		m_attach.left   = false;
		m_attach.right  = false;
		m_attach.top    = false;
		m_attach.bottom = false;
		if (wp == WMSZ_LEFT   || wp == WMSZ_TOPLEFT    || wp == WMSZ_BOTTOMLEFT)	{ m_attach.x = point.x - my_rect.left;   m_attach.left   = true; }
		if (wp == WMSZ_RIGHT  || wp == WMSZ_TOPRIGHT   || wp == WMSZ_BOTTOMRIGHT)	{ m_attach.x = point.x - my_rect.right;  m_attach.right  = true; }
		if (wp == WMSZ_TOP    || wp == WMSZ_TOPLEFT    || wp == WMSZ_TOPRIGHT)		{ m_attach.y = point.y - my_rect.top;    m_attach.top    = true; }
		if (wp == WMSZ_BOTTOM || wp == WMSZ_BOTTOMLEFT || wp == WMSZ_BOTTOMRIGHT)	{ m_attach.y = point.y - my_rect.bottom; m_attach.bottom = true; }
	}

	if (m_attach.left)		{ my_rect.left   = point.x - m_attach.x; }
	if (m_attach.right)		{ my_rect.right  = point.x - m_attach.x; }
	if (m_attach.top)		{ my_rect.top    = point.y - m_attach.y; }
	if (m_attach.bottom)	{ my_rect.bottom = point.y - m_attach.y; }

	// 自分以外のウィンドウの縁
	wap.width_h_wnd  = NULL;
	wap.height_h_wnd = NULL;
	for (ARRAY<HWND>::iterator itr = m_attach.h_wnd_list.begin(); itr != m_attach.h_wnd_list.end(); ++itr)	{
		if (*itr != m_h_wnd)	{
			// 表示されているウィンドウに対してのみアタッチ
			if (GetWindowLongPtr(*itr, GWL_STYLE) & WS_VISIBLE)	{
				GetWindowRect(*itr, &target_rect);
				attach_check(*itr, &my_rect, &target_rect, &wap);
			}
		}
	}

	// デスクトップ全体の縁
	wap.desktop_width_flag  = false;
	wap.desktop_height_flag = false;
	get_my_desktop_work_area_rect(&target_rect);
	attach_check(NULL, &my_rect, &target_rect, &wap);

	// 補正
	attach_rep(&my_rect, &wap);

	*(C_rect *)lp = my_rect;
}

// ****************************************************************
// ウィンドウアタッチ：チェック
// ================================================================
void C_window_base::attach_check(HWND h_target_wnd, C_rect *rect1, C_rect *rect2, WINDOW_ATTACH_PROC *wap)
{
	int tmp;
	if (
		rect1->left      < (rect2->right  + WINDOW_ATTACH_REP_SIZE)
		&& rect1->right  > (rect2->left   - WINDOW_ATTACH_REP_SIZE)
		&& rect1->top    < (rect2->bottom + WINDOW_ATTACH_REP_SIZE)
		&& rect1->bottom > (rect2->top    - WINDOW_ATTACH_REP_SIZE))
	{
		if (m_attach.left)	{
			tmp = rect2->left - rect1->left;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod = 0;
			}
			tmp = rect2->right - rect1->left;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod = 1;
			}
		}
		if (m_attach.right)	{
			tmp = rect2->left - rect1->right;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod  = 2;
			}
			tmp = rect2->right - rect1->right;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_width_flag = true;
				else						wap->width_h_wnd = h_target_wnd;
				wap->width_mod  = 3;
			}
		}
		if (m_attach.top)	{
			tmp = rect2->top - rect1->top;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 0;
			}
			tmp = rect2->bottom - rect1->top;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 1;
			}
		}
		if (m_attach.bottom)	{
			tmp = rect2->top - rect1->bottom;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 2;
			}
			tmp = rect2->bottom - rect1->bottom;
			if (-(WINDOW_ATTACH_REP_SIZE + 1) < tmp && tmp < (WINDOW_ATTACH_REP_SIZE + 1))	{
				if (h_target_wnd == NULL)	wap->desktop_height_flag = true;
				else						wap->height_h_wnd = h_target_wnd;
				wap->height_mod = 3;
			}
		}
	}
}

// ****************************************************************
// ウィンドウアタッチ：補正
// ================================================================
void C_window_base::attach_rep(C_rect *rect1, WINDOW_ATTACH_PROC *wap)
{
	int tmp;
	C_rect rect2;

	if (wap->desktop_width_flag || wap->width_h_wnd != NULL)	{
		if (wap->desktop_width_flag)	{
			get_my_desktop_work_area_rect(&rect2);
		}
		else if (wap->width_h_wnd != NULL)	{
			GetWindowRect(wap->width_h_wnd, &rect2);
		}
		switch (wap->width_mod)	{
			case 0: tmp = rect2.left  - rect1->left;  break;
			case 1: tmp = rect2.right - rect1->left;  break;
			case 2: tmp = rect2.left  - rect1->right; break;
			case 3: tmp = rect2.right - rect1->right; break;
		}
		if (m_attach.left)	{ rect1->left  += tmp; }
		if (m_attach.right)	{ rect1->right += tmp; }
	}

	if (wap->desktop_height_flag || wap->height_h_wnd != NULL)	{
		if (wap->desktop_height_flag)	{
			get_my_desktop_work_area_rect(&rect2);
		}
		else if (wap->height_h_wnd != NULL)	{
			GetWindowRect(wap->height_h_wnd, &rect2);
		}
		switch (wap->height_mod)	{
			case 0: tmp = rect2.top    - rect1->top;    break;
			case 1: tmp = rect2.bottom - rect1->top;    break;
			case 2: tmp = rect2.top    - rect1->bottom; break;
			case 3: tmp = rect2.bottom - rect1->bottom; break;
		}
		if (m_attach.top)		{ rect1->top    += tmp; }
		if (m_attach.bottom)	{ rect1->bottom += tmp; }
	}
}









// ****************************************************************
// ハンドルを取得
// ================================================================
HWND C_window_base::get_handle()
{
	return m_h_wnd;
}

// ****************************************************************
// id を取得
// ================================================================
WORD C_window_base::get_id()
{
	return (WORD)(PTR)GetWindowLongPtr(m_h_wnd, GWLP_ID);
}

// ****************************************************************
// クラスを取得
// ================================================================
TSTR C_window_base::get_class()
{
	TCHAR buf[1024];
	GetClassName(m_h_wnd, buf, 1024);

	return buf;
}

// ****************************************************************
// 名前を取得
// ================================================================
TSTR C_window_base::get_name()
{
	return m_window_name;
}

// ****************************************************************
// 名前を設定
// ================================================================
void C_window_base::set_name(CTSTR& name)
{
	m_window_name = name;
}

// ****************************************************************
// 親へのくっつき方を設定
// ================================================================
void C_window_base::set_adjust_way(int adjust_way)
{
	m_adjust_way = adjust_way;
}

// ****************************************************************
// グループ番号を取得
// ================================================================
int C_window_base::get_group_no()
{
	return m_group_no;
}

// ****************************************************************
// グループ番号を設定
// ================================================================
void C_window_base::set_group_no(int group_no)
{
	m_group_no = group_no;
}

// ****************************************************************
// 親ウィンドウ取得
// ================================================================
C_window_base* C_window_base::get_parent()
{
	return (C_window_base *)::GetProp(get_parent_handle(), _T("this"));
}

// ****************************************************************
// 親ウィンドウのハンドルを取得
// ================================================================
HWND C_window_base::get_parent_handle()
{
	return (HWND)(PTR)GetWindowLongPtr(m_h_wnd, GWLP_HWNDPARENT);
}

// ****************************************************************
// 親ウィンドウの名前を取得
// ================================================================
TSTR C_window_base::get_parent_name()
{
	C_window_base* parent = get_parent();
	if (parent)
		return parent->get_name();

	return _T("");
}

// ****************************************************************
// メインウィンドウに設定
// ================================================================
void C_window_base::set_main_window()
{
	G_app.h_wnd = m_h_wnd;
}

// ****************************************************************
// 位置と大きさを設定
// ================================================================
void C_window_base::set_window_pos(C_point pos)
{
	if (m_h_wnd == NULL)
		return;

	::SetWindowPos(m_h_wnd, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);			// 位置
}

void C_window_base::set_client_pos(C_point pos)
{
	if (m_h_wnd == NULL)
		return;

	// センタリング
	if (pos.x == -1)	pos.x = (GetSystemMetrics(SM_CXSCREEN) - get_client_rect().width()) / 2;
	if (pos.y == -1)	pos.y = (GetSystemMetrics(SM_CYSCREEN) - get_client_rect().height()) / 2;

	// クライアント座標 → ウィンドウ座標
	C_rect rect = client_to_window(C_rect(pos.x, pos.y, 0, 0));
	::SetWindowPos(m_h_wnd, 0, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);	// 位置
}

void C_window_base::set_window_size(C_size size)
{
	if (m_h_wnd == NULL)
		return;

	::SetWindowPos(m_h_wnd, 0, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);		// 大きさ
}

void C_window_base::set_client_size(C_size size)
{
	if (m_h_wnd == NULL)
		return;

	// クライアント座標 → ウィンドウ座標
	C_rect rect = client_to_window(C_rect(0, 0, size.cx, size.cy));
	::SetWindowPos(m_h_wnd, 0, 0, 0, rect.width(), rect.height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);		// 大きさ
}

void C_window_base::set_window_rect(C_rect rect)
{
	if (m_h_wnd == NULL)
		return;

	::SetWindowPos(m_h_wnd, 0, rect.left, rect.top, rect.width(), rect.height(), SWP_NOZORDER | SWP_NOACTIVATE);	// 位置と大きさ
}

void C_window_base::set_client_rect(C_rect rect)
{
	if (m_h_wnd == NULL)
		return;

	// クライアント座標 → ウィンドウ座標
	rect = client_to_window(rect);
	::SetWindowPos(m_h_wnd, 0, rect.left, rect.top, rect.width(), rect.height(), SWP_NOZORDER | SWP_NOACTIVATE);	// 位置と大きさ
}

// ****************************************************************
// 位置と大きさを取得
// ================================================================
C_rect C_window_base::get_window_rect()
{
	C_rect rect;
	GetWindowRect(m_h_wnd, &rect);
	return rect;
}

C_rect C_window_base::get_client_rect()
{
	C_rect rect;
	GetClientRect(m_h_wnd, &rect);
	return rect;
}

C_rect C_window_base::get_rltv_rect()
{
	C_rect rect = get_window_rect();
	HWND parent_h_wnd = get_parent_handle();
	C_point lt = rect.left_top();		ScreenToClient(parent_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ScreenToClient(parent_h_wnd, &rb);
	return C_rect(lt.x, lt.y, rb.x, rb.y);
}

// ****************************************************************
// デフォルトの位置と大きさを取得／設定
// ================================================================
C_rect C_window_base::get_def_window_rect()
{
	return m_def_window_rect;
}

void C_window_base::set_def_window_rect(C_rect rect)
{
	m_def_window_rect = rect;
}

C_rect C_window_base::get_def_client_rect()
{
	return m_def_client_rect;
}

void C_window_base::set_def_client_width(int width)
{
	C_rect new_rect = m_def_client_rect;

	new_rect.right = new_rect.left + width;

	m_def_client_rect = new_rect;
}

void C_window_base::set_def_client_height(int height)
{
	C_rect new_rect = m_def_client_rect;

	new_rect.bottom = new_rect.top + height;

	m_def_client_rect = new_rect;
}

void C_window_base::set_def_client_rect(C_rect rect)
{
	m_def_client_rect = rect;
}

C_rect C_window_base::get_def_rltv_rect()
{
	return m_def_rltv_rect;
}

void C_window_base::set_def_rltv_rect(C_rect rect)
{
	m_def_rltv_rect = rect;
}

// ****************************************************************
// 現在の矩形に合わせてデフォルトの矩形を再設定する
// ================================================================

void C_window_base::set_def_rect_from_now_rect()
{
	set_def_window_rect_from_now_rect();
	set_def_client_rect_from_now_rect();
	set_def_rltv_rect_from_now_rect();
}

// 現在の矩形に合わせてデフォルトのウィンドウ矩形を再設定する
void C_window_base::set_def_window_rect_from_now_rect()
{
	set_def_window_rect(get_window_rect());
}

// 現在の矩形に合わせてデフォルトのクライアント矩形を再設定する
void C_window_base::set_def_client_rect_from_now_rect()
{
	set_def_client_rect(get_client_rect());
}

// 現在の矩形に合わせてデフォルトの相対矩形を再設定する
void C_window_base::set_def_rltv_rect_from_now_rect()
{
	C_rect rect = get_window_rect();
	HWND parent_h_wnd = get_parent_handle();
	C_point lt = rect.left_top();		ScreenToClient(parent_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ScreenToClient(parent_h_wnd, &rb);
	set_def_rltv_rect(C_rect(lt.x, lt.y, rb.x, rb.y));
}

// ****************************************************************
// 現在のサイズに合わせてデフォルトの矩形を再設定する
// ================================================================

void C_window_base::set_def_rect_from_now_size()
{
	set_def_window_rect_from_now_size();
	set_def_client_rect_from_now_size();
	set_def_rltv_rect_from_now_size();
}

// 現在のサイズに合わせてデフォルトのウィンドウ矩形を再設定する
void C_window_base::set_def_window_rect_from_now_size()
{
	C_rect now_w_rect = get_window_rect();
	C_rect def_w_rect = get_def_window_rect();
	set_def_window_rect(C_rect(def_w_rect.left, def_w_rect.top, def_w_rect.left + now_w_rect.width(), def_w_rect.top + now_w_rect.height()));
}

// 現在のサイズに合わせてデフォルトのクライアント矩形を再設定する
void C_window_base::set_def_client_rect_from_now_size()
{
	C_rect now_w_rect = get_window_rect();
	C_rect def_c_rect = get_def_client_rect();
	set_def_window_rect(C_rect(def_c_rect.left, def_c_rect.top, def_c_rect.left + now_w_rect.width(), def_c_rect.top + now_w_rect.height()));
}

// 現在のサイズに合わせてデフォルトの相対矩形を再設定する
void C_window_base::set_def_rltv_rect_from_now_size()
{
	C_rect now_w_rect = get_window_rect();
	C_rect def_r_rect = get_def_rltv_rect();
	set_def_rltv_rect(C_rect(def_r_rect.left, def_r_rect.top, def_r_rect.left + now_w_rect.width(), def_r_rect.top + now_w_rect.height()));
}

// ****************************************************************
// ウィンドウサイズを設定してデフォルトの矩形を再設定する
// ================================================================

void C_window_base::set_window_size_with_def_size(int width, int height)
{
	set_window_size(C_size(width, height));
	set_def_rect_from_now_rect();
}

void C_window_base::set_window_size_with_def_size(C_size size)
{
	set_window_size_with_def_size(size.cx, size.cy);
}

// ****************************************************************
// ウィンドウサイズを設定してデフォルトの矩形を再設定する
// ================================================================

void C_window_base::add_window_size_with_def_size(int rep_width, int rep_height)
{
	C_rect rect = get_window_rect();
	set_window_size_with_def_size(rect.width() + rep_width, rect.height() + rep_height);
}

void C_window_base::add_window_size_with_def_size(C_size rep_size)
{
	add_window_size_with_def_size(rep_size.cx, rep_size.cy);
}

// ****************************************************************
// 相対座標を設定してデフォルトの矩形を再設定する
// ================================================================

void C_window_base::set_rltv_pos_with_def_rect(int x, int y)
{
	set_window_pos(C_point(x, y));
	set_def_rect_from_now_rect();
}

void C_window_base::set_rltv_pos_with_def_rect(C_point pos)
{
	set_rltv_pos_with_def_rect(pos.x, pos.y);
}

// ****************************************************************
// 相対座標を調整してデフォルトの矩形を再設定する
// ================================================================

void C_window_base::add_rltv_pos_with_def_rect(int rep_x, int rep_y)
{
	C_rect rect = get_rltv_rect();
	set_rltv_pos_with_def_rect(rect.left + rep_x, rect.top + rep_y);
}

void C_window_base::add_rltv_pos_with_def_rect(C_point rep_pos)
{
	add_rltv_pos_with_def_rect(rep_pos.x, rep_pos.y);
}

// ****************************************************************
// ウィンドウの存在するモニタのデスクトップ矩形取得
// ================================================================
void C_window_base::get_my_desktop_rect(C_rect *desktop_rect)
{
	C_rect window_rect = get_window_rect();

	get_desktop_rect_from_pos(desktop_rect, C_point(window_rect.left + window_rect.width() / 2, window_rect.top + window_rect.height() / 2));
}

// ****************************************************************
// ウィンドウの存在するモニタのワークエリア矩形取得（デスクトップからタスクバーなどの領域を除外した領域を取得）
// ================================================================
void C_window_base::get_my_desktop_work_area_rect(C_rect *work_area_rect)
{
	C_rect window_rect = get_window_rect();

	get_work_area_rect_from_pos(work_area_rect, C_point(window_rect.left + window_rect.width() / 2, window_rect.top + window_rect.height() / 2));
}

// ****************************************************************
// ウィンドウの存在するモニタの中央に移動
// ================================================================
void C_window_base::set_pos_my_desktop_center()
{
	C_rect my_desktop_rect;
	get_my_desktop_rect(&my_desktop_rect);

	int x = (my_desktop_rect.width() - get_window_rect().width()) / 2 + my_desktop_rect.left;
	int y = (my_desktop_rect.height() - get_window_rect().height()) / 2 + my_desktop_rect.top;
	set_window_pos(C_point(x, y));
}

// ****************************************************************
// デスクトップ範囲外にある場合、ウィンドウ座標をデスクトップの中心に補正する
// ================================================================
bool C_window_base::rep_window_pos_outside_desktop(int rep_mod, int outside_size)
{
	if (m_h_wnd == NULL)
		return false;

	bool ret = false;

	// ワークエリア取得
	C_rect work_area_rect;
	get_my_desktop_work_area_rect(&work_area_rect);

	// 仮想デスクトップ矩形取得
	C_rect virtual_desktop_rect;
	virtual_desktop_rect.left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	virtual_desktop_rect.top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	virtual_desktop_rect.right = virtual_desktop_rect.left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	virtual_desktop_rect.bottom = virtual_desktop_rect.top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

	// ウィンドウ矩形を取得
	C_rect window_rect = get_window_rect();

	// 領域補正
	C_rect wor;
	wor.left = work_area_rect.left + outside_size;
	wor.right = work_area_rect.right - outside_size;
	wor.top = work_area_rect.top + outside_size;
	wor.bottom = work_area_rect.bottom - outside_size;

	if (window_rect.left > wor.right || window_rect.right < wor.left || window_rect.top > wor.bottom || window_rect.bottom < wor.top)	{
		C_point new_window_pos = C_point(window_rect.left, window_rect.top);
		switch (rep_mod)	{
			case 1:				// ちょうど範囲内に納まるようにする
				if (window_rect.left > wor.right)	{	new_window_pos.x = work_area_rect.right - window_rect.width();		}
				if (window_rect.right < wor.left)	{	new_window_pos.x = 0;	}
				if (window_rect.top > wor.bottom)	{	new_window_pos.y = work_area_rect.bottom - window_rect.height();	}
				if (window_rect.bottom < wor.top)	{	new_window_pos.y = 0;	}
				break;
			case 2:				// 中央にする
				{
					C_size desktop_size = C_size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
					new_window_pos = C_point((desktop_size.cx - window_rect.width()) / 2, (desktop_size.cy - window_rect.height()) / 2);
				}
				break;
			case 0:	default:	// 補正値分のみ納まるようにする
				if (window_rect.left > wor.right)	{	new_window_pos.x = work_area_rect.right - outside_size;		}
				if (window_rect.right < wor.left)	{	new_window_pos.x = -(window_rect.width() - outside_size);	}
				if (window_rect.top > wor.bottom)	{	new_window_pos.y = work_area_rect.bottom - outside_size;	}
				if (window_rect.bottom < wor.top)	{	new_window_pos.y = -(window_rect.height() - outside_size);	}	// ▲これ間違ってるかも。work_area_rect.top 足さなきゃだめじゃね？
				break;
		}
		set_window_pos(new_window_pos);
		ret = true;
	}

	// 最終的にタスクバーがデスクトップの範囲外にあるようならさらに補正する（上下のみ補正）
	DWORD style = (DWORD)get_style();
	if (style & WS_CAPTION)	{		// キャプションバーを持っている
//	{
		C_rect window_rect = get_window_rect();
		if (window_rect.top < virtual_desktop_rect.top)	{
			window_rect.top = virtual_desktop_rect.top;
			set_window_pos(C_point(window_rect.left, window_rect.top));
			ret = true;
		}
		else	{
			DWORD ex_style = (DWORD)get_style_ex();
			int caption_size = (ex_style & WS_EX_TOOLWINDOW) ? GetSystemMetrics(SM_CYSMCAPTION) : GetSystemMetrics(SM_CYCAPTION);		// ツールウィンドウの場合、スモールキャプション
			if (window_rect.top > work_area_rect.bottom - caption_size)	{
				window_rect.top = work_area_rect.bottom - caption_size;
				set_window_pos(C_point(window_rect.left, window_rect.top));
				ret = true;
			}
		}
	}

	return ret;
}

// ****************************************************************
// 最前面に固定
// ================================================================
void C_window_base::set_top_most(bool top_most)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowPos(m_h_wnd, (top_most ? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

// ****************************************************************
// フォーカスを設定
// ================================================================
void C_window_base::set_focus()
{
	SetFocus(m_h_wnd);
}

// ****************************************************************
// アクティブにする
// ================================================================
void C_window_base::set_active()
{
	SetActiveWindow(m_h_wnd);
}

// ****************************************************************
// 最前面に設定
// ================================================================
void C_window_base::set_fore_ground()
{
	SetForegroundWindow(m_h_wnd);
}

// ****************************************************************
// 表示／非表示を設定
// ================================================================
void C_window_base::set_show(bool show)
{
	if (m_h_wnd == NULL)
		return;

	if (get_show() != show)
		ShowWindow(m_h_wnd, show ? SW_SHOW : SW_HIDE);
}

// ****************************************************************
// 表示／非表示を取得
// ================================================================
bool C_window_base::get_show()
{
	return (GetWindowLongPtr(m_h_wnd, GWL_STYLE) & WS_VISIBLE) > 0;
}

// ****************************************************************
// 有効／無効を設定
// ================================================================
void C_window_base::set_enable(bool enable)
{
	if (m_h_wnd == NULL)
		return;	

	if (get_enable() != enable)
		EnableWindow(m_h_wnd, enable);
}

// ****************************************************************
// 有効／無効を取得
// ================================================================
bool C_window_base::get_enable()
{
	if (m_h_wnd == NULL)
		return false;	

	return ::IsWindowEnabled(m_h_wnd) ? true : false;
}

// ****************************************************************
// アイコンを取得
// ================================================================
HICON C_window_base::get_icon()
{
	return (HICON)(PTR)GetClassLongPtr(m_h_wnd, GCLP_HICON);
}

// ****************************************************************
// アイコンを設定
// ================================================================
void C_window_base::set_icon(HICON h_icon)
{
	if (m_h_wnd == NULL)
		return;

	SetClassLongPtr(m_h_wnd, GCLP_HICON, (LONG)(PTR)h_icon);
}
void C_window_base::set_small_icon(HICON h_icon)
{
	if (m_h_wnd == NULL)
		return;

	SetClassLongPtr(m_h_wnd, GCLP_HICONSM, (LONG)(PTR)h_icon);
}

// ****************************************************************
// アイコンを設定
// ================================================================
void C_window_base::set_icon_from_resource(int icon_id)
{
	if (m_h_wnd == NULL)
		return;

	// 大きいアイコン
	if (h_large_icon)	{
		DestroyIcon(h_large_icon);
		h_large_icon = NULL;
	}
	h_large_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 32, 32, 0);
	if (h_large_icon == NULL)	{
		h_large_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 0, 0, 0);
	}
	set_icon(h_large_icon);

	// 小さいアイコン
	if (h_small_icon)	{
		DestroyIcon(h_small_icon);
		h_small_icon = NULL;
	}
	h_small_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 16, 16, 0);
	if (h_small_icon == NULL)	{
		h_small_icon = (HICON)LoadImage(G_app.h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, 0, 0, 0);
	}
	set_small_icon(h_small_icon);
}

// ****************************************************************
// メニューを取得
// ================================================================
HMENU C_window_base::get_menu()
{
	return GetMenu(m_h_wnd);
}

// ****************************************************************
// メニューを設定
// ================================================================
void C_window_base::set_menu(HMENU h_menu)
{
	if (m_h_wnd == NULL)
		return;

	SetMenu(m_h_wnd, h_menu);
}

// ****************************************************************
// テキストを取得
// ================================================================
TSTR C_window_base::get_text()
{
	if (m_h_wnd == NULL)
		return _T("");

	TCHAR buf[1024];
	GetWindowText(m_h_wnd, buf, 1024);
	return buf;
}

// ****************************************************************
// テキストを設定
// ================================================================
void C_window_base::set_text(CTSTR& str)
{
	if (m_h_wnd == NULL)
		return;

	if (get_text() == str)
		return;

	SetWindowText(m_h_wnd, str.c_str());
}

// ****************************************************************
// スタイルを設定
// ================================================================
void C_window_base::set_style(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWL_STYLE, (LONG)style);
}

// ****************************************************************
// スタイルを追加
// ================================================================
void C_window_base::add_style(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	set_style(get_style() | style);
}

// ****************************************************************
// スタイルを取得
// ================================================================
PTR C_window_base::get_style()
{
	return (PTR)GetWindowLongPtr(m_h_wnd, GWL_STYLE);
}

// ****************************************************************
// 拡張スタイルを設定
// ================================================================
void C_window_base::set_style_ex(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWL_EXSTYLE, (LONG)style);
}

// ****************************************************************
// 拡張スタイルを追加
// ================================================================
void C_window_base::add_style_ex(PTR style)
{
	if (m_h_wnd == NULL)
		return;

	set_style_ex(get_style_ex() | style);
}

// ****************************************************************
// 拡張スタイルを取得
// ================================================================
PTR C_window_base::get_style_ex()
{
	return (PTR)GetWindowLongPtr(m_h_wnd, GWL_EXSTYLE);
}

// ****************************************************************
// プロシージャを設定
// ================================================================
void C_window_base::set_proc(WNDPROC proc)
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWLP_WNDPROC, (LONG)(PTR)proc);
}

// ****************************************************************
// プロシージャを取得
// ================================================================
WNDPROC C_window_base::get_proc()
{
	return (WNDPROC)(PTR)GetWindowLongPtr(m_h_wnd, GWLP_WNDPROC);
}

// ****************************************************************
// プロシージャを元に戻す
// ================================================================
void C_window_base::reset_proc()
{
	if (m_h_wnd == NULL)
		return;

	SetWindowLongPtr(m_h_wnd, GWLP_WNDPROC, (LONG)(PTR)C_window_base::window_proc_base);
}

// ****************************************************************
// メッセージの送信
// ================================================================
LRESULT C_window_base::send_message(UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_h_wnd == NULL)
		return FALSE;

	return ::SendMessage(m_h_wnd, msg, wp, lp);
}

LRESULT C_window_base::post_message(UINT msg, WPARAM wp, LPARAM lp)
{
	if (m_h_wnd == NULL)
		return FALSE;

	return ::PostMessage(m_h_wnd, msg, wp, lp);
}

// ****************************************************************
// WM_SIZE を送信
// ================================================================
LRESULT C_window_base::send_wm_size()
{
	if (m_h_wnd == NULL)
		return FALSE;

	C_rect rect = get_client_rect();
	return send_message(WM_SIZE, NULL, MAKELPARAM(rect.width(), rect.height()));
}

// ****************************************************************
// 画面を更新
// ================================================================
void C_window_base::invalidate_rect(C_rect* rect, bool erase)
{
	if (m_h_wnd == NULL)
		return;

	::InvalidateRect(m_h_wnd, rect, erase ? TRUE : FALSE);
}

void C_window_base::invalidate_rect(ARRAY<C_rect>& rect_list, bool erase)
{
	if (m_h_wnd == NULL)
		return;

	// 全矩形に対して画面を更新
	for (ARRAY<C_rect>::iterator rect_itr = rect_list.begin(); rect_itr != rect_list.end(); ++rect_itr)
		::InvalidateRect(m_h_wnd, &*rect_itr, erase ? TRUE : FALSE);
}

// ****************************************************************
// 更新
// ================================================================
void C_window_base::update()
{
	if (m_h_wnd == NULL)
		return;

	::UpdateWindow(m_h_wnd);
}

// ****************************************************************
// スクリーン座標からクライアント座標へ変換
// ================================================================
C_point C_window_base::screen_to_client(int x, int y)
{
	return screen_to_client(C_point(x, y));
}

C_point C_window_base::screen_to_client(C_point point)
{
	ScreenToClient(m_h_wnd, &point);
	return point;
}

C_rect C_window_base::screen_to_client(C_rect rect)
{
	C_point lt = rect.left_top();		ScreenToClient(m_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ScreenToClient(m_h_wnd, &rb);
	return C_rect(lt.x, lt.y, rb.x, rb.y);
}

// ****************************************************************
// クライアント座標からスクリーン座標へ変換
// ================================================================
C_point C_window_base::client_to_screen(int x, int y)
{
	return client_to_screen(C_point(x, y));
}

C_point C_window_base::client_to_screen(C_point point)
{
	ClientToScreen(m_h_wnd, &point);
	return point;
}

C_rect C_window_base::client_to_screen(C_rect rect)
{
	C_point lt = rect.left_top();		ClientToScreen(m_h_wnd, &lt);
	C_point rb = rect.right_bottom();	ClientToScreen(m_h_wnd, &rb);

	return C_rect(lt.x, lt.y, rb.x, rb.y);
}

// ****************************************************************
// クライアント矩形からウィンドウ矩形へ変換
// ================================================================
C_rect C_window_base::client_to_window(C_rect rect)
{
	::AdjustWindowRectEx(&rect, GetWindowLong(m_h_wnd, GWL_STYLE), (GetMenu(m_h_wnd) != NULL) ? TRUE : FALSE, (DWORD)get_style_ex());
	return rect;
}

// ****************************************************************
// ini に保存
// ================================================================

// ini に文字列を保存
void C_window_base::save_ini_str(CTSTR& key, CTSTR& str, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_str(key, str, project_name, get_parent_name(), m_window_name);
}

// ini に bool 値を保存
void C_window_base::save_ini_bool(CTSTR& key, bool value, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_bool(key, value, project_name, get_parent_name(), m_window_name);
}

// ini に int 値を保存
void C_window_base::save_ini_int(CTSTR& key, int value, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_int(key, value, project_name, get_parent_name(), m_window_name);
}

// ini に位置を保存
void C_window_base::save_ini_pos(CTSTR& key, C_point pos, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_point(key, pos, project_name, get_parent_name(), m_window_name);
}

// ini に矩形を保存
void C_window_base::save_ini_rect(CTSTR& key, C_rect rect, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().save_rect(key, rect, project_name, get_parent_name(), m_window_name);
}

// ****************************************************************
// ini から取得
// ================================================================

// ini から文字列を取得
TSTR C_window_base::load_ini_str(CTSTR& key, CTSTR& def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_str(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ini から bool 値を取得
bool C_window_base::load_ini_bool(CTSTR& key, bool def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_bool(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ini から int 値を取得
int C_window_base::load_ini_int(CTSTR& key, int def, CTSTR& project_name)
{
	if (!m_window_name.empty())	{
		return C_ini_file::body().load_int(key, def, project_name, get_parent_name(), m_window_name);
	}

	return def;
}

// ini から位置を読込
C_point C_window_base::load_ini_pos(CTSTR& key, C_point def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_point(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ini から矩形を読込
C_rect C_window_base::load_ini_rect(CTSTR& key, C_rect def, CTSTR& project_name)
{
	if (!m_window_name.empty())
		return C_ini_file::body().load_rect(key, def, project_name, get_parent_name(), m_window_name);

	return def;
}

// ****************************************************************
// ini から削除
// ================================================================
void C_window_base::delete_ini(CTSTR& key, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().delete_value(key, project_name, get_parent_name(), m_window_name);
}

// ****************************************************************
// ini から project 以下を削除（key が _T("") で project_name のみなら、project_name に属するキーを全て削除する）
// ================================================================
void C_window_base::delete_ini_project(CTSTR& key, CTSTR& project_name)
{
	if (!m_window_name.empty())
		C_ini_file::body().delete_value(key, project_name);
}

// ****************************************************************
// ini にウィンドウ矩形を保存／読込
// ================================================================

// ini にウィンドウ位置を保存
void C_window_base::save_ini_window_pos()
{
	if (m_h_wnd == NULL)	{	return;	}

	// 最小化／最大化前のウィンドウ矩形の取得
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_h_wnd, &wp);

	save_ini_pos(_T("window_pos"), C_point(wp.rcNormalPosition.left, wp.rcNormalPosition.top));
}

// ini からウィンドウ位置を読込
void C_window_base::load_ini_window_pos()
{
	// ウィンドウ矩形の取得
	C_point wp = load_ini_pos(_T("window_pos"), C_point(INT_MIN, INT_MIN));
	if (wp != C_point(INT_MIN, INT_MIN))
		set_window_pos(wp);

	// ★マルチモニタの場合、ウィンドウ座標がマイナスってのは普通に起こりえます。
}

// ini にウィンドウ矩形を保存
void C_window_base::save_ini_window_rect()
{
	if (m_h_wnd == NULL)
		return;

	// 最小化／最大化前のウィンドウ矩形の取得
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_h_wnd, &wp);

	save_ini_rect(_T("window_rect"), wp.rcNormalPosition);
}

// ini からウィンドウ矩形を読込
void C_window_base::load_ini_window_rect(C_rect def)
{
	// ウィンドウ矩形の取得
	C_rect wr = load_ini_rect(_T("window_rect"), def);
	if (wr.width() > 0 && wr.height() > 0)
		set_window_rect(wr);

	// ★マルチモニタの場合、ウィンドウ座標がマイナスってのは普通に起こりえます。
}

// ****************************************************************
// ini に表示／非表示を保存／読込
// ================================================================

// ini に表示／非表示を保存
void C_window_base::save_ini_show()
{
	save_ini_bool(_T("window_show"), get_show());
}

// ini から表示／非表示を読込
void C_window_base::load_ini_show(bool def)
{
	set_show(load_ini_bool(_T("window_show"), def));
}

/*
// ****************************************************************
// スクロールバー：情報設定
// ================================================================
void C_window_base::SetScrollInfo(int type, int min, int max, int pageSize)
{
	SCROLLINFO info;
	ZeroMemory(&info, sizeof(SCROLLINFO));
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE;
	info.nMin = min;
	info.nMax = max;
	info.nPage = pageSize;
	::SetScrollInfo(hWnd, type, &info, TRUE);
}

// ****************************************************************
// スクロールバー：位置の設定
// ================================================================
//void	SetScrollPos(bool tate, int pos, bool redraw)

// ****************************************************************
// スクロールバー：自然な動き
//		type: 縦=SB_VERT, 横=SB_HORZ
// ================================================================
int C_window_base::ScrollNatural(int type, WPARAM wParam)
{
	// 情報取得
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_ALL;
	::GetScrollInfo(hWnd, type, &info);

	// 最小値と最大値
	int Min = info.nMin;
	int Max = info.nMax - info.nMin - info.nPage + 1;

	// 動作
	if (false);
	else if (LOWORD(wParam) == SB_ENDSCROLL)	{
		info.nPos = HIWORD(wParam);
	}
	else if (LOWORD(wParam) == SB_LEFT)	{				// 左の端
		info.nPos = info.nMin;
	}
	else if (LOWORD(wParam) == SB_RIGHT)	{			// 右の端
		info.nPos = info.nMax - info.nMin - info.nPage + 1;
	}
	else if (LOWORD(wParam) == SB_LINELEFT)	{			// ＜
		info.nPos = Limit(Min, info.nPos - 1, Max);
	}
	else if (LOWORD(wParam) == SB_LINERIGHT)	{		// ＞
		info.nPos = Limit(Min, info.nPos + 1, Max);
	}
	else if (LOWORD(wParam) == SB_PAGELEFT)	{			// ←[]
		info.nPos = Limit(Min, int(info.nPos - info.nPage), Max);
	}
	else if (LOWORD(wParam) == SB_PAGERIGHT)	{		// []→
		info.nPos = Limit(Min, int(info.nPos + info.nPage), Max);
	}
	else if (LOWORD(wParam) == SB_THUMBPOSITION)	{	// ドラッグ中
		info.nPos = HIWORD(wParam);
	}
	else if (LOWORD(wParam) == SB_THUMBTRACK)	{		// ドラッグ終了
		info.nPos = HIWORD(wParam);
	}

	// 位置のみを更新
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_POS;
	::SetScrollInfo(hWnd, type, &info, TRUE);

	return info.nPos;
}

*/






// ****************************************************************
// デスクトップ上の位置からモニタの情報を取得する
// ================================================================
bool get_monitor_info_from_pos(MONITORINFO* info, C_point pos)
{
	HMONITOR h_monitor = MonitorFromPoint(pos, MONITOR_DEFAULTTONEAREST);		// 指定した点を含むディスプレイモニタが見つからなかった場合は、指定した点に最も近い位置にあるディスプレイモニタのハンドルが返ります。
	if (h_monitor != NULL)	{
		ZeroMemory(info, sizeof(MONITORINFO));
		info->cbSize = sizeof(MONITORINFO);
		if (GetMonitorInfo(h_monitor, info))
			return true;
	}

	return false;
}

// ****************************************************************
// デスクトップ上の位置からデスクトップ矩形を取得する
// ================================================================
bool get_desktop_rect_from_pos(C_rect *desktop_rect, C_point pos)
{
	// 指定座標を含むモニタの情報を取得する
	MONITORINFO mi;
	if (get_monitor_info_from_pos(&mi, pos))	{
		*desktop_rect = mi.rcMonitor;
	}
	else	{
		// 失敗した場合はプライマリーモニターのデスクトップ矩形取得
		desktop_rect->left = 0;
		desktop_rect->top = 0;
		desktop_rect->right = ::GetSystemMetrics(SM_CXFULLSCREEN);
		desktop_rect->bottom = ::GetSystemMetrics(SM_CYFULLSCREEN);
	}

	return true;
}

// ****************************************************************
// デスクトップ上の位置からワークエリア矩形を取得する（デスクトップからタスクバーなどの領域を除外した領域）
// ================================================================
bool get_work_area_rect_from_pos(C_rect *work_area_rect, C_point pos)
{
	// 指定座標を含むモニタの情報を取得する
	MONITORINFO mi;
	if (get_monitor_info_from_pos(&mi, pos))	{
		*work_area_rect = mi.rcWork;
	}
	else	{
		// 失敗した場合はプライマリーモニターのワークエリア矩形取得
		SystemParametersInfo(SPI_GETWORKAREA, 0, work_area_rect, 0);
	}

	return true;
}






}
