//
// main.cpp
//


#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "..\..\systemNNN\nnnUtilLib\mySaveFolder.h"
#include "..\..\systemNNN\nnnUtilLib\getCommandLineParam.h"

#include "..\..\systemNNN\nyanlib\include\myFile.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"
#include "resource.h"

#include "main.h"


//
//単純に起動された場合には、アンインストーラーからの起動でないと判断したとのダイアログを出して終了
//

//アンインストーラーから起動された場合は、
//削除するかどうかのみ確認
//nnnDeleteSave.exe /uninstallでも同じ動作に
//
//init\game.xtxがない場合には、saveFolderType ==  0として処理
//消すフォルダーを表示
//saveFolderType == 1の場合には、上位フォルダー(からっぽのばあいのみ)消すかどうかのチェックボックスをつける




//sysfile.org を移動させるか　-> nyaあたり->しない
//検索順序を sav/sysfile.sav nya/sysfile.org sav/sysfile.org

//init/game.xtx
//saveFolderType : 0 delete current\sav\ 
//



//saveFolderType : 1 (folder設定ある場合のみ) delete mydoc\folder\folder2\sav\ 
//folder設定ある場合のみ
//rmdir folder\folder2\sav 以下 
// rmdir folder\folder2 (if empty)
// rmdir folder(if empty)



//
CNameList* m_list = NULL;


BOOL GetInitGameParam(int* lpVar, LPSTR name);
BOOL GetInitGameString(LPSTR* lpStr, LPSTR name);

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void CutLastFolder(LPSTR pathDir);

HBITMAP m_hBitmap = NULL;


int picSizeX = 80;
int picSizeY = 80;

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	char cmdBuff1[1024];
	char cmdBuff2[1024];
	char* lplpBuff[2];
	lplpBuff[0] = cmdBuff1;
	lplpBuff[1] = cmdBuff2;

	int cmd = CGetCommandLineParam::GetParam(2,1024,lplpBuff,lpCmdLine);

#if defined _DEBUG
	if (cmd == 0)
	{
		MessageBox(NULL,lpCmdLine,"cmdline",MB_OK);
	}
	else
	{
		for (int i=0;i<cmd;i++)
		{
			char mes[1024];
			wsprintf(mes,"param %d ='%s'",i+1,lplpBuff[i]);
			MessageBox(NULL,mes,"cmdline分割",MB_OK);
		}
	}
#endif





	m_hBitmap = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1));

    WNDCLASS wndclass;

    wndclass.style         = NULL;
	wndclass.lpfnWndProc   = MainWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor       = NULL;
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = "nnnDeleteSaveClass";
    RegisterClass(&wndclass);


	DWORD dwExStyle = WS_EX_TOPMOST;



	int desktopWindowSizeX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int desktopWindowSizeY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int desktopWindowStartX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int desktopWindowStartY = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int centerX = desktopWindowStartX + desktopWindowSizeX / 2;
	int centerY = desktopWindowStartY + desktopWindowSizeY / 2;


	HWND hWnd = CreateWindowEx(dwExStyle ,wndclass.lpszClassName,"NekoMeido",
								
								
								WS_POPUP |
								WS_VISIBLE ,

								centerX - picSizeX/2, centerY - picSizeY / 2,
								picSizeX,picSizeY,
								NULL, NULL, hInstance, NULL );






















#if !defined _DEBUG
	if (_stricmp(lpCmdLine,"/Uninstall") != 0)
	{
		MessageBox(hWnd,"アンインストールプログラム以外から使用されようとしています。\n(/Uninstall オプションがついていません)","Error",MB_ICONEXCLAMATION | MB_OK );
		DestroyWindow(hWnd);
		return 0;
	}
