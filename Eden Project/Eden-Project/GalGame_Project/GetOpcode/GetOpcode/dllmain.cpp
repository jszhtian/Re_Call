/**********************************
	Ever tried. Ever failed.
	No matter. Try again.
	Fail again. Fail better.
 **********************************/
#include "pch.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
ofstream TXT("log.txt");
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

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
	char *out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}
/**********************************************************************************************************************/
/*DC3WY
static DWORD HookPos1 = 0x2F94D;
static DWORD HookPos2 = 0x2F9BB;
static DWORD HookPos3 = 0x2F9F6;
static DWORD HookPos4 = 0x2FA62;
*/

static DWORD HookPos1 = 0x387CD;
static DWORD HookPos2 = 0x3883B;
static DWORD HookPos3 = 0x38876;
static DWORD HookPos4 = 0x388E2;

void __stdcall PrintOp(int op)
{
	cout << "Get Opcode:0x" << hex << setw(4) << setfill('0') << op << endl;
	TXT << "Get Opcode:0x" << hex << setw(4) << setfill('0') << op << endl;
}


void __stdcall PrintMessage(char* Text)
{
	cout << Text << endl;
	TXT << Text << endl;
}

char PrintCash[MAX_PATH] = { 0 };
void __stdcall PrintText(char* Text)
{
	lstrcpyA(PrintCash, Text);
	for (int i = 0; i < strlen(Text); i++)
		PrintCash[i] += 0x20;
	cout << wtocGBK(ctowJIS(PrintCash)) << endl;
	TXT << PrintCash << endl;
}

static PVOID pGetOpcode = (PVOID)(BaseAddr + HookPos1);
__declspec(naked) void GetOpcode()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call PrintOp
		popfd
		popad
		jmp pGetOpcode
	}
}


static PVOID pGetMessage = (PVOID)(BaseAddr + HookPos2);
static PVOID pGetMessage2 = (PVOID)(BaseAddr + HookPos3);
__declspec(naked) void MyGetMessage()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call PrintMessage
		popfd
		popad
		jmp pGetMessage
	}
}

__declspec(naked) void MyGetMessage2()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call PrintMessage
		popfd
		popad
		jmp pGetMessage2
	}
}

static PVOID pGetText = (PVOID)(BaseAddr + HookPos4);
__declspec(naked) void GetText()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call PrintText
		popfd
		popad
		jmp pGetText
	}
}
void HookEngineMainFunc()
{
	cout << pGetOpcode << endl;
	cout << pGetMessage << endl;
	cout << pGetMessage2 << endl;
	cout << pGetText << endl;
	DetourTransactionBegin();
	DetourAttach((void**)&pGetOpcode, GetOpcode);
	DetourAttach((void**)&pGetMessage, MyGetMessage);
	DetourAttach((void**)&pGetMessage2, MyGetMessage2);
	DetourAttach((void**)&pGetText, GetText);
	if (DetourTransactionCommit() != NOERROR) 
		MessageBox(NULL, L"HookEngineMainFunc  ERROR", L"AymKze", MB_OK | MB_ICONERROR);
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
		HookEngineMainFunc();
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