//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "..\..\systemNNN\nnnUtilLib\currentDirControl.h"

#include "main.h"





//#include "checkfiledate.h"

#include "resource.h"

//#include "packroutine.h"

#pragma comment(lib,"Comctl32.lib")

//bmp jpg file to dwq format

HWND g_staticText;
HWND g_progressBar;
HWND g_progressBar2;


HWND m_processDialog;
HWND m_editBox;

int m_version = 0;

int m_bar = 0;

//BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );


BOOL GetSearchInfo(void);
BOOL GetInstallDirectory(void);


FILE* list = NULL;


//
// program
// info.txt			見るべきレジストリー等が書いてある。
//					reg bara				の形式
//					mutex BaraByTinyan		の形式
//					dir patch				の形式	ない場合はデータのコピーはしない。（レジストリーの修正のみの場合など）
//					gamename 闇の声			の形式	ない場合は単に「ゲーム」になる				
//					result on				の形式	on:結果表示あり
//					readonly 1				の形式	ない場合は0として扱う 0:上書きしない(アップデートは失敗する) 1:確認後上書き 2:強制的に上書き
//					companyName cyc			の形式	ない場合はcyc
//					versionCheck			0しない 1:チェックしてはじく 2:警告して選択
//					lowVersion				100	チェック用の最低バージョン
//					highVersion				100	チェック用の最高バージョン
//					copyNumber				0	コピーするファイルの総数
//					hidden 1				の形式	ない場合は0として扱う 0:上書きしない(アップデートは失敗する) 1:確認後上書き 2:強制的に上書き
//					system 0				の形式	ない場合は0として扱う 0:上書きしない(アップデートは失敗する) 1:確認後上書き 2:強制的に上書き　(このフラグは非推奨です)
//					currentuser 0			reg KEY 0:LOCAL_MACHINE 1:CURRENT_USER					
//					createSubFolder			0 1:サブフォルダ以降も作成する 2:２階層目のサブフォルダーから作成する
//
// patch\			パッチ用のデータがはいっている。これをそのままコピーする(ディレクトリーそのまま)
//
//
//


#define INFOKOSUU (17)

char m_infoList[INFOKOSUU][256]=
{
	"dummy","reg","mutex","dir","gamename","result","readonly","companyName","versionCheck","lowVersion","middleVersion","highVersion","copyNumber","systemfile","hiddenfile","currentUser","createSubFolder"
};

char m_infoData[INFOKOSUU][256];

BOOL m_infoFlag[INFOKOSUU];

#define NOGAME_MESSAGE "「%s」がインストールされていません"


int SearchInfo(LPSTR infoname);
BOOL CopyAllFile(LPSTR srcDir, LPSTR dstDir,int level = 0);
void PrintNotInstallMessage(void);
void SetInfoNumber(void);


BOOL MyCopyFile(LPSTR srcFile, LPSTR dstFile,BOOL flg = FALSE);

void PrintNoGameXTX(void);
void PrintLowVersion(void);
void PrintHighVersion(void);
void PrintBadVersion(LPSTR desc);

int m_updateFileKosuu = 0;
int m_infoReadonlyNumber = 0;

int m_infoRegNumber;
int m_infoMutexNumber;
int m_infoDirNumber;
int m_infoGameNameNumber;
int m_infoResultNumber;
int m_infoCompaneNumber;

int m_infoVersionCheckNumber;
int m_infoLowVersionNumber;
int m_infoMiddleVersionNumber;
int m_infoHighVersionNumber;

int m_infoCopyKosuuNumber;

int m_currentUserNumber;

int m_createSubFolderNumber;

int m_copyKosuu = 0;
int m_copyedKosuu = 0;


int m_createSubFolderLevel = 0;

int m_versionCheck = 0;
int m_checkLowVersion = 100;
int m_checkHighVersion = 100;

char m_instDir[1024];
char m_destDir[1024];
char m_regName[1024];

HKEY hkey;

int m_gameVersion = 100;

int CheckVersion(void);

BOOL GetGameXTX(void);



