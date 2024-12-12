#include "framework.h"
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "zlib.h"
#pragma comment(lib, "zlib.lib")
#pragma pack(1)

typedef struct file_node_s
{
	char f_name[260];
	unsigned int position;
	unsigned int compr_size;
	unsigned int source_size;
	char f_reserved[0x20];
}file_node_t;

typedef struct file_header_s
{
	char header[MAX_PATH];
	unsigned int f_count;
}file_header_t;
#pragma pack()

typedef struct file_writer_node_s
{
	char f_name[260];
	char f_diskpath[260];
	unsigned int source_size;
}file_writer_node_t;

typedef enum
{
	FS_SUCCESS,
	FS_FILE_NOTFOUND,
	FS_HEADER_FAILED,
	FS_CREATE_FAILED,
}FileSystemErrno;

char* HeadSig = (char*)"AyaminokazeFileSystemVer1.00";

FILE* fd;
file_header_t f_header;
file_node_t* f_node;
file_writer_node_t* f_wrtnode;

PVOID CDECL HostAlloc(LONG Size)
{
	return HeapAlloc(GetProcessHeap(), 0, Size);
}

VOID CDECL HostFree(PVOID lpMem)
{
	if (!lpMem)
		return;

	__try
	{
		HeapFree(GetProcessHeap(), 0, lpMem);
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}

void Func1(BYTE* src, DWORD len)
{
	__asm {
		mov		eax, src;
		mov		ebx, len;
	Tag:
		mov     dx, word ptr[eax];
		movzx   esi, dx;
		and dx, 0x5555;
		and esi, 0xAAAAAAAA;
		shr     esi, 1;
		add     edx, edx;
		or si, dx;
		mov     edx, esi;
		mov     word ptr[eax], dx;
		inc     ecx;
		add     eax, 0x2;
		cmp     ebx, ecx;
		ja      Tag;
	End:
	}
}

BYTE rol(int val, int n)
{
	n %= 8;
	return (val << n) | (val >> (8 - n));
}

BYTE ror(int val, int n)
{
	n %= 8;
	return (val >> n) | (val << (8 - n));
}

void cryption(BYTE* buff, DWORD size, int dec)
{
	if (dec == 0)
	{
		DWORD count = (size - 0x10) / 4;
		BYTE* p = buff + 0x10;

		DWORD key1 = 114514;
		DWORD key2 = 0x3AD023;
		DWORD c = 0x114;

		for (DWORD i = 0; i < count; i++)
		{
			DWORD* dp = (DWORD*)p;
			*dp ^= key2;
			*dp -= key1 ^ key2;
			*dp ^= key1;
			p = (BYTE*)dp;
			*p = ror(*p, c++);
			c &= 0xFF;
			p += 4;
		}

		Func1(buff, size);
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0x01010101;
			buff[i] += 0x12;
		}
		Func1(buff, size);
	}
	else if (dec == 1)
	{
		DWORD count = (size - 0x10) / 4;
		BYTE* p = buff + 0x10;

		DWORD key1 = 114514;
		DWORD key2 = 0x3AD023;
		DWORD c = 0x114;
		Func1(buff, size);
		for (int i = 0; i < size; i++)
		{
			buff[i] -= 0x12;
			buff[i] ^= 0x01010101;
		}
		Func1(buff, size);
		for (DWORD i = 0; i < count; i++)
		{
			*p = rol(*p, c++);
			//*p = CPP_ROL(*p, c++);
			c &= 0xFF;
			DWORD* dp = (DWORD*)p;
			*dp ^= key1;
			*dp += key1 ^ key2;
			*dp ^= key2;
			p = (BYTE*)dp + 4;

		}
	}
	else if (dec == 2)
	{
		int seed = strlen(HeadSig);
		int key = (((seed << 5) ^ 0xA5) * (seed + 0x6F349)) ^ 0x34A9B129;
		unsigned int* p = (unsigned int*)(buff + 0x10);
		for (int i = 0; i < (seed - 13) / 4; i++)
		{
			p[i] ^= key;
		}
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0xFFFFFFFF;
		}

	}
	else if (dec == 3)
	{
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0xFFFFFFFF;
		}
		int seed = strlen(HeadSig);
		int key = (((seed << 5) ^ 0xA5) * (seed + 0x6F349)) ^ 0x34A9B129;
		unsigned int* p = (unsigned int*)(buff + 0x10);
		for (int i = 0; i < (seed - 13) / 4; i++)
		{
			p[i] ^= key;
		}
	}
}

