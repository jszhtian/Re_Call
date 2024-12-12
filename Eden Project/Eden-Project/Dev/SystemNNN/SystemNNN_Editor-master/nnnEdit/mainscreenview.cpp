//
// mainscreenview.cpp
//


#include <windows.h>
#include <Math.h>
#include <stdio.h>


#include "resource.h"


#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\myGraphics.h"
#include "..\..\systemNNN\nyanLib\include\picture.h"
#include "..\..\systemNNN\nyanLib\include\allGeo.h"


#include "..\..\systemNNN\nyanDirectXLib\mydirectDraw.h"
#include "..\..\systemNNN\nyanDirectXLib\mmx.h"

//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nnnUtilLib\myMessage.h"

#include "case.h"
#include "messageData.h"
#include "myapplicationBase.h"

#include "windowList.h"

#include "effectParam.h"

#include "case.h"

#include "komaData.h"

#include "mydocument.h"

//#include "effect.h"


//#include "myapplicationBase.h"

#include "mainscreenview.h"
#include "mainscreendoc.h"


int CMainScreenView::m_gazeData[][4]=
{
	{-4,-4,264,24},//
	{-4,-4,264,24},//
	{-4,-4,264,24},//
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-4,-4,264,24},//red
	{-4,-4,264,24},//green
	{-4,-4,264,24},//blue
	{-4,-4,264,24},//d red
	{-4,-4,264,24},//d green
	{-4,-4,264,24},//d blue
	{-4,-4,264,24},//trans
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
	{-1,0,258,30},//dmy
};


int CMainScreenView::m_arrowDeltaTable[10]=
{
	-19999,-100,-10,-1,0,1,10,100,19999,
};


CMainScreenView::CMainScreenView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("mainScreenWindow");


	m_message = ((CMainScreenDoc*)m_document)->GetMyMessage();

//	SetResizeFlag();

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	SetBlockSize(screenSizeX,screenSizeY);

	m_waitVSync = 0;


	m_leftRight = 0;
	m_topBottom = 0;

	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
//	SetNonBlockArea(0,0,0,0);
//	SetFreeSizeFlag(1);
//	SetScrollFlag(FALSE,TRUE);


	SetWindowStyle();

	SetViewWindowText("画面");



	GetEffectControlAndPara();

//	m_document = pDocument;
//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	SetWindowLong(m_hWnd,GWL_STYLE,style);
//	HWND hwnd = m_view->GetHWND();


	HWND hwnd0 = GetDesktopWindow();
	HDC hdc0 = GetDC(hwnd0);
	m_bpp = GetDeviceCaps(hdc0,BITSPIXEL);
