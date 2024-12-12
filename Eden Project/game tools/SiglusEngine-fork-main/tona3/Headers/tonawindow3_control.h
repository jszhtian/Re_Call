#pragma		once

#include	"tonawindow3_wbase.h"

namespace NT3
{

// ****************************************************************
// �R���g���[���e���v���[�g
// ================================================================
class C_control_template
{
public:
	void				init_param(int x, int y, int w, int h, PTR class_atom, WORD control_id, DWORD style, DWORD ex_style, PTR text_atom = -1);
	LPDLGITEMTEMPLATE	get();
	int					get_size();

private:
	void	push_atom(PTR atom);

	C_stream	stream;
};

// ****************************************************************
// �R���g���[��
// ================================================================
class C_control : public C_window_base
{
public:
	C_control();
	~C_control();

	bool	create(CTSTR& name, CTSTR& caption, int x, int y, int client_w, int client_h, bool is_menu, DWORD window_style, DWORD window_style_ex, bool is_limit_min, bool is_attach, HWND h_parent_wnd = NULL);
	bool	bind(HWND h_dlg, int ctrl_id, CTSTR& name, int adjust_way, int group_no);			// �R���g���[�������т���

	void	clear_text();					// �e�L�X�g�̃N���A
	void	set_text(CTSTR& str);			// �e�L�X�g�̐ݒ�
	void	set_text_adjust_width(CTSTR& str, int rep_width = 0, HFONT font = NULL);	// �e�L�X�g�̐ݒ��A�e�L�X�g�T�C�Y�ɍ��킹�ăR���g���[���̕��𒲐�����
	TSTR	get_text();						// �e�L�X�g�̎擾
	void	set_bitmap(HBITMAP h_bitmap);	// �r�b�g�}�b�v�̐ݒ�

	int		get_adjust_width_from_text(int rep_width = 0, HFONT font = NULL);	// �e�L�X�g�T�C�Y�ɍ��킹�ăR���g���[���̕����擾����
	int		adjust_width_from_text(int rep_width = 0, HFONT font = NULL);		// �e�L�X�g�T�C�Y�ɍ��킹�ăR���g���[���̕��𒲐�����

protected:
	TSTR	m_class_name;			// �R���g���[���ŗL�̃N���X���i�p����Őݒ肷��j
};

// ****************************************************************
// �X�^�e�B�b�N
// ================================================================
class Cc_static : public C_control
{
public:
	Cc_static()	{	m_class_name = WC_STATIC;	}

};

// ****************************************************************
// �O���[�v
// ================================================================
class Cc_group : public C_control
{
public:
	Cc_group()	{	m_class_name = WC_BUTTON;	}

};

// ****************************************************************
// �{�^��
// ================================================================
class Cc_button : public C_control
{
public:
	Cc_button()	{	m_class_name = WC_BUTTON;	}

	void	set_button_style(PTR style, bool redraw = true);
	bool	is_pushed();
};

// ****************************************************************
// ���W�I�{�^��
// ================================================================
class Cc_radio : public C_control
{
public:
	Cc_radio()	{	m_class_name = WC_BUTTON;	}

	void	set_check(bool check);
	bool	get_check();

	void	save_ini(CTSTR& project_name = _T(""));
	void	load_ini(bool def, CTSTR& project_name = _T(""));
};

// ****************************************************************
// �`�F�b�N�{�b�N�X
// ================================================================
class Cc_check : public C_control
{
public:
	Cc_check()	{	m_class_name = WC_BUTTON;	}

	void	set_check(bool check);
	bool	get_check();

	void	save_ini(CTSTR& project_name = _T(""));
	void	load_ini(bool def, CTSTR& project_name = _T(""));
};

// ****************************************************************
// �G�f�B�b�g�{�b�N�X
// ================================================================
class Cc_edit : public C_control
{
public:
	Cc_edit()	{	m_class_name = WC_EDIT;	}

	void	set_sel(int start, int end);		// �������I������iend = -1 �ōŌ�܂őI���j
	void	set_sel_all();						// �������S�đI������

	void	save_ini(CTSTR& project_name = _T(""));
	void	load_ini(CTSTR& def, CTSTR& project_name = _T(""));

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// �X�s��
// ================================================================
class Cc_spin : public C_control
{
public:
	Cc_spin()	{	m_class_name = _T("msctls_updown32");	}

	void	set_edit(HWND h_edit);				// �A������G�f�B�b�g��ݒ�
	void	set_edit_range(int min, int max);	// �G�f�B�b�g�̒l�͈̔͂�ݒ�
};

// ****************************************************************
// ���X�g�{�b�N�X
// ================================================================
class Cc_listbox : public C_control
{
public:
	Cc_listbox()	{ m_class_name = WC_LISTBOX; }

	void	clear();
	int		add_item(CTSTR& str);				// �A�C�e����K���Ȉʒu�ɒǉ�
	int		insert_item(int pos, CTSTR& str);	// �A�C�e�����ʒu���w�肵�Ēǉ��i-1 �Ŗ����j�iLBS_SORT ���w�肳��Ă��Ȃ����Ɓj
	void	delete_item(int pos);				// �A�C�e�����폜

	// �A�C�e���֘A
	int		get_item_cnt();						// �A�C�e�������擾
	TSTR	get_item_text(int item_no);			// �A�C�e�����擾

