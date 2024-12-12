// dllmain.cpp : ��ȡ�Զ��������ļ������Ƶ���Ϸ�ڴ档
// code by ����, 2014/2/25

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <map>
#include <string>
#include <shlwapi.h>
#include "detours.h"
#include "zlib.h"
using namespace std;

///////////////////����ṹ//////////////////////////////////////////////////////////
#pragma pack (1)
typedef struct {
	char magic[4];			/* "MORE" */
	DWORD header_length;
	DWORD is_compressed;
	DWORD index_offset;
	DWORD index_length;
} header_t;

typedef struct {
	DWORD name_offset;
	DWORD name_length;
	DWORD offset;
	DWORD length;
	DWORD org_length;
} entry_t;

typedef struct {
	DWORD offset;
	DWORD length;
	DWORD org_length;
} info_t;

typedef struct {
	char name[MAX_PATH];
	info_t info;
} my_entry_t;

#pragma pack ()

///////////////////ȫ�ֱ���//////////////////////////////////////////////////////////


char* Name = NULL;
DWORD DataOffset = 0;

typedef map<string, info_t> StringMap;
StringMap MyNameDic;
///////////////���ù���//////////////////////////////////////////////////////////////
void SetNopCode(BYTE* pnop, size_t size)
{
	DWORD oldProtect;
	VirtualProtect((PVOID)pnop, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	for (size_t i = 0; i<size; i++)
	{
		pnop[i] = 0x90;
	}
}

void memcopy(void* dest, void*src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}

//////////////////HookCreateFontIndirect/////////////////////////////////////////////

typedef HFONT(WINAPI* Func_CreateFontIndirectA)(LOGFONTA *lplf);
PVOID p_CreateFontIndirectA = CreateFontIndirectA;
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA *lplf)
{
	LOGFONTA lf;
	memcpy(&lf, lplf, sizeof(LOGFONTA));
	strcpy(lf.lfFaceName, "΢���ź�");
	lf.lfCharSet = GB2312_CHARSET;
	return (Func_CreateFontIndirectA(p_CreateFontIndirectA))(&lf);
}
/////////////����//////////////////////////////////////////////////////////////
/*
void InstallBorderPatch()
{
	//�߽���
	BYTE Patch1[] = { 0xFE };
	BYTE Patch2[] = { 0xFE };

	//һ����4���߽���
	memcopy((void*)0x429373, Patch1, sizeof(Patch1));
	memcopy((void*)0X42937D, Patch2, sizeof(Patch2));

	memcopy((void*)0x43A04E, Patch1, sizeof(Patch1));
	memcopy((void*)0x43A058, Patch2, sizeof(Patch2));

	memcopy((void*)0x43A223, Patch1, sizeof(Patch1));
	memcopy((void*)0x43A22B, Patch2, sizeof(Patch2));

	memcopy((void*)0x43B1AF, Patch1, sizeof(Patch1));
	memcopy((void*)0x43B1B9, Patch2, sizeof(Patch2));

}
*/
///////////////Copy�Լ����ļ����ڴ�//////////////////////////////////////////////////////

PVOID pGetName = (PVOID)0x47387E;
__declspec(naked)void GetName()
{
	__asm
	{
		pushad
			mov Name, edi
			popad
			jmp pGetName
	}
}

PVOID pDataOffset = (PVOID)0x4734AD;
__declspec(naked)void GetDataOffset()
{
	__asm
	{
		pushad
			mov DataOffset, esi
			popad
			jmp pDataOffset
	}
}



void FillMyNameDic()
{
	HANDLE hfile = CreateFileA("pack.lib", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hfile)
	{
		DWORD size = GetFileSize(hfile, NULL);
		BYTE *buff = new BYTE[size];
		DWORD len = 0;
		if (!ReadFile(hfile, (LPVOID)buff, size, &len, NULL))
		{
			MessageBoxW(NULL, L"��ȡpack.libʧ�ܣ�", L"SKP-SYSTEM", MB_OK | MB_ICONWARNING);
			CloseHandle(hfile);
		}
		CloseHandle(hfile);


		header_t *header = (header_t*)buff;
		entry_t *entry = (entry_t*)(buff + header->index_offset);

		DWORD entry_count = header->index_length / sizeof(entry_t);

		my_entry_t * my_entry = new my_entry_t[entry_count];


		int i = 0;
		for (i = 0; i < entry_count; i++)
		{
			memcpy(my_entry[i].name, buff + entry[i].name_offset, entry[i].name_length - 1);

			my_entry[i].info.offset = entry[i].offset;
			my_entry[i].info.length = entry[i].length;
			my_entry[i].info.org_length = entry[i].org_length;

			MyNameDic.insert(StringMap::value_type(my_entry[i].name, my_entry[i].info));
		}

		delete[]buff;
		delete[]my_entry;
	}
	else
	{
		MessageBoxW(NULL, L"��ȡpack.libʧ�ܣ�", L"SKP-SYSTEM", MB_OK | MB_ICONWARNING);
	}
}




void WINAPI CopyMyFile(DWORD offset)
{
	if (Name != NULL)
	{

		//MessageBoxA(NULL, Name, "Test", MB_OK);

		string name(Name, strlen(Name));

		//�����ǰ�ļ������ֵ���:
		if (MyNameDic.count(name))
		{
			StringMap::iterator iter;
			iter = MyNameDic.find(name);
			info_t info = iter->second;

			DWORD off = info.offset;
			DWORD len = info.length;
			DWORD orglen = info.org_length;

			HANDLE hfile = CreateFileA("pack.lib", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
			SetFilePointer(hfile, off, NULL, FILE_BEGIN);
			BYTE *buff = new BYTE[len];
			DWORD readsize = 0;
			if (!ReadFile(hfile, (LPVOID)buff, len, &readsize, NULL))
			{
				MessageBoxA(NULL, Name, "Error", MB_OK);
				CloseHandle(hfile);
			}
			BYTE *data = new BYTE[orglen];

			//��ʱ���ж��Ƿ�ѹ����Ĭ�ϲ���ѹ�����
			if (uncompress(data, &orglen, buff, len) == Z_OK)
			{
				memcpy((LPVOID)offset, data, orglen);
			}
			else
			{
				MessageBoxW(NULL, L"��ȡpack.libʧ�ܣ�", L"SKP-SYSTEM", MB_OK | MB_ICONWARNING);
			}

			delete[]data;
			delete[]buff;
			CloseHandle(hfile);

		}
	}

}

PVOID pCopyMyFile = (PVOID)0x4754FE;
__declspec(naked)void _CopyMyFile()
{
	__asm
	{
		pushad
			push DataOffset
			call CopyMyFile
			popad
			jmp pCopyMyFile
	}
}


//��װHook 
void SetHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&p_CreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach((void**)&pDataOffset, GetDataOffset);
	DetourAttach((void**)&pCopyMyFile, _CopyMyFile);
	DetourTransactionCommit();
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"��������ʧ�ܣ�������ԭ�����", L"SKP-SYSTEM", MB_OK | MB_ICONWARNING);
	}
	else
	{
		MessageBoxW(NULL, L"�������ɡ�����ѧԺ����������ֹһ��¼��ֱ������ҵ��Ϊ", L"SKP-SYSTEM", MB_OK | MB_ICONWARNING);
	}

	//InstallBorderPatch();

	//FillMyNameDic();

}

//ж��Hook
void DropHook()
{
	MyNameDic.clear();
}

__declspec(dllexport)void WINAPI Empty()
{

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		FillMyNameDic();
		SetHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DropHook();
		break;
	}
	return TRUE;
}

