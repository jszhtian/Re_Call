// ProcREPFile.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#pragma pack(1)
struct TextStruct
{
	__int32 crc32val;
	wchar_t str[126];
};
#pragma pack(1)
struct head
{
	char flag[8];
	__int32 Length;
	__int32 Zero;
};
#pragma pack()
using namespace std;
LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}
int CountLines(char *filename)//获取文件的行数
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

int main()
{
	auto strnum = CountLines("REP.txt");
	auto textbuf = new TextStruct[strnum];
	cout << "LOAD:" << strnum << " lines from file" << endl;
	ifstream fin("REP.txt");
	const int LineMax = 0x400;
	char str[LineMax];
	if (fin.is_open())
	{
		cout << "LOAD:" << "Load start." << endl;
		int counter = 0;
		while (fin.getline(str, LineMax))
		{
			auto wtmp = ctowUTF(str);
			wstring wline = wtmp;
			wstring crcval = wline.substr(2, 8);
			wstring wstr = wline.substr(11);
			DWORD crc = wcstoul(crcval.c_str(), NULL, 16);
			textbuf[counter].crc32val = crc;
			wcscpy(textbuf[counter].str, wstr.c_str());
			counter++;
		}
		cout << "LOAD:" << counter << " Recoders." << endl;
		
	}
	fstream fileout;
	fileout.open("Comp.pck",ios::binary|ios::out);
	head* hdr=new head;
	hdr->Length = strnum;
	hdr->Zero = 0;
	strcpy(hdr->flag, "PACKED");
	fileout.write((char*)hdr, 0x10);
	fileout.write((char*)textbuf, strnum*sizeof(TextStruct));
	fileout.close();
    return 0;
}

