// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <mutex>
#include "key.h"
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
/*
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
	if (strName == "data.xp3")
	{
		strNewName = strDirName + "\\plugin\\GreatGrandmother.dll";
	}
	else 
	{
		strNewName = strOldName;
	}
	cout << strNewName << endl;
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		strNewName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}
*/
/*
00611A02  |.  8D45 D0       lea eax,[local.12]
00611A05  |.  BA 02000000   mov edx,0x2
00611A0A  |.  E8 614E0800   call GreatGra.00696870
00611A0F  |.  68 295B6F00   push GreatGra.006F5B29                   ;  data.xp3
00611A14  |.  8D8D D0FAFFFF lea ecx,[local.332]
00611A1A  |.  51            push ecx                                 ;  GreatGra.<ModuleEntryPoint>
00611A1B  |.  E8 9028F5FF   call GreatGra.005642B0
*/
#include <mutex>
static std::mutex mtx;
static PVOID TEXT_JMPHookPoint;
static PVOID TEXT_JMPHookPointNext;
static char* FileName = (char*)"GreatGrandmother_cn.dat";
__declspec(naked) void FileRead()
{
	_asm
	{
		push FileName
		jmp TEXT_JMPHookPointNext
	}
}

void JMPHelper(PVOID HookPoint, PVOID JMP_FUNC)
{
	DWORD oldProtect = 0;
	VirtualProtect(HookPoint, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	DWORD offset = (DWORD)JMP_FUNC - (DWORD)HookPoint - 5;
	char JMPOp[] = { 0xE9 };
	memcpy(HookPoint, &JMPOp, 1);
	memcpy((char*)HookPoint + 1, &offset, sizeof(DWORD));
	VirtualProtect(HookPoint, 5, oldProtect, &oldProtect);
}
void init()
{
	*(DWORD*)& TEXT_JMPHookPoint = BaseAddr + 0x211A0F;
	*(DWORD*)& TEXT_JMPHookPointNext = BaseAddr + 0x211A14;
	JMPHelper(TEXT_JMPHookPoint, FileRead);
}

void XorEnc(PBYTE InBuffer, ULONG InSize)
{
	for (ULONG i = 50; i < 100; i++)
	{
		InBuffer[i] ^= Key3[i % 1024];
		InBuffer[i + 2] ^= key2[i % 1024];
		InBuffer[i + 3] ^= Key4[i % 1024];
		InBuffer[i + 4] ^= Key1[i % 1024];
	}
	for (ULONG i = 200; i < 320; i++)
	{
		InBuffer[i + 5] ^= Key1[i % 1024] ^ Key3[i % 1024];
		InBuffer[i + 6] ^= key2[i % 1024] ^ Key4[i % 1024];
		InBuffer[i + 7] ^= Key1[i % 1024] ^ key2[i % 1024] ^ Key4[i % 1024] ^ Key3[i % 1024];
		InBuffer[i + 8] ^= Key4[i % 1024] ^ key2[i % 1024] ^ Key1[i % 1024] ^ Key3[i % 1024];
	}
	for (ULONG i = 426; i < 795; i++)
	{
		InBuffer[i + 9] ^= Key4[i % 1024] ^ Key1[i % 1024];
		InBuffer[i + 10] ^= Key4[i % 1024] ^ key2[i % 1024];
		InBuffer[i + 11] ^= Key1[i % 1024] ^ key2[i % 1024] ^ Key4[i % 1024] ^ Key3[i % 1024];
		InBuffer[i + 12] ^= Key4[i % 1024] ^ key2[i % 1024] ^ Key1[i % 1024] ^ Key3[i % 1024];
	}
	for (ULONG i = 1000; i < 2046; i++)
	{
		InBuffer[i + 13] ^= key2[i % 1024] ^ Key4[i % 1024];
		InBuffer[i + 14] ^= Key1[i % 1024] ^ key2[i % 1024];
		InBuffer[i + 15] ^= Key1[i % 1024] ^ key2[i % 1024] ^ Key4[i % 1024] ^ Key3[i % 1024];
		InBuffer[i + 16] ^= Key1[i % 1024];
	}
	for (ULONG i = 2048; i < InSize; i++)
	{
		InBuffer[i] ^= Key4[i % 1024];
		InBuffer[i + 2] ^= Key1[i % 1024];
		InBuffer[i + 3] ^= key2[i % 1024];
		InBuffer[i + 4] ^= Key3[i % 1024];
	}
}

void TextFormat()
{
	FILE* fp = fopen("GreatGrandmother_cn.dat", "rb");
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned char* buff = new unsigned char[size];
	fread(buff, size, 1, fp);
	fclose(fp);

	XorEnc(buff, size);
	fp = fopen("GreatGrandmother_cn.dat", "wb");
	fwrite(buff, size, 1, fp);
	fclose(fp);
	delete[] buff;
}

void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}

void MyCharSet()
{
	BYTE Patch1[] = { 0xC6 };
	BYTE Patch2[] = { 0x45 };
	BYTE Patch3[] = { 0xD7 };
	BYTE Patch4[] = { 0x86 };
	BYTE Patch5[] = { 0x90 };
	BYTE Patch6[] = { 0x90 };

	BYTE Patch7[] = { 0x86 };

	int CharSet1 = BaseAddr + 0x9CAA1;
	int CharSet2 = BaseAddr + 0x9CAA2;
	int CharSet3 = BaseAddr + 0x9CAA3;
	int CharSet4 = BaseAddr + 0x9CAA4;
	int CharSet5 = BaseAddr + 0x9CAA5;
	int CharSet6 = BaseAddr + 0x9CAA6;
	int FontChange = BaseAddr + 0x11435D;

	memcopy((void*)CharSet1, Patch1, sizeof(Patch1));
	memcopy((void*)CharSet2, Patch2, sizeof(Patch2));
	memcopy((void*)CharSet3, Patch3, sizeof(Patch3));
	memcopy((void*)CharSet4, Patch4, sizeof(Patch4));
	memcopy((void*)CharSet5, Patch5, sizeof(Patch5));
	memcopy((void*)CharSet6, Patch6, sizeof(Patch6));

	memcopy((void*)FontChange, Patch7, sizeof(Patch7));

}

static void make_console() 
{
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
		//make_console();
		//TextFormat();
		MyCharSet();
		init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
extern "C" __declspec(dllexport) void dummy(void) {
	return;
}
