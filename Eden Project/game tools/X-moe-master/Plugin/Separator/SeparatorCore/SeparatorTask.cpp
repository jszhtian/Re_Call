#include "SeparatorTask.h"
#include "MessageDef.h"
#include "SeparatorArgsHost.h"

SeparatorTask* SeparatorTask::Handle = nullptr;

LPVOID operator new(size_t Size)
{
	return HeapAlloc(GetProcessHeap(), 0, Size);
}

VOID operator delete(LPVOID Buffer)
{
	HeapFree(GetProcessHeap(), 0, Buffer);
}

SeparatorTask::SeparatorTask():
IsTaskFinished(TRUE),
hCleanThread(INVALID_HANDLE_VALUE),
CleanThreadId(0),
Locker(nullptr),
HasTask(FALSE),
CoreCount(1)
{
	Locker = new Mutex;
	if (CleanTask() != S_OK)
	{
		MessageBoxW(NULL, L"Cannot launch thread!", L"FATAL", MB_OK);
		ExitProcess(-1);
	}
}

SeparatorTask::~SeparatorTask()
{
	this->Reset();

	ULONG ExitCode;
	PostThreadMessageW(this->CleanThreadId, XMOE_MSG_EXIT, NULL, NULL);
	WaitForSingleObject(this->hCleanThread, 200);
	GetExitCodeThread(this->hCleanThread, &ExitCode);
	if (ExitCode == STILL_ACTIVE)
	{
		TerminateThread(this->Handle, -1);
	}
	delete Locker;
}


HRESULT WINAPI SeparatorTask::SetCPUCore(ULONG Cores)
{
	if (Cores & 0xFFFFF000UL)
	{
		MessageBoxW(NULL, L"System cannot hold so many threads at the same time.", L"Error", MB_OK);
		return S_FALSE;
	}
	else
	{
		this->CoreCount = Cores;
		return S_OK;
	}
}

SeparatorTask* SeparatorTask::getInstance()
{
	if (Handle == nullptr)
	{
		Handle = new SeparatorTask;
	}
	return Handle;
}

HRESULT WINAPI SeparatorTask::Reset()
{
	if (TaskHolder)
	{
		TaskHolder->Clear();
	}
	//do not delete this holder

	if (this->IsTaskFinished == FALSE)
	{
		if (this->SeparatorTaskType == TaskType::FileTask ||
			this->SeparatorTaskType == TaskType::FolderTask)
		{
			Locker->Lock();
			if (this->TaskPool.size() == 0)
			{
				BOOL Value = TRUE;
				InterlockedExchangePointer(&(this->IsTaskFinished), &Value);
				return S_OK;
			}
			HANDLE *HandlePool = (HANDLE*)HeapAlloc(GetProcessHeap(), 0,
				sizeof(HANDLE) * TaskPool.size());
			for (ULONG Index = 0; Index < this->TaskPool.size(); Index++)
			{
				HandlePool[Index] = TaskPool[Index].hThread;
			}
			if (WaitForMultipleObjects(this->TaskPool.size(), HandlePool, TRUE, INFINITE) == WAIT_FAILED)
			{
				for (ULONG Index = 0; Index < this->TaskPool.size(); Index++)
				{
					ULONG ExitCode;
					GetExitCodeThread(HandlePool[Index], &ExitCode);
					//0x00000103L
					//Don't use this value as return value in your plugin.
					//REMEMBER, NEVER USE THIS VALUE!
					if (ExitCode == STILL_ACTIVE)
					{
						//
						PostThreadMessageW(this->TaskPool[Index].ThreadId, XMOE_MSG_EXIT, NULL, NULL);
						WaitForSingleObject(HandlePool[Index], 500);//big enough
						GetExitCodeThread(HandlePool[Index], &ExitCode);
						//...
						if (ExitCode == STILL_ACTIVE)
						{
							TerminateThread(HandlePool[Index], -1);
						}
					}
				}
			}
			HeapFree(GetProcessHeap(), 0, HandlePool);
			this->TaskPool.clear();
			BOOL Value = TRUE;
			InterlockedExchangePointer(&(this->IsTaskFinished), &Value);

			for (auto it : TaskPool)
			{
				if (it.hImage)
				{
					FreeLibrary(it.hImage);
					it.hImage = nullptr;
				}

				if (it.ArgsHolder)
				{
					it.ArgsHolder->Release();
				}
			}
			Locker->Release();
			TaskPool.clear();
		}
		else
		{
			//Clean up script-based tasks
		}
		return S_OK;
	}
	else
	{
		return S_OK;
	}
}

