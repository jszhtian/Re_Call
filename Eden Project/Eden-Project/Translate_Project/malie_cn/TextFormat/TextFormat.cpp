// TextFormat.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <codecvt>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windows.h>
#include <set>
#include <map>
using namespace std;

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

typedef enum _SIGNATURE
{
	TO_VOL = L'▲',//0x0708
	EOVOL = L'△',//0x0709
	STRUB = L'▼',//0x0A
	EOSTR = L'▽',//0x00
	EOPAR = L'◁',//0x06
	NXL = L'◆',//0x0704
	TO_RTN = L'◎',//0x0706
	TO_RUB = L'★',//0x0701
	NAME_SIG = L'※',//0x07
	UNKNOW_SIG = L'⊙',
};

int main()
{
	wifstream fin("Script.txt", ios::binary);
	//ofstream fout("Script2.txt");
	FILE* fp = fopen("Script2.txt", "wb");
	if (fin.fail())
	{
		MessageBox(0, L"Read File Failed.", L"ReadFile", 0);
		return 0;
	}
	// 	// apply BOM-sensitive UTF-16 facet
	fin.imbue(locale(fin.getloc(),new codecvt_utf16<wchar_t, 0x10ffff, consume_header>));
	wstring line;
	while (!getline(fin, line).eof())
	{

		line[line.size() - 1] = 0;
		for (int i = 0; i < line.size(); i++)
		{
			switch (line[i])
			{
			case TO_VOL:
				//line[i] = (wchar_t)L"\x07\x08";
				lstrcpy(&line[i], L"\x07\x08");
			case EOVOL:
				//line[i] = (wchar_t)L"\x07\x09";
				lstrcpy(&line[i], L"\x07\x09");
			case STRUB:
				//line[i] = (wchar_t)L"\x0A";
				lstrcpy(&line[i], L"\x0A");
			case EOSTR:
				//line[i] = (wchar_t)L"\x00";
				lstrcpy(&line[i], L"\x00");
			case NXL:
				//line[i] = (wchar_t)L"\x07\x04";
				lstrcpy(&line[i], L"\x07\x04");
			case TO_RTN:
				//line[i] = (wchar_t)L"\x07\x06";
				lstrcpy(&line[i], L"\x07\x06");
			case TO_RUB:
				//line[i] = (wchar_t)L"\x07\x01";
				lstrcpy(&line[i], L"\x07\x01");
			default:
				break;
			}
		}
		//fout << wtocUTF(line.c_str()) << endl;
		DWORD size = lstrlen(line.c_str()) * 2;
		//fwrite(wtocUTF(line.c_str()), size, 1, fp);
		//fwrite("\n", 1, 1, fp);
	}
	//fclose(fp);
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
