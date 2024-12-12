#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

// ****************************************************************
// �A�v���P�[�V����
// ================================================================
class C_app
{
public:
	C_app();
	~C_app();

	HINSTANCE		h_inst;				// �A�v���P�[�V�����̃n���h��
	HWND			h_wnd;				// ���C���E�B���h�E�̃n���h��
	TSTR			command_line;		// �R�}���h���C��
	TSTR			module_dir;			// ���W���[���̃f�B���N�g��
	TSTR			module_name;		// ���W���[���̃t�@�C����
	TSTR			app_name;			// �A�v���P�[�V������
	TSTR			app_dir;			// �A�v���P�[�V�����f�B���N�g��
	ARRAY<HWND>		window_list;		// �E�B���h�E���X�g
	bool			quit_flag;			// WM_QUIT �Ŕ������t���O


	bool	default_message_loop();				// ���b�Z�[�W���[�v
	bool	default_message_proc(MSG* msg);		// ���b�Z�[�W����
};

extern	C_app	G_app;



}
