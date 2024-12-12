#pragma once

#include <cstring>
#include <SDL.h>
#include <SDL_mixer.h>
#include "FileManager.h"


struct SoundInfo
{
	Mix_Chunk*  Handle;
	Stream*     stream;
	SDL_RWops*  op;
	int         GroupId;

	SoundInfo() :
		Handle(nullptr),
		stream(nullptr),
		op(nullptr),
		GroupId(0)
	{
	}
	~SoundInfo()
	{
		if(op)
			SDL_RWclose(op);
		if(stream)
			delete[] stream;
		if (Handle)
			Mix_FreeChunk(Handle);

		op     = nullptr;
		stream = nullptr;
		Handle = nullptr;
	}
};

class SoundManager
{
private:
	static SoundManager* Handle;
	SoundManager();

public:
	enum{Sound = 1, Audio = 2};
	~SoundManager();

	static SoundManager* GetHandle();
	int OpenAudio();
	int LoadLib();
	int AllocateChannels();

	bool Load(int attr, int index, const char *szFileName);
	bool Unload(int attr, int index);

	bool SoundHasData(int index);
	bool AudioHasData(int index);

	void AudioSilentOn(int Index);
	void AudioSetType(int index, int type);

	bool SoundGetState(int index);
	bool AudioGetState(int index);

	void PlaySound(int ch, int fadetime, bool loop);
	void PlayAudio(int ch, bool loop);

	void SoundSetVol(int index, float vol);
	void AudioSetVol(int index, float vol);

	void AudioStop(int index, int fadetime);
	void SoundStop(int index, int fadetime);

	void SoundSilentOn(int Index);
	void SoundSetType(int index, int type);
	void SoundTypeVol(int type, float vol);

	void SetSoundMasterVol(int vol);

public:
	static const int MAX_Audio_Size = 4;
	static const int MAX_Sound_Size = 256;

	static const int Audio_Start    = 0;
	static const int Audio_End      = 3;

	static const int Sound_Start    = 4;
	static const int Sound_End      = 256 + 3;

private:
	SoundInfo * AudioList[4];
	SoundInfo * SoundList[256];
};

