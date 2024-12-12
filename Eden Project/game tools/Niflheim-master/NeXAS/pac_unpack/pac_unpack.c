/*
���ڽ��NeXAS�����pac�ļ�
made by Darkness-TX
2016.12.01

����°�NeXAS֧��
upload by AyamiKaze
2020.03.18
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <Windows.h>
#include <zstd.h>
#include <zlib.h>
#include <locale.h>
#include "Huffman_dec.h"

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

unit32 FileNum = 0;//���ļ�������ʼ����Ϊ0

struct header
{
	unit8 magic[4];//PAC\0��PAC\x7F
	unit32 num;
	unit32 mode;//BH����4
}pac_header;

struct index
{
	unit8 name[64];//�ļ���
	unit32 Offset;//�ļ�ƫ��
	unit32 FileSize;//��ѹ��С
	unit32 ComSize;//δ��ѹ��С
}Index[7000];

void ReadIndex(char *fname)
{
	FILE *src, *dst;
	unit32 ComSize = 0, UncomSize = 0, i = 0;
	unit8 *cdata, *udata, dstname[200];
	src = fopen(fname, "rb");
	sprintf(dstname, "%s_INDEX", fname);
	fread(pac_header.magic, 4, 1, src);
	if (strncmp(pac_header.magic, "PAC\0", 4) != 0 && *(unit32 *)pac_header.magic != 0x7F434150)//PAC\x7F
	{
		printf("�ļ�ͷ����PAC\\0��PAC\\x7F!\nҪ��������밴��������������رճ���\n");
		system("pause");
	}
	fread(&pac_header.num, 4, 1, src);
	fread(&pac_header.mode, 4, 1, src);
	printf("%s filenum:%d mode:%d\n\n", fname, pac_header.num, pac_header.mode);
	if (pac_header.mode != 4 && pac_header.mode != 7)
	{
		printf("����ģʽ4��7��\n");
		system("pause");
		exit(0);
	}
	else
	{
		fseek(src, -4, SEEK_END);
		fread(&ComSize, 4, 1, src);
		fseek(src, -4 - ComSize, SEEK_END);
		cdata = malloc(ComSize);
		fread(cdata, ComSize, 1, src);
		for (i = 0; i < ComSize; i++)
			cdata[i] = ~cdata[i];
		UncomSize = 76 * pac_header.num;
		udata = malloc(UncomSize);
		huffman_uncompress(udata, &UncomSize, cdata, ComSize);
		dst = fopen(dstname, "wb");
		fwrite(udata, UncomSize, 1, dst);
		free(cdata);
		fclose(dst);
		fclose(src);
		for (i = 0; i < pac_header.num; i++)
			memcpy(&Index[i], &udata[i * 76], 76);
		free(udata);
	}
}

void UnpackFile(char *fname)
{
	FILE *src, *dst;
	WCHAR dstname[200];
	unit8 *cdata, *udata, dirname[200];
	unit32 i;
	src = fopen(fname, "rb");
	sprintf(dirname, "%s_unpack", fname);
	_mkdir(dirname);
	_chdir(dirname);
	for (i = 0; i < pac_header.num; i++)
	{
		MultiByteToWideChar(932, 0, Index[i].name, -1, dstname, 64);
		dst = _wfopen(dstname, L"wb");
		cdata = malloc(Index[i].ComSize);
		udata = malloc(Index[i].FileSize);
		fseek(src, Index[i].Offset, SEEK_SET);
		fread(cdata, Index[i].ComSize, 1, src);
		if (Index[i].ComSize != Index[i].FileSize)
		{
			if (pac_header.mode == 4)
				uncompress(udata, &Index[i].FileSize, cdata, Index[i].ComSize);
			else
				ZSTD_decompress(udata, Index[i].FileSize, cdata, Index[i].ComSize);
			fwrite(udata, Index[i].FileSize, 1, dst);
			wprintf(L"%ls offset:0x%X filesize:0x%X comsize:0x%X\n", dstname, Index[i].Offset, Index[i].FileSize, Index[i].ComSize);
		}
		else
		{
			fwrite(cdata, Index[i].FileSize, 1, dst);
			wprintf(L"%ls offset:0x%X filesize:0x%X\n", dstname, Index[i].Offset, Index[i].FileSize);
		}
		free(cdata);
		free(udata);
		fclose(dst);
		FileNum += 1;
	}
	fclose(src);
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-NeXAS\n���ڽ��NeXAS�����pac�ļ���\n��pac�ļ��ϵ������ϡ�\nby Darkness-TX 2016.12.01\n\n����°�NeXAS���֧��\nby AyamiKaze 2020.03.18\n\n");
	ReadIndex(argv[1]);
	UnpackFile(argv[1]);
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}