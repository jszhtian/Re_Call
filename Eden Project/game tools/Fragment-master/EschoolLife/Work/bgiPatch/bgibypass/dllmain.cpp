// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "compact_enc_det/compact_enc_det.h"
typedef LCID (WINAPI* fnGetUserDefaultLCID)(void);
typedef LANGID (WINAPI* fnGetSystemDefaultLangID)(void);
typedef LANGID (WINAPI* fnGetSystemDefaultUILanguage)(void);
typedef bool (WINAPI* fnSetWindowTextW)(HWND hWnd, LPCWSTR lpString);
typedef HFONT(WINAPI* fnCreateFontW)(_In_ int     nHeight,
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
	_In_ LPCWSTR lpszFace);
fnCreateFontW pCreateFontW;
fnGetSystemDefaultLangID pGetSystemDefaultLangID;
fnGetSystemDefaultUILanguage pGetSystemDefaultUILanguage;
fnGetUserDefaultLCID pGetUserDefaultLCID;
fnSetWindowTextW pSetWindowTextW;

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI *PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);

PVOID g_pOldWideCharToMultiByte = NULL;
typedef int(WINAPI *PfuncWideCharToMultiByte)(
	UINT                               CodePage,
	DWORD                              dwFlags,
	_In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr,
	int                                cchWideChar,
	LPSTR                              lpMultiByteStr,
	int                                cbMultiByte,
	LPCCH                              lpDefaultChar,
	LPBOOL                             lpUsedDefaultChar);


wchar_t* ctow(const char *str)
{
	wchar_t* buffer=nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(932, 0, (const char *)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n + 1];
		MultiByteToWideChar(932, 0, (const char *)str, int(nu), buffer, int(n));
	}
	return buffer;
}

HFONT WINAPI newCreateFontW(_In_ int     nHeight,
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
	_In_ LPCWSTR lpszFace)
{
	wchar_t* newlpszF = L"思源黑体";
	return (fnCreateFontW(pCreateFontW))(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, GB2312_CHARSET, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, newlpszF);
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	bool is_reliable;
	int bytes_consumed;
	Encoding encoding = CompactEncDet::DetectEncoding(lpMBS, strlen(lpMBS), nullptr, nullptr, nullptr, UNKNOWN_ENCODING, UNKNOWN_LANGUAGE, CompactEncDet::QUERY_CORPUS, true, &bytes_consumed, &is_reliable);
	UINT codepage = cp;
	if (encoding == UTF8 && is_reliable)
	{
		codepage = CP_UTF8;
	}
	
	return((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(codepage, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

int WINAPI NewWideCharToMultiByte(UINT cp, DWORD dwFg, LPCWCH lpWCS, int ccwc, LPSTR lpMbs, int ccmb, LPCCH lpdc,LPBOOL lpusdc)
{
	UINT codepage = cp;
	if (cp == 932)
	{
		codepage = CP_UTF8;
	}
	return((PfuncWideCharToMultiByte)g_pOldWideCharToMultiByte)(codepage,dwFg,lpWCS,ccwc,lpMbs,ccmb,lpdc,lpusdc);
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

bool WINAPI newSetWindowTextW(HWND hw, LPCWSTR lps)
{
	char chktxt[] = { 0x53, 0x00, 0x75, 0x00, 0x67, 0x00, 0x61, 0x00, 0x72, 0x00, 0x2a, 0x00, 0x53, 0x00, 0x74, 0x00, 0x79, 0x00, 0x6c, 0x00 };
	auto newtitle = L"【向阳寮宿舍管理委员会】Sugar*Style 简体中文版Ver1.1";
	if(memcmp(lps, chktxt,10)==0)
	{
		return ((fnSetWindowTextW)pSetWindowTextW)(hw, newtitle);
	}
	else
	{
		return ((fnSetWindowTextW)pSetWindowTextW)(hw, lps);
	}
	
}

void SetHook()
{
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	g_pOldWideCharToMultiByte = DetourFindFunction("Kernel32.dll", "WideCharToMultiByte");
	pCreateFontW = (fnCreateFontW)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontW");
	pGetSystemDefaultLangID = (fnGetSystemDefaultLangID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultLangID");
	pGetSystemDefaultUILanguage=(fnGetSystemDefaultUILanguage)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultUILanguage");
	pGetUserDefaultLCID=(fnGetUserDefaultLCID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetUserDefaultLCID");
	pSetWindowTextW=(fnSetWindowTextW)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextW");
	DetourTransactionBegin();
	DetourAttach((void**)&pCreateFontW, newCreateFontW);
	//DetourAttach((void**)&pGetSystemDefaultLangID, newGetSystemDefaultLangID);
	//DetourAttach((void**)&pGetSystemDefaultUILanguage, newGetSystemDefaultUILanguage);
	//DetourAttach((void**)&pGetUserDefaultLCID, newGetUserDefaultLCID);
	//DetourAttach((void**)&pSetWindowTextW, newSetWindowTextW);
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach(&g_pOldWideCharToMultiByte, NewWideCharToMultiByte);
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