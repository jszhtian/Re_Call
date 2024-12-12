#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "resource.h"

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
#include "..\..\systemNNN\nyanlib\include\areaControl.h"
#include "..\..\systemNNN\nyanlib\include\myGraphics.h"
#include "..\..\systemNNN\nyanlib\include\picture.h"


#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectlist.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "filmCaseData.h"
#include "filmData.h"
#include "komaData.h"
#include "messageData.h"

//#include "myInputDialog.h"

#include "aviSave.h"
#include "movieCreate.h"

HWND CMovieCreate::m_staticText = NULL;
HWND CMovieCreate::m_progressBar = NULL;
HWND CMovieCreate::m_progressBar2 = NULL;

CMovieCreate::CMovieCreate(CMyApplicationBase* app)
{
	m_app = app;
	m_hInstance = m_app->GetHInstance();
	m_effect = m_app->GetEffect();

	m_overrapPic = m_app->GetOverrapPic();
	m_overrapPic2 = m_app->GetOverrapPic2();

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();
	m_aviSave = new CAviSave(screenSizeX,screenSizeY);
}

CMovieCreate::~CMovieCreate()
{
	End();
}

void CMovieCreate::End(void)
{
	ENDDELETECLASS(m_aviSave);
}



void CMovieCreate::MakeMovieAll(void)
{
	m_app->SetPlayMode(FALSE);
//	m_playMode = FALSE;
//	MessageBox(m_frameHWND,"未実装","!",MB_OK | MB_ICONEXCLAMATION);
//	return;

	CFilmCaseData* filmCase = m_app->GetFilmCaseData();
	int filmKosuu = filmCase->GetObjectKosuu();
	if (filmKosuu <= 0) return;

	CreateProceedDialog();

	int totalFrame = 0;
	int i;
	for (i=0;i<filmKosuu;i++)
	{
		CFilmData* pFilm = (CFilmData*)(filmCase->GetObjectData(i));
		if (pFilm != NULL)
		{
			totalFrame += m_app->CalcuFilmTime(pFilm);
		}
	}

	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	OpenMovieFile(fileNameOnly,totalFrame);
//	OpenMovieFile(m_fileNameOnly,totalFrame);

	int amari = 0;

	for (i=0;i<filmKosuu;i++)
	{

		CFilmData* pFilm = (CFilmData*)(filmCase->GetObjectData(i));
		if (pFilm != NULL)
		{
			char mes[256];
			wsprintf(mes,"%s",pFilm->GetMyName());
			SetProceedMessage(mes);

			int pos = (100 * i) / filmKosuu;
			SetProceedBar(pos);

			amari = MakeMovieFilmSub(pFilm,amari);
		}
	}


	m_aviSave->CloseAviFile();

	DestroyProceedDialog();

	MessageBox(NULL,"おわりましたにゃ","!",MB_OK | MB_ICONEXCLAMATION);
}


void CMovieCreate::MakeMovieAllFilm(void)
{
//	m_playMode = FALSE;
	m_app->SetPlayMode(FALSE);
//	MessageBox(m_frameHWND,"未実装","!",MB_OK | MB_ICONEXCLAMATION);
//	return;

	CFilmCaseData* filmCase = m_app->GetFilmCaseData();
	int filmKosuu = filmCase->GetObjectKosuu();
	if (filmKosuu <= 0) return;

	CreateProceedDialog();

	for (int i=0;i<filmKosuu;i++)
	{
		CFilmData* pFilm = (CFilmData*)(filmCase->GetObjectData(i));
		if (pFilm != NULL)
		{
			char mes[256];
			wsprintf(mes,"%s",pFilm->GetMyName());
			SetProceedMessage(mes);

			int pos = (100 * i) / filmKosuu;
			SetProceedBar(pos);


			LPSTR name = pFilm->GetMyName();
			int frameTime = m_app->CalcuFilmTime(pFilm);

			//open
			OpenMovieFile(name,frameTime);

			//make

			MakeMovieFilmSub(pFilm);


			//close
			m_aviSave->CloseAviFile();

		}
	}

	DestroyProceedDialog();

	MessageBox(NULL,"おわりましたにゃ","!",MB_OK | MB_ICONEXCLAMATION);
}




