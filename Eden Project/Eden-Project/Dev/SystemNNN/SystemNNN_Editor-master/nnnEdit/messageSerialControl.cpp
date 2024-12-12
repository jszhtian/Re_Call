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

#include "commandData.h"
#include "storyData.h"
#include "storyBookData.h"

//#include "myInputDialog.h"

#include "messageSerialControl.h"

#include "commanddatatype.h"

CMessageSerialControl::CMessageSerialControl(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_selectSerialID = 1;
}

CMessageSerialControl::~CMessageSerialControl()
{
	End();
}

void CMessageSerialControl::End(void)
{
}


BOOL CMessageSerialControl::SetAllMessageSerial(BOOL updateFlag)
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



	int nnnKosuu = m_projectList->GetNameKosuu();


	m_selectSerialID = 1;


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
			SetMessageSerial(updateFlag);

			char filename[1024];
			wsprintf(filename,"nnndir\\nnn\\%s.nnn",nnnfilename);
			FILE* file = CMyFile::Open(filename,"wb");
			if (file == NULL)
			{
				MessageBox(NULL,"SAVE OPEN ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

			fwrite("NNN DATA HEADER",sizeof(char),16,file);
//			fwrite(m_myname,sizeof(char),64,file);
//			fwrite(m_memo,sizeof(char),256,file);
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
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;

}



BOOL CMessageSerialControl::SetMessageSerial(BOOL updateFlag)
{
	//CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	//CFilmCaseData* pCase = pDoc->GetFilmCaseData();

	CFilmCaseData* pCase = m_app->GetFilmCaseData();

	int kosuu = pCase->GetObjectKosuu();
	int serial = 1;

	if (updateFlag)
	{
		serial = GetMaxSerial();
	}


	int serialLimit = 1024;
	int slimit = 128;

	int nnnKosuu = m_projectList->GetNameKosuu();
	for (int ss=0;ss<8;ss++)
	{
		if (nnnKosuu>slimit)
		{
			break;
		}
		serialLimit *= 2;
		slimit /= 2;
	}



	int k;
	for (k=0;k<kosuu;k++)
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
				int typ = pMessage->GetMessageMode();

				if (typ == 3)
				{
					pMessage->SetID(0);
				}
				else
				{
					if (serial<serialLimit)
					{
						if ((updateFlag == FALSE) || (pMessage->GetID() == 0))
						{
							pMessage->SetID(serial);
							serial++;
							if (serial>=serialLimit)
							{
								MessageBox(NULL,"メッセージ数がおおすぎますにゃ","ERROR",MB_OK);
							}
						}
					}
				}
			}
		}
	}

	//選択肢にシリアルをうつ

//	CStoryBookDoc* pDoc2 = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
//	CStoryBookData* pCase2 = pDoc2->GetStoryBookData();

	CStoryBookData* pCase2 = m_app->GetStoryBookData();

	kosuu = pCase2->GetObjectKosuu();
	serial = 1;

	if (updateFlag)
	{
		serial = GetMaxSelectSerial();
	}

	for (k=0;k<kosuu;k++)
	{
		CStoryData* pStory = (CStoryData*)(pCase2->GetObjectData(k));
		int kosuu2 = pStory->GetObjectKosuu();
		for (int j=0;j<kosuu2;j++)
		{
			CCommandData* pCommand = pStory->GetCommand(j);
			
			int typ = pCommand->GetCommandType();
			if (typ == COMMANDDATATYPE_SELECT)
			{
				if ((updateFlag == FALSE) || (pCommand->GetSerial() == 0))
				{
					pCommand->SetSerial(serial);
					serial++;
				}

				pCommand->SetSelectMessageSerial(m_selectSerialID);
				m_selectSerialID++;
			}
		}
	}

	return TRUE;
}

int CMessageSerialControl::GetMaxSerial(void)
{

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
				int typ = pMessage->GetMessageMode();

				if (typ != 3)
				{
					int ss = pMessage->GetID();
					if (ss>=0)
					{
						if (ss>serial)
						{
							serial = ss;
						}
					}
				}
			}
		}
	}

	serial ++;
	return serial;
}

int CMessageSerialControl::GetMaxSelectSerial(void)
{
	CStoryBookData* pCase2 = m_app->GetStoryBookData();

	int kosuu = pCase2->GetObjectKosuu();
	int serial = 1;

	for (int k=0;k<kosuu;k++)
	{
		CStoryData* pStory = (CStoryData*)(pCase2->GetObjectData(k));
		int kosuu2 = pStory->GetObjectKosuu();
		for (int j=0;j<kosuu2;j++)
		{
			CCommandData* pCommand = pStory->GetCommand(j);
			
			int typ = pCommand->GetCommandType();
			if (typ == COMMANDDATATYPE_SELECT)
			{
				int ss = pCommand->GetSerial();
				if (ss>serial)
				{
					serial = ss;
				}
			}
		}
	}
	serial++;
	return serial;
}


/*_*/


