//
// commanddata.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\namelist.h"

#include "case.h"
#include "commanddata.h"

#include "undoMemoryObject.h"

#include "commanddatatype.h"

CNameList* CCommandData::m_functionList = NULL;
CNameList* CCommandData::m_commandList = NULL;

char CCommandData::m_tmpMessage[1024];

char CCommandData::m_commandTypeName[][16]=
{
	"[NOP]          ",
	"[IF]           ",
	"[ELSIF]        ",
	"[ELSE]         ",
	"[CASE]         ",
	"[STORY]        ",
	"[FILM]         ",
	"[BGM-MIDI]     ",
	"[BGM-CD]       ",
	"[SYSTEM]       ",
	"[CALCU]        ",
	"[RET]          ",
	"[NEXT]         ",
	"[END]          ",
	"[SCRIPT]       ",
	"               ",
	"               ",
	" [WHILE]       ",
	" [DEBUG]       ",
	" [JUMP]        ",
	" [SUBSCRIPT]   ",
};

int CCommandData::m_needBufferSize[]=
{
	0,		//NOP
	1024,	//IF
	1024,	//ELSIF
	0,		//ELSE
	256,	//CASE
	64,		//STORY
	64,		//FILM
	64,		//BGM-MIDI
	64,		//BGM-CD
	1024,	//SYSTEM
	4096,	//CALCU
	0,		//RET
	0,		//NEXT
	0,		//END
	64,		//SCRIPT
	0,
	0,
	1024,	//while
	0,
	0,		//ENDIF
	64,		//DEBUG
	64,		//JUMP
	64,		//SUBSCRIPT
};

//0 nothing 1:line 2:line(num) 3:text
int CCommandData::m_textType[]=
{
	0,		//NOP
	1,	//IF
	1,	//ELSIF
	0,		//ELSE
	3,	//SELECT
	1,		//STORY
	1,		//FILM
	1,		//BGM-MIDI
	2,		//BGM-CD
	1,	//SYSTEM
	3,	//CALCU
	0,		//RET
	0,		//NEXT
	0,		//END
	1,		//SCRIPT
	0,
	0,
	1,	//	while
	0,
	0,	//ENDIF
	1,	//DEBUG
	1,	//JUMP
	1,	//SUBSCRIPT
};



CCommandData::CCommandData() : CCase()
{
	m_commandType = COMMANDDATATYPE_NOP;

	m_buffer = NULL;
	m_bufferSize = 0;
	m_bufferSizeMax = 0;


	m_level = 0;
	m_subKosuu = 0;

	m_para = 0;

	m_serial = 0;

	m_color = 0;
	m_backColor = 0;

	m_systemCommandType = 0;
	m_systemCommandNumber = 0;
	m_bgmNumber = 0;

	m_selectMessageSerial = 0;

//	m_messageDataKosuuMax = 0;
//	m_messageDataKosuu = 0;
//	m_messageDataArray = NULL;

}

CCommandData::~CCommandData()
{
	End();
}

void CCommandData::End(void)
{
	DELETEARRAY(m_buffer);
}

void CCommandData::Init(LPVOID para)
{
	int typ = (int)para;

	SetLevel(0);
	SetSubKosuu(0);
	m_systemCommandType = 0;
	m_systemCommandNumber = 0;
	m_bgmNumber = 0;
	m_serial = 0;
	m_color = 0;
	m_backColor = 0;
	m_selectMessageSerial = 0;

	if (typ == -1) typ = COMMANDDATATYPE_NOP;

	if (typ != m_commandType) ChangeCommandType(typ);
}

int CCommandData::GetCommandType(void)
{
	return m_commandType;
}


BOOL CCommandData::ChangeCommandType(int typ,int para)
{
	//必要ならバッファーをとる

	int sz = m_needBufferSize[typ];
	if (sz>m_bufferSizeMax)
	{
		char* buf = new char[sz];
		if (buf == NULL)
		{
//			MessageBox(NULL
		}
		else
		{
			DELETEARRAY(m_buffer);
			m_buffer = buf;
			m_bufferSizeMax = sz;
		}
	}

	m_bufferSize = sz;
	if (sz != 0)
	{
		ZeroMemory(m_buffer,sz);
	}

	m_commandType = typ;
	m_para = para;

	//subKosuuをえる





	return TRUE;
}


