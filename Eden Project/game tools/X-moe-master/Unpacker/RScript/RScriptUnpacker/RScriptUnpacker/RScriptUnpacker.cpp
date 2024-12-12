#include <windows.h>
#include <stdio.h>

//works with forest


typedef struct Header
{
	DWORD Magic;
	DWORD ChunkSize;
	DWORD ChunkCount;
}Header;

//40
typedef struct ChunkItem
{
	CHAR FileName[0x20];
	DWORD Offset; //after the chunk segement
	DWORD Size;
}ChunkItem;

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return 0;

	FILE* fin = _wfopen(argv[1], L"rb");

	if (!fin)
		return 0;

	Header FileHeader;
	fread(&FileHeader, 1, sizeof(FileHeader), fin);
	
	if (FileHeader.Magic != 0x0001424cUL)
		goto End;

	PBYTE ChunkData = new BYTE[FileHeader.ChunkSize];
	if (!ChunkData)
		goto End;

	fread(ChunkData, 1, FileHeader.ChunkSize, fin);

	ULONG PostOffset = sizeof(FileHeader) + FileHeader.ChunkSize;
	for (ULONG i = 0; i < FileHeader.ChunkCount; i++)
	{
		ChunkItem* item = (ChunkItem*)(ChunkData + i * sizeof(ChunkItem));
		fseek(fin, PostOffset + item->Offset, SEEK_SET);

		FILE* fout = fopen(item->FileName, "wb");
		PBYTE FileData = new BYTE[item->Size];
		if (!FileData)
			continue;

		fread(FileData, 1, item->Size, fin);
		fwrite(FileData, 1, item->Size, fout);
		fclose(fout);

		delete[] FileData;
	}

	delete[] ChunkData;

	End:
	fclose(fin);
	return 0;
}

