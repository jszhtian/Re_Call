#include "SeparatorHolder.h"

SeparatorHolder* SeparatorHolder::Handle = nullptr;

LPVOID operator new(size_t Size)
{
	return HeapAlloc(GetProcessHeap(), 0, Size);
}

VOID operator delete(LPVOID Buffer)
{
	HeapFree(GetProcessHeap(), 0, Buffer);
}



SeparatorHolder::SeparatorHolder():
CPUCores(1),
CPUUsingCores(1),
CheckFile(FALSE),
CheckFolder(FALSE),
CheckScript(FALSE),
CheckPlugin(FALSE),
CheckArgs(FALSE)
{
}

SeparatorHolder::~SeparatorHolder()
{
	//nothing
}

SeparatorHolder* SeparatorHolder::getInstance()
{
	if (Handle == nullptr)
	{
		Handle = new SeparatorHolder;
	}
	return Handle;
}


HRESULT WINAPI SeparatorHolder::SetCheckFile(BOOL v)
{
	this->CheckFile = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetCheckFolder(BOOL v)
{
	this->CheckFolder = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetCheckScript(BOOL v)
{
	this->CheckScript = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetCheckPlugin(BOOL v)
{
	this->CheckPlugin = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetCheckArgs(BOOL v)
{
	this->CheckArgs = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetEditFile(wstring& v)
{
	this->FileName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetEditFolder(wstring& v)
{
	this->FolderName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetEditScript(wstring& v)
{
	this->ScriptName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetEditPlugin(wstring& v)
{
	this->PluginName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetEditArgs(wstring& v)
{
	this->ArgsLine = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::SetCPUCount(ULONG Cores)
{
	this->CPUCores = Cores;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::GetCheckFile(BOOL* v)
{
	if (v != nullptr && (!IsBadWritePtr(v, sizeof(BOOL))))
	{
		*v = this->CheckFile;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


HRESULT WINAPI SeparatorHolder::GetCheckFolder(BOOL* v)
{
	if (v != nullptr && (!IsBadWritePtr(v, sizeof(BOOL))))
	{
		*v = this->CheckFolder;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


HRESULT WINAPI SeparatorHolder::GetCheckScript(BOOL* v)
{
	if (v != nullptr && (!IsBadWritePtr(v, sizeof(BOOL))))
	{
		*v = this->CheckScript;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


HRESULT WINAPI SeparatorHolder::GetCheckPlugin(BOOL* v)
{
	if (v != nullptr && (!IsBadWritePtr(v, sizeof(BOOL))))
	{
		*v = this->CheckPlugin;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


HRESULT WINAPI SeparatorHolder::GetCheckArgs(BOOL* v)
{
	if (v != nullptr && (!IsBadWritePtr(v, sizeof(BOOL))))
	{
		*v = this->CheckArgs;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


HRESULT WINAPI SeparatorHolder::GetEditFile(wstring& v)
{
	this->FileName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::GetEditFolder(wstring& v)
{
	this->FolderName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::GetEditScript(wstring& v)
{
	this->ScriptName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::GetEditPlugin(wstring& v)
{
	this->PluginName = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::GetEditArgs(wstring& v)
{
	this->ArgsLine = v;
	return S_OK;
}


HRESULT WINAPI SeparatorHolder::GetCPUCount(ULONG* Cores)
{
	if (Cores != nullptr && (!IsBadWritePtr(Cores, sizeof(ULONG))))
	{
		*Cores = this->CPUCores;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT WINAPI SeparatorHolder::SetCPUUsingCores(ULONG Cores)
{
	if (Cores > CPUCores)
	{
		return S_FALSE;
	}
	else
	{
		CPUUsingCores = Cores;
		return S_OK;
	}
}

HRESULT WINAPI SeparatorHolder::GetCPUUsingCores(ULONG* Cores)
{
	if (Cores != nullptr && (!IsBadWritePtr(Cores, sizeof(ULONG))))
	{
		*Cores = this->CPUUsingCores;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

