// extACV.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>

#include "zlib.h"
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

#pragma pack (1)
struct ACVHeader 
{
	char Magic[4]; // "ACV1"
	unsigned int FileCount;
};
struct ACVIndex
{
	unsigned long long Hash;
	unsigned char Flag;
	unsigned int Offset;
	unsigned int FileSize;
	unsigned int UncompressSize;
};
#pragma pack ()

#define KEY1 0x8B6A4E5F
#define KEY2 0xBABA18A9

int main(int argc, char* argv[])
{
	ACVHeader Header;
	if (argc != 2)
	{
		MessageBox(0, L"extACV.exe <input dat file>", L"AyamiKaze", 0);
		//return -1;
	}
	//char* FileName = argv[1];
	char FileName[] = "script.dat";
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rb");
	if (!fp)
		return E("No fp");

	fread(&Header, 1, sizeof(ACVHeader), fp);

	if (strncmp(Header.Magic, "ACV1", 4))
	{
		fclose(fp);
		cout << Header.Magic << endl;
		return E("Magic Not Match");
	}
	unsigned int FileCount = Header.FileCount ^ KEY1;
	cout << "FileCount:" << dec << FileCount << endl;

	unsigned int IndexSize = FileCount * sizeof(ACVIndex);
	ACVIndex* Index = new ACVIndex[IndexSize];
	fread(Index, 1, IndexSize, fp);

	string tmp(FileName);
	string dir = tmp + "__unpack\\";
	mkdir(dir.c_str());
	for (unsigned int i = 0; i < FileCount; i++)
	{
		unsigned char Flag = Index[i].Flag ^ Index[i].Hash;
		unsigned int Offset = Index[i].Offset ^ Index[i].Hash ^ KEY1;
		unsigned int FileSize = Index[i].FileSize ^ Index[i].Hash;
		unsigned int UncompressSize = Index[i].UncompressSize ^ Index[i].Hash;
		unsigned int Key = Index[i].Hash ^ KEY2;
		cout << "---------------------------------------------------" << endl;
		cout << "FileHash:0x" << hex << Index[i].Hash << endl;
		cout << "Flag:0x" << hex << (unsigned int)Flag << endl;
		cout << "Offset:0x" << hex << Offset << endl;
		cout << "FileSize:0x" << hex << FileSize << endl;
		cout << "UncompressSize:0x" << hex << UncompressSize << endl;
		cout << "Key:0x" << hex << Key << endl;

		fseek(fp, Offset, SEEK_SET);
		unsigned char* buff = new unsigned char[FileSize];
		fread(buff, 1, FileSize, fp);

		FILE* dmp = fopen("dump.dmp", "wb");
		fwrite(buff, 1, FileSize, dmp);
		fclose(dmp);

		unsigned int DecSize = FileSize / 4;
		///*
		for (unsigned int pos = 0; pos < FileSize / 4; pos++)
			buff[pos] ^= Key;
		//*/
		/*
		_asm
		{
			mov eax, buff;
			mov ebx, KEY2;
			mov ecx, 0;
			mov edx, DecSize;
		do_loop:
			xor dword ptr ds : [eax + ecx] , ebx;
			add ecx, 4
			cmp ecx, edx;
			jb do_loop;
		}
		*/
		unsigned long DstSize = UncompressSize * 2;
		unsigned char* UncompressBuff = new unsigned char[DstSize];
		if (uncompress(UncompressBuff, &DstSize, buff, FileSize) != Z_OK)
		{
			FILE* dmp = fopen("dump2.dmp", "wb");
			fwrite(buff, 1, FileSize, dmp);
			fclose(dmp);
			delete[] buff;
			delete[] UncompressBuff;
			fclose(fp);
			return E("Zlib Error");
		}

		delete[] buff;
		delete[] UncompressBuff;
	}

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
