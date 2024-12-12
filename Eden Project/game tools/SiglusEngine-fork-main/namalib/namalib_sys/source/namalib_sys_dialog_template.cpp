#include	"pch.h"
#include	"namalib_sys_dialog_template.h"

namespace Nnama
{

// ****************************************************************
// ダイアログ準備
// ================================================================
void Cnamalib_dialog_template::init_standard_tool_window(CTSTR& title, bool size_change_flag)
{
	DWORD style = DS_SHELLFONT | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	if (size_change_flag)	{	style |= WS_THICKFRAME;		}
	init_param(0, 0, 0, 0, title, style, WS_EX_TOOLWINDOW, -1, 8, _T("MS Shell Dlg"));
}


// ****************************************************************
// ボタン追加
// ================================================================
void Cnamalib_dialog_template::add_btn(int x, int y, int w, int h, WORD control_id, CTSTR& text, bool group_flag)
{
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	if (group_flag)	{	style |= WS_GROUP;	}
	add_control(x, y, w, h, (PTR)_T("BUTTON"), control_id, style, 0, (PTR)text.c_str());
}

// ****************************************************************
// テキスト追加
// ================================================================
void Cnamalib_dialog_template::add_txt(int x, int y, int w, int h, WORD control_id, CTSTR& text, bool right_flag)
{
	DWORD style = WS_CHILD | WS_VISIBLE | WS_GROUP | SS_CENTERIMAGE;
	DWORD ex_style = right_flag ? WS_EX_RIGHT : 0;
	add_control(x, y, w, h, (PTR)_T("STATIC"), control_id, style, ex_style, (PTR)text.c_str());
}

// ****************************************************************
// エディットボックス追加
// ================================================================
void Cnamalib_dialog_template::add_edt(int x, int y, int w, int h, WORD control_id, bool dropdown_flag)
{
	DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_GROUP | ES_AUTOHSCROLL;
	DWORD ex_style = dropdown_flag ? WS_EX_ACCEPTFILES : 0;
	add_control(x, y, w, h, (PTR)_T("EDIT"), control_id, style, ex_style, NULL);
}

// ****************************************************************
// リストビュー追加（レポート型）
// ================================================================
void Cnamalib_dialog_template::add_lstv(int x, int y, int w, int h, WORD control_id)
{
	DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_GROUP | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT;
	add_control(x, y, w, h, (PTR)_T("SysListView32"), control_id, style, 0, NULL);
}

// ****************************************************************
// 配置パラメータ初期化
// ================================================================
void Cnamalib_dialog_template::init_ap(void)
{
	ap.undo_x = ap.push_x = ap.max_x = ap.x = NAMALIB_DLGTPLT_BASE_LEFT;
	ap.undo_y = ap.push_y = ap.max_y = ap.y = NAMALIB_DLGTPLT_BASE_TOP;
}

// ****************************************************************
// 配置パラメータ次の座標へ
// ================================================================
void Cnamalib_dialog_template::next_ap(int ap_mod, int w, int h)
{
	int max;
	max = ap.x + w;		if (max > ap.max_x)	{	ap.max_x = max;	}
	if (ap_mod == 0 || ap_mod == 2)	{	ap.undo_x = ap.x;	ap.x = (max + NAMALIB_DLGTPLT_SPACE_X);		}
	max = ap.y + h;		if (max > ap.max_y)	{	ap.max_y = max;	}
	if (ap_mod == 1 || ap_mod == 2)	{	ap.undo_y = ap.y;	ap.y = (max + NAMALIB_DLGTPLT_SPACE_Y);		}
}

// ****************************************************************
// 配置パラメータからダイアログサイズを設定
// ================================================================
void Cnamalib_dialog_template::set_size_from_ap(void)
{
	set_size(ap.max_x + NAMALIB_DLGTPLT_BASE_RIGHT, ap.max_y + NAMALIB_DLGTPLT_BASE_BOTTOM);
}

// ****************************************************************
// 配置パラメータから現在のアイテムの最大幅を設定
// ================================================================
int Cnamalib_dialog_template::get_ap_max_width(void)
{
	return ap.max_x - NAMALIB_DLGTPLT_BASE_LEFT;
}

// ****************************************************************
// 配置パラメータから現在のアイテムの最大高を設定
// ================================================================
int Cnamalib_dialog_template::get_ap_max_height(void)
{
	return ap.max_y - NAMALIB_DLGTPLT_BASE_TOP;
}

// ****************************************************************
// 配置パラメータＸ戻す
// ================================================================
void Cnamalib_dialog_template::undo_ap_x(void)
{
	ap.x = ap.undo_x;
}

// ****************************************************************
// 配置パラメータＹ戻す
// ================================================================
void Cnamalib_dialog_template::undo_ap_y(void)
{
	ap.y = ap.undo_y;
}

// ****************************************************************
// 配置パラメータＸ設定
// ================================================================
void Cnamalib_dialog_template::set_ap_x(int x)
{
	if (x == 0)	{	x = NAMALIB_DLGTPLT_BASE_LEFT;		}		ap.x = x;	ap.undo_x = ap.x;
}

// ****************************************************************
// 配置パラメータＹ設定
// ================================================================
void Cnamalib_dialog_template::set_ap_y(int y)
{
	if (y == 0)	{	y = NAMALIB_DLGTPLT_BASE_TOP;		}		ap.y = y;	ap.undo_y = ap.y;
}

// ****************************************************************
// 配置パラメータＸ加算
// ================================================================
void Cnamalib_dialog_template::add_ap_x(int x)
{
	ap.x += x;	ap.undo_x = ap.x;
}

// ****************************************************************
// 配置パラメータＹ加算
// ================================================================
void Cnamalib_dialog_template::add_ap_y(int y)
{
	ap.y += y;	ap.undo_y = ap.y;
}

// ****************************************************************
// 配置パラメータＸ設定（右位置した場合のＸ座標）
// ================================================================
void Cnamalib_dialog_template::set_ap_x_right_arrangement(int cnt, ...)
{
	if (cnt <= 0)	{	return;	}

	int width = get_ap_max_width();

	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		width -= va_arg(arg, int);
	}
	va_end(arg);

