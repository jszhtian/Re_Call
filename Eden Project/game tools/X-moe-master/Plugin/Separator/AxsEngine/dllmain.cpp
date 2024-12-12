#include <Windows.h>
#include "Nikaido.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, LPVOID lpReserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		if (Nikaido::getInstance() == nullptr)
		{
			MessageBoxW(NULL, L"Failed to Init AxsEngine!", L"Separator", MB_OK);
			ExitProcess(-1);
		}
		Nikaido::getInstance()->SetSelfModule(hModule);
	}
	return TRUE;
}