BOOL CCommandData::Load(FILE* file,CUndoMemoryObject* memory)
{
	char head[16];
//	fread(head,sizeof(char),16,file);	//skip header
//	fread(head,sizeof(char),16,file);	//skip name
	CaseRead(head,sizeof(char),16,file,memory);	//skip header
	CaseRead(head,sizeof(char),16,file,memory);	//skip name
	int tmp[16];
//	fread(tmp,sizeof(int),16,file);
	CaseRead(tmp,sizeof(int),16,file,memory);

//	m_commandType = tmp[0];
	ChangeCommandType(tmp[0],tmp[3]);
//	m_bufferSize = tmp[1];				//typeから計算
	m_level = tmp[2];
	m_para = tmp[3];
	m_subKosuu = tmp[4];
	m_systemCommandType = tmp[5];
	m_systemCommandNumber = tmp[6];
	m_bgmNumber = tmp[7];
	m_serial = tmp[8];
	
	int col = tmp[9];
	m_color = col & 0xffff;
	m_backColor = (col>>16) & 0x7fff;

	m_selectMessageSerial = tmp[10];

	if (m_bufferSize != 0)
	{
//		fread(m_buffer,sizeof(char),m_bufferSize,file);
		CaseRead(m_buffer,sizeof(char),m_bufferSize,file,memory);
	}

	return TRUE;
}

BOOL CCommandData::Save(FILE* file,CUndoMemoryObject* memory)
{
//	fwrite("-COMMANDDATA   ",sizeof(char),16,file);
//	fwrite(&m_commandTypeName[m_commandType][0],sizeof(char),16,file);
	CaseWrite("-COMMANDDATA   ",sizeof(char),16,file,memory);
	CaseWrite(&m_commandTypeName[m_commandType][0],sizeof(char),16,file,memory);

	int tmp[16];
	for (int i=0;i<16;i++) tmp[i] = 0;
	tmp[0] = m_commandType;
	tmp[1] = m_bufferSize;		//dummy save
	tmp[2] = m_level;
	tmp[3] = m_para;
	tmp[4] = m_subKosuu;
	tmp[5] = m_systemCommandType;
	tmp[6] = m_systemCommandNumber;
	tmp[7] = m_bgmNumber;	//はずす？
	tmp[8] = m_serial;
	
	tmp[9] = m_color | (m_backColor << 16);
	tmp[10] = m_selectMessageSerial;

//	fwrite(tmp,sizeof(int),16,file);
	CaseWrite(tmp,sizeof(int),16,file,memory);

	if (m_bufferSize != 0)
	{
//		fwrite(m_buffer,sizeof(char),m_bufferSize,file);
		CaseWrite(m_buffer,sizeof(char),m_bufferSize,file,memory);
	}

	//これは最下層オブジェクトなのでsaveArray必要なし

	return TRUE;
}



LPSTR CCommandData::GetMyName(void)
{
	return NULL;
}

void CCommandData::SetMyName(LPSTR name)
{
}


CCase* CCommandData::NewObject(void)
{
	return NULL;
}


LPSTR CCommandData::GetTextBuffer(void)
{
	if (m_bufferSize == 0) return NULL;
	return m_buffer;
}

BOOL CCommandData::SetTextBuffer(LPSTR mes)
{
	if (m_buffer == NULL) return FALSE;
	if (m_bufferSize == 0) return FALSE;
	if (mes == NULL)
	{
		m_buffer[0] = 0;
		return FALSE;
	}

	int ln = strlen(mes);
	if (ln>m_bufferSize-2) ln = m_bufferSize-2;
	if (ln<0) ln = 0;
	if (ln>0)
	{
		memcpy(m_buffer,mes,ln);
	}
	m_buffer[ln] = 0;
	m_buffer[ln+1] = 0;

	return TRUE;
}

/*
int CCommandData::GetLevel(void)
{
	return m_level;
}



void CCommandData::SetLevel(int level)
{
	m_level = level;
}
*/

int CCommandData::GetPara(void)
{
	return m_para;
}


int CCommandData::GetSubKosuu(void)
{
	return m_subKosuu;
}

void CCommandData::SetSubKosuu(int subKosuu)
{
	m_subKosuu = subKosuu;
}

