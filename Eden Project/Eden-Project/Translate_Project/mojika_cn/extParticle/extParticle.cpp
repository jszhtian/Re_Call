// extParticle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

int E(const char* msg)
{
	MessageBoxA(0, msg, 0, 0);
	return -1;
}


/*
int main(int argc, char* argv[])
{
    //std::cout << "Hello World!\n";
	CHAR* FileName = argv[1];//(CHAR*)"フェス文字00_01.particle";
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rb");
	if (!fp)
		return E("No File");
	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	CHAR* buff = new CHAR[size];
	fread(buff, size, 1, fp);
	fclose(fp);

	CHAR fnm[MAX_PATH] = { 0 };
	wsprintfA(fnm, "%s.txt", FileName);
	fopen_s(&fp, fnm, "wb");
	if (!fp)
		return E("No File2");

	CHAR txt[0x1000] = { 0 };
	for (int i = 0; i < size;)
	{
		if (buff[i] == 'C' && buff[i + 1] == 'H' && buff[i + 2] == 'A' && buff[i + 3] == 'R' && buff[i + 4] == ':')
		{
			fprintf_s(fp, "%08x|%s\r\n", i, buff + i);
			i += lstrlenA(buff);
		}
		else
			i++;
	}
	fclose(fp);
	//system("pause");
	return 0;
}
*/

map<uint32_t, string>TextList;


void InsertList(CHAR* fnm)
{
	ifstream fin(fnm);
	const int LineMax = 0x400;
	char str[LineMax];
	if (fin.is_open())
	{
		while (fin.getline(str, LineMax))
		{
			string wline = str;
			string numChr = wline.substr(0, 8);
			string mstr = wline.substr(9);
			DWORD num = strtoul(numChr.c_str(), NULL, 16);
			//cout << hex << num << "|" << mstr << endl;
			TextList.insert(pair<uint32_t, string>(num, mstr));
		}
	}
	else
	{
		MessageBox(0, L"File Open fail!", L"Error", MB_OK);
		exit(0);
	}
}

///*
int main(int argc, char* argv[])
{
	//std::cout << "Hello World!\n";
	CHAR* FileName = argv[1];//(CHAR*)"フェス文字00_01.particle";
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rb");
	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	CHAR* buff = new CHAR[size];
	fread(buff, size, 1, fp);
	fclose(fp);

	CHAR fnm[MAX_PATH] = { 0 };
	wsprintfA(fnm, "%s.txt\0", FileName);
	InsertList(fnm);

	for (auto& it : TextList)
	{
		if(lstrlenA(it.second.c_str()) <= 9)
			lstrcpyA(buff + it.first, it.second.c_str());
		else
			cout << fnm << ":" << it.second.c_str() << endl;
	}


	wsprintfA(fnm, "%s.new\0", FileName);

	fopen_s(&fp, fnm, "wb");
	fwrite(buff, size, 1, fp);
	fclose(fp);

	//system("pause");
	return 0;
}
//*/
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
