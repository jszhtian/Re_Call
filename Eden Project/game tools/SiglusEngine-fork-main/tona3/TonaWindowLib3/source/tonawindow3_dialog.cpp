#include	"tonawindow3_pch.h"
#include	"tonawindow3_control.h"
#include	"tonawindow3_dialog.h"

namespace NT3
{

#define		DLGTPLT_OFFSET_ITEM_CNT		(sizeof(WORD) + sizeof(WORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD))
#define		DLGTPLT_OFFSET_WIDTH		(sizeof(WORD) + sizeof(WORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(WORD) + sizeof(short) + sizeof(short))
#define		DLGTPLT_OFFSET_HEIGHT		(DLGTPLT_OFFSET_WIDTH + sizeof(short))

// ****************************************************************
// �_�C�A���O�e���v���[�g�F�p�����[�^������
// ================================================================
//		menu_atom: ���j���[ID �������� ��������w��
//		font_name: �t�H���g���Bstyle �� DS_SETFONT ���w�肷��K�v������B
// ================================================================
void C_dialog_template::init_param(int x, int y, int w, int h, CTSTR& title, DWORD style, DWORD ex_style, PTR menu_atom, WORD font_size, CTSTR& font_name)
{
	// �e���v���[�g�쐬�p�̃X�g���[�����쐬
	m_stream.clear();
	m_stream.push<WORD>(1);			// �_�C�A���O�e���v���[�g�̃o�[�W�����i�K�� 1 ���w��j
	m_stream.push<WORD>(0xFFFF);	// �g���ł̃_�C�A���O�e���v���[�g�ł��邱�Ƃ��w��i�K�� 0xFFFF ���w��j
	m_stream.push<DWORD>(0);		// �w���v ID ���w��i�������FWM_HELP ���Q�Ɓj
	m_stream.push<DWORD>(ex_style);	// �g���E�B���h�E�X�^�C��
	m_stream.push<DWORD>(style);	// �E�B���h�E�X�^�C��
	m_stream.push<WORD>(0);			// �R���g���[�����i���l�j
	m_stream.push<short>(x);
	m_stream.push<short>(y);
	m_stream.push<short>(w);
	m_stream.push<short>(h);
	push_atom(menu_atom);
	m_stream.push<WORD>(0);			// �_�C�A���O�Ȃ̂ŃE�B���h�E�N���X�� 0
	m_stream.push_str_null(TSTR_to_WSTR(title));
	if (style & (DS_SETFONT | DS_SHELLFONT))	{
		m_stream.push<WORD>(font_size);
		m_stream.push<WORD>(FW_NORMAL);
		m_stream.push<BYTE>(FALSE);
		m_stream.push<BYTE>(DEFAULT_CHARSET);
		m_stream.push_str_null(TSTR_to_WSTR(font_name));
	}
}

// ****************************************************************
// �_�C�A���O�e���v���[�g�F�T�C�Y�ݒ�
// ================================================================
void C_dialog_template::set_size(int w, int h)
{
	if (m_stream.empty())
		return;

	*(short *)&m_stream[DLGTPLT_OFFSET_WIDTH]  = w;
	*(short *)&m_stream[DLGTPLT_OFFSET_HEIGHT] = h;
}

// ****************************************************************
// �_�C�A���O�e���v���[�g�F�A�C�e���ǉ�
// ================================================================
void C_dialog_template::add_control(int x, int y, int w, int h, PTR class_atom, WORD control_id, DWORD style, DWORD ex_style, PTR text_atom)
{
	if (m_stream.empty())
		return;

	// �X�g���[���̃T�C�Y����p�f�B���O��}��
	int padding = m_stream.size() % 4;
	for (int i = 0; i < padding; i++)
		m_stream.push<BYTE>(0);

	// �R���g���[���e���v���[�g���쐬
	C_control_template ctrl_tplt;
	ctrl_tplt.init_param(x, y, w, h, class_atom, control_id, style, ex_style, text_atom);
	// �R���g���[���e���v���[�g��ǉ�
	m_stream.push(ctrl_tplt.get(), ctrl_tplt.get_size());

	// �R���g���[�����𑝂₷
	(*(WORD *)&m_stream[DLGTPLT_OFFSET_ITEM_CNT]) ++;
}

// ****************************************************************
// �_�C�A���O�e���v���[�g�F�A�g���Z�b�g
// ================================================================
void C_dialog_template::push_atom(PTR atom)
{
	if (atom == -1)	{
		m_stream.push<WORD>(0);
	}
	else if (atom <= 0xFFFF)	{
		m_stream.push<WORD>(0xFFFF);
		m_stream.push<WORD>((WORD)atom);
	}
	else	{
		m_stream.push_str_null(TSTR_to_WSTR((TCHAR *)atom));
	}
}

// ****************************************************************
// �_�C�A���O�e���v���[�g�F�f�[�^�̎擾
// ================================================================
DLGTEMPLATE* C_dialog_template::get()
{
	return (DLGTEMPLATE *)m_stream.get();
}

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_dialog::C_dialog()
{
}

// ****************************************************************
// �f�X�g���N�^
// ================================================================
C_dialog::~C_dialog()
{
}

// ****************************************************************
// �_�C�A���O�쐬
//		dialog_id : �_�C�A���O�̃��\�[�X�h�c
//		name : �E�B���h�E���iini �ۑ����Ɏg�p�j
//		is_limit_min : �E�B���h�E�̍ŏ���`�𐧌�
//		is_attach : ���̃E�B���h�E�ɃA�^�b�`
//		h_parent_wnd : �e�E�B���h�E�n���h��
// ================================================================
bool C_dialog::create(PTR dialog_id, CTSTR& name, bool is_limit_min, bool is_attach, HWND h_parent_wnd, int adjust_way)
{
	if (m_h_wnd != NULL)
		return true;

	// �_�C�A���O�̖��O
	m_window_name = name;
	// �E�B���h�E�̍ŏ���`�𐧌�
	m_is_limit_window_rect_min = is_limit_min;
	// ���̃E�B���h�E�ɃA�^�b�`
	m_is_attach_window = is_attach;
	// �e�ւ̂�������
	m_adjust_way = adjust_way;

	// �_�C�A���O�̍\�z
	m_h_wnd = ::CreateDialogParam(G_app.h_inst, (LPCTSTR)dialog_id, h_parent_wnd, (DLGPROC)window_proc_base, (LPARAM)this);
	// �\�z�Ɏ��s
	if (m_h_wnd == NULL)	{
		set_last_error_win(_T("CreateDialogParam"));
		return false;
	}

	// ���̃E�B���h�E�ɃA�^�b�`
	if (m_is_attach_window)
		m_attach.h_wnd_list.push_back(m_h_wnd);

	// IME �𖳌��ɂ���
//	ImmAssociateContext(h_wnd, NULL);

	return true;
}

// ****************************************************************
// �_�C�A���O�쐬
//		dialog_id : �_�C�A���O�̃��\�[�X�h�c
//		name : �E�B���h�E���iini �ۑ����Ɏg�p�j
//		is_limit_min : �E�B���h�E�̍ŏ���`�𐧌�
//		is_attach : ���̃E�B���h�E�ɃA�^�b�`
//		h_parent_wnd : �e�E�B���h�E�n���h��
// ================================================================
bool C_dialog::create_indirect(C_dialog_template* dlg_tpl, CTSTR& name, bool is_limit_min, bool is_attach, HWND h_parent_wnd, int adjust_way)
{
	if (m_h_wnd != NULL)
		return true;

	// �_�C�A���O�̖��O
	m_window_name = name;
	// �E�B���h�E�̍ŏ���`�𐧌�
	m_is_limit_window_rect_min = is_limit_min;
	// ���̃E�B���h�E�ɃA�^�b�`
	m_is_attach_window = is_attach;
	// �e�ւ̂�������
	m_adjust_way = adjust_way;

	// �_�C�A���O�̍\�z
	m_h_wnd = ::CreateDialogIndirectParam(G_app.h_inst, dlg_tpl->get(), h_parent_wnd, (DLGPROC)window_proc_base, (LPARAM)this);

	// �\�z�Ɏ��s
	if (m_h_wnd == NULL)	{
		set_last_error_win(_T("CreateDialogParam"));
		return false;
	}

	// ���̃E�B���h�E�ɃA�^�b�`
	if (m_is_attach_window)
		m_attach.h_wnd_list.push_back(m_h_wnd);

	// IME �𖳌��ɂ���
//	ImmAssociateContext(h_wnd, NULL);

	return true;
}

// ****************************************************************
// �R���g���[�������т���
// ================================================================
void C_dialog::bind_control(C_control* ctrl, int ctrl_id, CTSTR& name, int adjust_way, int group_no)
{
	ctrl->bind(m_h_wnd, ctrl_id, name, adjust_way, group_no);
	m_child_list.push_back(ctrl);
}

// ****************************************************************
// �`���C���h���X�g�ɒǉ�
// ================================================================
void C_dialog::add_child(C_window_base* p_wnd, int adjust_way, int group_no)
{
	p_wnd->set_adjust_way(adjust_way);
	p_wnd->set_group_no(group_no);
	m_child_list.push_back(p_wnd);
}

// ****************************************************************
// �E�B���h�E�v���V�[�W���i�v���j
// ================================================================
void C_dialog::window_proc_pre(UINT msg, WPARAM wp, LPARAM lp)
{
	C_window_base::window_proc_pre(msg, wp, lp);

	switch (msg)	{
		case WM_INITDIALOG:		on_init_dialog_pre();		break;
	}
}

// ****************************************************************
// �_�C�A���O�����ꂽ�i�v���j
// ================================================================
void C_dialog::on_init_dialog_pre()
{
	// �E�B���h�E��`���擾
	m_def_window_rect = get_window_rect();
	// �N���C�A���g��`���擾
	m_def_client_rect = get_client_rect();
	// �E�B���h�E��o�^����
	G_app.window_list.push_back(m_h_wnd);
}

// ****************************************************************
// �E�B���h�E�v���V�[�W��
// ================================================================
LRESULT	C_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_DESTROY:		if (!on_destroy())				return FALSE;	break;
		case WM_SIZE:			if (!on_size(wp, lp))			return FALSE;	break;
		case WM_MOUSEMOVE:		if (!on_mousemove(wp, lp))		return FALSE;	break;
		case WM_SETCURSOR:		if (!on_setcursor(wp, lp))		return TRUE;	break;
		case WM_LBUTTONDOWN:	if (!on_lbuttondown(wp, lp))	return FALSE;	break;
	}

	return C_window_base::window_proc(msg, wp, lp);
}

