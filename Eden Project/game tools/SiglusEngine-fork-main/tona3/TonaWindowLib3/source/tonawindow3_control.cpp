#include	"tonawindow3_pch.h"
#include	"tonawindow3_dialog.h"
#include	"tonawindow3_control.h"

namespace NT3
{

// ****************************************************************
// コントロールテンプレート：パラメータ初期化
// ================================================================
void C_control_template::init_param(int x, int y, int w, int h, PTR class_atom, WORD control_id, DWORD style, DWORD ex_style, PTR text_atom)
{
	stream.clear();

	stream.push<DWORD>(0);
	stream.push<DWORD>(ex_style);
	stream.push<DWORD>(style);
	stream.push<short>(x);
	stream.push<short>(y);
	stream.push<short>(w);
	stream.push<short>(h);
	stream.push<DWORD>(control_id);
	push_atom(class_atom);
	push_atom(text_atom);
	stream.push<WORD>(0);	// creation_data_size
}

// ****************************************************************
// コントロールテンプレート：アトムセット
// ================================================================
void C_control_template::push_atom(PTR atom)
{
	if (atom == -1)	{
		stream.push<WORD>(0);
	}
	else if (atom <= 0xFFFF)	{
		stream.push<WORD>(0xFFFF);
		stream.push<WORD>((WORD)atom);
	}
	else	{
		stream.push_str_null(TSTR_to_WSTR((TCHAR *)atom));
	}
}

// ****************************************************************
// コントロールテンプレート：データの取得
// ================================================================
LPDLGITEMTEMPLATE C_control_template::get()
{
	return (LPDLGITEMTEMPLATE)stream.get();
}

int C_control_template::get_size()
{
	return stream.size();
}

// ****************************************************************
// コントロール：コンストラクタ
// ================================================================
C_control::C_control()
{
	m_adjust_way = 7;		// 左上
}

// ****************************************************************
// コントロール：デストラクタ
// ================================================================
C_control::~C_control()
{
}

// ****************************************************************
// コントロール：作成
//		name : ウィンドウの名前（ini 保存時に使用）
//		caption : ウィンドウタイトル
//		x : ウィンドウ位置Ｘ座標（CW_USEDEFAULT でデフォルト位置）
//		y : ウィンドウ位置Ｙ座標（CW_USEDEFAULT でデフォルト位置）
//		client_w : クライアント領域の大きさＸ
//		client_h : クライアント領域の大きさＹ
//		style : ウィンドウスタイル
//			LVS_ICON : アイコン
//			LVS_LIST : リストビュー
//			LVS_REPORT : レポート
//		ex_style : 拡張ウィンドウスタイル
//		is_limit_min : ウィンドウの最小矩形を制限
//		is_attach : 他のウィンドウにアタッチ
//		h_parent_wnd : 親ウィンドウハンドル
//		adjust_way : 親にくっつく方向（ﾃﾝｷｰ の方向で指定）
// ================================================================
bool C_control::create(CTSTR& name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD style, DWORD ex_style, bool is_limit_min, bool is_attach, HWND h_parent_wnd)
{
	if (m_class_name.empty())
		return false;

	return C_window_base::create(name, m_class_name, caption, x, y, client_w, client_h, is_menu, style, ex_style, is_limit_min, is_attach, h_parent_wnd);
}

// ****************************************************************
// コントロール：既存のコントロールに結びつける
//		name : ウィンドウの名前（ini に保存するときに使用）
//		adjust_way : 親ウィンドウにくっつく向き（1-9: テンキー準拠）
// ================================================================
bool C_control::bind(HWND h_dlg, int ctrl_id, CTSTR& name, int adjust_way, int group_no)
{
	// メンバの初期設定
	m_window_name = name;
	m_adjust_way = adjust_way;
	m_group_no = group_no;

	m_h_wnd = GetDlgItem(h_dlg, ctrl_id);

	// ウィンドウハンドルとウィンドウを結びつける
	C_window_base::bind(m_h_wnd);

	return true;
}

// ****************************************************************
// コントロール：テキストのクリア
// ================================================================
void C_control::clear_text()
{
	send_message(WM_SETTEXT, 0, (LPARAM)_T(""));
}

// ****************************************************************
// コントロール：テキストの設定
// ================================================================
void C_control::set_text(CTSTR& str)
{
	// 改行を 0x0a → 0x0d 0x0a に変換
	TSTR str_a, str_da;
	str_a += 0x0a;
	str_da += 0x0d;
	str_da += 0x0a;
	TSTR new_str = str_replace_ret(str, str_a, str_da);

	C_window_base::set_text(new_str);
}

// ****************************************************************
// コントロール：テキストの設定後、テキストサイズに合わせてコントロールの幅を調整する
// ================================================================
void C_control::set_text_adjust_width(CTSTR& str, int rep_width, HFONT font)
{
	set_text(str);
	adjust_width_from_text(rep_width, font);
}

// ****************************************************************
// コントロール：テキストの取得
// ================================================================
TSTR C_control::get_text()
{
	ARRAY<TCHAR> t_str;

	int len = (int)send_message(WM_GETTEXTLENGTH, 0, 0);
	t_str.resize(len + 16);
	(int)send_message(WM_GETTEXT, len + 16, (LPARAM)t_str.get());

	TSTR str = t_str.get();

	// 改行を 0x0a → 0x0d 0x0a に変換
	TSTR str_a, str_da;
	str_a += 0x0a;
	str_da += 0x0d;
	str_da += 0x0a;
	str_replace(str, str_da, str_a);

	return str;
}

// ****************************************************************
// コントロール：ビットマップの設定
// ================================================================
void C_control::set_bitmap(HBITMAP h_bitmap)
{
	HBITMAP h_old_bitmap = (HBITMAP)send_message(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)h_bitmap);
	if (h_old_bitmap)	{

		// 古いビットマップが返ってくる
		// XP スタイルで 32bit ビットマップを指定した場合、
		// これを解放しないとメモリリークになる！！！！！
		// 嘘のような本当の話。
		::DeleteObject(h_old_bitmap);
	}
}