//	int wx = GetDeviceCaps(hdc0,HORZRES);
//	int wy = GetDeviceCaps(hdc0,VERTRES);

	int desktopWindowSizeX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int desktopWindowSizeY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int desktopWindowStartX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int desktopWindowStartY = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int desktopWindowEndX = desktopWindowStartX + desktopWindowSizeX;
	int desktopWindowEndY = desktopWindowStartY + desktopWindowSizeY;


	ReleaseDC(hwnd0,hdc0);

	for (int i=0;i<32;i++)
	{
		m_gazePic[i] = NULL;
	}

	m_gazePic[1] = new CPicture();
	m_gazePic[1]->LoadDWQ("ta_edit_gaze",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[2] = new CPicture();
	m_gazePic[2]->LoadDWQ("ta_edit_gaze",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[8] = new CPicture();
	m_gazePic[8]->LoadDWQ("ta_gradBarRed",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[9] = new CPicture();
	m_gazePic[9]->LoadDWQ("ta_gradBarGreen",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[10] = new CPicture();
	m_gazePic[10]->LoadDWQ("ta_gradbarBlue",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[11] = new CPicture();
	m_gazePic[11]->LoadDWQ("ta_gradBarDeltaRed",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[12] = new CPicture();
	m_gazePic[12]->LoadDWQ("ta_gradBarDeltaGreen",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[13] = new CPicture();
	m_gazePic[13]->LoadDWQ("ta_gradbarDeltaBlue",FALSE,"nnndir\\setup\\dwq");
	m_gazePic[14] = new CPicture();
	m_gazePic[14]->LoadDWQ("ta_gradBarTrans",FALSE,"nnndir\\setup\\dwq");

	m_markPic = new CPicture();
	m_markPic->LoadDWQ("ta_edit_marker",FALSE,"nnndir\\setup\\dwq");
	m_outerArrowPic = new CPicture();
	m_outerArrowPic->LoadDWQ("ta_arrow1",FALSE,"nnndir\\setup\\dwq");
	m_upperButtonPic = new CPicture();
	m_upperButtonPic->LoadDWQ("ta_upperButton",FALSE,"nnndir\\setup\\dwq");
	m_speedTypePic = new CPicture();
	m_speedTypePic->LoadDWQ("ta_movetype",FALSE,"nnndir\\setup\\dwq");
	m_suujiPic = new CPicture();
	m_suujiPic->LoadDWQ("nekofont",FALSE,"nnndir\\setup\\dwq");
	m_centerTargetPic = new CPicture();
	m_centerTargetPic->LoadDWQ("ta_edit_center",FALSE,"nnndir\\setup\\dwq");
	m_startTargetPic = new CPicture();
	m_startTargetPic->LoadDWQ("ta_edit_startPoint",FALSE,"nnndir\\setup\\dwq");
	m_sizeTargetPic = new CPicture();
	m_sizeTargetPic->LoadDWQ("ta_edit_sizePoint",FALSE,"nnndir\\setup\\dwq");
	m_transButtonPic = new CPicture();
	m_transButtonPic->LoadDWQ("ta_edit_transButton",FALSE,"nnndir\\setup\\dwq");
	m_transButton3Pic = new CPicture();
	m_transButton3Pic->LoadDWQ("ta_edit_transButton3",FALSE,"nnndir\\setup\\dwq");
	m_leftButtonPic = new CPicture();
	m_leftButtonPic->LoadDWQ("ta_leftButton",FALSE,"nnndir\\setup\\dwq");
	m_rightButtonPic = new CPicture();
	m_rightButtonPic->LoadDWQ("ta_rightButton",FALSE,"nnndir\\setup\\dwq");
	m_onoffButtonPic = new CPicture();
	m_onoffButtonPic->LoadDWQ("ta_edit_onoffButton",FALSE,"nnndir\\setup\\dwq");

	m_frameTypePic = new CPicture();
	m_frameTypePic->LoadDWQ("ta_frameType",FALSE,"nnndir\\setup\\dwq");
	m_frameGazePic = new CPicture();
	m_frameGazePic->LoadDWQ("ta_frameGaze",FALSE,"nnndir\\setup\\dwq");
	m_frameMarkerPic = new CPicture();
	m_frameMarkerPic->LoadDWQ("ta_frame_marker",FALSE,"nnndir\\setup\\dwq");
	m_frameNumberPic = new CPicture();
	m_frameNumberPic->LoadDWQ("ta_frameNumber",FALSE,"nnndir\\setup\\dwq");

	m_playModeIconPic = new CPicture();
	m_playModeIconPic->LoadDWQ("ta_playModeIcon",FALSE,"nnndir\\setup\\dwq");

	m_messageWindowPic = new CPicture();
	m_messageWindowPic->LoadDWQ("sys\\ta_meswin01");
	m_nameWindowPic = new CPicture();
	m_nameWindowPic->LoadDWQ("sys\\ta_meswin02");

	m_mesWinPic = ((CMainScreenDoc*)m_document)->GetMesNameWinPic(0);
	m_nameWinPic = ((CMainScreenDoc*)m_document)->GetMesNameWinPic(0);

	m_mmx = new CMMX();
	m_mmx->SetViewParam(screenSizeX,screenSizeY,0,0);

	m_directX = new CMyDirectDraw(m_hWnd,m_hInstance,screenSizeX,screenSizeY,m_bpp,FALSE);
	m_directX->SetWindowSize(desktopWindowSizeX,desktopWindowSizeY);
	POINT pt2;
	pt2.x = desktopWindowStartX;
	pt2.y = desktopWindowStartY;
	m_directX->SetWindowStart(pt2);
	POINT pt3;
	pt3.x = desktopWindowEndX;
	pt3.y = desktopWindowEndY;
	m_directX->SetWindowEnd(pt3);











	m_mmx->Set565Mode(m_directX->Check565Mode());
	m_mmx->SetRGB24Mode(m_directX->CheckRGB24Mode());
	m_mmx->SetRGB32Mode(m_directX->CheckRGB32Mode());

	int* dst = CMyGraphics::GetScreenBuffer();
	ZeroMemory(dst,screenSizeX*screenSizeY*sizeof(int));


	int* src = CMyGraphics::GetScreenBuffer();
	if (m_directX->Lock())
	{
		LPVOID dst = m_directX->GetSurfaceAddr();
		int lPitch = m_directX->GetLPitch();
		

		m_mmx->SetBufferParameter(src,screenSizeX*4,screenSizeX,screenSizeY);
		m_mmx->SetSurfaceParameter(dst,lPitch);
		m_mmx->MMXPrint(m_bpp,0,0,screenSizeX,screenSizeY);

		m_directX->Unlock();
	}















	m_mouseXMae = 0;
	m_mouseYMae = 0;

	m_shiftMode = 0;

	m_effectMenuPrintZahyo[0].x = 360;
	m_effectMenuPrintZahyo[0].y = 20;
	m_effectMenuPrintZahyo[1].x = 360;
	m_effectMenuPrintZahyo[1].y = screenSizeY - 30;



	m_effectBarPrintZahyo[0].x = 88;
	m_effectBarPrintZahyo[0].y = 80;
	m_effectBarPrintZahyo[1].x = screenSizeX-88-256;
	m_effectBarPrintZahyo[1].y = 80;


	m_effectButtonPrintZahyo[0].x = 88;
	m_effectButtonPrintZahyo[0].y = 48;
	m_effectButtonPrintZahyo[1].x = screenSizeX-88-256;
	m_effectButtonPrintZahyo[1].y = 48;



	m_effectDescPrintZahyo[0].x = screenSizeX - 15*17 - 6;
	m_effectDescPrintZahyo[0].y = 80;
	m_effectDescPrintZahyo[1].x = 6;
	m_effectDescPrintZahyo[1].y = 80;

	m_effectParamDescPrintZahyo[0].x = m_effectDescPrintZahyo[0].x;
	m_effectParamDescPrintZahyo[0].y = 240;
	m_effectParamDescPrintZahyo[1].x = m_effectDescPrintZahyo[1].x;
	m_effectParamDescPrintZahyo[1].y = 240;

	m_effectLeftButtonPrintZahyo[0].x = -72;
	m_effectLeftButtonPrintZahyo[0].y = -4;
	m_effectLeftButtonPrintZahyo[1].x = -72;
	m_effectLeftButtonPrintZahyo[1].y = -4;

	m_effectRightButtonPrintZahyo[0].x = 256+8;
	m_effectRightButtonPrintZahyo[0].y = -4;
	m_effectRightButtonPrintZahyo[1].x = 256+8;
	m_effectRightButtonPrintZahyo[1].y = -4;

	m_buttonPrintDX[0] = 24*0;
	m_buttonPrintDX[1] = 24*1;
	m_buttonPrintDX[2] = 24*2;
	m_buttonPrintDX[3] = 24*3;
	m_buttonPrintDX[4] = 24*4;
	m_buttonPrintDX[5] = 24*5;
	m_buttonPrintDX[6] = 24*6;

	m_effectFrameZahyo[0].x = m_effectMenuPrintZahyo[0].x + 18+4+4;
	m_effectFrameZahyo[0].y = m_effectMenuPrintZahyo[0].y -4 - 12;
	m_effectFrameZahyo[1].x = m_effectMenuPrintZahyo[1].x + 18+4+4;
	m_effectFrameZahyo[1].y = m_effectMenuPrintZahyo[1].y -4 - 12;
}




CMainScreenView::~CMainScreenView()
{
	End();
}

void CMainScreenView::End(void)
{
	ENDDELETECLASS(m_directX);
	ENDDELETECLASS(m_mmx);

	ENDDELETECLASS(m_nameWindowPic);
	ENDDELETECLASS(m_messageWindowPic);

	ENDDELETECLASS(m_playModeIconPic);
	ENDDELETECLASS(m_frameNumberPic);
	ENDDELETECLASS(m_frameMarkerPic);
	ENDDELETECLASS(m_frameGazePic);
	ENDDELETECLASS(m_frameTypePic);

	ENDDELETECLASS(m_onoffButtonPic);
	ENDDELETECLASS(m_rightButtonPic);
	ENDDELETECLASS(m_leftButtonPic);
	ENDDELETECLASS(m_transButton3Pic);
	ENDDELETECLASS(m_transButtonPic);
	ENDDELETECLASS(m_sizeTargetPic);
	ENDDELETECLASS(m_startTargetPic);
	ENDDELETECLASS(m_centerTargetPic);
	ENDDELETECLASS(m_suujiPic);
	ENDDELETECLASS(m_speedTypePic);
	ENDDELETECLASS(m_upperButtonPic);
	ENDDELETECLASS(m_outerArrowPic);
	ENDDELETECLASS(m_markPic);

	for (int i=0;i<32;i++)
	{
		ENDDELETECLASS(m_gazePic[i]);
	}

	DELETEARRAY(m_controlWork);
}



LRESULT CMainScreenView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int xPos,yPos;

	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document;

	switch (message)
	{

	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_PAINT:
		if (1)
		{
			RECT updateRect;
			RECT rcMax;
			RECT rcArea;

			int screenSizeX = CMyGraphics::GetScreenSizeX();
			int screenSizeY = CMyGraphics::GetScreenSizeY();

			SetRect(&rcMax,0,0,screenSizeX,screenSizeY);
			BOOL flg = FALSE;

			if (GetUpdateRect(hWnd,NULL,FALSE))
			{
				GetUpdateRect(hWnd,&updateRect,FALSE);
				if (IntersectRect(&updateRect,&updateRect,&rcMax))
				{
					flg = TRUE;
					rcArea = updateRect;
//					YoyakuArea(&updateRect);
				}
			}
			else
			{
				flg = TRUE;
				rcArea = rcMax;
//					YoyakuArea(&rcMax);
			}

			if (flg)
			{
				m_directX->NiseFlip(rcArea.left,rcArea.top,rcArea.right - rcArea.left,rcArea.bottom - rcArea.top,m_waitVSync);
			}
		}

//			m_directX->NiseFlip(0,0,screenSizeX,screenSizeY);
	
		break;

	case WM_KEYDOWN:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			switch (wParam)
			{
			case VK_UP:
				pDoc->AdjustPicture(0,-1);
				break;
			case VK_DOWN:
				pDoc->AdjustPicture(0,1);
				break;
			case VK_LEFT:
				pDoc->AdjustPicture(-1,0);
				break;
			case VK_RIGHT:
				pDoc->AdjustPicture(1,0);
				break;
			}
		}
		else
		{
			switch (wParam)
			{
			case VK_UP:
				pDoc->MovePicture(0,-1);
				break;
			case VK_DOWN:
				pDoc->MovePicture(0,1);
				break;
			case VK_LEFT:
				pDoc->MovePicture(-1,0);
				break;
			case VK_RIGHT:
				pDoc->MovePicture(1,0);
				break;
			case VK_NUMPAD8:
				pDoc->MovePicture(0,-10);
				break;
			case VK_NUMPAD2:
				pDoc->MovePicture(0,10);
				break;
			case VK_NUMPAD4:
				pDoc->MovePicture(-10,0);
				break;
			case VK_NUMPAD6:
				pDoc->MovePicture(10,0);
				break;
			case VK_NEXT:
				pDoc->OnUpDownEffectGaze(-1);
				break;
			case VK_PRIOR:
				pDoc->OnUpDownEffectGaze(1);
				break;
			}
		}
		break;


	case WM_LBUTTONDOWN:
		xPos = (int)(short) LOWORD(lParam);
		yPos = (int)(short) HIWORD(lParam);

		m_shiftMode = 0;

//		if (wParam & MK_CONTROL)
//		{
//			pDoc->MoveToByCenter(xPos,yPos);
//		}

		((CMainScreenDoc*)m_document)->OnLButtonDown(xPos,yPos);

		//gaze control
		if (((CMainScreenDoc*)m_document)->GetPlayMode() == FALSE)
		{
			CheckGazeControl(xPos,yPos,FALSE,wParam);
		}



		m_mouseXMae = xPos;
		m_mouseYMae = yPos;
		break;

	case WM_RBUTTONDOWN:
		OnClickRButton(wParam,lParam);
		break;
	case WM_MOUSEMOVE:

		xPos = (int)(short) LOWORD(lParam);
		yPos = (int)(short) HIWORD(lParam);

		if (wParam & MK_LBUTTON)
		{
			int dx,dy;
			dx = xPos - m_mouseXMae;
			dy = yPos - m_mouseYMae;

			if (wParam & MK_CONTROL)
			{
				if (m_shiftMode == 0)
				{
					int absx,absy;
					absx = dx;
					absy = dy;
					if (absx<0) absx *= -1;
					if (absy<0) absy *= -1;

					if (absx>absy)
					{
						m_shiftMode = 1;
					}
					else
					{
						m_shiftMode = 2;
					}
				}
			}

			if (wParam & MK_SHIFT)
			{
				m_shiftMode = 1;
			}

			if (m_shiftMode == 1) dy = 0;

			if (m_shiftMode == 2) dx = 0;

		
//			pDoc->MovePicture(xPos-m_mouseXMae,yPos-m_mouseYMae);
			pDoc->MovePicture(dx,dy);

			if (((CMainScreenDoc*)m_document)->GetPlayMode() == FALSE)
			{
				CheckGazeControl(xPos,yPos,TRUE,wParam);
			}


//			((CMainScreenDoc*)m_document)->OnLButtonDown(xPos,yPos);
		}

		m_mouseXMae = xPos;
		m_mouseYMae = yPos;

		break;

	case WM_MOVE:
		xPos = (int)(short) LOWORD(lParam);    // horizontal position 
		yPos = (int)(short) HIWORD(lParam);    // vertical position 
		






		if (m_directX != NULL)
		{
			RECT rc;
			GetWindowRect(hWnd,&rc);

//			m_directWindowX = rc.left + xPos;
			m_directX->WindowIsMoved(rc.left + xPos, rc.top + yPos);
//			CAreaControl::SetNextAllPrint();
		}


		break;
	case WM_CLOSE:
///		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;
//	case WM_COMMAND:
//		switch (wParam)
//		{
//		case ID_WINDOW_MAINSCREEN:
//			MessageBox(NULL,"b","a",MB_OK);
//			break;
//		}
//		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}



void CMainScreenView::PrintLayerHint(int btn)
{
	if ((btn < 5) || (btn > 7)) return;

	CKomaData* pKoma = m_document->GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_document->GetNowSelectPictureLayer();
	if (layer == -1) return;

	if (pKoma->GetPictureNumber(layer) == -1) return;


	LPSTR filename = pKoma->GetPicFileName(layer);
	if (filename != NULL)
	{
		if ((*filename) != 0)
		{
			m_message->PrintMessage(100,8,filename);
		}
	}


	RECT rc;
	pKoma->GetEffectRect(&rc,layer,btn-5);	//dst,dstfrom dstro


	int areaX = 0;
	int areaY = 0;

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	int x = rc.left;
	int y = rc.top;

	int sizeX= rc.right;
	int sizeY = rc.bottom;

	if (x>=screenSizeX) areaX = 1;
	if ((x+sizeX)<0) areaX = -1;
	if (y>=screenSizeY) areaY = 1;
	if ((y+sizeY)<0) areaY = -1;




	//print
	if ((areaX == 0) && (areaY == 0))
	{

					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);

					x = rc.left + rc.right - 32;
					y = rc.top;
					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					x = rc.left + rc.right - 2;
					y = rc.top;
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);

					x = rc.left;
					y = rc.top + rc.bottom - 2;
					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					x = rc.left;
					y = rc.top + rc.bottom - 32;
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);

					x = rc.left + rc.right - 32;
					y = rc.top + rc.bottom - 2;
					CAllGeo::BoxFill(x,y,32,2,0,0,0);
					CAllGeo::BoxFill(x,y,32,1,255,255,255);
					x = rc.left + rc.right - 2;
					y = rc.top + rc.bottom - 32;
					CAllGeo::BoxFill(x,y,2,32,0,0,0);
					CAllGeo::BoxFill(x,y,1,32,255,255,255);
	}
	else
	{
		int putX = screenSizeX/2 + (screenSizeX/2-16)*areaX;
		int putY = screenSizeY/2 + (screenSizeY/2-16)*areaY;
		int srcX = (areaX+1)*32;
		int srcY = (areaY+1)*32;

		m_outerArrowPic->Blt(putX-16,putY-16,srcX,srcY,32,32,TRUE);
	}
}





void CMainScreenView::PrintEffectHint(int effectLayer)
{
	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	CKomaData* pKoma = m_document->GetNowSelectKoma();
	if (pKoma != NULL)
	{
		int effect = pKoma->GetEffect(effectLayer);
		if (effect != -1)
		{
			CEffectParam* effectParam = ((CMainScreenDoc*)m_document)->GetEffectParam(effect);
			if (effectParam != NULL)
			{
				CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document;

				int effectMenuPrintFlag = pDoc->GetEffectMenuPrintMode();
				int effectMenuUpDown = pDoc->GetEffectMenuPrintUpDown();
				int effectPrintLeftRight = pDoc->GetEffectPrintLeftRight();
				int effectBarPrintFlag = pDoc->GetEffectBarPrintMode();
				int effectNamePrintFlag = 0;
				int effectDescPrintFlag = 0;
				int effectParamDescPrintFlag = 0;

				m_leftRight = effectPrintLeftRight;
				m_topBottom = effectMenuUpDown;

				if (effectBarPrintFlag)
				{
					effectNamePrintFlag = pDoc->GetEffectParamNamePrintMode();
					effectDescPrintFlag = pDoc->GetEffectDescPrintMode();
					effectParamDescPrintFlag = pDoc->GetEffectParamDescPrintMode();
				}


				int paramKosuu = pKoma->GetEffectParaKosuu(effectLayer);
				int sel = pKoma->GetSelectParam(effectLayer);
				int offset = m_effectTableOffset[effect];

				//name
				if (effectMenuPrintFlag)
				{
					int namePrintX = m_effectMenuPrintZahyo[effectMenuUpDown].x;
					int namePrintY = m_effectMenuPrintZahyo[effectMenuUpDown].y;
					namePrintX += 24*7+8;
					m_message->PrintMessage(namePrintX,namePrintY,effectParam->GetEffectName());

				}
				//minipic?


				//button
				if (effectMenuPrintFlag)
				{
					int buttonX = m_effectMenuPrintZahyo[effectMenuUpDown].x;
					int buttonY = m_effectMenuPrintZahyo[effectMenuUpDown].y;

					for (int b=0;b<7;b++)
					{
						int st = 0;

						if (b == 0) st = effectMenuUpDown;
						if (b == 1) st = 1-effectBarPrintFlag;

						if ((b == 2) || (b == 3) || (b == 4))
						{
							if (effectBarPrintFlag)
							{
								if (b == 2) st = 1-effectNamePrintFlag;
								if (b == 3) st = 1-effectDescPrintFlag;
								if (b == 4) st = 1-effectParamDescPrintFlag;
							}
							else
							{
								st = 2;
							}
						}

						if (b == 5) st = effectPrintLeftRight;
						if (b == 6) st = 0;

						PutUpperButton(buttonX+b*24,buttonY,b,st);
					}
				}


				//frame gaze
				if (effectMenuPrintFlag)
				{
					POINT pt = GetEffectFrameZahyo(effectMenuUpDown);
					
					POINT pt2 = pt;
					pt2.x -= 4;

					int ftype = pDoc->GetEditFrameType();
					int frame = pDoc->GetEditFrame();

					PutFrameGaze(pt2,ftype);

					pt2 = pt;
					pt2.x -= (4+18);
					PutFrameType(pt2,ftype);

					pt2 = pt;
					pt2.x -= 6;
					PutFrameMarker(pt2,frame);

					pt2 = pt;
					pt2.x += (100+4+4);
					PrintFrameSuuji(pt2,frame);
				}

				if (effectBarPrintFlag)
				{
					for (int i=0;i<paramKosuu;i++)
					{
						int type = m_controlWork[(offset+i)*3];
						int selMode = 0;
						if (i != sel) selMode = 1;


						if ((type == 1) || (type == 2) || ((type >= 8) && (type <= 14)))
						{
							POINT pt = GetBarBaseZahyo(effect,i);
							
							int minData = effectParam->GetParaMin(i);
							int maxData = effectParam->GetParaMax(i);

							int d = pKoma->GetEffectPara(effectLayer,i);
							int dv = maxData - minData;
							if (dv<1) dv = 1;
							int delta = (255*(d - minData)) / dv;

							PutBar(type,pt,delta,selMode);

							if (effectNamePrintFlag)
							{
								LPSTR paramName = effectParam->GetParamName(i);
								POINT pt2 = GetParamNameZahyo(effect,i);
								PutParamName(pt2,paramName,selMode);
							}
						}
						else if (type == 3)
						{
							int d = pKoma->GetEffectPara(effectLayer,i);
							int putX = d;

							int sizeY = CMyGraphics::GetScreenSizeX();
							CAllGeo::BoxFill(putX,0,1,sizeY,255,255,255);
							CAllGeo::BoxFill(putX+1,0,1,sizeY,0,0,0);

						}
						else if (type == 4)
						{
							int d = pKoma->GetEffectPara(effectLayer,i);
							int putY = d;

							int editPara = effectParam->GetParaEditPara(i);

							if (editPara < 0)
							{
								putY += pKoma->GetEffectPara(effectLayer,i+editPara);
							}



							int sizeX = CMyGraphics::GetScreenSizeX();
							CAllGeo::BoxFill(0,putY,sizeX,1,255,255,255);
							CAllGeo::BoxFill(0,putY+1,sizeX,1,0,0,0);

						}
						else if (type == 5)	//6は5が同時に描画
						{
							POINT startPoint = GetStartPointXY(effectLayer,i,effectParam,pKoma);

							int x = pKoma->GetEffectPara(effectLayer,i) + startPoint.x;
							int y = pKoma->GetEffectPara(effectLayer,i+1) + startPoint.y;
							m_centerTargetPic->Put(x-32,y-32,TRUE);
						}
						else if (type == 7)
						{
							POINT pt = GetButtonBaseZahyo(effect,i);
							int d = pKoma->GetEffectPara(effectLayer,i);
							PutSpeedTypePic(pt,d,selMode);
						}
						else if (type == 15)	//startX (16含む)
						{
							int x = pKoma->GetEffectPara(effectLayer,i);
							int y = pKoma->GetEffectPara(effectLayer,i+1);

							m_startTargetPic->Put(x-32,y-32,TRUE);
						}
						else if (type == 17)	//sizeX (18含む)
						{
							POINT startPoint = GetStartPointXY(effectLayer,i,effectParam,pKoma);

							int x = pKoma->GetEffectPara(effectLayer,i) + startPoint.x;
							int y = pKoma->GetEffectPara(effectLayer,i+1) + startPoint.y;

							m_sizeTargetPic->Put(x-32,y-32,TRUE);
						}
						else if ((type == 19) || (type == 20) || (type == 21))	//transButton,3,oboff
						{
							POINT pt = GetButtonBaseZahyo(effect,i);
							int d = pKoma->GetEffectPara(effectLayer,i);
							PutButtonPic(type,pt,d,selMode);
						}
						else if (type == 22)	//23ふくむ
						{
							POINT startPoint;
							startPoint.x = screenSizeX / 2;
							startPoint.y = screenSizeY / 2;

							int mae = effectParam->GetParaEditPara(i);
							if (mae<0)
							{
								startPoint = GetStartPointXY(effectLayer,i,effectParam,pKoma);
							}

							int sx = startPoint.x;
							int sy = startPoint.y;

							int dx = pKoma->GetEffectPara(effectLayer,i);
							int dy = pKoma->GetEffectPara(effectLayer,i+1);

							PrintLine3(sx,sy,sx+dx,sy+dy,0,0,0);


							if ((dx!=0) || (dy!=0))
							{
								int dx2 = -70*dx + 70*dy;
								int dy2 = -70*dx - 70*dy;
								dx2 /= 100;
								dy2 /= 100;

								int dx3 = -70*dx - 70*dy;
								int dy3 =  70*dx - 70*dy;
								dx3 /= 100;
								dy3 /= 100;

								int rr = (int)(sqrt((double)(dx*dx+dy*dy)));
								if (rr<1) rr = 1;

								dx2 *= 16;
								dy2 *= 16;
								dx3 *= 16;
								dy3 *= 16;
								dx2 /= rr;
								dy2 /= rr;
								dx3 /= rr;
								dy3 /= rr;

								PrintLine3(sx+dx,sy+dy,sx+dx+dx2,sy+dy+dy2,0,0,0);
								PrintLine3(sx+dx,sy+dy,sx+dx+dx3,sy+dy+dy3,0,0,0);

								CAllGeo::Line(sx+dx,sy+dy,sx+dx+dx2,sy+dy+dy2,255,255,255);
								CAllGeo::Line(sx+dx,sy+dy,sx+dx+dx3,sy+dy+dy3,255,255,255);
							}

							CAllGeo::Line(sx,sy,sx+dx,sy+dy,255,255,255);






						}

						//////////////////////

						if ((type == 1) || (type == 2) || ((type >= 8) && (type <= 14)))
						{
							int md = 0;
							if (i != sel) md = 1;

							POINT pt = GetLeftArrowButtonZahyo(effect,i);
							PutLeftButton(pt,md);

							pt = GetRightArrowButtonZahyo(effect,i);
							PutRightButton(pt,md);
						}
					}


					if (effectDescPrintFlag)
					{
						int descX = m_effectDescPrintZahyo[effectPrintLeftRight].x;
						int descY = m_effectDescPrintZahyo[effectPrintLeftRight].y;

						CAllGeo::TransBoxFill(descX-2,descY-2,17*15+4,24*6+4,64,192,128,60);

						for (int desc=0;desc<6;desc++)
						{
							LPSTR text = effectParam->GetMainDesc(desc);
							if (text != NULL)
							{
								if ((*text) != '@')
								{
									m_message->PrintMessage(descX,descY+24*desc,text);
								}
							}
						}
					}

					if (effectParamDescPrintFlag)
					{
						int descX = m_effectParamDescPrintZahyo[effectPrintLeftRight].x;
						int descY = m_effectParamDescPrintZahyo[effectPrintLeftRight].y;

						CAllGeo::TransBoxFill(descX-2,descY-2,17*15+4,24*3+4,64,192,128,60);

						for (int desc=0;desc<3;desc++)
						{
							LPSTR text = effectParam->GetParamDesc(sel,desc);
							if (text != NULL)
							{
								if ((*text) != '@')
								{
									m_message->PrintMessage(descX,descY+24*desc,text);
								}
							}
						}
					}


				}
			}
		}
	}

/*

	//test
	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document;
	int effect = pDoc->GetNowEffect();
	if (effect != -1)
	{
		CEffectParam* effectParam = pDoc->GetEffectParam(effect);
		if (effectParam != NULL)
		{
			m_gazePic->Put(32,100+2*50,TRUE);
			m_markPic->Put(100+2*30,100+2*50,TRUE);
		}
	}

*/
}






void CMainScreenView::PrintFrameTime(int nowFrame,int setFrame,int printPlace)
{
	int printX = 16;
	int printY = 8;
	if (printPlace == 1)
	{
		//
	}

	if (nowFrame >= 0)
	{
		PrintSuuji(printX,printY,nowFrame);
		printX += 48;
		PutChara(printX,printY,'/');
		printX += 16;
	}

	if (setFrame > 0)
	{
		int keta = 3;
		if (setFrame < 10)
		{
			keta = 1;
		}
		else if (setFrame < 100)
		{
			keta = 2;
		}

		PrintSuuji(printX,printY,setFrame,keta);
	}
	else
	{
		PrintMiniMessage(printX,printY,"DEF");
	}
}

void CMainScreenView::PrintShakin(int shakin,int skip,int totalSkip)
{
	if (shakin > 99) shakin = 99;
	if (skip>99) skip = 99;
	if (totalSkip>999) totalSkip = 999;

	int printX = 16;
	int printY = 28;

	PrintSuuji(printX,printY,shakin,2);
	printX += 16*2;

	PutChara(printX,printY,':');
	printX += 16;

	PrintSuuji(printX,printY,skip,2);
	printX += 16*2;

	PutChara(printX,printY,'/');
	printX += 16;

	int keta = 3;
	if (totalSkip < 10)
	{
		keta = 1;
	}
	else if (totalSkip<100)
	{
		keta = 2;
	}
	PrintSuuji(printX,printY,totalSkip,keta);
}



void CMainScreenView::PrintSuuji(int x, int y, int d,int keta)
{
	int sizeX = 16;
	int sizeY = 16;

	BOOL minusFlag = FALSE;

	if (d<0)
	{
		minusFlag = TRUE;
		d *= -1;
		PutChara(x-sizeX,y,'-');
	}


	BOOL flg = FALSE;

	int dv = 1;
	int i;
	for (i=0;i<keta-1;i++)
	{
		dv *= 10;
	}

	for (i=0;i<keta;i++)
	{
		int ch = d / dv;
		ch %= 10;

		if (ch != 0) flg = TRUE;
		if (i==keta-1) flg = TRUE;

		if (flg)
		{
			PutChara(x+sizeX*i,y,ch+'0');
		}

		d %= dv;
		dv /= 10;
	}
}


void CMainScreenView::PrintMiniMessage(int x, int y, LPSTR mes)
{
	if (mes == NULL) return;
	int ln = strlen(mes);
	for (int i=0;i<ln;i++)
	{
		char c = *(mes+i);
		int ch = (int)c;
		ch &= 0xff;

		if ((ch >= 'a') && (ch <= 'z'))
		{
			ch -= 0x20;
		}

		PutChara(x+i*16,y,ch);
	}
}


void CMainScreenView::PutChara(int x, int y,int ch)
{
	if (m_suujiPic == NULL) return;

	ch -= ' ';
	ch &= 0x3f;

	int sizeX = 16;
	int sizeY = 16;

	int srcX = sizeX * (ch % 16);
	int srcY = sizeY * (ch / 16);

	m_suujiPic->Blt(x,y,srcX,srcY,sizeX,sizeY,FALSE);
}





void CMainScreenView::FlipToScreen(void)
{
//	CMyApplication* app = m_document->GetApp();
//	CEffect* effect = app->GetEffect();
//	int* buf = m_effec

//	m_messageWindowPic->Blt(0,428,0,0,800,172,TRUE);
//	m_nameWindowPic->Blt(0,384,0,0,194,139,TRUE);

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	int* src = CMyGraphics::GetScreenBuffer();
	if (m_directX->Lock())
	{
		LPVOID dst = m_directX->GetSurfaceAddr();
		int lPitch = m_directX->GetLPitch();
		

		m_mmx->SetBufferParameter(src,screenSizeX*4,screenSizeX,screenSizeY);
		m_mmx->SetSurfaceParameter(dst,lPitch);
		m_mmx->MMXPrint(m_bpp,0,0,screenSizeX,screenSizeY);

		m_directX->Unlock();
	}

	m_directX->NiseFlip(0,0,screenSizeX,screenSizeY,m_waitVSync);
}


void CMainScreenView::Moved(WPARAM wParam,LPARAM lParam)
{
	m_directX->WindowIsMoved(LOWORD(lParam),HIWORD(lParam));
}


//
// type
// 0:
// 1: gazeX mouseX
// 2: *gazeX mouseY
// 3: lineX mouseX
// 4: lineY mouseY
// 5: gazeXY mouseX
// 6: gazeXY mouseY
//
void CMainScreenView::GetEffectControlAndPara(void)
{
	int kosuu = 0;
	int i;
	for (i=0;i<256;i++)
	{
		m_effectTableOffset[i] = kosuu;
		CEffectParam* effectParam = ((CMainScreenDoc*)m_document)->GetEffectParam(i);
		if (effectParam != NULL)
		{
			kosuu += effectParam->GetParaKosuu();
		}
	}

	m_controlWork = new int[kosuu*3];//type printx printy

	for (i=0;i<256;i++)
	{
		CEffectParam* effectParam = ((CMainScreenDoc*)m_document)->GetEffectParam(i);
		if (effectParam != NULL)
		{
			int offset = m_effectTableOffset[i];
			int n = effectParam->GetParaKosuu();

			int baseX = 0;
			int baseY = 0;

			int baseX2 = 0;
			int baseY2 = 0;


			for (int k=0;k<n;k++)
			{
				int type = effectParam->GetParaEditType(k);
				m_controlWork[(offset+k)*3] = type;

				if ((type == 1) || (type == 2) || ((type >= 8) && (type <= 14)))
				{
					m_controlWork[(offset+k)*3+1] = baseX;
					m_controlWork[(offset+k)*3+2] = baseY;

					baseY += 26;
				}
				else if (type == 3)
				{
					//
				}
				else if ((type == 7) || ((type >=19) && (type <= 21)))
				{
					m_controlWork[(offset+k)*3+1] = baseX2;
					m_controlWork[(offset+k)*3+2] = baseY2;

					baseX2 += 40;
				}
			}
		}
	}
}



void CMainScreenView::CheckGazeControl(int mouseX,int mouseY,BOOL dragFlag,WPARAM wParam)
{
	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document;

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	if (dragFlag == FALSE) m_captureGaze = -1;

	CKomaData* pKoma = m_document->GetNowSelectKoma();
	if (pKoma == NULL) return;

	int effectLayer = m_document->GetNowSelectEffectLayer();
	if (effectLayer == -1) return;

	if (pKoma->GetEffectFlag(effectLayer) == FALSE) return;

	int effect = pKoma->GetEffect(effectLayer);
	if (effect == -1) return;

	int useOkParam = pKoma->GetEffectParaKosuu(effectLayer);
	int selectParam = pKoma->GetSelectParam(effectLayer);
	


	CEffectParam* effectParam = ((CMainScreenDoc*)m_document)->GetEffectParam(effect);
	if (effectParam == NULL) return;

	int offset = m_effectTableOffset[effect];

	//int paraKosuu = effectParam->GetParaKosuu();
	//int paraKosuu = ((CMainScreenDoc*)m_document)->GetActiveEffectParaKosuu();
	int activeParaKosuu = pKoma->GetEffectParaKosuu(effectLayer);



	int effectMenuPrintFlag = pDoc->GetEffectMenuPrintMode();
	int effectMenuUpDown = pDoc->GetEffectMenuPrintUpDown();
	int effectPrintLeftRight = pDoc->GetEffectPrintLeftRight();
	int effectBarPrintFlag = pDoc->GetEffectBarPrintMode();



	int effectNamePrintFlag = 0;
//	int effectDescPrintFlag = 0;
//	int effectParamDescPrintFlag = 0;

//	m_leftRight = effectPrintLeftRight;
//	m_topBottom = effectMenuUpDown;

	if (effectBarPrintFlag)
	{
		effectNamePrintFlag = pDoc->GetEffectParamNamePrintMode();
//					effectDescPrintFlag = pDoc->GetEffectDescPrintMode();
//				effectParamDescPrintFlag = pDoc->GetEffectParamDescPrintMode();
	}


	//check menu
	if (effectMenuPrintFlag)
	{
		BOOL f = FALSE;

		if (dragFlag == FALSE)
		{
			int menuX = m_effectMenuPrintZahyo[effectMenuUpDown].x;
			int menuY = m_effectMenuPrintZahyo[effectMenuUpDown].y;

			for (int i=0;i<7;i++)
			{
				int buttonX = menuX + m_buttonPrintDX[i];
				int buttonY = menuY;

				int x = mouseX - buttonX;
				int y = mouseY - buttonY;

				if ((x>=0) && (x<24) && (y>=0) && (y<24))
				{
					pDoc->OnClickEffectMenu(i);
					f = TRUE;
					break;
				}
			}

			if (CheckOnEffectType(mouseX,mouseY,effectMenuUpDown))
			{
				pDoc->OnClickEffectType();
				f = TRUE;
			}

			int g = GetOnEffectGaze(mouseX,mouseY,effectMenuUpDown);
			if (g != -1)
			{
				pDoc->OnClickEffectGaze(g);
				m_captureGaze = -2;
				f = TRUE;
			}
		}
		else
		{
			if (m_captureGaze == -2)
			{
				pDoc->OnDragEffectGaze(mouseX-m_mouseXMae);
				f = TRUE;
			}
		}


		if (f)
		{
			pDoc->UpdateMyWindow(EFFECT_WINDOW);
			pDoc->UpdateMainScreen();
	//		pDoc->UpdateMyWindow(MAINSCREEN_WINDOW);

			return;
		}

	}





	//capture check

	if (m_captureGaze == -1)
	{
		BOOL flg = FALSE;

		int sel = pKoma->GetSelectParam(effectLayer);

		if ((dragFlag == FALSE) && (effectBarPrintFlag))
		{
			if (wParam & MK_CONTROL)
			{
				if ((selectParam >= 0) && (selectParam < activeParaKosuu))
				{
					m_captureGaze = selectParam;
					m_captureDrag = TRUE;
					m_captureWithY = FALSE;
					BOOL flg = FALSE;
				}
			}
			else
			{
				//first now select
				//second GUI
				//third gaze
				for (int cloop=0;cloop<3;cloop++)
				{
					if (flg) break;

					for (int i=0;i<activeParaKosuu;i++)
					{
						if (cloop == 0)
						{
							if (i != sel) continue;
						}

						int type = m_controlWork[(offset+i)*3];

						if ((type == 1) || (type == 2) || ((type >= 8) && (type <= 14)))
						{
							if (cloop == 1) continue;
						}
						else
						{
							if (cloop == 2) continue;
						}

						if (type == 3)
						{
							int x = mouseX - pKoma->GetEffectPara(effectLayer,i);
							if ((x>=-16) && (x<16))
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								pDoc->ChangeSelectParam(m_captureGaze);

								int dataX = CheckAndAdjustParam(effectParam,i,mouseX);
								pKoma->SetEffectPara(effectLayer,i,dataX);
								flg = TRUE;
								break;
							}
						}
						else if (type == 4)
						{
							int editPara = effectParam->GetParaEditPara(i);
							int mouseY00 = mouseY;

							if (editPara < 0)
							{
								mouseY00 -= pKoma->GetEffectPara(effectLayer,i+editPara);
							}

							int y = mouseY00 - pKoma->GetEffectPara(effectLayer,i);

							if ((y>=-16) && (y<16))
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								pDoc->ChangeSelectParam(m_captureGaze);


								int dataY = CheckAndAdjustParam(effectParam,i,mouseY00);
								pKoma->SetEffectPara(effectLayer,i,dataY);
								flg = TRUE;
								break;
							}
						}
						else if (type == 5)	//XY
						{
							POINT startPoint = GetStartPointXY(effectLayer,i,effectParam,pKoma);

							int x = mouseX - pKoma->GetEffectPara(effectLayer,i) - startPoint.x;
							int y = mouseY - pKoma->GetEffectPara(effectLayer,i+1) - startPoint.y;
							if ((x>=-32) && (x<32) && (y>=-32) && (y<32))
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								pDoc->ChangeSelectParam(m_captureGaze);

								int dataX = CheckAndAdjustParam(effectParam,i,mouseX-startPoint.x);
								pKoma->SetEffectPara(effectLayer,i,dataX);
								if ((i+1)<activeParaKosuu)
								{
									int dataY = CheckAndAdjustParam(effectParam,i+1,mouseY-startPoint.y);
									pKoma->SetEffectPara(effectLayer,i+1,dataY);
								}

								flg = TRUE;
								break;
							}
						}
						else if (type == 6)	//no check
						{
						}
						else if (type == 7)
						{
							if (CheckOnStatusButton(mouseX,mouseY,effect,i))
							{
	//							m_captureGaze = i;
								m_captureDrag = FALSE;

								if (i != sel)
								{
									pDoc->ChangeSelectParam(i);
								}
								else
								{
									int d = pKoma->GetEffectPara(effectLayer,i);
									int rev = (d / 100) % 2;
									int dRnd = d / 200;
									int curve = (d % 100) / 15;
									
//									if ((d % 100)>=75)
//									{
//										curve = 3;
//									}

									int arrowType = (d % 100) % 15;

									arrowType++;
									if ((arrowType >= 11) || (curve * 15 + arrowType) >= 91)
									{
										arrowType = 0;
										if (curve == 0)
										{
											curve++;
										}
										curve++;

										if (curve >= 6)
										{
											curve = 0;
											rev++;
											rev %= 2;
											if (rev == 0)
											{
												dRnd++;
												dRnd %= 2;
											}
										}
									}


									int p = dRnd * 200 + rev * 100 + curve * 15 + arrowType;
//									if (curve == 3)
//									{
//										p -= 15;
//									}

									pKoma->SetEffectPara(effectLayer,i,p);
								}

								flg = TRUE;
								break;

							}
						}
						else if (type == 15)	//startXY,X
						{
							int x = mouseX - pKoma->GetEffectPara(effectLayer,i);
							int y = mouseY - pKoma->GetEffectPara(effectLayer,i+1);
							if ((x>=-32) && (x<32) && (y>=-32) && (y<32))
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								m_captureWithY = TRUE;

								pDoc->ChangeSelectParam(m_captureGaze);

								int dataX = CheckAndAdjustParam(effectParam,i,mouseX);
								pKoma->SetEffectPara(effectLayer,i,dataX);
								if ((i+1)<activeParaKosuu)
								{
									int dataY = CheckAndAdjustParam(effectParam,i+1,mouseY);
									pKoma->SetEffectPara(effectLayer,i+1,dataY);
								}

								flg = TRUE;
								break;
							}

							if (flg == FALSE)	//checkXOnly
							{
								int x = mouseX - pKoma->GetEffectPara(effectLayer,i);
								//if ((x>=-16) && (x<16))
								if (0)
								{
									m_captureGaze = i;
									m_captureDrag = FALSE;
									pDoc->ChangeSelectParam(m_captureGaze);

									int dataX = CheckAndAdjustParam(effectParam,i,mouseX);
									pKoma->SetEffectPara(effectLayer,i,dataX);
									m_captureWithY = FALSE;
									flg = TRUE;
									break;
								}
							}
						}
						else if (type == 16)	//startY
						{
							int y = mouseY - pKoma->GetEffectPara(effectLayer,i);
							//if ((y>=-16) && (y<16))
							if (0)
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								pDoc->ChangeSelectParam(m_captureGaze);

								int dataY = CheckAndAdjustParam(effectParam,i,mouseY);
								pKoma->SetEffectPara(effectLayer,i,dataY);
								m_captureWithY = FALSE;
								flg = TRUE;
								break;
							}
						}
						else if (type == 17)	//sizeXY,X
						{
							POINT startPoint = GetStartPointXY(effectLayer,i,effectParam,pKoma);

							int x = mouseX - pKoma->GetEffectPara(effectLayer,i) - startPoint.x;
							int y = mouseY - pKoma->GetEffectPara(effectLayer,i+1) - startPoint.y;
							if ((x>=-32) && (x<32) && (y>=-32) && (y<32))
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								m_captureWithY = TRUE;

								pDoc->ChangeSelectParam(m_captureGaze);

								int dataX = CheckAndAdjustParam(effectParam,i,mouseX-startPoint.x);
								pKoma->SetEffectPara(effectLayer,i,dataX);
								if ((i+1)<activeParaKosuu)
								{
									int dataY = CheckAndAdjustParam(effectParam,i+1,mouseY-startPoint.y);
									pKoma->SetEffectPara(effectLayer,i+1,dataY);
								}

								flg = TRUE;
								break;
							}


							if (flg == FALSE)	//checkXOnly
							{

								int x = mouseX - pKoma->GetEffectPara(effectLayer,i) - startPoint.x;
								int sizeY = pKoma->GetEffectPara(effectLayer,i+1);

								int y = mouseY - startPoint.y;

								if ((x>=-16) && (x<16) && (y>=0) && (y<sizeY))
								{
									m_captureGaze = i;
									m_captureDrag = FALSE;
									pDoc->ChangeSelectParam(m_captureGaze);

									int dataX = CheckAndAdjustParam(effectParam,i,mouseX-startPoint.x);
									pKoma->SetEffectPara(effectLayer,i,dataX);
									m_captureWithY = FALSE;
									flg = TRUE;
									break;
								}
							}
						}
						else if (type == 18)	//sizeY
						{
continue;//dont check!
							int startY = GetStartPointY(effectLayer,i,effectParam,pKoma);

							int y = mouseY - pKoma->GetEffectPara(effectLayer,i) - startY;

							if ((y>=-16) && (y<16))
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								pDoc->ChangeSelectParam(m_captureGaze);

								int dataY = CheckAndAdjustParam(effectParam,i,mouseY-startY);
								pKoma->SetEffectPara(effectLayer,i,dataY);
								m_captureWithY = FALSE;
								flg = TRUE;
								break;
							}
						}
						else if ((type >= 19) && (type <= 21))
						{
							if (CheckOnStatusButton(mouseX,mouseY,effect,i))
							{
								if (i != sel)
								{
									pDoc->ChangeSelectParam(i);
								}
								else
								{
									int d = pKoma->GetEffectPara(effectLayer,i);
									int minData = effectParam->GetParaMin(i);
									int maxData = effectParam->GetParaMax(i);
									d++;
									if (d>maxData) d = minData;
									pKoma->SetEffectPara(effectLayer,i,d);
								}

								flg = TRUE;
								break;
							}
						}
						else if (type == 22)//speedXY,X
						{
							POINT startPoint;
							startPoint.x = screenSizeX / 2;
							startPoint.y = screenSizeY / 2;

							int mae = effectParam->GetParaEditPara(i);
							if (mae<0)
							{
								startPoint = GetStartPointXY(effectLayer,i,effectParam,pKoma);
							}

							int x = mouseX - (startPoint.x+pKoma->GetEffectPara(effectLayer,i));
							int y = mouseY - (startPoint.y+pKoma->GetEffectPara(effectLayer,i+1));

							if ((x>=-16) && (x<16) && (y>=-16) && (y<16))
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								m_captureWithY = TRUE;

								pDoc->ChangeSelectParam(m_captureGaze);



								int dataX = CheckAndAdjustParam(effectParam,i,mouseX-startPoint.x);
								pKoma->SetEffectPara(effectLayer,i,dataX);
								if ((i+1)<activeParaKosuu)
								{
									int dataY = CheckAndAdjustParam(effectParam,i+1,mouseY-startPoint.y);
									pKoma->SetEffectPara(effectLayer,i+1,dataY);
								}

								flg = TRUE;
								break;
							}

							/*
							if (flg == FALSE)	//checkXOnly
							{
								int x = mouseX - pKoma->GetEffectPara(effectLayer,i);
								//if ((x>=-16) && (x<16))
								if (0)
								{
									m_captureGaze = i;
									m_captureDrag = FALSE;
									pDoc->ChangeSelectParam(m_captureGaze);

									int dataX = CheckAndAdjustParam(effectParam,i,mouseX);
									pKoma->SetEffectPara(effectLayer,i,dataX);
									m_captureWithY = FALSE;
									flg = TRUE;
									break;
								}
							}
							*/



						}
						else if ((type == 1) || (type == 2) || ((type >= 8) && (type <= 14)))
						{
							int gz = GetOnGaze(mouseX,mouseY,effect,i);
							if (gz >= 0)
							{
								m_captureGaze = i;
								m_captureDrag = FALSE;
								if (i != sel)
								{
									pDoc->ChangeSelectParam(m_captureGaze);
								}
								else
								{
									int p = CalcuParamByGaze(effectParam,i,gz,255);
									pKoma->SetEffectPara(effectLayer,m_captureGaze,p);
								}

								flg = TRUE;
								break;
							}

							if (sel == i)
							{
								int arrowButton = GetOnArrowButton(mouseX,mouseY,effect,i);
								if (arrowButton != -1)
								{
									int d = pKoma->GetEffectPara(effectLayer,sel);
									d = CheckAndAdjustParam(effectParam,sel,d+m_arrowDeltaTable[arrowButton]);
									pKoma->SetEffectPara(effectLayer,sel,d);
									flg = TRUE;
									break;
								}
							}

							if (effectNamePrintFlag)
							{
								if (CheckOnParamName(mouseX,mouseY,effect,i))
								{
									if (i != sel)
									{
										m_captureDrag = FALSE;
										pDoc->ChangeSelectParam(i);
										flg = TRUE;
										break;
									}
								}
							}
						}
					}

				}
			}
		}

		if (flg)
		{
			pDoc->UpdateMyWindow(EFFECT_WINDOW);
			pDoc->UpdateMainScreen();
		}

		return;
	}
	else if (m_captureGaze >= 0)
	{
		int type = m_controlWork[(offset+m_captureGaze)*3];

		int checkX = m_controlWork[(offset+m_captureGaze)*3+1];
		int checkY = m_controlWork[(offset+m_captureGaze)*3+2];

		checkX += m_effectBarPrintZahyo[effectPrintLeftRight].x;
		checkY += m_effectBarPrintZahyo[effectPrintLeftRight].y;

		BOOL flg = FALSE;

		if ((type == 1) || ((type >= 8) && (type <= 14)))
		{
			if (m_captureDrag)
			{
				int d = pKoma->GetEffectPara(effectLayer,m_captureGaze);
				d = CheckAndAdjustParam(effectParam,m_captureGaze,d+mouseX-m_mouseXMae);
				pKoma->SetEffectPara(effectLayer,m_captureGaze,d);
			}
			else
			{
				int d = CalcuParamByGaze(effectParam,m_captureGaze,mouseX-checkX,255);
				pKoma->SetEffectPara(effectLayer,m_captureGaze,d);
			}
			flg = TRUE;
		}
		else if (type == 2)
		{
			if (m_captureDrag)
			{
				int d = pKoma->GetEffectPara(effectLayer,m_captureGaze);
				d = CheckAndAdjustParam(effectParam,m_captureGaze,d+mouseY-m_mouseYMae);
				pKoma->SetEffectPara(effectLayer,m_captureGaze,d);
			}
			else
			{
				int d = CalcuParamByGaze(effectParam,m_captureGaze,mouseX-checkX,255);
				pKoma->SetEffectPara(effectLayer,m_captureGaze,d);
			}
			flg = TRUE;
		}
		else if (type == 3)
		{
			int dataX;
			if (dragFlag)
			{
				dataX = pKoma->GetEffectPara(effectLayer,m_captureGaze);
				dataX = CheckAndAdjustParam(effectParam,m_captureGaze,dataX+mouseX-m_mouseXMae);
			}
			else
			{
				dataX = CheckAndAdjustParam(effectParam,m_captureGaze,mouseX);
			}

			pKoma->SetEffectPara(effectLayer,m_captureGaze,dataX);
			flg = TRUE;
		}
		else if (type == 4)
		{
			int dataY;
			if (dragFlag)
			{
				dataY = pKoma->GetEffectPara(effectLayer,m_captureGaze);
				dataY = CheckAndAdjustParam(effectParam,m_captureGaze,dataY+mouseY-m_mouseYMae);
			}
			else
			{

				int editPara = effectParam->GetParaEditPara(m_captureGaze);
				int mouseY0 = mouseY;
				if (editPara < 0)
				{
					mouseY0 -= pKoma->GetEffectPara(effectLayer,m_captureGaze+editPara);
				}

				dataY = CheckAndAdjustParam(effectParam,m_captureGaze,mouseY0);
			}

			pKoma->SetEffectPara(effectLayer,m_captureGaze,dataY);
			flg = TRUE;
		}
		else if ((type == 5) || (type == 6))
		{
			int dataX;
			int dataY;

			if (dragFlag)
			{
				dataX = pKoma->GetEffectPara(effectLayer,m_captureGaze+(5-type));
				dataY = pKoma->GetEffectPara(effectLayer,m_captureGaze+(6-type));

				dataX = CheckAndAdjustParam(effectParam,m_captureGaze+(5-type),dataX+mouseX-m_mouseXMae);
				dataY = CheckAndAdjustParam(effectParam,m_captureGaze+(6-type),dataY+mouseY-m_mouseYMae);
			}
			else
			{
				POINT startPoint = GetStartPointXY(effectLayer,m_captureGaze+5-type,effectParam,pKoma);

				dataX = CheckAndAdjustParam(effectParam,m_captureGaze+(5-type),mouseX-startPoint.x);
				dataY = CheckAndAdjustParam(effectParam,m_captureGaze+(6-type),mouseY-startPoint.y);
			}

			pKoma->SetEffectPara(effectLayer,m_captureGaze+(5-type),dataX);
			if ((m_captureGaze+(6-type))<activeParaKosuu)
			{
				pKoma->SetEffectPara(effectLayer,m_captureGaze+(6-type),dataY);
			}

			flg = TRUE;
		}
		else if (type == 7)
		{
		}
		else if ((type == 15) || (type == 16))
		{
			int dataX;
			int dataY;


			if (dragFlag)
			{
				dataX = pKoma->GetEffectPara(effectLayer,m_captureGaze+(15-type));
				dataY = pKoma->GetEffectPara(effectLayer,m_captureGaze+(16-type));

				dataX = CheckAndAdjustParam(effectParam,m_captureGaze+(15-type),dataX+mouseX-m_mouseXMae);
				dataY = CheckAndAdjustParam(effectParam,m_captureGaze+(16-type),dataY+mouseY-m_mouseYMae);
			}
			else
			{
				dataX = CheckAndAdjustParam(effectParam,m_captureGaze+(15-type),mouseX);
				dataY = CheckAndAdjustParam(effectParam,m_captureGaze+(16-type),mouseY);
			}

			if (type == 15)
			{
				pKoma->SetEffectPara(effectLayer,m_captureGaze+(15-type),dataX);
			}

			if ((m_captureGaze+(16-type))<activeParaKosuu)
			{
				if ((type == 16) || m_captureWithY)
				{
					pKoma->SetEffectPara(effectLayer,m_captureGaze+(16-type),dataY);
				}
			}

			flg = TRUE;
		}
		else if ((type == 17) || (type == 18))
		{
			int dataX;
			int dataY;

			int para1 = m_captureGaze +17-type;
			int para2 = m_captureGaze +18-type;

			if (dragFlag)
			{

				dataX = pKoma->GetEffectPara(effectLayer,para1);
				dataY = pKoma->GetEffectPara(effectLayer,para2);

				dataX = CheckAndAdjustParam(effectParam,para1,dataX+mouseX-m_mouseXMae);
				dataY = CheckAndAdjustParam(effectParam,para2,dataY+mouseY-m_mouseYMae);
			}
			else
			{
				POINT startPoint = GetStartPointXY(effectLayer,m_captureGaze+17-type,effectParam,pKoma);

				dataX = CheckAndAdjustParam(effectParam,para1,mouseX-startPoint.x);
				dataY = CheckAndAdjustParam(effectParam,para2,mouseY-startPoint.y);
			}

			if (type == 17)
			{
				pKoma->SetEffectPara(effectLayer,para1,dataX);
			}

			if (para2<activeParaKosuu)
			{
				if ((type == 18) || m_captureWithY)
				{
					pKoma->SetEffectPara(effectLayer,para2,dataY);
				}
			}

			flg = TRUE;
		}
		else if ((type == 22) || (type == 23))
		{
			int dataX;
			int dataY;

			int para1 = m_captureGaze +22-type;
			int para2 = m_captureGaze +23-type;

			if (dragFlag)
			{

				dataX = pKoma->GetEffectPara(effectLayer,para1);
				dataY = pKoma->GetEffectPara(effectLayer,para2);

				dataX = CheckAndAdjustParam(effectParam,para1,dataX+mouseX-m_mouseXMae);
				dataY = CheckAndAdjustParam(effectParam,para2,dataY+mouseY-m_mouseYMae);
			}
			else
			{
				POINT startPoint = GetStartPointXY(effectLayer,m_captureGaze+22-type,effectParam,pKoma);

				dataX = CheckAndAdjustParam(effectParam,para1,mouseX-startPoint.x);
				dataY = CheckAndAdjustParam(effectParam,para2,mouseY-startPoint.y);
			}

			if (type == 22)
			{
				pKoma->SetEffectPara(effectLayer,para1,dataX);
			}

			if (para2<activeParaKosuu)
			{
				if ((type == 23) || m_captureWithY)
				{
					pKoma->SetEffectPara(effectLayer,para2,dataY);
				}
			}

			flg = TRUE;

		}


		if (flg)
		{
			pDoc->UpdateMyWindow(EFFECT_WINDOW);
			pDoc->UpdateMainScreen();
		}
	}


