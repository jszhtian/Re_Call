#ifndef _XP3Info_
#define _XP3Info_

#include <Windows.h>
#include <string>

using std::wstring;

#pragma pack(1)


typedef struct PACK_FILE_INFO
{
	LONG    Encrypt;
	LONG    Compress;
	WCHAR   FileName[MAX_PATH];

	PACK_FILE_INFO() : Encrypt(0), Compress(0)
	{
		ZeroMemory(FileName, MAX_PATH * 2);
	}
} PACK_FILE_INFO;


typedef struct KRKR2_XP3_DATA_HEADER
{
	BYTE            bZlib;
	LARGE_INTEGER   ArchiveSize;
	LARGE_INTEGER   OriginalSize;

	KRKR2_XP3_DATA_HEADER() : bZlib(0)
	{
		ArchiveSize.QuadPart = (ULONG64)0;
		OriginalSize.QuadPart = (ULONG64)0;
	}

} KRKR2_XP3_DATA_HEADER;

typedef struct KRKR2_XP3_INDEX
{
	KRKR2_XP3_DATA_HEADER   DataHeader;
	BYTE                    Index[1];
} KRKR2_XP3_INDEX;


static unsigned char XP3Mark1[] =
{ 0x58/*'X'*/, 0x50/*'P'*/, 0x33/*'3'*/, 0x0d/*'\r'*/,
0x0a/*'\n'*/, 0x20/*' '*/, 0x0a/*'\n'*/, 0x1a/*EOF*/,
0xff /* sentinel */ };

static unsigned char XP3Mark2[] =
{ 0x8b, 0x67, 0x01, 0xff/* sentinel */ };

const static unsigned char StaticXP3V2Magic[0xB] = { 0x58, 0x50, 0x33, 0x0d, 0x0a, 0x20, 0x0a, 0x1a, 0x8b, 0x67, 0x01 };

typedef struct KRKR2_XP3_HEADER
{
	BYTE          Magic[0xB];
	LARGE_INTEGER IndexOffset;

	KRKR2_XP3_HEADER()
	{
		IndexOffset.QuadPart = (ULONG64)0;
		CopyMemory(Magic, XP3Mark1, 8);
		CopyMemory(Magic + 8, XP3Mark2, 3);
	}
	KRKR2_XP3_HEADER(PBYTE Mark, ULONG64 Offset)
	{
		CopyMemory(Magic, Mark, 0xB);
		IndexOffset.QuadPart = Offset;
	}

} KRKR2_XP3_HEADER;


typedef struct KRKRZ_XP3_INDEX_CHUNK_Yuzu
{
	CHAR Magic[4]; //Yuzu,etc...
	LARGE_INTEGER ChunkSize;
	ULONG Hash;
	USHORT Len;
	wstring Name;

	KRKRZ_XP3_INDEX_CHUNK_Yuzu() : Hash(0){}
} KRKRZ_XP3_INDEX_CHUNK_Yuzu;


typedef struct KRKR2_XP3_INDEX_CHUNK_INFO2
{
	CHAR            Magic[4];     // 'info'
	LARGE_INTEGER   ChunkSize;
	ULONG           EncryptedFlag;
	LARGE_INTEGER   OriginalSize;
	LARGE_INTEGER   ArchiveSize;
	USHORT          FileNameLength;
	wstring         FileName;
} KRKR2_XP3_INDEX_CHUNK_INFO2;

typedef struct KRKR2_XP3_INDEX_CHUNK_SEGM_DATA
{
	BOOL            bZlib;     // bZlib & 7 -> 1: compressed  0: raw  other: error
	LARGE_INTEGER   Offset;
	LARGE_INTEGER   OriginalSize;
	LARGE_INTEGER   ArchiveSize;
} KRKR2_XP3_INDEX_CHUNK_SEGM_DATA;

typedef struct KRKR2_XP3_INDEX_CHUNK_SEGM
{
	CHAR                            Magic[4];
	LARGE_INTEGER                   ChunkSize;
	KRKR2_XP3_INDEX_CHUNK_SEGM_DATA segm[1];
} KRKR2_XP3_INDEX_CHUNK_SEGM;

