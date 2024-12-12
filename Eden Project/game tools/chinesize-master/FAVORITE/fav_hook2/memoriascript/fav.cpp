﻿#include <windows.h>

#include <ilhook.h>
#include "fav.h"

#define FILE_NAME_BEGIN 4860
#define FILE_NAME_END 4892
#define FILE_CNT (FILE_NAME_END-FILE_NAME_BEGIN+2)

StringTable StrTables[FILE_CNT];
typedef int (WINAPI *D3dCreateRoutine)(UINT);
D3dCreateRoutine CreateFunc;


int cmp1(const void* v1, const void* v2)
{
	DWORD i1 = *(DWORD*)v1;
	DWORD i2 = *(DWORD*)v2;
	if (i1 > i2)
		return 1;
	else if (i1 < i2)
		return -1;
	return 0;
}

char* FindTextByOffset(DWORD off)
{
	int i = 0;
	for (; i < FILE_CNT; i++)
	{
		if (StrTables[i].offsets &&
            off >= StrTables[i].offsets[0] && off <= StrTables[i].offsets[StrTables[i].cnt - 1])
		{
			break;
		}
	}
	if (i == FILE_CNT)
		return 0;
	StringTable* tbl = &StrTables[i];
	if (!tbl->offsets)
		return 0;
	DWORD* rslt = (DWORD*)bsearch(&off, tbl->offsets, tbl->cnt, 4, cmp1);
	if (!rslt)
		return 0;
	char* s = StrTables[i].stringList[rslt - StrTables[i].offsets];
	return s;
}

void my_process_string_internal(ScriptObject* obj, char** str)
{
	BYTE* ps = (BYTE*)*str;
	if (ps[-2] != 0xe || ps[ps[-1] - 1] != 0)
		return;

	char* ns = FindTextByOffset(ps - 1 - (BYTE*)obj->ScriptBuffer);
	if (ns)
		*str = ns;
}

void HOOKFUNC MyProcessString(Registers* regs)
{
	auto obj = (ScriptObject*)regs->esi;
	auto str = (char**)(regs->esp);
	my_process_string_internal(obj, str);
}

void HOOKFUNC MyProcessString2(Registers* regs)
{
	auto str = (char**)regs->esp;
	ScriptObject* obj = (ScriptObject*)(regs->ebx + 0xf1b58);
	my_process_string_internal(obj, str);
}

void HOOKFUNC MyCreateFontA(DWORD* charset)
{
	*charset = 0x86;
}
void HOOKFUNC MyEnumFontFamliesExA(LPLOGFONTA lpf)
{
	lpf->lfCharSet = 0x86;
}

void HOOKFUNC MyCreateWindowExA(char** title)
{
	//     if (*(DWORD*)*title == 0xeb82a282)
	//     {
	wchar_t* name = L"你朱眸里的五彩世界-WORLD'S END LOVE WILL LAST FOREVER.- Extra";
	static char mb[100];

	int ret = WideCharToMultiByte(CP_ACP, 0, name, -1, mb, 100, 0, 0);

	*title = mb;
	//    }
}

int HOOKFUNC MyLstrcmpiA(char* s1, char* s2)
{
	return CompareStringA(0x411, NORM_IGNORECASE, s1, -1, s2, -1) - 2;
}
//
//int GetTxtLines(char* str)
//{
//	int cnt=0;
//	char* p=str;
//	while(*p)
//	{
//		if(*p++==0xa)
//			cnt++;
//	}
//	if(p[-1]!=0xa)
//		cnt++;
//	return cnt;
//}

int SplitTxt(char* str, char** strCnt, int cnt)
{
	char* p = str;
	int i = 0;
	while (true)
	{
		if (i >= cnt)
			break;
		strCnt[i++] = p;
		while (*p && *p != 0xa)
		{
			p++;
		}
		if (!*p)
			break;
		*p = 0;
		if (p[-1] == 0xd)
			p[-1] = 0;
		p++;
	}
	return i;
}

