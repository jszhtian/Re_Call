#include "SystemPrevAPI.h"

/*
	SysProjFolder ( "AngelWish");
	ConfigDisplay ( 0x0080, 0x00e7, 0x56, 0x29, 0x00, 0x03, 0x01, 0x05, 0x05);
	ConfigEtc ( 0x00, 0x00, 0x01);
	ConfigSound ( 0x64, 0x64, 0x64, 0x64, 0x64);
	SysAtSkipName ( "��å��`�������å�[ON](&A)", "��å��`�������å�[OFF](&A)");
	funtion_0000f715();
	ChrAdd ( "voice/000000", 0x64, 0x33, "��ľ��   �}��");
	ChrAdd ( "voice/001153", 0x64, 0x34, "ǧ�r     ˮϣ");
	ChrAdd ( "voice/000634", 0x64, 0x35, "ǧ�r     �椭��");
	ChrAdd ( "voice/000309", 0x64, 0x36, "ϲ��Ҋ  ��֪");
	ChrAdd ( "voice/033693", 0x64, 0x37, "��ƽ     ����");
	ChrAdd ( "voice/001134", 0x64, 0x38, "�Ǒ���  �ߺ�");
	ChrAdd ( "voice/051058", 0x64, 0x39, "���");
	ChrAdd ( "voice/000190", 0x64, 0x3a, "�Q�R     ���F");
	ChrAdd ( "voice/000250", 0x64, 0x3b, "������");
	ConfigSet ();
*/


int __fastcall ConfigDisplay(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}


int __fastcall ConfigEtc(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;	
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}


int __fastcall ConfigSound(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

int __fastcall ConfigSet(ScriptObject *stack)
{
	return 0;
}

int __fastcall LoadFile(ScriptObject *stack)
{
	return 0;
}

int __fastcall LoadQuick(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

int __fastcall LoadTitle(ScriptObject *stack)
{
	return 0;
}

int __fastcall SaveFile(ScriptObject *stack)
{
	return 0;
}

int __fastcall SaveLoadMenu(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	return 0;
}

//String Name, int Index
int __fastcall SaveName(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

int __fastcall SaveQuick(ScriptObject *stack)
{
	//VarInfo &saveIndex
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	return 0;
}

//Never used
int __fastcall SaveTitle(ScriptObject *stack)
{
	return 0;
}

//VarInfo &a, VarInfo &b, VarInfo &c, VarInfo &d
int __fastcall TextHistory(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

//VarInfo &a, VarInfo &b, VarInfo &c
int __fastcall TextHyphenation(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

//unk 0
//int args[00,07]-->PrimSprite Index?
//String filename
int __fastcall MoviePlay(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

/*
	QuickCopy ( 0, 0x00);
	QuickCopy ( 0, 0x01);
	QuickCopy ( 0, 0x02);
	QuickCopy ( 0, 0x03);
*/
//VarInfo &a, VarInfo &b
int __fastcall QuickCopy(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

int __fastcall QuickState(ScriptObject *stack)
{
	stack->LocalStack[stack->CurStackFrame].Relase();
	stack->CurStackFrame--;

	return 0;
}

