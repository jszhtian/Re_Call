// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

void __stdcall AymShowTexW(wchar_t* text)
{
    if(wcslen(text)!=0)
        cout << wtoc(text) << endl;
}

PVOID pGetFileName = (PVOID)(BaseAddr + 0xEC5C4);
__declspec(naked)void mGetFileName()
{
    __asm
    {
        pushad
        pushfd
        push edi
        call AymShowTexW
        popfd
        popad
        jmp pGetFileName
    }
}

void InlineHook()
{
    DetourTransactionBegin();
    DetourAttach((void**)&pGetFileName, mGetFileName);
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
        InlineHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

