#ifndef _FunctionExporter_
#define _FunctionExporter_

#include <Windows.h>

struct InterfaceFunctionExporter
{
	virtual void*   __stdcall QueryFunction(const WCHAR* func) = 0;
	virtual HRESULT __stdcall AddTask() = 0;
};


#endif