// ****************************************************************
// �_�C�A���O���j�󂳂ꂽ
// ================================================================
bool C_dialog::on_destroy()
{
	// �q�����X�g�̃N���A
	m_child_list.clear();

	return true;
}

// ****************************************************************
// �_�C�A���O�̃T�C�Y���ύX���ꂽ
// ================================================================
bool C_dialog::on_size(WPARAM wp, LPARAM lp)
{
	int dw = LOWORD(lp) - m_def_client_rect.width();
	int dh = HIWORD(lp) - m_def_client_rect.height();

	// �q���̈ʒu�𒲐�����
	adjust_child(dw, dh);

	// �X�e�[�^�X�o�[�̃T�C�Y�𒲐�����
	statusbar.on_size(m_h_wnd, wp, lp);

	// �Z�p���[�^�[�̃T�C�Y�𒲐�����
	if (separater_manager.on_size(m_h_wnd, wp, lp))
		return false;

	return true;
}

// ****************************************************************
// �}�E�X�J�[�\�����ړ�����
// ================================================================
bool C_dialog::on_mousemove(WPARAM wp, LPARAM lp)
{
	if (separater_manager.on_mousemove(m_h_wnd, wp, lp))
		return false;	// �Z�p���[�^

	return true;
}

// ****************************************************************
// �}�E�X�J�[�\���̕ύX�ʒm
// ================================================================
bool C_dialog::on_setcursor(WPARAM wp, LPARAM lp)
{
	if (separater_manager.on_setcursor(m_h_wnd, wp, lp))
		return false;	// �Z�p���[�^

	return true;
}

