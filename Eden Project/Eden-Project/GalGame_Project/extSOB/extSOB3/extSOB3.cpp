// extSOB3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
typedef struct SOB_HDR {
	char magic[4];		/* "SOB0" */
	unsigned int szOfRelocateTbl;
} SOB_HDR_t;

typedef struct SOB_CODE {
	unsigned int insts_size;
	unsigned int code_size;
}SOB_CODE_t;
#pragma pack ()

int main(int argc, char* argv[])
{
	SOB_HDR_t hdr;
	SOB_CODE_t cde;
	char FileName[] = "s01_02a.sob";
	auto fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	unsigned int szOfFile = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread(&hdr, sizeof(SOB_HDR_t), 1, fp);
	if (strncmp(hdr.magic, "SOB0", 4))
		return E("Not SOB0");
	cout << "------------" << FileName << "------------" << endl;
	cout << "szOfRelocateTbl:0x" << hex << hdr.szOfRelocateTbl << endl;

	char* RelocateTbl = (char*)malloc(hdr.szOfRelocateTbl);
	fread(RelocateTbl, hdr.szOfRelocateTbl, 1, fp);

	fseek(fp, sizeof(SOB_HDR_t) + hdr.szOfRelocateTbl, SEEK_SET);
	fread(&cde, sizeof(SOB_CODE_t), 1, fp);
	cout << "insts_size:0x" << hex << cde.insts_size << endl;
	cout << "code_size:0x" << hex << cde.code_size << endl;

	char* code = (char*)malloc(cde.code_size);
	fread(code, cde.code_size, 1, fp);

	auto dmp = fopen("dump.dmp", "wb");
	fwrite(code, cde.code_size, 1, dmp);
	fclose(dmp);

	cout << "------------" << FileName << "------------" << endl;
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