// ****************************************************************
// コントロール：テキストサイズに合わせてコントロールの幅を取得する
// ================================================================
int C_control::get_adjust_width_from_text(int rep_width, HFONT h_font)
{
	// font が指定されていない場合はデフォルトの GUI フォントを使う
	if (h_font == NULL)	{
		h_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}

	TSTR str = get_text();
	HDC h_dc = GetDC(m_h_wnd);
	HFONT old_font = (HFONT)SelectObject(h_dc, h_font);
	C_size size;
	GetTextExtentPoint32(h_dc, str.c_str(), (int)str.size(), &size);
	SelectObject(h_dc, old_font);	// フォントを元に戻す
	ReleaseDC(m_h_wnd, h_dc);
	return size.cx + rep_width;
}

// ****************************************************************
// コントロール：テキストサイズに合わせてコントロールの幅を調整する
// ================================================================
int C_control::adjust_width_from_text(int rep_width, HFONT font)
{
	int width = get_adjust_width_from_text(rep_width, font);
	C_rect rect = get_window_rect();
	set_window_size(C_size(width, rect.height()));
	set_def_rect_from_now_size();
	return width;
}

// ****************************************************************
// ボタン：スタイルを設定
// ================================================================
void Cc_button::set_button_style(PTR style, bool redraw)
{
	send_message(BM_SETSTYLE, style, redraw ? TRUE : FALSE);
}

// ****************************************************************
// ボタン：押し下げられているか？
// ================================================================
bool Cc_button::is_pushed()
{
	return (send_message(BM_GETSTATE, 0, 0) & BST_PUSHED) ? true : false;
}

// ****************************************************************
// ラジオボタン：ini に保存
// ================================================================
void Cc_radio::save_ini(CTSTR& project_name)
{
	save_ini_bool(_T("check"), get_check(), project_name);
}

// ****************************************************************
// ラジオボタン：ini から読み込み
// ================================================================
void Cc_radio::load_ini(bool def, CTSTR& project_name)
{
	set_check(load_ini_bool(_T("check"), def, project_name));
}

