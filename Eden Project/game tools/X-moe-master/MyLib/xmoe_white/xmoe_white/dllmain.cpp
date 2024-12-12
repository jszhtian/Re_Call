// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "windows.h"
#include "Common.h"

//User32.lib
#pragma comment(lib, "User32.lib")

FARPROC g_pOriFunc;
FARPROC g_pOriFunc_Title;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//0045C546  |. 68 23C54500    PUSH WhiteEte.0045C523                   ; /pModule = "KERNEL32.dll"
		//0045D0DC >768499BB  粰剉    kernel32.lstrcmpiA
		//MessageBoxA(NULL, "Working!","FVPLoader", MB_OK);
		g_pOriFunc = GetProcAddress(GetModuleHandleA("KERNEL32.dll"),"lstrcmpiA");
		g_pOriFunc_Title = GetProcAddress(GetModuleHandleA("user32.dll"),"CreateWindowExA");
		if(FALSE == StartHook("KERNEL32.dll", g_pOriFunc, (PROC)lstrcmpiEX))
			MessageBoxA(NULL, "Failed to Load xmoe_white.dll","FVPLoader", MB_OK);
		if(FALSE == StartHook("user32.dll", g_pOriFunc_Title, (PROC)MyCreateWindowExA))
			MessageBoxA(NULL, "Failed to init title", "FVPLoader", MB_OK);
		//if(FALSE == StartHook("user32.dll", g_pOriFunc_Title, (PROC)MySendMessageA))
		//	MessageBoxA(NULL, "Failed to Load xmoe_white.dll","FVPLoader", MB_OK);
		//SetTitle();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

