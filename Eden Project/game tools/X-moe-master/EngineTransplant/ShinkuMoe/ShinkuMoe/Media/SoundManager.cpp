#include "SoundManager.h"
#include "ErrorLog.h"

SoundManager* SoundManager::Handle = NULL;

SoundManager* SoundManager::GetHandle()
{
	if (Handle == NULL)
	{
		Handle = new SoundManager;
	}
	return Handle;
}

SoundManager::SoundManager()
{
	memset(AudioList, 0, sizeof(AudioList));
	memset(SoundList, 0, sizeof(SoundList));
}


SoundManager::~SoundManager()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (AudioList[i])
		{
			Mix_HaltChannel(i);
			delete[] AudioList[i];
			AudioList[i] = nullptr;
		}
	}
	for (i = 0; i < 256; i++)
	{
		if (SoundList[i])
		{
			Mix_HaltChannel(i + MAX_Audio_Size);
			delete[] SoundList[i];
			SoundList[i] = 0;
		}
	}
}

int SoundManager::OpenAudio()
{
	return Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096);
}

int SoundManager::LoadLib()
{
	return Mix_Init(MIX_INIT_OGG);
}

int SoundManager::AllocateChannels()
{
	int ret = Mix_AllocateChannels(MAX_Audio_Size + MAX_Sound_Size);
	if(ret != MAX_Audio_Size + MAX_Sound_Size)
	{
		WriteLog(fvpError, "System require %d channels, but in fact we can only allocate %d channels",
			MAX_Audio_Size + MAX_Sound_Size, ret);
	}
	return ret;
}


bool SoundManager::Load(int attr, int index, const char *szFileName)
{
	switch(attr)
	{
		case Audio:
		{
			if(AudioList[index])
			{
				if (Mix_Playing(index))
					Mix_HaltChannel(index);

				delete AudioList[index];
				AudioList[index] = nullptr;
				AudioList[index] = NULL;
			}

			Stream* stream = LoadFile(szFileName);
			if(!stream)
			{
				WriteLog(fvpError, "Failed Load Audio %s %d\n", szFileName, index);
				return false;
			}

			SoundList[index] = new SoundInfo;
			AudioList[index]->stream = stream;
			AudioList[index]->op = SDL_RWFromMem(AudioList[index]->stream->Buffer, AudioList[index]->stream->Size);
			AudioList[index]->Handle = Mix_LoadWAV_RW(AudioList[index]->op, 0);
		}
		break;

		case Sound:
		{
			if(SoundList[index])
			{
				if (Mix_Playing(index + MAX_Audio_Size))
					Mix_HaltChannel(index + MAX_Audio_Size);
				
				delete SoundList[index];
				SoundList[index] = nullptr;
			}
			
			Stream* stream = LoadFile(szFileName);
	
			if(!stream)
				return false;

			SoundList[index] = new SoundInfo;
			SoundList[index]->stream = stream;
			SoundList[index]->op = SDL_RWFromMem(SoundList[index]->stream->Buffer, SoundList[index]->stream->Size);
			SoundList[index]->Handle = Mix_LoadWAV_RW(SoundList[index]->op, 0);
		}
		break;

		default:
			WriteLog(fvpError, "SoundManager: Unknown sound attribution");
		return false;
	}
	return true;
}


bool SoundManager::Unload(int attr, int index)
{
	switch(attr)
	{
		case Audio:
		{
			if(AudioList[index])
			{
				if (Mix_Playing(index))
					Mix_HaltChannel(index);
				
				delete AudioList[index];
				AudioList[index] = nullptr;
			}
		}
		break;

		case Sound:
		{
			if(SoundList[index])
			{
				if (Mix_Playing(index + MAX_Audio_Size))
					Mix_HaltChannel(index + MAX_Audio_Size);

				delete SoundList[index];
				SoundList[index] = nullptr;
			}
		}
		break;

		default:
			WriteLog(fvpError, "SoundManager: Unknown sound attribution");
		return false;
	}
	return true;
}

bool SoundManager::SoundHasData(int index)
{
	return SoundList[index] == NULL ? false : true;
}

bool SoundManager::AudioHasData(int index)
{
	return AudioList[index] == NULL ? false : true;
}

void SoundManager::AudioSilentOn(int Index)
{
	if (AudioList[Index] != 0)
	{
		if (Mix_Playing(Index))
			Mix_Pause(Index);
		else
			Mix_Resume(Index);
	}
}

void SoundManager::AudioSetType(int index, int type)
{
	AudioList[index]->GroupId = type;
}

void SoundManager::PlaySound(int ch, int fadetime, bool loop)
{
	if(fadetime <= 0)
	{
		Mix_Volume(ch+MAX_Audio_Size, 128);
		Mix_PlayChannel(ch+MAX_Audio_Size, SoundList[ch]->Handle, loop == 1? -1:0);
	}
	else
	{
		Mix_Volume(ch+MAX_Audio_Size, 128);
		Mix_FadeInChannel(ch+MAX_Audio_Size, SoundList[ch]->Handle, loop == 1? -1:0, fadetime);
	}
}


void SoundManager::SoundSilentOn(int Index)
{
	if (SoundList[Index] != 0)
	{
		if (Mix_Playing(Index + MAX_Audio_Size))
			Mix_Pause(Index + MAX_Audio_Size);
		else
			Mix_Resume(Index + MAX_Audio_Size);
	}
}

void SoundManager::SoundSetType(int index, int type)
{
	SoundList[index]->GroupId = type;
}


//impl
void SoundManager::SetSoundMasterVol(int vol)
{
	return;
}

void SoundManager::SoundTypeVol(int type, float vol)
{
	for(unsigned int i = 0; i<= 255; i++)
	{
		if (SoundHasData(i) && (SoundList[i]->GroupId == type))
		{
			Mix_Volume(i + MAX_Audio_Size, vol);
		}
	}
}

void SoundManager::PlayAudio(int ch, bool loop)
{
	Mix_Volume(ch, 128);
	Mix_PlayChannel(ch, AudioList[ch]->Handle, 0, loop ? -1 : 0);
}


bool SoundManager::SoundGetState(int index)
{
	if (SoundList[index] && Mix_Playing(index + MAX_Audio_Size))
	{
		return true;
	}
	return false;
}


void SoundManager::AudioStop(int index, int fadetime)
{
	if (AudioHasData(index))
	{
		if (!fadetime)
		{
			Mix_HaltChannel(index);
		}
		else
		{
			Mix_FadeOutChannel(index, fadetime > 0 ? fadetime : 0);
		}
	}
}

void SoundManager::SoundStop(int index, int fadetime)
{
	if (SoundHasData(index))
	{
		if (!fadetime)
		{
			Mix_HaltChannel(index + MAX_Audio_Size);
		}
		else
		{
			Mix_FadeOutChannel(index + MAX_Audio_Size, fadetime > 0 ? fadetime : 0);
		}
	}
}


bool SoundManager::AudioGetState(int index)
{
	if (SoundList[index] && Mix_Playing(index))
	{
		return true;
	}
	return false;
}


void SoundManager::SoundSetVol(int index, float vol)
{
	if (!SoundHasData(index))
		return;

	Mix_Volume(index + MAX_Audio_Size, vol);
}

void SoundManager::AudioSetVol(int index, float vol)
{
	if (!AudioHasData(index))
		return;

	Mix_Volume(index, vol);
}
