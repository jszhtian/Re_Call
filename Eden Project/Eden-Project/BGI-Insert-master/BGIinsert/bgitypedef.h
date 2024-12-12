#pragma once
#ifndef _ScriptHeader_
#define _ScriptHeader_

#include <Windows.h>

//0x1C
#pragma pack(1)
typedef struct ScriptHeader
{
	CHAR  Magic[0x1C];
	ULONG HeaderSize;
} BGI_BURIKO_SCRIPT_HEADER;
//====================================
#pragma pack()
//接下来是FrameWorker段落的记录
//Header段的String都是DWORD对齐的

#endif