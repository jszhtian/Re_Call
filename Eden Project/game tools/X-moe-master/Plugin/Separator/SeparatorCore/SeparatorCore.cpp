#include <windows.h>
#include "SeparatorTask.h"
#include "TaskType.h"

extern "C" __declspec(dllexport)
HRESULT WINAPI SeparatorEntry(int TaskInfo, const WCHAR* Name, const WCHAR* PluginName,
const WCHAR* Args, ULONG CPULogicCount)
{
	SeparatorTask * o = SeparatorTask::getInstance();
	if (o)
	{
		if (TaskInfo == TaskType::FileTask)
		{
			if (!Name || Name[0] == 0)
			{
				MessageBoxW(NULL, L"Empty file name.", L"FATAL", MB_OK);
				return S_FALSE;
			}
			else
			{
				AllocConsole();
				o->LaunchFileTask(wstring(Name), wstring(PluginName), wstring(Args), CPULogicCount);
				FreeConsole();
			}
		}
		else if (TaskInfo == TaskType::FolderTask)
		{
			if (!Name || Name[0] == 0)
			{
				MessageBoxW(NULL, L"Empty folder name.", L"FATAL", MB_OK);
				return S_FALSE;
			}
			else
			{
				AllocConsole();
				o->LaunchFolderTask(wstring(Name), wstring(PluginName), wstring(Args), CPULogicCount);
				FreeConsole();
			}
		}
		else if (TaskInfo == TaskType::ScriptTask)
		{
			if (!Name || Name[0] == 0)
			{
				MessageBoxW(NULL, L"Empty script name.", L"FATAL", MB_OK);
				return S_FALSE;
			}
			else
			{
				AllocConsole();
				o->LaunchScriptTask(wstring(Name), wstring(Args) ,CPULogicCount);
				FreeConsole();
			}
		}
		else
		{
			MessageBoxW(NULL, L"Unknown task(s).", L"FATAL", MB_OK);
			return S_FALSE;
		}
	}
	else
	{
		MessageBoxW(NULL, L"Cannot Launch your task(s).", L"FATAL", MB_OK);
		return S_FALSE;
	}
	return S_OK;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI GetTaskLife()
{
	SeparatorTask* o = SeparatorTask::getInstance();
	if (o)
	{
		return o->GetTaskStatement();
	}
	else
	{
		return S_FALSE;
	}
}
