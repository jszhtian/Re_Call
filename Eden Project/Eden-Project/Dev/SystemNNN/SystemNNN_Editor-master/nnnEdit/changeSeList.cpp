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

#include "changeSeList.h"


CChangeSeList::CChangeSeList(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_charaList = m_app->GetCharaList();

	m_seList2 = NULL;
	m_fromToTable = NULL;
}


CChangeSeList::~CChangeSeList()
{
	End();
}

void CChangeSeList::End(void)
{
	DELETEARRAY(m_fromToTable);
	ENDDELETECLASS(m_seList2);
}

BOOL CChangeSeList::AllChange(void)
{
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


	m_seList2 = new CNameList();
	if (m_seList2->LoadFile("nnndir\\selist2.txt") == FALSE)
	{
		ENDDELETECLASS(m_seList2);
		MessageBox(NULL,"nnndir\\selist2.txtが見つかりません","Error",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}


	//make table
	m_seList = m_app->GetSeList();
	int ks = m_seList->GetNameKosuu() / 2;
	int ks2 = m_seList2->GetNameKosuu() / 2;

	DELETEARRAY(m_fromToTable);
	m_fromToTable = new int[ks];
	
	int checkType = 0;
	int rt000 = MessageBox(NULL,"効果音名で変換しますか(Noだとファイル名で変換)","選択",MB_YESNO);
	if (rt000 == IDYES)
	{
		checkType = 1;
	}

	for (int i=0;i<ks;i++)
	{
		m_fromToTable[i] = -1;

		int found = -1;
		LPSTR seName1 = m_seList->GetName(i*2+checkType);

		for (int ii=0;ii<ks2;ii++)
		{
			LPSTR seName2 = m_seList2->GetName(ii*2+checkType);
			if (_stricmp(seName1,seName2) == 0)
			{
				found = ii;
				break;
			}
		}

		if (found != -1)
		{
			m_fromToTable[i] = found;
		}
	}

	int nnnKosuu = m_projectList->GetNameKosuu();

	//check

	BOOL checkFlag = TRUE;


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


			if (CheckRoutine() == FALSE)
			{
				checkFlag = FALSE;
				break;
			}
		}
	}

	if (checkFlag == FALSE)
	{
		DELETEARRAY(m_fromToTable);
		ENDDELETECLASS(m_seList2);
		return FALSE;
	}



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



			ChangeSeRoutine();

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


	//copy
	CopyFile("nnndir\\selist2.txt","nnndir\\selist.txt",FALSE);
	m_app->ReLoadSeList();

	DELETEARRAY(m_fromToTable);
	ENDDELETECLASS(m_seList2);

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);


	//copy
	


//	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
//	LoadByFileNameOnly(lastFileName);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}

BOOL CChangeSeList::CheckRoutine(void)
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
							for (int ch=0;ch<8;ch++)
							{
								if (pMessage->CheckSEFlag(ch))
								{
									int se = pMessage->GetSENumber(ch);
									if (se>=0)
									{
										if (m_fromToTable[se] == -1)
										{
										//	pMessage->GetMessage1Gyo(0);
											PrintError(m_app->GetFileNameOnly(),pFilm->GetMyName(),j+1,i+1,pMessage->GetMessageData());
											return FALSE;
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

	return TRUE;

}

void CChangeSeList::PrintError(LPSTR nnnName,LPSTR filmName,int koma,int mes,LPSTR firstMes)
{
	char emes[1024];
	wsprintf(emes,"%s\n%s\n%dコマ\n%d番メッセージ\n%s",nnnName,filmName,koma,mes,firstMes);
	MessageBox(NULL,emes,"変換できない効果音エラー",MB_OK | MB_ICONEXCLAMATION);
}


BOOL CChangeSeList::ChangeSeRoutine(void)
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
							for (int ch=0;ch<8;ch++)
							{
								if (pMessage->CheckSEFlag(ch))
								{
									int se = pMessage->GetSENumber(ch);
									if (se>=0)
									{
										pMessage->SetSENumber(m_fromToTable[se],ch);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return TRUE;
}

/*
BOOL CChangeSeList::ChangeSeList(BOOL flg)
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
								pMessage->SetVoiceLock(flg);
							}
						}
					}
				}
			}
		}
	}

	return TRUE;

}
*/




/*_*/


