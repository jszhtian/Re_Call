//
// EffectPic.cpp
//

#include <windows.h>
#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "myBMP.h"
#include "EffectPic.h"

CEffectPic::CEffectPic(HWND hwnd, HINSTANCE hinstance, LPCSTR name) : CMyBMP(hwnd, hinstance,name)
{
	m_sizeX = 32;
	m_sizeY = 24;
}

CEffectPic::CEffectPic(HWND hwnd, HINSTANCE hinstance, HBITMAP hbitmap) : CMyBMP(hwnd, hinstance,hbitmap)
{
	m_sizeX = 32;
	m_sizeY = 24;
}

CEffectPic::~CEffectPic()
{
	End();
}

void CEffectPic::End(void)
{
}

void CEffectPic::PutEffect(HDC hdcDst, int x, int y,int num)
{
	int srcX = (num % 10) * m_sizeX;
	int srcY = (num / 10) * m_sizeY;
	Put(hdcDst, x,y,srcX,srcY,m_sizeX,m_sizeY);
}



/*_*/

