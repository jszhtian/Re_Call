#ifndef _tThread_H_
#define _tThread_H_

#include "ScriptObject.h"

//inner thread
class tThread
{
public:
	   //ThreadNext是从Ready和Run中间切换， 单独调用Sleep和Raise用于无视调度
	enum{ Readying = 1, Running = 2, Suspending = 4};

	tThread();
	~tThread();

	void Sleep();
	void Raise();
	bool Create(unsigned int _pos, unsigned char _id);
	bool Exit();
	
	ScriptObject *iter;
	ScriptObject ctx;
	bool isExit;//在新建“线程”的时候先把ThreadPool中已经死去的线程给清理了
	int  _state;
	unsigned char ThreadID;
	unsigned int  ScriptPos;
};

#endif
