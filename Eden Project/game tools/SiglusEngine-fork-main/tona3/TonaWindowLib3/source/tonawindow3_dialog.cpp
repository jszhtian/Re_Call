#include	"tonawindow3_pch.h"
#include	"tonawindow3_control.h"
#include	"tonawindow3_dialog.h"

namespace NT3
{

#define		DLGTPLT_OFFSET_ITEM_CNT		(sizeof(WORD) + sizeof(WORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD))
#define		DLGTPLT_OFFSET_WIDTH		(sizeof(WORD) + sizeof(WORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(WORD) + sizeof(short) + sizeof(short))
#define		DLGTPLT_OFFSET_HEIGHT		(DLGTPLT_OFFSET_WIDTH + sizeof(short))

// ****************************************************************
// ダイアログテンプレート：パラメータ初期化
// ================================================================
//		menu_atom: メニューID もしくは 文字列を指定
//		font_name: フォント名。style に DS_SETFONT を指定する必要がある。
// ================================================================
void C_dialog_template::init_param(int x, int y, int w, int h, CTSTR& title, DWORD style, DWORD ex_style, PTR menu_atom, WORD font_size, CTSTR& font_name)
{
	// テンプレート作成用のストリームを作成
	m_stream.clear();
	m_stream.push<WORD>(1);			// ダイアログテンプレートのバージョン（必ず 1 を指定）
	m_stream.push<WORD>(0xFFFF);	// 拡張版のダイアログテンプレートであることを指定（必ず 0xFFFF を指定）
	m_stream.push<DWORD>(0);		// ヘルプ ID を指定（未実装：WM_HELP を参照）
	m_stream.push<DWORD>(ex_style);	// 拡張ウィンドウスタイル
	m_stream.push<DWORD>(style);	// ウィンドウスタイル
	m_stream.push<WORD>(0);			// コントロール数（仮値）
	m_stream.push<short>(x);
	m_stream.push<short>(y);
	m_stream.push<short>(w);
	m_stream.push<short>(h);
	push_atom(menu_atom);
	m_stream.push<WORD>(0);			// ダイアログなのでウィンドウクラスは 0
	m_stream.push_str_null(TSTR_to_WSTR(title));
	if (style & (DS_SETFONT | DS_SHELLFONT))	{
		m_stream.push<WORD>(font_size);
		m_stream.push<WORD>(FW_NORMAL);
		m_stream.push<BYTE>(FALSE);
		m_stream.push<BYTE>(DEFAULT_CHARSET);
		m_stream.push_str_null(TSTR_to_WSTR(font_name));
	}
}

// ****************************************************************
// ダイアログテンプレート：サイズ設定
// ================================================================
void C_dialog_template::set_size(int w, int h)
{
	if (m_stream.empty())
		return;

	*(short *)&m_stream[DLGTPLT_OFFSET_WIDTH]  = w;
	*(short *)&m_stream[DLGTPLT_OFFSET_HEIGHT] = h;
}

// ****************************************************************
// ダイアログテンプレート：アイテム追加
// ================================================================
void C_dialog_template::add_control(int x, int y, int w, int h, PTR class_atom, WORD control_id, DWORD style, DWORD ex_style, PTR text_atom)
{
	if (m_stream.empty())
		return;

	// ストリームのサイズからパディングを挿入
	int padding = m_stream.size() % 4;
	for (int i = 0; i < padding; i++)
		m_stream.push<BYTE>(0);

	// コントロールテンプレートを作成
	C_control_template ctrl_tplt;
	ctrl_tplt.init_param(x, y, w, h, class_atom, control_id, style, ex_style, text_atom);
	// コントロールテンプレートを追加
	m_stream.push(ctrl_tplt.get(), ctrl_tplt.get_size());

	// コントロール数を増やす
	(*(WORD *)&m_stream[DLGTPLT_OFFSET_ITEM_CNT]) ++;
}

// ****************************************************************
// ダイアログテンプレート：アトムセット
// ================================================================
void C_dialog_template::push_atom(PTR atom)
{
	if (atom == -1)	{
		m_stream.push<WORD>(0);
	}
	else if (atom <= 0xFFFF)	{
		m_stream.push<WORD>(0xFFFF);
		m_stream.push<WORD>((WORD)atom);
	}
	else	{
		m_stream.push_str_null(TSTR_to_WSTR((TCHAR *)atom));
	}
}

// ****************************************************************
// ダイアログテンプレート：データの取得
// ================================================================
DLGTEMPLATE* C_dialog_template::get()
{
	return (DLGTEMPLATE *)m_stream.get();
}

// ****************************************************************
// コンストラクタ
// ================================================================
C_dialog::C_dialog()
{
}

// ****************************************************************
// デストラクタ
// ================================================================
C_dialog::~C_dialog()
{
}

// ****************************************************************
// ダイアログ作成
//		dialog_id : ダイアログのリソースＩＤ
//		name : ウィンドウ名（ini 保存時に使用）
//		is_limit_min : ウィンドウの最小矩形を制限
//		is_attach : 他のウィンドウにアタッチ
//		h_parent_wnd : 親ウィンドウハンドル
// ================================================================
bool C_dialog::create(PTR dialog_id, CTSTR& name, bool is_limit_min, bool is_attach, HWND h_parent_wnd, int adjust_way)
{
	if (m_h_wnd != NULL)
		return true;

	// ダイアログの名前
	m_window_name = name;
	// ウィンドウの最小矩形を制限
	m_is_limit_window_rect_min = is_limit_min;
	// 他のウィンドウにアタッチ
	m_is_attach_window = is_attach;
	// 親へのくっつき方
	m_adjust_way = adjust_way;

	// ダイアログの構築
	m_h_wnd = ::CreateDialogParam(G_app.h_inst, (LPCTSTR)dialog_id, h_parent_wnd, (DLGPROC)window_proc_base, (LPARAM)this);
	// 構築に失敗
	if (m_h_wnd == NULL)	{
		set_last_error_win(_T("CreateDialogParam"));
		return false;
	}

	// 他のウィンドウにアタッチ
	if (m_is_attach_window)
		m_attach.h_wnd_list.push_back(m_h_wnd);

	// IME を無効にする
//	ImmAssociateContext(h_wnd, NULL);

	return true;
}

// ****************************************************************
// ダイアログ作成
//		dialog_id : ダイアログのリソースＩＤ
//		name : ウィンドウ名（ini 保存時に使用）
//		is_limit_min : ウィンドウの最小矩形を制限
//		is_attach : 他のウィンドウにアタッチ
//		h_parent_wnd : 親ウィンドウハンドル
// ================================================================
bool C_dialog::create_indirect(C_dialog_template* dlg_tpl, CTSTR& name, bool is_limit_min, bool is_attach, HWND h_parent_wnd, int adjust_way)
{
	if (m_h_wnd != NULL)
		return true;

	// ダイアログの名前
	m_window_name = name;
	// ウィンドウの最小矩形を制限
	m_is_limit_window_rect_min = is_limit_min;
	// 他のウィンドウにアタッチ
	m_is_attach_window = is_attach;
	// 親へのくっつき方
	m_adjust_way = adjust_way;

	// ダイアログの構築
	m_h_wnd = ::CreateDialogIndirectParam(G_app.h_inst, dlg_tpl->get(), h_parent_wnd, (DLGPROC)window_proc_base, (LPARAM)this);

	// 構築に失敗
	if (m_h_wnd == NULL)	{
		set_last_error_win(_T("CreateDialogParam"));
		return false;
	}

	// 他のウィンドウにアタッチ
	if (m_is_attach_window)
		m_attach.h_wnd_list.push_back(m_h_wnd);

	// IME を無効にする
//	ImmAssociateContext(h_wnd, NULL);

	return true;
}

// ****************************************************************
// コントロールを結びつける
// ================================================================
void C_dialog::bind_control(C_control* ctrl, int ctrl_id, CTSTR& name, int adjust_way, int group_no)
{
	ctrl->bind(m_h_wnd, ctrl_id, name, adjust_way, group_no);
	m_child_list.push_back(ctrl);
}

// ****************************************************************
// チャイルドリストに追加
// ================================================================
void C_dialog::add_child(C_window_base* p_wnd, int adjust_way, int group_no)
{
	p_wnd->set_adjust_way(adjust_way);
	p_wnd->set_group_no(group_no);
	m_child_list.push_back(p_wnd);
}

// ****************************************************************
// ウィンドウプロシージャ（プレ）
// ================================================================
void C_dialog::window_proc_pre(UINT msg, WPARAM wp, LPARAM lp)
{
	C_window_base::window_proc_pre(msg, wp, lp);

	switch (msg)	{
		case WM_INITDIALOG:		on_init_dialog_pre();		break;
	}
}

// ****************************************************************
// ダイアログが作られた（プレ）
// ================================================================
void C_dialog::on_init_dialog_pre()
{
	// ウィンドウ矩形を取得
	m_def_window_rect = get_window_rect();
	// クライアント矩形を取得
	m_def_client_rect = get_client_rect();
	// ウィンドウを登録する
	G_app.window_list.push_back(m_h_wnd);
}

// ****************************************************************
// ウィンドウプロシージャ
// ================================================================
LRESULT	C_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_DESTROY:		if (!on_destroy())				return FALSE;	break;
		case WM_SIZE:			if (!on_size(wp, lp))			return FALSE;	break;
		case WM_MOUSEMOVE:		if (!on_mousemove(wp, lp))		return FALSE;	break;
		case WM_SETCURSOR:		if (!on_setcursor(wp, lp))		return TRUE;	break;
		case WM_LBUTTONDOWN:	if (!on_lbuttondown(wp, lp))	return FALSE;	break;
	}

	return C_window_base::window_proc(msg, wp, lp);
}

