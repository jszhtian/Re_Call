﻿// BGIScript_Enc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>

#define ZLIB_WINAPI
#include "zlib.h"
#undef ZLIB_WINAPI

#include "camellia.h"

#define CACHE_SIZE 1024*1024*10 //10M

static const unsigned char camelia_key[] = {
	0xC4, 0xF8, 0x36, 0x44, 0x7E, 0xA9, 0x3F, 0xDD, 0xE8, 0x99, 0x38, 0xAA, 0xD5, 0xB4, 0x5E, 0x1C,
	0x43, 0x84, 0x59, 0x6B, 0xF9, 0xD5, 0x50, 0xF2, 0x6F, 0xE5, 0x57, 0x85, 0xCC, 0x70, 0x33, 0x1B,
	0x5A, 0x40, 0x34, 0x6C, 0xE0, 0x11, 0x3D, 0xF5, 0x76, 0x21, 0x3A, 0x82, 0xE7, 0x3C, 0x85, 0x12,
	0x71, 0x0C, 0x82, 0x65, 0xCB, 0x5D, 0x8B, 0xFC, 0x5D, 0x6D, 0x8C, 0x8B, 0xFE, 0xF8, 0xE8, 0x15,
	0xF4, 0x51, 0x6B, 0x6B, 0x62, 0x61, 0x6C, 0x1C, 0xD8, 0x30, 0x65, 0x85, 0x4E, 0x00, 0x62, 0xF2,
	0xC6, 0xD9, 0xB0, 0x65, 0x50, 0xE9, 0xB7, 0x12, 0xEA, 0xB8, 0xBE, 0x8B, 0x7C, 0x88, 0xB9, 0xFC,
	0x58, 0x61, 0xB2, 0x4D, 0xCE, 0x51, 0xB5, 0x3A, 0x74, 0x00, 0xBC, 0xA3, 0xE2, 0x30, 0xBB, 0xD4,
	0x6A, 0xE9, 0x69, 0x43, 0xFC, 0xD9, 0x6E, 0x34, 0x46, 0x88, 0x67, 0xAD, 0xD0, 0xB8, 0x60, 0xDA,
	0x3C, 0x71, 0x05, 0x50, 0xAA, 0x41, 0x02, 0x27, 0x10, 0x10, 0x0B, 0xBE, 0x86, 0x20, 0x0C, 0xC9,
	0x0E, 0xF9, 0xDE, 0x5E, 0x98, 0xC9, 0xD9, 0x29, 0x22, 0x98, 0xD0, 0xB0, 0xB4, 0xA8, 0xD7, 0xC7,
	0x20, 0x83, 0xB8, 0xED, 0xB6, 0xB3, 0xBF, 0x9A, 0x0C, 0xE2, 0xB6, 0x03, 0x9A, 0xD2, 0xB1, 0x74,
	0x12, 0x0B, 0x63, 0xE3, 0x84, 0x3B, 0x64, 0x94, 0x3E, 0x6A, 0x6D, 0x0D, 0xA8, 0x5A, 0x6A, 0x7A,
	0x44, 0x93, 0x0F, 0xF0, 0xD2, 0xA3, 0x08, 0x87, 0x68, 0xF2, 0x01, 0x1E, 0xFE, 0xC2, 0x06, 0x69,
	0x76, 0x1B, 0xD4, 0xFE, 0xE0, 0x2B, 0xD3, 0x89, 0x5A, 0x7A, 0xDA, 0x10, 0xCC, 0x4A, 0xDD, 0x67,
	0xE8, 0xA3, 0xD6, 0xD6, 0x7E, 0x93, 0xD1, 0xA1, 0xC4, 0xC2, 0xD8, 0x38, 0x52, 0xF2, 0xDF, 0x4F, 
	0xDA, 0x2B, 0x0D, 0xD8, 0x4C, 0x1B, 0x0A, 0xAF, 0xF6, 0x4A, 0x03, 0x36, 0x60, 0x7A, 0xB0, 0x60
};

using namespace std;
void camelia_enc(char* FILENAME)
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
		buff[i] += 0x20;
	}
	unsigned long comp_size = compressBound(size);
	unsigned char* comp_buff = new unsigned char[comp_size];
	compress2(comp_buff, &comp_size, buff, size, 9);
	comp_size += (CAMELLIA_BLOCK_SIZE - comp_size % CAMELLIA_BLOCK_SIZE);//16字节对齐
	unsigned char* encbuff = new unsigned char[comp_size];
	memset(encbuff, 0, comp_size);
	KEY_TABLE_TYPE kt;
	memset(kt, 0, sizeof(kt));
	Camellia_Ekeygen(128, camelia_key, kt);

	unsigned char* pla = comp_buff, * enc = encbuff;

	for (int i = 0; i < comp_size / CAMELLIA_BLOCK_SIZE; i++)
	{
		Camellia_EncryptBlock(128, pla, kt, enc);

		enc += CAMELLIA_BLOCK_SIZE;
		pla += CAMELLIA_BLOCK_SIZE;
	}
	strcat(FILENAME, ".xbs");
	fp = fopen(FILENAME, "wb");
	for (int i = 0; i < comp_size; i++)
	{
		encbuff[i] ^= comp_size;
	}
	fwrite(encbuff, comp_size, 1, fp);
	fclose(fp);

	delete[] buff;
	delete[] encbuff;
	delete[] comp_buff;

}
void camelia_dec(char* FILENAME)
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
	strcat(FILENAME, ".dec");
	for (int i = 0; i < decomp_size; i++)
	{
		decomp_buff[i] -= 0x20;
		decomp_buff[i] ^= decomp_size;
	}
	fp = fopen(FILENAME, "wb");
	fwrite(decomp_buff, decomp_size, 1, fp);
	fclose(fp);

	delete[] decomp_buff;
	delete[] buff;
	delete[] decbuff;
}

bool DecTest = FALSE;
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage:" << argv[0] << " File" << endl;
	}
	char* FILENAME = argv[1];
	camelia_enc(FILENAME);
	if (DecTest)
	{
		camelia_dec(FILENAME);
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
