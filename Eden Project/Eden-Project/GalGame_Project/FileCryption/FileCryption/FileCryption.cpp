// FileCryption.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include "zlib.h"
using namespace std;

typedef struct AymHeader {
	char HeaderSig1[9];
	char HeaderSig2[10];
	char HeaderSig3[24];
	char HeaderSig4[62];
	char key1[150];
	char key2[120];
	char key3[19];
	char key4[53];
	DWORD Ver1;
	DWORD Ver2;
	DWORD Ver3;
	DWORD Hash;
	DWORD CompressSig;
	DWORD CompressSize;
}AymHd_t;

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

bool Main2(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	if (!fp)
	{
		MessageBox(0, L"找不到文件。", 0, 0);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* buff = new unsigned char[size];
	fread(buff, size, 1, fp);
	fclose(fp);

	fp = fopen("enc.bin", "wb");

	unsigned long compress_size = compressBound(size);
	unsigned char* compress_buffer = new unsigned char[compress_size];

	AymHd_t Aym;
	strcpy(Aym.HeaderSig1, "AYM_FILE");
	strcpy(Aym.HeaderSig2, "VER 1.0.0");
	strcpy(Aym.HeaderSig3, "THIS IS A ENCRYPT FILE.");
	strcpy(Aym.HeaderSig4, "YOU CAN NOT DECRYPT IT UNLESS YOU HAVE ACCESS FROM DEVELOPER.");
	strcpy(Aym.key1, "aw0i8rjh0q938rj39hrpvb8ph8y38y38y38y38y3u8yhq3987 y78tt8uy75t467[-=()(9u)(UV*ＶＵＮＨＢ（）｛Ｕ＊ｙｈｐ９０８ｓｈｉｔｐ９８５0[a8hrjig[0q389q[+)(_~}i");
	strcpy(Aym.key2, "29999999999997y45ttttyyyyyyyhu089q233333171717171717171717171717171717171717170888888888888888888888888888888888884y91f");
	strcpy(Aym.key3, "【希月学园汉化组】");
	strcpy(Aym.key4, "charabration！恋爱少女人格崩坏 | 简体中文化补丁V1.01");
	Aym.Ver1 = 1;
	Aym.Ver2 = 0;
	Aym.Ver3 = 0;
	Aym.Hash = MurmurHash64B(buff, size);
	fwrite(Aym.HeaderSig1, sizeof(Aym.HeaderSig1), 1, fp);
	fwrite(Aym.HeaderSig2, sizeof(Aym.HeaderSig2), 1, fp);
	fwrite(Aym.HeaderSig3, sizeof(Aym.HeaderSig3), 1, fp);
	fwrite(Aym.HeaderSig4, sizeof(Aym.HeaderSig4), 1, fp);
	fwrite(Aym.key1, sizeof(Aym.key1), 1, fp);
	fwrite(Aym.key2, sizeof(Aym.key2), 1, fp);
	fwrite(Aym.key3, sizeof(Aym.key3), 1, fp);
	fwrite(Aym.key4, sizeof(Aym.key4), 1, fp);
	fwrite(&Aym.Ver1, sizeof(Aym.Ver1), 1, fp);
	fwrite(&Aym.Ver2, sizeof(Aym.Ver2), 1, fp);
	fwrite(&Aym.Ver3, sizeof(Aym.Ver3), 1, fp);
	fwrite(&Aym.Hash, sizeof(Aym.Hash), 1, fp);
	cout << Aym.Hash << endl;
	for (int i = 0; i < size; i++)
	{
		buff[i] ^= Aym.key1[i % 150];
		Aym.key1[i % 150] += i;
		Aym.key1[i % 150] ^= Aym.key2[i % 120];
		Aym.key2[i % 120] += i;
		Aym.key2[i % 120] ^= Aym.key3[i % 19];
		Aym.key3[i % 19] += i;
		Aym.key3[i % 19] ^= Aym.key4[i % 53];
		Aym.key4[i % 53] += i;
		buff[i] ^= Aym.key4[i % 53];
		buff[i] -= i;
		buff[i] += Aym.Hash;
		Aym.Hash ^= i;
	}
	if (compress2(compress_buffer, &compress_size, buff, size,9) == Z_OK)
	{
		cout << "compressed." << endl;
		Aym.CompressSig = 0xFFFFFFFF;
		Aym.CompressSize = compress_size;
		fwrite(&Aym.CompressSize, sizeof(Aym.CompressSize), 1, fp);
		for (int i = 0; i < compress_size; i++)
		{
			compress_buffer[i] ^= 0x01010101;
			compress_buffer[i] += 0x01010101;
			compress_buffer[i] ^= Aym.Hash;
		}
		fwrite(compress_buffer, compress_size, 1, fp);
		fclose(fp);
		return true;
	}
	Aym.CompressSize = 0x00000000;
	fwrite(&Aym.CompressSize, sizeof(Aym.CompressSize), 1, fp);
	fwrite(buff, size, 1, fp);
	fclose(fp);
	return true;
}

bool Decrypt(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	if (!fp)
	{
		MessageBox(0, L"找不到文件。", 0, 0);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* buff = new unsigned char[size];
	fread(buff, size, 1, fp);
	fclose(fp);
	AymHd_t Aym;
	memcpy(Aym.HeaderSig1, buff, 9);
	buff += 9;
	memcpy(Aym.HeaderSig2, buff, 10);
	buff += 10;
	memcpy(Aym.HeaderSig3, buff, 24);
	buff += 24;
	memcpy(Aym.HeaderSig4, buff, 62);
	if (strcmp(Aym.HeaderSig1, "AYM_FILE") != 0
		&& strcmp(Aym.HeaderSig2, "VER 1.0.0") != 0
		&& strcmp(Aym.HeaderSig3, "THIS IS A ENCRYPT FILE.") != 0
		&& strcmp(Aym.HeaderSig4, "YOU CAN NOT DECRYPT IT UNLESS YOU HAVE ACCESS FROM DEVELOPER.") != 0)
	{
		cout << Aym.HeaderSig1 << endl;
		cout << Aym.HeaderSig2 << endl;
		cout << Aym.HeaderSig3 << endl;
		cout << Aym.HeaderSig4 << endl;
		MessageBox(0, L"不正确的文件头。", 0, 0);
		return false;
	}
	buff += 62;
	memcpy(Aym.key1, buff, 150);
	buff += 150;
	memcpy(Aym.key2, buff, 120);
	buff += 120;
	memcpy(Aym.key3, buff, 19);
	buff += 19;
	memcpy(Aym.key4, buff, 53);
	if (strcmp(Aym.key1, "aw0i8rjh0q938rj39hrpvb8ph8y38y38y38y38y3u8yhq3987 y78tt8uy75t467[-=()(9u)(UV*ＶＵＮＨＢ（）｛Ｕ＊ｙｈｐ９０８ｓｈｉｔｐ９８５0[a8hrjig[0q389q[+)(_~}i") != 0
		&& strcmp(Aym.key2, "29999999999997y45ttttyyyyyyyhu089q233333171717171717171717171717171717171717170888888888888888888888888888888888884y91f") != 0
		&& strcmp(Aym.key3, "【希月学园汉化组】") != 0
		&& strcmp(Aym.key4, "charabration！恋爱少女人格崩坏 | 简体中文化补丁V1.01") != 0)
	{
		cout << Aym.key1 << endl;
		cout << Aym.key2 << endl;
		cout << Aym.key3 << endl;
		cout << Aym.key4 << endl;
		MessageBox(0, L"不正确的key。", 0, 0);
		return false;
	}
	buff + 53;
	Aym.Ver1 = *(DWORD*)buff;
	buff += 4;
	Aym.Ver2 = *(DWORD*)buff;
	buff += 4;
	Aym.Ver3 = *(DWORD*)buff;
	if (Aym.Ver1 != 1 && Aym.Ver2 != 0 && Aym.Ver3 != 0)
	{
		cout << Aym.Ver1 << endl;
		cout << Aym.Ver2 << endl;
		cout << Aym.Ver3 << endl;
		MessageBox(0, L"不正确的版本。", 0, 0);
		return false;
	}
	buff += 4;
	Aym.Hash = *(DWORD*)buff;
	cout << Aym.Hash << endl;
	buff += 4;
	Aym.CompressSig = *(DWORD*)buff;
	cout << Aym.CompressSig << endl;
	buff += 4;
	Aym.CompressSize = *(DWORD*)buff;
	buff += 4;
	cout << Aym.CompressSize << endl;
	if (Aym.CompressSig == 0xFFFFFFFF && Aym.CompressSize!=0x00000000)
	{
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= Aym.Hash;
			buff[i] -= 0x01010101;
			buff[i] ^= 0x01010101;
		}
		return false;
	}
	fp = fopen("dec.bin", "wb");
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBox(0, L"Usage: FileCryption.exe <FileCryption>", 0, 0);
		return -1;
	}
	char* FILENAME = argv[1];
	//char* FILENAME = (char*)"image.dat";
	bool succ = Main2(FILENAME);
	//bool succ = Decrypt(FILENAME);
	if (!succ)
	{
		MessageBox(0, L"程序运行失败。", 0, 0);
		return -1;
	}
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
