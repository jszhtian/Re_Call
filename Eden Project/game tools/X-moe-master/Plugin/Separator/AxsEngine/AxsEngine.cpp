#include <Windows.h>
#include "SeparatorArgs.h"
#include "Nikaido.h"

HRESULT WINAPI AxsEngineEntry(const WCHAR* ScriptName, ULONG Cores, LPVOID o)
{
	Nikaido* Engine = nullptr;
	Engine = Nikaido::getInstance();
	if (!Engine)
	{
		return S_FALSE;
	}
	else
	{
		Engine->SetCPUCore(Cores);
		try
		{
			Engine->Run(wstring(ScriptName), (SeparatorArgs*)o);
		}
		catch (...)
		{
			delete Engine;
			return S_FALSE;
		}

		Engine->Stop();
		delete Engine;
		return S_OK;
	}
}