	// �A�C�e���̑I���֘A
	void	set_item_select_state(int item_no, bool selected);	// �A�C�e���̑I����Ԃ�ݒ�i�����I���^�C�v�Ŏg�p�j
	void	set_item_select_state_all(bool selected);			// �A�C�e���̑I����Ԃ�ݒ�i�����I���^�C�v�Ŏg�p�j
	bool	get_item_select_state(int item_no);					// �A�C�e���̑I����Ԃ��擾�i�����I���^�C�v�Ŏg�p�j
	int		get_selected_item_cnt();							// �I������Ă���A�C�e�������擾�i�����I���^�C�v�Ŏg�p�j
	void	set_selected_item_no(int item_no);					// �A�C�e����I���i�P��I���^�C�v�Ŏg�p�j
	int		get_selected_item_no();								// �I������Ă���A�C�e���ԍ����擾�i�P��I���^�C�v�Ŏg�p�j�i�G���[�FLB_ERR�j

	void	set_horizontal_scroll_bar();		// �����X�N���[���o�[�ݒ�
};

// ****************************************************************
// �R���{�{�b�N�X
// ================================================================
class Cc_combobox :  public C_control
{
public:
	Cc_combobox()	{	m_class_name = WC_COMBOBOX;	item_cnt_max = 10;	}

	int		get_item_cnt();				// �A�C�e�������擾
	void	set_sel_no(int sel_no);		// �I�𒆂̃A�C�e���ԍ���ݒ�
	int		get_sel_no();				// �I�𒆂̃A�C�e���ԍ����擾

	TSTR	get_item_text(int item_no);								// �A�C�e���̕�������擾
	void	set_item_text(int item_no, CTSTR& text);				// �A�C�e���̕������ݒ�
	TSTR	get_sel_item_text();									// �I�𒆂̃A�C�e���̕�������擾

	void	clear_item_list();										// �A�C�e�����X�g���N���A
	void	insert_item(CTSTR& text, int index, bool erase_same);	// �A�C�e����}��
	void	insert_item_to_top(CTSTR& text, bool erase_same);		// �擪�ɃA�C�e����ǉ�
	void	insert_item_to_bottom(CTSTR& text, bool erase_same);	// �����ɃA�C�e����ǉ�
	int		find_item(CTSTR& text);									// �A�C�e��������
	void	delete_item(int item_no);								// �A�C�e�����폜

	void	save_ini_sel_no(CTSTR& project_name = _T(""));					// ini �Z�[�u�F�I��ԍ�
	void	load_ini_sel_no(int def_sel_no, CTSTR& project_name = _T(""));	// ini ���[�h�F�I��ԍ�
	void	save_ini_drop(CTSTR& project_name = _T(""));					// ini �Z�[�u�F�h���b�v
	void	load_ini_drop(CTSTR& project_name = _T(""));					// ini ���[�h�F�h���b�v

private:
	int		item_cnt_max;
};

// ****************************************************************
// �c���[�r���[
// ================================================================
class Cc_treeview :  public C_control
{
public:
	Cc_treeview()	{	m_class_name = WC_TREEVIEW;	}

	void		clear();
	HTREEITEM	add_item(HTREEITEM h_parent, HTREEITEM h_pos, CTSTR& text);		// TVI_ROOT / TVI_FIRST / TVI_LAST / TVI_SORT

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// �X�N���[���o�[
// ================================================================
class Cc_scroll : public C_control
{
public:
	Cc_scroll()		{	m_class_name = WC_SCROLLBAR;	}

	void	set_scroll_info(int min, int max, int page_size);	// �X�N���[������ݒ肷��
	void	set_scroll_pos(int pos);			// ���݂̒l��ݒ肷��
	int		get_scroll_pos();					// ���݂̒l���擾����
	int		scroll_natural(WPARAM wp);			// WM_HSCROLL �� WM_VSCROLL �ɑ΂��Ď��R�ȓ������s��

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};

// ****************************************************************
// �X���C�_�[
// ================================================================
class Cc_slider : public C_control
{
public:
	Cc_slider()		{	m_class_name = TRACKBAR_CLASS;	}

	void	set_slider_pos(int pos);				// ���݂̒l��ݒ肷��
	int		get_slider_pos();						// ���݂̒l���擾����
	void	set_slider_range(int min, int max);		// �X���C�h�̍ŏ��l�ƍő�l��ݒ肷��
};

// ****************************************************************
// �v���O���X�o�[
// ================================================================
class Cc_progress : public C_control
{
public:

	void	clear();					// ��Ԃ��N���A
	void	set_max_prg(int max_prg);	// �ő�l��ݒ肷��
	void	set_prg(int prg);			// ���݂̒l��ݒ肷��
};

// ****************************************************************
// �^�u
// ================================================================
class Cc_tab : public C_control
{
public:
	Cc_tab()	{	m_class_name = WC_TABCONTROL; tab_cnt = 0;	}

	void	add_item(CTSTR& str);					// �^�u�̒ǉ�
	int		get_item_cnt();							// �^�u�̐����擾
	void	set_item_text(int item_no, CTSTR& str);	// �^�u�̕�����ݒ�
	TSTR	get_item_text(int item_no);				// �^�u�̕�����擾
	void	set_selected_item_no(int item_no);		// �I������Ă���^�u��ݒ�
	int		get_selected_item_no();					// �I������Ă���^�u���擾
	C_rect	get_display_rect();						// �\���G���A���擾

private:
	int		tab_cnt;
};

// ****************************************************************
// �s�N�`��
// ================================================================
class Cc_picture : public C_control
{
public:
	Cc_picture()	{	m_class_name = WC_STATIC;	}

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);
};


}
