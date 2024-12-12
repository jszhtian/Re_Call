/*
���ڵ���prsͼƬ
made by Darkness-TX
2022.09.10
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <Windows.h>
#include <locale.h>
#include <png.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

unit32 FileNum = 0;//���ļ�������ʼ����Ϊ0

struct prs_header
{
	unit8 magic[2];
	unit32 width;
	unit32 height;
	unit16 bpp;
}Prs_Header;

struct index
{
	WCHAR FileName[MAX_PATH];//�ļ���
	unit32 FileSize;//�ļ���С
}Index[50000];

unit32 process_dir(char* dname)
{
	long Handle;
	struct _wfinddata64i32_t FileInfo;
	_chdir(dname);//��ת·��
	if ((Handle = _wfindfirst(L"*.prs", &FileInfo)) == -1L)
	{
		printf("û���ҵ�ƥ�����Ŀ���뽫��׺����Ϊ.prs\n");
		system("pause");
		exit(0);
	}
	do
	{
		if (FileInfo.name[0] == L'.')  //���˱���Ŀ¼�͸�Ŀ¼
			continue;
		wsprintf(Index[FileNum].FileName, FileInfo.name);
		Index[FileNum].FileSize = FileInfo.size;
		FileNum++;
	} while (_wfindnext(Handle, &FileInfo) == 0);
	return FileNum;
}

void WritePng(FILE* Pngname, unit32 Width, unit32 Height, unit32 Bpp, unit8* data)
{
	png_structp png_ptr;
	png_infop info_ptr;
	int i = 0;
	unit8 buff = 0;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		printf("PNG��Ϣ����ʧ��!\n");
		exit(0);
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		printf("info��Ϣ����ʧ��!\n");
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		exit(0);
	}
	png_init_io(png_ptr, Pngname);
	if (Bpp == 24)//��Ȼ��24������ȴ��BGRA
	{
		png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr);
		for (i = 0; i < Width * Height; i++)
		{
			buff = data[i * 4 + 0];
			data[i * 4 + 0] = data[i * 4 + 2];
			data[i * 4 + 2] = buff;
		}
		for (i = Height - 1; i >= 0; i--)//���µߵ�
			png_write_row(png_ptr, data + i * Width * 4);
	}
	else
	{
		printf("��֧�ֵ�bppģʽ!");
		system("pause");
		exit(0);
	}
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
}

void WritePngFile()
{
	FILE* src = NULL, * dst = NULL;
	unit32 i = 0;
	unit8* data = NULL;
	WCHAR dstname[MAX_PATH];
	for (i = 0; i < FileNum; i++)
	{
		src = _wfopen(Index[i].FileName, L"rb");
		fread(Prs_Header.magic, 2, 1, src);
		fread(&Prs_Header.width, 4, 1, src);
		fread(&Prs_Header.height, 4, 1, src);
		fread(&Prs_Header.bpp, 2, 1, src);
		wprintf(L"%ls width:%d height:%d bpp:%d\n", Index[i].FileName, Prs_Header.width, Prs_Header.height, Prs_Header.bpp);
		if (strncmp(Prs_Header.magic, "AP", 2) != 0)
		{
			printf("�ļ�ͷ����AP��\n");
			system("pause");
			exit(0);
		}
		wsprintf(dstname, L"%ls.png", Index[i].FileName);
		data = malloc(Index[i].FileSize - 0xC);
		fread(data, Index[i].FileSize - 0xC, 1, src);
		dst = _wfopen(dstname, L"wb");
		WritePng(dst, Prs_Header.width, Prs_Header.height, Prs_Header.bpp, data);
		free(data);
		fclose(dst);
		fclose(src);
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-KaGuYa\n���ڵ���prsͼƬ��\n���ļ����ϵ������ϡ�\nby Darkness-TX 2022.09.10\n\n");
	process_dir(argv[1]);
	WritePngFile();
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}