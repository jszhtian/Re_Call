#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "filmCaseData.h"
#include "filmData.h"
#include "komaData.h"
#include "messageData.h"

#include "storyBookData.h"

//#include "myInputDialog.h"

#include "deleteVoice.h"


CDeleteVoice::CDeleteVoice(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_charaList = m_app->GetCharaList();
}


CDeleteVoice::~CDeleteVoice()
{
	End();
}

void CDeleteVoice::End(void)
{
}

BOOL CDeleteVoice::DeleteVoiceAll(void)
{
	if (FinalAnswer() == FALSE) return FALSE;

	int rt00 = MessageBox(NULL,"本当にいいですか","確認",MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION );
	if (rt00 != IDYES) return FALSE;

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
//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);
			if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);



			DeleteVoiceRoutine();

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


//			CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
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
//	LoadByFileNameOnly(lastFileName);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}

BOOL CDeleteVoice::DeleteVoice(void)
{
	if (FinalAnswer() == FALSE) return FALSE;
	m_app->SetModify();
	return DeleteVoiceRoutine();
}

BOOL CDeleteVoice::DeleteVoiceRoutine(void)
{
//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	CFilmCaseData* pCase = pDoc->GetFilmCaseData();
	CFilmCaseData* pCase = m_app->GetFilmCaseData();
	if (pCase == NULL) return FALSE;

	int filmKosuu = pCase->GetObjectKosuu();
	int nm[256];
	for (int ii=0;ii<256;ii++) nm[ii] = 0;

//	char checkname[1024];

//	int fileNumber = m_projectList->SearchName(m_fileNameOnly) + 1;
	int fileNumber = m_projectList->SearchName(m_app->GetFileNameOnly()) + 1;

	for (int k=0;k<filmKosuu;k++)
	{
		CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(k));
		if (pFilm != NULL)
		{
			int komaKosuu = pFilm->GetObjectKosuu();
			for (int j=0;j<komaKosuu;j++)
			{
				CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(j));
				if (pKoma != NULL)
				{
					int messageKosuu = pKoma->GetObjectKosuu();
					for (int i=0;i<messageKosuu;i++)
					{
						CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
						if (pMessage != NULL)
						{
							if (pMessage->GetVoiceFlag())
							{
								pMessage->SetVoiceFlag(FALSE);
								pMessage->SetVoiceFlag(FALSE,1);
								pMessage->SetVoiceFlag(FALSE,2);
								pMessage->SetVoiceFlag(FALSE,3);
							}
						}
					}
				}
			}
		}
	}

	return TRUE;

}



BOOL CDeleteVoice::FinalAnswer(void)
{
	int rt = MessageBox(NULL,"すべての音声を削除します。\nよろしいですか?","削除確認",MB_YESNO | MB_ICONEXCLAMATION);
	if (rt == IDYES) return TRUE;

	return FALSE;
}


/*_*/


