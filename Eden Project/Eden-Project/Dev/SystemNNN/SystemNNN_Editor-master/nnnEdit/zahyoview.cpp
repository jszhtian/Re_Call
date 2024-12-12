//
// zahyoview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanEffectLib\effectList.h"
#include "..\..\systemNNN\nyanLib\include\myGraphics.h"

//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "myBitmap.h"

#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"

#include "mydocument.h"

//#include "myPopupMenu.h"

#include "zahyoview.h"
#include "zahyoDoc.h"

#include "suuji.h"

#include "komadata.h"


int CZahyoView::m_switchXY[]=
{
	0,0, 60,0, 60,36, 180,0, 180,36,

	//0,72, 60,72, 60,108, 
	60,92, 0,132,120,132,
	
	180,72, 180,108,
};

int CZahyoView::m_switchXY2[]=
{
	0,0, 1,1, 0,0,0,0,
	0,0, 1,1, 0,0,0,0,
	0,0, 1,1, 0,0,0,0,
	0,0, 1,1, 0,0,0,0,
	0,0, 1,1, 0,0,0,0,

	0,56,	24,48,	0,0,24,0,
	24,56,	40,24,	48,0,48,24,
	24,80,	40,24,	88,0,88,24
};

int CZahyoView::m_presetZahyo[][32]=
{
	{1,128, 35,128, 69,128, 103,128, 137,128, 171,128, 205,128},
	{1,176, 35,176, 69,176, 103,176, 137,176, 171,176, 205,176},
	{1,208, 35,208, 69,208, 103,208, 137,208, 171,208, 205,208},
};


CZahyoView::CZahyoView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("zahyoWindow");

//	SetResizeFlag();
	SetBlockSize(240,240);
	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
//	SetNonBlockArea(0,0,0,0);
//	SetFreeSizeFlag(1);
//	SetScrollFlag(TRUE,FALSE);

	SetWindowStyle();

	HICON hicon = LoadIcon(hinstance,MAKEINTRESOURCE(IDI_ICON_MDICHILDWINDOW));
	SendMessage(m_hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hicon);

	SetViewWindowText("À•W");


//	m_view->SetViewWindowText("À•W");
//	m_view->MoveViewWindow(1031,23,240,240);


//	m_document = pDocument;
//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_HSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_fontBitmap = new CMyBitmap("nnndir\\setup\\bmp\\zahyofont.bmp");
	m_fontBitmap = new CMyBitmap("nnndir\\setup\\bmp\\nekofont_s.bmp");
	//m_hFontBitmap = m_document->GetCommonBitmap(1);
	m_hFontBitmap = m_fontBitmap->GetHBitmap();

//	m_suuji = new CEditSuuji(m_hFontBitmap,16,24);
	m_suuji = new CEditSuuji(m_hFontBitmap,12,12);

	m_bitmap = new CMyBitmap("nnndir\\setup\\bmp\\zahyoswitch2.bmp");
	m_hBitmap = m_bitmap->GetHBitmap();
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_ZAHYOSWITCH));

	m_presetBitmap = new CMyBitmap("nnndir\\setup\\bmp\\zahyopresetswitch.bmp");
	m_hPresetBitmap = m_presetBitmap->GetHBitmap();


	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_PRESET));

//	m_switchKosuu = 10;
//	m_nowSelectSwitch = 7;

//	for (int i=0;i<m_switchKosuu;i++)
//	{
//		m_switchFlag[i] = FALSE;
//	}

//	m_switchFlag[5] = TRUE;
//	m_switchFlag[6] = TRUE;
//	m_switchFlag[7] = TRUE;

//	HWND hwnd = m_view->GetHWND();


//	HWND hwnd0 = GetDesktopWindow();
//	HDC hdc0 = GetDC(hwnd0);
//	int bpp = GetDeviceCaps(hdc0,BITSPIXEL);
//	ReleaseDC(hwnd0,hdc0);
//
//	m_directX = new CMyDirectX(m_hWnd,m_hInstance,640,480,bpp,FALSE);

//	m_hFontBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_NEKOFONT));

//EM_SETLIMITTEXT 
//wParam = (WPARAM) cbMax;     
//lParam = 0;



	//dummy
	
}






CZahyoView::~CZahyoView()
{
	End();
}

