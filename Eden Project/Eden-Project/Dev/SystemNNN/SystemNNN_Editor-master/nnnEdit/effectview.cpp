//
// effectview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\wheelmouse.h"
//#include "mydirectx.h"
#include "..\..\systemNNN\nyanLib\include\myGraphics.h"

#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"

//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "myBitmap.h"

#include "effectdoc.h"
#include "effectview.h"

//@@@#include "printmessage.h"

#include "effectParam.h"

#include "komadata.h"

#include "suuji.h"





//int CEffectView::m_deltaTable[] = {1,-1,10,-10,100,-100, 19999,-19999};
int CEffectView::m_deltaTable[] = {-19999,-100,-10,-1,0,1,10,100,19999};



CEffectView::CEffectView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("effectWindow");

///	m_message = new CPrintMessage();

	SetResizeFlag();
	SetBlockSize(256-24,24*24+24+32);
	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
//	SetNonBlockArea(0,0,0,0);
	SetFreeSizeFlag(0);
//	SetScrollFlag(TRUE,FALSE);

	SetWindowStyle();

	SetViewWindowText("エフェクト");



//	m_effectParaList = new CEffectParaList();

//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_VSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

	m_bitmapSel = new CMyBitmap("nnndir\\setup\\bmp\\seleffect.bmp");
	m_bitmapDelta = new CMyBitmap("nnndir\\setup\\bmp\\effectButton.bmp");

//	m_hBitmapSel = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_SELEFFECT));
//	m_hBitmapDelta = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_EFFECT_DELTA));

	m_hBitmapSel = m_bitmapSel->GetHBitmap();
	m_hBitmapDelta = m_bitmapDelta->GetHBitmap();

	m_hBitmapFont = m_document->GetCommonBitmap(1);
	m_suuji = new CEditSuuji(m_hBitmapFont);

//	m_editKosuu = 0;
//	m_nowSelect = -1;


	m_deltaButtonPrintX = 8;
	m_deltaButtonPrintY = 24+24*24;
	m_deltaButtonNextX = 48;
	m_deltaButtonNextY = 32;
	m_deltaButtonSizeX = 32;
	m_deltaButtonSizeY = 24;
}


CEffectView::~CEffectView()
{
	End();
}

void CEffectView::End(void)
{
////@@	ENDDELETECLASS(m_message);
//	ENDDELETECLASS(m_effectParaList);

	ENDDELETECLASS(m_bitmapSel);
	ENDDELETECLASS(m_bitmapDelta);

//	DELETEGDIOBJECT(m_hBitmapSel);
//	DELETEGDIOBJECT(m_hBitmapDelta);

	ENDDELETECLASS(m_suuji);

}


LRESULT CEffectView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	int x,y;
	CEffectDoc* pDoc = (CEffectDoc*)m_document;

	HDC hdc;
//	HDC src;
	HGDIOBJ old;

	RECT rc;

	int xPos,yPos;

	switch (message)
	{
	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;

	case WM_MOUSEWHEEL:
		int mh;
		mh = (int)(short)HIWORD(wParam);
		if (mh > 0)
		{
			pDoc->ClickDeltaButton0(-1,TRUE);
		}
		else if (mh < 0)
		{
			pDoc->ClickDeltaButton0(1,TRUE);
		}
		break;

	case WM_ERASEBKGND:
		return FALSE;
//		EraseSplit((HDC)wParam);

		GetClientRect(hWnd,&rc);
		hdc = (HDC)wParam;
		old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));


		PatBlt(hdc,0,0,rc.right,rc.bottom,PATCOPY);

		SelectObject(hdc,old);
		return TRUE;
		break;

	case WM_LBUTTONDOWN:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
//		if (pDoc->GetEditMode() == 0)
//		{
			ClickRoutineNormalMode(xPos,yPos);
//		}
//		else
//		{
//			ClickRoutineSelectMode(xPos,yPos);
//		}
		break;

	case WM_PAINT:
		PrintNormalMode(hWnd,wParam,lParam);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}



