#include "tThread.h"

tThread::tThread() :
	iter(0),
	isExit(false),
	_state(Readying),
	ThreadID(39), //avoid terminating main thread
	ScriptPos(4) //first function
{
	iter = &ctx;
}

tThread::~tThread()
{
	//do nothing here
}


void tThread::Sleep()
{
	if(_state & Suspending)
		return;
	else
		_state |= Suspending;
}

void tThread::Raise()
{
	if(_state & Suspending)
		_state &= ~Suspending;
}

bool tThread::Create(unsigned int _pos, unsigned char _id)
{
	ScriptPos = _pos;
	ThreadID  = _id;
	ctx.ScriptPos = _pos;
	return true;
}

//we shouldn't call 'delete' after we released it
bool tThread::Exit()
{
	try
	{
		this->~tThread();
	}
	catch(...)
	{
		return false;
	}
	return true;
}
