#pragma once

#include <string>
#include "Anz.h"
#include "ErrorLog.h"
#include "ArrayManager.h"
#include "GlobalVarManager.h"
#include "ScriptObject.h"
#include <SDL.h>
#include "CoroutineManager.h"


#define MaxSystemCall 150

#pragma pack(1)
struct HcbHeader
{
	AnzUInt32  EntryPoint;
	unsigned short count1;
	unsigned short count2;
	unsigned short ResolutionIndex; 
	unsigned char  Len;
};
#pragma pack() 

class ScriptEngine
{
private:
	ScriptEngine() :
		tabCount(0),
		ScriptBuffer(nullptr)
	{
		_ShouldExit = false;
	}

public:

	static ScriptEngine* GetHandle();

	bool        LoadScript(const char *str);
	AnzUInt     GetOEP();
	AnzUShort   GetHeight();
	AnzUShort   GetWidth();
	std::string GetTitle();

	bool        initVMCall();
	void        RunScript();

	class ThreadImpl: public ThreadFunction
	{
		int Function()
		{
			ScriptEngine* impl = (ScriptEngine*) _Buffer;
			ScriptObject* tPtr = impl->_instance->Sc->iter;
			if (tPtr == NULL)
			{
				RaiseExceptionEx("Null script object.");
			}
			while(!impl->_ShouldExit)
			{
				SDL_PumpEvents();

				bool Exec_error = false;
				bool stack_error;
				
				stack_error = (impl->*(impl->VMCallPool[*(unsigned char*)(tPtr->ScriptPos + impl->_scriptBuffer)]))(tPtr);

				if(Exec_error || !stack_error)
				{
					ErrorLog(fvpError, "Catched a bad code from script![%08x]\n", impl->_instance->Sc->iter->ScriptPos);
					RaiseExceptionEx("Catched a bad code from script!", "xMemoria");
					impl->_ShouldExit = true;
					break;
				}
			}//end while -- engine safety loop 
			ErrorLog(fvpLog, "---------The end of script----------");
			return 0;
		}
	};

	ThreadImpl* RtThread;

public:
	~ScriptEngine();

	bool SaveGlobalData();
	bool LoadGlobalData();

private:

	AnzUInt8 *ScriptBuffer;
	bool      ShouldExit;

	AnzUShort    Height;
	AnzUShort    Width;
	std::string  Title;
	AnzUInt      HeaderOffset;
	AnzUInt      EntryOffset;

	
	///WordMap & CharMap
	std::map<unsigned char ,std::map<VarInfo, VarInfo>> CharMap;
	std::map<unsigned short,std::map<VarInfo, VarInfo>> WordMap;
	std::map<unsigned char ,std::map<VarInfo, VarInfo>>::iterator itCharMap;
	std::map<unsigned short,std::map<VarInfo, VarInfo>>::iterator itWordMap;
	std::map<VarInfo       ,VarInfo              >::iterator itVarInfo;

	CoroutineManager* _instance;
	void InitThread()
	{
		ThreadInstance
		_instance = _co;
	}

	bool ScriptCreateThread(AnzUInt ThreadId, AnzUInt32 Offset);
	bool ScriptThreadSuspend(AnzUInt ThreadId);
	bool ScriptThreadResume(AnzUInt ThreadId);
	bool ScriptThreadExit(AnzUInt ThreadId);
	bool ScriptThreadNext();

public:
	typedef bool(__fastcall *VMCall)(ScriptObject * stack);
	static VMCall VMCallPool[0x28];
	
	typedef int(__fastcall *SysCall)(ScriptObject * stack);
	static SysCall SystemCallPool[MaxSystemCall];

public:
	GlobalVarManager GlobalVar;
	ArrayManager     GlobalArray;
};


