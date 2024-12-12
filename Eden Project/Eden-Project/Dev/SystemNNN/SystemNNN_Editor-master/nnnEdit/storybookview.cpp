//
// storybookview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\wheelMouse.h"

#include "bitmapnumber.h"
#include "mybutton.h"

#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"


#include "storybookdoc.h"
#include "storybookview.h"

#include "storybookdata.h"

#include "storydata.h"



CStoryBookView::CStoryBookView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("storyBookWindow");

	SetResizeFlag();
	SetBlockSize(480,24);
	SetNonBlockArea(0,24,0,0);

	SetBlockKosuuMin(1,2);
	SetBlockKosuuMax(1,64);
	SetFreeSizeFlag(1);
	SetScrollFlag(FALSE,TRUE);

	SetWindowStyle();

	SetViewWindowText("シナリオ集");

//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_VSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_scrollFlag[0] = TRUE;
	SetScrollPara(0,4,5);
	SetScrollArea(0,24,480,24+24*5);
//	SetBlockSize(480,24);

//@@@	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_STORYBOOK));

	m_buttonNew = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NEW),0,0);
	m_buttonOpen = new CMyButton(1,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_OPEN),24,0);
	m_buttonSave = new CMyButton(2,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SAVE),48,0);

	m_buttonDelete = new CMyButton(3,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_DELETE),80,0);

	m_buttonUndo = new CMyButton(4,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_UNDO),112,0);
	
	m_buttonSearch = new CMyButton(5,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SEARCH),112+48,0);

	AddBalloonCheckButton(m_buttonNew);
	AddBalloonCheckButton(m_buttonOpen);
	AddBalloonCheckButton(m_buttonSave);
	AddBalloonCheckButton(m_buttonDelete);
	AddBalloonCheckButton(m_buttonUndo);
	AddBalloonCheckButton(m_buttonSearch);

	ReCalcuScrollPara();
}


CStoryBookView::~CStoryBookView()
{
	End();
}

void CStoryBookView::End(void)
{
	ENDDELETECLASS(m_buttonNew);
	ENDDELETECLASS(m_buttonOpen);
	ENDDELETECLASS(m_buttonSave);
	ENDDELETECLASS(m_buttonDelete);
	ENDDELETECLASS(m_buttonUndo);
	ENDDELETECLASS(m_buttonSearch);
}

/*
void CStoryBookView::ReCalcuScrollPara(void)
{
	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document;
	CStoryBookData* pBook = pDoc->GetStoryBookData();

	int kosuu = pDoc->GetStoryKosuu();
	int block = GetBlockKosuu();

	int pgMax = (kosuu + 1) - 1;
	int pos = GetMyScrollPos();
	if (pgMax<block-1) pgMax = block-1;
	int n = pDoc->GetNowSelectNumber();
	if (n != -1)
	{
		pos = n-(block-1);
		if (pos<0) pos = 0;
	}
	SetScrollPara(pos,pgMax);

}
*/


LRESULT CStoryBookView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	int x,y;
	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document;
	int n;

	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_USER_LBUTTONDOWN:
			switch (lParam)
			{
			case 0:
				pDoc->OnNewStory();
				break;
			case 1:
				pDoc->OnOpenStory();
				break;
			case 2:
				pDoc->OnSaveStory();
				break;
			case 3:
				pDoc->OnDelete();
				break;
			case 4:
				pDoc->OnUndo();
				break;
			case 5:
				pDoc->OnSearch();
				break;
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		if (m_buttonNew != NULL) m_buttonNew->CalcuLButtonDown(wParam,lParam);
		if (m_buttonOpen != NULL) m_buttonOpen->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSave != NULL) m_buttonSave->CalcuLButtonDown(wParam,lParam);
		if (m_buttonDelete != NULL) m_buttonDelete->CalcuLButtonDown(wParam,lParam);
		if (m_buttonUndo != NULL) m_buttonUndo->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSearch != NULL) m_buttonSearch->CalcuLButtonDown(wParam,lParam);

		int x,y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		n = GetContentsNumber(x,y);

//		if ((y>=24) && (y<24*6))
		if (n != -1)
		{
//			y -= 24;
//			y /= 24;
//			int pg = GetMyScrollPos();
			pDoc->OnSelectStory(n);
		}
		break;

	case WM_RBUTTONDOWN:
		int cmd;
		cmd = OpenPopupMenu(wParam,lParam);
