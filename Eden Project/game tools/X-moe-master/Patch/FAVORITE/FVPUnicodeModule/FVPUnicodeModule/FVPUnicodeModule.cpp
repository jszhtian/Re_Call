#include "FVPUnicodeModule.h"

BOOL StartHook(LPCSTR szDllName, PROC pfnOrg, PROC pfnNew)
{
	HMODULE hmod;
	LPCSTR szLibName;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk;
	DWORD dwOldProtect, dwRVA;
	PBYTE pAddr;

	hmod = GetModuleHandleW(NULL);
	pAddr = (PBYTE)hmod;
	pAddr += *((DWORD*)&pAddr[0x3C]);
	dwRVA = *((DWORD*)&pAddr[0x80]);

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hmod + dwRVA);

	for (; pImportDesc->Name; pImportDesc++)
	{
		szLibName = (LPCSTR)((DWORD)hmod + pImportDesc->Name);
		if (!stricmp(szLibName, szDllName))
		{
			pThunk = (PIMAGE_THUNK_DATA)((DWORD)hmod + pImportDesc->FirstThunk);

			for (; pThunk->u1.Function; pThunk++)
			{
				if (pThunk->u1.Function == (DWORD)pfnOrg)
				{
					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
					pThunk->u1.Function = (DWORD)pfnNew;
					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, dwOldProtect, &dwOldProtect);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


int WINAPI HooklStrcmpiA(LPCSTR lpString1, LPCSTR lpString2)
{
	int ret = CompareStringA(0x411, 1, lpString1, -1, lpString2, -1);
	return ret - 2;
}

HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	RECT    rcWordArea;
	ULONG   Length;
	LPWSTR  ClassName, WindowName;


	Length = lstrlenA(lpClassName) + 1;
	ClassName = (LPWSTR)alloca(Length * sizeof(WCHAR));
	RtlZeroMemory(ClassName, Length * sizeof(WCHAR));
	MultiByteToWideChar(932, 0, lpClassName, Length, ClassName, Length * sizeof(WCHAR));

	Length = lstrlenA(lpWindowName) + 1;
	WindowName = (LPWSTR)alloca(Length * sizeof(WCHAR));
	RtlZeroMemory(WindowName, Length * sizeof(WCHAR));
	MultiByteToWideChar(932, 0, lpWindowName, Length, WindowName, Length * sizeof(WCHAR));

	return CreateWindowExW(dwExStyle, ClassName, WindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

int WINAPI HookMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	WCHAR* uTitle = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 1024 * 2);
	WCHAR* uInfo = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 1024 * 2);
	MultiByteToWideChar(932, 0, lpText, -1, uInfo, 1024);
	MultiByteToWideChar(932, 0, lpCaption, -1, uTitle, 1024);
	int result = MessageBoxW(hWnd, uInfo, uTitle, uType);
	HeapFree(GetProcessHeap(), 0, uTitle);
	HeapFree(GetProcessHeap(), 0, uInfo);
	return result;
}


VOID WINAPI OutputStringA(const CHAR* lpString)
{
	HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD nRet = 0;
	WriteConsoleA(hOutputHandle, lpString, lstrlenA(lpString), &nRet, NULL);
}


static char mbPath[260] = { 0 };
FILE *fin = NULL;
HANDLE WINAPI HookCreateFileA(
	_In_     LPCSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
	)
{
	fprintf(fin, "%s\r\n", lpFileName);

	if (strstr(lpFileName, mbPath))
	{
		return INVALID_HANDLE_VALUE;
	}
	else
	{
		return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
}

BOOL WINAPI HookIsDebuggerPresent()
{
	return 0;
}


FARPROC pfIsDebuggerPresent = NULL;
FARPROC pfCreateWindowA = NULL;
FARPROC pfMessageBoxA = NULL;
FARPROC pflStrcmpiA = NULL;
FARPROC pfCreateFileA = NULL;

VOID WINAPI InitHook()
{
	//AllocConsole();

	GetCurrentDirectoryA(260, mbPath);
	lstrcatA(mbPath, "/graph/");
	for (ULONG i = 0; i < lstrlenA(mbPath); i++)
	{
		if (mbPath[i] == '\\')
		{
			mbPath[i] = '/';
		}
	}

	fin = fopen("0.txt", "wb");

	pfCreateWindowA = GetProcAddress(GetModuleHandleW(L"User32.dll"), "CreateWindowExA");
	pfMessageBoxA = GetProcAddress(GetModuleHandleW(L"User32.dll"), "MessageBoxA");
	pflStrcmpiA = GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "lstrcmpiA");
	//pfCreateFileA = GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "CreateFileA");
	pfIsDebuggerPresent = GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "IsDebuggerPresent");

	StartHook("User32.dll", pfCreateWindowA, (PROC)HookCreateWindowExA);
	StartHook("User32.dll", pfMessageBoxA, (PROC)HookMessageBoxA);
	StartHook("Kernel32.dll", pflStrcmpiA, (PROC)HooklStrcmpiA);
	//StartHook("Kernel32.dll", pfCreateFileA, (PROC)HookCreateFileA);
	StartHook("Kernel32.dll", pfIsDebuggerPresent, (PROC)HookIsDebuggerPresent);
}

__declspec(dllexport)WCHAR* WINAPI XmoeLinkProc()
{
	static WCHAR* Result = L"xmoe_20111019";
	return Result;
}

