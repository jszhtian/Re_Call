// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "TVPHook.h"
TVPHook* objTVPhook = nullptr;
bool InitPatch()
{
	objTVPhook = GetobjTVPhook();
	return objTVPhook->init();
}

bool unInitPatch()
{
	if (objTVPhook!=nullptr)
	{
		objTVPhook->uninit();
	}
	return true;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "Open Console Success!" << std::endl;
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
		InitPatch();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void) {
	return;
}

