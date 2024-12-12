#include "Stdafx.h"
#include "ConvertToIPA.h"
#include "Common.h"
#include "resource.h"

#define CONTENT			"</dict>\n</plist>"
#define REPLACE_CONTENT	"\t<key>SignerIdentity</key>\n\t<string>Apple iPhone OS Application Signing</string>\n</dict>\n</plist>"

HWND hDlgWnd = NULL;
HBRUSH brsh;
HICON hIcon;

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, 
						 WPARAM wParam, LPARAM lParam);

BOOL ConvertPXLToIPA(IN const TCHAR *pszWinRAR, 
					 IN const TCHAR *pszPXL, 
					 IN const TCHAR *pszIPA);

VOID GenarateCmd(IN const TCHAR *pszWinRAR, 
				 IN const TCHAR *szZipFile, 
				 IN const TCHAR *pszPXL,
				 OUT TCHAR *pszCmd);

VOID CenterWindow(HWND hwndDlg);

BOOL FixInfoList(const wstring &wstrInfoList);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					 LPSTR lpCmdLine, int nShowCmd)
{
	
	brsh = CreateSolidBrush(RGB(228, 232, 235));
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

	hDlgWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN_DLG), NULL, DialogProc);
	if (NULL == hDlgWnd)
	{
		MessageBox(NULL, TEXT("Application Error!"), 
			TEXT("ERROR!"), MB_ICONERROR|MB_OK);
		return -1;
	}
	
	ShowWindow(hDlgWnd, SW_SHOWNORMAL);
	UpdateWindow(hDlgWnd);

	MSG msg;
	BOOL bRet;
	while ( (bRet = GetMessage(&msg, NULL, 0, 0)) != 0 ) 
	{ 
		if (bRet == -1 )
		{
			// handle the error and possibly exit
		}
		else if (!IsWindow(hDlgWnd) || !IsDialogMessage(hDlgWnd, &msg)) 
		{ 
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		} 
	}

	return msg.wParam;
}


BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
    {
	case WM_INITDIALOG:
	{
		SendMessage(hwndDlg, WM_SETICON, (WPARAM)(FALSE), (LPARAM)hIcon);
		SendMessage(hwndDlg, WM_SETICON, (WPARAM)(TRUE), (LPARAM)hIcon);
	
		return TRUE;
	}
	
	// DialogColor
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORBTN:
		return (INT_PTR)brsh;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR)brsh;

	case WM_COMMAND:
	{
		TCHAR psPath[260] = {0};
		
		switch (LOWORD(wParam))
		{ 
		case ID_CONVERT:
			{
				TCHAR pstrWinRar[260] = {0};
				TCHAR pstrPXL[260] = {0};
				TCHAR pstrIPA[260] = {0};

				GetDlgItemText(hwndDlg, IDC_WINRAR_PATH, pstrWinRar, 260);
				GetDlgItemText(hwndDlg, IDC_PXL_PATH, pstrPXL, 260);
				GetDlgItemText(hwndDlg, IDC_IPA_PATH, pstrIPA, 260);

				if (ConvertPXLToIPA(pstrWinRar, pstrPXL, pstrIPA))
					return TRUE;
			}
			break;
			
		case IDCANCEL: 
			DestroyWindow(hwndDlg);
			hDlgWnd = NULL;
			return TRUE;
			
		case IDC_WINRAR_BTN:
			
			if (OpenCommonFileDlg(hwndDlg, TEXT("WinRAR.EXE\0WinRAR.EXE\0"), psPath))
			{
				SetDlgItemText(hwndDlg, IDC_WINRAR_PATH, psPath);
				return TRUE;
			}
			break;
			
		case IDC_PXL_BTN:
			if (OpenCommonFileDlg(hwndDlg, TEXT("All\0*.*\0PXL\0*.PXL\0"), psPath))
			{
				SetDlgItemText(hwndDlg, IDC_PXL_PATH, psPath);
				return TRUE;
			}
			break;
			
		case IDC_IPA_BTN:
			if (SaveCommonFileDlg(hwndDlg, TEXT("All\0*.*\0IPA\0*.IPA\0"), psPath))
			{
				wstring szTempIPA = psPath;
				if (wstring::npos == szTempIPA.find(TEXT(".ipa")))
					szTempIPA.append(TEXT(".ipa"));

				SetDlgItemText(hwndDlg, IDC_IPA_PATH, szTempIPA.c_str());
				return TRUE;
			}
			break;
		}
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(wParam);
		return TRUE;
    }

	return FALSE;
}


