#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <direct.h>
#include <png.h>

using namespace std;

#pragma pack (1)

typedef struct {
	char magic[4];
	DWORD filesize;
}fSG_header_t;

typedef struct {
	char magic[4];//cJPG
	DWORD size;//��ǰcJPG��ĳ���
	DWORD head_size;//cJPGͷ�ĳ���
	DWORD data_length;//��ǰ������ݳ���
	USHORT x_limit;//��ͼ����������ռ�������
	USHORT y_limit;//��ͼ����������ռ�����߶�
	USHORT x;//��ͼԭ��x��λ��
	USHORT y;//��ͼԭ��y��λ��
	USHORT width;
	USHORT height;
	USHORT unknown0;//0x48
	USHORT unknown1;//0x00
	DWORD seed;//������
} cJPG_header_t;

typedef struct {
	char magic[4];//cRGB
	DWORD size;//��ǰcRGB��ĳ���
	DWORD head_size;//cRGBͷ�ĳ���
	DWORD data_length;//��ǰ������ݳ���
	USHORT mode;//0, 1(bg), 2(with palette), 3(����), 4, etc
	USHORT unknown2;//type2: 0x8808
	USHORT x_limit;//��ͼ����������ռ�������
	USHORT y_limit;//��ͼ����������ռ�����߶�
	USHORT x;//��ͼԭ��x��λ��
	USHORT y;//��ͼԭ��y��λ��
	USHORT width;
	USHORT height;
	USHORT unknown4;//0x48
	USHORT bpp;//< 24����cRGB�� 0x18(with palette) or 0x20
	BYTE unknown[8];//0x08 0x18 0x08 0x10 0x08 0x08 0x08 0x00
} cRGB_header_t;

#pragma pack()

class sg
{
public:
	sg(string sgname);
	void sg_decode();
	~sg();

private:
	bool ReadHeader(string sgname);
	bool ReadSecHeader();
	void _cRGB_decode(BYTE *srcdata, BYTE *dstdata);
	bool _cRGB2png(FILE *file, BYTE *data);
	void _cJPG_decode(BYTE *srcdata, BYTE *dstdata);
	FILE *fp;
	fSG_header_t fSG_header;
	cJPG_header_t cJPG_header;
	cRGB_header_t cRGB_header;
	string filename;
	char sec_head[4];
	bool Header_OK;
};