// ****************************************************************
// �}�E�X�̍��{�^����������
// ================================================================
bool C_dialog::on_lbuttondown(WPARAM wp, LPARAM lp)
{
	if (separater_manager.on_lbuttondown(m_h_wnd, wp, lp))
		return false;	// �Z�p���[�^

	return true;
}

// ****************************************************************
// �T�C�Y�ɂ��킹�Ďq���𓮂���
// ================================================================
void C_dialog::adjust_child(int dw, int dh)
{
	// �S�Ă̎q���𓮂���
	CHILDITR itr;
	for (itr = m_child_list.begin(); itr != m_child_list.end(); ++itr)
		(*itr)->adjust_to_parent(dw, dh);
}

// ****************************************************************
// �v���V�[�W���̐ݒ�
//		�߂�l�F�Â��v���V�[�W��
// ================================================================
DLGPROC C_dialog::set_proc(DLGPROC proc)
{
	return (DLGPROC)(PTR)SetWindowLongPtr(m_h_wnd, DWLP_DLGPROC, (LONG)(PTR)proc);
}

// ****************************************************************
// �v���V�[�W���̎擾
// ================================================================
DLGPROC C_dialog::get_proc()
{
	return (DLGPROC)(PTR)GetWindowLongPtr(m_h_wnd, DWLP_DLGPROC);
}

