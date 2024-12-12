#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <fstream>
static void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}
static LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static LPWSTR ctowGBK(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(936, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(936, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static char* wtocSJIS(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(932, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(932, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE);
	char* bbb = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
	return bbb;
}

static char* wtoc932(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(932, NULL, str, -1, NULL, 0, NULL, FALSE);
	char* bbb = new char[dwMinSize];
	WideCharToMultiByte(932, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
	return bbb;
}

static LPWSTR ctow(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_ACP, 0, str, -1, out, dwMinSize);//转换
	return out;
}