/*

		if (type != 0)
		{
			int printX = m_controlWork[(offset+i)*3+1];
			int printY = m_controlWork[(offset+i)*3+2];

			if (type == 1)
			{
				int x = mouseX - printX;
				int y = mouseY - printY;

				if (dragFlag)
				{
					if (i == m_captureGaze)
					{
						y = 0;
						if (x<0) x = 0;
						if (x>255) x = 255;
					}
					else
					{
						y = -9999;
					}
				}

				if ((y>=-16) && (y<16) && (x>=0) && (x<=255))
				{
					if (dragFlag == FALSE)
					{
						m_captureGaze = i;
						if (i != selectParam)
						{
							pDoc->ChangeSelectParam(i);
							selectParam = i;
						}
					}

					int minData = effectParam->GetParaMin(i);
					int maxData = effectParam->GetParaMax(i);

					int d = minData + ((maxData-minData)*x)/255;

					pKoma->SetEffectPara(effectLayer,i,d);

					m_document->UpdateMyWindow(EFFECT_WINDOW);
					((CMainScreenDoc*)m_document)->UpdateMainScreen();
					return;
				}
			}
			else if (type == 2)
			{
			}
			else if (type == 3)
			{
			}
			else if (type == 4)
			{
				int x = mouseX - pKoma->GetEffectPara(layer,i);
				int y = mouseY - pKoma->GetEffectPara(layer,i+1);
				if ((x>=-32) && (x<32) && (y>=-32) && (y<32))
				{

				}
				
			}
			else if (type == 5)	//nothing to do
			{
			}
		}
	}


	if ((paraNumber >= 0) && (paraNumber < activeParaKosuu))
	{
		int type = m_controlWork[(offset+paraNumber)*3];
		
		if (type == 2)
		{
			int d = mouseX;

			pKoma->SetEffectPara(effectLayer,paraNumber,d);
			m_document->UpdateMyWindow(EFFECT_WINDOW);
			((CMainScreenDoc*)m_document)->UpdateMainScreen();
		}
		else if (type == 3)
		{
			int d = mouseY;

			pKoma->SetEffectPara(effectLayer,paraNumber,d);
			m_document->UpdateMyWindow(EFFECT_WINDOW);
			((CMainScreenDoc*)m_document)->UpdateMainScreen();
		}
		else if (type == 4)
		{
		}
		else if (type == 5)
		{
		}
	}
*/


}


