//
// scriptdata.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\scriptcommand.h"
#include "scriptdata.h"


CScriptData::CScriptData()
{
	m_pc = 0;
	m_object = NULL;
	m_objectSizeMax = 0;

	m_messagePointer = 0;
	m_messageData = NULL;
	m_messageSizeMax = 0;

	m_textPointer = 0;
	m_textData = NULL;
	m_textSizeMax = 0;

	m_messageTable = NULL;
	m_textTable = NULL;

	m_objectSizeMax = 65536*4;
	m_object = new int[m_objectSizeMax];

	m_textSizeMax = 65536;
	m_textData = new int[m_textSizeMax];

	m_messageSizeMax = 65536*4;
	m_messageData = new int[m_messageSizeMax];

	m_messageTable = new int[16384];
	m_textTable = new int[16384];

	Init();
}

CScriptData::~CScriptData()
{
	End();
}

void CScriptData::End(void)
{
	DELETEARRAY(m_object);
	m_objectSizeMax = 0;
	DELETEARRAY(m_textData);
	m_textSizeMax = 0;
	DELETEARRAY(m_messageData);
	m_messageSizeMax = 0;
	DELETEARRAY(m_messageTable);
	DELETEARRAY(m_textTable);
}



void CScriptData::Init(void)
{
	m_pc = 0;
	m_messagePointer = 0;
	m_textPointer = 0;

	m_messageNumberMax = 0;
	m_textNumberMax;
}



void CScriptData::SetPC(int pc)
{
	m_pc = pc;
}

int CScriptData::GetPC(void)
{
	return m_pc;
}

int CScriptData::SetScript(int cmd, LPVOID para, int kosuu)
{


	return 0;
}

int CScriptData::SetFunction(int funcNum,int kosuu,int* paraPtr)
{
	if (m_object == NULL) return 0;
	if ((m_pc + 2 + kosuu) > m_objectSizeMax) return 0;

	m_object[m_pc] = 2 + kosuu;
	m_object[m_pc+1] = SCRIPT_FUNCTION | funcNum;
	for (int i=0;i<kosuu;i++)
	{
		m_object[m_pc+2+i] = *(paraPtr+i);
	}


	m_pc += (kosuu + 2);
	return kosuu+1;
}



//print lprint append
int CScriptData::SetPrint(int cmd, int n, LPSTR mes)
{
	int ln = strlen(mes) + 1;
	int k = (ln+3) / 4;

	m_object[m_pc] = 3;
	m_object[m_pc+1] = cmd;
	m_object[m_pc+2] = n;
	m_pc += 3;


	memcpy(m_messageData+m_messagePointer,mes,ln);
	m_messageTable[n] = m_messagePointer;
	m_messagePointer += k;

	if (n>m_messageNumberMax) m_messageNumberMax = n;

	return 3;
}



int CScriptData::SetTextSub(LPSTR mes)
{
	int ln = strlen(mes) + 1;
	int k = (ln+3) / 4;

	memcpy(m_textData+m_textPointer,mes,ln);
	m_textTable[m_textNumberMax] = m_textPointer;
	m_textPointer += k;

	m_textNumberMax++;

	return m_textNumberMax;
}
