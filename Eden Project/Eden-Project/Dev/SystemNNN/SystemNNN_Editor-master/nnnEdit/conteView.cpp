//
// Conteview.cpp
//


#include <windows.h>
#include <stdio.h>

#include "dlgs.h"

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\wheelMouse.h"
//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "windowlist.h"
#include "case.h"
#include "messageData.h"
#include "myapplicationBase.h"

#include "bitmapnumber.h"

#include "case.h"


#include "myBitmap.h"

#include "Contedoc.h"
#include "Conteview.h"

#include "mybutton.h"

#include "komadata.h"
#include "filmdata.h"
#include "messageData.h"

#include "myfileopen.h"

//#include "myapplicationBase.h"

#include "minipiccache.h"


CConteView::CConteView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("conteWindow");

	SetResizeFlag();
	SetBlockSize(256,50);
	SetNonBlockArea(0,24,0,0);

	SetBlockKosuuMin(1,2);
	SetBlockKosuuMax(1,32);
	SetFreeSizeFlag(1);
	SetScrollFlag(FALSE,TRUE);


	SetWindowStyle();

	SetViewWindowText("ƒRƒ“ƒe");

	m_fontSize = 10;
	m_font = CreateFont(m_fontSize,m_fontSize/2,0,0,FW_REGULAR,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FIXED_PITCH | FF_MODERN,"‚l‚r ƒSƒVƒbƒN");

	SetScrollPara(0,4,5);

	m_filmBitmap = new CMyBitmap("nnndir\\setup\\bmp\\film64x48.bmp");
//	m_hBitmapFilm64x48 = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_FILM64X48));
	m_hBitmapFilm64x48 = m_filmBitmap->GetHBitmap();


//	m_document = pDocument;
//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style &= ~(WS_MINIMIZEBOX);
//	style |= WS_VSCROLL;

//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_PIC));
//	m_popupMenu[1] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_EFFECT));
//	m_buttonCut = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),0,0);

	ReCalcuScrollPara();
}


CConteView::~CConteView()
{
	End();
}

void CConteView::End(void)
{
	DELETEGDIOBJECT(m_font);
//	DELETEGDIOBJECT(m_hBitmapFilm64x48);
	DELETECLASS(m_filmBitmap);

}



LRESULT CConteView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int x,y;
	int n;

//	int xNum;
//	int yNum;
//	POINT pt;

	CConteDoc* pDoc = (CConteDoc*)m_document;

	switch (message)
	{
	case WM_MOUSEWHEEL:
		OnWheelScroll(message,wParam,lParam);
		return TRUE;
		break;
	case WM_VSCROLL:
		OnScroll(message,wParam,lParam);
		break;


	case WM_ERASEBKGND:
		EraseSplit((HDC)wParam);
		return TRUE;
		break;

///	case WM_LBUTTONDBLCLK:
///		MessageBox(m_hWnd,"doucle click","aaa",MB_OK);
///		break;

	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		n = GetContentsNumber(x,y);
		if (n != -1)
		{
			pDoc->OnClickFilm(n);
		}

		break;


	case WM_CLOSE:
		Show(FALSE);
		return 0;
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);
		HDC hdc = ps.hdc;



		RECT rcWnd;
		GetClientRect(hWnd,&rcWnd);


		CKomaData* pKoma;
		pKoma = pDoc->GetNowSelectKoma();
//		if (pKoma == NULL)
//		{
//			HGDIOBJ old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
//
//
//			
//			PatBlt(hdc,0,24,rcWnd.right-rcWnd.left,rcWnd.bottom - rcWnd.top - 24,PATCOPY);
//			
//			SelectObject(hdc,old);
//			EndPaint(hWnd,&ps);
//			return 0;
//			break;
//		}


//		RECT tmpRect;
//		RECT rc;

		//ps.rcPaint‚Æ‚­‚ç‚×‚Ä‚©‚«‚È‚¨‚· IntersectRect IntersectClipRect


		HDC src = CreateCompatibleDC(hdc);

//RECT rc0;
//SetRect(&rc0,0,0,1000,1000);
//FillRect(hdc,&rc0,(HBRUSH)BLACK_BRUSH);

//		if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,2,&ps.rcPaint);
//		if (m_buttonDelete != NULL) m_buttonDelete->Print(hdc,src,0,&ps.rcPaint);

//		HGDIOBJ old;

//		int picConte;
//		int effConte;

		HGDIOBJ oldFont = SelectObject(hdc,m_font);
//		TextOut(hdc,32,32,"‚ ‚¢‚¤‚¦‚¨‚P‚Q‚R‚S‚T‚U‚V‚W‚X‚O‚`‚a‚b‚c‚d",40);
//		TextOut(hdc,32,64,"Š¿Žš•\Ž¦",8);

		RECT rc9;
		GetClientRect(hWnd,&rc9);
