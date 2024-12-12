// EdenVM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>
using namespace std;
#pragma pack(1)
struct TextStruct
{
	char raw[128];
	char str[128];
};
#pragma pack(1)
typedef struct head
{
	char Header[25];
	DWORD Length;
	DWORD flag;
}EdenHeader;
#pragma pack()

unsigned long long MurmurHash64B(const void* key, int len, unsigned int seed = 0xEE6B27EB)
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
char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}
int CountLines(char* filename)//获取文件的行数
{
	ifstream file;
	int n = 0;
	string temp;
	file.open(filename, ios::in);//ios::in 表示以只读的方式读取文件
	if (file.fail())//文件打开失败:返回0
	{
		return 0;
	}
	else//文件存在,返回文件行数
	{
		while (getline(file, temp))
		{
			n++;
		}
		file.close();
		return n;
	}

}


void EdenReadFile(char* FileName)
{
	auto strnum = CountLines(FileName);
	auto textbuf = new TextStruct[strnum];
	cout << "LOAD:" << strnum << " lines from file" << endl;
	ifstream fin(FileName);
	const int LineMax = 10000;
	char str[LineMax];
	if (fin.is_open())
	{
		cout << "LOAD:" << "Load start." << endl;
		int counter = 0;
		int rawsize = 0;
		int chssize = 0;
		while (fin.getline(str, LineMax))
		{
			auto wtmp = ctowUTF(str);
			wstring wline = wtmp;
			if (wline[1] == (wchar_t)L'/' && wline[1] == (wchar_t)L'/')
			{
				wline = wline.substr(2);
				//cout << "RAW:" << wtocGBK(wline.c_str()) << endl;
				//wcscpy(textbuf[counter].raw, wline.c_str());
				strcpy(textbuf[counter].str, wtocGBK(wline.c_str()));
				rawsize++;
			}
			else
			{
				//cout << "CHS:" << wtocGBK(wline.c_str()) << endl;
				//wcscpy(textbuf[counter].str, wline.c_str());
				strcpy(textbuf[counter].str, wtocGBK(wline.c_str()));
				chssize++;
			}
			counter++;
			/*
			wstring crcval = wline.substr(2, 8);
			wstring wstr = wline.substr(11);
			DWORD crc = wcstoul(crcval.c_str(), NULL, 16);
			textbuf[counter].crc32val = crc;
			wcscpy(textbuf[counter].str, wstr.c_str());
			counter++;
			*/
		}
		cout << "LOAD:" << counter << " Recoders." << endl;
		if (rawsize != chssize)
		{
			cout << "rawsize != chssize" << endl;
			cout << rawsize << " | " << chssize << endl;
			return;
		}
		fstream fileout;
		fileout.open("EdenScript.evm", ios::binary | ios::out);
		EdenHeader hdr;
		strcpy(hdr.Header, "EdenFileSystemVer1.00");
		fileout.write((hdr.Header), sizeof(hdr.Header));
		fileout.write((char*)textbuf, (strnum)*sizeof(TextStruct));
		fileout.close();
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage:" << argv[0] << " File" << endl;
		return 0;
	}
	char* FILENAME = argv[1];
	//Cryption(FILENAME);
	//Cryption2(FILENAME);
	EdenReadFile(FILENAME);
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