int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	CCurrentDirControl::ChangeCurrentToExe();

	SetInfoNumber();

	char mes[1024];
	if (GetSearchInfo() == FALSE)
	{
		MessageBox(NULL,"インフォメーションファイル info.txt がみつかりません","ERROR",MB_OK | MB_ICONEXCLAMATION);
		CCurrentDirControl::RestoreCurrent();
		return 1;
	}

	if (m_infoFlag[m_infoRegNumber] == FALSE)
	{
		MessageBox(NULL,"info.txt に、レジストリーの指定がありません","ERROR",MB_OK | MB_ICONEXCLAMATION);
		CCurrentDirControl::RestoreCurrent();
		return 2;
	}

	if (GetInstallDirectory() == FALSE)
	{
		PrintNotInstallMessage();
		CCurrentDirControl::RestoreCurrent();
		return 3;
	}

	wsprintf(mes,"以下のディレクトリーを修正してよろしいですか?\n発見した場所:%s",m_instDir);
	int rt = MessageBox(NULL,mes,"確認",MB_OKCANCEL);
	if (rt  != IDOK)
	{
		MessageBox(NULL,"中止しました","中止",MB_OK);
		CCurrentDirControl::RestoreCurrent();
		return -1;
	}

	if (m_infoFlag[m_createSubFolderNumber])
	{
		m_createSubFolderLevel = atoi(m_infoData[m_createSubFolderNumber]);
	}


	int versionResult = CheckVersion();
	if (versionResult == -9999)
	{
		PrintNoGameXTX();
		CCurrentDirControl::RestoreCurrent();
		return 5;
	}
	if (versionResult < 0)
	{
		PrintLowVersion();
		CCurrentDirControl::RestoreCurrent();
		return 6;
	}
	else if (versionResult > 0)
	{
		PrintHighVersion();
		CCurrentDirControl::RestoreCurrent();
		return 7;
	}



	HANDLE mx = NULL;
	if (m_infoFlag[m_infoMutexNumber])
	{
		mx = CreateMutex(NULL,TRUE,m_infoData[m_infoMutexNumber]);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL,"二重起動及び、ゲーム動作中は起動できません","Error",MB_OK | MB_ICONEXCLAMATION);
			CCurrentDirControl::RestoreCurrent();
			return 8;
		}
	}

	if (m_infoFlag[m_infoCopyKosuuNumber])
	{
		m_copyKosuu = atoi(m_infoData[m_infoCopyKosuuNumber]);
	}

	InitCommonControls();

	m_processDialog = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG_PROCESS),NULL,DlgProc2);
	ShowWindow(m_processDialog,SW_SHOW);



	if (m_infoDirNumber > 0)
	{
		if (m_infoFlag[m_infoDirNumber])
		{
			if (CopyAllFile(m_infoData[m_infoDirNumber],m_destDir) == FALSE)
			{
				if (mx != NULL)
				{
					ReleaseMutex( mx );
				}

				MessageBox(NULL,"ファイルのコピーに失敗しました\nこの状態ではゲームは正しく遊べません\n再度修正プログラムを実行してください","重大エラー",MB_OK | MB_ICONEXCLAMATION);
				
				DestroyWindow(m_processDialog);
		
				CCurrentDirControl::RestoreCurrent();
				return 4;
			}
		}
	}

	DestroyWindow(m_processDialog);


	if (mx != NULL)
	{
		ReleaseMutex( mx );
	}

	BOOL resultFlag = FALSE;
	if (m_infoFlag[m_infoResultNumber])
	{
		if (_stricmp(m_infoData[m_infoResultNumber],"on") == 0)
		{
			resultFlag = TRUE;
		}
	}

	if (resultFlag)
	{
		wsprintf(mes,"%d個のファイルを修正又は追加しました。\nアップデートは正常に終了しました",m_updateFileKosuu);
		MessageBox(NULL,mes,"正常終了",MB_OK);
	}
	else
	{
		MessageBox(NULL,"アップデートは正常に終了しました","正常終了",MB_OK);
	}

	CCurrentDirControl::RestoreCurrent();

	return 0;
}

void SetInfoNumber(void)
{
	m_infoRegNumber = SearchInfo("reg");
	m_infoMutexNumber = SearchInfo("mutex");
	m_infoDirNumber = SearchInfo("dir");
	m_infoGameNameNumber = SearchInfo("gamename");
	m_infoResultNumber = SearchInfo("result");
	m_infoCompaneNumber = SearchInfo("companyName");
	m_infoReadonlyNumber = SearchInfo("readonly");
	m_infoVersionCheckNumber = SearchInfo("versionCheck");
	m_infoLowVersionNumber = SearchInfo("lowVersion");
	m_infoHighVersionNumber = SearchInfo("highVersion");
	m_infoCopyKosuuNumber = SearchInfo("copyNumber");
	m_currentUserNumber = SearchInfo("currentUser");
	m_createSubFolderNumber = SearchInfo("createSubFolder");
}

