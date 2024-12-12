#include	"pch.h"
#include	"namalib_sys_dialog_template.h"
#include	"namalib_sys_int_window.h"

enum	{
	ITEM_LSTV_MSG,
};

// ****************************************************************
// コンストラクタ
// ================================================================
Cnamalib_num_window::Cnamalib_num_window()
{
	dummy_num = -9999999;
}

// ****************************************************************
// 準備
// ================================================================
void Cnamalib_num_window::ready(CTSTR& window_title_str_, CTSTR& section_name_str_, int num_cnt)
{
	window_title_str = window_title_str_;
	section_name_str = section_name_str_;

	num_list.clear();
	num_list.resize(num_cnt);
}

// ****************************************************************
// 最後の状態をＩＮＩに記憶
// ================================================================
void Cnamalib_num_window::save_ini_last_state()
{
	Cini_file::save_bool(section_name_str + _T("_DIALOG"), _T("LAST_OPEN_STATE"), h_wnd ? true : false);
}

// ****************************************************************
// 最後の状態をＩＮＩから読み込む
// ================================================================
bool Cnamalib_num_window::load_ini_last_state()
{
	return Cini_file::load_bool(section_name_str + _T("_DIALOG"), _T("LAST_OPEN_STATE"), false);
}

// ****************************************************************
// ウィンドウプロシージャ
// ================================================================
LRESULT Cnamalib_num_window::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)	{
		case WM_INITDIALOG:
			assign_control(&_item_lstv_numlist, ITEM_LSTV_MSG, _T("ITEM_LSTV_MSG"), 5);

			_item_lstv_numlist.easy_setting(true, true, false, false);
			_item_lstv_numlist.add_column(_T("INDEX"), _T("INDEX"), 45);
			_item_lstv_numlist.add_column(_T("VAL"), _T("VAL"), 45);

			load_ini_window_rect();
			_item_lstv_numlist.load_ini();

			update();
			set_show(true);
			break;
		case WM_DESTROY:
			save_ini_window_rect();
			_item_lstv_numlist.save_ini();
			break;
		case WM_SIZE:
			{
				int dw = LOWORD(lp) - def_client_rect.width();
				int dh = HIWORD(lp) - def_client_rect.height();
				adjust_control(dw, dh);
			}
			break;
	}

	// 基底のウィンドウプロシージャを呼ぶ
	return Cdialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// 開く／閉じる（作成／破棄）
// ================================================================
void Cnamalib_num_window::open_close(HWND parent_h_wnd, bool parent_focus)
{
	if (h_wnd == NULL)	{
		Cnamalib_dialog_template dlgtplt;
		dlgtplt.init_standard_tool_window(window_title_str, true);
		dlgtplt.init_ap();

		dlgtplt.add_ap_lstv(0, 90, 60, ITEM_LSTV_MSG);

		dlgtplt.set_size_from_ap();
		create_indirect(&dlgtplt, section_name_str, true, true, parent_h_wnd);

		if (parent_h_wnd && parent_focus)	{	SetFocus(parent_h_wnd);	}
	}
	else	{
		destroy();
	}
}

// ****************************************************************
// 起動時のオープン
// ================================================================
void Cnamalib_num_window::start_open(HWND parent_h_wnd)
{
	if (load_ini_last_state())	{	open_close(parent_h_wnd, false);	}
}

// ****************************************************************
// 数値設定
// ================================================================
void Cnamalib_num_window::set_num(int index, int val)
{
	if (0 <= index && index < (int)num_list.size()){	num_list[index] = val;	}
}

// ****************************************************************
// アップデートプロセス
// ================================================================
void Cnamalib_num_window::update_proc()
{
	if (h_wnd == NULL)	{	return;	}

	if (num_list.size() != num_list_last.size())	{
		_item_lstv_numlist.clear();
		num_list_last.clear();

		if (!num_list.empty())	{
			num_list_last.resize(num_list.size());
			for (DWORD i = 0; i < num_list.size(); i++)	{
				_item_lstv_numlist.add_item(0, format(_T("[ %03d ]"), i));
				num_list_last[i] = num_list[i] + 1;		// +1 = 違う値にする
			}
		}
	}

	if (!num_list.empty())	{
		for (DWORD i = 0; i < num_list.size(); i++)	{
			if (num_list_last[i] != num_list[i])	{
				_item_lstv_numlist.set_cell_text(i, 1, tostr(num_list[i]));
				num_list_last[i] = num_list[i];
			}
		}
	}
}

