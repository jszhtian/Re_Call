//
// filmdata.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"



#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nyanLib\include\effect.h"
//#include "..\cyclib\effectList.h"
//#include "..\cycGameLib\scriptDefine.h"

#include "..\..\systemNNN\nnnUtilLib\scriptcommand.h"





#include "case.h"

#include "komadata.h"
#include "filmdata.h"

#include "undoMemoryObject.h"


#include "case.h"
#include "scriptdata.h"

char CFilmData::m_fileHeaderName[16] = "FILMDATA       ";
int CFilmData::m_komaOfFilm = 1;
int CFilmData::m_komaOfFilmType = 0;

//CFilmData::CFilmData(CEffect* lpEffect,CMyApplication* app) : CCase(app)
CFilmData::CFilmData(CEffect* lpEffect) : CCase()
{
	m_effect = lpEffect;
	m_miniPic = new int[32*24];
	m_miniPicFlag = 0;
	if (m_miniPic != NULL) ZeroMemory(m_miniPic,32*24*sizeof(int));

	ZeroMemory(m_myname,sizeof(m_myname));
	m_clearEffectFlag = TRUE;

	m_filmColor = 0;
	m_filmSpecialTypeTime = 0;	//plus autofilm minusââèoÉtÉBÉãÉÄ
	m_configMask = 0;	//bit pattern
	m_taikenLevel = 0;
	m_cutinFlag = 0;
	m_skiptofilmend = 0;
	m_renameLayer = 0;

//	SetModifyFlag(FALSE);

	AddArray(16);
}


CFilmData::~CFilmData()
{
	End();
}

void CFilmData::End(void)
{
	DELETEARRAY(m_miniPic);
}


LPSTR CFilmData::GetMyName(void)
{
	return m_myname;
}

void CFilmData::SetMyName(LPSTR name)
{
//	SetModifyFlag();

	if (name == NULL)
	{
		m_myname[0] = 0;
		return;
	}

	int ln = strlen(name);
	if (ln>62) ln = 62;
	memcpy(m_myname,name,ln);
	m_myname[ln] = 0;
	m_myname[ln+1] = 0;
}


void CFilmData::Make64x32KomaPic(int n, int* buffer)
{
	CKomaData* koma = GetKoma(n);
	if (koma == NULL)
	{
		//number??
		return;
	}

	koma->Make64x48Pic(buffer);
}


void CFilmData::MakeMiniPic(void)
{
	CKomaData* koma = GetKoma(1);
	if (koma == NULL)
	{
		koma = GetKoma(0);
		if (koma == NULL)
		{
			//number
			return;
		}
	}


//	if ((koma->CheckModify()) || CheckModify())
	if (koma->CheckMiniPic() == FALSE)
	{
		koma->Make32x24Pic(m_miniPic);
		m_miniPicFlag = 1;
	}
}


void CFilmData::SetMiniPic(BOOL flg)
{
	CKomaData* koma = GetKoma(0);
	if (koma != NULL)
	{
		koma->SetMiniPic(flg,m_miniPic);
	}
}

void CFilmData::SetLargePic(BOOL flg)
{
	CKomaData* koma = GetKoma(0);
	if (koma != NULL)
	{
		koma->SetLargePic(flg);
	}
}

void CFilmData::SetVeryLargePic(BOOL flg)
{
	CKomaData* koma = GetKoma(0);
	if (koma != NULL)
	{
		koma->SetVeryLargePic(flg);
	}
}



CKomaData* CFilmData::GetKoma(int n)
{
	return (CKomaData*)GetObjectData(n);
}

int* CFilmData::GetMiniPic(void)
{
	return m_miniPic;
}



BOOL CFilmData::Load(FILE* file,CUndoMemoryObject* memory)
{
//	int hhh = 0;
//	hhh++;

	char head[16];
//	fread(head,sizeof(char),16,file);
//	fread(m_myname,sizeof(char),64,file);
	CaseRead(head,sizeof(char),16,file,memory);
	CaseRead(m_myname,sizeof(char),64,file,memory);

	int tmp[16];
//	fread(tmp,sizeof(int),16,file);
	CaseRead(tmp,sizeof(int),16,file,memory);
	m_clearEffectFlag = tmp[1];
	m_miniPicFlag = tmp[2];
	m_filmColor = tmp[3];
	m_filmSpecialTypeTime = tmp[4];
	m_configMask = tmp[5];
	m_taikenLevel = tmp[6];
	m_cutinFlag = tmp[7];
	m_skiptofilmend = tmp[8];
	m_renameLayer = tmp[9];

	if (m_miniPicFlag)
	{
//		fread(m_miniPic,sizeof(int),32*24,file);
		CaseRead(m_miniPic,sizeof(int),32*24,file,memory);
	}

//	SetModifyFlag(FALSE);
	LoadArrayObject(tmp[0],file,memory);
	if (m_miniPicFlag)
	{
		SetMiniPic();
	}

	return TRUE;
}




