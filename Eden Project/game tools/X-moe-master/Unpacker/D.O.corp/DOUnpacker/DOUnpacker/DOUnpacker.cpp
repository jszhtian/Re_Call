#include "WinFile.h"

//works with 永遠だと思っていたあの頃

//数据似乎是0x10对齐的

#pragma pack(push, 1)
typedef struct DoHeader
{
	BYTE  Magic[8];
	ULONG IndexCount;
	ULONG IndexEndPos;
	BYTE  PackName[8];
	ULONG Unk;
	ULONG Attr; //FILE_ATTRIBUTE_ARCHIVE
}DoHeader, *pDoHeader;

typedef struct ChunkInfo
{
	BYTE  FileName[0xC];
	ULONG Offset;
	ULONG Size;
}ChunkInfo, *pChunkInfo;
#pragma pack(pop)

//0x20

int wmain(int argc, WCHAR* argv[])
{
	WinFile InFile;
	DoHeader Header;
	PBYTE IndexBuffer = nullptr;
	ULONG IndexSize;

	do
	{
		if (InFile.Open(argv[1], WinFile::FileRead) != S_OK)
			break;

		static BYTE Mark[8] = { 0x53, 0x4D, 0x32, 0x4D, 0x50, 0x58, 0x31, 0x30 };
		InFile.Read(Header.Magic, 8);

		if (memcmp(Mark, Header.Magic, 8))
			break;

		InFile.Read((PBYTE)&(Header.IndexCount), sizeof(DoHeader) - 8);

		IndexSize = Header.IndexEndPos - 0x20; //sizeof(DoHeader)
		IndexBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, IndexSize);

		if (!IndexBuffer)
			break;
		
		InFile.Read(IndexBuffer, IndexSize);
		
		for (ULONG i = 0; i < Header.IndexCount; i++)
		{
			pChunkInfo Info = (pChunkInfo)(IndexBuffer + i*sizeof(ChunkInfo));
			WCHAR WideName[0xC] = { 0 };
			MultiByteToWideChar(932, 0, (PCHAR)Info->FileName, lstrlenA((PCHAR)Info->FileName), WideName, 0xC);

			WinFile OutFile;

			if (OutFile.Open(WideName, WinFile::FileWrite) != S_OK)
				continue;

			PBYTE DataBuffer = nullptr;
			DataBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, Info->Size);
			
			if (!DataBuffer)
				continue;

			InFile.Seek(Info->Offset, FILE_BEGIN);
			InFile.Read(DataBuffer, Info->Size);

			OutFile.Write(DataBuffer, Info->Size);

			if (DataBuffer)
				HeapFree(GetProcessHeap(), 0, DataBuffer);

			OutFile.Release();
		}
	}
	while (0);

	if (IndexBuffer)
		HeapFree(GetProcessHeap(), 0, IndexBuffer);

	InFile.Release();
	return 0;
}

