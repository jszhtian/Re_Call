#include "ScriptEngine.h"
#include "Exception.h"
#include "CoroutineManager.h"

ScriptEngine* Handle = nullptr;

ScriptEngine* GetHandle()
{
	return Handle;
}

ScriptEngine* Init()
{
	if (Handle == nullptr)
	{
		Handle = new ScriptEngine;
		if (!Handle)
		{
			throw std::string("Couldn't Initialization ScriptEngine");
		}
	}
	return Handle;
}


ScriptEngine::~ScriptEngine()
{
	if(ScriptBuffer)
		delete[] ScriptBuffer;

	ScriptBuffer = nullptr;
}

unsigned int ScriptEngine::GetOEP()
{
	return EntryOffset;
}
unsigned short ScriptEngine::GetHeight()
{
	return Height;
}

unsigned short ScriptEngine::GetWidth()
{
	return Width;
}

std::string ScriptEngine::GetTitle()
{
	return Title;
}

bool ScriptEngine::LoadScript(const char* str)
{
	FILE* file = fopen(str, "rb");
	
	if (!file)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	AnzUInt ScriptLength = ftell(file);
	rewind(file);
	ScriptBuffer = new AnzUInt8[ScriptLength];
	fread(ScriptBuffer, 1, ScriptLength, file);
	fclose(file);

	HeaderOffset = *(AnzUInt*)(ScriptBuffer);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	HeaderOffset = SDL_Swap32(HeaderOffset);
#endif

	Title = std::string((char*)(ScriptBuffer + HeaderOffset + sizeof(HcbHeader)));

	if (HeaderOffset >= ScriptLength)
	{
		delete[] ScriptBuffer;
		ScriptBuffer = nullptr;
		return false;
	}

	HcbHeader *hcbHeader = (HcbHeader*)(ScriptBuffer + HeaderOffset);

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	EntryOffset = hcbHeader->EntryPoint;
#else
	EntryPoint = SDL_Swap32(hcbHeader->EntryPoint);
#endif

	AnzUShort ResolutionIndex = 1;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	ResolutionIndex = hcbHeader->ResolutionIndex;
#else
	ResolutionIndex = SDL_Swap16(hcbHeader->ResolutionIndex);
#endif

	switch (ResolutionIndex)
	{
	case 1: Width = 800;  Height = 600; break;
	case 7: Width = 1024; Height = 640; break;
	default:
		ErrorLog(fvpError, "Unknown resolutionIndex, using[800x600] as default!\n");
		Height = 800;
		Width = 600;
		break;
	}
	ErrorLog(fvpLog, "Script Loaded!");

	return true;
}


//VM
bool __fastcall JZ(ScriptObject *stack)
{
	stack->ScriptPos++;
	AnzUInt hcbJZ = *(AnzUInt*)(stack->Buffer + stack->ScriptPos);
	stack->ScriptPos+=4;
		
	#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	hcbJZ = SDL_Swap32(hcbJZ);
	#endif

	if(stack->LocalStack[stack->CurStackFrame].Value.IntVar == 0)
	{
		stack->LocalStack[stack->CurStackFrame].Relase();
		stack->CurStackFrame--;
		stack->ScriptPos = hcbJZ;
	}
	else
	{
		stack->LocalStack[stack->CurStackFrame].Relase();
		stack->CurStackFrame--;
	}
	return true;
}

//0x06
bool __fastcall JMP(ScriptObject *stack)
{
	stack->ScriptPos++;
	
	AnzUInt hcbJMP = *(AnzUInt*)(stack->Buffer + stack->ScriptPos);
	
	#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	hcbJMP = SDL_Swap32(hcbJMP);
	#endif

	stack->ScriptPos = hcbJMP;
	return true;
}

bool __fastcall PushInt32(ScriptObject *stack)
{
	stack->ScriptPos++;
	stack->CurStackFrame++;

	VarInfo* var = &(stack->LocalStack[stack->CurStackFrame]);
	var->type = VarInfo::pINT;
	var->Value.IntVar =
		*(AnzUInt*)(stack->Buffer + stack->ScriptPos);

	#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	var->Value.IntVar  = SDL_Swap32(var->Value.IntVar);
	#endif
	
	stack->ScriptPos += 4;
	return true;
}

