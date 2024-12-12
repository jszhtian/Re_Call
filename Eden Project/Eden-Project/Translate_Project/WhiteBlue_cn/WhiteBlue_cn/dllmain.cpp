// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

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
	const char* szWndName = "『WhiteBlue』汉化v1.0【脸肿汉化组】| 汉化禁止用于任何平台直播和录播上传分享";

	HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	//SetWindowDisplayAffinity(hw, WDA_MONITOR);
	return hw;
}

wchar_t szTitle[] = L"提示";
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
	//cout << lpText << endl;
	//TXT << Pstr << endl;
	if (!strcmp(Pstr, "終了してよろしいですか？"))
	{
		strcpy(Pstr, "即将结束游戏，确认吗？");
	}
	else if (!strcmp(Pstr, "タイトルに戻ります。よろしいですか？"))
	{
		strcpy(Pstr, "即将返回标题，确认吗？");
	}
	return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}

void Init()
{
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	//DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"API HOOK ERROR", L"AyamiKaze", MB_OK);
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
		Init();
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
