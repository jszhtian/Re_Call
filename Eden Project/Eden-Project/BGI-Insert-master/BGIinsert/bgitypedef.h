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
//��������FrameWorker����ļ�¼
//Header�ε�String����DWORD�����

#endif