int CMainScreenView::CalcuParamByGaze(CEffectParam* effectParam,int n,int d,int dv)
{
	int minData = effectParam->GetParaMin(n);
	int maxData = effectParam->GetParaMax(n);

	if (dv<1) dv = 1;
	int dt = minData + ((maxData-minData)*d)/dv;

	return CheckAndAdjustParam(effectParam,n,dt);
}


int CMainScreenView::CheckAndAdjustParam(CEffectParam* effectParam,int n,int d)
{
	int minData = effectParam->GetParaMin(n);
	int maxData = effectParam->GetParaMax(n);

	if (d<minData) d = minData;
	if (d>maxData) d = maxData;

	return d;
}


void CMainScreenView::PutUpperButton(int x, int y, int n, int md)
{
	int sizeX = 24;
	int sizeY = 24;
	int srcX = md * sizeX;
	int srcY = n * sizeY;

	m_upperButtonPic->Blt(x,y,srcX,srcY,sizeX,sizeY,TRUE);
}



void CMainScreenView::OnClickRButton(WPARAM wParam,LPARAM lParam)
{
	CMainScreenDoc* pDoc = (CMainScreenDoc*)m_document;
	if (pDoc->GetPlayMode()) return;


	CKomaData* pKoma = m_document->GetNowSelectKoma();
	if (pKoma == NULL) return;

	int effectLayer = m_document->GetNowSelectEffectLayer();
	if (effectLayer == -1) return;

	if (pKoma->GetEffectFlag(effectLayer) == FALSE) return;

	int effect = pKoma->GetEffect(effectLayer);
	if (effect == -1) return;


	

	if (pDoc->GetEffectMenuPrintMode() == 0)
	{
		pDoc->OnClickEffectMenu(6);
		pDoc->UpdateMyWindow(EFFECT_WINDOW);
		pDoc->UpdateMainScreen();
	}
}


