//
// myview.cpp
//

#include <windows.h>
#include <stdio.h>

//#include <winuser.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\myGraphics.h"
#include "..\..\systemNNN\nyanLib\include\picture.h"
#include "define.h"

#include "case.h"
//#include "..\cyclib\effect.h"
#include "myapplicationBase.h"
#include "mybutton.h"

#include "mydocument.h"
//#include "mytoolbar.h"

#include "myview.h"

//#include "myapplicationBase.h"


int CMyView::m_viewKosuu = 0;
HWND CMyView::m_viewHWNDTable[VIEW_KOSUU_MAX];
CMyView* CMyView::m_viewTable[VIEW_KOSUU_MAX];



CMyView::CMyView(CMyDocument* pDocument, HWND clientHWND,HINSTANCE hinstance)
{
	m_windowName[0] = 0;


	m_document = pDocument;
//	m_clientHWND = m_document->GetClientHWND();
	m_clientHWND = clientHWND;
//	m_hInstance = m_document->GetMyInstance();
	m_hInstance = hinstance;

	m_freeSizeFlag[0] = FALSE;
	m_freeSizeFlag[1] = FALSE;

	m_scrollFlag[0] = FALSE;
	m_scrollFlag[1] = FALSE;

	m_buttonForCheckNumber = 0;
	for (int i=0;i<64;i++)
	{
		m_buttonForCheck[i] = NULL;
	}


	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

//	int screenSizeX = CPicture::m_screenSizeX;
//	int screenSizeY = CPicture::m_screenSizeY;

	SetResizeFlag(FALSE);
	SetBlockSize(screenSizeX,screenSizeY);
	SetNonBlockArea(0,0,0,0);
	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
	SetScrollFlag(FALSE,FALSE);
	SetBlockKosuu(1,1);

//	m_myToolBar = NULL;

	for (int i=0;i<10;i++)
	{
		m_popupMenu[i] = NULL;
	}

//	RECT rc;
//	BOOL flg;
//	m_document->GetInitWindowZahyo(&rc,&flg);


	m_windowX = 0;
	m_windowY = 0;
	m_windowSizeX = screenSizeX;
	m_windowSizeY = screenSizeY;
//	m_clientX = 0;
//	m_clientY = 0;
//	m_clientSizeX = 640;
//	m_clientSizeY = 480;

	m_hWnd = NULL;

	if (m_viewKosuu>=VIEW_KOSUU_MAX) return;

	
	MDICREATESTRUCT mdic;

//	mdic.szClass = childClassName;
	mdic.szClass = MYCHILDCLASSNAME;
	mdic.szTitle = "MDI SUB WINDOW";
//	mdic.hOwner  = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	mdic.hOwner  = m_hInstance;

	mdic.x  = mdic.y  = CW_USEDEFAULT;
	mdic.cx = mdic.cy = CW_USEDEFAULT;
	mdic.style   = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	mdic.style &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	mdic.lParam  = (LPARAM)0;
	m_number = m_viewKosuu;
	m_viewTable[m_number] = this;
	SendMessage(m_clientHWND,WM_MDICREATE ,0,(LPARAM)&mdic);
	m_hWnd = m_viewHWNDTable[m_number];
	m_viewKosuu++;

//		lpmdic->style &= ~WS_MINIMIZE;

//
//GWL_EXSTYLE
//WS_EX_TOOLWINDOW

//	ShowWindow(m_hWnd,SW_SHOW);

	m_showFlag = TRUE;
}



CMyView::~CMyView()
{
	End();
}


void CMyView::End(void)
{
	for (int i=0;i<10;i++)
	{
		if (m_popupMenu[i] != NULL)
		{
			DestroyMenu(m_popupMenu[i]);
			m_popupMenu[i] = NULL;
		}
	}

	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
		m_viewHWNDTable[m_number] = NULL;
		m_viewTable[m_number] = NULL;
	}
}


