#include <Windows.h>
#include "ShellData.h"
#include <stdio.h>

SHELL_DATA32 Crystallizer
{
	(ULONG64)TAG8('Crys', 'tall'),
	(ULONG64)TAG8('izer', 'Stat'),
	(DWORD)0
};

extern "C" _declspec(dllexport) void LinkProc()
{
	//GlobalData.OriOEP();
	//MessageBoxW(NULL, L"code", L"Nop", MB_OK);
	
	__asm call Crystallizer.OriOEP;
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
#if 0
		DWORD OldProtect = 0, NewProtect;
		auto Mod = GetModuleHandle(nullptr);
		auto pDosHeader = (PIMAGE_DOS_HEADER)Mod;
		auto pNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)(pDosHeader->e_lfanew) + (SIZE_T)Mod);
		VirtualProtect(&(pNtHeader->OptionalHeader.AddressOfEntryPoint), sizeof(DWORD),
			PAGE_EXECUTE_READWRITE, &OldProtect);
		pNtHeader->OptionalHeader.AddressOfEntryPoint = (SIZE_T)LinkProc - (SIZE_T)Mod;
		VirtualProtect(&(pNtHeader->OptionalHeader.AddressOfEntryPoint), sizeof(DWORD),
			OldProtect, &NewProtect);
#else
		/*
		auto Mod = GetModuleHandle(nullptr);
		auto pDosHeader = (PIMAGE_DOS_HEADER)Mod;
		auto pNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)(pDosHeader->e_lfanew) + (SIZE_T)Mod);
		//Crystallizer.OriOEP = pNtHeader->OptionalHeader.AddressOfEntryPoint;
		//char Info[30] = { 0 };
		//sprintf(Info, "%08x\n", Crystallizer.OriOEP);
		//MessageBoxA(0, Info, 0, 0);

		DWORD OldProtect = 0, NewProtect;

		//LinkProc();
		Crystallizer.OriOEP += (SIZE_T)GetModuleHandle(nullptr);

		//__asm call Crystallizer.OriOEP;
		VirtualProtect(&(pNtHeader->OptionalHeader.AddressOfEntryPoint), sizeof(DWORD),
			PAGE_EXECUTE_READWRITE, &OldProtect);
		pNtHeader->OptionalHeader.AddressOfEntryPoint = 
			(SIZE_T)LinkProc - (SIZE_T)GetModuleHandle(nullptr);
		VirtualProtect(&(pNtHeader->OptionalHeader.AddressOfEntryPoint), sizeof(DWORD),
			OldProtect, &NewProtect);
			*/
#endif

	}
	break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

typedef void(*OEPFunction)();

struct GlobalData32
{
	OEPFunction OriOEP;
};


GlobalData32 GlobalData;