bool __fastcall PushInt16(ScriptObject *stack)
{
	stack->ScriptPos++;
	stack->CurStackFrame++;

	VarInfo* var = &(stack->LocalStack[stack->CurStackFrame]);
	var->type = VarInfo::pINT;
	var->Value.IntVar =
		*(AnzUShort*)(stack->Buffer + stack->ScriptPos);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	var->Value.IntVar  = SDL_Swap32(var->Value.IntVar);
#endif

	stack->ScriptPos += 2;
	return true;
}

bool __fastcall PushInt8(ScriptObject *stack)
{
	stack->ScriptPos++;
	stack->CurStackFrame++;

	VarInfo* var = &(stack->LocalStack[stack->CurStackFrame]);
	var->type = VarInfo::pINT;
	var->Value.IntVar = *(AnzUChar*)(stack->Buffer + stack->ScriptPos);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	var->Value.IntVar  = SDL_Swap32(var->Value.IntVar);
#endif
	stack->ScriptPos += 1;
	return true;
}

bool __fastcall PushFloat32(ScriptObject *stack)
{
	stack->ScriptPos++;
	stack->CurStackFrame++;

	VarInfo* var = &(stack->LocalStack[stack->CurStackFrame]);
	var->type = VarInfo::pINT;
	var->Value.FloatVar = *(float*)(stack->Buffer + stack->ScriptPos);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	var->Value.FloatVar = SDL_SwapFloat(var->Value.FloatVar);
#endif

	stack->ScriptPos += 4;
	return true;
}

//0x08
bool __fastcall Push0(ScriptObject *stack)
{
	stack->ScriptPos++;
	stack->CurStackFrame++;

	VarInfo* var = &(stack->LocalStack[stack->CurStackFrame]);
	var->type = VarInfo::pINT;
	var->Value.IntVar  = 0;

	return true;
}

bool __fastcall Push1(ScriptObject *stack)
{
	stack->ScriptPos++;
	stack->CurStackFrame++;

	VarInfo* var = &(stack->LocalStack[stack->CurStackFrame]);
	var->type = VarInfo::pINT;
	var->Value.IntVar = 1;

	return true;
}

//pushGlobal(i16 num) 0x0F
bool __fastcall PushGlobal(ScriptObject *stack)
{
	stack->ScriptPos++;

	AnzUInt16 hcbTemp = *(AnzUInt16*)(stack->Buffer + stack->ScriptPos);
	
	#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	hcbTemp = SDL_Swap16(hcbTemp);
	#endif

	//使用search key
	stack->CurStackFrame++;
	stack->LocalStack[stack->CurStackFrame] = *(ScriptEngine::GetHandle()->GlobalVar[hcbTemp]);
	stack->ScriptPos += 2;
	return true;
}

bool __fastcall PopGlobal(ScriptObject *stack)
{
	stack->ScriptPos++;
	AnzUInt16 hcbIndex = *(AnzUInt16*)(stack->Buffer + stack->ScriptPos);
	
	#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	hcbIndex = SDL_Swap16(hcbIndex);
	#endif

	ScriptEngine::GetHandle()->GlobalVar.Set(hcbIndex,
		stack->LocalStack[stack->CurStackFrame]);
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->ScriptPos +=2;
	return true;
}

bool __fastcall PushString(ScriptObject *stack)
{
	stack->ScriptPos++;
		
	AnzUInt8 StringLength = *(AnzUInt8*)(stack->Buffer + stack->ScriptPos);
	stack->ScriptPos++;
		
	stack->CurStackFrame++;
		
	VarInfo* hcbTemp = &(stack->LocalStack[stack->CurStackFrame]);
	hcbTemp->type = VarInfo::STRING;
	hcbTemp->Value.StringVar = new tString;
	hcbTemp->Value.StringVar->length = StringLength - 1;
	hcbTemp->Value.StringVar->buffer = new char[StringLength];
	memcpy(hcbTemp->Value.StringVar->buffer, 
	stack->Buffer+stack->ScriptPos, StringLength);

	stack->ScriptPos += StringLength;
	return true;
}