// ****************************************************************
// ラジオボタン：チェックの設定
// ================================================================
void Cc_radio::set_check(bool check)
{
	send_message(BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}

// ****************************************************************
// ラジオボタン：チェックの取得
// ================================================================
bool Cc_radio::get_check()
{
	return send_message(BM_GETCHECK, 0, 0) & BST_CHECKED;
}

// ****************************************************************
// チェックボックス：ini に保存
// ================================================================
void Cc_check::save_ini(CTSTR& project_name)
{
	save_ini_bool(_T("check"), get_check(), project_name);
}

// ****************************************************************
// チェックボックス：ini から読み込み
// ================================================================
void Cc_check::load_ini(bool def, CTSTR& project_name)
{
	set_check(load_ini_bool(_T("check"), def, project_name));
}

// ****************************************************************
// チェックボックス：チェックの設定
// ================================================================
void Cc_check::set_check(bool check)
{
	send_message(BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}

// ****************************************************************
// チェックボックス：チェックの取得
// ================================================================
bool Cc_check::get_check()
{
	return send_message(BM_GETCHECK, 0, 0) & BST_CHECKED;
}

// ****************************************************************
// エディットボックス
// ================================================================
LRESULT Cc_edit::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{

		// Ｄ＆Ｄ乗っ取り
		case WM_DROPFILES:
			set_text(get_droped_link_path(wp));
			break;

		// ダイアログキー乗っ取り
		case WM_GETDLGCODE:
			if (lp != NULL)	{
				MSG* dlg_msg = (MSG *)lp;
				// エンターキーの処理を乗っ取り
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_RETURN)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
				// エスケープキーの処理を乗っ取り
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_ESCAPE)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
				// 上キーの処理を乗っ取り
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_UP)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
				// 下キーの処理を乗っ取り
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_DOWN)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
			}
			break;
	}

	return C_control::window_proc(msg, wp, lp);
}

// ****************************************************************
// エディットボックス：選択
// ================================================================
void Cc_edit::set_sel(int start, int end)
{
	send_message(EM_SETSEL, start, end);
}

void Cc_edit::set_sel_all()
{
	send_message(EM_SETSEL, 0, -1);
}

// ****************************************************************
// エディットボックス：ini に保存
// ================================================================
void Cc_edit::save_ini(CTSTR& project_name)
{
	save_ini_str(_T("text"), get_text(), project_name);
}

// ****************************************************************
// エディットボックス：ini から読み込み
// ================================================================
void Cc_edit::load_ini(CTSTR& def, CTSTR& project_name)
{
	set_text(load_ini_str(_T("text"), def, project_name));
}

// ****************************************************************
// スピン：連動するエディットを設定
// ================================================================
void Cc_spin::set_edit(HWND h_edit)
{
	send_message(UDM_SETBUDDY, (WPARAM)h_edit, 0);
}

// ****************************************************************
// スピン：エディットの値の範囲を設定
// ================================================================
void Cc_spin::set_edit_range(int min, int max)
{
	send_message(UDM_SETRANGE32, min, max);
}

// ****************************************************************
// リストボックス：クリア
// ================================================================
void Cc_listbox::clear()
{
	send_message(LB_RESETCONTENT, 0, 0);
}

// ****************************************************************
// リストボックス：アイテムの追加
//		戻り値：追加されたアイテムのインデックス
// ================================================================
int Cc_listbox::add_item(CTSTR &str)
{
	return (int)send_message(LB_ADDSTRING, 0, (LPARAM)str.c_str());
}

// ****************************************************************
// リストボックス：アイテムの追加（位置を指定）
//		pos：挿入する位置（-1 で末尾）
//		戻り値：追加されたアイテムのインデックス
// ================================================================
int Cc_listbox::insert_item(int pos, CTSTR &str)
{
	return (int)send_message(LB_INSERTSTRING, pos, (LPARAM)str.c_str());
}

// ****************************************************************
// リストボックス：アイテムの削除
//		戻り値：追加されたアイテムのインデックス
// ================================================================
void Cc_listbox::delete_item(int pos)
{
	send_message(LB_DELETESTRING, pos, 0);
}

