#pragma		once

namespace NT3
{

// ****************************************************************
// ウィンドウアタッチ構造体
// ================================================================
const int 		WINDOW_ATTACH_REP_SIZE = 16;

struct WINDOW_ATTACH
{
	int			mod;						// 1=移動時 2=サイズ変更時
	HWND		h_wnd;						// アタッチ元のウィンドウハンドル
	int			x, y;						// アタッチ開始時のマウス座標
	int			w, h;						// アタッチ元のウィンドウサイズ
	bool		left, right, top, bottom;	// それぞれの向きにくっつくフラグ
	ARRAY<HWND>	h_wnd_list;					// アタッチを行うウィンドウリスト
};

struct WINDOW_ATTACH_PROC
{
	HWND width_h_wnd, height_h_wnd;
	int width_mod, height_mod;
	bool desktop_width_flag, desktop_height_flag;
};

// ****************************************************************
// 基底のウィンドウ
// ================================================================
class C_window_base
{
public:
	C_window_base();
	~C_window_base();

	static	bool	regist_class(CTSTR& class_name, int icon_id, int menu_id, int brush_type);
	static	void	unregist_class(CTSTR& class_name);

	// ウィンドウの構築と破壊
	bool	create(CTSTR& name, CTSTR& class_name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD style, DWORD ex_style, bool limit_min, bool attach, HWND h_parent_wnd);
	void	close();			// ウィンドウを閉じる
	void	destroy();			// ウィンドウを破壊する
	bool	bind(HWND h_wnd);	// ウィンドウハンドルとインスタンスを結びつける
	void	unbind();			// ウィンドウハンドルとインスタンスを結びつけを解除する
	bool	is_open()	{	return m_h_wnd != NULL;	}

	// ini に保存
	void	save_ini_str(CTSTR& key, CTSTR& str, CTSTR& project_name = _T(""));		// ini に文字列を保存
	void	save_ini_bool(CTSTR& key, bool value, CTSTR& project_name = _T(""));	// ini に bool 値を保存
	void	save_ini_int(CTSTR& key, int value, CTSTR& project_name = _T(""));		// ini に整数を保存
	void	save_ini_pos(CTSTR& key, C_point pos, CTSTR& project_name = _T(""));		// ini に位置を保存
	void	save_ini_rect(CTSTR& key, C_rect rect, CTSTR& project_name = _T(""));	// ini に矩形を保存

	// ini から取得
	TSTR	load_ini_str(CTSTR& key, CTSTR& def, CTSTR& project_name = _T(""));		// ini から文字列を読込
	bool	load_ini_bool(CTSTR& key, bool def, CTSTR& project_name = _T(""));		// ini から bool 値を読込
	int		load_ini_int(CTSTR& key, int def, CTSTR& project_name = _T(""));		// ini から整数を読込
	C_point	load_ini_pos(CTSTR& key, C_point def, CTSTR& project_name = _T(""));		// ini から位置を読込
	C_rect	load_ini_rect(CTSTR& key, C_rect def, CTSTR& project_name = _T(""));		// ini から矩形を読込

	// ini から削除（key が _T("") で project_name のみなら、project_name に属するキーを全て削除する）
	void	delete_ini(CTSTR& key, CTSTR& project_name);							// ini から削除

	// ini から project 以下を削除（key が _T("") で project_name のみなら、project_name に属するキーを全て削除する）
	void	delete_ini_project(CTSTR& key, CTSTR& project_name);					// ini から project 以下を削除

	// ini 関連
	void	save_ini_window_pos();				// ini にウィンドウ位置を保存
	void	load_ini_window_pos();				// ini からウィンドウ位置を読込
	void	save_ini_window_rect();				// ini にウィンドウ矩形を保存
	void	load_ini_window_rect(C_rect def = C_rect(0, 0, 0, 0));	// ini からウィンドウ矩形を読込
	void	save_ini_show();					// ini に表示／非表示を保存
	void	load_ini_show(bool def);			// ini から表示／非表示を読込