BOOL CMovieCreate::OpenMovieFile(LPSTR name,int frameTime)
{
	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	int movieRate = m_app->GetConfig("defaultMovieRate");
	int movieScale = m_app->GetConfig("defaultMovieScale");

	CreateDirectory("avi",NULL);

	char filename[256];
	wsprintf(filename,"avi\\%s.avi",name);

	int movieSize = m_app->GetConfig("movieSize");
	m_aviSave->SetMovieSize(movieSize);

	int moviePackType = m_app->GetConfig("moviePackType");
	m_aviSave->SetMoviePackType(moviePackType);


	LONGLONG frames = frameTime * 1000;
	
	LONGLONG dv = 1000 * 1000;
	dv *= movieScale;
	dv /= movieRate;
//	LONGLONG dv = 1000 * movieRate;
//	dv /= movieScale;

	frames /= dv;

	//LONGLONG frames = frameTime * movieScale;
	//frames /= movieRate;

	int totalFrame = (int)frames;

//char mes2[256];
//sprintf(mes2,"%s[%d]\n",filename,totalFrame);
//OutputDebugString(mes2);

	return m_aviSave->CreateAVIFile(filename,totalFrame,movieRate,movieScale);
}

int CMovieCreate::MakeMovieFilmSub(CFilmData* pFilm,int amari)
{
//	m_playMode = FALSE;
	m_app->SetPlayMode(FALSE);
	if (pFilm == NULL) return amari;

	LONGLONG amari2 = amari;

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	int movieRate = m_app->GetConfig("defaultMovieRate");
	int movieScale = m_app->GetConfig("defaultMovieScale");
	int movieSize = m_app->GetConfig("movieSize");
	m_aviSave->SetMovieSize(movieSize);


	int komaKosuu = pFilm->GetObjectKosuu();

	int frameTime = m_app->GetDefaultFrameTime();

	LONGLONG movieFrameTime = 1000 * movieScale;
	movieFrameTime *= 1000;
	movieFrameTime /= movieRate;

	int proceedKosuu = 0;
	LONGLONG totalProceedKosuuLL = m_app->CalcuFilmTime(pFilm);
	totalProceedKosuuLL *= 1000;

	LONGLONG dvdv = 1000 * 1000;
	dvdv *= movieScale;
	dvdv /= movieRate;

	totalProceedKosuuLL /= dvdv;

	int totalProceedKosuu = (int)totalProceedKosuuLL;


	for (int i=0;i<komaKosuu;i++)
	{
//OutputDebugString("\n");
		CAreaControl::SetNextAllPrint();
		CKomaData* pKoma = pFilm->GetKoma(i);
		if (pKoma != NULL)
		{
			int overrapType = pKoma->GetOverrapType();
			int overrapTime = pKoma->GetOverrapCount();

			if (overrapTime > 0)
			{
				m_overrapPic->GetScreen(0,0,screenSizeX,screenSizeY);
//				m_effect->ResetCount();
			}
			else
			{
			}



			for (int i99=0;i99<16;i99++)
			{
				int eff = pKoma->GetEffect(i99);
				if (eff != EFFECT_MAE)
				{
					m_effect->ResetCount(i99);
				}
			}


			pKoma->SetAllEffect();
			pKoma->LoadAllPicture();


			int fr = pKoma->GetFrameTime();
			if (fr == -1) frameTime = m_app->GetDefaultFrameTime();
			if (fr > 0) frameTime = fr;

			if (overrapTime > 0)
			{
				CAreaControl::SetNextAllPrint();
				m_effect->BeginEffect();
				m_effect->CalcuOnly();
				m_effect->Print(FALSE,TRUE);
				m_effect->EndEffect();
				m_overrapPic2->GetScreen(0,0,screenSizeX,screenSizeY);

				LONGLONG ms = overrapTime * frameTime;
				ms *= 1000;
				LONGLONG amari0 = amari2;
				amari2 += ms;

				int overrapKomaKosuu = (int)(amari2 / movieFrameTime);

				for (int p=0;p<overrapKomaKosuu;p++)
				{
					CAreaControl::SetNextAllPrint();

					LONGLONG mms = movieFrameTime * p;
					mms -= amari0;
					int k = 0;

					if (mms>=0)
					{
						k = (int)((mms * overrapTime)/ ms);
					}

//					m_effect->PrintSimpleWipe(overrapType,m_overrapPic,m_overrapPic2,p,overrapTime);
					m_effect->PrintSimpleWipe(overrapType,m_overrapPic,m_overrapPic2,k,overrapTime);
					m_aviSave->SetData();

					if ((proceedKosuu % 10) == 0)
					{
						int pos = (100 * proceedKosuu) / totalProceedKosuu;
						SetProceedBar(pos,1);
					}
					proceedKosuu++;
				}

				amari2 -= movieFrameTime * overrapKomaKosuu;
			}


			int mesKosuu = pKoma->GetObjectKosuu();
			if (mesKosuu>0)
			{
				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(0));
				if (pMessage != NULL)
				{
					int md = pMessage->GetMessageMode();
					if (md == 3)
					{
						int drawCount = pMessage->GetDrawCount();

						LONGLONG ms = drawCount * frameTime;
						ms *= 1000;
						LONGLONG amari0 = amari2;
						amari2 += ms;

						int drawKomaKosuu = (int)(amari2 / movieFrameTime);


						int oldK = 0;

						for (int p=0;p<drawKomaKosuu;p++)
						{
							CAreaControl::SetNextAllPrint();

							LONGLONG mms = movieFrameTime * p;
							mms -= amari0;
							int k = 0;

							if (mms>=0)
							{
								k = (int)((mms * drawCount)/ ms);
							}

							m_effect->BeginEffect();
							m_effect->CalcuOnly();
							m_effect->Print(FALSE,TRUE);
							m_effect->EndEffect();
							m_aviSave->SetData();
//OutputDebugString("D");

							if (k > oldK)
							{
								m_effect->CountUp(k - oldK);
								oldK = k;
							}

							if ((proceedKosuu % 10) == 0)
							{
								int pos = (100 * proceedKosuu) / totalProceedKosuu;
								SetProceedBar(pos,1);
							}
							proceedKosuu++;


						}
						amari2 -= movieFrameTime * drawKomaKosuu;
//char mes2[256];
//sprintf(mes2,"[%d]",drawKomaKosuu);
//OutputDebugString(mes2);

					}
				}
			}
		}
	}

	return (int)amari2;
}


