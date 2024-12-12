#include "stdafx.h"
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

using std::fstream;
using std::string;
using std::vector;


vector<string> TextPool;

static int count = 0;
static unsigned int Number = 0;

//¡ð
static unsigned char Mark1[3] = { 0xE2, 0x97, 0x8B };
//¡ñ
static unsigned char Mark2[3] = { 0xE2, 0x97, 0x8F };
//¡ø
static unsigned char Mark3[3] = { 0xE2, 0x96, 0xB2 };


struct Info
{
	string Line1;
	string Line2;
	string Line3;

	void Clear()
	{
		Line1.clear();
		Line2.clear();
		Line3.clear();
	}
};

Info LineInfo;

int main(int argc, char* argv[])
{
	if (argc != 2)
		return -1;
	unsigned int iPos = 0;
	fstream fin(argv[1]);
	string outputFile(argv[1]);
	outputFile += ".txt";

	bool flag = false;
	string ReadLine;
	string LastLine;
	while (getline(fin, ReadLine))
	{
		string tmp = ReadLine;
		if (!strncmp(tmp.c_str(), (const char*)Mark2, 3))
		{
			string tmp2 = tmp.substr(14, tmp.length());
			LastLine = tmp2;
			continue;
		}
		else if(!strncmp(tmp.c_str(), (const char*)Mark3, 3))
		{
			string tmp2 = tmp.substr(14, tmp.length());
			if (tmp2.length())
			{
				ReadLine = tmp2;
			}
			else
			{
				ReadLine = LastLine;
			}
		}
		else
		{
			continue;
		}

		/*****************/
		if (ReadLine[0] == '/' && ReadLine[1] == '/')
		{
			continue;
		}
		//else if (ReadLine.size() == 0)
		//{
		//	continue;
		//}
		else if (!strncmp(ReadLine.c_str(), "BG_", 3))
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "CG_", 3))
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "DG_", 3))
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "EF_", 3))
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "FC_", 3))
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "SE_", 3))
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "KM_", 3))
		{
			continue;
		}
		//LB_
		else if (!strncmp(ReadLine.c_str(), "LB_", 3))
		{
			continue;
		}
		else if (ReadLine[0] == '*' && ReadLine[1] == '*' && ReadLine[2] == '*')
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "EF_FLAG_", 8))
		{
			continue;
		}
		else if (!strcmp(ReadLine.c_str(), "EF_NOP_1"))
		{
			continue;
		}
		else if (!strcmp(ReadLine.c_str(), "EF_FOUT"))
		{
			continue;
		}
		//EF_C0092_0_-1
		else if (!strncmp(ReadLine.c_str(), "EF_C", 4))
		{
			continue;
		}
		else if (!strncmp(ReadLine.c_str(), "MV_", 3))
		{
			continue;
		}
		//EF_WAIT_
		else if (!strncmp(ReadLine.c_str(), "EF_WAIT_", 8))
		{
			continue;
		}
		else
		{
			int count = 0;
			for (auto it : ReadLine)
			{
				if (it < 0x7F)
				{
					count++;
				}
			}
			TextPool.push_back(ReadLine);
			if (count >3)
			{
				flag = true;
				WCHAR Info[1024] = { 0 };
				MultiByteToWideChar(CP_UTF8, 0, ReadLine.c_str(), ReadLine.length(), Info, 1024);
				wprintf(L"%s\n", Info);
			}
		}
		ReadLine.clear();
	}
	fin.close();

	FILE* f = fopen(outputFile.c_str(), "wb");
	for (auto it : TextPool)
	{
		fprintf(f, "%s\r\n", it.c_str());
	}
	fclose(f);

	if (flag)
	{
		getchar();
	}
	return 0;
}

