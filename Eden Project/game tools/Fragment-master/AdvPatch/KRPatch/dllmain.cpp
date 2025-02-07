// dllmain.cpp :  定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "TransText.h"
#include "resource.h"
using namespace std;

static int(WINAPI *OldMultiByteToWideChar)(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar) = MultiByteToWideChar;
static int(WINAPI *OldWideCharToMultiByte)(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar) = WideCharToMultiByte;
static HFONT(WINAPI *OldCreateFontIndirectW)(CONST LOGFONTW *lplf) = CreateFontIndirectW;

TransText translatetext;
LPVOID pMap = nullptr;
TextStruct *textbuf = nullptr;
HANDLE hFile = INVALID_HANDLE_VALUE;
HANDLE hFileMap = INVALID_HANDLE_VALUE;

char *wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = OldWideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); 
	char *bbb = new char[dwMinSize];
	OldWideCharToMultiByte(CP_ACP, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
	return bbb;
}


HFONT WINAPI newCreateFontIndirectW(LOGFONTW *lplf)
{
	__asm
		{
		pushad
		}

	LOGFONTW lf;
	memcpy(&lf, lplf, sizeof(LOGFONTW));
	wcscpy(lf.lfFaceName, L"微软雅黑");
	lf.lfCharSet = GB2312_CHARSET;
	__asm
	{
		popad
	}

	return OldCreateFontIndirectW(&lf);
}

static volatile int thread_searching = 0;
std::mutex mut;
void FindandReplaceStr(LPWSTR lpWCS, int *ret)
{
	mut.lock();
	if ((USHORT)*lpWCS == 0x25)
	{
		goto startsearch;
	}
	if ((USHORT)*lpWCS == 0x7B)
	{
		goto startsearch;
	}

	if ((USHORT)*lpWCS > 0x7f)
	{
		goto startsearch;
	}
	goto exiting;
startsearch:
	auto res = translatetext.searchtext(lpWCS);
	if (res != nullptr)
	{
		*ret = wcslen(res);
		wcscpy(lpWCS, res);
	}
exiting:
	mut.unlock();
	//thread_searching = 0;
}

int WINAPI newMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	__asm
	{
		pushfd
		pushad
	}

	int ret = 0;
	ret = OldMultiByteToWideChar(932, dwFg, lpMBS, cbMB, lpWCS, ccWC);
	if (lpWCS != 0)
	{
		wchar_t chk[] = L"■";
		if (memcmp(lpWCS, chk, 2)==0)
		{
			ret= OldMultiByteToWideChar(936, dwFg, lpMBS, cbMB, lpWCS, ccWC);
			memcpy(lpWCS, chk, 2);
		}
		else
		{
			FindandReplaceStr(lpWCS, &ret);
		}
		
	}
	else
	{
		ret = 0x100;
	}
	__asm
	{
		popad
		popfd
	}
	return ret;
}

bool wc2mbchk(const WCHAR* rawstr)
{
	DWORD dwMinSize;
	dwMinSize = OldWideCharToMultiByte(932, NULL, rawstr, -1, NULL, 0, NULL, FALSE);
	char *bbb = new char[dwMinSize];
	OldWideCharToMultiByte(932, NULL, rawstr, -1, bbb, dwMinSize, NULL, FALSE);
	wchar_t* buffer;
	size_t nu = strlen(bbb);
	size_t n = (size_t)OldMultiByteToWideChar(932, 0, (const char *)bbb, int(nu), NULL, 0);
	buffer = new wchar_t[n + 1];
	memset(buffer, 0, sizeof(wchar_t)*(n + 1));
	OldMultiByteToWideChar(932, 0, (const char *)bbb, int(nu), buffer, int(n));
	auto chkresult = wcscmp(buffer, rawstr);
	delete bbb;
	delete buffer;
	if (chkresult == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int WINAPI newWideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar)
{
	__asm
		{
		pushad
		}
	int ret;
	if (wc2mbchk(lpWideCharStr))
	{
		ret = OldWideCharToMultiByte(932, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
	}
	else
	{
		ret = OldWideCharToMultiByte(936, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
	}
	

	__asm
	{
		popad
	}

	return ret;
}


void LoadStringMap()
{
	HMODULE ghmodule = GetModuleHandle(L"AdvPatch.dll");
	HRSRC hRsrc = FindResource(ghmodule, _T("#101"), L"PAK");
	if (hRsrc == NULL)
	{
		MessageBoxW(NULL, L"Load Resource fail!", L"严重错误", MB_OK);
	}
	DWORD dwResSize = SizeofResource(ghmodule, hRsrc);
	HGLOBAL hGlobal = LoadResource(ghmodule, hRsrc);
	LPVOID pBuffer = LockResource(hGlobal);
	DWORD dwBytesToWrite;
	auto buffer2 = new BYTE[1024*1024*30];
	DWORD rawSize;
	int res = uncompress(buffer2, &rawSize, (Byte*)pBuffer, dwResSize);
	if (res != Z_OK)
	{
		MessageBoxW(NULL, L"Uncompress Resource fail!", L"严重错误", MB_OK);
	}
	hFile = CreateFileW(L"pak.tmp", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN |FILE_FLAG_DELETE_ON_CLOSE , NULL);
	WriteFile(hFile, buffer2, rawSize, &dwBytesToWrite, NULL);
	FreeResource(hGlobal);
	delete buffer2;
	DWORD dwBytesRead, dwBytesToRead;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxW(NULL, L"CreateFile fail!", L"严重错误", MB_OK);
	}
	DWORD filesize = GetFileSize(hFile, NULL);
	head *hdr = new head;
	dwBytesRead = sizeof(head);
	ReadFile(hFile, hdr, dwBytesRead, &dwBytesToRead, NULL);
	hFileMap = CreateFileMappingW(hFile, NULL, PAGE_READONLY, 0, filesize, NULL);
	if (hFileMap == NULL)
	{
		MessageBoxW(NULL, L"CreateFileMapping fail!", L"严重错误", MB_OK);
	}
	pMap = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, filesize);
	if (pMap == NULL)
	{
		MessageBoxW(NULL, L"MapViewOfFile fail!", L"严重错误", MB_OK);
	}
	textbuf = (TextStruct *)((char *)pMap + 0x10);
	translatetext.setStorage(textbuf);
	translatetext.setTableLength(hdr->Length);
	delete hdr;
}

void BeginDetour()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID &)OldMultiByteToWideChar, newMultiByteToWideChar);
	DetourAttach(&(PVOID &)OldCreateFontIndirectW, newCreateFontIndirectW);
	DetourAttach(&(PVOID &)OldWideCharToMultiByte, newWideCharToMultiByte);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Hook目标函数失败", L"严重错误", MB_OK | MB_ICONWARNING);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		LoadStringMap();
		//must load texts before Hook!
		BeginDetour();
	case DLL_THREAD_ATTACH:
		//Unload();
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void)
{
	return;
}