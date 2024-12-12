// extnut.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include  <Windows.h>
using namespace std;

void Exec(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	if (!fp)
	{
		MessageBox(0, L"找不到文件。", 0, 0);
		return;
	}
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	if (buff)
	{
		fread(buff, size, 1, fp);
		fclose(fp);
		string Old(FileName);
		string NewFileName = Old + ".txt";
		FILE* fp2 = fopen(NewFileName.c_str(), "wb");
		if (fp2)
		{
			buff += 832;
			for (int i = 0; i < size; i += 4)
			{
				DWORD code = *(DWORD*)buff;
				buff += 4;
				if (code == 0x8000010)
				{
					DWORD len = *(DWORD*)buff;
					if (len != 0)
					{
						buff += 4;
						fwrite(buff, len, 1, fp2);
						fwrite("\n", 1, 1, fp2);
						buff += len;
					}
				}
			}
			fclose(fp2);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		//cout << "Usage:" << argv[0] << " File" << endl;
		MessageBox(0, L"exnut.exe <input nut file>", L"AyamiKaze", 0);
		//return -1;
	}
	//char* FILENAME = argv[1];
	char* FILENAME = (char*)"a0049h_01.nut";
	Exec(FILENAME);
	//system("pause");
	cout << "successful" << endl;
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