LRESULT CALLBACK CMyView::staticViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		MDICREATESTRUCT* lpmdic = (MDICREATESTRUCT*)lParam;
		m_viewHWNDTable[m_viewKosuu] = hWnd;
		return 0;
	}

	int found = -1;
	for (int i=0;i<m_viewKosuu;i++)
	{
		if (m_viewHWNDTable[i] == hWnd)
		{
			found = i;
			break;
		}
	}


	if (found != -1)
	{
		CMyView* pView = m_viewTable[found];
		if (pView != NULL)
		{
			if (message == WM_CLOSE)
			{
//				pView->Show(FALSE);
				(pView->GetMyDocument())->OnCloseButton();
			}

			if (message == WM_MOVE)
			{
				pView->WindowIsMoved((int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam));
			}

			if (message == WM_SIZE)
			{
				if (1)
				{
					pView->WindowSizeIsChanged((int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam));
				}
			}


			if (message == WM_SIZING)
			{
				pView->AdjustWindowSize(wParam,(LPRECT)lParam);
			}


//			switch (message)
//			{
//			case WM_LBUTTONDOWN:
//			case WM_LBUTTONUP:
//			case WM_MOUSEMOVE:
//				pView->OnToolButton(message,wParam,lParam);
//				break;
//			default:
//				break;
//			}

			if (message == WM_MOUSEMOVE)
			{
				CMyDocument* pDoc = pView->GetMyDocument();
				if (pDoc != NULL)
				{
					int xPos = LOWORD(lParam); 
					int yPos = HIWORD(lParam);
					POINT pt0;
					pt0.x = xPos;
					pt0.y = yPos;

					ClientToScreen(pView->m_hWnd,&pt0);
					pView->MoveMouse(xPos,yPos,pt0);
				}
			}

			if ((message == WM_LBUTTONDOWN) || (message == WM_RBUTTONDOWN))
			{
				CMyDocument* pDoc = pView->GetMyDocument();
				if (pDoc != NULL)
				{
					pDoc->PreClickMouse(pView->m_number);
//					CMyApplication* pApp = pDoc->GetApp();
//					if (pApp != NULL)
//					{
//						pApp->PreClickMouse(pView->m_number);
//					}

					pView->EraseBalloon();
				}
			}

			if (message == WM_LBUTTONDOWN)
			{
				SetCapture(hWnd);
				pView->m_captureFlag = TRUE;
			}

			if (message == WM_LBUTTONUP)
			{
				if (pView->m_captureFlag)
				{
					ReleaseCapture();
					pView->m_captureFlag = FALSE;
				}
			}

			if (message == WM_LBUTTONDBLCLK)
			{
			}

			if (message == WM_CHAR)
			{
				//F:6 S:19 C:3 X:24 V:22
				if ((wParam >=1) && (wParam <= 26))
				{
	//	char mes[256];
	//	sprintf_s(mes,"[ key=%d]",wParam);
	//	OutputDebugString(mes);

					if (pView->OnControlKey(wParam,lParam) == FALSE)
					{
						CMyDocument* pDoc = pView->GetMyDocument();
						int wn = pDoc->GetWindowNumber();
						pDoc->GetApp()->OnControlKey(wn,wParam,lParam);


					}


				}
				if (wParam == VK_RETURN)
				{
					pView->OnEnterKey();
				}
			}

			if (message == WM_KEYDOWN)
			{
				if (wParam == VK_UP)
				{
					pView->OnUpKey();
				}
				if (wParam == VK_DOWN)
				{
					pView->OnDownKey();
				}
				if (wParam == VK_LEFT)
				{
					pView->OnLeftKey();
				}
				if (wParam == VK_RIGHT)
				{
					pView->OnRightKey();
				}

				if (wParam == VK_PRIOR)
				{
					pView->OnPageUp();
				}
				if (wParam == VK_NEXT)
				{
					pView->OnPageDown();
				}

				if (wParam == VK_RETURN)
				{
		//			pView->OnEnterKey();
				}

				if (wParam == VK_DELETE)
				{
					pView->OnDeleteKey();
				}

				if (wParam == VK_INSERT)
				{
					pView->OnInsertKey();
				}

				if (wParam == VK_SPACE)
				{
					pView->OnSpaceKey();
				}

				if (wParam == VK_ESCAPE)
				{
					pView->OnEscapeKey();
				}

				if (wParam == VK_TAB)
				{
					//check shift
					int shift = 0;
					if (GetAsyncKeyState(VK_SHIFT) & 0x80000000) shift = 1;
					pView->OnTabKey(shift);
				}

				if ((wParam >= VK_F1) && (wParam <= VK_F12))
				{
					pView->OnFunctionKey(wParam - VK_F1 + 1);
				}
			}

			if (message == WM_DESTROY)
			{
				pView->m_hWnd = NULL;
			}

			return pView->ViewWndProc(hWnd,message,wParam,lParam);
		}
	}
	

	return DefMDIChildProc(hWnd,message,wParam,lParam);