bool __fastcall Nagetive(ScriptObject *stack)
{
	stack->ScriptPos++;
	
	switch(stack->LocalStack[stack->CurStackFrame].type)
	{
	case VarInfo::pINT:
		stack->LocalStack[stack->CurStackFrame].Value.IntVar = 
			-stack->LocalStack[stack->CurStackFrame].Value.IntVar;
		break;
		
	case VarInfo::pFLOAT:
		stack->LocalStack[stack->CurStackFrame].Value.FloatVar = 
			-stack->LocalStack[stack->CurStackFrame].Value.FloatVar;
		break;
	}
	return true;
}

bool __fastcall Add(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1] = ((*temp2) + (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall Sub(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1] = ((*temp2) - (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall Mul(ScriptObject *stack)
{
	stack->ScriptPos++;
	
	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1] = ((*temp2) * (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall Div(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1] = ((*temp2) / (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall Mod(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1] = ((*temp2) % (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall BitTest(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	AnzUInt Result = (temp2->Value.IntVar &(1 << temp1->Value.IntVar)) ? 1 : 0;
	temp1->Relase();
	temp2->Relase();

	temp2->type = VarInfo::pINT;
	temp2->Value.IntVar = Result;
	stack->CurStackFrame--;
	return true;
}

bool __fastcall CondAnd(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar = 
		temp2->Value.IntVar && temp1->Value.IntVar;
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall CondOR(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar =
		temp2->Value.IntVar || temp1->Value.IntVar;
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall SetE(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Relase();
	stack->LocalStack[stack->CurStackFrame - 1].type = VarInfo::pINT;
	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar = ((*temp2) == (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall SetNE(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Relase();
	stack->LocalStack[stack->CurStackFrame - 1].type = VarInfo::pINT;
	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar = ((*temp2) != (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall SetG(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Relase();
	stack->LocalStack[stack->CurStackFrame - 1].type = VarInfo::pINT;
	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar = ((*temp2) > (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall SetLE(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Relase();
	stack->LocalStack[stack->CurStackFrame - 1].type = VarInfo::pINT;
	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar = ((*temp2) <= (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall SetL(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Relase();
	stack->LocalStack[stack->CurStackFrame - 1].type = VarInfo::pINT;
	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar = ((*temp2) < (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall SetGE(ScriptObject *stack)
{
	stack->ScriptPos++;

	VarInfo* temp1 = &(stack->LocalStack[stack->CurStackFrame - 0]);
	VarInfo* temp2 = &(stack->LocalStack[stack->CurStackFrame - 1]);

	stack->LocalStack[stack->CurStackFrame - 1].Relase();
	stack->LocalStack[stack->CurStackFrame - 1].type = VarInfo::pINT;
	stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar = ((*temp2) >= (*temp1));
	stack->LocalStack[stack->CurStackFrame - 0].Relase();
	stack->CurStackFrame--;
	return true;
}

bool __fastcall InitStack(ScriptObject *stack)
{
	stack->ScriptPos++;

	stack->LocalVarCount = *(AnzUInt8*)(stack->Buffer + stack->ScriptPos);
	stack->ScriptPos++;
	stack->ParamVarCount = *(AnzUInt8*)(stack->Buffer + stack->ScriptPos);
	stack->ScriptPos++;
	return true;
}

//0x02  CALL
bool __fastcall Call(ScriptObject *stack)
{
	stack->ScriptPos++;
	
	AnzUInt hcbCall = *(AnzUInt*)(stack->Buffer + stack->ScriptPos);
	
	#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	hcbCall = SDL_Swap32(hcbCall);
	#endif

	stack->Next = new ScriptObject;
	stack->Next->ScriptPos    = hcbCall;
	stack->Next->ThreadID     = stack->ThreadID;
	stack->Next->Prev         = stack;

	//Push stack
	ThreadInstance
	_co->SetIterNext(stack);
	
	AnzUInt8 Count = *(AnzUInt8*)(stack->Buffer+hcbCall+1);
	AnzUInt8 start = 0xFE;
	if(Count != 0)
	{
		while(Count--)
		{
			stack->Next->LocalStack[start] = stack->LocalStack[stack->CurStackFrame];
			stack->LocalStack[stack->CurStackFrame].Relase();
			stack->CurStackFrame--;
			start--;
		}
	}
	stack->ScriptPos += 4;
	return true;
}

//0x04 return void
//不仅仅是返回，还有可能是线程函数结束
bool __fastcall ReturnVoid(ScriptObject *stack)
{
	stack->ScriptPos++;
		
	ThreadInstance
	if(!stack->Prev)
	{
		AnzUInt ID = stack->ThreadID;
		_co->DummyRelease(ID);
		ErrorLog(fvpLog, "Thread [%02x] Exit!\n", ID);
		return true;
	}

	_co->ReturnVoid(stack);
	return true;
}


//0x05 return eax

bool __fastcall ReturnWithEAX(ScriptObject *stack)
{
	stack->ScriptPos++;

	ThreadInstance
	if(!stack->Prev)
	{
		AnzUInt ID = stack->ThreadID;
		_co->DummyRelease(ID);
		ErrorLog(fvpLog, "Thread [%02x] Exit!\n", ID);
		return true;
	}
	_co->ReturnValue(stack);
	return true;
}


bool __fastcall PushStack(ScriptObject *stack)
{
	stack->ScriptPos++;
	
	AnzUInt8 hcbTemp = *(AnzUInt8*)(stack->Buffer + stack->ScriptPos);

	stack->CurStackFrame++;
	stack->LocalStack[stack->CurStackFrame] = stack->LocalStack[hcbTemp];
	stack->ScriptPos++;

	return true;
}

//0x13 PushTop
bool __fastcall PushTop(ScriptObject *stack)
{
	stack->ScriptPos++;
	stack->LocalStack[stack->CurStackFrame + 1] = stack->LocalStack[stack->CurStackFrame];
	stack->CurStackFrame++;
	return true;
}

//0x14 pushTemp
bool __fastcall PushTemp(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame + 1] = stack->Eax;
	stack->LocalVarCount++;
	stack->ScriptPos++;
	return true;
}


//0x16 stackcpy
bool __fastcall StackCopy(ScriptObject *stack)
{
	stack->ScriptPos++;	
	AnzUInt8 hcbTemp = *(AnzUInt8*)(stack->Buffer + stack->ScriptPos);

	stack->LocalStack[hcbTemp]  = stack->LocalStack[stack->CurStackFrame];
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->ScriptPos++;
	return true;
}


//0x12 
bool __fastcall PushLocalArray(ScriptObject *stack)
{
	stack->ScriptPos++;

	AnzUInt8 hcbTemp = *(AnzUInt8*)(stack->Buffer + stack->ScriptPos);

	VarInfo* var = stack->LocalArray.Get(hcbTemp, 
		stack->LocalStack[stack->CurStackFrame].Value.IntVar);
	
	stack->LocalStack[stack->CurStackFrame] = *var;
	stack->ScriptPos++;
	return true;
}

//0x18 Pop, value = (top), index = (top - 1)
bool __fastcall PopLocalArray(ScriptObject *stack)
{
	stack->ScriptPos++;
	
	AnzUInt8 hcbTemp = *(AnzUInt8*)(stack->Buffer + stack->ScriptPos);
	
	stack->LocalArray.Set(hcbTemp, 
		stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar,
		stack->LocalStack[stack->CurStackFrame]);

	stack->CurStackFrame--;
	return true;
}

//0x11
bool __fastcall PushGlobalArray(ScriptObject *stack)
{
	stack->ScriptPos++;
		
	AnzUInt16 hcbTemp = *(AnzUInt16*)(stack->Buffer + stack->ScriptPos);
	ScriptEngine* Engine = ScriptEngine::GetHandle();

	#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	hcbTemp = SDL_Swap16(hcbTemp);
	#endif
	
	VarInfo* var =  Engine->GlobalArray.Get(hcbTemp,
		stack->LocalStack[stack->CurStackFrame].Value.IntVar);

	stack->LocalStack[stack->CurStackFrame] = *var;

	stack->ScriptPos+=2;
	return true;
}


//0x17

bool __fastcall PopGlobalArray(ScriptObject *stack)
{
	stack->ScriptPos++;

	AnzUInt16 hcbTemp = *(AnzUInt16*)(stack->Buffer + stack->ScriptPos);
	ScriptEngine* Engine = ScriptEngine::GetHandle();

	Engine->GlobalArray.Set(hcbTemp,
		stack->LocalStack[stack->CurStackFrame - 1].Value.IntVar,
		stack->LocalStack[stack->CurStackFrame]);

	stack->ScriptPos += 2;
	stack->CurStackFrame--;
	return true;
}


bool __fastcall NopWithEx(ScriptObject *stack)
{
	stack->ScriptPos++;
	return true;
}


bool __fastcall SystemCall(ScriptObject *stack)
{
	stack->ScriptPos++;
	AnzUInt16 hcbCall = *(AnzUInt16*)(stack->Buffer + stack->ScriptPos);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	hcbCall = SDL_Swap16(hcbCall);
#endif
	(*ScriptEngine::SystemCallPool[hcbCall])(stack);

	stack->ScriptPos += 2;
	return true;
}

bool ScriptEngine::initVMCall()
{
	VMCallPool[0x00] = &NopWithEx;
	VMCallPool[0x01] = &InitStack; 
	VMCallPool[0x02] = &Call;      
	VMCallPool[0x03] = &SystemCall;
	VMCallPool[0x04] = &ReturnVoid;
	VMCallPool[0x05] = &ReturnWithEAX;
	VMCallPool[0x06] = &JMP;       
	VMCallPool[0x07] = &JZ;        
	VMCallPool[0x08] = &Push0;     
	VMCallPool[0x09] = &Push1;     
	VMCallPool[0x0A] = &PushInt32; 
	VMCallPool[0x0B] = &PushInt16; 
	VMCallPool[0x0C] = &PushInt8;  
	VMCallPool[0x0D] = &PushFloat32;
	VMCallPool[0x0E] = &PushString;
	VMCallPool[0x0F] = &PushGlobal;
	VMCallPool[0x10] = &PushStack; 
	VMCallPool[0x11] = &PushGlobalArray;
	VMCallPool[0x12] = &PushLocalArray;
	VMCallPool[0x13] = &PushTop;   
	VMCallPool[0x14] = &PushTemp;  
	VMCallPool[0x15] = &PopGlobal; 
	VMCallPool[0x16] = &StackCopy; 
	VMCallPool[0x17] = &PopGlobalArray;
	VMCallPool[0x18] = &PopLocalArray;
	VMCallPool[0x19] = &Nagetive;  
	VMCallPool[0x1A] = &Add;       
	VMCallPool[0x1B] = &Sub;       
	VMCallPool[0x1C] = &Mul;       
	VMCallPool[0x1D] = &Div;       
	VMCallPool[0x1E] = &Mod;       
	VMCallPool[0x1F] = &BitTest;   
	VMCallPool[0x20] = &CondAnd;   
	VMCallPool[0x21] = &CondOR;    
	VMCallPool[0x22] = &SetE;      
	VMCallPool[0x23] = &SetNE;     
	VMCallPool[0x24] = &SetG;      
	VMCallPool[0x25] = &SetLE;     
	VMCallPool[0x26] = &SetL;      
	VMCallPool[0x27] = &SetGE;
	return true;
}


void ScriptEngine::RunScript()
{
	initVMCall();
	ThreadInstance
	if (!_co->Add(0, EntryOffset))
	{
		RaiseExceptionEx("Failed to create the main thread of script model");
	}
	_co->Sc = _co->ThreadPool[0];
	_instance = _co;
	if (!ScriptBuffer)
	{
		RaiseExceptionEx("No vm code loaded!");
	}
	RtThread = new ThreadImpl;
	RtThread->Create(EntryOffset, 0, this);
}

