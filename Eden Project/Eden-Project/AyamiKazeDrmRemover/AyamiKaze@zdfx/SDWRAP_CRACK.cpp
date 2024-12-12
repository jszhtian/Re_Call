#include "framework.h"

WNDPROC Func = NULL;

LRESULT CALLBACK MyWinBack(
    _In_ HWND   hwnd,
    _In_ UINT   uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    int result;
    if (!Func)
    {
        MessageBox(0, L"Can't find callback func.", L"Aym", 0);
        exit(-1);
    }
    if (uMsg == 1)
    {
        result = Func(hwnd, 1045, 0, 0);
    }
    else
    {
        result = Func(hwnd, uMsg, wParam, lParam);
    }
    return result;
}


PVOID g_pOldRegisterClassExA = RegisterClassExA;
typedef ATOM(WINAPI* PfuncRegisterClassExA) (WNDCLASSEXA* Arg1);
ATOM WINAPI NewRegisterClassExA(WNDCLASSEXA* Arg1)
{
    if (Arg1 && Arg1->lpfnWndProc && !strcmp(Arg1->lpszClassName, "SDWRAP"))
    {
        Func = Arg1->lpfnWndProc;
        Arg1->lpfnWndProc = MyWinBack;
        return ((PfuncRegisterClassExA)g_pOldRegisterClassExA)(Arg1);
    }

    return ((PfuncRegisterClassExA)g_pOldRegisterClassExA)(Arg1);
}

HANDLE hThread = nullptr;
BOOL IsOnce = FALSE;
PVOID g_pOldCreateProcessA = CreateProcessA;
typedef ATOM(WINAPI* PfuncCreateProcessA)(
    LPCSTR                lpApplicationName,
    LPSTR                 lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCSTR                lpCurrentDirectory,
    LPSTARTUPINFOA        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
    );
ATOM WINAPI NewCreateProcessA(
    LPCSTR                lpApplicationName,
    LPSTR                 lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCSTR                lpCurrentDirectory,
    LPSTARTUPINFOA        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
)
{
    if (IsOnce)
    {
        if (hThread)
            ResumeThread(hThread);
        ExitProcess(0);
    }
    IsOnce = TRUE;
    ATOM ret = ((PfuncCreateProcessA)g_pOldCreateProcessA)(
        lpApplicationName,
        lpCommandLine,
        lpProcessAttributes,
        lpThreadAttributes,
        bInheritHandles,
        dwCreationFlags,
        lpEnvironment,
        lpCurrentDirectory,
        lpStartupInfo,
        lpProcessInformation
        );
    if (lpProcessInformation)
    {
        //cout << "Get lpProcessInformation:0x" << *(DWORD*)(lpProcessInformation + 0xC) << endl;
        hThread = OpenThread(2, 0, *(DWORD*)(lpProcessInformation + 0x0C));
    }
    return ret;
}

BOOL SDWRAP_CRACK()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldRegisterClassExA, NewRegisterClassExA);
    DetourAttach(&g_pOldCreateProcessA, NewCreateProcessA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"CRACK ERROR\nCODE: 1", L"SDWRAP_CRACK", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}