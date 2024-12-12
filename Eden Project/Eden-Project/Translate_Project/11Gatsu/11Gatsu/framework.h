// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <tchar.h>
#include <iomanip>
#include <mutex>
#include <stdio.h>
#include <map>
#include <string>
#include <shlwapi.h>
#include "detours.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <sstream>
#include <vector>
//#include "FileCopy.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
static DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
using namespace std;
static PVOID JMPHookPoint;
static PVOID JMPHookPointNext;
static PVOID JMPHookPoint2;
static PVOID JMPHookPointNext2;
static PVOID TmpJmp;
static DWORD BufferAddr;
static std::mutex mtx;
static char* EdnFileBuffer;
static bool debug = false;


static void make_console()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

static bool JMPHelper(PVOID HookPoint, PVOID JMP_FUNC)
{
	DWORD oldProtect = 0;
	if (VirtualProtect(HookPoint, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
		return false;
	DWORD offset = (DWORD)JMP_FUNC - (DWORD)HookPoint - 5;
	char JMPOp[] = { 0xE9 };
	memcpy(HookPoint, &JMPOp, 1);
	memcpy((char*)HookPoint + 1, &offset, sizeof(DWORD));
	if (VirtualProtect(HookPoint, 5, oldProtect, &oldProtect))
		return false;
	return true;
}

static void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}

static bool ReplaceJump(PBYTE src, PBYTE dst)
{
	DWORD oldProtect;

	if (VirtualProtect((LPVOID)src, 10, PAGE_EXECUTE_READWRITE, &oldProtect)) {

		src[0] = 0xE9;
		*(DWORD*)& src[1] = (DWORD)(dst - src - 5);
		return true;
	}
	return false;
}

static void SetNopCode(int pnop, int size)
{
	BYTE Patch1[] = { 0xFE };
	for (int i = 0; i < size; i++) 
	{
		memcopy((void*)(pnop + i), Patch1, sizeof(Patch1));
	}
}

static void MEM_SetNopCode(BYTE* lpNop, SIZE_T dwSize)
{
	DWORD op;

	SIZE_T i;
	PBYTE adr;
	BYTE nix = 0x90;
	VirtualProtect(lpNop, dwSize, PAGE_EXECUTE_READWRITE, &op);

	for (i = 0; i < dwSize; i++)
	{
		adr = &lpNop[i];
		WriteProcessMemory(GetCurrentProcess(), adr, &nix, sizeof(BYTE), NULL);
	}
}
/************************************************************************************************************/

/****************************************************************************************************************/