void CMainScreenView::PutLeftButton(POINT pt,int md)
{
	int x = pt.x;
	int y = pt.y;

	if (md == 0)
	{
		m_leftButtonPic->Put(x,y,TRUE);
	}
	else
	{
		m_leftButtonPic->TransLucentBlt2(x,y,0,0,64,24,50);
	}
}

void CMainScreenView::PutRightButton(POINT pt,int md)
{
	int x = pt.x;
	int y = pt.y;

	if (md == 0)
	{
		m_rightButtonPic->Put(x,y,TRUE);
	}
	else
	{
		m_rightButtonPic->TransLucentBlt2(x,y,0,0,64,24,50);
	}
}

POINT CMainScreenView::GetBarBaseZahyo(int effect,int n)
{
	POINT pt;
	int offset = m_effectTableOffset[effect];
	int type = m_controlWork[(offset+n)*3];

	int x = m_controlWork[(offset+n)*3+1];
	int y = m_controlWork[(offset+n)*3+2];

	x += m_effectBarPrintZahyo[m_leftRight].x;
	y += m_effectBarPrintZahyo[m_leftRight].y;

	pt.x = x;
	pt.y = y;

	return pt;
}

POINT CMainScreenView::GetButtonBaseZahyo(int effect,int n)
{
	POINT pt;
	int offset = m_effectTableOffset[effect];
	int type = m_controlWork[(offset+n)*3];

	int x = m_controlWork[(offset+n)*3+1];
	int y = m_controlWork[(offset+n)*3+2];

	x += m_effectButtonPrintZahyo[m_leftRight].x;
	y += m_effectButtonPrintZahyo[m_leftRight].y;

	pt.x = x;
	pt.y = y;

	return pt;
}