BOOL GetSearchInfo(void)
{
	FILE* file = NULL;

	fopen_s(&file,"info.txt","rt");
	if (file == NULL)
	{
		return FALSE;
	}


	char name[256];
	char tmp[256];

	name[0] = 0;
	
	for (int i=0;i<INFOKOSUU;i++)
	{
		m_infoData[i][0] = 0;
		m_infoFlag[i] = FALSE;
	}

	while (fscanf_s(file,"%s",name,256) != EOF)
	{
		if ( ((*name) =='/') && ((*(name+1)) == '/') ) continue;
		if (fscanf_s(file,"%s",tmp,256) == EOF) break;

		int found = -1;
		for (int i=0;i<INFOKOSUU;i++)
		{
			if (_stricmp(name,m_infoList[i]) == 0)
			{
				found = i;
				break;
			}
		}

		if (found != -1)
		{
			int ln = strlen(tmp);
			if (ln > 254) ln = 254;
			memcpy(m_infoData[found],tmp,ln);
			m_infoData[found][ln] = 0;
			m_infoData[found][ln+1] = 0;
			m_infoFlag[found] = TRUE;
		}
	}
	
	fclose(file);
	return TRUE;
}


int CheckVersion(void)
{
	if (m_infoFlag[m_infoVersionCheckNumber] == FALSE)
	{
		return 0;	//no check ok
	}
	if (atoi(m_infoData[m_infoVersionCheckNumber]) == 0)
	{
		return 0;
	}

	if (m_infoFlag[m_infoLowVersionNumber])
	{
		m_checkLowVersion = atoi(m_infoData[m_infoLowVersionNumber]);
	}
	
	if (m_infoFlag[m_infoHighVersionNumber])
	{
		m_checkHighVersion = atoi(m_infoData[m_infoHighVersionNumber]);
	}

	if (GetGameXTX() == FALSE)
	{
		return -9999;
	}

	if (m_gameVersion < m_checkLowVersion) return -1;
	if (m_gameVersion > m_checkHighVersion) return 1;

	return 0;
}


BOOL GetGameXTX(void)
{
	CNameList* nameList = new CNameList();
	if (nameList->LoadInit("game") == FALSE)
	{
		return FALSE;
	}

	int lowVersion = 0;
	int middleVersion = 0;
	int highVersion = 0;

	int n1 = nameList->SearchBlock("lowVersion");
	if (n1 != -1)
	{
		lowVersion = atoi(nameList->GetName(n1*2+1));
	}

	int n2 = nameList->SearchBlock("middleVersion");
	if (n2 != -1)
	{
		middleVersion = atoi(nameList->GetName(n2*2+1));
	}

	int n3 = nameList->SearchBlock("highVersion");
	if (n3 != -1)
	{
		highVersion = atoi(nameList->GetName(n3*2+1));
	}




	m_gameVersion = (highVersion+1) * 100 + middleVersion *10 + lowVersion;
	return TRUE;
}


BOOL GetInstallDirectory(void)
{
	BOOL installFlag = FALSE;
	if (m_infoFlag[m_infoCompaneNumber])
	{
		wsprintf(m_regName,"Software\\%s\\%s",m_infoData[m_infoCompaneNumber],m_infoData[m_infoRegNumber]);
	}
	else
	{
		wsprintf(m_regName,"Software\\cyc\\%s",m_infoData[m_infoRegNumber]);
	}



	int n= 0;
	int ln = strlen(m_regName);
	while (n<ln)
	{
		char c = m_regName[n];
		if (c == '?')
		{
			m_regName[n] = ' ';
		}
		if (c & 0x80)
		{
			n++;
		}
		n++;
	}



	LONG result;
	if (m_infoFlag[m_currentUserNumber] == 0)
	{
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,m_regName,0,KEY_READ,&hkey);
	}
	else
	{
		result = RegOpenKeyEx(HKEY_CURRENT_USER,m_regName,0,KEY_READ,&hkey);
	}

	if (result == ERROR_SUCCESS)
	{
		DWORD nType;
		DWORD pcbData = 256;
		if (RegQueryValueEx(hkey,"installdirectory",NULL,&nType,(unsigned char*)m_instDir,&pcbData) == ERROR_SUCCESS)
		{		
			installFlag = TRUE;
		}
		RegCloseKey(hkey);
	}


	if (installFlag)
	{
		BOOL bShort = FALSE;
		char shortDir[1024];
		if (GetShortPathName(m_instDir,shortDir,1023) > 0)
		{
			bShort = TRUE;
		}

		if (bShort == FALSE)
		{
			wsprintf(m_destDir,"%s",m_instDir);
		}
		else
		{
			wsprintf(m_destDir,"%s",shortDir);
		}
	}

	return installFlag;
}

