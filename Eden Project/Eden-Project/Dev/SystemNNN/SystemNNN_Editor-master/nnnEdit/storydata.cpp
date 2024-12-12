//
// storydata.cpp
//


#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "case.h"

#include "commanddata.h"
#include "commanddataType.h"
#include "undoMemoryObject.h"

//#include "komadata.h"
#include "storydata.h"


BOOL CStoryData::m_adjustEnfIf = FALSE;

CStoryData::CStoryData()
{
	ZeroMemory(m_myname,sizeof(m_myname));
	AddArray(32);
}


CStoryData::~CStoryData()
{
	End();
}


void CStoryData::End(void)
{
}


LPSTR CStoryData::GetMyName(void)
{
	return m_myname;
}

void CStoryData::SetMyName(LPSTR name)
{
	if (name == NULL)
	{
		if (m_myname != NULL)
		{
			m_myname[0] = 0;
		}
		return;
	}

	int ln = strlen(name);
	if (ln>62) ln = 62;
	memcpy(m_myname,name,ln);
	m_myname[ln] = 0;
	m_myname[ln+1] = 0;
}



CCommandData* CStoryData::GetCommand(int n)
{
	if ((n<0) || (n>= m_objectKosuu)) return NULL;
	return (CCommandData*)m_objectDataArray[n];
}


BOOL CStoryData::Load(FILE* file,CUndoMemoryObject* memory)
{
	char head[16];
//	fread(head,sizeof(char),16,file);
//	fread(m_myname,sizeof(char),64,file);
	CaseRead(head,sizeof(char),16,file,memory);
	CaseRead(m_myname,sizeof(char),64,file,memory);

	int tmp[16];
//	fread(tmp,sizeof(int),16,file);
	CaseRead(tmp,sizeof(int),16,file,memory);

	m_version = tmp[1];

	LoadArrayObject(tmp[0],file,memory);

	if ((m_version < 1) || (m_adjustEnfIf))
	{
		//endifの追加,elsif,elseではじまるものの修正？//@@@@@@@
		int parent[256];
		int ifflag[256];
		for (int i=0;i<256;i++)
		{
			ifflag[i] = FALSE;
		}

		int n = 0;
		int iflevel = -1;

		int oldLevel = -1;


		while (n<m_objectKosuu)
		{
			CCommandData* pCommand = (CCommandData*)(GetObjectData(n));

			int level = pCommand->GetLevel();
			int kosuu = pCommand->GetSubKosuu();
			int typ = pCommand->GetCommandType();



			BOOL f = FALSE;

			if (iflevel >= 0)
			{
				if (level == iflevel)
				{
					if ((typ != COMMANDDATATYPE_ELSIF) && (typ != COMMANDDATATYPE_ELSE) && (typ != COMMANDDATATYPE_ENDIF))
					{
						f = TRUE;
					}
				}
				else if (level < iflevel)
				{
					f = TRUE;
				}
			}


			if (f)
			{
				while (iflevel >= level)
				{
					if (ifflag[iflevel])
					{
						//insert endif-command and change parent have child kosuu

						CreateObjectData(n);
						CCommandData* pCommandNew = (CCommandData*)(GetObjectData(n));
						pCommandNew->ChangeCommandType(COMMANDDATATYPE_ENDIF);


						if (iflevel > 0)
						{
							CCommandData* pCommandParent = GetParentObject(parent[iflevel]);
							int subKosuu = pCommandParent->GetSubKosuu();
							pCommandParent->SetSubKosuu(subKosuu+1);
						}


						ifflag[iflevel] = FALSE;
					}

					iflevel--;
				}
			}
			else
			{
				if (typ == COMMANDDATATYPE_IF)
				{
					ifflag[level] = TRUE;
					parent[level] = n;
					iflevel = level;
				}
			}

			n++;

			if (n == m_objectKosuu)
			{
				level = 0;

				while (iflevel >= level)
				{
					if (ifflag[iflevel])
					{
						//insert endif-command and change parent have child kosuu

						CreateObjectData(n);
						CCommandData* pCommandNew = (CCommandData*)(GetObjectData(n));
						pCommandNew->ChangeCommandType(COMMANDDATATYPE_ENDIF);


						if (iflevel > 0)
						{
							CCommandData* pCommandParent = GetParentObject(parent[iflevel]);
							int subKosuu = pCommandParent->GetSubKosuu();
							pCommandParent->SetSubKosuu(subKosuu+1);
						}


						ifflag[iflevel] = FALSE;
					}

					iflevel--;
				}


			}


		}



		m_version = 1;
	}

	m_nowSelectNumber = 0;

	return TRUE;
}