typedef HRESULT(WINAPI* MatchProc)(const WCHAR* FileName);
typedef HRESULT(WINAPI* DoExtractProc)(const WCHAR* FileName, SeparatorArgs* args);

HRESULT WINAPI SeparatorTask::LaunchFileTask(wstring& Name, wstring& PluginName,
	wstring& Args, ULONG Cores)
{
	Reset();
	SetCPUCore(1);
	this->TaskPool.resize(Cores);
	SeparatorTaskType = TaskType::FileTask;
	SetCurrentArgs(Args);

	if (PluginName.length())
	{
		HMODULE hPlugin = nullptr;
		wstring PluginPath(L"Axm\\");
		PluginPath += PluginName;
		hPlugin = LoadLibraryW(PluginPath.c_str());
		if (hPlugin)
		{
			MatchProc Match = (MatchProc)GetProcAddress(hPlugin, "Match");
			DoExtractProc DoExtract = (DoExtractProc)GetProcAddress(hPlugin, "DoExtract");
			if (Match == nullptr || DoExtract == nullptr)
			{
				FreeLibrary(hPlugin);
				return S_FALSE;
			}
			if (!Match(Name.c_str()))
			{
				FreeLibrary(hPlugin);
				return S_FALSE;
			}
			else
			{
				SeparatorArgsHost* HostInfo = new SeparatorArgsHost(Args);
				HostInfo->SetTaskId(1);
				DoExtract(Name.c_str(), HostInfo);
				TaskHandle Info;
				Info.hImage = hPlugin;
				HostInfo->GetThreadId(&(Info.ThreadId));
				HostInfo->GetThreadHandle(&(Info.hThread));
				Info.TaskId = 1;
				TaskPool.push_back(Info);
				BOOL Value = FALSE;
				InterlockedExchangePointer(&(this->IsTaskFinished), &Value);
			}
		}
	}
	else
	{
		WIN32_FIND_DATAW Data = { 0 };
		HANDLE hFind = FindFirstFileW(L"Axm\\*.axm", &Data);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return S_FALSE;
		}
		HMODULE hPlugin = nullptr;
		while (FindNextFileW(hFind, &Data))
		{
			wstring PluginPath(L"Axm\\");
			PluginPath += Data.cFileName;
			hPlugin = LoadLibraryW(PluginPath.c_str());
			if (hPlugin)
			{
				MatchProc Match = (MatchProc)GetProcAddress(hPlugin, "Match");
				DoExtractProc DoExtract = (DoExtractProc)GetProcAddress(hPlugin, "DoExtract");
				if (Match == nullptr || DoExtract == nullptr)
				{
					FreeLibrary(hPlugin);
					continue;
				}
				if (!Match(Name.c_str()))
				{
					FreeLibrary(hPlugin);
					continue;
				}
				else
				{
					SeparatorArgsHost* HostInfo = new SeparatorArgsHost(Args);
					HostInfo->SetTaskId(1);
					DoExtract(Name.c_str(), HostInfo);
					TaskHandle Info;
					Info.hImage = hPlugin;
					HostInfo->GetThreadId(&(Info.ThreadId));
					HostInfo->GetThreadHandle(&(Info.hThread));
					Info.TaskId = 1;
					TaskPool.push_back(Info);
					BOOL Value = FALSE;
					InterlockedExchangePointer(&(this->IsTaskFinished), &Value);
					break;
				}
			}
		}
	}
	return S_OK;
}


