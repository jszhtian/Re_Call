#include <Windows.h>
#include "WinFile.h"
#include "CMem.h"

#include <stdio.h>

#pragma pack(push, 1)
typedef struct FileInfo
{
	DWORD Offset;
	DWORD Length;
} FileInfo, *pFileInfo;

typedef struct HeaderPair
{
	ULONG Offset;
	ULONG Length;
}HeaderPair, *pHeaderPair;

typedef struct SceneHeader
{
	ULONG HeaderLength;
	HeaderPair v1;
	HeaderPair StringIndexInfo;
	HeaderPair StringDataInfo;
	HeaderPair v4;
	HeaderPair v5;
	HeaderPair v6;
	HeaderPair v7;
	HeaderPair v8;
	HeaderPair v9;
	HeaderPair v10;
	HeaderPair v11;
	HeaderPair v12;
	HeaderPair v13;
	HeaderPair v14;
	HeaderPair v15;
	HeaderPair v16;
}SceneHeader, *pSceneHeader;
#pragma pack(pop)

void StringDecoder(PWCHAR Buffer, PWCHAR OutBuffer, ULONG length, LONG key)
{
	key *= 0x7087;
	for (ULONG i = 0; i<length; i++)
	{
		OutBuffer[i] = (WCHAR)(Buffer[i] ^ key);
	}
}

int wmain(int argc, WCHAR* argv[])
{
	WinFile       File, OutFile;
	PBYTE         FileBuffer;
	ULONG         FileSize;
	WCHAR         OutName[MAX_PATH];
	WCHAR*        StringData;
	SceneHeader*  Header;
	FileInfo*     StringIndex;
	WCHAR*        OldString;
	WCHAR*        NewString;
	CHAR*         UTF8String;
	FileInfo*     info;
	CHAR          Prefix[128];

	if (argc != 2)
	{
		return 0;
	}
	if (FAILED(File.Open(argv[1], WinFile::FileRead)))
	{
		return 0;
	}
	FileSize = File.GetSize32();
	FileBuffer = (PBYTE)CMem::Alloc(FileSize);
	if (FileBuffer == nullptr)
	{
		return 0;
	}
	File.Read(FileBuffer, FileSize);
	File.Release();

	RtlZeroMemory(OutName, MAX_PATH * 2);
	wsprintfW(OutName, L"%s.txt", argv[1]);
	if (FAILED(OutFile.Open(OutName, WinFile::FileWrite)))
	{
		return 0;
	}

	Header = (SceneHeader*)FileBuffer;
	StringIndex = (FileInfo*)&FileBuffer[Header->StringIndexInfo.Offset];
	StringData = (wchar_t*)&FileBuffer[Header->StringDataInfo.Offset];
	
	for (ULONG i = 0; i<Header->StringIndexInfo.Length; i++)
	{
		printf("%08x\n", (size_t)&StringIndex[i] - (size_t)FileBuffer);
		//getchar();
		info = &StringIndex[i];
		OldString = &StringData[info->Offset];
		NewString = (PWCHAR)CMem::Alloc((info->Length * 2 + 2) * sizeof(WCHAR));
		UTF8String = (CHAR*)CMem::Alloc(info->Length * 4);
		RtlZeroMemory(NewString, (info->Length * 2 + 2) * sizeof(WCHAR));
		RtlZeroMemory(UTF8String, info->Length * 4);
		RtlZeroMemory(Prefix, 128);

		LONG Len = info->Length;
		printf("Length : %08x, Offset %08x\n", Len > 0 ? Len : -Len, info->Offset);
		
		//not a null-terminated string, we should parse the length table to make sure the real length of each string.
		if(Len > 0)StringDecoder(OldString, NewString, Len > 0 ? Len : -Len, i);

		//wprintf(L"%s\n", NewString);

		WideCharToMultiByte(CP_UTF8, 0, NewString, lstrlenW(NewString), UTF8String, info->Length * 4,
			nullptr, nullptr);
		wsprintfA(Prefix, "[0x%08x]", i);
		OutFile.Write((PBYTE)Prefix, lstrlenA(Prefix));
		OutFile.Write((PBYTE)UTF8String, lstrlenA(UTF8String));
		OutFile.Write((PBYTE)"\r\n;", 3);
		wsprintfA(Prefix, "[0x%08x]", i);
		OutFile.Write((PBYTE)Prefix, lstrlenA(Prefix));
		OutFile.Write((PBYTE)"\r\n\r\n", 4);
		CMem::Free(UTF8String);
		CMem::Free(NewString);
	}
	OutFile.Release();
	return 0;
}

