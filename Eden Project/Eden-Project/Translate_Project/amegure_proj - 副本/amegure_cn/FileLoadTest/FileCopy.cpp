#include "framework.h"

char* Name = NULL;
DWORD DataOffset = 0;

typedef map<string, info_t> StringMap;
StringMap MyNameDic;

/*
0089D9A0      55            push ebp                                 ;  EDI IS NAME
0089D9A1      8BEC          mov ebp,esp
0089D9A3      6A FF         push -0x1
0089D9A5   .  68 88299100   push amegure.00912988
0089D9AA   .  64:A1 0000000>mov eax,dword ptr fs:[0]
0089D9B0   .  50            push eax
0089D9B1   .  83EC 14       sub esp,0x14
0089D9B4   .  53            push ebx
0089D9B5   .  56            push esi                                 ;  amegure.<ModuleEntryPoint>
0089D9B6   .  57            push edi                                 ;  amegure.<ModuleEntryPoint>
0089D9B7   .  A1 34479300   mov eax,dword ptr ds:[0x934734]
0089D9BC   .  33C5          xor eax,ebp
0089D9BE   .  50            push eax
0089D9BF   .  8D45 F4       lea eax,dword ptr ss:[ebp-0xC]
0089D9C2   .  64:A3 0000000>mov dword ptr fs:[0],eax
0089D9C8   .  8965 F0       mov dword ptr ss:[ebp-0x10],esp
0089D9CB   .  8BF1          mov esi,ecx                              ;  amegure.<ModuleEntryPoint>
0089D9CD   .  56            push esi                                 ;  amegure.<ModuleEntryPoint>
0089D9CE   .  E8 0DF6FFFF   call amegure.0089CFE0
0089D9D3   .  83C4 04       add esp,0x4
0089D9D6   .  85C0          test eax,eax
0089D9D8   .  0F84 E9000000 je amegure.0089DAC7
0089D9DE   .  33DB          xor ebx,ebx
0089D9E0   .  895D FC       mov dword ptr ss:[ebp-0x4],ebx
0089D9E3   .  8D45 E0       lea eax,dword ptr ss:[ebp-0x20]
0089D9E6   .  E8 3559FCFF   call amegure.00863320
0089D9EB   .  C645 FC 01    mov byte ptr ss:[ebp-0x4],0x1
0089D9EF   .  8BC6          mov eax,esi                              ;  amegure.<ModuleEntryPoint>
0089D9F1   .  8D75 E0       lea esi,dword ptr ss:[ebp-0x20]
0089D9F4   .  E8 975AFCFF   call amegure.00863490
0089D9F9   .  85C0          test eax,eax
0089D9FB   .  0F84 89000000 je amegure.0089DA8A
0089DA01   .  8BC6          mov eax,esi                              ;  amegure.<ModuleEntryPoint>
0089DA03   .  E8 A85BFCFF   call amegure.008635B0
0089DA08   .  8BF0          mov esi,eax
0089DA0A   .  81FE 00000004 cmp esi,0x4000000
0089DA10   .  77 67         ja short amegure.0089DA79
0089DA12   .  56            push esi                                 ;  amegure.<ModuleEntryPoint>
0089DA13   .  E8 806B0400   call amegure.008E4598
0089DA18   .  83C4 04       add esp,0x4
0089DA1B   .  8BF8          mov edi,eax
0089DA1D   .  8D45 E0       lea eax,dword ptr ss:[ebp-0x20]
0089DA20   .  50            push eax
0089DA21   .  8BCE          mov ecx,esi                              ;  amegure.<ModuleEntryPoint>
0089DA23   .  8BD7          mov edx,edi                              ;  amegure.<ModuleEntryPoint>
0089DA25   .  E8 065BFCFF   call amegure.00863530                    ;  EDX IS OFFSET
0089DA2A   .  3BC6          cmp eax,esi                              ;  EDI IS BUFFER

call amegure.00863530:
00863530      55            push ebp
00863531      8BEC          mov ebp,esp
00863533      6A 00         push 0x0
00863535  |.  8D45 08       lea eax,[arg.1]                          ; |
00863538  |.  50            push eax                                 ; |pBytesRead = 00F3F99C
00863539  |.  8B45 08       mov eax,[arg.1]                          ; |
0086353C  |.  51            push ecx                                 ; |BytesToRead = 8E61BE (9331134.)
0086353D  |.  8B48 04       mov ecx,dword ptr ds:[eax+0x4]           ; |ntdll.77169F80
00863540      52            push edx                                 ;  amegure.<ModuleEntryPoint>
00863541      51            push ecx                                 ;  amegure.<ModuleEntryPoint>
00863542      FF15 C8429100 call dword ptr ds:[<&KERNEL32.ReadFile>] ;  kernel32.ReadFile
00863548      85C0          test eax,eax
0086354A      75 04         jnz short amegure.00863550
0086354C      5D            pop ebp                                  ;  kernel32.75CB6359
0086354D  |.  C2 0400       retn 0x4
00863550      8B45 08       mov eax,dword ptr ss:[ebp+0x8]
00863553      5D            pop ebp                                  ;  kernel32.75CB6359
00863554      C2 0400       retn 0x4


*/

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