BOOL CStoryData::Save(FILE* file,CUndoMemoryObject* memory)
{
//	fwrite("STORYDATA      ",sizeof(char),16,file);
//	fwrite(m_myname,sizeof(char),64,file);
	CaseWrite("STORYDATA      ",sizeof(char),16,file,memory);
	CaseWrite(m_myname,sizeof(char),64,file,memory);

	int tmp[16];
	for (int i=0;i<16;i++) tmp[i] = 0;
	tmp[0] = m_objectKosuu;
	
	tmp[1] = 1;	//version

//	fwrite(tmp,sizeof(int),16,file);
	CaseWrite(tmp,sizeof(int),16,file,memory);

	SaveArrayObject(file,memory);

	return TRUE;
}


void CStoryData::Init(LPVOID para)
{
	m_objectKosuu = 0;
	m_nowSelectNumber = 0;
	m_nowSelectSubNumber = -1;

	LPSTR name = (LPSTR)para;
	SetMyName(name);
}


CCase* CStoryData::NewObject(void)
{
	CCommandData* pCommand = new CCommandData();
	return pCommand;
}


BOOL CStoryData::CheckIamTop(void)
{
	if (strcmp(GetMyName(),"★トップ") == 0) return TRUE;
	return FALSE;
}

/*
void CStoryData::OutputScriptSource(FILE* file)
{
	OutputScriptSourceComment(file,"STORYDATA");
//	fwrite("// STORYDATA:",sizeof(char),13,file);
	LPSTR name = GetMyName();
	if (name != NULL)
	{
		fwrite(name,sizeof(char),strlen(name),file);
	}
	fwrite("\x00d\x00a",sizeof(char),2,file);

	//ラベルをつける



	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSource(file);
	}






	if (CheckIamTop())
	{
		fwrite("\tend\x00d\x00a",sizeof(char),6,file);
	}
	else
	{
		fwrite("\tret\x00d\x00a",sizeof(char),6,file);
	}
}
*/

CCommandData* CStoryData::GetParentObject(int n)
{
	int kosuu = GetObjectKosuu();

	if (n>=kosuu) return NULL;

	CCommandData* pCommandChild = (CCommandData*)(GetObjectData(n));
	int childLevel = pCommandChild->GetLevel();

	for (int i=n-1;i>=0;i--)
	{
		CCommandData* pCommand = (CCommandData*)(GetObjectData(i));
		int level = pCommand->GetLevel();
		if (level == childLevel - 1)
		{
			return pCommand;
		}
	}

	return NULL;
}



BOOL CStoryData::CopyOriginalData(CCase* lpFrom)
{
	LPSTR name = lpFrom->GetMyName();
	SetMyName(name);
	return TRUE;
}


