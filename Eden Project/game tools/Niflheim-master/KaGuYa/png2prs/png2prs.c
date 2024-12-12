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

unit8* ReadPng(FILE* pngfile)
{
	png_structp png_ptr;
	png_infop info_ptr, end_ptr;
	png_bytep* rows;
	int i = 0, width = 0, height = 0, bpp = 0, format = 0, j = 0;
	unit8 buff = 0, * data = NULL;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		printf("PNG��Ϣ����ʧ��!\n");
		exit(0);
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		printf("info��Ϣ����ʧ��!\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		exit(0);
	}
	end_ptr = png_create_info_struct(png_ptr);
	if (end_ptr == NULL)
	{
		printf("end��Ϣ����ʧ��!\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		exit(0);
	}
	png_init_io(png_ptr, pngfile);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height, &bpp, &format, NULL, NULL, NULL);
	if (Prs_Header.bpp != 24)
	{
		printf("��֧�ֵ�bppģʽ!");
		system("pause");
		exit(0);
	}
	if (format != PNG_COLOR_TYPE_RGB_ALPHA)
	{
		printf("ԭʼprs�ļ�Ϊ32λrgbaͼ����png�ļ�����32λrgbaͼ����ת����format:%d\n", format);
		system("pause");
		exit(0);
	}
	if (width != Prs_Header.width || height != Prs_Header.height)
	{
		printf("ͼƬ�ĳ�����ԭͼ������\n");
		system("pause");
		exit(0);
	}
	data = malloc(Prs_Header.height * Prs_Header.width * 4);
	rows = (png_bytep*)malloc(Prs_Header.height * sizeof(char*));
	for (i = Prs_Header.height - 1; i >= 0; i--)//���µߵ�
		rows[j++] = (png_bytep)(data + Prs_Header.width * i * 4);
	png_read_image(png_ptr, rows);
	free(rows);
	for (i = 0; i < width * height; i++)
	{
		buff = data[i * 4 + 0];
		data[i * 4 + 0] = data[i * 4 + 2];
		data[i * 4 + 2] = buff;
	}
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
	return data;
}

void ReadPngFile()
{
	FILE* src = NULL, * dst = NULL, * png = NULL;
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
		png = _wfopen(dstname, L"rb");
		data = ReadPng(png);
		fclose(png);
		wsprintf(dstname, L"%ls.new", Index[i].FileName);
		dst = _wfopen(dstname, L"wb");
		fwrite(Prs_Header.magic, 2, 1, dst);
		fwrite(&Prs_Header.width, 4, 1, dst);
		fwrite(&Prs_Header.height, 4, 1, dst);
		fwrite(&Prs_Header.bpp, 2, 1, dst);
		fwrite(data, Index[i].FileSize - 0xC, 1, dst);
		free(data);
		fclose(dst);
		fclose(src);
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-KaGuYa\n���ڵ���ap3ͼƬ��\n���ļ����ϵ������ϡ�\nby Darkness-TX 2022.09.10\n\n");
	process_dir(argv[1]);
	ReadPngFile();
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}