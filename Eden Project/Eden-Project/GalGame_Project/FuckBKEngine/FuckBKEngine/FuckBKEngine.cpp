// FuckBKEngine.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

int sub_466E10(int a1, int a2, int a3, int a4)
{
	int v4; // edi
	int v5; // esi
	int v6; // eax
	int v7; // ebx
	unsigned __int8 v8; // dl
	int v9; // eax
	bool v10; // zf
	int v12; // [esp+8h] [ebp-4h]

	v4 = a3;
	v5 = a2;
	if (!a3)
		return a4;
	v6 = a1 - a2;
	v7 = a4;
	v12 = a1 - a2;
	do
	{
		v8 = *(BYTE*)(v6 + v5++);
		v9 = v4-- * v8;
		v7 = v9 + 0x5D588B65 + (v8 ^ v7);
		v6 = v12;
		v10 = a3-- == 1;
		*(BYTE*)(v5 - 1) = v7 ^ v8;
	} while (!v10);
	return v7;
}

#pragma pack (1)
typedef struct {
	char Magic[8];//BKARC\x04
	unsigned int  xorkey1;//v17
	unsigned int  xorkey2;//v18
	unsigned int  xorkey3;//v19
	unsigned int  zero1;//0
}bkarc_header_t;
typedef struct
{
	unsigned int  xorkey4;//nNumberOfBytesToRead
	unsigned int  xorkey5;//v22
	unsigned int  xorkey6;//unk
	unsigned int  zero2;//unk
}bkarc_header2_t;

#pragma pack ()
int main(int argc, char* argv[])
{
	bkarc_header_t header;
	bkarc_header2_t header2;
	if (argc != 2)
	{
		MessageBox(0, L"FuckBKEngine.exe <input bkarc file>", L"AyamiKaze", 0);
		return -1;
	}
	char* FileName = argv[1];
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread(&header, sizeof(header), 1, fp);
	cout << "Magic:" << header.Magic << endl;
	cout << "xorkey1:0x" << hex << header.xorkey1 << endl;
	cout << "xorkey2:0x" << hex << header.xorkey2 << endl;
	cout << "xorkey3:0x" << hex << header.xorkey3 << endl;
	cout << "zero1:0x" << hex << header.zero1 << endl;

	unsigned int  pos =  header.xorkey1 ^ ((header.xorkey2 ^ ((header.xorkey3 ^ header.xorkey1 & 0xFFFF00) - 0x6C078965)) - 0x5E89F12A);

	cout << "pos:0x" << hex << pos << endl;

	fseek(fp, pos + 0x10, SEEK_SET);
	fread(&header2, sizeof(header2), 1, fp);
	cout << "xorkey4:0x" << hex << header2.xorkey4 << endl;
	cout << "xorkey5:0x" << hex << header2.xorkey5 << endl;
	cout << "xorkey6:0x" << hex << header2.xorkey6 << endl;
	cout << "zero2:0x" << hex << header2.zero2 << endl;
	unsigned int  index_uncompr_len = header.xorkey1 ^ ((header.xorkey2 ^ ((header2.xorkey5 ^ header.xorkey1 & 0xFFFF00) - 0x6C078965)) - 0x5E89F12A);
	unsigned int  index_compr_len = header.xorkey1 ^ ((header.xorkey2 ^ ((header2.xorkey4 ^ header.xorkey1 & 0xFFFF00) - 0x6C078965)) - 0x5E89F12A);
	cout << "index_uncompr_len:0x" << hex << index_uncompr_len << endl;
	cout << "index_compr_len:0x" << hex << index_compr_len << endl;
	BYTE* index_compr_buff = (BYTE*)malloc(index_compr_len);
	if (!index_compr_buff)
		return E("NOT index_compr_buff");
	fread(index_compr_buff, index_compr_len, 1, fp);
	sub_466E10((int)index_compr_buff, (int)index_compr_buff, index_compr_len, header2.xorkey6);

	fclose(fp);
	fp = fopen("index_dump.dmp", "wb");
	fwrite(index_compr_buff, index_compr_len, 1, fp);
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
