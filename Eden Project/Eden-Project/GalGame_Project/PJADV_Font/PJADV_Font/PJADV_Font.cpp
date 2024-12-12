// PJADV_Font.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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


int main()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "MEM_0C99979E_0000034B.mem", "rb");
	if (!fp)
		return E("No fp");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE* buff = new BYTE[size];
	fread(buff, size, 1, fp);
	fclose(fp);


	BYTE* dbuff = new BYTE[size];
	memset(dbuff, 0, size);
	for (int i = 0; i < size;i++)
	{
		dbuff[i] = buff[i] << 24 | 0xFFFFFF;
	}

	fopen_s(&fp, "dump.bin", "wb");
	if (!fp)
		return E("No fp2");
	fwrite(dbuff, size, 1, fp);
	fclose(fp);

	delete[] buff;
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
