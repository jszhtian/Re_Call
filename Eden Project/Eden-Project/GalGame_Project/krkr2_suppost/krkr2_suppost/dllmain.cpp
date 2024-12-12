// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <iostream>
#include <Psapi.h>
#include "IATProc.h"
//#include "TextStream.h"
#include "tp_stub.h"
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "detours.lib")

using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID pOldLoadLibaryA = NULL;
typedef HMODULE(WINAPI* PLoadLibaryA)(LPCSTR lpFileName);
HMODULE WINAPI MyLoadLibaryA(LPCSTR lpFileName)
{
	return ((PLoadLibaryA)pOldLoadLibaryA)(lpFileName);
}


PVOID pGetProcAddress = NULL;
typedef FARPROC(WINAPI* PGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
FARPROC WINAPI MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (!strcmp(lpProcName, "GetSystemWow64DirectoryA"))
	{
		return NULL;
	}
	return ((PGetProcAddress)pGetProcAddress)(hModule, lpProcName);
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
	pOldLoadLibaryA = DetourFindFunction("Kernel32.dll", "LoadLibaryA");
	DetourAttach(&pOldLoadLibaryA, MyLoadLibaryA);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	pGetProcAddress = DetourFindFunction("Kernel32.dll", "GetProcAddress");
	DetourAttach(&pGetProcAddress, MyGetProcAddress);
	DetourTransactionCommit();
}

typedef iTVPFunctionExporter* (WINAPI* TVPGetFunctionExporterFunc)();
TVPGetFunctionExporterFunc pfTVPGetFunctionExporter = nullptr;
iTVPFunctionExporter* WINAPI HookTVPGetFunctionExporter()
{
	iTVPFunctionExporter* result = pfTVPGetFunctionExporter();
	TVPInitImportStub(result);
	static char funcname1[] = "IStream * ::TVPCreateIStream(const ttstr &,tjs_uint32)";
	PVOID pfTVPGetPlacedPath = TVPGetImportFuncPtr(funcname1);
	cout << "TVPCreateIStream:0x" << hex << pfTVPGetPlacedPath << endl;//0x00670FC4
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

	*(FARPROC*)&pfTVPGetFunctionExporter = GetProcAddress(hModule, "TVPGetFunctionExporter");
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
		TJSHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

