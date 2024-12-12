#include "Common.h"

typedef BOOL(WINAPI* pfSetProcessDPIAware)();

HRESULT WINAPI DisableHighDPI()
{
	HMODULE hDll = LoadLibraryW(L"User32.dll");
	if (hDll)
	{
		pfSetProcessDPIAware StubSetProcessDPIAware = (pfSetProcessDPIAware)GetProcAddress(hDll, "SetProcessDPIAware");
		if (StubSetProcessDPIAware)
		{
			StubSetProcessDPIAware();
			FreeLibrary(hDll);
			return S_OK;
		}
	}
	return S_FALSE;
}


