//
// mySearchDialog.cpp
//

#include <windows.h>
#include <stdio.h>



#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "myVarSearchDialog.h"


CMyVarSearchDialog::CMyVarSearchDialog(HWND parentHWND,HINSTANCE hinstance, int maxText)
{
	m_parentHWND = parentHWND;
	m_hInstance = hinstance;

	m_maxText = maxText;

	m_radio = 1;
	m_radio2 = 0;

	memcpy(m_searchText,"サーチ文字列",strlen("サーチ文字列")+1);
	memcpy(m_replaceText,"置き換え文字列",strlen("置き換え文字列")+1);

	m_textBuffer = new char[maxText];

	m_windowZahyoFlag = FALSE;


}


CMyVarSearchDialog::~CMyVarSearchDialog()
{
	End();
}

void CMyVarSearchDialog::End(void)
{
	DELETEARRAY(m_textBuffer);
}


int CMyVarSearchDialog::GetCommand(BOOL replaceFlag,BOOL startFlag)
{

	m_replaceFlag = replaceFlag;
//	m_allFlag = allFlag;
	m_startFlag = startFlag;
//	m_endFlag = endFlag;

	int rt = DialogBoxParam(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_VARREPLACE),m_parentHWND,DialogProc,(LPARAM)this);

	return rt;
}

LPSTR CMyVarSearchDialog::GetSearchText(void)
{
	return m_searchText;
}

void CMyVarSearchDialog::SetSearchText(LPSTR text)
{
	if (text == NULL) return;
	int ln = strlen(text);
	if (ln>1022) ln = 1022;
	memcpy(m_searchText,text,ln);
	m_searchText[ln] = 0;
	m_searchText[ln+1] = 1;
}

LPSTR CMyVarSearchDialog::GetReplaceText(void)
{
	return m_replaceText;
}

void CMyVarSearchDialog::SetReplaceText(LPSTR text)
{
	if (text == NULL) return;
	int ln = strlen(text);
	if (ln>1022) ln = 1022;
	memcpy(m_replaceText,text,ln);
	m_replaceText[ln] = 0;
	m_replaceText[ln+1] = 1;
}

void CMyVarSearchDialog::SetDeaultRadio(int radio)
{
	m_radio = radio;
}

void CMyVarSearchDialog::SetDefaultRadio2(int radio2)
{
	m_radio2 = radio2;
}


int CMyVarSearchDialog::GetRadio(void)
{
	return m_radio;
}

int CMyVarSearchDialog::GetRadio2(void)
{
	return m_radio2;
}




BOOL CALLBACK CMyVarSearchDialog::DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CMyVarSearchDialog* thisObject;

	static HWND searchEditBox;
	static HWND replaceEditBox;

	static HWND doButton;
	static HWND allDoButton;


	static HWND blockRadio;
	static HWND allRadio;
//	static HWND filmRadio;
//	static HWND nnnRadio;

	static HWND nowRadio;
	static HWND topRadio;
	static HWND nowEndRadio;

	int radio;
	int radio2;

	int state1;
	int state2;
//	int state3;
//	int state4;

	int state10;
	int state11;
	int state12;

	int returnCommand;

	switch (message)
	{
	case WM_INITDIALOG:
		thisObject = (CMyVarSearchDialog*)lParam;

		searchEditBox = GetDlgItem(hwndDlg,IDC_EDIT_REPLACE_FROM);
		SetWindowText(searchEditBox,thisObject->m_searchText);

		replaceEditBox = GetDlgItem(hwndDlg,IDC_EDIT_REPLACE_TO);
		SetWindowText(replaceEditBox,thisObject->m_replaceText);


		doButton = GetDlgItem(hwndDlg,IDC_BUTTON_REPLACE_DO);
		allDoButton = GetDlgItem(hwndDlg,IDC_BUTTON_REPLACE_ALL);

		if (thisObject->m_replaceFlag)
		{
			if (thisObject->m_startFlag == FALSE)
			{
				EnableWindow(doButton,TRUE);
			}
			else
			{
				EnableWindow(doButton,FALSE);
			}
			EnableWindow(allDoButton,TRUE);
			EnableWindow(replaceEditBox,TRUE);
		}
		else
		{
			EnableWindow(doButton,FALSE);
			EnableWindow(allDoButton,FALSE);
			EnableWindow(replaceEditBox,FALSE);
		}

//		messageRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_MESSAGE);
//		komaRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_KOMA);
//		filmRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_FILM);
//		nnnRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_NNN);
		blockRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_BLOCK);
		allRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_ALLVAR);

		EnableWindow(blockRadio,TRUE);
		EnableWindow(allRadio,TRUE);

		
		radio = thisObject->m_radio;
		radio2 = thisObject->m_radio2;

		state1 = BST_UNCHECKED;
		state2 = BST_UNCHECKED;
