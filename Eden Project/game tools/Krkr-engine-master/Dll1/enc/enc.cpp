// TestCrypt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <algorithm> 
#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
using namespace std;

#define __PAIR__(high, low) (((unsigned __int64)(high)<<sizeof(high)*8) | low)
unsigned __int64 CRC64_String(char* str)
{
	int i, j, x;
	int hashidx;
	int string_length;
	static int init_crc64 = 0;
	static unsigned __int64 crc64_table[256];
	unsigned __int64 hashvalue = 0;
	unsigned int retvlow, retvhig;

	if (!init_crc64)
	{
		init_crc64 = 1;
		for (i = 0; i < 256; i++)
		{
			hashvalue = i;
			for (j = 0; j < 8; j++)
			{
				if (hashvalue & 1)
				{
					hashvalue ^= 0x85E1C3D753D46D27;
				}
				hashvalue = hashvalue >> 1;
			}
			crc64_table[i] = hashvalue;
		}
	}
	retvlow = -1;
	retvhig = -1;
	string_length = strlen(str);
	for (x = 0; x < string_length; x++)
	{
		hashidx = (int)((unsigned char)retvlow ^ (unsigned char)str[x]);
		hashvalue = ((__PAIR__(retvhig, retvlow)) >> 8);
		retvlow = hashvalue ^ crc64_table[hashidx];
		retvhig = (hashvalue ^ crc64_table[hashidx]) >> 32;
	}
	hashvalue = ~(__PAIR__(retvhig, retvlow));
	return hashvalue;
}

unsigned long long MurmurHash64B(const void* key, int len, unsigned __int64 seed)
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

template<class T>
void FileNameToLower(T& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE);
	char* bbb = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
	return bbb;
}

int main(int argc, wchar_t* argv[])
{
	//if (argc == 1)
	//	return -1;
	_setmode(_fileno(stdout), _O_U16TEXT);
	wstring filename(L"Test.txt");
	FileNameToLower(filename);
	FILE* fp = fopen(wtoc(filename.c_str()), "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	if (buff)
	{
		fread(buff, size, 1, fp);
		fclose(fp);
		unsigned long long hash = MurmurHash64B(filename.c_str(), filename.length(), CRC64_String(wtoc(filename.c_str())));
		char* fn = (char*)(to_string(hash).c_str());
		cout << hash << endl;
		fp = fopen(fn, "wb");
		fwrite(buff, size, 1, fp);
		fclose(fp);
		free(buff);

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
