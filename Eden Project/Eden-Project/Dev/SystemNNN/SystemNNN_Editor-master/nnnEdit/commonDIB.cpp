//
// commonDIB.cpp
//

#include <windows.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "commonDIB.h"


CCommonDIB::CCommonDIB(int sizeX,int sizeY,HDC hdc,int bpp)
{
	m_memHDC = NULL;
	m_old = NULL;

	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_bpp = bpp;

	m_hBitmap = NULL;
	m_buffer = NULL;

	m_memHDC = CreateCompatibleDC(hdc);
	if (m_memHDC == NULL) return;	//error

	BITMAPINFO bmi;
	ZeroMemory(&bmi,sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biBitCount = m_bpp;
	bmi.bmiHeader.biHeight = -m_sizeY;
	bmi.bmiHeader.biWidth = m_sizeX;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	m_hBitmap = CreateDIBSection(m_memHDC,&bmi,DIB_RGB_COLORS,(void**)&m_buffer,NULL,NULL);
	if (m_hBitmap == NULL) return;	//error

	m_old = SelectObject(m_memHDC,m_hBitmap);
}


CCommonDIB::~CCommonDIB()
{
	End();
}


void CCommonDIB::End(void)
{
	if (m_memHDC != NULL)
	{
		if (m_old != NULL)
		{
			SelectObject(m_memHDC,m_old);
		}

		DeleteDC(m_memHDC);
		m_memHDC = NULL;
	}

	DELETEGDIOBJECT(m_hBitmap);
}


BOOL CCommonDIB::ReCreateDIB(int sizeX,int sizeY,HDC hdc)
{

	return FALSE;
}


//srcX srcY‚Í“––Ê‚Â‚©‚í‚É‚á‚¢
BOOL CCommonDIB::Put(HDC hdc, int x, int y, int sizeX ,int sizeY, int* lpData, int srcX, int srcY)
{
	if (hdc == NULL) return FALSE;
	if (m_memHDC == NULL) return FALSE;
	if (m_buffer == NULL) return FALSE;

	if (sizeX>m_sizeX) return FALSE;
	if (sizeY>m_sizeY) return FALSE;

	//srcX srcY area check


	//mem copy
	int* dst = m_buffer;
	int* dst0 = dst;

	for (int j=0;j<sizeY;j++)
	{
		dst = dst0;

		for (int i=0;i<sizeX;i++)
		{
			*dst = *lpData;
			lpData++;
			dst++;
		}

		dst0 += m_sizeX;	//???
	}


	BitBlt(hdc,x,y,sizeX,sizeY,m_memHDC,srcX,srcY,SRCCOPY);

	return TRUE;
}


HDC CCommonDIB::GetMemoryHDC(void)
{
	return m_memHDC;
}



int* CCommonDIB::GetBuffer(void)
{
	return m_buffer;
}

void CCommonDIB::GetSize(SIZE* lpSize)
{
}

/*_*/


