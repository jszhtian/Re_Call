// dllmain.cpp : 定义 DLL 应用程序的入口点。
//For ティンクル☆くるせいだーす -Passion Star Stream-
#include "framework.h"
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
//ofstream TXT("Text.txt");

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
fnCreateFontA CreateFontAOLD;
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
	wchar_t* wcs = nullptr;
	fdwCharSet = GB2312_CHARSET;
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lpszFace, strlen(lpszFace));
	wcs = ctow936(buffer);
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, wcs);
}

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI *pfuncCreateWindowExA)(
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
	const char* szWndName = "闪耀十字军 ｜ 简体中文化补丁V1.0";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI *PfuncCreateFontIndirectA)(LOGFONTA *lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA *lplf)
{
	lplf->lfCharSet = ANSI_CHARSET;
	strcpy(lplf->lfFaceName, "EdenFont");

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
/*
signed int(*OldStr)(int String, int a2, LPCSTR lpString, int a4, int a5);
signed int __cdecl MyStr(int String, int a2, LPCSTR lpString, int a4, int a5)
{
	_asm
	{
		pushad
	}
	CRC32 crc;
	int ret;
	int index;
	char* Font = (char*)String;
	strcpy(Font, "黑体");
	ret = OldStr(String, a2, lpString, a4, a5);
	DWORD strcrc = crc.Calc((char*)lpString, ret);
	index = *(int*)ret;
	char* Pstr = (char*)lpString;
	//cout << "0x" << hex << setw(8) << setfill('0') << strcrc << "|" << ctow936(Pstr) << endl;
	//TXT << "0x" << hex << setw(8) << setfill('0') << strcrc << "|" << ctow936(Pstr) << endl;
	cout << wtocGBK(ctowJIS(Pstr)) <<endl;
	TXT << wtocGBK(ctowJIS(Pstr)) <<endl;
	
	if (strcmp(wtocGBK(ctowJIS(Pstr)),"あなたの誕生日を入力してください")==0)
	{
		MessageBox(0, L"汉化文本加载成功", L"提示", MB_OK);
		ret = OldStr(String, a2, "请输入你的生日。", a4+0x10, a5);
	}
	if (strcmp(wtocGBK(ctowJIS(Pstr)), "誕生日を７月２７日に設定しますか？") == 0)
	{
		MessageBox(0, L"汉化文本加载成功", L"提示", MB_OK);
		ret = OldStr(String, a2, "这样可以吗（游戏中不能修改）", a4+0x10, a5);
	}
	//char* tmpStr = wtocGBK(ctowJIS((char*)a4+0x10));
	//if (strcmp(tmpStr,"")==0)
	//{
		//lstrcatA((char*)a4 + 0x10, "中文测试123");
	//}
	/*
	auto scitr = REPList.find(strcrc);
	if (scitr != REPList.end())
	{
		wcscpy((wchar_t*)lpString, scitr->second.c_str());
		ret = scitr->second.length();
		cout << "REPLACE :" << "0x" << hex << scitr->first << wtocGBK(scitr->second.c_str()) << endl;
	}
	
	_asm
	{
		popad
	}
	return ret;
}
*/

DWORD* (*PJADVEnumFont)(int a1, int a2, int a3, HDC hdc, int a5, int FontSig, int a7);
DWORD* __cdecl EdenEnumFont(int a1, int a2, int a3, HDC hdc, int a5, int FontSig, int a7)
{
	if (FontSig == 5)
		FontSig = 1;
	cout << FontSig << endl;
	return PJADVEnumFont(a1, a2, a3, hdc, a5, FontSig, a7);
}

void Hook()
{
	DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
	cout << "BaseAddress:0x" <<hex<< BaseAddr << endl;
	*(DWORD*)& PJADVEnumFont = BaseAddr + 0x663E0;
	cout << "HookAddress:0x" << hex << (BaseAddr + 0x663E0) <<"\n"<< endl;
	DetourTransactionBegin();
	DetourAttach((void**)& PJADVEnumFont, EdenEnumFont);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	if(DetourTransactionCommit()!=NOERROR){ MessageBox(NULL, L"ERROR", L"Crescendo", MB_OK); }
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