void CEffectView::ClickRoutineNormalMode(int x, int y)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document;

	CKomaData* pKoma = pDoc->GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = pDoc->GetNowSelectEffectLayer();

	if (layer == -1) return;

	int kosuu = pKoma->GetEffectParaKosuu(layer);
	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

//	int selectParam = pKoma->GetSelectParam();

	CEffectParam* effectParam = ((CEffectDoc*)m_document)->GetEffectParam(eff);

//	int kosuuMin = CEffectParaList::m_para[eff].paraKosuuMin;
//	int kosuuMax = CEffectParaList::m_para[eff].paraKosuuMax;
	int kosuuMin = effectParam->GetNeedParaKosuu();
	int kosuuMax = effectParam->GetParaKosuu();

	//check button
	if ((x>=8) && (x<8+216) && (y>=24+2) && (y<48+2))
	{
		int bt = (x - 8)/24;
		pDoc->ClickDeltaButton0(m_deltaTable[bt]);
		return;
	}

/*
	for (int j=0;j<2;j++)
	{
		for (int i=0;i<4;i++)
		{
			int xx = x - (m_deltaButtonPrintX + i * m_deltaButtonNextX);
			int yy = y - (m_deltaButtonPrintY + j * m_deltaButtonNextY);
			if ((xx>=0) && (xx<32) && (yy>=0) && (yy<24))
			{
	//			pDoc->ClickDeltaButton(m_nowSelect,m_deltaTable[i + j*4]);
				pDoc->ClickDeltaButton0(m_deltaTable[i + j*4]);
				return;
			}
		}
	}
*/



	y -= 24*2;
	if (y<0) return;

	y /= 24;
//	if ((y<kosuuMin) || (y>=kosuuMax)) return;

	if (x<=24)
	{
		if ((y>=kosuuMin) || (y<kosuuMax))
		{
			pDoc->ClickSelButton(y);
		}
	}
	else
	{
		if ((y>=0) && (y<kosuu))
		{
			int selectParam = pKoma->GetSelectParam(layer);

			if (y != selectParam)
			{
				pDoc->ChangeSelectParam(y);
//				m_nowSelect = y;
				MyInvalidateRect();
			}
		}
	}
}





//void CEffectView::ClickRoutineSelectMode(int x, int y)
//{
//}