// ****************************************************************
// �v���V�[�W�������ɖ߂�
// ================================================================
void C_dialog::reset_proc()
{
	SetWindowLongPtr(m_h_wnd, DWLP_DLGPROC, (LONG)(PTR)C_window_base::window_proc_base);
}

// ****************************************************************
// �Z�p���[�^�[�̊��蓖��
// ================================================================
void C_dialog::bind_separater(C_dialog_separater* separater, CTSTR& name)
{
	separater_manager.bind_separater(separater, m_window_name, name);
}

// ****************************************************************
// �O���[�v�ɑ�����R���g���[���̗L���^������ݒ�
// ================================================================
void C_dialog::group_set_enable(int group_no, bool enable)
{
	for (CHILDITR itr = m_child_list.begin(); itr != m_child_list.end(); ++itr)	{
		if ((*itr)->get_group_no() == group_no)	{
			(*itr)->set_enable(enable);
		}
	}
}

// ****************************************************************
// ���[�_���_�C�A���O�F�쐬
// ================================================================
int C_modal_dialog::create(PTR dialog_id, CTSTR& name, bool limit_min, HWND parent)
{
	if (m_h_wnd != NULL)
		return false;

	// �_�C�A���O�̖��O�̐ݒ�
	m_window_name = name;
	// �E�B���h�E�̍ŏ���`�̐����t���O
	m_is_limit_window_rect_min = limit_min;

	// �_�C�A���O�̍\�z
	return (int)::DialogBoxParam(G_app.h_inst, (LPCTSTR)dialog_id, parent, (DLGPROC)window_proc_base, (LPARAM)this);
}

// ****************************************************************
// ���[�_���_�C�A���O�F�쐬
// ================================================================
int C_modal_dialog::create_indirect(C_dialog_template* dlg_tpl, CTSTR& name, bool is_limit_min, HWND h_parent_wnd)
{
	if (m_h_wnd != NULL)
		return false;

	// �_�C�A���O�̖��O
	m_window_name = name;
	// �E�B���h�E�̍ŏ���`�𐧌�
	m_is_limit_window_rect_min = is_limit_min;

	// �_�C�A���O�̍\�z
	return (int)::DialogBoxIndirectParam(G_app.h_inst, dlg_tpl->get(), h_parent_wnd, (DLGPROC)window_proc_base, (LPARAM)this);
}

