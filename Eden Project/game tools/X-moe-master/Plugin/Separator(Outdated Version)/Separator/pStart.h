#pragma once

#include <list>
#include <string>
using namespace std;

// pStart 对话框


enum CMDMask
{
	UsingUnicode = 0x1UL, //文件名会被转码,需要配合ForceXXX使用
	SaveTempData = 2,
	ForceCHSCP = 4, //源是gbk
	ForceJPCP = 8,  //源是sjis
	ExtractSingle = 16, //提取单独的文件
	UsingImageEx = 32 //强制混合32-bit的alpha通道
};


class pStart
{

public:
	pStart();
	~pStart();


public:
	int ParseFile(CString &ccFile, CString &EngineName);
	int ParseDic (CString &ccDic, CString &EngineName);
	HMODULE LoadDLL(CString &ccFile, CString &EngineName);

private:
	HMODULE libPtr;

};