void PrintNotInstallMessage(void)
{
	char mes[1024];
	if (m_infoFlag[m_infoGameNameNumber])
	{
		wsprintf(mes,NOGAME_MESSAGE,m_infoData[m_infoGameNameNumber]);
	}
	else
	{
		wsprintf(mes,NOGAME_MESSAGE,"ゲーム");
	}
	
	MessageBox(NULL,mes,"ERROR",MB_OK | MB_ICONEXCLAMATION);
}

void PrintNoGameXTX(void)
{
	MessageBox(NULL,"ゲームのバージョン情報が取得できませんでした","ERROR",MB_OK | MB_ICONEXCLAMATION);
}

void PrintLowVersion(void)
{
	PrintBadVersion("バージョンが古いためアップデートできません");
}

void PrintHighVersion(void)
{
	PrintBadVersion("ゲームのほうがが新しいためアップデートの必要はありません");
}

void PrintBadVersion(LPSTR desc)
{
	char mes[1024];
	int d = m_gameVersion;
	int h1 = d / 100;
	d -= h1*100;
	int m1 = d / 10;
	int l1 = d % 10;

	int d2 = m_checkLowVersion;
	int h2 = d2 / 100;
	d2 -= h2*100;
	int m2 = d2 / 10;
	int l2 = d2 % 10;

	int d3 = m_checkHighVersion;
	int h3 = d3 / 100;
	d3 -= h3*100;
	int m3 = d3 / 10;
	int l3 = d3 % 10;

	wsprintf(mes,"%s\n\nインストールVer%d.%d%d\nバージョンアップ可能なものは以下のものです\nVer%d.%d%d ～ %d.%d%d",desc,h1,m1,l1,h2,m2,l2,h3,m3,l3);
	MessageBox(NULL,mes,"このバージョンはアップデートできません",MB_OK | MB_ICONEXCLAMATION);
}


int SearchInfo(LPSTR infoname)
{
	for (int i=1;i<INFOKOSUU;i++)
	{
		if (_stricmp(infoname,m_infoList[i]) == 0) return i;
	}

	return 0;
}



//	int pos = (100*i*3+50)/21;
//	SendMessage(g_progressBar,PBM_SETPOS,pos,0);
BOOL CopyAllFile(LPSTR srcDir, LPSTR dstDir,int level)
{
	WIN32_FIND_DATA findData;

	
	char startFile[256];
	wsprintf(startFile,"%s\\*",srcDir);
	HANDLE handle = FindFirstFile(startFile,&findData);

	if (handle != INVALID_HANDLE_VALUE)
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((*(findData.cFileName)) != '.')
			{
				char srcDir2[1024];
				char dstDir2[1024];
				wsprintf(srcDir2,"%s\\%s",srcDir,findData.cFileName);
				wsprintf(dstDir2,"%s\\%s",dstDir,findData.cFileName);


				if ((m_infoFlag[m_createSubFolderNumber]) && (level+1 >= m_createSubFolderLevel))
				{
//					MessageBox(NULL,dstDir2,"createSubFolder",MB_OK);

					CreateDirectory(dstDir2,NULL);
				}

				if (CopyAllFile(srcDir2,dstDir2,level+1) == FALSE) return FALSE;
			}
		}
		else
		{
			if ((*(findData.cFileName)) != '.')
			{
				char srcFile[1024];
				char dstFile[1024];
				wsprintf(srcFile,"%s\\%s",srcDir,findData.cFileName);
				wsprintf(dstFile,"%s\\%s",dstDir,findData.cFileName);
				if (MyCopyFile(srcFile,dstFile,FALSE) == FALSE)
				{
					return FALSE;
				}
				m_updateFileKosuu++;
			}
		}

		WIN32_FIND_DATA findData2;
		while (FindNextFile(handle,&findData2) != 0)
		{
			if (findData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((*(findData2.cFileName)) != '.')
				{
					char srcDir2[1024];
					char dstDir2[1024];
					wsprintf(srcDir2,"%s\\%s",srcDir,findData2.cFileName);
					wsprintf(dstDir2,"%s\\%s",dstDir,findData2.cFileName);

					if ((m_infoFlag[m_createSubFolderNumber]) && (level+1 >= m_createSubFolderLevel))
					{
		//				MessageBox(NULL,dstDir2,"createSubFolder",MB_OK);

						CreateDirectory(dstDir2,NULL);
					}

					if (CopyAllFile(srcDir2,dstDir2,level+1) == FALSE) return FALSE;
				}
			}
			else
			{
				if ((*(findData2.cFileName)) != '.')
				{
					char srcFile[1024];
					char dstFile[1024];
					wsprintf(srcFile,"%s\\%s",srcDir,findData2.cFileName);
					wsprintf(dstFile,"%s\\%s",dstDir,findData2.cFileName);
					if (MyCopyFile(srcFile,dstFile,FALSE) == FALSE)
					{
						return FALSE;
					}
					m_updateFileKosuu++;
				}
			}
		}
		FindClose(handle);
	}

	return TRUE;
}



