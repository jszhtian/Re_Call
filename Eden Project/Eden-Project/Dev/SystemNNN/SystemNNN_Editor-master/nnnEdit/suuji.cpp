//
// suuji.cpp
//

#include <windows.h>
#include <stdio.h>

#include "suuji.h"


CEditSuuji::CEditSuuji(HBITMAP hbitmap,int sizeX,int sizeY)
{
	m_hBitmap = hbitmap;
	m_sizeX = sizeX;
	m_sizeY = sizeY;
}


CEditSuuji::~CEditSuuji()
{
	End();
}


void CEditSuuji::End(void)
{
}



void CEditSuuji::PrintSuuji(HDC hdc, int x, int y, int d, int keta, HDC src)
{

	BOOL nullFlag = FALSE;
	HGDIOBJ old = NULL;

	if (src == NULL)
	{
		if (m_hBitmap == NULL) return;
		nullFlag = TRUE;
		src = CreateCompatibleDC(hdc);
		old = SelectObject(src,m_hBitmap);
	}


	BOOL minusFlag = FALSE;

	if (d<0)
	{
		minusFlag = TRUE;
		d *= -1;
	}

	x += m_sizeX * (keta-1);

	for (int i=0;i<keta;i++)
	{
		int c = 0;

		if (d == 0)
		{
			if (i == 0)
			{
				c = '0';
			}
			else
			{
				c = ' ';
			}
		}
		else
		{
			c = d % 10;
			d /= 10;
			c += '0';
		}

		PutChara(hdc,src,x,y,c);
		x -= m_sizeX;
		if (d == 0) break;
	}

	if (minusFlag)
	{
		PutChara(hdc,src,x,y,'-');
	}

	if (nullFlag)
	{
		SelectObject(src,old);
		DeleteDC(src);
	}
}

void CEditSuuji::PrintMessage(HDC hdc, int x, int y, LPSTR mes, HDC src)
{

	BOOL nullFlag = FALSE;
	HGDIOBJ old = NULL;

	if (src == NULL)
	{
		if (m_hBitmap == NULL) return;
		nullFlag = TRUE;
		src = CreateCompatibleDC(hdc);
		old = SelectObject(src,m_hBitmap);
	}

	int c = *mes;
	c &= 0xff;

	while (c != 0)
	{
		PutChara(hdc,src,x,y,c);

		x += m_sizeX;
		mes++;
		c = *mes;
		c &= 0xff;
	}





	if (nullFlag)
	{
		SelectObject(src,old);
		DeleteDC(src);
	}

}

void CEditSuuji::PutChara(HDC hdc, HDC src, int x, int y, int c)
{
	c -= 0x20;
	c &= 0x3f;
	int sx = c & 0xf;
	int sy = c >> 4;
	sx *= m_sizeX;
	sy *= m_sizeY;

	BitBlt(hdc,x,y,m_sizeX,m_sizeY,src,sx,sy,SRCCOPY);
}


