// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

PVOID g_pOldRegOpenKeyExA = RegOpenKeyExA;
typedef LSTATUS(WINAPI* PfuncRegOpenKeyExA)(
    HKEY hKey,
    LPCSTR lpSubKey, 
    DWORD  ulOptions, 
    REGSAM samDesired, 
    PHKEY  phkResult
    );
LSTATUS WINAPI HookRegOpenKeyExA(
    HKEY hKey,
    LPCSTR lpSubKey,
    DWORD  ulOptions,
    REGSAM samDesired,
    PHKEY  phkResult
)
{
    if (lpSubKey)
        return RegOpenKeyExW(hKey, ctow((char*)lpSubKey, 932), ulOptions, samDesired, phkResult);
    return((PfuncRegOpenKeyExA)g_pOldRegOpenKeyExA)(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}


PVOID g_pOldGetLocalTime = GetLocalTime;
typedef void(WINAPI* PfuncGetLocalTime)(LPSYSTEMTIME lpSystemTime);
void WINAPI HookGetLocalTime(LPSYSTEMTIME lpSystemTime)
{
     ///*
    lpSystemTime->wYear = 1980;
    lpSystemTime->wMonth = 1;
    lpSystemTime->wDayOfWeek = 2;
    lpSystemTime->wDay = 1;
    lpSystemTime->wHour = 8;
    lpSystemTime->wMinute = 0;
    lpSystemTime->wSecond = 0;
    lpSystemTime->wMilliseconds = 0;
    //*/
    //Sleep(10000);
    //return ((PfuncGetLocalTime)g_pOldGetLocalTime)(lpSystemTime);
}

void WINAPI APIHook()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldRegOpenKeyExA, HookRegOpenKeyExA);
    DetourAttach(&g_pOldGetLocalTime, HookGetLocalTime);
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