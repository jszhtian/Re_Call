// EncryptAymScript.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>

//#include "zconf.h"
#include "zdict.h"
#include "zlib.h"
#include "zstd.h"
#include "zstd_errors.h"

using namespace std;

#define XOR_KEY "\r\n未经允许，禁止擅自拆解本文件\r\n"
#define XOR_KEY_LEN 33

#pragma pack(1)
struct aym_header
{
    char flag[23];
    unsigned int str_count;
};

struct aym_text_struct
{
	unsigned int off;
	unsigned int sz;
	unsigned int jmp;
};

struct aym_text_header {
	char flag[29];

	unsigned int HeaderSize;
	unsigned long long HeaderHash;
	char HeaderKey[XOR_KEY_LEN];

	unsigned long EntrySize;
	unsigned long long EntryHash;
	unsigned long EntryCompSize;

	unsigned long TextSize;
	unsigned long long TextHash;
	unsigned long TextCompSize;
};
#pragma pack()

unsigned long long MurmurHash64B(const void* key, int len, unsigned int seed = 0x114514)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h1 = seed ^ len;
	unsigned int h2 = 0;

	const unsigned int* data = (const unsigned int*)key;

	while (len >= 8)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;

		unsigned int k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}

	if (len >= 4)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}

	switch (len)
	{
	case 3: h2 ^= ((unsigned char*)data)[2] << 16;
	case 2: h2 ^= ((unsigned char*)data)[1] << 8;
	case 1: h2 ^= ((unsigned char*)data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	unsigned long long h = h1;

	h = (h << 32) | h2;

	return h;
}

int main()
{
    aym_text_header header;

    strcpy(header.flag, "AyamiKazeFormatScriptVer1.00");

    FILE* fp = fopen("Script.aym", "rb");

    fseek(fp, 0, SEEK_END);
    unsigned int FileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Get Header Size
	header.HeaderSize = sizeof(aym_header);
	char* HeaderBuffer = (char*)malloc(header.HeaderSize);
	if (!HeaderBuffer)
	{
		cout << "Alloc Header Buffer Error" << endl;
		system("pause");
		return 0;
	}
    cout << "header.HeaderSize:" << header.HeaderSize << endl;

	// Read HeaderBuffer
    fread(HeaderBuffer, header.HeaderSize, 1, fp);
	aym_header* Header = (aym_header*)HeaderBuffer;
	if (strcmp(Header->flag, "AyamiKazeScriptVer1.00"))
	{
		cout << "Header Not Match" << endl;
		cout << Header->flag << endl;
		system("pause");
		return 0;
	}

	// Get Entry Size
	header.EntrySize = Header->str_count * sizeof(aym_text_struct);
	cout << "header.EntrySize:" << header.EntrySize << endl;
	unsigned char* EntryBuff = (unsigned char*)malloc(header.EntrySize);
	if (!EntryBuff)
	{
		cout << "Alloc EntryBuff Error" << endl;
		system("pause");
		return 0;
	}

	// Get Text Size
	header.TextSize = FileSize - header.HeaderSize - header.EntrySize;
	cout << "header.TextSize:" << header.TextSize << endl;
	unsigned char* TextBuff = (unsigned char*)malloc(header.TextSize);
	if (!TextBuff)
	{
		cout << "Alloc TextBuff Error" << endl;
		system("pause");
		return 0;
	}

	// Read EntryBuff and TextBuff
	fread(EntryBuff, header.EntrySize, 1, fp);
	fread(TextBuff, header.TextSize, 1, fp);

	// close
	fclose(fp);

	// Hash
	header.HeaderHash = MurmurHash64B(HeaderBuffer, header.HeaderSize);
	header.EntryHash = MurmurHash64B(EntryBuff, header.EntrySize);
	header.TextHash = MurmurHash64B(TextBuff, header.TextSize);
	cout << "header.HeaderHash:" << header.HeaderHash << endl;
	cout << "header.EntryHash:" << header.EntryHash << endl;
	cout << "header.TextHash:" << header.TextHash << endl;

	// Encrypt and Compress
    strcpy(header.HeaderKey, XOR_KEY);
    for (int i = 0; i < header.HeaderSize; i++)
    {
        HeaderBuffer[i] += 0x20;
        HeaderBuffer[i] ^= header.HeaderKey[i % XOR_KEY_LEN];
    }

	header.EntryCompSize = header.EntrySize;
	unsigned char* EntryCompBuff = (unsigned char*)malloc(header.EntryCompSize);
	if (!EntryCompBuff)
	{
		cout << "Alloc EntryCompBuff Error" << endl;
		system("pause");
		return 0;
	}

	if (compress2(EntryCompBuff, &header.EntryCompSize, EntryBuff, header.EntrySize, Z_DEFAULT_COMPRESSION) != Z_OK)
	{
		cout << "Entry Compress Error" << endl;
		system("pause");
		return 0;
	}
	cout << "header.EntryCompSize:" << header.EntryCompSize << endl;

	header.TextCompSize = header.TextSize;
	unsigned char* TextCompBuff = (unsigned char*)malloc(header.TextCompSize);
	if (!TextCompBuff)
	{
		cout << "Alloc TextCompBuff Error" << endl;
		system("pause");
		return 0;
	}
	size_t ret = ZSTD_compress(TextCompBuff, header.TextCompSize, TextBuff, header.TextSize, 1);
	if (ZSTD_isError(ret) != ZSTD_error_no_error)
	{
		cout << "Text Compress Error:" << ZSTD_isError(ret) << endl;
		system("pause");
		return 0;
	}
	header.TextCompSize = ret;
	cout << "header.TextCompSize:" << header.TextCompSize << endl;

	// Write
	fp = fopen("Script.bin", "wb");
	fwrite(&header, sizeof(aym_text_header), 1, fp);
	fwrite(HeaderBuffer, header.HeaderSize, 1, fp);
	fwrite(EntryCompBuff, header.EntryCompSize, 1, fp);
	fwrite(TextCompBuff, header.TextCompSize, 1, fp);
	fclose(fp);

	system("pause");
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