int CStoryData::GetSubTotalKosuu(int n)
{
	CCommandData* pCommand = (CCommandData*)(GetObjectData(n));
	
	int subKosuu = pCommand->GetSubKosuu();
	int level = pCommand->GetLevel();

	int kosuu = 0;
	int calcuedKosuu = 0;
	int totalKosuu = GetObjectKosuu();

	for (int i=n+1;i<totalKosuu;i++)
	{
		CCommandData* pCommand2 = (CCommandData*)(GetObjectData(i));
		
		if (pCommand2->GetLevel() <= level)
		{
			break;
		}

		if (pCommand2->GetLevel() == level+1)
		{
			kosuu += 1;
			kosuu += GetSubTotalKosuu(i);
			calcuedKosuu++;
			if (calcuedKosuu >= subKosuu) break;
		}


	}

	return kosuu;
}

void CStoryData::DeleteChildBlock(int n,int k)
{
	int p = SearchChild(n,k);
	if (p == -1) return;//error

	DeleteWithChild(p);
}


void CStoryData::DeleteWithChild(int n)
{
//char mes[256];
//wsprintf(mes,"\n[DeleteWithChild:%d]",n);
//OutputDebugString(mes);

	CCommandData* pCommand = (CCommandData*)(GetObjectData(n));
	if (pCommand == NULL) return ;	//error

	int level = pCommand->GetLevel();
	int kosuu = pCommand->GetSubKosuu();

	int typ = pCommand->GetCommandType();

	for (int i=kosuu-1;i>=0;i--)
	{
		int p = SearchChild(n,i);
		if (p != -1)
		{
			DeleteWithChild(p);
		}
	}

//wsprintf(mes,"\n[Delete %d]",n);
//OutputDebugString(mes);

	DeleteObjectData(n);

	//delete n




	if (typ == COMMANDDATATYPE_IF)
	{
		//change next or delete endif
		if (GetObjectKosuu() > 0)	//用心
		{
			if (n<GetObjectKosuu())
			{
				CCommandData* pCommand2 = (CCommandData*)(GetObjectData(n));
				if (pCommand2 != NULL)
				{


					int type2 = pCommand2->GetCommandType();

					if (type2 == COMMANDDATATYPE_ELSIF)
					{
//wsprintf(mes,"\n[change elsif to to if %d]",n);
//OutputDebugString(mes);

						pCommand2->ChangeCommandType(COMMANDDATATYPE_IF);
					}

					if (type2 == COMMANDDATATYPE_ELSE)
					{
//wsprintf(mes,"\n[change else to to if %d]",n);
//OutputDebugString(mes);
						pCommand2->ChangeCommandType(COMMANDDATATYPE_IF);
					}

					if (type2 == COMMANDDATATYPE_ENDIF)
					{
//wsprintf(mes,"\n[delete endif %d]",n);
//OutputDebugString(mes);
						DeleteObjectData(n);
					}
				}
			}
		}
	}
}


int CStoryData::SearchChild(int n,int k)
{
	int kosuuMax = GetObjectKosuu();

	CCommandData* pCommand = (CCommandData*)(GetObjectData(n));
	if (pCommand == NULL) return -1;	//error

	int level = pCommand->GetLevel();
	int kosuu = pCommand->GetSubKosuu();

	if ((k<0) || (k>=kosuu)) return -1;

	int foundKosuu = 0;

	for (int i=n+1;i<kosuuMax;i++)
	{
		CCommandData* pCommand2 = (CCommandData*)(GetObjectData(i));
		if (pCommand2 != NULL)
		{
			int level2 = pCommand2->GetLevel();
			if (level+1 == level2)
			{
				//endifは数えない

				if (pCommand2->GetCommandType() != COMMANDDATATYPE_ENDIF)
				{
					if (foundKosuu == k) return i;

					foundKosuu++;
				}
			}

			if (level >= level2) return -1;//not found
		}
	}

	return -1;
}


CCommandData* CStoryData::GetBackSameLevel(int n)
{
	CCommandData* pCommand0 = (CCommandData*)(GetObjectData(n));
	if (pCommand0 == NULL) return NULL;

	int level = pCommand0->GetLevel();


	for (int i=n-1;i>=0;i--)
	{
		CCommandData* pCommand = (CCommandData*)(GetObjectData(i));
		if (pCommand == NULL) return NULL;

		int level2 = pCommand->GetLevel();
		if (level2<level) return NULL;//not found

		if (level2 == level) return pCommand;
	}
	return NULL;
}


