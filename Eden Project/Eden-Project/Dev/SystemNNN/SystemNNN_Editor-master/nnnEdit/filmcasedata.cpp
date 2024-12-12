//
// filmcasedata.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "case.h"

#include "undoMemoryObject.h"

#include "filmdata.h"
#include "filmcasedata.h"
#include "scriptdata.h"

//char CFilmCaseData::m_myname[64] = "dummy";

//CFilmCaseData::CFilmCaseData(CEffect* lpEffect,CMyApplication* app) : CCase(app)
CFilmCaseData::CFilmCaseData(CEffect* lpEffect) : CCase()
{
	m_effect = lpEffect;
	
	m_labelTable = NULL;
	m_labelTableKosuu = 0;
	
//	m_filmKosuu = 0;
//	m_filmKosuuMax = 0;
//	m_filmDataArray = NULL;
//	m_nowSelectNumber = 0;

	AddArray(64);
}


CFilmCaseData::~CFilmCaseData()
{
	End();
}



void CFilmCaseData::End(void)
{
//	if (m_filmDataArray != NULL)
//	{
//		for (int i=0;i<m_filmKosuuMax;i++)
//		{
//			ENDDELETECLASS(m_filmDataArray[i]);
//		}
//	}
//
//	DELETEARRAY(m_filmDataArray);
	DELETEARRAY(m_labelTable);
	m_labelTableKosuu = 0;
}

/*
int CFilmCaseData::GetFilmKosuu(void)
{
	return m_filmKosuu;
}


CFilmData* CFilmCaseData::GetNowSelectFilm(void)
{
	return GetFilm(m_nowSelectNumber);
}



CFilmData* CFilmCaseData::GetFilm(int n)
{
	if (n == -1) n = m_nowSelectNumber;
	if ((n<0) || (n>=m_filmKosuu)) return NULL;

	return m_filmDataArray[n];
}

int CFilmCaseData::GetNowSelectNumber(void)
{
	return m_nowSelectNumber;
}

*/


BOOL CFilmCaseData::LoadAll(int n,FILE* file,CUndoMemoryObject* memory)
{
	LoadArrayObject(n,file,memory);
	return TRUE;
}

BOOL CFilmCaseData::ClearAllFilm(void)
{
	m_objectKosuu = 0;
	m_nowSelectNumber = 0;
	return TRUE;
}

BOOL CFilmCaseData::Load(FILE* file,CUndoMemoryObject* memory)
{
	char tmpc[16];
//	fread(tmpc,sizeof(char),16,file);
	CaseRead(tmpc,sizeof(char),16,file,memory);

	int tmp[16];
//	fread(tmp,sizeof(int),16,file);
	CaseRead(tmp,sizeof(int),16,file,memory);

	LoadArrayObject(tmp[0],file,memory);
	return TRUE;
}


BOOL CFilmCaseData::Save(FILE* file,CUndoMemoryObject* memory)
{
//	fwrite("[FILMCASEDATA] ",sizeof(char),16,file);
	CaseWrite("[FILMCASEDATA] ",sizeof(char),16,file,memory);

	int tmp[16];
	for (int i=0;i<16;i++) tmp[i] = 0;
	tmp[0] = m_objectKosuu;
//	fwrite(tmp,sizeof(int),16,file);
	CaseWrite(tmp,sizeof(int),16,file,memory);

	SaveArrayObject(file,memory);
	return TRUE;
}



LPSTR CFilmCaseData::GetMyName(void)
{
	return NULL;
}

void CFilmCaseData::SetMyName(LPSTR name)
{
	//nothing to do
}


CCase* CFilmCaseData::NewObject(void)
{
//	CFilmData* pFilm = new CFilmData(m_effect,m_app);
	CFilmData* pFilm = new CFilmData(m_effect);
	return pFilm;
}
/*
int CFilmCaseData::Compile(CScriptData* lpScript)
{
	int kosuu = GetObjectKosuu();
	for (int i=0;i<kosuu;i++)
	{
		CFilmData* pFilm = (CFilmData*)(GetObjectData(i));
		if (pFilm != NULL)
		{
			pFilm->Compile(lpScript);
		}
	}

	return lpScript->GetPC();
}
*/


