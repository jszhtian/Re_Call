// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
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
	const char* szWndName = "【守夜人汉化组】猫咪猫咪猫｜简体中文化补丁V1.0";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void __stdcall AymGetText(char* Text)
{
	cout << wtocGBK(ctowJIS(Text)) << endl;
}

PVOID pGetText = NULL;
__declspec(naked)void GetText()
{
	__asm
	{
		pushad
		pushfd
		push dword ptr ds : [ecx + 0x8]
		call AymGetText
		popfd
		popad
		jmp pGetText
	}
}
bool IsSuccess = false;
void __stdcall AymGetDllAddr(char* name, DWORD addr)
{
	if (!strcmp(name, "resident.dll") && !IsSuccess)
	{
		pGetText = (PVOID)(addr + 0x236bba);
		cout << hex << pGetText << endl;
		DetourTransactionBegin();
		DetourAttach((void**)&pGetText, GetText);
		if (DetourTransactionCommit() != NO_ERROR)
		{
			MessageBox(0, L"Dll hook error", L"AymError", 0);
			return;
		}
		IsSuccess = true;
	}
}

PVOID pGetDllAddr = NULL;
__declspec(naked)void GetDllAddr()
{
	__asm
	{
		pushad
		pushfd
		push eax
		push edi
		call AymGetDllAddr
		popfd
		popad
		jmp pGetDllAddr
	}
}

void InitHook()
{
	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBox(0, L"API hook error", L"AymError", 0);
		return ;
	}
	//DLL hook
	pGetDllAddr = (PVOID)(BaseAddr + 0x4c8ab);
	cout << hex << pGetDllAddr << endl;
	DetourTransactionBegin();
	DetourAttach((void**)&pGetDllAddr, GetDllAddr);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBox(0, L"start Dll hook error", L"AymError", 0);
		return;
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
		make_console();
		InitHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

