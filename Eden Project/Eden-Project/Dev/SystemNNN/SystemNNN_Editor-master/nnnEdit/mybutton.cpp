//
// mybutton.cpp
//

#include <windows.h>

#include "mybutton.h"

CMyButton::CMyButton(int n, HWND parentHWND,HBITMAP hbitmap, int printX, int printY, int sizeX, int sizeY)
{
	m_number = n;
	m_parentHWND = parentHWND;
	m_hBitmap = hbitmap;

	m_mode = 0;

	m_printX = printX;
	m_printY = printY;
	m_sizeX = sizeX;
	m_sizeY = sizeY;
}



CMyButton::~CMyButton()
{
	End();
}


void CMyButton::End(void)
{
}


void CMyButton::Print(HDC hdc, HDC src, int md, RECT* lpRect)
{
	if (m_hBitmap == NULL) return;

	if (lpRect != NULL)
	{
		RECT rc;
		SetRect(&rc,m_printX,m_printY,m_printX+m_sizeX,m_printY+m_sizeY);
		if (IntersectRect(&rc,lpRect,&rc) == 0) return;
	}

	BOOL srcFlag = FALSE;
	
	if (src == NULL)
	{
		srcFlag = TRUE;
		src = CreateCompatibleDC(hdc);
	}
	HGDIOBJ old = SelectObject(src,m_hBitmap);

	//print
	if (md == -1) md = m_mode;

	BitBlt(hdc,m_printX,m_printY,m_sizeX,m_sizeY,src,m_sizeX*md,m_sizeY*0,SRCCOPY);




	SelectObject(src,old);

	if (srcFlag)
	{
		DeleteDC(src);
	}
}

void CMyButton::CalcuLButtonDown(WPARAM wParam,LPARAM lParam)
{
	if (m_mode == 2) return;
	if (m_parentHWND == NULL) return;

	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int x = xPos - m_printX;
	int y = yPos - m_printY;
	if ((x>=0) && (y>=0) && (x<m_sizeX) && (y<m_sizeY))
	{
		PostMessage(m_parentHWND,WM_COMMAND,(WPARAM)ID_USER_LBUTTONDOWN,(LPARAM)m_number);
	}


}

BOOL CMyButton::CheckOn(int xPos,int yPos)
{
//	if (m_mode == 2) return FALSE;
	if (m_parentHWND == NULL) return FALSE;


	int x = xPos - m_printX;
	int y = yPos - m_printY;
	if ((x>=0) && (y>=0) && (x<m_sizeX) && (y<m_sizeY))
	{
		return TRUE;
	}

	return FALSE;

}


int CMyButton::GetNumber(void)
{
	return m_number;
}