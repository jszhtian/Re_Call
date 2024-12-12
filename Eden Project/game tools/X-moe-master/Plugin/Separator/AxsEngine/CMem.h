#ifndef _CMem_
#define _CMem_

#include <malloc.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <memory>
#endif

class CMem
{
public:
	static void* __fastcall Alloc(ULONG Size)
	{
#ifdef WIN32
		return HeapAlloc(GetProcessHeap(), 0, Size);
#else
		return malloc(Size);
#endif
	}

	static void* __fastcall AllocZero(ULONG Size)
	{
#ifdef WIN32
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Size);
#else
		PVoid Result = malloc(Size);
		if (Result)
		{
			memset(Result, 0, Size);
		}
		return Result;
#endif
	}

	static void* __fastcall ReAlloc(PVOID Buffer, ULONG Size)
	{
#ifdef WIN32
		return HeapReAlloc(GetProcessHeap(), 0, Buffer, Size);
#else
		return realloc(Buffer, Size);
#endif
	}

	static void* __fastcall StackAlloc(ULONG Size)
	{
		return _alloca(Size);
	}

	static void __fastcall Free(PVOID Buffer)
	{
#ifdef WIN32
		HeapFree(GetProcessHeap(), 0, Buffer);
#else
		free(Buffer);
#endif
	}

};

#endif
