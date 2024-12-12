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

#include "makeBook.h"

char CMakeBook::m_bookTypeName[][16]=
{
	"dummy",
	"通常",
	"強調",
	"のみ",
	"以外",
};

char CMakeBook::m_bookTypeDesc[][16]=
{
	"dummy",
	"通常",
	"強調",
	"のみ",
	"以外",
};

CMakeBook::CMakeBook(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_charaList = m_app->GetCharaList();

	m_errorSkipFlag = FALSE;
//	m_whiteSpaceStr = new char[1024];
//	ZeroMemory(m_whiteSpaceStr,1024);

}

CMakeBook::~CMakeBook()
{
	End();
}


void CMakeBook::End(void)
{
//	DELETEARRAY(m_whiteSpaceStr);
}


//
// makeBook
//
// 全員共通分
// キャラ
// キャラ強調
// 本人のみ
//

BOOL CMakeBook::MakeBook(int allFlag,int makeBookMode)
{
	int defaultVoiceFlag = m_app->GetConfig("addDefaultVoice");



	m_makeBookMode = makeBookMode;
	m_makeBookAllFlag = allFlag;

	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (m_app->CheckSave() == FALSE) return FALSE;


	SetErrorSkip(FALSE);

	CreateDirectory("nnndir\\book",NULL);

	int nnnKosuu = m_projectList->GetNameKosuu();


	for (int i=0;i<256;i++)
	{
		m_voiceCount[i] = 0;
	}

	int makeBookModeOrg = m_makeBookMode;

	int loops = 4;
	if (makeBookMode == 4)
	{
		loops = 5;
	}

	for (int makeMode=0;makeMode<loops;makeMode++)
	{
		if ((makeMode == m_makeBookMode) || (m_makeBookAllFlag && (makeMode < 4)))
		{
			if (m_makeBookAllFlag) m_makeBookMode = makeMode;

			for (int pp=0;pp<nnnKosuu;pp++)
			{
				LPSTR nnnfilename = m_projectList->GetName(pp);
				if (nnnfilename != NULL)
				{
					if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
					{
						PrintUtilLoadError(nnnfilename);
						continue;
					}

					int charaKosuu = m_charaList->GetNameKosuu();
					charaKosuu /= 2;

					if (m_makeBookMode == 0) charaKosuu = 1;

					int startChara = 0;
					if (m_makeBookMode > 0) startChara = 1;

					for (int cc=startChara;cc<charaKosuu;cc++)
					{
//						CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//						CFilmCaseData* pCase = pDoc->GetFilmCaseData();
						CFilmCaseData* pCase = m_app->GetFilmCaseData();
						if (pCase == NULL) return FALSE;

						int filmKosuu = pCase->GetObjectKosuu();

						char filename[1024];
						if (cc==0)
						{
							wsprintf(filename,"nnndir\\book\\all_%s.txt",nnnfilename);
						}
						else
						{
							LPSTR charaNameTag = m_charaList->GetName(cc*2);
							LPSTR charaName = m_charaList->GetName(cc*2+1);
							char dirName[256];


							wsprintf(dirName,"nnndir\\book\\%s",charaNameTag);
							CreateDirectory(dirName,NULL);

							wsprintf(dirName,"nnndir\\book\\%s\\%s",charaNameTag,m_bookTypeName[m_makeBookMode]);
							CreateDirectory(dirName,NULL);

							wsprintf(filename,"%s\\%s[%s]_%s_%s.txt",dirName,charaNameTag,charaName,nnnfilename,m_bookTypeDesc[m_makeBookMode]);

//
//							if (m_makeBookMode == 1)
//							{
//								sprintf(filename,"nnndir\\book\\%s_%s_のみ.txt",m_charaList->GetName(cc*2),nnnfilename);
//							}
//							else if (m_makeBookMode == 2)
//							{
//								sprintf(filename,"nnndir\\book\\%s_%s_強調.txt",m_charaList->GetName(cc*2),nnnfilename);
//							}
						}


						FILE* file = CMyFile::Open(filename,"wb");
						if (file == NULL)
						{
							MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
							return FALSE;
						}

						BOOL foundFlag = FALSE;
						if (cc == 0) foundFlag = TRUE;


						char title[256];
						char sep[256];

						if (cc == 0)
						{
//							wsprintf(title,"全台本[%s]\r\n\r\n",m_app->GetFileNameOnly());
							wsprintf(title,"全台本[%s]\r\n\r\n",nnnfilename);
						}
						else
						{
//							wsprintf(title,"%s台本[%s]\r\n\r\n",m_charaList->GetName(cc*2+1),m_app->GetFileNameOnly());
							wsprintf(title,"%s台本[%s]\r\n\r\n",m_charaList->GetName(cc*2+1),nnnfilename);
						}

						fwrite(title,sizeof(char),strlen(title),file);

						for (int k=0;k<filmKosuu;k++)
						{
							CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(k));
							if (pFilm != NULL)
							{
								wsprintf(sep,"\r\n\r\n★【フィルム】(%s)\r\n\r\n",pFilm->GetMyName());
								fwrite(sep,sizeof(char),strlen(sep),file);


								//checkある？

//								if ((cc == 0) || (m_makeBookMode == 2) || (pFilm->CheckExistChara(cc)))
								if ((makeMode == 4) || (cc == 0) || (pFilm->CheckExistChara(cc)))
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
													int type = pMessage->GetMessageMode();
													if (type != 3)
													{
														int pln = pMessage->GetVoicePlayerNumber();





		///												if ((cc == 0) || (pln != 0))


														BOOL ff = FALSE;
														BOOL enf = FALSE;

														if (cc == 0) ff = TRUE;


														if (m_makeBookMode == 0)
														{
															ff = TRUE;
														}

														if (m_makeBookMode == 1)
														{
															ff = TRUE;
														}
														
														if (m_makeBookMode == 2)
														{
															ff = TRUE;
															if (pln == cc) enf = TRUE;
														}

														if (m_makeBookMode == 3)
														{
															if (pln == cc) ff = TRUE;
														}

														if (m_makeBookMode == 4)
														{
															if (pln == 0)
															{
																ff = TRUE;
															}

														}


														if (m_makeBookMode == 3)
														{
															if (pMessage->GetVoiceFlag() == 0)
															{
																ff = FALSE;
															}
														}


														if (ff)
														{
															int defNam = 0;
															if (defaultVoiceFlag)
															{
																defNam = pMessage->CheckIncludeSeimei(defaultVoiceFlag);
															}

															if (m_makeBookAllFlag)
															{
																if (makeMode == 0)
																{
																	if (pln>0)
																	{
																		m_voiceCount[pln]++;
																		if (defaultVoiceFlag)
																		{
																			if (defNam)
																			{
																				m_voiceCount[pln]++;
																			}
																		}
																	}
																}
															}

															pMessage->MakeBook(file,TRUE,enf,defNam);
															foundFlag = TRUE;
														}
													}
												}
											}
										}
									}
								}
							}
						}

						fclose(file);

						if ((makeMode < 4) && (foundFlag == FALSE))
						{
							//kill
							if (DeleteFile(filename) == FALSE)
							{
								MessageBox(NULL,filename,"DELETE ERROR!",MB_OK | MB_ICONEXCLAMATION);
							}
						}
					}
				}
			}
		}
	}


	FILE* voiceFile = CMyFile::Open("nnndir\\book\\voiceCount.txt","wb");
	if (voiceFile != NULL)
	{
		char mes[256];
		wsprintf(mes,"音声の個数\x00d\x00a\x00d\x00a");
		fwrite(mes,sizeof(char),strlen(mes),voiceFile);

		int nz = m_charaList->GetNameKosuu() / 2;
		for (int i2=1;i2<nz;i2++)
		{
			wsprintf(mes,"%s : %d個\x00d\x00a",m_charaList->GetName(i2*2+1),m_voiceCount[i2]);
			fwrite(mes,sizeof(char),strlen(mes),voiceFile);
		}

//		sprintf(mes,"-----------------------------------------------------------");
//		fwrite(mes,sizeof(char),strlen(mes),voiceFile);
//
//		sprintf(mes,"ファイル別\x00d\x00a\x00d\x00a");
//		fwrite(mes,sizeof(char),strlen(mes),voiceFile);

		fclose(voiceFile);
	}



	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}


void CMakeBook::PrintUtilLoadError(LPSTR filename)
{
	if (CheckErrorSkip()) return;
	char text[1024];
	char caption[256];

	wsprintf(caption,"Load nnn Error");
	wsprintf(text,"Filename:%s\n以降エラー表示を中止しますか?",filename);
	int rt = MessageBox(NULL,text,caption,MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2);

	if (rt == IDYES) SetErrorSkip();
}

/*_*/