// ****************************************************************
// リストボックス：アイテム数を取得
//		戻り値：アイテム数
// ================================================================
int Cc_listbox::get_item_cnt()
{
	return (int)send_message(LB_GETCOUNT, 0, 0);
}

// ****************************************************************
// リストボックス：アイテムの取得
// ================================================================
TSTR Cc_listbox::get_item_text(int item_no)
{
	TSTR str;

	int len = (int)send_message(LB_GETTEXTLEN, item_no, 0);
	if (len > 0){
		ARRAY<TCHAR> moji(len + 1);		// 配列に文字列を取得
		send_message(LB_GETTEXT, item_no, (LPARAM)moji.get());
		str = moji.get();
	}

	return str;
}

// ****************************************************************
// リストボックス：アイテムの選択状態を設定
// ================================================================
void Cc_listbox::set_item_select_state(int item_no, bool selected)
{
	send_message(LB_SETSEL, selected ? TRUE : FALSE, item_no);
}

// ****************************************************************
// リストボックス：全てのアイテムの選択状態を設定
// ================================================================
void Cc_listbox::set_item_select_state_all(bool selected)
{
	int item_cnt = get_item_cnt();
	for (int i = 0; i < item_cnt; i++)
		send_message(LB_SETSEL, selected ? TRUE : FALSE, i);
}

// ****************************************************************
// リストボックス：アイテムの選択状態を取得
// ================================================================
bool Cc_listbox::get_item_select_state(int item_no)
{
	return send_message(LB_GETSEL, item_no, 0) ? true : false;
}

// ****************************************************************
// リストボックス：アイテムを選択
// ================================================================
void Cc_listbox::set_selected_item_no(int item_no)
{
	send_message(LB_SETCURSEL, item_no, 0);
}

// ****************************************************************
// リストボックス：選択されているアイテム番号を取得
//		戻り値：選択されているアイテムのインデックス（エラー：LB_ERR）
// ================================================================
int Cc_listbox::get_selected_item_no()
{
	return (int)send_message(LB_GETCURSEL, 0, 0);
}

// ****************************************************************
// リストボックス：選択されているアイテム数を取得
// ================================================================
int Cc_listbox::get_selected_item_cnt()
{
	return (int)send_message(LB_GETSELCOUNT, 0, 0);
}

// ****************************************************************
// リストボックス：水平スクロールバー設定
//		水平スクロールバーの稼動範囲を設定します。
// ================================================================
void Cc_listbox::set_horizontal_scroll_bar()
{
	int item_cnt = get_item_cnt();
	int max_width = 0;

	if (item_cnt > 0)	{
		HDC dc = GetDC(m_h_wnd);
		for (int i = 0; i < item_cnt; i++)	{
			TSTR str = get_item_text(i);

			// 文字列のピクセル幅を取得
			SIZE size;
			GetTextExtentPoint32(dc, str.c_str(), (int)str.size(), &size);
			if (size.cx > max_width)	{	max_width = size.cx;		}
		}
		ReleaseDC(m_h_wnd, dc);
	}

	send_message(LB_SETHORIZONTALEXTENT, max_width, 0);
}

// ****************************************************************
// コンボボックス：現在のアイテム数を取得
// ================================================================
int Cc_combobox::get_item_cnt()
{
	return (int)send_message(CB_GETCOUNT, 0, 0);
}

// ****************************************************************
// コンボボックス：現在の選択番号を設定
// ================================================================
void Cc_combobox::set_sel_no(int sel_no)
{
	send_message(CB_SETCURSEL, sel_no, 0);
}

// ****************************************************************
// コンボボックス：現在の選択番号を取得
//		エラー：CB_ERR
// ================================================================
int Cc_combobox::get_sel_no()
{
	return (int)send_message(CB_GETCURSEL, 0, 0);
}

// ****************************************************************
// コンボボックス：アイテムの文字列の取得
// ================================================================
TSTR Cc_combobox::get_item_text(int item_no)
{
	int len = (int)send_message(CB_GETLBTEXTLEN, item_no, 0);

	// 配列で文字列を受け取る
	ARRAY<TCHAR> str(len + 16);		// 16: 予備
	send_message(CB_GETLBTEXT, item_no, (LPARAM)str.get());

	// 文字列に変換
	return TSTR(str.get());
}