//	return DefWindowProc(hWnd,message,wParam,lParam);
}

void CMyView::SetHWND(HWND hwnd)
{
	m_hWnd = hwnd;
}

HWND CMyView::GetHWND(void)
{
	return m_hWnd;
}


//LRESULT CMyView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{



//	if (message == WM_MOVE)
//	{
//		int hhh = 0;
//		hhh++;
//	}
//	return DefMDIChildProc(hWnd,message,wParam,lParam);
//	return DefWindowProc(hWnd,message,wParam,lParam);
//}


BOOL CMyView::SetViewWindowText(LPSTR text)
{
	if (m_hWnd == NULL) return FALSE;
	return SetWindowText(m_hWnd,text);
}

BOOL CMyView::MoveViewWindow(RECT* lpRect)
{
	return MoveViewWindow(lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);
}

void CMyView::WindowIsMoved(int x, int y)
{
	m_windowX = x;
	m_windowY = y;

	m_document->WindowIsMoved(x,y);

	CMyDocument* pDoc = GetMyDocument();
	if (pDoc != NULL)
	{
		POINT dmy;
		dmy.x = 0;
		dmy.y = 0;
		POINT dmy2;
		dmy2.x = 0;
		dmy2.y = 0;

		pDoc->GetApp()->OnBalloonArea(-2,dmy,0,dmy2);
	}

}


void CMyView::WindowSizeIsChanged(int sizeX, int sizeY)
{
	m_windowSizeX = sizeX;
	m_windowSizeY = sizeY;
//char mes[256];
//sprintf(mes,"[y=%d**]",sizeY);
//OutputDebugString(mes);

	//scroll para changed


//OutputDebugString("+++");
	ReCalcuScrollPara();	//???


	m_document->WindowSizeIsChanged(sizeX,sizeY);
}


BOOL CMyView::MoveViewWindow(int wx, int wy, int sizeX, int sizeY)
{
	if (m_hWnd == NULL) return FALSE;


	if (false)
	{
		RECT rc;
		rc.left = wx;
		rc.top = wy;
		rc.right = wx + sizeX;
		rc.bottom = wy + sizeY;

		int nonFullFlag = 0xffffffff;
		DWORD style = ((WS_OVERLAPPED |
			WS_CAPTION |
			WS_SYSMENU |
			//								WS_THICKFRAME |
			WS_MAXIMIZEBOX |
			WS_MINIMIZEBOX
			)&nonFullFlag) |
			WS_POPUP |
			WS_VISIBLE
			;
		DWORD exStyle = 0;
		AdjustWindowRectEx(&rc, style, false, exStyle);

		wx = rc.left;
		wy = rc.top;
		sizeX = rc.right - rc.left;
		sizeY = rc.bottom - rc.top;

		m_windowX = wx;
		m_windowY = wy;
		m_windowSizeX = sizeX;
		m_windowSizeY = sizeY;

		return MoveWindow(m_hWnd, wx, wy, sizeX, sizeY, TRUE);
	}


	m_windowX = wx;
	m_windowY = wy;
	m_windowSizeX = sizeX;
	m_windowSizeY = sizeY;






//char mes[256];
//sprintf(mes,"[y=%d***]",sizeY);
//OutputDebugString(mes);

	if (m_resizeFlag)
	{
		sizeX += 2*GetSystemMetrics(SM_CXSIZEFRAME);
		sizeY += 2*GetSystemMetrics(SM_CYSIZEFRAME);
		wx -= GetSystemMetrics(SM_CXSIZEFRAME );
		wy -= GetSystemMetrics(SM_CYSIZEFRAME );
	}
	else
	{
		sizeX += 2*GetSystemMetrics(SM_CXFIXEDFRAME);
		sizeY += 2*GetSystemMetrics(SM_CYFIXEDFRAME);
		wx -= GetSystemMetrics(SM_CXFIXEDFRAME );
		wy -= GetSystemMetrics(SM_CYFIXEDFRAME );
	}
	
	if (m_scrollFlag[0])
	{
		sizeX += GetSystemMetrics(SM_CXVSCROLL);
	}

	if (m_scrollFlag[1])
	{
		sizeY += GetSystemMetrics(SM_CYHSCROLL);
	}

	sizeY += GetSystemMetrics(SM_CYCAPTION);
	wy -= GetSystemMetrics(SM_CYCAPTION);

	return MoveWindow(m_hWnd,wx,wy,sizeX,sizeY,TRUE);
}



