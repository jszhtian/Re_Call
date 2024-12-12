#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

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

#include "layerDoc.h"
#include "conteDoc.h"

#include "zahyoDoc.h"
#include "varDoc.h"

#include "effectDoc.h"

#include "storyBookDoc.h"
#include "storyDoc.h"

#include "windowUpdateControl.h"

#include "windowlist.h"



CWindowUpdateControl::CWindowUpdateControl(CMyApplicationBase* app)
{
	m_app = app;
}

CWindowUpdateControl::~CWindowUpdateControl()
{
	End();
}

void CWindowUpdateControl::End(void)
{
}

void CWindowUpdateControl::MyUpdateWindow(int windowNumber)
{

}


void CWindowUpdateControl::FilmIsChanged(void)
{
//	CFilmDoc* pFilm = (CFilmDoc*)m_document[FILM_WINDOW];
	CFilmDoc* pFilm = (CFilmDoc*)(m_app->GetMyDocument(FILM_WINDOW));
	if (pFilm != NULL)
	{
		pFilm->InitMiniPicTable();
		pFilm->ReCalcuScrollPara();
	}

	CFilmData* pFilmData = m_app->GetNowSelectFilm();
	if (pFilmData != NULL)
	{
		char mes[256];
		LPSTR name = pFilmData->GetMyName();
		wsprintf(mes,"フィルム-[%s]",name);
		m_app->SetWindowName(FILM_WINDOW,mes);
	}
	else
	{
		m_app->SetWindowName(FILM_WINDOW,"フィルム");
	}


//	CConteDoc* pDoc0 = (CConteDoc*)m_document[CONTE_WINDOW];
	CConteDoc* pDoc0 = (CConteDoc*)(m_app->GetMyDocument(CONTE_WINDOW));
	if (pDoc0 != NULL)
	{
		pDoc0->ReCalcuScrollPara();
		pDoc0->UpdateMyWindow();
	}


//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)(m_app->GetMyDocument(FILMCASE_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();

	KomaIsChanged();
}


void CWindowUpdateControl::KomaIsChanged(void)
{
//	CFilmDoc* pDoc = (CFilmDoc*)m_document[FILM_WINDOW];
	CFilmDoc* pDoc = (CFilmDoc*)(m_app->GetMyDocument(FILM_WINDOW));
	if (pDoc != NULL)
	{
		pDoc->ReCalcuScrollPara();
		pDoc->UpdateMyWindow();
	}

	CKomaData* pKoma = m_app->GetNowSelectKoma();
	LPSTR komaName = NULL;
	if (pKoma != NULL)
	{
		komaName = pKoma->GetKomaName();
	}


//	CGameMessageDoc* pDoc2 = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	CGameMessageDoc* pDoc2 = (CGameMessageDoc*)(m_app->GetMyDocument(GAMEMESSAGE_WINDOW));
	if (pDoc2 != NULL)
	{
		if (komaName != NULL)
		{
			char mes[256];
			wsprintf(mes,"コマ-[%s]",komaName);
			m_app->SetWindowName(GAMEMESSAGE_WINDOW,mes);
		}
		else
		{
			m_app->SetWindowName(GAMEMESSAGE_WINDOW,"コマ");
		}
			
		pDoc2->ReCalcuScrollPara();
	}


//	CConteDoc* pDoc3 = (CConteDoc*)m_document[CONTE_WINDOW];
	CConteDoc* pDoc3 = (CConteDoc*)(m_app->GetMyDocument(CONTE_WINDOW));
	if (pDoc3 != NULL)
	{
		pDoc3->ReCalcuScrollPara();
		pDoc3->UpdateMyWindow();
	}

	MessageIsChanged();
	LayerIsChanged();
//	EffectIsChanged();
}



void CWindowUpdateControl::MessageIsChanged(void)
{
//	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document[GAMEMESSAGE_WINDOW];
	CGameMessageDoc* pDoc = (CGameMessageDoc*)(m_app->GetMyDocument(GAMEMESSAGE_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
	
	ConteMessageIsChanged();
}

void CWindowUpdateControl::LayerIsChanged(void)
{
//	CLayerDoc* pDoc = (CLayerDoc*)m_document[LAYER_WINDOW];
	CLayerDoc* pDoc = (CLayerDoc*)(m_app->GetMyDocument(LAYER_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
	EffectIsChanged();
	ZahyoIsChanged();
}

void CWindowUpdateControl::EffectIsChanged(void)
{
//	CEffectDoc* pDoc = (CEffectDoc*)m_document[EFFECT_WINDOW];
	CEffectDoc* pDoc = (CEffectDoc*)(m_app->GetMyDocument(EFFECT_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
	m_app->UpdateMainScreen();
}


void CWindowUpdateControl::ZahyoIsChanged(void)
{
//	CZahyoDoc* pDoc = (CZahyoDoc*)m_document[ZAHYO_WINDOW];
	CZahyoDoc* pDoc = (CZahyoDoc*)(m_app->GetMyDocument(ZAHYO_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
}



void CWindowUpdateControl::StoryIsChanged(void)
{
//	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	CStoryBookDoc* pDoc = (CStoryBookDoc*)(m_app->GetMyDocument(STORYBOOK_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();

//	CStoryDoc* pStory = (CStoryDoc*)m_document[STORY_WINDOW];
	CStoryDoc* pStory = (CStoryDoc*)(m_app->GetMyDocument(STORY_WINDOW));
	pStory->ReCalcuScrollPara();

	CStoryData* pStoryData = m_app->GetNowSelectStory();
	if (pStoryData != NULL)
	{
		char mes[256];
		LPSTR name = pStoryData->GetMyName();
		wsprintf(mes,"シナリオ-[%s]",name);
		m_app->SetWindowName(STORY_WINDOW,mes);
	}
	else
	{
		m_app->SetWindowName(STORY_WINDOW,"シナリオ");
	}


	CommandIsChanged();
}

void CWindowUpdateControl::CommandIsChanged(void)
{
//	CStoryDoc* pDoc = (CStoryDoc*)m_document[STORY_WINDOW];
	CStoryDoc* pDoc = (CStoryDoc*)(m_app->GetMyDocument(STORY_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();

	ProgramIsChanged();
}


void CWindowUpdateControl::ProgramIsChanged(void)
{
//	CProgramDoc* pDoc = (CProgramDoc*)m_document[PROGRAM_WINDOW];
	CProgramDoc* pDoc = (CProgramDoc*)(m_app->GetMyDocument(PROGRAM_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
}

void CWindowUpdateControl::VarIsChanged(void)
{
//	CVarDoc* pDoc = (CVarDoc*)m_document[VAR_WINDOW];
	CVarDoc* pDoc = (CVarDoc*)(m_app->GetMyDocument(VAR_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
}

void CWindowUpdateControl::ConteIsChanged(void)
{
//	CConteDoc* pDoc = (CConteDoc*)m_document[CONTE_WINDOW];
	CConteDoc* pDoc = (CConteDoc*)(m_app->GetMyDocument(CONTE_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
}

void CWindowUpdateControl::ConteMessageIsChanged(void)
{
//	CConteMessageDoc* pDoc = (CConteMessageDoc*)m_document[CONTEMESSAGE_WINDOW];
	CConteMessageDoc* pDoc = (CConteMessageDoc*)(m_app->GetMyDocument(CONTEMESSAGE_WINDOW));
	if (pDoc != NULL) pDoc->UpdateMyWindow();
}

/*_*/