	void	set_main_window();				// メインウィンドウに設定（破壊されたときに WM_QUIT を飛ばすようになります）
	HWND	get_handle();					// ウィンドウハンドルを取得
	WORD	get_id();						// ウィンドウ id を取得
	TSTR	get_class();					// ウィンドウクラスを取得
	TSTR	get_name();						// 名前を取得
	void	set_name(CTSTR& name);			// 名前を設定
	void	set_adjust_way(int adjust_way);	// 親へのくっつきかたを設定
	int		get_group_no();					// グループ番号を取得
	void	set_group_no(int group_no);		// グループ番号を設定

	C_window_base*	get_parent();				// 親ウィンドウを取得
	HWND			get_parent_handle();		// 親のウィンドウハンドルを取得
	TSTR			get_parent_name();			// 親ウィンドウの名前を取得

	void	set_window_pos(C_point pos);		// ウィンドウの位置を設定（スクリーン座標系で指定）
	void	set_client_pos(C_point pos);		// クライアント領域の位置を設定（スクリーン座標系で指定）
	void	set_window_size(C_size size);		// ウィンドウの大きさを設定
	void	set_client_size(C_size size);		// クライアント領域の大きさを設定
	void	set_window_rect(C_rect rect);		// ウィンドウの位置と大きさを設定（スクリーン座標系で指定）
	void	set_client_rect(C_rect rect);		// クライアント領域の位置と大きさを設定（スクリーン座標系で指定）
	C_rect	get_window_rect();					// ウィンドウの位置と大きさを取得（スクリーン座標系で取得）
	C_rect	get_client_rect();					// クライアント領域の位置と大きさを取得（クライアント座標系で取得）
	C_rect	get_rltv_rect();					// ウィンドウの位置と大きさを取得（親のクライアント座標系で取得）

	void	set_def_window_rect(C_rect rect);	// デフォルトのウィンドウの位置と大きさを設定（スクリーン座標系で設定）
	C_rect	get_def_window_rect();				// デフォルトのウィンドウの位置と大きさを取得（スクリーン座標系で取得）
	void	set_def_client_width(int width);	// デフォルトのクライアント領域の幅を設定（クライアント座標系で設定）
	void	set_def_client_height(int height);	// デフォルトのクライアント領域の高さを設定（クライアント座標系で設定）
	void	set_def_client_rect(C_rect rect);	// デフォルトのクライアント領域の位置と大きさを設定（クライアント座標系で設定）
	C_rect	get_def_client_rect();				// デフォルトのクライアント領域の位置と大きさを取得（クライアント座標系で取得）
	void	set_def_rltv_rect(C_rect rect);		// デフォルトのウィンドウの位置と大きさを設定（親のクライアント座標系で設定）
	C_rect	get_def_rltv_rect();				// デフォルトのウィンドウの位置と大きさを取得（親のクライアント座標系で取得）

	void	set_def_rect_from_now_rect();			// 現在の矩形に合わせてデフォルトの矩形を再設定する
	void	set_def_window_rect_from_now_rect();	// 現在の矩形に合わせてデフォルトのウィンドウ矩形を再設定する
	void	set_def_client_rect_from_now_rect();	// 現在の矩形に合わせてデフォルトのクライアント矩形を再設定する
	void	set_def_rltv_rect_from_now_rect();		// 現在の矩形に合わせてデフォルトの相対矩形を再設定する

	void	set_def_rect_from_now_size();			// 現在のサイズに合わせてデフォルトの矩形を再設定する
	void	set_def_window_rect_from_now_size();	// 現在のサイズに合わせてデフォルトのウィンドウ矩形を再設定する
	void	set_def_client_rect_from_now_size();	// 現在のサイズに合わせてデフォルトのクライアント矩形を再設定する
	void	set_def_rltv_rect_from_now_size();		// 現在のサイズに合わせてデフォルトの相対矩形を再設定する

