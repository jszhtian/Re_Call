// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <mutex>
using namespace std;
#pragma warning(disable:4311)
#pragma warning(disable:4312)  
#pragma warning(disable:4996) 
typedef LSTATUS (WINAPI* Func_RegOpenKeyExA)(HKEY   hKey, LPCSTR lpSubKey, DWORD  ulOptions, REGSAM samDesired, PHKEY  phkResult);
PVOID p_RegOpenKeyEx = RegOpenKeyExA;
typedef bool (WINAPI* Func_SetWindowTextA)(HWND hWnd, LPCSTR lpString);
PVOID p_SetWindowTextA = SetWindowTextA;
typedef HFONT (WINAPI* Func_CreateFontIndirectA)(LOGFONTA *lplf);
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
LSTATUS WINAPI NewRegOpenKeyExA(HKEY hk,LPCSTR subkey,DWORD ulO,REGSAM sam,PHKEY phk)
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

bool WINAPI NewSetWindowTextA(HWND hw, LPCSTR lps)
{
	wchar_t buffer[256];
	wchar_t chsTitle[]=L"住在下体升级岛上的贫乳该如何是好？ 【善雅鸽汉化组】 build 20200929";
	if (lps)
	{
		size_t nu = strlen(lps);
		size_t n = (size_t)MultiByteToWideChar(932, 0, lps, int(nu), NULL, 0);
		memset(buffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, lps, int(nu), buffer, int(n));
		if (wcscmp(buffer,L"抜きゲーみたいな島に住んでる貧乳はどうすりゃいいですか？")==0)
		{
			wcscpy(buffer,chsTitle);
		}
	}
	
	return SetWindowTextW(hw, buffer);

}

void memcopy(void* dest, void*src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
	DWORD oldProtect2;
	VirtualProtect(dest, size, oldProtect, &oldProtect2);
}

/*
 byte *__cdecl sub_405940(LPCSTR LoadName, DWORD *OrigLength)
{
  const CHAR *FileName; // esi@2
  byte *fileBuf; // eax@3
  char *v4; // eax@4
  int v5; // ebp@9
  const CHAR *v6; // ebx@10
  CHAR String2; // [sp+10h] [bp-104h]@26

  if ( !sub_415390() )
	goto LABEL_27;
  FileName = LoadName;
  if ( strchr(LoadName, 58) )
  {
	fileBuf = LoadFileFromPAK(&Default, LoadName, OrigLength);
	if ( fileBuf )
	  return fileBuf;
	v4 = strrchr(LoadName, 92);
	if ( v4 || (v4 = strrchr(LoadName, 58)) != 0 )
	  FileName = v4 + 1;
  }
  if ( !Exe_Path || (fileBuf = LoadFileFromPAK(&Exe_Path, FileName, OrigLength)) == 0 )
  {
	v5 = 0;
	if ( dword_D0A7A8 > 0 )
	{
	  v6 = &byte_8EEA50;
	  do
	  {
		fileBuf = LoadFileFromPAK(v6, FileName, OrigLength);
		if ( fileBuf )
		  return fileBuf;
		++v5;
		v6 += 260;
	  }
	  while ( v5 < dword_D0A7A8 );
	}
	if ( Data )
	{
	  fileBuf = LoadFileFromPAK(&Data, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A28C )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A28C, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	fileBuf = LoadFileFromPAK(&Buffer, FileName, OrigLength);
	if ( fileBuf )
	  return fileBuf;
	if ( String )
	{
	  fileBuf = LoadFileFromPAK(&String, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A394 )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A394, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A398 )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A398, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	if ( byte_D0A49C )
	{
	  fileBuf = LoadFileFromPAK(&byte_D0A49C, FileName, OrigLength);
	  if ( fileBuf )
		return fileBuf;
	}
	sub_415410();
	wsprintfA(&String2, aFileNotFound_S, FileName);
	lstrcpyA(&String1, &String2);
LABEL_27:
	fileBuf = 0;
  }
  return fileBuf;
}
 
 */

BYTE* __cdecl NewReadCompFile(char* fnm,DWORD* ori)
{
	cout << "FILE:" << fnm << endl;
	DWORD dwFileSize = 0;
	DWORD dwBytesRead = 0;
	char* FileBuf = 0;
	bool isReadSuccess = false;
	wchar_t buffer[256];
	if (fnm)
	{
		size_t nu = strlen(fnm);
		size_t n = (size_t)MultiByteToWideChar(932, 0, fnm, int(nu), NULL, 0);
		memset(buffer, 0, sizeof(wchar_t) * 256);
		MultiByteToWideChar(932, 0, fnm, int(nu), buffer, int(n));
	}

	auto hFile = CreateFile(buffer, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		goto defaultRouting;
	}
	dwFileSize = GetFileSize(hFile, 0);
	//FileBuf = new char[dwFileSize];
	FileBuf = (char*)VirtualAlloc(0, dwFileSize, 0x103000, PAGE_READWRITE);
	if (FileBuf == 0)
	{
		goto defaultRouting;
	}
	memset(FileBuf, 0, dwFileSize);
	isReadSuccess = ReadFile(hFile, FileBuf, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);
	if (!isReadSuccess)
	{
		VirtualFree(FileBuf, 0, MEM_RELEASE);
		goto defaultRouting;
	}
	*ori = dwFileSize;
	return (BYTE*)FileBuf;
defaultRouting:
	return (Func_ReadCompFile(p_ReadCompFile))(fnm, ori);
}




void init()
{
	DWORD baseAddr = (DWORD)GetModuleHandle(NULL);
	p_ReadCompFile = (PVOID)(baseAddr + 0x5940);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&p_RegOpenKeyEx, NewRegOpenKeyExA);
	//DetourAttach(&p_SetWindowTextA, NewSetWindowTextA);
	//DetourAttach(&p_ReadCompFile, NewReadCompFile);
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

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//setlocale(LC_ALL, "chinese");
		//make_console();
		init();
		//MessageBoxW(NULL, L"MSG", L"TEST", MB_OK);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C"  void dummy(void) {
	return;
}