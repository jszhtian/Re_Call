// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>
#include <filesystem>
using namespace std::filesystem;
using namespace std::chrono;

void DumpAllFiles(const path& src_dir, const path& dst_dir)
{
	copy_options op = copy_options::overwrite_existing | copy_options::recursive;
	copy(src_dir, dst_dir, op);
}



void WINAPI Init2()
{
	TCHAR file_name[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), file_name, ARRAYSIZE(file_name));
	path src = path(file_name).parent_path();
	path dst = "E:/Dump/";
	cout << src << endl;
	//cout << relative(file_name, dst).generic_string() << endl;
	DumpAllFiles(src, dst);
}

void WINAPI Init3(char* FileName)
{
	auto fp = fopen(FileName, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		auto size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (size > 0)
		{
			auto buff = malloc(size);
			fread(buff, size, 1, fp);
			auto fout = fopen(FileName, "wb");
			if (fout)
			{
				fwrite(buff, size, 1, fout);
				free(buff);
				fclose(fout);
			}
		}
	}
}

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
	if (strstr(lpFileName, "\\\\"))
		cout << lpFileName << endl;
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}


PVOID g_pOldCreateFileW = CreateFileW;
typedef HANDLE(WINAPI* PfuncCreateFileW)(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);


HANDLE WINAPI NewCreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	char* File = wtoc(lpFileName);
	if (strstr(File, "\\\\"))
		cout << wtoc(lpFileName) << endl;
	return ((PfuncCreateFileW)(g_pOldCreateFileW))(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

void Init4()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	//DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	DetourAttach(&g_pOldCreateFileW, NewCreateFileW);
	DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		make_console();
		//Init3();
		//Init4();
		//Init2();
	}
	case DLL_THREAD_ATTACH:
		Init4();
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

