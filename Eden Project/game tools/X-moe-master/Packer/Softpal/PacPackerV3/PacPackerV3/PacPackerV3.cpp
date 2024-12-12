//useless?

#include <windows.h>
#include "WinFile.h"
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::wstring;

#define PAC_VERSION 3

#pragma pack(1)
typedef struct PacHeader
{
	UCHAR Magic[4]; // "PAC"
	DWORD unknown1;
	DWORD FileCount;
	UCHAR junk[2040];
}PacHeader;

typedef struct PacChunk
{
	UCHAR FileName[32];
	DWORD Length;
	DWORD Offset;
}PacChunk;
#pragma pack()


vector<string> FilesPool;
int wmain(int argc, wchar_t** argv) 
{
	if (argc != 4) 
	{
		MessageBoxW(NULL, L"Exe OriginalPackage Path OutputPackage", L"PacPackerV3", MB_OK);
		return -1;
	}

	WinFile OriginalPack;
	WinFile OutputPack;
	wstring OriPackName(argv[1]);
	wstring Path(argv[2]);
	wstring OutputName(argv[3]);

	INT64 DiffSize = 0;

	if (OriginalPack.Open(OriPackName.c_str(), WinFile::FileRead) != S_OK)
	{
		MessageBoxW(NULL, L"Couldn't open Original Package", L"PacPackerV3", MB_OK);
		ExitProcess(-1);
	}

	if (OutputPack.Open(OutputName.c_str(), WinFile::FileWrite) != S_OK)
	{
		MessageBoxW(NULL, L"Couldn't open Original Package", L"PacPackerV3", MB_OK);
		ExitProcess(-1);
	}

	//Parse at first
	PacHeader hdr;
	OriginalPack.Read((PBYTE)&hdr, sizeof(hdr));

	PacChunk* Chunks = (PacChunk*)HeapAlloc(GetProcessHeap(), 0, hdr.FileCount * sizeof(PacChunk));
	OriginalPack.Read((PBYTE)Chunks, hdr.FileCount * sizeof(PacChunk));

	for (ULONG i = 0; i < hdr.FileCount; i++)
	{
		FilesPool.push_back((char*)(Chunks[i].FileName));
	}

	//ReCorrent Header Info Later
	OutputPack.Write((PBYTE)(&hdr), sizeof(PacHeader));
	OutputPack.Write((PBYTE)(Chunks), hdr.FileCount * sizeof(PacChunk));

	ULONG Offset = sizeof(PacHeader)+hdr.FileCount * sizeof(PacChunk);

	wstring BasePath = Path + L"\\";
	for (ULONG i = 0; i < hdr.FileCount; i++)
	{
		WCHAR ACPFileName[MAX_PATH] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, FilesPool[i].c_str(), FilesPool[i].length(), ACPFileName, MAX_PATH);
		wstring NewFileName = BasePath + ACPFileName;

		WinFile File;
		if (File.Open(NewFileName.c_str(), WinFile::FileRead) != S_OK)
		{
			MessageBoxW(NULL, wstring(L"Couldn't open file : " + NewFileName).c_str(), L"PacPackerV3", MB_OK );
			ExitProcess(-1);
		}

		Chunks[i].Length = File.GetSize32();
		Chunks[i].Offset = Offset;
		Offset += Chunks[i].Length;

#if 0
		DiffSize += (File.GetSize32() - Chunks[i].Length);
		if (DiffSize != 0)
		{
			printf("Different %d\n", DiffSize);
			Chunks[i].Length = File.GetSize32();
			for (ULONG j = i + 1; j < hdr.FileCount; j++)
			{
				Chunks[j].Offset += DiffSize;
			}
		}

#endif

		PBYTE Buffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, File.GetSize32());
		File.Read(Buffer, File.GetSize32());
		OutputPack.Write(Buffer, File.GetSize32());
		HeapFree(GetProcessHeap(), 0, Buffer);
		File.Release();
	}

	OutputPack.Seek(0, FILE_BEGIN);
	OutputPack.Write((PBYTE)(&hdr), sizeof(PacHeader));
	OutputPack.Write((PBYTE)(Chunks), hdr.FileCount * sizeof(PacChunk));

	HeapFree(GetProcessHeap(), 0, Chunks);
	OriginalPack.Release();
	OutputPack.Release();
	return 0;
}

