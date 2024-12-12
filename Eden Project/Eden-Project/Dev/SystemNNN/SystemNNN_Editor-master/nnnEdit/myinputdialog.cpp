//
// myinputdialog.cpp
//

#include <windows.h>
#include <stdio.h>



#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "myinputdialog.h"

char CMyInputDialog::m_textCaption[] = "名前入力";
char CMyInputDialog::m_text2Caption[] = "文章入力";
char CMyInputDialog::m_numberCaption[] = "数値入力";

char CMyInputDialog::m_pointCaption[] = "座標入力";
char CMyInputDialog::m_sizeCaption[] = "サイズ入力";
char CMyInputDialog::m_rectCaption[] = "座標・サイズ入力";

CMyInputDialog::CMyInputDialog(HWND parentHWND,HINSTANCE hinstance, int maxText)
{
	m_parentHWND = parentHWND;
	m_hInstance = hinstance;

//	if (maxText > 16384) maxText = 16384;

	m_maxText = maxText;
	m_autoFlag = TRUE;
	m_fullFlag = FALSE;
	m_autoCRFlag = FALSE;

	m_textBuffer = new char[maxText];
}


CMyInputDialog::~CMyInputDialog()
{
	End();
}

void CMyInputDialog::End(void)
{
	DELETEARRAY(m_textBuffer);
}


LPSTR CMyInputDialog::GetText(LPSTR oldText,BOOL suujiFlag,LPSTR captionName)
{
	if (oldText == NULL)
	{
		m_textBuffer[0] = 0;
	}
	else
	{
		int ln = strlen(oldText);
		if (ln>m_maxText-2) ln = m_maxText-2;
		memcpy(m_textBuffer,oldText,ln);
		m_textBuffer[ln] = 0;
		m_textBuffer[ln+1] = 0;
	}

	MYTEXTSTRUCT mt;
	ZeroMemory(&mt,sizeof(mt));

	mt.type = 0;
	mt.maxLength = m_maxText;
	mt.buff = m_textBuffer;
	mt.suujiFlag = suujiFlag;
	mt.autoFlag = FALSE;

	if (captionName != NULL)
	{
		mt.caption = captionName;
	}
	else
	{
		mt.caption = m_textCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_NAMEINPUT),m_parentHWND,DialogProc,(LPARAM)&mt);
	if (rt == 0)
	{
		return m_textBuffer;
	}

	return NULL;
}



LPSTR CMyInputDialog::GetText2(LPSTR oldText,LPSTR captionName)
{
//	m_autoFlag = FALSE;

	if (oldText == NULL)
	{
		m_textBuffer[0] = 0;
	}
	else
	{
		int ln = strlen(oldText);
		if (ln>m_maxText-2) ln = m_maxText-2;
		memcpy(m_textBuffer,oldText,ln);
		m_textBuffer[ln] = 0;
		m_textBuffer[ln+1] = 0;
	}

	MYTEXTSTRUCT mt;
	ZeroMemory(&mt,sizeof(mt));
	mt.type = 1;
	mt.maxLength = m_maxText;
	mt.buff = m_textBuffer;
	mt.suujiFlag = FALSE;
	mt.autoFlag = m_autoFlag;
	mt.fullFlag = FALSE;
	mt.autoCRFlag = FALSE;

	if (captionName != NULL)
	{
		mt.caption = captionName;
	}
	else
	{
		mt.caption = m_text2Caption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_TEXTINPUT),m_parentHWND,DialogProc,(LPARAM)&mt);

	m_autoFlag = mt.autoFlag;
	m_fullFlag = mt.fullFlag;
	m_autoCRFlag = mt.autoCRFlag;

	if (rt == 0)
	{
		return m_textBuffer;
	}

	return NULL;
}


BOOL CMyInputDialog::GetAutoFlag(void)
{
	return m_autoFlag;
}



