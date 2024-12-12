#pragma		once

namespace NT3
{

// ****************************************************************
// ���j���[
// ================================================================
class C_menu
{
public:
	C_menu();
	~C_menu();

	bool	create();				// �\�z
	void	destroy();				// �j��
	bool	bind(HMENU h_menu);		// ���蓖�Ă�
	void	unbind();				// ���蓖�Ă���������

	// �A�C�e��
	int		get_item_cnt();
	int		add_item(int menu_id, CTSTR& str, bool enable = true, bool check = false);
	int		insert_item(int pos, int menu_id, CTSTR& str, bool enable = true, bool check = false);
	void	delete_all_item();
	void	remove_all_item();
	void	set_item_text(int menu_id, CTSTR& str);
	// �Z�p���[�^
	int		add_separator();
	int		insert_separator(int pos);
	// �T�u���j���[
	int		add_sub_menu(int menu_id, CTSTR& str, HMENU h_sub_menu, bool enable = true, bool check = false);
	int		insert_sub_menu(int pos, int menu_id, CTSTR& str, HMENU h_sub_menu, bool enable = true, bool check = false);
	bool	set_sub_menu(int menu_id, HMENU h_sub_menu);
	bool	into_parent(HMENU h_parent_menu, int menu_id);

	// �L���^����
	void	set_enable(int menu_id, bool enable);
	bool	get_enable(int menu_id);
	bool	reverse_enable(int menu_id);

	// �`�F�b�N
	void	set_check(int menu_id, bool check);
	bool	get_check(int menu_id);
	bool	reverse_check(int menu_id);

	// �n���h���̎擾
	HMENU	get_handle()	{	return m_h_menu;	}

protected:
	HMENU	m_h_menu;
	bool	m_is_create;
};


// ****************************************************************
// �|�b�v�A�b�v���j���[
// ================================================================
class C_popup_menu : public C_menu
{
public:
	C_popup_menu();

	bool	create();

	int		select(HWND h_wnd, int x, int y);
};

}
