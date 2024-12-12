#pragma once

#include <stack>
#include <map>
#include "Anz.h"
#include "VarInfo.h"
#include "ArrayManager.h"

/*
Type :
0 - false or refers to nullptr
1 - true
2 - int
3 - float
4 - string
5 - global
6 - array(search key must be a int,1 byte for local, 2 bytes for global)
*/

struct ScriptObject
{
	AnzUInt LocalVarCount;
	AnzUInt ParamVarCount;
	AnzUInt ScriptPos;

	VarInfo LocalStack[0x100];
	AnzUInt8* Buffer;
	VarInfo Eax;

	//local变量开始
	AnzUInt CurStackFrame;
	//P当前Proc的stack base
	AnzUInt BaseStackFrame;

	//Callback Ctrl
	AnzUInt ThreadID;

	ScriptObject* Prev;
	ScriptObject* Next;

	ArrayManager LocalArray;
	
	ScriptObject(): 
		LocalVarCount(0), 
		ParamVarCount(0), 
		ScriptPos(4),
		CurStackFrame(0), 
		BaseStackFrame(0),
		ThreadID(0),
		Prev(nullptr),
		Next(nullptr),
		Buffer(nullptr)
	{
	}

	~ScriptObject()
	{
	}
};
