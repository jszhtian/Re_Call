// pStart.cpp : 实现文件
//

#include "stdafx.h"
#include "Separator.h"
#include "pStart.h"
#include "afxdialogex.h"
//#include "XMOEAPI.h"

using namespace std;

CString GetExtName(CString fileName)
{
	int pos=fileName.Find(L"."); //获取 . 的位置
	if(pos==-1)
	{ //如果没有找到，直接返回该字符串
		return fileName; 
	}
	else
	{
		return GetExtName(fileName.Mid(pos+1)); //找到了的话，往深层遍历，直到最底层
	}
}



pStart::pStart() :
	libPtr(NULL)
{
	::AllocConsole();    // 打开控件台资源
    freopen("CONOUT$", "w+t", stdout);
}

pStart::~pStart()
{
	if(libPtr)
		FreeLibrary(libPtr);
	FreeConsole(); 
}

HMODULE pStart::LoadDLL(CString &ccFile, CString &EngineName)
{
	typedef BOOL (*Func)(TCHAR *str, TCHAR *szFileName);
	typedef BOOL (*pEngine)(TCHAR *szEngine);

	WIN32_FIND_DATAW fd;
	HANDLE hSearch;
	wchar_t Path[300] = {0};
	GetCurrentDirectoryW(300, Path);
	StrCatW(Path, L"\\core\\*.xmoe");
	hSearch = FindFirstFileW(Path, &fd);
	if(hSearch != INVALID_HANDLE_VALUE)
	{
		/*
		HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD size = 0;
			std::wstring Info(L"Trying " + std::wstring(fd.cFileName) + L"\n");
			WriteConsoleW(hout, Info.c_str(), Info.length(), &size, NULL);
			wprintf(L"Trying %s\n", fd.cFileName);
			*/
	}
	else
	{
		MessageBoxW(NULL, L"No Plugins found!\nYour Request(s) will be ignored!", L"Separator::Loading", MB_OK);
		return NULL;
	}

	if(hSearch != INVALID_HANDLE_VALUE)
	{
		WCHAR wPath[300] = {0};
		GetCurrentDirectoryW(300, wPath);
		StrCatW(wPath, L"\\core\\");
		StrCatW(wPath, fd.cFileName);
		HMODULE pDLL = LoadLibraryW(wPath);
		if(pDLL != NULL)
		{
			Func pMatch = (Func)GetProcAddress(pDLL, "Match" );
			pEngine Engine = (pEngine)GetProcAddress(pDLL, "Engine");
			if (pMatch != NULL && Engine != NULL)
			{
				CString ext = GetExtName(ccFile);
				HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
				DWORD size = 0;
				std::wstring Info(L"Trying " + std::wstring(fd.cFileName) + L"\n");
				WriteConsoleW(hout, Info.c_str(), Info.length(), &size, NULL);
				//wprintf(L"Trying %s\n", fd.cFileName);
				BOOL ret = pMatch((wchar_t*)ext.GetString(), (wchar_t*)ccFile.GetString());
				BOOL ret2 = Engine((wchar_t*)EngineName.GetString());
				if (ret == TRUE || ret2 == TRUE)
				{
					libPtr = pDLL;
					return pDLL;
				}
				else
				{
					libPtr = NULL;
					FreeLibrary(pDLL);
				}
			}
			else
			{
				libPtr = NULL;
				FreeLibrary(pDLL);
			}
		}
	}

	while(FindNextFileW(hSearch, &fd))
	{
		WCHAR wPath2[300] = {0};
		GetCurrentDirectoryW(300, wPath2);
		StrCatW(wPath2, L"\\core\\");
		StrCatW(wPath2, fd.cFileName);
		HMODULE pDLL = LoadLibraryW(wPath2);
		if(pDLL != NULL)
		{
			Func pMatch = (Func)GetProcAddress(pDLL, "Match" );
			pEngine Engine = (pEngine)GetProcAddress(pDLL, "Engine");
			HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD size = 0;
			std::wstring Info(L"Trying " + std::wstring(fd.cFileName) + L"\n");
			WriteConsoleW(hout, Info.c_str(), Info.length(), &size, NULL);
			if (pMatch == NULL || Engine == NULL)
			{
				continue;
			}
			CString ext = GetExtName(ccFile);
			BOOL ret = pMatch((wchar_t*)ext.GetString(), (wchar_t*)ccFile.GetString());
			BOOL ret2 = Engine((wchar_t*)EngineName.GetString());
			if (ret == TRUE || ret2 == TRUE)
			{
				libPtr = pDLL;
				return pDLL;
			}
			else
			{
				libPtr = NULL;
				FreeLibrary(pDLL);
			}
		}
	}
	return libPtr;
}


