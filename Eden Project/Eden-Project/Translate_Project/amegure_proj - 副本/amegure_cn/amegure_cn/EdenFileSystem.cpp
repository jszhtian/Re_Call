#include "framework.h"

void EdenFileSystem_Main(char* FILENAME)
{
	FILE* fp = fopen(FILENAME, "rb");
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned char* buff = new unsigned char[size];
	fread(buff, size, 1, fp);
	fclose(fp);
	for (int i = 0; i < size; i++)
	{
		buff[i] ^= size;
	}
	unsigned char* decbuff = new unsigned char[size];
	memset(decbuff, 0, size);
	KEY_TABLE_TYPE kt;
	memset(kt, 0, sizeof(kt));
	Camellia_Ekeygen(128, camelia_key, kt);
	unsigned char* enc = buff, * dec = decbuff;
	for (int i = 0; i < size / CAMELLIA_BLOCK_SIZE; i++)
	{
		Camellia_DecryptBlock(128, enc, kt, dec);

		enc += CAMELLIA_BLOCK_SIZE;
		dec += CAMELLIA_BLOCK_SIZE;
	}

	unsigned long decomp_size = CACHE_SIZE;
	unsigned char* decomp_buff = new unsigned char[decomp_size];
	uncompress(decomp_buff, &decomp_size, decbuff, size);
	DWORD dwBytesToWrite;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFileW(L"~crash", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_DELETE_ON_CLOSE, NULL);
	WriteFile(hFile, decomp_buff, decomp_size, &dwBytesToWrite, NULL);
}