void CMyView::GetWindowZahyo(RECT* lpRect)
{
	lpRect->left = m_windowX;
	lpRect->top = m_windowY;
	lpRect->right = m_windowSizeX;
	lpRect->bottom = m_windowSizeY;
}

BOOL CMyView::Show(BOOL bFlag)
{
	m_showFlag = bFlag;

	if (bFlag)
	{
		return ShowWindow(m_hWnd,SW_SHOW);
	}
	else
	{
		return ShowWindow(m_hWnd,SW_HIDE);
	}
}


CMyDocument* CMyView::GetMyDocument(void)
{
	return m_document;
}

void CMyView::MyInvalidateRect(RECT* lpRect,BOOL bErase)
{
	if (m_hWnd == NULL) return;

	InvalidateRect(m_hWnd,lpRect,bErase);
}


int CMyView::OpenPopupMenu(WPARAM wParam,LPARAM lParam,int nPos1, int nPos2,int n)
{
	if (m_popupMenu[n] == NULL) return 0;

	HMENU submenu = GetSubMenu(m_popupMenu[n],nPos1);
	if (nPos2 != -1)
	{
		submenu = GetSubMenu(submenu,nPos2);
	}

	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	ClientToScreen(m_hWnd, &pt);

	return TrackPopupMenu(submenu,TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,pt.x,pt.y,0,m_hWnd,NULL);
}


HBITMAP CMyView::GetCommonBitmap(int n)
{
	if (m_document == NULL) return NULL;
	return m_document->GetCommonBitmap(n);
}


void CMyView::EraseSplit(HDC hdc, int col)
{
	EraseUpper(hdc);
	EraseLower(hdc,col);
}


void CMyView::EraseUpper(HDC hdc, int col)
{
	if (col == -1) col = LTGRAY_BRUSH;
	RECT rc;

	GetClientRect(m_hWnd,&rc);

//	GetWindowZahyo(&rc);
	HGDIOBJ old = SelectObject(hdc,GetStockObject(col));
	PatBlt(hdc,0,0,rc.right,24,PATCOPY);
	SelectObject(hdc,old);
}


void CMyView::EraseLower(HDC hdc, int col)
{
	if (col == -1) col = WHITE_BRUSH;
	RECT rc;

	GetClientRect(m_hWnd,&rc);

//	GetWindowZahyo(&rc);

	HGDIOBJ old = SelectObject(hdc,GetStockObject(col));
	PatBlt(hdc,0,24,rc.right,rc.bottom - 24,PATCOPY);
	SelectObject(hdc,old);
}

/*
void CMyView::OnToolButton(UINT message,WPARAM wParam,LPARAM lParam)
{
	if (m_myToolBar == NULL) return;

	m_myToolBar->Calcu(message,wParam,lParam);
}
*/

void CMyView::SetWindowName(LPSTR name)
{
	if (m_hWnd != NULL)
	{
		SetWindowText(m_hWnd,name);
	}
}


int CMyView::AdjustWindowSizeX(int x)
{
	return x;
}

