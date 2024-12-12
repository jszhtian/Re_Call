/*
���ڽ���ļ�ͷΪGAR ��gar�ļ�
made by Darkness-TX
2020.06.11
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

#pragma pack(1)
struct gar_header {
	unit8 magic[4];//"GAR "
	unit32 version;//1
	unit64 index_off;
	unit32 zero;//��Ϊ0
	int num_start;//��Ϊ2�����ڴ����Ŀ�����ļ��������Ϊ2�Ŀ�һ��Ϊ���һ���飬�˿��offsetΪindex_off
	unit32 dir_num;//��Ϊ1
}GAR_Header;

struct gar_header2 {
	unit32 num;//������������GAR_Header2
	unit64 size;//���ļ���С��0x14������һ����Ĵ�С��
	unit32 zero1;
	unit32 zero2;
	unit32 zero3;
	unit64 first_offset;
}GAR_Header2;

struct index {
	int number;
	unit64 offset;
	unit64 size;
	char filename[MAX_PATH];
}Index[1000];
#pragma pack()

typedef struct fileinfo
{
	char filename[MAX_PATH];
	unit32 index;
	struct fileinfo *next;
}NodeFileInfo, *LinkFileInfo;
LinkFileInfo FileInfo = NULL;

unit32 ReadNumber(FILE *src, unit32 length_code)
{
	unit32 count = 0, i = 0, n = 0, rank = 0, b = 0;
	count = (length_code & 7) + 1;
	if (count > 4)
	{
		printf("count����4��pos:0x%X count:0x%X\n", ftell(src), count);
		system("pause");
		exit(0);
	}
	for (i = 0; i < count; ++i)
	{
		fread(&b, 1, 1, src);
		n |= b << rank;
		rank += 8;
	}
	if (count <= 3)
	{
		unit32 sign = n & (1 << (8 * count - 1));
		if (sign != 0)
			n -= sign << 1;
	}
	return n;
}

unit32 ReadInteger(FILE *src)
{
	unit32 opcode = 0;
	fread(&opcode, 1, 1, src);
	if ((opcode & 0xE0) != 0)
	{
		if ((opcode & 0xF8) != 0x80)
		{
			printf("opcode��Ϊ0x80��pos:0x%X opcode:0x%X\n", ftell(src) - 1, opcode);
			system("pause");
			exit(0);
		}
		opcode = ReadNumber(src, opcode);
	}
	else
		opcode = (opcode & 0x0F) - (opcode & 0x10);
	return opcode;
}

void SkipObject(FILE *src)
{
	unit32 opcode = 0;
	fread(&opcode, 1, 1, src);
	if ((opcode & 0xE0) != 0)
		ReadNumber(src, opcode);
}

void ReadString(FILE *src, char *dstname)
{
	unit32 opcode = 0, offset = 4, name_length = 0, savepos = 0;
	char name_str[MAX_PATH];
	fread(&opcode, 1, 1, src);
	if ((opcode & 0xF8) != 0x90)
	{
		printf("opcode������0x90��pos:0x%X opcode:0x%X\n", ftell(src) - 1, opcode);
		system("pause");
		exit(0);
	}
	offset += ReadNumber(src, opcode);
	savepos = ftell(src);
	fseek(src, offset, SEEK_SET);
	fread(&name_length, 4, 1, src);
	fread(name_str, name_length, 1, src);
	name_str[name_length] = '\0';
	sprintf(dstname, "%s", name_str);
	fseek(src, savepos, SEEK_SET);
}

void ReadIndex(char *fname)
{
	unit32 opcode_off = 0, name_count = 0, param_count = 0, arc_index = 0, i = 0, j = 0;
	char name[MAX_PATH], type[MAX_PATH], arc_type[MAX_PATH], arc_name[MAX_PATH], param_name[MAX_PATH], *filename = NULL;
	FileInfo = malloc(sizeof(NodeFileInfo));
	FileInfo->next = NULL;
	LinkFileInfo p = FileInfo;
	if (strrchr(fname, '\\') != NULL)
		filename = strrchr(fname, '\\') + 1;
	else
		filename = fname;
	FILE *src = fopen("SEC5/RES2", "rb");
	if (src == NULL)
	{
		printf("RES2�򿪴�������SEC5�ļ������Ƿ���RES2\n");
		system("pause");
		exit(0);
	}
	fread(&opcode_off, 4, 1, src);
	opcode_off += 4;
	fseek(src, opcode_off, SEEK_SET);
	fread(&name_count, 4, 1, src);
	for (i = 0; i < name_count; i++)
	{
		ReadString(src, name);
		ReadString(src, type);
		ReadString(src, arc_type);
		param_count = ReadInteger(src);
		arc_index = 0;
		arc_name[0] = '\0';
		for (j = 0; j < param_count; j++)
		{
			ReadString(src, param_name);
			if (strncmp("path", param_name, 4) == 0)
				ReadString(src, arc_name);
			else if (strncmp("arc-index", param_name, 9) == 0)
				arc_index = ReadInteger(src);
			else if (strncmp("arc-path", param_name, 8) == 0)
				ReadString(src, name);
			else
			{
				printf("����δ֪�Ĳ�������%s offset:0x%X\n", param_name, ftell(src));
				system("pause");
				SkipObject(src);
			}
		}
		if (strcmp(filename, arc_name) == 0)
		{
			p->next = malloc(sizeof(NodeFileInfo));
			if (strncmp("arc-path", param_name, 8))
				sprintf(p->next->filename, "%s.mpg", name);
			else
				sprintf(p->next->filename, name + 1);//ȥ��б��
			p->next->index = arc_index;
			p = p->next;
			p->next = NULL;
			//printf("arc_name:%s arc_index:%d arc_type:%s file_name:%s file_type:%s \n", arc_name, arc_index, arc_type, name, type);
		}
	}
	fclose(src);
}

void UnpackFile(char *fname)
{
	FILE *src = NULL, *dst = NULL;
	unit32 i = 0;
	unit8 *data = NULL;
	LinkFileInfo p = FileInfo;
	char dstname[MAX_PATH];
	src = fopen(fname, "rb");
	fread(&GAR_Header, sizeof(GAR_Header), 1, src);
	if (strncmp(GAR_Header.magic, "GAR ", 4))
	{
		printf("�ļ�ͷ����GAR !\n");
		system("pause");
		exit(0);
	}
	if (GAR_Header.version != 1)
	{
		printf("version����1!\n");
		system("pause");
		exit(0);
	}
	_fseeki64(src, GAR_Header.index_off, SEEK_SET);
	fread(&GAR_Header2, sizeof(GAR_Header2), 1, src);
	printf("version:%d index_offset:0x%llX dir_num:%d num:%d num_start:%d\n", GAR_Header.version, GAR_Header.index_off, GAR_Header.dir_num, GAR_Header2.num, GAR_Header.num_start);
	sprintf(dstname, "%s_unpack", fname);
	for (i = 0; i < GAR_Header2.num - 1; i++)
		fread(&Index[i], 0x14, 1, src);
	_mkdir(dstname);
	_chdir(dstname);
	for (i = 0; i < GAR_Header2.num - 1; i++)
	{
		if (Index[i].number == GAR_Header.num_start)
			continue;
		if (Index[i].number > GAR_Header.num_start)
		{
			p = p->next;
			sprintf(Index[i].filename, p->filename);
		}
		else
			sprintf(Index[i].filename, "%08d.bin", i);
		printf("\t%s num:%d offset:0x%llX size:0x%llX\n", Index[i].filename, Index[i].number, Index[i].offset, Index[i].size);
		_fseeki64(src, Index[i].offset, SEEK_SET);
		data = malloc((unit32)Index[i].size);//TMDһ��GAL�����д���2G����Ƶ��.........
		fread(data, (unit32)Index[i].size, 1, src);
		dst = fopen(Index[i].filename, "wb");
		fwrite(data, (unit32)Index[i].size, 1, dst);
		free(data);
		fclose(dst);
		FileNum++;
	}
	fclose(src);
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-StudioSeldomAdventureSystem\n���ڽ���ļ�ͷΪGAR ��gar�ļ���\n��gar�ļ��ϵ������ϡ�\nby Darkness-TX 2020.06.11\n\n");
	ReadIndex(argv[1]);
	UnpackFile(argv[1]);
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}