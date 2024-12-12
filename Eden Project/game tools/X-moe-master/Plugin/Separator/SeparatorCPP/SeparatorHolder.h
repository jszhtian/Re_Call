#ifndef _SeparatorHolder_
#define _SeparatorHolder_

#include <Windows.h>
#include <string>

using std::wstring;

class SeparatorHolder
{
private:
	SeparatorHolder();
	static SeparatorHolder* Handle;

public:
	static SeparatorHolder* getInstance();
	~SeparatorHolder();

	HRESULT WINAPI SetCheckFile(BOOL v);
	HRESULT WINAPI SetCheckFolder(BOOL v);
	HRESULT WINAPI SetCheckScript(BOOL v);
	HRESULT WINAPI SetCheckPlugin(BOOL v);
	HRESULT WINAPI SetCheckArgs(BOOL v);
	HRESULT WINAPI SetEditFile(wstring& v);
	HRESULT WINAPI SetEditFolder(wstring& v);
	HRESULT WINAPI SetEditScript(wstring& v);
	HRESULT WINAPI SetEditPlugin(wstring& v);
	HRESULT WINAPI SetEditArgs(wstring& v);
	HRESULT WINAPI SetCPUCount(ULONG Cores);
	HRESULT WINAPI SetCPUUsingCores(ULONG Cores);

	HRESULT WINAPI GetCheckFile(BOOL* v);
	HRESULT WINAPI GetCheckFolder(BOOL* v);
	HRESULT WINAPI GetCheckScript(BOOL* v);
	HRESULT WINAPI GetCheckPlugin(BOOL* v);
	HRESULT WINAPI GetCheckArgs(BOOL* v);
	HRESULT WINAPI GetEditFile(wstring& v);
	HRESULT WINAPI GetEditFolder(wstring& v);
	HRESULT WINAPI GetEditScript(wstring& v);
	HRESULT WINAPI GetEditPlugin(wstring& v);
	HRESULT WINAPI GetEditArgs(wstring& v);
	HRESULT WINAPI GetCPUCount(ULONG* Cores);
	HRESULT WINAPI GetCPUUsingCores(ULONG* Cores);

private:
	ULONG CPUUsingCores;
	ULONG CPUCores;
	BOOL CheckFile;
	BOOL CheckFolder;
	BOOL CheckScript;
	BOOL CheckPlugin;
	BOOL CheckArgs;
	wstring FileName;
	wstring FolderName;
	wstring ScriptName;
	wstring PluginName;
	wstring ArgsLine;
};

#endif