// ****************************************************************
// ダイアログが破壊された
// ================================================================
bool C_dialog::on_destroy()
{
	// 子供リストのクリア
	m_child_list.clear();

	return true;
}

// ****************************************************************
// ダイアログのサイズが変更された
// ================================================================
bool C_dialog::on_size(WPARAM wp, LPARAM lp)
{
	int dw = LOWORD(lp) - m_def_client_rect.width();
	int dh = HIWORD(lp) - m_def_client_rect.height();

	// 子供の位置を調整する
	adjust_child(dw, dh);

	// ステータスバーのサイズを調整する
	statusbar.on_size(m_h_wnd, wp, lp);

	// セパレーターのサイズを調整する
	if (separater_manager.on_size(m_h_wnd, wp, lp))
		return false;

	return true;
}

// ****************************************************************
// マウスカーソルが移動した
// ================================================================
bool C_dialog::on_mousemove(WPARAM wp, LPARAM lp)
{
	if (separater_manager.on_mousemove(m_h_wnd, wp, lp))
		return false;	// セパレータ

	return true;
}

// ****************************************************************
// マウスカーソルの変更通知
// ================================================================
bool C_dialog::on_setcursor(WPARAM wp, LPARAM lp)
{
	if (separater_manager.on_setcursor(m_h_wnd, wp, lp))
		return false;	// セパレータ

	return true;
}

