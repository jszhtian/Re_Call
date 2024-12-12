// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

static const char* Title = "Lkyt 1.10\x00";
/*
PVOID TitleHook = (PVOID)(BaseAddr + 0x47C3F);
static const char* Title= "【T.S.WORKS】双叶☆频道4.5 - v1.0\x00";

void __stdcall LumCreateTitleText(char* OldTitle)
{
    memset(OldTitle, 0, strlen(OldTitle));
    memcpy(OldTitle, Title, 35);
}

__declspec(naked) void TitleChange()
{
    _asm
    {
        pushad
        pushfd
        push eax
        call LumCreateTitleText
        popfd
        popad
        jmp TitleHook
    }
}

void TitleHookInit()
{
	DetourTransactionBegin();
	DetourAttach((void**)&TitleHook, TitleChange);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"TitleHookInit Error", 0, MB_OK);
}
*/
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

wchar_t szTitle[] = L"确认";
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	if (strcmp(Pstr, "ゲームを終了します。よろしいですか？") == 0)
	{
		strcpy(Pstr, "即将结束游戏。确认吗？");

	}
	return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}


typedef HFONT(WINAPI* fnCreateFontA)(_In_ int     nHeight,
	_In_ int     nWidth,
	_In_ int     nEscapement,
	_In_ int     nOrientation,
	_In_ int     fnWeight,
	_In_ DWORD   fdwItalic,
	_In_ DWORD   fdwUnderline,
	_In_ DWORD   fdwStrikeOut,
	_In_ DWORD   fdwCharSet,
	_In_ DWORD   fdwOutputPrecision,
	_In_ DWORD   fdwClipPrecision,
	_In_ DWORD   fdwQuality,
	_In_ DWORD   fdwPitchAndFamily,
	_In_ LPCTSTR lpszFace);
fnCreateFontA pCreateFontA;

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	//wchar_t* wcs = nullptr;
	lplf->lfCharSet = GB2312_CHARSET;
	strcpy(lplf->lfFaceName, "黑体");
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

HFONT WINAPI newCreateFontA(_In_ int     nHeight,
	_In_ int     nWidth,
	_In_ int     nEscapement,
	_In_ int     nOrientation,
	_In_ int     fnWeight,
	_In_ DWORD   fdwItalic,
	_In_ DWORD   fdwUnderline,
	_In_ DWORD   fdwStrikeOut,
	_In_ DWORD   fdwCharSet,
	_In_ DWORD   fdwOutputPrecision,
	_In_ DWORD   fdwClipPrecision,
	_In_ DWORD   fdwQuality,
	_In_ DWORD   fdwPitchAndFamily,
	_In_ LPCSTR lpszFace)
{
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, GB2312_CHARSET, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, L"黑体");
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
	HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)Title, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	return hw;
}


PVOID g_pOldTextOutA = TextOutA;
typedef int(WINAPI* PfuncTextOutA)(HDC hdc, int x, int y, LPCSTR lpString, int c);
int WINAPI HookTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
	if (!lstrcmpA(lpString, "☆"))
	{
		LOGFONTW* lplf = (LOGFONTW*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(LOGFONTW));

		HFONT hOldFont = (HFONT)GetCurrentObject(hdc, OBJ_FONT);
		GetObject(hOldFont, sizeof(LOGFONTW), lplf);
		LOGFONTW Info = { 0 };

		Info.lfHeight = lplf->lfHeight;
		Info.lfWidth = lplf->lfWidth;
		Info.lfEscapement = lplf->lfEscapement;
		Info.lfOrientation = lplf->lfOrientation;
		Info.lfWeight = lplf->lfWeight;
		Info.lfItalic = lplf->lfItalic;
		Info.lfUnderline = lplf->lfUnderline;
		Info.lfStrikeOut = lplf->lfStrikeOut;
		Info.lfOutPrecision = lplf->lfOutPrecision;
		Info.lfClipPrecision = lplf->lfClipPrecision;
		Info.lfQuality = lplf->lfQuality;
		Info.lfPitchAndFamily = lplf->lfPitchAndFamily;
		lstrcpyW(Info.lfFaceName, L"MS Gothic");
		lplf->lfCharSet = SHIFTJIS_CHARSET;

		HFONT hFont = CreateFontIndirectW(&Info);

		hOldFont = (HFONT)SelectObject(hdc, hFont);
		//6A 26 
		int Result = TextOutW(hdc, x, y, L"♪", c);

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
		HeapFree(GetProcessHeap(), 0, lplf);

		return Result;
	}
	return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, lpString, c);
}

void __fastcall APIHook()
{
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach((void**)&pCreateFontA, newCreateFontA);
	DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourAttach(&g_pOldTextOutA, HookTextOutA);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"APIHook Error", 0, MB_OK);
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
		//TitleHookInit();
		APIHook();
		//FileSystem();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI AyamiKaze()
{
}