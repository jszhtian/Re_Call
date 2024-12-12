#include "Mutex.h"
#include <Windows.h>

Mutex::Mutex() :
Holder(new CRITICAL_SECTION)
{
	InitializeCriticalSection(Holder);
}

Mutex::~Mutex()
{
	CRITICAL_SECTION* cs = Holder;
	DeleteCriticalSection(cs);
	delete cs;
	Holder = 0;
}

void Mutex::Lock()
{
	EnterCriticalSection(Holder);
}

void Mutex::Release()
{
	LeaveCriticalSection(Holder);
}

