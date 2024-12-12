#include	"tonawindow3_pch.h"
#include	"tonawindow3_dialog_separater.h"

namespace NT3
{

// ****************************************************************
// ダイアログセパレーター：コンストラクタ
// ================================================================
C_dialog_separater::C_dialog_separater()
{
	clear();
}

// ****************************************************************
// ダイアログセパレーター：クリア
// ================================================================
void C_dialog_separater::clear()
{
	_window_name.clear();
	_name.clear();
	_ctrl_list_first.clear();
	_ctrl_list_second.clear();
	state.get_flag = false;
	state.percent = -1;
	_hv_mod = false;
}

// ****************************************************************
// ダイアログセパレーター：名前の設定
// ================================================================
void C_dialog_separater::set_name(CTSTR& window_name, CTSTR& name)
{
	_window_name = window_name;
	_name = name;
}

// ****************************************************************
// ダイアログセパレーター：ini に保存
// ================================================================
void C_dialog_separater::save_ini(CTSTR& project_name)
{
	if (!_name.empty())	{
		C_ini_file::body().save_int(_T("percent"), state.percent, project_name, _window_name, _name);
	}
}

// ****************************************************************
// ダイアログセパレーター：ini から読み込み
// ================================================================
void C_dialog_separater::load_ini(CTSTR& project_name)
{
	if (!_name.empty())	{
		state.percent = C_ini_file::body().load_int(_T("percent"), -1, project_name, _window_name, _name);
		on_size();				// サイズが変更されたときの処理を呼ぶ
	}
}

// ****************************************************************
// ダイアログセパレーター：現在位置を％に変換する（％ＭＡＸ：100000％）
// ================================================================

#define		PERCENT_POS_MAX		100000

void C_dialog_separater::now_to_percent()
{
	if (state.now_pos <= state.move_top_pos)	{
		state.percent = 0;
	}
	else if (state.now_pos >= state.move_end_pos)	{
		state.percent = PERCENT_POS_MAX;
	}
	else	{
		state.percent = MulDiv(PERCENT_POS_MAX, state.now_pos, state.dlg_end - state.dlg_top);
	}
}

// ****************************************************************
// ダイアログセパレーター：％を現在位置に変換する（％ＭＡＸ：100000％）
// =========================================v=======================
void C_dialog_separater::percent_to_now()
{
	if (state.percent == 0)	{
		state.now_pos = state.move_top_pos;
	}
	else if (state.percent == PERCENT_POS_MAX)	{
		state.now_pos = state.move_end_pos;
	}
	else	{
		state.now_pos = limit(state.move_top_pos, MulDiv(state.dlg_end - state.dlg_top, state.percent, PERCENT_POS_MAX), state.move_end_pos);
	}
}

// ****************************************************************
// ダイアログセパレーター：水平垂直モードの取得
//		hv_mod : 水平垂直モード（false=horizontal true=vertical）
// ================================================================
bool C_dialog_separater::get_hv_mod()
{
	return _hv_mod;
}

// ****************************************************************
// ダイアログセパレーター：水平垂直モードの設定（※コントロールの登録リストが初期化されます）
//		hv_mod : 水平垂直モード（false=horizontal true=vertical）
// ================================================================
void C_dialog_separater::set_hv_mod(bool hv_mod_)
{
	_hv_mod = hv_mod_;
	_ctrl_list_first.clear();
	_ctrl_list_second.clear();
}

// ****************************************************************
// ダイアログセパレーター：水平垂直モードの設定及び、第一コントロール、第二コントロールの設定（※コントロールの登録リストが初期化されます）
//		hv_mod : 水平／垂直モード（false=horizontal true=vertical）
// ================================================================
void C_dialog_separater::set_hv_mod(bool hv_mod_, C_window_base* first_ctrl, C_window_base* second_ctrl)
{
	set_hv_mod(hv_mod_);
	add_control_first(first_ctrl);
	add_control_second(second_ctrl);
}

// ****************************************************************
// ダイアログセパレーター：第一コントロールの追加
// ================================================================
void C_dialog_separater::add_control_first(C_window_base* ctrl)
{
	_ctrl_list_first.push_back(ctrl);
}

// ****************************************************************
// ダイアログセパレーター：第二コントロールの追加
// ================================================================
void C_dialog_separater::add_control_second(C_window_base* ctrl)
{
	_ctrl_list_second.push_back(ctrl);
}

// ****************************************************************
// ダイアログセパレーター：コントロールリストの合成矩形を取得
// ================================================================
C_rect C_dialog_separater::get_ctrl_list_rect(ARRAY<C_window_base *> *ctrl_list)
{
	ARRAY<C_window_base *>::iterator end = ctrl_list->end();
	ARRAY<C_window_base *>::iterator itr = ctrl_list->begin();
	(*itr)->get_parent_handle();
	C_rect total_rect = (*itr)->get_rltv_rect();
	itr++;
	C_rect rect;
	for (; itr != end; ++itr)	{
		rect = (*itr)->get_rltv_rect();
		if (total_rect.top    > rect.top)		{	total_rect.top    = rect.top;		}
		if (total_rect.bottom < rect.bottom)	{	total_rect.bottom = rect.bottom;	}
		if (total_rect.left   > rect.left)		{	total_rect.left   = rect.left;		}
		if (total_rect.right  < rect.right)		{	total_rect.right  = rect.right;		}
	}
	return total_rect;
}

// ****************************************************************
// ダイアログセパレーター：矩形を取得
// ================================================================
C_rect C_dialog_separater::get_rect()
{
	if (_ctrl_list_first.empty() || _ctrl_list_second.empty())
		return C_rect(0, 0, 0, 0);

	C_rect client_rect;

	C_rect first_rect  = get_ctrl_list_rect(&_ctrl_list_first);
	C_rect second_rect = get_ctrl_list_rect(&_ctrl_list_second);

	if (_hv_mod == false)	{		// 水平
		client_rect.top    = first_rect.bottom;
		client_rect.bottom = second_rect.top;
		client_rect.left   = (first_rect.left  > second_rect.left)  ? first_rect.left  : second_rect.left;
		client_rect.right  = (first_rect.right < second_rect.right) ? first_rect.right : second_rect.right;
	}
	else	{						// 垂直
		client_rect.top    = (first_rect.top    > second_rect.top)    ? first_rect.top    : second_rect.top;
		client_rect.bottom = (first_rect.bottom < second_rect.bottom) ? first_rect.bottom : second_rect.bottom;
		client_rect.left   = first_rect.right;
		client_rect.right  = second_rect.left;
	}

	return client_rect;
}

// ****************************************************************
// ダイアログセパレーター：セパレーターの状態を取得
// ================================================================

bool C_dialog_separater::get_state_func(STATE* state)
{
	state->get_flag = false;
	if (_ctrl_list_first.empty() || _ctrl_list_second.empty())
		return false;

	ARRAY<C_window_base *>::iterator end;
	ARRAY<C_window_base *>::iterator itr;
	C_rect rect;

	// コントロール範囲の始点座標
	end = _ctrl_list_first.end();
	itr = _ctrl_list_first.begin();
	rect = (*itr)->get_rltv_rect();
	int ctrl_top_pos = (_hv_mod == false) ? rect.top : rect.left;
	itr++;
	for (; itr != end; ++itr)	{
		rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	if (ctrl_top_pos < rect.top)	{	ctrl_top_pos = rect.top;	}	}	// 水平
		else					{	if (ctrl_top_pos < rect.left)	{	ctrl_top_pos = rect.left;	}	}	// 垂直
	}