//		Ellipse(hdc,rc0.left,rc9.top,rc9.right,rc9.bottom);
//		Ellipse(hdc,0,0,m_windowSizeX,m_windowSizeY);



		int md = GetBkMode(hdc);
		SetBkMode(hdc,TRANSPARENT);


		CFilmData* pFilm;
		pFilm = pDoc->GetNowSelectFilm();
		if (pFilm != NULL)
		{
			int nowSelect = pFilm->GetNowSelectNumber();
			int kosuuMax = pFilm->GetObjectKosuu();

			int pg;
			pg = GetMyScrollPos();

			char mes[1000];

//			int nowSelect = pFilm->GetNowSelectNumber();

			for (int i=0;i<=GetBlockKosuu();i++)	//‚Ä‚«‚Æ‚¤
			{
				int x,y;
				x = 4;
				y = 24+2+50*i;
				RECT rc;
				SetRect(&rc,x,y,x+64,y+48);
				if (IntersectRect(&ps.rcPaint,&rc,&rc) != 0)
				{
					int k = pg + i;
//					if (k>63) k = 63;

					if ((k>=0) && (k<=kosuuMax))
					{
						if (k<kosuuMax)
						{
							HBITMAP hbitmap = pDoc->GetHBitmap(k);
							if (hbitmap != NULL)
							{
								SelectObject(src,hbitmap);
								BitBlt(hdc,x,y,64,48,src,0,0,SRCCOPY);
							}
						}
						else if (k == kosuuMax)
						{
							if (k == nowSelect)
							{
								HGDIOBJ old3 = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
								PatBlt(hdc,x-3,y-3,64+6,48+6,PATCOPY);					
								SelectObject(hdc,old3);

//								HGDIOBJ old = SelectObject(src,m_hBitmapFilm64x48);
								HGDIOBJ old = m_document->SelObjOnly(src,m_hBitmapFilm64x48);	//@@@
								BitBlt(hdc,x,y,64,48,src,0,0,NOTSRCCOPY);
								BitBlt(hdc,x,y,64,48,src,0,0,NOTSRCCOPY);
								SelectObject(src,old);
							}
							else
							{
//								HGDIOBJ old = SelectObject(src,m_hBitmapFilm64x48);
								HGDIOBJ old = m_document->SelObjOnly(src,m_hBitmapFilm64x48);	//@@@
								BitBlt(hdc,x,y,64,48,src,0,0,SRCCOPY);
								SelectObject(src,old);
							}

						}

						CKomaData* pKoma = pFilm->GetKoma(k);

						int mesPrintX = 72;
						int mesPrintY = i*50 + 24;
						int mesEndX = rcWnd.right; // - rcWnd.left - mesPrintX;
						int mesEndY = mesPrintY + 48;

						if (pKoma != NULL)
						{
							//message
							CMessageData* pMessage0 = (CMessageData*)(pKoma->GetObjectData(0));
							CMessageData* pMessage = pMessage0;
							
							if (pMessage0 != NULL)
							{
								int type0 = pMessage0->GetMessageMode();
								int type = type0;


								if (type0 == 3)
								{
									if (pKoma->GetObjectKosuu() > 1)
									{
										pMessage = (CMessageData*)(pKoma->GetObjectData(1));
										if (pMessage != NULL)
										{
											type = pMessage->GetMessageMode();
										}
									}
								}

								RECT rc00;
								SetRect(&rc00,mesPrintX,mesPrintY,mesEndX,mesEndY);

//								FillRect(hdc,&rc00,(HBRUSH)(COLOR_GRAYTEXT+1));

								if (pMessage != NULL)
								{
									if (type == 3)
									{
										wsprintf(mes,"DRAW = %d",pMessage->GetDrawCount());
										TextOut(hdc,mesPrintX,mesPrintY,mes,strlen(mes));
									}
									else
									{
										LPSTR text = pMessage->GetMessageData();
										int ln = strlen(text);
										if (text != NULL)
										{
											RECT rc000 = rc00;
											if (type0 == 3)
											{
												wsprintf(mes,"DRAW = %d",pMessage0->GetDrawCount());
												TextOut(hdc,mesPrintX,mesPrintY,mes,strlen(mes));
												rc000.top += 10;
											}
											DrawText(hdc,text,ln,&rc000,DT_NOPREFIX);
										}
									}
								}
							}
						}

						if (k == nowSelect)
						{
							PatBlt(hdc,mesPrintX,mesPrintY,mesEndX - mesPrintX, mesEndY - mesPrintY,DSTINVERT);
						}
					}
				}
			}
		}

		SetBkMode(hdc,md);

		SelectObject(hdc,oldFont);

		DeleteDC(src);


		EndPaint(hWnd,&ps);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


BOOL CConteView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 



	int subType = -1;
	int type = CONTE_WINDOW;

	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

//	OutputDebugString("*moveMouse layer*\x00f\x00a");
	return FALSE;
}



/*_*/



