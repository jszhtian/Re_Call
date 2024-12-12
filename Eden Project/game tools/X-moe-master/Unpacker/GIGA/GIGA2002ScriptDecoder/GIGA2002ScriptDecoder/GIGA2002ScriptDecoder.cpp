#include "stdafx.h"
#include <Windows.h>
#include <cstdio>
#include <string>

using std::wstring;

#pragma pack(1)
typedef struct ScriptHeader
{
	UCHAR Magic[3];
	ULONG dib_len;
	ULONG flag_bits;
};

typedef struct ScriptHeader_s
{
	UCHAR Magic[3];
};
#pragma pack()

inline unsigned char getbit_le(unsigned char byte, unsigned int pos)
{
	return !!(byte & (1 << pos));
}

static void baldrx_decompress(BYTE *uncompr, DWORD uncomprlen, BYTE *compr,
	DWORD comprlen, BYTE *flag_bitmap, DWORD flag_bits)
{
	DWORD act_uncomprlen = 0;

	for (DWORD i = 0; i < flag_bits; i++) {
		BYTE flag = flag_bitmap[i / 8];

		if (!getbit_le(flag, i & 7))
			uncompr[act_uncomprlen++] = *compr++;
		else {
			DWORD copy_bytes, win_offset;

			copy_bytes = *compr++;
			win_offset = *compr++ << 8;

			win_offset = (copy_bytes | win_offset) >> 5;
			copy_bytes = (copy_bytes & 0x1f) + 1;
			for (DWORD k = 0; k < copy_bytes; k++) {
				uncompr[act_uncomprlen] = uncompr[act_uncomprlen - win_offset - 1];
				act_uncomprlen++;
			}
		}
	}
}


int DecodeScript(wstring& filename, FILE* fin)
{
	fseek(fin, 0, SEEK_SET);
	ULONG FileSize = 0;
	fseek(fin, 0, SEEK_END);
	FileSize = ftell(fin);
	rewind(fin);
	ScriptHeader *header = (ScriptHeader*)malloc(FileSize);
	PBYTE Buffer = (PBYTE)(header)+sizeof(header);
	fread(header, 1, FileSize, fin);

	ULONG flag_bitmap_len = (header->flag_bits + 7) >> 3;
	PBYTE flag_bitmap = Buffer;
	ULONG comprLen = *(DWORD *)&flag_bitmap[flag_bitmap_len];
	PBYTE compr = &flag_bitmap[flag_bitmap_len] + 4;

	PBYTE uncompr = (BYTE *)malloc(header->dib_len);
	if (!uncompr)
	{
		free(header);
		return -1;
	}

	baldrx_decompress(uncompr, header->dib_len, compr, comprLen, flag_bitmap, flag_bitmap_len);

	return 0;
}

void UnFilter(PBYTE Buffer, ULONG Size)
{
	for (ULONG i = 0; i < Size; i++)
	{
		Buffer[i] = ~Buffer[i];
	}
}

int DecodeScriptV2(wstring& filename, FILE* pkg)
{
	ULONG dib_len, flag_bits, comprlen;
	DWORD flag_bitmap_len;
	BYTE *flag_bitmap, *compr;

	fseek(pkg, sizeof(ScriptHeader_s), SEEK_SET);
	fread(&dib_len, 1, 4, pkg);
	fread(&flag_bits, 1, 4, pkg);

	ULONG uncomprlen = dib_len;
	PBYTE uncompr = (BYTE *)malloc(uncomprlen);

	flag_bitmap_len = (flag_bits + 7) / 8;
	flag_bitmap = (BYTE *)malloc(flag_bitmap_len);
	if (!flag_bitmap)
	{
		free(uncompr);
		return -1;
	}

	fseek(pkg, sizeof(ScriptHeader_s) + 8, SEEK_SET);
	fread(flag_bitmap, 1, flag_bitmap_len, pkg);

	//Decode((char*)flag_bitmap, flag_bitmap_len);

	fseek(pkg, flag_bitmap_len + sizeof(ScriptHeader_s) + 8, SEEK_SET);
	fread(&comprlen, 1, 4, pkg);

	compr = (BYTE *)malloc(comprlen);
	if (!compr)
	{
		free(flag_bitmap);
		return -1;
	}

	fseek(pkg, flag_bitmap_len + sizeof(ScriptHeader_s) + 12, SEEK_SET);
	fread(compr, 1, comprlen, pkg);
	//Decode((char*)compr, comprlen);

	baldrx_decompress(uncompr, uncomprlen, compr, comprlen, flag_bitmap, flag_bits);
	free(compr);
	free(flag_bitmap);

	UnFilter(uncompr, uncomprlen);

	FILE* out = _wfopen(filename.c_str(), L"wb");
	fwrite(uncompr, 1, uncomprlen, out);
	fclose(out);
	return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		return 0;
	}

	FILE* fin = _wfopen(argv[1], L"rb");
	if (fin == nullptr)
	{
		return 0;
	}
	fseek(fin, 0, SEEK_END);
	ULONG FileSize = ftell(fin);
	rewind(fin);
	UCHAR Magic[3] = { 0 };
	fread(Magic, 1, 3, fin);
	if (memcmp(Magic, "COD", 2))
	{
		return 0;
	}
	ULONG ScriptIndexSize = 0;
	fread(&ScriptIndexSize, 1, 4, fin);
	rewind(fin);

	wstring filename(argv[1]);
	filename += L".unfilter";
	DecodeScriptV2(filename, fin);

	fclose(fin);
	return 0;
}

