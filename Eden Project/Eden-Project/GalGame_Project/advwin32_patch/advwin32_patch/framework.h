// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <fstream>
#include <iostream>
//#include <imm.h>
#include "detours.h"
//#pragma comment(lib, "Imm32.lib")
using namespace std;
static DWORD BaseAddr = (DWORD)GetModuleHandleA(NULL);
static DWORD SystemUntAddr = 0;
static DWORD DIBWORKFTLAddr = 0;
static void make_console()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Debug mode\n" << endl;
}

static LPWSTR ctow(char* str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(cp, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(cp, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static char* wtoc(LPCTSTR str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static void write_bmp(const char* filename,
	char* buff,
	long    len,
	long    width,
	long    height,
	short   depth_bytes)
{
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;

	memset(&bmf, 0, sizeof(bmf));
	memset(&bmi, 0, sizeof(bmi));

	bmf.bfType = 0x4D42;
	bmf.bfSize = sizeof(bmf) + sizeof(bmi) + len;
	bmf.bfOffBits = sizeof(bmf) + sizeof(bmi);

	bmi.biSize = sizeof(bmi);
	bmi.biWidth = width;
	bmi.biHeight = height;
	bmi.biPlanes = 1;
	bmi.biBitCount = depth_bytes;
	char FileName[MAX_PATH];
	strcpy(FileName, filename);
	strcat(FileName, ".bmp");
	FILE* fd = fopen(FileName, "wb");
	fwrite(&bmf, 1, sizeof(bmf), fd);
	fwrite(&bmi, 1, sizeof(bmi), fd);
	fwrite(buff, 1, len, fd);
	fclose(fd);
}