char* LoadATxt(wchar_t* filename)
{
	HANDLE pheap = GetProcessHeap();
	HANDLE hf = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hf == INVALID_HANDLE_VALUE)
		return 0;
	DWORD fileLen = GetFileSize(hf, 0);
	auto oriStr = (char*)HeapAlloc(pheap, 0, fileLen + 3);
	if (!oriStr)
	{
		CloseHandle(hf);
		return 0;
	}
	DWORD bytesRead;
	ReadFile(hf, oriStr, fileLen, &bytesRead, 0);
	CloseHandle(hf);
	if (bytesRead != fileLen)
		return 0; //内存就不释放了，麻烦

	oriStr[fileLen] = 0;
	oriStr[fileLen + 1] = 0;
	oriStr[fileLen + 2] = 0;

	if (*(WORD*)oriStr == 0xfeff)
	{
		auto oriStr2 = (char*)HeapAlloc(pheap, 0, fileLen + 3);
		if (!oriStr2)
			return 0;
		int len = WideCharToMultiByte(936, 0, (wchar_t*)oriStr + 1, -1, oriStr2, fileLen + 3, 0, 0);
		if (len == 0)
			return 0;
		oriStr2[len] = 0;
		HeapFree(pheap, 0, oriStr);
		oriStr = oriStr2;
	}
	return oriStr;
}

DWORD* LoadAIdx(wchar_t* filename, int* cnt)
{
	HANDLE pheap = GetProcessHeap();
	HANDLE hf = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hf == INVALID_HANDLE_VALUE)
		return 0;
	DWORD fileLen = GetFileSize(hf, 0);

	auto idxbuff = (DWORD*)HeapAlloc(pheap, 0, fileLen);
	if (!idxbuff)
	{
		CloseHandle(hf);
		return 0;
	}
	DWORD bytesRead;
	ReadFile(hf, idxbuff, fileLen, &bytesRead, 0);
	CloseHandle(hf);
	if (bytesRead != fileLen)
		return 0;

	*cnt = bytesRead / 4;
	return idxbuff;
}

BOOL ReadAText(int idx, StringTable* tbl)
{
	wchar_t fname[40];
	wsprintf(fname, L"txt\\%04d.idx", idx);
	tbl->offsets = LoadAIdx(fname, &tbl->cnt);
	if (!tbl->offsets)
		return 0;

	wsprintf(fname, L"txt\\%04d.txt", idx);
	char* oriStr = LoadATxt(fname);
	if (!oriStr)
	{
		tbl->offsets = 0;
		return 0;
	}
	tbl->stringList = (char**)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, tbl->cnt * 4);
	if (!tbl->stringList)
		return 0;
	SplitTxt(oriStr, tbl->stringList, tbl->cnt);

	return TRUE;
}

BOOL PatchMemory(void* addr, void* content, int length)
{
	DWORD oldProt;
	if (!VirtualProtect(addr, length, PAGE_EXECUTE_READWRITE, &oldProt))
		return FALSE;
	memcpy(addr, content, length);
	return TRUE;
}

int InitializeStringTables()
{
	ReadAText(0, &StrTables[0]);
	for (int i = FILE_NAME_BEGIN; i <= FILE_NAME_END; i++)
	{
		ReadAText(i, &StrTables[i - FILE_NAME_BEGIN + 1]);
	}
	return 1;
}

