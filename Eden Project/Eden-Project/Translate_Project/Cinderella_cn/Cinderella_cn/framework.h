// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <algorithm> 
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <tchar.h>
#include <iomanip>
#include <mutex>
#include <stdio.h>
#include <map>
#include <string>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <sstream>
#include <vector>
#include <tlhelp32.h>
#include "detours.h"
#include "compact_enc_det/compact_enc_det.h"
static DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
using namespace std;
static PVOID JMPHookPoint;
static PVOID JMPHookPointNext;
static PVOID JMPHookPoint2;
static PVOID JMPHookPointNext2;
static PVOID TmpJmp;
static DWORD BufferAddr;
static std::mutex mtx;
static char* EdnFileBuffer;
static bool debug = true;


static void make_console()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

static bool JMPHelper(PVOID HookPoint, PVOID JMP_FUNC)
{
	DWORD oldProtect = 0;
	if (VirtualProtect(HookPoint, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
		return false;
	DWORD offset = (DWORD)JMP_FUNC - (DWORD)HookPoint - 5;
	char JMPOp[] = { 0xE9 };
	memcpy(HookPoint, &JMPOp, 1);
	memcpy((char*)HookPoint + 1, &offset, sizeof(DWORD));
	if (VirtualProtect(HookPoint, 5, oldProtect, &oldProtect))
		return false;
	return true;
}

static void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}

static bool ReplaceJump(PBYTE src, PBYTE dst)
{
	DWORD oldProtect;

	if (VirtualProtect((LPVOID)src, 10, PAGE_EXECUTE_READWRITE, &oldProtect)) {

		src[0] = 0xE9;
		*(DWORD*)&src[1] = (DWORD)(dst - src - 5);
		return true;
	}
	return false;
}

static void SetNopCode(int pnop, int size)
{
	BYTE Patch1[] = { 0xFE };
	for (int i = 0; i < size; i++)
	{
		memcopy((void*)(pnop + i), Patch1, sizeof(Patch1));
	}
}

static void MEM_SetNopCode(BYTE* lpNop, SIZE_T dwSize)
{
	DWORD op;

	SIZE_T i;
	PBYTE adr;
	BYTE nix = 0x90;
	VirtualProtect(lpNop, dwSize, PAGE_EXECUTE_READWRITE, &op);

	for (i = 0; i < dwSize; i++)
	{
		adr = &lpNop[i];
		WriteProcessMemory(GetCurrentProcess(), adr, &nix, sizeof(BYTE), NULL);
	}
}


static wstring WINAPI GetPackageNameW(wstring& fileName)
{
	wstring temp(fileName);
	wstring::size_type pos = temp.find_last_of(L"\\");

	if (pos != wstring::npos)
	{
		temp = temp.substr(pos + 1, temp.length());
	}

	wstring temp2(temp);
	wstring::size_type pos2 = temp2.find_last_of(L"/");
	if (pos2 != wstring::npos)
	{
		temp2 = temp2.substr(pos + 1, temp2.length());
	}
	return temp2;
}

static string WINAPI GetPackageNameA(string& fileName)
{
	string temp(fileName);
	string::size_type pos = temp.find_last_of("\\");

	if (pos != string::npos)
	{
		temp = temp.substr(pos + 1, temp.length());
	}

	string temp2(temp);
	string::size_type pos2 = temp2.find_last_of("/");
	if (pos2 != wstring::npos)
	{
		temp2 = temp2.substr(pos + 1, temp2.length());
	}
	return temp2;
}

static void FileNameToLower(std::string& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

#pragma comment(lib, "..//3rd//cryptlib_Release.lib") 

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <fstream>

#include "..\3rd\cryptopp\sha.h"
#include "..\3rd\cryptopp\gcm.h"
#include "..\3rd\cryptopp\eax.h"
#include "..\3rd\cryptopp\camellia.h"
#include "..\3rd\cryptopp\hc256.h"
#include "..\3rd\cryptopp\zlib.h"
#include "..\3rd\cryptopp\hex.h"
#include "..\3rd\cryptopp\osrng.h"
#include "..\3rd\cryptopp\files.h"
using namespace CryptoPP;
using namespace std;

#define PACK_NAME "imaimo_cn.cpk"
#define KEY_NAME  "imaimo_cn.key"
#define TBL_NAME  "imaimo_cn.tbl"
#define HEADER_KEY "绿茶汉化组"

#define MAGIC "CPK\0"
#define VERSION 0x02000005
#define WARNING "本补丁由【绿茶汉化组】制作，禁止拆解破解本补丁。"

#define KEY_TOP_KEY 0x21212212

class CPKFileSystem
{
public:
    CPKFileSystem();
    ~CPKFileSystem();
    BOOL InitFileSystem();
    BOOL UnInitFileSystem();
#ifdef TEST
    void Test(string fnm);
#else
    BOOL GetFileByName(char* FileName, BYTE** FileBuff, DWORD* FileSize);
#endif
private:

#pragma pack (1)
    // CPKFile
    struct CPKHeader {
        char Magic[4];
        char Warning[128];
        DWORD Version;
        //DWORD IndexCount;
    };
    struct CPKIndex {
        DWORD Offset;
        DWORD FileSize;
        DWORD CompressFileSize;
    };
    // KeyFile
    struct KeyHeader {
        char Magic[4]; // "CPK\0"
        char HeaderHash[SHA256::DIGESTSIZE];
    };
    struct CamelliaKey {
        BYTE Key[Camellia::DEFAULT_KEYLENGTH];
        BYTE Iv[Camellia::BLOCKSIZE];
    };
    struct FileNameStruct {
        DWORD Len;
        DWORD Offset;
    };
    struct KeyIndex {
        char IndexHash[SHA256::DIGESTSIZE];
        CamelliaKey KeyAfterCompress;
        //FileNameStruct FileName;
    };
    // TBL File
    struct TBLHeader {
        DWORD FileCount;
        CHAR Magic[4];
    };
    struct TBLIndex {
        char Hash[SHA256::DIGESTSIZE];
        DWORD ID;
    };
    // Pack Use
    struct IndexInfo {
        char IndexHash[SHA256::DIGESTSIZE];
        BYTE CPK[sizeof(CPKIndex)];
        CamelliaKey Key;
    };

#pragma pack ()

    unordered_map<DWORD, IndexInfo>IndexMap;

    FILE* cpk;
    FILE* key;
    FILE* tbl;

    IndexInfo Info;
    BYTE* KeyHeaderBuff;
    KeyHeader* Key_Header;
    BYTE* CPKHeaderBuff;
    CPKHeader* CPK_Header;
    TBLHeader* TBL_Header;

    CPKIndex* CPK_Index;
    KeyIndex* Key_Index;
    TBLIndex* TBL_Index;

    DWORD TblSize = 0;
    BYTE* TblBuff = nullptr;

    BYTE FileNameKeyTable[11] = {
    0xD1, 0x58, 0x8A, 0x56,
    0x2A, 0xFD, 0xA5, 0xF7,
    0x7C, 0x3E, 0xF4
    };

    BOOL E(const char* msg);
    void GenSHA256(byte* Str, DWORD StrLen, char** SHAContent);
    void Decrypt(BYTE* data, DWORD index, DWORD length, DWORD key);
    BYTE rol(int val, int n);
};