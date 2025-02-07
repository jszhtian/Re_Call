//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2007 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// safe 64bit System Tick Count
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include <mmsystem.h>
#ifdef __BORLANDC__
#include <syncobjs.hpp>
#endif
#include "tjsUtils.h"
#include "TickCount.h"
#include "SysInitIntf.h"
#include "ThreadIntf.h"


//---------------------------------------------------------------------------
// 64bit may enough to hold usual time count.
// ( 32bit is clearly insufficient )
//---------------------------------------------------------------------------
static tjs_uint64 TVPTickCountBias = 0;
static DWORD TVPWatchLastTick;
static tTJSCriticalSection TVPTickWatchCS;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
static DWORD TVPCheckTickOverflow()
{
	DWORD curtick;
	{	// thread-protected
		tTJSCriticalSectionHolder holder(TVPTickWatchCS);

		curtick = timeGetTime();
		if(curtick < TVPWatchLastTick)
		{
			// timeGetTime() was overflowed
			TVPTickCountBias += 0x100000000L; // add 1<<32
		}
		TVPWatchLastTick = curtick;
	}	// end-of-thread-protected
	return curtick;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
class tTVPWatchThread : public tTVPThread
{
	// thread which watches overflow of 32bit counter of timeGetTime

	tTVPThreadEvent Event;

public:

	tTVPWatchThread();
	~tTVPWatchThread();

protected:
	void Execute();

} static * TVPWatchThread = NULL;
//---------------------------------------------------------------------------
tTVPWatchThread::tTVPWatchThread() : tTVPThread(true)
{
	TVPWatchLastTick = timeGetTime();
	SetPriority(ttpNormal);
	Resume();
}
//---------------------------------------------------------------------------
tTVPWatchThread::~tTVPWatchThread()
{
	Terminate();
	Resume();
	Event.Set();
	WaitFor();
}
//---------------------------------------------------------------------------
void tTVPWatchThread::Execute()
{
	while(!GetTerminated())
	{
		TVPCheckTickOverflow();

		Event.WaitFor(0x10000000);
			// 0x10000000 will be enough to watch timeGetTime()'s counter overflow.
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
static void TVPWatchThreadInit()
{
	if(!TVPWatchThread)
	{
		TVPWatchThread = new tTVPWatchThread();
	}
}
//---------------------------------------------------------------------------
static void TVPWatchThreadUninit()
{
	if(TVPWatchThread)
	{
		delete TVPWatchThread;
		TVPWatchThread = NULL;
	}
}
//---------------------------------------------------------------------------
static tTVPAtExit TVPWatchThreadUninitAtExit(TVP_ATEXIT_PRI_SHUTDOWN,
	TVPWatchThreadUninit);
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// TVPGetTickCount
//---------------------------------------------------------------------------
tjs_uint64 TVPGetTickCount()
{
	TVPWatchThreadInit();

	DWORD curtick = TVPCheckTickOverflow();

	return curtick + TVPTickCountBias;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// TVPStartTickCount
//---------------------------------------------------------------------------
void TVPStartTickCount()
{
	TVPWatchThreadInit();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// TVPGetRoughTickCount
//---------------------------------------------------------------------------
tjs_uint32 TVPGetRoughTickCount32()
{
	// inaccurate but fast GetTickCount.
	// this does not cause to load winmm.dll.
	return GetTickCount();
}
//---------------------------------------------------------------------------

