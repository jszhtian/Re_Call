// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

CHAR TextBox[0x1000] = { 0 };

PVOID g_pOldTextOutA = TextOutA;
typedef int(WINAPI* PfuncTextOutA)(HDC hdc, int x, int y, LPCSTR lpString, int c);
int WINAPI HookTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
    CHAR fstTxt[0x3] = { 0 };
    memcpy(fstTxt, lpString, 2);
    if (!strcmp(fstTxt, "乽"))
    {
        lstrcpyA(TextBox, lpString);
        memcpy(TextBox, "「", 2);
        return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, TextBox, c);
    }
    else if (!strcmp(fstTxt, "丂"))
    {
        lstrcpyA(TextBox, lpString);
        memcpy(TextBox, "　", 2);
        return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, TextBox, c);
    }
    else if (!strcmp(fstTxt, "乧"))
    {
        lstrcpyA(TextBox, lpString);
        memcpy(TextBox, "」", 2);
        return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, TextBox, c);
    }
    return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, lpString, c);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    strcpy(lplf->lfFaceName, "SimHei");

    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID g_pOldCreateFontA = CreateFontA;
typedef HFONT(WINAPI* PfuncCreateFontA)(int nHeight,
    int nWidth,
    int nEscapement,
    int nOrientation,
    int fnWeight,
    DWORD fdwltalic,
    DWORD fdwUnderline,
    DWORD fdwStrikeOut,
    DWORD fdwCharSet,
    DWORD fdwOutputPrecision,
    DWORD fdwClipPrecision,
    DWORD fdwQuality,
    DWORD fdwPitchAndFamily,
    LPCTSTR lpszFace);
HFONT WINAPI HookCreateFontA(int nHeight,
    int nWidth,
    int nEscapement,
    int nOrientation,
    int fnWeight,
    DWORD fdwltalic,
    DWORD fdwUnderline,
    DWORD fdwStrikeOut,
    DWORD fdwCharSet,
    DWORD fdwOutputPrecision,
    DWORD fdwClipPrecision,
    DWORD fdwQuality,
    DWORD fdwPitchAndFamily,
    LPCTSTR lpszFace)
{
    return CreateFontW(nHeight,
        nWidth,
        nEscapement,
        nOrientation,
        fnWeight,
        fdwltalic,
        fdwUnderline,
        fdwStrikeOut,
        GB2312_CHARSET,
        fdwOutputPrecision,
        fdwClipPrecision,
        fdwQuality,
        fdwPitchAndFamily,
        L"SimHei");
}

#define iris

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
    DWORD dwExStyle,
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam);
HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
#ifdef himeko
    const char* szWndName = "【风铃汉化组】雨のマージナル - v1.0";
#endif
#ifdef iris
    const char* szWndName = "【风铃汉化组】ナルキッソス外伝－小さなイリス－ - v1.0";
#endif

    HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    //SetWindowDisplayAffinity(hw, WDA_MONITOR);
    return hw;
}

wchar_t szTitle[] = L"提示";
PVOID g_pOldMessageBoxA = MessageBoxA;
typedef int (WINAPI* pfuncMessageboxA)(
    _In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType
    );
int WINAPI HookMessageBoxA(_In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType)
{
    WCHAR* Pstr = ctow((CHAR*)lpText, 932);
    //cout << lpText << endl;
    //TXT << Pstr << endl;
    if (!wcscmp(Pstr, L"終了してよろしいですか？"))
        return MessageBoxW(hWnd, L"即将结束游戏，确认吗？", szTitle, uType);
    else if (!wcscmp(Pstr, L"タイトルに戻ります。よろしいですか？"))
        return MessageBoxW(hWnd, L"即将返回标题，确认吗？", szTitle, uType);
    return MessageBoxW(hWnd, Pstr, szTitle, uType);
}


void APIInit()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
    DetourAttach(&g_pOldTextOutA, HookTextOutA);
    DetourAttach(&g_pOldCreateFontA, HookCreateFontA);
    DetourAttach(&g_pOldCreateWindowExA, HookCreateWindowExA);
    DetourAttach(&g_pOldMessageBoxA, HookMessageBoxA);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBox(NULL, L"APIHook Error.", L"APIInit", MB_OK | MB_ICONERROR);
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
        APIInit();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI AyamiKaze()
{
}