// ****************************************************************
// マウスの左ボタン押し下げ
// ================================================================
bool C_dialog::on_lbuttondown(WPARAM wp, LPARAM lp)
{
	if (separater_manager.on_lbuttondown(m_h_wnd, wp, lp))
		return false;	// セパレータ

	return true;
}

// ****************************************************************
// サイズにあわせて子供を動かす
// ================================================================
void C_dialog::adjust_child(int dw, int dh)
{
	// 全ての子供を動かす
	CHILDITR itr;
	for (itr = m_child_list.begin(); itr != m_child_list.end(); ++itr)
		(*itr)->adjust_to_parent(dw, dh);
}

// ****************************************************************
// プロシージャの設定
//		戻り値：古いプロシージャ
// ================================================================
DLGPROC C_dialog::set_proc(DLGPROC proc)
{
	return (DLGPROC)(PTR)SetWindowLongPtr(m_h_wnd, DWLP_DLGPROC, (LONG)(PTR)proc);
}

// ****************************************************************
// プロシージャの取得
// ================================================================
DLGPROC C_dialog::get_proc()
{
	return (DLGPROC)(PTR)GetWindowLongPtr(m_h_wnd, DWLP_DLGPROC);
}

// ****************************************************************
// プロシージャを元に戻す
// ================================================================
void C_dialog::reset_proc()
{
	SetWindowLongPtr(m_h_wnd, DWLP_DLGPROC, (LONG)(PTR)C_window_base::window_proc_base);
}

// ****************************************************************
// セパレーターの割り当て
// ================================================================
void C_dialog::bind_separater(C_dialog_separater* separater, CTSTR& name)
{
	separater_manager.bind_separater(separater, m_window_name, name);
}

// ****************************************************************
// グループに属するコントロールの有効／無効を設定
// ================================================================
void C_dialog::group_set_enable(int group_no, bool enable)
{
	for (CHILDITR itr = m_child_list.begin(); itr != m_child_list.end(); ++itr)	{
		if ((*itr)->get_group_no() == group_no)	{
			(*itr)->set_enable(enable);
		}
	}
}

