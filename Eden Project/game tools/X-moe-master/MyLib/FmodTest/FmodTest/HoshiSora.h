#ifndef __Hoshi__
#define __Hoshi__

#include "VarInfo.h"
#include "FMOD\fmod.h"
#include "FMOD\fmod.hpp"
#include "FMOD\fmod_errors.h"

#pragma comment(lib,"fmodex_vc.lib")

static const int MAX_Audio_Size = 4;
static const int MAX_Sound_Size = 256;

static const int Audio_Start    = 0;
static const int Audio_End      = 3;

static const int Sound_Start    = 4;
static const int Sound_End      = 256 + 3;

class HoshiSora
{
public:
	HoshiSora();
int AudioLoad(VarInfo &filename, VarInfo &ch);
int AudioPlay(VarInfo &fadein, VarInfo &ch);
int AudioStop(VarInfo &fadetime ,VarInfo &ch);
int AudioSilentOn(VarInfo &flag);
int AudioVol(VarInfo &ags1, VarInfo &Vol, VarInfo &ch);
	struct SoundManager
	{
		FMOD::Channel* Handle;
		FMOD::Sound  * sound;
		FMOD::SoundGroup *Group;
		char         * Buffer;
		int          type;

		SoundManager()
		{
			Handle = 0;
			sound  = 0;
			Buffer = 0;
			Group  = 0;
			type   = -1;//invalid handle value
		}
	};
	SoundManager * AudioList[4];
	
	FMOD::System *fmod_System;
	FMOD_RESULT   fmod_Result;
};

#endif