BOOL MyCopyFile(LPSTR srcFile, LPSTR dstFile,BOOL flg)
{

	char mes[1024];
	wsprintf(mes,"copy:%s",srcFile);
	SetWindowText(g_staticText,mes);
	UpdateWindow(g_staticText);

	SendMessage(g_progressBar,PBM_SETPOS,m_bar % 100,0);
	m_bar += 10;
	m_bar %= 100;

	m_copyedKosuu++;
	if (m_copyKosuu > 0)
	{
		int ps0 = ((m_copyedKosuu-1) * 100) / m_copyKosuu;
		int ps = (m_copyedKosuu * 100) / m_copyKosuu;
		if (ps != ps0)
		{
			SendMessage(g_progressBar2,PBM_SETPOS,ps,0);
		}
	}

	if (m_bar == 0)
	{
		wsprintf(mes,"コピー中 %s",srcFile);
		SetWindowText(g_staticText,mes);
		UpdateWindow(g_staticText);
	}

	BOOL rt = CopyFile(srcFile,dstFile,FALSE);
	if (rt) return TRUE;

	DWORD at = GetFileAttributes(dstFile);

	if (at & FILE_ATTRIBUTE_READONLY)
	{
		int type = m_infoFlag[m_infoReadonlyNumber];
		if (type == 0) return FALSE;
		
		
		if (type == 1)
		{
			wsprintf(mes,"コピー先のファイルが読み取り専用になっています\n%s\n\n上書きしますか?",dstFile);
			int rt = MessageBox(NULL,mes,"読み取り専用です",MB_YESNO);
			if (rt != IDYES) return FALSE;
		}
		at &= (~FILE_ATTRIBUTE_READONLY);
	}

	if (at & FILE_ATTRIBUTE_HIDDEN)
	{
		int type = m_infoFlag[m_infoReadonlyNumber];
		if (type == 0) return FALSE;
		
		
		if (type == 1)
		{
			wsprintf(mes,"コピー先のファイルが隠しファイルになっています\n%s\n\n上書きしますか?",dstFile);
			int rt = MessageBox(NULL,mes,"隠しファイルです",MB_YESNO);
			if (rt != IDYES) return FALSE;
		}
		at &= (~FILE_ATTRIBUTE_HIDDEN);
	}

	if (at & FILE_ATTRIBUTE_SYSTEM)
	{
		int type = m_infoFlag[m_infoReadonlyNumber];
		if (type == 0) return FALSE;
		
		
		if (type == 1)
		{
			wsprintf(mes,"コピー先のファイルはシステムファイルです\n%s\n\n上書きしますか(非推奨)?",dstFile);
			int rt = MessageBox(NULL,mes,"システムファイルです",MB_YESNO);
			if (rt != IDYES) return FALSE;
		}
		at &= (~FILE_ATTRIBUTE_SYSTEM);
	}

	if (SetFileAttributes(dstFile,at) == FALSE)
	{
		MessageBox(NULL,"ファイルの属性を変更できませんでした","エラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return CopyFile(srcFile,dstFile,FALSE);
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


/*
BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int i;

	switch (message)
	{
	case WM_INITDIALOG:
		for (i=0;i<9;i++)
		{
			m_check[i] = GetDlgItem(hWnd,m_checkButtonTable[i]);
			SetCheckButton(i);
		}

		for (i=0;i<9;i++)
		{
			m_radio[i] = GetDlgItem(hWnd,m_radioButtonTable[i]);
			SetRadioButton(i);
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
			for (i=0;i<9;i++)
			{
				GetCheckButton(i);
			}
			for (i=0;i<2;i++)
			{
				GetRadioButton(i);
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


void SetRadioButton(int n)
{
	int st = BST_UNCHECKED;
	if (m_radioButtonStatus[n]) st = BST_CHECKED;

	SendMessage(m_radio[n],BM_SETCHECK,st,0);
}

void GetCheckButton(int n)
{
	int st = 0;
	if (SendMessage(m_check[n],BM_GETCHECK,0,0) == BST_CHECKED) st = 1;
	m_checkButtonStatus[n] = st;
}

void GetRadioButton(int n)
{
	int st = 0;
	if (SendMessage(m_radio[n],BM_GETCHECK,0,0) == BST_CHECKED) st = 1;
	m_radioButtonStatus[n] = st;
}
*/



/*_*/