//		state3 = BST_UNCHECKED;
//		state4 = BST_UNCHECKED;
		if (radio == 0) state1 = BST_CHECKED;
		if (radio == 1) state2 = BST_CHECKED;
//		if (radio == 2) state3 = BST_CHECKED;
//		if (radio == 3) state4 = BST_CHECKED;

		SendMessage(blockRadio,BM_SETCHECK,(WPARAM)state1,0);
		SendMessage(allRadio,BM_SETCHECK,(WPARAM)state2,0);
//		SendMessage(filmRadio,BM_SETCHECK,(WPARAM)state3,0);
//		SendMessage(nnnRadio,BM_SETCHECK,(WPARAM)state4,0);


		state10 = BST_UNCHECKED;
		state11 = BST_UNCHECKED;
		state12 = BST_UNCHECKED;
		if (radio2 == 0) state10 = BST_CHECKED;
		if (radio2 == 1) state11 = BST_CHECKED;
		if (radio2 == 2) state12 = BST_CHECKED;

		nowRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_NOWSTART2);
		topRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_TOPSTART);
		nowEndRadio = GetDlgItem(hwndDlg,IDC_RADIO_REPLACE_NOWEND);

		SendMessage(nowRadio,BM_SETCHECK,(WPARAM)state10,0);
		SendMessage(topRadio,BM_SETCHECK,(WPARAM)state11,0);
		SendMessage(nowEndRadio,BM_SETCHECK,(WPARAM)state12,0);

		
		SendMessage(searchEditBox,EM_SETSEL,0,-1);

		SendMessage(searchEditBox,EM_SETLIMITTEXT,1022,0);
		SendMessage(replaceEditBox,EM_SETLIMITTEXT,1022,0);


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

		if (thisObject->m_windowZahyoFlag)
		{
			MoveWindow(hwndDlg,thisObject->m_windowX,thisObject->m_windowY,sx,sy,FALSE);
		}
		else
		{
			thisObject->m_windowX = wx;
			thisObject->m_windowY = wy;
			thisObject->m_windowZahyoFlag = TRUE;
			MoveWindow(hwndDlg,wx,wy,sx,sy,FALSE);
		}


		SetFocus(searchEditBox);

		return FALSE;

		break;

	case WM_CLOSE:
		EndDialog(hwndDlg,-1);
		break;

	case WM_MOVE:
		int wwx,wwy;

		wwx = (int)(short)LOWORD(lParam);
		wwy =  (int)(short)HIWORD(lParam);

		wwx -= GetSystemMetrics(SM_CXFIXEDFRAME);
		wwy -= GetSystemMetrics(SM_CYFIXEDFRAME);
		wwy -= GetSystemMetrics(SM_CYCAPTION);

		thisObject->m_windowX = wwx;
		thisObject->m_windowY = wwy;

		thisObject->m_windowZahyoFlag = TRUE;
		break;

	case WM_COMMAND:
		returnCommand = 0;
		switch (wParam)
		{
		case IDC_BUTTON_REPLACE_ALL:
			returnCommand++;
		case IDC_BUTTON_REPLACE_DO:
			returnCommand++;
		case IDC_BUTTON_REPLACE_NEXT:
			

			if (SendMessage(blockRadio,BM_GETCHECK,0,0) == BST_CHECKED) thisObject->m_radio = 0;
			if (SendMessage(allRadio,BM_GETCHECK,0,0) == BST_CHECKED) thisObject->m_radio = 1;
//			if (SendMessage(filmRadio,BM_GETCHECK,0,0) == BST_CHECKED) thisObject->m_radio = 2;
//			if (SendMessage(nnnRadio,BM_GETCHECK,0,0) == BST_CHECKED) thisObject->m_radio = 3;

			if (SendMessage(nowRadio,BM_GETCHECK,0,0) == BST_CHECKED) thisObject->m_radio2 = 0;
			if (SendMessage(topRadio,BM_GETCHECK,0,0) == BST_CHECKED) thisObject->m_radio2 = 1;
			if (SendMessage(nowEndRadio,BM_GETCHECK,0,0) == BST_CHECKED) thisObject->m_radio2 = 2;

			GetWindowText(searchEditBox,thisObject->m_searchText,1022);
			GetWindowText(replaceEditBox,thisObject->m_replaceText,1022);

			EndDialog(hwndDlg,returnCommand);
			break;

		case IDCANCEL:
			EndDialog(hwndDlg,-1);
			break;
		}
	}

	return FALSE;
}

