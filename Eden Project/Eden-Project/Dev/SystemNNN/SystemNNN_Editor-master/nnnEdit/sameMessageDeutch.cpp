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


#include "sameMessageDeutch.h"

CSameMessageDeutch::CSameMessageDeutch(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();

}	

CSameMessageDeutch::~CSameMessageDeutch()
{
	End();
}

void CSameMessageDeutch::End(void)
{
}



BOOL CSameMessageDeutch::SameMessageToDeutch(void)
{
	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (m_app->CheckSave() == FALSE) return FALSE;


	int kosuu = m_projectList->GetNameKosuu();



	char* mesBuf = new char[1024*1024 * 8];
	char* filenameBuf = new char[1024*16 * 8];

	int totalSrcMessageKosuu = 0;

	int totalChangeKosuu = 0;

	for (int j=0;j<kosuu;j++)
	{
		LPSTR nnnfilename = m_projectList->GetName(j);
		
		if (nnnfilename != NULL)
		{
			if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
				continue;
			}
		}

		char startMes[256];
		wsprintf(startMes,"\n%sと同じものを検索中...",nnnfilename);
		OutputDebugString(startMes);

		totalSrcMessageKosuu = 0;

//		CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//		CFilmCaseData* pCase = pDoc->GetFilmCaseData();
		CFilmCaseData* pCase = m_app->GetFilmCaseData();

		int filmKosuu = pCase->GetObjectKosuu();
		for (int f=0;f<filmKosuu;f++)
		{
			CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(f));
			if (pFilm != NULL)
			{
				int komaKosuu = pFilm->GetObjectKosuu();
				for (int k=0;k<komaKosuu;k++)
				{
					CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(k));
					if (pKoma != NULL)
					{
						int messageKosuu =pKoma->GetObjectKosuu();
						for (int m=0;m<messageKosuu;m++)
						{
							CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(m));
							if (pMessage != NULL)
							{
								int typ = pMessage->GetMessageMode();
								if ((typ == 0) || (typ == 1) || (typ == 2))
								{
									LPSTR mes = pMessage->GetMessageData();
									int ln = 0;
									if (mes == NULL)
									{
										*(mesBuf+1024*totalSrcMessageKosuu)=0;
									}
									else
									{
										ln = strlen(mes);
										if (ln>1022) ln = 1022;

										memcpy(mesBuf+1024*totalSrcMessageKosuu,mes,ln+1);
									}

									int ln2 = 0;
									*(filenameBuf+16*totalSrcMessageKosuu)=0;

									if (pMessage->GetVoiceFlag())
									{
										LPSTR fname = pMessage->GetVoiceFileName();
										if (fname != NULL)
										{
											ln2 = strlen(fname);

											if (ln2>14) ln = 14;

											memcpy(filenameBuf+16*totalSrcMessageKosuu,fname,ln+1);
										}
									}

									totalSrcMessageKosuu++;

									if (totalSrcMessageKosuu>=1024 * 8)
									{
										MessageBox(NULL,"ERROR Message Is To Long","ERROR",MB_OK | MB_ICONEXCLAMATION);
									}
								}
							}
						}
					}
				}
			}
		}



		for (int i=j;i<kosuu;i++)
		{

			LPSTR nnnfilename2 = m_projectList->GetName(i);
			
			if (nnnfilename2 != NULL)
			{
				if (m_app->LoadByFileNameOnly(nnnfilename2) == FALSE)
				{
					MessageBox(NULL,"ERROR!","ERROR",MB_OK | MB_ICONEXCLAMATION);
					continue;
				}
			}


			//check main



//			CFilmCaseDoc* pDoc2 = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//			CFilmCaseData* pCase2 = pDoc2->GetFilmCaseData();
			CFilmCaseData* pCase2 = m_app->GetFilmCaseData();

			BOOL flg = FALSE;
			int changeKosuu = 0;


			int dstMesNum = 0;

			int filmKosuu2 = pCase2->GetObjectKosuu();
			for (int f2=0;f2<filmKosuu2;f2++)
			{
				CFilmData* pFilm2 = (CFilmData*)(pCase2->GetObjectData(f2));
				if (pFilm2 != NULL)
				{
					int komaKosuu2 = pFilm2->GetObjectKosuu();
					for (int k2=0;k2<komaKosuu2;k2++)
					{
						CKomaData* pKoma2 = (CKomaData*)(pFilm2->GetObjectData(k2));
						if (pKoma2 != NULL)
						{
							int messageKosuu2 = pKoma2->GetObjectKosuu();
							for (int m2=0;m2<messageKosuu2;m2++)
							{
								CMessageData* pMessage2 = (CMessageData*)(pKoma2->GetObjectData(m2));
								if (pMessage2 != NULL)
								{
									int typ2 = pMessage2->GetMessageMode();
									if ((typ2 == 0) || (typ2 == 1) || (typ2 == 2))
									{
										int ed = totalSrcMessageKosuu;

										LPSTR dstMes = pMessage2->GetMessageData();


										if (i == j)
										{
											ed = dstMesNum;
										}

										for (int c=0;c<ed;c++)
										{
											if (strcmp(dstMes,mesBuf+1024*c) == 0)
											{
												if (pMessage2->GetVoiceFlag())
												{
													LPSTR vname2 = pMessage2->GetVoiceFileName();
													if (strcmp(vname2,filenameBuf+16*c) != 0)
													{
														flg = TRUE;
														changeKosuu++;

														//change data
														pMessage2->SetVoiceFileName(filenameBuf+16*c);


														pMessage2->SetRenameFlag();

														totalChangeKosuu++;
													}
												}

												break;
											}

										}

										dstMesNum++;
									}
								}
							}
						}
					}
				}
			}

			if (flg)
			{
				//save???
				char mesmes[1024];
				wsprintf(mesmes,"[%sに%d個]",nnnfilename2,changeKosuu);
				OutputDebugString(mesmes);



				char filename[1024];
				wsprintf(filename,"nnndir\\nnn\\%s.nnn",nnnfilename2);
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


//				CFilmCaseDoc* pDocS = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//				CFilmCaseData* pCaseS = pDocS->GetFilmCaseData();
				CFilmCaseData* pCaseS = m_app->GetFilmCaseData();
				pCaseS->Save(file);


//				CStoryBookDoc* pDoc2S = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
//				CStoryBookData* pBookS = pDoc2S->GetStoryBookData();
				CStoryBookData* pBookS = m_app->GetStoryBookData();
				pBookS->Save(file);

				fclose(file);


			}


		}
	}








	char endmes[256];
	wsprintf(endmes,"%d個のメッセージのファイル名を変更しましたにゃん",totalChangeKosuu);

	MessageBox(NULL,endmes,"確認",MB_OK);

	m_app->LoadByFileNameOnly(lastFileName);

	return TRUE;
}
