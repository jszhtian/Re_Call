//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "main.h"

#include "..\..\systemNNN\nyanlib\include\myFile.h"



//#include "checkfiledate.h"

#include "resource.h"

#include "unpackroutine.h"

#pragma comment(lib,"Comctl32.lib")

//bmp jpg file to dwq format

HWND g_staticText;
HWND g_progressBar;
HWND g_progressBar2;

int m_checkButtonTable[]=
{
	IDC_CHECK_TODWQ,
	IDC_CHECK_TOBMPJPG,
	IDC_CHECK_TOVAW,
	IDC_CHECK_TOWAVOGG,
};

//?
int m_checkPackTable[]=
{
	0,0, 1,1, 2,2, -1,-1, 3,4, 5,5, 6,6,
};

//???
char m_packDirTable[][3][16]=
{
	{"ta","",""},
	{"bg","",""},
	{"ev","",""},
	{"sys","",""},
	{"sys","ta",""},
	{"sys","sm","sm"},
	{"sys","sc","sc"},
};


HWND m_processDialog;


BOOL m_bmpFlag = FALSE;
BOOL m_jpgFlag = FALSE;

HWND m_check[9];
HWND m_radio[9];

HWND m_editBox;

int m_checkButtonStatus[4];
int m_version = 0;

char m_versionBuffer[256];

//CCheckFileDate* m_checkDate = NULL;
CUnpackRoutine* m_unpackRoutine = NULL;

void SetCheckButton(int n);
void SetRadioButton(int n);
void GetCheckButton(int n);
void GetRadioButton(int n);

BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

BOOL CheckExistDir(LPSTR dir);
BOOL CheckExistDir(LPSTR srcDir0,LPSTR srcDir1,LPSTR srcDir2);
BOOL CheckAndCreateDir(LPSTR srcDir,LPSTR dstDir);


//void PackMain(LPSTR dirName,BOOL packFlag = FALSE,BOOL GameFlag = FALSE);

//int Pack1(LPSTR filename,LPSTR filename2);
//int Pack0(LPSTR filename,LPSTR filename2,LPSTR filenameonly,BOOL gameFlag = FALSE);


//char* loadBuffer = NULL;
//char* saveBuffer = NULL;


//
// IF PACKTYPE==0__
// CUT THIS 64 BYTE
// THEN REMAKE BMP_
// PACKTYPE=0______			ここはアスキーコードの0から9までの数字がはいる
//

/*
char nopackHeader[]= "\
IF PACKTYPE==0  \
CUT THIS 64 BYTE\
THEN REMAKE BMP \
PACKTYPE=0      ";
*/

char packHeader1[]= "\
IF PACKTYPE==0  \
CUT THIS 64 BYTE\
THEN REMAKE BMP \
PACKTYPE=1      ";

char packHeader2[]= "\
BMP(NORMAL)+MASK\
                \
                \
PACKTYPE=2      ";

char packHeader3[]= "\
BMP(PACK)+MASK  \
                \
                \
PACKTYPE=3      ";

char packHeader5[]= "\
JPEG            \
                \
                \
PACKTYPE=5      ";

char packHeader7[]= "\
JPEG+MASK       \
                \
                \
PACKTYPE=7      ";

FILE* list = NULL;





int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	m_checkButtonStatus[0] = 0;
	m_checkButtonStatus[1] = 0;
	m_checkButtonStatus[2] = 1;
	m_checkButtonStatus[3] = 1;

	FILE* file = NULL;
	if (fopen_s(&file,"devilreverse.cfg","rb") == 0)
	{
		for (int i=0;i<4;i++)
		{
			fread(m_checkButtonStatus,sizeof(int),4,file);
		}
		fclose(file);
	}





	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG_SETUP),NULL,DlgProc,(LPARAM)NULL);
	if (rt != 0) return 1;

	InitCommonControls();

	m_processDialog = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG_PROCESS),NULL,DlgProc2);
	ShowWindow(m_processDialog,SW_SHOW);

	m_unpackRoutine = new CUnpackRoutine();
	m_unpackRoutine->SetProgressWindow(g_staticText,g_progressBar,g_progressBar2);

	CreateDirectory("reverse",NULL);

		CreateDirectory("reverse\\bmp",NULL);
			CreateDirectory("reverse\\bmp\\ev",NULL);
			CreateDirectory("reverse\\bmp\\bg",NULL);
			CreateDirectory("reverse\\bmp\\ta",NULL);
			CreateDirectory("reverse\\bmp\\sys",NULL);
				CreateDirectory("reverse\\bmp\\sys\\ta",NULL);

		CreateDirectory("reverse\\jpg",NULL);
			CreateDirectory("reverse\\jpg\\ev",NULL);
			CreateDirectory("reverse\\jpg\\bg",NULL);
			CreateDirectory("reverse\\jpg\\ta",NULL);
			CreateDirectory("reverse\\jpg\\sys",NULL);
				CreateDirectory("reverse\\jpg\\sys\\ta",NULL);


		CreateDirectory("reverse\\cdwave",NULL);
		CreateDirectory("reverse\\wav",NULL);
			CreateDirectory("reverse\\wav\\se",NULL);
			CreateDirectory("reverse\\wav\\sys",NULL);


		CreateDirectory("reverse\\dwq",NULL);
			CreateDirectory("reverse\\dwq\\ev",NULL);
			CreateDirectory("reverse\\dwq\\bg",NULL);
			CreateDirectory("reverse\\dwq\\ta",NULL);
			CreateDirectory("reverse\\dwq\\sys",NULL);
				CreateDirectory("reverse\\dwq\\sys\\sc",NULL);
				CreateDirectory("reverse\\dwq\\sys\\sm",NULL);


		CreateDirectory("reverse\\cdvaw",NULL);
		CreateDirectory("reverse\\bgm",NULL);
	//	CreateDirectory("reverse\\vaw",NULL);
	//		CreateDirectory("reverse\\vaw\\se",NULL);
	//		CreateDirectory("reverse\\vaw\\sys",NULL);





	if (m_checkButtonStatus[0])
	{
		SendMessage(g_progressBar,PBM_SETPOS,0,0);
		m_unpackRoutine->BunkaiToDWQ(m_version);
	}

	if (m_checkButtonStatus[1])
	{
		SendMessage(g_progressBar,PBM_SETPOS,25,0);
		m_unpackRoutine->ConvertToBMP();

		int erc = m_unpackRoutine->GetDwqErrorCount();
		if (erc > 0)
		{
			char mes[1024];
			wsprintf(mes,"%d個のDWQが変換できませんでした",erc);
			MessageBox(NULL,mes,"エラー",MB_OK | MB_ICONEXCLAMATION);
		}

	}

	if (m_checkButtonStatus[2])
	{
		SendMessage(g_progressBar,PBM_SETPOS,50,0);
		m_unpackRoutine->BunkaiToVAW();
	}

	if (m_checkButtonStatus[3])
	{
		SendMessage(g_progressBar,PBM_SETPOS,75,0);
		m_unpackRoutine->ConvertToWAV();

		int erc = m_unpackRoutine->GetVawErrorCount();
		if (erc > 0)
		{
			char mes[1024];
			wsprintf(mes,"%d個の音声/効果音が変換できませんでした",erc);
			MessageBox(NULL,mes,"エラー",MB_OK | MB_ICONEXCLAMATION);
		}
	}


	if (m_unpackRoutine != NULL)
	{
		m_unpackRoutine->End();
		delete m_unpackRoutine;
		m_unpackRoutine = NULL;
	}


	file = NULL;
	if (fopen_s(&file,"devilreverse.cfg","wb") == 0)
	{
		for (int i=0;i<4;i++)
		{
			fwrite(m_checkButtonStatus,sizeof(int),4,file);
		}
		fclose(file);
	}


	//start dialog



