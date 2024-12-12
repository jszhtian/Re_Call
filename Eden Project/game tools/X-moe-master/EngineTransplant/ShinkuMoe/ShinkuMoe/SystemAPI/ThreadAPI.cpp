#include "ThreadAPI.h"
#include "CoroutineManager.h"
#include "ErrorLog.h"

//ThreadID 
int __fastcall ThreadExit(ScriptObject *stack)
{
	AnzUInt ThreadID = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	stack->CurStackFrame--;

	if(ThreadID < 0 || ThreadID >= 40)
		return -1;

	WriteLog(fvpLog, "Thread[%02x] exit!\n", ThreadID);

	ThreadInstance
	_co->Release(ThreadID);
	return 0;
}


int __fastcall ThreadNext(ScriptObject *stack)
{
	ThreadInstance
	_co->ThreadNext();
	return 0;
}

int __fastcall ThreadRaise(ScriptObject *stack)
{
	AnzUInt ThreadID = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	stack->CurStackFrame--;

	if(ThreadID < 0 || ThreadID >= 40)
		return -1;

	WriteLog(fvpLog, "Raise Thread[%02x]\n", ThreadID);
	ThreadInstance
	_co->Raise(ThreadID);
	return 0; 
}

//未使用 
int __fastcall ThreadSleep(ScriptObject *stack)
{
	AnzUInt ThreadID = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	stack->CurStackFrame--;

	if(ThreadID < 0 || ThreadID >= 40)
		return -1;
	
	WriteLog(fvpLog, "Sleep Thread[%02x]\n", ThreadID);
	ThreadInstance
	_co->Sleep(ThreadID);
	return 0;
}

//param1 (DWORD)@内部Function 编号
//param2 (u8)   @ThreadID 
int __fastcall ThreadStart(ScriptObject *stack)
{
	AnzUInt Ofs = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	stack->CurStackFrame--;
	AnzUInt ThreadID = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	stack->CurStackFrame--;

	if(ThreadID < 0 || ThreadID >= 40)
		return -1;

	WriteLog(fvpLog, "Thread[%02x] start!\n", ThreadID);
	ThreadInstance
	_co->Add(ThreadID, Ofs);
	return 0;
}


int __fastcall ThreadWait(ScriptObject *stack)
{
	int ThreadID = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	stack->CurStackFrame--;

	if(ThreadID < 0 || ThreadID >= 40)
		return -1;
	
	WriteLog(fvpLog, "Waitint Thread[%02x]\n", ThreadID);
	ThreadInstance
	_co->Wait(ThreadID);
	return 0;
}