BOOL ConvertPXLToIPA(const TCHAR *pszWinRAR, const TCHAR *pszPXL, const TCHAR *pszIPA)
{
	ASSERT(NULL != pszWinRAR);
	ASSERT(NULL != pszIPA);
	ASSERT(NULL != pszPXL);

	if (!IsFileExsit(pszWinRAR))
	{
		MessageErrorBox(TEXT("WINRAR 路径错误"));
		return FALSE;
	}
	if (!IsFileExsit(pszPXL))
	{
		MessageErrorBox(TEXT("PXL包 路径错误"));
		return FALSE;
	}

	// Rename the PXL file to ZIP file;
	TCHAR szZipFile[MAX_PATH] = {0};
	ChangeFileExtend(szZipFile, pszPXL, TEXT(".zip"));
	MoveFile(pszPXL, szZipFile);	

	// 生成解压命令行
	TCHAR szCmd[1024] = {0};
	GenarateCmd(pszWinRAR, szZipFile, pszPXL, szCmd);
	
 	STARTUPINFO sinfo = {sizeof(sinfo)};
 	sinfo.wShowWindow = SW_FORCEMINIMIZE;
 	PROCESS_INFORMATION pinfo;
 	if (!CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &pinfo))
 	{
 		MessageErrorBox(TEXT("调用 WinRAR 失败"));
 	}
 	
 	if (WAIT_OBJECT_0 == WaitForSingleObject(pinfo.hProcess, INFINITE))
 	MoveFile(szZipFile, pszPXL);
 
	TCHAR szAppFolderName[MAX_PATH] = {0};
	TCHAR szAppFolderPath[MAX_PATH] = {0};
	_tcscpy(szAppFolderPath, pszPXL);
	DeleteExtend(szAppFolderPath);

	// 搜索.app文件夹
 	if (!SearchAppFile(szAppFolderPath, szAppFolderName))
 	{
		MessageErrorBox(TEXT("没有找到.app文件夹，文件有误!"));
 		return FALSE;
 	}
 
	// 将 .app 文件移动到上一层目录
 	wstring wszCurDir;
 	wszCurDir += TEXT(".\\");
	wszCurDir += szAppFolderName;

	wstring wszNewDir;
	wszNewDir += TEXT("..\\");
	wszNewDir += szAppFolderName;
	MoveFile(wszCurDir.c_str(), wszNewDir.c_str());

	_tchdir(TEXT(".."));

	// 拷贝 app 文件夹里面的icon.png到上一层文件夹，并命名为iTunesArtwork
	wstring wstrCurIconFile = wszCurDir;
	wstrCurIconFile += TEXT("\\icon.png");

	wstring wstrNewIconFile;
	wstrNewIconFile = TEXT(".\\iTunesArtwork");

 	CopyFile(wstrCurIconFile.c_str(), wstrNewIconFile.c_str(), FALSE);
 	
	// 创建Payload文件夹
 	CreateDirectory(TEXT(".\\Payload"), NULL);

	wstring wstrInfoList;
	wstrInfoList = TEXT(".\\");
	wstrInfoList += szAppFolderName;
	wstrInfoList += TEXT("\\Info.plist");

	if (!FixInfoList(wstrInfoList))
	{
		MessageBoxLastError();
		return FALSE;
	}

	wstring ttt;
	ttt = TEXT(".\\Payload\\");
	ttt += szAppFolderName;
 	MoveFile(wszCurDir.c_str(), ttt.c_str());
 	
	wstring wstrIPA = pszIPA;
	if (wstrIPA.empty())
	{
		wstrIPA = TEXT(".\\");
		wstrIPA += szAppFolderName;
		wstrIPA += TEXT(".ipa");
	}

	ttt = pszWinRAR;
	ttt += TEXT(" a -r -ep1 -afzip \"");
	ttt += wstrIPA;
	ttt += TEXT("\" .\\iTunesArtwork .\\Payload");
 
 	ZeroMemory(szCmd, 1024);
 	_tcscpy(szCmd, ttt.c_str());
	if (!CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &pinfo))
		MessageErrorBox(TEXT("调用 WinRAR 失败"));
	
	if (WAIT_OBJECT_0 == WaitForSingleObject(pinfo.hProcess, INFINITE))
	{
		// 清理操作
		wstring wszCmd;
		wszCmd = TEXT("del /Q \"");
		wszCmd += szAppFolderPath;
		wszCmd += TEXT("\\\"");
		_tsystem(wszCmd.c_str());
		
		wszCmd = TEXT("rd /S /Q \"");
		wszCmd += szAppFolderPath;
		wszCmd += TEXT("\\\"");
		_tsystem(wszCmd.c_str());
		
		wszCmd = TEXT("del /Q \"");
		wszCmd += TEXT(".\\Payload\\\"");
		_tsystem(wszCmd.c_str());
		
		wszCmd = TEXT("rd /S /Q \"");
		wszCmd += TEXT(".\\Payload\\\"");
		_tsystem(wszCmd.c_str());

		wszCmd = TEXT("del /Q \"");
		wszCmd += TEXT(".\\iTunesArtwork\"");
		_tsystem(wszCmd.c_str());
	}

 	return TRUE;
 }