void CZahyoView::End(void)
{
//	ENDDELETECLASS(m_directX);

	ENDDELETECLASS(m_suuji);
//	DELETEGDIOBJECT(m_hFontBitmap);

//	DELETEGDIOBJECT(m_hBitmap);
	ENDDELETECLASS(m_bitmap);
	ENDDELETECLASS(m_fontBitmap);
	ENDDELETECLASS(m_presetBitmap);

}


LRESULT CZahyoView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HDC hdc;
	HDC src;
	HGDIOBJ old;

	CZahyoDoc* pDoc = (CZahyoDoc*)m_document;

	BOOL f;
	char mes[256];

	SIZE screenSize = pDoc->GetScreenSize();
	if (screenSize.cx < 1) screenSize.cx = 1;
	if (screenSize.cy < 1) screenSize.cy = 1;


	int zahyoPrintType = 0;

	switch (message)
	{
	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;

	case WM_CHAR:
		if (wParam == 19)
		{
//			OutputDebugString("Push Ctrl-S");
		}
		break;

	case WM_LBUTTONDOWN:
		OnClickLButton(wParam,lParam);
		break;
	case WM_RBUTTONDOWN:
		{
			if (pDoc->GetPresetButtonEnable())
			{
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				int n = GetOnPresetButton(x,y);
				if ((n>=0) && (n<7))
				{
					int cmd = OpenPopupMenu(wParam,lParam);
					if (cmd == ID_PRESET_GET)
					{
						CheckAndCopyZahyo(n);
					}
				}
			}
		}
		break;
//	case WM_COMMAND:
//		switch (wParam)
//		{
//		case ID_WINDOW_MAINSCREEN:
//			MessageBox(NULL,"b","a",MB_OK);
//			break;
//		}
//		break;

	case WM_ERASEBKGND :
		hdc = (HDC)wParam;
//		src = CreateCompatibleDC(hdc);
//		old = SelectObject(src,GetStockObject(BLACK_BRUSH));

		old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
		PatBlt(hdc,0,0,240,240,PATCOPY);
		SelectObject(hdc,old);

//		SelectObject(src,old);
//		DeleteDC(src);
		return TRUE;
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd,&ps);

		CKomaData* pKoma = m_document->GetNowSelectKoma();
		
		f = FALSE;
		if (pKoma != NULL)
		{
			int layer = pDoc->GetNowSelectLayer();
			if (layer != -1)
			{
				int eff = pKoma->GetEffect(layer);
				if (eff != -1)
				{
					int pic = pKoma->GetPictureNumber(layer);
					if (pic != -1)
					{
						f = TRUE;
					}
				}
			}
		}

		if (f == FALSE)
		{
			old = SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));
			PatBlt(hdc,0,0,240,240,PATCOPY);
			SelectObject(hdc,old);
			EndPaint(hWnd,&ps);
			return 0;
			break;
		}

		src = CreateCompatibleDC(hdc);
		old = SelectObject(src,m_hFontBitmap);

		if (pKoma != NULL)
		{
			int layer = pDoc->GetNowSelectLayer();


			if (layer != -1)
			{
				int eff = pKoma->GetEffect(layer);
				if (eff != -1)
				{
					int pic = pKoma->GetPictureNumber(layer);
					if (pic != -1)
					{
						LPSTR filename = pKoma->GetPicFileName(layer);
						if (filename != NULL)
						{
//							TextOut(hdc,0,144,filename,strlen(filename));
							TextOut(hdc,4,4,filename,strlen(filename));
						}

						zahyoPrintType = m_document->GetApp()->GetZahyoPrintType();

//						RECT srcPicRect;
//						pDoc->GetZahyoRect(0,&srcPicRect);


						if (m_hFontBitmap != NULL)
						{
							RECT rc;
							pDoc->GetZahyoRect(2,&rc);



							sprintf_s(mes,256,"%d X %d",rc.right,rc.bottom);
							m_suuji->PrintMessage(hdc,8,30,mes,src);

//							m_suuji->PrintMessage(hdc,8,30,"SIZEX:     ",src);
//							m_suuji->PrintMessage(hdc,8,58,"SIZEY:     ",src);
//							m_suuji->PrintMessage(hdc,8,180,"X:           ",src);
//							m_suuji->PrintMessage(hdc,8,208,"Y:           ",src);

						//	int j;
						//	m_suuji->PrintSuuji(hdc,8+16*7, 30,rc.right,4,src);
						//	m_suuji->PrintSuuji(hdc,8+16*7, 58,rc.bottom,4,src);

							if (zahyoPrintType)
							{
								AdjustCenter(&rc);
							}

							sprintf_s(mes,256,"%d , %d",rc.left,rc.top);
							int x = m_switchXY2[6*8+0] + m_switchXY2[6*8+2];
							int y = m_switchXY2[6*8+1];
							x += 8;
							y += 6;

							m_suuji->PrintMessage(hdc,x,y,mes,src);


//							m_suuji->PrintSuuji(hdc,8+16*3, 180,rc.left,4,src);
//							m_suuji->PrintSuuji(hdc,8+16*3, 208,rc.top,4,src);

							pDoc->GetZahyoRect(3,&rc);
							if (zahyoPrintType)
							{
								AdjustCenter(&rc);
							}

							sprintf_s(mes,256,"%d , %d",rc.left,rc.top);
							x = m_switchXY2[7*8+0] + m_switchXY2[7*8+2];
							y = m_switchXY2[7*8+1];
							x += 8;
							y += 6;
							m_suuji->PrintMessage(hdc,x,y,mes,src);

//							m_suuji->PrintSuuji(hdc,8+16*9, 180,rc.left,4,src);
//							m_suuji->PrintSuuji(hdc,8+16*9, 208,rc.top,4,src);
						}

						SelectObject(src,m_hBitmap);
						int i;
						int switchKosuu = pDoc->GetSwitchKosuu();
						int nowSelectSwitch = pDoc->GetSelectSwitch();

						for (i=5;i<=7;i++)
						{
							RECT rc;
							RECT rcTmp;

//							int px = m_switchXY[i*2];
//							int py = m_switchXY[i*2+1];
							int px = m_switchXY2[i*8];
							int py = m_switchXY2[i*8+1];
							int sx = m_switchXY2[i*8+2];
							int sy = m_switchXY2[i*8+3];
							rc.left = px;
							rc.top = py;
							rc.right = px + sx;
							rc.bottom = py + sy;
							int stype = 0;

//							int srcX = (i-5) * 120;
//							BOOL flg[12];
//							flg[5] = FALSE;
//							flg[6] = pDoc->CheckSwitchFlag(5);
//							flg[7] = flg[6];

							if (IntersectRect(&rcTmp,&rc,&ps.rcPaint))
							{
								//int srcY = 0;
								
								if (eff == EFFECT_NOP)
								{
									if ((nowSelectSwitch >= 5) && (nowSelectSwitch <= 7))
									{
										//srcY = 40;
										stype = 1;
									}
								}
								else
								{
									if ((i == nowSelectSwitch) || (nowSelectSwitch == 5))
									{
										//srcY = 40;
										stype = 1;
	//									if (i == nowSelectSwitch) srcX = 12;
									}
								}

								int srcX = m_switchXY2[i*8+4+stype*2];
								int srcY = m_switchXY2[i*8+5+stype*2];

								BitBlt(hdc,px,py,sx,sy,src,srcX,srcY,SRCCOPY);
							}
						}
						int j;
						int presetok;
						presetok = ((CZahyoDoc*)m_document)->GetPresetButtonEnable();

						SelectObject(src,m_hPresetBitmap);
						for (j=0;j<1;j++)
						{
							for (i=0;i<7;i++)
							{
								int sizeX = 32;
								int sizeY = 24;
								int x = m_presetZahyo[j][i*2+0];
								int y = m_presetZahyo[j][i*2+1];
								POINT delta = pDoc->GetPresetZahyo(i);

								int dx = (delta.x * 32) / screenSize.cx;
								int dy = (delta.y * 24) / screenSize.cy;
								int type = presetok;


								RECT rc;
								RECT rcTmp;

								rc.left = x;
								rc.top = y;
								rc.right = x + sizeX;
								rc.bottom = y + sizeY;
								if (IntersectRect(&rcTmp,&rc,&ps.rcPaint))
								{
									PrintPresetButton(hdc,src,x,y,dx,dy,type);
								}
							}
						}
					}
				}
			}

		}

		SelectObject(src,old);

		DeleteDC(src);

		EndPaint(hWnd,&ps);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}

