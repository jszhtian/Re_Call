// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <mutex>
#pragma comment(lib, "detours.lib")
using namespace std;
ofstream TXT("Text.txt");
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

wchar_t* ctow(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(932, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(932, 0, (const char*)str, int(nu), buffer, int(n));
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
	wchar_t* wcs = nullptr;
	fdwCharSet = GB2312_CHARSET;
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lpszFace, strlen(lpszFace));
	wcs = ctow936(buffer);
	//return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, wcs);
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, ctow("黑体"));
}

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
	DWORD dwExStyle,
	LPCTSTR lpClassName,
	LPCTSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam);
HWND WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	const char* szWndName = "琉璃的都合 ｜ 简体中文化补丁V1.0";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	strcpy(lplf->lfFaceName, "仿宋");
	//lplf->lfWeight = 48;

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

typedef int (WINAPI* fnEnumFontFamiliesExA)(
	HDC           hdc,
	LPLOGFONTA    lpLogfont,
	FONTENUMPROCA lpProc,
	LPARAM        lParam,
	DWORD         dwFlags
	);
fnEnumFontFamiliesExA pEnumFontFamiliesExA;
int WINAPI newEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	//lpLogfont->lfCharSet = 936;
	LOGFONTA Lfont;
	memcpy(&Lfont, lpLogfont, sizeof(LOGFONTA));
	Lfont.lfCharSet = GB2312_CHARSET;
	memcpy(lpLogfont, &Lfont, sizeof(LOGFONTA));
	return ((fnEnumFontFamiliesExA)pEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

void Hook()
{
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
	DetourTransactionBegin();
	DetourAttach((void**)& CreateFontAOLD, CreateFontAEx);
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourAttach((void**)& pEnumFontFamiliesExA, newEnumFontFamiliesExA);
	if (DetourTransactionCommit() != NOERROR) 
		MessageBox(NULL, L"ERROR", L"Crescendo", MB_OK);
}


static void _stdcall EdenTextReplace(char* Text)
{
	int size = strlen(Text);
	if (size != 0)
	{
		bool retn = true;
		for (int i = 0 ; i < size ; i++)
		{
			if ((Text[i] > 0xFFFFFF80 && Text[i] < 0xFFFFFFA0) || (Text[i] > 0xFFFFFFE0 and Text[i] < 0xFFFFFFFC))
			{
				retn = false;
				break;
			}
			i++;
		}
		//MessageBox(0, L"Find Text.", 0, 0);
		if (retn == false)
		{
			cout << wtocGBK(ctowJIS(Text)) << endl;
			TXT << Text << endl;
			char* ret = wtocGBK(ctowJIS(Text));
			strcpy(Text, ret);
		}
		else
		{
			return;
		}
	}
	return;
}

static void _stdcall MajiroSystemFunctionName(char* Text)
{
	int size = strlen(Text);
	if (size != 0)
	{
		bool retn = true;
		for (int i = 0; i < size; i++)
		{
			if ((Text[i] > 0xFFFFFF80 && Text[i] < 0xFFFFFFA0) || (Text[i] > 0xFFFFFFE0 and Text[i] < 0xFFFFFFFC))
			{
				retn = false;
				break;
			}
			i++;
		}
		//MessageBox(0, L"Find Text.", 0, 0);
		if (retn == false)
		{
			cout << wtocGBK(ctowJIS(Text)) << endl;
			TXT << Text << endl;
		}
		else
		{
			return;
		}
	}
	return;
}

PVOID TextHookAddress = NULL;
__declspec(naked) void TextHook()
{
	_asm
	{
		pushad
			pushfd
				push ecx
				call EdenTextReplace
			popfd
		popad
		jmp TextHookAddress
	}
}
PVOID pGetMajiroSystemFunctionName = NULL;
__declspec(naked) void GetMajiroSystemFunctionName()
{
	_asm
	{
		pushad
			pushfd
				push eax
				call MajiroSystemFunctionName
			popfd
		popad
		jmp pGetMajiroSystemFunctionName
	}
}

unsigned __int8* (*mEdenTextHook)(char* a1, char* a2);
unsigned __int8* __cdecl EdenTextHook(char* a1, char* a2)
{

	int size = strlen(a1);
	if (size != 0)
	{
		bool retn = true;
		for (int i = 0; i < size; i++)
		{
			if ((a1[i] > 0xFFFFFF80 && a1[i] < 0xFFFFFFA0) || (a1[i] > 0xFFFFFFE0 and a1[i] < 0xFFFFFFFC))
			{
				retn = false;
				break;
			}
			i++;
		}
		//MessageBox(0, L"Find Text.", 0, 0);
		if (retn == false)
		{
			cout << wtocGBK(ctowJIS(a1)) << endl;
			TXT << a1 << endl;
			char* ret = wtocGBK(ctowJIS(a1));
			if(strcmp(ret,"どこまでも続く真っ直ぐな海岸線。")==0)
				return mEdenTextHook(ret, a2);
			else
				return mEdenTextHook(a1, a2);
		}
		else
		{
			return mEdenTextHook(a1,a2);
		}
	}
	return mEdenTextHook(a1, a2);
}



void Hook2()
{
	TextHookAddress = (PVOID)(BaseAddr + 0x42787);
	pGetMajiroSystemFunctionName = (PVOID)(BaseAddr + 0x3B5B8);
	cout << hex << BaseAddr << endl;
	cout << hex << TextHookAddress << endl;
	cout << hex << pGetMajiroSystemFunctionName << endl;
	DetourTransactionBegin();
	DetourAttach((void**)& TextHookAddress, TextHook);
	DetourAttach((void**)& pGetMajiroSystemFunctionName, GetMajiroSystemFunctionName);
	DetourTransactionCommit();
	/*
	* (DWORD*)& mEdenTextHook = BaseAddr + 0x7C7A0;
	cout << hex << BaseAddr << endl;
	cout << hex << mEdenTextHook << endl;
	DetourTransactionBegin();
	DetourAttach((void**)& mEdenTextHook, EdenTextHook);
	DetourTransactionCommit();
		*/
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
		Hook2();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

