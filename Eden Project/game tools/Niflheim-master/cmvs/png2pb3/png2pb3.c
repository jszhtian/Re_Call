/*
���ڽ�pngͼƬת���ɼ򵥵�pb3
������Longinus 1.4.1.0
made by Darkness-TX
2018.11.09
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
typedef unsigned __int64 unit64;

unit32 FileNum = 0;//���ļ�������ʼ����Ϊ0

struct index
{
	WCHAR FileName[MAX_PATH];//�ļ���
	unit32 FileSize;//�ļ���С
}Index[50000];

unit32 process_dir(char *dname)
{
	long Handle;
	struct _wfinddata64i32_t FileInfo;
	_chdir(dname);//��ת·��
	if ((Handle = _wfindfirst(L"*.png", &FileInfo)) == -1L)
	{
		printf("û���ҵ�ƥ�����Ŀ���뽫��׺����Ϊ.png\n");
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

unit8* ReadPng(FILE *pngfile, unit32 *picsize)
{
	png_structp png_ptr;
	png_infop info_ptr, end_ptr;
	png_bytep *rows;
	unit32 i, width = 0, height = 0, bpp = 0, format = 0, num_palette = 0;
	unit8 buff = 0;

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
	if (format == PNG_COLOR_TYPE_RGB)
	{
		wprintf(L"width:%d height:%d bpp:24\n", width, height);
		unit8 *odata = malloc(width * height * 3);
		*picsize = width * height * 4;
		rows = (png_bytep*)malloc(height * sizeof(char*));
		for (i = 0; i < height; i++)
			rows[i] = (png_bytep)(odata + width*i * 3);
		png_read_image(png_ptr, rows);
		free(rows);
		png_read_end(png_ptr, info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
		for (i = 0; i < height*width; i++)
		{
			buff = odata[i * 3 + 0];
			odata[i * 3 + 0] = odata[i * 3 + 2];
			odata[i * 3 + 2] = buff;
		}
		unit8 *data = malloc(width * height * 4);
		for (i = 0; i < height*width; i++)
		{
			memcpy(&data[i * 4], &odata[i * 3], 3);
			data[i * 4 + 3] = 0xFF;
		}
		free(odata);
		return data;
	}
	else if (format == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		wprintf(L"width:%d height:%d bpp:32\n", width, height);
		unit8 *data = malloc(width * height * 4);
		*picsize = width * height * 4;
		rows = (png_bytep*)malloc(height * sizeof(char*));
		for (i = 0; i < height; i++)
			rows[i] = (png_bytep)(data + width*i * 4);
		png_read_image(png_ptr, rows);
		free(rows);
		png_read_end(png_ptr, info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
		for (i = 0; i < height*width; i++)
		{
			buff = data[i * 4 + 0];
			data[i * 4 + 0] = data[i * 4 + 2];
			data[i * 4 + 2] = buff;
		}
		return data;
	}
	else
	{
		wprintf(L"��֧�ֵ�ͼƬ���ͣ�\n");
		system("pause");
	}
	return NULL;
}

void WritePngFile()
{
	FILE *src = NULL, *dst = NULL;
	unit32 i = 0, picsize = 0;
	unit8 *data = NULL;
	WCHAR dstname[MAX_PATH], *buff;
	for (i = 0; i < FileNum; i++)
	{
		src = _wfopen(Index[i].FileName, L"rb");
		wprintf(L"name:%ls ", Index[i].FileName);
		data = ReadPng(src, &picsize);
		buff = wcsrchr(Index[i].FileName, L'.');
		*buff = L'\0';
		wsprintfW(dstname, L"%s.pb3", Index[i].FileName);
		dst = _wfopen(dstname, L"wb");
		fwrite(data, picsize, 1, dst);
		free(data);
		fclose(src);
		fclose(dst);
	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-cmvs\n���ڽ�pngͼƬת���ɼ򵥵�pb3��\n���ļ����ϵ������ϡ�\nby Darkness-TX 2018.11.09\n\n");
	process_dir(argv[1]);
	WritePngFile();
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}