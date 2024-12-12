// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
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
	fdwCharSet = GB2312_CHARSET;
	return CreateFontA(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
}

typedef bool (WINAPI* fnSetWindowTextA)(HWND hWnd, LPCSTR lpString);
fnSetWindowTextA pSetWindowTextA;
bool WINAPI newSetWindowTextA(HWND hw, LPCSTR lps)
{
	wchar_t newtitle[] = L"【脸肿汉化组】轮舞曲 | 简体中文化补丁V1.0";
	return SetWindowTextW(hw, newtitle);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	strcpy(lplf->lfFaceName, "黑体");
	//lplf->lfWeight = 48;

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

DWORD OldSize, OldBuff = NULL;
bool suc = false;
bool suc2 = false;

void _stdcall AymReadFile(DWORD Buff,DWORD Size)
{
	if (!suc) 
	{
		FILE* f = fopen("START.AX", "rb");
		if (!f)
		{
			MessageBox(0, L"Can't find START.AX file.", L"AymReadFile", 0);
			return;
		}
		fseek(f, 0, SEEK_END);
		DWORD size = ftell(f);
		fseek(f, 0, SEEK_SET);
		char* buff = (char*)malloc(size);
		if (buff != NULL && size != 0) 
		{
			fread(buff, size, 1, f);
			fclose(f);
			memcpy((void*)Buff, buff, size);
			free(buff);
			size = NULL;
			suc = true;
		}
	}
	return;
}

PVOID pGetOldSize = (PVOID)(BaseAddr + 0x2709C);
PVOID pGetOldBuff = (PVOID)(BaseAddr + 0x270B4);
PVOID pCopyMyFile = (PVOID)(BaseAddr + 0x270C0);
__declspec(naked)void GetOldSize()
{
	__asm
	{
		add eax,0x114524
		jmp pGetOldSize
	}
}

__declspec(naked)void GetOldBuff()
{
	__asm
	{
		pushad
			pushfd
				mov OldBuff,eax
			popfd
		popad
		jmp pGetOldBuff
	}
}

__declspec(naked)void CopyMyFile()
{
	__asm
	{
		pushad
			pushfd
				push OldSize
				push OldBuff
				call AymReadFile
			popfd
		popad
		jmp pCopyMyFile
	}
}

void InitHook()
{
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pSetWindowTextA = (fnSetWindowTextA)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextA");
	DetourTransactionBegin();
	DetourAttach((void**)&CreateFontAOLD, CreateFontAEx);
	DetourAttach((void**)&pSetWindowTextA, newSetWindowTextA);
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach((void**)&pGetOldSize, GetOldSize);
	DetourAttach((void**)&pGetOldBuff, GetOldBuff);
	DetourAttach((void**)&pCopyMyFile, CopyMyFile);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Can't Hook", L"InitHook", MB_OK | MB_ICONWARNING);
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
		InitHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