//@@@		if (cmd == ID_MENU_STORYBOOK_CHANGENAME)
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			n = GetContentsNumber(x,y);
//			int y = HIWORD(lParam);
//			if ((y>=24) && (y<24*6))
			if (n != -1)
			{
//				int pos = GetMyScrollPos();
				pDoc->OnChangeName(n);
			}
		}
		break;

	case WM_MOUSEWHEEL:
		OnWheelScroll(message,wParam,lParam);
		return TRUE;
		break;

	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;
	case WM_VSCROLL:
		OnScroll(message,wParam,lParam);
		break;
	case WM_ERASEBKGND:
		EraseSplit((HDC)wParam);
		return TRUE;
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);

//		RECT tmpRect;
//		RECT rc;

		//ps.rcPaintとくらべてかきなおす IntersectRect IntersectClipRect


		HDC hdc = ps.hdc;
		HDC src = CreateCompatibleDC(hdc);

		if (m_buttonNew != NULL) m_buttonNew->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonOpen != NULL) m_buttonOpen->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonSave != NULL) m_buttonSave->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonDelete != NULL) m_buttonDelete->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonUndo != NULL)
		{
			int umd = 2;
			if (pDoc->CheckExistUndo()) umd = 0;
			m_buttonUndo->Print(hdc,src,umd,&ps.rcPaint);
		}
		if (m_buttonSearch != NULL) m_buttonSearch->Print(hdc,src,0,&ps.rcPaint);

//		HGDIOBJ old;


		int kosuu = pDoc->GetStoryKosuu();
		int page = GetMyScrollPos();
		int block = GetBlockKosuu();
		int nm = pDoc->GetNowSelectNumber();

		int i;
		for (i=0;i<block;i++)
		{
			int k = page + i;
			if (k<kosuu)
			{
				CStoryData* pStory = pDoc->GetStory(k);
				if (pStory == NULL)
				{
					TextOut(hdc,0,24+4+i*24,"_エラー",strlen("_エラー"));
				}
				else
				{
//					m_dib->SetPic(pFilm->GetMiniPic());
//					m_dib->Put(0,24+i*24,hdc,src);
					LPSTR mes = pStory->GetMyName();
					TextOut(hdc,0,24+4+i*24,mes,strlen(mes));
				}
			}
			else if (k == kosuu)
			{
				TextOut(hdc,0,24+4+i*24,"_新規",strlen("_新規"));
			}
			else
			{
				TextOut(hdc,0,24+4+i*24,"-----",strlen("-----"));
			}
			
			if (k == nm)
			{
				RECT rc;
				GetWindowRect(hWnd,&rc);
//				HGDIOBJ old99 = SelectObject(hdc,GetStockObject(WHITE_BRUSH));
				PatBlt(hdc,1,24+i*24,rc.right-rc.left-2,24,DSTINVERT);
//				SelectObject(hdc,old99);
			}
		}





		DeleteDC(src);

		EndPaint(hWnd,&ps);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


BOOL CStoryBookView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = STORYBOOK_WINDOW;
	int subType = CheckOnBalloonButton(x,y);

	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}


BOOL CStoryBookView::OnControlKey(WPARAM wParam,LPARAM lParam)
{
	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document;
	if (wParam == 6)
	{
		//F
		pDoc->OnSearch();
		return TRUE;
	}
	if (wParam == 19)
	{
		//S
		pDoc->OnSaveStory();
		return TRUE;
	}
	if (wParam == 3)
	{
		//C
//		pDoc->OnCopyS();
//		return TRUE;
	}
	if (wParam == 24)
	{
		//X
//		pDoc->OnCut();
//		return TRUE;
	}
	if (wParam == 22)
	{
		//V
//		pDoc->OnPaste();
//		return TRUE;
	}
	if (wParam == 14)
	{
		//N
		pDoc->OnNewStory();
		return TRUE;
	}
	if (wParam == 15)
	{
		pDoc->OnOpenStory();
		return TRUE;
		//O
	}
	if (wParam == 26)
	{
		return pDoc->OnUndo(); 
	}

	return FALSE;

}

/*_*/



