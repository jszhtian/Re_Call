//
// scrollview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "case.h"

#include "myView.h"
#include "scrollview.h"
#include "mydocument.h"

//#include "..\cyclib\wheelmouse.h"


CMyScrollView::CMyScrollView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument, clientHWND, hinstance)
{
	m_scrollFlag[0] = FALSE;
	m_scrollFlag[1] = FALSE;

	m_scrollPos[0] = 0;
	m_scrollPos[1] = 0;
}


CMyScrollView::~CMyScrollView()
{
	End();
}

void CMyScrollView::End(void)
{
}


void CMyScrollView::SetScrollArea(int startX,int startY,int endX,int endY)
{
	m_scrollAreaStartX = startX;
	m_scrollAreaStartY = startY;
	m_scrollAreaEndX = endX;
	m_scrollAreaEndY = endY;
}


void CMyScrollView::SetScrollPara(int pos, int posMax, int page,int vh)
{
	if (vh == -1)
	{
		if (m_scrollFlag[1]) vh = 1;
		if (m_scrollFlag[0]) vh = 0;
	}

	if ((vh != 0) && (vh != 1)) return;

	if (m_scrollFlag[vh] == FALSE) return;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.nMin = 0;
	si.nMax = posMax;
	si.nPage = page;
	si.nPos = pos;
	si.fMask = SIF_DISABLENOSCROLL | SIF_POS;
	if (posMax != -1) si.fMask |= SIF_RANGE;
	if (page != -1) si.fMask |= SIF_PAGE;

	int fnBar;
	if (vh == 0)
	{
		fnBar = SB_VERT;
	}
	else
	{
		fnBar = SB_HORZ;
	}

	if (m_hWnd == NULL) return;
	SetScrollInfo(m_hWnd,fnBar,&si,TRUE);
	if (page != -1)
	{
		m_scrollPage[vh] = page;
//		m_blockKosuu[vh] = page;
	}

	if (posMax != -1) m_scrollMax[vh] = posMax;
	m_scrollPos[vh] = pos;
}

void CMyScrollView::OnWheelScroll(UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT msg;
	int vh = -1;
	if (m_scrollFlag[0])
	{
		vh = 0;
		msg = WM_VSCROLL;
	}

	if (m_scrollFlag[1])
	{
		vh = 1;
		msg = WM_HSCROLL;
	}
	if (vh == -1) return;

	int wheelMove = (int)(short) HIWORD(wParam);   
//	m_wheelX = (int)(short) LOWORD(lParam);
//	m_wheelY = (int)(short) HIWORD(lParam);    



	if (wheelMove == 0) return;
	if (wheelMove > 0)
	{
		OnScroll(msg,SB_LINEUP,lParam);
	}
	else
	{
		OnScroll(msg,SB_LINEDOWN,lParam);
	}
}

void CMyScrollView::OnScroll(UINT message, WPARAM wParam, LPARAM lParam)
{
	int vh = -1;
	if (message == WM_VSCROLL) vh = 0;
	if (message == WM_HSCROLL) vh = 1;
	if (vh == -1) return;
	if (m_scrollFlag[vh] == FALSE) return;

	int delta = 0;

	switch (LOWORD(wParam))
	{
	case SB_LINEUP:
		delta = -1;
		break;
	case SB_LINEDOWN:
		delta = 1;
		break;
	case SB_PAGEUP:
		delta = -m_scrollPage[vh];
		break;
	case SB_PAGEDOWN:
		delta = m_scrollPage[vh];
		break;
	case SB_THUMBTRACK:
		delta = HIWORD(wParam) - m_scrollPos[vh];
		break;
	}

	if (delta == 0) return;

	int pos = m_scrollPos[vh] + delta;
	if (pos<0) pos = 0;

	if (pos>m_scrollMax[vh]-m_blockKosuu[vh]+1) pos = m_scrollMax[vh]-m_blockKosuu[vh]+1;
	if (pos<0) pos = 0;


	delta = pos - m_scrollPos[vh];
	if (delta == 0) return;

	m_scrollPos[vh] = pos;

	SetScrollPara(pos,-1,-1,vh);


	int dx = m_blockSizeX * delta;
	int dy = m_blockSizeY * delta;
	if (vh == 0) dx = 0;
	if (vh == 1) dy = 0;

	if (m_hWnd != NULL)
	{
		RECT rc;
		SetRect(&rc,m_scrollAreaStartX,m_scrollAreaStartY,m_scrollAreaEndX,m_scrollAreaEndY);
		//ScrollWindow(m_hWnd,-dx,-dy,&rc,&rc);
		ScrollWindowEx(m_hWnd,-dx,-dy,&rc,&rc,NULL,NULL,SW_INVALIDATE | SW_ERASE);
		UpdateWindow(m_hWnd);
	}
}