// ****************************************************************
// ���[�_���_�C�A���O�F�I��
// ================================================================
void C_modal_dialog::destroy(int return_value)
{
	EndDialog(m_h_wnd, (PTR)return_value);
}

// ****************************************************************
// ���[�_���_�C�A���O�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT	C_modal_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_CLOSE:		if (!on_close())		return FALSE;	break;
	}

	return C_dialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// ���[�_���_�C�A���O�FWM_CLOSE
// ================================================================
bool C_modal_dialog::on_close()
{
	destroy(0);

	// �f�t�H���g�̓�������s���Ȃ��I
	return false;
}

// ****************************************************************
// �v���p�e�B�V�[�g�y�[�W
// ================================================================
bool C_property_sheet_page::create(PTR dialog_id, CTSTR& name, CTSTR& title, DWORD flag)
{
	PROPSHEETPAGE psp;
	ZeroMemory(&psp, sizeof(psp));
	psp.dwSize = sizeof(psp);
	psp.dwFlags = flag;
	psp.hInstance = G_app.h_inst;
	psp.pszTemplate = (LPCTSTR)dialog_id;
	psp.pfnDlgProc = (DLGPROC)window_proc_base;
	psp.pszTitle = title.c_str();
	psp.lParam = (LPARAM)this;
	m_h_wnd = (HWND)CreatePropertySheetPage(&psp);

	// �\�z�Ɏ��s
	if (m_h_wnd == NULL)
		return false;

	// �E�B���h�E�n���h���ƃI�u�W�F�N�g�����т���
	bind(m_h_wnd);

	return true;
}

// ****************************************************************
// �ȈՃG�f�B�b�g�_�C�A���O
// ================================================================
bool Ceasy_edit_dialog::create(CTSTR& window_title)
{
	C_dialog_template dlgtmpl;
	dlgtmpl.init_param(-1, -1, 200, 100, window_title, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0);
	dlgtmpl.add_control(10, 10, 150, 50, (PTR)WC_EDIT, EDIT, WS_CHILD | WS_VISIBLE, 0);
	dlgtmpl.add_control(10, 60, 70, 10, (PTR)WC_BUTTON, BTN_OK, WS_CHILD | WS_VISIBLE, 0);
	dlgtmpl.add_control(90, 60, 70, 10, (PTR)WC_BUTTON, BTN_CANCEL, WS_CHILD | WS_VISIBLE, 0);

	Ceasy_edit_dialog_sub dlg;
	if (!dlg.create_indirect(&dlgtmpl, _T(""), false))
		return false;

	result = dlg.get_result();
	return true;
}

LRESULT Ceasy_edit_dialog_sub::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init_dialog())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_modal_dialog::window_proc(msg, wp, lp);
}

bool Ceasy_edit_dialog_sub::on_init_dialog()
{
	bind_control(&edit, EDIT);
	bind_control(&btn_ok, BTN_OK);
	bind_control(&btn_cancel, BTN_CANCEL);

	btn_ok.set_text(_T("����"));
	btn_cancel.set_text(_T("�L�����Z��"));

	return true;
}

bool Ceasy_edit_dialog_sub::on_close()
{
	EndDialog(m_h_wnd, FALSE);		// ���s
	return true;
}

bool Ceasy_edit_dialog_sub::on_command(WPARAM wp, LPARAM lp)
{
	if (false);
	else if (LOWORD(wp) == BTN_OK)	{
		result = edit.get_text();
		EndDialog(m_h_wnd, TRUE);	// ����
	}
	else if (LOWORD(wp) == BTN_CANCEL)	{
		EndDialog(m_h_wnd, FALSE);	// ���s
	}

	return true;
}

}
