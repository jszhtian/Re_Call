#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
#include "..\..\systemNNN\nyanlib\include\areaControl.h"
#include "..\..\systemNNN\nyanlib\include\allGeo.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectlist.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"
#include "..\..\systemNNN\nnnUtilLib\myMessage.h"

//#include "..\nnnUtilLib\cutinControl.h"

#include "..\..\systemNNN\nnnUtilLib\cutinNameData.h"
#include "..\..\systemNNN\nnnUtilLib\cutinMessageWindow.h"

#include "..\..\systemNNN\nnnUtilLib\shakin.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "filmCaseData.h"
#include "filmData.h"
#include "komaData.h"
#include "messageData.h"

#include "storyData.h"
#include "storyBookData.h"

#include "mySearchDialog.h"

#include "filmCaseDoc.h"
#include "filmDoc.h"
#include "GameMessageDoc.h"
#include "conteMessageDoc.h"

#include "programDoc.h"

#include "mainScreenDoc.h"

#include "layerDoc.h"
#include "conteDoc.h"

#include "zahyoDoc.h"
#include "varDoc.h"

#include "effectDoc.h"

#include "storyBookDoc.h"
#include "storyDoc.h"

#include "myView.h"



#include "messageSetup.h"

#include "myOnTimer.h"

#include "windowlist.h"

#include "rubiMaker.h"



CMyOnTimer::CMyOnTimer(CMyApplicationBase* app)
{
	m_app = app;
	m_effect = m_app->GetEffect();
	m_message = m_app->GetMyMessage();
	m_messageSetup = m_app->GetMessageSetup();

	m_rubiMaker = m_app->GetRubiMaker();
	m_shakinControl = m_app->GetShakinControl();


	m_messagePrintX = m_app->GetConfig("mesPrintX");
	m_messagePrintY = m_app->GetConfig("mesPrintY");
	m_messageLPrintX = m_app->GetConfig("mesLPrintX");
	m_messageLPrintY = m_app->GetConfig("mesLPrintY");
	m_messageAppendX = m_app->GetConfig("mesAppendX");
	m_messageAppendY = m_app->GetConfig("mesAppendY");
	
	m_messageNextY = m_app->GetConfig("mesPrintNextY");

	m_messageWindowPrintMode = m_app->GetConfig("mesWinPrintMode");
	m_messageWindowPrintZahyo = m_messageSetup->GetMesWinZahyo();
	m_messageWindowSize = m_messageSetup->GetMesWinSize();

//	m_messageWindowPrintX = m_app->GetConfig("mesWinPrintX");
//	m_messageWindowPrintY = m_app->GetConfig("mesWinPrintY");
//	m_messageWindowSizeX = m_app->GetConfig("mesWinSizeX");
//	m_messageWindowSizeY = m_app->GetConfig("mesWinSizeY");

	m_messageFontSize = m_app->GetConfig("mesFontSize");
	m_messageWindowColorR = 50;
	m_messageWindowColorG =200;
	m_messageWindowColorB = 150;
	m_messageWindowPercent = 50;

	m_cutinMessageWindow = new CCutinMessageWindow();

	m_fps = 50;
	m_totalSkipFrame = 0;
	m_fpsWork = 0;
	m_oldTime = GetLocalTimeCount();
	m_totalSkipFrame = 0;
	m_skipFrame = 0;
}



CMyOnTimer::~CMyOnTimer()
{
	End();
}

void CMyOnTimer::End(void)
{
	ENDDELETECLASS(m_cutinMessageWindow);
}


LONGLONG CMyOnTimer::GetLocalTimeCount(void)
{
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	int hour = tm.wHour;
	int minute = tm.wMinute;
	int second = tm.wSecond;
	int millSecond = tm.wMilliseconds;

	LONGLONG d = hour;
	d *= 60;
	d += minute;
	d *= 60;
	d += second;
	d *= 1000;
	d += millSecond;

	return d;
}


