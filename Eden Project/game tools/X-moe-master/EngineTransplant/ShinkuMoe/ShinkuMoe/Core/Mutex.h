#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class AnzCriticalSection
{
#ifdef _WIN32
	CRITICAL_SECTION CS;

public:
	AnzCriticalSection()
	{
		InitializeCriticalSection(&CS);
	}

	~AnzCriticalSection()
	{
		DeleteCriticalSection(&CS);
	}

	void Enter()
	{
		EnterCriticalSection(&CS);
	}

	void Leave()
	{
		LeaveCriticalSection(&CS);
	}

#else

	pthread_mutex_t CS;

public:
	AnzCriticalSection()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&CS, &attr);
	}

	~AnzCriticalSection()
	{
		pthread_mutex_destroy(&CS);
		delete mutex;
	}

	void Enter()
	{
		pthread_mutex_lock(&CS);
	}

	void Leave()
	{
		pthread_mutex_unlock(&CS);
	}

#endif
};


class AnzCriticalSectionHolder
{
	AnzCriticalSection *Section;

public:
	AnzCriticalSectionHolder(AnzCriticalSection &cs)
	{
		Section = &cs;
		Section->Enter();
	}

	~AnzCriticalSectionHolder()
	{
		Section->Leave();
	}
};

