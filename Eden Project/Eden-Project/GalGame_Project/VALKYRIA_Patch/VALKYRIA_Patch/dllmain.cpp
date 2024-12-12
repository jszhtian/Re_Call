// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

char(__cdecl* sub_41D430)(char* ReadFileName, int a2, DWORD* a3, DWORD* a4, char a5);
char __cdecl GetFileInfomationFromPackage(char* ReadFileName, int a2, DWORD* a3, DWORD* a4, char a5)
{
    cout << ReadFileName << endl;

    return sub_41D430(ReadFileName, a2, a3, a4, a5);
}

VOID Init()
{
    *(DWORD*)&sub_41D430 = BaseAddr + 0x1D430;

    DetourTransactionBegin();
    DetourAttach((void**)&sub_41D430, GetFileInfomationFromPackage);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBox(NULL, L"InlinePatch Error.", L"InlinePatch", MB_OK | MB_ICONERROR);
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
        make_console();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