	void	set_window_size_with_def_size(int width, int height);			// ウィンドウサイズを設定してデフォルトの矩形を再設定する
	void	set_window_size_with_def_size(C_size size);						// ウィンドウサイズを設定してデフォルトの矩形を再設定する
	void	add_window_size_with_def_size(int rep_width, int rep_height);	// ウィンドウサイズを設定してデフォルトの矩形を再設定する
	void	add_window_size_with_def_size(C_size rep_size);					// ウィンドウサイズを設定してデフォルトの矩形を再設定する

	void	set_rltv_pos_with_def_rect(int x, int y);				// 相対座標を設定してデフォルトの矩形を再設定する
	void	set_rltv_pos_with_def_rect(C_point pos);				// 相対座標を設定してデフォルトの矩形を再設定する
	void	add_rltv_pos_with_def_rect(int rep_x, int rep_y);		// 相対座標を調整してデフォルトの矩形を再設定する
	void	add_rltv_pos_with_def_rect(C_point rep_pos);			// 相対座標を調整してデフォルトの矩形を再設定する

	bool	get_my_monitor_info(MONITORINFO* info);					// ウィンドウの存在するモニタのハンドルを取得
	void	get_my_desktop_rect(C_rect *desktop_rect);				// ウィンドウの存在するモニタのデスクトップ矩形取得
	void	get_my_desktop_work_area_rect(C_rect *work_area_rect);	// ウィンドウの存在するモニタのワークエリア矩形取得（デスクトップからタスクバーなどの領域を除外した領域を取得）
	void	set_pos_my_desktop_center();							// ウィンドウの存在するモニタの中央に移動

	bool	rep_window_pos_outside_desktop(int rep_mod = 0, int outside_size = 34);		// デスクトップ範囲外にある場合、ウィンドウ座標をデスクトップの中心に補正する

	C_point	screen_to_client(int x, int y);		// スクリーン座標系 → クライアント座標系
	C_point	screen_to_client(C_point point);	// スクリーン座標系 → クライアント座標系
	C_rect	screen_to_client(C_rect rect);		// スクリーン座標系 → クライアント座標系
	C_point	client_to_screen(int x, int y);		// クライアント座標系 → スクリーン座標系
	C_point	client_to_screen(C_point point);	// クライアント座標系 → スクリーン座標系
	C_rect	client_to_screen(C_rect rect);		// クライアント座標系 → スクリーン座標系
	C_rect	client_to_window(C_rect rect);		// クライアント座標系 → ウィンドウ座標系

	void	adjust_to_parent(int dw, int dh);					// 親ウィンドウにあわせて位置を調整
	void	adjust_to_parent_4(int dl, int dt, int dw, int dh);	// 親ウィンドウにあわせて位置を調整

	void	set_top_most(bool top_most);	// 常に手前に表示
	void	set_focus();					// フォーカスを設定
	void	set_active();					// アクティブにする
	void	set_fore_ground();				// 最前面に設定
	void	set_show(bool show);			// 表示／非表示を設定
	bool	get_show();						// 表示／非表示を取得
	void	set_enable(bool enable);		// 有効／無効を設定
	bool	get_enable();					// 有効／無効を取得
	void	set_text(CTSTR& str);			// テキストを設定
	TSTR	get_text();						// テキストを取得
	void	set_icon(HICON h_icon);			// アイコンを設定
	void	set_small_icon(HICON h_icon);	// 小さいアイコンを設定
	void	set_icon_from_resource(int icon_id);	// アイコンを設定（リソースから）
	HICON	get_icon();						// アイコンを取得
	void	set_menu(HMENU h_menu);			// メニューを設定
	HMENU	get_menu();						// メニューを取得
	void	set_style(PTR style);			// スタイルを設定
	void	add_style(PTR style);			// スタイルを追加
	PTR		get_style();					// スタイルを取得
	void	set_style_ex(PTR style);		// 拡張スタイルを設定
	void	add_style_ex(PTR style);		// 拡張スタイルを追加
	PTR		get_style_ex();					// 拡張スタイルを取得
	void	set_proc(WNDPROC proc);			// プロシージャを設定
	WNDPROC	get_proc();						// プロシージャを取得
	void	reset_proc();					// プロシージャを元に戻す