	// コントロール範囲の終点座標
	end = _ctrl_list_second.end();
	itr = _ctrl_list_second.begin();
	rect = (*itr)->get_rltv_rect();
	int ctrl_end_pos = (_hv_mod == false) ? rect.bottom : rect.right;
	itr++;
	for (; itr != end; ++itr)	{
		rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	if (ctrl_end_pos > rect.bottom)	{	ctrl_end_pos = rect.bottom;	}	}	// 水平
		else					{	if (ctrl_end_pos > rect.right)	{	ctrl_end_pos = rect.right;	}	}	// 垂直
	}

	// 現在のセパレーター座標とスペース
	rect = get_rect();
	if (_hv_mod == false)	{			// 水平
		state->space = rect.height();	// スペース（第一コントロールから第二コントロールの間のドット数 ）
		state->now_pos = rect.top;		// 現在のセパレーター座標
	}
	else	{							// 垂直
		state->space = rect.width();	// // スペース（第一コントロールから第二コントロールの間のドット数 ）
		state->now_pos = rect.left;		// 現在のセパレーター座標
	}

	// ダイアログのクライアント範囲
	HWND parent_h_wnd = (*_ctrl_list_first.begin())->get_parent_handle();		// 全部同じはずだから、どのコントロールのものでも良いのです
	GetClientRect(parent_h_wnd, &rect);
	state->dlg_top = 0;
	if (_hv_mod == false)	{			// 水平
		state->dlg_end = rect.height();	// 高サイズ
	}
	else	{							// 垂直
		state->dlg_end = rect.width();	// 幅サイズ
	}

	// 稼動範囲の始点座標
	state->move_top_pos = ctrl_top_pos + 16;

	// 稼動範囲の終点座標
	state->move_end_pos = ctrl_end_pos - 1 - state->space - 16;

	if (state->move_top_pos > state->move_end_pos)	{	return false;	}

	// 現在位置を％に変換する
	now_to_percent();

	state->get_flag = true;
	return true;
}

