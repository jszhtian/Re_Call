// dllmain.cpp : 定义 DLL 应用程序的入口点。
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
/*************************************************************************************/

wstring GetKrkrFileName(LPCWSTR Name)
{
	std::wstring Info(Name);

	if (Info.find_last_of(L">") != std::wstring::npos)
		Info = Info.substr(Info.find_last_of(L">") + 1, std::wstring::npos);

	//if (Info.find_last_of(L"/") != std::wstring::npos)
		//Info = Info.substr(Info.find_last_of(L"/") + 1, std::wstring::npos);

	return Info;
}

char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_OEMCP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

char* FileBuff = NULL;
DWORD FileSize = NULL;
DWORD Buff = NULL;
void __stdcall EdenReadFile(LPCWSTR lpFileName)
{
	wstring FileName = GetKrkrFileName(lpFileName);
	wstring Path = L"osanafd_cn\\" + FileName;
	char* ReadFileName = wtoc(Path.c_str());
	FILE* fp = fopen(ReadFileName, "rb");
	if (fp)
	{
		cout << "TryToReadFileFrom:〖" << ReadFileName << "〗" << endl;
		fseek(fp, 0, SEEK_END);
		FileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		FileBuff = (char*)malloc(FileSize);
		fread(FileBuff, FileSize, 1, fp);
		fclose(fp);
	}
}

void __stdcall EdenAllocMem(DWORD OldSize)
{
	if (FileBuff && FileSize)
		OldSize = FileSize;
}

void __stdcall EdenMemCpy(DWORD offset, DWORD OldSize)
{
	if (FileBuff && FileSize && offset)
	{
		memcpy((void*)offset, FileBuff, FileSize);
		//OldSize = FileSize;
		FileBuff = NULL;
		FileSize = NULL;
		Buff = NULL;
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

PVOID AllocMemAddr = NULL;
__declspec(naked) void AllocMem()
{
	_asm
	{
		pushfd
		pushad
		//push dword ptr ss : [ebp - 0x104]
		//call EdenAllocMem
		mov eax, dword ptr ss : [ebp - 0x104] 
		cmp FileBuff,0
		je have
		cmp FileSize,0
		je have
		mov eax,FileSize
		have:
		popad
		popfd
		jmp AllocMemAddr
	}
}

PVOID GetFileBuffAddr = NULL;
__declspec(naked) void GetFileBuff()
{
	_asm
	{
		pushfd
		pushad
		mov Buff, eax
		popad
		popfd
		jmp GetFileBuffAddr
	}
}

PVOID MemCpyAddr = NULL;
__declspec(naked) void MemCpy()
{
	_asm
	{
		pushfd
		pushad
		push dword ptr ss : [ebp - 0x10C]
		push Buff
		call EdenMemCpy
		popad
		popfd
		jmp MemCpyAddr
	}
}
void EdenNativeHook()
{
	GetFileNameAddr = (PVOID)(BaseAddr + 0x1FB253);
	AllocMemAddr = (PVOID)(BaseAddr + 0x1EB0F0);
	GetFileBuffAddr = (PVOID)(BaseAddr + 0x1EB172);
	MemCpyAddr = (PVOID)(BaseAddr + 0x1EB186);

	cout << hex << BaseAddr << endl;
	cout << hex << GetFileNameAddr << endl;
	cout << hex << AllocMemAddr << endl;
	cout << hex << GetFileBuffAddr << endl;
	cout << hex << MemCpyAddr << endl;

	DetourTransactionBegin();
	DetourAttach((void**)& GetFileNameAddr, GetFileName);
	DetourAttach((void**)& AllocMemAddr, AllocMem);
	DetourAttach((void**)& GetFileBuffAddr, GetFileBuff);
	DetourAttach((void**)& MemCpyAddr, MemCpy);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"启动失败", L"启动失败", MB_OK);
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
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
		EdenNativeHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