int CMyOnTimer::OnTimer(int n)
{
	m_fpsWork++;

	LONGLONG newTime = GetLocalTimeCount();


	if ((newTime - m_oldTime) >= 1000)
	{
		m_fps = (m_fpsWork * 1000) / ((int)(newTime - m_oldTime));
		if (m_fps>999) m_fps = 999;

		m_oldTime = newTime;
		m_fpsWork = 0;
	}

	int dmyrnd = rand();

	POINT pt;
	GetCursorPos(&pt);
//	CMainScreenDoc* mdoc = (CMainScreenDoc*)m_document[MAINSCREEN_WINDOW];
	CMainScreenDoc* mdoc = (CMainScreenDoc*)(m_app->GetMyDocument(MAINSCREEN_WINDOW));
	CMyView* mview = mdoc->GetView();
	

	ScreenToClient(mview->GetHWND(),&pt);
	m_effect->SetMouseZahyo(pt);

	m_cutinNameData = m_app->GetCutinNameData();


	m_skipFrame = 0;

	if (m_app->GetPlayMode())
	{
		m_app->SetNextPrint();

		if (m_app->GetPauseFlag() == FALSE)
		{
			BOOL b = TRUE;
			CGameMessageDoc* pDoc = (CGameMessageDoc*)(m_app->GetMyDocument(GAMEMESSAGE_WINDOW));
			CMessageData* pMessage = (CMessageData*)(pDoc->GetNowSelectMessage());

			int kaeseru = 0;

			if (pMessage != NULL)
			{
				CKomaData* pKoma = m_app->GetNowSelectKoma();
				if (pKoma != NULL)
				{
					if (pKoma->GetNowSelectNumber() == 0)
					{
						if (m_app->GetGameCount() < pKoma->GetOverrapCount())
						{
							b = FALSE;
//							if (m_app->GetGameCount() == (pKoma->GetOverrapCount()) - 1)
							if (m_app->GetGameCount() >= (pKoma->GetOverrapCount()) - 1)
							{
//OutputDebugString("OnTimer()");
								m_app->CheckAndPlaySound();
							}
				
							kaeseru = pKoma->GetOverrapCount() - m_app->GetGameCount() - 1;
							if (kaeseru < 1) kaeseru = 1;
						}
					}

					if (kaeseru == 0)
					{
						CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(pKoma->GetNowSelectNumber()));
						if (pMessage != NULL)
						{
							if (pMessage->GetMessageMode() == 3)
							{
								kaeseru = pMessage->GetDrawCount() - (m_app->GetGameCount() - pKoma->GetOverrapCount());
								if (kaeseru < 0) kaeseru = 0;
							}
							else
							{
								//print
								kaeseru = 999;
							}
						}
					}
				}
			}


			int kaesu = m_shakinControl->GetHensaiFrame(kaeseru);
			m_shakinControl->HensaiFrame(kaesu);


			if (kaesu > 0)
			{
				if (b)
				{
					m_effect->CountUp(kaesu);
				}

				m_app->GameCountUp(kaesu);
			}

			m_skipFrame = (kaesu-1);
			if (m_skipFrame < 0) m_skipFrame = 0;

//			m_gameCount++;
		}

		m_app->CheckNextMessage();

	}
	else
	{
		m_shakinControl->Clear();
		m_totalSkipFrame = 0;
	}

	m_totalSkipFrame += m_skipFrame;

	if (m_app->GetNextPrintFlag())
	{
		m_app->SetNextPrint(FALSE);

		int overrapCount = 0;
		int overrapType = 0;

		BOOL b = TRUE;
		CGameMessageDoc* pDoc = (CGameMessageDoc*)(m_app->GetMyDocument(GAMEMESSAGE_WINDOW));
		CMessageData* pMessage = (CMessageData*)(pDoc->GetNowSelectMessage());

		int rubiPrintFlag = m_app->GetRubiPrintFlag();

		if (pMessage != NULL)
		{
			int md = pMessage->GetMessageMode();
//			if (md == 3)
//			{
//				CFilmDoc* pDoc2 = (CFilmDoc*)m_document[FILM_WINDOW];
//				CKomaData* pKoma = (CKomaData*)(pDoc2->GetNowSelectKoma());
				CKomaData* pKoma = m_app->GetNowSelectKoma();
				if (pKoma != NULL)
				{
					if (pKoma->GetNowSelectNumber() == 0)
					{
						overrapCount = pKoma->GetOverrapCount();
						overrapType = pKoma->GetOverrapType();

						if (m_app->GetGameCount() < overrapCount)
						{
							b = FALSE;
						}
					}
				}
//			}
		}


		if (overrapCount == 0) b = TRUE;

//		if (m_playMode != TRUE) b = TRUE;
		if (m_app->GetPlayMode() != TRUE) b = TRUE;

		if (b)
		{
			CAreaControl::SetNextAllPrint();
			m_effect->BeginEffect();
			//	m_effect->CountUp();
			m_effect->CalcuOnly();
			m_effect->Print(FALSE, TRUE);
			if (m_messageWindowPrintMode == 2)
			{
				if (pMessage != NULL)
				{
					int md = pMessage->GetMessageMode();
					if (md != 3)
					{
						m_app->PrintMessageWindow();
					}
				}
			}
			m_effect->Print(TRUE, TRUE);


m_app->PrintHint();

			m_effect->EndEffect();


			if (pMessage != NULL)
			{
				int md = pMessage->GetMessageMode();
				if (md != 3)
				{
					int fontSize = m_messageFontSize;
					int messageNextY = m_messageNextY;

					LPSTR mes = pMessage->GetMessageDataSkipAllComment();
//					m_message->PrintMessage(24,360,mes);


					int cutinWindowFlag = 0;


					int mesPutX = m_messagePrintX;
					int mesPutY = m_messagePrintY;

					int winPutX = m_messageWindowPrintZahyo.x;
					int winPutY = m_messageWindowPrintZahyo.y;
					int winSizeX = m_messageWindowSize.cx;
					int winSizeY = m_messageWindowSize.cy;
					int winColorR = m_messageWindowColorR;
					int winColorG = m_messageWindowColorG;
					int winColorB = m_messageWindowColorB;
					int winColorA = m_messageWindowPercent;


					int messageLayer = -1;

					int havegyo = pMessage->GetMessageGyo();
					int mesMax = 0;
					int mesGyo = 0;
					int pmy = 0;
					int cutin = -1;

					if (m_app->GetCutinFlag())
					{
						CFilmData* pFilm = m_app->GetNowSelectFilm();
						int komaNumber = pFilm->GetNowSelectNumber();


						if (pFilm->CheckNowCutin(komaNumber))
						{
							if (m_cutinNameData != NULL)
							{

								LPSTR picFileName[16];

								for (int pm=0;pm<havegyo;pm++)
								{
									int mln = pMessage->GetMessage1Gyo(pm);
									LPSTR rmes = pMessage->GetTmpMessage();

									if (mln>=2)
									{
										if ((*rmes) == '/')
										{
											if ((*(rmes+1)) == '/') continue;
										}
									}

									if (pmy == 0)
									{
										cutin = m_cutinNameData->SearchFirst(rmes);
										if (cutin>0)
										{
											pFilm->GetNowPicList(picFileName,komaNumber);
										}

										while (cutin>0)
										{
											LPSTR tagName = m_cutinNameData->GetFileNameTag(cutin);

											if (CheckTagPicFile(tagName,picFileName))
											{
												break;
											}
											cutin = m_cutinNameData->GetNext();
										}

										if (cutin > 0)
										{
										}
										else
										{
											int mmm = m_message->GetMessageRealLength(rmes);
											if (mmm>mesMax) mesMax = mmm;
											mesGyo++;
										}
									}
									else
									{
										int mmm = m_message->GetMessageRealLength(rmes);
										if (mmm>mesMax) mesMax = mmm;
										mesGyo++;
									}
									pmy++;
								}

								if (cutin > 0)
								{
									POINT cZahyo = m_cutinNameData->GetZahyo(cutin);
									mesPutX = cZahyo.x;
									mesPutY = cZahyo.y;
									POINT wZahyo = m_cutinNameData->GetWindowZahyo(cutin);
									winPutX = wZahyo.x;
									winPutY = wZahyo.y;
									int windowType = m_cutinNameData->GetWindowType(cutin);
									
									POINT nPoint = m_cutinNameData->GetNextZahyo(cutin);

									fontSize = m_messageFontSize + m_cutinNameData->GetFontSizeDelta(cutin);
									messageNextY = nPoint.y;

									if (windowType == 0)
									{
										winSizeX = 0;
										winSizeY = 0;
									}
									else if (windowType >= 1)
									{
										if (windowType == 1)
										{
											SIZE wSize = m_cutinNameData->GetWindowSize(cutin,m_messageFontSize,mesGyo,mesMax );
											winSizeX = wSize.cx;
											winSizeY = wSize.cy;
										}
										else
										{
											winSizeX = windowType;
											winSizeY = (mesGyo-1) * nPoint.y + fontSize;
											SIZE wDXDY = m_cutinNameData->GetWindowDXDY(cutin);
											winSizeY += wDXDY.cy * 2;
										}

										winColorR = m_cutinNameData->GetWindowColorR(cutin);
										winColorG = m_cutinNameData->GetWindowColorG(cutin);
										winColorB = m_cutinNameData->GetWindowColorB(cutin);
										winColorA = m_cutinNameData->GetWindowColorA(cutin);

									}
									else if (windowType < 0)
									{
										m_cutinMessageWindow->LoadPic(-windowType);
										cutinWindowFlag = 1;
										//pic not support
									}
								}
							}
						}
					}
		
					if (md == 0)
					{
						if (cutinWindowFlag)
						{
							POINT wp;
							wp.x = winPutX;
							wp.y = winPutY;
							m_cutinMessageWindow->Print(wp);
						}
						else
						{
							if (m_messageWindowPrintMode == 1)
							{
								CAllGeo::TransBoxFill(winPutX,winPutY,winSizeX,winSizeY,winColorR,winColorG,winColorB,winColorA);
							}
						}
					}
					else if (md == 1)
					{
						mesPutX = m_messageLPrintX;
						mesPutY = m_messageLPrintY;
					}
					else if (md == 2)
					{
						mesPutX = m_messageAppendX;
						mesPutY = m_messageAppendY;
					}

//					int havegyo = pMessage->GetMessageGyo();
//					int pmy = 0;
					BOOL nameFound = FALSE;

					pmy = 0;
					for (int pm=0;pm<havegyo;pm++)
					{
						int mln = pMessage->GetMessage1Gyo(pm);
						LPSTR rmes = pMessage->GetTmpMessage();

						if (mln>=2)
						{
							if ((*rmes) == '/')
							{
								if ((*(rmes+1)) == '/') continue;
							}
						}

						if ((pmy == 0) && (nameFound == FALSE) && (cutin != -1))
						{
							nameFound = TRUE;
							continue;
						}

						LPSTR rmes2 = rmes;
						if (rubiPrintFlag == 2)
						{
							rmes2 = m_rubiMaker->RubiConvert(rmes);
						}

						m_message->PrintMessage(mesPutX,mesPutY+pmy*messageNextY,rmes2,fontSize,255,255,255,1,30,0);
						pmy++;
					}

					//m_message->PrintMessage(mesPutX,mesPutY,mes,m_messageFontSize,255,255,255,1,30,0);
				}
			}
		}
		else
		{
			CAreaControl::SetNextAllPrint();

			m_effect->BeginEffect();
///			m_overrapPic->Overrap(m_overrapPic2,((m_gameCount)*100)/overrapCount);

			m_app->PrintOverrap(overrapType,m_app->GetGameCount(),overrapCount);
			//m_effect->PrintSimpleWipe(overrapType,m_overrapPic,m_overrapPic2,m_gameCount,overrapCount);



//			m_effect->BeginEffect();
//			m_effect->CalcuOnly();
//			m_effect->Print(FALSE,TRUE);
			m_effect->EndEffect();
			//overrap
		}

		//message??
//		CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
//		CMessageData* pMessage = (CMessageData*)(pDoc->GetNowSelectMessage());

//		((CMainScreenDoc*)m_document[MAINSCREEN_WINDOW])->FlipToScreen();
		m_app->FlipToScreen();
	}


//	if (m_playMode)
//	{
//	}

//	char mes[256];
//	sprintf(mes,"PLAY=%d,PAUSE=%d,COMMAND=%d,COUNT=%d",m_playMode,m_pauseFlag,m_playCommand,m_gameCount);
///	m_frame->SetStatusMessage(mes);

	return 0;
}



BOOL CMyOnTimer::CheckTagPicFile(LPSTR tagName,LPSTR* fileList)
{
	int ln = strlen(tagName);
	char work[1024];
	for (int i=0;i<16;i++)
	{
		LPSTR checkFile = fileList[i];
		if (checkFile != NULL)
		{
			int ln2 = strlen(checkFile);
			if (ln2 >= ln)
			{
				memcpy(work,checkFile,ln);
				work[ln] = 0;
				work[ln+1] = 0;
				if (strcmp(work,tagName) == 0)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


int CMyOnTimer::GetFPS(void)
{
	return m_fps;
}

int CMyOnTimer::GetNowSkip(void)
{
	return m_skipFrame;
}

int CMyOnTimer::GetTotalSkip(void)
{
	return m_totalSkipFrame;
}


/*_*/

