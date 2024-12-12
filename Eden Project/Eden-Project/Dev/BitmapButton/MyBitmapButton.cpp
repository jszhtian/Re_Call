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

//������Ҫ����λͼID����Ҫ͸��Ϊ����ɫ�Ĳ��ֵ�RGBֵ
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
	
	for (int i=0;i<bmp.bmWidth;i++)//����ѭ����ͼƬ�����ص㶼ɨ��һ��
	{
		for(int j=0;j<bmp.bmHeight;j++)
		{
			if(dcMem.GetPixel(i,j)==clrRemove)//���Ϊ�����õ�Ҫȥ�����ֵ���ɫ
			{
				rgnTemp=CreateRectRgn(i,j,i+1,j+1);
				CombineRgn(m_rgnFinal,m_rgnFinal,rgnTemp,RGN_DIFF);//ʹ�����������rgnFinal�е�rgnTempȥ��
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
