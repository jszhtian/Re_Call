// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <tchar.h>
#include <iomanip>
#include <mutex>
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
static DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

static void make_console() 
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}