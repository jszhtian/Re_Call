// dllmain.cpp : 定义 DLL 应用程序的入口点。
//For ティンクル☆くるせいだーす -Passion Star Stream-
#include "pch.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "crc32.h"
#include <iomanip>
#pragma comment(lib, "detours.lib")
using namespace std;
ofstream TXT("Text.txt");

//map<DWORD, wstring> SCRList;
//map<DWORD, wstring> REPList;

LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}

wchar_t* ctow(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(CP_UTF8, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(CP_UTF8, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}

wchar_t* ctow936(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(936, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(936, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

BOOL IsJis(char* text, rsize_t len)
{
	for (int i = 0; i < len; i++)
	{
		if ((USHORT)text[i] > 0x7F)
			return TRUE;
	}
	return FALSE;
}

void WINAPI HookText(void* SourceBuff, rsize_t SourceSize)
{
	//cout << SourceSize << endl;
	char* text = new char[SourceSize];
	memset(text, 0, SourceSize);
	memcpy(text, SourceBuff, SourceSize);
	//if(IsJis(text, SourceSize))
		TXT << text << endl;
	delete[] text;
}

PVOID pHookText = 0;
_declspec(naked) void gHookText()
{
	_asm
	{
		pushad;
		pushfd;
		push edi;
		push ebp;
		call HookText;
		popfd;
		popad;
		jmp pHookText;
	}
}

void Hook()
{
	DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
	pHookText = (PVOID)(BaseAddr + 0x2501);
	DetourTransactionBegin();
	DetourAttach((void**)&pHookText, gHookText);
	if (DetourTransactionCommit() != NOERROR) 
	{ 
		MessageBox(NULL, L"ERROR2", L"Crescendo", MB_OK); 
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		make_console();
		Hook();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void) {
	return;
}
/*
char * OLDSTR(int String, int a2, LPCSTR lpString, int a4, int a5)
{
	cout << "lpString:" << lpString << endl;
	cout << " lpString:" << String << endl;
	flog << " lpString:" << lpString << endl;
	flog <<  " lpString:" << String << endl;
	return OLDSTR(String, a2, lpString,  a4,  a5);
}*/

