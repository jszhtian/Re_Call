// extXFL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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


#pragma pack (1)
struct XFLHeader {
	char Magic[2]; // "LB"
	WORD Version; // "0x0001"
	DWORD IndexSize;
	DWORD IndexCount;
};

struct XFLIndex {
	char FileName[0x20];
	DWORD Offset; // 这里记录的是相对offset，seek时要加上sizeof(XFLIndex)和IndexSize。
	DWORD FileSize;
};

struct LWGHeader {
	char Magic[2]; // "LG"
	WORD Version; // "0x0001"
	DWORD Height;
	DWORD Width;
	DWORD IndexCount;
	DWORD Zero;
	DWORD IndexSize;
};

struct LWGIndex {
	DWORD X;
	DWORD Y;
	BYTE Unk;
	DWORD Offset;
	DWORD FileSize;
	BYTE FileNameSize;
	char *FileSize
};
#pragma pack ()

int main(int argc, char* argv[])
{
	XFLHeader header;

	if (argc != 2)
	{
		MessageBox(0, L"extXFL.exe <input xfl file>", L"AyamiKaze", 0);
		//return -1;
	}
	//char* FileName = argv[1];
	char FileName[] = "grps.xfl";

	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_SET);
	fread(&header, sizeof(XFLHeader), 1, fp);
	if (memcmp(header.Magic, "LB", 2))
		return E("Magic Not Match");
	if (header.Version != 1)
		return E("Version not 1");

	XFLIndex* Index = new XFLIndex[header.IndexSize];
	fread(Index, header.IndexSize, 1, fp);

	string tmp(FileName);
	string dir = tmp + "__unpack\\";
	mkdir(dir.c_str());

	DWORD BaseOfset = sizeof(XFLHeader) + header.IndexSize;
	for (DWORD i = 0; i < header.IndexCount; i++)
	{
		string fnm(Index[i].FileName);
		fnm = dir + fnm;

		FILE* fout = fopen(fnm.c_str(), "wb");
		fseek(fp, BaseOfset + Index[i].Offset, SEEK_SET);
		BYTE* buff = new BYTE[Index[i].FileSize];
		fread(buff, Index[i].FileSize, 1, fp);
		fwrite(buff, Index[i].FileSize, 1, fout);
		fclose(fout);
		delete[] buff;

		cout << "unpacked:" << Index[i].FileName << endl;
	}

	delete[] Index;
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
