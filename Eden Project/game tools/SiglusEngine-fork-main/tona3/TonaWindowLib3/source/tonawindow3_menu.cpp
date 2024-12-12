#include	"tonawindow3_pch.h"
#include	"tonawindow3_menu.h"

namespace NT3
{

// ****************************************************************
// メニュー
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
// メニュー：メニューの構築
// ================================================================
bool C_menu::create()
{
	destroy();

	// メニューを作成
	m_h_menu = CreateMenu();
	if (m_h_menu == NULL)
		return false;

	m_is_create = true;	// 自分で作ったフラグ
	return true;
}

// ****************************************************************
// メニュー：メニューの破壊
// ================================================================
void C_menu::destroy()
{
	if (m_h_menu)	{

		// 自分で作った場合は破壊する
		if (m_is_create)
			DestroyMenu(m_h_menu);

		// 外部から割り当てた場合は割り当てを解除する
		else
			unbind();
	}

	// 初期化
	m_h_menu = NULL;
	m_is_create = false;
}

// ****************************************************************
// メニュー：メニューの割り当て
// ================================================================
bool C_menu::bind(HMENU h_menu)
{
	destroy();

	m_h_menu = h_menu;

	return true;
}

// ****************************************************************
// メニュー：メニューの割り当て解除
// ================================================================
void C_menu::unbind()
{
	m_h_menu = NULL;
}

// ****************************************************************
// メニュー：アイテム数の取得
// ================================================================
int C_menu::get_item_cnt()
{
	if (!m_h_menu)	{	return -1;	}
	return GetMenuItemCount(m_h_menu);
}

// ****************************************************************
// メニュー：アイテム追加
//		enable: 有効 / 無効（無効の場合グレー表示になります）
//		check:  チェックのオンオフ
// ================================================================
int C_menu::add_item(int menu_id, CTSTR& str, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	// アイテム位置 = 末尾
	int pos = get_item_cnt();

	// アイテムを挿入
	return insert_item(pos, menu_id, str, enable, check);
}

// ****************************************************************
// メニュー：アイテムを挿入
//		enable: 有効 / 無効（無効の場合グレー表示になります）
//		check:  チェックのオンオフ
// ================================================================
int C_menu::insert_item(int pos, int menu_id, CTSTR& str, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	TCHAR t_str[256];
	_tcscpy_s(t_str, 256, str.c_str());

	// メニューアイテムの構築
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
// メニュー：全アイテムを削除
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
// メニュー：全アイテムを削除（サブメニューはデタッチする）
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
// メニュー：アイテムのテキストを設定
//		enable: 有効 / 無効（無効の場合グレー表示になります）
//		check:  チェックのオンオフ
// ================================================================
void C_menu::set_item_text(int menu_id, CTSTR& str)
{
	if (!m_h_menu)	{	return;	}

	TCHAR t_str[256];
	_tcscpy_s(t_str, 256, str.c_str());

	// メニューアイテムの構築
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize     = sizeof(MENUITEMINFO);
	info.fMask      = MIIM_STRING;
	info.dwTypeData = t_str;
	info.cch        = 256;

	SetMenuItemInfo(m_h_menu, menu_id, FALSE, &info);
}

// ****************************************************************
// メニュー：サブメニューを追加
//		enable: 有効 / 無効（無効の場合グレー表示になります）
// ================================================================
int C_menu::add_sub_menu(int menu_id, CTSTR& str, HMENU h_sub_menu, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	// アイテム位置 = 末尾
	int pos = get_item_cnt();

	// アイテムを挿入
	return insert_sub_menu(pos, menu_id, str, h_sub_menu, enable, check);
}

// ****************************************************************
// メニュー：サブメニューを挿入
//		enable: 有効 / 無効（無効の場合グレー表示になります）
// ================================================================
int C_menu::insert_sub_menu(int pos, int menu_id, CTSTR& str, HMENU h_sub_menu, bool enable, bool check)
{
	if (!m_h_menu)	{	return -1;	}

	TCHAR t_str[256];
	_tcscpy_s(t_str, 256, str.c_str());

	// メニューアイテムの構築
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
// メニュー：既存のアイテムにサブメニューを設定
// ================================================================
bool C_menu::set_sub_menu(int menu_id, HMENU h_sub_menu)
{
	if (!m_h_menu)	{	return false;	}

	// メニューアイテムの構築
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

	// メニューアイテムの構築
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize     = sizeof(MENUITEMINFO);
	info.fMask      = MIIM_SUBMENU;
	info.hSubMenu   = m_h_menu;

	SetMenuItemInfo(h_parent_menu, menu_id, FALSE, &info);
	return true;
}


// ****************************************************************
// メニュー：セパレータを追加
// ================================================================
int C_menu::add_separator()
{
	if (!m_h_menu)	{	return -1;	}

	// アイテム位置 = 末尾
	int pos = get_item_cnt();

	// アイテムを挿入
	return insert_separator(pos);
}

// ****************************************************************
// メニュー：セパレータを挿入
// ================================================================
int C_menu::insert_separator(int pos)
{
	if (!m_h_menu)	{	return -1;	}

	// メニューアイテムの構築
	MENUITEMINFO info;
	ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask  = MIIM_TYPE;
	info.fType  = MFT_SEPARATOR;

	InsertMenuItem(m_h_menu, pos, TRUE, &info);
	return pos;
}

// ****************************************************************
// メニュー：チェックの設定
// ================================================================
void C_menu::set_check(int menu_id, bool check)
{
	if (!m_h_menu)	{	return;	}
	CheckMenuItem(m_h_menu, menu_id, MF_BYCOMMAND | (check ? MF_CHECKED : MF_UNCHECKED));
}

// ****************************************************************
// メニュー：チェックの取得
// ================================================================
bool C_menu::get_check(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	// メニューの状態の取得
	UINT state = GetMenuState(m_h_menu, menu_id, MF_BYCOMMAND);

	return (state & MF_CHECKED) > 0;
}

// ****************************************************************
// メニュー：チェックの反転
// ================================================================
bool C_menu::reverse_check(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	bool check = !get_check(menu_id);
	set_check(menu_id, check);
	return check;
}

// ****************************************************************
// メニュー：有効／無効の設定
// ================================================================
void C_menu::set_enable(int menu_id, bool enable)
{
	if (!m_h_menu)	{	return;	}
	EnableMenuItem(m_h_menu, menu_id, MF_BYCOMMAND | (enable ? MF_ENABLED : MF_GRAYED));
}

// ****************************************************************
// メニュー：有効／無効の取得
// ================================================================
bool C_menu::get_enable(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	// メニューの状態の取得
	UINT state = GetMenuState(m_h_menu, menu_id, MF_BYCOMMAND);

	return (state & MF_ENABLED) > 0;
}

// ****************************************************************
// メニュー：有効／無効の反転
// ================================================================
bool C_menu::reverse_enable(int menu_id)
{
	if (!m_h_menu)	{	return false;	}

	bool enable = !get_enable(menu_id);
	set_enable(menu_id, enable);
	return enable;
}

// ****************************************************************
// ポップアップメニュー
// ================================================================
C_popup_menu::C_popup_menu()
{
}

// ****************************************************************
// コンテキストメニュー：メニューの作成
// ================================================================
bool C_popup_menu::create()
{
	// メニューの作成
	m_h_menu = CreatePopupMenu();
	if (m_h_menu == NULL)
		return false;

	m_is_create = true;	// 自分で作ったフラグ
	return true;
}

// ****************************************************************
// コンテキストメニュー：選択開始
// ================================================================
int C_popup_menu::select(HWND h_wnd, int x, int y)
{
	if (!m_h_menu)	{	return 0;	}

	// 右クリックメニュー開始
	int res = TrackPopupMenu(m_h_menu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON, x, y, 0, h_wnd, 0);

	return res;
}

}