void CMainScreenView::PutSpeedTypePic(POINT pt,int d,int md)
{
	int putX = pt.x;
	int putY = pt.y;

	int sizeX = 32;
	int sizeY = 24;

	int rev = (d / 100) % 2;
	int dRnd = (d / 200);//未使用
	int kagensoku = (d % 100) / 15;


	if (d >= 75)
	{
//		kagensoku = 3;
	}

	int srcX = ((d % 100) % 15) * sizeX;
	int srcY = (kagensoku+rev*4+dRnd*8)*sizeY;

	if (md == 0)
	{
		m_speedTypePic->Blt(putX,putY,srcX,srcY,sizeX,sizeY,TRUE);
	}
	else
	{
		m_speedTypePic->TransLucentBlt2(putX,putY,srcX,srcY,sizeX,sizeY,50);
	}
}


void CMainScreenView::PutButtonPic(int type,POINT pt,int d,int md)
{
	int putX = pt.x;
	int putY = pt.y;

	int sizeX = 32;
	int sizeY = 24;

	int srcX = d  * sizeX;
	int srcY = 0;

	CPicture* lpPic = m_transButtonPic;
	if (type == 20) lpPic = m_transButton3Pic;
	if (type == 21) lpPic = m_onoffButtonPic;

	if (md == 0)
	{
		lpPic->Blt(putX,putY,srcX,srcY,sizeX,sizeY,TRUE);
	}
	else
	{
		lpPic->TransLucentBlt2(putX,putY,srcX,srcY,sizeX,sizeY,50);
	}

}


