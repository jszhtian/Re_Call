#include	"tonawindow3_pch.h"
#include	"tonawindow3_dialog_separater.h"

namespace NT3
{

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�R���X�g���N�^
// ================================================================
C_dialog_separater::C_dialog_separater()
{
	clear();
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�N���A
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
// �_�C�A���O�Z�p���[�^�[�F���O�̐ݒ�
// ================================================================
void C_dialog_separater::set_name(CTSTR& window_name, CTSTR& name)
{
	_window_name = window_name;
	_name = name;
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�Fini �ɕۑ�
// ================================================================
void C_dialog_separater::save_ini(CTSTR& project_name)
{
	if (!_name.empty())	{
		C_ini_file::body().save_int(_T("percent"), state.percent, project_name, _window_name, _name);
	}
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�Fini ����ǂݍ���
// ================================================================
void C_dialog_separater::load_ini(CTSTR& project_name)
{
	if (!_name.empty())	{
		state.percent = C_ini_file::body().load_int(_T("percent"), -1, project_name, _window_name, _name);
		on_size();				// �T�C�Y���ύX���ꂽ�Ƃ��̏������Ă�
	}
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F���݈ʒu�����ɕϊ�����i���l�`�w�F100000���j
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
// �_�C�A���O�Z�p���[�^�[�F�������݈ʒu�ɕϊ�����i���l�`�w�F100000���j
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
// �_�C�A���O�Z�p���[�^�[�F�����������[�h�̎擾
//		hv_mod : �����������[�h�ifalse=horizontal true=vertical�j
// ================================================================
bool C_dialog_separater::get_hv_mod()
{
	return _hv_mod;
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�����������[�h�̐ݒ�i���R���g���[���̓o�^���X�g������������܂��j
//		hv_mod : �����������[�h�ifalse=horizontal true=vertical�j
// ================================================================
void C_dialog_separater::set_hv_mod(bool hv_mod_)
{
	_hv_mod = hv_mod_;
	_ctrl_list_first.clear();
	_ctrl_list_second.clear();
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�����������[�h�̐ݒ�y�сA���R���g���[���A���R���g���[���̐ݒ�i���R���g���[���̓o�^���X�g������������܂��j
//		hv_mod : �����^�������[�h�ifalse=horizontal true=vertical�j
// ================================================================
void C_dialog_separater::set_hv_mod(bool hv_mod_, C_window_base* first_ctrl, C_window_base* second_ctrl)
{
	set_hv_mod(hv_mod_);
	add_control_first(first_ctrl);
	add_control_second(second_ctrl);
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F���R���g���[���̒ǉ�
// ================================================================
void C_dialog_separater::add_control_first(C_window_base* ctrl)
{
	_ctrl_list_first.push_back(ctrl);
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F���R���g���[���̒ǉ�
// ================================================================
void C_dialog_separater::add_control_second(C_window_base* ctrl)
{
	_ctrl_list_second.push_back(ctrl);
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�R���g���[�����X�g�̍�����`���擾
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
// �_�C�A���O�Z�p���[�^�[�F��`���擾
// ================================================================
C_rect C_dialog_separater::get_rect()
{
	if (_ctrl_list_first.empty() || _ctrl_list_second.empty())
		return C_rect(0, 0, 0, 0);

	C_rect client_rect;

	C_rect first_rect  = get_ctrl_list_rect(&_ctrl_list_first);
	C_rect second_rect = get_ctrl_list_rect(&_ctrl_list_second);

	if (_hv_mod == false)	{		// ����
		client_rect.top    = first_rect.bottom;
		client_rect.bottom = second_rect.top;
		client_rect.left   = (first_rect.left  > second_rect.left)  ? first_rect.left  : second_rect.left;
		client_rect.right  = (first_rect.right < second_rect.right) ? first_rect.right : second_rect.right;
	}
	else	{						// ����
		client_rect.top    = (first_rect.top    > second_rect.top)    ? first_rect.top    : second_rect.top;
		client_rect.bottom = (first_rect.bottom < second_rect.bottom) ? first_rect.bottom : second_rect.bottom;
		client_rect.left   = first_rect.right;
		client_rect.right  = second_rect.left;
	}

	return client_rect;
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�Z�p���[�^�[�̏�Ԃ��擾
// ================================================================

bool C_dialog_separater::get_state_func(STATE* state)
{
	state->get_flag = false;
	if (_ctrl_list_first.empty() || _ctrl_list_second.empty())
		return false;

	ARRAY<C_window_base *>::iterator end;
	ARRAY<C_window_base *>::iterator itr;
	C_rect rect;

	// �R���g���[���͈͂̎n�_���W
	end = _ctrl_list_first.end();
	itr = _ctrl_list_first.begin();
	rect = (*itr)->get_rltv_rect();
	int ctrl_top_pos = (_hv_mod == false) ? rect.top : rect.left;
	itr++;
	for (; itr != end; ++itr)	{
		rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	if (ctrl_top_pos < rect.top)	{	ctrl_top_pos = rect.top;	}	}	// ����
		else					{	if (ctrl_top_pos < rect.left)	{	ctrl_top_pos = rect.left;	}	}	// ����
	}

	// �R���g���[���͈͂̏I�_���W
	end = _ctrl_list_second.end();
	itr = _ctrl_list_second.begin();
	rect = (*itr)->get_rltv_rect();
	int ctrl_end_pos = (_hv_mod == false) ? rect.bottom : rect.right;
	itr++;
	for (; itr != end; ++itr)	{
		rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	if (ctrl_end_pos > rect.bottom)	{	ctrl_end_pos = rect.bottom;	}	}	// ����
		else					{	if (ctrl_end_pos > rect.right)	{	ctrl_end_pos = rect.right;	}	}	// ����
	}

	// ���݂̃Z�p���[�^�[���W�ƃX�y�[�X
	rect = get_rect();
	if (_hv_mod == false)	{			// ����
		state->space = rect.height();	// �X�y�[�X�i���R���g���[��������R���g���[���̊Ԃ̃h�b�g�� �j
		state->now_pos = rect.top;		// ���݂̃Z�p���[�^�[���W
	}
	else	{							// ����
		state->space = rect.width();	// // �X�y�[�X�i���R���g���[��������R���g���[���̊Ԃ̃h�b�g�� �j
		state->now_pos = rect.left;		// ���݂̃Z�p���[�^�[���W
	}

	// �_�C�A���O�̃N���C�A���g�͈�
	HWND parent_h_wnd = (*_ctrl_list_first.begin())->get_parent_handle();		// �S�������͂�������A�ǂ̃R���g���[���̂��̂ł��ǂ��̂ł�
	GetClientRect(parent_h_wnd, &rect);
	state->dlg_top = 0;
	if (_hv_mod == false)	{			// ����
		state->dlg_end = rect.height();	// ���T�C�Y
	}
	else	{							// ����
		state->dlg_end = rect.width();	// ���T�C�Y
	}

	// �ғ��͈͂̎n�_���W
	state->move_top_pos = ctrl_top_pos + 16;

	// �ғ��͈͂̏I�_���W
	state->move_end_pos = ctrl_end_pos - 1 - state->space - 16;

	if (state->move_top_pos > state->move_end_pos)	{	return false;	}

	// ���݈ʒu�����ɕϊ�����
	now_to_percent();

	state->get_flag = true;
	return true;
}

bool C_dialog_separater::get_state()
{
	return get_state_func(&state);
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�}�E�X�q�b�g����
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
// �_�C�A���O�Z�p���[�^�[�F�}�E�X�J�[�\���ύX
// ================================================================
bool C_dialog_separater::change_mouse_cursor()
{
	HCURSOR hcursor = LoadCursor(NULL, _hv_mod ? IDC_SIZEWE : IDC_SIZENS);
	if (hcursor != GetCursor())	{	SetCursor(hcursor);		return true;	}
	return false;
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�R���g���[���̃T�C�Y�ύX
// ================================================================
void C_dialog_separater::change_control_size(int val, int space)
{
	// �ғ��͈͊O�Ȃ炱���Ń��^�[��
	if (state.get_flag == false || state.move_top_pos > val || val > state.move_end_pos)
		return;

	ARRAY<C_window_base *>::iterator end;
	ARRAY<C_window_base *>::iterator itr;
	C_rect rect;

	// ���R���g���[���̃T�C�Y�ύX
	end = _ctrl_list_first.end();
	for (itr = _ctrl_list_first.begin(); itr != end; ++itr)	{
		rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	rect.bottom = val;	}	// ����
		else					{	rect.right  = val;	}	// ����
		(*itr)->set_window_rect(rect);
	}

	// ���R���g���[���̃T�C�Y�ύX
	val += space;
	end = _ctrl_list_second.end();
	for (itr = _ctrl_list_second.begin(); itr != end; ++itr)	{
		C_rect rect = (*itr)->get_rltv_rect();
		if (_hv_mod == false)	{	rect.top  = val;	}	// ����
		else					{	rect.left = val;	}	// ����
		(*itr)->set_window_rect(rect);
	}
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�F�_�C�A���O�̃T�C�Y���ύX���ꂽ
// ================================================================
void C_dialog_separater::on_size()
{
	int percent = state.percent;
	if (get_state() == false)
		return;

	if (percent != -1)	{		// ����N�����̂� ini ��񂪖����̂� -1 �ɂȂ��Ă�͂�
		state.percent = percent;
		percent_to_now();
	}
	change_control_size(state.now_pos, state.space);
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�R���X�g���N�^
// ================================================================
Cdialog_separater_manager::Cdialog_separater_manager()
{
	clear();
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�N���A
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
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�Z�p���[�^�[�̊��蓖��
// ================================================================
void Cdialog_separater_manager::bind_separater(C_dialog_separater* separater, CTSTR& window_name, CTSTR& name)
{
	separater->set_name(window_name, name);
	separater_list.push_back(separater);
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�Fini �ɕۑ�
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
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�Fini ����ǂݍ��� 
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
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�}�E�X���q�b�g���Ă���Z�p���[�^�[�̃|�C���^�[���擾
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
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�}�E�X�J�[�\�����ړ�����
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
			ReleaseCapture();		// ���}�E�X�L���v�`���[�I��
			target_separater = NULL;
		}
	}
	return false;
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�}�E�X�J�[�\���̕ύX�ʒm
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
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�}�E�X�̍��{�^����������
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
			SetCapture(h_wnd);		// ���}�E�X�L���v�`���[�J�n
			return true;
		}
		else	{
			target_separater = NULL;
		}
	}
	return false;
}

// ****************************************************************
// �_�C�A���O�Z�p���[�^�[�}�l�[�W���[�F�_�C�A���O�̃T�C�Y���ύX���ꂽ
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
