#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectlist.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "filmCaseData.h"
#include "filmData.h"
#include "komaData.h"
#include "messageData.h"

#include "checkDWQ.h"


CCheckDWQ::CCheckDWQ(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
}

CCheckDWQ::~CCheckDWQ()
{
	End();
}

void CCheckDWQ::End(void)
{
}

BOOL CCheckDWQ::CheckDWQAll(void)
{
	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (m_app->CheckSave() == FALSE) return FALSE;



	int nnnKosuu = m_projectList->GetNameKosuu();
	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);
		if (nnnfilename != NULL)
		{
		//	memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);

			if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);



			if (CheckDWQ() == FALSE) break;
		}
	}

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

//	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;

}


BOOL CCheckDWQ::CheckDWQ(void)
{
//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	CFilmCaseData* pCase = pDoc->GetFilmCaseData();
	CFilmCaseData* pCase = m_app->GetFilmCaseData();

	int kosuu = pCase->GetObjectKosuu();
	int serial = 1;

	for (int k=0;k<kosuu;k++)
	{
//OutputDebugString(":");
		CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(k));
		int kosuu2 = pFilm->GetObjectKosuu();
		for (int j=0;j<kosuu2;j++)
		{
//OutputDebugString("@");
			CKomaData* pKoma = pFilm->GetKoma(j);
			for (int i=0;i<16;i++)
			{
				if (pKoma->GetPictureNumber(i) != -1)
				{
					LPSTR dwqName = pKoma->GetPicFileName(i);
					if (*(dwqName) != 0)
					{
						//check 
//OutputDebugString("*");
						char filename[256];
						wsprintf(filename,"dwq\\__\\%s.dwq",dwqName);
						filename[4] = *(dwqName);
						filename[5] = *(dwqName+1);
						FILE* file = CMyFile::Open(filename,"rb");
						if (file == NULL)
						{
//							char errMes[256];
//							sprintf(errMes,m_dwqName);


							char eMes[1024];
							wsprintf(eMes,"スクリプト:%s\nフィルム:%s\n%dコマ目\nレイヤー:%d\n%s\n\n続けますか",m_app->GetFileNameOnly(),pFilm->GetMyName(),j+1,i,dwqName);
							int yesno = MessageBox(NULL,eMes,m_app->GetFileNameOnly(),MB_YESNO | MB_ICONSTOP);
//							MessageBox(NULL,dwqName,m_app->GetFileNameOnly(),MB_OK | MB_ICONSTOP);
							
							if (yesno != IDYES)
							{
								return FALSE;
							}
						}
						else
						{
							fclose(file);
						}
					}
				}
			}
		}
	}

	return TRUE;
}

