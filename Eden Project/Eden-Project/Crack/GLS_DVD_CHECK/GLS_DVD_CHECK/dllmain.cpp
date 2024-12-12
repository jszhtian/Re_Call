// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "crc32.h"
#include <iomanip>
#pragma comment(lib, "detours.lib")
using  namespace std;

static char* File;

PVOID g_pOldCreateFileA = CreateFileA;
typedef HANDLE(WINAPI* PfuncCreateFileA)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);


HANDLE WINAPI NewCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	if (!lpFileName)
	{
		return INVALID_HANDLE_VALUE;
	}

	string strOldName(lpFileName);
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string strNewName;
	if (File)
	{
		if (strName == File)
		{
			//MessageBox(0, L"FIND", L"AymKze", 0);
			strNewName = File;
		}
		else
		{
			strNewName = strOldName;
		}
	}
	else
	{
		MessageBox(0, L"Can't read check file", L"AymKze", 0);
		strNewName = strOldName;
	}
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		strNewName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

bool MyGetFileInfo()
{
	FILE* fp = fopen("AymKze.txt", "rb");
	if (!fp)
	{
		MessageBox(0, L"Can't load AymKze.txt", L"AymKze", 0);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	File = buff;
	return true;
}

void Init()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&g_pOldCreateFileA, NewCreateFileA);

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
		if(MyGetFileInfo())
			Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

