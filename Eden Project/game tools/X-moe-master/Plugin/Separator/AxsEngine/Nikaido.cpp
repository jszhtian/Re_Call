#include "Nikaido.h"

//===========================================
Nikaido* Nikaido::Handle = nullptr;

//===========================================

Nikaido* Nikaido::getInstance(HWND hWin = nullptr)
{
	if (Handle == nullptr)
	{
		Handle = new Nikaido;
	}
	if (Handle && hWin && (Handle->hWindow != nullptr))
	{
		Handle->hWindow = hWin;
	}
	return Handle;
}

//===========================================

Nikaido::Nikaido():
	CPUCores(1),
	hWindow(nullptr),
	hSelfModule(nullptr)
{
}

HRESULT WINAPI Nikaido::Run(const wstring& ScriptName, SeparatorArgs* Client)
{

}


HRESULT WINAPI Nikaido::Stop()
{

}


HRESULT WINAPI Nikaido::SetCPUCore(ULONG Cores)
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	CPUCores = min(Cores >= 32 ? 32 : Cores, info.dwNumberOfProcessors);
	return CPUCores >= info.dwNumberOfProcessors ? S_OK : S_FALSE;
}


HRESULT WINAPI Nikaido::SetSelfModule(HMODULE hSelf)
{
	if (hSelf)
	{
		if (hSelfModule == nullptr)
		{
			hSelfModule = hSelf;
		}
	}
	return hSelfModule ? S_OK : S_FALSE;
}


HRESULT WINAPI Nikaido::axCreateTask(
	const std::string& FileName,
	const std::string& PluginName,
	const std::string& Args,
	ULONG Method
	)
{

}
