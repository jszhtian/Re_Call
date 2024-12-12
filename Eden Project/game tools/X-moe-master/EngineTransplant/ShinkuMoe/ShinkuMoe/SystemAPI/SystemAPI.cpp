#include <stdlib.h>
#include <SDL.h>
#include "SystemAPI.h"
#include "SaveLoadSystem.h"
#include "EventDispatcher.h"
#include "TimerManager.h"
#include "SaveLoad.h"
#include "HistoryManager.h"


//keyΪ1��ʱ���lockȫ���Ŀ���
//��������ED��ʱ������һ�в���
int __fastcall ControlMask(ScriptObject *object)
{
	int Flag = _stack.top().Value.IntVar;
	_stack.pop();
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	tEvent->SetControlMask(key);
	return 0; 
}

//û��param
// &= 0xFFFFFFFDUL;
int __fastcall ControlPulse(ScriptObject *object)
{
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	tEvent->SetControlPulse();
	return 0; 
}



int __fastcall CursorChange(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	return 0;
}

int __fastcall CursorMove(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	return 0;
}


int __fastcall CursorShow(ScriptObject *stack)
{
	switch (stack->LocalStack[stack->CurStackFrame].Value.IntVar)
	{
	case 0:
		SDL_ShowCursor(SDL_DISABLE);
		break;

	case 1:
		SDL_ShowCursor(SDL_ENABLE);
		break;
	}
	stack->CurStackFrame--;
	return 0;
}

//??
int __fastcall ExitDialog(ScriptObject *stack)
{
	return 0;
}

//return 1
//param -->0 ��һ��
//ȡֵ[0,4]
//Return: 1->success 0->fail
//unknown
int __fastcall ExitMode(ScriptObject *stack)
{
	int code = _stack.top().Value.IntVar;
	_stack.pop();

	int result  = 0;
	if(code >= 1 && code <= 4)
	{
		switch(code)
		{
		case 1:
			//*(_BYTE *)(LODWORD(dword_481368) + 129) = 1;
			//SysExitMode = 1;
			break;
		case 2:
			//SysExitMode = 0;
			break;
		case 3:
			/*
			  result = LODWORD(dword_481368);
              *(_BYTE *)(LODWORD(dword_481368) + 6979186) = 1;
              *(_BYTE *)(this + 6626664) = 1;
              *(_DWORD *)(this + 6626668) = *(_DWORD *)(this + 6973136);
			*/
			//result = SysExitMode;
			//SysExitMode = 1;
			break;
		case 4:
			//result = LODWORD(dword_481368);
			//result = SysExitMode;
			break;
		}
	}
	else //code == 0
	{
		//save global date
		//result = SysExitMode;
	}
	

	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar  = result;

	return 0;
}

//�����������
int __fastcall FlagGet(ScriptObject *stack)
{
	//Not Present Now
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	return -1;
}

//�����������
int __fastcall FlagSet(ScriptObject *stack)
{
	//Not Present Now 
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	return 0;
}

int __fastcall FloatToInt(ScriptObject *stack)
{
	stack->Eax.Relase();
	stack->Eax.Value.IntVar = (int)stack->LocalStack[stack->CurStackFrame].Value.IntVar;
	stack->CurStackFrame--;
	return 0;
}

//Format Length, Source
int __fastcall IntToText(ScriptObject *stack)
{
	int FormatLength = _stack.top().Value.IntVar;
	_stack.pop();
	int _Source      = _stack.top().Value.IntVar;
	_stack.pop();

	char szFormatString[20];
	sprintf(szFormatString, "%%0%dd", FormatLength);

	if(object->EAX.type != VarInfo::STRING && object->EAX.Value.StringVar == NULL)
	{
		object->EAX.Value.StringVar = new tString;
	}
	object->EAX.type = VarInfo::STRING;
	if(FormatLength)
	{
		sprintf(object->EAX.Value.StringVar->buffer, szFormatString, _Source);
	}
	else
	{
		sprintf(object->EAX.Value.StringVar->buffer, "%d", _Source);
	}
	return 0;
}

