// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> 
#include "shlwapi.h"
#include <unordered_map>
//#include <map>
#include <intrin.h>
#include <assert.h>
#include <stdint.h>

#include <vector>
#include <shlobj.h>

#include "detours.h"

#include "FileReader.h"
#include "NakedMemory.h"

#include "crc32key.cpp"

#pragma comment(lib, "Shlwapi.lib")
using namespace std;

HMODULE Base = GetModuleHandle(NULL);
DWORD BaseAddr = (DWORD)Base;

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "Open Console Success!" << std::endl;
}

static BOOL IATPatch(LPCSTR szDllName, PROC pfnOrg, PROC pfnNew)
{
	HMODULE hmod;
	LPCSTR szLibName;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk;
	DWORD dwOldProtect, dwRVA;
	PBYTE pAddr;

	hmod = GetModuleHandleW(NULL);
	pAddr = (PBYTE)hmod;
	pAddr += *((DWORD*)&pAddr[0x3C]);
	dwRVA = *((DWORD*)&pAddr[0x80]);

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hmod + dwRVA);

	for (; pImportDesc->Name; pImportDesc++)
	{
		szLibName = (LPCSTR)((DWORD)hmod + pImportDesc->Name);
		if (!stricmp(szLibName, szDllName))
		{
			pThunk = (PIMAGE_THUNK_DATA)((DWORD)hmod + pImportDesc->FirstThunk);
			for (; pThunk->u1.Function; pThunk++)
			{
				if (pThunk->u1.Function == (DWORD)pfnOrg)
				{
					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
					pThunk->u1.Function = (DWORD)pfnNew;
					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, dwOldProtect, &dwOldProtect);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

template<class T>
static BOOL Hook(T& OriginalFunction, T DetourFunction)
{
	DetourUpdateThread(GetCurrentThread());
	DetourTransactionBegin();
	DetourAttach(&(PVOID&)OriginalFunction, (PVOID&)DetourFunction);
	if (DetourTransactionCommit() != NOERROR)
		return FALSE;
	return TRUE;
}


template<class T>
static BOOL UnHook(T& OriginalFunction, T DetourFunction)
{
	DetourUpdateThread(GetCurrentThread());
	DetourTransactionBegin();
	DetourDetach(&(PVOID&)OriginalFunction, (PVOID&)DetourFunction);
	if (DetourTransactionCommit() != NOERROR)
		return FALSE;
	return TRUE;
}

static void ErrorToFileW(PVOID buffer, DWORD size)
{
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, "LogErrorW.txt", "ab+");
	if (!fp)
	{
		MessageBoxW(0, L"Create error file error.", L"ErrorToFileW", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}
	fwrite(buffer, size, 1, fp);
	fwrite(L"\r\n", 4, 1, fp);
	fclose(fp);
}

static void Err(const wchar_t* format, ...)
{
	wchar_t buffer[0x1000];
	va_list ap;
	va_start(ap, format);
	auto size = vswprintf_s(buffer, format, ap);
	std::wcout << buffer << std::endl;
	MessageBoxW(0, buffer, L"Error", MB_OK | MB_ICONERROR);
	ErrorToFileW(buffer, size * 2);
	va_end(ap);
}

static char* UniodeToAnsi(LPCTSTR str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static LPWSTR AnsiToUnicode(char* str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(cp, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(cp, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static void BMPToDIB(PBYTE data, int width, int height, int BitCount)
{
	BYTE* TempBuffer = nullptr;
	int i = 0;
	int widthlen = 0;
	int nAlignWidth = 0;
	size_t BufferSize = 0;
	nAlignWidth = (width * 32 + 31) / 32;
	BufferSize = 4 * nAlignWidth * height;
	TempBuffer = (BYTE*)malloc(BufferSize);

	//反转图片,修正图片信息
	widthlen = width * (BitCount / 8); //对齐宽度大小
	for (i = 0; i < height; i++) {
		memcpy(&TempBuffer[(((height - i) - 1) * widthlen)], &data[widthlen * i], widthlen);
	}

	memcpy(data, TempBuffer, BufferSize);

	free(TempBuffer);
}

static string ReplaceString(string& line)
{
	string out;
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == '\\')
		{
			i++;
			if (line[i] == 'r')
				out += '\r';
			else if (line[i] == 'n')
				out += '\n';
			else
				out += line[i];
		}
		else
			out += line[i];
	}
	return out;
}

BOOL IsStr(const CHAR* str){
	for (int i = 0; i < strlen(str);i++) {
		if (str[i] < 0x20)
			return FALSE;
	}
	return TRUE;
}

BOOL WriteMemory(PVOID lpAddress, PVOID lpBuffer, DWORD nSize)
{
	DWORD dwProtect;

	if (VirtualProtect(lpAddress, nSize, PAGE_EXECUTE_READWRITE, &dwProtect))
	{
		memcpy(lpAddress, lpBuffer, nSize);
		VirtualProtect(lpAddress, nSize, dwProtect, &dwProtect);
		return TRUE;
	}

	return FALSE;
}


class Mjo
{
private:
	struct MjoIAT
	{
		uint32_t hash;
		uint32_t offset;
	};

	struct JmpTbl
	{
		uint8_t* offset;
	};

	struct MultiJmpTbl
	{
		uint32_t tbl_cnt;
		uint8_t* offset;
	};

	struct StreamPtr
	{
		uint8_t* ptr;
		uint32_t ori_size;
	};

	struct MjoInfo
	{
		char magic[16];
		uint32_t entry_point;
		uint32_t unk;
		uint32_t iat_cnt;
		uint32_t code_size;
	};

	static const int OpTable[80];

	enum MajiroOps
	{
		OpPushStr = 0x801,
		OpLineNumber = 0x83a,
		OpCatString = 0x840,
		OpProcessString = 0x841,
		OpCtrlString = 0x842,
		OpJumpTable = 0x850,
	};

public:
	Mjo() :info_({ 0 }) {}

	bool read(const NakedMemory& mem);

	bool pack(const std::vector<std::string>& lines);

	NakedMemory write();

	std::vector<std::string>& get_lines()
	{
		return lines_;
	}

private:
	void xor_dec(void* block, uint32_t size);
	bool read_to_mem(const NakedMemory& mem);
	bool parse_code();

	bool pack_line(const std::string& line, uint32_t idx);
	NakedMemory gen_new_inst(const std::string& line);
	void insert_block(uint8_t* cur, uint32_t cur_size, uint8_t* newp, uint32_t new_size, uint8_t* cur_end);
	void fix_offsets(uint32_t cur_idx, uint8_t* start, int32_t dist);

private:
	MjoInfo info_;
	std::vector<MjoIAT> iat_;
	NakedMemory code_;

	std::vector<JmpTbl> jmp_tbl_;
	std::vector<MultiJmpTbl> mjmp_tbl_;

	std::vector<std::string> lines_;
	std::vector<StreamPtr> ptrs_;

	NakedMemory temp_;
};

vector<string> SplitTxtA(NakedMemory& txt)
{
	auto buff = (uint8_t*)txt.Get();
	uint32_t i = 0;
	uint32_t last = 0;
	auto ls = vector<string>();
	while (true)
	{
		while (i < txt.GetSize() && buff[i] != '\r') i++;
		ls.push_back(string((char*)buff + last, i - last));
		if (i + 1 >= txt.GetSize()) break;
		i += 2;
		last = i;
	}
	return ls;
}

NakedMemory CvtToAnsi(NakedMemory& str)
{
	const auto codepage = CP_ACP;
	auto buff = (uint8_t*)str.Get();
	if (*(WORD*)buff == 0xfeff)
	{
		auto newsize = WideCharToMultiByte(codepage, 0, (wchar_t*)(buff + 2), (str.GetSize() - 2) / 2, 0, 0, 0, 0);
		NakedMemory ansi(newsize);
		WideCharToMultiByte(codepage, 0, (wchar_t*)(buff + 2), (str.GetSize() - 2) / 2, (char*)ansi.Get(), ansi.GetSize(), 0, 0);
		return std::move(ansi);
	}
	else
	{
		return str;
	}
}