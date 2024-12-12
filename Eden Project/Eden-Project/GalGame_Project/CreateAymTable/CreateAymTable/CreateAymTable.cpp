/*
用于将码表转成Longinus适用的内存格式
made by Darkness-TX
2017.01.07
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <Windows.h>
#include <locale.h>
#include "Key.h"


typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

void XorEnc(char* InBuffer, ULONG InSize)
{
	for (ULONG i = 0; i < InSize; i++)
	{
		InBuffer[i] ^= Key3[i % 1024];
		InBuffer[i + 2] ^= key2[i % 1024];
		InBuffer[i + 3] ^= Key4[i % 1024];
		InBuffer[i + 4] ^= Key1[i % 1024];
		InBuffer += InSize;
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project：Longinus\n用于将码表转成Longinus适用的内存格式。\n将码表txt文件拖到程序上。\nby Darkness-TX 2017.01.07\n\n");
	FILE* src, * dst;
	unit8 atbl[2];
	unit32 slen;
	unit16* cdata;
	wchar_t tbl, data[256], * find, hex[5];
	src = fopen(argv[1], "rt,ccs=UNICODE");
	dst = fopen("CHS.TBL", "wb");
	cdata = (unit16*)malloc(0xF000 * 2);
	memset(cdata, 0, 0xF000 * 2);
	while (fgetws(data, 256, src) != NULL)
	{
		find = wcschr(data, L'=');
		tbl = data[find - data + 1];
		wcsncpy(hex, data, find - data);
		hex[find - data] = L'\0';
		slen = WideCharToMultiByte(936, 0, &tbl, 1, NULL, 0, NULL, FALSE);
		WideCharToMultiByte(936, 0, &tbl, 1, (LPSTR)atbl, slen, NULL, FALSE);
		if (slen == 2)
		{
			memcpy(&cdata[wcstol(hex, NULL, 16)], &atbl[1], 1);
			memcpy((unit8*)& cdata[wcstol(hex, NULL, 16)] + 1, &atbl[0], 1);
		}
		else if (slen == 1)
			memcpy(&cdata[wcstol(hex, NULL, 16)], atbl, 1);
		//printf("ch:%s  offset:0x%X\n", atbl, wcstol(hex, NULL, 16));
	}
	fwrite(cdata, 1, 0xF000 * 2, dst);
	fclose(src);
	fclose(dst);
	free(cdata);
	system("pause");
	FILE* fp=fopen("CHS.TBL", "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	XorEnc(buff, size);
	fopen("GlyphOutline.Aym", "wb");
	fwrite(buff, size, 1, fp);
	fclose(fp);
	return 0;
}