int SystemCallBack::HistoryGet(stack<VarInfo>& _stack, ScriptObject *object)
{
	int index = _stack.top().Value.IntVar;
	_stack.pop();
	int method= _stack.top().Value.IntVar;
	_stack.pop();

	HistoryManager* tHistory = HistoryManager::GetHandle();
	if(tHistory == NULL)
		return -1;

	switch(method)
	{
	case 0:
	{
		ReleaseStr(object);
		object->EAX.type = VarInfo::STRING;
		object->EAX.Value.StringVar = new tString;
		const char* tTemp = NULL;
		strcpy(object->EAX.Value.StringVar->buffer, tTemp = tHistory->GetTitle(index));
		object->EAX.Value.StringVar->length = strlen(tTemp) + 1;
	}
		break;
	case 1:
	{
		ReleaseStr(object);
		object->EAX.type = VarInfo::STRING;
		object->EAX.Value.StringVar = new tString;
		const char* tTemp = NULL;
		strcpy(object->EAX.Value.StringVar->buffer, tTemp = tHistory->GetText(index));
		object->EAX.Value.StringVar->length = strlen(tTemp) + 1;
	}
		break;
	case 2:
	{
		ReleaseStr(object);
		object->EAX.type = VarInfo::pINT;
		object->EAX.Value.IntVar =
			tHistory->GetVoice(index);
	}
		break;
	default:
		ErrorLog(fvpError, "Unknown History method = %d", method);
		break;
	}
	return 0;
}
/*
	HistorySet ( Global[0x00af], 0x00);
	HistorySet ( Global[0x00ad], 0x01);
	HistorySet ( Global[0x00dc], 0x02);
*/

//HistorySet:
/*
0: name(string)
1: main text(string)
2: voice(int)
*/
int __fastcall HistorySet(ScriptObject *stack)
{
	void* val = NULL;
	int tType = VarInfo::pINT;
	if(_stack.top().type == VarInfo::STRING)
	{
		int len = _stack.top().Value.StringVar->length;
		val = new char[len];
		memcpy(val, _stack.top().Value.StringVar->buffer, len);
		tType = VarInfo::STRING;
	}
	else
	{
		val = (void*)_stack.top().Value.IntVar;
	}
	_stack.pop();

	int  Index = _stack.top().Value.IntVar;
	_stack.pop();

	HistoryManager* tHistory = HistoryManager::GetHandle();
	if(tHistory == NULL)
		return -1;

	switch(Index)
	{
	case 0:
		tHistory->PushTitle((const char*)val);
		break;
	case 1:
		tHistory->PushText((const char*)val);
		break;
	case 2:
		tHistory->PushVoice((int)val);
		break;
	default:
		ErrorLog(fvpError, "Unknown History method = %d", Index);
		break;
	}
	return 0;
}


/*
00442409  |. 6A FF          PUSH -1                                  ; /Timeout = INFINITE
0044240B  |. 50             PUSH EAX                                 ; |hObject //hMutex = 0xF4
0044240C  |. FF15 60D04500  CALL DWORD PTR DS:[<&KERNEL32.WaitForSin>; \WaitForSingleObject
*/
//Win32�����ʹ�õģ���������
int __fastcall InputFlash(ScriptObject *stack)
{
	return 0;
}

//Unknown in fvp4.0 & fvp5.0
int __fastcall InputGetCursIn(ScriptObject *stack)
{
	SDL_PumpEvents();
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar = 0;
	return 0;
}

int __fastcall InputGetCursX(ScriptObject *stack)
{
	SDL_PumpEvents();
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar = tEvent->GetCurPosX();
	return 0;
}

int __fastcall InputGetCursY(ScriptObject *stack)
{
	SDL_PumpEvents();
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar = tEvent->GetCurPosY();
	return 0;
}

//���ĸ�����������ȥ��233
//ʹ�õĶ����Ƽ��ϱ���(1<<0 1<<2 1<<3...) 
int __fastcall InputGetDown(ScriptObject *stack)
{
	SDL_PumpEvents();
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar = tEvent->GetDown();
	return 0;
}

//�����������
int __fastcall InputGetEvent(ScriptObject *stack)
{
	//return SystemCallBack::StaticStatement;
	//Not Present Now!
	ErrorLog(fvpWarning, "Unknown method [InputGetEvent] was called! System will automatically return 0\n");
	return 0; 
}

