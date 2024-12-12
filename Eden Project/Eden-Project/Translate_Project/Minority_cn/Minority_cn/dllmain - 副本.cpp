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
	if(strcmp((lplf->lfFaceName),"‚l‚r ƒSƒVƒbƒN")==0)
		strcpy(lplf->lfFaceName, "ºÚÌå");
	else if (strcmp((lplf->lfFaceName), "‚l‚r –¾’©")==0)
		strcpy(lplf->lfFaceName, "·ÂËÎ");

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
char* FileBuff = NULL;
//Need alloc mem by yourself.
void __stdcall EdenReadFile(char* FileName)
{
	if (FileName)
	{
		cout << "------------------------------------------------------" << endl;
		cout << "EdenReadFile:ReadFileName:" << FileName << endl;
		string fnm((char*)FileName);
		string Dir = "Dump\\";
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

PVOID GetMNGImageBufferAddr = NULL;
__declspec(naked) void GetMNGImageBuffer()
{
	_asm
	{
		pushfd
			pushad
				mov MNG_Buffer,ECX
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
		pushfd;
			pushad;
				cmp     FileBuff, 0;
				je have;
				cmp     FileSize, 0;
				je have;
				mov     eax, FileSize;
				add     eax, 0xA1A1;
				mov     dword ptr ss : [ebp - 0x44] , eax;
			have:
				popad;
				popfd;
				jmp AllocMemAddr;
	}
}

void EdenReadFileSuppost()
{
	GetFileNameAddr = (PVOID)(BaseAddr + 0x7CE65);
	GetNormalFileBufferAddr = (PVOID)(BaseAddr + 0x7AA38);
	GetMNGImageBufferAddr = (PVOID)(BaseAddr + 0x7A996);
	ReadMNGImageFileAddr = (PVOID)(BaseAddr + 0x7A99D);
	AllocMemAddr = (PVOID)(BaseAddr + 0x7A97B);

	cout << hex << BaseAddr << endl;
	cout << hex << GetFileNameAddr << endl;
	cout << hex << GetNormalFileBufferAddr << endl;
	cout << hex << GetMNGImageBufferAddr << endl;
	cout << hex << ReadMNGImageFileAddr << endl;
	cout << hex << AllocMemAddr << endl;

	DetourTransactionBegin();
	DetourAttach((void**)& GetFileNameAddr, GetFileName);
	DetourAttach((void**)& GetNormalFileBufferAddr, GetNormalFileBuffer);
	DetourAttach((void**)& GetMNGImageBufferAddr, GetMNGImageBuffer);
	DetourAttach((void**)& ReadMNGImageFileAddr, ReadMNGImageFile);
	DetourAttach((void**)& AllocMemAddr, AllocMem);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"Æô¶¯Ê§°Ü", L"Æô¶¯Ê§°Ü", MB_OK);
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
		make_console();
		APIHook();
		EdenReadFileSuppost();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void)
{
	return;
}