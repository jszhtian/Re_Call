// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

/*
0046690C  |> \83C4 04       |add esp,0x4
0046690F  |.  89AF 5C220000 |mov dword ptr ds:[edi+0x225C],ebp
00466915  |>  8B4B 1C       |mov ecx,dword ptr ds:[ebx+0x1C]         ;  this
00466918  |.  8D6B 1C       |lea ebp,dword ptr ds:[ebx+0x1C]
*/
void AymGetText()
{
    char* Text;
    _asm
    {
        mov ecx, [ebx + 0x1C];
        mov Text, ecx;
    }
    cout << Text << endl;
    char* RetText = (char*)malloc(strlen(Text) * 4);
    strcpy(RetText, Text);
    _asm
    {
        mov ecx, Text;
        mov [ebx + 0x1C], ecx;
    }
}

PVOID pGetText = (PVOID)(BaseAddr + 0x66915);
PVOID dGetText = (PVOID)(BaseAddr + 0x6691E);
__declspec(naked)void mGetText()
{
    __asm
    {
        pushad
        pushfd
        call AymGetText
        popfd
        popad
        jmp dGetText
    }
}

void InlineHook()
{
    DetourTransactionBegin();
    DetourAttach((void**)&pGetText, mGetText);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"ERROR", L"Aym", MB_OK);
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