VOID GenarateCmd(IN const TCHAR *pszWinRAR, 
				 IN const TCHAR *szZipFile, 
				 IN const TCHAR *pszPXL,
				 OUT TCHAR *pszCmd)
{
	ASSERT(NULL != pszWinRAR);
	ASSERT(NULL != szZipFile);
	ASSERT(NULL != pszPXL);
	ASSERT(NULL != pszCmd);

	wstring wstrCmd = pszWinRAR;
	wstrCmd += TEXT(" x ");
	
	wstring wstrZip = szZipFile;
	wstrZip.insert(0, TEXT("\""));
	wstrZip.append(TEXT("\""));
	
	wstrCmd += wstrZip;
	wstrCmd += TEXT(" ");
	
	TCHAR szAppFolderPath[MAX_PATH] = {0};
	_tcscpy(szAppFolderPath, pszPXL);
	DeleteExtend(szAppFolderPath);
	wstring wstrAppFolderPath = szAppFolderPath;
	wstrAppFolderPath.insert(0, TEXT("\""));
	wstrAppFolderPath.append(TEXT("\""));
	
	wstrCmd += wstrAppFolderPath;
	wstrCmd += TEXT("\\");

	_tcscpy(pszCmd, wstrCmd.c_str());
}

BOOL FixInfoList(const wstring &wstrInfoList)
{
	HANDLE hFile = CreateFile(wstrInfoList.c_str(), GENERIC_WRITE | GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (INVALID_HANDLE_VALUE == hFile)
	{
		int nError = GetLastError();
		SetLastError(E_NO_INFOLIST);
		CloseHandle(hFile);
		return FALSE;
	}
	
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwExtendSize = dwFileSize + 200;
	
	PBYTE pstrContent = new BYTE[dwExtendSize];
	ZeroMemory(pstrContent, dwExtendSize);
	ASSERT(NULL != pstrContent);
	
	DWORD dwReadedSize;
	ReadFile(hFile, pstrContent, dwExtendSize, &dwReadedSize, NULL);
	
	for (int i = dwFileSize - 1; ; i--)
	{
		if (pstrContent[i] != 0x0A)
			break;
	}
	
	int nStart = i - 16 + 1;
	TCHAR szTemp[17] = {0};
	int nSize = MultiByteToWideChar(CP_ACP, 0, 
		(char*)(&pstrContent[nStart]), 16, szTemp, 17);
	
	TCHAR szTemp2[17] = TEXT(CONTENT);
	if (0 != _tcscmp(szTemp, szTemp2))
	{
		SetLastError(E_NO_WRONG_INFOLIST);
		CloseHandle(hFile);
		return FALSE;
	}
	
	char *pFixed = REPLACE_CONTENT;
	SetFilePointer(hFile, nStart, NULL, FILE_BEGIN);
	
	DWORD dwWritedNum;
	WriteFile(hFile, pFixed, strlen(pFixed), &dwWritedNum, NULL);
	SetEndOfFile(hFile);
	
	CloseHandle(hFile);
	return TRUE;
}

void CenterWindow(HWND hwndDlg)
{
	int nX = GetSystemMetrics(SM_CXSCREEN);
	int nY = GetSystemMetrics(SM_CYSCREEN);
	
	RECT rect;
	GetClientRect(hwndDlg, &rect);
	
	MoveWindow(hwndDlg, (nX / 2) - ((rect.right - rect.left) / 2),
		(nY / 2) - ((rect.bottom - rect.top) / 2), (rect.right - rect.left),
		(rect.bottom - rect.top), FALSE);
}