#pragma once
#include "tp_stub.h"
/*V2Link*/
typedef HRESULT(__stdcall *PfuncV2Link)(iTVPFunctionExporter *);
/*CreateStream*/
typedef tTJSBinaryStream* (__fastcall *PfuncCreateStream)(const ttstr &, tjs_uint32);
/*HOOK MultiByteToWideChar*/
typedef int(WINAPI *PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);
/*HOOK LoadLibraryW*/
typedef HMODULE(WINAPI *PfuncLoadLibraryW)(
	_In_ LPCTSTR lpFileName
	);
class TVPHook
{
public:
	static TVPHook* handle;

	TVPHook();
	~TVPHook();
	bool init();
	bool uninit();
	void initKrkrHook(LPCWSTR lpFileName, HMODULE Module);
	IStream* CreateLocalStream(LPCWSTR lpFileName);
	PVOID g_POldV2Link;
	PVOID g_POldCreateStream;
	PVOID g_pOldMultiByteToWideChar;
	PVOID g_pOldLoadLibraryW;
	iTVPFunctionExporter* TVPFunctionExporter;
	bool inited;
};

TVPHook* GetobjTVPhook();



#pragma once
#pragma comment(lib, "..//3rd//cryptlib_Release.lib") 
#define NONE

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm> 

#include "..\3rd\cryptopp\sha.h"
#include "..\3rd\cryptopp\gcm.h"
#include "..\3rd\cryptopp\eax.h"
#include "..\3rd\cryptopp\camellia.h"
#include "..\3rd\cryptopp\zlib.h"
#include "..\3rd\cryptopp\hex.h"
#include "..\3rd\cryptopp\osrng.h"
using namespace CryptoPP;
using namespace std;

#define PACK_NAME "game_cn.cpk"
#define KEY_NAME  "game_cn.key"
#define HEADER_KEY "希月学园&红发小女孩"

#define MAGIC "CPK\0"
#define VERSION "\x02\x00\x00\x00"
#define WARNING "本补丁由【希月学园&红发小女孩】制作，禁止拆解破解本补丁。"

#define KEY_TOP_KEY 0x11451498
class CPKFileSystem
{
public:
    CPKFileSystem();
    ~CPKFileSystem();
    BOOL InitFileSystem();
    BOOL UnInitFileSystem();
#ifdef TEST
    void Test();
#endif
    BOOL GetFileByName(char* FileName, BYTE** FileBuff, DWORD* FileSize);
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
        char FileName[32];
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