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
	UsingUnicode = 0x1UL, //�ļ����ᱻת��,��Ҫ���ForceXXXʹ��
	SaveTempData = 2,
	ForceCHSCP = 4, //Դ��gbk
	ForceJPCP = 8,  //Դ��sjis
	ExtractSingle = 16, //��ȡ�������ļ�
	UsingImageEx = 32 //ǿ�ƻ��32-bit��alphaͨ��
};


extern "C" _declspec (dllexport) void ShowInfo();

extern "C" _declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile);

extern "C" _declspec (dllexport) BOOL Engine(TCHAR *pszStr);

extern "C" _declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output = NULL, const TCHAR *args = NULL);



#endif