void CMainScreenView::PutBar(int type,POINT pt,int d,int md)
{
	int putX = pt.x + m_gazeData[type][0];
	int putY = pt.y + m_gazeData[type][1];

	int sizeX = m_gazeData[type][2];
	int sizeY = m_gazeData[type][3];

	if (md == 0)
	{
		m_gazePic[type]->Put(putX,putY,TRUE);
	}
	else
	{
		m_gazePic[type]->TransLucentBlt2(putX,putY,0,0,sizeX,sizeY,50);
	}

	int sizeX2 = 11;
	int sizeY2 = 30;

	int putX2 = pt.x - 5;
	int putY2 = putY + sizeY/2 - sizeY2 / 2;

	putX2 += d;

	if (md == 0)
	{
		m_markPic->Put(putX2,putY2,TRUE);
	}
	else
	{
		m_markPic->TransLucentBlt2(putX2,putY2,0,0,sizeX2,sizeY2,50);
	}
}



POINT CMainScreenView::GetParamNameZahyo(int effect,int n)
{
	POINT pt = GetBarBaseZahyo(effect,n);

	pt.y += 4;
	
	if (m_leftRight == 0)
	{
		pt.x += (256+8+64+8);
	}
	else
	{
		pt.x -= (8+64+8+6*17);
	}

	return pt;
}



