// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "detours.h"
#include <iostream>
#include <mutex>
#pragma comment(lib, "detours.lib")
using namespace std;

typedef HFONT(WINAPI *fnCreateFontA)(
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
fnCreateFontA pCreateFontA;

PVOID g_pOldCreateWindowExW = CreateWindowExW;
typedef HWND(WINAPI* pfuncCreateWindowExW)(
	DWORD dwExStyle,
	LPCWSTR lpClassName,
	LPCWSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam);

PVOID g_pOldTextOutA = TextOutA;
typedef HWND(WINAPI* pfuncTextOutA)(
	HDC    hdc,
	int    x,
	int    y,
	LPCSTR lpString,
	int    c
);

typedef int(__fastcall* fnDrawText)(void* obj, DWORD edxDummy, int x, int y, char* str, int unk5, COLORREF col, int unk7);
fnDrawText pDrawText;
static wchar_t WStrBuf[256];
static char StrBuf[256];
wchar_t* ctowJIS(char* str)
{
	memset(WStrBuf, 0, sizeof(wchar_t) * 256);
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	MultiByteToWideChar(932, 0, str, -1, WStrBuf, dwMinSize);//转换
	return WStrBuf;
}

char* wtocGBK(LPCWSTR str)
{
	memset(StrBuf, 0, sizeof(char) * 256);
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	WideCharToMultiByte(936, NULL, str, -1, StrBuf, dwMinSize, NULL, FALSE);//转换
	return StrBuf;
}
static int ret1;
int __fastcall NewDrawText(void* obj, DWORD edx, int x, int y, char* str, int unk5, COLORREF col, int unk7)
{
	__asm {
		pushad
		pushfd
	}
	auto TmpWstr = ctowJIS(str);
	auto TmpStr = wtocGBK(TmpWstr);
	char teast[] = "喝一杯咖啡";
	//std::cout << "TEXT:" << TmpStr << std::endl;
	if (strlen(str)>2)
	{
		std::cout << "select:" << TmpStr << std::endl;
		//strcpy(str, teast);
	}
	if (wcscmp(TmpWstr, L"コーヒーを選ぶ") == 0)
	{
		//strcpy(str, teast);
	}

	ret1 = (fnDrawText(pDrawText))(obj, edx, x, y, str, unk5, col, unk7);
	__asm {
		popfd
		popad
	}
	return ret1;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Open Console Success!" << endl;
}


static std::mutex mtx;
static PVOID JMPHookPoint;
static PVOID JMPHookPointNext;
static char* ret2;
char* __stdcall replaceText(char* dst, char* src)
{

	__asm {
		pushad
		pushfd
	}
	auto TmpWstr = ctowJIS(src);
	auto TmpStr = wtocGBK(TmpWstr);
	char teast[] = "这是中文测试123";
	std::cout << "text:" << TmpStr << std::endl;
	//ret2 = strcpy(dst, src);
	//strcat(dst, teast);
	__asm {
		popfd
		popad
	}
	return ret2;
}
__declspec(naked) void JmpHelper()
{
	_asm
	{
		call replaceText
		jmp JMPHookPointNext
	}
}
HWND WINAPI NewCreateWindowExW(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	
	if (wcscmp(lpWindowName, L"Clover Heart's[幪偰偒傟側偄婓朷偺壠楬]") == 0)
	{
		const wchar_t szWndName[] = L"Clover Heart's[被舍弃的过去]";
		return ((pfuncCreateWindowExW)g_pOldCreateWindowExW)(dwExStyle, lpClassName, szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}
}



HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
	fdwCharSet = SHIFTJIS_CHARSET;
	return ((fnCreateFontA)pCreateFontA)(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "宋体");
}

HWND WINAPI NewTextOutA(HDC hdc,int x,int y,LPCSTR lpString,int c)
{
	if (LPCSTR text1 = ("不意に押し寄せた胸の痛みを忘れるように見上げた夕空。"))
	{
		LPCSTR text = ("TESTt");
		return ((pfuncTextOutA)g_pOldTextOutA)(hdc, x, y, text, c);
	}
	else
	{
		return ((pfuncTextOutA)g_pOldTextOutA)(hdc, x, y, lpString, c);
	}
}



void setRawJMPOpcode(PVOID HookPos, PVOID JMPProc)
{
	DWORD oldProtect = 0;
	VirtualProtect(HookPos, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	DWORD offset = (DWORD)JMPProc - (DWORD)HookPos - 5;
	char JMPOp[] = { 0xE9 };
	memcpy(HookPos, &JMPOp, 1);
	memcpy((char*)HookPos + 1, &offset, sizeof(DWORD));
	VirtualProtect(HookPos, 5, oldProtect, &oldProtect);
}

void BeginDetour()
{
	setRawJMPOpcode(JMPHookPoint, &JmpHelper);
	DetourTransactionBegin();
	DetourAttach(&(PVOID&)pCreateFontA, CreateFontAEx);
	DetourAttach(&(PVOID&)pDrawText, NewDrawText);
	//DetourAttach(&g_pOldCreateWindowExW, NewCreateWindowExW);
	//DetourAttach(&g_pOldTextOutA, NewTextOutA);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Hook目标函数失败", L"严重错误", MB_OK | MB_ICONWARNING);
	}
}


void initGlobalData()
{
	DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
	*(DWORD*)&JMPHookPoint = BaseAddr + 0x4D515;
	*(DWORD*)&JMPHookPointNext = BaseAddr + 0x4D51B;
	pDrawText = (fnDrawText)((PVOID)(BaseAddr + 0x163B1));
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandleW(L"gdi32.dll"), "CreateFontA");
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		initGlobalData();
		make_console();
		BeginDetour();
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

