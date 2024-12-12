#ifndef _Mutex_
#define _Mutex_

#include <Windows.h>

class Mutex
{
public:
	Mutex();
	~Mutex();

	void Lock();
	void Release();

private:
	CRITICAL_SECTION* Holder;
};

#endif
