// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include "detours.h"
#include <iostream>
#include <intrin.h>

#include "camellia.h"

using namespace std;

typedef enum _SIGNATURE
{
	TO_VOL = L'▲',//0x0708
	EOVOL = L'△',//0x0709
	STRUB = L'▼',//0x0A
	EOSTR = L'▽',//0x00
	EOPAR = L'◁',//0x06
	NXL = L'◆',//0x0704
	TO_RTN = L'◎',//0x0706
	TO_RUB = L'★',//0x0701
	NAME_SIG = L'※',//0x07
	UNKNOW_SIG = L'⊙',
};

static char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}