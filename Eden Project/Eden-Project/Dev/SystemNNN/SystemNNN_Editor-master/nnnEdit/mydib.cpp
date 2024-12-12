//
// mydib.cpp
//

#include <windows.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "mydib.h"

#include "selectobjectonly.h"

CMyDIB::CMyDIB(HWND hwnd, int x, int y, int bpp)
{
	m_sizeX = x;
	m_sizeY = y;
	m_bpp = bpp;

	m_hBitmap = NULL;
	m_buffer = NULL;

	m_selObj = new CSelectObjectOnly();


	HDC hdc = GetDC(hwnd);
	BITMAPINFO bmi;

	ZeroMemory(&bmi,sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biBitCount = m_bpp;
	bmi.bmiHeader.biHeight = -m_sizeY;
	bmi.bmiHeader.biWidth = m_sizeX;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	m_hBitmap = CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,(void**)&m_buffer,NULL,NULL);

	ReleaseDC(hwnd,hdc);
}

CMyDIB::~CMyDIB()
{
	End();
}

void CMyDIB::End(void)
{
	DELETEGDIOBJECT(m_hBitmap);
	ENDDELETECLASS(m_selObj);

}

void CMyDIB::Put(int x, int y, HDC hdc, HDC src)
{
	if (m_hBitmap == NULL) return;
	BOOL srcFlag = FALSE;
	if (src == NULL)
	{
		src = CreateCompatibleDC(hdc);
		srcFlag = TRUE;
	}

//	HGDIOBJ old = SelectObject(src,m_hBitmap);
	HGDIOBJ old = m_selObj->Sel(src,m_hBitmap);


	BitBlt(hdc,x,y,m_sizeX,m_sizeY,src,0,0,SRCCOPY);

	SelectObject(src,old);

	if (srcFlag)
	{
		DeleteDC(src);
	}
}

void CMyDIB::SetPic(LPVOID data)
{
	if (m_hBitmap == NULL) return;
	if (m_buffer == NULL) return;

	int sz = m_bpp / 8;
	memcpy(m_buffer,data,sz*m_sizeX*m_sizeY);
}
