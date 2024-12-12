// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
//#include <D3D9.h>

__declspec(dllexport)void WINAPI AyamiKaze()
{
    return;
}
HMODULE Base = GetModuleHandle(NULL);
DWORD BaseAddr = (DWORD)Base;

DWORD FuncAddr0[4]{
	0x471E77,
	0x475C79,
	0x475DAB,
	0x5753F3,
};

DWORD FuncAddr[1]{
    0x462138,
};

DWORD FuncAddr2[2]{
    0x475E53,
    0x57518B,
};

DWORD FuncAddr3[1]{
    0x575404,
};

void __stdcall HookCinit0()
{
    for (DWORD Func : FuncAddr0)
    {
        _asm call Func;
    }
}

void __stdcall HookCinit1()
{
	for (DWORD Func : FuncAddr)
	{
        //cout << hex << Func << endl;
		_asm call Func;
	}
}

void __stdcall HookCinit2()
{
	for (DWORD Func : FuncAddr2)
	{
		_asm call Func;
	}
}

void __stdcall HookCinit3()
{
	for (DWORD Func : FuncAddr3)
	{
		_asm call Func;
	}
}

PVOID pHookCinit0 = (PVOID)(BaseAddr + 0x70167);
PVOID dHookCinit0 = (PVOID)(BaseAddr + 0x7016C);
__declspec(naked)void gHookCinit0()
{
    _asm
    {
        //pop[EBP + 0x8];
        //pop[EBP + 0xC];
        call HookCinit0;
        //mov eax, 1;
        jmp dHookCinit0
    }
}

PVOID pHookCinit1 = (PVOID)(BaseAddr + 0x70176);
PVOID dHookCinit1 = (PVOID)(BaseAddr + 0x7017B);
__declspec(naked)void gHookCinit1()
{
    _asm
    {
        //pop [EBP + 0x8];
        //pop [EBP + 0xC];
        call HookCinit1;
		//mov eax, 0x1;
        jmp dHookCinit1
    }
}

PVOID pHookCinit2 = (PVOID)(BaseAddr + 0x70210);
PVOID dHookCinit2 = (PVOID)(BaseAddr + 0x70215);
__declspec(naked)void gHookCinit2()
{
	_asm
	{
		//pop [EBP + 0x8];
		//pop [EBP + 0xC];
		call HookCinit2;
		//mov eax, 0x1;
		jmp dHookCinit2
	}
}

PVOID pHookCinit3 = (PVOID)(BaseAddr + 0x70221);
PVOID dHookCinit3 = (PVOID)(BaseAddr + 0x70226);
__declspec(naked)void gHookCinit3()
{
	_asm
	{
		//pop[EBP + 0x8];
		//pop[EBP + 0xC];
        call HookCinit3;
		//mov eax, 0x1;
		jmp dHookCinit3
	}
}

PVOID g_pOldMultiByteToWideChar = MultiByteToWideChar;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);
int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	cout << lpMBS << "|" << _ReturnAddress() << endl;
	//TXT << lpMBS << "|" << hex << (DWORD)_ReturnAddress() << endl;
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);;
}

void SetHook()
{
	//cout << hex << Base << endl;
	//cout << hex << pHookCinit1 << endl;
	//cout << hex << dHookCinit1 << endl;
	DetourTransactionBegin();
	DetourAttach((void**)&pHookCinit0, gHookCinit0);
	DetourAttach((void**)&pHookCinit1, gHookCinit1);
	DetourAttach((void**)&pHookCinit2, gHookCinit2);
	DetourAttach((void**)&pHookCinit3, gHookCinit3);

	//DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	if (DetourTransactionCommit() != NOERROR)
	{
		MessageBox(0, L"Error!", L"Crack Info", MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Open Console Success!" << endl;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {

		//make_console();

        /* MEGASUKI 02
        PBYTE pOldIAT = (PBYTE)Base + 0x177000;
        PBYTE pNewIAT = (PBYTE)Base + 0x958000;
        DWORD dwIATSize = 0x498;
        */

        /* StudySteady
        PBYTE pOldIAT = (PBYTE)Base + 0x4CB000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0xB8E000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x7B4;// 这里是IAT大小
        */

        /* SuzukuriKarinChan
        PBYTE pOldIAT = (PBYTE)Base + 0xD3000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0x60C000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x860;// 这里是IAT大小
        */

        /* MEGASUKI 01
        PBYTE pOldIAT = (PBYTE)Base + 0x177000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0x95c000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x498;// 这里是IAT大小
        */

        /* MEGASUKI 03
        PBYTE pOldIAT = (PBYTE)Base + 0x177000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0x958000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x498;// 这里是IAT大小
        */

        /* Himukai
        PBYTE pOldIAT = (PBYTE)Base + 0x2C0000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0x98A000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x518;// 这里是IAT大小
        */

        /* PrimalHearts
        PBYTE pOldIAT = (PBYTE)Base + 0x3F5000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0x9FF000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x770;// 这里是IAT大小
        */

        /* PrimalHearts 2
        PBYTE pOldIAT = (PBYTE)Base + 0x3F5000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0xA00000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x770;// 这里是IAT大小
        */

        /* Honey Honey Honey!
        PBYTE pOldIAT = (PBYTE)Base + 0x1CF000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)Base + 0xEBF000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x4D4;// 这里是IAT大小
        */

		/* kakuriyo_1 zh & ch
		PBYTE pOldIAT = (PBYTE)BaseAddr + 0x5AA000;// 这里是原来的IAT位置
		PBYTE pNewIAT = (PBYTE)BaseAddr + 0xC85000;// 这里是新的IAR位置
		DWORD dwIATSize = 0xEC8;// 这里是IAT大小
		*/

        /*HaremKingdom ENG
        PBYTE pOldIAT = (PBYTE)BaseAddr + 0xF3000;// 这里是原来的IAT位置
        PBYTE pNewIAT = (PBYTE)BaseAddr + 0x5D9000;// 这里是新的IAR位置
        DWORD dwIATSize = 0x5F4;// 这里是IAT大小
       /*/


		///*
		//make_console();
		SetHook();

		/*
        DWORD dwType;
        VirtualProtect(pOldIAT, dwIATSize, PAGE_READWRITE, &dwType);
        RtlCopyMemory(pOldIAT, pNewIAT, dwIATSize);// 复制
        VirtualProtect(pOldIAT, dwIATSize, dwType, &dwType);
		*/
		//*/
        MessageBox(0, L"Crack by zdfx", L"Crack Info", MB_OK | MB_ICONINFORMATION);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}