int fs_open(const char* path) {
	int node_size;

	fd = fopen(path, "rb");

	if (!fd) {
		return FS_FILE_NOTFOUND;
	}

	if (fread(&f_header, sizeof(f_header), 1, fd) != 1) {
		fclose(fd);
		fd = NULL;
		return FS_HEADER_FAILED;
	}

	if (strcmp(f_header.header, HeadSig) != 0) {
		fclose(fd);
		fd = NULL;
		return FS_HEADER_FAILED;
	}

	node_size = sizeof(file_node_t) * f_header.f_count;
	f_node = (file_node_t*)malloc(node_size);
	if (fread(f_node, 1, node_size, fd) != node_size) {
		fclose(fd);
		fd = NULL;
		return FS_HEADER_FAILED;
	}

	return FS_SUCCESS;
}

int fs_havefile(const char* f_name) {
	unsigned int i;
	if (!f_node) return 0;

	for (i = 0; i < f_header.f_count; i++) {
		if (_stricmp(f_node[i].f_name, f_name) == 0) {
			return 1;
		}
	}
	return 0;
}

unsigned int fs_filesize(const char* f_name) {
	unsigned int i;
	if (!f_node) return 0;

	for (i = 0; i < f_header.f_count; i++) {
		if (_stricmp(f_node[i].f_name, f_name) == 0) {
			return f_node[i].source_size;
		}
	}
	return 0;
}

void fs_readbuffer(file_node_t* node, void** data, unsigned long* f_size) {
	unsigned char* compr_buf;
	unsigned char* uncompr_buf;
	unsigned long uncompr_length;

	fseek(fd, node->position, SEEK_SET);

	compr_buf = (unsigned char*)malloc(node->compr_size);
	uncompr_buf = (unsigned char*)malloc(node->source_size);
	uncompr_length = node->source_size;
	fread(compr_buf, node->compr_size, 1, fd);
	cryption(compr_buf, node->compr_size, 3);
	if (uncompress(uncompr_buf, &uncompr_length, compr_buf, node->compr_size) == Z_OK) {
		*data = uncompr_buf;
		*f_size = uncompr_length;
		byte key = 0;
		cryption(uncompr_buf, uncompr_length, 1);
		free(compr_buf);
	}
	else {
		free(compr_buf);
		free(uncompr_buf);

		*data = NULL;
		*f_size = 0;
	}
}

bool fs_readfile(const char* f_name, void** data, unsigned long* f_size) {
	unsigned int i;
	if (!f_node) return false;

	for (i = 0; i < f_header.f_count; i++) {
		if (_stricmp(f_node[i].f_name, f_name) == 0) {
			fs_readbuffer(&f_node[i], data, f_size);
			return true;
		}
	}
	return false;
}
char* Name = NULL;

