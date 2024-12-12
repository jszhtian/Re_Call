// MyBitmapButton.cpp: implementation of the CMyBitmapButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "glinkotdr.h"
#include "MyBitmapButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBitmapButton::CMyBitmapButton()
{

}

CMyBitmapButton::~CMyBitmapButton()
{
	DeleteObject(m_rgnFinal);
}

//输入需要处理位图ID和需要透明为背景色的部分的RGB值
void CMyBitmapButton::SetBmpRgn(UINT bitmapResource,COLORREF clrRemove ) 
{
	CDC *pDC=GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap  bmpButton;
	CBitmap *pOldBitMap;
	bmpButton.LoadBitmap(bitmapResource);

	BITMAP bmp;
	bmpButton.GetBitmap(&bmp);
	pOldBitMap=dcMem.SelectObject(&bmpButton);
	HRGN rgnFinal,rgnTemp;

	m_rgnFinal=CreateRectRgn(0,0,bmp.bmWidth,bmp.bmHeight);
	
	for (int i=0;i<bmp.bmWidth;i++)//两个循环把图片中像素点都扫面一遍
	{
		for(int j=0;j<bmp.bmHeight;j++)
		{
			if(dcMem.GetPixel(i,j)==clrRemove)//这点为你设置的要去除部分的颜色
			{
				rgnTemp=CreateRectRgn(i,j,i+1,j+1);
				CombineRgn(m_rgnFinal,m_rgnFinal,rgnTemp,RGN_DIFF);//使用这个函数把rgnFinal中的rgnTemp去除
//				DeleteObject(rgnTemp);
				
			}
			
		}		
	}

	if(!this->IsWindowVisible())
	{
		ShowWindow(SW_SHOW);
	}
	this->SetWindowRgn(rgnFinal,TRUE);
	
	dcMem.SelectObject(pOldBitMap);
	bmpButton.DeleteObject();

// 	DeleteObject(rgnFinal);
    DeleteObject(rgnTemp);

	dcMem.DeleteDC();
	ReleaseDC(pDC);

}