bool C_dialog_separater::get_state()
{
	return get_state_func(&state);
}

// ****************************************************************
// ダイアログセパレーター：マウスヒット判定
// ================================================================
bool C_dialog_separater::check_mouse_hit(int screen_mouse_pos_x, int screen_mouse_pos_y)
{
	C_rect rect = get_rect();
	if (rect.top != 0 || rect.bottom != 0 || rect.left != 0 || rect.right != 0)	{
		if (rect.is_in(screen_mouse_pos_x, screen_mouse_pos_y))	{	return true;	}
	}
	return false;
}

// ****************************************************************
// ダイアログセパレーター：マウスカーソル変更
// ================================================================
bool C_dialog_separater::change_mouse_cursor()
{
	HCURSOR hcursor = LoadCursor(NULL, _hv_mod ? IDC_SIZEWE : IDC_SIZENS);
	if (hcursor != GetCursor())	{	SetCursor(hcursor);		return true;	}
	return false;
}

// ****************************************************************
// ダイアログセパレーター：コントロールのサイズ変更
// ================================================================
void C_dialog_separater::change_control_size(int val, int space)
{
	// 稼動範囲外ならここでリターン
	if (state.get_flag == false || state.move_top_pos > val || val > state.move_end_pos)
		return;

	ARRAY<C_window_base *>::iterator end;
	ARRAY<C_window_base *>::iterator itr;
	C_rect rect;

	// 第一コントロールのサイズ変更
	end = _ctrl_list_first.end();
	for (itr = _ctrl_list_first.begin(); itr != end; ++itr)	{
		rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	rect.bottom = val;	}	// 水平
		else					{	rect.right  = val;	}	// 垂直
		(*itr)->set_window_rect(rect);
	}

	// 第二コントロールのサイズ変更
	val += space;
	end = _ctrl_list_second.end();
	for (itr = _ctrl_list_second.begin(); itr != end; ++itr)	{
		C_rect rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	rect.top  = val;	}	// 水平
		else					{	rect.left = val;	}	// 垂直
		(*itr)->set_window_rect(rect);
	}
}

// ****************************************************************
// ダイアログセパレーター：ダイアログのサイズが変更された
// ================================================================
void C_dialog_separater::on_size()
{
	int percent = state.percent;
	if (get_state() == false)
		return;

	if (percent != -1)	{		// 初回起動時のみ ini 情報が無いので -1 になってるはず
		state.percent = percent;
		percent_to_now();
	}
	change_control_size(state.now_pos, state.space);
}

// ****************************************************************
// ダイアログセパレーターマネージャー：コンストラクタ
// ================================================================
Cdialog_separater_manager::Cdialog_separater_manager()
{
	clear();
}

// ****************************************************************
// ダイアログセパレーターマネージャー：クリア
// ================================================================
void Cdialog_separater_manager::clear()
{
	if (!separater_list.empty())	{
		for (ARRAY<C_dialog_separater *>::iterator itr = separater_list.begin(); itr != separater_list.end(); ++itr)	{
			(*itr)->clear();
		}
		separater_list.clear();
	}
	target_separater = NULL;
}

// ****************************************************************
// ダイアログセパレーターマネージャー：セパレーターの割り当て
// ================================================================
void Cdialog_separater_manager::bind_separater(C_dialog_separater* separater, CTSTR& window_name, CTSTR& name)
{
	separater->set_name(window_name, name);
	separater_list.push_back(separater);
}

