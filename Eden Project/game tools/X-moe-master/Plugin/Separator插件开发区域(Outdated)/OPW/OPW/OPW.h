#ifndef _OPW_
#define _OPW_

#include <Windows.h>
#include <cstdio>
#include <string>
#include <tchar.h>
#include <map>

using std::wstring;
using std::map;

enum CMDMask
{
	UsingUnicode = 0x1UL, //文件名会被转码,需要配合ForceXXX使用
	SaveTempData = 2,
	ForceCHSCP = 4, //源是gbk
	ForceJPCP = 8,  //源是sjis
	ExtractSingle = 16, //提取单独的文件
	UsingImageEx = 32 //强制混合32-bit的alpha通道
};


extern "C" _declspec (dllexport) void ShowInfo();

extern "C" _declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile);

extern "C" _declspec (dllexport) BOOL Engine(TCHAR *pszStr);

extern "C" _declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output = NULL, const TCHAR *args = NULL);



#endif
