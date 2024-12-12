#include	"tonawindow3_pch.h"
#include	"tonawindow3_menu.h"

namespace NT3
{

// ****************************************************************
// ���j���[
// ================================================================
C_menu::C_menu()
{
	m_h_menu = NULL;
	m_is_create = false;
}

C_menu::~C_menu()
{
	destroy();
}

// ****************************************************************
// ���j���[�F���j���[�̍\�z
// ================================================================
bool C_menu::create()
{
	destroy();

	// ���j���[���쐬
	m_h_menu = CreateMenu();
	if (m_h_menu == NULL)
		return false;

	m_is_create = true;	// �����ō�����t���O
	return true;
}

// ****************************************************************
// ���j���[�F���j���[�̔j��
// ================================================================
void C_menu::destroy()
{
	if (m_h_menu)	{

		// �����ō�����ꍇ�͔j�󂷂�
		if (m_is_create)
			DestroyMenu(m_h_menu);

		// �O�����犄�蓖�Ă��ꍇ�͊��蓖�Ă���������
		else
			unbind();
	}

	// ������
	m_h_menu = NULL;
	m_is_create = false;
}

// ****************************************************************
// ���j���[�F���j���[�̊��蓖��
// ================================================================
bool C_menu::bind(HMENU h_menu)
{
	destroy();

	m_h_menu = h_menu;

	return true;
}

// ****************************************************************
// ���j���[�F���j���[�̊��蓖�ĉ���
// ================================================================
void C_menu::unbind()
{
	m_h_menu = NULL;
}

// ****************************************************************
// ���j���[�F�A�C�e�����̎擾
// ================================================================
int C_menu::get_item_cnt()
{
	if (!m_h_menu)	{	return -1;	}
	return GetMenuItemCount(m_h_menu);
}

// ****************************************************************
// ���j���[�F�A�C�e���ǉ�
//		enable: �L�� / �����i�����̏ꍇ�O���[�\���ɂȂ�܂��j
//		check:  �`�F�b�N�̃I���I�t
// ================================================================
int C_menu::add_item(int menu_id, CTSTR& str, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	// �A�C�e���ʒu = ����
	int pos = get_item_cnt();

	// �A�C�e����}��
	return insert_item(pos, menu_id, str, enable, check);
}

// ****************************************************************
// ���j���[�F�A�C�e����}��
//		enable: �L�� / �����i�����̏ꍇ�O���[�\���ɂȂ�܂��j
//		check:  �`�F�b�N�̃I���I�t
// ================================================================
int C_menu::insert_item(int pos, int menu_id, CTSTR& str, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	TCHAR t_str[256];
	_tcscpy_s(t_str, 256, str.c_str());

	// ���j���[�A�C�e���̍\�z
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize     = sizeof(MENUITEMINFO);
	info.fMask      = MIIM_TYPE | MIIM_ID | MIIM_STATE;
	info.fType      = MFT_STRING;
	info.fState     = (enable ? MFS_ENABLED : MFS_DISABLED) | (check ? MFS_CHECKED : MFS_UNCHECKED);
	info.wID        = menu_id;
	info.dwTypeData = t_str;
	info.cch        = 256;

	InsertMenuItem(m_h_menu, pos, TRUE, &info);
	return pos;
}

// ****************************************************************
// ���j���[�F�S�A�C�e�����폜
// ================================================================
void C_menu::delete_all_item()
{
	if (!m_h_menu)	{	return;	}

	int cnt = get_item_cnt();
	for (int i = 0; i < cnt; i++)	{
		DeleteMenu(m_h_menu, 0, MF_BYPOSITION);
	}
}

// ****************************************************************
// ���j���[�F�S�A�C�e�����폜�i�T�u���j���[�̓f�^�b�`����j
// ================================================================
void C_menu::remove_all_item()
{
	if (!m_h_menu)	{	return;	}

	int cnt = get_item_cnt();
	for (int i = 0; i < cnt; i++)	{
		RemoveMenu(m_h_menu, 0, MF_BYPOSITION);
	}
}

// ****************************************************************
// ���j���[�F�A�C�e���̃e�L�X�g��ݒ�
//		enable: �L�� / �����i�����̏ꍇ�O���[�\���ɂȂ�܂��j
//		check:  �`�F�b�N�̃I���I�t
// ================================================================
void C_menu::set_item_text(int menu_id, CTSTR& str)
{
	if (!m_h_menu)	{	return;	}

	TCHAR t_str[256];
	_tcscpy_s(t_str, 256, str.c_str());

	// ���j���[�A�C�e���̍\�z
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize     = sizeof(MENUITEMINFO);
	info.fMask      = MIIM_STRING;
	info.dwTypeData = t_str;
	info.cch        = 256;

	SetMenuItemInfo(m_h_menu, menu_id, FALSE, &info);
}

// ****************************************************************
// ���j���[�F�T�u���j���[��ǉ�
//		enable: �L�� / �����i�����̏ꍇ�O���[�\���ɂȂ�܂��j
// ================================================================
int C_menu::add_sub_menu(int menu_id, CTSTR& str, HMENU h_sub_menu, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	// �A�C�e���ʒu = ����
	int pos = get_item_cnt();

	// �A�C�e����}��
	return insert_sub_menu(pos, menu_id, str, h_sub_menu, enable, check);
}

// ****************************************************************
// ���j���[�F�T�u���j���[��}��
//		enable: �L�� / �����i�����̏ꍇ�O���[�\���ɂȂ�܂��j
// ================================================================
int C_menu::insert_sub_menu(int pos, int menu_id, CTSTR& str, HMENU h_sub_menu, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	TCHAR t_str[256];
	_tcscpy_s(t_str, 256, str.c_str());

	// ���j���[�A�C�e���̍\�z
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize     = sizeof(MENUITEMINFO);
	info.fMask      = MIIM_TYPE | MIIM_ID | MIIM_STATE | MIIM_SUBMENU;
	info.fType      = MF_STRING;
	info.fState     = (enable ? MFS_ENABLED : MFS_DISABLED) | (check ? MFS_CHECKED : MFS_UNCHECKED);
	info.hSubMenu   = h_sub_menu;
	info.wID        = menu_id;
	info.dwTypeData = t_str;
	info.cch        = 256;

	InsertMenuItem(m_h_menu, pos, TRUE, &info);
	return pos;
}

// ****************************************************************
// ���j���[�F�����̃A�C�e���ɃT�u���j���[��ݒ�
// ================================================================
bool C_menu::set_sub_menu(int menu_id, HMENU h_sub_menu)
{
	if (!m_h_menu)	{	return false;	}

	// ���j���[�A�C�e���̍\�z
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize     = sizeof(MENUITEMINFO);
	info.fMask      = MIIM_SUBMENU;
	info.hSubMenu   = h_sub_menu;

	SetMenuItemInfo(m_h_menu, menu_id, FALSE, &info);
	return true;
}

bool C_menu::into_parent(HMENU h_parent_menu, int menu_id)
{
	if (!h_parent_menu)	{	return false;	}

	// ���j���[�A�C�e���̍\�z
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize     = sizeof(MENUITEMINFO);
	info.fMask      = MIIM_SUBMENU;
	info.hSubMenu   = m_h_menu;

	SetMenuItemInfo(h_parent_menu, menu_id, FALSE, &info);
	return true;
}


// ****************************************************************
// ���j���[�F�Z�p���[�^��ǉ�
// ================================================================
int C_menu::add_separator()
{
	if (!m_h_menu)	{	return -1;	}

	// �A�C�e���ʒu = ����
	int pos = get_item_cnt();

	// �A�C�e����}��
	return insert_separator(pos);
}

// ****************************************************************
// ���j���[�F�Z�p���[�^��}��
// ================================================================
int C_menu::insert_separator(int pos)
{
	if (!m_h_menu)	{	return -1;	}

	// ���j���[�A�C�e���̍\�z
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask  = MIIM_TYPE;
	info.fType  = MFT_SEPARATOR;

	InsertMenuItem(m_h_menu, pos, TRUE, &info);
	return pos;
}

// ****************************************************************
// ���j���[�F�`�F�b�N�̐ݒ�
// ================================================================
void C_menu::set_check(int menu_id, bool check)
{
	if (!m_h_menu)	{	return;	}
	CheckMenuItem(m_h_menu, menu_id, MF_BYCOMMAND | (check ? MF_CHECKED : MF_UNCHECKED));
}

// ****************************************************************
// ���j���[�F�`�F�b�N�̎擾
// ================================================================
bool C_menu::get_check(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	// ���j���[�̏�Ԃ̎擾
	UINT state = GetMenuState(m_h_menu, menu_id, MF_BYCOMMAND);

	return (state & MF_CHECKED) > 0;
}

// ****************************************************************
// ���j���[�F�`�F�b�N�̔��]
// ================================================================
bool C_menu::reverse_check(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	bool check = !get_check(menu_id);
	set_check(menu_id, check);
	return check;
}

// ****************************************************************
// ���j���[�F�L���^�����̐ݒ�
// ================================================================
void C_menu::set_enable(int menu_id, bool enable)
{
	if (!m_h_menu)	{	return;	}
	EnableMenuItem(m_h_menu, menu_id, MF_BYCOMMAND | (enable ? MF_ENABLED : MF_GRAYED));
}

// ****************************************************************
// ���j���[�F�L���^�����̎擾
// ================================================================
bool C_menu::get_enable(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	// ���j���[�̏�Ԃ̎擾
	UINT state = GetMenuState(m_h_menu, menu_id, MF_BYCOMMAND);

	return (state & MF_ENABLED) > 0;
}

// ****************************************************************
// ���j���[�F�L���^�����̔��]
// ================================================================
bool C_menu::reverse_enable(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	bool enable = !get_enable(menu_id);
	set_enable(menu_id, enable);
	return enable;
}

// ****************************************************************
// �|�b�v�A�b�v���j���[
// ================================================================
C_popup_menu::C_popup_menu()
{
}

// ****************************************************************
// �R���e�L�X�g���j���[�F���j���[�̍쐬
// ================================================================
bool C_popup_menu::create()
{
	// ���j���[�̍쐬
	m_h_menu = CreatePopupMenu();
	if (m_h_menu == NULL)
		return false;

	m_is_create = true;	// �����ō�����t���O
	return true;
}

// ****************************************************************
// �R���e�L�X�g���j���[�F�I���J�n
// ================================================================
int C_popup_menu::select(HWND h_wnd, int x, int y)
{
	if (!m_h_menu)	{	return 0;	}

	// �E�N���b�N���j���[�J�n
	int res = TrackPopupMenu(m_h_menu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON, x, y, 0, h_wnd, 0);

	return res;
}

}
