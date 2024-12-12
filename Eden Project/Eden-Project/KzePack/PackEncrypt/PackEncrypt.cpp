// PackEncrypt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include  <Windows.h>
EXTERN_C
{
#include "xxhash.h"
}
using namespace std;



char* HeaderSig = (char*)"mdf";
typedef struct file_header_s
{
	char header[MAX_PATH];
	DWORD main_size;
	DWORD hash;
}file_header_t;


void Encrypt(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);

	file_header_t f_header;

	f_header.main_size = size;
	f_header.hash = XXH32(buff, size, 0x114514);
	strcpy(f_header.header, HeaderSig);
	int key = 0x114514 * strlen("ＮＭＳＬ～");
	for (int i = 0; i < size; i++)
	{
		buff[i] ^= key;
		buff[i] += f_header.hash;
		key -= f_header.hash;
	}
	fp = fopen("enc", "wb");
	fwrite(f_header.header, sizeof(f_header.header), 1, fp);
	fwrite(&f_header.main_size, sizeof(f_header.main_size), 1, fp);
	fwrite(&f_header.hash, sizeof(f_header.hash), 1, fp);
	fwrite(buff, size, 1, fp);
	fclose(fp);
	cout << hex << f_header.main_size << endl;
	cout << hex << f_header.hash << endl;
	free(buff);
}

void Decrypt(char* FileName)
{
	file_header_t f_header;
	DWORD main_size;
	DWORD hash,CheckHash;

	FILE* fp = fopen(FileName, "rb");
	fread(&f_header, sizeof(f_header), 1, fp);
	//HeaderSig:
	if (strcmp(f_header.header, "mdf"))
	{
		MessageBox(0, L"sig error", 0, 0);
		cout << f_header.header << endl;
		return;
	}
	main_size = f_header.main_size;
	hash = f_header.hash;
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp)- sizeof(f_header);
	fseek(fp, sizeof(f_header), SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	int key = 0x114514 * strlen("ＮＭＳＬ～");
	for (int i = 0; i < size; i++)
	{
		buff[i] -= f_header.hash;
		buff[i] ^= key;
		key -= f_header.hash;
	}
	//check
	CheckHash = XXH32(buff, size, 0x114514);
	if (CheckHash != hash || size != main_size)
	{
		MessageBox(0, L"Check error", 0, 0);
		cout << main_size << endl;
		cout << size << endl;
		cout << hash << endl;
		cout << CheckHash << endl;
		return;
	}
	fp = fopen("dec", "wb");
	fwrite(buff, size, 1, fp);
	fclose(fp);
	free(buff);
}

bool DecTest = true;
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage:" << argv[0] << " File" << endl;
		//return -1;
	}
	//char* FILENAME = argv[1];
	char* FILENAME = (char*)"TG0-06-1.png";
	Encrypt(FILENAME);
	system("pause");
	if (DecTest)
	{
		Decrypt((char*)"enc");
		system("pause");
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
