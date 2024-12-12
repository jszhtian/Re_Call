#include	"tonawindow3_pch.h"
#include	"tonawindow3_dialog.h"
#include	"tonawindow3_control.h"

namespace NT3
{

// ****************************************************************
// �R���g���[���e���v���[�g�F�p�����[�^������
// ================================================================
void C_control_template::init_param(int x, int y, int w, int h, PTR class_atom, WORD control_id, DWORD style, DWORD ex_style, PTR text_atom)
{
	stream.clear();

	stream.push<DWORD>(0);
	stream.push<DWORD>(ex_style);
	stream.push<DWORD>(style);
	stream.push<short>(x);
	stream.push<short>(y);
	stream.push<short>(w);
	stream.push<short>(h);
	stream.push<DWORD>(control_id);
	push_atom(class_atom);
	push_atom(text_atom);
	stream.push<WORD>(0);	// creation_data_size
}

// ****************************************************************
// �R���g���[���e���v���[�g�F�A�g���Z�b�g
// ================================================================
void C_control_template::push_atom(PTR atom)
{
	if (atom == -1)	{
		stream.push<WORD>(0);
	}
	else if (atom <= 0xFFFF)	{
		stream.push<WORD>(0xFFFF);
		stream.push<WORD>((WORD)atom);
	}
	else	{
		stream.push_str_null(TSTR_to_WSTR((TCHAR *)atom));
	}
}

// ****************************************************************
// �R���g���[���e���v���[�g�F�f�[�^�̎擾
// ================================================================
LPDLGITEMTEMPLATE C_control_template::get()
{
	return (LPDLGITEMTEMPLATE)stream.get();
}

int C_control_template::get_size()
{
	return stream.size();
}

// ****************************************************************
// �R���g���[���F�R���X�g���N�^
// ================================================================
C_control::C_control()
{
	m_adjust_way = 7;		// ����
}

// ****************************************************************
// �R���g���[���F�f�X�g���N�^
// ================================================================
C_control::~C_control()
{
}

// ****************************************************************
// �R���g���[���F�쐬
//		name : �E�B���h�E�̖��O�iini �ۑ����Ɏg�p�j
//		caption : �E�B���h�E�^�C�g��
//		x : �E�B���h�E�ʒu�w���W�iCW_USEDEFAULT �Ńf�t�H���g�ʒu�j
//		y : �E�B���h�E�ʒu�x���W�iCW_USEDEFAULT �Ńf�t�H���g�ʒu�j
//		client_w : �N���C�A���g�̈�̑傫���w
//		client_h : �N���C�A���g�̈�̑傫���x
//		style : �E�B���h�E�X�^�C��
//			LVS_ICON : �A�C�R��
//			LVS_LIST : ���X�g�r���[
//			LVS_REPORT : ���|�[�g
//		ex_style : �g���E�B���h�E�X�^�C��
//		is_limit_min : �E�B���h�E�̍ŏ���`�𐧌�
//		is_attach : ���̃E�B���h�E�ɃA�^�b�`
//		h_parent_wnd : �e�E�B���h�E�n���h��
//		adjust_way : �e�ɂ����������i�ݷ� �̕����Ŏw��j
// ================================================================
bool C_control::create(CTSTR& name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD style, DWORD ex_style, bool is_limit_min, bool is_attach, HWND h_parent_wnd)
{
	if (m_class_name.empty())
		return false;

	return C_window_base::create(name, m_class_name, caption, x, y, client_w, client_h, is_menu, style, ex_style, is_limit_min, is_attach, h_parent_wnd);
}

// ****************************************************************
// �R���g���[���F�����̃R���g���[���Ɍ��т���
//		name : �E�B���h�E�̖��O�iini �ɕۑ�����Ƃ��Ɏg�p�j
//		adjust_way : �e�E�B���h�E�ɂ����������i1-9: �e���L�[�����j
// ================================================================
bool C_control::bind(HWND h_dlg, int ctrl_id, CTSTR& name, int adjust_way, int group_no)
{
	// �����o�̏����ݒ�
	m_window_name = name;
	m_adjust_way = adjust_way;
	m_group_no = group_no;

	m_h_wnd = GetDlgItem(h_dlg, ctrl_id);

	// �E�B���h�E�n���h���ƃE�B���h�E�����т���
	C_window_base::bind(m_h_wnd);

	return true;
}

// ****************************************************************
// �R���g���[���F�e�L�X�g�̃N���A
// ================================================================
void C_control::clear_text()
{
	send_message(WM_SETTEXT, 0, (LPARAM)_T(""));
}

// ****************************************************************
// �R���g���[���F�e�L�X�g�̐ݒ�
// ================================================================
void C_control::set_text(CTSTR& str)
{
	// ���s�� 0x0a �� 0x0d 0x0a �ɕϊ�
	TSTR str_a, str_da;
	str_a += 0x0a;
	str_da += 0x0d;
	str_da += 0x0a;
	TSTR new_str = str_replace_ret(str, str_a, str_da);

	C_window_base::set_text(new_str);
}

// ****************************************************************
// �R���g���[���F�e�L�X�g�̐ݒ��A�e�L�X�g�T�C�Y�ɍ��킹�ăR���g���[���̕��𒲐�����
// ================================================================
void C_control::set_text_adjust_width(CTSTR& str, int rep_width, HFONT font)
{
	set_text(str);
	adjust_width_from_text(rep_width, font);
}

// ****************************************************************
// �R���g���[���F�e�L�X�g�̎擾
// ================================================================
TSTR C_control::get_text()
{
	ARRAY<TCHAR> t_str;

	int len = (int)send_message(WM_GETTEXTLENGTH, 0, 0);
	t_str.resize(len + 16);
	(int)send_message(WM_GETTEXT, len + 16, (LPARAM)t_str.get());

	TSTR str = t_str.get();

	// ���s�� 0x0a �� 0x0d 0x0a �ɕϊ�
	TSTR str_a, str_da;
	str_a += 0x0a;
	str_da += 0x0d;
	str_da += 0x0a;
	str_replace(str, str_da, str_a);

	return str;
}

// ****************************************************************
// �R���g���[���F�r�b�g�}�b�v�̐ݒ�
// ================================================================
void C_control::set_bitmap(HBITMAP h_bitmap)
{
	HBITMAP h_old_bitmap = (HBITMAP)send_message(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)h_bitmap);
	if (h_old_bitmap)	{

		// �Â��r�b�g�}�b�v���Ԃ��Ă���
		// XP �X�^�C���� 32bit �r�b�g�}�b�v���w�肵���ꍇ�A
		// �����������Ȃ��ƃ��������[�N�ɂȂ�I�I�I�I�I
		// �R�̂悤�Ȗ{���̘b�B
		::DeleteObject(h_old_bitmap);
	}
}

