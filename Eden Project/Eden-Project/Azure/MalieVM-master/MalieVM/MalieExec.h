﻿#pragma once
//#include "Malie_VMParse.h"
#include <windows.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "MalieCHS.h"

using namespace std;

typedef enum _SIGNATURE
{
	TO_VOL=L'▲',//0x0708 d
	EOVOL=L'△',//0x0709 d
	STRUB=L'▼',//0x0A d
	EOSTR=L'▽',//0x00 d
	EOPAR=L'◁',//0x06
	NXL=L'◆',//0x0704 d
	TO_RTN=L'◎',//0x0706 d
	TO_RUB=L'★',//0x0701 d
	NAME_SIG=L'※',//0x07
	UNKNOW_SIG = L'⊙',
	CH_HJN = L'∴', // d
};

typedef struct _STRING_INFO 
{
	DWORD offset;
	DWORD length;
	_STRING_INFO(DWORD offset=0,DWORD length=0)
		:offset(offset),length(length)
	{

	}
} STRING_INFO, *PSTRING_INFO;

typedef struct _VM_FUNCTION
{
	wstring wstrName;
	DWORD dwID;
	DWORD dwReserved0;
	DWORD dwVMCodeOffset;
	DWORD dwReserved1;
} VM_FUNCTION, *PVM_FUNCTION;

typedef struct _MALIE_LABEL
{
	wstring wstrName;
	DWORD dwVMCodeOffset;
} MALIE_LABEL, *PMALIE_LABEL;

class binfstream
{
public:
	binfstream(LPCSTR lpFileName, int iReadWrite=OF_READ)
		:hFile(0),cbFile(0)
	{
		switch (iReadWrite)
		{
		case OF_READ:
			hFile = _lopen(lpFileName,iReadWrite);
			cbFile = _llseek(hFile,0,FILE_END);
			_llseek(hFile,0,FILE_BEGIN);
			break;
		case OF_WRITE:
			hFile = _lcreat(lpFileName,0);
			break;
		default:
			break;
		}
	}
	~binfstream()
	{
		close();
	}

	bool close()
	{
		if (hFile)
		{
			_lclose(hFile);
			return false;
		}
		return false;
	}

	virtual size_t read(void *dest,size_t cbRead)
	{
		return _lread(hFile,dest,cbRead);
	}

	virtual size_t write(void *src,size_t cbWirte)
	{
		return _lwrite(hFile,(LPCCH)src,cbWirte);
	}

	bool eof()
	{
		return _llseek(hFile,0,FILE_CURRENT)==cbFile;
	}

	bool is_open()
	{
		return hFile!=-1;
	}

	size_t seek(LONG lOffset,int iOrigin)
	{
		return _llseek(hFile,lOffset,iOrigin);
	}

	size_t GetFileSize()
	{
		return cbFile;
	}
protected:
	size_t cbFile;
	HFILE hFile;
};

class execstream : public binfstream
{
public:
	execstream(LPCSTR lpFileName, int iReadWrite=OF_READ)
		:binfstream(lpFileName,iReadWrite)
	{

	}

	DWORD readdw()
	{
		DWORD val;
		read(&val,sizeof(DWORD));
		return val;
	}

	WORD readw()
	{
		WORD val;
		read(&val,sizeof(WORD));
		return val;
	}

	BYTE readb()
	{
		BYTE val;
		read(&val,sizeof(BYTE));
		return val;
	}
};


// #pragma pack(push)
// #pragma pack(1)
// typedef struct _vCall
// {
// 	BYTE cCode;
// 	DWORD dwFuncId;
// } vCall;
// #pragma pack(pop)

class MalieExec
{
public:
	MalieExec(char *lpFileName);
	~MalieExec(void);
private:
	map<wstring,MALIE_LABEL> labelList;
	map<wstring,VM_FUNCTION> funcList;
	vector<VM_FUNCTION> vecFuncList;
	vector<STRING_INFO> vStrIndex;
	//CMalieCHS dbCHS;
	unsigned char *pStrTable;
	unsigned char *pVM_CODE;
	unsigned char *pVM_DATA;

	DWORD szVM_CODE;
	DWORD szVM_DATA;
	DWORD szStrTable;
	DWORD cntStrIndex;
	
	size_t offStrTable;

	void GetNext(execstream *bin)
	{
		DWORD dwFlag = bin->readdw();
		if (dwFlag)
		{
			bin->seek(sizeof(DWORD),FILE_CURRENT);
			GetNext(bin);
		}
		return;
	}
public:
	DWORD GetFuncOffset(wstring funcName);
	DWORD GetFuncOffset(size_t funcId);
	wstring GetFuncName(size_t funcId);
	int GetFuncId(wstring funcName);
	wstring ParseString(DWORD dwIndex);
	wstring ImportString(DWORD dwIndex, wstring chsLine);
	pair<vector<STRING_INFO>, wstring> RebuildStringSection(CMalieCHS &db);
	int RebuildVMBinary(CMalieCHS &scene, char *lpInFile, char *lpOutFile);
	int ExportStrByCode(void);
	unsigned char * GetVMCodeBase(void);
	unsigned char * GetVMDataBase(void);
};
//static FILE* fp = fopen("Script2.txt", "wb");
static ofstream fout("Script2.txt");
static ofstream fout2("Script3.txt");