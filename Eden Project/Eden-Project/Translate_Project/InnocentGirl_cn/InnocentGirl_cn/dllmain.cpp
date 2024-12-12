// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

typedef HFONT(WINAPI* fnCreateFontIndirectA)(LOGFONTA* lplf);
fnCreateFontIndirectA pCreateFontIndirectA;
HFONT WINAPI newCreateFontIndirectA(LOGFONTA* lplf)
{
    if (!lstrcmpA(lplf->lfFaceName, "俵俽 僑僔僢僋"))
    {
        lstrcpyA(lplf->lfFaceName, "黑体");
        lplf->lfCharSet = GB2312_CHARSET;
    }
    return pCreateFontIndirectA(lplf);
}

typedef int (WINAPI* fnMessageboxA)(
    _In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType
    );
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType)
{
    // 僀僲僙儞僩僈乕儖 傪廔椆偟偰傛傠偟偄偱偡偐丠
    if (lpText) 
    {
        if (!lstrcmpA(lpText, "僀僲僙儞僩僈乕儖 傪廔椆偟偰傛傠偟偄偱偡偐丠"))
            return MessageBoxAOLD(hWnd, "即将结束游戏，确认吗？", "确认", uType);
    }
    return MessageBoxAOLD(hWnd, lpText, lpCaption, uType);
}

typedef bool (WINAPI* fnSetWindowTextA)(HWND hWnd, LPCSTR lpString);
fnSetWindowTextA pSetWindowTextA;
bool WINAPI newSetWindowTextA(HWND hw, LPCSTR lps)
{
    wchar_t newtitle[] = L"Innocent Girl - v1.0";
    return SetWindowTextW(hw, newtitle);
}

void Init()
{
    pCreateFontIndirectA = (fnCreateFontIndirectA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontIndirectA");
    MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
    pSetWindowTextA = (fnSetWindowTextA)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextA");
    DetourTransactionBegin();
    DetourAttach((void**)&pCreateFontIndirectA, newCreateFontIndirectA);
    DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
    DetourAttach((void**)&pSetWindowTextA, newSetWindowTextA);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBoxW(NULL, L"API Function Hook Error.", L"Init", MB_OK | MB_ICONERROR);
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
        make_console();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