void CFilmCaseData::OutputScriptSource(FILE* file)
{
	OutputScriptSourceComment(file,"フィルムデータ");
	OutputData(file,"FilmCase");
	OutputData(file,"{");
	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSource(file);
	}
	OutputData(file,"}");
	//OutputScriptComment("");
	//OutputScriptComment("FILMCASE");
	//OutputScriptComment("");


	//fwrite("// DEFAULT\x00d\x00a",sizeof(char),12,file);

	//
}



void CFilmCaseData::OutputScriptSourceJson(FILE* file)
{
	OutputData(file,"\"FilmCase\":",1);

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

	OutputData(file,"]",1);
}


int CFilmCaseData::Pass1(void)
{
	int sz = 0;

	int kosuu = GetObjectKosuu();
	if (kosuu>m_labelTableKosuu)
	{
		DELETEARRAY(m_labelTable);
		m_labelTable = new int[kosuu];
		m_labelTableKosuu = kosuu;
	}

	for (int i=0;i<kosuu;i++)
	{
		CCase* pCase = GetObjectData(i);
		if (pCase != NULL)
		{
			m_labelTable[i] = sz;

			int ln = pCase->Pass1();

			//error check




			sz += ln;
		}
	}


	return sz;
}

BOOL CFilmCaseData::CopyOriginalData(CCase* lpCase)
{
	return TRUE;
}

int CFilmCaseData::Clean(void)
{
	return 0;
}

int CFilmCaseData::SearchFilm(LPSTR filmName,int searchStart,int noCheckNumber)
{
	int kosuu = GetObjectKosuu();
	if (kosuu<1) return -1;

	int loop = kosuu / 2 + 1;
	int n1 = (searchStart + kosuu) % kosuu;
	int n2 = (searchStart-1 + kosuu*2) % kosuu;

	for (int i=0;i<loop;i++)
	{
		CFilmData* pFilm;
		if (n1 != noCheckNumber)
		{
			pFilm = (CFilmData*)(GetObjectData(n1));
			if (pFilm != NULL)
			{
				if (strcmp(filmName,pFilm->GetMyName()) == 0) return n1;
			}
		}

		if (n2 != noCheckNumber)
		{
			pFilm = (CFilmData*)(GetObjectData(n2));
			if (pFilm != NULL)
			{
				if (strcmp(filmName,pFilm->GetMyName()) == 0) return n2;
			}
		}

		n1 += 1;
		n1 %= kosuu;
		n2 +=(kosuu*2-1);
		n2 %= kosuu;
	}

	return -1;
}

void CFilmCaseData::ExchangeData(int n1,int n2)
{
	int kosuu = GetObjectKosuu();
	if ((n1<0) || (n1>=kosuu)) return;
	if ((n2<0) || (n2>=kosuu)) return;

	CCase* case1 = m_objectDataArray[n1];
	CCase* case2 = m_objectDataArray[n2];

	m_objectDataArray[n1] = case2;
	m_objectDataArray[n2] = case1;

}

void CFilmCaseData::MoveAndInsertData(int from,int to)
{
	int kosuu = GetObjectKosuu();
	if ((from<0) || (from>=kosuu)) return;
	if ((to<0) || (to>kosuu)) return;

	if (from == to) return;

	if (from < to)
	{
		CCase* fromCase = m_objectDataArray[from];
		for (int i=0;i<to-from;i++)
		{
			if (i+from+1<kosuu)
			{
				m_objectDataArray[from+i] = m_objectDataArray[from+i+1];
			}
		}

		if (to < kosuu)
		{
			m_objectDataArray[to] = fromCase;
		}
		else
		{
			m_objectDataArray[to-1] = fromCase;
		}

		return;
	}

	CCase* fromCase = m_objectDataArray[from];
	for (int i=0;i<from-to;i++)
	{
		m_objectDataArray[from-i] = m_objectDataArray[from-i-1];
	}
	m_objectDataArray[to] = fromCase;
}


/*_*/

