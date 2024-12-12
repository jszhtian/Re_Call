#include	"tonawindow3_pch.h"
#include	"tonawindow3_dialog_statusbar.h"

namespace NT3
{

// ****************************************************************
// ダイアログステータスバー：コンストラクタ
// ================================================================
C_dialog_statusbar::C_dialog_statusbar()
{
	create_flag = false;
	h_wnd = NULL;
}

// ****************************************************************
// ダイアログステータスバー：ステータスバーのサイズグリップ作成
// ================================================================
bool C_dialog_statusbar::create_sizegrip(HWND parent_h_wnd)
{
	h_wnd = CreateStatusWindow(WS_CHILD|WS_VISIBLE|WS_GROUP|WS_CLIPSIBLINGS|CCS_BOTTOM|SBARS_SIZEGRIP, _T(""), parent_h_wnd, IDEX_STATUS_BAR00);
	if (h_wnd == NULL)	{
		create_flag = false;
		return false;
	}
	SendMessage(h_wnd, SB_SETTEXT, (WPARAM)(0 | SBT_NOBORDERS), (LPARAM)_T(""));
	create_flag = true;
	return true;
}

// ****************************************************************
// ダイアログステータスバー：サイズ調整
// ================================================================
void C_dialog_statusbar::on_size(HWND parent_h_wnd, WPARAM wp, LPARAM lp)
{
	if (!create_flag)	{	return;	}
	SendMessage(h_wnd, WM_SIZE, wp, lp);
}

}
