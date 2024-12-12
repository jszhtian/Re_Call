#include	"tonawindow3_pch.h"
#include	"tonawindow3_dialog_statusbar.h"

namespace NT3
{

// ****************************************************************
// �_�C�A���O�X�e�[�^�X�o�[�F�R���X�g���N�^
// ================================================================
C_dialog_statusbar::C_dialog_statusbar()
{
	create_flag = false;
	h_wnd = NULL;
}

// ****************************************************************
// �_�C�A���O�X�e�[�^�X�o�[�F�X�e�[�^�X�o�[�̃T�C�Y�O���b�v�쐬
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
// �_�C�A���O�X�e�[�^�X�o�[�F�T�C�Y����
// ================================================================
void C_dialog_statusbar::on_size(HWND parent_h_wnd, WPARAM wp, LPARAM lp)
{
	if (!create_flag)	{	return;	}
	SendMessage(h_wnd, WM_SIZE, wp, lp);
}

}
