// mText.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <windows.h>
#include <vector>
#include <iomanip>
using namespace std;
unsigned long long MurmurHash64B(const void* key, int len, unsigned int seed = 0xABC84D09)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h1 = seed ^ len;
	unsigned int h2 = 0;

	const unsigned int* data = (const unsigned int*)key;

	while (len >= 8)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;

		unsigned int k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}

	if (len >= 4)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}

	switch (len)
	{
	case 3: h2 ^= ((unsigned char*)data)[2] << 16;
	case 2: h2 ^= ((unsigned char*)data)[1] << 8;
	case 1: h2 ^= ((unsigned char*)data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	unsigned long long h = h1;

	h = (h << 32) | h2;

	return h;
}

LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}
char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}


int main(int argc, char* argv[])
{
	unsigned long long murmurhash_num;
	if (argc != 2)
	{
		cout << "Usage:" << argv[0] << " TxtFile" << endl;
	}
	vector<DWORD> murlist;
	ifstream fin(argv[1]);
	const int LineMax = 0x1000;
	char str[LineMax];
	vector<wstring>rawlist;
	vector<wstring>newlist;
	if (fin.is_open())
	{
		int counter = 0;
		while (fin.getline(str, LineMax))
		{
			auto tmp = ctowUTF(str);
			wstring wline = tmp;
			if (wline[1] == (wchar_t)L'/' && wline[1] == (wchar_t)L'/')
			{
				wline = wline.substr(2);
				rawlist.push_back(wline);
			}
			else
			{
				newlist.push_back(wline);
			}
		}
		fin.close();
	}
	size_t index = 0;
	unsigned long long mur;
	for (index; index < rawlist.size(); ++index)
	{
		DWORD murval = MurmurHash64B((char*)rawlist.at(index).c_str(), rawlist.at(index).length());
		murlist.push_back(murval);
	}
	string namefile = argv[1];
	ofstream fout("COKEZIGE.SCN");
	for (int i = 0; i < murlist.size(); i++)
	{
		fout << "0x" << hex << setw(8) << setfill('0') << murlist[i] << "|" << wtocUTF(newlist[i].c_str()) << endl;
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
