#pragma once
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>
#include <unordered_map>

//#include "zconf.h"
#include "zdict.h"
#include "zlib.h"
#include "zstd.h"
#include "zstd_errors.h"

using namespace std;

#define XOR_KEY "\r\n未经允许，禁止擅自拆解本文件\r\n"
#define XOR_KEY_LEN 33

#pragma pack(1)
struct aym_header
{
	char flag[23];
	unsigned int str_count;
};

struct aym_text_struct
{
	unsigned int off;
	unsigned int sz;
	unsigned int jmp;
};

struct aym_text_buff {
	unsigned int sz;
	WCHAR* txt;
};

struct bin_text_header 
{
	char flag[29];

	unsigned int HeaderSize;
	unsigned long long HeaderHash;
	char HeaderKey[XOR_KEY_LEN];

	unsigned long EntrySize;
	unsigned long long EntryHash;
	unsigned long EntryCompSize;

	unsigned long TextSize;
	unsigned long long TextHash;
	unsigned long TextCompSize;
};
#pragma pack()

class ReadTextFromAymScript
{
private:
	bin_text_header header;
	//aym_header t_header;
	aym_text_buff t_buf;
	unordered_map<unsigned int, aym_text_buff>AymScriptList;

	FILE* fp;
	unsigned int FileSize;
	unsigned int EntrySize;
	unsigned int TextSize;
	unsigned char* EntryBuff;
	unsigned char* TextBuff;

	void AymGetFileSize(FILE* fp);
	unsigned long long MurmurHash64B(const void* key, int len);
public:
	ReadTextFromAymScript();
	~ReadTextFromAymScript();

	bool InitAymScript();

	pair<WCHAR*, unsigned int> GetTextByOff(unsigned int off);
};