void CMainScreenView::PutParamName(POINT pt,LPSTR name,int md)
{
	//if (m_leftRight != 0) return;

	int putX = pt.x;
	int putY = pt.y;

	if (m_leftRight != 0)
	{
		int ln = strlen(name);
		if (ln>3)
		{
			if ((*name) == '#')
			{
				ln -= 3;
			}
		}

		putX += (6-ln/2)*17;
	}

	CAllGeo::TransBoxFill(pt.x-1,pt.y-1,17*6+1,18,64,192,128,60);
	if (name != NULL)
	{
		m_message->PrintMessage(putX, putY, name);
	}
	else
	{
		m_message->PrintMessage(putX, putY, "エラー");
	}
}




POINT CMainScreenView::GetStartPointXY(int effectLayer,int n,CEffectParam* effectParam,CKomaData* pKoma)
{
	int startX = 0;
	int startY = 0;

	int mae = effectParam->GetParaEditPara(n);
	if (mae < 0)
	{
		startX = pKoma->GetEffectPara(effectLayer,n+mae);
	}
	else if (mae>0)
	{
		startX = mae;
		if (mae == 9999)
		{
			startX = CMyGraphics::GetScreenSizeX() / 2;
		}
	}

	int mae2 = effectParam->GetParaEditPara(n+1);
	if (mae2 < 0)
	{
		startY = pKoma->GetEffectPara(effectLayer,n+1+mae2);
	}
	else if (mae2 > 0)
	{
		startY = mae2;
		if (mae2 == 9999)
		{
			startY = CMyGraphics::GetScreenSizeY() / 2;
		}

	}

	POINT pt;
	pt.x = startX;
	pt.y = startY;

	return pt;
}


int CMainScreenView::GetStartPointY(int effectLayer,int n,CEffectParam* effectParam,CKomaData* pKoma)
{
	int startY = 0;
	int mae2 = effectParam->GetParaEditPara(n);
	if (mae2 < 0)
	{
		startY = pKoma->GetEffectPara(effectLayer,n+mae2);
	}
	else if (mae2 > 0)
	{
		startY = mae2;
		if (mae2 == 9999)
		{
			startY = CMyGraphics::GetScreenSizeY() / 2;
		}
	}

	return startY;
}

void CMainScreenView::PutFrameGaze(POINT pt,int type)
{
	type = 0;

	int sizeX = 108;
	int sizeY = 12;
	int srcX = 0;
	int srcY = sizeY * type;
	int putX = pt.x;
	int putY = pt.y;
	m_frameGazePic->Blt(putX,putY,srcX,srcY,sizeX,sizeY,TRUE);
}

void CMainScreenView::PutFrameType(POINT pt,int type)
{
	int sizeX = 18;
	int sizeY = 12;
	int srcX = sizeX * type;
	int srcY = 0;
	int putX = pt.x;
	int putY = pt.y;
	m_frameTypePic->Blt(putX,putY,srcX,srcY,sizeX,sizeY,TRUE);
}

void CMainScreenView::PutFrameMarker(POINT pt,int frame)
{
	int sizeX = 12;
	int sizeY = 12;
	int srcX = 0;
	int srcY = 0;
	if (frame > 100)
	{
		srcX += sizeX;
		frame = 100;
	}

	int putX = pt.x + frame;
	int putY = pt.y;

	m_frameMarkerPic->Blt(putX,putY,srcX,srcY,sizeX,sizeY,TRUE);
}

void CMainScreenView::PrintFrameSuuji(POINT pt,int d)
{
	if (d>9999) d = 9999;
	if (d<0) d = 0;

	int putX = pt.x;
	int putY = pt.y;
	int sizeX = 8;
	int sizeY = 12;

	int dv = 1000;
	BOOL flg = FALSE;
	for (int i=0;i<4;i++)
	{
		int c = d / dv;
		if ((c>0) || flg || (dv == 1))
		{
			flg = TRUE;
			int srcX = sizeX * c;
			int srcY = 0;
			m_frameNumberPic->Blt(putX,putY,srcX,srcY,sizeX,sizeY,TRUE);
			putX += sizeX;
		}

		d %= dv;
		dv /= 10;

	}
}


POINT CMainScreenView::GetEffectFrameZahyo(int updown)
{
	if (updown<0) updown = 0;
	if (updown > 1) updown = 1;

	POINT pt;

	pt.x = m_effectFrameZahyo[updown].x;
	pt.y = m_effectFrameZahyo[updown].y;


	return pt;
}


POINT CMainScreenView::GetEffectTypeZahyo(int updown)
{
	POINT pt = GetEffectFrameZahyo(updown);
	pt.x -= (4+18);
	return pt;
}


int CMainScreenView::GetOnEffectGaze(int mouseX,int mouseY,int updown)
{
	POINT pt = GetEffectFrameZahyo(updown);
	int x = mouseX - pt.x;
	int y = mouseY - pt.y;
	if ((y<0) || (y>=12)) return -1;
	if ((x<0) || (x>256)) return -1;
	return x;
}


BOOL CMainScreenView::CheckOnEffectType(int mouseX,int mouseY,int updown)
{
	POINT pt = GetEffectTypeZahyo(updown);
	int x = mouseX - pt.x;
	int y = mouseY - pt.y;

	if ((x>=0) && (x<18) && (y>=0) && (y<12)) return TRUE;

	return FALSE;
}


int CMainScreenView::GetOnGaze(int mouseX,int mouseY,int effect,int n)
{
	POINT pt = GetBarBaseZahyo(effect,n);
	int x = mouseX - pt.x;
	int y = mouseY - pt.y;

	if ((x>=0) && (x<=255) && (y>=0) && (y<=(16+3))) return x;

	return -1;
}


POINT CMainScreenView::GetLeftArrowButtonZahyo(int effect,int n)
{
	POINT pt = GetBarBaseZahyo(effect,n);

	pt.x += m_effectLeftButtonPrintZahyo[m_leftRight].x;
	pt.y += m_effectLeftButtonPrintZahyo[m_leftRight].y;

	return pt;
}

POINT CMainScreenView::GetRightArrowButtonZahyo(int effect,int n)
{
	POINT pt = GetBarBaseZahyo(effect,n);

	pt.x += m_effectRightButtonPrintZahyo[m_leftRight].x;
	pt.y += m_effectRightButtonPrintZahyo[m_leftRight].y;

	return pt;
}

int CMainScreenView::GetOnArrowButton(int mouseX,int mouseY,int effect,int n)
{
	POINT pt = GetLeftArrowButtonZahyo(effect,n);

	int x = mouseX - pt.x;
	int y = mouseY - pt.y;

	int sizeX = 16;
	int sizeY = 24;

	if ((x>=0) && (x<sizeX*4) && (y>=0) && (y<sizeY)) return x/sizeX;


	pt = GetRightArrowButtonZahyo(effect,n);
	x = mouseX - pt.x;
	y = mouseY - pt.y;

	if ((x>=0) && (x<sizeX*4) && (y>=0) && (y<sizeY)) return (x/sizeX) + 5;	//4は■ボタン用予約

	return -1;
}


BOOL CMainScreenView::CheckOnParamName(int mouseX,int mouseY,int effect,int n)
{
	POINT pt = GetParamNameZahyo(effect,n);
	int x = mouseX - pt.x;
	int y = mouseY - pt.y;

	if ((x>=0) && (x<16*7) && (y>=0) && (y<16))
	{
		return TRUE;
	}

	return FALSE;
}


BOOL CMainScreenView::CheckOnStatusButton(int mouseX,int mouseY,int effect,int n)
{
	POINT pt = GetButtonBaseZahyo(effect,n);
	int x = mouseX - pt.x;
	int y = mouseY - pt.y;

	if ((x>=0) && (x<32) && (y>=0) && (y<24)) return TRUE;

	return FALSE;
}





void CMainScreenView::PrintPlayIcon(int playMode,int storyMode)
{
	int putX = 700;
	int putY = 8;
	int sizeX = 96;
	int sizeY = 32;
	int srcX = 0;
	int srcY = playMode * sizeY;
	if ((storyMode != 0) && (playMode != 0))
	{
		srcY += sizeY * 2;
	}
	m_playModeIconPic->Blt(putX,putY,srcX,srcY,sizeX,sizeY,TRUE);
}

void CMainScreenView::PrintLine3(int sx,int sy,int ex,int ey,int r,int g,int b)
{
	for (int i=0;i<2;i++)
	{
		CAllGeo::Line(sx-1+i,sy+1,ex-1+i,ey+1,r,g,b);
	}

	for (int j=0;j<1;j++)
	{
		CAllGeo::Line(sx+1,sy-1+j,ex+1,ey-1+j,r,g,b);
	}
}


BOOL CMainScreenView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 



	int subType = -1;
	int type = MAINSCREEN_WINDOW;

	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

//	OutputDebugString("*moveMouse layer*\x00f\x00a");
	return FALSE;
}

/*_*/

