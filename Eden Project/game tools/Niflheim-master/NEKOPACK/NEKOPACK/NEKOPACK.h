#define _CRT_SECURE_NO_WARNINGS
#define _USE_32BIT_TIME_T
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream>
#include <map>
#include <algorithm>
#include <Windows.h>
#include <direct.h>
#include <io.h>

using namespace std;

#pragma pack(1)
typedef struct dat_header_s
{
	char magic[8];//NEKOPACK
	DWORD seed;//��Կ
	DWORD order;//һ��Ϊ0�������ֵԽ��ķ��������е��ļ���Խ���ȶ�ȡ�������������ֵ���ʱ��dat�ļ��Ķ�ȡ˳���ȡ
	DWORD parity;//��length��seed������ɣ�0��������
	DWORD index_length;
} dat_header_t;

typedef struct file_s
{
	DWORD file_name_hash;
	DWORD file_length;
	DWORD parity;
	DWORD length;//��file_lengthһ��
	string name;
} file_t;

typedef struct findex_s
{
	DWORD dir_name_hash;
	DWORD filenum;
	string name;
	vector<file_t> files;
} findex_t;
#pragma pack()

class NEKOPACK
{
public:
	DWORD filenum = 0;
	NEKOPACK(string datname, string dirname, string filename);
	NEKOPACK(string pathname);
	~NEKOPACK();
	void GetKey(DWORD& key0, DWORD& key1, DWORD hash);
	void Decode(BYTE* buf, DWORD len, WORD* key);
	void Encode(BYTE* buf, DWORD len, WORD* key);
	DWORD GetNameHash(DWORD hash, char* name);
	DWORD ParityCheck(DWORD key0, DWORD key1);
	bool ReadIndex(string datname);
	void AddExtraName(string txt, string type);
	void GetFiles(string name, DWORD indexsize);
	bool Export();
	bool Import();
	void FileSort();
	bool Make();
	vector<findex_t> findexs;

private:
	ifstream src;
	dat_header_t dat_header;
	string dirname;
	bool Index_OK;
	static BYTE name_table[];
	static vector<string> dir_names;
	static vector<string> file_names;
};