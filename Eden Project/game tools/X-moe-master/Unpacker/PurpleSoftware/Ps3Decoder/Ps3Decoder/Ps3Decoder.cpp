#include "stdafx.h"
#include <Windows.h>
#include <string>

using std::wstring;

typedef struct 
{
	UCHAR magic[4];			// "PS2A"
	ULONG header_length;		// 0x30
	ULONG unknown0;	
	ULONG key;
	ULONG unknown1_count;		// Ã¿Ïî4×Ö½Ú
	ULONG unknown2_length;
	ULONG unknown3_length;
	ULONG name_index_length;
	ULONG unknown4;			// 0
	ULONG comprlen;
	ULONG uncomprlen;
	ULONG unknown5;			// 0
} ps2_header_t;

void ps2_uncompress(BYTE *uncompr, DWORD uncomprlen, BYTE *compr, DWORD comprlen)
{
	DWORD curbyte = 0;
	DWORD flag = 0;
	DWORD cur_winpos = 0x7df;
	BYTE window[2048];

	memset(window, 0, cur_winpos);
	while (1) {
		flag >>= 1;
		if (!(flag & 0x0100)) {
			if (curbyte >= comprlen)
				break;
			flag = compr[curbyte++] | 0xff00;
		}

		if (flag & 1) {
			BYTE data;

			if (curbyte >= comprlen)
				break;

			data = compr[curbyte++];
			window[cur_winpos++] = data;
			*uncompr++ = data;
			cur_winpos &= 0x7ff;
		}
		else {
			DWORD offset, count;

			if (curbyte >= comprlen)
				break;
			offset = compr[curbyte++];

			if (curbyte >= comprlen)
				break;
			count = compr[curbyte++];

			offset |= (count & 0xe0) << 3;
			count = (count & 0x1f) + 2;

			for (DWORD i = 0; i < count; ++i) {
				BYTE data;

				data = window[(offset + i) & 0x7ff];
				*uncompr++ = data;
				window[cur_winpos++] = data;
				cur_winpos &= 0x7ff;
			}
		}
	}
}


//sub_457AB0
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		return 0;
	}
	
	ps2_header_t ps2_header;
	FILE* fin = _wfopen(argv[1], L"rb");
	if (!fin)
	{
		return 0;
	}

	fread(&ps2_header, 1, sizeof(ps2_header_t), fin);
	DWORD comprlen = ps2_header.comprlen;
	BYTE *compr = (BYTE *)malloc(comprlen);
	if (!compr)
	{
		return 0;
	}
	fseek(fin, ps2_header.header_length, SEEK_SET);
	fread(compr, 1, comprlen, fin);

	BYTE xor = (BYTE)((ps2_header.key >> 24) + (ps2_header.key >> 3));
	BYTE shifts = ((ps2_header.key >> 20) % 5) + 1;
	for (DWORD i = 0; i < comprlen; ++i)
	{
		BYTE tmp = (compr[i] - 0x7c) ^ xor;
		compr[i] = (tmp >> shifts) | (tmp << (8 - shifts));
	}

	DWORD uncomprlen = ps2_header.uncomprlen + sizeof(ps2_header);
	BYTE *uncompr = (BYTE *)malloc(uncomprlen);
	if (!uncompr) 
	{
		free(compr);
		return 0;
	}

	ps2_uncompress(uncompr + sizeof(ps2_header),
		uncomprlen - sizeof(ps2_header), compr, comprlen);
	memcpy(uncompr, &ps2_header, sizeof(ps2_header));
	fclose(fin);
	
	wstring OutName(argv[1]);
	OutName +=  L"_ss";
	FILE* out = _wfopen(OutName.c_str(), L"wb");
	fwrite(uncompr, 1, uncomprlen, out);
	fclose(out);
	return 0;
}

