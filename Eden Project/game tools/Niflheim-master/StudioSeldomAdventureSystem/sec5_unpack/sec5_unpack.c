/*
���ڽ���ļ�ͷΪSEC5��sec5�ļ�
made by Darkness-TX
2018.05.24
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <Windows.h>
#include <locale.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;
typedef unsigned __int64 unit64;

unit32 FileNum = 0;//���ļ�������ʼ����Ϊ0

struct header
{
	unit8 magic[4];
	unit32 size;
}Header;

void UnpackFile(char *fname)
{
	FILE *src = NULL, *dst = NULL;
	unit8 *data = NULL, dstname[5];
	unit32 filesize = 0;
	src = fopen(fname, "rb");
	fseek(src, 0, SEEK_END);
	filesize = ftell(src);
	fseek(src, 0, SEEK_SET);
	fread(&Header, sizeof(Header), 1, src);
	if (strncmp(Header.magic, "SEC5", 4))
	{
		printf("�ļ�ͷ����SEC5!\n");
		system("pause");
		exit(0);
	}
	_mkdir("SEC5");
	_chdir("SEC5");
	while ((unit32)ftell(src) < filesize)
	{
		fread(&Header, sizeof(Header), 1, src);
		if (strncmp(Header.magic, "ENDS", 4) == 0)
			continue;
		snprintf(dstname, 5, "%s", Header.magic);
		printf("filename:%s size:0x%X\n", dstname, Header.size);
		data = malloc(Header.size);
		fread(data, Header.size, 1, src);
		if (strncmp(Header.magic, "CODE", 4) == 0)
		{
			unit8 key = 0, buff = 0;
			for (unit32 i = 0; i < Header.size; i++)
			{
				//mov dl,al
				//xor dl,cl
				//add al,0x12
				//add cl,al
				buff = data[i];
				data[i] ^= key;
				key += (unit8)(buff + 0x12);
			}
		}
		dst = fopen(dstname, "wb");
		fwrite(data, Header.size, 1, dst);
		free(data);
		fclose(dst);
		FileNum++;
	}
	fclose(src);
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-StudioSeldomAdventureSystem\n���ڽ���ļ�ͷΪSEC5��sec5�ļ���\n��sec5�ļ��ϵ������ϡ�\nby Darkness-TX 2018.05.24\n\n");
	UnpackFile(argv[1]);
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}