#include "CoroutineManager.h"

CoroutineManager* CoroutineManager::Handle = NULL;

CoroutineManager* CoroutineManager::Init()
{
	if (Handle == NULL)
	{
		try
		{
			Handle = new CoroutineManager;
		}
		catch (...)
		{
			AllocError("CoroutineManager");
		}
	}
	return Handle;
}

CoroutineManager* CoroutineManager::GetHandle()
{
	return Handle;
}

CoroutineManager::CoroutineManager() :
	tPtr(0),
	Sc(0),
	tLock(0),
	tLockId(-1)
{
	memset(ThreadPool, 0, sizeof(ThreadPool));
	Sc = ThreadPool[0];
}

CoroutineManager::~CoroutineManager()
{
	for(int i = 0; i < 40; i++)
	{
		if(ThreadPool[i])
			delete ThreadPool[i];
	}
}


void CoroutineManager::ThreadNext()
{
	if(tLock && ThreadPool[tLock] != Sc)
	{
		if(ThreadPool[tLock] != 0)
		{
			Sc->_state &= ~tThread::Running;
			Sc->_state |=  tThread::Readying;

			tPtr = tLockId;
			Sc = ThreadPool[tLockId];
			Sc->_state &= ~tThread::Readying;
			Sc->_state |=  tThread::Running;

			return;
		}
	}

	Sc->_state &= ~tThread::Running;
	Sc->_state |=  tThread::Readying;
	++tPtr;
	while(1)
	{
		if(ThreadPool[tPtr] != NULL)
		{
			if(ThreadPool[tPtr]->_state & tThread::Readying)
			{
				break;
			}
		}
		else
			++tPtr %= 40;
	}
	Sc = ThreadPool[tPtr];
	Sc->_state &= ~tThread::Readying;
	Sc->_state |=  tThread::Running;
}

void CoroutineManager::Sleep(unsigned _id)
{
	if(ThreadPool[_id] == NULL)
		return;

	if(tLock)
	{
		if(tLockId == _id)
			return;
	}

	ThreadPool[_id]->Sleep();
}


void CoroutineManager::Raise(unsigned _id)
{
	if(ThreadPool[_id] == NULL)
		return;

	if(tLock)
	{
		if(tLock == _id)
			return;
	}
	ThreadPool[_id]->Raise();
}

bool CoroutineManager::Add(unsigned id, unsigned int _pos)
{
	if(ThreadPool[id])
	{
		if(ThreadPool[id]->isExit)
		{
			if(tLock && id == tLockId)
			{
				tLock   = 0;
				tLockId = -1;
			}
			delete ThreadPool[id];
		}
		else
			return false;
	}
	else
		ThreadPool[id] = new tThread;

	bool tRet = ThreadPool[id]->Create(_pos, id);
	if (id == 0)
	{
		//Update Main Coroutine Handle
		Sc = ThreadPool[0];
	}
	ThreadPool[id]->ctx.ThreadID = id;
	return tRet;
}

bool CoroutineManager::Release(unsigned id)
{
	if(ThreadPool[id])
	{
		bool ret = ThreadPool[id]->Exit();
		ThreadPool[id] = NULL;
		if(tLock)
		{
			if(tLockId == id)
			{
				tLockId = -1;
				tLock   = 0;
			}
		}
		return ret;
	}
	else
		return false;
}

void CoroutineManager::DummyRelease(unsigned id)
{
	if(ThreadPool[id])
	{
		bool ret = ThreadPool[id]->isExit = true;
		if(tLock)
		{
			if(tLockId == id)
			{
				tLockId = -1;
				tLock   = 0;
			}
		}
	}
}

//还需要在Thread自行退出的地方检测
void CoroutineManager::Wait(unsigned id)
{
	if(ThreadPool[id] == NULL)
		return;

	if(tLock)
		return;

	if(ThreadPool[id]->_state & tThread::Suspending)
		return;

	tLock = 1;
	tLockId = id;
}

void CoroutineManager::ReturnVoid(ScriptObject *& stack_obj)
{
	ScriptObject *temp = stack_obj;

	temp = stack_obj->prev;
	stack_obj = stack_obj->prev;
	ThreadPool[temp->ThreadID]->iter = stack_obj;
	delete temp->next;
	temp->next = 0;
}

void CoroutineManager::ReturnValue(ScriptObject *& stack_obj)
{
	stack_obj->prev->stackinfo.push(stack_obj->EAX);
	ScriptObject *temp = stack_obj;
	temp = stack_obj->prev;

	stack_obj = stack_obj->prev;
		
	ThreadPool[temp->ThreadID]->iter = stack_obj;
	delete temp->next;
	temp->next = 0;
}

void CoroutineManager::SetIterNext(ScriptObject *& stack_obj)
{
	ThreadPool[stack_obj->ThreadID]->iter = stack_obj->next;
	Sc->iter = stack_obj->next;
	
}