void CZahyoView::AdjustCenter(RECT* lpRect)
{

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();
	int picSizeX = lpRect->right;
	int picSizeY = lpRect->bottom;
	int cx = screenSizeX / 2;
	int cy = screenSizeY / 2;
	int px = lpRect->left + picSizeX / 2;
	int py = lpRect->top + picSizeY / 2;
	int dx = px - cx;
	int dy = py - cy;
	lpRect->left = dx;
	lpRect->top = dy;


}

int CZahyoView::GetOnPresetButton(int x,int y)
{
	for (int j=0;j<1;j++)
	{
		for (int i=0;i<7;i++)
		{
			int dx = x - m_presetZahyo[j][i*2];
			int dy = y - m_presetZahyo[j][i*2+1];
			if ((dx>=0) && (dx<32) && (dy>=0) && (dy<24)) return i+j*7;
	
		}
	}

	return -1;
}

void CZahyoView::PrintPresetButton(HDC hdc,HDC src,int x,int y, int dx,int dy,int type)
{

	BitBlt(hdc,x,y,32,24,src,type*32,0,SRCCOPY);


	//clip
	int maskX = 1;
	int maskY = 1;
	int maskSrcX = 1;
	int maskSrcY = 1;
	int maskSizeX = 30;
	int maskSizeY = 22;
	maskX += dx;
	maskY += dy;
	if (maskX<1)
	{
		maskSizeX -= (1-maskX);
		maskSrcX += (1-maskX);
		maskX = 1;
		if (maskSizeX<=0) return;
	}
	if (maskX+maskSizeX>30)
	{
		maskSizeX -= (maskX+maskSizeX-30);
		if (maskSizeX<=0) return;
	}
	if (maskY<1)
	{
		maskSizeY -= (1-maskY);
		maskSrcY += (1-maskY);
		maskY = 1;
		if (maskSizeY<=0) return;
	}
	if (maskY+maskSizeY>22)
	{
		maskSizeY -= (maskY+maskSizeY-22);
		if (maskSizeY<=0) return;
	}

	maskSrcX += 32*type;
	maskSrcY += 24;

	//and
	BitBlt(hdc,x+maskX,y+maskY,maskSizeX,maskSizeY,src,maskSrcX,maskSrcY,SRCAND);


	//or
	maskSrcY += 24;
	BitBlt(hdc,x+maskX,y+maskY,maskSizeX,maskSizeY,src,maskSrcX,maskSrcY,SRCPAINT);
}