int SystemCallBack::InputGetRepeat(stack<VarInfo>& _stack, ScriptObject *object)
{
	SDL_PumpEvents();
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar = tEvent->GetRepeat();
	return 0;
}

//һֱ����
int __fastcall InputGetState(ScriptObject *stack)
{
	SDL_PumpEvents();
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar = tEvent->GetState();
	return 0;
}

//̧������ 
int __fastcall InputGetUp(ScriptObject *stack)
{
	SDL_PumpEvents();
	EventDispatcher* tEvent = EventDispatcher::GetEventHandle();
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar = tEvent->GetUp();
	return 0;
}

//����0--���ϻ���
//С��0--���»���
//����0--û�л���

int __fastcall InputGetWheel(ScriptObject *stack)
{
	//����
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar =  0;
	return 0;
}

int __fastcall InputSetClick(ScriptObject *stack)
{
	//Not Present Now
	return 0;
}

//Load�ļ������˵�ǰ������+��ͼ�� 
int __fastcall Load(ScriptObject *stack)
{
	int index = _stack.top().Value.IntVar;
	_stack.pop();

	if(index < 0 || index > 999)
		return -1;
	
	FILE* saveHandle = 0;
	char tmpName[20] = {0};
	sprintf(tmpName, "s%03d.bin", index);
	saveHandle = fopen(tmpName, "rb");
	if(saveHandle == 0)
		return -1;
	
	///ToDo
	return 0;
}

int __fastcall MenuMessSkip(ScriptObject *stack)
{
	_stack.pop();
	return 0;
}

//Running in another thread!! 
//fvp5.0֮ǰ��Movie�����ö���ֻ�ǲ���op
//5.0��ʼ��Ҳ���Ժ�Ҳ�������ġ� Movie������EffectVideo�����á�����������ΪMovieEX 
//ֻ������Ч����
//VarInfo &filename: 1 args
int __fastcall Movie(ScriptObject *stack)
{
	char szFileName[100] = {0};
	strcpy(szFileName, _stack.top().Value.StringVar->buffer);
	_stack.pop();
	int i;
	for(i = strlen(szFileName); i>=0; i++)
	{
		if(szFileName[i] == '.')
			break;
	}
	if(strcmp(szFileName+i, "wmv")!=0 || strcmp(szFileName+i, "avi")!=0 || strcmp(szFileName+i, "mpg")!=0)
	{
		ErrorLog(fvpWarning,"Format[%s] cannot be played on IOS drives[hardware rendering]!\n"
			"xMemoria Only Support H.264(420p) Video\n", szFileName+i);
	}
	memcpy(szFileName+i,"mp4",3);
	MoviePlayer* tPlayer = MoviePlayer::CetVideoHandle();
	tPlayer->Play(szFileName);
	return 0;
}

//since fvp 5.0
//flag, VarInfo &filename
//flag->0
int __fastcall MovieEX(ScriptObject *stack)
{
	int flag = _stack.top().Value.IntVar;
	_stack.pop();
	char szFileName[100] = {0};
	strcpy(szFileName, _stack.top().Value.StringVar->buffer);
	_stack.pop();

	int i;
	for(i = strlen(szFileName); i>=0; i++)
	{
		if(i == '.')
			break;
	}
	if(strcmp(szFileName+i, "wmv")!=0 || strcmp(szFileName+i, "avi")!=0 || strcmp(szFileName+i, "mpg")!=0)
	{
		ErrorLog(fvpWarning,"Format[%s] cannot be played on IOS drives[hardware rendering]!\n"
			"xMemoria Only Support H.264(420p) Video\n", szFileName+i);
	}
	memcpy(szFileName+i,"mp4",3);
	MoviePlayer* tPlayer = MoviePlayer::CetVideoHandle();
	tPlayer->Play(szFileName);
	return 0;
}

