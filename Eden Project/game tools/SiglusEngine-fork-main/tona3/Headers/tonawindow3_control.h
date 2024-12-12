#pragma		once

#include	"tonawindow3_wbase.h"

namespace NT3
{

// ****************************************************************
// コントロールテンプレート
// ================================================================
class C_control_template
{
public:
	void				init_param(int x, int y, int w, int h, PTR class_atom, WORD control_id, DWORD style, DWORD ex_style, PTR text_atom = -1);
	LPDLGITEMTEMPLATE	get();
	int					get_size();

private:
	void	push_atom(PTR atom);

	C_stream	stream;
};

// ****************************************************************
// コントロール
// ================================================================
class C_control : public C_window_base
{
public:
	C_control();
	~C_control();

	bool	create(CTSTR& name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD window_style, DWORD window_style_ex, bool is_limit_min, bool is_attach, HWND h_parent_wnd = NULL);
	bool	bind(HWND h_dlg, int ctrl_id, CTSTR& name, int adjust_way, int group_no);			// コントロールを結びつける

	void	clear_text();					// テキストのクリア
	void	set_text(CTSTR& str);			// テキストの設定
	void	set_text_adjust_width(CTSTR& str, int rep_width = 0, HFONT font = NULL);	// テキストの設定後、テキストサイズに合わせてコントロールの幅を調整する
	TSTR	get_text();						// テキストの取得
	void	set_bitmap(HBITMAP h_bitmap);	// ビットマップの設定

	int		get_adjust_width_from_text(int rep_width = 0, HFONT font = NULL);	// テキストサイズに合わせてコントロールの幅を取得する
	int		adjust_width_from_text(int rep_width = 0, HFONT font = NULL);		// テキストサイズに合わせてコントロールの幅を調整する

protected:
	TSTR	m_class_name;			// コントロール固有のクラス名（継承先で設定する）
};

// ****************************************************************
// スタティック
// ================================================================
class Cc_static : public C_control
{
public:
	Cc_static()	{	m_class_name = WC_STATIC;	}

};

// ****************************************************************
// グループ
// ================================================================
class Cc_group : public C_control
{
public:
	Cc_group()	{	m_class_name = WC_BUTTON;	}

};

// ****************************************************************
// ボタン
// ================================================================
class Cc_button : public C_control
{
public:
	Cc_button()	{	m_class_name = WC_BUTTON;	}

	void	set_button_style(PTR style, bool redraw = true);
	bool	is_pushed();
};

// ****************************************************************
// ラジオボタン
// ================================================================
class Cc_radio : public C_control
{
public:
	Cc_radio()	{	m_class_name = WC_BUTTON;	}

	void	set_check(bool check);
	bool	get_check();

	void	save_ini(CTSTR& project_name = _T(""));
	void	load_ini(bool def, CTSTR& project_name = _T(""));
};

// ****************************************************************
// チェックボックス
// ================================================================
class Cc_check : public C_control
{
public:
	Cc_check()	{	m_class_name = WC_BUTTON;	}

	void	set_check(bool check);
	bool	get_check();

	void	save_ini(CTSTR& project_name = _T(""));
	void	load_ini(bool def, CTSTR& project_name = _T(""));
};

// ****************************************************************
// エディットボックス
// ================================================================
class Cc_edit : public C_control
{
public:
	Cc_edit()	{	m_class_name = WC_EDIT;	}

	void	set_sel(int start, int end);		// 文字列を選択する（end = -1 で最後まで選択）
	void	set_sel_all();						// 文字列を全て選択する

	void	save_ini(CTSTR& project_name = _T(""));
	void	load_ini(CTSTR& def, CTSTR& project_name = _T(""));

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// スピン
// ================================================================
class Cc_spin : public C_control
{
public:
	Cc_spin()	{	m_class_name = _T("msctls_updown32");	}

	void	set_edit(HWND h_edit);				// 連動するエディットを設定
	void	set_edit_range(int min, int max);	// エディットの値の範囲を設定
};

// ****************************************************************
// リストボックス
// ================================================================
class Cc_listbox : public C_control
{
public:
	Cc_listbox()	{ m_class_name = WC_LISTBOX; }

	void	clear();
	int		add_item(CTSTR& str);				// アイテムを適当な位置に追加
	int		insert_item(int pos, CTSTR& str);	// アイテムを位置を指定して追加（-1 で末尾）（LBS_SORT が指定されていないこと）
	void	delete_item(int pos);				// アイテムを削除

	// アイテム関連
	int		get_item_cnt();						// アイテム数を取得
	TSTR	get_item_text(int item_no);			// アイテムを取得

