#include <Windows.h>
#include "WinFile.h"

enum MemoryFlag
{
	MemoryAllocNormal,
	MemoryAllocZero
};

extern "C" __declspec(dllexport)
IStream* WINAPI sceOpenFile(const WCHAR* FileName, int method)
{

}

extern "C" __declspec(dllexport)
BOOL WINAPI sceReleaseFile(WinFile* File)
{
	if (File == nullptr || IsBadReadPtr(File, sizeof(WinFile)))
	{
		return FALSE;
	}
	else
	{
		File->Release();
		return TRUE;
	}
}

extern "C" __declspec(dllexport)
LPVOID WINAPI sceMemoryAlloc(ULONG Flag, ULONG Size)
{
	if (Flag == MemoryFlag::MemoryAllocNormal)
	{
		return HeapAlloc(GetProcessHeap(), 0, Size);
	}
	else
	{
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Size);
	}
}

extern "C" __declspec(dllexport)
BOOL WINAPI sceMemoryFree(LPVOID Buffer)
{
	if (Buffer)
	{
		return HeapFree(GetProcessHeap(), 0, Buffer);
	}
	return FALSE;
}