PVOID pGetName = NULL;
PVOID dGetName = NULL;
__declspec(naked)void GetName()
{
	__asm
	{
		pushad
			pushfd
				mov Name, edi
			popfd
		popad
		push eax
		sub esp,0x14
		push ebx
		jmp dGetName
	}
}
char* FileNameToLower(char* FileName)
{
	char* Result=nullptr;

	for (ULONG i = 0; i < strlen(FileName); i++)
	{
		if (FileName[i] <= 'Z' && FileName[i] >= 'A')
			Result += towlower(FileName[i]);
		else
			Result += FileName[i];
	}
	return Result;
}
/*
int CheckFileName(char* FileName)
{
	PVOID jmpaddr = (PVOID)(BaseAddr + 0x33490);
	PVOID jmpout = (PVOID)(BaseAddr + 0x6d9f9);
	string strOldName(FileName);
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	//string strNewName;
	if (strName=="BGI.gdb")
	{
		_asm {
			jmp jmpaddr;
			jmp jmpout;
		}
	}
	else
	{
		int check=fs_havefile(FileName);
		return check;
	}
}
*/
LPVOID MyHost(char* FileName)
{
	if (fs_filesize(FileName) != 0)
	{
		return HeapAlloc(GetProcessHeap(), 0, fs_filesize(FileName));
	}
	else
	{
		PVOID jmpin = (PVOID)(BaseAddr+ 0xb4598);
		PVOID jmpout = (PVOID)(BaseAddr+ 0x6db6f);
		_asm {
			push edi;
			jmp jmpin;
			popfd;
			popad;
			jmp jmpout;
		}
	}
}
PVOID pGetAlloc = NULL;
PVOID dGetAlloc = NULL;
__declspec(naked)void GetAlloc()
{
	__asm
	{
		pushad
			pushfd
				push esi
				call MyHost
			popfd
		popad
		jmp dGetName
	}
}
void WINAPI CopyMyFile(void* offset)
{

	if (Name)
	{
		void* buff;
		unsigned long size;
		if (fs_readfile(Name, &buff, &size))
		{
			cout << Name << endl;
			//PBYTE copybuff=(PBYTE)HostAlloc(size);
			memcpy(offset, buff, size);
		}
	}

}
PVOID LoadFileBuffer_HookAddr = NULL;
PVOID LoadFileBuffer_Endl = NULL;
__declspec(naked)void LoadFileBuffer()
{
	_asm
	{
		pushad
			pushfd
				push ecx //offset
				call CopyMyFile
			popfd
		popad
		push eax
		sub esp, 0x28
		push ebx
		jmp LoadFileBuffer_Endl
	}
}

bool FileCopy()
{
	pGetName = (PVOID)(BaseAddr + 0x6d9b0);
	dGetName = (PVOID)(BaseAddr + 0x6d9b5);
	LoadFileBuffer_HookAddr = (PVOID)(BaseAddr + 0x6d750);
	LoadFileBuffer_Endl = (PVOID)(BaseAddr + 0x6d755);
	pGetAlloc = (PVOID)(BaseAddr + 0x6db6A);
	dGetAlloc = (PVOID)(BaseAddr + 0x6db6f);
	cout << hex << pGetName << endl;
	cout << hex << dGetName << endl;
	cout << hex << LoadFileBuffer_HookAddr << endl;
	cout << hex << LoadFileBuffer_Endl << endl;
	cout << hex << pGetAlloc << endl;
	cout << hex << dGetAlloc << endl;
	if (fs_open("Kaze.kze")) {
		MessageBox(0, L"无法打开Kaze.kze", L"Ayaminokaze File System", 0);
		return false;
	}
	MEM_SetNopCode((BYTE*)pGetName, 5);
	MEM_SetNopCode((BYTE*)LoadFileBuffer_HookAddr, 5);
	if (ReplaceJump((PBYTE)pGetName, (PBYTE)&GetName))
	{
		if (ReplaceJump((PBYTE)LoadFileBuffer_HookAddr, (PBYTE)&LoadFileBuffer))
		{
			//if (ReplaceJump((PBYTE)pGetAlloc, (PBYTE)& GetAlloc))
				return true;
		}
		else 
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	/*
	if (pGetName) 
	{
		if (LoadFileBuffer_HookAddr) 
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach((void**)& pGetName, GetName);
			DetourAttach((void**)& LoadFileBuffer_HookAddr, LoadFileBuffer);
			if (DetourTransactionCommit() != NO_ERROR)
			{
				MessageBoxW(NULL, L"Read file error.", L"FileSystem", MB_OK | MB_ICONERROR);
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
	*/
}