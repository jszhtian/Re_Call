//
// selectdialog.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\namelist.h"
#include "selectdialog.h"

#include "case.h"
#include "messageData.h"

#include "gameMessageDoc.h"
#include "filmDoc.h"

/*
typedef struct _tagMYSELECTDIALOGSTRUCT
{
	CNameList* list;
	int oldSelect;
	int nameType;
	LPSTR* pNamePtr;
	int nameKosuu;
	CGameMessageDoc* doc;
	CFilmDoc* doc2;
	LPSTR caption;
} MYSELECTDIALOGSTRUCT;
*/


char CSelectDialog::m_selectCaption[] = "‘I‘ð";

CSelectDialog::CSelectDialog(HWND parentHWND,HINSTANCE hinstance)
{
	m_parentHWND = parentHWND;
	m_hInstance = hinstance;
	m_captionExistFlag = FALSE;
}


CSelectDialog::~CSelectDialog()
{
	End();
}

void CSelectDialog::End(void)
{
}

void CSelectDialog::SetCaption(LPSTR caption)
{
	m_captionExistFlag = TRUE;
	m_caption = caption;
}

int CSelectDialog::GetSelect(LPSTR* pNamePtr,int nameKosuu,int oldSelect,CGameMessageDoc* lpDoc,CFilmDoc* lpDoc2)
{
	MYSELECTDIALOGSTRUCT sl;
//	sl.list = lpNameList;
	sl.oldSelect = oldSelect;
	sl.nameType = 1;
	sl.pNamePtr = pNamePtr;
	sl.nameKosuu = nameKosuu;
	sl.doc = lpDoc;
	sl.doc2 = lpDoc2;

	if (m_captionExistFlag)
	{
		sl.caption = m_caption;
		m_captionExistFlag = FALSE;
	}
	else
	{
		sl.caption = m_selectCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_SELECTNAME),m_parentHWND,DialogProc,(LPARAM)&sl);
	return rt;
}


int CSelectDialog::GetSelect(CNameList* lpNameList, int oldSelect,CGameMessageDoc* lpDoc,CFilmDoc* lpDoc2)
{
	MYSELECTDIALOGSTRUCT sl;
	sl.list = lpNameList;
	sl.oldSelect = oldSelect;
	sl.nameType = 0;
	sl.doc = lpDoc;
	sl.doc2 = lpDoc2;

	if (m_captionExistFlag)
	{
		sl.caption = m_caption;
		m_captionExistFlag = FALSE;
	}
	else
	{
		sl.caption = m_selectCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_SELECTNAME),m_parentHWND,DialogProc,(LPARAM)&sl);
	return rt;
}

int CSelectDialog::GetSelect1(CNameList* lpNameList, int oldSelect,CGameMessageDoc* lpDoc,CFilmDoc* lpDoc2)
{
	MYSELECTDIALOGSTRUCT sl;
	sl.list = lpNameList;
	sl.oldSelect = oldSelect;
	sl.nameType = 2;
	sl.doc = lpDoc;
	sl.doc2 = lpDoc2;

	if (m_captionExistFlag)
	{
		sl.caption = m_caption;
		m_captionExistFlag = FALSE;
	}
	else
	{
		sl.caption = m_selectCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_SELECTNAME),m_parentHWND,DialogProc,(LPARAM)&sl);
	return rt;
}



