#pragma		once

#include	"tonawindow3_control.h"

namespace NT3
{

// ****************************************************************
// ダイアログセパレーター
// ================================================================
class C_dialog_separater
{
public:
	C_dialog_separater();

	// 状態
	struct STATE{
		bool	get_flag;		// 状態を取得したフラグ（false にする事で現在の取得状態は無効になる）
		int		space;			// スペース（第一コントロールから第二コントロールの間のドット数 ）
		int		now_pos;		// 現在のセパレーター座標
		int		move_top_pos;	// 稼動範囲の始点座標
		int		move_end_pos;	// 稼動範囲の終点座標
		int		dlg_top;		// ダイアログのクライアント始点
		int		dlg_end;		// ダイアログのクライアント終点
		int		percent;		// 現在位置の％（ＭＡＸ：100000％）
	} state;

	void	clear();											// クリア
	void	set_name(CTSTR& window_name, CTSTR& name_);			// 名前の設定
	void	save_ini(CTSTR& project_name = _T(""));				// ini に保存
	void	load_ini(CTSTR& project_name = _T(""));				// ini から読み込み

	void	now_to_percent();									// ダイアログセパレーター：現在位置を％に変換する（％ＭＡＸ：100000％）
	void	percent_to_now();									// ダイアログセパレーター：％を現在位置に変換する（％ＭＡＸ：100000％）

	bool	get_hv_mod();										// 水平垂直モードの取得
	void	set_hv_mod(bool hv_mod_);							// 水平垂直モードの設定（コントロールの登録リストが初期化されます）
	void	set_hv_mod(bool hv_mod_, C_window_base* first_ctrl, C_window_base* second_ctrl);		// 水平垂直モードの設定及び、第一コントロール、第二コントロールの設定（コントロールの登録リストが初期化されます）

	void	add_control_first(C_window_base* ctrl);					// 第一コントロールの追加
	void	add_control_second(C_window_base* ctrl);					// 第二コントロールの追加

	C_rect	get_ctrl_list_rect(ARRAY<C_window_base *> *ctrl_list);	// コントロールリストの合成矩形を取得
	C_rect	get_rect();												// 矩形を取得
	bool	get_state_func(STATE* state);							// セパレーターの状態を取得
	bool	get_state();											// セパレーターの状態を取得

	bool	check_mouse_hit(int screen_mouse_pos_x, int screen_mouse_pos_y);	// マウスヒット判定
	bool	change_mouse_cursor();												// マウスカーソル変更
	void	change_control_size(int val, int space);							// コントロールのサイズ変更
	void	on_size();															// ダイアログのサイズが変更された

protected:
	TSTR					_window_name;			// 親ウィンドウ名
	TSTR					_name;					// 名前
	bool					_hv_mod;				// 水平／垂直モード（false=horizontal true=vertical）
	ARRAY<C_window_base *>	_ctrl_list_first;		// 第一コントロールのリスト
	ARRAY<C_window_base *>	_ctrl_list_second;		// 第二コントロールのリスト
};

// ****************************************************************
// ダイアログセパレーターマネージャー
// ================================================================
class Cdialog_separater_manager
{
public:
	Cdialog_separater_manager();								// コンストラクタ

	void	clear();											// クリア

	void	bind_separater(C_dialog_separater* separater, CTSTR& window_name = _T(""), CTSTR& name = _T(""));		// セパレーターの割り当て

	void	save_ini();		// ini に保存
	void	load_ini();		// ini にから読込

	C_dialog_separater*	get_mouse_hit_separater(HWND h_wnd);	// マウスがヒットしているセパレーターのポインターを取得

	bool	on_mousemove(HWND h_wnd, WPARAM wp, LPARAM lp);		// マウスカーソルが移動した
	bool	on_setcursor(HWND h_wnd, WPARAM wp, LPARAM lp);		// マウスカーソルの変更通知
	bool	on_lbuttondown(HWND h_wnd, WPARAM wp, LPARAM lp);	// マウスの左ボタン押し下げ
	bool	on_size(HWND h_wnd, WPARAM wp, LPARAM lp);			// ダイアログのサイズが変更された

protected:
	ARRAY<C_dialog_separater *>	separater_list;				// セパレーターのリスト
	C_dialog_separater*			target_separater;			// 移動中のセパレーターのポインタ
	int							move_start_mouse_pos;		// 移動開始時のマウス座標
	int							move_start_separater_pos;	// 移動開始時のセパレーター座標
};

}