#endif


	int rt0 = MessageBox(hWnd,"セーブデータも削除しますか","セーブデータの削除の確認",MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST | MB_TASKMODAL);
	if (rt0 == IDNO)
	{
		DestroyWindow(hWnd);
		return 0;
	}

	//search game.xtx

	char fullname[1024];
	GetModuleFileName(NULL,fullname,sizeof(fullname));

	char drive[16];
	char pathDir[1024];
	char filenameonly[1024];
	char ext[256];
	
	_splitpath_s(fullname,drive,sizeof(drive),pathDir,sizeof(pathDir),filenameonly,sizeof(filenameonly),ext,sizeof(ext));


	if (1)
	{
		CutLastFolder(pathDir);
	}



	int found = FALSE;

	char initFilename[1024];
	char exeDir[1024];
	wsprintf(exeDir,"%s%s",drive,pathDir);
	int ln = strlen(exeDir);
	if (ln >= 4)
	{
		if (exeDir[ln-1] == '\\')
		{
			exeDir[ln-1] = 0;
		}
	}



	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	char checkDir[1024];
	wsprintf(checkDir,"%s%sinit2",drive,pathDir);

	int init2DirFound = 0;
	int initDirFound = 0;
	hFind = FindFirstFile(checkDir,&findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		init2DirFound = 1;
		FindClose(hFind);
	}

	if (init2DirFound == 0)
	{
		wsprintf(checkDir,"%s%sinit",drive,pathDir);

		hFind = FindFirstFile(checkDir,&findFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
		initDirFound = 1;
			FindClose(hFind);
		}
	}

	char currentDir[1024];
	GetCurrentDirectory(1024,currentDir);
	SetCurrentDirectory(exeDir);


	if (init2DirFound)
	{
		wsprintf(initFilename,"%s%sinit2\\game.fxf",drive,pathDir);
		FILE* file = NULL;
		fopen_s(&file,initFilename,"rb");
		if (file == NULL)
		{
			//error no data
			OutputDebugString("\nError");
		}
		else
		{
			fclose(file);
			found = TRUE;
			OutputDebugString("\nFound");
		}
	}
	else if (initDirFound)
	{
		wsprintf(initFilename,"%s%sinit\\game.xtx",drive,pathDir);
		FILE* file = NULL;
		fopen_s(&file,initFilename,"rb");
		if (file == NULL)
		{
			//error no data
			OutputDebugString("\nError");
		}
		else
		{
			fclose(file);
			found = TRUE;
			OutputDebugString("\nFound");
		}
	}




	if (found == FALSE)
	{
		int rt1 = MessageBox(hWnd,"セーブデータの保存場所が確定できませんでした\nカレントにあるセーブデータを削除しますか","",MB_YESNO | MB_ICONEXCLAMATION);
		if (rt1 == IDNO)
		{
			DestroyWindow(hWnd);
			return 0;
		}
	}


	int kakunin = 1;

	int folderType = 0;

	int deleteFolderFlag = 1;
	int deleteSubFolderFlag = 1;
	int orgDeleteFlag = 0;
	int deleteSavOnly = 0;

	if (found)
	{
		m_list = new CNameList();
//		if (m_list->LoadInit(initFilename))
		if (m_list->LoadInit("game"))
		{
			GetInitGameParam(&folderType,"saveFolderType");
			LPSTR saveCompanyFolder = NULL;
			GetInitGameString(&saveCompanyFolder,"saveCompanyFolder");
			LPSTR saveGameFolder = NULL;
			GetInitGameString(&saveGameFolder,"saveGameFolder");

			CMySaveFolder::ChangeMyFolder(saveCompanyFolder,saveGameFolder,folderType);

			GetInitGameParam(&kakunin,"deleteSaveLastConfirm");

			GetInitGameParam(&deleteFolderFlag,"deleteParentSaveFolder");
			GetInitGameParam(&deleteSubFolderFlag,"deleteSubSaveFolder");
			GetInitGameParam(&orgDeleteFlag,"deleteSaveOrg");
			GetInitGameParam(&deleteSavOnly,"deleteOnlySave");
		}
	}


	//削除フォルダー
	char targetDir[1024];
	if (found && (folderType != 0))
	{
		wsprintf(targetDir,"%s",CMySaveFolder::GetFullFolder());
	}
	else
	{
		wsprintf(targetDir,"%s%s",drive,pathDir);
	}

	//最終確認
	if (kakunin)
	{
		char mes[1024];
		if (found && (folderType != 0))
		{
			wsprintf(mes,"以下のセーブファイルを削除します\n\n%s\n\n本当に削除してもよろしいですか",targetDir);
		}
		else
		{
			wsprintf(mes,"以下のセーブファイルを削除します\n\n%s\\sav\n\n本当に削除してもよろしいですか",targetDir);
		}

		int rt3 = MessageBox(hWnd,mes,"最終確認",MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST);
		if (rt3 != IDYES)
		{
			if (m_list != NULL)
			{
				delete m_list;
			}
			DestroyWindow(hWnd);
			return 0;
		}
	}

//	char currentDir[1024];
//	GetCurrentDirectory(1024,currentDir);
	SetCurrentDirectory(exeDir);

	if (CMySaveFolder::DeleteSaveData(deleteFolderFlag,deleteSubFolderFlag,orgDeleteFlag,deleteSavOnly) == FALSE)
	{
		MessageBox(hWnd,"一部ファイルが削除できませんでした","注意",MB_OK | MB_ICONEXCLAMATION);
	}

	SetCurrentDirectory(currentDir);

	if (m_list != NULL)
	{
		delete m_list;
	}

#if defined _DEBUG
	int rt2 = MessageBox(hWnd,"戻り値を0にしますか","?",MB_OKCANCEL);
	if (rt2 != IDOK)
	{
		DestroyWindow(hWnd);
		return 1;
	}
#endif


	DestroyWindow(hWnd);
	return 0;
}





BOOL GetInitGameParam(int* lpVar, LPSTR name)
{
	int rNum = m_list->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpVar = atoi(m_list->GetName(rNum + 1));

	return TRUE;
}

BOOL GetInitGameString(LPSTR* lpStr, LPSTR name)
{
	int rNum = m_list->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpStr = m_list->GetName(rNum + 1);

	return TRUE;
}

void CutLastFolder(LPSTR pathDir)
{
	int ln = strlen(pathDir);
	BOOL found = FALSE;
	BOOL found2 = FALSE;

	int n = 0;
	int lastEn = 0;
	int lastEn2 = 0;

	while (n<ln)
	{
		unsigned char c = *(pathDir+n);
		if (c == '\\')
		{
			if (found)
			{
				found2 = TRUE;
				lastEn2 = lastEn;
			}

			found = TRUE;
			lastEn = n;
		}

		n++;
		int cc = ((int)c) & 0xff;

		//if (c>= (unsigned char)0x80)
		if (((cc >= 0x80) && (cc < 0xa0)) || ((cc >= 0xe0) && (cc < 0x100)))
		{
			n++;
		}
	}

	if (found2)
	{
		*(pathDir+lastEn2+1) = 0;
	}
}





LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (message == WM_DESTROY)
	{
		ShowWindow(hWnd,SW_HIDE);
		PostQuitMessage(0);
		return 0;
	}

	if (message == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		HDC src;
		HGDIOBJ old;

		hdc = BeginPaint(hWnd,&ps);

		src = CreateCompatibleDC(hdc);
		old = SelectObject(src,m_hBitmap);
		BitBlt(hdc,0,0,picSizeX,picSizeY,src,0,0,SRCCOPY);
		SelectObject(src,old);
		DeleteDC(src);

		EndPaint(hWnd,&ps);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

} /* MainWndproc */

/*_*/



