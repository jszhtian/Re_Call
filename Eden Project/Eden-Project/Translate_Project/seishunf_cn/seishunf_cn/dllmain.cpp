// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

DWORD FileSize = 0;
char* FileBuff = NULL;
//Need alloc mem by yourself.
void __stdcall EdenReadFile(char* FileName)
{
	if (FileName)
	{
		cout << "------------------------------------------------------" << endl;
		cout << "EdenReadFile:ReadFileName:" << FileName << endl;
		string fnm((char*)FileName);
		string Dir = "Project\\";
		string OutName = Dir + fnm;
		FILE* f = fopen(OutName.c_str(), "rb");
		if (f != NULL)
		{
			cout << "EdenReadFile:ReadOutFile:" << (char*)FileName << endl;
			fseek(f, 0, SEEK_END);
			FileSize = ftell(f);
			fseek(f, 0, SEEK_SET);
			FileBuff = (char*)malloc(FileSize);
			if (FileBuff != NULL && FileSize != 0)
			{
				fread(FileBuff, FileSize, 1, f);
				fclose(f);
				cout << "EdenReadFile:ReadOutFileSuccess" << endl;
			}
		}
		cout << "------------------------------------------------------" << endl;
	}
}

void __stdcall EdenFileReplace(char* Buffer)
{
	if (FileBuff && FileSize != 0)
	{
		memcpy(Buffer, FileBuff, FileSize);
		FileBuff = NULL;
		FileSize = NULL;
	}
}

PVOID GetFileNameAddr = NULL;
__declspec(naked) void GetFileName()
{
	_asm
	{
		pushfd
		pushad
		push edx
		call EdenReadFile
		popad
		popfd
		jmp GetFileNameAddr
	}
}

PVOID GetNormalFileBufferAddr = NULL;
__declspec(naked) void GetNormalFileBuffer()
{
	_asm
	{
		pushfd
		pushad
		push EDX
		call EdenFileReplace
		popad
		popfd
		jmp GetNormalFileBufferAddr
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
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

