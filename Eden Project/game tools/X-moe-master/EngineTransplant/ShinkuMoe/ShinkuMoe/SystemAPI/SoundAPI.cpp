#include "SoundAPI.h"
#include "ErrorLog.h"
#include "FileManager.h"
#include "SoundManager.h"

//const char *filename
//int index 
int __fastcall AudioLoad(ScriptObject *stack)
{
	const char* FileName = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar ? 
		stack->LocalStack[stack->CurStackFrame - 0].Value.StringVar->buffer : nullptr;

	AnzInt Channel       = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;

	if (Channel < 0 || Channel > SoundManager::Audio_End)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	if (FileName)
		_manager->Load(SoundManager::Audio, Channel, FileName);
	else
		_manager->Unload(SoundManager::Audio, Channel);

	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame -= 2;

	return 0;
}

//2 
//Loop(0/1) + ch
int __fastcall AudioPlay(ScriptObject *stack)
{
	int Loop    = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int Channel = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	
	stack->CurStackFrame -= 2;

	if(Channel < 0)
		return -1;
	if(Channel > SoundManager::MAX_Audio_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	_manager->PlayAudio(Channel, Loop);
	return 0;
}

int __fastcall AudioStop(ScriptObject *stack)
{
	int Loop     = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int Channel  = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;

	stack->CurStackFrame -= 2;
	
	if(Channel <0 || Channel > SoundManager::MAX_Audio_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();
	_manager->AudioStop(Channel, Loop);
	return 0;
}


int __fastcall AudioSilentOn(ScriptObject *stack)
{
	SoundManager *_manager = SoundManager::GetHandle();

	_manager->AudioSilentOn(stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar);
	stack->CurStackFrame--;
	return 0;
}

//当前Sound状态 
int __fastcall AudioState(ScriptObject *stack)
{
	int Channel = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	stack->CurStackFrame--;

	if (Channel < 0 || Channel > SoundManager::Audio_End)
	{
		stack->Eax.Relase();
		stack->Eax.Value.IntVar = 0;
		return 0;
	}

	SoundManager *_manager = SoundManager::GetHandle();

	if (!_manager->AudioGetState(Channel))
	{
		stack->Eax.Relase();
		stack->Eax.Value.IntVar = 0;
	}
	else
	{
		stack->Eax.Relase();
		stack->Eax.Value.IntVar = 0;
	}
	return 0;
}

int __fastcall AudioType(ScriptObject *stack)
{
	int type = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int ch   = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;

	stack->CurStackFrame -= 2;

	if(ch <= 0 || ch > SoundManager::MAX_Audio_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	if(!_manager->AudioHasData(ch))
		return -1;

	_manager->AudioSetType(ch, type);
	return 0;
}

/*
[0003e05f]PushInt8 (64);
[0003e061]Push0 ();
[0003e062]AudioVol (); //3
*/ 
//三个值  第一个应该是fade 
int __fastcall AudioVol(ScriptObject *stack)
{
	int fade = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int Vol  = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	int ch   = stack->LocalStack[stack->CurStackFrame - 2].Value.IntVar;
	
	stack->CurStackFrame -= 3;

	SoundManager *_manager = SoundManager::GetHandle();

	if(ch <= 0 || ch > 4)
		return -1;
	
	float real = (float)Vol/100.0f * 128.0f;
	
	_manager->SoundSetVol(ch, real);
	return 0;
}

/******************************************************************/
//Sound Impl : SE,系统音(Sound) 

//Push0 == Set Temp NULL
//const char *filename
//int channel 

int __fastcall SoundLoad(ScriptObject *stack)
{
	const char* FileName = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar ?
		stack->LocalStack[stack->CurStackFrame - 0].Value.StringVar->buffer : nullptr;

	AnzInt Channel = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;

	if (Channel < 0 || Channel > SoundManager::Audio_End)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	if (FileName)
		_manager->Load(SoundManager::Sound, Channel, FileName);
	else
		_manager->Unload(SoundManager::Sound, Channel);

	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame -= 2;
	return 0;
}

//impl
int __fastcall SoundMasterVol(ScriptObject *stack)
{
	float vol = (float)stack->LocalStack[stack->CurStackFrame].Value.IntVar/100.0f * 128.0f;
	//not impl yet

	stack->CurStackFrame--;
	return 0;
}


int __fastcall SoundPlay(ScriptObject *stack)
{
	int loop     = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int fadetime = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	int ch       = stack->LocalStack[stack->CurStackFrame - 2].Value.IntVar;
	
	stack->CurStackFrame -= 3;

	if(ch < 0)
		return -1;
	if(ch > SoundManager::MAX_Sound_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();
	
	if(!_manager->SoundHasData(ch))
		return -1;

	_manager->PlaySound(ch, fadetime, loop);
	return 0;
}


int __fastcall SoundSilentOn(ScriptObject *stack)
{
	SoundManager *_manager = SoundManager::GetHandle();
	_manager->SoundSilentOn(stack->LocalStack[stack->CurStackFrame].Value.IntVar);
	stack->CurStackFrame--;
	return 0;
}

//2个 
int __fastcall SoundStop(ScriptObject *stack)
{
	int fadetime = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int ch       = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	stack->CurStackFrame -= 2;

	if(ch <= 0 || ch > SoundManager::MAX_Sound_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	if(!_manager->SoundHasData(ch))
		return -1;

	_manager->SoundStop(ch, fadetime);
	return 0;
}


int __fastcall SoundType(ScriptObject *stack)
{
	int type = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int ch   = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	stack->CurStackFrame -= 2;

	if(ch <= 0 || ch > SoundManager::MAX_Sound_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	if(!_manager->SoundHasData(ch))
		return -1;

	_manager->SoundSetType(ch, type);
	return 0;
}


int __fastcall SoundTypeVol(ScriptObject *stack)
{
	float vol = (float)stack->LocalStack[stack->CurStackFrame].Value.IntVar / 100.0f * 128.0f;
	stack->CurStackFrame--;
	
	SoundManager *_manager = SoundManager::GetHandle();

	_manager->SoundTypeVol(stack->LocalStack[stack->CurStackFrame].Value.IntVar, vol);
	stack->CurStackFrame--;
	return 0;
}


//一共三个参数变量      ///fade?
int __fastcall SoundVol(ScriptObject *stack)
{
	int args = stack->LocalStack[stack->CurStackFrame - 0].Value.IntVar;
	int Vol =  stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	int ch =   stack->LocalStack[stack->CurStackFrame - 2].Value.IntVar;
	
	stack->CurStackFrame -= 3;

	if(ch < 0 || ch > SoundManager::MAX_Sound_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	if(!_manager->SoundHasData(ch))
		return -1;


	float vol = (float)Vol/100.0f * 128.0f;

	_manager->SoundSetVol(ch, vol);
	return 0;
}

//化石级别的API
int __fastcall SoundPan(ScriptObject *stack)
{
	int Pan = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	int ch  = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	stack->CurStackFrame -= 2;
	
	Mix_SetPanning(ch + SoundManager::MAX_Audio_Size, Pan, 0xFF - Pan);

	return 0;
}

/******************************************/
//fvp 1.0
int __fastcall SoundVolPrev(ScriptObject *stack)
{
	int Vol = stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	int obj = stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar;
	
	stack->CurStackFrame -= 2;

	if(obj < 0 || obj > SoundManager::MAX_Sound_Size)
		return -1;

	SoundManager *_manager = SoundManager::GetHandle();

	if(!_manager->SoundHasData(obj))
		return -1;

	float vol = (float)Vol/100.0f * 128.0f;

	_manager->SoundSetVol(obj, vol);
	return 0;
}



