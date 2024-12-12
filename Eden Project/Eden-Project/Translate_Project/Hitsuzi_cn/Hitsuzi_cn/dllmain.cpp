// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    lstrcpyA(lplf->lfFaceName, "SimHei");
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID g_pOldSetWindowTextA = SetWindowTextA;
typedef BOOL (WINAPI* PfuncSetWindowTextA)(HWND hWnd, LPCSTR lpString);
BOOL WINAPI HookSetWindowTextA(HWND hWnd, LPCSTR lpString)
{
    if (!lstrcmpW(ctow((CHAR*)lpString, 932), L"羊の方舟"))
    {
        wchar_t newtitle[] = L"【2030汉化组】羊之方舟 - v1.0 [BuildTime: 20220526]";
        return SetWindowTextW(hWnd, newtitle);
    }
    return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hWnd, lpString);
}

PVOID g_pOldMessageBoxA = MessageBoxA;
typedef int (WINAPI* PfuncMessageBoxA)(
    HWND   hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT   uType
);
int WINAPI HookMessageBoxA(
    HWND   hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT   uType
)
{
    if (!lstrcmpW(ctow((char*)lpText, 932), L"ゲームを終了しますか？"))
        return MessageBoxW(hWnd, L"确认结束游戏吗？", ctow((char*)lpCaption,936), uType);

    return ((PfuncMessageBoxA)g_pOldMessageBoxA)(hWnd, lpText, lpCaption, uType);
}

PVOID g_pOldTextOutA = TextOutA;
typedef int(WINAPI* PfuncTextOutA)(HDC hdc, int x, int y, LPCSTR lpString, int c);
int WINAPI HookTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
    // 0416FB60   023F38E0  |String = "丂丂丂丂丂丂丂丂丂丂丂丂"

    /*
    if (!lstrcmpA(lpString, "丂丂丂丂丂丂丂丂丂丂丂丂"))
        return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, "　　　　　　　　　　　　", c);
    else if (!lstrcmpA(lpString, "丂丂丂丂丂丂丂丂丂丂丂丂谢辞"))
        return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, "　　　　　　　　　　　　谢辞", c);
    */
    for (int i = 0; i < c; i++)
    {
        uint16_t uChar = 0;
        memcpy((BYTE*)&uChar + 1, (lpString + i), 1);
        memcpy((BYTE*)&uChar, (lpString + i + 1), 1);
        if (uChar == 0x8140)
        {
            memcpy((CHAR*)(lpString + i), "\xA1\xA1", 2);
        }
    }
    return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, lpString, c);
}

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


HANDLE WINAPI NewCreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    if (!lstrcmpA(lpFileName, "Game.pak")) 
    {
        return ((PfuncCreateFileA)(g_pOldCreateFileA))(
            "Game.cpk",
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile);
    }
    else if (!lstrcmpA(lpFileName, "Script.pak"))
    {
        return ((PfuncCreateFileA)(g_pOldCreateFileA))(
            "Script.cpk",
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile);
    }
    return ((PfuncCreateFileA)(g_pOldCreateFileA))(
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile);
}


void WINAPI APIHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
    DetourAttach(&g_pOldSetWindowTextA, HookSetWindowTextA);
    DetourAttach(&g_pOldMessageBoxA, HookMessageBoxA);
    DetourAttach(&g_pOldTextOutA, HookTextOutA);
    DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBoxW(NULL, L"API Function Hook Error.", L"APIHook", MB_OK | MB_ICONERROR);
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
        APIHook();
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