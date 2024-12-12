// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>

#pragma pack (1)
typedef struct CPKHEADER {
	unsigned int FileCount;
}CPKHEADER_T;

typedef struct CPKENTRY {
	char FileName[51];
	unsigned int Offset;
	unsigned int FileSize;
}CPKENTRY_T;
#pragma pack ()

static FILE* fp;
static CPKHEADER_T header;
static CPKENTRY_T* entry = nullptr;

static BOOL InitPackInfo()
{
	fp = fopen("cnpack", "rb");
	if (!fp)
	{
		MessageBox(0, L"Can't find cnpack.", 0, 0);
		return FALSE;
	}
	fread(&header, sizeof(header), 1, fp);
	entry = (CPKENTRY_T*)malloc(header.FileCount * sizeof(CPKENTRY_T));
	if (!entry)
	{
		MessageBox(0, L"Can't alloc cnpack entry buffer.", 0, 0);
		return FALSE;
	}
	fread(entry, header.FileCount * sizeof(CPKENTRY_T), 1, fp);
	return TRUE;
}

static BOOL GetFileBuffByName(char* fnm, BYTE** buff, DWORD* size)
{
	for (int i = 0; i < header.FileCount; i++)
	{
		if (!strncmp(entry[i].FileName, fnm, strlen(fnm)))
		{
			fseek(fp, header.FileCount * sizeof(CPKENTRY_T) + entry[i].Offset, SEEK_SET);
			BYTE* outbuf = (BYTE*)malloc(entry[i].FileSize);
			if (!outbuf)
			{
				char msg[MAX_PATH];
				sprintf(msg, "Can't read file:%s", fnm);
				MessageBoxA(0, msg, 0, 0);
				return FALSE;
			}
			fread(outbuf, entry[i].FileSize, 1, fp);

			for (int a = 0; a < entry[i].FileSize; a++)
				outbuf[a] ^= 0xE4;

			auto dmp = fopen(fnm, "wb");
			fwrite(outbuf, entry[i].FileSize, 1, dmp);
			fclose(dmp);

			*buff = outbuf;
			*size = entry[i].FileSize;
			return TRUE;
		}
	}
	return FALSE;
}