// ****************************************************************
// �R���g���[���F�e�L�X�g�T�C�Y�ɍ��킹�ăR���g���[���̕����擾����
// ================================================================
int C_control::get_adjust_width_from_text(int rep_width, HFONT h_font)
{
	// font ���w�肳��Ă��Ȃ��ꍇ�̓f�t�H���g�� GUI �t�H���g���g��
	if (h_font == NULL)	{
		h_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}

	TSTR str = get_text();
	HDC h_dc = GetDC(m_h_wnd);
	HFONT old_font = (HFONT)SelectObject(h_dc, h_font);
	C_size size;
	GetTextExtentPoint32(h_dc, str.c_str(), (int)str.size(), &size);
	SelectObject(h_dc, old_font);	// �t�H���g�����ɖ߂�
	ReleaseDC(m_h_wnd, h_dc);
	return size.cx + rep_width;
}

// ****************************************************************
// �R���g���[���F�e�L�X�g�T�C�Y�ɍ��킹�ăR���g���[���̕��𒲐�����
// ================================================================
int C_control::adjust_width_from_text(int rep_width, HFONT font)
{
	int width = get_adjust_width_from_text(rep_width, font);
	C_rect rect = get_window_rect();
	set_window_size(C_size(width, rect.height()));
	set_def_rect_from_now_size();
	return width;
}

