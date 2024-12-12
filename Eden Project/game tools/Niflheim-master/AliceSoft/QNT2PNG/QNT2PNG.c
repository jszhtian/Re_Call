/*
���ڵ���QNTͼƬ
made by Darkness-TX
2018.07.16
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
#include <zlib.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

unit32 FileNum = 0;//���ļ�������ʼ����Ϊ0

struct qnt_header
{
	unit8 sign[4];//QNT\0
	unit32 version;
	unit32 head_size;
	unit32 offsetX;
	unit32 offsetY;
	unit32 width;
	unit32 height;
	unit32 bpp;
	unit32 block_num;//δ֪
	unit32 rgb_size;
	unit32 alpha_size;
	unit32 zero;
	unit32 zero2;
	unit32 zero3;
	unit32 zero4;
	unit32 zero5;
	unit32 zero6;
}QNT_Header;

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
	if ((Handle = _wfindfirst(L"*.QNT", &FileInfo)) == -1L)
	{
		printf("û���ҵ�ƥ�����Ŀ���뽫��׺����Ϊ.QNT\n");
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

void ReadIndex(FILE *src)
{
	fread(&QNT_Header, sizeof(QNT_Header), 1, src);
	if (strncmp(QNT_Header.sign, "QNT\0", 4))
	{
		printf("��֧�ֵ��ļ����ͣ��ļ�ͷ����QNT\\0\n");
		system("pause");
		exit(0);
	}
	if (QNT_Header.version != 2)
	{
		printf("��֧�ֵ��ļ����ͣ��ļ��汾����2\n");
		system("pause");
		exit(0);
	}
	if (QNT_Header.alpha_size != 0)
		QNT_Header.bpp = 32;
	wprintf(L"headsize:0x%X bpp:%d X:%d Y:%d width:%d height:%d\n", QNT_Header.head_size, QNT_Header.bpp, QNT_Header.offsetX, QNT_Header.offsetY, QNT_Header.width, QNT_Header.height);
}

void WritePng(FILE *Pngname, unit32 Width, unit32 Height, unit32 Bpp, unit8* data)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unit32 i = 0;
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
	if (Bpp == 24)
	{
		png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr);
		for (i = 0; i < Width * Height; i++)
		{
			buff = data[i * 3 + 0];
			data[i * 3 + 0] = data[i * 3 + 2];
			data[i * 3 + 2] = buff;
		}
		for (i = 0; i < Height; i++)
			png_write_row(png_ptr, data + i*Width * 3);
	}
	else if (Bpp == 32)
	{
		png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr);
		for (i = 0; i < Width * Height; i++)
		{
			buff = data[i * 4 + 0];
			data[i * 4 + 0] = data[i * 4 + 2];
			data[i * 4 + 2] = buff;
		}
		for (i = 0; i < Height; i++)
			png_write_row(png_ptr, data + i*Width * 4);
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

void Build(unit8 *udata, unit8 *bitmap, unit8 *alpha, unit32 size)
{
	unit32 src = 0, dst = 0, i = 0, stride = size * QNT_Header.width;
	for (unit32 b = 0; b < 3; b++)//���������������BGRֵ��ԭʼ�������ȴ�����B���ٴ�����G���ٴ�����R
	{
		dst = b;
		for (unit32 h = 0; h < QNT_Header.height / 2; h++)
		{
			for (unit32 w = 0; w < QNT_Header.width; w++)
			{
				udata[dst] = bitmap[src++];
				udata[dst + stride] = bitmap[src++];
				dst += size;
			}
			dst += stride;
			src += 2 * (QNT_Header.width & 1);
		}
		if (QNT_Header.height % 2 == 1)
		{
			for (unit32 w = 0; w < QNT_Header.width; w++)
			{
				udata[dst] = bitmap[src];
				dst += size;
				src += 2;//��ֻ��һ���ֽ����ã��ڶ����ֽڿհ�
			}
			src += 2 * (QNT_Header.width & 1);
		}
	}
	if (size == 4)//���alphaֵ
	{
		dst = 3;
		src = 0;
		for (unit32 h = 0; h < QNT_Header.height; h++)
		{
			for (unit32 w = 0; w < QNT_Header.width; w++)
			{
				udata[dst] = alpha[src++];
				dst += 4;
			}
			src += QNT_Header.width & 1;
		}
	}
	dst = size;
	for (i = stride - size; i != 0; --i)//�����һ��
	{
		unit8 b = udata[dst - size] - udata[dst];
		udata[dst++] = (unit8)b;
	}
	for (unit32 j = QNT_Header.height - 1; j != 0; --j)//�ӵڶ��п�ʼ
	{
		for (i = 0; i != size; ++i)//������һ�е�һ�����ص���Ϣ
		{
			udata[dst] = (unit8)(udata[dst - stride] - udata[dst]);
			++dst;
		}
		for (i = stride - size; i != 0; --i)//������һ��
		{
			unit32 b = ((unit32)udata[dst - stride] + udata[dst - size]) >> 1;
			b -= udata[dst];
			udata[dst++] = (unit8)b;
		}
	}
}

void WritePngFile()
{
	FILE *src = NULL, *dst = NULL;
	unit8 *cdata = NULL, *udata = NULL, *bitmap = NULL, *alpha = NULL;
	unit32 i = 0, decomp_size = 0, w = 0, h = 0;
	WCHAR dstname[MAX_PATH];
	for (i = 0; i < FileNum; i++)
	{
		src = _wfopen(Index[i].FileName, L"rb");
		wprintf(L"name:%ls ", Index[i].FileName);
		ReadIndex(src);
		w = (QNT_Header.width + 1) & ~1;
		h = (QNT_Header.height + 1) & ~1;
		wsprintf(dstname, L"%ls.png", Index[i].FileName);
		cdata = malloc(QNT_Header.rgb_size);
		fread(cdata, QNT_Header.rgb_size, 1, src);
		decomp_size = w * h * 3;
		bitmap = malloc(decomp_size);
		uncompress(bitmap, &decomp_size, cdata, QNT_Header.rgb_size);
		free(cdata);
		if (QNT_Header.alpha_size != 0)
		{
			cdata = malloc(QNT_Header.alpha_size);
			fread(cdata, QNT_Header.alpha_size, 1, src);
			decomp_size = w * QNT_Header.height;
			alpha = malloc(decomp_size);
			uncompress(alpha, &decomp_size, cdata, QNT_Header.alpha_size);
			free(cdata);
		}
		udata = malloc(QNT_Header.width * QNT_Header.height * (QNT_Header.bpp / 8));
		Build(udata, bitmap, alpha, QNT_Header.bpp / 8);
		free(bitmap);
		if (QNT_Header.alpha_size != 0)
			free(alpha);
		dst = _wfopen(dstname, L"wb");
		WritePng(dst, QNT_Header.width, QNT_Header.height, QNT_Header.bpp, udata);
		free(udata);
		fclose(src);
		fclose(dst);
	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-AliceSoft\n���ڵ���QNTͼƬ��\n���ļ����ϵ������ϡ�\nby Darkness-TX 2018.07.16\n\n");
	process_dir(argv[1]);
	WritePngFile();
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}