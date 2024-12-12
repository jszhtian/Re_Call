#ifndef _SeparatorArgs_
#define _SeparatorArgs_

#include <Windows.h>

//Interface
class SeparatorArgs
{
public:
	virtual HRESULT WINAPI GetArgs(ULONG Index, WCHAR* Buffer, ULONG MaxLength) = 0;
	virtual HRESULT WINAPI GetArgsCount(ULONG* Count) = 0;
	virtual HRESULT WINAPI SetThreadInfo(HANDLE hThread, ULONG ThreadId) = 0;
	virtual HRESULT WINAPI GetThreadId(ULONG* ThreadId) = 0;
	virtual HRESULT WINAPI GetThreadHandle(HANDLE* ThreadHandle) = 0;
	virtual HRESULT WINAPI SetTaskId(ULONG Id) = 0;
	virtual HRESULT WINAPI GetTaskId(ULONG* Id) = 0;
	virtual HRESULT WINAPI Release() = 0;

private:
	virtual HRESULT WINAPI AddRefer(ULONG* Refs) = 0;
	virtual HRESULT WINAPI SubRefer(ULONG* Refs) = 0;
};


#endif
