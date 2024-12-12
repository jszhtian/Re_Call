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
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string>

#include "detours.h"

#include "cryptopp\sha.h"
#include "cryptopp\gcm.h"
#include "cryptopp\eax.h"
#include "cryptopp\camellia.h"
#include "cryptopp\zlib.h"
#include "cryptopp\hex.h"
#include "cryptopp\osrng.h"

#include "crc32.h"

#include "Key.h"
using namespace CryptoPP;
using namespace std;

#define PACK_NAME "game_cn.cpk"
#define KEY_NAME  "game_cn.key"
#define HEADER_KEY "一青二白汉化组"

#define MAGIC "CPK\0"
#define VERSION "\x02\x00\x00\x00"
#define WARNING "本补丁由【一青二白汉化组】制作，禁止拆解破解本补丁。"

#define KEY_TOP_KEY 0x23361458

static unordered_map<UINT, string> TextList;

struct Registers
{
	DWORD eflags;
	DWORD edi;
	DWORD esi;
	DWORD ebp;
	DWORD esp;
	DWORD ebx;
	DWORD edx;
	DWORD ecx;
	DWORD eax;
};

#pragma pack(1)
struct AYMHeader
{
	CHAR Magic[4]; // "AYM\0"
	UINT Version;
	UINT StrCount;
};
struct AYMIndex
{
	UINT off;
	UINT jmp_off;
};
struct CompressInfo
{
	UINT UncompressSize;
	UINT CompressSize;
};
#pragma pack()

static HMODULE Base = GetModuleHandle(NULL);
static DWORD BaseAddr = (DWORD)Base;

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "Open Console Success!" << std::endl;
}

static void FileNameToLower(std::wstring& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
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

static char* wtoc(LPCTSTR str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static LPWSTR ctow(char* str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(cp, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(cp, 0, str, -1, out, dwMinSize);//转换
	return out;
}

class CPKFileSystem
{
public:
	CPKFileSystem();
	~CPKFileSystem();
	BOOL InitFileSystem();
	BOOL UnInitFileSystem();
	BOOL GetFileByName(WCHAR* FileName, BYTE** FileBuff, DWORD* FileSize);
private:

#pragma pack (1)
	// CPKFile
	struct CPKHeader {
		char Magic[4];
		char Version[4];
		char Warning[128];
		DWORD IndexCount;
	};
	struct CPKIndex {
		WCHAR FileName[64];
		DWORD FileSize;
		DWORD CompressFileSize;
		DWORD Offset;
	};
	// KeyFile
	struct KeyHeader {
		char Magic[4]; // "CPK\0"
		DWORD IndexCount;
		char HeaderHash[SHA256::DIGESTSIZE];
	};
	struct CamelliaKey {
		DWORD KeySize;
		DWORD IvSize;
		BYTE Key[Camellia::DEFAULT_KEYLENGTH];
		BYTE Iv[Camellia::BLOCKSIZE];
	};
	struct KeyIndex {
		char IndexHash[SHA256::DIGESTSIZE];
		CamelliaKey KeyAfterCompress;
	};
#pragma pack ()

	FILE* cpk;
	FILE* key;

	BYTE* KeyHeaderBuff;
	KeyHeader* Key_Header;
	BYTE* CPKHeaderBuff;
	CPKHeader* CPK_Header;

	CPKIndex* CPK_Index;
	KeyIndex* Key_Index;

	BOOL E(const char* msg);
};