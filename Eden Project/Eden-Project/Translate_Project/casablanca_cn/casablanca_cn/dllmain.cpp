// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include <iostream>
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

static const char* Title= "【伊甸学院×葵海计划】卡萨布兰卡之蕾 - v1.0";

void __stdcall LumCreateTitleText(char* OldTitle)
{
	const char* copy = nullptr;
	copy = Title;
    memset(OldTitle, 0, strlen(copy) + 0x1);
    memcpy(OldTitle, copy, strlen(copy));
}

PVOID TitleHook = (PVOID)(BaseAddr + 0x567FB);
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

void __stdcall AymReplaceText(char* dst, const char* src, const char* src2)
{
	string s(src);
	string s2(src2);
	string d(dst);
	size_t pos = 0;

	pos = d.find(s);
	if (pos != string::npos)
	{
		d = d.replace(pos, s.size(), s2);
		strcpy(dst, d.c_str());
	}
}

void __stdcall LumChangeName(char* Name)
{
	//062AB638  3C 91 49 91 F0 8E 88 3E                          <慖戰巿>...


	AymReplaceText(Name, "乽", "「");
	AymReplaceText(Name, "<慖戰巿>", "<选择支>");
	AymReplaceText(Name, "亂儐儕亃", "【由莉】");
	AymReplaceText(Name, "亂僀僣僉亃", "   【一木】");
	AymReplaceText(Name, "亂彈巕妛惗俙亃", "【女学生Ａ】");
	AymReplaceText(Name, "亂丠丠丠亃", "【？？？】");
	AymReplaceText(Name, "亂僇僫亃", "【加奈】");
	AymReplaceText(Name, "亂儈僋亃", "【未来】");
	AymReplaceText(Name, "亂抝巕妛惗俙亃", "【男学生Ａ】");
	AymReplaceText(Name, "亂僀僣僉仌儐儕亃", "【一木＆由莉】");
	AymReplaceText(Name, "亂幁揷栮亃", "【鹿田爷爷】");
	AymReplaceText(Name, "亂屻攜彈巕俛亃", "【后辈女生Ｂ】");
	AymReplaceText(Name, "亂屻攜彈巕俠亃", "【后辈女生Ｃ】");
	AymReplaceText(Name, "亂抝巕妛惗俛亃", "【男学生Ｂ】");
	AymReplaceText(Name, "亂愭攜彈巕俙亃", "【后辈女生Ａ】");
	AymReplaceText(Name, "亂梡柋堳亃", "【杂用工】");
	AymReplaceText(Name, "亂彈巕妛惗俢亃", "【女学生Ｄ】");
	AymReplaceText(Name, "亂嫵巘亃", "【老师】");
	AymReplaceText(Name, "亂妛惗亃", "【学生】");
	AymReplaceText(Name, "亂屻攜彈巕俢亃", "【后辈女生Ｄ】");
	AymReplaceText(Name, "亂悢妛嫵巘亃", "【数学老师】");
	AymReplaceText(Name, "亂懱堢嫵巘亃", "【体育老师】");
	AymReplaceText(Name, "亂彈巕妛惗俤亃", "【女学生Ｅ】");
	AymReplaceText(Name, "亂梴岇嫵桜亃", "【保健老师】");
	AymReplaceText(Name, "亂彈巕妛惗俠亃", "【女学生Ｃ】");
	AymReplaceText(Name, "亂抝巕妛惗俠亃", "【男学生Ｃ】");
	AymReplaceText(Name, "亂嫵巘俙亃", "【老师Ａ】");
	AymReplaceText(Name, "亂彈巕妛惗俧亃", "【女学生Ｇ】");
	AymReplaceText(Name, "亂彈巕妛惗俫亃", "【女学生Ｈ】");
	AymReplaceText(Name, "亂捙柤曣亃", "【椎名母亲】");
	AymReplaceText(Name, "亂僀僣僉仌僇僫亃", "【一木＆加奈】");
	AymReplaceText(Name, "亂抝巕妛惗俧亃", "【男学生Ｇ】");
	AymReplaceText(Name, "亂屻攜彈巕俧亃", "【后辈女生Ｇ】");
	/*
	if (!strcmp(Name, "乽"))
		strcpy(Name, "「");
	else if (!strcmp(Name, "亂儐儕亃"))
		strcpy(Name, "【由莉】");
	else if (!strcmp(Name, "亂僀僣僉亃"))
		strcpy(Name, "【一木】");
	else if (!strcmp(Name, "亂彈巕妛惗俙亃"))
		strcpy(Name, "【女学生Ａ】");
	else if (!strcmp(Name, "亂丠丠丠亃"))
		strcpy(Name, "【？？？】");
	else if (!strcmp(Name, "亂僇僫亃"))
		strcpy(Name, "【加奈】");
	else if (!strcmp(Name, "亂儈僋亃"))
		strcpy(Name, "【未来】");
	else if (!strcmp(Name, "亂抝巕妛惗俙亃"))
		strcpy(Name, "【男学生Ａ】");
	else if (!strcmp(Name, "亂僀僣僉仌儐儕亃"))
		strcpy(Name, "【一木＆由莉】");
	else if (!strcmp(Name, "亂幁揷栮亃"))
		strcpy(Name, "【鹿田爷爷】");
	else if (!strcmp(Name, "亂屻攜彈巕俛亃"))
		strcpy(Name, "【后辈女生Ｂ】");
	else if (!strcmp(Name, "亂屻攜彈巕俠亃"))
		strcpy(Name, "【后辈女生Ｃ】");
	else if (!strcmp(Name, "亂抝巕妛惗俛亃"))
		strcpy(Name, "【男学生Ｂ】");
	else if (!strcmp(Name, "亂愭攜彈巕俙亃"))
		strcpy(Name, "【后辈女生Ａ】");
	else if (!strcmp(Name, "亂梡柋堳亃"))
		strcpy(Name, "【杂用工】");
	else if (!strcmp(Name, "亂彈巕妛惗俢亃"))
		strcpy(Name, "【女学生Ｄ】");
	else if (!strcmp(Name, "亂嫵巘亃"))
		strcpy(Name, "【老师】");
	else if (!strcmp(Name, "亂妛惗亃"))
		strcpy(Name, "【学生】");
	else if (!strcmp(Name, "亂屻攜彈巕俢亃"))
		strcpy(Name, "【后辈女生Ｄ】");
	else if (!strcmp(Name, "亂悢妛嫵巘亃"))
		strcpy(Name, "【数学老师】");
	else if (!strcmp(Name, "亂懱堢嫵巘亃"))
		strcpy(Name, "【体育老师】");
	else if (!strcmp(Name, "亂彈巕妛惗俤亃"))
		strcpy(Name, "【女学生Ｅ】");
	else if (!strcmp(Name, "亂梴岇嫵桜亃"))
		strcpy(Name, "【保健老师】");
	else if (!strcmp(Name, "亂彈巕妛惗俠亃"))
		strcpy(Name, "【女学生Ｃ】");
	else if (!strcmp(Name, "亂抝巕妛惗俠亃"))
		strcpy(Name, "【男学生Ｃ】");
	else if (!strcmp(Name, "亂嫵巘俙亃"))
		strcpy(Name, "【老师Ａ】");
	else if (!strcmp(Name, "亂彈巕妛惗俧亃"))
		strcpy(Name, "【女学生Ｇ】");
	else if (!strcmp(Name, "亂彈巕妛惗俫亃"))
		strcpy(Name, "【女学生Ｈ】");
	else if (!strcmp(Name, "亂捙柤曣亃"))
		strcpy(Name, "【椎名母亲】");
	else if (!strcmp(Name, "亂僀僣僉仌僇僫亃"))
		strcpy(Name, "【一木＆加奈】");
	else if (!strcmp(Name, "亂抝巕妛惗俧亃"))
		strcpy(Name, "【男学生Ｇ】");
	else if (!strcmp(Name, "亂屻攜彈巕俧亃"))
		strcpy(Name, "【后辈女生Ｇ】");
	*/
}

PVOID NameHook = (PVOID)(BaseAddr + 0x276CD);
__declspec(naked) void NameChange()
{
	_asm
	{
		pushad
		pushfd
		push edi
		call LumChangeName
		popfd
		popad
		jmp NameHook
	}
}

void TitleHookInit()
{
	DetourTransactionBegin();
	DetourAttach((void**)&TitleHook, TitleChange);
	//DetourAttach((void**)&NameHook, NameChange);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"TitleHookInit Error", 0, MB_OK);
}

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

void __fastcall APIHook()
{
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach((void**)&pCreateFontA, newCreateFontA);
	DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
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
		TitleHookInit();
		APIHook();
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