//void CEffectView::PrintNormalMode(HWND hwnd,HDC hdc, HDC src)
void CEffectView::PrintNormalMode(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd,&ps);

	HDC hdc = ps.hdc;

	CEffectDoc* pDoc = (CEffectDoc*)m_document;


	HBITMAP effectPic = pDoc->GetEffectMini();
	HBITMAP fontPic = pDoc->GetMyFont();

	CKomaData* pKoma = pDoc->GetNowSelectKoma();

	int zahyoPrintType = m_document->GetApp()->GetZahyoPrintType();


	HDC src = CreateCompatibleDC(hdc);
	HGDIOBJ old = SelectObject(src,effectPic);

	BOOL printFlag = FALSE;

	if (pKoma != NULL)
	{
		int layer = pDoc->GetNowSelectEffectLayer();
		if (layer != -1)
		{
			int eff = pKoma->GetEffect(layer);
			if ((eff == 0) || (eff == -1))
			{
				//nothing
			}
			else
			{
				printFlag = TRUE;


				RECT rcClient;
				GetClientRect(hWnd,&rcClient);
				HGDIOBJ old0 = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
				PatBlt(hdc,0,0,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 0,PATCOPY);
				SelectObject(hdc,old0);






				CEffectParam* effectParam = ((CEffectDoc*)m_document)->GetEffectParam(eff);

				int selectParam =pKoma->GetSelectParam(layer);

				COLORREF oldColor = GetTextColor(hdc);
				COLORREF oldBack = GetBkColor(hdc);
				int oldMode = GetBkMode(hdc);

				SetBkMode(hdc,TRANSPARENT);

				//pic

				int ex = (eff % 10) * 32;
				int ey = (eff / 10) * 24;
				HGDIOBJ old = SelectObject(src,effectPic);
				BitBlt(hdc,0,0,32,24,src,ex,ey,SRCCOPY);
				SelectObject(src,old);

				//name

				//LPSTR name = CEffectParaList::m_para[eff].name;
				LPSTR name = effectParam->GetEffectName();

				SetTextColor(hdc,COLORREF(0xffffff));
				TextOut(hdc,32+4,4,name,strlen(name));


				//int kosuuMin = CEffectParaList::m_para[eff].paraKosuuMin;
				//int kosuuMax = CEffectParaList::m_para[eff].paraKosuuMax;
				int kosuuMin = effectParam->GetNeedParaKosuu();
				int kosuuMax = effectParam->GetParaKosuu();

				//para
				int kosuu = pKoma->GetEffectParaKosuu(layer);


				int i;
				for (i=0;i<kosuuMax;i++)
				{
//					LPSTR name2 = CEffectParaList::m_para[eff].effectParaData[i].paraName;
					LPSTR name2 = effectParam->GetParamName(i);
					int px = 16+4+4 + 24;
					int py = 4+24+24*i;

					py += 24;

					if (i<kosuu)
					{
						if (i == selectParam)
						{
							SetTextColor(hdc,COLORREF(0x000000));
							SetBkColor(hdc,COLORREF(0xffffff));
							SetBkMode(hdc,OPAQUE);
							TextOut(hdc,px,py,name2,strlen(name2));
							SetTextColor(hdc,COLORREF(0xffffff));
							SetBkMode(hdc,TRANSPARENT);
						}
						else
						{
							TextOut(hdc,px,py,name2,strlen(name2));
						}
//						int para = pKoma->GetEffectPara(layer,i);
					}
					else
					{
//						COLORREF oldColor = GetTextColor(hdc);
						SetTextColor(hdc,COLORREF(0x0000ff));
						TextOut(hdc,px,py,name2,strlen(name2));
//						SetTextColor(hdc,oldColor);

						//default
						
					}
				}
			
				//suuchi
				SelectObject(src,m_hBitmapFont);

				for (i=0;i<kosuuMax;i++)
				{
					int px = 16*10 + 4 - 24 +32;
					int px2 = px + 36 -32 -4;
					int py = 4+24+24*i;

					py += 24;

					if (i<kosuu)
					{
						int d = pKoma->GetEffectPara(layer,i);

						if (zahyoPrintType)
						{
							int et = effectParam->GetParaEditType(i);
							if ((et == 5) || (et == 15))
							{
								d = AdjustCenterX(d);
							}
							else if ((et == 6) || (et == 16))
							{
								d = AdjustCenterY(d);

							}
						}


						if ((d<=-10000) || (d>=10000))
						{
							m_suuji->PrintSuuji(hdc,px2-16,py,d,5,src);
						}
						else
						{
							m_suuji->PrintSuuji(hdc,px2,py,d,4,src);
						}
					}
					else
					{
						m_suuji->PrintMessage(hdc,px,py,"-----",src);
					}
				}

				



				//button
				SelectObject(src,m_hBitmapSel);

				for (i=0;i<kosuuMax;i++)
				{
					int py = 24+24+4+24*i;

					if (i<kosuu)
					{
						int srcx = 0;
						if (i == selectParam) srcx += 16;
						int srcy = 16;

						if (i<kosuuMin)
						{
							BitBlt(hdc,4,py,16,16,src,0,0,SRCCOPY);
						}
						else
						{
							BitBlt(hdc,4,py,16,16,src,16,0,SRCCOPY);
							srcy += 16;
						}

						BitBlt(hdc,4+24,py,16,16,src,srcx,srcy,SRCCOPY);
					}
					else
					{
						BitBlt(hdc,4   ,py,16,16,src,32,0,SRCCOPY);
						BitBlt(hdc,4+24,py,16,16,src,0,32,SRCCOPY);
					}
				}






				SelectObject(src,m_hBitmapDelta);
				BitBlt(hdc,8,24+2,216,24,src,0,0,SRCCOPY);

//				int yy = 24+24*24;
//				BitBlt(hdc,8,yy,32,24,src,0,0,SRCCOPY);
//				BitBlt(hdc,8+48*1,yy,32,24,src,0,24,SRCCOPY);
//				BitBlt(hdc,8+48*2,yy,32,24,src,0,48,SRCCOPY);
//				BitBlt(hdc,8+48*3,yy,32,24,src,0,72,SRCCOPY);

//				yy += 32;
//				BitBlt(hdc,8,yy,32,24,src,0,96,SRCCOPY);
//				BitBlt(hdc,8+48*1,yy,32,24,src,0,120,SRCCOPY);
//				BitBlt(hdc,8+48*2,yy,32,24,src,0,144,SRCCOPY);
//				BitBlt(hdc,8+48*3,yy,32,24,src,0,168,SRCCOPY);

				SetBkMode(hdc,oldMode);
				SetBkColor(hdc,oldBack);
				SetTextColor(hdc,oldColor);
			}
		}



	}


	if (printFlag == FALSE)
	{
		RECT rcClient;
		GetClientRect(hWnd,&rcClient);
		HGDIOBJ old0 = SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));
		PatBlt(hdc,0,0,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 0,PATCOPY);
		SelectObject(hdc,old0);
	}




	SelectObject(src,old);

	DeleteDC(src);
	EndPaint(hWnd,&ps);
}

