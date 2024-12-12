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

#include "makeUseVawBat.h"


CMakeUseVawBat::CMakeUseVawBat(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_seList = m_app->GetSeList();
	m_charaList = m_app->GetCharaList();

}


CMakeUseVawBat::~CMakeUseVawBat()
{
	End();
}

void CMakeUseVawBat::End(void)
{
}


BOOL CMakeUseVawBat::MakeBat(void)
{
	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (m_app->CheckSave() == FALSE) return FALSE;

	int rtt = MessageBox(NULL,"体験版にしますか","確認",MB_OKCANCEL | MB_DEFBUTTON2);

	int nameKosuuMax = 15000;
	char* namelist = new char[16*nameKosuuMax];
	int nameKosuu = 0;

	int seFlag[1024];
	for (int ss=0;ss<1024;ss++) seFlag[ss] = 0;

	CreateDirectory("tmp",NULL);
	CreateDirectory("tmp\\cdvaw",NULL);

	int ckk = m_charaList->GetNameKosuu() / 2;
	for (int pp=1;pp<ckk;pp++)
	{
		LPSTR cnm = m_charaList->GetName(pp*2+0);
		char subdir[1024];
		wsprintf(subdir,"tmp\\cdvaw\\%s",cnm);
		CreateDirectory(subdir,NULL);
	}



	FILE* batFile = CMyFile::Open("usewav.bat","wb");

	int nnnKosuu = m_projectList->GetNameKosuu();


	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);

		if (nnnfilename != NULL)
		{
//			MessageBox(NULL,nnnfilename,"@",MB_OK);

//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);

//			if (pp==23)
//			{
//				MessageBox(NULL,"a1","t",MB_OK);
//			}

			if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}

	//		if (pp==23)
	//		{
	//			MessageBox(NULL,"a2","t",MB_OK);
	//		}

			//memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
			m_app->SetFileNameOnly(nnnfilename);

	//		if (pp==23)
	//		{
	//			MessageBox(NULL,"a3","t",MB_OK);
	//		}


//			CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//			CFilmCaseData* pCase = pDoc->GetFilmCaseData();

			CFilmCaseData* pCase = m_app->GetFilmCaseData();

	//		if (pp==23)
	//		{
	//			MessageBox(NULL,"a4","t",MB_OK);
	//		}

			if (pCase == NULL) continue;

	//		if (pp==23)
	//		{
	//			MessageBox(NULL,"a5","t",MB_OK);
	//		}

			int filmKosuu = pCase->GetObjectKosuu();

	//		if (pp==23)
	//		{
	//			char mes888[256];
	//			sprintf(mes888,"filmKosuu=%d",filmKosuu);
	//			MessageBox(NULL,mes888,"!0",MB_OK);
	//		}

			for (int f=0;f<filmKosuu;f++)
			{

				CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(f));
				if (pFilm != NULL)
				{
				//	if (pp==23)
				//	{
				//		char mes888[256];
				//		sprintf(mes888,"%s",pFilm->GetMyName());
				//		MessageBox(NULL,mes888,"!1",MB_OK);
				//	}

					if (rtt == IDOK)
					{
						if (pFilm->GetTaikenLevel() > 0)
						{
							continue;
						}
					}

				//	if (pp==23)
				//	{
				//		char mes888[256];
				//		sprintf(mes888,"%s",pFilm->GetMyName());
				//		MessageBox(NULL,mes888,"!2",MB_OK);
				//	}

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
															delete [] namelist;
															nameKosuuMax += 5000;
															namelist = new char[nameKosuuMax * 16];
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

														wsprintf(buf2,"copy cdvaw\\%s\\%s.vaw tmp\\cdvaw\\%s\r\n",subdir,voicename,subdir);
		//												sprintf(buf2,"if not exist cdwave\\%s\\%s.wav echo %s\r\n",subdir,voicename,voicename);
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
	for (int senum=0;senum<1024;senum++)
	{
		if (seFlag[senum] == 1)
		{
			char* sename = m_seList->GetName(senum*2);


			char buf3[256];
			wsprintf(buf3,"copy vaw\\se\\%s.vaw tmp\\vaw\\se\r\n",sename);

			fwrite(buf3,sizeof(char),strlen(buf3),batFile);
		}
	}



	fclose(batFile);

	DELETEARRAY(namelist);

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

	//memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}

int CMakeUseVawBat::SearchPicNameSub(LPSTR name, int kosuu, char* buff)
{
	for (int i=0;i<kosuu;i++)
	{
		if (strcmp(name,buff+i*16) == 0) return i;
	}

	return -1;
}

/*_*/