//note:
//DO NOT use global resources in your plugins
//OR just lock this resources for multi task module
HRESULT WINAPI SeparatorTask::LaunchFolderTask(wstring& Name, wstring& PluginName,
	wstring& Args, ULONG Cores)
{
	Reset();
	SetCPUCore(Cores);
	SeparatorTaskType = TaskType::FolderTask;
	this->TaskPool.resize(Cores);
	SetCurrentArgs(Args);
	//multi-task 

	vector<wstring> FileNamePool;
	//Get All Files firstly
	{
		WIN32_FIND_DATAW Data;
		HANDLE hFindFile = INVALID_HANDLE_VALUE;
		wstring FullFilePath;
		wstring FullPathPrefix;
		ULONG Index = 0;
		while (Name[Index])
		{
			if (Name[Index] == L'/' || Name[Index] == L'\\')
			{
				FullFilePath += L'\\';
			}
			else
			{
				FullFilePath += Name[Index];
			}
			Index++;
		}
		if (FullFilePath[Index - 1] == L'\\')
		{
			FullPathPrefix = FullFilePath;
			FullFilePath += L"*.*";
		}
		else
		{
			FullPathPrefix = FullFilePath + L"\\";
			FullFilePath += L"\\*.*";
		}
		hFindFile = FindFirstFileW(FullFilePath.c_str(), &Data);
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
			return S_FALSE;
		}
		else
		{
			while (FindNextFileW(hFindFile, &Data))
			{
				if (Data.cFileName[0] == L'.' || Data.cFileName[1] == L'.')
				{
					continue;
				}
				else if (Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ||
						 Data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				{
					continue;
				}
				else
				{
					FileNamePool.push_back(FullPathPrefix + Data.cFileName);
				}
			}
		}
	}

	//enum plugins
	if (PluginName.length())
	{
		HMODULE hPlugin = nullptr;
		wstring PluginPath(L"Axm\\");
		PluginPath += PluginName;
		hPlugin = LoadLibraryW(PluginPath.c_str());
		if (hPlugin)
		{
			MatchProc Match = (MatchProc)GetProcAddress(hPlugin, "Match");
			DoExtractProc DoExtract = (DoExtractProc)GetProcAddress(hPlugin, "DoExtract");
			if (Match == nullptr || DoExtract == nullptr)
			{
				FreeLibrary(hPlugin);
				return S_FALSE;
			}
			if (!Match(FileNamePool[0].c_str()))
			{
				FreeLibrary(hPlugin);
				return S_FALSE;
			}
			else
			{
				ULONG iPos = 1;
				BOOL IsFullStatement = FALSE;
				FreeLibrary(hPlugin);
				for (auto it : FileNamePool)
				{
					if (iPos == CoreCount + 1 && IsFullStatement == FALSE)
					{
						IsFullStatement = TRUE;
					}
					iPos %= CoreCount;
					if (IsFullStatement == FALSE)
					{
						hPlugin = LoadLibraryW(PluginPath.c_str());
						SeparatorArgsHost* HostInfo = new SeparatorArgsHost(Args);
						HostInfo->SetTaskId(iPos);
						DoExtract(it.c_str(), HostInfo);
						TaskHandle Info;
						Info.hImage = hPlugin;
						HostInfo->GetThreadId(&(Info.ThreadId));
						HostInfo->GetThreadHandle(&(Info.hThread));
						Info.TaskId = iPos;
						TaskPool.push_back(Info);
						BOOL Value = FALSE;
						InterlockedExchangePointer(&(this->IsTaskFinished), &Value);
					}
					else
					{
						TaskHolder->PushTask(iPos, it, PluginName);
					}
					iPos++;
				}
			}
		}
	}
	else
	{
		//test all plugins
		for (auto it : FileNamePool)
		{
			WIN32_FIND_DATAW Data = { 0 };
			HANDLE hFind = FindFirstFileW(L"Axm\\*.axm", &Data);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				return S_FALSE;
			}
			HMODULE hPlugin = nullptr;
			while (FindNextFileW(hFind, &Data))
			{
				wstring PluginPath(L"Axm\\");
				PluginPath += Data.cFileName;
				ULONG iPos = 1;
				BOOL IsFullStatement = FALSE;
				hPlugin = LoadLibraryW(PluginPath.c_str());
				if (hPlugin)
				{
					MatchProc Match = (MatchProc)GetProcAddress(hPlugin, "Match");
					DoExtractProc DoExtract = (DoExtractProc)GetProcAddress(hPlugin, "DoExtract");
					if (Match == nullptr || DoExtract == nullptr)
					{
						FreeLibrary(hPlugin);
						goto NextFile;
					}
					if (!Match(it.c_str()))
					{
						FreeLibrary(hPlugin);
						goto NextFile;
					}
					FreeLibrary(hPlugin);
					if (iPos == CoreCount + 1 && IsFullStatement == FALSE)
					{
						IsFullStatement = TRUE;
					}
					iPos %= CoreCount;
					if (IsFullStatement == FALSE)
					{
						hPlugin = LoadLibraryW(PluginPath.c_str());
						SeparatorArgsHost* HostInfo = new SeparatorArgsHost(Args);
						HostInfo->SetTaskId(iPos);
						DoExtract(it.c_str(), HostInfo);
						TaskHandle Info;
						Info.hImage = hPlugin;
						HostInfo->GetThreadId(&(Info.ThreadId));
						HostInfo->GetThreadHandle(&(Info.hThread));
						Info.TaskId = iPos;
						TaskPool.push_back(Info);
						BOOL Value = FALSE;
						InterlockedExchangePointer(&(this->IsTaskFinished), &Value);
					}
					else
					{
						TaskHolder->PushTask(iPos, it, PluginName);
					}
					iPos++;
				}
			}//while
		NextFile:
			__asm{nop}
		}
	}
	return S_OK;
}

