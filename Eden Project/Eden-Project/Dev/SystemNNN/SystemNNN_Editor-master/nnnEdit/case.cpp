//
// case.cpp			ケースと中身という形式のデータの基本クラス
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "case.h"
#include "undoMemoryObject.h"

//#include "myApplication.h"


#include "scriptdata.h"

CCase::CCase()
{
//	m_app = app;
	m_nowSelectNumber = 0;
	m_nowSelectSubNumber = -1;
	m_objectKosuu = 0;
	m_objectKosuuMax = 0;
	m_objectDataArray = NULL;
	m_minAddKosuu = 16;
	m_version = 0;

	m_assertCheck = 0xabcdef01;
}


CCase::~CCase()
{
	End();
}


void CCase::End(void)
{
	if (m_assertCheck != 0xabcdef01)
	{
OutputDebugString("Damage!");
	}

	if (m_objectDataArray != NULL)
	{
		for (int i=0;i<m_objectKosuuMax;i++)
		{
			ENDDELETECLASS(m_objectDataArray[i]);
		}
	}
	DELETEARRAY(m_objectDataArray);
}




int CCase::GetNowSelectNumber(void)
{
	return m_nowSelectNumber;
}

int CCase::GetNowSelectSubNumber(void)
{
	return m_nowSelectSubNumber;
}



CCase* CCase::GetNowSelectObject(void)
{
	return GetObjectData(m_nowSelectNumber);
}


CCase* CCase::GetObjectData(int n)
{
	if (n == -1) n = m_nowSelectNumber;
	if ((n<0) || (n>=m_objectKosuu)) return NULL;

	return m_objectDataArray[n];
}

BOOL CCase::AddArray(int n, int place)
{
	if (n < 1) n = 1;
	if (place == -1) place = m_objectKosuu;
	if ((place<0) || (place>m_objectKosuu)) place = m_objectKosuu;

	CCase** ppCase = new CCase* [m_objectKosuuMax + n];
	//copy

	int k = 0;
	int i;
	for (i=0;i<place;i++)
	{
		*(ppCase+i) = m_objectDataArray[k];
		k++;
	}

	for (i=place;i<place+n;i++)
	{
		*(ppCase+i) = NULL;
	}

	for (i=place+n;i<m_objectKosuuMax+n;i++)
	{
		*(ppCase+i) = m_objectDataArray[k];
		k++;
	}

	delete [] m_objectDataArray;
	m_objectDataArray = ppCase;
	m_objectKosuuMax += n;


	return TRUE;
}


BOOL CCase::CreateObjectData(int place,int kosuu)
{
	if (kosuu < 1) kosuu = 1;
	if (place == -1) place = m_objectKosuu;
	if ((place<0) || (place>m_objectKosuu)) place = m_objectKosuu;

	//ひつようならarrayをかくちょう
	if ((m_objectKosuu+kosuu) > m_objectKosuuMax)
	{
		int k = m_objectKosuu + kosuu - m_objectKosuuMax;
		if (k<m_minAddKosuu) k = m_minAddKosuu;
		AddArray(k);
	}

	CCase** ppBuf = new CCase*[kosuu];

	int i;
	for (i=0;i<kosuu;i++)
	{
		ppBuf[i] = m_objectDataArray[m_objectKosuuMax-kosuu+i];
	}
	//ぽいんたのいどう
	for (i=m_objectKosuuMax-kosuu-1;i>=place;i--)
	{
		m_objectDataArray[i+kosuu] = m_objectDataArray[i];
	}
	for (i=0;i<kosuu;i++)
	{
		m_objectDataArray[place+i] = ppBuf[i];
	}

	delete [] ppBuf;

	//必要ならさくせい
	for (i=place;i<place+kosuu;i++)
	{
		CCase* pObj = m_objectDataArray[i];
		if (pObj == NULL)
		{
			pObj = NewObject();
			m_objectDataArray[i] = pObj;
		}
		pObj->Init();
	}

	m_objectKosuu += kosuu;
	m_nowSelectNumber = place;
	return TRUE;
}


