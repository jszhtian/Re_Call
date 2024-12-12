//
// controldoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "windowlist.h"

#include "case.h"

//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "controlview.h"

#include "controldoc.h"

#include "myapplicationBase.h"

#include "filmdata.h"

#include "komadata.h"

#include "undoMemoryObject.h"



CControlDoc::CControlDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = CONTROL_WINDOW;

	m_view = new CControlView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();
//	m_view->SetViewWindowText("Control");
//	m_view->MoveViewWindow(5,681,136,128);
}



CControlDoc::~CControlDoc()
{
	End();
}

void CControlDoc::End(void)
{
}

//void CControlDoc::OnCloseButton(void)
//{
//
//}


void CControlDoc::OnControlButton(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	CKomaData* pKoma = NULL;
	if (pFilm != NULL) pKoma = (CKomaData*)(pFilm->GetNowSelectObject());

	CEffect* effect = m_app->GetEffect();

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		if (undo != NULL)
		{
			undo->Clear();
		}
	}


	switch (n)
	{
	case 0:	//PLAY
		if (pKoma != NULL)
		{


			m_app->SetKomaMode(FALSE);

			if (m_app->GetPlayMode() == FALSE)
			{
				m_app->SetPlayMode(TRUE,n);
				m_app->SetPauseFlag(FALSE);
				pKoma->SetAllEffect();
			}
			else
			{
				if (m_app->CheckPauseMode())
				{
					m_app->SetPauseFlag(FALSE);
				}
				else
				{
					//clear count

				}
			}

			m_app->SetPlayMode(TRUE,n);
			

		}
		break;
	case 1:	//STOP
		if (pKoma != NULL)
		{
			//effect‚Ìcount=0


			m_app->StopAllMusicAndSound();
			m_app->SetPlayMode(FALSE,n);
		}
		break;
	case 2:	//PAUSE or >>|
		if (pKoma != NULL)
		{
			if (m_app->CheckPlayMode())
			{
				m_app->SetPlayMode(FALSE,n);
			}
			else
			{
				m_app->SetPlayMode(TRUE,n);
			}
		}
		break;
	case 3:	// |<<
		if (pKoma != NULL)
		{
			//effect‚Ìcount=0
			m_app->MoveFilmTop();
//			m_app->SetPlayMode(FALSE,n);
		}
		break;
//	case 4:	// >|
//		if (pKoma != NULL)
//		{
//			//1count‚·‚·‚ß‚Ä•\Ž¦
//
//			m_app->SetPlayMode(TRUE,n,FALSE);
//			m_app->SetPauseFlag();
//			m_app->CountUp();
//		}
//		break;
	case 4:	// story
		if (pKoma != NULL)
		{
			m_app->SetPlayMode(FALSE);
			m_app->ChangeFilmPlayMode();

//			m_app->SetPlayMode(TRUE,n);
			m_app->SetKomaMode();
		}
		break;
	}

	m_view->MyInvalidateRect();

//	m_app->FilmIsChanged();

}


int CControlDoc::GetPlayCommand(void)
{
	if (m_app->CheckPlayMode() == FALSE) return 1;	//stop
	return m_app->GetPlayCommand();
}


int CControlDoc::GetFilmPlayMode(void)
{
	return m_app->GetFilmPlayMode();
}
/*_*/