// ****************************************************************
// コンボボックス：アイテムの文字列の設定
// ================================================================
void Cc_combobox::set_item_text(int item_no, CTSTR& text)
{
	insert_item(text, item_no, false);
	delete_item(item_no + 1);
}

// ****************************************************************
// コンボボックス：選択中のアイテムの文字列の取得
// ================================================================
TSTR Cc_combobox::get_sel_item_text()
{
	int sel_no = get_sel_no();
	if (sel_no == CB_ERR)
		return _T("");

	return get_item_text(sel_no);
}

// ****************************************************************
// コンボボックス：アイテムリストをクリア
// ================================================================
void Cc_combobox::clear_item_list()
{
	int item_cnt = get_item_cnt();
	for (int i = 0; i < item_cnt; i++)	{
		delete_item(0);
	}
}

// ****************************************************************
// コンボボックス：アイテムを挿入
// ================================================================
void Cc_combobox::insert_item(CTSTR& text, int index, bool erase_same)
{
	int item_no;

	// 同一の項目を削除
	if (erase_same)	{
		while ((item_no = find_item(text)) >= 0)
			delete_item(item_no);
	}

	// アイテムの追加
	send_message(CB_INSERTSTRING, index, (LPARAM)text.c_str());
}

// ****************************************************************
// コンボボックス：先頭にアイテムを追加
// ================================================================
void Cc_combobox::insert_item_to_top(CTSTR& text, bool erase_same)
{
	int item_no;

	// 同一の項目を削除
	if (erase_same)	{
		while ((item_no = find_item(text)) >= 0)
			delete_item(item_no);
	}

	// アイテムの追加
	send_message(CB_INSERTSTRING, 0, (LPARAM)text.c_str());
}

// ****************************************************************
// コンボボックス：末尾にアイテムを追加
// ================================================================
void Cc_combobox::insert_item_to_bottom(CTSTR& text, bool erase_same)
{
	int item_no;

	// 同一の項目を削除
	if (erase_same)	{
		while ((item_no = find_item(text)) >= 0)
			delete_item(item_no);
	}

	item_no = std::max(get_item_cnt(), 0);
	send_message(CB_INSERTSTRING, item_no, (LPARAM)text.c_str());
}

// ****************************************************************
// コンボボックス：文字列の検索
// ================================================================
int Cc_combobox::find_item(CTSTR& text)
{
	int item_no = (int)send_message(CB_FINDSTRING, -1, (LPARAM)text.c_str());
	if (item_no == CB_ERR)
		return -1;

	return item_no;
}

// ****************************************************************
// コンボボックス：項目の削除
// ================================================================
void Cc_combobox::delete_item(int item_no)
{
	send_message(CB_DELETESTRING, item_no, 0);
}

// ****************************************************************
// コンボボックス：ini に保存
// ================================================================
void Cc_combobox::save_ini_sel_no(CTSTR& project_name)
{
	save_ini_int(_T("sel_no"), get_sel_no(), project_name);
}

void Cc_combobox::save_ini_drop(CTSTR& project_name)
{
	// アイテム数
	int item_cnt = get_item_cnt();
	save_ini_int(_T("item_count"), item_cnt);
	// アイテム
	for (int i = 0; i < item_cnt; i++)
		save_ini_str(_T("item_") + tostr(i), get_item_text(i), project_name);
	// エディット
	save_ini_str(_T("edit"), get_text(), project_name);
}

// ****************************************************************
// コンボボックス：ini から読み込み
// ================================================================
void Cc_combobox::load_ini_sel_no(int def_sel_no, CTSTR& project_name)
{
	set_sel_no(load_ini_int(_T("sel_no"), def_sel_no, project_name));
}

void Cc_combobox::load_ini_drop(CTSTR& project_name)
{
	// アイテム数
	int item_cnt = limit(0, load_ini_int(_T("item_count"), 0), item_cnt_max);

	// アイテム
	for (int i = 0; i < item_cnt; i++)
		insert_item_to_bottom(load_ini_str(tostr(i), _T(""), project_name), false);
	// エディット
	set_text(load_ini_str(_T("edit"), _T(""), project_name));
}