BOOL CCase::DeleteObjectData(int place, int kosuu)
{
	if (kosuu<1) kosuu = 1;
	if ((place < 0) || (place>=m_objectKosuu)) return FALSE;
	if ((place + kosuu) > m_objectKosuu) kosuu = m_objectKosuu - place;


	//ずらす
	CCase** ppBuf = new CCase*[kosuu];
	int i;
	for (i=0;i<kosuu;i++)
	{
		ppBuf[i] = m_objectDataArray[place+i];
	}
	for (i=place;i<m_objectKosuuMax-kosuu;i++)
	{
		m_objectDataArray[i] = m_objectDataArray[i+kosuu];
	}
	for (i=0;i<kosuu;i++)
	{
		m_objectDataArray[m_objectKosuuMax-kosuu+i] = ppBuf[i];
	}
	delete [] ppBuf;

	m_objectKosuu -= kosuu;
	if (m_nowSelectNumber>=place)
	{
		if (m_nowSelectNumber<place+kosuu)
		{
			m_nowSelectNumber = place;
		}
		else
		{
			m_nowSelectNumber -= kosuu;
		}
	}

	if (m_nowSelectNumber<0) m_nowSelectNumber = 0;
	if (m_nowSelectNumber>m_objectKosuu) m_nowSelectNumber = m_objectKosuu;

	return TRUE;
}





void CCase::Init(LPVOID para)
{
}

BOOL CCase::CaseRead(LPVOID ptr,size_t elementSize,size_t count,FILE* file,CUndoMemoryObject* memory)
{
	if (memory ==NULL)
	{
		fread(ptr,elementSize,count,file);

		return TRUE;
	}
	else
	{
		memory->Read(ptr,elementSize,count);
		return TRUE;
	}
}

BOOL CCase::CaseWrite(LPVOID ptr,size_t elementSize,size_t count,FILE* file,CUndoMemoryObject* memory)
{
	if (memory ==NULL)
	{
		fwrite(ptr,elementSize,count,file);

		return TRUE;
	}
	else
	{
		memory->Write(ptr,elementSize,count);
		return TRUE;
	}
}



BOOL CCase::LoadArrayObject(int n, FILE* file,CUndoMemoryObject* memory)
{
	//n個なければarray作成
	if (n>m_objectKosuuMax)
	{
		AddArray(n-m_objectKosuuMax);
	}


	for (int i=0;i<n;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		if (pCase == NULL)
		{
			pCase = NewObject();
			m_objectDataArray[i] = pCase;
		}

		pCase->Load(file,memory);
	}

	m_objectKosuu = n;
	if (m_nowSelectNumber<0) m_nowSelectNumber = 0;
	if (m_nowSelectNumber>m_objectKosuu) m_nowSelectNumber = m_objectKosuu;

	return TRUE;
}




BOOL CCase::SaveArrayObject(FILE* file,CUndoMemoryObject* memory)
{
	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* lpObj = m_objectDataArray[i];
		if (lpObj == NULL)
		{
//			MessageBox(NULL,"no CStoryData in CStoryBookDoc::Save()","致命的ERROR",MB_OK);
		}
		else
		{
			lpObj->Save(file,memory);
		}
	}

	return TRUE;
}


void CCase::SetSelectNumber(int n)
{
	if (n<-1) n = -1;
	if (n>m_objectKosuu) n = m_objectKosuu;

	m_nowSelectNumber = n;
	m_nowSelectSubNumber = -1;
}


void CCase::SetSelectSubNumber(int n)
{
	if (n == -1)
	{
		m_nowSelectSubNumber = n;
	}

	if (n<-1) return;
	if (n>=m_objectKosuu) return;

	if (m_nowSelectNumber == -1) SetSelectNumber(n);
	m_nowSelectSubNumber = n;
}



int CCase::Compile(CScriptData* lpScript)
{
	//default 動作
	int kosuu = GetObjectKosuu();
	for (int i=0;i<kosuu;i++)
	{
		CCase* pCase = GetObjectData(i);
		if (pCase != NULL)
		{
			if (pCase->Compile(lpScript) == -1)
			{
				return -1;
			}
		}
	}
	return lpScript->GetPC();
}

