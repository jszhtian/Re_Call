//
// buttonPic.cpp
//

#include <windows.h>
#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "myBMP.h"
#include "buttonPic.h"

CButtonPic::CButtonPic(HWND hwnd, HINSTANCE hinstance, LPCSTR name) : CMyBMP(hwnd, hinstance,name)
{
	m_sizeX = 24;
	m_sizeY = 24;
}


CButtonPic::CButtonPic(HWND hwnd, HINSTANCE hinstance, HBITMAP hbitmap) : CMyBMP(hwnd, hinstance,hbitmap)
{
	m_sizeX = 24;
	m_sizeY = 24;
}

CButtonPic::~CButtonPic()
{
	End();
}

void CButtonPic::End(void)
{
}

void CButtonPic::PutButton(HDC hdcDst, int x, int y,int buttonNum, int buttonType)
{
	int srcX = buttonType * m_sizeX;
	int srcY = buttonNum * m_sizeY;
	Put(hdcDst, x,y,srcX,srcY,m_sizeX,m_sizeY);
}













/*_*/