// ****************************************************************
// ツリービュー：ウィンドウプロシージャ
// ================================================================
LRESULT Cc_treeview::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
//	switch (msg)	{
//		default:
//			break;
//	}

	return C_control::window_proc(msg, wp, lp);
}

// ****************************************************************
// ツリービュー：クリア
// ================================================================
void Cc_treeview::clear()
{
	send_message(TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}

// ****************************************************************
// ツリービュー：アイテムの追加
// ================================================================
HTREEITEM Cc_treeview::add_item(HTREEITEM h_parent, HTREEITEM h_pos, CTSTR& text)
{
	TV_INSERTSTRUCT tvi;
	ZeroMemory(&tvi, sizeof(tvi));

	tvi.hParent = h_parent;
	tvi.hInsertAfter = h_pos;
	tvi.item.mask = TVIF_TEXT;
	tvi.item.pszText = const_cast<LPWSTR>(text.c_str());
	return (HTREEITEM)send_message(TVM_INSERTITEM, 0, (LPARAM)&tvi);
}

// ****************************************************************
// スクロールバー：ウィンドウプロシージャ
// ================================================================
LRESULT Cc_scroll::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_VSCROLL:
		case WM_HSCROLL:
			scroll_natural(wp);
			break;
	}

	return C_control::window_proc(msg, wp, lp);
}

// ****************************************************************
// スクロールバー：範囲の設定
// ================================================================
void Cc_scroll::set_scroll_info(int min, int max, int page_size)
{
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE;
	info.nMin = min;
	info.nMax = max;
	info.nPage = page_size;
	send_message(SBM_SETSCROLLINFO, TRUE, LPARAM(&info));
}

// ****************************************************************
// スクロールバー：現在位置の設定
// ================================================================
void Cc_scroll::set_scroll_pos(int pos)
{
	send_message(SBM_SETPOS, WPARAM(pos), LPARAM(TRUE));
}

// ****************************************************************
// スクロールバー：現在位置の取得
// ================================================================
int Cc_scroll::get_scroll_pos()
{
	return int(send_message(SBM_GETPOS, 0, 0));
}

// ****************************************************************
// スクロールバー：自然な動き
// ================================================================
int Cc_scroll::scroll_natural(WPARAM wp)
{
	// 情報取得
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	send_message(SBM_GETSCROLLINFO, 0, LPARAM(&info));

	// 最小値と最大値
	int Min = info.nMin;
	int Max = info.nMax - info.nMin - info.nPage + 1;

	// 動作
	if (false);
	else if (LOWORD(wp) == SB_ENDSCROLL)	{
		return info.nPos;
	}
	else if (LOWORD(wp) == SB_LEFT)	{				// 左の端
		info.nPos = info.nMin;
	}
	else if (LOWORD(wp) == SB_RIGHT)	{			// 右の端
		info.nPos = info.nMax - info.nMin - info.nPage + 1;
	}
	else if (LOWORD(wp) == SB_LINELEFT)	{			// ＜
		info.nPos = limit(Min, info.nPos - 1, Max);
	}
	else if (LOWORD(wp) == SB_LINERIGHT)	{		// ＞
		info.nPos = limit(Min, info.nPos + 1, Max);
	}
	else if (LOWORD(wp) == SB_PAGELEFT)	{			// ←[]
		info.nPos = limit(Min, int(info.nPos - info.nPage), Max);
	}
	else if (LOWORD(wp) == SB_PAGERIGHT)	{		// []→
		info.nPos = limit(Min, int(info.nPos + info.nPage), Max);
	}
	else if (LOWORD(wp) == SB_THUMBPOSITION)	{	// ドラッグ中
		info.nPos = (short)HIWORD(wp);
	}
	else if (LOWORD(wp) == SB_THUMBTRACK)	{		// ドラッグ終了
		info.nPos = (short)HIWORD(wp);
	}

	// 位置を更新
	send_message(SBM_SETPOS, (WPARAM)info.nPos, (LPARAM)TRUE);

	return info.nPos;
}