void CZahyoView::OnClickLButton(WPARAM wParam, LPARAM lParam)
{
	int mouseX = LOWORD(lParam);
	int mouseY = HIWORD(lParam);

	CZahyoDoc* pDoc = (CZahyoDoc*)m_document;
	int kosuu = pDoc->GetSwitchKosuu();

//	for (int i=0;i<kosuu;i++)
	for (int i=5;i<=7;i++)
	{
		int startX = m_switchXY2[i*8+0];
		int startY = m_switchXY2[i*8+1];
		int sizeX = m_switchXY2[i*8+2];
		int sizeY = m_switchXY2[i*8+3];
		int x = mouseX - startX;
		int y = mouseY - startY;
		if ((x>=0) && (x<sizeX) && (y>=0) && (y<sizeY))
		{
			pDoc->OnClickSwitch(i);
			return;
		}
	}

	int presetButton = GetOnPresetButton(mouseX,mouseY);
	if ((presetButton >= 0) && (presetButton < 7))
	{
		CheckAndPresetZahyo(presetButton);
	}
}

void CZahyoView::CheckAndPresetZahyo(int n)
{
	CZahyoDoc* pDoc = (CZahyoDoc*)m_document;
	CKomaData* pKoma = m_document->GetNowSelectKoma();

	int zahyoPrintType = m_document->GetApp()->GetZahyoPrintType();

	BOOL f = FALSE;
	int layer;
	if (pKoma != NULL)
	{
		layer = pDoc->GetNowSelectLayer();
		if (layer != -1)
		{
			int eff = pKoma->GetEffect(layer);
			if (eff != -1)
			{
				int pic = pKoma->GetPictureNumber(layer);
				if (pic != -1)
				{
					LPSTR filename = pKoma->GetPicFileName(layer);
					if (filename != NULL)
					{
						f = TRUE;
					}
				}
			}
		}
	}

	if (f)
	{
		int nowSelectSwitch = pDoc->GetSelectSwitch();
		RECT rc;


		if ((nowSelectSwitch == 5) || (nowSelectSwitch == 6))
		{
			pDoc->GetZahyoRect(2,&rc);
			POINT pt = pDoc->GetPresetZahyo(n);

			if (zahyoPrintType)
			{
				int screenSizeX = CMyGraphics::GetScreenSizeX();
				int screenSizeY = CMyGraphics::GetScreenSizeY();
				pt.x += screenSizeX / 2;
				pt.y += screenSizeY / 2;
				pt.x -= rc.right / 2;
				pt.y -= rc.bottom / 2;

			}


			rc.left = pt.x;
			rc.top = pt.y;
			pKoma->SetEffectRect(&rc,layer,1 | 2);
			pDoc->SetZahyoRect(2,&rc);
			pDoc->UpdateMyWindow();
			pDoc->UpdateMainWindow();
			//from
		}

		if ((nowSelectSwitch == 5) || (nowSelectSwitch == 7))
		{
			pDoc->GetZahyoRect(3,&rc);
			POINT pt = pDoc->GetPresetZahyo(n);
			if (zahyoPrintType)
			{
				int screenSizeX = CMyGraphics::GetScreenSizeX();
				int screenSizeY = CMyGraphics::GetScreenSizeY();
				pt.x += screenSizeX / 2;
				pt.y += screenSizeY / 2;
				pt.x -= rc.right / 2;
				pt.y -= rc.bottom / 2;

			}

			rc.left = pt.x;
			rc.top = pt.y;
			pKoma->SetEffectRect(&rc,layer,1 | 4);
			pDoc->SetZahyoRect(3,&rc);
			pDoc->UpdateMyWindow();
			pDoc->UpdateMainWindow();
			//to
		}
	}
}