int CMyScrollView::GetMyScrollPos(int vh)
{
	if (vh == -1)
	{
		if (m_scrollFlag[1]) vh = 1;
		if (m_scrollFlag[0]) vh = 0;
	}

	if ((vh != 0) && (vh != 1)) return 0;
	if (m_scrollFlag[vh] == FALSE) return 0;

	return m_scrollPos[vh];
}

int CMyScrollView::GetBlockKosuu(int vh)
{
	if (vh == -1)
	{
		if (m_scrollFlag[1]) vh = 1;
		if (m_scrollFlag[0]) vh = 0;
	}

	if ((vh != 0) && (vh != 1)) return 0;
	if (m_scrollFlag[vh] == FALSE) return 0;

	return m_blockKosuu[vh];
}




void CMyScrollView::ReCalcuScrollPara(void)
{
	int kosuu = m_document->GetScrollObjectKosuu();

	int vh = 0;
	if (m_scrollFlag[1]) vh = 1;

	int block = 1;

	if (vh == 0)
	{
		block = (m_windowSizeY - m_topArea - m_bottomArea) / m_blockSizeY;
	}
	else
	{
		block = (m_windowSizeX - m_leftArea - m_rightArea) / m_blockSizeX;
	}

	if (block<=0) block = 1;

	if (vh == 0)
	{
		SetBlockKosuu(1,block);
	}
	else
	{
		SetBlockKosuu(block,1);
	}


	//int posMax = kosuu - 1;
	int posMax = kosuu-1;// - block;
	int pos = GetMyScrollPos();

	//‚±‚±‚Åƒ|ƒWƒVƒ‡ƒ“‚ð’²®‚·‚é
	if (block >= kosuu) pos = 0;

	int n = m_document->GetNowSelectNumber();

	if (n != -1)
	{
		if (pos>(posMax-block+1))
		{
			pos = posMax-block+1;
		}

		if (pos<0) pos = 0;
	}

//char mes[256];
//sprintf(mes,"%d %d %d %d\n",pos,posMax,block,vh);
//OutputDebugString(mes);

	SetScrollPara(pos,posMax,block,vh);
	SetScrollArea(m_leftArea, m_topArea, m_leftArea + m_windowSizeX, m_topArea + m_windowSizeY);
}


int CMyScrollView::GetContentsNumber(int x, int y)
{
	if ((x>=m_scrollAreaStartX) && (x<m_scrollAreaEndX) && (y>=m_scrollAreaStartY) && (y<m_scrollAreaEndY))
	{
		x -= m_scrollAreaStartX;
		y -= m_scrollAreaStartY;

		int nx = 0;
		int ny = 0;

		if (m_scrollFlag[0])
		{
			ny = y / m_blockSizeY;
		}

		if (m_scrollFlag[1])
		{
			nx = x / m_blockSizeX;
		}

		int pg = GetMyScrollPos();
		return pg + nx + ny;
	}

	return -1;
}

void CMyScrollView::ScrollTo(int n)
{
	int blockKosuu = GetBlockKosuu();

	int vh = 0;
	if (m_scrollFlag[1]) vh = 1;
	if (m_scrollFlag[0]) vh = 0;

	int pos = n;

	if (pos>m_scrollMax[vh]-m_blockKosuu[vh]+1) pos = m_scrollMax[vh]-m_blockKosuu[vh]+1;
	if (pos<0) pos = 0;

	SetScrollPara(pos);
	ReCalcuScrollPara();
}


