#pragma once
#include <iomanip>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
extern ofstream TXT;
extern FILE* fp;

static bool IsHalf(const string& s)
{
	for (auto c : s)
	{
		if ((unsigned char)c >= 0x80)
		{
			return false;
		}
	}
	return true;
}

static bool IsSystem(char* str, int len)
{
	if (str[0] == 's' && str[1] == 'e' ||
		str[0] == 'S' && str[1] == 'E' ||
		str[0] == 'E' && str[1] == 'V' ||
		str[0] == 'e' && str[1] == 'v' ||
		str[0] == '<' && str[1] == 'v' ||
		str[0] == 'c' && str[1] == 'g' ||
		str[0] == 'C' && str[1] == 'r' && str[2] == 'e' && str[3] == 'a' && str[4] == 't' && str[5] == 'e' ||
		str[0] == 'R' && str[1] == 'a' && str[2] == 'n' && str[3] == 'd' && str[4] == 'o' && str[5] == 'm' ||
		str[0] == 'S' && str[1] == 'c' && str[2] == 'a' || 
		str[0] == 'S' && str[1] == 'e' && str[2] == 't' ||
		str[0] == 'M' && str[1] == 'o' && str[2] == 'v')
	{
		return true;
	}
	/*
	for (int i = 0; i < len; i++)
	{
		if (str[i] == 's' && str[i + 1] == 'e' ||
			str[i] == 'S' && str[i + 1] == 'E' ||
			str[i] == 'E' && str[i + 1] == 'V' ||
			str[i] == 'e' && str[i + 1] == 'v' ||
			str[i] == '<' && str[i + 1] == 'v' ||
			str[i] == 'c' && str[i + 1] == 'g')
		{
			return true;
		}
	}
	*/
	return false;
}

static void WriteText(std::string& str)
{
	if (/*!IsHalf(str) && */!IsSystem((char*)str.c_str(), strlen(str.c_str())) && strlen(str.c_str())!=0)
	{
		/*
		fwrite("########################################\n", 41, 1, fp);
		fwrite(str.c_str(), strlen(str.c_str()), 1, fp);
		fwrite("\n########################################\n", 42, 1, fp);
		fwrite("****************************************\n", 41, 1, fp);
		fwrite(str.c_str(), strlen(str.c_str()), 1, fp);
		fwrite("\n****************************************\n\n", 43, 1, fp);
		*/
		fwrite(str.c_str(), strlen(str.c_str()), 1, fp);
		fwrite("\n", 1, 1, fp);
	}
}

static void mWriteText(char* str)
{
		fwrite("########################################\n", 41, 1, fp);
		fwrite(str, strlen(str), 1, fp);
		fwrite("\n########################################\n", 42, 1, fp);
		fwrite("****************************************\n", 41, 1, fp);
		fwrite(str, strlen(str), 1, fp);
		fwrite("\n****************************************\n\n", 43, 1, fp);
}