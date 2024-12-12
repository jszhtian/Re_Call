#ifndef _SeparatorArgsHost_
#define _SeparatorArgsHost_

#include "SeparatorArgs.h"
#include <string>
#include <vector>
#include "ToolChain.h"

using std::wstring;
using std::vector;

class SeparatorArgsHost : public SeparatorArgs
{
public:
	SeparatorArgsHost(wstring& ArgsList):
		hThread(INVALID_HANDLE_VALUE),
		ThreadId(0),
		TaskId(0),
		ReferCount(0)
	{
		ArgsCmd = ArgsList;
		wstring TempCmd = ArgsList;
		wstring Info;
		BOOL FoundFirst = FALSE;
		int iPos = 0;
		while ((iPos = TempCmd.find_first_of(L",")) != wstring::npos)
		{
			FoundFirst = TRUE;
			Info = TempCmd.substr(0, iPos);
			TempCmd = TempCmd.substr(iPos + 1, wstring::npos);
			TrimW(Info);

			if (Info.size())
			{
				ArgsPool.push_back(Info);
			}
		}

		if (FoundFirst == FALSE && TempCmd.size())
		{
			TrimW(TempCmd);
			if (TempCmd.size())
			{
				ArgsPool.push_back(TempCmd);
			}
		}
		else if (FoundFirst == TRUE && TempCmd.substr(iPos + 1, wstring::npos).length())
		{
			TempCmd = TempCmd.substr(iPos + 1, wstring::npos);
			TrimW(TempCmd);
			if (TempCmd.size())
			{
				ArgsPool.push_back(TempCmd);
			}
		}

		ReferCount = 1;
	}

	~SeparatorArgsHost()
	{
		ArgsPool.clear();
		ArgsCmd.clear();
	}

	HRESULT WINAPI GetArgs(ULONG Index, WCHAR* Buffer, ULONG MaxLength)
	{
		if (Index >= 0 && Index < (ULONG)ArgsPool.size())
		{
			return S_FALSE;
		}
		if (Buffer == nullptr || IsBadWritePtr(Buffer, MaxLength * 2))
		{
			return S_FALSE;
		}
		if (ArgsPool[Index].length() >= MaxLength)
		{ 
			RtlZeroMemory(Buffer, MaxLength * 2);
			lstrcpynW(Buffer, ArgsPool[Index].c_str(), MaxLength - 1);
			return S_FALSE;
		}
		else
		{
			RtlZeroMemory(Buffer, MaxLength * 2);
			lstrcpynW(Buffer, ArgsPool[Index].c_str(), ArgsPool[Index].length());
			return S_OK;
		}
	}

	HRESULT WINAPI GetArgsCount(ULONG* Count)
	{
		if (Count == nullptr || IsBadReadPtr(Count, sizeof(ULONG)))
		{
			return S_FALSE;
		}
		*Count = (ULONG)ArgsPool.size();
		return S_OK;
	}

	HRESULT WINAPI SetThreadInfo(HANDLE hThread, ULONG ThreadId)
	{
		this->hThread = hThread;
		this->ThreadId = ThreadId;
		return S_OK;
	}

	HRESULT WINAPI GetThreadId(ULONG* ThreadId)
	{
		if (IsBadWritePtr(ThreadId, 4))
		{
			return S_FALSE;
		}
		else
		{
			*ThreadId = this->ThreadId;
			return S_OK;
		}
	}

	HRESULT WINAPI GetThreadHandle(HANDLE* ThreadHandle)
	{
		if (IsBadWritePtr(ThreadHandle, 4))
		{
			return S_FALSE;
		}
		else
		{
			*ThreadHandle = this->hThread;
			return S_OK;
		}
		return S_OK;
	}

	HRESULT WINAPI SetTaskId(ULONG Id)
	{
		this->TaskId = Id;
		return S_OK;
	}

	HRESULT WINAPI GetTaskId(ULONG* Id)
	{
		if (Id != nullptr && !IsBadWritePtr(Id, sizeof(ULONG)))
		{
			*Id = this->TaskId;
			return S_OK;
		}
		else
		{
			return S_FALSE;
		}
	}

	HRESULT WINAPI Release()
	{
		if (ReferCount - 1 > 0)
		{
			ReferCount++;
		}
		else
		{
			delete this;
		}
		return S_OK;
	}

private:
	//no reference
	HRESULT WINAPI AddRefer(ULONG* Refs)
	{
		this->ReferCount++;
		if (Refs == nullptr)
		{
			return S_FALSE;
		}
		if (!IsBadWritePtr(Refs, sizeof(ULONG)))
		{
			*Refs = this->ReferCount;
			return S_OK;
		}
		else
		{
			return S_FALSE;
		}
	}

	HRESULT WINAPI SubRefer(ULONG* Refs)
	{
		this->ReferCount--;
		if (Refs == nullptr)
		{
			return S_FALSE;
		}
		if (!IsBadWritePtr(Refs, sizeof(ULONG)))
		{
			*Refs = this->ReferCount;
			return S_OK;
		}
		else
		{
			return S_FALSE;
		}
	}

private:
	HANDLE hThread;
	ULONG  ThreadId;
	ULONG  TaskId;
	vector<wstring> ArgsPool;
	wstring ArgsCmd;
	ULONG ReferCount;
};

#endif
