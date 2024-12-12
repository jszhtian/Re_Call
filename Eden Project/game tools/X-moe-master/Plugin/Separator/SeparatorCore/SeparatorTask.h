#ifndef _SeparatorTask_
#define _SeparatorTask_

#include <Windows.h>
#include <vector>
#include <string>
#include "Mutex.h"
#include "TaskType.h"
#include "MultiCoreTask.h"
#include "SeparatorArgs.h"

using std::wstring;
using std::vector;

typedef struct TaskHandle
{
	HANDLE hThread;
	ULONG  ThreadId;
	ULONG  TaskId;
	HMODULE hImage;
	SeparatorArgs* ArgsHolder;

	TaskHandle():
		ThreadId(0),
		hThread(INVALID_HANDLE_VALUE),
		TaskId(0),
		hImage(nullptr),
		ArgsHolder(nullptr)
	{
	}

	~TaskHandle()
	{
		ThreadId = 0;
		hThread = INVALID_HANDLE_VALUE;
		TaskId = 0;
		if (hImage)
		{
			FreeLibrary(hImage);
			hImage = nullptr;
		}

	}

	void operator = (TaskHandle& o)
	{
		this->hThread = o.hThread;
		this->ThreadId = o.ThreadId;
		this->TaskId = o.TaskId;
		this->hImage = o.hImage;
		this->ArgsHolder = o.ArgsHolder;
	}
}TaskHandle, *pTaskHandle;

class SeparatorTask
{
private:
	SeparatorTask();
	static SeparatorTask* Handle;

public:
	static SeparatorTask* getInstance();
	~SeparatorTask();

	HRESULT WINAPI SetCPUCore(ULONG Cores);
	HRESULT WINAPI Reset();
	HRESULT WINAPI LaunchFileTask(wstring& Name, wstring& PluginName, 
		wstring& Args, ULONG Cores);
	HRESULT WINAPI LaunchFolderTask(wstring& Name, wstring& PluginName,
		wstring& Args, ULONG Cores);
	//both args string and plugin name are useless for script tasks.
	HRESULT WINAPI LaunchScriptTask(wstring& Name, wstring& Args, ULONG Cores);

	HRESULT WINAPI GetTaskStatement();
	HRESULT WINAPI SetCurrentArgs(wstring& Args);
	HRESULT WINAPI GetCurrentArgs(wstring& Args);

private:
	HRESULT WINAPI CleanTask();

private:
	HANDLE hCleanThread;
	ULONG CleanThreadId;

public:
	Mutex* Locker;
	BOOL  IsTaskFinished;
	vector<TaskHandle> TaskPool;
	TaskType SeparatorTaskType;
	BOOL HasTask;
	MultiCoreTask* TaskHolder;
	ULONG CoreCount;
	wstring CurrentArgs;
};

#endif
