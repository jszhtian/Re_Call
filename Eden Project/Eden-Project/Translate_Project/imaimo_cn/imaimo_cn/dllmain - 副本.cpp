// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <mutex>
//#include "compact_enc_det/compact_enc_det.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
typedef LCID(WINAPI* fnGetUserDefaultLCID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultLangID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultUILanguage)(void);
typedef bool (WINAPI* fnSetWindowTextA)(HWND hWnd, LPCSTR lpString);
PVOID g_pOldGetFileAttributesA = GetFileAttributesA;
typedef DWORD(WINAPI* pfuncGetFileAttributesA)(
	LPCSTR lpFileName
	);
PVOID g_pOldTextOutA = TextOutA;
typedef HWND(WINAPI* pfuncTextOutA)(
	HDC    hdc,
	int    x,
	int    y,
	LPCSTR lpString,
	int    c
	);
typedef HFONT(WINAPI* fnCreateFontA)(_In_ int     nHeight,
	_In_ int     nWidth,
	_In_ int     nEscapement,
	_In_ int     nOrientation,
	_In_ int     fnWeight,
	_In_ DWORD   fdwItalic,
	_In_ DWORD   fdwUnderline,
	_In_ DWORD   fdwStrikeOut,
	_In_ DWORD   fdwCharSet,
	_In_ DWORD   fdwOutputPrecision,
	_In_ DWORD   fdwClipPrecision,
	_In_ DWORD   fdwQuality,
	_In_ DWORD   fdwPitchAndFamily,
	_In_ LPCTSTR lpszFace);
typedef int (WINAPI* fnEnumFontFamiliesExA)(
	HDC           hdc,
	LPLOGFONTA    lpLogfont,
	FONTENUMPROCA lpProc,
	LPARAM        lParam,
	DWORD         dwFlags
	);
