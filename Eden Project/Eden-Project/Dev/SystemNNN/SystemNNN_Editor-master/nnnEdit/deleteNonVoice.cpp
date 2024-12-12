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


#include "storyBookData.h"

#include "myInputDialog.h"

#include "deleteNonVoice.h"


CDeleteNonVoice::CDeleteNonVoice(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_input = m_app->GetMyInputDialog();
}

CDeleteNonVoice::~CDeleteNonVoice()
{
	End();
}

void CDeleteNonVoice::End(void)
{
}

BOOL CDeleteNonVoice::DeleteAllNonVoice(void)
{

	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (m_app->CheckSave() == FALSE) return FALSE;


	LPSTR delPlayer = m_input->GetText("削除するプレイヤー番号");
	if (delPlayer == NULL) return FALSE;

	m_deletePlayerNumber = atoi(delPlayer);

	int nnnKosuu = m_projectList->GetNameKosuu();
	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);
		if (nnnfilename != NULL)
		{
//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);

			if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);



//			AutoVoice();
//			SetMessageSerial();
			DeleteNonVoice(FALSE);

			char filename[1024];
			wsprintf(filename,"nnndir\\nnn\\%s.nnn",nnnfilename);
			FILE* file = CMyFile::Open(filename,"wb");
			if (file == NULL)
			{
				MessageBox(NULL,"SAVE OPEN ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

			fwrite("NNN DATA HEADER",sizeof(char),16,file);
			fwrite(m_app->GetMyName(),sizeof(char),64,file);
			fwrite(m_app->GetMemo(),sizeof(char),256,file);

			int tmp[16];
			for (int i=0;i<16;i++) tmp[i] = 0;

			tmp[0] = 2;
			fwrite(tmp,sizeof(int),16,file);


			//CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//			CFilmCaseData* pCase = pDoc->GetFilmCaseData();
			CFilmCaseData* pCase = m_app->GetFilmCaseData();
			pCase->Save(file);


//			CStoryBookDoc* pDoc2 = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
//			CStoryBookData* pBook = pDoc2->GetStoryBookData();
			CStoryBookData* pBook = m_app->GetStoryBookData();
			pBook->Save(file);

			fclose(file);
		}
	}

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

//	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}



BOOL CDeleteNonVoice::DeleteNonVoice(BOOL selFlag)
{

	if (selFlag)
	{
		LPSTR delPlayer = m_input->GetText("削除するプレイヤー番号");
		if (delPlayer == NULL) return FALSE;

		m_deletePlayerNumber = atoi(delPlayer);
	}


//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	CFilmCaseData* pCase = pDoc->GetFilmCaseData();
	CFilmCaseData* pCase = m_app->GetFilmCaseData();

	int kosuu = pCase->GetObjectKosuu();
	int serial = 1;

	for (int k=0;k<kosuu;k++)
	{
		CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(k));
		int kosuu2 = pFilm->GetObjectKosuu();
		for (int j=0;j<kosuu2;j++)
		{
			CKomaData* pKoma = pFilm->GetKoma(j);

			int kosuu3 = pKoma->GetObjectKosuu();
			for (int i=0;i<kosuu3;i++)
			{
				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
				if (pMessage->GetVoiceFlag())
				{
					int pl = pMessage->GetVoicePlayerNumber();
					if (pl == m_deletePlayerNumber)
					{
						pMessage->SetVoiceFlag(FALSE);
					}
				}
			}
		}
	}

	return TRUE;
}