BOOL CFilmData::Save(FILE* file,CUndoMemoryObject* memory)
{
//	fwrite(m_fileHeaderName,sizeof(char),16,file);
//	fwrite(m_myname,sizeof(char),64,file);
	CaseWrite(m_fileHeaderName,sizeof(char),16,file,memory);
	CaseWrite(m_myname,sizeof(char),64,file,memory);

	int tmp[16];
	for (int i=0;i<16;i++) tmp[i] = 0;
	tmp[0] = m_objectKosuu;
	tmp[1] = m_clearEffectFlag;
	tmp[2] = 1;
	tmp[3] = m_filmColor;
	tmp[4] = m_filmSpecialTypeTime;
	tmp[5] = m_configMask;
	tmp[6] = m_taikenLevel;
	tmp[7] = m_cutinFlag;
	tmp[8] = m_skiptofilmend;
	tmp[9] = m_renameLayer;

	CKomaData* koma = GetKoma(0);
	if (koma != NULL)
	{
		if (koma->CheckMiniPic())
		{
			koma->Make32x24Pic(m_miniPic);
		}
	}
	m_miniPicFlag = 1;

//	fwrite(tmp,sizeof(int),16,file);
	CaseWrite(tmp,sizeof(int),16,file,memory);

//	fwrite(m_miniPic,sizeof(int),32*24,file);
	CaseWrite(m_miniPic,sizeof(int),32*24,file,memory);

	SaveArrayObject(file,memory);

//	SetModifyFlag(FALSE);

	return TRUE;
}


void CFilmData::Init(LPSTR name)
{
	m_objectKosuu = 0;
	m_clearEffectFlag = TRUE;
	m_filmColor = 0;
	m_filmSpecialTypeTime = 0;
	m_configMask = 0;
	m_taikenLevel = 0;
	m_cutinFlag = 0;
	m_renameLayer = 0;

	//clear buffer
	if (m_miniPic != NULL) ZeroMemory(m_miniPic,32*24*sizeof(int));

	SetMyName(name);
}

CCase* CFilmData::NewObject(void)
{
	CKomaData* pKoma = new CKomaData(m_effect);
//	SetModifyFlag();
	return pKoma;
}




int CFilmData::Compile(CScriptData* lpScript)
{
	//cleareffect
	if (m_clearEffectFlag)
	{
		lpScript->SetFunction(FUNCTION_CLEAREFFECT);
	}

	//koma
	//default ìÆçÏ
	int kosuu = GetObjectKosuu();
	for (int i=0;i<kosuu;i++)
	{
		CCase* pCase = GetObjectData(i);
		if (pCase != NULL)
		{
			pCase->Compile(lpScript);
		}
	}


	return lpScript->GetPC();
}


void CFilmData::OutputScriptSource(FILE* file)
{
//	fwrite("\x00d\x00a",sizeof(char),2,file);
//	fwrite("// FILMDATA:",sizeof(char),12,file);
//	LPSTR name = GetMyName();
//	fwrite(name,sizeof(char),strlen(name),file);
//	fwrite("\x00d\x00a",sizeof(char),2,file);

	char mes[1024];
	wsprintf(mes,"FilmData(\"%s\")",GetMyName());
	OutputData(file,mes,1);
	OutputData(file,"{",1);


	int specialTypeTime = GetFilmSpecialTypeTime();
	int noClear = 0;
	if (m_clearEffectFlag == FALSE) noClear = 1;

	int configMask = GetConfigMask();
	int taikenLevel = GetTaikenLevel();

	if ((specialTypeTime != 0) || (noClear != 0) || (configMask != 0) || (taikenLevel != 0))
	{
		OutputData(file,"filmParam()",2);
		OutputData(file,"{",2);

		if (noClear != 0)
		{
			wsprintf(mes,"noClearEffect=%d",noClear);
			OutputData(file,mes,3);
		}

		if (specialTypeTime != 0)
		{
			wsprintf(mes,"specialTypeTime=%d",specialTypeTime);
			OutputData(file,mes,3);
		}

		if (configMask != 0)
		{
			wsprintf(mes,"configMask=%d",configMask);
			OutputData(file,mes,3);
		}

		if (taikenLevel != 0)
		{
			wsprintf(mes,"taikenLevel=%d",taikenLevel);
			OutputData(file,mes,3);
		}

		OutputData(file,"}",2);
	}




	for (int i=0;i<m_objectKosuu;i++)
	{
		CCase* pCase = m_objectDataArray[i];
		pCase->OutputScriptSource(file);
	}
	OutputData(file,"}",1);
}

