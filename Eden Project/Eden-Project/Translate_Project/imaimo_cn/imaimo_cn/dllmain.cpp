// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <mutex>
//#include "compact_enc_det/compact_enc_det.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
typedef LCID (WINAPI* fnGetUserDefaultLCID)(void);
typedef LANGID (WINAPI* fnGetSystemDefaultLangID)(void);
typedef LANGID (WINAPI* fnGetSystemDefaultUILanguage)(void);
typedef bool (WINAPI* fnSetWindowTextA)(HWND hWnd, LPCSTR lpString);
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
typedef DWORD (WINAPI* fnGetFileAttributesA)(
	_In_ LPCSTR lpFileName
);
typedef HANDLE(WINAPI* fnCreateFileA)(
	_In_     LPCSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
	);
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
fnCreateFontA pCreateFontA;
fnGetSystemDefaultLangID pGetSystemDefaultLangID;
fnGetSystemDefaultUILanguage pGetSystemDefaultUILanguage;
fnGetUserDefaultLCID pGetUserDefaultLCID;
fnSetWindowTextA pSetWindowTextA;
fnCreateFileA pCreateFileA;
fnGetFileAttributesA pGetFileAttributesA;

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI *PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);

wchar_t* ctow(const char *str)
{
	wchar_t* buffer=nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(932, 0, (const char *)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[128];
		memset(buffer, 0, sizeof(wchar_t) * 128);
		((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(932, 0, (const char *)str, int(nu), buffer, int(n));
	}
	return buffer;
}

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
	auto ret = (pGetVolumeInformationA)(lpRootPathName, lpVolumeNameBuffer, nVolumeNameSize, lpVolumeSerialNumber, lpMaximumComponentLength, lpFileSystemFlags, lpFileSystemNameBuffer, nFileSystemNameSize);
	strcpy(lpVolumeNameBuffer, "FRS001");
	_asm
	{
		popad
	}
	return ret;
}

HANDLE WINAPI newCreateFileA(LPCSTR lpFN, DWORD dwDesAc, DWORD dwShareM, LPSECURITY_ATTRIBUTES lpSA, DWORD dwCDis, DWORD dwFlag, HANDLE hTemp)
{
	_asm
	{
		pushad
	}
	HANDLE ret = INVALID_HANDLE_VALUE;
	wchar_t* newlpFN = ctow(lpFN);
	ret= CreateFileW(newlpFN, dwDesAc, dwShareM, lpSA, dwCDis, dwFlag, hTemp);
	if (ret==INVALID_HANDLE_VALUE)
	{
		ret = pCreateFileA(lpFN, dwDesAc, dwShareM, lpSA, dwCDis, dwFlag, hTemp);
	}
	_asm
	{
		popad
	}
	return ret;
}

DWORD WINAPI newGetFileAttributesA(_In_ LPCSTR lpFileName)
{
	_asm
	{
		pushad
	}
	DWORD ret = -1;
	wchar_t* newlpFN = ctow(lpFileName);
	ret= GetFileAttributesW(newlpFN);
	
	if (ret==-1)
	{
		ret = pGetFileAttributesA(lpFileName);
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
	_In_ LPCTSTR lpszFace)
{
	_asm
	{
		pushad
	}
	//wchar_t* newlpszF = ctow((char*)lpszFace);
	fdwCharSet = GB2312_CHARSET;
	_asm
	{
		popad
	}
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, L"黑体");
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
	_asm
	{
		pushad
	}
	wchar_t* newtitle = (wchar_t*)L"【绿茶汉化组】いますぐお兄ちゃんに妹だっていいたい！ | 简体中文化补丁V1.0";
	_asm
	{
		popad
	}
	return SetWindowTextW(hw, newtitle);
}

void SetHook()
{
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pGetSystemDefaultLangID = (fnGetSystemDefaultLangID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultLangID");
	pGetSystemDefaultUILanguage=(fnGetSystemDefaultUILanguage)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultUILanguage");
	pGetUserDefaultLCID=(fnGetUserDefaultLCID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetUserDefaultLCID");
	pSetWindowTextA=(fnSetWindowTextA)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextA");
	pGetFileAttributesA = (fnGetFileAttributesA)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetFileAttributesA");
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
	DetourAttach((void**)&pGetFileAttributesA, newGetFileAttributesA);
	DetourAttach((void**)&pGetVolumeInformationA, newGetVolumeInformationA);
	DetourAttach((void**)&pCreateFileA, newCreateFileA);
	DetourTransactionCommit();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		SetHook();
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