//
// controlview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "mydirectx.h"

#include "myBitmap.h"

#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"


#include "controldoc.h"
#include "controlview.h"


int CControlView::m_buttonTable[]=
{
	96,48,40,40,	//play
	48,0,40,40,		//stop
	48,48,40,40,	//pause
	0,48,40,40,		//|<<
	8,96,24,24,		//STORY

	//old2
	72,48,64,40,	//PLAY
	0,48,64,40,		//STOP
	48,0,40,40,		//PAUSE

	0,0,40,40,		// |<<
	96,0,40,40,	// >|

	8,96,24,24,		//STORY


//old

	72,0, 40,24,	//PLAY
	24,0,24,24,	//STOP
	48,0,24,24,	//PAUSE
	0,0,24,24,	//REV
	112,0,24,24,	//FF

	0,24,24,24,	// |<<
	112,24,24,24,	// >>|
	56,24,24,24,	// >|>
	24,24,24,24,	// |<
	84,24,24,24,	// >|

	0,48,32,24,	// |->

	52,72,32,24,	// :>
	0,72,24,24,	// :<<
	112,72,24,24,	// >>:
	24,72,24,24,	// :<
	90,72,24,24,	// >:

	0,96,32,24,	// :->
};

CControlView::CControlView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("controlWindow");

//	SetResizeFlag();
	SetBlockSize(136,128);
	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
//	SetNonBlockArea(0,24,0,0);
//	SetFreeSizeFlag(1);
//	SetScrollFlag(TRUE,FALSE);

	SetWindowStyle();

	SetViewWindowText("Control");

//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_buttonBitmap = new CMyBitmap("nnndir\\setup\\bmp\\control.bmp");
	m_buttonBitmap = new CMyBitmap("nnndir\\setup\\bmp\\filmPlayerButton.bmp");
	//m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_CONTROL));
	m_hBitmap = m_buttonBitmap->GetHBitmap();
}

CControlView::~CControlView()
{
	End();
}

void CControlView::End(void)
{
//	DELETEGDIOBJECT(m_hBitmap);
	ENDDELETECLASS(m_buttonBitmap);
}



LRESULT CControlView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	int x,y;
	CControlDoc* pDoc = (CControlDoc*)m_document;

	switch (message)
	{
	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;
	case WM_LBUTTONDOWN:
		CheckButton(LOWORD(lParam),HIWORD(lParam));
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);

//		RECT tmpRect;
//		RECT rc;

		//ps.rcPaint‚Æ‚­‚ç‚×‚Ä‚©‚«‚È‚¨‚· IntersectRect IntersectClipRect


		HDC hdc = ps.hdc;
		HDC src = CreateCompatibleDC(hdc);
		
//		HGDIOBJ old;

		HGDIOBJ old;
		old = SelectObject(src,m_hBitmap);


		int i;
		int sel;
		sel = pDoc->GetPlayCommand();
//		for (i=0;i<17;i++)
		for (i=0;i<5;i++)
		{
			int px = m_buttonTable[i*4+0];
			int py = m_buttonTable[i*4+1];
			int sx = m_buttonTable[i*4+2];
			int sy = m_buttonTable[i*4+3];
			int srcX = px;
			int srcY = py;

			
			if (i == sel) srcX += 136;
			if (i == 4)
			{
				if (pDoc->GetFilmPlayMode())
				{
					srcX += 136;
				}
			}

			BitBlt(hdc,px,py,sx,sy,src,srcX,srcY,SRCCOPY);
		}

//		BitBlt(hdc,24,24,24,24,src,0,0,SRCCOPY);
//		BitBlt(hdc,48,24,24,24,src,0,24,SRCCOPY);
//		BitBlt(hdc,72,24,24,24,src,0,48,SRCCOPY);


		SelectObject(src,old);

		DeleteDC(src);

		EndPaint(hWnd,&ps);

		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}

void CControlView::CheckButton(int x, int y)
{
	for (int i=0;i<5;i++)
	{
		int xx = x - m_buttonTable[i*4+0];
		int yy = y - m_buttonTable[i*4+1];
		if ((xx>=0) && (xx<m_buttonTable[i*4+2]) && (yy>=0) && (yy<m_buttonTable[i*4+3]))
		{
			CControlDoc* pDoc = (CControlDoc*)m_document;
			pDoc->OnControlButton(i);
			return;
		}
	}
}

void CControlView::OnLeftKey(void)
{
	CControlDoc* pDoc = (CControlDoc*)m_document;
	pDoc->OnControlButton(3);
}

void CControlView::OnRightKey(void)
{
	CControlDoc* pDoc = (CControlDoc*)m_document;
	pDoc->OnControlButton(0);
}

void CControlView::OnUpKey(void)
{
	CControlDoc* pDoc = (CControlDoc*)m_document;
	pDoc->OnControlButton(1);
}

void CControlView::OnDownKey(void)
{
	CControlDoc* pDoc = (CControlDoc*)m_document;
	pDoc->OnControlButton(2);
}

BOOL CControlView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 



	int subType = -1;
	int type = CONTROL_WINDOW;


	for (int i=0;i<5;i++)
	{
		int xx = x - m_buttonTable[i*4+0];
		int yy = y - m_buttonTable[i*4+1];
		if ((xx>=0) && (xx<m_buttonTable[i*4+2]) && (yy>=0) && (yy<m_buttonTable[i*4+3]))
		{
			subType = i;
			break;
		}
	}


	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

//	OutputDebugString("*moveMouse layer*\x00f\x00a");
	return FALSE;
}

/*_*/



