// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

void WINAPI ShowMsgW(WCHAR* text)
{
    //cout << wtoc(text, CP_ACP) << endl;
    lstrcatW(text, L"yuno JP");
}

PVOID pGetArgInfo = NULL;
_declspec(naked) void GetArgInfo()
{
    _asm
    {
        pushad;
        pushfd;
        push eax;
        call ShowMsgW;
        popfd;
        popad;
        jmp pGetArgInfo;
    }
}

void WINAPI InitHook()
{
    pGetArgInfo = (PVOID)(BaseAddr + 0x674EE);

    DetourTransactionBegin();
    DetourAttach((void**)&pGetArgInfo, GetArgInfo);
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
        make_console();
        InitHook();
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