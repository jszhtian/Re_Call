//
//
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "myBitmap.h"
#include "myPopupMenu.h"



/*
1,TEST1,-1
2,NOP,0
2,DUMMY,1
2,DUMMY2,-1
3,AAA,5
3,BBB,6
1,CCC,7




TEST1(POPUP)
 -NOP
 -DUMMY
 -DUMMY2(POPUP)
 --AAA
 --BBB
CCC


@付きだとbitmap読み込み
				%つきだとbitmap読み込みかつ名前も使う（これは不可能だった）
	bitmap名はメニュー名と同じ@%共に追加予定機能（未実装）
-999:サブメニュー
-666:separator

*/


CMyPopupMenu::CMyPopupMenu(LPSTR filename,int menuCommandNumber)
{
	m_menuCommandNumber = menuCommandNumber;

	m_nameList = new CNameList();
	char filename2[256];
	wsprintf(filename2,"nnndir\\setup\\menu\\%s.txt",filename);
	m_nameList->LoadFile(filename2);

	//create popup menu
//	m_mainMenuKosuu = atoi(m_nameList->GetName(0));

	int listKosuu = m_nameList->GetNameKosuu() / 3;

	//必要ビットマップ個数を数える
	m_bitmapKosuu = 0;

	int i;
	for (i=0;i<listKosuu;i++)
	{
		LPSTR name = m_nameList->GetName(i*3+1);
		if ((*name) == '@') m_bitmapKosuu++;
	}

	m_bitmap = NULL;
	if (m_bitmapKosuu > 0)
	{
		m_bitmap = new CMyBitmap* [m_bitmapKosuu];
	}


	m_menu = CreatePopupMenu();
	m_hWnd = NULL;

	//10階層まで
	HMENU menu[10];
	int kosuu[10];
	kosuu[0] = 0;
	menu[0] = m_menu;

	int bitmapNumber = 0;


	for (i=0;i<listKosuu;i++)
	{
		int level = atoi(m_nameList->GetName(i*3));
		LPSTR name = m_nameList->GetName(i*3+1);
		int typePara = atoi(m_nameList->GetName(i*3+2));

		HMENU parentMenu = menu[level-1];

		MENUITEMINFO mif;
		ZeroMemory(&mif,sizeof(mif));

		mif.cbSize = sizeof(mif);


		if (typePara == -666)	//separator
		{
			mif.fMask = MIIM_TYPE;
			mif.fType = MFT_SEPARATOR;
		}
		else if (typePara == -999)	//popup
		{
			menu[level] = CreatePopupMenu();
			mif.fMask = MIIM_SUBMENU;
			mif.hSubMenu = menu[level];
			kosuu[level] = 0;

			mif.fMask |= MIIM_TYPE;
			mif.fType |= MFT_STRING;
			mif.dwTypeData = name;
			mif.cch = strlen(name);

		}
		else				//menuItem
		{
//			mif.fMask = MIIM_TYPE | MIIM_ID | MIIM_DATA;
			mif.fMask = MIIM_TYPE | MIIM_ID;
			mif.fType = 0;
//			mif.dwItemData = typePara;
			mif.wID = typePara + m_menuCommandNumber + 1;


			BOOL strFlag = TRUE;
			BOOL bitmapFlag = FALSE;

			if ((*name) == '@')
			{
				bitmapFlag = TRUE;
				strFlag = FALSE;
			}

//			if ((*name) == '%')
//			{
//				bitmapFlag = TRUE;
//				strFlag = FALSE;
//			}

			if (strFlag)
			{
				mif.fType |= MFT_STRING;
				mif.dwTypeData = name;
				mif.cch = strlen(name);
			}

			if (bitmapFlag)
			{
				mif.fType |= MFT_BITMAP;

				char filename[256];
				wsprintf(filename,"nnndir\\setup\\%s.bmp",name+1);
				m_bitmap[bitmapNumber] = new CMyBitmap(filename);
				mif.dwTypeData = (LPSTR)(LOWORD(m_bitmap[bitmapNumber]->GetHBitmap()));

				bitmapNumber++;
			}
		}

		InsertMenuItem(parentMenu,kosuu[level-1],TRUE,&mif);
		kosuu[level-1]++;
	}
}



CMyPopupMenu::~CMyPopupMenu()
{
	End();
}

void CMyPopupMenu::End(void)
{
	if (m_bitmap != NULL)
	{
		for (int i=0;i<m_bitmapKosuu;i++)
		{
			ENDDELETECLASS(m_bitmap[i]);
		}
		DELETEARRAY(m_bitmap);
	}

	if (m_menu != NULL)
	{
		if (m_hWnd != NULL)
		{
			DestroySubMenu(m_menu);
			DestroyMenu(m_menu);
			m_menu = NULL;
			m_hWnd = NULL;
		}
	}

	ENDDELETECLASS(m_nameList);
}


void CMyPopupMenu::SetMyMenu(HWND hwnd)
{
	m_hWnd = hwnd;
//	SetMenu(m_hWnd,m_menu);
}

int CMyPopupMenu::OpenMenu(POINT pt,HWND hwnd)
{
	if (hwnd == NULL) hwnd = m_hWnd;
	if (hwnd == NULL) return -1;
	int cmd = TrackPopupMenu(m_menu,TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,pt.x,pt.y,0,hwnd,NULL);
	if (cmd == 0) return -1;
	return cmd - 1;
}

void CMyPopupMenu::DestroySubMenu(HMENU menu)
{
	int kosuu = GetMenuItemCount(menu);
	if (menu == NULL) return;

	for (int i=kosuu-1;i>=0;i--)
	{
		MENUITEMINFO info;
		info.cbSize = sizeof(info);
		info.fMask = MIIM_SUBMENU;

		GetMenuItemInfo(menu,i,TRUE,&info);
//		if (info.fMask & MIIM_SUBMENU)
		if (info.hSubMenu != NULL)
		{
//			HMENU subMenu = GetSubMenu(menu,i);
			HMENU subMenu = info.hSubMenu;
			DestroySubMenu(subMenu);
			DestroyMenu(subMenu);//menu,i,MF_BYPOSITION);
		}
	}
}



/*_*/