typedef HRESULT(WINAPI* AxsEngineEntryProc)(const WCHAR* ScriptName, ULONG Cores, LPVOID o);
HRESULT WINAPI SeparatorTask::LaunchScriptTask(wstring& Name, wstring& Args, ULONG Cores)
{
	Reset();
	SetCPUCore(Cores);
	this->TaskPool.resize(1);
	SeparatorTaskType = TaskType::ScriptTask;
	SetCurrentArgs(Args);
	//Do not use multi task here
	//Separator will provide some thread-related apis that allow you create threads in script
	SeparatorArgsHost* Host = new SeparatorArgsHost(Args);
	///Block
	///won't return until Script Thread received a quit message
	HMODULE hScrMod = LoadLibraryW(L"AxsEngine.dll");
	if (hScrMod)
	{
		AxsEngineEntryProc AxsEngineEntry = (AxsEngineEntryProc)GetProcAddress(hScrMod, "AxsEngineEntry");
		if (AxsEngineEntry == nullptr)
		{
			MessageBoxW(NULL, L"Invalid Script Plugin", L"FATAL", MB_OK);
			return S_FALSE;
		}
		else
		{
			Host->SetTaskId(0);
			Host->SetThreadInfo(INVALID_HANDLE_VALUE, 0);
			TaskHandle Info;
			Info.ArgsHolder = Host;
			Info.hThread = INVALID_HANDLE_VALUE;
			Info.hImage = nullptr;
			Info.TaskId = 0;
			Info.ThreadId = 0;
			TaskPool.push_back(Info);
			HRESULT Result = AxsEngineEntry(Name.c_str(), Cores, Host);
			Host->Release();
			FreeLibrary(hScrMod);
			return Result;
		}
	}
	else
	{
		MessageBoxW(NULL, L"Couldn't load AxsEngine.dll", L"FATAL", MB_OK);
		return S_FALSE;
	}
}

