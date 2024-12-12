#pragma once

#include <list>
#include <string>
using namespace std;

// pStart �Ի���


enum CMDMask
{
	UsingUnicode = 0x1UL, //�ļ����ᱻת��,��Ҫ���ForceXXXʹ��
	SaveTempData = 2,
	ForceCHSCP = 4, //Դ��gbk
	ForceJPCP = 8,  //Դ��sjis
	ExtractSingle = 16, //��ȡ�������ļ�
	UsingImageEx = 32 //ǿ�ƻ��32-bit��alphaͨ��
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
