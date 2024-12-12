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

#include "makeUseDWQBat.h"


CMakeUseDWQBat::CMakeUseDWQBat(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_seList = m_app->GetSeList();
}


CMakeUseDWQBat::~CMakeUseDWQBat()
{
	End();
}

void CMakeUseDWQBat::End(void)
{
}


BOOL CMakeUseDWQBat::MakeBat(void)
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


	char* namelist = new char[16*10000];
	int nameKosuu = 0;

	CreateDirectory("tmp",NULL);
	CreateDirectory("tmp\\ta",NULL);
	CreateDirectory("tmp\\bg",NULL);
	CreateDirectory("tmp\\ev",NULL);

	FILE* batFile = CMyFile::Open("copydwq.bat","wb");



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
					if (rtt == IDOK)
					{
						if (pFilm->GetTaikenLevel() > 0)
						{
							continue;
						}
					}

					int komaKosuu = pFilm->GetObjectKosuu();
					for (int j=0;j<komaKosuu;j++)
					{
						CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(j));
						if (pKoma != NULL)
						{
							for (int k=0;k<16;k++)
							{
								if (pKoma->GetEffectFlag(k))
								{
									if (pKoma->GetPictureNumber(k) != -1)
									{
										char* picname = pKoma->GetPicFileName(k);
										if ((*picname) != 0)
										{
											int nm = SearchPicNameSub(picname,nameKosuu,namelist);
											if (nm == -1)
											{
												memcpy(namelist + nameKosuu * 16,picname,strlen(picname)+1);
												nameKosuu++;

												char buf2[256];
												char subdir[16];


												char c1 = *picname;
												char c2 = *(picname+1);

												subdir[0] = 'e';
												subdir[1] = 't';
												subdir[2] = 'c';
												subdir[3] = 0;

												if (
														((c1 == 't') && (c2 == 'a')) ||
														((c1 == 'b') && (c2 == 'g')) ||
														((c1 == 'e') && (c2 == 'v')) ||
														((c1 == 'e') && (c2 == 'd')) 
													)
												{
													subdir[0] = c1;
													subdir[1] = c2;
													subdir[2] = 0;
												}




												wsprintf(buf2,"copy dwq\\%s\\%s.dwq tmp\\%s\r\n",subdir,picname,subdir);
												fwrite(buf2,sizeof(char),strlen(buf2),batFile);
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

	fclose(batFile);

	DELETEARRAY(namelist);

	MessageBox(NULL,"おわりましたにゃ","確認",MB_OK);

//	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}


int CMakeUseDWQBat::SearchPicNameSub(LPSTR name, int kosuu, char* buff)
{
	for (int i=0;i<kosuu;i++)
	{
		if (strcmp(name,buff+i*16) == 0) return i;
	}

	return -1;
}