	// アイテムの選択関連
	void	set_item_select_state(int item_no, bool selected);	// アイテムの選択状態を設定（複数選択タイプで使用）
	void	set_item_select_state_all(bool selected);			// アイテムの選択状態を設定（複数選択タイプで使用）
	bool	get_item_select_state(int item_no);					// アイテムの選択状態を取得（複数選択タイプで使用）
	int		get_selected_item_cnt();							// 選択されているアイテム数を取得（複数選択タイプで使用）
	void	set_selected_item_no(int item_no);					// アイテムを選択（単一選択タイプで使用）
	int		get_selected_item_no();								// 選択されているアイテム番号を取得（単一選択タイプで使用）（エラー：LB_ERR）

	void	set_horizontal_scroll_bar();		// 水平スクロールバー設定
};

// ****************************************************************
// コンボボックス
// ================================================================
class Cc_combobox :  public C_control
{
public:
	Cc_combobox()	{	m_class_name = WC_COMBOBOX;	item_cnt_max = 10;	}

	int		get_item_cnt();				// アイテム数を取得
	void	set_sel_no(int sel_no);		// 選択中のアイテム番号を設定
	int		get_sel_no();				// 選択中のアイテム番号を取得

	TSTR	get_item_text(int item_no);								// アイテムの文字列を取得
	void	set_item_text(int item_no, CTSTR& text);				// アイテムの文字列を設定
	TSTR	get_sel_item_text();									// 選択中のアイテムの文字列を取得

	void	clear_item_list();										// アイテムリストをクリア
	void	insert_item(CTSTR& text, int index, bool erase_same);	// アイテムを挿入
	void	insert_item_to_top(CTSTR& text, bool erase_same);		// 先頭にアイテムを追加
	void	insert_item_to_bottom(CTSTR& text, bool erase_same);	// 末尾にアイテムを追加
	int		find_item(CTSTR& text);									// アイテムを検索
	void	delete_item(int item_no);								// アイテムを削除

	void	save_ini_sel_no(CTSTR& project_name = _T(""));					// ini セーブ：選択番号
	void	load_ini_sel_no(int def_sel_no, CTSTR& project_name = _T(""));	// ini ロード：選択番号
	void	save_ini_drop(CTSTR& project_name = _T(""));					// ini セーブ：ドロップ
	void	load_ini_drop(CTSTR& project_name = _T(""));					// ini ロード：ドロップ

private:
	int		item_cnt_max;
};

// ****************************************************************
// ツリービュー
// ================================================================
class Cc_treeview :  public C_control
{
public:
	Cc_treeview()	{	m_class_name = WC_TREEVIEW;	}

	void		clear();
	HTREEITEM	add_item(HTREEITEM h_parent, HTREEITEM h_pos, CTSTR& text);		// TVI_ROOT / TVI_FIRST / TVI_LAST / TVI_SORT

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// スクロールバー
// ================================================================
class Cc_scroll : public C_control
{
public:
	Cc_scroll()		{	m_class_name = WC_SCROLLBAR;	}

	void	set_scroll_info(int min, int max, int page_size);	// スクロール情報を設定する
	void	set_scroll_pos(int pos);			// 現在の値を設定する
	int		get_scroll_pos();					// 現在の値を取得する
	int		scroll_natural(WPARAM wp);			// WM_HSCROLL や WM_VSCROLL に対して自然な動きを行う

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// スライダー
// ================================================================
class Cc_slider : public C_control
{
public:
	Cc_slider()		{	m_class_name = TRACKBAR_CLASS;	}

	void	set_slider_pos(int pos);				// 現在の値を設定する
	int		get_slider_pos();						// 現在の値を取得する
	void	set_slider_range(int min, int max);		// スライドの最小値と最大値を設定する
};

// ****************************************************************
// プログレスバー
// ================================================================
class Cc_progress : public C_control
{
public:

	void	clear();					// 状態をクリア
	void	set_max_prg(int max_prg);	// 最大値を設定する
	void	set_prg(int prg);			// 現在の値を設定する
};

// ****************************************************************
// タブ
// ================================================================
class Cc_tab : public C_control
{
public:
	Cc_tab()	{	m_class_name = WC_TABCONTROL; tab_cnt = 0;	}

	void	add_item(CTSTR& str);					// タブの追加
	int		get_item_cnt();							// タブの数を取得
	void	set_item_text(int item_no, CTSTR& str);	// タブの文字列設定
	TSTR	get_item_text(int item_no);				// タブの文字列取得
	void	set_selected_item_no(int item_no);		// 選択されているタブを設定
	int		get_selected_item_no();					// 選択されているタブを取得
	C_rect	get_display_rect();						// 表示エリアを取得

private:
	int		tab_cnt;
};

// ****************************************************************
// ピクチャ
// ================================================================
class Cc_picture : public C_control
{
public:
	Cc_picture()	{	m_class_name = WC_STATIC;	}

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};


}