BOOL CMyInputDialog::GetNumber(int n,int* lpNumber,LPSTR captionName)
{
	if (lpNumber == NULL) return FALSE;

	char oldText[256];
	wsprintf(oldText,"%d",n);

	int ln = strlen(oldText);
	if (ln>m_maxText-2) ln = m_maxText-2;
	memcpy(m_textBuffer,oldText,ln);
	m_textBuffer[ln] = 0;
	m_textBuffer[ln+1] = 0;

	MYTEXTSTRUCT mt;
	ZeroMemory(&mt,sizeof(mt));

	mt.type = 2;
	mt.maxLength = m_maxText;
	mt.buff = m_textBuffer;
	mt.suujiFlag = TRUE;

	if (captionName != NULL)
	{
		mt.caption = captionName;
	}
	else
	{
		mt.caption = m_numberCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_SELECTSUUJI),m_parentHWND,DialogProc,(LPARAM)&mt);
	if (rt == 0)
	{
		int dat = atoi(m_textBuffer);
		*lpNumber = dat;
		return TRUE;
	}

	return FALSE;
}


BOOL CMyInputDialog::GetPoint(POINT pt,POINT* lpPoint,LPSTR captionName)
{
	if (lpPoint == NULL) return FALSE;

	MYTEXTSTRUCT mt;
	ZeroMemory(&mt,sizeof(mt));

	mt.type = 3;
	mt.pt = pt;

	if (captionName != NULL)
	{
		mt.caption = captionName;
	}
	else
	{
		mt.caption = m_pointCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_RECT),m_parentHWND,DialogProc,(LPARAM)&mt);
	if (rt == 0)
	{
		*lpPoint = mt.pt;
		return TRUE;
	}

	return FALSE;
}



BOOL CMyInputDialog::GetSize(SIZE sz,SIZE* lpSize,LPSTR captionName)
{
	if (lpSize == NULL) return FALSE;

	MYTEXTSTRUCT mt;
	ZeroMemory(&mt,sizeof(mt));

	mt.type = 4;
	mt.sz = sz;

	if (captionName != NULL)
	{
		mt.caption = captionName;
	}
	else
	{
		mt.caption = m_sizeCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_RECT),m_parentHWND,DialogProc,(LPARAM)&mt);
	if (rt == 0)
	{
		*lpSize = mt.sz;
		return TRUE;
	}

	return FALSE;
}




BOOL CMyInputDialog::GetRect(RECT rc,RECT* lpRect,LPSTR captionName)
{
	if (lpRect == NULL) return FALSE;

	MYTEXTSTRUCT mt;
	ZeroMemory(&mt,sizeof(mt));

	mt.type = 5;
	mt.pt.x = rc.left;
	mt.pt.y = rc.top;
	mt.sz.cx = rc.right;
	mt.sz.cy = rc.bottom;

	if (captionName != NULL)
	{
		mt.caption = captionName;
	}
	else
	{
		mt.caption = m_rectCaption;
	}

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_RECT),m_parentHWND,DialogProc,(LPARAM)&mt);
	if (rt == 0)
	{
		lpRect->left = mt.pt.x;
		lpRect->top = mt.pt.y;
		lpRect->right = mt.sz.cx;
		lpRect->bottom = mt.sz.cy;
		return TRUE;
	}

	return FALSE;
}



BOOL CALLBACK CMyInputDialog::DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LPSTR buff;
	static int maxLength;
	static int typ;
	static BOOL autoFlag;
	static BOOL fullFlag;
	static BOOL autoCRFlag;
	static LPMYTEXTSTRUCT lpmt;
	static LPSTR caption;

	HWND edt = NULL;
	HWND btn;
	HWND checkbox;

