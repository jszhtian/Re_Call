// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <mutex>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
#pragma warning(disable:4311)
#pragma warning(disable:4312)  
#pragma warning(disable:4996) 
typedef LSTATUS(WINAPI* Func_RegOpenKeyExA)(HKEY   hKey, LPCSTR lpSubKey, DWORD  ulOptions, REGSAM samDesired, PHKEY  phkResult);
PVOID p_RegOpenKeyEx = RegOpenKeyExA;
typedef bool (WINAPI* Func_SetWindowTextA)(HWND hWnd, LPCSTR lpString);
PVOID p_SetWindowTextA = SetWindowTextA;
typedef HFONT(WINAPI* Func_CreateFontIndirectA)(LOGFONTA* lplf);
PVOID p_CreateFontIndirectA = CreateFontIndirectA;
typedef BYTE* (__cdecl* Func_ReadCompFile)(char* FileName, DWORD* origlength);
PVOID p_ReadCompFile;
typedef HFONT(WINAPI* Func_CreateFontA)(_In_ int     nHeight,
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
	_In_ LPCSTR lpszFace);
PVOID p_CreateFontA = CreateFontA;
LSTATUS WINAPI NewRegOpenKeyExA(HKEY hk, LPCSTR subkey, DWORD ulO, REGSAM sam, PHKEY phk)
{
	wchar_t buffer[256];
	if (subkey)
	{
		size_t nu = strlen(subkey);
		size_t n = (size_t)MultiByteToWideChar(932, 0, subkey, int(nu), NULL, 0);
		memset(buffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, subkey, int(nu), buffer, int(n));
	}
	return RegOpenKeyExW(hk, buffer, ulO, sam, phk);
}

HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	LOGFONTA lf;
	memcpy(&lf, lplf, sizeof(LOGFONTA));
	strcpy(lf.lfFaceName, "黑体");
	lf.lfCharSet = GB2312_CHARSET;
	return (Func_CreateFontIndirectA(p_CreateFontIndirectA))(&lf);
}

bool WINAPI NewSetWindowTextA(HWND hw, LPCSTR lps)
{
	wchar_t buffer[256];
	if (lps)
	{
		size_t nu = strlen(lps);
		size_t n = (size_t)MultiByteToWideChar(932, 0, lps, int(nu), NULL, 0);
		memset(buffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, lps, int(nu), buffer, int(n));
	}

	return SetWindowTextW(hw, buffer);

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
	lpszFace = "黑体";
	fdwCharSet = GB2312_CHARSET;
	return ((Func_CreateFontA)p_CreateFontA)(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
}

void DumpFileFromMem(char* FileName, char* FileBuff)
{
	
}

BOOL read = FALSE;
BYTE* __cdecl NewReadCompFile(char* fnm, DWORD* ori)
{
	DWORD dwFileSize = 0;
	DWORD dwBytesRead = 0;
	char* FileBuf = 0;
	bool isReadSuccess = false;
	cout << "FILE:" << fnm << endl;
	wchar_t buffer[256];
	if (fnm)
	{
		size_t nu = strlen(fnm);
		size_t n = (size_t)MultiByteToWideChar(932, 0, fnm, int(nu), NULL, 0);
		memset(buffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, fnm, int(nu), buffer, int(n));
	}
	if (read) 
	{
		auto hFile = CreateFile(buffer, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			goto defaultRouting;
		}
		dwFileSize = GetFileSize(hFile, 0);
		FileBuf = new char[dwFileSize];
		if (FileBuf == 0)
		{
			goto defaultRouting;
		}
		memset(FileBuf, 0, dwFileSize);
		isReadSuccess = ReadFile(hFile, FileBuf, dwFileSize, &dwBytesRead, NULL);
		CloseHandle(hFile);
		if (!isReadSuccess)
		{
			goto defaultRouting;
		}
		*ori = dwFileSize;
		return (BYTE*)FileBuf;
	}
	else
	{
		/*
		cout << "Dump this file out." << endl;
		char* W_FileName = strcat((char*)".\\", fnm);
		FILE* f = fopen(W_FileName, "wb");
		fwrite()
		*/

	}
defaultRouting:
	return (Func_ReadCompFile(p_ReadCompFile))(fnm, ori);
}

void EdenInitHook()
{
	DWORD baseAddr = (DWORD)GetModuleHandle(NULL);
	p_ReadCompFile = (PVOID)(baseAddr + 0x5940);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&p_RegOpenKeyEx, NewRegOpenKeyExA);
	DetourAttach(&p_SetWindowTextA, NewSetWindowTextA);
	//DetourAttach(&p_CreateFontIndirectA, NewCreateFontIndirectA);
	//DetourAttach(&p_CreateFontA, newCreateFontA);
	DetourAttach(&p_ReadCompFile, NewReadCompFile);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Hook目标函数失败", L"严重错误", MB_OK | MB_ICONWARNING);
	}
	//SJISBypass();

}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "Open Console Success!" << std::endl;
}

void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
	DWORD oldProtect2;
	VirtualProtect(dest, size, oldProtect, &oldProtect2);
}

void Bypass()
{
	BYTE Patch1[] = { 0xFF };
	BYTE Patch2[] = { 0x90 };
	BYTE Patch3[] = { 0xEB };
	//memcopy((void*)(BaseAddr + 0x92FA), Patch2, sizeof(Patch2));
	//memcopy((void*)(BaseAddr + 0x92FB), Patch2, sizeof(Patch2));
	memcopy((void*)(BaseAddr + 0x26332), Patch3, sizeof(Patch3));
	//cmp al,0x9F
	memcopy((void*)(BaseAddr + 0x3ADD3), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x3ADDB), Patch1, sizeof(Patch1));
	//cmp cl,0x9F
	memcopy((void*)(BaseAddr + 0x29D1C), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x29D26), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x3ABE6), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x3ABF0), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x3BDD4), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x3BDDE), Patch1, sizeof(Patch1));
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
		Bypass();
		EdenInitHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

