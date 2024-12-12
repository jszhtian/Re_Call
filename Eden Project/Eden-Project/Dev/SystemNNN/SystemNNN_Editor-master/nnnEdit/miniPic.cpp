//
//
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "miniPic.h"


CMiniPic::CMiniPic(HWND hwnd)
{
	m_hWnd = hwnd;
	HDC hdc = GetDC(hwnd);

	BITMAPINFO bmi;
	ZeroMemory(&bmi,sizeof(BITMAPINFOHEADER));

//	m_buffer = new int[64*48];

	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = 64;
	bmi.bmiHeader.biHeight = -48;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	LPVOID lpBuffer;

	m_hBitmap = CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,&lpBuffer,NULL,NULL);
	if (m_hBitmap == NULL)
	{
		//error
	}

	m_buffer = (int*)lpBuffer;

	ZeroMemory(m_buffer,sizeof(int)*64*32);

	ReleaseDC(hwnd,hdc);

	SetFlag(FALSE);
}



CMiniPic::~CMiniPic()
{
	End();
}

void CMiniPic::End(void)
{
	if (m_hBitmap != NULL)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

//	DELETEARRAY(m_buffer);	//çÌèúÇµÇƒÇÕÇ¢ÇØÇ»Ç¢

}


void CMiniPic::SetData(int* ptr)
{
	memcpy(m_buffer,ptr,sizeof(int)*64*48);
	SetFlag();
}


/*_*/

