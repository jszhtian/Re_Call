#pragma		once

#include	"tonawindow3_wbase.h"
#include	"tonawindow3_dialog_separater.h"
#include	"tonawindow3_dialog_statusbar.h"

namespace NT3
{

class	C_control;

// ****************************************************************
// �_�C�A���O�e���v���[�g
// ================================================================
class C_dialog_template
{
public:
	void			init_param(int x, int y, int w, int h, CTSTR& title, DWORD style, DWORD ex_style, PTR menu_atom = -1, WORD font_size = 0, CTSTR& font_name = _T(""));
	void			add_control(int x, int y, int w, int h, PTR class_atom, WORD control_id, DWORD style, DWORD ex_style, PTR text_atom = -1);
	void			set_size(int w, int h);
	DLGTEMPLATE*	get();

private:
	void	push_atom(PTR atom);

	C_stream	m_stream;
};

// ****************************************************************
// ���[�h���X�_�C�A���O
// ================================================================
class C_dialog : public C_window_base
{
public:
	C_dialog();
	~C_dialog();

	bool	create(PTR dialog_id, CTSTR& name, bool limit_min, bool attach, HWND h_parent_wnd = NULL, int adjust_way = 0);							// �_�C�A���O�̍\�z
	bool	create_indirect(C_dialog_template* dlg_tpl, CTSTR& name, bool limit_min, bool attach, HWND h_parent_wnd = NULL, int adjust_way = 0);	// �_�C�A���O�̍\�z
	void	bind_control(C_control* ctrl, int ctrl_id, CTSTR& name = _T(""), int adjust_way = 0, int group_no = 0);									// �R���g���[���̊��蓖��
	void	add_child(C_window_base* p_wnd, int adjust_way = 0, int group_no = 0);																	// �`���C���h���X�g�ɒǉ�

	void		adjust_child(int dw, int dh);		// �q���̈ʒu�𒲐�
	DLGPROC		set_proc(DLGPROC proc);				// �v���V�[�W����ݒ�
	DLGPROC		get_proc();							// �v���V�[�W�����擾
	void		reset_proc();						// �v���V�[�W�������ɖ߂�

	void		group_set_enable(int group_no, bool enable);

	C_dialog_statusbar			statusbar;			// �X�e�[�^�X�o�[

protected:

	void	window_proc_pre(UINT msg, WPARAM wp, LPARAM lp);
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);

	void	on_init_dialog_pre();
	bool	on_destroy();
	bool	on_size(WPARAM wp, LPARAM lp);
	bool	on_mousemove(WPARAM wp, LPARAM lp);
	bool	on_setcursor(WPARAM wp, LPARAM lp);
	bool	on_lbuttondown(WPARAM wp, LPARAM lp);

	void	bind_separater(C_dialog_separater* separater, CTSTR& name);	// �Z�p���[�^�[�̊��蓖��
	Cdialog_separater_manager	separater_manager;	// �Z�p���[�^�[�}�l�[�W���[
};

// ****************************************************************
// ���[�_���_�C�A���O
// ================================================================
class C_modal_dialog : public C_dialog
{
public:
	int		create(PTR dialog_id, CTSTR& name, bool limit_min, HWND parent = NULL);
	int		create_indirect(C_dialog_template* dlg_tpl, CTSTR& name, bool limit_min, HWND h_parent_wnd = NULL);		// �_�C�A���O�̍\�z
	void	destroy(int return_value);

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
	bool	on_close();
};

// ****************************************************************
// �v���p�e�B�V�[�g�y�[�W
// ================================================================
class C_property_sheet_page : public C_dialog
{
public:
	bool	create(PTR dialog_id, CTSTR& name, CTSTR& title, DWORD flag);
};

// ****************************************************************
// �ȈՃG�f�B�b�g�_�C�A���O
// ================================================================
class Ceasy_edit_dialog
{
public:
	Ceasy_edit_dialog()	{}

	bool	create(CTSTR& window_title);
	TSTR	get_result()	{	return result;	}

private:
	enum
	{
		EDIT = 0,
		BTN_OK = 1,
		BTN_CANCEL = 2,
	};

	TSTR	result;
};

class Ceasy_edit_dialog_sub : public C_modal_dialog
{
public:
	TSTR		get_result()	{	return result;	}

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);

private:
	bool		on_init_dialog();
	bool		on_close();
	bool		on_command(WPARAM wp, LPARAM lp);

	Cc_edit		edit;
	Cc_button	btn_ok;
	Cc_button	btn_cancel;

	enum
	{
		EDIT = 0,
		BTN_OK = 1,
		BTN_CANCEL = 2,
	};

	TSTR		result;
};

}
