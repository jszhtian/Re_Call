#include "framework.h"
#pragma pack (1)
typedef struct {
	char magic[8];			
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
char* Name = NULL;
DWORD DataOffset = 0;

typedef map<string, info_t> StringMap;
StringMap MyNameDic;
/*
void JMPFix()
{
	BYTE Patch1[] = { 0x85 };
	BYTE Patch2[] = { 0x7F };

	int jmpfix1 = BaseAddr + 0x6c9fc;
	int jmpfix2 = BaseAddr + 0x6ca10;


	memcopy((void*)jmpfix1, Patch1, sizeof(Patch1));
	memcopy((void*)jmpfix2, Patch2, sizeof(Patch2));
}

void JMPReplace()
{
	BYTE Patch1[] = { 0x84 };
	BYTE Patch2[] = { 0x77 };

	int jmpfix1 = BaseAddr + 0x6c9fc;
	int jmpfix2 = BaseAddr + 0x6ca10;


	memcopy((void*)jmpfix1, Patch1, sizeof(Patch1));
	memcopy((void*)jmpfix2, Patch2, sizeof(Patch2));
}

void PrintSup(char* input)
{
	cout << "The file name get is : "<<input << endl;
}
*/

static PVOID pGetName = (PVOID)(BaseAddr + 0x6d9b0);
static PVOID dGetName;// = (PVOID)(BaseAddr + 0x6c9a5);
__declspec(naked)void GetName()
{
	__asm
	{
		pushad
			pushfd
				mov Name, edi
			popfd
		popad
		jmp pGetName
	}
}
/*
static PVOID pDataOffset;// = (PVOID)(BaseAddr + 0x6ca18);
static PVOID dDataOffset;// = (PVOID)(BaseAddr + 0x6ca1D);
__declspec(naked)void GetDataOffset()
{
	__asm
	{
		pushad
		pushfd
			mov DataOffset, eax
		popfd
		popad
		add esp,0x4
		mov edi,eax
		jmp dDataOffset
	}
}
*/

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

void FillMyNameDic()
{
	HANDLE hfile = CreateFileA("amegure_cn.xb2", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hfile)
	{
		DWORD size = GetFileSize(hfile, NULL);
		BYTE* buff = new BYTE[size];
		DWORD len = 0;
		if (!ReadFile(hfile, (LPVOID)buff, size, &len, NULL))
		{
			MessageBoxA(NULL, "读取amegure_cn.xb2失败!", "Error", MB_OK);
			CloseHandle(hfile);
		}
		CloseHandle(hfile);


		header_t* header = (header_t*)buff;
		entry_t* entry = (entry_t*)(buff + header->index_offset);

		DWORD entry_count = header->index_length / sizeof(entry_t);

		my_entry_t* my_entry = new my_entry_t[entry_count];


		int i = 0;
		for (i = 0; i < entry_count; i++)
		{
			memcpy(my_entry[i].name, buff + entry[i].name_offset, entry[i].name_length - 1);

			my_entry[i].info.offset = entry[i].offset;
			my_entry[i].info.length = entry[i].length;
			my_entry[i].info.org_length = entry[i].org_length;
			if (debug)
			{
				cout << "ReadFile: " << my_entry[i].name << " | "<<strlen(my_entry[i].name)<<endl;
			}
			MyNameDic.insert(StringMap::value_type(my_entry[i].name, my_entry[i].info));
			cout << MyNameDic.count(my_entry[i].name) << endl;
		}
		delete[]buff;
		delete[]my_entry;
	}
	else
	{
		MessageBoxA(NULL, "读取amegure_cn.xb2失败!", "Error", MB_OK);
	}
}

void WINAPI CopyMyFile(DWORD offset)
{
	//cout << Name << endl;
	//cout << offset << endl;
	if (Name != NULL)
	{
		string name(Name, strlen(Name));
		if (debug)
		{
			cout << "CheckName: " << name << endl;
			cout << "Len：" << strlen(Name) << endl;
			cout << "NameDicCount: " << MyNameDic.count(name) << endl;
		}
		//如果当前文件名在字典中:
		if (MyNameDic.count(name))
		{
			StringMap::iterator iter;
			iter = MyNameDic.find(name);
			info_t info = iter->second;

			DWORD off = info.offset;
			DWORD len = info.length;
			DWORD orglen = info.org_length;

			HANDLE hfile = CreateFileA("amegure_cn.xb2", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
			SetFilePointer(hfile, off, NULL, FILE_BEGIN);
			BYTE* buff = new BYTE[len];
			DWORD readsize = 0;
			if (!ReadFile(hfile, (LPVOID)buff, len, &readsize, NULL))
			{
				MessageBoxA(NULL, Name, "Error", MB_OK);
				CloseHandle(hfile);
			}
			BYTE* data = new BYTE[orglen];
			if (uncompress(data, &orglen, buff, len) == Z_OK)
			{
				memcpy((LPVOID)offset, data, orglen);
			}
			else
			{
				MessageBoxA(NULL, "解压失败", "Error", MB_OK);
			}

			delete[]data;
			delete[]buff;
			CloseHandle(hfile);

		}
		else 
		{
			cout << "Can not read file: " << name << " Because this file isn't in xb2 file." << endl;
		}
	}

}
/*
static PVOID pCopyMyFile;// = (PVOID)(BaseAddr + 0x6ca2e);
static PVOID dCopyMyFile;// = (PVOID)(BaseAddr + 0x6ca35);
__declspec(naked)void _CopyMyFile()
{
	__asm
	{
		pushad
		pushfd
			push DataOffset
			call CopyMyFile
		popfd
		popad
		mov ecx, dword ptr ss : [ebp + 0x14]
		push ecx
		mov edx, dword ptr ss : [ebp + 0x10]
		jmp dCopyMyFile
	}
}
*/
static PVOID LoadFileBuffer_HookAddr = (PVOID)(BaseAddr + 0x6d750);
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
		jmp LoadFileBuffer_HookAddr
	}
}

