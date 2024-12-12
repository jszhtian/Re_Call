// MyScript.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <random>
#include "BH_huffman_comp.h"
#include "xxhash.h"
using namespace std;
struct MyHeader {
	CHAR Magic[16];
	DWORD seed;
	DWORD RawSize;
	DWORD zero;
	DWORD zero2;
	DWORD Hash;
};

typedef struct ScriptHeader
{
	CHAR  Magic[0x1C];
	ULONG HeaderSize;
} BGI_BURIKO_SCRIPT_HEADER;


void GetKey(DWORD& key0, DWORD& key1, DWORD hash)
{
	DWORD tmp = hash ^ (hash + 0x5D588B65);
	DWORD tmp2 = tmp ^ (hash - 0x359D3E2A);
	key0 = tmp2 ^ (tmp - 0x70E44324);
	key1 = key0 ^ (tmp2 + 0x6C078965);
}
DWORD ParityCheck(DWORD key0, DWORD key1)
{
	DWORD tmp = (key1 ^ ((key0 ^ key1) + 0x5D588B65)) - 0x359D3E2A;
	DWORD v = (key1 ^ ((key1 ^ tmp) - 0x70E44324)) + 0x6C078965;
	return v << (tmp >> 27) | v >> (32 - (tmp >> 27));
}
int main()
{
	FILE* fp = fopen("_1_01_010", "rb");
	fseek(fp, 0, SEEK_END);
	DWORD m_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE* buff = new BYTE[m_size];
	fread(buff, m_size, 1, fp);
	fclose(fp);
	BGI_BURIKO_SCRIPT_HEADER hdr = { 0 };
	memcpy(&hdr, buff, sizeof(BGI_BURIKO_SCRIPT_HEADER));
	// 取反
	for (int i = 0x1C; i < m_size; i++)
	{
		buff[i] ^= 0x45;
		buff[i] += 0x20;
		buff[i] ^= 0xFF;
	}

	for (int i = 0x1c; i < hdr.HeaderSize; i++)
	{
		buff[i] += 0x20;
		buff[i] ^= 0xFF;
	}
	BYTE* compBuff = new BYTE[m_size];
	DWORD compSize = m_size;
	huffman_compress(compBuff, &compSize, buff, m_size);
	delete[] buff;

	for (int i = 0; i < compSize; i++)
		compBuff[i] ^= 0xFF;
	DWORD compHash = XXH32(compBuff, compSize, 0xB92C6C17);
	DWORD key[2];
	GetKey(key[0], key[1], compHash);
	DWORD SetKey = ParityCheck(key[0], key[1]);

	for (int i = 0; i < compSize; i++)
	{
		*(BYTE*)(compBuff + i) ^= SetKey;
		SetKey *= i;
	}


	std::random_device rd;  // 获取随机数种子
	std::mt19937 gen(rd()); // 以 rd() 初始化 Mersenne Twister 引擎
	std::uniform_int_distribution<> distrib(0x114514, 0x191981);
	MyHeader mhdr;
	memcpy(mhdr.Magic, "DSC FORMAT 1.00", 16);
	mhdr.seed = distrib(gen);
	mhdr.RawSize = m_size;
	mhdr.zero = 2;
	mhdr.zero2 = 0;
	mhdr.Hash = compHash;
	fp = fopen("_1_01_010.bin", "wb");
	fwrite(&mhdr, sizeof(MyHeader), 1, fp);
	fwrite(compBuff, compSize, 1, fp);
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