BOOL CALLBACK CSelectDialog::DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LPSTR buff;
	static int maxLength;
	static int typ;
	static BOOL playFlag;
	static CGameMessageDoc* doc;
	static CFilmDoc* doc2;

	HWND edt;


	switch (message)
	{
	case WM_INITDIALOG:
		MYSELECTDIALOGSTRUCT* lpSel;
		lpSel = (MYSELECTDIALOGSTRUCT*)lParam;

		int oldSelect;
		oldSelect = lpSel->oldSelect;

		int kosuu;

		edt = GetDlgItem(hwndDlg,IDC_LIST_NAME);
//		edt = 0;
		int i;


		doc = lpSel->doc;
		doc2 = lpSel->doc2;

		if (lpSel->nameType == 0)
		{
			CNameList* list;
			list = lpSel->list;
	
			kosuu = list->GetNameKosuu();
			kosuu /= 2;

			for (i=0;i<kosuu;i++)
			{
				LPSTR mes;
				mes = list->GetName(i*2+1);
				int place;

				place = SendMessage(edt,LB_INSERTSTRING,(WPARAM)-1,(LPARAM)(mes));
				SendMessage(edt,LB_SETITEMDATA,place,i);
			}
		}
		else if (lpSel->nameType == 2)
		{
			CNameList* list;
			list = lpSel->list;
	
			kosuu = list->GetNameKosuu();

			for (i=0;i<kosuu;i++)
			{
				LPSTR mes;
				mes = list->GetName(i);
				int place;

				place = SendMessage(edt,LB_INSERTSTRING,(WPARAM)-1,(LPARAM)(mes));
				SendMessage(edt,LB_SETITEMDATA,place,i);
			}
		}
		else if (lpSel->nameType == 1)
		{
			kosuu = lpSel->nameKosuu;

			LPSTR* ppMes = lpSel->pNamePtr;

			for (i=0;i<kosuu;i++)
			{
				LPSTR mes;
				mes = *(ppMes+i);
				int place;

				place = SendMessage(edt,LB_INSERTSTRING,(WPARAM)-1,(LPARAM)(mes));
				SendMessage(edt,LB_SETITEMDATA,place,i);
			}

		}


		HWND playButton;
		playButton = GetDlgItem(hwndDlg,IDC_PLAYSOUND);
		if ((doc != NULL) || (doc2 != NULL))
		{
			EnableWindow(playButton,TRUE);
		}
		else
		{
			EnableWindow(playButton,FALSE);
		}



		for (i=0;i<kosuu;i++)
		{
			int dat;
			dat = SendMessage(edt,LB_GETITEMDATA,(WPARAM)i,0);
			if (dat == oldSelect)
			{
				SendMessage(edt,LB_SETCURSEL,(WPARAM)i,0);
				break;
			}
		}




		RECT rc;
		HWND hWndDesktop;
		hWndDesktop = GetDesktopWindow();

		GetWindowRect(hWndDesktop,&rc);
		int dsksx,dsksy;
		dsksx = rc.right - rc.left;
		dsksy = rc.bottom - rc.top;


		GetWindowRect(hwndDlg,&rc);
		int wx,wy,sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;
		wx = dsksx / 2 - sx / 2;
		wy = dsksy / 2 - sy / 2;


		MoveWindow(hwndDlg,wx,wy,sx,sy,FALSE);

		//focus
		HWND hwndList;
		hwndList = GetDlgItem(hwndDlg,IDC_LIST_NAME);
		SetFocus(hwndList);

		return FALSE;

		break;

	case WM_CLOSE:
		EndDialog(hwndDlg,-1);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			//get data

			//IDC_EDIT_NAMEINPUT
			edt = GetDlgItem(hwndDlg,IDC_LIST_NAME);
//			edt = 0;

			int pl;
			pl = SendMessage(edt,LB_GETCURSEL,0,0);
			if (pl == LB_ERR)
			{
				EndDialog(hwndDlg,-1);
			}
			else
			{
				int nn;
				nn = SendMessage(edt,LB_GETITEMDATA,(WPARAM)pl,0);
				EndDialog(hwndDlg,nn);
			}

			break;

		case IDCANCEL:
			EndDialog(hwndDlg,-1);
			break;
		case IDC_PLAYSOUND:
			if (doc != NULL)
			{
				int pl2;
				edt = GetDlgItem(hwndDlg,IDC_LIST_NAME);
				pl2 = SendMessage(edt,LB_GETCURSEL,0,0);
				if (pl2 != LB_ERR)
				{
					int nn2;
					nn2 = SendMessage(edt,LB_GETITEMDATA,(WPARAM)pl2,0);
					doc->PlaySeTest(nn2);
				}
			}
			else if (doc2 != NULL)
			{
				int pl3;
				edt = GetDlgItem(hwndDlg,IDC_LIST_NAME);
				pl3 = SendMessage(edt,LB_GETCURSEL,0,0);
				if (pl3 != LB_ERR)
				{
					int nn3;
					nn3 = SendMessage(edt,LB_GETITEMDATA,(WPARAM)pl3,0);
					doc2->PlayMusicTest(nn3);
				}
			}

			break;
		}
	}

	return FALSE;
}

