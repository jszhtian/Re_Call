// FmodTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "HoshiSora.h"
#include "VarInfo.h"
#include <cstring>

#include "FMOD\fmod.h"
#include "FMOD\fmod.hpp"
#include "FMOD\fmod_errors.h"

#include "FileManager.h"
int _tmain(int argc, _TCHAR* argv[])
{
	
	char filename[1024];
	memset(filename, 0, sizeof(filename));
	FILE *config = fopen("fvp.txt", "rb");
	if(config == NULL)
		return -1;
	fscanf(config, "%s", filename);
	fclose(config);
	HoshiSora test;
	VarInfo file;
	file.type = VarInfo::STRING;
	strcpy(file.str, filename);
	VarInfo ch;
	ch.type = VarInfo::pINT;
	ch.var = 1;
	test.AudioLoad(file, ch);
	VarInfo fade;
	fade.type = VarInfo::pINT;
	fade.var = 0;
	test.AudioPlay(fade, ch);

	while(1)
	{
		
	}
	
	/*
	FMOD::System *fmod_System;
	FMOD::System_Create(&fmod_System);
	FMOD::Sound  * sound;
	FMOD::Channel* Handle;
	*/
	/*
	ARCinfo *info;
	unsigned int size = 0;
	char* ret = info->getFile("bgm/02", size);
	*/
	/*
	FILE *fin = fopen("0.mp3","rb");
	fseek(fin,0,SEEK_END);
	unsigned int FileSize=ftell(fin);
	rewind(fin);
	char *pFile=new char [FileSize];
	fread(pFile,FileSize,1,fin);
	fclose(fin);
	*/

	/*
	FILE *fout = fopen("dump", "wb");
	fwrite(ret, 1, size, fout);
	fclose(fout);
	*/
	/*
	FMOD_CREATESOUNDEXINFO in = {0};
	//memset(&in, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	in.cbsize  =sizeof(FMOD_CREATESOUNDEXINFO);
	in.length = size;
	fmod_System->init(MAX_Audio_Size + MAX_Sound_Size, FMOD_INIT_NORMAL, NULL);
	fmod_System->createSound((const char*)ret, FMOD_OPENMEMORY|FMOD_SOFTWARE|FMOD_2D|FMOD_CREATESAMPLE, &in, &(sound));
	fmod_System->playSound(FMOD_CHANNEL_FREE , sound, false, &(Handle));
	fmod_System->update();
	bool isPlaying  = true;
	
	
	while(isPlaying)
	{
		Handle->isPlaying(&isPlaying);
	}
	*/
	
	return 0;
}

