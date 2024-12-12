#include "stdafx.h"

#include "VarInfo.h"
#include "HoshiSora.h"
#include "FileManager.h"

#include "FMOD\fmod.h"
#include "FMOD\fmod.hpp"
#include "FMOD\fmod_errors.h"


using namespace std;

HoshiSora::HoshiSora()
{
	fmod_Result = FMOD::System_Create(&fmod_System);
	if(fmod_Result != FMOD_OK)
	{
		//ErrorLog(fvpError, "Failed to Create a sound system! Please check your hardware or sound driver!\n");
		exit(-1);
	}

	fmod_Result = fmod_System->init(MAX_Audio_Size + MAX_Sound_Size, FMOD_INIT_NORMAL, NULL);
	if(fmod_Result != FMOD_OK)
	{
		//ErrorLog(fvpError, "Failed to Init the sound system!\n");
		exit(-1);
	}
}


/********************************************************************/
//Audio Impl : 人声(Koe) 

//Global Var , name 
//const char *filename
//int index 
int HoshiSora::AudioLoad(VarInfo &filename, VarInfo &ch)
{
	if(ch.var < 0  || ch.var > Audio_End)
		return -1;

	if(filename.type == VarInfo::pINT && filename.var == 0)//free
	{
		bool ret = false;
		AudioList[ch.var]->Handle->isPlaying(&ret);
		if(ret)
		{
			fmod_Result = fmod_System->playSound(FMOD_CHANNEL_FREE, (AudioList[ch.var]->sound), true, &(AudioList[ch.var]->Handle));
			fmod_System->update();
			AudioList[ch.var]->sound->release();
			AudioList[ch.var]->sound->release();
			AudioList[ch.var]->Handle->stop();
			if(AudioList[ch.var]->Buffer)
				delete[] AudioList[ch.var]->Buffer;
		}
		delete AudioList[ch.var];
		AudioList[ch.var] = 0;
		return 0;
	}
	if(!filename.str)
		return -1;

	AudioList[ch.var] = new SoundManager;
	FILE *debug3 = fopen("debug3.txt", "a");
	fprintf(debug3, "AudioLoad[%s] @ channel[%d]\n", filename.str, ch.var);
	fclose(debug3);

	ARCinfo *info;
	unsigned audio_size = 0;
	char *ret = info->getFile(filename.str, audio_size);
	if(ret == 0 || audio_size == 0)
		return -1;

	FMOD_CREATESOUNDEXINFO in = {0};
	//memset(&in, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	in.cbsize  =sizeof(FMOD_CREATESOUNDEXINFO);
	in.length = audio_size;

	//FMOD_RESULT ret;
	FMOD_RESULT fret = fmod_System->createSound((const char*)ret, FMOD_OPENMEMORY|FMOD_SOFTWARE|FMOD_2D|FMOD_CREATESAMPLE, &in, &(AudioList[ch.var]->sound));
	
	if(fret != FMOD_OK)
	{
		debug3 = fopen("debug3.txt", "a");
		fprintf(debug3, "Fmod --> failed to init system [%s]\n", FMOD_ErrorString(fret));
		fclose(debug3);
	}
	return 0;
}

//2 
int HoshiSora::AudioPlay(VarInfo &fadein, VarInfo &ch)
{
	if(ch.var <= -1)
		return -1;
	if(ch.var > MAX_Audio_Size)
		return -1;

	FILE *debug3  = fopen("debug3.txt", "a");
	fprintf(debug3, "AudioPlay @ [%d, %d]\n", fadein.var, ch.var);
	fclose(debug3);

	AudioList[ch.var]->Handle->setLoopCount(-1);
	AudioList[ch.var]->Handle->setMode(FMOD_LOOP_NORMAL);
	AudioList[ch.var]->Handle->setPosition(0, FMOD_TIMEUNIT_MS);
	
	float fade = (float) fadein.var/1000.0f;
	//AudioList[ch.var]->sound->setSoundGroup((AudioList[ch.var]->Group));
	//AudioList[ch.var]->Group->setMuteFadeSpeed(fade);
	fmod_System->playSound(FMOD_CHANNEL_FREE , AudioList[ch.var]->sound, false, &(AudioList[ch.var]->Handle));
	fmod_System->update();

	bool isPlaying  = true;
	
	
	while(isPlaying)
	{
		AudioList[ch.var]->Handle->isPlaying(&isPlaying);
	}
	/*
	if(!play_chunk[ch.var])
		return 0;
	if(fadein.var <= 0)
	{
		Mix_Volume(ch.var, 128);
		Mix_PlayChannel(ch.var, play_chunk[ch.var], 0);
	}
	else
	{
		Mix_Volume(ch.var, 128);
		Mix_FadeInChannel(ch.var, play_chunk[ch.var],0,fadein.var);
	}
	*/
	return 0;
}

int HoshiSora::AudioStop(VarInfo &fadetime ,VarInfo &ch)
{
	if(ch.var <0 || ch.var >4)
		return -1;
	if(AudioList[ch.var] == 0)
		return -1;

	float fade = (float) fadetime.var/1000.0f;
	AudioList[ch.var]->Group->setMuteFadeSpeed(fade);

	bool ret = true;
	AudioList[ch.var]->Handle->isPlaying(&ret);
	if(ret)
	{
		AudioList[ch.var]->Handle->stop();
		fmod_System->update();
	}
	/*
	if(!play_chunk[ch.var]) return 0;
	
	if(fadetime.var == 0) 
		Mix_HaltChannel(ch.var);
	else
		Mix_FadeOutChannel(ch.var, time);
	
	//Mix_FreeChunk(play_chunk[ch.var]);
	play_chunk[ch.var] = 0;
	*/
	return 0;
}

//1
//应该是暂停之类的 
int HoshiSora::AudioSilentOn(VarInfo &flag)
{
	if(flag.var == 1)
	{
		for(unsigned int i = 0; i <= 3; i++)
		{
			if(AudioList[i])
			{
				AudioList[i]->Handle->setPaused(true);
				fmod_System->update();
			}
		}
	}
	else
	{
		for(unsigned int i = 0; i <= 3; i++)
		{
			if(AudioList[i])
			{
				AudioList[i]->Handle->setPaused(false);
				fmod_System->update();
			}
		}
	}


	/*
	if(ch.var == 1)
	{
		for(int i = Audio_Start; i <= Audio_End; i++)
		{
			if(play_chunk[i] != 0) Mix_Pause(i);
		}
		
		return 1;
	}
	else if(ch.var == 0)
	{
		for(int i = Audio_Start; i <= Audio_End; i++)
		{
			if(play_chunk[i] != 0) Mix_Resume(i);
		}
		
		return 1;
	}
	*/
	return 0;
}

//三个值  第一个应该是fade 
int HoshiSora::AudioVol(VarInfo &ags1, VarInfo &Vol, VarInfo &ch)
{
	if(ch.var <= 0 || ch.var > 4)
		return -1;
	if(AudioList[ch.var] == 0)
		return -1;

	float vol = (float)Vol.var/1000.0f;
	AudioList[ch.var]->Handle->setVolume(vol);
	fmod_System->update();
	//Mix_Volume(ch.var, Vol.var);
	return 0;
}