int CEffectView::AdjustCenterX(int d)
{
	int centerX = CMyGraphics::GetScreenSizeX();
	return d - centerX / 2;
}

int CEffectView::AdjustCenterY(int d)
{
	int centerY = CMyGraphics::GetScreenSizeY();
	return d - centerY / 2;
}

void CEffectView::OnEnterKey(void)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document;
	pDoc->OnEnterKey();
}


void CEffectView::OnLeftKey(void)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document;
	pDoc->OnLeftKey();
}

void CEffectView::OnRightKey(void)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document;
	pDoc->OnRightKey();
}

void CEffectView::OnUpKey(void)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document;
	pDoc->OnUpKey();
}

void CEffectView::OnDownKey(void)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document;
	pDoc->OnDownKey();
}

BOOL CEffectView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = EFFECT_WINDOW;
	int subType = -1;//CheckOnBalloonButton(x,y);

	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}

/*
void CEffectView::PrintSelectMode(HDC hdc, HDC src)
{
	CEffectDoc* pDoc = (CEffectDoc*)m_document;

	HBITMAP effectPic = pDoc->GetEffectMini();
	HBITMAP fontPic = pDoc->GetMyFont();

	HGDIOBJ old = SelectObject(src,effectPic);

	//選択中のやつ

	BitBlt(hdc,0,0,32,24,src,0,0,SRCCOPY);

	//一覧

	for (int j=0;j<8;j++)
	{
		for (int i=0;i<8;i++)
		{
			int k = i + j * 8;
			BitBlt(hdc,i*32,j*24+32,32,24,src,(k % 10)*32,(k / 10)*24,SRCCOPY);
		}
	}
//		old = SelectObject(src,bm);
//		BitBlt(hdc,0,0,32,24,src,0,0,SRCCOPY);
//		SelectObject(src,old);

	SelectObject(src,fontPic);
	//名前
	m_message->PrintMessage(hdc,32,0,"AIUEO",src);
	//コマンド OK CANCEL

	m_message->PrintMessage(hdc,32,24*15,"OK",src);
	m_message->PrintMessage(hdc,96,24*15,"CANCEL",src);



	SelectObject(src,old);
}
*/


/*_*/



