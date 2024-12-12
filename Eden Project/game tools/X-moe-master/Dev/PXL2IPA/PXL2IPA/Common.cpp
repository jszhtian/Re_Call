#include "Stdafx.h"
#include "Common.h"

BOOL OpenCommonFileDlg(IN HWND hOwner, IN const TCHAR *pstrFilter, 
					   OUT TCHAR *pstrFile)
{
//	ASSERT(NULL != pstrFilter);
	ASSERT(NULL != pstrFile);
	ASSERT(NULL != hOwner);
	ASSERT(IsWindow(hOwner));
	
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = {0};       // buffer for file name
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = pstrFilter;
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Display the Open dialog box. 
	
	if (!GetOpenFileName(&ofn))
	{
		DWORD dwError = CommDlgExtendedError();
		return FALSE;
	}
	
	_tcscpy(pstrFile, ofn.lpstrFile);
	
	return TRUE;
}

BOOL SaveCommonFileDlg(IN HWND hOwner, IN const TCHAR *pstrFilter, 
					   OUT TCHAR *pstrFile)
{
	//	ASSERT(NULL != pstrFilter);
	ASSERT(NULL != pstrFile);
	ASSERT(NULL != hOwner);
	ASSERT(IsWindow(hOwner));
	
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = {0};       // buffer for file name
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = pstrFilter/*TEXT("All\0*.*\0EXT\0*.EXT\0")*/;
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Display the Open dialog box. 
	
	if (!GetSaveFileName(&ofn))
	{
		DWORD dwError = CommDlgExtendedError();
		return FALSE;
	}
	
	_tcscpy(pstrFile, ofn.lpstrFile);
	
	return TRUE;
}

BOOL IsFileExsit(const TCHAR *pszFilePath)
{
	HANDLE hHandle = CreateFile(pszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hHandle)
		return FALSE;

	CloseHandle(hHandle);
	return TRUE;
}

VOID GetErrorString(IN UINT ErrCode, OUT TCHAR *pstrErrStr)
{
	if ((ErrCode & 0x20000000) == 0x00000000)
		return;

	switch (ErrCode)
	{
	case E_LONG_PATH:
		_tcscpy(pstrErrStr, E_LONG_PATH_STR);
		break;
	case E_FILE_PATH:
		break;
	case E_NO_INFOLIST:
		_tcscpy(pstrErrStr, E_NO_INFOLIST_STR);
		break;
	case E_NO_WRONG_INFOLIST:
		_tcscpy(pstrErrStr, E_NO_WRONG_INFOLIST_STR);
		break;
	}
}

VOID MessageErrorBox(const TCHAR *pszErrorMsg)
{
	MessageBox(NULL, pszErrorMsg, TEXT("Error!"), MB_OK | MB_ICONERROR);
}


BOOL ChangeFileExtend(OUT TCHAR *pDestFile, 
					  IN const TCHAR *pSrcFile, 
					  IN const TCHAR *pRplaceExtend)
{
	ASSERT(NULL != pDestFile);
	ASSERT(NULL != pSrcFile);
	ASSERT(NULL != pRplaceExtend);
	
	wstring wstrSrcFile = pSrcFile;
	wstrSrcFile.erase(wstrSrcFile.rfind(TEXT(".")));
	wstrSrcFile += pRplaceExtend;
	
	_tcscpy(pDestFile, wstrSrcFile.c_str());
	
	return TRUE;
}

BOOL DeleteExtend(IN OUT TCHAR *pFilePath)
{
	ASSERT(NULL != pFilePath);
	
	size_t nLength = _tcslen(pFilePath);
	if (nLength > MAX_PATH)
	{
		SetLastError(E_LONG_PATH);
		return FALSE;
	}
	
	wstring wstrFilePath = pFilePath;
	wstrFilePath.erase(wstrFilePath.rfind(TEXT(".")));
	
	ZeroMemory(pFilePath, nLength);
	_tcscpy(pFilePath, wstrFilePath.c_str());
	
	return TRUE;
}

BOOL SearchAppFile(IN const TCHAR *pszAppFolderPath, OUT TCHAR *pszAppFolderName)
{
	ASSERT(NULL != pszAppFolderPath);
	ASSERT(NULL != pszAppFolderName);
	
	
	if (-1 == _tchdir(pszAppFolderPath))
	{
		SetLastError(E_FILE_PATH);
		return FALSE;
	}
	
	LONG handle;
	struct _tfinddata_t filestruct;
	
	handle = _tfindfirst(TEXT("*.app"), &filestruct);
	if (-1 == handle)
		return FALSE;
	
	BOOL bAppIsExsit = FALSE;
	if (::GetFileAttributes(filestruct.name) & FILE_ATTRIBUTE_DIRECTORY)
	{
		_tcscpy(pszAppFolderName, filestruct.name);
		bAppIsExsit = TRUE;
	}
	
	_findclose(handle);
	return bAppIsExsit;
}

VOID MessageBoxLastError()
{
	TCHAR szErrorMsg[200];
	GetErrorString(GetLastError(), szErrorMsg);
	MessageErrorBox(szErrorMsg);
}