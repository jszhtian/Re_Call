// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "..\3rd\detours.h"

using namespace std;

BOOL SDWRAP_CRACK();
BOOL DMM_GAME_PLAYER_CRACK();