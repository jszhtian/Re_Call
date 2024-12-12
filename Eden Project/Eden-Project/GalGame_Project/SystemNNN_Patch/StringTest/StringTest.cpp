// StringTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <Windows.h>
using namespace std;

unordered_map<DWORD, string> mesMap;
unordered_map<DWORD, string> strMap;

static char* wtoc(LPCTSTR str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static LPWSTR ctow(char* str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(cp, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(cp, 0, str, -1, out, dwMinSize);//转换
	return out;
}

string ReplaceString(string& line)
{
	string out;
	for (int i=0;i<line.size();i++)
	{
		if (line[i] == '\\')
		{
			i++;
			if (line[i] == 'r')
				out += '\r';
			else if (line[i] == 'n')
				out += '\n';
			else
				out += line[i];
		}
		else
			out += line[i];
	}
	return out;
}

int main()
{
	ifstream fstm;
	fstm.open("01.txt.txt");
	if (fstm.is_open()) 
	{
		CHAR tmp[0x1000] = { 0 };
		vector<string> vec_str;

		while (true)
		{
			fstm.getline(tmp, 0x1000);
			int outNum = fstm.gcount();
			//cout << outNum << endl;
			if (outNum == 0)
				break;
			vec_str.push_back(string(tmp));
		}
		for (int i = 0; i < vec_str.size(); i++)
		{
			string line = vec_str.at(i);
			if (line.size() > 0) 
			{
				string header = line.substr(0,2);
				//cout << header << endl;
				if (header == "cm")
				{
					string num = line.substr(3, 4);
					DWORD mesNum = stoi(num.c_str());

					string str = line.substr(8);
					string c_str = ReplaceString(str);
					c_str = wtoc(ctow((CHAR*)c_str.c_str(), CP_UTF8), 936);
					//cout << mesNum << "|" << c_str << endl;
					mesMap.emplace(mesNum, c_str);
				}
				else if (header == "cs")
				{
					string num = line.substr(3, 4);
					DWORD strNum = stoi(num.c_str());

					string str = line.substr(8);
					string c_str = ReplaceString(str);
					c_str = wtoc(ctow((CHAR*)c_str.c_str(), CP_UTF8), 936);
					//cout << strNum << "|" << c_str << endl;
					strMap.emplace(strNum, c_str);
				}
			}
		}
		///*
		for (auto it = mesMap.begin(); it != mesMap.end(); it++)
		{
			cout << "mesMap:" << it->first << "|" << it->second << endl;
		}
		system("pause");
		for (auto it = strMap.begin(); it != strMap.end(); it++)
		{
			cout << "strMap:" << it->first << "|" << it->second << endl;
		}
		//*/
	}
	fstm.close();
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