int CMyView::AdjustWindowSizeY(int y)
{
	return y;
}


void CMyView::SetFreeSizeFlag(int vh,BOOL flg)
{
	if ((vh != 0) && (vh != 1)) return;
	m_freeSizeFlag[vh] = flg;
}


void CMyView::SetResizeFlag(BOOL flg)
{
	m_resizeFlag = flg;
}

void CMyView::SetBlockKosuuMax(int nx, int ny)
{
	SIZE sz = m_document->GetDesktopSize();
	SIZE sz2 = CalcuWindowSize(0,0);

	int x = sz.cx - sz2.cx;
	int y = sz.cy - sz2.cy;

	int maxX = 9999;
	if (m_blockSizeX>0)
	maxX = x / m_blockSizeX;

	int maxY = 9999;
	if (m_blockSizeY>0)
	maxY = y / m_blockSizeY;

	if (nx>maxX) nx = maxX;
	if (ny>maxY) ny = maxY;

	m_blockKosuuMax[0] = ny;
	m_blockKosuuMax[1] = nx;
}

void CMyView::SetBlockKosuuMin(int nx, int ny)
{
	m_blockKosuuMin[0] = ny;
	m_blockKosuuMin[1] = nx;
}

void CMyView::SetBlockKosuu(int nx, int ny)
{
	if (nx>m_blockKosuuMax[1]) nx = m_blockKosuuMax[1];
	if (ny>m_blockKosuuMax[0]) ny = m_blockKosuuMax[0];

	if (nx<m_blockKosuuMin[1]) nx = m_blockKosuuMin[1];
	if (ny<m_blockKosuuMin[0]) ny = m_blockKosuuMin[0];


	m_blockKosuu[0] = ny;
	m_blockKosuu[1] = nx;
}

void CMyView::SetScrollFlag(BOOL hFlag, BOOL vFlag)
{
	m_scrollFlag[0] = vFlag;
	m_scrollFlag[1] = hFlag;
}



SIZE CMyView::CalcuWindowSize(int nx, int ny)
{
	int sx = nx * m_blockSizeX;
	int sy = ny * m_blockSizeY;

	int fx = 0;
	int fy = 0;

	if (m_resizeFlag)
	{
		fx = GetSystemMetrics(SM_CXSIZEFRAME);
		fy = GetSystemMetrics(SM_CYSIZEFRAME);
	}
	else
	{
		fx = GetSystemMetrics(SM_CXFIXEDFRAME);
		fy = GetSystemMetrics(SM_CYFIXEDFRAME);
	}

	sx += fx*2;
	sy += fy*2;

	sy += GetSystemMetrics(SM_CYCAPTION);

	if (m_scrollFlag[0])
	{
		sx += GetSystemMetrics(SM_CXVSCROLL);
	}

	if (m_scrollFlag[1])
	{
		sy += GetSystemMetrics(SM_CYHSCROLL);
	}

	sx += m_leftArea;
	sx += m_rightArea;
	sy += m_topArea;
	sy += m_bottomArea;

	SIZE size;
	size.cx = sx;
	size.cy = sy;

	return size;
}


