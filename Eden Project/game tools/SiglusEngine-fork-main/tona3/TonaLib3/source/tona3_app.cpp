#include	"tona3_pch.h"
#include	"tona3_app.h"

namespace NT3
{

// ****************************************************************
// �A�v���P�[�V�����̃C���X�^���X
// ================================================================
C_app	G_app;

// ****************************************************************
// �A�v���P�[�V����
// ================================================================
C_app::C_app()
{
	h_inst = NULL;
	h_wnd = NULL;
	quit_flag = false;
}

C_app::~C_app()
{
}

// ****************************************************************
// ���b�Z�[�W���[�v
// ================================================================
bool C_app::default_message_loop()
{
	MSG msg;

	// �I���t���O�������Ă���ΏI��
	if (quit_flag)
		return false;

	// ���b�Z�[�W�����o��
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	{
		// WM_QUIT �������� false
		if (!GetMessage(&msg, NULL, 0, 0))	{
			quit_flag = true;	// �I���t���O
			return false;
		}
		// ���b�Z�[�W����
		if (!default_message_proc(&msg))
			return false;
	}

	// ���b�Z�[�W�����Ȃ�������߂�
	return true;
}

// ****************************************************************
// ���b�Z�[�W����
// ================================================================
bool C_app::default_message_proc(MSG* msg)
{
	// �_�C�A���O���b�Z�[�W�̏���
	for (ARRAY<HWND>::iterator itr = G_app.window_list.begin(); itr != G_app.window_list.end(); ++itr)	{
		if (GetWindowLong(*itr, DWL_DLGPROC))	{
			if (IsDialogMessage(*itr, msg) != 0)
				return true;
		}
	}

	// �ʏ�̃��b�Z�[�W����
	TranslateMessage(msg);
	DispatchMessage(msg);

	return true;
}

}