// ****************************************************************
// モーダルダイアログ：作成
// ================================================================
int C_modal_dialog::create(PTR dialog_id, CTSTR& name, bool limit_min, HWND parent)
{
	if (m_h_wnd != NULL)
		return false;

	// ダイアログの名前の設定
	m_window_name = name;
	// ウィンドウの最小矩形の制限フラグ
	m_is_limit_window_rect_min = limit_min;

	// ダイアログの構築
	return (int)::DialogBoxParam(G_app.h_inst, (LPCTSTR)dialog_id, parent, (DLGPROC)window_proc_base, (LPARAM)this);
}

// ****************************************************************
// モーダルダイアログ：作成
// ================================================================
int C_modal_dialog::create_indirect(C_dialog_template* dlg_tpl, CTSTR& name, bool is_limit_min, HWND h_parent_wnd)
{
	if (m_h_wnd != NULL)
		return false;

	// ダイアログの名前
	m_window_name = name;
	// ウィンドウの最小矩形を制限
	m_is_limit_window_rect_min = is_limit_min;

	// ダイアログの構築
	return (int)::DialogBoxIndirectParam(G_app.h_inst, dlg_tpl->get(), h_parent_wnd, (DLGPROC)window_proc_base, (LPARAM)this);
}

// ****************************************************************
// モーダルダイアログ：終了
// ================================================================
void C_modal_dialog::destroy(int return_value)
{
	EndDialog(m_h_wnd, (PTR)return_value);
}

// ****************************************************************
// モーダルダイアログ：ウィンドウプロシージャ
// ================================================================
LRESULT	C_modal_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_CLOSE:		if (!on_close())		return FALSE;	break;
	}

	return C_dialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// モーダルダイアログ：WM_CLOSE
// ================================================================
bool C_modal_dialog::on_close()
{
	destroy(0);

	// デフォルトの動作を実行しない！
	return false;
}

// ****************************************************************
// プロパティシートページ
// ================================================================
bool C_property_sheet_page::create(PTR dialog_id, CTSTR& name, CTSTR& title, DWORD flag)
{
	PROPSHEETPAGE psp;
	ZeroMemory(&psp, sizeof(psp));
	psp.dwSize = sizeof(psp);
	psp.dwFlags = flag;
	psp.hInstance = G_app.h_inst;
	psp.pszTemplate = (LPCTSTR)dialog_id;
	psp.pfnDlgProc = (DLGPROC)window_proc_base;
	psp.pszTitle = title.c_str();
	psp.lParam = (LPARAM)this;
	m_h_wnd = (HWND)CreatePropertySheetPage(&psp);

	// 構築に失敗
	if (m_h_wnd == NULL)
		return false;

	// ウィンドウハンドルとオブジェクトを結びつける
	bind(m_h_wnd);

	return true;
}

// ****************************************************************
// 簡易エディットダイアログ
// ================================================================
bool Ceasy_edit_dialog::create(CTSTR& window_title)
{
	C_dialog_template dlgtmpl;
	dlgtmpl.init_param(-1, -1, 200, 100, window_title, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0);
	dlgtmpl.add_control(10, 10, 150, 50, (PTR)WC_EDIT, EDIT, WS_CHILD | WS_VISIBLE, 0);
	dlgtmpl.add_control(10, 60, 70, 10, (PTR)WC_BUTTON, BTN_OK, WS_CHILD | WS_VISIBLE, 0);
	dlgtmpl.add_control(90, 60, 70, 10, (PTR)WC_BUTTON, BTN_CANCEL, WS_CHILD | WS_VISIBLE, 0);

	Ceasy_edit_dialog_sub dlg;
	if (!dlg.create_indirect(&dlgtmpl, _T(""), false))
		return false;

	result = dlg.get_result();
	return true;
}

LRESULT Ceasy_edit_dialog_sub::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init_dialog())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_modal_dialog::window_proc(msg, wp, lp);
}

bool Ceasy_edit_dialog_sub::on_init_dialog()
{
	bind_control(&edit, EDIT);
	bind_control(&btn_ok, BTN_OK);
	bind_control(&btn_cancel, BTN_CANCEL);

	btn_ok.set_text(_T("決定"));
	btn_cancel.set_text(_T("キャンセル"));

	return true;
}

bool Ceasy_edit_dialog_sub::on_close()
{
	EndDialog(m_h_wnd, FALSE);		// 失敗
	return true;
}

bool Ceasy_edit_dialog_sub::on_command(WPARAM wp, LPARAM lp)
{
	if (false);
	else if (LOWORD(wp) == BTN_OK)	{
		result = edit.get_text();
		EndDialog(m_h_wnd, TRUE);	// 成功
	}
	else if (LOWORD(wp) == BTN_CANCEL)	{
		EndDialog(m_h_wnd, FALSE);	// 失敗
	}

	return true;
}

}