// ****************************************************************
// �{�^���F�X�^�C����ݒ�
// ================================================================
void Cc_button::set_button_style(PTR style, bool redraw)
{
	send_message(BM_SETSTYLE, style, redraw ? TRUE : FALSE);
}

// ****************************************************************
// �{�^���F�����������Ă��邩�H
// ================================================================
bool Cc_button::is_pushed()
{
	return (send_message(BM_GETSTATE, 0, 0) & BST_PUSHED) ? true : false;
}

// ****************************************************************
// ���W�I�{�^���Fini �ɕۑ�
// ================================================================
void Cc_radio::save_ini(CTSTR& project_name)
{
	save_ini_bool(_T("check"), get_check(), project_name);
}

// ****************************************************************
// ���W�I�{�^���Fini ����ǂݍ���
// ================================================================
void Cc_radio::load_ini(bool def, CTSTR& project_name)
{
	set_check(load_ini_bool(_T("check"), def, project_name));
}

// ****************************************************************
// ���W�I�{�^���F�`�F�b�N�̐ݒ�
// ================================================================
void Cc_radio::set_check(bool check)
{
	send_message(BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}

// ****************************************************************
// ���W�I�{�^���F�`�F�b�N�̎擾
// ================================================================
bool Cc_radio::get_check()
{
	return send_message(BM_GETCHECK, 0, 0) & BST_CHECKED;
}

// ****************************************************************
// �`�F�b�N�{�b�N�X�Fini �ɕۑ�
// ================================================================
void Cc_check::save_ini(CTSTR& project_name)
{
	save_ini_bool(_T("check"), get_check(), project_name);
}

// ****************************************************************
// �`�F�b�N�{�b�N�X�Fini ����ǂݍ���
// ================================================================
void Cc_check::load_ini(bool def, CTSTR& project_name)
{
	set_check(load_ini_bool(_T("check"), def, project_name));
}

// ****************************************************************
// �`�F�b�N�{�b�N�X�F�`�F�b�N�̐ݒ�
// ================================================================
void Cc_check::set_check(bool check)
{
	send_message(BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}

// ****************************************************************
// �`�F�b�N�{�b�N�X�F�`�F�b�N�̎擾
// ================================================================
bool Cc_check::get_check()
{
	return send_message(BM_GETCHECK, 0, 0) & BST_CHECKED;
}

// ****************************************************************
// �G�f�B�b�g�{�b�N�X
// ================================================================
LRESULT Cc_edit::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{

		// �c���c������
		case WM_DROPFILES:
			set_text(get_droped_link_path(wp));
			break;

		// �_�C�A���O�L�[������
		case WM_GETDLGCODE:
			if (lp != NULL)	{
				MSG* dlg_msg = (MSG *)lp;
				// �G���^�[�L�[�̏�����������
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_RETURN)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
				// �G�X�P�[�v�L�[�̏�����������
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_ESCAPE)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
				// ��L�[�̏�����������
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_UP)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
				// ���L�[�̏�����������
				if (dlg_msg->message == WM_KEYDOWN && dlg_msg->wParam == VK_DOWN)	{
					return C_control::window_proc(msg, wp, lp) | DLGC_WANTALLKEYS;
				}
			}
			break;
	}

	return C_control::window_proc(msg, wp, lp);
}

// ****************************************************************
// �G�f�B�b�g�{�b�N�X�F�I��
// ================================================================
void Cc_edit::set_sel(int start, int end)
{
	send_message(EM_SETSEL, start, end);
}