//Push��ȥ��Param:0--Launched 1--Playing
//�����״̬���棬�ͷ���0 ���򷵻�1 
int __fastcall MovieState(ScriptObject *stack)
{
	int flag = _stack.top().Value.IntVar;
	_stack.pop();

	MoviePlayer* tPlayer = MoviePlayer::CetVideoHandle();
	if(flag == 0)
	{
		if(tPlayer->isLaunched())
		{
			ReleaseStr(object)
			object->EAX.type = VarInfo::pINT;
			object->EAX.Value.IntVar = 1;
			return 1;
		}
		else
		{
			ReleaseStr(object)
			object->EAX.type = VarInfo::pINT;
			object->EAX.Value.IntVar  = 0;
			return 0;
		}
	}
	else
	{
		if(tPlayer->isPlaying())
		{
			ReleaseStr(object)
			object->EAX.type = VarInfo::pINT;
			object->EAX.Value.IntVar = 1;
			return 1;
		}
		else
		{
			ReleaseStr(object)
			object->EAX.type = VarInfo::pINT;
			object->EAX.Value.IntVar  = 0;
			return 0;
		}
	}
}

//ֱ��ֹͣMovie--����fvp5.0�����ܼ����Ժ� ��˵��Movie������EffectVideo����OP֮���
//��ֻ�ܼ���һ���� 
//����ֹͣ��ʱ�� ֱ��ֹͣ��������ok�� 
int __fastcall MovieStop(ScriptObject *stack)
{
	MoviePlayer* tPlayer = MoviePlayer::CetVideoHandle();
	if(tPlayer->isPlaying())
	{
		tPlayer->Stop();
		delete tPlayer;
		tPlayer = NULL;
		Renderer::GetHandle()->RemoveVideoBuffer();
	}
	return 0;
}

//ò����ָ����type
//�ɹ��Ļ� �ͷ���1-->��ô...SaveData��������ڣ� 
/*
	var_00 = SaveCreate ( Global[0x0355], 0x00);
	var_00 = SaveCreate ( "�ͣ��С��ͣϣ֣ţͣţΣ�", 0x02);
	var_00 = SaveCreate ( 0x0388, 0x03);
*/
//TYPE : [0, 3]
/*
0 ��String
Title?
//Refer to a null string or a white string

1 : String
//Never used

2 : String 
MainString->���浱ǰ��ʾ��һ�仰

3 : Int
CreateBuffer By Index

*/
int __fastcall SaveCreate(ScriptObject *stack)
{
	int tType = VarInfo::pINT;
	void* val = NULL;
	if(_stack.top().type == VarInfo::pINT)
	{
		tType = VarInfo::pINT;
		val = (void*)_stack.top().Value.IntVar;
	}
	else
	{
		tType = VarInfo::STRING;
		int length = _stack.top().Value.StringVar->length;
		val = new char[length];
		memcpy(val, _stack.top().Value.StringVar->buffer, length);
	}
	_stack.pop();
	int type  = _stack.top().Value.IntVar;
	_stack.pop();

	SaveLoad* tSave = SaveLoad::GetSaveLoadHandle();

	int ret = 0;
	switch (type)
	{
	case 0:
		tSave->CreateSaveBufferTitle((const char*)val);
		break;

	case 1:
		ErrorLog(fvpError, "Unknown SaveCreate method called!");
		break;

	case 2:
		tSave->CreateSaveBufferTitle((const char*)val);
		break;

	case 3:
		tSave->CreateBufferIndex((int)val);
		break;

	default:
		ErrorLog(fvpError, "Unknown SaveCreate method called!");
		break;
	}

	ReleaseStr(object);
	object->EAX.Value.IntVar = ret;
	object->EAX.type= VarInfo::pINT;

	if(tType == VarInfo::STRING)
	{
		delete[] val;
	}
	return 0;
}

//SaveThumbSize ( 0x64, 0x00a0);
//�趨�浵��ͼ�ĳ���  ������趨֮���ÿһ���浵
//���õ��������֮��ϵͳ�Ϳ�ʼ����Ļ��ͼ
int __fastcall SaveThumbSize(ScriptObject *stack)
{
	int SaveHeight = _stack.top().Value.IntVar;
	_stack.pop();
	int save_size_width  = _stack.top().Value.IntVar;
	_stack.pop();

	SaveLoad* tS = SaveLoad::GetSaveLoadHandle();
	tS->SetThumbSize(save_size_width, save_size_height);
	tS->GetThumbData();
	return 0;
}


