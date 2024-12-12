#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <direct.h>
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

using namespace std;

#pragma pack(1)

typedef struct font_s
{
	//��ʵ����GetGlyphOutline��GGO_GRAY8_BITMAP
	DWORD offset;		//��ģ���ڵ�ַ
	DWORD gmBlackBoxX;	//�ֿ�
	DWORD height;		//ͼƬ��
	DWORD x;			//gmptGlyphOrigin.x
	DWORD y;			//gmptGlyphOrigin.y����������
	DWORD advance;		//δ֪���ݶ�������е�����Freetype��FT_Bitmap�е�advance���
	DWORD size;			//��ģռ���ֽ�
	DWORD width;		//��size / gmBlackBoxY�õ���ת��ͼƬ��Ŀ�
}font_t;

#pragma pack()

class FONT
{
public:
	FONT(string fontname);
	bool font2png(DWORD i);
	BYTE* makefont(WCHAR chText, DWORD i);
	bool FT_Init(string font_path, DWORD font_height);
	BYTE* makefont_ft(WCHAR chText, DWORD i);
	~FONT();
	FILE *fp;
	string dirname;
	DWORD count;
	DWORD tbl_start = 2199;
	vector<font_t> findexs;
private:
	FT_Library library;
	FT_Face face;
	FT_Error error;
	bool ReadIndex(string fontname);
	void GetFontCount(FILE *src);
};