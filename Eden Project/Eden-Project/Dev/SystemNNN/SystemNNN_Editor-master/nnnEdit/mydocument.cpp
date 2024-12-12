//
// mydocument.cpp
//

#include <windows.h>
#include <stdio.h>

#include <commctrl.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\myGraphics.h"
//#include "..\nyanLib\include\picture.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "case.h"
#include "messageData.h"

#include "myapplicationBase.h"

//#include "..\cyclib\effect.h"
#include "case.h"

#include "myframewindow.h"
#include "myclientwindow.h"

#include "myview.h"
#include "scrollView.h"

#include "mydocument.h"

#include "messagedata.h"
#include "komadata.h"
#include "filmdata.h"

#include "commanddata.h"
#include "storydata.h"

#include "windowList.h"

CMyDocument::CMyDocument(CMyApplicationBase* application)
//						 HWND hWndFrame, HWND hWndClient, HINSTANCE hInstance)
{
	m_app = application;

	m_windowNumber = -1;

	m_viewHWND = NULL;
	m_view = NULL;

	m_hInstance = m_app->GetMyInstance();
	m_frame = m_app->GetMyFrame();
	m_client = m_app->GetMyClient();

	m_frameHWND = m_app->GetFrameHWND();
	m_clientHWND = m_app->GetClientHWND();

//	m_view = new CMyView(this,m_clientHWND,m_hInstance);
}

CMyDocument::~CMyDocument()
{
	End();
}

void CMyDocument::End(void)
{
	ENDDELETECLASS(m_view);

}

//BOOL CMyDocument::Show(BOOL bFlag)
//{
//	/return m_view->Show(bFlag);
//}


void CMyDocument::OnCloseButton(void)
{
//	OutputDebugString("CLOSE BUTTON (MyDocument)\n");

	m_app->HideWindow(m_windowNumber);
}


CMyApplicationBase* CMyDocument::GetApp(void)
{
	return m_app;
}

CMyView* CMyDocument::GetView(void)
{
	return m_view;
}

void CMyDocument::MyInvalidateRect(RECT* lpRect, BOOL bErase)
{
	if (m_view == NULL) return;

	m_view->MyInvalidateRect(lpRect,bErase);
}


void CMyDocument::UpdateMyWindow(int windowNumber)
{
	if (windowNumber == -1) windowNumber = m_windowNumber;
	m_app->UpdateMyWindow(windowNumber);
}

HBITMAP CMyDocument::GetCommonBitmap(int n)
{
	return m_app->GetCommonBitmap(n);
}


CFilmData* CMyDocument::GetNowSelectFilm(void)
{
	return m_app->GetNowSelectFilm();
}


CKomaData* CMyDocument::GetNowSelectKoma(void)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return NULL;

	return (CKomaData*)( pFilm->GetNowSelectObject());
}


CMessageData* CMyDocument::GetNowSelectMessage(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return NULL;

	return (CMessageData*)(pKoma->GetNowSelectObject());
}



CStoryData* CMyDocument::GetNowSelectStory(void)
{
	return m_app->GetNowSelectStory();
}



CCommandData* CMyDocument::GetNowSelectCommand(void)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return NULL;

	return (CCommandData*)(pStory->GetNowSelectObject());
}

void CMyDocument::SetWindowName(LPSTR name)
{
	if (m_view != NULL)
	{
		m_view->SetWindowName(name);
	}
}


void CMyDocument::WindowIsMoved(int x, int y)
{
	m_app->WindowIsMoved(m_windowNumber,x,y);
}


void CMyDocument::WindowSizeIsChanged(int sizeX,int sizeY)
{
	m_app->WindowSizeIsChanged(m_windowNumber,sizeX,sizeY);
}


HGDIOBJ CMyDocument::SelObjOnly(HDC hdc, HGDIOBJ obj)
{
	return m_app->SelObjOnly(hdc,obj);
}

void CMyDocument::GetInitWindowZahyo(LPSTR windowName,RECT* lpRect, BOOL* lpFlag)
{
	m_app->GetInitWindowZahyo(windowName, m_windowNumber,lpRect,lpFlag);
}