void FileCopy()
{
	/*
	*(DWORD*)& pGetName = BaseAddr + 0x6d9a0;
	*(DWORD*)& dGetName = BaseAddr + 0x6d9a5;
	*(DWORD*)& pDataOffset = BaseAddr + 0x6da18;
	*(DWORD*)& dDataOffset = BaseAddr + 0x6da1D;
	*(DWORD*)& pCopyMyFile = BaseAddr + 0x6da2e;
	*(DWORD*)& dCopyMyFile = BaseAddr + 0x6da35;
	if (debug)
	{
		cout << hex << pGetName << endl;
		cout << hex << dGetName << endl;
		cout << hex << pDataOffset << endl;
		cout << hex << dDataOffset << endl;
		cout << hex << pCopyMyFile << endl;
		cout << hex << dCopyMyFile << endl;
	}
	JMPHelper(pGetName, GetName);
	JMPHelper(pDataOffset, GetDataOffset);
	JMPHelper(pCopyMyFile, _CopyMyFile);
	/*
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)& pGetName, GetName);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Read file error.\nCan not find file.", L"FileSystem", MB_OK | MB_ICONERROR);
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)& pDataOffset, GetDataOffset);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Read file error.\nOffset is wrong.", L"FileSystem", MB_OK | MB_ICONERROR);
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)& pCopyMyFile, _CopyMyFile);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Read file error.\nCopy error.", L"FileSystem", MB_OK | MB_ICONERROR);
	}
	MessageBoxA(NULL, "loaded", "Test", MB_OK);
	*/
	FillMyNameDic();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)& pGetName, GetName);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Read file error.\nCan not find file.", L"FileSystem", MB_OK | MB_ICONERROR);
	}
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)& LoadFileBuffer_HookAddr, LoadFileBuffer);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"Read file error.", L"FileSystem", MB_OK | MB_ICONERROR);
	}
}

void FileClose()
{
	MyNameDic.clear();
}