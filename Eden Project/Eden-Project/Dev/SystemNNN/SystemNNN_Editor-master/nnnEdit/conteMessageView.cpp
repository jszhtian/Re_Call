//
// ConteMessageView.cpp
//


#include <windows.h>
#include <stdio.h>

#include "dlgs.h"

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "bitmapnumber.h"

#include "case.h"
#include "windowlist.h"
//#include "case.h"
#include "messageData.h"
#include "myapplicationBase.h"

#include "ConteMessagedoc.h"
#include "ConteMessageView.h"

#include "mybutton.h"

#include "messageData.h"
#include "komadata.h"



#include "myfileopen.h"

//#include "myapplication.h"

#include "minipiccache.h"


CConteMessageView::CConteMessageView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument,clientHWND,hinstance)
{
	SetMyViewWindowName("conteMessageWindow");

	SetResizeFlag();
	SetBlockSize(200,800);
	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
	SetNonBlockArea(0,24,0,0);

	SetFreeSizeFlag(0);
	SetFreeSizeFlag(1);

	SetWindowStyle();

	SetViewWindowText("ÉRÉìÉeÉÅÉbÉZÅ[ÉW");

	m_fontSize = 10;

	m_font = CreateFont(m_fontSize,m_fontSize / 2,0,0,FW_REGULAR,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FIXED_PITCH | FF_MODERN,"ÇlÇr ÉSÉVÉbÉN");



//	m_document = pDocument;
//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style &= ~(WS_MINIMIZEBOX);
//	style |= WS_VSCROLL;

//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_PIC));
//	m_popupMenu[1] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_EFFECT));
//	m_buttonCut = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),0,0);
}


CConteMessageView::~CConteMessageView()
{
	End();
}

void CConteMessageView::End(void)
{
	DELETEGDIOBJECT(m_font);
}



LRESULT CConteMessageView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	int x,y;
//	int xNum;
//	int yNum;
//	POINT pt;

	CConteMessageDoc* pDoc = (CConteMessageDoc*)m_document;

	switch (message)
	{
///	case WM_LBUTTONDBLCLK:
///		MessageBox(m_hWnd,"doucle click","aaa",MB_OK);
///		break;



	case WM_CLOSE:
		Show(FALSE);
		return 0;
		break;
	case WM_ERASEBKGND:
//		EraseUpper((HDC)wParam);

		EraseSplit((HDC)wParam);

		return TRUE;
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);
		HDC hdc = ps.hdc;

		CKomaData* pKoma;
		pKoma = pDoc->GetNowSelectKoma();

		RECT rcClient;
		GetClientRect(hWnd,&rcClient);

		if (pKoma == NULL)
		{
			HGDIOBJ old = SelectObject(hdc,GetStockObject(GRAY_BRUSH));
			PatBlt(hdc,0,24,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 24,PATCOPY);
			
			SelectObject(hdc,old);
			EndPaint(hWnd,&ps);
			return 0;
			break;
		}


//		RECT tmpRect;
//		RECT rc;

		//ps.rcPaintÇ∆Ç≠ÇÁÇ◊ÇƒÇ©Ç´Ç»Ç®Ç∑ IntersectRect IntersectClipRect


		HDC src = CreateCompatibleDC(hdc);

//RECT rc0;
//SetRect(&rc0,0,0,1000,1000);
//FillRect(hdc,&rc0,(HBRUSH)BLACK_BRUSH);

//		if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,2,&ps.rcPaint);
//		if (m_buttonDelete != NULL) m_buttonDelete->Print(hdc,src,0,&ps.rcPaint);

//		HGDIOBJ old;

//		int picConte;
//		int effConte;

		DeleteDC(src);


		HGDIOBJ oldFont = SelectObject(hdc,m_font);

		RECT rcNew;
		if (IntersectRect(&rcNew,&rcClient,&ps.rcPaint))
		{
			int md = GetBkMode(hdc);
			SetBkMode(hdc,TRANSPARENT);
			OnPaint(hWnd,hdc,rcNew);
			SetBkMode(hdc,md);
		}




//		TextOut(hdc,32,32,"Ç†Ç¢Ç§Ç¶Ç®ÇPÇQÇRÇSÇTÇUÇVÇWÇXÇOÇ`ÇaÇbÇcÇd",40);
//		TextOut(hdc,32,64,"äøéöï\é¶",8);



		SelectObject(hdc,oldFont);


		EndPaint(hWnd,&ps);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


void CConteMessageView::OnPaint(HWND hwnd, HDC hdc, RECT rc)
{
	//button??

	int yLine = 25;
	CConteMessageDoc* pDoc = (CConteMessageDoc*)m_document;
	if (pDoc == NULL) return;

	CKomaData* pKoma = pDoc->GetNowSelectKoma();
	if (pKoma == NULL) return;

	int n = pKoma->GetObjectKosuu();
	
	char mes[1000];

	RECT rcWnd;
	GetWindowRect(hwnd,&rcWnd);

	for (int i=0;i<n;i++)
	{
		CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
		if (pMessage != NULL)
		{
			RECT rc00;
			SetRect(&rc00,0,yLine,rcWnd.right - rcWnd.left,yLine+1);
			FillRect(hdc,&rc00,(HBRUSH)(COLOR_GRAYTEXT+1));

			int type = pMessage->GetMessageMode();
			if (type == 3)
			{
				wsprintf(mes,"DRAW = %d",pMessage->GetDrawCount());
				TextOut(hdc,0,yLine,mes,strlen(mes));
				yLine += m_fontSize;
			}
			else
			{
				RECT rc0;
				LPSTR text = pMessage->GetMessageData();
				int ln = strlen(text);

				//í∑Ç≥í≤êÆ
				int sy = 1;
				for (int k=0;k<ln;k++)
				{
					if ((*(text+k)) == 0xd) sy++;
				}

				for (int ii=0;ii<3;ii++)
				{
					if (ln>=4)
					{
						if ((*(text+ln-1) == 0xa) && (*(text+ln-2) == 0xd))
						{
							ln -= 2;
							sy --;
						}
					}
				}

			//	if (sy>4) sy = 4;
				if (sy == 0) sy = 1;

				SetRect(&rc0,0,yLine,rcWnd.right - rcWnd.left,yLine+m_fontSize*sy);

				DrawText(hdc,text,ln,&rc0,DT_NOPREFIX);

				yLine += sy * m_fontSize;

			}

			yLine++;

			if (yLine >= rc.bottom) return;
		}
	}
}


BOOL CConteMessageView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 



	int subType = -1;
	int type = CONTEMESSAGE_WINDOW;

	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

//	OutputDebugString("*moveMouse layer*\x00f\x00a");
	return FALSE;
}

/*_*/



