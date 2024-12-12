// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

// Hook Process
PVOID g_OldCreateFontA = CreateFontA;
typedef HFONT (WINAPI* pFuncCreateFontA) (
	int    cHeight,
	int    cWidth,
	int    cEscapement,
	int    cOrientation,
	int    cWeight,
	DWORD  bItalic,
	DWORD  bUnderline,
	DWORD  bStrikeOut,
	DWORD  iCharSet,
	DWORD  iOutPrecision,
	DWORD  iClipPrecision,
	DWORD  iQuality,
	DWORD  iPitchAndFamily,
	LPCSTR pszFaceName
);
HFONT WINAPI HookCreateFontA(
	int    cHeight,
	int    cWidth,
	int    cEscapement,
	int    cOrientation,
	int    cWeight,
	DWORD  bItalic,
	DWORD  bUnderline,
	DWORD  bStrikeOut,
	DWORD  iCharSet,
	DWORD  iOutPrecision,
	DWORD  iClipPrecision,
	DWORD  iQuality,
	DWORD  iPitchAndFamily,
	LPCSTR pszFaceName
)
{
	return ((pFuncCreateFontA)g_OldCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight,
		bItalic, bUnderline, bStrikeOut, GB2312_CHARSET, iOutPrecision,
		iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
}

PVOID g_OldEnumFontFamiliesExA = EnumFontFamiliesExA;
typedef int(WINAPI* pFuncEnumFontFamiliesExA)(
	HDC           hdc,
	LPLOGFONTA    lpLogfont,
	FONTENUMPROCA lpProc,
	LPARAM        lParam,
	DWORD              dwFlags
);
int WINAPI HookEnumFontFamiliesExA(
	HDC           hdc,
	LPLOGFONTA    lpLogfont,
	FONTENUMPROCA lpProc,
	LPARAM        lParam,
	DWORD              dwFlags
)
{
	lpLogfont->lfCharSet = GB2312_CHARSET;
	return ((pFuncEnumFontFamiliesExA)g_OldEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

PVOID g_OldlstrcmpiA = lstrcmpiA;
typedef int(WINAPI* pFunclstrcmpiA)(
	LPCSTR s1,
	LPCSTR s2
);
int WINAPI HooklstrcmpiA(
	LPCSTR s1,
	LPCSTR s2
)
{
	return CompareStringA(0x411, NORM_IGNORECASE, s1, -1, s2, -1) - 2;
}

//0019FD8C   03041AA8  |WindowName = "偝偔傜丄傕備丅 -as the Night's, Reincarnation-"
PVOID g_OldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pFuncCreateWindowExA)(
	DWORD     dwExStyle,
	LPCSTR    lpClassName,
    LPCSTR    lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
);
HWND WINAPI HookCreateWindowExA(
	DWORD     dwExStyle,
	LPCSTR    lpClassName,
	LPCSTR    lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
)
{
	if (!strcmp(lpWindowName, "偝偔傜丄傕備丅 -as the Night's, Reincarnation-"))
	{
		LPCSTR wndName = "樱花、萌放 -as the Night's, Reincarnation-  v1.0";
		return ((pFuncCreateWindowExA)g_OldCreateWindowExA)(dwExStyle, lpClassName, wndName, dwStyle, X, Y,
			nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}

	return ((pFuncCreateWindowExA)g_OldCreateWindowExA)(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y,
		nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void SetHook()
{
	DetourTransactionBegin();
	DetourAttach(&g_OldCreateFontA, HookCreateFontA);
	DetourAttach(&g_OldEnumFontFamiliesExA, HookEnumFontFamiliesExA);
	DetourAttach(&g_OldlstrcmpiA, HooklstrcmpiA);
	DetourAttach(&g_OldCreateWindowExA, HookCreateWindowExA);
	if (DetourTransactionCommit() != NOERROR)
	{
		MessageBox(0, L"API hook失败", L"SetHook", MB_OK | MB_ICONERROR);
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
		SetHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
__declspec(dllexport)void WINAPI Proc()
{
}
