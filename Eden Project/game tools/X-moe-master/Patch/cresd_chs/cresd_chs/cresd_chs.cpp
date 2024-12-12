#include "Common.h"
#include "detours.h"

#pragma comment(lib, "detours.lib")
/*
CPU Disasm
Address                        Hex dump      Command                                  Comments
004138F0                       /$  81EC 1402 sub esp,214

*/

PVOID pfCheckDVD = (PVOID)0x004138F0;
BOOL __fastcall HookCheckDVD(void *This)
{
	return 1;
}


HRESULT WINAPI Init()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&pfCheckDVD, HookCheckDVD);
	DetourTransactionCommit();
	return S_OK;
}

HRESULT WINAPI Uninit()
{
	return S_OK;
}


