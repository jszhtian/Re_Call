#include <cstdio>
#include <memory>

#define ZLIB_WINAPI
#include "../Lib/zlib.h"
#undef ZLIB_WINAPI

#include "../Lib/camellia.h"

#define CACHE_SIZE 1024*1024*10 //10M
 
int main()
{
	unsigned char key[] = { 0xB9, 0x9A, 0xC2, 0x68, 0x9C, 0x9E, 0x6B, 0xBA, 0xE5, 0xEB, 0x82, 0x70, 0xC5, 0x1A, 0xCD, 0xE6 }; //For AA
	//unsigned char key[] = {0x8C, 0x51, 0x90, 0xC2, 0x82, 0xCC, 0x8B, 0xF3, 0x82, 0xF0, 0x89, 0x7A, 0x82, 0xA6, 0x82, 0xC4}; //For AEF 


	FILE *fp = fopen("EXEC.bin", "rb");
	if (fp == NULL) return 1;

	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char *buff = new unsigned char[size];
	fread(buff, size, 1, fp);
	fclose(fp);

	unsigned char *decbuff = new unsigned char[size];
	memset(decbuff, 0, size);

	KEY_TABLE_TYPE kt;
	memset(kt, 0, sizeof(kt));
	Camellia_Ekeygen(128, key, kt);

	unsigned char *enc = buff, *dec = decbuff;

	for (int i = 0; i < size / CAMELLIA_BLOCK_SIZE; i++)
	{
		Camellia_DecryptBlock(128, enc, kt, dec);

		enc += CAMELLIA_BLOCK_SIZE;
		dec += CAMELLIA_BLOCK_SIZE;
	}

	//unsigned long decomp_size = CACHE_SIZE;
	//unsigned char *decomp_buff = new unsigned char[decomp_size];
	//if (uncompress(decomp_buff, &decomp_size, decbuff, size) != Z_OK)
	//	return -1;

	fp = fopen("DeEXEC.bin", "wb");
	fwrite(decbuff, size, 1, fp);
	fclose(fp);

	/*delete[] decomp_buff;*/
	delete[] buff;
	delete[] decbuff;

	return 0;
}
