#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nyanlib\include\myfile.h"

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

#include "autoVoice.h"


CAutoVoice::CAutoVoice(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();
	m_charaList = m_app->GetCharaList();
}

CAutoVoice::~CAutoVoice()
{
	End();
}

void CAutoVoice::End(void)
{
}



BOOL CAutoVoice::SetVoice(void)
{
//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	CFilmCaseData* pCase = pDoc->GetFilmCaseData();

	CFilmCaseData* pCase = m_app->GetFilmCaseData();
	if (pCase == NULL) return FALSE;

	int filmKosuu = pCase->GetObjectKosuu();
	int nm[256];
	for (int ii=0;ii<256;ii++) nm[ii] = 0;

	char checkname[1024];

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
							int type = pMessage->GetMessageMode();

							if ((type == 0) || (type == 1) || (type == 2))
							{
								//てぬき、改行までのチェックはにゃい。


//								LPSTR name = pMessage->GetMessageData();
								LPSTR name = pMessage->GetMessageDataSkipComment();
								if (name != NULL)
								{
									char c = *name;
									int ln = 0;
									while ((c != 0) && (c != 0xd) && (ln<1023))
									{
										checkname[ln] = c;
										name++;
										ln++;
										c = *name;
									}
									checkname[ln] = 0;

									int nameNum = m_charaList->SearchName(checkname,-1,TRUE);

									if (nameNum != -1)
									{
										if ((nameNum % 2) == 1)
										{
											nameNum /= 2;
											nm[nameNum]++;

											pMessage->SetVoiceFlag();

											if (pMessage->CheckVoiceLock() == FALSE)
											{
												pMessage->SetVoicePlayerNumber(nameNum);
												int vn = nm[nameNum];
												pMessage->SetVoiceNumber(vn);
												char filename[256];
	//int fileNumber = 1;	//dummy
												wsprintf(filename,"%s%d%d%d%d%d%d",m_charaList->GetName(nameNum*2),
													fileNumber / 10,
													fileNumber % 10,
													(vn / 1000) % 10,
													(vn / 100) % 10,
													(vn / 10) % 10,
													vn % 10);
//OutputDebugString(filename);

												if (vn >= 10000)
												{
													filename[4] = 'a' + ((vn / 1000) - 10);
												}

												if (vn >= 36000)
												{
													int vnn = vn - 36000;

													filename[4] = 'a' + ((vnn / (26*26*26)) % 26);
													filename[5] = 'a' + ((vnn / (26*26)) % 26);
													filename[6] = 'a' + ((vnn / 26) % 26);
													filename[7] = 'a' + (vnn % 26);
												}

												pMessage->SetVoiceFileName(filename);
												pMessage->SetRenameFlag(FALSE);
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

	return TRUE;
}



BOOL CAutoVoice::AllSetVoice(void)
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



			SetVoice();

			char filename[1024];
			wsprintf(filename,"nnndir\\nnn\\%s.nnn",nnnfilename);
//			FILE* file = fopen(filename,"wb");
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
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}

/*_*/

