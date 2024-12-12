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
	bool pgd_uncompress();
	bool ge2png(BYTE *out);
	~PGD();
private:
	bool ReadHeader(string pgdname);
	void _pgd_uncompress32(BYTE *compr, BYTE *uncompr, DWORD uncomprlen);
	void _pgd3_ge_process_24(BYTE *out, DWORD out_len, BYTE *__ge, DWORD __ge_length, DWORD width, DWORD height);
	void _pgd3_ge_process_32(BYTE *out, DWORD out_len, BYTE *__ge, DWORD __ge_length, DWORD width, DWORD height);
	void pgd_ge_process2(BYTE *out, DWORD out_len, BYTE *__ge, DWORD __ge_length, DWORD width, DWORD height);
	void pgd_ge_process3(BYTE *out, DWORD out_len, BYTE *__ge, DWORD __ge_length, DWORD width, DWORD height, DWORD bpp);
	FILE* fp;
	bool Header_OK;
	string filename;
	pgd32_header_s pgd32_header;
	pgd32_info_t pgd32_info;
	ge_header_t ge_header;
};