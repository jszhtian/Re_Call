//
// Contedoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"


#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
//#include "..\nyanLib\include\effect.h"


#include "windowlist.h"

#include "case.h"

#include "Conteview.h"


#include "Contedoc.h"

#include "myapplicationBase.h"

#include "filmdata.h"
#include "komadata.h"
#include "MessageData.h"

#include "myfileopen.h"



CConteDoc::CConteDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = CONTE_WINDOW;
	m_view = new CConteView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();
}



CConteDoc::~CConteDoc()
{
	End();
}

void CConteDoc::End(void)
{
}


//HBITMAP CConteDoc::GetPicture(int n)
//{
//	return m_app->GetFilmLargePic(n);
//}

HBITMAP CConteDoc::GetHBitmap(int placeNumber)
{
	return m_app->GetFilmLargePic(placeNumber);
}


int CConteDoc::GetScrollObjectKosuu(int vh)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 1;

	return pFilm->GetObjectKosuu() + 1;
	return 0;
}


int CConteDoc::GetNowSelectNumber(void)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return -1;

	return pFilm->GetNowSelectNumber();

	return -1;
}


CFilmData* CConteDoc::GetNowSelectFilm(void)
{
	return m_app->GetNowSelectFilm();
}


void CConteDoc::OnClickFilm(int n)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm != NULL)
	{
		int kosuu = pFilm->GetObjectKosuu();
		if ((n>=0) && (n<=kosuu))
		{
			pFilm->SetSelectNumber(n);
			m_app->KomaIsChanged();
		}
	}
}


void CConteDoc::ReCalcuScrollPara(void)
{
	((CConteView*)m_view)->ReCalcuScrollPara();
}


CCase* CConteDoc::GetNowSelectCaseObject(void)
{
	return GetNowSelectFilm();
}

void CConteDoc::OnSelectNumber(int n)
{
	m_app->KomaIsChanged();
}


/*_*/