void CMyScrollView::OnLeftKey(void)
{
	if (m_scrollFlag[1] == 0) return;
	m_oldSelect = m_document->GetNowSelectNumber();

	m_document->SelectPrevObject();
	CheckInWindow(WM_HSCROLL,SB_LINEUP,0);
//	OnScroll(WM_HSCROLL,SB_LINEUP,0);
}

void CMyScrollView::OnRightKey(void)
{
	if (m_scrollFlag[1] == 0) return;
	m_oldSelect = m_document->GetNowSelectNumber();

	m_document->SelectNextObject();
	CheckInWindow(WM_HSCROLL,SB_LINEDOWN,0);
//	OnScroll(WM_HSCROLL,SB_LINEDOWN,0);
}

void CMyScrollView::OnUpKey(void)
{
	if (m_scrollFlag[0] == 0) return;
	m_oldSelect = m_document->GetNowSelectNumber();

	m_document->SelectPrevObject();
	CheckInWindow(WM_VSCROLL,SB_LINEUP,0);
//	OnScroll(WM_VSCROLL,SB_LINEUP,0);
}

void CMyScrollView::OnDownKey(void)
{
	if (m_scrollFlag[0] == 0) return;
	m_oldSelect = m_document->GetNowSelectNumber();

	m_document->SelectNextObject();
	CheckInWindow(WM_VSCROLL,SB_LINEDOWN,0);
//	OnScroll(WM_VSCROLL,SB_LINEDOWN,0);
}

void CMyScrollView::OnPageUp(void)
{
	UINT msg;
	int vh = -1;
	if (m_scrollFlag[0])
	{
		vh = 0;
		msg = WM_VSCROLL;
	}

	if (m_scrollFlag[1])
	{
		vh = 1;
		msg = WM_HSCROLL;
	}
	if (vh == -1) return;

	m_oldSelect = m_document->GetNowSelectNumber();

	int delta = -m_scrollPage[vh];

	m_document->SelectPrevObject(delta);
	CheckInWindow(msg,SB_PAGEUP,0);
}

void CMyScrollView::OnPageDown(void)
{
	UINT msg;
	int vh = -1;
	if (m_scrollFlag[0])
	{
		vh = 0;
		msg = WM_VSCROLL;
	}

	if (m_scrollFlag[1])
	{
		vh = 1;
		msg = WM_HSCROLL;
	}
	if (vh == -1) return;

	m_oldSelect = m_document->GetNowSelectNumber();

	int delta = m_scrollPage[vh];

	m_document->SelectNextObject(delta);
	CheckInWindow(msg,SB_PAGEDOWN,0);
}

void CMyScrollView::CheckInWindow(UINT message, WPARAM wParam, LPARAM lParam)
{
	int pos = GetMyScrollPos();
	int kosuu = m_document->GetScrollObjectKosuu();
	int blockKosuu = GetBlockKosuu();

	int n = m_document->GetNowSelectNumber();
	
	if ((n<pos) || (n>=(pos+blockKosuu)))
	{
		int delta = n;
		if (n>=(pos+blockKosuu))
		{
			delta = n - blockKosuu + 1;
		}

		wParam = (SB_THUMBTRACK & 0xffff) | ((delta & 0xffff) << 16);
		OnScroll(message,wParam,lParam);
		InvalidateRect(m_hWnd,NULL,TRUE);
	}
	else
	{
		UpdateBlock(m_oldSelect);
		UpdateBlock(n);
	}

	UpdateWindow(m_hWnd);
}


void CMyScrollView::UpdateBlock(int n)
{
	int pos = GetMyScrollPos();

	int vh = -1;
	if (m_scrollFlag[1]) vh = 1;
	if (m_scrollFlag[0]) vh = 0;

	if (vh == -1) return;


	int sizeX = m_blockSizeX;
	int sizeY = m_blockSizeY;

	int x = m_scrollAreaStartX;
	int y = m_scrollAreaStartY;

	if (vh == 0)
	{
		y += (n-pos) * sizeY;
	}
	else
	{
		x += (n-pos) * sizeX;
	}

	RECT rc;
	SetRect(&rc,x,y,x+sizeX,y+sizeY);
	InvalidateRect(m_hWnd,&rc,TRUE);
}



/*_*/



