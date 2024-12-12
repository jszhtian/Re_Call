// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include <Windows.h>
#include <fstream>
#include "functiondef.h"

extern void InitStep();
extern void ExitExe();

extern "C" __declspec(dllexport) void dummy(void) {
	return;
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
		make_console();
#endif // DEBUG
		InitStep();
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		ExitExe();
        break;
    }
    return TRUE;
}

