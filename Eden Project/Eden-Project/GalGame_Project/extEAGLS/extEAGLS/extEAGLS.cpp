// extEAGLS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

/*
0049F278  31 71 61 7A 32 77 73 78 33 65 64 63 34 72 66 76  1qaz2wsx3edc4rfv
0049F288  35 74 67 62 36 79 68 6E 37 75 6A 6D 38 69 6B 2C  5tgb6yhn7ujm8ik,
0049F298  39 6F 6C 2E 30 70 3B 2F 2D 40 3A 5E 5B 5D        9ol.0p;/-@:^[]
*/

#define DumpDecrypedIndex
const char* IndexKey = "1qaz2wsx3edc4rfv5tgb6yhn7ujm8ik,9ol.0p;/-@:^[]";

#pragma pack (1)
struct IDXIndex {
	char FileName[20];
	DWORD Offset;
	DWORD FileSize;
} ;
#pragma pack ()

int main()
{
	char* FileName = (char*)"CGPACK.idx";
	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD IndexSize = ftell(fp) - 4; // "最后的DWORD是seed"
	fseek(fp, 0, SEEK_SET);
	BYTE* IndexBuff = new BYTE[IndexSize];
	int Seed = 0;
	fread(IndexBuff, IndexSize, 1, fp);
	fread(&Seed, sizeof(int), 1, fp);
	fclose(fp);
	cout << "Seed:0x" << hex << Seed << endl;
	// Decrypt Index
	DWORD IndexKeyLen = strlen(IndexKey);
	for (DWORD i = 0; i < IndexSize; i++)
	{
		Seed = Seed * 0x343FD + 0x269EC3;
		IndexBuff[i] ^= IndexKey[((Seed >> 16) & 0x7fff) % IndexKeyLen];
	}

#ifdef DumpDecrypedIndex
	char fnm[MAX_PATH] = { 0 };
	strcpy(fnm, FileName);
	strcat(fnm, ".dmp");
	fp = fopen(fnm, "wb");
	fwrite(IndexBuff, IndexSize, 1, fp);
	fclose(fp);
#endif

	IDXIndex* Index = (IDXIndex*)IndexBuff;
	delete[] IndexBuff;
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