void CMyView::AdjustWindowSize(WPARAM fwFlag, LPRECT lpRect)
{
	int sx = lpRect->right - lpRect->left;
	int sy = lpRect->bottom - lpRect->top;
	int x = lpRect->left;
	int y = lpRect->top;
	int ex = lpRect->right;
	int ey = lpRect->bottom;


	SIZE minSize = CalcuWindowSize(m_blockKosuuMin[1],m_blockKosuuMin[0]);
	SIZE maxSize = CalcuWindowSize(m_blockKosuuMax[1],m_blockKosuuMax[0]);

	SIZE amariSize = CalcuWindowSize(1,1);
	amariSize.cx -= m_blockSizeX;
	amariSize.cy -= m_blockSizeY;


	if (m_scrollFlag[0])
	{
//		sx -= GetSystemMetrics(SM_CXVSCROLL);
	}

	if (m_scrollFlag[1])
	{
//		sy -= GetSystemMetrics(SM_CYHSCROLL);
	}



	int leftright = m_leftArea + m_rightArea;
	int topbottom = m_topArea + m_bottomArea;




	int blockKosuuX = (sx - amariSize.cx + m_blockSizeX/2) / m_blockSizeX;
	int blockKosuuY = (sy - amariSize.cy + m_blockSizeY/2) / m_blockSizeY;




	if (m_freeSizeFlag[1] == FALSE)
	{
		sx = m_blockSizeX * blockKosuuX + amariSize.cx;
		if (sx<minSize.cx) sx = minSize.cx;
		if (sx>maxSize.cx) sx = maxSize.cx;
	}

	if (m_freeSizeFlag[0] == FALSE)
	{
		sy = m_blockSizeY * blockKosuuY + amariSize.cy;
		if (sy<minSize.cy) sy = minSize.cy;
		if (sy>maxSize.cy) sy = maxSize.cy;


//char mes9[256];
//sprintf(mes9,"[sy=%d]\n",sy);
//OutputDebugString(mes9);
	}

//int sy1 = sy;

	//adjust x
	if ((fwFlag == WMSZ_TOPLEFT) || (fwFlag == WMSZ_LEFT) || (fwFlag == WMSZ_BOTTOMLEFT))
	{
		x = ex - sx;
	}
	else
	{
		ex = x + sx;
	}

	//adjust y
	if ((fwFlag == WMSZ_TOPLEFT) || (fwFlag == WMSZ_TOP) || (fwFlag == WMSZ_TOPRIGHT))
	{
//OutputDebugString("@");
		y = ey - sy;
	}
	else
	{
//OutputDebugString("*");
		ey = y + sy;
	}

//	sprintf(mes,"[%d %d %d %d]",x,y,sx,sy);
//	OutputDebugString(mes);

	if (m_resizeFlag)
	{
		m_windowX = x + GetSystemMetrics(SM_CXSIZEFRAME);
		m_windowY = y + GetSystemMetrics(SM_CYSIZEFRAME);
//OutputDebugString("=");
	}
	else
	{
		m_windowX = x + GetSystemMetrics(SM_CXFIXEDFRAME);
		m_windowY = y + GetSystemMetrics(SM_CYFIXEDFRAME);
//OutputDebugString("-");
	}
	m_windowY += GetSystemMetrics(SM_CYCAPTION);

	m_windowSizeX = ex - x - amariSize.cx + leftright;
	m_windowSizeY = ey - y - amariSize.cy + topbottom;



//char mes[256];
//sprintf(mes,"[resize Y = %d]",m_windowSizeY);
//OutputDebugString(mes);

//sprintf(mes,"[sy0=%d sy1=%d y=%d* amariY=%d blockKosuuY=%d blockSizeY=%d]",sy0,sy1,m_windowSizeY,amariSize.cy,blockKosuuY,m_blockSizeY);
//OutputDebugString(mes);


	//スクロール関連のパラメーターの再設定






	SetRect(lpRect,x,y,ex,ey);
}


void CMyView::SetBlockSize(int sizeX,int sizeY)
{
	m_blockSizeX = sizeX;
	m_blockSizeY = sizeY;

}



void CMyView::SetNonBlockArea(int left,int top, int right ,int bottom)
{
	m_leftArea = left;
	m_topArea = top;
	m_rightArea = right;
	m_bottomArea = bottom;
}


void CMyView::SetWindowStyle(void)
{
	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
	if (m_resizeFlag)
	{
		style |= WS_SIZEBOX;
	}
	else
	{
		style &= (~WS_SIZEBOX);
	}


	if (m_scrollFlag[0])
	{
		style |= WS_VSCROLL;
	}
	else
	{
		style &= (~WS_VSCROLL);
	}

	if (m_scrollFlag[1])
	{
		style |= WS_HSCROLL;
	}
	else
	{
		style &= (~WS_HSCROLL);
	}

	SetWindowLong(m_hWnd,GWL_STYLE,style);
}


void CMyView::InitWindowZahyo(void)
{
	RECT rc;
	BOOL flg;
	m_document->GetInitWindowZahyo(m_windowName,&rc,&flg);
	MoveViewWindow(&rc);
}



