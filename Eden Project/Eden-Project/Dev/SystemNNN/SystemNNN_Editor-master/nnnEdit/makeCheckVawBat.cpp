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

//#include "myInputDialog.h"

#include "makeCHeckVawBat.h"


CMakeCheckVawBat::CMakeCheckVawBat(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_seList = m_app->GetSeList();
}


CMakeCheckVawBat::~CMakeCheckVawBat()
{
	End();
}

void CMakeCheckVawBat::End(void)
{
}


BOOL CMakeCheckVawBat::MakeBat(void)
{
	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (m_app->CheckSave() == FALSE) return FALSE;


	int nameKosuuMax = 15000;
	char* namelist = new char[16*nameKosuuMax];
	int nameKosuu = 0;

	int seFlag[256];
	for (int ss=0;ss<256;ss++) seFlag[ss] = 0;

//	int seKosuu = 0;


	FILE* batFile = CMyFile::Open("checkwav.bat","wb");

	char* mes0 = "echo off\r\n";
	fwrite(mes0,sizeof(char),strlen(mes0),batFile);




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


			CFilmCaseData* pCase = m_app->GetFilmCaseData();

//			CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//			CFilmCaseData* pCase = pDoc->GetFilmCaseData();
			if (pCase == NULL) continue;

			int filmKosuu = pCase->GetObjectKosuu();

			for (int f=0;f<filmKosuu;f++)
			{
				CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(f));
				if (pFilm != NULL)
				{
					int komaKosuu = pFilm->GetObjectKosuu();
					for (int j=0;j<komaKosuu;j++)
					{
						CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(j));
						if (pKoma != NULL)
						{
							int messageKosuu = pKoma->GetObjectKosuu();
							for (int m=0;m<messageKosuu;m++)
							{
								CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(m));
								if (pMessage != NULL)
								{

									if (pMessage->GetVoiceFlag())
									{
										for (int channel = 0;channel<4;channel++)
										{
											if (pMessage->CheckVoiceFlag(channel))
											{
												char* voicename = pMessage->GetVoiceFileName(channel);

												if ((*voicename) != 0)
												{
													int nm = SearchPicNameSub(voicename,nameKosuu,namelist);
													if (nm == -1)
													{
														if (nameKosuu >= nameKosuuMax)
														{
															//bug!コピーしてにゃい

															delete [] namelist;
															nameKosuuMax += 5000;
															namelist = new char[16*nameKosuuMax];
														}

														memcpy(namelist + nameKosuu * 16,voicename,strlen(voicename)+1);
														nameKosuu++;

														char buf2[256];
														char subdir[16];


														char c1 = *voicename;
														char c2 = *(voicename+1);

														subdir[0] = c1;
														subdir[1] = c2;
														subdir[2] = 0;

				//												sprintf(buf2,"copy cdwave\\%s\\%s.wav tmp\\cdwave\\%s >>log\r\n",subdir,voicename,subdir);
														wsprintf(buf2,"if not exist cdwave\\%s\\%s.wav echo %s\r\n",subdir,voicename,voicename);
														fwrite(buf2,sizeof(char),strlen(buf2),batFile);
													}


												}
											}
										}

									}

									if (pMessage->GetSEFlag())
									{
										for (int channel = 0;channel<8;channel++)
										{
											int se = pMessage->GetSENumber(channel);
											if (se != -1)
											{
												seFlag[se] = 1;
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

	//make se list
	for (int senum=0;senum<256;senum++)
	{
		if (seFlag[senum] == 1)
		{
			char* sename = m_seList->GetName(senum*2);


			char buf3[256];
//			sprintf(buf3,"copy wav\\se\\%s.wav tmp\\wav\\se >>log\r\n",sename);
			wsprintf(buf3,"if not exist wav\\se\\%s.wav echo %s\r\n",sename,sename);

			fwrite(buf3,sizeof(char),strlen(buf3),batFile);
		}
	}

	fwrite("pause",sizeof(char),strlen("pause"),batFile);

	fclose(batFile);

	DELETEARRAY(namelist);

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

//	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
//	LoadByFileNameOnly(lastFileName);

	return TRUE;
}


int CMakeCheckVawBat::SearchPicNameSub(LPSTR name, int kosuu, char* buff)
{
	for (int i=0;i<kosuu;i++)
	{
		if (strcmp(name,buff+i*16) == 0) return i;
	}

	return -1;
}