//	SetCursor(oldCursor);

	DestroyWindow(m_processDialog);
	MessageBox(NULL,"おわりましたにゃ","End",MB_OK);

	return 0;
}





LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

} /* MainWndproc */



BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		g_staticText = GetDlgItem(hWnd,IDC_MYSTATIC);
		g_progressBar = GetDlgItem(hWnd,IDC_PROGRESS1);
		g_progressBar2 = GetDlgItem(hWnd,IDC_PROGRESS2);

	//	EnableWindow(m_progressBar,TRUE);
	//	SendMessage(m_progressBar,PBM_SETRANGE,0,MAKELPARAM(0,100));
//		SendMessage(m_progressBar,PBM_SETPOS,70,0);

	//	SetWindowText(m_staticText,"test666");

	//	HWND aaa;
	//	aaa = GetDlgItem(hWnd,IDC_EDIT1);
	//	SetWindowText(aaa,"testdayo");


		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		HWND hwnd0;
		hwnd0 = GetDesktopWindow();

		HDC hdc0;
		hdc0 = GetDC(hwnd0);

		int wx,wy;
		wx = GetDeviceCaps(hdc0,HORZRES);
		wy = GetDeviceCaps(hdc0,VERTRES);
		ReleaseDC(hwnd0,hdc0);

		MoveWindow(hWnd,(wx-sx)/2,(wy-sy)/2,sx,sy,TRUE);

		return TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hWnd,1);

		break;
	}


	return 0;
}



BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int i;

	switch (message)
	{
	case WM_INITDIALOG:
		for (i=0;i<4;i++)
		{
			m_check[i] = GetDlgItem(hWnd,m_checkButtonTable[i]);
			SetCheckButton(i);
		}

		m_editBox = GetDlgItem(hWnd,IDC_EDIT_VERSION);

		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		HWND hwnd0;
		hwnd0 = GetDesktopWindow();

		HDC hdc0;
		hdc0 = GetDC(hwnd0);

		int wx,wy;
		wx = GetDeviceCaps(hdc0,HORZRES);
		wy = GetDeviceCaps(hdc0,VERTRES);
		ReleaseDC(hwnd0,hdc0);

		MoveWindow(hWnd,(wx-sx)/2,(wy-sy)/2,sx,sy,TRUE);

		return 1;
		break;


	case WM_CLOSE:
		EndDialog(hWnd,1);
		break;


	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			for (i=0;i<4;i++)
			{
				GetCheckButton(i);
			}

			if (GetWindowText(m_editBox,m_versionBuffer,32))
			{
				m_version = atoi(m_versionBuffer);
			}

			EndDialog(hWnd,0);
			break;
		case IDCANCEL:
			EndDialog(hWnd,1);
			break;
		}
		break;
	}

	return FALSE;
}


void SetCheckButton(int n)
{
	int st = BST_UNCHECKED;
	if (m_checkButtonStatus[n]) st = BST_CHECKED;

	SendMessage(m_check[n],BM_SETCHECK,st,0);
}


void GetCheckButton(int n)
{
	int st = 0;
	if (SendMessage(m_check[n],BM_GETCHECK,0,0) == BST_CHECKED) st = 1;
	m_checkButtonStatus[n] = st;
}



/*_*/



