// CRC64.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CRC64.h"

uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l)
{
	uint64_t j;
		for (j = 0; j < l; j++) {
		uint8_t byte = s[j];
		crc = crc64_tab[(uint8_t)crc ^ byte] ^ (crc >> 8);
		
	}
	return crc;
	
}

union Large
{
	uint64_t I64;
	unsigned long I32[2];
};

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		return 0;
	}
	FILE* file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		return -1;
	}

	rewind(file);
	fseek(file, 0, SEEK_END);
	unsigned long size = ftell(file);
	rewind(file);
	unsigned char* Buffer = new unsigned char[size];
	fread(Buffer, 1, size, file);
	fclose(file);

	FILE *out = fopen("Log.txt", "wb");
	Large pInfo;
	pInfo.I64 = crc64(0, Buffer, size);
	fprintf(out, "%s  %08x %08x\r\n", argv[1], pInfo.I32[0], pInfo.I32[1]);
	fclose(out);

	delete[] Buffer;
	return 0;
}