void CMovieCreate::MakeMovieFilm(void)
{
	//	m_playMode = FALSE;
	m_app->SetPlayMode(FALSE);
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	LPSTR name = pFilm->GetMyName();
	int frameTime = m_app->CalcuFilmTime(pFilm);

	//open
	OpenMovieFile(name,frameTime);

	CreateProceedDialog();

	//make
	char mes[256];
	wsprintf(mes,"%s",pFilm->GetMyName());
	SetProceedMessage(mes);

	MakeMovieFilmSub(pFilm);

	//close
	m_aviSave->CloseAviFile();

	DestroyProceedDialog();

	MessageBox(NULL,"ムービー処理おわりにゃ","OK",MB_OK);
	return;

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	int komaKosuu = pFilm->GetObjectKosuu();
	int frames = 0;
	int i;
	for (i=0;i<komaKosuu;i++)
	{
		CKomaData* pKoma = pFilm->GetKoma(i);
		if (pKoma != NULL)
		{
			int overrapType = pKoma->GetOverrapType();
			int overrapTime = pKoma->GetOverrapCount();
//			if ((overrapType > 0) && (overrapTime > 0))
			if (overrapTime > 0)
			{
				frames += overrapTime;
			}


			int mesKosuu = pKoma->GetObjectKosuu();
			if (mesKosuu>0)
			{
				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(0));
				if (pMessage != NULL)
				{
					int md = pMessage->GetMessageMode();
					if (md == 3)
					{
						int drawCount = pMessage->GetDrawCount();
						frames += drawCount;
					}
				}
			}
		}
	}

	if (frames <= 0) return;


	char filename[256];
	wsprintf(filename,"avi\\%s.avi",pFilm->GetMyName());
	CreateDirectory("avi",NULL);

	int movieRate = m_app->GetConfig("defaultMovieRate");
	int movieScale = m_app->GetConfig("defaultMovieScale");

	int movieSize = m_app->GetConfig("movieSize");
	m_aviSave->SetMovieSize(movieSize);


	if (m_aviSave->CreateAVIFile(filename,frames,movieRate,movieScale) == FALSE)
	{
		MessageBox(NULL,"aviファイルが作成できませんでした",filename,MB_OK | MB_ICONEXCLAMATION);
		return;
	}


	for (i=0;i<komaKosuu;i++)
	{
		CAreaControl::SetNextAllPrint();
		CKomaData* pKoma = pFilm->GetKoma(i);
		if (pKoma != NULL)
		{
			int overrapType = pKoma->GetOverrapType();
			int overrapTime = pKoma->GetOverrapCount();

//			if ((overrapType > 0) && (overrapTime > 0))
			if (overrapTime > 0)
			{
				m_overrapPic->GetScreen(0,0,screenSizeX,screenSizeY);
			}

			m_effect->ResetCount();
			pKoma->SetAllEffect();
			pKoma->LoadAllPicture();

//			if ((overrapType > 0) && (overrapTime > 0))
			if (overrapTime > 0)
			{
				CAreaControl::SetNextAllPrint();
				m_effect->BeginEffect();
				m_effect->CalcuOnly();
				m_effect->Print(FALSE,TRUE);
				m_effect->EndEffect();
				m_overrapPic2->GetScreen(0,0,screenSizeX,screenSizeY);

				for (int p=0;p<overrapTime;p++)
				{
					CAreaControl::SetNextAllPrint();
					m_effect->PrintSimpleWipe(overrapType,m_overrapPic,m_overrapPic2,p,overrapTime);
					m_aviSave->SetData();
				}
			}

			int mesKosuu = pKoma->GetObjectKosuu();
			if (mesKosuu>0)
			{
				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(0));
				if (pMessage != NULL)
				{
					int md = pMessage->GetMessageMode();
					if (md == 3)
					{
						int drawCount = pMessage->GetDrawCount();

						for (int dr=0;dr<drawCount;dr++)
						{
							CAreaControl::SetNextAllPrint();
							m_effect->BeginEffect();
							m_effect->CalcuOnly();
							m_effect->Print(FALSE,TRUE);
							m_effect->EndEffect();
							m_aviSave->SetData();
							m_effect->CountUp(1);
						}
					}
				}
			}
		}
	}
	m_aviSave->CloseAviFile();
	MessageBox(NULL,"ムービー処理おわりにゃ","OK",MB_OK);
}

