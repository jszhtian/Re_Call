// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <mutex>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
ofstream TXT("Text.txt");

PVOID g_pOldCreateFileA = CreateFileA;
typedef HANDLE(WINAPI* PfuncCreateFileA)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);
HANDLE WINAPI NewCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	if (!lpFileName)
	{
		return INVALID_HANDLE_VALUE;
	}

	string strOldName(lpFileName);
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string strNewName;
	strNewName = strOldName;
	cout << strNewName << endl;
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		strNewName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

typedef HFONT(WINAPI* fnCreateFontA)(
	int nHeight, // logical height of font height
	int nWidth, // logical average character width
	int nEscapement, // angle of escapement
	int nOrientation, // base-line orientation angle
	int fnWeight, // font weight
	DWORD fdwItalic, // italic attribute flag
	DWORD fdwUnderline, // underline attribute flag
	DWORD fdwStrikeOut, // strikeout attribute flag
	DWORD fdwCharSet, // character set identifier
	DWORD fdwOutputPrecision, // output precision
	DWORD fdwClipPrecision, // clipping precision
	DWORD fdwQuality, // output quality
	DWORD fdwPitchAndFamily, // pitch and family
	LPCSTR lpszFace // pointer to typeface name string
	);
fnCreateFontA CreateFontAOLD;
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
	fdwCharSet = 0x86;
	fnWeight = FW_SEMIBOLD;
	return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
}

void init()
{
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)& CreateFontAOLD, CreateFontAEx);
	DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	DetourTransactionCommit();
}

/*************************************************************************************************************************************************************************
0041C360  |> \8BBC24 840000>mov edi,dword ptr ss:[esp+0x84]          ;  push edi
0041C367  |.  83C9 FF       or ecx,-0x1
0041C36A  |.  33C0          xor eax,eax
0041C36C  |.  8B96 30010000 mov edx,dword ptr ds:[esi+0x130]         ;  shell32.76012BB1
0041C372  |.  F2:AE         repne scas byte ptr es:[edi]
0041C374  |.  F7D1          not ecx
0041C376  |.  49            dec ecx
0041C377  |.  895424 44     mov dword ptr ss:[esp+0x44],edx
0041C37B  |.  8BC1          mov eax,ecx
0041C37D  |.  99            cdq
**************************************************************************************************************************************************************************/

static std::mutex mtx;
static PVOID JMPHookPoint;
static PVOID JMPHookPointNext;
static char* ret2;
char* __stdcall replaceText(char* src)
{

	__asm {
		pushad
		pushfd
	}
	//auto PushStr = wtocUTF(ctowJIS(src));
	char* dst;
	//cout << "【TEXT】	|" << wtocGBK(ctowJIS(src)) << endl;
	//TXT << PushStr << endl;
	cout << "【TEXT】	|" << src << endl;
	TXT << src << endl;
	/*
	if (strcmp(PushStr, "そのほんの少しの隙間から、どこからか小さな春色が舞い込んで――") == 0) {
		strcpy(src, "在那狭窄的间隙中，飘来了不知何处的小小春色――");
	}
	if (strcmp(PushStr, "　世界がそっと、ため息をつく――") == 0) {
		strcpy(src, "　世界悄然地，发出一声叹息——");
	}
	*/
	ret2 = strcpy(dst, src);

	__asm {
		popfd
		popad
	}
	return ret2;
}

__declspec(naked) void JMP_FUNC()
{
	_asm
	{
		mov edi,[esp + 0x84]
		pushad
			push edi
			call replaceText
		popad
		jmp JMPHookPointNext
	}
}
void JMPHelper(PVOID HookPoint, PVOID JMP_FUNC)
{
	DWORD oldProtect = 0;
	VirtualProtect(HookPoint, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	DWORD offset = (DWORD)JMP_FUNC - (DWORD)HookPoint - 5;
	char JMPOp[] = { 0xE9 };
	memcpy(HookPoint, &JMPOp, 1);
	memcpy((char*)HookPoint + 1, &offset, sizeof(DWORD));
	VirtualProtect(HookPoint, 5, oldProtect, &oldProtect);
}

void GetRawText()
{
	*(DWORD*)& JMPHookPoint = BaseAddr + 0x1C360;
	*(DWORD*)& JMPHookPointNext = BaseAddr + 0x1C367;
	cout << JMPHookPoint << " | " << JMPHookPointNext << "\n------------------------------\nSystemNNN DEBUG MODE VER 1.00\nCreate By SORATANE（空穜）\n------------------------------\n" << endl;
	JMPHelper(JMPHookPoint, JMP_FUNC);
}

static void make_console() 
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
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
		init();
		GetRawText();
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
