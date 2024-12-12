#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <map>
#include "detours.h"
#include "IATProc.h"
#include "crc32.h"
#include <Psapi.h>
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "Psapi.lib")
using namespace std;
static DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
static LPWSTR ctowJIS(char* str)
{
	static wchar_t result[1024];
	int len = MultiByteToWideChar(932, 0, str, -1, NULL, 0);
	MultiByteToWideChar(932, 0, str, -1, result, len);
	result[len] = L'\0';
	return result;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Open Console Success!" << endl;
}

static char* wtoc(LPCTSTR str)
{
	static char result[1024];
	int len = WideCharToMultiByte(936, 0, str, -1, NULL, 0, NULL, 0);
	WideCharToMultiByte(936, 0, str, -1, result, len, NULL, 0);
	result[len] = '\0';
	return result;
}

static char* wtocUTF(LPCTSTR str)
{
	static char result[1024];
	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, 0);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, result, len, NULL, 0);
	result[len] = '\0';
	return result;
}


static LPWSTR ctowUTF(char* str)
{
	static wchar_t result[1024];
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, result, len);
	result[len] = L'\0';
	return result;
}

static bool Hook(PVOID addr, PVOID func)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&addr, func);
	if (DetourTransactionCommit() != NOERROR)
		return false;
	return true;
}