//	char btntext[32];

	char nmbuf[64];

	switch (message)
	{
	case WM_INITDIALOG:
		lpmt = (LPMYTEXTSTRUCT)lParam;

		buff = lpmt->buff;
		maxLength = lpmt->maxLength;
		typ = lpmt->type;
		autoFlag = lpmt->autoFlag;
		fullFlag = lpmt->fullFlag;
		autoCRFlag = lpmt->autoCRFlag;
		caption = lpmt->caption;

		SetWindowText(hwndDlg,caption);

		switch (typ)
		{
		case 0:
			edt = GetDlgItem(hwndDlg,IDC_EDIT_NAMEINPUT);
			SetWindowText(edt,buff);
			SendMessage(edt,EM_SETSEL,0,-1);

			break;
		case 1:
			checkbox = GetDlgItem(hwndDlg,IDC_CHECK_INPUT);
			SendMessage(checkbox,BM_SETCHECK,(WPARAM)autoFlag,(LPARAM)0);

			checkbox = GetDlgItem(hwndDlg,IDC_CHECK_LPRINT);
			SendMessage(checkbox,BM_SETCHECK,(WPARAM)fullFlag,(LPARAM)0);

			checkbox = GetDlgItem(hwndDlg,IDC_CHECK_AUTOCR);
			SendMessage(checkbox,BM_SETCHECK,(WPARAM)autoCRFlag,(LPARAM)0);

			edt = GetDlgItem(hwndDlg,IDC_EDIT_TEXTINPUT);
//			SetWindowText(edt,buff);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)buff);
			SendMessage(edt,EM_SETSEL,0,-1);
			int st;
			int ed;
			SendMessage(edt,EM_GETSEL,(WPARAM)&st,(LPARAM)&ed);

			SendMessage(edt,EM_SETSEL,ed,st);

			break;
		case 2:
			edt = GetDlgItem(hwndDlg,IDC_EDIT_NAMEINPUT);
			SetWindowText(edt,buff);
			SendMessage(edt,EM_SETSEL,0,-1);

			break;
		case 3:
			edt = GetDlgItem(hwndDlg,IDC_EDIT_LEFT);
			wsprintf(nmbuf,"%d",lpmt->pt.x);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);

			edt = GetDlgItem(hwndDlg,IDC_EDIT_TOP);
			wsprintf(nmbuf,"%d",lpmt->pt.y);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);
			break;
		case 4:
			edt = GetDlgItem(hwndDlg,IDC_EDIT_WIDTH);
			wsprintf(nmbuf,"%d",lpmt->sz.cx);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);

			edt = GetDlgItem(hwndDlg,IDC_EDIT_HEIGHT);
			wsprintf(nmbuf,"%d",lpmt->sz.cy);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);
			break;
		case 5:
			edt = GetDlgItem(hwndDlg,IDC_EDIT_LEFT);
			wsprintf(nmbuf,"%d",lpmt->pt.x);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);

			edt = GetDlgItem(hwndDlg,IDC_EDIT_TOP);
			wsprintf(nmbuf,"%d",lpmt->pt.y);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);

			edt = GetDlgItem(hwndDlg,IDC_EDIT_WIDTH);
			wsprintf(nmbuf,"%d",lpmt->sz.cx);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);

			edt = GetDlgItem(hwndDlg,IDC_EDIT_HEIGHT);
			wsprintf(nmbuf,"%d",lpmt->sz.cy);
			SendMessage(edt,EM_SETSEL,0,-1);
			SendMessage(edt,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)nmbuf);
			SendMessage(edt,EM_SETSEL,0,-1);

			break;
		}

		SendMessage(edt,EM_SETLIMITTEXT,maxLength,0);

		if (lpmt->suujiFlag)
		{


			//このフラグは未使用
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

		SetFocus(edt);

		return FALSE;

		break;

	case WM_CLOSE:
		EndDialog(hwndDlg,1);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			//get data

			//IDC_EDIT_NAMEINPUT

			switch (typ)
			{
			case 0:
				edt = GetDlgItem(hwndDlg,IDC_EDIT_NAMEINPUT);
				GetWindowText(edt,buff,maxLength);
				break;
			case 1:
				checkbox = GetDlgItem(hwndDlg,IDC_CHECK_INPUT);
				if (SendMessage(checkbox,BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
				{
					lpmt->autoFlag = TRUE;
				}
				else
				{
					lpmt->autoFlag = FALSE;
				}

				checkbox = GetDlgItem(hwndDlg,IDC_CHECK_LPRINT);
				if (SendMessage(checkbox,BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
				{
					lpmt->fullFlag = TRUE;
				}
				else
				{
					lpmt->fullFlag = FALSE;
				}


				checkbox = GetDlgItem(hwndDlg,IDC_CHECK_AUTOCR);
				if (SendMessage(checkbox,BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
				{
					lpmt->autoCRFlag = TRUE;
				}
				else
				{
					lpmt->autoCRFlag = FALSE;
				}

				edt = GetDlgItem(hwndDlg,IDC_EDIT_TEXTINPUT);
//				SendMessage(edt,EM_SETSEL,0,-1);
				GetWindowText(edt,buff,maxLength);
//				SendMessage(edt,EM_GETSELTEXT,(WPARAM)0,(LPARAM)buff);
				break;
			case 2:
				edt = GetDlgItem(hwndDlg,IDC_EDIT_NAMEINPUT);
				GetWindowText(edt,buff,maxLength);
				break;
			case 3:
				edt = GetDlgItem(hwndDlg,IDC_EDIT_LEFT);
				GetWindowText(edt,nmbuf,62);
				lpmt->pt.x = atoi(nmbuf);

				edt = GetDlgItem(hwndDlg,IDC_EDIT_TOP);
				GetWindowText(edt,nmbuf,62);
				lpmt->pt.y = atoi(nmbuf);

				break;
			case 4:
				edt = GetDlgItem(hwndDlg,IDC_EDIT_WIDTH);
				GetWindowText(edt,nmbuf,62);
				lpmt->sz.cx = atoi(nmbuf);

				edt = GetDlgItem(hwndDlg,IDC_EDIT_HEIGHT);
				GetWindowText(edt,nmbuf,62);
				lpmt->sz.cy = atoi(nmbuf);
				break;
			case 5:
				edt = GetDlgItem(hwndDlg,IDC_EDIT_LEFT);
				GetWindowText(edt,nmbuf,62);
				lpmt->pt.x = atoi(nmbuf);

				edt = GetDlgItem(hwndDlg,IDC_EDIT_TOP);
				GetWindowText(edt,nmbuf,62);
				lpmt->pt.y = atoi(nmbuf);

				edt = GetDlgItem(hwndDlg,IDC_EDIT_WIDTH);
				GetWindowText(edt,nmbuf,62);
				lpmt->sz.cx = atoi(nmbuf);

				edt = GetDlgItem(hwndDlg,IDC_EDIT_HEIGHT);
				GetWindowText(edt,nmbuf,62);
				lpmt->sz.cy = atoi(nmbuf);

				break;
			}

			EndDialog(hwndDlg,0);
			break;

		case IDCANCEL:
			EndDialog(hwndDlg,1);
			break;
		case IDC_BUTTON_0:
		case IDC_BUTTON_1:
		case IDC_BUTTON_2:
		case IDC_BUTTON_3:
		case IDC_BUTTON_4:
		case IDC_BUTTON_5:
		case IDC_BUTTON_6:
		case IDC_BUTTON_7:
		case IDC_BUTTON_8:
		case IDC_BUTTON_9:
		case IDC_BUTTON_10:
		case IDC_BUTTON_15:
		case IDC_BUTTON_20:
		case IDC_BUTTON_30:
		case IDC_BUTTON_40:
		case IDC_BUTTON_50:
			edt = GetDlgItem(hwndDlg,IDC_EDIT_NAMEINPUT);
//			SendMessage(edt,EM_SETSEL,0,-1);
			btn = GetDlgItem(hwndDlg,wParam);
//			GetWindowText(btn,btntext,32);
			GetWindowText(btn,buff,maxLength);
//			SetWindowText(edt,btntext);
			EndDialog(hwndDlg,0);
			break;
		}
	}

	return FALSE;
}

int CMyInputDialog::TextToSuuji(void)
{
	if (m_textBuffer == NULL) return 0;

	int d = 0;
	int ln = strlen(m_textBuffer);
	for (int i=0;i<ln;i++)
	{
		int c = m_textBuffer[i];
		c &= 0xff;
		c -= '0';
		if ((c<0) || (c>=10)) break;
		d *= 10;
		d += c;
	}

	return d;
}