//int WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
//{
//	if (dwReason == DLL_PROCESS_ATTACH)
//	{
//
//		auto stubMem = (BYTE*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
//		if (!stubMem)
//		{
//			MessageBox(0, L"似乎没内存了", 0, 0);
//			return FALSE;
//		}
//
//		HookSrcObject srcObj;
//		HookStubObject stubObj;
//
//		if (!InitializeHookSrcObject(&srcObj, (void*)0x00444B0D) ||
//			!InitializeStubObject(&stubObj, stubMem, 0x1000) ||
//			!Hook32(&srcObj, 0, &stubObj, MyProcessString, "y\x01"))
//		{
//			MessageBox(0, L"无法hook函数1", 0, 0);
//			return FALSE;
//		}
//		if (!InitializeHookSrcObject(&srcObj, (void*)0x00444B24) ||
//			!InitializeStubObject(&stubObj, stubMem + 100, 0x1000 - 100) ||
//			!Hook32(&srcObj, 0, &stubObj, MyProcessString, "y\x01"))
//		{
//			MessageBox(0, L"无法hook函数2", 0, 0);
//			return FALSE;
//		}
//		if (!InitializeHookSrcObject(&srcObj, (void*)0x00444BC6) ||
//			!InitializeStubObject(&stubObj, stubMem + 200, 0x1000 - 200) ||
//			!Hook32(&srcObj, 0, &stubObj, MyProcessString, "y\x01"))
//		{
//			MessageBox(0, L"无法hook函数3", 0, 0);
//			return FALSE;
//		}
//		if (!InitializeHookSrcObject(&srcObj, (void*)0x0043CB51) ||
//			!InitializeStubObject(&stubObj, stubMem + 300, 0x1000 - 300) ||
//			!Hook32(&srcObj, 0, &stubObj, MyProcessString2, "b\x01"))
//		{
//			MessageBox(0, L"无法hook函数4", 0, 0);
//			return FALSE;
//		}
//
//		hm = LoadLibrary(L"gdi32.dll");
//		LPVOID func = GetProcAddress(hm, "CreateFontA");
//		if (!InitializeHookSrcObject(&srcObj, func) ||
//			!InitializeStubObject(&stubObj, stubMem + 400, 0x1000 - 400) ||
//			!Hook32(&srcObj, 0, &stubObj, MyCreateFontA, "\x09"))
//		{
//			MessageBox(0, L"无法hook cf", 0, 0);
//			return FALSE;
//		}
//
//		func = GetProcAddress(hm, "EnumFontFamiliesExA");
//		if (!InitializeHookSrcObject(&srcObj, func) ||
//			!InitializeStubObject(&stubObj, stubMem + 500, 0x1000 - 500) ||
//			!Hook32(&srcObj, 0, &stubObj, MyEnumFontFamliesExA, "2"))
//		{
//			MessageBox(0, L"无法hook eff", 0, 0);
//			return FALSE;
//		}
//
//		hm = LoadLibrary(L"user32.dll");
//		func = GetProcAddress(hm, "CreateWindowExA");
//		if (!InitializeHookSrcObject(&srcObj, func) ||
//			!InitializeStubObject(&stubObj, stubMem + 600, 0x1000 - 600) ||
//			!Hook32(&srcObj, 0, &stubObj, MyCreateWindowExA, "\x03"))
//		{
//			MessageBox(0, L"无法hook cwe", 0, 0);
//			return FALSE;
//		}
//
//		hm = LoadLibrary(L"kernel32.dll");
//		func = GetProcAddress(hm, "lstrcmpiA");
//		if (!InitializeHookSrcObject(&srcObj, func) ||
//			!InitializeStubObject(&stubObj, stubMem + 700, 0x1000 - 700, 8, STUB_DIRECTLYRETURN | STUB_OVERRIDEEAX) ||
//			!Hook32(&srcObj, 0, &stubObj, MyLstrcmpiA, "12"))
//		{
//			MessageBox(0, L"无法hook lstrcmpi", 0, 0);
//			return FALSE;
//		}
//
//		PatchMemory((void*)0x0043281F, "\x33", 1);
//		PatchMemory((void*)0x00443B68, "\x33", 1);
//		char* fontName = "SimHei";
//		PatchMemory((void*)0x00440F88, &fontName, 4);
//	}
//	return TRUE;
//}