static PVOID pGetName;// = (PVOID)(BaseAddr + 0x6c9a0);
static PVOID dGetName;// = (PVOID)(BaseAddr + 0x6c9a5);
__declspec(naked)void GetName()
{
	__asm
	{
		pushad
			mov Name, edi
			push Name
			call PrintSup
		popad
		push ebp
		mov ebp,esp
		push - 0x1
		jmp dGetName
	}
}

/*
00863530      55            push ebp
00863531      8BEC          mov ebp,esp
00863533      6A 00         push 0x0
*/


static PVOID pDataOffset;// = (PVOID)(BaseAddr + 0x6ca18);
static PVOID dDataOffset;// = (PVOID)(BaseAddr + 0x6ca1D);
__declspec(naked)void GetDataOffset()
{
	__asm
	{
		pushad
			mov DataOffset, eax
		popad
		add esp,0x4
		mov edi,eax
		jmp dDataOffset
	}
}

void FillMyNameDic()
{
	HANDLE hfile = CreateFileA("amegure_cn.rio", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hfile)
	{
		DWORD size = GetFileSize(hfile, NULL);
		BYTE* buff = new BYTE[size];
		DWORD len = 0;
		if (!ReadFile(hfile, (LPVOID)buff, size, &len, NULL))
		{
			MessageBoxW(NULL, L"Read file error.\nCan not read amegure_cn.rio", L"FileSystem", MB_OK | MB_ICONERROR);
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
			cout << "rio file name: "<<my_entry[i].name << endl;
			MyNameDic.insert(StringMap::value_type(my_entry[i].name, my_entry[i].info));
		}

		delete[]buff;
		delete[]my_entry;
	}
	else
	{
		MessageBoxW(NULL, L"Read file error.\nCan not find amegure_cn.rio", L"FileSystem", MB_OK | MB_ICONERROR);
	}
}




void WINAPI CopyMyFile(DWORD offset)
{
	if (Name != NULL)
	{

		//MessageBoxA(NULL, Name, "Test", MB_OK);

		string name(Name, strlen(Name));

		//如果当前文件名在字典中:
		if (MyNameDic.count(name))
		{
			//修正判断
			//JMPFix();
			MessageBoxA(NULL, Name, "Test", MB_OK);
			StringMap::iterator iter;
			iter = MyNameDic.find(name);
			info_t info = iter->second;

			DWORD off = info.offset;
			DWORD len = info.length;
			DWORD orglen = info.org_length;

			HANDLE hfile = CreateFileA("amegure_cn.rio", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
			SetFilePointer(hfile, off, NULL, FILE_BEGIN);
			BYTE* buff = new BYTE[len];
			DWORD readsize = 0;
			if (!ReadFile(hfile, (LPVOID)buff, len, &readsize, NULL))
			{
				MessageBoxW(NULL, L"Read file error.\nRead error.", L"FileSystem", MB_OK | MB_ICONERROR);
				CloseHandle(hfile);
			}
			BYTE* data = new BYTE[orglen];

			//暂时不判断是否压缩，默认采用压缩封包
			if (uncompress(data, &orglen, buff, len) == Z_OK)
			{
				memcpy((LPVOID)offset, data, orglen);
			}
			else
			{
				MessageBoxW(NULL, L"Read file error.\nUncompress faild.", L"FileSystem", MB_OK | MB_ICONERROR);
			}

			delete[]data;
			delete[]buff;
			CloseHandle(hfile);

		}
	}

}
/*
00863550      8B45 08       mov eax,dword ptr ss:[ebp+0x8]
00863553      5D            pop ebp                                  ;  amegure.0089DA2A
00863554      C2 0400       retn 0x4
*/
static PVOID pCopyMyFile;// = (PVOID)(BaseAddr + 0x6ca2e);
static PVOID dCopyMyFile;// = (PVOID)(BaseAddr + 0x6ca35);
__declspec(naked)void _CopyMyFile()
{
	__asm
	{
		mov ecx, dword ptr ss : [ebp + 0x14]
		push ecx
		mov edx, dword ptr ss : [ebp + 0x10]
		pushad
			push DataOffset
			call CopyMyFile
		popad
		jmp dCopyMyFile
	}
}

void FileCopy()
{
	*(DWORD*)& pGetName = BaseAddr + 0x6c9a0;
	*(DWORD*)& dGetName = BaseAddr + 0x6c9a5;
	*(DWORD*)& pDataOffset = BaseAddr + 0x6ca18;
	*(DWORD*)& dDataOffset = BaseAddr + 0x6ca1D;
	*(DWORD*)& pCopyMyFile = BaseAddr + 0x6ca2e;
	*(DWORD*)& dCopyMyFile = BaseAddr + 0x6ca35;
	cout <<hex<< pGetName << endl;
	cout << hex << dGetName << endl;
	cout << hex << pDataOffset << endl;
	cout << hex << dDataOffset << endl;
	cout << hex << pCopyMyFile << endl;
	cout << hex << dCopyMyFile << endl;
	JMPHelper(pGetName, GetName);
	JMPHelper(pDataOffset, GetDataOffset);
	JMPHelper(pCopyMyFile, _CopyMyFile);
	FillMyNameDic();
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
}

void FileClose()
{
	MyNameDic.clear();
}