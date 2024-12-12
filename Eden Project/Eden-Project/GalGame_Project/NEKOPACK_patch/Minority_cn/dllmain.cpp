#include "framework.h"
#include "detours.h"
#include <iostream>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	if (strcmp((lplf->lfFaceName), "俵俽 僑僔僢僋") == 0)
		strcpy(lplf->lfFaceName, "黑体");
	else if (strcmp((lplf->lfFaceName), "俵俽 柧挬") == 0)
		strcpy(lplf->lfFaceName, "宋体");

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

void APIHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourTransactionCommit();
}

DWORD MNG_Buffer = NULL;

DWORD FileSize = 0;
BYTE* FileBuff = nullptr;

BOOL IsPackSet = FALSE;
//Need alloc mem by yourself.
void __stdcall EdenReadFile(char* FileName)
{
	if (FileName)
	{
		//cout << "ReadFileName:" << FileName << endl;
		char OutNme[MAX_PATH];
		sprintf(OutNme, "%s%s", "Active_cn\\", FileName);
		FILE* f = fopen(OutNme, "rb");
		if (f != NULL)
		{
			fseek(f, 0, SEEK_END);
			FileSize = ftell(f);
			fseek(f, 0, SEEK_SET);
			FileBuff = (BYTE*)malloc(FileSize);
			if (FileBuff != NULL && FileSize != 0)
			{
				fread(FileBuff, FileSize, 1, f);
				cout << "ReadOutFile:" << (char*)FileName << endl;
			}
			else
			{
				MessageBox(0, L"Can't read FileBuff.", 0, 0);
			}
			fclose(f);
		}
		else if (IsPackSet)
		{
			BOOL ret = GetFileBuffByName(FileName, &FileBuff, &FileSize);
			if(ret)
				cout << "ReadPackageFile:" << (char*)FileName << endl;
		}
	}
}

void __stdcall EdenFileReplace(char* Buffer)
{
	if (FileSize != 0)
	{
		memcpy(Buffer, FileBuff, FileSize);
		free(FileBuff);
		FileSize = 0;
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

PVOID GetMNGImageBufferAddr = NULL;
__declspec(naked) void GetMNGImageBuffer()
{
	_asm
	{
		pushfd
		pushad
		mov MNG_Buffer, ECX
		popad
		popfd
		jmp GetMNGImageBufferAddr
	}
}

PVOID ReadMNGImageFileAddr = NULL;
__declspec(naked) void ReadMNGImageFile()
{
	_asm
	{
		pushfd
		pushad
		push MNG_Buffer
		call EdenFileReplace
		popad
		popfd
		jmp ReadMNGImageFileAddr
	}
}

PVOID AllocMemAddr = NULL;
__declspec(naked) void AllocMem()
{
	_asm
	{
		cmp     FileSize, 0;
		je have;
		mov     ebx, FileSize;
	have:
		jmp AllocMemAddr;
	}
}

PVOID AllocMemAdd2 = NULL;
__declspec(naked) void AllocMem2()
{
	_asm
	{
		cmp     FileSize, 0;
		je have;
		mov     ebx, FileSize;
	have:
		jmp AllocMemAdd2;
	}
}

void EdenReadFileSuppost()
{
	GetFileNameAddr = (PVOID)(BaseAddr + 0xF369D);
	GetNormalFileBufferAddr = (PVOID)(BaseAddr + 0xF13C0);
	GetMNGImageBufferAddr = (PVOID)(BaseAddr + 0xF131E);
	ReadMNGImageFileAddr = (PVOID)(BaseAddr + 0xF1325);
	AllocMemAddr = (PVOID)(BaseAddr + 0xF130A);
	AllocMemAdd2 = (PVOID)(BaseAddr + 0xF1376);

	cout << hex << BaseAddr << endl;
	cout << hex << GetFileNameAddr << endl;
	cout << hex << GetNormalFileBufferAddr << endl;
	cout << hex << GetMNGImageBufferAddr << endl;
	cout << hex << ReadMNGImageFileAddr << endl;
	cout << hex << AllocMemAddr << endl;

	DetourTransactionBegin();
	DetourAttach((void**)&GetFileNameAddr, GetFileName);
	DetourAttach((void**)&GetNormalFileBufferAddr, GetNormalFileBuffer);
	DetourAttach((void**)&GetMNGImageBufferAddr, GetMNGImageBuffer);
	DetourAttach((void**)&ReadMNGImageFileAddr, ReadMNGImageFile);
	DetourAttach((void**)&AllocMemAddr, AllocMem);
	DetourAttach((void**)&AllocMemAdd2, AllocMem2);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"启动失败", L"启动失败", MB_OK);
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//make_console();
		if (InitPackInfo())
		{
			IsPackSet = TRUE;
		}
		APIHook();
		EdenReadFileSuppost();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
/*
extern "C" __declspec(dllexport) void dummy(void)
{
	//return;
}
*/