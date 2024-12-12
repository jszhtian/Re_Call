// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

#define 少女グラフィティ
#define FREE
#define Encrypt

#ifdef Kiss
const char* Title = "Kiss&Crisis";
const char* Title2 = "Kiss&Crisis【胖次汉化组】gkd.galkey.cc";
PVOID TitleHook = (PVOID)(BaseAddr + 0x56803);
PVOID FileSystemSet = (PVOID)(BaseAddr + 0x68FE9);
#endif

#ifdef ゴールデンアワー
const char* Title = "Golden hour";
const char* Title2 = "Golden hour【胖次汉化组】gkd.galkey.cc";
PVOID TitleHook = (PVOID)(BaseAddr + 0x567FB);
PVOID FileSystemSet = (PVOID)(BaseAddr + 0x68FE1);
#endif

#ifdef スワローテイル
const char* Title = "Swallowtail -那一天超越青空";
const char* Title2 = "Swallowtail -那一天超越青空【胖次汉化组】gkd.galkey.cc";
PVOID TitleHook = (PVOID)(BaseAddr + 0x56833);
PVOID FileSystemSet = (PVOID)(BaseAddr + 0x69061);
#endif

#ifdef ペトリコール
const char* Title = "雨后泥土的气息 -Petrichor-";
const char* Title2 = "雨后泥土的气息 -Petrichor-【胖次汉化组】gkd.galkey.cc";
PVOID TitleHook = (PVOID)(BaseAddr + 0x56833);
PVOID FileSystemSet = (PVOID)(BaseAddr + 0x69061);
#endif

#ifdef 少女グラフィティ
const char* Title = "少女涂鸦";
const char* Title2 = "少女涂鸦【胖次汉化组】gkd.galkey.cc";
PVOID TitleHook = (PVOID)(BaseAddr + 0x56803);
PVOID FileSystemSet = (PVOID)(BaseAddr + 0x68FE9);
#endif
void __stdcall LumCreateTitleText(char* OldTitle)
{
	const char* copy = nullptr;
#ifdef FREE
	copy = Title2;
#else
	copy = Title;
#endif
    memset(OldTitle, 0, strlen(copy) + 0x1);
    memcpy(OldTitle, copy, strlen(copy));
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
#ifdef FREE
	HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)Title2, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
#else
	HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)Title, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
#endif
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

#define WorkPath ".\\_WorkPath\\SCN\\"
#include <algorithm> 
#include "md5.h"
VOID WINAPI GenMD5Code(const char* FileName, string& OutHex)
{
	BYTE OutBuffer[16] = { 0 };

	MD5Context Context;
	MD5Init(&Context);
	MD5Update(&Context, (BYTE*)FileName, lstrlenA(FileName) * 2);
	MD5Final(OutBuffer, &Context);

	char OutTemp[3] = { 0 };

	for (ULONG32 i = 0; i < 16; i++)
	{
		sprintf(OutTemp, "%d", (OutBuffer[i] & 0xF0) >> 4);
		OutHex += OutTemp;
		RtlZeroMemory(OutTemp, 0, sizeof(char) * 3);
		sprintf(OutTemp, "%d", OutBuffer[i] & 0x0F);
		OutHex += OutTemp;
		RtlZeroMemory(OutTemp, 0, sizeof(char) * 3);
	}
}

void FileNameToLower(std::string& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

BOOL IsFileEx(string FileName)
{
	if (GetFileAttributesA(FileName.c_str()) == INVALID_FILE_ATTRIBUTES)
		return FALSE;
	return TRUE;
}

void __stdcall LumFileSystem(char* FileName)
{
	string fn(FileName);
	fn = fn.substr(fn.find_last_of("\\") + 1);
	FileNameToLower(fn);

	char* Cfn = (char*)fn.c_str();
	for (int i = 0; i < strlen(Cfn); i++)
	{
		Cfn[i] += 0x1;
	}
	string hash_out;
	string hash_out2;
	GenMD5Code(Cfn, hash_out);
	GenMD5Code(fn.c_str(), hash_out2);
	fn = hash_out + hash_out2 + ".scn";

	string Path = WorkPath + fn;
	if (IsFileEx(Path)) 
	{
		memset(FileName, 0, strlen(FileName));
		strcpy(FileName, Path.c_str());
	}
	cout << FileName << endl;
}

/*
__declspec(naked) void FileChange()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call LumFileSystem
		popfd
		popad
		jmp FileSystemSet
	}
}
*/
__declspec(naked) void FileChange()
{
	_asm
	{
		mov eax,0x94114514
		jmp FileSystemSet
	}
}

void __fastcall FileSystem()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)&FileSystemSet, FileChange);
	DetourTransactionCommit();
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
#ifdef Encrypt
		FileSystem();
#endif
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI PanciLocaleTranslateGroup()
{
}