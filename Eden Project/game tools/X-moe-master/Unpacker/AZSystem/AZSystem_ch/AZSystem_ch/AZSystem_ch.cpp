// AZSystem_ch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <string>

#include "SFMT-19937.h"
#include "SFMT-132049.h"
#include "SFMT-params.h"
#include "SFMT-params132049.h"
#include "SFMT-params19937.h"
//#include "ewf_crc.h"

#include "zlib\zlib.h"

#pragma comment(lib,"zlib.lib")

using std::string;

typedef struct ASBNew
{
	char  sign[4];	//ASB 0x1A
	DWORD compressed;
	DWORD uncompressed;
};

unsigned int ewf_crc(void *buffer, unsigned int buffer_size, unsigned int previous_key)
{
	unsigned char *cbuffer = (unsigned char *)buffer;
	unsigned int b = previous_key & 0xffff;
	unsigned int d = (previous_key >> 16) & 0xffff;
	unsigned int iterator = 0;

	while (iterator < buffer_size)
	{
		b += cbuffer[iterator];
		d += b;

		if ((iterator != 0) && ((iterator % 0x15b0 == 0) || (iterator == buffer_size - 1)))
		{
			b = b % 0xfff1;
			d = d % 0xfff1;
		}
		iterator++;
	}
	return((d << 16) | b);
}

#define crc ewf_crc



struct KeyInfo
{

	char string[MAX_PATH];
	DWORD crc;
};

static KeyInfo asb_decode_table[] = 
{
	{ "Clover heart's", 0x20061215}, 
	{ "", 0	}
};


unsigned char Keys[0xFF];


int DecodeInit()
{
	unsigned int v0; // ecx@1
	int result; // eax@2x
	unsigned int v2; // edx@2
	signed int v3; // esi@2

	v0 = 0;
	do
	{
		result = v0 << 24;
		v2 = v0;
		v3 = 8;
		do
		{
			if (result >= 0)
				result *= 2;
			else
				result = 2 * result ^ 0x4C11DB7;
			if (v2 & 1)
				v2 = (v2 >> 1) ^ 0xEDB88320;
			else
				v2 >>= 1;
			--v3;
		} while (v3);
		Keys[v0] = result;
		Keys[v0++] = v2;
	} while (v0 <= 0xFF);
	return result;
}

#if 0
int __cdecl GenKeysCH(const BYTE* a1, int a2, int a3)
{
	int v3; // edx@1
	const BYTE* v4; // ecx@1
	int result; // eax@1
	int v6; // eax@2

	v3 = a2;
	v4 = a1;
	result = a3;
	if (a2 > 0)
	{
		v6 = ~a3;
		do
		{
			v6 = Keys[(unsigned __int8)v6 ^ *(BYTE *)v4++] ^ ((unsigned int)v6 >> 8);
			--v3;
		} while (v3);
		result = ~v6;
	}
	return result;
}


unsigned int CHAllKeys()
{
	unsigned int v0; // ecx@3
	unsigned int v1; // ecx@3
	int v3; // eax@1
	int v4; // eax@1
	int v5; // eax@1

	v3 = GenKeysCH("ALcot", 6, 537071912);
	v4 = GenKeysCH("Clover Heart's", 15, v3);
	v5 = GenKeysCH("http://www.alcot.biz/", 22, v4);
	sub_40C953(v5);
	--dword_494090;
	if (!dword_494090)
		sub_40C996();
	v0 = *(_DWORD *)dword_7E5838;
	dword_7E5838 += 4;
	v1 = ((((v0 >> 11) ^ v0) & 0xFF3A58AD) << 7) ^ (v0 >> 11) ^ v0;
	return ((v1 & 0xFFFFDF8C) << 15) ^ v1 ^ ((((v1 & 0xFFFFDF8C) << 15) ^ v1) >> 18);
}



#endif


extern DWORD ewf_crc(void *buffer, DWORD buffer_size, DWORD previous_key);
extern void init_genrand(unsigned long s);
extern unsigned long genrand_int32(void);

static int azsystem_arc1a_decompress(BYTE *uncompr, DWORD uncomprlen, BYTE *compr, DWORD comprlen)
{
	DWORD act_uncomprlen;

	if (crc32(0, compr + 4, comprlen - 4) != *(DWORD *)compr)
		return -1;

	act_uncomprlen = uncomprlen;
	if (uncompress(uncompr, &act_uncomprlen, compr + 4, comprlen - 4) != Z_OK)
		return -1;

	if (act_uncomprlen != uncomprlen)
		return -1;

	return 0;
}

static int asb1a_decompress(BYTE *uncompr, DWORD uncomprlen, BYTE *compr, DWORD comprlen, DWORD key)
{
	DWORD dword_count = comprlen / 4;
	DWORD *enc_data = (DWORD *)compr;
	key = (uncomprlen ^ key) ^ ((((uncomprlen ^ key) << 12) | (uncomprlen ^ key)) << 11);
	while (dword_count--)
		*enc_data++ -= key;
	return azsystem_arc1a_decompress(uncompr, uncomprlen, compr, comprlen);
}


static int __asb1a_process(ASBNew *asb1a_header,
	const char *string, DWORD seed,
	BYTE *uncompr, DWORD uncomprlen)
{
	BYTE *asb1a_compr = (BYTE *)(asb1a_header + 1);

	/*	v3 = GenKeysCH("ALcot", 6, 537071912);
	v4 = GenKeysCH("Clover Heart's", 15, v3);
	v5 = GenKeysCH("http://www.alcot.biz/", 22, v4);
	*/

	seed = crc32(seed, (BYTE *)"ALcot", 6);
	seed = crc32(seed, (BYTE *)"Clover Heart's", 15);
	seed = crc32(seed, (BYTE *)"http://www.alcot.biz/", 22);
	init_genrand(seed);
	DWORD key = genrand_int32();
	return asb1a_decompress(uncompr, uncomprlen, asb1a_compr,
		asb1a_header->compressed, key);
}

static int asb1a_process(BYTE *compr, DWORD comprlen, BYTE *uncompr, DWORD uncomprlen)
{
	ASBNew *asb1a_header = (ASBNew *)compr;
	BYTE *compr_backup = (BYTE *)malloc(comprlen);
	if (!compr_backup)
		return -1;
	memcpy(compr_backup, compr, comprlen);

	int ret = -1;
	ret = __asb1a_process(asb1a_header, "Clover Heart's",
			537071912, uncompr, uncomprlen);

	free(compr_backup);
	return ret;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		return 0;
	}

	FILE* file = nullptr;
	file = fopen(argv[1], "rb");
	if (file)
	{
		rewind(file);
		fseek(file, 0, SEEK_END);
		unsigned long size = ftell(file);
		unsigned char* pBuffer = new unsigned char[size];
		rewind(file);
		fread(pBuffer, 1, size, file);
		fclose(file);

		if (!strncmp((char *)pBuffer, "ASB\x1a", 4))
		{
			ASBNew *asb1a_header = (ASBNew *)pBuffer;

			DWORD uncomprlen = asb1a_header->uncompressed;
			unsigned char* uncompr = new unsigned char[uncomprlen];

			int ret = asb1a_process(pBuffer, asb1a_header->compressed, uncompr, uncomprlen);

			FILE *out = fopen((std::string(argv[1]) + ".out").c_str(), "wb");
			fwrite(uncompr, 1, uncomprlen, out);
			fclose(out);
			delete[] pBuffer;
		}
	}
	else
	{
		MessageBoxA(NULL, ("Failed to open : " + string(argv[1])).c_str(), 
			               "AZSystem for clover heart", MB_OK);
	}
	return 0;
}

