// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <mutex>
#include <vector>
#include <functional>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
//ofstream TXT("Text.txt");

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
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

char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

LPWSTR ctowGBK(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(936, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(936, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

LPWSTR ctow(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_ACP, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

PVOID g_pOldSetWindowTextW = NULL;
typedef int (WINAPI* PfuncSetWindowTextW)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextW(HWND hwnd, LPWSTR lpString)
{
	lstrcpy(lpString, L"【白井木学园汉化组】命运交织的塔罗牌-简体中文Ver1.0版本-参与汉化&交流&讨论请加群171900769");
	return ((PfuncSetWindowTextW)g_pOldSetWindowTextW)(hwnd, lpString);
}

PVOID g_pOldCreateFileW = CreateFileW;
typedef HANDLE(WINAPI* PfuncCreateFileW)(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	);
HANDLE WINAPI NewCreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	if (lstrcmp(lpFileName,L".\\data2.dat")==0)
	{
		return ((PfuncCreateFileW)(g_pOldCreateFileW))(
			L".\\malie_cn.kze",
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);
	}
	else 
	{
		return ((PfuncCreateFileW)(g_pOldCreateFileW))(
			lpFileName,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);
	}
}

/***********************************************************************
DWORD TextSize = 0;
void __stdcall AymGetName(void* Text)
{
	cout << wtocGBK((LPCTSTR)Text) << endl;
	TXT << wtocUTF((LPCTSTR)Text) << endl;
}

void __stdcall AymGetTextSize(DWORD size)
{
	TextSize = size;
}


void __stdcall AymGetText(wchar_t* Text, DWORD size)
{
	//MalieString str(wstring(Text, size));
	//str.init();
	//auto new_str = str.import_str(make_str_func(index));
	int VOICE = 0;
	for (int i = 0; i < size-2; i++)
	{
		if (Text[i] == 0x07)
		{
			if (Text[i + 1] == 0x01)
			{
				Text += 2;
			}
			if (Text[i + 1] == 0x08)
			{
				Text += 12;
				VOICE = 1;
			}
		}
		if (Text[i] == 0x00)
		{
			Text += 1;
		}
	}
	cout << wtocGBK(Text) << endl;
}
/*
void __stdcall AymGetText(wchar_t* Text,DWORD size)
{
	//void* Chip = malloc(TextSize);//这里要改成中文的文本长度
	//memset(Chip, 0, TextSize+0x2);
	//memcpy(Chip, Text, TextSize);
	wchar_t* cmp2 = (wchar_t*)L"　学校からの帰り道。その日はいつも通り友達の少女と二人でランドセルを揺らしながら、家への道を歩いていた。";
	wchar_t* chs2 = (wchar_t*)L"　那天，如同往常一样，我和一位跟我要好的少女，一起背着双肩包，摇晃地走在放学回家的路上。\x07\x06";
	DWORD SPACE = 0x3000;
	if (!memcmp(Text, cmp2, size - 0x4))
	{
		memset(Text, 0, size);
		memcpy(Text, chs2, lstrlenW(chs2)*2);
	}
	cout << size << endl;
	cout <<  lstrlenW(chs2) << endl;
	//memset(Text, 0, TextSize);
	//lstrcpy((LPWSTR)Text, L"This is a test");
}

PVOID pGetTextSize = NULL;
__declspec(naked) void GetTextSize()
{
	_asm
	{
		pushad
		pushfd
		push dword ptr ds : [eax + 0x4]
		call AymGetTextSize
		popfd
		popad
		jmp pGetTextSize
	}
}


PVOID pGetName = NULL;
__declspec(naked) void GetName()
{
	_asm
	{
		pushad
		pushfd
		push dword ptr ss : [ebp + 0x10]
		call AymGetName
		popfd
		popad
		jmp pGetName
	}
}


PVOID pGetText = NULL;
__declspec(naked) void GetText()
{
	_asm
	{
		pushad
		pushfd
		push edx
		push edi
		call AymGetText
		popfd
		popad
		jmp pGetText
	}
}
*/
void Hook()
{
	DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
	//pGetName = (PVOID)(BaseAddr + 0x261e3a);
	//pGetTextSize = (PVOID)(BaseAddr + 0x276311);
	//pGetText = (PVOID)(BaseAddr + 0x27631D);
	//cout << "BaseAddress:0x" << hex << (PVOID)BaseAddr << endl;
	//cout << "HookAddress:0x" << hex << pGetName << endl;
	//cout << "HookAddress:0x" << hex << pGetTextSize << endl;
	//cout << "HookAddress:0x" << hex << pGetText << endl;
	g_pOldSetWindowTextW = DetourFindFunction("USER32.dll", "SetWindowTextW");
	DetourTransactionBegin();
	//DetourAttach((void**)&pGetName, GetName);
	//DetourAttach((void**)&pGetTextSize, GetTextSize);
	//DetourAttach((void**)&pGetText, GetText);
	DetourAttach(&g_pOldSetWindowTextW, NewSetWindowTextW);
	DetourAttach(&g_pOldCreateFileW, NewCreateFileW);
	if(DetourTransactionCommit()!=NOERROR)
	{ 
		MessageBox(NULL, L"ERROR", L"AymKze", MB_OK);
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
		//make_console();
		Hook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