// ****************************************************************
// スライダー：現在位置の設定
// ================================================================
void Cc_slider::set_slider_pos(int pos)
{
	send_message(TBM_SETPOS, (WPARAM)TRUE, (LPARAM)pos);
}

// ****************************************************************
// スライダー：現在位置の取得
// ================================================================
int Cc_slider::get_slider_pos()
{
	return (int)send_message(TBM_GETPOS, 0, 0);
}

// ****************************************************************
// スライダー：範囲を指定
// ================================================================
void Cc_slider::set_slider_range(int min, int max)
{
	send_message(TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(min, max));
}

// ****************************************************************
// プログレスバー：クリア
// ================================================================
void Cc_progress::clear()
{
	set_prg(0);
}

// ****************************************************************
// プログレスバー：最大値を設定
// ================================================================
void Cc_progress::set_max_prg(int max_prg)
{
	send_message(PBM_SETRANGE32, 0, (LPARAM)max_prg);
}

// ****************************************************************
// プログレスバー：現在の値を設定
// ================================================================
void Cc_progress::set_prg(int prg)
{
	send_message(PBM_SETPOS, (WPARAM)prg, 0);
}

// ****************************************************************
// タブコントロール：タブの追加
// ================================================================
void Cc_tab::add_item(CTSTR& str)
{
	TCHAR t_str[MAX_PATH];
	_tcscpy_s(t_str, MAX_PATH, str.c_str());

	TCITEM tc_item;
	tc_item.mask = TCIF_TEXT;
	tc_item.pszText = t_str;
	tc_item.cchTextMax = MAX_PATH;

	send_message(TCM_INSERTITEM, (WPARAM)tab_cnt++, (LPARAM)&tc_item);
}

// ****************************************************************
// タブコントロール：タブの数を取得
// ================================================================
int Cc_tab::get_item_cnt()
{
	return (int)send_message(TCM_GETITEMCOUNT, 0, 0);
}

// ****************************************************************
// タブコントロール：タブの文字列設定
// ================================================================
void Cc_tab::set_item_text(int item_no, CTSTR& str)
{
	int cnt = get_item_cnt();
	if (!(0 <= item_no && item_no < cnt))	{	return;	}
	if (Cc_tab::get_item_text(item_no) == str)	{	return;	}
	TC_ITEM tc;
	ZeroMemory(&tc, sizeof(tc));
	tc.mask = TCIF_TEXT;
	TSTR tmp = str;
	tc.pszText = (TCHAR *)tmp.c_str();
	send_message(TCM_SETITEM, item_no, (LPARAM)&tc);
}

// ****************************************************************
// タブコントロール：タブの文字列取得
// ================================================================
TSTR Cc_tab::get_item_text(int item_no)
{
	int cnt = get_item_cnt();
	if (!(0 <= item_no && item_no < cnt))	{	return TSTR();	}
	TC_ITEM tc;
	ZeroMemory(&tc, sizeof(tc));
	tc.mask = TCIF_TEXT;
	TCHAR str[1024];
	tc.cchTextMax = 1024;
	tc.pszText = str;
	send_message(TCM_GETITEM, item_no, (LPARAM)&tc);
	return TSTR(str);
}

// ****************************************************************
// タブコントロール：選択されているタブを設定
// ================================================================
void Cc_tab::set_selected_item_no(int item_no)
{
	send_message(TCM_SETCURSEL, item_no, 0);
}

// ****************************************************************
// タブコントロール：選択されているタブを取得
// ================================================================
int Cc_tab::get_selected_item_no()
{
	return (int)send_message(TCM_GETCURSEL, 0, 0);
}

// ****************************************************************
// タブコントロール：表示エリアを取得
// ================================================================
C_rect Cc_tab::get_display_rect()
{
	C_rect rect = get_window_rect();
	send_message(TCM_ADJUSTRECT, FALSE, (LPARAM)&rect);
	return rect;
}

// ****************************************************************
// ピクチャコントロール：ウィンドウプロシージャ
// ================================================================
LRESULT Cc_picture::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	return C_control::window_proc(msg, wp, lp);
}

};