void Cc_edit::set_sel_all()
{
	send_message(EM_SETSEL, 0, -1);
}

// ****************************************************************
// �G�f�B�b�g�{�b�N�X�Fini �ɕۑ�
// ================================================================
void Cc_edit::save_ini(CTSTR& project_name)
{
	save_ini_str(_T("text"), get_text(), project_name);
}

// ****************************************************************
// �G�f�B�b�g�{�b�N�X�Fini ����ǂݍ���
// ================================================================
void Cc_edit::load_ini(CTSTR& def, CTSTR& project_name)
{
	set_text(load_ini_str(_T("text"), def, project_name));
}

// ****************************************************************
// �X�s���F�A������G�f�B�b�g��ݒ�
// ================================================================
void Cc_spin::set_edit(HWND h_edit)
{
	send_message(UDM_SETBUDDY, (WPARAM)h_edit, 0);
}

// ****************************************************************
// �X�s���F�G�f�B�b�g�̒l�͈̔͂�ݒ�
// ================================================================
void Cc_spin::set_edit_range(int min, int max)
{
	send_message(UDM_SETRANGE32, min, max);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�N���A
// ================================================================
void Cc_listbox::clear()
{
	send_message(LB_RESETCONTENT, 0, 0);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̒ǉ�
//		�߂�l�F�ǉ����ꂽ�A�C�e���̃C���f�b�N�X
// ================================================================
int Cc_listbox::add_item(CTSTR &str)
{
	return (int)send_message(LB_ADDSTRING, 0, (LPARAM)str.c_str());
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̒ǉ��i�ʒu���w��j
//		pos�F�}������ʒu�i-1 �Ŗ����j
//		�߂�l�F�ǉ����ꂽ�A�C�e���̃C���f�b�N�X
// ================================================================
int Cc_listbox::insert_item(int pos, CTSTR &str)
{
	return (int)send_message(LB_INSERTSTRING, pos, (LPARAM)str.c_str());
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̍폜
//		�߂�l�F�ǉ����ꂽ�A�C�e���̃C���f�b�N�X
// ================================================================
void Cc_listbox::delete_item(int pos)
{
	send_message(LB_DELETESTRING, pos, 0);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e�������擾
//		�߂�l�F�A�C�e����
// ================================================================
int Cc_listbox::get_item_cnt()
{
	return (int)send_message(LB_GETCOUNT, 0, 0);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̎擾
// ================================================================
TSTR Cc_listbox::get_item_text(int item_no)
{
	TSTR str;

	int len = (int)send_message(LB_GETTEXTLEN, item_no, 0);
	if (len > 0){
		ARRAY<TCHAR> moji(len + 1);		// �z��ɕ�������擾
		send_message(LB_GETTEXT, item_no, (LPARAM)moji.get());
		str = moji.get();
	}

	return str;
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̑I����Ԃ�ݒ�
// ================================================================
void Cc_listbox::set_item_select_state(int item_no, bool selected)
{
	send_message(LB_SETSEL, selected ? TRUE : FALSE, item_no);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�S�ẴA�C�e���̑I����Ԃ�ݒ�
// ================================================================
void Cc_listbox::set_item_select_state_all(bool selected)
{
	int item_cnt = get_item_cnt();
	for (int i = 0; i < item_cnt; i++)
		send_message(LB_SETSEL, selected ? TRUE : FALSE, i);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̑I����Ԃ��擾
// ================================================================
bool Cc_listbox::get_item_select_state(int item_no)
{
	return send_message(LB_GETSEL, item_no, 0) ? true : false;
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e����I��
// ================================================================
void Cc_listbox::set_selected_item_no(int item_no)
{
	send_message(LB_SETCURSEL, item_no, 0);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�I������Ă���A�C�e���ԍ����擾
//		�߂�l�F�I������Ă���A�C�e���̃C���f�b�N�X�i�G���[�FLB_ERR�j
// ================================================================
int Cc_listbox::get_selected_item_no()
{
	return (int)send_message(LB_GETCURSEL, 0, 0);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�I������Ă���A�C�e�������擾
// ================================================================
int Cc_listbox::get_selected_item_cnt()
{
	return (int)send_message(LB_GETSELCOUNT, 0, 0);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�����X�N���[���o�[�ݒ�
//		�����X�N���[���o�[�̉ғ��͈͂�ݒ肵�܂��B
// ================================================================
void Cc_listbox::set_horizontal_scroll_bar()
{
	int item_cnt = get_item_cnt();
	int max_width = 0;

	if (item_cnt > 0)	{
		HDC dc = GetDC(m_h_wnd);
		for (int i = 0; i < item_cnt; i++)	{
			TSTR str = get_item_text(i);

			// ������̃s�N�Z�������擾
			SIZE size;
			GetTextExtentPoint32(dc, str.c_str(), (int)str.size(), &size);
			if (size.cx > max_width)	{	max_width = size.cx;		}
		}
		ReleaseDC(m_h_wnd, dc);
	}

	send_message(LB_SETHORIZONTALEXTENT, max_width, 0);
}

// ****************************************************************
// �R���{�{�b�N�X�F���݂̃A�C�e�������擾
// ================================================================
int Cc_combobox::get_item_cnt()
{
	return (int)send_message(CB_GETCOUNT, 0, 0);
}

// ****************************************************************
// �R���{�{�b�N�X�F���݂̑I��ԍ���ݒ�
// ================================================================
void Cc_combobox::set_sel_no(int sel_no)
{
	send_message(CB_SETCURSEL, sel_no, 0);
}

// ****************************************************************
// �R���{�{�b�N�X�F���݂̑I��ԍ����擾
//		�G���[�FCB_ERR
// ================================================================
int Cc_combobox::get_sel_no()
{
	return (int)send_message(CB_GETCURSEL, 0, 0);
}

// ****************************************************************
// �R���{�{�b�N�X�F�A�C�e���̕�����̎擾
// ================================================================
TSTR Cc_combobox::get_item_text(int item_no)
{
	int len = (int)send_message(CB_GETLBTEXTLEN, item_no, 0);

	// �z��ŕ�������󂯎��
	ARRAY<TCHAR> str(len + 16);		// 16: �\��
	send_message(CB_GETLBTEXT, item_no, (LPARAM)str.get());

	// ������ɕϊ�
	return TSTR(str.get());
}

// ****************************************************************
// �R���{�{�b�N�X�F�A�C�e���̕�����̐ݒ�
// ================================================================
void Cc_combobox::set_item_text(int item_no, CTSTR& text)
{
	insert_item(text, item_no, false);
	delete_item(item_no + 1);
}

// ****************************************************************
// �R���{�{�b�N�X�F�I�𒆂̃A�C�e���̕�����̎擾
// ================================================================
TSTR Cc_combobox::get_sel_item_text()
{
	int sel_no = get_sel_no();
	if (sel_no == CB_ERR)
		return _T("");

	return get_item_text(sel_no);
}

// ****************************************************************
// �R���{�{�b�N�X�F�A�C�e�����X�g���N���A
// ================================================================
void Cc_combobox::clear_item_list()
{
	int item_cnt = get_item_cnt();
	for (int i = 0; i < item_cnt; i++)	{
		delete_item(0);
	}
}

// ****************************************************************
// �R���{�{�b�N�X�F�A�C�e����}��
// ================================================================
void Cc_combobox::insert_item(CTSTR& text, int index, bool erase_same)
{
	int item_no;

	// ����̍��ڂ��폜
	if (erase_same)	{
		while ((item_no = find_item(text)) >= 0)
			delete_item(item_no);
	}

	// �A�C�e���̒ǉ�
	send_message(CB_INSERTSTRING, index, (LPARAM)text.c_str());
}

// ****************************************************************
// �R���{�{�b�N�X�F�擪�ɃA�C�e����ǉ�
// ================================================================
void Cc_combobox::insert_item_to_top(CTSTR& text, bool erase_same)
{
	int item_no;

	// ����̍��ڂ��폜
	if (erase_same)	{
		while ((item_no = find_item(text)) >= 0)
			delete_item(item_no);
	}

	// �A�C�e���̒ǉ�
	send_message(CB_INSERTSTRING, 0, (LPARAM)text.c_str());
}

// ****************************************************************
// �R���{�{�b�N�X�F�����ɃA�C�e����ǉ�
// ================================================================
void Cc_combobox::insert_item_to_bottom(CTSTR& text, bool erase_same)
{
	int item_no;

	// ����̍��ڂ��폜
	if (erase_same)	{
		while ((item_no = find_item(text)) >= 0)
			delete_item(item_no);
	}

	item_no = std::max(get_item_cnt(), 0);
	send_message(CB_INSERTSTRING, item_no, (LPARAM)text.c_str());
}

// ****************************************************************
// �R���{�{�b�N�X�F������̌���
// ================================================================
int Cc_combobox::find_item(CTSTR& text)
{
	int item_no = (int)send_message(CB_FINDSTRING, -1, (LPARAM)text.c_str());
	if (item_no == CB_ERR)
		return -1;

	return item_no;
}

// ****************************************************************
// �R���{�{�b�N�X�F���ڂ̍폜
// ================================================================
void Cc_combobox::delete_item(int item_no)
{
	send_message(CB_DELETESTRING, item_no, 0);
}

// ****************************************************************
// �R���{�{�b�N�X�Fini �ɕۑ�
// ================================================================
void Cc_combobox::save_ini_sel_no(CTSTR& project_name)
{
	save_ini_int(_T("sel_no"), get_sel_no(), project_name);
}

void Cc_combobox::save_ini_drop(CTSTR& project_name)
{
	// �A�C�e����
	int item_cnt = get_item_cnt();
	save_ini_int(_T("item_count"), item_cnt);
	// �A�C�e��
	for (int i = 0; i < item_cnt; i++)
		save_ini_str(_T("item_") + tostr(i), get_item_text(i), project_name);
	// �G�f�B�b�g
	save_ini_str(_T("edit"), get_text(), project_name);
}

// ****************************************************************
// �R���{�{�b�N�X�Fini ����ǂݍ���
// ================================================================
void Cc_combobox::load_ini_sel_no(int def_sel_no, CTSTR& project_name)
{
	set_sel_no(load_ini_int(_T("sel_no"), def_sel_no, project_name));
}

void Cc_combobox::load_ini_drop(CTSTR& project_name)
{
	// �A�C�e����
	int item_cnt = limit(0, load_ini_int(_T("item_count"), 0), item_cnt_max);

	// �A�C�e��
	for (int i = 0; i < item_cnt; i++)
		insert_item_to_bottom(load_ini_str(tostr(i), _T(""), project_name), false);
	// �G�f�B�b�g
	set_text(load_ini_str(_T("edit"), _T(""), project_name));
}

// ****************************************************************
// �c���[�r���[�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT Cc_treeview::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
//	switch (msg)	{
//		default:
//			break;
//	}

	return C_control::window_proc(msg, wp, lp);
}

// ****************************************************************
// �c���[�r���[�F�N���A
// ================================================================
void Cc_treeview::clear()
{
	send_message(TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}

// ****************************************************************
// �c���[�r���[�F�A�C�e���̒ǉ�
// ================================================================
HTREEITEM Cc_treeview::add_item(HTREEITEM h_parent, HTREEITEM h_pos, CTSTR& text)
{
	TV_INSERTSTRUCT tvi;
	ZeroMemory(&tvi, sizeof(tvi));

	tvi.hParent = h_parent;
	tvi.hInsertAfter = h_pos;
	tvi.item.mask = TVIF_TEXT;
	tvi.item.pszText = const_cast<LPWSTR>(text.c_str());
	return (HTREEITEM)send_message(TVM_INSERTITEM, 0, (LPARAM)&tvi);
}

// ****************************************************************
// �X�N���[���o�[�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT Cc_scroll::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_VSCROLL:
		case WM_HSCROLL:
			scroll_natural(wp);
			break;
	}

	return C_control::window_proc(msg, wp, lp);
}

// ****************************************************************
// �X�N���[���o�[�F�͈͂̐ݒ�
// ================================================================
void Cc_scroll::set_scroll_info(int min, int max, int page_size)
{
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE;
	info.nMin = min;
	info.nMax = max;
	info.nPage = page_size;
	send_message(SBM_SETSCROLLINFO, TRUE, LPARAM(&info));
}

// ****************************************************************
// �X�N���[���o�[�F���݈ʒu�̐ݒ�
// ================================================================
void Cc_scroll::set_scroll_pos(int pos)
{
	send_message(SBM_SETPOS, WPARAM(pos), LPARAM(TRUE));
}

// ****************************************************************
// �X�N���[���o�[�F���݈ʒu�̎擾
// ================================================================
int Cc_scroll::get_scroll_pos()
{
	return int(send_message(SBM_GETPOS, 0, 0));
}

// ****************************************************************
// �X�N���[���o�[�F���R�ȓ���
// ================================================================
int Cc_scroll::scroll_natural(WPARAM wp)
{
	// ���擾
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	send_message(SBM_GETSCROLLINFO, 0, LPARAM(&info));

	// �ŏ��l�ƍő�l
	int Min = info.nMin;
	int Max = info.nMax - info.nMin - info.nPage + 1;

	// ����
	if (false);
	else if (LOWORD(wp) == SB_ENDSCROLL)	{
		return info.nPos;
	}
	else if (LOWORD(wp) == SB_LEFT)	{				// ���̒[
		info.nPos = info.nMin;
	}
	else if (LOWORD(wp) == SB_RIGHT)	{			// �E�̒[
		info.nPos = info.nMax - info.nMin - info.nPage + 1;
	}
	else if (LOWORD(wp) == SB_LINELEFT)	{			// ��
		info.nPos = limit(Min, info.nPos - 1, Max);
	}
	else if (LOWORD(wp) == SB_LINERIGHT)	{		// ��
		info.nPos = limit(Min, info.nPos + 1, Max);
	}
	else if (LOWORD(wp) == SB_PAGELEFT)	{			// ��[]
		info.nPos = limit(Min, int(info.nPos - info.nPage), Max);
	}
	else if (LOWORD(wp) == SB_PAGERIGHT)	{		// []��
		info.nPos = limit(Min, int(info.nPos + info.nPage), Max);
	}
	else if (LOWORD(wp) == SB_THUMBPOSITION)	{	// �h���b�O��
		info.nPos = (short)HIWORD(wp);
	}
	else if (LOWORD(wp) == SB_THUMBTRACK)	{		// �h���b�O�I��
		info.nPos = (short)HIWORD(wp);
	}

	// �ʒu���X�V
	send_message(SBM_SETPOS, (WPARAM)info.nPos, (LPARAM)TRUE);

	return info.nPos;
}

// ****************************************************************
// �X���C�_�[�F���݈ʒu�̐ݒ�
// ================================================================
void Cc_slider::set_slider_pos(int pos)
{
	send_message(TBM_SETPOS, (WPARAM)TRUE, (LPARAM)pos);
}

// ****************************************************************
// �X���C�_�[�F���݈ʒu�̎擾
// ================================================================
int Cc_slider::get_slider_pos()
{
	return (int)send_message(TBM_GETPOS, 0, 0);
}

// ****************************************************************
// �X���C�_�[�F�͈͂��w��
// ================================================================
void Cc_slider::set_slider_range(int min, int max)
{
	send_message(TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(min, max));
}

// ****************************************************************
// �v���O���X�o�[�F�N���A
// ================================================================
void Cc_progress::clear()
{
	set_prg(0);
}

// ****************************************************************
// �v���O���X�o�[�F�ő�l��ݒ�
// ================================================================
void Cc_progress::set_max_prg(int max_prg)
{
	send_message(PBM_SETRANGE32, 0, (LPARAM)max_prg);
}

// ****************************************************************
// �v���O���X�o�[�F���݂̒l��ݒ�
// ================================================================
void Cc_progress::set_prg(int prg)
{
	send_message(PBM_SETPOS, (WPARAM)prg, 0);
}

// ****************************************************************
// �^�u�R���g���[���F�^�u�̒ǉ�
// ================================================================
void Cc_tab::add_item(CTSTR& str)
{
	TCHAR t_str[MAX_PATH];
	_tcscpy_s(t_str, MAX_PATH, str.c_str());

	TCITEM tc_item;
	tc_item.mask = TCIF_TEXT;
	tc_item.pszText = t_str;
	tc_item.cchTextMax = MAX_PATH;

	send_message(TCM_INSERTITEM, (WPARAM)tab_cnt++, (LPARAM)&tc_item);
}

// ****************************************************************
// �^�u�R���g���[���F�^�u�̐����擾
// ================================================================
int Cc_tab::get_item_cnt()
{
	return (int)send_message(TCM_GETITEMCOUNT, 0, 0);
}

// ****************************************************************
// �^�u�R���g���[���F�^�u�̕�����ݒ�
// ================================================================
void Cc_tab::set_item_text(int item_no, CTSTR& str)
{
	int cnt = get_item_cnt();
	if (!(0 <= item_no && item_no < cnt))	{	return;	}
	if (Cc_tab::get_item_text(item_no) == str)	{	return;	}
	TC_ITEM tc;
	ZeroMemory(&tc, sizeof(tc));
	tc.mask = TCIF_TEXT;
	TSTR tmp = str;
	tc.pszText = (TCHAR *)tmp.c_str();
	send_message(TCM_SETITEM, item_no, (LPARAM)&tc);
}

// ****************************************************************
// �^�u�R���g���[���F�^�u�̕�����擾
// ================================================================
TSTR Cc_tab::get_item_text(int item_no)
{
	int cnt = get_item_cnt();
	if (!(0 <= item_no && item_no < cnt))	{	return TSTR();	}
	TC_ITEM tc;
	ZeroMemory(&tc, sizeof(tc));
	tc.mask = TCIF_TEXT;
	TCHAR str[1024];
	tc.cchTextMax = 1024;
	tc.pszText = str;
	send_message(TCM_GETITEM, item_no, (LPARAM)&tc);
	return TSTR(str);
}

// ****************************************************************
// �^�u�R���g���[���F�I������Ă���^�u��ݒ�
// ================================================================
void Cc_tab::set_selected_item_no(int item_no)
{
	send_message(TCM_SETCURSEL, item_no, 0);
}

// ****************************************************************
// �^�u�R���g���[���F�I������Ă���^�u���擾
// ================================================================
int Cc_tab::get_selected_item_no()
{
	return (int)send_message(TCM_GETCURSEL, 0, 0);
}

// ****************************************************************
// �^�u�R���g���[���F�\���G���A���擾
// ================================================================
C_rect Cc_tab::get_display_rect()
{
	C_rect rect = get_window_rect();
	send_message(TCM_ADJUSTRECT, FALSE, (LPARAM)&rect);
	return rect;
}

// ****************************************************************
// �s�N�`���R���g���[���F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT Cc_picture::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	return C_control::window_proc(msg, wp, lp);
}

};