void CCommandData::OutputScriptSource(FILE* file)
{
//	fwrite("// COMMANDDATA\x00d\x00a",sizeof(char),16,file);

	int level = GetLevel();
	int tab = level + 2;

	char mes[16384];

	int i;
	int mesKosuu;

	switch (m_commandType)
	{
	case COMMANDDATATYPE_NOP:
		OutputData(file,"nop;",tab);
		break;
	case COMMANDDATATYPE_IF:
		wsprintf(mes,"if(%s)",GetTextBuffer());
		OutputData(file,mes,tab);
		OutputData(file,"{",tab);
		break;
	case COMMANDDATATYPE_ELSIF:
		wsprintf(mes,"elsif(%s)",GetTextBuffer());
		OutputData(file,mes,tab);
		OutputData(file,"{",tab);
		break;
	case COMMANDDATATYPE_ELSE:
		OutputData(file,"else",tab);
		OutputData(file,"{",tab);
		break;
	case COMMANDDATATYPE_SELECT:
		OutputData(file,"select",tab);
		OutputData(file,"(",tab);

		mesKosuu = GetMessageGyo();
		for (i=0;i<mesKosuu;i++)
		{
			if (GetMessage1Gyo(i)>0)
			{
				OutputData(file,m_tmpMessage,tab+1);
			}
		}
		OutputData(file,")",tab);
		OutputData(file,"{",tab);
		break;
	case COMMANDDATATYPE_STORY:
		wsprintf(mes,"Story(\"%s\");",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_FILM:
		wsprintf(mes,"Film(\"%s\");",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
//	case COMMANDDATATYPE_BGM:
//		break;
//	case COMMANDDATATYPE_BGM_CD:
//		break;
	case COMMANDDATATYPE_SYSTEM:
		break;
	case COMMANDDATATYPE_CALCU:
		OutputData(file,"calcu()",tab);
		OutputData(file,"{",tab);
		mesKosuu = GetMessageGyo();
		for (i=0;i<mesKosuu;i++)
		{
			if (GetMessage1Gyo(i)>0)
			{
				OutputData(file,m_tmpMessage,tab+1);
			}
		}
		OutputData(file,"}",tab);
		break;
	case COMMANDDATATYPE_RET:
		OutputData(file,"ret;",tab);
		break;
	case COMMANDDATATYPE_NEXT:
		OutputData(file,"next;",tab);
		break;
	case COMMANDDATATYPE_END:
		OutputData(file,"end;",tab);
		break;
	case COMMANDDATATYPE_SCRIPT:
		wsprintf(mes,"Script(\"%s\");",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_EXIT:
		OutputData(file,"exit;",tab);
		break;
//	case COMMANDDATATYPE_ENDFILM:
//		break;
	case COMMANDDATATYPE_WHILE:
		wsprintf(mes,"while(%s)",GetTextBuffer());
		OutputData(file,"{",tab);
		break;
//	case COMMANDDATATYPE_ENDKOMA:
//		break;
	case COMMANDDATATYPE_ENDIF:
		OutputData(file,"endif;",tab);
		break;
	case COMMANDDATATYPE_DEBUG:
		wsprintf(mes,"Debug(\"%s\");",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_JUMP:
		wsprintf(mes,"ScriptJump(\"%s\");",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_SUBSCRIPT:
		wsprintf(mes,"SubScript(\"%s\");",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_GOTO:
		break;
	}
/*
	if (m_commandType == COMMANDDATATYPE_SYSTEM)
	{
		if (m_systemCommandType == 0)
		{
			char mes[256];

			BOOL flg = FALSE;

			if (m_systemCommandNumber == 2)
			{
				sprintf(mes,"setcg(%s);//セットCG\r\n",m_buffer);
				flg = TRUE;
			}
			if (m_systemCommandNumber == 3)
			{
				sprintf(mes,"setscene(%s);//セットシーン\r\n",m_buffer);
				flg = TRUE;
			}

			if (flg)
			{
				fwrite(mes,sizeof(char),strlen(mes),file);
			}
		}

	}
*/

//#define COMMANDDATATYPE_BGM 7
//#define COMMANDDATATYPE_BGM_CD 8







	
}

void CCommandData::OutputScriptSourceJson(FILE* file)
{
//	fwrite("// COMMANDDATA\x00d\x00a",sizeof(char),16,file);

	int level = GetLevel();
	int tab = level*2 + 4;

	char mes[16384];

	int i;
	int mesKosuu;
	int type;
	int fn;
	LPSTR fc;
	LPSTR fcn;
	LPSTR fcd;

	switch (m_commandType)
	{
	case COMMANDDATATYPE_NOP:
		OutputData(file,"{ \"type\" : \"nop\" },",tab);
		break;
	case COMMANDDATATYPE_IF:
		wsprintf(mes,"{ \"type\" : \"if\" , \"Param\" : \"%s\",",GetTextBuffer());
		OutputData(file,mes,tab);

		OutputData(file,"\"ChildCommand\" : ",tab+1);
		OutputData(file,"[",tab+1);

		break;
	case COMMANDDATATYPE_ELSIF:
		wsprintf(mes,"{ \"type\" : \"elsif\" , \"Param:\" : \"%s\",",GetTextBuffer());
		OutputData(file,mes,tab);

		OutputData(file,"\"ChildCommand\" : ",tab+1);
		OutputData(file,"[",tab+1);
		break;
	case COMMANDDATATYPE_ELSE:
		OutputData(file,"{ \"type\" : \"else\" ,",tab);

		OutputData(file,"\"ChildCommand\" : ",tab+1);
		OutputData(file,"[",tab+1);
		break;
	case COMMANDDATATYPE_SELECT:
		OutputData(file,"{ \"type\" : \"select\" , \"Param:\" :",tab);
		OutputData(file,"{",tab+1);

		mesKosuu = GetMessageGyo();
		wsprintf(mes,"\"number\" : %d ,",mesKosuu);
		OutputData(file,mes,tab+2);


		OutputData(file,"\"messageList\" : ",tab+2);
		OutputData(file,"[",tab+2);

		for (i=0;i<mesKosuu;i++)
		{
			if (GetMessage1Gyo(i)>0)
			{
				if (i<mesKosuu-1)
				{
					wsprintf(mes,"\"%s\",",m_tmpMessage);
				}
				else
				{
					wsprintf(mes,"\"%s\"",m_tmpMessage);
				}
				OutputData(file,mes,tab+3);
			}
		}

		OutputData(file,"]",tab+2);

		OutputData(file,"},",tab+1);



		OutputData(file,"\"ChildCommand\" : ",tab+1);
		OutputData(file,"[",tab+1);

		break;
	case COMMANDDATATYPE_STORY:
		wsprintf(mes,"{ \"type\" : \"Scenario\" , \"Param:\" : \"%s\" },",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_FILM:
		wsprintf(mes,"{ \"type\" : \"Film\" , \"Param:\" : \"%s\" },",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
//	case COMMANDDATATYPE_BGM:
//		break;
//	case COMMANDDATATYPE_BGM_CD:
//		break;
	case COMMANDDATATYPE_SYSTEM:
		type = GetSystemCommandType();
		fc = "systemfunction";
		if (type == 1)
		{
			fc = "systemcommand";
		}
		fn = GetSystemCommandNumber();
		fcn = NULL;
		fcd = NULL;
		if (type == 0)
		{
			fcn = m_functionList->GetName(fn*2);
			fcd = m_functionList->GetName(fn*2+1);
		}
		else
		{
			fcn = m_commandList->GetName(fn*2);
			fcd = m_commandList->GetName(fn*2+1);
		}


		wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : \"%s\" },",fc,fcn);
		OutputData(file,mes,tab);


		break;
		//function,command

	case COMMANDDATATYPE_CALCU:
		OutputData(file,"{ \"type\" : \"calcu\" , \"Param:\" :",tab);

	//	OutputData(file,"calcu()",tab);
		OutputData(file,"[",tab+1);
		mesKosuu = GetMessageGyo();
		for (i=0;i<mesKosuu;i++)
		{
			if (GetMessage1Gyo(i)>0)
			{
				if (i < mesKosuu-1)
				{
					wsprintf(mes,"\"%s\",",m_tmpMessage);
				}
				else
				{
					wsprintf(mes,"\"%s\"",m_tmpMessage);
				}
				OutputData(file,mes,tab+2);
			}
		}

		OutputData(file,"]",tab+1);
		OutputData(file,"},",tab);
		break;
	case COMMANDDATATYPE_RET:
		OutputData(file,"{ \"type\" : \"ret\" },",tab);
		break;
	case COMMANDDATATYPE_NEXT:
		OutputData(file,"{ \"type\" : \"next\" },",tab);
		break;
	case COMMANDDATATYPE_END:
		OutputData(file,"{ \"type\" : \"end\" },",tab);
		break;
	case COMMANDDATATYPE_SCRIPT:
		wsprintf(mes,"{ \"type\" : \"script\" , \"Param:\" : \"%s\" },",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_EXIT:
		OutputData(file,"{ \"type\" : \"exit\" },",tab);
		break;
//	case COMMANDDATATYPE_ENDFILM:
//		break;
	case COMMANDDATATYPE_WHILE:
//		wsprintf(mes,"while(%s)",GetTextBuffer());
		wsprintf(mes,"{ \"type\" : \"while\" , \"Param:\" : \"%s\" ,",GetTextBuffer());
		OutputData(file,mes,tab);

		OutputData(file,"\"ChildCommand\" : ",tab+1);
		OutputData(file,"[",tab+1);


		break;
//	case COMMANDDATATYPE_ENDKOMA:
//		break;
	case COMMANDDATATYPE_ENDIF:
		OutputData(file,"{ \"type\" : \"endif\" },",tab);
		break;
	case COMMANDDATATYPE_DEBUG:
//		wsprintf(mes,"Debug(\"%s\");",GetTextBuffer());
//		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_JUMP:
		wsprintf(mes,"{ \"type\" : \"ScriptJump\" , \"Param:\" : \"%s\" },",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_SUBSCRIPT:
		wsprintf(mes,"{ \"type\" : \"SubScript\" , \"Param:\" : \"%s\" },",GetTextBuffer());
		OutputData(file,mes,tab);
		break;
	case COMMANDDATATYPE_GOTO:
		break;
	}
/*
	if (m_commandType == COMMANDDATATYPE_SYSTEM)
	{
		if (m_systemCommandType == 0)
		{
			char mes[256];

			BOOL flg = FALSE;

			if (m_systemCommandNumber == 2)
			{
				sprintf(mes,"setcg(%s);//セットCG\r\n",m_buffer);
				flg = TRUE;
			}
			if (m_systemCommandNumber == 3)
			{
				sprintf(mes,"setscene(%s);//セットシーン\r\n",m_buffer);
				flg = TRUE;
			}

			if (flg)
			{
				fwrite(mes,sizeof(char),strlen(mes),file);
			}
		}

	}
*/

//#define COMMANDDATATYPE_BGM 7
//#define COMMANDDATATYPE_BGM_CD 8







	
}

int CCommandData::GetTextType(void)
{
	int typ = GetCommandType();
	if (typ == -1) return 0;
	return m_textType[typ];
}





int CCommandData::GetSystemCommandType(void)
{
	return m_systemCommandType;
}

void CCommandData::SetSystemFunction(int funcNumber)
{
	m_systemCommandType = 0;
	m_systemCommandNumber = funcNumber;
}

void CCommandData::SetSystemCommand(int commandNumber)
{
	m_systemCommandType = 1;
	m_systemCommandNumber = commandNumber;
}

int CCommandData::GetSystemCommandNumber(void)
{
	return m_systemCommandNumber;
}


int CCommandData::GetBGMNumber(void)
{
	return m_bgmNumber;
}

void CCommandData::SetBGMNumber(int n)
{
	m_bgmNumber = n;
}


void CCommandData::SetSerial(int n)
{
	m_serial = n;
}

int CCommandData::GetSerial(void)
{
	return m_serial;
}

int CCommandData::GetSelectMessageSerial(void)
{
	return m_selectMessageSerial;
}
void CCommandData::SetSelectMessageSerial(int n)
{
	m_selectMessageSerial = n;
}


BOOL CCommandData::CopyOriginalData(CCase* lpFrom)
{
	return TRUE;
}

int CCommandData::GetMessageGyo(void)
{
	if (m_buffer == NULL) return 0;

	int kosuu = 0;
	int ln = strlen(m_buffer);
	int ptr = 0;

	while (ptr<ln)
	{
		if (*(m_buffer+ptr) == 0x00d)
		{
			kosuu++;
			ptr++;
		}
		ptr++;
	}

	if (*(m_buffer+ln-2) != 0x00d)
	{
		kosuu++;
	}

	return kosuu;
}




int CCommandData::GetMessage1Gyo(int n)
{
	if (m_buffer == NULL) return 0;

	int ln = strlen(m_buffer);
	
	int ptr = 0;

	int found = -1;
	if (n == 0) found = 0;
	int k = 0;

	if (n>0)
	{
		while (ptr<ln)
		{
			if (*(m_buffer+ptr) == 0x00d)
			{
				k++;
				ptr += 2;
				if (n == k)
				{
					found = ptr;
					break;
				}
			}
			else
			{
				ptr++;
			}
		}
	}

	if (found == -1) return 0;

	int ptr2 = found;
	int ln2 = 0;

	while (ptr2<ln)
	{
		if (*(m_buffer+ptr2) == 0x00d)
		{
			memcpy(m_tmpMessage,m_buffer+found,ln2);
			m_tmpMessage[ln2] = 0;
			m_tmpMessage[ln2+1] = 0;
			return ln2;
		}

		ptr2++;
		ln2++;
	}

	memcpy(m_tmpMessage,m_buffer+found,ln2);
	m_tmpMessage[ln2] = 0;
	m_tmpMessage[ln2+1] = 0;


	return ln2;
}

/*_*/

