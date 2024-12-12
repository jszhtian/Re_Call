#pragma		once

#include	"tonawindow3_control.h"

namespace NT3
{

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[
// ================================================================
class C_dialog_separater
{
public:
	C_dialog_separater();

	// ���
	struct STATE{
		bool	get_flag;		// ��Ԃ��擾�����t���O�ifalse �ɂ��鎖�Ō��݂̎擾��Ԃ͖����ɂȂ�j
		int		space;			// �X�y�[�X�i���R���g���[��������R���g���[���̊Ԃ̃h�b�g�� �j
		int		now_pos;		// ���݂̃Z�p���[�^�[���W
		int		move_top_pos;	// �ғ��͈͂̎n�_���W
		int		move_end_pos;	// �ғ��͈͂̏I�_���W
		int		dlg_top;		// �_�C�A���O�̃N���C�A���g�n�_
		int		dlg_end;		// �_�C�A���O�̃N���C�A���g�I�_
		int		percent;		// ���݈ʒu�́��i�l�`�w�F100000���j
	} state;

	void	clear();											// �N���A
	void	set_name(CTSTR& window_name, CTSTR& name_);			// ���O�̐ݒ�
	void	save_ini(CTSTR& project_name = _T(""));				// ini �ɕۑ�
	void	load_ini(CTSTR& project_name = _T(""));				// ini ����ǂݍ���

	void	now_to_percent();									// �_�C�A���O�Z�p���[�^�[�F���݈ʒu�����ɕϊ�����i���l�`�w�F100000���j
	void	percent_to_now();									// �_�C�A���O�Z�p���[�^�[�F�������݈ʒu�ɕϊ�����i���l�`�w�F100000���j

	bool	get_hv_mod();										// �����������[�h�̎擾
	void	set_hv_mod(bool hv_mod_);							// �����������[�h�̐ݒ�i�R���g���[���̓o�^���X�g������������܂��j
	void	set_hv_mod(bool hv_mod_, C_window_base* first_ctrl, C_window_base* second_ctrl);		// �����������[�h�̐ݒ�y�сA���R���g���[���A���R���g���[���̐ݒ�i�R���g���[���̓o�^���X�g������������܂��j

	void	add_control_first(C_window_base* ctrl);					// ���R���g���[���̒ǉ�
	void	add_control_second(C_window_base* ctrl);					// ���R���g���[���̒ǉ�

	C_rect	get_ctrl_list_rect(ARRAY<C_window_base *> *ctrl_list);	// �R���g���[�����X�g�̍�����`���擾
	C_rect	get_rect();												// ��`���擾
	bool	get_state_func(STATE* state);							// �Z�p���[�^�[�̏�Ԃ��擾
	bool	get_state();											// �Z�p���[�^�[�̏�Ԃ��擾

	bool	check_mouse_hit(int screen_mouse_pos_x, int screen_mouse_pos_y);	// �}�E�X�q�b�g����
	bool	change_mouse_cursor();												// �}�E�X�J�[�\���ύX
	void	change_control_size(int val, int space);							// �R���g���[���̃T�C�Y�ύX
	void	on_size();															// �_�C�A���O�̃T�C�Y���ύX���ꂽ

protected:
	TSTR					_window_name;			// �e�E�B���h�E��
	TSTR					_name;					// ���O
	bool					_hv_mod;				// �����^�������[�h�ifalse=horizontal true=vertical�j
	ARRAY<C_window_base *>	_ctrl_list_first;		// ���R���g���[���̃��X�g
	ARRAY<C_window_base *>	_ctrl_list_second;		// ���R���g���[���̃��X�g
};

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[
// ================================================================
class Cdialog_separater_manager
{
public:
	Cdialog_separater_manager();								// �R���X�g���N�^

	void	clear();											// �N���A

	void	bind_separater(C_dialog_separater* separater, CTSTR& window_name = _T(""), CTSTR& name = _T(""));		// �Z�p���[�^�[�̊��蓖��

	void	save_ini();		// ini �ɕۑ�
	void	load_ini();		// ini �ɂ���Ǎ�

	C_dialog_separater*	get_mouse_hit_separater(HWND h_wnd);	// �}�E�X���q�b�g���Ă���Z�p���[�^�[�̃|�C���^�[���擾

	bool	on_mousemove(HWND h_wnd, WPARAM wp, LPARAM lp);		// �}�E�X�J�[�\�����ړ�����
	bool	on_setcursor(HWND h_wnd, WPARAM wp, LPARAM lp);		// �}�E�X�J�[�\���̕ύX�ʒm
	bool	on_lbuttondown(HWND h_wnd, WPARAM wp, LPARAM lp);	// �}�E�X�̍��{�^����������
	bool	on_size(HWND h_wnd, WPARAM wp, LPARAM lp);			// �_�C�A���O�̃T�C�Y���ύX���ꂽ

protected:
	ARRAY<C_dialog_separater *>	separater_list;				// �Z�p���[�^�[�̃��X�g
	C_dialog_separater*			target_separater;			// �ړ����̃Z�p���[�^�[�̃|�C���^
	int							move_start_mouse_pos;		// �ړ��J�n���̃}�E�X���W
	int							move_start_separater_pos;	// �ړ��J�n���̃Z�p���[�^�[���W
};

}