int CMyDocument::GetScrollObjectKosuu(int vh)
{
	return 0;
}

int CMyDocument::GetNowSelectNumber(void)
{
	return -1;
}

SIZE CMyDocument::GetDesktopSize(void)
{
	return m_app->GetDesktopSize();
}


BOOL CMyDocument::DeleteChildObject(int n,int sub)
{
	return FALSE;
}


void CMyDocument::PreClickMouse(int windowNumber)
{
	m_app->PreClickMouse(windowNumber);
}

CCase* CMyDocument::GetChildObject(int n)
{
	return NULL;
}


int CMyDocument::GetNowSelectPictureLayer(void)
{
	return m_app->GetNowSelectPictureLayer();
}

int CMyDocument::GetNowSelectEffectLayer(void)
{
	return m_app->GetNowSelectEffectLayer();
}

int CMyDocument::GetBlockKosuu(void)
{
	if (m_view == NULL) return 1;

	return m_view->GetBlockKosuu();
}

void CMyDocument::SelectPrevObject(int prev)
{
	CCase* pCase = GetNowSelectCaseObject();
	if (pCase == NULL) return;

	int kosuu = pCase->GetObjectKosuu();
	int sel = pCase->GetNowSelectNumber();
	sel += prev;
	if (sel<0) sel = 0;
//	if (sel > 0) sel--;
	pCase->SetSelectNumber(sel);
	pCase->SetSelectSubNumber(-1);
	OnSelectNumber(sel);
}


void CMyDocument::SelectNextObject(int next)
{
	CCase* pCase = GetNowSelectCaseObject();
	if (pCase == NULL) return;

	int kosuu = pCase->GetObjectKosuu();
	int sel = pCase->GetNowSelectNumber();
	sel += next;
	if (sel > kosuu) sel = kosuu;
//	if (sel < kosuu) sel++;
	pCase->SetSelectNumber(sel);
	pCase->SetSelectSubNumber(-1);
	OnSelectNumber(sel);
}

void CMyDocument::OnTabKey(int shift)
{
	m_app->OnTabKey(m_windowNumber, shift);
}

HWND CMyDocument::GetViewHWND(void)
{
	if (m_view == NULL) return NULL;
	return m_view->GetHWND();
}

void CMyDocument::OnFunctionKey(int n)
{
	if (n == 1)
	{
		m_app->ChangeWindowIfCan(MAINSCREEN_WINDOW);
	}

	if (n == 2)
	{
		m_app->ChangeWindowIfCan(LAYER_WINDOW);
	}

	if (n == 4)
	{
		m_app->ChangeWindowIfCan(CONTROL_WINDOW);
	}

	if (n == 5)
	{
		m_app->ChangeWindowIfCan(FILMCASE_WINDOW);
	}

	if (n == 6)
	{
		m_app->ChangeWindowIfCan(FILM_WINDOW);
	}

	if (n == 7)
	{
		m_app->ChangeWindowIfCan(GAMEMESSAGE_WINDOW);
	}

	if (n == 9)
	{
		m_app->ChangeWindowIfCan(STORYBOOK_WINDOW);
	}

	if (n == 11)
	{
		m_app->ChangeWindowIfCan(STORY_WINDOW);
	}

	if (n == 12)
	{
		m_app->ChangeWindowIfCan(VAR_WINDOW);
	}

}


/*
HWND CMyDocument::GetMyFrameHWND(void)
{
	return m_app->GetFrameHWND();
}

HINSTANCE CMyDocument::GetMyInstance(void)
{

}
*/

//int CMyDocument::GetNowSelectLayer(void)
//{
//	return 0;
//	return m_app->GetNowSelectLayer();
//}




//HWND CMyDocument::GetClientHWND(void)
//{
//	return m_clientHWND;
//}

//HINSTANCE CMyDocument::GetMyInstance(void)
//{
//	return m_hInstance;
//}
/*_*/

