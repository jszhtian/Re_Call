// extGIGA_bin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}

void WriteString(FILE* fp, BYTE* str, DWORD len)
{
	fwrite(str, len, 1, fp);
	fwrite("\r\n", 2, 1, fp);
}
#define DEBUG
#define REBUILD

#ifdef DUMP
int main(int argc, char* argv[])
{
#ifdef RELEASE
	if (argc != 2)
	{
		MessageBox(0, L"extGIGA_bin.exe <input bin file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
#endif
#ifdef DEBUG
	char FileName[] = "00_01【プロローグ】.bin.txt.bin";
#endif
	auto fp = fopen(FileName, "rb");
	if (!fp)
		return E("No fp");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE* buff = (BYTE*)malloc(size);
	if (!buff)
		return E("No buff");
	fread(buff, size, 1, fp);
	fclose(fp);
	DWORD version = *(DWORD*)(buff);
	DWORD pos = 0;
	DWORD opsize = 0;
	if (version == 0)
	{
		pos = 4;
		opsize = *(DWORD*)(buff + pos);
		pos = opsize * 8 + 8;
	}
	else if (version == 1)
	{
		pos = 12;
		opsize = *(DWORD*)(buff + pos);
		pos = opsize * 8 + 16;
	}
	else
		return E("Not support version");
	DWORD count = *(DWORD*)(buff + pos);
	cout << "Find String Count:" << count << endl;
	pos += 4;
	char fnm[MAX_PATH];
	strcpy(fnm, FileName);
	strcat(fnm, ".txt");
	fp = fopen(fnm, "wb");
	do
	{
		WriteString(fp, (buff + pos), strlen((char*)(buff + pos)));
		pos += strlen((char*)(buff + pos)) + 1;
		count -= 1;
	} while (count);
	fclose(fp);
	cout << "fin" << endl;
	//system("pause");
	return 0;
}


#else
vector<char*>StringPool;
int main(int argc, char* argv[])
{
#ifdef RELEASE
	if (argc != 2)
	{
		MessageBox(0, L"extGIGA_bin.exe <input bin file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
#else
	char FileName[] = "00_01【プロローグ】.bin";
#endif
	char TXTName[MAX_PATH];

	//bin file
	auto fp = fopen(FileName, "rb");
	if (!fp)
		return E("No fp");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE* buff = (BYTE*)malloc(size);
	if (!buff)
		return E("No buff");
	fread(buff, size, 1, fp);
	fclose(fp);

	DWORD pos = 0;
	DWORD opsize = 0;
	pos = 4;
	opsize = *(DWORD*)(buff + pos);
	DWORD TableSize = opsize * 8 + 8;
	pos = TableSize;
	pos += 4;
	DWORD count = *(DWORD*)(buff + TableSize);
	while (count > 0)
	{
		pos += lstrlenA((char*)(buff + pos)) + 1;
		count--;
	}
	fseek(fp, pos, SEEK_SET);
	DWORD VMSize = size - pos;
	BYTE* VMBuff = (BYTE*)malloc(VMSize);
	memcpy(VMBuff, buff + pos, VMSize);
	//txt file
	strcpy(TXTName, FileName);
	strcat(TXTName, ".txt");
	ifstream txt(TXTName);
	char str[4096];
	if (!txt.is_open())
		return E("No txt");

	strcat(TXTName, ".bin");
	fp = fopen(TXTName, "wb");
	fwrite(buff, TableSize, 1, fp);
	fseek(fp, TableSize + 0x4, SEEK_SET);
	while (txt.getline(str, 4096))
	{
		StringPool.push_back(str);
		fwrite(str, strlen(str), 1, fp);
		fwrite("\x00", 1, 1, fp);
	}
	txt.close();
	cout << "TxtSize:0x" << StringPool.size() << endl;
	fwrite(VMBuff, VMSize, 1, fp);
	fseek(fp, TableSize, SEEK_SET);
	DWORD szofstr = StringPool.size();
	fwrite(&szofstr, 0x4, 1, fp);

	//return
	fclose(fp);
	system("pause");
	return 0;
}
#endif
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
