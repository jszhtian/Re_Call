// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "crc32.h"
#include <iomanip>
#pragma comment(lib, "detours.lib")

static char* Buff;

void __stdcall AymDumpFile(DWORD size)
{
    if (Buff)
    {
        FILE* fp=fopen("dump.xml", "wb");
        fwrite(Buff, size, 1, fp);
        fclose(fp);
        Buff = NULL;
    }
}

PVOID pGetBuff = NULL;
PVOID pDump = NULL;//(PVOID)(0x10021C4D);
__declspec(naked)void GetBuff()
{
    __asm
    {
        pushad
        pushfd
        mov Buff, eax;
        popfd
        popad
        jmp pGetBuff
    }
}

__declspec(naked)void Dump()
{
    __asm
    {
        pushad
        pushfd
        push eax
        call AymDumpFile
        popfd
        popad
       jmp pDump
    }
}

void AymMainFunc()
{
    DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
   // pGetBuff = (PVOID)(BaseAddr + 0x3f3aaf);见习恋人
    //pDump = (PVOID)(BaseAddr + 0x3f3abf);
    //pGetBuff = (PVOID)(BaseAddr + 0x1710);//l2q
    //pDump = (PVOID)(BaseAddr + 0x1728);
    pGetBuff = (PVOID)(BaseAddr + 0x36FAFF);//mashimaro
    pDump = (PVOID)(BaseAddr + 0x36FB0F);
    DetourTransactionBegin();
    DetourAttach((void**)&pGetBuff, GetBuff);
    DetourAttach((void**)&pDump, Dump);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"ERROR", L"AymKze", MB_OK);
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
        AymMainFunc();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

