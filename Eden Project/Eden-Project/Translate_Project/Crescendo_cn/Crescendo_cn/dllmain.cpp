// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

char* (__fastcall* sub_40CEF0)(char* this_ptr, char* null_ptr, LPCSTR FileName, int a3);
char* __fastcall Hook_sub_40CEF0(char* this_ptr, char* null_ptr, LPCSTR FileName, int a3)
{
    cout << FileName << endl;
    return sub_40CEF0(this_ptr, null_ptr, FileName, a3);
}

// DVD CHECK
int(__fastcall* sub_4138F0)(const char** this_ptr, const char** null_ptr);
int __fastcall Hook_sub_4138F0(const char** this_ptr, const char** null_ptr)
{
    return 1;
}

void WINAPI InlineHook()
{
    *(DWORD*)&sub_40CEF0 = BaseAddr + 0xCEF0;
    *(DWORD*)&sub_4138F0 = BaseAddr + 0x138F0;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((void**)&sub_40CEF0, Hook_sub_40CEF0);
    DetourAttach((void**)&sub_4138F0, Hook_sub_4138F0);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(0, L"InlineHookError.", L"InlineHook", 0);


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

