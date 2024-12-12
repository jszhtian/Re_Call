// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "detours.h"
#include "IATProc.h"
#include "TextStream.h"
#include <iostream>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
static ofstream TXT("LOG.txt");

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID pGetProcAddress = NULL;
typedef FARPROC(WINAPI* PGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
FARPROC WINAPI MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (!strcmp(lpProcName, "GetSystemWow64DirectoryA"))
		return NULL;
	return ((PGetProcAddress)pGetProcAddress)(hModule, lpProcName);
}

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);
int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	switch (cp)
	{
	case CP_ACP:
	case CP_OEMCP:
	case CP_THREAD_ACP:
		cp = 932;
		dwFg &= ~MB_ERR_INVALID_CHARS;
		break;

	default:
		break;
	}
	return((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

void APIHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	pGetProcAddress = DetourFindFunction("Kernel32.dll", "GetProcAddress");
	DetourAttach(&pGetProcAddress, MyGetProcAddress);
	DetourTransactionCommit();
	/*
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourTransactionCommit();
	*/
}
/*****************************************************************************************/

int WINAPI HookMultiByteToWideChar(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
)
{
	_asm pushad;
	_asm pushfd;
	//TXT << lpMultiByteStr << endl;
	switch (CodePage)
	{
	case CP_ACP:
	case CP_OEMCP:
	case CP_THREAD_ACP:
		CodePage = 932;
		dwFlags &= ~MB_ERR_INVALID_CHARS;
		break;
	default:
		break;
	}
	_asm popfd;
	_asm popad;
	return MultiByteToWideChar(CodePage, dwFlags,
		lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

void IATHook()
{
	HMODULE result = GetModuleHandleW(NULL);
	if (result == 0)
	{
		MessageBoxW(0, L"GetModuleHandleW returned error code。", L"IATHook Error", 0);
		return;
	}
	if (!IATPatch(result, "Kernel32.dll", GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "MultiByteToWideChar"), (PROC)HookMultiByteToWideChar))
	{
		MessageBoxW(0, L"MultiByteToWideChar Hook Error。", L"IATHook Error", 0);
		return;
	}
}
/// <summary>
/// inline test.
/// </summary>
bool IsMoji(const wstring& s)
{
	for (auto i: s)
	{
		if ((unsigned char)i < 0x20)
			return false;
	}
	return true;
}
PVOID ret_hook = NULL;
typedef int (WINAPI* PfuncTVPGetPlacedPath)(const ttstr&);

ttstr WINAPI HookTVPGetPlacedPath(const ttstr& name)
{
	//if(IsMoji(name.c_str()))
	//TXT << name << endl;
	return ((PfuncTVPGetPlacedPath)ret_hook)(name);
}


typedef iTVPFunctionExporter* (WINAPI* TVPGetFunctionExporterFunc)();
TVPGetFunctionExporterFunc pfTVPGetFunctionExporter = nullptr;
iTVPFunctionExporter* WINAPI HookTVPGetFunctionExporter()
{
	iTVPFunctionExporter* result = pfTVPGetFunctionExporter();
	TVPInitImportStub(result);
	static char funcname1[] = "ttstr ::TVPGetPlacedPath(const ttstr &)";
	PVOID pfTVPGetPlacedPath = TVPGetImportFuncPtr(funcname1);
	cout << "TVPGetPlacedPath:0x" << hex << pfTVPGetPlacedPath << endl;//0x00670FC4
	/*
	if (pfTVPGetPlacedPath)
	{
		ret_hook = pfTVPGetPlacedPath;
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach((PVOID*)&pfTVPGetPlacedPath, HookTVPGetPlacedPath);
		if (DetourTransactionCommit() != NOERROR)
			MessageBox(0, L"ERROR TO HOOK TVP INLINE FUNCTION", 0, 0);
	}
	*/
	return result;
}

void TJSHook()
{
	HMODULE hModule = GetModuleHandle(NULL);

	//Some corporation disabled this exported function.(Some krkrz-based game)
	*(FARPROC*)&pfTVPGetFunctionExporter = GetProcAddress(hModule, "TVPGetFunctionExporter");
	//cout << hex << pfTVPGetFunctionExporter << endl;
	if (pfTVPGetFunctionExporter != nullptr)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach((PVOID*)&pfTVPGetFunctionExporter, HookTVPGetFunctionExporter);
		DetourTransactionCommit();
	}
	else
	{
		MessageBox(0, L"Hook TJS error", 0, 0);
	}
	//LoadLibraryW(L"NtLayer.dll");
}

static void make_console()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Debug mode\n" << endl;
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
		APIHook();
		IATHook();
		//TJSHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

