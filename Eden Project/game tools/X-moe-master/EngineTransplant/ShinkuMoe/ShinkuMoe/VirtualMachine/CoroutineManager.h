#pragma once

#include "VirtualThread.h"
#include "ScriptObject.h"

#define ThreadInstance CoroutineManager* _co = CoroutineManager::GetHandle();

class CoroutineManager
{
private:
	CoroutineManager();
	static CoroutineManager* Handle;

public:
	static CoroutineManager* Init();
	static CoroutineManager* GetHandle();

public:
	~CoroutineManager();

	void ThreadNext();
	void Wait(unsigned _id);

	bool Add(unsigned id, unsigned int _pos);
	bool Release(unsigned id);
	void DummyRelease(unsigned id);

	void Sleep(unsigned _id);
	void Raise(unsigned _id);

	void ReturnVoid(ScriptObject *& stack_obj);
	void ReturnValue(ScriptObject *& stack_obj);

	void SetIterNext(ScriptObject *& stack_obj);

	tThread* Sc;//正在执行的thread
	int      tPtr;
	int      tLock;//Wait的执行锁
	int      tLockId;

	tThread* ThreadPool[40];
};
