#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <direct.h>
#include <png.h>

using namespace std;

#pragma pack(1)

typedef struct pgd32_header_s
{
	char maigc[2];//GE
	WORD sizeof_header;//32
	DWORD orig_x;//ͼ�󶥵���������ʾ�����λ��
	DWORD orig_y;
	DWORD width;
	DWORD height;
	DWORD orig_width;//ͼ��������ʾ��������ʾ�Ŀ��
	DWORD orig_height;//ͼ��������ʾ��������ʾ�ĸ߶�
	WORD compr_method;
	WORD unknown;
} pgd32_header_t;

typedef struct pgd32_info_s
{
	DWORD uncomprlen;
	DWORD comprlen;//������Info��Ϣ���ڵ�8�ֽ�
} pgd32_info_t;

typedef struct ge_header_s
{
	WORD unknown;//7
	WORD bpp;//32 or 24
	WORD width;
	WORD height;
} ge_header_t;

#pragma pack()

class PGD
{
public:
	PGD(string pgdname);
	bool pgd_compress();
	~PGD();
private:
	bool png2raw(BYTE *TexData);
	bool ReadHeader(string pgdname);
	void _pgd_uncompress32(BYTE *compr, BYTE *uncompr, DWORD uncomprlen);
	DWORD _pgd_compress32(BYTE *uncompr, DWORD uncomprlen, FILE *pgdfile);
	void _pgd3_ge_restore_24(BYTE *out, DWORD out_len, BYTE *__ge, DWORD __ge_length, WORD width, WORD height);
	void _pgd3_ge_restore_32(BYTE *out, DWORD out_len, BYTE *__ge, DWORD __ge_length, WORD width, WORD height);
	void pgd_ge_restore3(BYTE *out, DWORD out_len, BYTE *__ge, DWORD __ge_length, WORD width, WORD height, DWORD bpp);
	FILE* fp;
	bool Header_OK;
	string filename;
	pgd32_header_s pgd32_header;
	pgd32_info_t pgd32_info;
	ge_header_t ge_header;
};