void CCase::OutputScriptSourceJson(FILE* file)
{

	
	fwrite("// DEFAULT\x00d\x00a",sizeof(char),12,file);


	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSourceJson(file);
	}
}


void CCase::OutputScriptSource(FILE* file)
{
	fwrite("// DEFAULT\x00d\x00a",sizeof(char),12,file);
	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSource(file);
	}
}


void CCase::OutputScriptSourceComment(FILE* file,LPSTR mes)
{
	char tmp[256];
	if (mes == NULL)
	{
		wsprintf(tmp,"//\x00d\x00a");
	}
	else
	{
		int ln = strlen(mes);
		if (ln>250) ln = 250;
		tmp[0]='/';
		tmp[1]='/';
		tmp[2]=' ';
		
		if (ln>0)
		{
			memcpy(tmp+3,mes,ln);
		}
		tmp[3+ln] = 0x00d;
		tmp[3+ln+1] = 0x00a;
		tmp[3+ln+2] = 0;

	}

	int ln2 = strlen(tmp);
	fwrite(tmp,sizeof(char),ln2,file);
}


int CCase::Pass1(void)
{
	//サイズ計算のみ

	int kosuu = GetObjectKosuu();
	int sz = 0;

	for (int i=0;i<kosuu;i++)
	{
		CCase* pCase = GetObjectData(i);
		if (pCase != NULL)
		{
			int ln = pCase->Pass1();

			//error check




			sz += ln;
		}
	}

	return sz;
}


BOOL CCase::CheckExistChara(int chataNumner)
{
	return FALSE;
}


int CCase::GetDataVersion(void)
{
	return m_version;
}

void CCase::SetDataVersion(int version)
{
	m_version = version;
}


BOOL CCase::CopyData(CCase* lpFrom)
{
	int fromObjectKosuu = lpFrom->GetObjectKosuu();
	int toObjectKosuuMax = GetObjectKosuuMax();
	
	if (fromObjectKosuu > toObjectKosuuMax)
	{
		AddArray(fromObjectKosuu - toObjectKosuuMax);
	}

	CopyOriginalData(lpFrom);

	//object
	for (int i=0;i<fromObjectKosuu;i++)
	{
		CCase* fromObject = lpFrom->GetObjectData(i);
		CCase* toObject = GetObjectData(i);
		toObject->CopyData(fromObject);
	}

	m_objectKosuu = fromObjectKosuu;

	return TRUE;
}


int CCase::GetSelectStart(void)
{
	if (m_nowSelectSubNumber == -1) return m_nowSelectNumber;

	int st = m_nowSelectNumber;
	if (st>m_nowSelectSubNumber) st = m_nowSelectSubNumber;

	return st;
}

int CCase::GetSelectEnd(void)
{
	if (m_nowSelectSubNumber == -1) return m_nowSelectNumber;

	int ed = m_nowSelectSubNumber;
	if (ed<m_nowSelectNumber) ed = m_nowSelectNumber;

	return ed;

}


BOOL CCase::CheckInSubArea(int n)
{
	if (n<0) return FALSE;
	if (n>=GetObjectKosuu()) return FALSE;

	if (n == GetNowSelectNumber()) return FALSE;

	int st = GetSelectStart();
	int ed = GetSelectEnd();

	if ((n>=st) && (n<=ed)) return TRUE;

	return FALSE;
}

void CCase::OutputData(FILE* file,LPSTR mes,int tab,BOOL crFlag)
{
	if (tab > 0)
	{
		for (int i=0;i<tab;i++)
		{
			fwrite("\t",sizeof(char),1,file);
		}
	}

	int ln = strlen(mes);
	fwrite(mes,sizeof(char),ln,file);

	if (crFlag)
	{
		fwrite("\x00d\x00a",sizeof(char),2,file);
	}
}

/*
CCase* CCase::CreateSubObject(void)
{
	return new CCase();	//dummy
}
*/

/*_*/