//if(! (SaveData ( 0, var_fe, 0x01) == 1)) goto hcb_000761f7;
//���һ���浵�Ƿ���� �����򷵻�1 ���򷵻�0 
//��һ����Զ��0��������
//�ڶ�����index����Ҫ���Ĵ浵���
//��������Զ��1�������� 

//���ԷŻ�String?
//TextPrint ( SaveData ( 0, Map2[0498][Global[0x04d2]], 0x06), 0x15);

/////���һ��������һ��flag Ϊ0x1��ʱ����Ƿ�������浵�Ƿ����
/////Ϊ0x6��ʱ�򣬾��Ƿ�������浵�Ĵ浵����


//VarInfo &unk, VarInfo &index, VarInfo &function_flag
/*
Flag:
01 -- just test, return 0 if target savefile existing
02 -- create a empty savefile, return 1 in order to avoid some unknown errors
03 -- copy savefile, return 1 order to avoid some unknown errors
*/

//TYPE:
/*
0 :
int index = 0;
while(index < 1000){ReadData(index); index++}
��ȡ�浵�ؼ����ݣ������������浵���ͼ��ػ�������
[�޷���ֵ]

1 :
���Դ浵�Ƿ����(memory method)
[bool����ֵ]

2 : DeteleFileA() called
ɾ��һ��ָ��index�Ĵ浵
[�޷���ֵ]

3 :
���ж���[0, 999]֮��ĺϷ�index
CopyFileA() called 
Copyһ���浵(a2 <- a3)
[�޷���ֵ]

4:
ָ���浵 *(_DWORD *)(v2 + 8) ��DWORD����
û�оͷ���0[String]

5 :
ָ���浵 *(_DWORD *)(v2 + 12) ��DWORD����
û�оͷ���0[String]

6 : //Main Text, return String
ָ���浵 *(_DWORD *)(v2 + 16) ��DWORD����
û�оͷ���0[String]

7 :
ָ���浵 *(_BYTE *)(v2 + 2) ��Word Flag����
û�оͷ���-1

8 :
ָ���浵 *(_BYTE *)(v2 + 2) ��Flag //��һ��Flag��WORD��С�������0x2��ʼ
����
û�оͷ���-1

9 :
ָ���浵 *(_BYTE *)(v2 + 3) ��Flag����
û�оͷ���-1

10:
ָ���浵 *(_BYTE *)(v2 + 4) ��Flag
û�оͷ���-1

11:
ָ���浵 *(_BYTE *)(v2 + 5) ��Flag��Сʱ��24Сʱ��
û�оͷ���-1

12:
ָ���浵 *(_BYTE *)(v2 + 6) ��Flag������
û�оͷ���-1

13:
��ȡ�����浵��ͼ
��ʱtSource Ϊһ��PrimIndex
[�޷���ֵ]

*/
int __fastcall SaveData(ScriptObject *object)
{
	int tSource = _stack.top().Value.IntVar;
	_stack.pop();
	int tTarget = _stack.top().Value.IntVar;
	_stack.pop();
	int mode       = _stack.top().Value.IntVar;
	_stack.pop();

	int ret = 1;
	switch (mode)
	{
	case 0:
	break;

	case 1:
	{
		char szName[20] = {0};
		sprintf(szName, "s%03d.bin", tTarget);
		FILE *hSave = NULL;
		hSave = fopen(szName, "rb");
		if(hSave == NULL)
		{
			ret = 0;
		}
		if(hSave)
		{
			fclose(hSave);
		}
		ReleaseStr(object)
		object->EAX.Value.IntVar = ret;
		object->EAX.type = VarInfo::pINT;
	}
	break;

	case 2:
	{
		char szName[20];
		sprintf(szName, "s%03d.bin", tTarget);
		FILE *hSave = NULL;
		hSave = fopen(szName, "wb");
		if(hSave == NULL)
		{
			ret = 0;
		}
		if(hSave)
		{
			fclose(hSave);
		}
		if(ret)
		{
			if(remove(szName) == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
		}
	}
	break;

	case 3:
	{
		char szSourceName[20];
		char szDestName[20];
		sprintf(szSourceName, "s%03d.bin", tSource);
		sprintf(szDestName, "s%03d.bin", tTarget);
		FILE *src = NULL;
		FILE *dest= NULL;

		src = fopen(szSourceName, "rb");
		if(szSourceName == NULL)
		{
			ret = 0;
			fclose(src);
			ErrorLog(fvpError, "Failed to copy savefile[%s], reason[file not found]\n", szSourceName);
			break;
		}
		dest = fopen(szDestName, "wb");
		if(dest == NULL)
		{
			fclose(src);
			fclose(dest);
			ret = 0;
			ErrorLog(fvpError, "Failed to Create Destination file[%s]\n", szDestName);
			break;
		}
		fseek(src,0,SEEK_END);
		unsigned int FileSize=ftell(src);
		rewind(src);
		char *pFile = NULL;

		bool b_Success = true;
		try
		{
			pFile = new char[FileSize];
		}
		catch(...)
		{
			delete[] pFile;
			ret = 0;
			b_Success = false;
			ErrorLog(fvpError, "Failed to Allocate memory to save file.\n");
		}

		if(!b_Success)
		{
			fclose(src);
			fclose(dest);
			break;
		}
		memset(pFile, 0, sizeof(pFile));
   		fread(pFile,FileSize,1,src);
		fclose(src);
		
		fwrite(pFile, 1, FileSize, dest);
		fclose(dest);
	}
	break;

	case 4:
	{
		ErrorLog(fvpError, "Unknown save method %d\n", mode);
		ReleaseStr(object)
		object->EAX.Value.StringVar = new tString;
		object->EAX.type = VarInfo::STRING;
	}
	break;

	case 5:
	{
		ErrorLog(fvpError, "Unknown save method %d\n", mode);
		ReleaseStr(object)
		object->EAX.Value.StringVar = new tString;
		object->EAX.type = VarInfo::STRING;
	}
	break;

	case 6:
	{
		ReleaseStr(object)
		object->EAX.Value.StringVar = new tString;
		SaveLoad* st = SaveLoad::GetSaveLoadHandle();
		std::string str;
		if(st->GetSaveDataStr(tTarget, str))
		{
			strcpy(object->EAX.Value.StringVar->buffer, str.c_str());
		}
		object->EAX.type = VarInfo::STRING;
	}
	break;

	case 7:
	{
		ReleaseStr(object)
		SaveLoad* st = SaveLoad::GetSaveLoadHandle();
		int tYear = -1;
		if((tYear = st->GetSaveDataYear(tTarget)) != -1)
		{
			object->EAX.Value.IntVar = tYear;
		}
		else
		{
			object->EAX.Value.IntVar  = -1;
		}
		object->EAX.type = VarInfo::pINT;
	}
	break;

	case 8:
	{
		ReleaseStr(object)
		SaveLoad* st = SaveLoad::GetSaveLoadHandle();
		int tMonth = -1;
		if((tMonth = st->GetSaveDataMonth(tTarget)) != -1)
		{
			object->EAX.Value.IntVar = tMonth;
		}
		else
		{
			object->EAX.Value.IntVar  = -1;
		}
		object->EAX.type = VarInfo::pINT;
	}
	break;

	case 9:
	{
		ReleaseStr(object)
		SaveLoad* st = SaveLoad::GetSaveLoadHandle();
		int tDay = -1;
		if((tDay = st->GetSaveDataDay(tTarget)) != -1)
		{
			object->EAX.Value.IntVar = tDay;
		}
		else
		{
			object->EAX.Value.IntVar  = -1;
		}
		object->EAX.type = VarInfo::pINT;
	}
	break;

	case 10:
	{
		ErrorLog(fvpError, "Unknown save method %d\n", mode);
		ReleaseStr(object)
		object->EAX.Value.IntVar = -1;
		object->EAX.type = VarInfo::STRING;
	}
	break;

	case 11:
	{
		ReleaseStr(object)
		SaveLoad* st = SaveLoad::GetSaveLoadHandle();
		int tHour = -1;
		if((tHour = st->GetSaveDataHour(tTarget)) != -1)
		{
			object->EAX.Value.IntVar = tHour;
		}
		else
		{
			object->EAX.Value.IntVar  = -1;
		}
		object->EAX.type = VarInfo::pINT;
	}
	break;

	case 12:
	{
		ReleaseStr(object)
		SaveLoad* st = SaveLoad::GetSaveLoadHandle();
		int tMin = -1;
		if((tMin = st->GetSaveDataMin(tTarget)) != -1)
		{
			object->EAX.Value.IntVar = tMin;
		}
		else
		{
			object->EAX.Value.IntVar  = -1;
		}
		object->EAX.type = VarInfo::pINT;
	}
	break;

	case 13:
	{
		SaveLoad* st = SaveLoad::GetSaveLoadHandle();
		st->GetSaveDataImage(tTarget, tSource);
	}
	break;

	default:
		ErrorLog(fvpError, "SaveFile: unknown mothod[%d]\n", mode);
	break;
	}

	ReleaseStr(object);
	object->EAX.Value.IntVar = ret;
	object->EAX.type= VarInfo::pINT;

	return ret;
}

//д��浵 
//����ֱ�Ӹ���
int __fastcall SaveWrite(ScriptObject *stack)
{
	int index = _stack.top().Value.IntVar;
	_stack.pop();
	char *name = new char[20];
	sprintf(name, "s%03d.bin", index);
	FILE *bin = fopen(name, "wb");
	if(bin == 0)
	{
		ErrorLog(fvpError, "Cannot Write a savefile[%s]", name);
		delete[] name;
		return -1;
	}
	/**************************/
	//1  д��stack��Ϣ ����Ŀǰ�ĸ���״̬
	//2  д����Ƶ��Ϣ
	//3  д��Sprite��Ϣ

	delete[] name;
	return 0;
}

//Menu�е�
int __fastcall SysAtSkipName(ScriptObject *stack)
{
	_stack.pop();
	_stack.pop();
	return 0;
}

int __fastcall SysProjFolder(ScriptObject *stack)
{
	_stack.pop();
	return 0;
}

//if(! (TimerGet ( 0x64, 0x04) <= 0x64)) goto hcb_000524a4;
//�趨һ�����ٽ�ֵ��TimerSet���������Ժ�....��ô����Ҫ��ʱ 
//�������GetҪ������ 
int TimerGet(ScriptObject *stack)
{
	int setTime = _stack.top().Value.IntVar;
	_stack.pop();
	int index   = _stack.top().Value.IntVar;
	_stack.pop();

	if(index < 0 || index > 15)
		return -1;
	
	TimerManager* tTimer = TimerManager::GetHandle();
	ReleaseStr(object)
	object->EAX.Value.IntVar = tTimer->Get(index, setTime);
	object->EAX.type = VarInfo::pINT;

	return 0;
}

int __fastcall TimerSet(ScriptObject *stack)
{
	int args = _stack.top().Value.IntVar;
	_stack.pop();
	int index= _stack.top().Value.IntVar;
	_stack.pop();

	if(index < 0 || index > 15)
		return -1;

	TimerManager* tTimer = TimerManager::GetHandle();
	tTimer->Set(index, args);
	return 0;
}

int __fastcall TimerSuspend(ScriptObject *object)
{
	//Not Present Now
	int Flag = _stack.top().Value.IntVar;
	_stack.pop();
	TimerManager* tTimer = TimerManager::GetHandle();
	tTimer->SetSuspend(Flag);
	return 0;
}

//nullsub in original code
//We do nothing here
int __fastcall TitleMenu(ScriptObject *stack)
{
	int bFlag = _stack.top().Value.IntVar;
	_stack.pop();
	return 0;
}

//�趨ȫ���ʹ���ģʽ��233
int __fastcall WindowMode(ScriptObject *stack)
{
	_stack.pop();
	ReleaseStr(object)
	object->EAX.type = VarInfo::pINT;
	object->EAX.Value.IntVar  = 0;
	return 0;
}

int __fastcall Rand(ScriptObject *stack)
{
	srand(time(NULL));
	ReleaseStr(object)
	object->EAX.Value.IntVar = rand();
	object->EAX.type= VarInfo::pINT;
	return 0;
}



