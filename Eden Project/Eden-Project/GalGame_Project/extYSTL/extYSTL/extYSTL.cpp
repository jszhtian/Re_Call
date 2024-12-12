// extYSTL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
struct YSTLHeader {
	CHAR* Magic;
	uint32_t Version;
	uint32_t YBNCount;
};
struct YSTLIndex {
	uint32_t YBNNum;
	uint32_t YSTSize;
};
#pragma pack ()

int main()
{
	FILE* fp = nullptr;
	fopen_s(&fp, "yst_list.ybn", "rb");
	if (!fp)
		return E("No file");
	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = new char[size];
	fread(buff, 1, size, fp);
	fclose(fp);
	cout << "read fin" << endl;

	YSTLHeader* header = (YSTLHeader*)buff;
	//if (memcmp(header->Magic, "YSTL", 4))
	//	return E("not header");
	cout << "version:" << header->Version << endl;
	cout << "count:" << header->YBNCount << endl;

	FILE* writer = nullptr;
	fopen_s(&writer, "list.txt", "wb");
	if (!writer)
		return E("no writer");

	YSTLIndex* index = nullptr;
	char num[MAX_PATH] = { 0 };
	for (uint32_t i = sizeof(YSTLHeader); i < size;)
	{
		index = (YSTLIndex*)(buff + i);
		i += sizeof(YSTLIndex);
		char* str = (buff + i);
		wsprintfA(num, "%d", index->YBNNum);
		fwrite(num, 1, lstrlenA(num), writer);
		fwrite("->", 1, 2, writer);
		fwrite(str, 1, index->YSTSize, writer);
		fwrite("\r\n", 1, 2, writer);

		i += index->YSTSize + 20;
	}
	fclose(writer);

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