void CZahyoView::CheckAndCopyZahyo(int n)
{
	CZahyoDoc* pDoc = (CZahyoDoc*)m_document;
	CKomaData* pKoma = m_document->GetNowSelectKoma();
		
	int zahyoPrintType = m_document->GetApp()->GetZahyoPrintType();

	BOOL f = FALSE;
	if (pKoma != NULL)
	{
		int layer = pDoc->GetNowSelectLayer();
		if (layer != -1)
		{
			int eff = pKoma->GetEffect(layer);
			if (eff != -1)
			{
				int pic = pKoma->GetPictureNumber(layer);
				if (pic != -1)
				{
					LPSTR filename = pKoma->GetPicFileName(layer);
					if (filename != NULL)
					{
						f = TRUE;
					}
				}
			}
		}
	}

	if (f)
	{
		int nowSelectSwitch = pDoc->GetSelectSwitch();
		RECT rc;
		POINT pt;
		BOOL getFlag = FALSE;

		if ((nowSelectSwitch == 5) || (nowSelectSwitch == 6))
		{
			pDoc->GetZahyoRect(2,&rc);
			pt.x = rc.left;
			pt.y = rc.top;
			getFlag = TRUE;

			//from
		}
		else if ((nowSelectSwitch == 5) || (nowSelectSwitch == 7))
		{
			pDoc->GetZahyoRect(2,&rc);
			pt.x = rc.left;
			pt.y = rc.top;
			getFlag = TRUE;
			//to
		}

		if (getFlag)
		{
			if (zahyoPrintType)
			{
				int screenSizeX = CMyGraphics::GetScreenSizeX();
				int screenSizeY = CMyGraphics::GetScreenSizeY();

				int xx = pt.x + rc.right / 2;
				int yy = pt.y + rc.bottom / 2;
				xx -= screenSizeX / 2;
				yy -= screenSizeY / 2;
				pt.x = xx;
				pt.y = yy;
			}

			pDoc->SetPresetZahyo(n,pt);
		}
	}
}

BOOL CZahyoView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = ZAHYO_WINDOW;

	int subType = -1;
	for (int i=5;i<=7;i++)
	{
		int startX = m_switchXY2[i*8+0];
		int startY = m_switchXY2[i*8+1];
		int sizeX = m_switchXY2[i*8+2];
		int sizeY = m_switchXY2[i*8+3];
		int dx = x - startX;
		int dy = y - startY;
		if ((dx>=0) && (dx<sizeX) && (dy>=0) && (dy<sizeY))
		{
			subType = i;
			break;
		}
	}


	int n = GetOnPresetButton(x,y);
	if ((n>=0) && (n<7))
	{
		subType = 10+n;
	}


	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}

