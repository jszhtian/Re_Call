#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"

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

#include "mySearchDialog.h"

#include "searchOrReplace.h"

#include "windowlist.h"

CSearchOrReplace::CSearchOrReplace(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();

	m_search = new CMySearchDialog(m_app->GetFrameHWND(),m_app->GetHInstance(),1024);
}

CSearchOrReplace::~CSearchOrReplace()
{
	End();
}

void CSearchOrReplace::End(void)
{
	ENDDELETECLASS(m_search);
}

void CSearchOrReplace::SearchOrReplace(BOOL replaceFlag,BOOL allFlag)
{
	char lastFileName[256];
	if (allFlag)
	{
		if (m_app->CheckSave() == FALSE) return;
		LPSTR fnameonly = m_app->GetFileNameOnly();
		memcpy(lastFileName,fnameonly,strlen(fnameonly)+1);
	}

	BOOL startFlag = TRUE;

	int cmd = m_search->GetCommand(replaceFlag,allFlag,startFlag);
	if (cmd == -1) return;

	int radio = m_search->GetRadio();
	int radio2 = m_search->GetRadio2();

	LPSTR searchString = m_search->GetSearchText();
	LPSTR replaceString = m_search->GetReplaceText();

	int startNNN = 1;
	if (allFlag)
	{
		if (_stricmp(m_app->GetFileNameOnly(),m_projectList->GetName(1)) != 0)
		{
			m_app->LoadByFileNameOnly(m_projectList->GetName(1));
			radio2 = 1;
			m_search->SetDefaultRadio2(radio2);
		}
	}
	else
	{
		if (_stricmp(m_app->GetFileNameOnly(),"sys") == 0)
		{
			return;
		}
	}



//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	CFilmDoc* pDoc2 = (CFilmDoc*)m_document[FILM_WINDOW];
//	CGameMessageDoc* pDoc3 = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	CFilmCaseData* pFilmCase = m_app->GetFilmCaseData();

//	int startFilm = pDoc->GetNowSelectNumber();
	int startFilm = pFilmCase->GetNowSelectNumber();

	if (allFlag && (radio2 == 1))
	{
		startFilm = 0;
	}

	if ((radio >= 2) && (radio2 == 1))
	{
		startFilm = 0;
	}


//	CFilmData* filmData = pDoc->GetFilm(startFilm);
	CFilmData* filmData = (CFilmData*)(pFilmCase->GetObjectData(startFilm));



	int startKoma = 0;
	if (filmData != NULL) startKoma = filmData->GetNowSelectNumber();


	if (allFlag && (radio2 == 1))
	{
		startKoma = 0;
	}

	if ((radio >= 1) && (radio2 == 1))
	{
		startKoma = 0;
	}


	CKomaData* komaData = NULL;
	if (komaData != NULL) komaData = filmData->GetKoma(startKoma);


	int startMessage = 0;

	if (komaData != NULL) startMessage = komaData->GetNowSelectNumber();

	if (allFlag && (radio2 == 1))
	{
		startMessage = 0;
	}

	if ((radio >= 0) && (radio2 == 1))
	{
		startMessage = 0;
	}

	CMessageData* messageData = NULL;
	if (komaData != NULL) messageData = (CMessageData*)(komaData->GetObjectData(startMessage));

	int startPointer = 0;

	if (radio2 == 1)
	{
		//??
	}



	int nnnKosuu = m_projectList->GetNameKosuu();
	
	BOOL allReplaceFlag = FALSE;
	if (cmd == 2) allReplaceFlag = TRUE;

	BOOL quitFlag = FALSE;


	while (startNNN < nnnKosuu)
	{
//		BOOL replaceFlag = FALSE;

//		int filmKosuu = pDoc->GetFilmKosuu();
		int filmKosuu = pFilmCase->GetObjectKosuu();

		while (startFilm < filmKosuu)
		{
//			filmData = pDoc->GetFilm(startFilm);
			filmData = (CFilmData*)(pFilmCase->GetObjectData(startFilm));

			if (filmData == NULL) break;

			int komaKosuu = filmData->GetObjectKosuu();
			while (startKoma < komaKosuu)
			{
				komaData = filmData->GetKoma(startKoma);
				if (komaData == NULL) break;

				int messageKosuu = komaData->GetObjectKosuu();
				while (startMessage < messageKosuu)
				{
					messageData = (CMessageData*)(komaData->GetObjectData(startMessage));
					if (messageData == NULL) break;

					if (messageData->GetMessageMode() != 3)
					{
						LPSTR message = messageData->GetMessageData();
						int messageLength = strlen(message);

						while (startPointer < messageLength)
						{
							BOOL proceed = FALSE;

							if (cmd == 2) allReplaceFlag = TRUE;

							LPSTR found = NULL;
							if (((*message) != 0) && ((*searchString) != 0))
							{
								found = strstr(message+startPointer,searchString);
							}

							if (found == NULL)
							{
								startPointer = messageLength;
							}
							else
							{
								//みつけた
								startFlag = FALSE;
								int foundPtr = found - (message+startPointer);


//char fff[256];
//sprintf(fff,"[%d %d %d %d %d %d]\n",startNNN,startFilm,startKoma,startMessage,startPointer,foundPtr);
//OutputDebugString(fff);
								startPointer += foundPtr;

								int srcLength = strlen(searchString);

								if (cmd == 2)
								{
									//replace
									int replaceLength = strlen(replaceString);
									messageData->Replace(startPointer,srcLength,replaceString);

									message = messageData->GetMessageData();
									messageLength = strlen(message);
									startPointer += replaceLength;
								}

								if (allReplaceFlag == FALSE)
								{
//									pDoc->SetSelectNumber(startFilm);
									pFilmCase->SetSelectNumber(startFilm);
									filmData->SetSelectNumber(startKoma);
									komaData->SetSelectNumber(startMessage);
									//scroll

								//	pDoc->ScrollToSelect();
								//	pDoc2->ScrollToSelect();
								//	pDoc3->ScrollToSelect();
									m_app->ScrollToSelect(FILMCASE_WINDOW);
									m_app->ScrollToSelect(FILM_WINDOW);
									m_app->ScrollToSelect(GAMEMESSAGE_WINDOW);

									m_app->UpdateMyWindow(-1);

									m_search->SetDefaultRadio2(0);

									cmd = m_search->GetCommand(replaceFlag,allFlag,startFlag);

									searchString = m_search->GetSearchText();
									replaceString = m_search->GetReplaceText();

									if (cmd == 0)
									{
										startPointer += srcLength;
									}

									if ((cmd == 1) || (cmd == 2))
									{
										int replaceLength = strlen(replaceString);
										messageData->Replace(startPointer,srcLength,replaceString);

										message = messageData->GetMessageData();
										messageLength = strlen(message);
										startPointer += replaceLength;
									}

									if (cmd == 2)
									{
										allReplaceFlag = TRUE;
									}

									if (cmd == -1)
									{
										quitFlag = TRUE;
										break;
									}
								}
							}

							if (cmd == -1)
							{
								quitFlag = TRUE;
								break;
							}
						}
					}

					if (quitFlag) break;
					if (allFlag == FALSE)
					{
						if (radio < 1) break;
					}

					startMessage++;
					startPointer = 0;
				}

				if (quitFlag) break;
				if (allFlag == FALSE)
				{
					if (radio < 2) break;
				}

				startKoma++;
				startMessage = 0;
				startPointer = 0;
			}

			if (quitFlag) break;
			if (allFlag == FALSE)
			{
				if (radio < 3) break;
			}
			startFilm++;
			startKoma = 0;
			startMessage = 0;
			startPointer = 0;
		}

		if (allFlag == FALSE)
		{
			if (replaceFlag)
			{
				m_app->SetModify();
			}
			break;
		}

		if (replaceFlag)
		{
			m_app->SaveRoutine(startNNN);
		}

		startNNN++;
		startFilm = 0;
		startKoma = 0;
		startMessage = 0;
		startPointer = 0;

		if (quitFlag) break;

		if (startNNN < nnnKosuu)
		{
			m_app->LoadByFileNameOnly(m_projectList->GetName(startNNN));
		}
	}

	if (allFlag)
	{
		m_app->LoadByFileNameOnly(lastFileName);
	}

	MessageBox(NULL,"検索・置換終了","終了",MB_OK);
}

/*_*/



