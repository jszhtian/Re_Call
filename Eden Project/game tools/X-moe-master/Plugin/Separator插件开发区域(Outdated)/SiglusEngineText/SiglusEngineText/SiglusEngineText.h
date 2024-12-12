#ifndef _SiglusEngineText_
#define _SiglusEngineText_

#include <Windows.h>
#include <cstdio>
#include <tchar.h>
#include <string>
#include <locale.h>

using std::wstring;

typedef unsigned char byte;
unsigned char* buf;
typedef unsigned int ulong;

typedef struct FILE_INFO{
	DWORD offset;
	DWORD length;
} FILE_INFO, *PFILE_INFO;

typedef struct HEADERPAIR{
	int offset;
	int count;
}HEADERPAIR;

typedef struct ss_header{
	DWORD szZiped;
	DWORD szOrigianl;
}ss_header;

typedef struct SCENEHEADER{
	int headerLength;
	HEADERPAIR v1;
	HEADERPAIR string_index_pair;
	HEADERPAIR string_data_pair;
	HEADERPAIR v4;
	HEADERPAIR v5;
	HEADERPAIR v6;
	HEADERPAIR v7;
	HEADERPAIR v8;
	HEADERPAIR v9;
	HEADERPAIR v10;
	HEADERPAIR v11;
	HEADERPAIR v12;
	HEADERPAIR v13;
	HEADERPAIR v14;
	HEADERPAIR v15;
	HEADERPAIR v16;
}SCENEHEADER;
typedef struct file_offset_info_s
{
	ulong offsets;
	ulong sizes;
}file_offset_info_t;


enum CMDMask
{
	UsingUnicode = 0x1UL, //文件名会被转码,需要配合ForceXXX使用
	SaveTempData = 2,
	ForceCHSCP = 4, //源是gbk
	ForceJPCP = 8,  //源是sjis
	ExtractSingle = 16, //提取单独的文件
	UsingImageEx = 32 //强制混合32-bit的alpha通道
};

extern "C" _declspec (dllexport) void ShowInfo();

extern "C" _declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile);

extern "C" _declspec (dllexport) BOOL Engine(TCHAR *pszStr);

extern "C" _declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output = NULL, const TCHAR *args = NULL);


#endif
