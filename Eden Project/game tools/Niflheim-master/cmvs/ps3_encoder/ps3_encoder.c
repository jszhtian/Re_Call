/*
���ڼ��ܲ������ļ�ͷΪPS2A��ps3�ļ����ı�
made by Darkness-TX
2018.04.20
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <Windows.h>
#include <locale.h>
#include "lzss.h"

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

unit32 FileNum = 0;//���ļ�������ʼ����Ϊ0

struct ps3_header
{
	unit8 magic[4];//PS2A
	unit32 header_length;// 0x30
	unit32 unknown0;// 0x205b8
	unit32 key;
	unit32 unknown1_count;// ÿ��4�ֽ�
	unit32 unknown2_length;
	unit32 unknown3_length;
	unit32 name_index_length;
	unit32 unknown4;// 0
	unit32 comprlen;
	unit32 uncomprlen;
	unit32 unknown5;// 0
	//����Ϊ����
	wchar_t FileName[260];//�ļ���
	unit32 FileSize;//�ļ���С
} PS3_Header[500];

unit32 process_dir(char *dname)
{
	long Handle;
	struct _wfinddata64i32_t FileInfo;
	_chdir(dname);//��ת·��
	if ((Handle = _wfindfirst(L"*.ps3", &FileInfo)) == -1L)
	{
		printf("û���ҵ�ƥ�����Ŀ���뽫��׺����Ϊ.ps3\n");
		system("pause");
		return -1;
	}
	do
	{
		if (FileInfo.name[0] == L'.')  //���˱���Ŀ¼�͸�Ŀ¼
			continue;
		wsprintfW(PS3_Header[FileNum].FileName, FileInfo.name);
		PS3_Header[FileNum].FileSize = FileInfo.size;
		FileNum++;
	} while (_wfindnext(Handle, &FileInfo) == 0);
	return FileNum;
}

BOOL ReadIndex(FILE *src, unit32 i)
{
	wprintf(L"name:%ls ", PS3_Header[i].FileName);
	fread(&PS3_Header[i], 0x30, 1, src);
	if (strncmp(PS3_Header[i].magic, "PS2A", 4))
	{
		printf("\n��֧�ֵ��ļ����ͣ��ļ�ͷ����PS2A\n");
		system("pause");
		return FALSE;
	}
	if (PS3_Header[i].header_length != 0x30)
	{
		printf("\n��֧�ֵ��ļ����ͣ��ļ�ͷ���Ȳ���0x30\n");
		system("pause");
		return FALSE;
	}
	return TRUE;
}

void FileEncode(char *fname)
{
	unit32 i = 0, k = 0;
	FILE *src, *dst;
	unit8 *cdata, *udata;
	wchar_t dstname[MAX_PATH];
	_chdir(fname);
	for (i = 0; i < FileNum; i++)
	{
		wsprintfW(dstname, L"%ls.enc", PS3_Header[i].FileName);
		src = _wfopen(dstname, L"rb");
		if (ReadIndex(src, i))
		{
			unit8 xor = (unit8)((PS3_Header[i].key >> 24) + (PS3_Header[i].key >> 3));
			unit8 shifts = ((PS3_Header[i].key >> 20) % 5) + 1;
			udata = malloc(PS3_Header[i].uncomprlen);
			fread(udata, PS3_Header[i].uncomprlen, 1, src);
			cdata = malloc(PS3_Header[i].uncomprlen * 2);
			PS3_Header[i].comprlen = lzss_encode(udata, PS3_Header[i].uncomprlen, cdata, PS3_Header[i].uncomprlen * 2);
			free(udata);
			for (k = 0; k < PS3_Header[i].comprlen; k++)
			{
				unit8 tmp = (cdata[k] >> (8 - shifts)) | (cdata[k] << shifts);
				tmp ^= xor;
				cdata[k] = tmp + 0x7C;
			}
			wsprintfW(dstname, L"%ls.new", PS3_Header[i].FileName);
			dst = _wfopen(dstname, L"wb");
			fwrite(&PS3_Header[i], PS3_Header[i].header_length, 1, dst);
			fwrite(cdata, PS3_Header[i].comprlen, 1, dst);
			fclose(dst);
			free(cdata);
			wprintf(L"complen:0x%X decomplen:0x%X key:0x%X ", PS3_Header[i].comprlen, PS3_Header[i].uncomprlen, PS3_Header[i].key);
			wprintf(L"str_offset:0x%X str_length:0x%X\n", PS3_Header[i].unknown2_length + PS3_Header[i].unknown1_count * 4, PS3_Header[i].name_index_length);
		}
		fclose(src);
	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-cmvs\n���ڼ��ܲ������ļ�ͷΪPS2A��ps3�ļ����ı���\n��Ŀ¼�ϵ������ϡ�\nby Darkness-TX 2018.04.20\n\n");
	process_dir(argv[1]);
	FileEncode(argv[1]);
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}