int pStart::ParseFile(CString &ccFile, CString &EngineName)
{
	HMODULE pDLL = LoadDLL(ccFile, EngineName);
	if(pDLL == NULL)
	{
		MessageBoxW(NULL, L"Guessing unsupported file.\n", L"Separator", MB_OK);
		return -1;
	}
	typedef BOOL(*Func)(TCHAR *str, TCHAR *szFileName);
	typedef BOOL(*pEngine)(TCHAR *szEngine);
	typedef void (*pInfo)(void);
	typedef int  (*pExec)(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *outputL, const TCHAR *args);
	pInfo ProcInfo = (pInfo)GetProcAddress(pDLL, "ShowInfo" );
	ProcInfo();
	pExec ProcExec = (pExec)GetProcAddress(pDLL, "Exec" );
	if(ProcExec == NULL)
	{
		MessageBoxW(NULL, L"Bad plugin(s) or invalid plugin(s).\n", L"Separator", MB_OK);
		return -1;
	}
	ProcExec(ccFile.GetString(), 0, 0, NULL, NULL);
	return 0;
}

int pStart::ParseDic(CString &ccDic, CString &EngineName)
{
	typedef BOOL (*Func)(TCHAR *str, TCHAR *szFileName);
	typedef BOOL (*pEngine)(TCHAR *szEngine);
	typedef void (*pInfo)(void);
	typedef int  (*pExec)(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *outputL, const TCHAR *args);

	WIN32_FIND_DATAW fd;
	HANDLE hPackage = FindFirstFileW(L"*.*", &fd);
	if(hPackage == INVALID_HANDLE_VALUE)
	{
		MessageBoxW(NULL, L"Empty Dictionary!", L"Separator::Searcher", MB_OK);
		return -1;
	}

	list<wstring> pFileList;

	wstring Dic(ccDic.GetString());
	Dic += L"\\";
	if(hPackage != INVALID_HANDLE_VALUE)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		{
			if (wcscmp(fd.cFileName, L".") != 0)
			{
				FILE *fin = NULL;
				_wfopen_s(&fin, wstring(Dic + fd.cFileName).c_str(), L"rb");
				if (fin)
				{
					pFileList.push_back(wstring(Dic + fd.cFileName));
				}
				else
				{
					if (fin)
					{
						fclose(fin);
					}
				}
				fclose(fin);
			}
		}
	}
	else
		return -1;

	while(FindNextFileW(hPackage, &fd))
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			continue;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		{
			wprintf(L"%s\n", wstring(Dic + fd.cFileName).c_str());
			if (wcscmp(fd.cFileName, L".") == 0)
			{
				continue;
			}
			FILE *fin = NULL;
			_wfopen_s(&fin, wstring(Dic + fd.cFileName).c_str(), L"rb");
			if (fin)
			{
				pFileList.push_back(wstring(Dic + fd.cFileName));
			}
			else
			{
				if (fin)
				{
					fclose(fin);
				}
			}
			fclose(fin);
		}
	}

	for(list<wstring>::iterator it = pFileList.begin(); it != pFileList.end(); it++)
	{
		MessageBox(NULL, it->c_str(), L"", MB_OK);
		HMODULE pDLL = LoadDLL(CString(it->c_str()), EngineName);
		if(pDLL == NULL)
		{
			continue;
		}
		else
		{
			pInfo ProcInfo = (pInfo)GetProcAddress(pDLL, "ShowInfo" );
			pExec ProcExec = (pExec)GetProcAddress(pDLL, "Exec" );
			Func Match = (Func)GetProcAddress(pDLL, "Match");
			BOOL MatchOk = FALSE;
			BOOL CheckEngine = FALSE;

			if(ProcExec == NULL)
			{
				MessageBoxW(NULL, L"Bad plugin(s) or invalid plugin(s).\n", L"Separator", MB_OK);
				return -1;
			}
			ProcExec(it->c_str(), 0, 0, NULL, NULL);
		}
		FreeLibrary(pDLL);
	}

	pFileList.clear();
	return 0;
}
