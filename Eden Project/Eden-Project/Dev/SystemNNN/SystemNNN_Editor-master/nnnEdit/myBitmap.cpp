//
//
//


#include <windows.h>
#include <stdio.h>
//#include <wingdi.h>

#include "..\..\systemNNN\nyanLib\include\commonMacro.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "myBitmap.h"



CMyBitmap::CMyBitmap(LPSTR filename,HDC hdc)
{
	HWND hwnd = NULL;
	m_bitmap = NULL;

	if (hdc == NULL)
	{
		hwnd = GetDesktopWindow();
		hdc = GetDC(hwnd);
	}

	m_size.cx = 1;
	m_size.cy = 1;

	FILE* file = CMyFile::Open(filename,"rb");
	if (file != NULL)
	{
		BITMAPFILEHEADER bfh;
		fread(&bfh,sizeof(BITMAPFILEHEADER),1,file);

		char tmp[256+sizeof(BITMAPINFOHEADER) + sizeof(DWORD)*4 + sizeof(RGBQUAD)*256];

		BITMAPINFOHEADER* lpBif = (BITMAPINFOHEADER*)tmp;
		fread(tmp,sizeof(DWORD),1,file);

		int sz = lpBif->biSize;
		fread(tmp+sizeof(DWORD),sz-sizeof(DWORD),1,file);

		sz = sizeof(BITMAPINFOHEADER);
		lpBif->biSize = sz;

		//palette?
		int bitCount = lpBif->biBitCount;
		if (bitCount <= 8)
		{
			int pal = lpBif->biClrUsed;
			
			if (pal == 0)
			{
				if (bitCount == 1) pal = 2;
				if (bitCount == 4) pal = 16;
				if (bitCount == 8) pal = 256;
			}

			fread(tmp+sz,sizeof(RGBQUAD)*pal,1,file);
		}

		int height = lpBif->biHeight;
		if (height < 0) height *= -1;
		int sz2 = (lpBif->biWidth * height * bitCount) / 8;

		m_size.cx = lpBif->biWidth;
		m_size.cy = height;

		char* tmp2 = new char[sz2];
		fread(tmp2,sizeof(char),sz2,file);

		fclose(file);

		UINT flg = 0;
		
		if (lpBif->biBitCount <= 8)
		{
			flg = DIB_RGB_COLORS;
		}

		m_bitmap = CreateDIBitmap(hdc,lpBif,CBM_INIT ,tmp2,(BITMAPINFO*)lpBif,flg);

		delete [] tmp2;
	}


	if (hwnd != NULL)
	{
		ReleaseDC(hwnd,hdc);
	}
}


CMyBitmap::~CMyBitmap()
{
	End();
}


void CMyBitmap::End(void)
{
	if (m_bitmap != NULL)
	{
		DeleteObject(m_bitmap);
		m_bitmap = NULL;
	}
}





void CMyBitmap::Blt(HDC hdc,POINT dstPoint,POINT srcPoint,SIZE putSize,HDC src,int bltType)
{
	Blt(hdc,dstPoint.x,dstPoint.y,srcPoint.x,srcPoint.y,putSize.cx,putSize.cy,src,bltType);
}

void CMyBitmap::Blt(HDC hdc,int dstX,int dstY,int srcX,int srcY,int sizeX,int sizeY,HDC src,int bltType)
{
	if (m_bitmap == NULL) return;

	BOOL flg = FALSE;

	HGDIOBJ old = NULL;
	if (src == NULL)
	{
		src = CreateCompatibleDC(hdc);
		old = SelectObject(src,m_bitmap);
		flg = TRUE;
	}

	int raster = SRCCOPY;
	if (bltType == 1) raster = SRCAND;
	if (bltType == 2) raster = SRCPAINT;

	BitBlt(hdc,dstX,dstY,sizeX,sizeY,src,srcX,srcY,raster);

	if (flg)
	{
		SelectObject(src,old);
		DeleteDC(src);
	}
}
