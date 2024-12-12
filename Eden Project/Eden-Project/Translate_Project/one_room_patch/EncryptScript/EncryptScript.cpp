// EncryptScript.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

unsigned long long MurmurHash64B(const void* key, int len, unsigned int seed = 0xEE6B27EB)
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

#pragma pack (1)
typedef struct {
	char magic[54];		/* "ARCX" */
	unsigned int size;
	unsigned long long hash;
} dat_header_t;
#pragma pack ()

#define enc

int main(int argc, char* argv[])
{
#ifdef enc
	char* FileName = argv[1];
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	if (buff)
	{
		fread(buff, size, 1, fp);
		fclose(fp);
		dat_header_t hdr;
		strcpy(hdr.magic, "本补丁由【天之圣杯汉化组】制作，禁止一切拆解行为。\r\n");
		hdr.size = size;
		hdr.hash = MurmurHash64B(buff, size);
		for (unsigned int i = 0; i < size; i++)
		{
			buff[i] ^= 0xFF;
			buff[i] += 0x20;
		}
		fp = fopen("tsubaki.dat", "wb");
		fwrite(&hdr, sizeof(hdr), 1, fp);
		fwrite(buff, size, 1, fp);
		fclose(fp);
	}
#else
	dat_header_t hdr;
	char* FileName = argv[1];
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread(&hdr, sizeof(hdr), 1, fp);
	cout << hdr.size << endl;
	cout << hdr.hash << endl;
	fseek(fp, sizeof(hdr), SEEK_SET);
	char* buff = (char*)malloc(hdr.size);
	if (!buff)
		return E("NoBuff");
	fread(buff, hdr.size, 1, fp);
	fclose(fp);
	for (unsigned int i = 0; i < hdr.size; i++)
	{
		buff[i] -= 0x20;
		buff[i] ^= 0xFF;
	}
	unsigned long long BuffHash = MurmurHash64B(buff, hdr.size);
	if (BuffHash == hdr.hash)
	{
		fp = fopen("decrypt.dat", "wb");
		fwrite(buff, hdr.size, 1, fp);
		fclose(fp);
	}
	else
		cout << "Hash Failed" << endl;
#endif
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