CCommandData* CStoryData::GetNextSameLevel(int n)
{
	int kosuu = GetObjectKosuu();

	CCommandData* pCommand0 = (CCommandData*)(GetObjectData(n));
	if (pCommand0 == NULL) return NULL;

	int level = pCommand0->GetLevel();


	for (int i=n+1;i<kosuu;i++)
	{
		CCommandData* pCommand = (CCommandData*)(GetObjectData(i));
		if (pCommand == NULL) return NULL;

		int level2 = pCommand->GetLevel();
		if (level2<level) return NULL;//not found

		if (level2 == level) return pCommand;
	}
	return NULL;
}

void CStoryData::OutputScriptSource(FILE* file)
{
	char mes[1024];
	wsprintf(mes,"StoryData(\"%s\")",GetMyName());
	OutputData(file,mes,1);
	OutputData(file,"{",1);


	int lastLevel = 0;
	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];

		CCommandData* pCommand = (CCommandData*)pCase;

		int level = pCommand->GetLevel();
		if (level < lastLevel)
		{
			for (int t=lastLevel-1;t>=level;t--)
			{
				OutputData(file,"}",t+2);
			}
		}

		lastLevel = level;

//		OutputData(file,"command");
		pCase->OutputScriptSource(file);

		if (i == (m_objectKosuu - 1))
		{
			int type = pCommand->GetCommandType();

			if (CheckIamTop())
			{
				if ((type != COMMANDDATATYPE_END) && (type != COMMANDDATATYPE_EXIT) && (type != COMMANDDATATYPE_NEXT))
				{
					OutputData(file,"end;",2);
				}
			}
			else
			{
				if ((type != COMMANDDATATYPE_RET) && (type != COMMANDDATATYPE_EXIT) && (type != COMMANDDATATYPE_NEXT))
				{
					OutputData(file,"ret;",2);
				}
			}
		}
	}

	OutputData(file,"}",1);
}


void CStoryData::OutputScriptSourceJson(FILE* file)
{
	char mes[1024];

//	OutputData(file,"\"StoryData\":",2);
	OutputData(file,"{",2);


	wsprintf(mes,"\"ScenarioName\":\"%s\",",GetMyName());
	OutputData(file,mes,3);


	OutputData(file,"\"CommandList\":",3);
	OutputData(file,"[",3);


	int lastLevel = 0;
	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];

		CCommandData* pCommand = (CCommandData*)pCase;

		int level = pCommand->GetLevel();
		if (level < lastLevel)
		{
			for (int t=lastLevel-1;t>=level;t--)
			{
				OutputData(file,"{ \"type\" : \"_\" }",t*2+6);
				OutputData(file,"]",t*2+5);
				OutputData(file,"},",t*2+4);
			}
		}

		lastLevel = level;

//		OutputData(file,"command");
		pCase->OutputScriptSourceJson(file);

		if (i == (m_objectKosuu - 1))
		{
			int type = pCommand->GetCommandType();

			if (CheckIamTop())
			{
				if ((type != COMMANDDATATYPE_END) && (type != COMMANDDATATYPE_EXIT) && (type != COMMANDDATATYPE_NEXT))
				{
					OutputData(file,"{ \"type\" : \"end\" },",4);
				}
			}
			else
			{
				if ((type != COMMANDDATATYPE_RET) && (type != COMMANDDATATYPE_EXIT) && (type != COMMANDDATATYPE_NEXT))
				{
					OutputData(file,"{ \"type\" : \"ret\" },",4);
				}
			}

			OutputData(file,"{ \"type\" : \"_\" }",4);
		}
	}

	OutputData(file,"]",3);
	OutputData(file,"}",2,FALSE);
}

/*_*/



