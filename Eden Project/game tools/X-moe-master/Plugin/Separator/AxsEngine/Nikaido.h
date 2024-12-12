#ifndef _Nikaido_
#define _Nikaido_

#include <Windows.h>
#include "SeparatorArgs.h"
#include <string>
#include "Lua/lua.hpp"
#include "WinFile.h"

using std::wstring;

class Nikaido
{
private:
	Nikaido();
	static Nikaido* Handle;
	
public:
	static Nikaido* getInstance(HWND hWin = nullptr);
	~Nikaido();
	
	HRESULT WINAPI Run(const wstring& ScriptName, SeparatorArgs* Client);
	HRESULT WINAPI Stop();
	HRESULT WINAPI SetCPUCore(ULONG Cores);
	HRESULT WINAPI SetSelfModule(HMODULE hSelf);

	//TaskMethod
	enum axTaskInfo
	{
		axAutoDetect,
		axFileName,
		axFilePlugin
	};

	//warpped API
	//ֻ��Ҫ�ṩ���������ľ͹���
	//�����ĺ��� ����дӢ��װ����
public:
	HRESULT WINAPI axCreateTask(const std::string& FileName, const std::string& PluginName, const std::string& Args, ULONG Method);
	
private:
	HWND      hWindow;
	HMODULE   hSelfModule;
	ULONG     CPUCores;
};

#endif
