#pragma once

// Error Code
#define E_LONG_PATH					0xE0000001
#define E_LONG_PATH_STR				TEXT("文件名过长，超过260")

#define E_FILE_PATH					0xE0000002
#define E_FILE_PATH_STR		

#define E_NO_INFOLIST				0xE0000003
#define E_NO_INFOLIST_STR			TEXT("没有找到Info.plist文件，请确认是否为正确的PXL文件！")

#define E_NO_WRONG_INFOLIST			0xE0000004
#define E_NO_WRONG_INFOLIST_STR		TEXT("Info.plist文件格式不对，</dict>\n</plist>")

BOOL OpenCommonFileDlg(IN HWND hOwner, IN const TCHAR *pstrFilter, 
					   OUT TCHAR *pstrFile);

BOOL SaveCommonFileDlg(IN HWND hOwner, IN const TCHAR *pstrFilter, 
					   OUT TCHAR *pstrFile);

BOOL IsFileExsit(const TCHAR *pszFilePath);

VOID GetErrorString(IN UINT ErrCode, OUT TCHAR *pstrErrStr);

VOID MessageErrorBox(const TCHAR *pszErrorMsg);

BOOL ChangeFileExtend(OUT TCHAR *pDestFile, 
					  IN const TCHAR *pSrcFile, 
					  IN const TCHAR *pRplaceExtend);

BOOL DeleteExtend(IN OUT TCHAR *pFilePath);

BOOL SearchAppFile(IN const TCHAR *pszAppFolderPath, OUT TCHAR *pszAppFolderName);

VOID MessageBoxLastError();