//
// myBMP.cpp
//


#include <windows.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "myBMP.h"


CMyBMP::CMyBMP(HWND hwnd, HINSTANCE hinstance, LPCSTR name)
{
	m_hWnd = hwnd;
	m_hInstance = hinstance;

	m_hBitmap = LoadBitmap(m_hInstance,name);

	HDC hdc = GetDC(m_hWnd);
	m_hdc = CreateCompatibleDC(hdc);
	ReleaseDC(m_hWnd,hdc);

	m_old = SelectObject(m_hdc,m_hBitmap);
}

CMyBMP::CMyBMP(HWND hwnd, HINSTANCE hinstance, HBITMAP hbitmap)
{
	m_hWnd = hwnd;
	m_hInstance = hinstance;

	m_hBitmap = NULL;

	HDC hdc = GetDC(m_hWnd);
	m_hdc = CreateCompatibleDC(hdc);
	ReleaseDC(m_hWnd,hdc);

	m_old = SelectObject(m_hdc,hbitmap);
}


CMyBMP::~CMyBMP()
{
	End();
}



void CMyBMP::End(void)
{
	if (m_hdc != NULL)
	{
		if (m_old != NULL)
		{
			SelectObject(m_hdc,m_old);
			m_old = NULL;
		}

		DeleteDC(m_hdc);
		m_hdc = NULL;
	}

	DELETEGDIOBJECT(m_hBitmap);

}


void CMyBMP::Put(HDC hdcDst, int putX, int putY, int srcX, int srcY, int sizeX, int sizeY)
{
	BitBlt(hdcDst,putX,putY,sizeX,sizeY,m_hdc,srcX,srcY,SRCCOPY);
}


/*_*/

