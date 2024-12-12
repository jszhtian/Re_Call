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

#include "myInputDialog.h"

#include "FrameChange.h"


CFrameChange::CFrameChange(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_charaList = m_app->GetCharaList();
	m_inputDialog = m_app->GetInputDialog();
}

CFrameChange::~CFrameChange()
{
	End();
}

void CFrameChange::End(void)
{
}


BOOL CFrameChange::FrameChange(int oldFrame,int newFrame)
{
//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	CFilmCaseData* pCase = pDoc->GetFilmCaseData();

	CFilmCaseData* pCase = m_app->GetFilmCaseData();
	if (pCase == NULL) return FALSE;

	if (oldFrame < 1)
	{
		if (m_inputDialog->GetNumber(m_app->GetDefaultFrameTime(),&oldFrame,"古いフレーム時間をミリ秒で入れてください") == FALSE) return FALSE;
	}

	if (newFrame < 1)
	{
		if (m_inputDialog->GetNumber(oldFrame,&newFrame,"新しいフレーム時間をミリ秒で入れてください") == FALSE) return FALSE;
	}

	if ((oldFrame < 1) || (newFrame < 1)) return FALSE;






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
			int nowFrameTime = oldFrame;

			int komaKosuu = pFilm->GetObjectKosuu();
			for (int j=0;j<komaKosuu;j++)
			{
				CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(j));
				if (pKoma != NULL)
				{

					int f = pKoma->GetFrameTime();
					if (f == -1)
					{
						nowFrameTime = oldFrame;
					}
					else if (f > 0)
					{
						nowFrameTime = f;
						pKoma->SetFrameTime(newFrame);
					}

					int overrapType = pKoma->GetOverrapType();
					int overrapCount = pKoma->GetOverrapCount();

					if (overrapCount > 0)
					{
						overrapCount *= nowFrameTime;
						overrapCount /= newFrame;
						if (overrapCount < 1) overrapCount = 1;

						pKoma->SetOverrapCount(overrapCount);
					}


					//effect
					for (int layer = 0;layer<16;layer++)
					{
						if (pKoma->GetEffectFlag(layer))
						{
							int eff = pKoma->GetEffect(layer);
							if (eff != -1)
							{
								if (eff != 0)	//effect_nop
								{
									int ek = pKoma->GetEffectParaKosuu(layer);
									if (ek>0)
									{
										int frame = pKoma->GetEffectPara(layer,0);
										frame *= nowFrameTime;
										frame /= newFrame;
										if (frame < 1) frame = 1;
										pKoma->SetEffectPara(layer,0,frame);
									}
								}
							}
						}
					}






					int messageKosuu = pKoma->GetObjectKosuu();
					for (int i=0;i<messageKosuu;i++)
					{
						CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
						if (pMessage != NULL)
						{
							int type = pMessage->GetMessageMode();

							if (type == 3)
							{
								int draw = pMessage->GetDrawCount();
								draw *= nowFrameTime;
								draw /= newFrame;
								if (draw < 1) draw = 1;
								pMessage->SetDrawCount(draw);
							}
						}
					}
				}
			}
		}
	}

	return TRUE;
}




BOOL CFrameChange::AllFrameChange(void)
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




	int oldFrame = m_app->GetDefaultFrameTime();
	int newFrame = oldFrame;

	m_inputDialog->GetNumber(m_app->GetDefaultFrameTime(),&oldFrame,"古いフレーム時間をミリ秒で入れてください");
	m_inputDialog->GetNumber(oldFrame,&newFrame,"新しいフレーム時間をミリ秒で入れてください");


	int nnnKosuu = m_projectList->GetNameKosuu();
	if ((oldFrame != newFrame) && (oldFrame > 0) && (newFrame > 0))
	{
		for (int pp=0;pp<nnnKosuu;pp++)
		{
			LPSTR nnnfilename = m_projectList->GetName(pp);
			if (nnnfilename != NULL)
			{
		//		memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
				m_app->SetFileNameOnly(nnnfilename);

				if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
				{
					MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
					continue;
				}

	//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
				m_app->SetFileNameOnly(nnnfilename);



	//			SetVoice();
				FrameChange(oldFrame,newFrame);//@@@

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
	}

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

//	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}

/*_*/

