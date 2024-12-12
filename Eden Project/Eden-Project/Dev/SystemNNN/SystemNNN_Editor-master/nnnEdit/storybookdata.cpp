//
// storybookdata.cpp
//

#include <windows.h>
#include <stdio.h>

#include "case.h"

#include "storybookdata.h"
#include "storydata.h"


CStoryBookData::CStoryBookData() : CCase()
{
	AddArray(64);
}


CStoryBookData::~CStoryBookData()
{
	End();
}


void CStoryBookData::End(void)
{

}


BOOL CStoryBookData::LoadAll(int n,FILE* file,CUndoMemoryObject* memory)
{
	LoadArrayObject(0,file,memory);
	return TRUE;
}


BOOL CStoryBookData::DeleteAll(void)
{
	m_objectKosuu = 0;
	m_nowSelectNumber = 0;
	return TRUE;
}



BOOL CStoryBookData::Load(FILE* file,CUndoMemoryObject* memory)
{
	char head[16];

	//fread(head,sizeof(char),16,file);
	CaseRead(head,sizeof(char),16,file,memory);

	int tmp[16];
//	fread(tmp,sizeof(int),16,file);
	CaseRead(tmp,sizeof(int),16,file,memory);

	LoadArrayObject(tmp[0],file,memory);

	return TRUE;
}



BOOL CStoryBookData::Save(FILE* file,CUndoMemoryObject* memory)
{
//	fwrite("[STORYBOOKDATA]",sizeof(char),16,file);
	CaseWrite("[STORYBOOKDATA]",sizeof(char),16,file,memory);
	
	int tmp[16];
	for (int i=0;i<16;i++) tmp[i] = 0;
	tmp[0] = m_objectKosuu;
//	fwrite(tmp,sizeof(int),16,file);
	CaseWrite(tmp,sizeof(int),16,file,memory);

	SaveArrayObject(file,memory);

	return TRUE;
}



LPSTR CStoryBookData::GetMyName(void)
{
	return NULL;
}


void CStoryBookData::SetMyName(LPSTR name)
{
	//NOTHING TO DO
}


CCase* CStoryBookData::NewObject(void)
{
	CStoryData* pStory = new CStoryData();
	return pStory;
}


int CStoryBookData::SearchStory(LPSTR storyName,int searchStart,int noCheckNumber)
{
	int kosuu = GetObjectKosuu();
	if (kosuu<1) return -1;

	int loop = kosuu / 2 + 1;
	int n1 = (searchStart + kosuu) % kosuu;
	int n2 = (searchStart-1 + kosuu) % kosuu;

	for (int i=0;i<loop;i++)
	{
		CStoryData* pStory;
		if (n1 != noCheckNumber)
		{
			pStory = (CStoryData*)(GetObjectData(n1));
			if (pStory != NULL)
			{
				if (strcmp(storyName,pStory->GetMyName()) == 0) return n1;
			}
		}

		if (n2 != noCheckNumber)
		{
			pStory = (CStoryData*)(GetObjectData(n2));
			if (pStory != NULL)
			{
				if (strcmp(storyName,pStory->GetMyName()) == 0) return n2;
			}
		}

		n1 += 1;
		n1 %= kosuu;
		n2 +=(kosuu*2-1);
		n2 %= kosuu;
	}

	return -1;
}

void CStoryBookData::OutputScriptSource(FILE* file)
{
	OutputData(file,"StoryBookList");

	OutputData(file,"{");

	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSource(file);
	}

	OutputData(file,"}");
}




void CStoryBookData::OutputScriptSourceJson(FILE* file)
{
	OutputData(file,"\"ScenarioBook\":",1);

	OutputData(file,"[",1);

	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSourceJson(file);
		if (i<m_objectKosuu-1)
		{
			OutputData(file,",");
		}
		else
		{
			OutputData(file,"");
		}
	}

	OutputData(file,"],",1);
}

