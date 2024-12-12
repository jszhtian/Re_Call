#pragma once
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


#define IMPORT_SIGNATURE L'◆'
#define CHS_SIGNATURE L'●'

class CMojiCHS
{
public:
	CMojiCHS(const TCHAR *pFileName, bool bSaveIndex = false, bool bParseIndex = true);
	~CMojiCHS(void);
private:
	vector<wstring> vecString;
	vector<wstring> vecIndex;
	vector<int> vIdx;
	bool bParseIndex;
public:
	wstring GetString(int index);
	wstring GetIndexDescr(int index)
	{
		return vecIndex[index];
	}
	size_t GetDictSize(void)
	{
		return vecString.size();
	}
	map<int, wstring> GetDB()
	{
		map<int, wstring> db;
		size_t lines = vIdx.size();
		for (size_t i = 0; i < lines; ++i)
		{
			db[vIdx[i]] = vecString[i];
		}
		return move(db);
	}
};

static char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}
static char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}