BOOL CALLBACK CMovieCreate::MyProceedDialogProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_staticText = GetDlgItem(hWnd,IDC_MYSTATIC);
		m_progressBar = GetDlgItem(hWnd,IDC_PROGRESS1);
		m_progressBar2 = GetDlgItem(hWnd,IDC_PROGRESS2);

	//	EnableWindow(m_progressBar,TRUE);
	//	SendMessage(m_progressBar,PBM_SETRANGE,0,MAKELPARAM(0,100));
//		SendMessage(m_progressBar,PBM_SETPOS,70,0);

	//	SetWindowText(m_staticText,"test666");

	//	HWND aaa;
	//	aaa = GetDlgItem(hWnd,IDC_EDIT1);
	//	SetWindowText(aaa,"testdayo");


		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		HWND hwnd0;
		hwnd0 = GetDesktopWindow();

		HDC hdc0;
		hdc0 = GetDC(hwnd0);

		int wx,wy;
		wx = GetDeviceCaps(hdc0,HORZRES);
		wy = GetDeviceCaps(hdc0,VERTRES);
		ReleaseDC(hwnd0,hdc0);

		MoveWindow(hWnd,(wx-sx)/2,(wy-sy)/2,sx,sy,TRUE);

		return TRUE;
		break;
	case WM_CLOSE:
		EndDialog(hWnd,1);

		break;
	}


	return 0;
}


BOOL CMovieCreate::CreateProceedDialog(void)
{
	m_proceedHWND = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_DIALOG_PROG),NULL,MyProceedDialogProc);
	ShowWindow(m_proceedHWND,SW_SHOW);
	return TRUE;
}


void CMovieCreate::SetProceedMessage(LPSTR mes)
{
	SetWindowText(m_staticText,mes);
	UpdateWindow(m_staticText);
}


void CMovieCreate::SetProceedBar(int pos,int n)
{
	HWND hwnd = m_progressBar;
	if (n == 1) hwnd = m_progressBar2;

	SendMessage(hwnd,PBM_SETPOS,pos,0);
}


BOOL CMovieCreate::DestroyProceedDialog(void)
{
	DestroyWindow(m_proceedHWND);
	return TRUE;
}


/*_*/