	void	update();												// 更新
	void	invalidate_rect(C_rect* rect, bool erase);				// 矩形を更新（WM_PAINT が送信されます）
	void	invalidate_rect(ARRAY<C_rect>& rect_list, bool erase);	// 矩形を更新（複数指定）（WM_PAINT が送信されます）

	LRESULT	send_message(UINT msg, WPARAM wp, LPARAM lp);	// メッセージを送信
	LRESULT	post_message(UINT msg, WPARAM wp, LPARAM lp);	// メッセージを送信
	LRESULT	send_wm_size();									// WM_SIZE を送信

	// 子供コントロールリスト
	typedef	ARRAY<C_window_base *>	CHILDLIST;
	typedef	CHILDLIST::iterator		CHILDITR;
	CHILDITR	get_child_list_begin()	{	return m_child_list.begin();	}
	CHILDITR	get_child_list_end()	{	return m_child_list.end();		}

public:
	// コールバック本体（全ウィンドウで共通）
	static LRESULT CALLBACK window_proc_base(HWND h_wnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	// ウィンドウ情報
	HWND		m_h_wnd;					// ウィンドウハンドル
	TSTR		m_window_name;				// 名前（ini 保存時に使用）
	WNDPROC		m_old_window_proc;			// 古いウィンドウプロシージャ
	bool		m_is_dialog;				// ダイアログフラグ
	C_rect		m_def_window_rect;			// デフォルトのウィンドウ矩形（スクリーン座標系）
	C_rect		m_def_client_rect;			// デフォルトのクライアント矩形（クライアント座標系）
	C_rect		m_def_rltv_rect;			// デフォルトのウィンドウ矩形（親のクライアント座標系）
	bool		m_is_limit_window_rect_min;	// 最小矩形を制限
	bool		m_is_attach_window;			// 他のウィンドウにアタッチ
	int			m_adjust_way;				// 親ダイアログのサイズ変更時に親ダイアログにくっつく向き（1-9: テンキー準拠）
	int			m_group_no;					// グループ番号（自由にお使いください）
	CHILDLIST	m_child_list;				// 子供リスト
	static	WINDOW_ATTACH	m_attach;		// ウィンドウアタッチ情報
	HICON		h_large_icon;				// 大アイコン（３２×３２）
	HICON		h_small_icon;				// 小アイコン（１６×１６）

	void		init_param();				// 初期化
	
	virtual	void		window_proc_pre(UINT msg, WPARAM wp, LPARAM lp);	// 個別のプレプロシージャ（継承する）
	virtual	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);		// 個別のプロシージャ（継承する）

private:
	void	on_create_pre();
	bool	on_close();
	bool	on_destroy();
	bool	on_moving(WPARAM wp, LPARAM lp);
	bool	on_sizing(WPARAM wp, LPARAM lp);

	void	limit_window_size(WPARAM wp, LPARAM lp, C_rect min_rect);	// ウィンドウサイズを制限する

	// ウィンドウアタッチ系
	void	attach_init();
	void	attach_on_moving(WPARAM wp, LPARAM lp);
	void	attach_on_sizing(WPARAM wp, LPARAM lp);
	void	attach_check(HWND h_wnd, C_rect *rect1, C_rect *rect2, WINDOW_ATTACH_PROC *wap);
	void	attach_rep(C_rect *rect1, WINDOW_ATTACH_PROC *wap);

};

// ウィンドウ関連の関数
bool	get_monitor_info_from_pos(MONITORINFO* info, C_point pos);
bool	get_desktop_rect_from_pos(C_rect *desktop_rect, C_point pos);
bool	get_work_area_rect_from_pos(C_rect *work_area_rect, C_point pos);

}