// ****************************************************************
// ダイアログセパレーターマネージャー：ini に保存
// ================================================================
void Cdialog_separater_manager::save_ini()
{
	if (separater_list.empty())
		return;

	for (ARRAY<C_dialog_separater *>::iterator itr = separater_list.begin(); itr != separater_list.end(); ++itr)	{
		(*itr)->save_ini();
	}
}

// ****************************************************************
// ダイアログセパレーターマネージャー：ini から読み込み 
// ================================================================
void Cdialog_separater_manager::load_ini()
{
	if (separater_list.empty())
		return;

	for (ARRAY<C_dialog_separater *>::iterator itr = separater_list.begin(); itr != separater_list.end(); ++itr)	{
		(*itr)->load_ini();
	}
}

// ****************************************************************
// ダイアログセパレーターマネージャー：マウスがヒットしているセパレーターのポインターを取得
// ================================================================
C_dialog_separater* Cdialog_separater_manager::get_mouse_hit_separater(HWND h_wnd)
{
	if (separater_list.empty())
		return NULL;

	C_point pos;
	GetCursorPos(&pos);
	ScreenToClient(h_wnd, &pos);
	bool mouse_hit = false;
	for (ARRAY<C_dialog_separater *>::iterator itr = separater_list.begin(); itr != separater_list.end(); ++itr)	{
		if ((*itr)->check_mouse_hit(pos.x, pos.y))	{	return *itr;	}
	}
	return NULL;
}

// ****************************************************************
// ダイアログセパレーターマネージャー：マウスカーソルが移動した
// ================================================================
bool Cdialog_separater_manager::on_mousemove(HWND h_wnd, WPARAM wp, LPARAM lp)
{
	if (separater_list.empty())
		return false;

	if (target_separater)	{
		if ((GetKeyState(VK_LBUTTON) & 0x80000000))	{
			C_point pos;
			GetCursorPos(&pos);
			int val = (target_separater->get_hv_mod() == false) ? pos.y : pos.x;
			val = move_start_separater_pos + (val - move_start_mouse_pos);
			C_dialog_separater::STATE* state = &target_separater->state;
			state->now_pos = limit(state->move_top_pos, val, state->move_end_pos);
			target_separater->now_to_percent();
			target_separater->change_control_size(state->now_pos, state->space);
			return true;
		}
		else	{
			ReleaseCapture();		// ★マウスキャプチャー終了
			target_separater = NULL;
		}
	}
	return false;
}

// ****************************************************************
// ダイアログセパレーターマネージャー：マウスカーソルの変更通知
// ================================================================
bool Cdialog_separater_manager::on_setcursor(HWND h_wnd, WPARAM wp, LPARAM lp)
{
	if (separater_list.empty())
		return false;

	C_dialog_separater* sepa;
	if (target_separater)	{	sepa = target_separater;				}
	else					{	sepa = get_mouse_hit_separater(h_wnd);	}
	if (sepa)	{
		sepa->change_mouse_cursor();
		return true;
	}
	return false;
}

// ****************************************************************
// ダイアログセパレーターマネージャー：マウスの左ボタン押し下げ
// ================================================================
bool Cdialog_separater_manager::on_lbuttondown(HWND h_wnd, WPARAM wp, LPARAM lp)
{
	if (separater_list.empty())
		return false;

	target_separater = get_mouse_hit_separater(h_wnd);
	if (target_separater)	{
		if (target_separater->get_state())	{
			C_point pos;
			GetCursorPos(&pos);
			move_start_mouse_pos = (target_separater->get_hv_mod() == false) ? pos.y : pos.x;
			move_start_separater_pos = target_separater->state.now_pos;
			SetCapture(h_wnd);		// ★マウスキャプチャー開始
			return true;
		}
		else	{
			target_separater = NULL;
		}
	}
	return false;
}

// ****************************************************************
// ダイアログセパレーターマネージャー：ダイアログのサイズが変更された
// ================================================================
bool Cdialog_separater_manager::on_size(HWND h_wnd, WPARAM wp, LPARAM lp)
{
	if (separater_list.empty())
		return false;

	for (ARRAY<C_dialog_separater *>::iterator itr = separater_list.begin(); itr != separater_list.end(); ++itr)	{
		(*itr)->on_size();
	}
	return false;
}


}