typedef struct KRKR2_XP3_INDEX_CHUNK_ADLR
{
	CHAR            Magic[4];
	LARGE_INTEGER   ChunkSize;
	ULONG           Hash;
} KRKR2_XP3_INDEX_CHUNK_ADLR;

typedef struct KRKR2_XP3_INDEX_CHUNK_TIME
{
	CHAR            Magic[4];
	LARGE_INTEGER   ChunkSize;
	FILETIME        FileTime;
} KRKR2_XP3_INDEX_CHUNK_TIME;


typedef struct KRKR2_XP3_INDEX_CHUNK_FILE
{
	CHAR            Magic[4];
	LARGE_INTEGER   ChunkSize;
} KRKR2_XP3_INDEX_CHUNK_FILE;

typedef struct XP3Index
{
	KRKRZ_XP3_INDEX_CHUNK_Yuzu  yuzu;
	KRKR2_XP3_INDEX_CHUNK_FILE  file;
	KRKR2_XP3_INDEX_CHUNK_TIME  time;
	KRKR2_XP3_INDEX_CHUNK_ADLR  adlr;
	KRKR2_XP3_INDEX_CHUNK_SEGM  segm;
	KRKR2_XP3_INDEX_CHUNK_INFO2 info;

	BOOL isM2Format;

	XP3Index() : isM2Format(TRUE)
	{

	}
} XP3Index;

typedef struct XP3IndexKrkr2
{
	KRKR2_XP3_INDEX_CHUNK_FILE  file;
	KRKR2_XP3_INDEX_CHUNK_TIME  time;
	KRKR2_XP3_INDEX_CHUNK_ADLR  adlr;
	KRKR2_XP3_INDEX_CHUNK_SEGM  segm;
	KRKR2_XP3_INDEX_CHUNK_INFO2 info;
} XP3IndexKrkr2;


typedef struct
{
	KRKRZ_XP3_INDEX_CHUNK_Yuzu  yuzu;
	KRKR2_XP3_INDEX_CHUNK_FILE  file;
	KRKR2_XP3_INDEX_CHUNK_TIME  time;
	KRKR2_XP3_INDEX_CHUNK_ADLR  adlr;
	KRKR2_XP3_INDEX_CHUNK_SEGM  segm;
	KRKR2_XP3_INDEX_CHUNK_INFO2 info;
} SMyXP3IndexM2;

typedef struct
{
	KRKR2_XP3_INDEX_CHUNK_FILE  file;
	KRKR2_XP3_INDEX_CHUNK_TIME  time;
	KRKR2_XP3_INDEX_CHUNK_ADLR  adlr;
	KRKR2_XP3_INDEX_CHUNK_SEGM  segm;
	KRKR2_XP3_INDEX_CHUNK_INFO2 info;
} SMyXP3IndexNormal;

#pragma pack(4)

const UCHAR CHUNK_MAGIC_FILE[4] = { 'F', 'i', 'l', 'e' };
const UCHAR CHUNK_MAGIC_INFO[4] = { 'i', 'n', 'f', 'o' };
const UCHAR CHUNK_MAGIC_SEGM[4] = { 's', 'e', 'g', 'm' };
const UCHAR CHUNK_MAGIC_ADLR[4] = { 'a', 'd', 'l', 'r' };
const UCHAR CHUNK_MAGIC_TIME[4] = { 't', 'i', 'm', 'e' };

//�Ѿ������Զ�ʶ���ײ�(
const UCHAR CHUNK_MAGIC_YUZU[4] = { 'Y', 'u', 'z', 'u' };
const UCHAR CHUNK_MAGIC_KRKRZ_M2[4] = { 'e', 'l', 'i', 'F' };
const UCHAR CHUNK_MAGIC_KRKRZ_NEKO[4] = { 'n', 'e', 'k', 'o' };
const UCHAR CHUNK_MAGIC_KRKRZ_FENG[4] = { 'f', 'e', 'n', 'g' };

#endif