	width -= (NAMALIB_DLGTPLT_SPACE_X * (cnt - 1));

	set_ap_x(NAMALIB_DLGTPLT_BASE_LEFT + width);
}

// ****************************************************************
// 配置パラメータＸＰＵＳＨ
// ================================================================
void Cnamalib_dialog_template::push_ap_x(void)
{
	ap.push_x = ap.x;
}

// ****************************************************************
// 配置パラメータＹＰＵＳＨ
// ================================================================
void Cnamalib_dialog_template::push_ap_y(void)
{
	ap.push_y = ap.y;
}

// ****************************************************************
// 配置パラメータＸＰＯＰ
// ================================================================
void Cnamalib_dialog_template::pop_ap_x(void)
{
	ap.x = ap.push_x;
}

// ****************************************************************
// 配置パラメータＹＰＯＰ
// ================================================================
void Cnamalib_dialog_template::pop_ap_y(void)
{
	ap.y = ap.push_y;
}

// ****************************************************************
// 配置パラメータ版：ボタン追加
// ================================================================
void Cnamalib_dialog_template::add_ap_btn(int ap_mod, int w, int h, WORD control_id, CTSTR& text, bool group_flag)
{
	if (h == 0)	{	h = NAMALIB_DLGTPLT_HEIGHT;	}
	add_btn(ap.x, ap.y, w, h, control_id, text, group_flag);
	next_ap(ap_mod, w, h);
}

// ****************************************************************
// 配置パラメータ版：テキストとエディットボックス追加
// ================================================================
void Cnamalib_dialog_template::add_ap_tedt(int ap_mod, int txt_w, WORD txt_control_id, CTSTR& text, int edt_w, WORD edt_control_id, bool dropdown_flag)
{
	int h = NAMALIB_DLGTPLT_HEIGHT;
	int w = txt_w + 1 + edt_w;
	add_txt(ap.x,             ap.y, txt_w, h, txt_control_id, text, true);
	add_edt(ap.x + txt_w + 1, ap.y, edt_w, h, edt_control_id, dropdown_flag);
	next_ap(ap_mod, w, h);
}

// ****************************************************************
// 配置パラメータ版：リストビュー追加（レポート型）
// ================================================================
void Cnamalib_dialog_template::add_ap_lstv(int ap_mod, int w, int h, WORD control_id)
{
	add_lstv(ap.x, ap.y, w, h, control_id);
	next_ap(ap_mod, w, h);
}

}