//virtual
void CMyView::ReCalcuScrollPara(void)
{
}

void CMyView::SetScrollArea(int startX, int startY, int endX, int endY)
{
}

void CMyView::SetScrollPara(int pos, int posMax , int page , int vh)
{
}

int CMyView::GetBlockKosuu(int vh)
{
	return 1;
}

void CMyView::SetMyViewWindowName(LPSTR name)
{
	if (name == NULL) return;
	int ln = strlen(name);
	if (ln>=62)
	{
		MessageBox(NULL,"Bad Window Name","ERROR",MB_OK);
		ln = 62;
	}
	memcpy(m_windowName,name,ln);
	m_windowName[ln] = 0;
}


LPSTR CMyView::GetMyViewWindowName(void)
{
	return m_windowName;
}

void CMyView::OnLeftKey(void)
{
	m_document->OnLeftKey();
}

void CMyView::OnRightKey(void)
{
	m_document->OnRightKey();
}

void CMyView::OnUpKey(void)
{
	m_document->OnUpKey();
}

void CMyView::OnDownKey(void)
{
	m_document->OnDownKey();
}

void CMyView::OnPageUp(void)
{
	m_document->OnPageUp();
}

void CMyView::OnPageDown(void)
{
	m_document->OnPageDown();
}

void CMyView::OnEnterKey(void)
{
	m_document->OnEnterKey();
}

void CMyView::OnDeleteKey(void)
{
	m_document->OnDeleteKey();
}

void CMyView::OnInsertKey(void)
{
	m_document->OnInsertKey();
}

void CMyView::OnSpaceKey(void)
{
	m_document->OnSpaceKey();
}

void CMyView::OnEscapeKey(void)
{
	m_document->OnEscapeKey();
}

void CMyView::OnTabKey(int shift)
{
	m_document->OnTabKey(shift);
}

void CMyView::OnFunctionKey(int n)
{
	m_document->OnFunctionKey(n);
}


BOOL CMyView::MoveMouse(int x,int y,POINT screenPos)
{
	return FALSE;
}

void CMyView::EraseBalloon(void)
{
	CMyDocument* pDoc = GetMyDocument();
	if (pDoc != NULL)
	{
		POINT dmy;
		dmy.x = 0;
		dmy.y = 0;
		POINT dmy2;
		dmy2.x = 0;
		dmy2.y = 0;

		pDoc->GetApp()->OnBalloonArea(-1,dmy,0,dmy2);
	}
}

int CMyView::CheckOnBalloonButton(int x,int y)
{

	for (int i=0;i<m_buttonForCheckNumber;i++)
	{
		/*
		FILE* f = NULL;
		fopen_s(&f,"log.txt","ab+");
		char mes[256];
		sprintf_s(mes,256,"[%d:max(%d) mode=%s]",i,m_buttonForCheckNumber,m_windowName);
		fwrite(mes,sizeof(char),strlen(mes),f);
		fclose(f);
		*/

		CMyButton* button = m_buttonForCheck[i];
		if (button != NULL)
		{
			if (button->CheckOn(x,y)) return i;
		}
	}

	return -1;
}

void CMyView::AddBalloonCheckButton(CMyButton* button)
{
	int n = button->GetNumber();
	SetBalloonCheckButton(n,button);
}

void CMyView::SetBalloonCheckButton(int n,CMyButton* button)
{
	if ((n>=0) && (n<64))
	{
		m_buttonForCheck[n] = button;
		if (n+1 > m_buttonForCheckNumber)
		{
/*
		FILE* f = NULL;
		fopen_s(&f,"log.txt","ab+");
		char mes[256];
		sprintf_s(mes,256,"[exp button %d:max(%d)]",n+1,m_buttonForCheckNumber);
		fwrite(mes,sizeof(char),strlen(mes),f);
		fclose(f);
*/
			m_buttonForCheckNumber = n + 1;
		}
	}
}

BOOL CMyView::OnControlKey(WPARAM wParam,LPARAM lParam)
{
	return FALSE;
}


/*_*/

