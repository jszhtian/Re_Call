// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

typedef LCID(WINAPI* fnGetUserDefaultLCID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultLangID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultUILanguage)(void);
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
typedef int (WINAPI* fnEnumFontFamiliesExA)(
	HDC           hdc,
	LPLOGFONTA    lpLogfont,
	FONTENUMPROCA lpProc,
	LPARAM        lParam,
	DWORD         dwFlags
	);
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
PVOID g_pOldCreateFileA = CreateFileA;
typedef HANDLE(WINAPI* PfuncCreateFileA)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);
fnMessageboxA MessageBoxAOLD;
fnCreateFontA pCreateFontA;
fnGetSystemDefaultLangID pGetSystemDefaultLangID;
fnGetSystemDefaultUILanguage pGetSystemDefaultUILanguage;
fnGetUserDefaultLCID pGetUserDefaultLCID;
fnSetWindowTextA pSetWindowTextA;
fnEnumFontFamiliesExA pEnumFontFamiliesExA;
PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);

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
LPWSTR ctowJIS(char* str)
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
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, 
		fdwUnderline, fdwStrikeOut, GB2312_CHARSET, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, L"黑体");
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

/*

06FEC6EC  82 A2 82 DC 82 B7 82 AE 82 A8 8C 5A 82 BF 82 E1  偄傑偡偖偍孼偪傖
06FEC6FC  82 F1 82 C9 96 85 82 BE 82 C1 82 C4 82 A2 82 A2  傫偵枀偩偭偰偄偄
06FEC70C  82 BD 82 A2 81 49 00                             偨偄両.?瓠?



*/

CHAR OldTitle[39] = { 0x82, 0xA2, 0x82, 0xDC, 0x82, 0xB7, 0x82, 0xAE, 0x82, 0xA8, 0x8C, 0x5A, 0x82, 0xBF, 0x82, 0xE1,
						0x82, 0xF1, 0x82, 0xC9, 0x96, 0x85, 0x82, 0xBE, 0x82, 0xC1, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xA2,
						0x82, 0xBD, 0x82, 0xA2, 0x81, 0x49, 0x00
};
bool WINAPI newSetWindowTextA(HWND hw, LPCSTR lps)
{
	//if(!memcmp(lps, OldTitle, 0x24) || !memcmp(lps, OldTitle2, 0x20))
	return pSetWindowTextA(hw, "【绿茶汉化组】现在立刻就想对哥哥说我是妹妹！ - v1.0 [BuildTime: 220401]");
}

int WINAPI newEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	LOGFONTA Lfont;
	memcpy(&Lfont, lpLogfont, sizeof(LOGFONTA));
	Lfont.lfCharSet = GB2312_CHARSET;
	memcpy(lpLogfont, &Lfont, sizeof(LOGFONTA));
	return ((fnEnumFontFamiliesExA)pEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(936, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	WCHAR* Pstr = ctow((char*)lpText);
	WCHAR* Pstr2 = ctow((char*)lpCaption);
	return MessageBoxW(hWnd, Pstr, Pstr2, uType);
}

void EdenInitHook()
{
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
	pGetSystemDefaultLangID = (fnGetSystemDefaultLangID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultLangID");
	pGetSystemDefaultUILanguage = (fnGetSystemDefaultUILanguage)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultUILanguage");
	pGetUserDefaultLCID = (fnGetUserDefaultLCID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetUserDefaultLCID");
	pSetWindowTextA = (fnSetWindowTextA)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextA");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	DetourTransactionBegin();
	DetourAttach((void**)&pCreateFontA, newCreateFontA);
	DetourAttach((void**)&pGetSystemDefaultLangID, newGetSystemDefaultLangID);
	DetourAttach((void**)&pGetSystemDefaultUILanguage, newGetSystemDefaultUILanguage);
	DetourAttach((void**)&pGetUserDefaultLCID, newGetUserDefaultLCID);
	DetourAttach((void**)&pSetWindowTextA, newSetWindowTextA);
	DetourAttach((void**)&pEnumFontFamiliesExA, newEnumFontFamiliesExA);
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"API Hook Error", L"EdenInitHook", MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//make_console();
		EdenInitHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI Kuzushita()
{
}