HRESULT WINAPI SeparatorTask::GetTaskStatement()
{
	if (this->IsTaskFinished == FALSE)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


HRESULT WINAPI SeparatorTask::SetCurrentArgs(wstring& Args)
{
	this->CurrentArgs = Args;
	return S_OK;
}

HRESULT WINAPI SeparatorTask::GetCurrentArgs(wstring& Args)
{
	Args = this->CurrentArgs;
	return S_OK;
}

//private
//Process Task List
ULONG WINAPI CleanThreadProc(LPVOID Param)
{
	SeparatorTask* task = (SeparatorTask*)Param;
	MSG Message;
	ULONG IndexCounter = 1;

	while (GetMessageW(&Message, NULL, NULL, NULL))
	{
		if (Message.message == XMOE_MSG_EXIT)
		{
			return 0;
		}
		else if (task->SeparatorTaskType == TaskType::FileTask ||
			task->SeparatorTaskType == TaskType::FolderTask)
		{
			if (task->GetTaskStatement() == S_FALSE)
			{
				ULONG Index = IndexCounter % task->CoreCount;
				IndexCounter++;
				ULONG ExitCode;
				task->Locker->Lock();
				GetExitCodeThread(task->TaskPool[Index].hThread, &ExitCode);
				if (ExitCode != STILL_ACTIVE)
				{
					FreeLibrary(task->TaskPool[Index].hImage);
					//auto it = task->TaskPool.begin() + Index;
					//task->TaskPool.erase(it);
					if (task->TaskPool[Index].ArgsHolder)
					{
						task->TaskPool[Index].ArgsHolder->Release();
						task->TaskPool[Index].ArgsHolder = nullptr;
					}
					wstring RequestFileName;
					wstring RequestPluginName;
					MatchProc Match = nullptr;
					DoExtractProc DoExtract = nullptr;
					wstring CurrentArgs;
					task->GetCurrentArgs(CurrentArgs);
					BOOL Result = task->TaskHolder->GetTask(Index, RequestFileName, RequestPluginName);
					if (Result)
					{
						HMODULE hPlugin = LoadLibraryW(RequestPluginName.c_str());
						if (hPlugin)
						{
							Match = (MatchProc)GetProcAddress(hPlugin, "Match");
							DoExtract = (DoExtractProc)GetProcAddress(hPlugin, "DoExtract");
						}
						if (Match != nullptr && DoExtract != nullptr)
						{
							task->TaskPool[Index].hImage = hPlugin;
							SeparatorArgsHost* HostInfo = new SeparatorArgsHost(CurrentArgs);
							HostInfo->SetTaskId(Index);
							DoExtract(RequestFileName.c_str(), HostInfo);
							task->TaskPool[Index].hImage = hPlugin;
							HostInfo->GetThreadId(&(task->TaskPool[Index].ThreadId));
							HostInfo->GetThreadHandle(&(task->TaskPool[Index].hThread));
							task->TaskPool[Index].TaskId = Index;
							BOOL Value = FALSE;
							InterlockedExchangePointer(&(task->IsTaskFinished), &Value);
						}
					}
					else
					{
						//clean up
						task->TaskPool[Index].hImage = nullptr;
						task->TaskPool[Index].hThread = INVALID_HANDLE_VALUE;
						task->TaskPool[Index].TaskId = 0;
						task->TaskPool[Index].ThreadId = 0;
					}
				}
				task->Locker->Release();
			}
			Sleep(20);
		}
		TranslateMessage(&Message);
		DispatchMessageW(&Message);
	}
	return 0;
}

HRESULT WINAPI SeparatorTask::CleanTask()
{
	this->hCleanThread = CreateThread(NULL, NULL, CleanThreadProc, this, NULL, &(this->CleanThreadId));
	if (this->hCleanThread != INVALID_HANDLE_VALUE)
	{
		SetThreadPriority(this->hCleanThread, THREAD_PRIORITY_BELOW_NORMAL);
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}