BOOL CFilmData::GetNoClearEffect(void)
{
	return m_clearEffectFlag;
}

void CFilmData::SetNoClearEffect(BOOL flg)
{
	m_clearEffectFlag = flg;
}

BOOL CFilmData::CheckExistChara(int charaNumber)
{
	if (charaNumber == 4)
	{
		int hhh = 0;
		hhh++;
	}

	for (int i=0;i<m_objectKosuu;i++)
	{
		CKomaData* pKoma = (CKomaData*)GetObjectData(i);
		if (pKoma != NULL)
		{
			if (pKoma->CheckExistChara(charaNumber)) return TRUE;
		}
	}
	return FALSE;
}

int CFilmData::Pass1(void)
{
	int sz = 4;	//filmlabel

	if (m_clearEffectFlag) sz += 2;	//cleareffect

	int kosuu = GetObjectKosuu();

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


	sz += 2;		//ret or end

	return sz;
}


BOOL CFilmData::CopyOriginalData(CCase* lpCase)
{
	return TRUE;
}

int CFilmData::Clean(void)
{
	return 0;
}


int CFilmData::GetSMKomaNumber(void)
{
	int kosuu = GetObjectKosuu();
	if (kosuu<1) return 0;	//error!??
	if (kosuu == 1) return 0;

	int n = m_komaOfFilm;
	int type = m_komaOfFilmType;

	if (type==0)
	{
		if (n>=kosuu) n = kosuu-1;
		if (n<0) n = 0;
		return n;
	}

	n *= (kosuu-1);
	n /= 100;

	if (n>=kosuu) n = kosuu-1;
	if (n<0) n = 0;

	return n;
}


BOOL CFilmData::CheckNowCutin(int komaNumber)
{
	int filmCutin = GetCutinFlag();
	if ((komaNumber < 0) || (komaNumber >= m_objectKosuu))
	{
		return FALSE;
	}

	for (int koma = komaNumber;koma>=0;koma--)
	{
		CKomaData* pKoma = (CKomaData*)(GetObjectData(koma));
		if (pKoma != NULL)
		{
			int komaCutin = pKoma->GetCutinFlag();
			if (komaCutin == 1)
			{
				return TRUE;
			}

			if (komaCutin == -1)
			{
				return FALSE;
			}
		}
	}

	return filmCutin;
}

void CFilmData::GetNowPicList(LPSTR* picFileName,int komaNumber)
{
	for (int i=0;i<16;i++)
	{
		picFileName[i] = NULL;
	}

	if ((komaNumber < 0) || (komaNumber >= m_objectKosuu))
	{
		return;
	}

	for (int i=0;i<16;i++)
	{
		CKomaData* pKoma = (CKomaData*)(GetObjectData(komaNumber));
		if (pKoma != NULL)
		{
			if (pKoma->GetEffectFlag(i))
			{
				int effect = pKoma->GetEffect(i);
				if (effect != -1)
				{
					for (int koma = komaNumber;koma >= 0; koma--)
					{
						CKomaData* pKoma2 = (CKomaData*)(GetObjectData(koma));
						if (pKoma2 != NULL)
						{
							if (pKoma2->GetEffectFlag(i))
							{
								int effect2 = pKoma->GetEffect(i);
								if (effect2 != -1)
								{
									LPSTR name = pKoma->GetPicFileName(i);
									if (name != NULL)
									{
										if ((*name) != 0)
										{
											picFileName[i] = name;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

/*
void CFilmData::SetModifyFlag(BOOL flg)
{
	m_modifyFlag = flg;
}


BOOL CFilmData::CheckModify(void)
{
	return m_modifyFlag;
}
*/

/*_*/

