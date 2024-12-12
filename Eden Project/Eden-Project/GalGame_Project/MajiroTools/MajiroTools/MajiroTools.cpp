// MajiroTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include  <Windows.h>
#include "MjoKey.h"
using namespace std;
typedef struct MjoHeader {
	char magic[16];		/* "MajiroObjX1.000" */
	unsigned int default_entry;	//默认入口点，通常为最后一个函数
	unsigned int unknown0;
	unsigned int unknown1;
	unsigned int entries;		/* 8 bytes per entry */
	unsigned char* entries_buffer;
	unsigned int data_length;
	unsigned char* data_buffer;
} mjo_header_t;
typedef struct {		/* 接下来0x300字节是调色版 */
	char magic[16];		/* "MajiroObjV1.000" */
	unsigned int unknown0;
	unsigned int unknown1;
	unsigned int entries;		/* 8 bytes per entry */
	unsigned int* entries_buffer;
	DWORD data_length;
	char* data_buffer;
} mjoV_header_t;
void* __cdecl Majiro_Malloc(int a1)
{
	void* result; // eax@1
	void* v2; // esi@1

	result = malloc(a1 + 32);
	v2 = result;
	if (!result)
	{
		MessageBoxA(0, "儊儌儕梕検偑晄懌偟偰偄傑偡丅張棟傪拞抐偟傑偡丅", "malloc", 0x10u);
		result = v2;
	}
	return result;
}
char __cdecl MajiroObjX_Decrypt(unsigned int a1, unsigned int a2)
{

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
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE* buff = (BYTE*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	mjo_header_t mjo;
	mjo.unknown0 = *(DWORD*)(buff + 16);
	buff += 20;

	mjo.unknown1 = *(DWORD*)buff;
	buff += 4;
	mjo.entries = *(DWORD*)buff;
	buff += 4;
	mjo.entries_buffer = buff;
	buff += mjo.entries * 8;
	mjo.data_length = *(DWORD*)buff;
	buff += 4;
	mjo.data_buffer = buff;

	BYTE* write_buff = mjo.data_buffer;
	DWORD write_size = mjo.data_length;
	cout << "读取成功" << endl;
	cout << "Size of file:" << size << endl;
	cout << "Size of data_buffer:0x" << hex << mjo.data_length << endl;
	for (unsigned int i = 0; i < mjo.data_length; i++)
		mjo.data_buffer[i] ^= MjoKey[i & (sizeof(MjoKey) - 1)];

	cout << "解密完成" << endl;
	cout << "Size of Decrypt_buff:0x" << hex << write_size << endl;
	char* dst = FileName;
	strcat(dst, ".dec");
	mjoV_header_t mjov;
	strcpy(mjov.magic, "MajiroObjV1.000");
	fp = fopen(dst, "wb");
	fwrite(mjov.magic, sizeof(mjov.magic), 1, fp);
	fwrite(&mjo.unknown0, sizeof(mjo.unknown0), 1, fp);
	fwrite(&mjo.unknown1, sizeof(mjo.unknown1), 1, fp);
	fwrite(&mjo.entries, sizeof(mjo.entries), 1, fp);
	fwrite(&mjo.entries_buffer, sizeof(mjo.entries_buffer), 1, fp);
	fwrite(&mjo.data_length, sizeof(mjo.data_length), 1, fp);
	fwrite(mjo.data_buffer, mjo.data_length, 1, fp);
	fclose(fp);
	//MessageBox(0, L"读取成功", 0, 0);
	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		MessageBox(0, L"Usage: MajiroTools.exe <InputFile.mjo>", 0, 0);
		return -1;
	}
	char* FILENAME = argv[1];
	bool succ=Main2(FILENAME);
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
