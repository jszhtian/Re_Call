// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

void WINAPI PritntMsgBE(DWORD lhs, DWORD rhs, DWORD c)
{
    //if(c == 0x81 || c == 0x9F || c == 0xE0 || c == 0xFC || c == 0xFE)
    ///TXT << "cnt:0x" << hex << c << endl;
    //cout << "cnt:0x" << hex << c << endl;
    //if (lhs == 0xD6)
    //cout << hex << lhs << ">=" << rhs << endl;
    TXT << hex << lhs << ">=" << c << endl;
}

void WINAPI PritntMsgLE(DWORD lhs, DWORD rhs, DWORD c)
{
    //if(c == 0x81 || c == 0x9F || c == 0xE0 || c == 0xFC || c == 0xFE)
    ///TXT << "cnt:0x" << hex << c << endl;
    //cout << "cnt:0x" << hex << c << endl;
    //if (lhs == 0xD6)
    //cout << hex << lhs << "<=" << rhs << endl;
    TXT << hex << lhs << "<=" << c << endl;
}

void WINAPI PrintMsgSame(DWORD lhs, DWORD rhs, DWORD c)
{
    TXT << hex << lhs << "!=" << c << endl;
}

PVOID  BE_Start = (PVOID)(BaseAddr + 0x22B59);

ASM VOID BE()
{
    INLINE_ASM
    {
        PUSHAD
        PUSHFD
        PUSH [EAX * 8 + 0x7DB9C0] // unk
        PUSH [EAX * 8 + 0x7DB9BC]
        PUSH [EAX * 8 + 0x7DB9B8]
        CALL PritntMsgBE
        POPFD
        POPAD
        JMP BE_Start
    }
}

PVOID  LE_Start = (PVOID)(BaseAddr + 0x22AC1);

ASM VOID LE()
{
    INLINE_ASM
    {
        PUSHAD
        PUSHFD
        PUSH[EAX * 8 + 0x7DB9C0] // unk
        PUSH[EAX * 8 + 0x7DB9BC]
        PUSH[EAX * 8 + 0x7DB9B8]
        CALL PritntMsgLE
        POPFD
        POPAD
        JMP LE_Start
    }
}

PVOID  Same_Start = (PVOID)(BaseAddr + 0x22BF1);

ASM VOID Same()
{
    INLINE_ASM
    {
        PUSHAD
        PUSHFD
        PUSH[EAX * 8 + 0x7DB9C0] // unk
        PUSH[EAX * 8 + 0x7DB9BC]
        PUSH[EAX * 8 + 0x7DB9B8]
        CALL PrintMsgSame
        POPFD
        POPAD
        JMP Same_Start
    }
}

VOID WINAPI INIT()
{
    DetourTransactionBegin();
    //DetourAttach((void**)&BE_Start, BE);
    //DetourAttach((void**)&LE_Start, LE);
    DetourAttach((void**)&Same_Start, Same);
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
        make_console();
        INIT();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