fnCreateFontA pCreateFontA;
fnGetSystemDefaultLangID pGetSystemDefaultLangID;
fnGetSystemDefaultUILanguage pGetSystemDefaultUILanguage;
fnGetUserDefaultLCID pGetUserDefaultLCID;
fnSetWindowTextA pSetWindowTextA;
fnEnumFontFamiliesExA pEnumFontFamiliesExA;
wchar_t* ctow(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(932, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(932, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}

wchar_t* ctow936(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(936, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(936, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}
static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "debug" << std::endl;
}
HFONT WINAPI newCreateFontA(_In_ int     nHeight,
	_In_ int     nWidth,
	_In_ int     nEscapement,
	_In_ int     nOrientation,
	_In_ int     fnWeight,
	_In_ DWORD   fdwItalic,
	_In_ DWORD   fdwUnderline,
	_In_ DWORD   fdwStrikeOut,
	_In_ DWORD   fdwCharSet,
	_In_ DWORD   fdwOutputPrecision,
	_In_ DWORD   fdwClipPrecision,
	_In_ DWORD   fdwQuality,
	_In_ DWORD   fdwPitchAndFamily,
	_In_ LPCSTR lpszFace)
{
	//char newlpszF[] = "黑体";
	//wchar_t* newlpszF = ctow936((char*)lpszFace);


	wchar_t* wcs = nullptr;
	fdwCharSet = GB2312_CHARSET;
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lpszFace, strlen(lpszFace));
	wcs = ctow936(buffer);
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, wcs);
}

LCID WINAPI newGetUserDefaultLCID()
{
	return 0x411;
}

LANGID WINAPI newGetSystemDefaultLangID()
{
	return 0x411;
}

LANGID WINAPI newGetSystemDefaultUILanguage()
{
	return 0x411;
}

bool WINAPI newSetWindowTextA(HWND hw, LPCSTR lps)
{
	wchar_t newtitle[] = L"いますぐお兄ちゃんに妹だっていいたい！ | 简体中文化补丁V1.0";
	return SetWindowTextW(hw, newtitle);
}

int WINAPI newEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	//lpLogfont->lfCharSet = 936;
	LOGFONTA Lfont;
	memcpy(&Lfont, lpLogfont, sizeof(LOGFONTA));
	Lfont.lfCharSet = GB2312_CHARSET;
	memcpy(lpLogfont, &Lfont, sizeof(LOGFONTA));
	return ((fnEnumFontFamiliesExA)pEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

/*
bool WINAPI newTextOutA(HDC hw, int x, int y, LPCSTR lpStr, int len)
{
	wchar_t* wcs = nullptr;
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lpStr, len);
	bool is_reliable;
	int bytes_consumed;
	Encoding encoding = CompactEncDet::DetectEncoding(buffer, len, nullptr, nullptr, nullptr, UNKNOWN_ENCODING, UNKNOWN_LANGUAGE, CompactEncDet::WEB_CORPUS, true, &bytes_consumed, &is_reliable);
	if (encoding == JAPANESE_SHIFT_JIS || encoding == JAPANESE_CP932 && is_reliable)
	{
		wcs = ctow(buffer);
		return TextOutW(hw, x, y, wcs, len/2);
	}
	else
	{
		//wcs = ctow936(lpStr);
		return ((fnTextOutA)pTextOutA)(hw, x, y, lpStr, len);
	}
}

HWND WINAPI NewTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
	std::cout << "text:" << lpString << std::endl;
	return ((pfuncTextOutA)g_pOldTextOutA)(hdc, x, y, lpString, c);
}*/
LPWSTR ctowJP(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}
char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}
DWORD WINAPI NewGetFileAttributesA(
	LPCSTR lpFileName
)
{
	_asm
	{
		pushad
	}
	DWORD ret = -1;
	wchar_t* newlpFN = ctow(lpFileName);
	ret = GetFileAttributesW(newlpFN);

	if (ret == -1)
	{
		ret = ((pfuncGetFileAttributesA)g_pOldGetFileAttributesA)(lpFileName);
	}
	else
	{
		ret = FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_READONLY;
	}
	_asm
	{
		popad
	}
	return ret;
}

typedef BOOL(WINAPI* fnGetVolumeInformationA)(
	_In_opt_  LPCSTR lpRootPathName,
	_Out_opt_ LPSTR  lpVolumeNameBuffer,
	_In_      DWORD   nVolumeNameSize,
	_Out_opt_ LPDWORD lpVolumeSerialNumber,
	_Out_opt_ LPDWORD lpMaximumComponentLength,
	_Out_opt_ LPDWORD lpFileSystemFlags,
	_Out_opt_ LPSTR  lpFileSystemNameBuffer,
	_In_      DWORD   nFileSystemNameSize
	);
fnGetVolumeInformationA pGetVolumeInformationA;
BOOL WINAPI newGetVolumeInformationA(_In_opt_  LPCSTR lpRootPathName,
	_Out_opt_ LPSTR  lpVolumeNameBuffer,
	_In_      DWORD   nVolumeNameSize,
	_Out_opt_ LPDWORD lpVolumeSerialNumber,
	_Out_opt_ LPDWORD lpMaximumComponentLength,
	_Out_opt_ LPDWORD lpFileSystemFlags,
	_Out_opt_ LPSTR  lpFileSystemNameBuffer,
	_In_      DWORD   nFileSystemNameSize)
{
	_asm
	{
		pushad
	}
	strcpy(lpVolumeNameBuffer, "FRS001");
	auto ret = (pGetVolumeInformationA)(lpRootPathName, lpVolumeNameBuffer, nVolumeNameSize, lpVolumeSerialNumber, lpMaximumComponentLength, lpFileSystemFlags, lpFileSystemNameBuffer, nFileSystemNameSize);
	_asm
	{
		popad
	}
	return ret;
}

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	//MessageBoxW(0, L"Hooked!", L"Info", 0);
	UINT outcp;
	if (cp == 932)
	{
		char chk[] = { 0x81,0xA5 };
		outcp = 936;
		if (memcmp((void*)lpMBS, chk, 2) == 0)
		{
			outcp = 932;
		}
	}
	else
	{
		outcp = cp;
	}
	return((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(outcp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

typedef HANDLE(WINAPI* fnCreateFileA)(
	_In_     LPCSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
	);
fnCreateFileA pCreateFileA;
HANDLE WINAPI newCreateFileA(LPCSTR lpFN, DWORD dwDesAc, DWORD dwShareM, LPSECURITY_ATTRIBUTES lpSA, DWORD dwCDis, DWORD dwFlag, HANDLE hTemp)
{
	_asm
	{
		pushad
	}
	HANDLE ret = INVALID_HANDLE_VALUE;
	wchar_t* newlpFN = ctow(lpFN);
	ret = CreateFileW(newlpFN, dwDesAc, dwShareM, lpSA, dwCDis, dwFlag, hTemp);
	if (ret == INVALID_HANDLE_VALUE)
	{
		ret = pCreateFileA(lpFN, dwDesAc, dwShareM, lpSA, dwCDis, dwFlag, hTemp);
	}
	_asm
	{
		popad
	}
	return ret;
}

void SetHook()
{
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
	pGetSystemDefaultLangID = (fnGetSystemDefaultLangID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultLangID");
	pGetSystemDefaultUILanguage = (fnGetSystemDefaultUILanguage)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultUILanguage");
	pGetUserDefaultLCID = (fnGetUserDefaultLCID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetUserDefaultLCID");
	pSetWindowTextA = (fnSetWindowTextA)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextA");
	pCreateFileA = (fnCreateFileA)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "CreateFileA");
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	pGetVolumeInformationA = (fnGetVolumeInformationA)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetVolumeInformationA");
	DetourTransactionBegin();
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach((void**)&pCreateFontA, newCreateFontA);
	DetourAttach((void**)&pGetSystemDefaultLangID, newGetSystemDefaultLangID);
	DetourAttach((void**)&pGetSystemDefaultUILanguage, newGetSystemDefaultUILanguage);
	DetourAttach((void**)&pGetUserDefaultLCID, newGetUserDefaultLCID);
	DetourAttach((void**)&pSetWindowTextA, newSetWindowTextA);
	DetourAttach((void**)&pEnumFontFamiliesExA, newEnumFontFamiliesExA);
	//DetourAttach(&g_pOldTextOutA, NewTextOutA);
	DetourAttach((void**)&g_pOldGetFileAttributesA, NewGetFileAttributesA);
	DetourAttach((void**)&pGetVolumeInformationA, newGetVolumeInformationA);
	DetourAttach((void**)&pCreateFileA, newCreateFileA);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"API HOOK ERROR!!!", L"Aym", MB_OK | MB_ICONWARNING);
	}
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetHook();
		make_console();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
extern "C" __declspec(dllexport) void dummy(void) {
	return;
}