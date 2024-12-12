#ifndef _tThread_H_
#define _tThread_H_

#include "ScriptObject.h"

//inner thread
class tThread
{
public:
	   //ThreadNext�Ǵ�Ready��Run�м��л��� ��������Sleep��Raise�������ӵ���
	enum{ Readying = 1, Running = 2, Suspending = 4};

	tThread();
	~tThread();

	void Sleep();
	void Raise();
	bool Create(unsigned int _pos, unsigned char _id);
	bool Exit();
	
	ScriptObject *iter;
	ScriptObject ctx;
	bool isExit;//���½����̡߳���ʱ���Ȱ�ThreadPool���Ѿ���ȥ���̸߳�������
	int  _state;
	unsigned char ThreadID;
	unsigned int  ScriptPos;
};

#endif
