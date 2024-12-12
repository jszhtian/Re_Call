#include "stdafx.h"
#include <cstdio>
#include <Windows.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

#pragma pack(1)
typedef struct PackHeader
{
	UCHAR Magic[3];
	ULONG IndexBuffer;
}PackHeader;

typedef struct PackIndex
{
	string FileName; //以0结尾，需要解密
	ULONG Offset; //不计算文件头和Index记录
	ULONG Size;
};
#pragma pack()

vector<PackIndex> FilePool;

void DecodeFileName(string& eFileName)
{
	string RawString = eFileName;
	for (ULONG i = 0; i < eFileName.length(); i++)
	{
		RawString[i] = ~RawString[i];
	}
	eFileName = RawString;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		printf("Error Args\n");
		getchar();
		return 0;
	}
	FILE* fin = _wfopen(argv[1], L"rb");
	if (fin == nullptr)
	{
		printf("Couldn't open input file.\n");
		getchar();
		return -1;
	}
	fseek(fin, 3, SEEK_SET);
	ULONG IndexSize = 0;
	fread(&IndexSize, 1, 4, fin);
	rewind(fin);
	PBYTE IndexBuffer = (PBYTE)GlobalAlloc(0x40u, IndexSize);
	if (!IndexBuffer)
	{
		printf("Failed to allocate memory for index buffer[%08x].\n", IndexSize);
		getchar();
		return 0;
	}
	fread(IndexBuffer, 1, IndexSize, fin);
	if (memcmp(IndexBuffer, "PAC", 3))
	{
		printf("not a pac file.\n");
		getchar();
		return 0;
	}
	ULONG OffsetPtr = 7;
	while (OffsetPtr < IndexSize)
	{
		string EncryptedFileName((CHAR*)(IndexBuffer + OffsetPtr));
		OffsetPtr += EncryptedFileName.length() + 1;
		ULONG FileOffset = *(ULONG*)(IndexBuffer + OffsetPtr);
		OffsetPtr += sizeof(ULONG);
		ULONG FileSize = *(ULONG*)(IndexBuffer + OffsetPtr);
		OffsetPtr += sizeof(ULONG);

		DecodeFileName(EncryptedFileName);
		
		PackIndex Info;
		Info.FileName = EncryptedFileName;
		Info.Offset = FileOffset;
		Info.Size = FileSize;

		printf("Found File : %s, Size[0x%08x]\n", Info.FileName.c_str(), Info.Size);
		FilePool.push_back(Info);
	}
	
	for (auto it : FilePool)
	{
		WCHAR WideFileName[260] = { 0 };
		MultiByteToWideChar(932, 0, it.FileName.c_str(), it.FileName.length(), WideFileName, 260);
		FILE* fout = _wfopen(WideFileName, L"wb");
		PBYTE FileBuffer = (PBYTE)GlobalAlloc(0, it.Size);
		if (fout == nullptr || !FileBuffer)
		{
			continue;
		}
		fseek(fin, it.Offset + IndexSize, SEEK_SET);
		fread(FileBuffer, 1, it.Size, fin);
		fwrite(FileBuffer, 1, it.Size, fout);
		fclose(fout);
		GlobalFree(FileBuffer);
	}

	GlobalFree(IndexBuffer);
	fclose(fin);

	//getchar();
	return 0;
}

