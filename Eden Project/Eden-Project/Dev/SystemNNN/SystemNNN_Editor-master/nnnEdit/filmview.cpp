//
// filmview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\wheelMouse.h"

#include "bitmapnumber.h"

#include "mybutton.h"

#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"

#include "mydocument.h"

#include "myBitmap.h"


//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"


#include "filmview.h"
#include "filmdoc.h"

#include "filmdata.h"
#include "komadata.h"



//CFilmView::CFilmView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument, clientHWND, hinstance)
CFilmView::CFilmView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("filmWindow");

	SetResizeFlag();
	SetBlockSize(72,24+48+24);
	SetNonBlockArea(0,24,0,0);

	SetBlockKosuuMin(2,1);


	SetBlockKosuuMax(32,1);
//	SetFreeSizeFlag(1);
	SetScrollFlag(TRUE,FALSE);

	SetWindowStyle();


	SetViewWindowText("フィルム");


//	SetScrollPara(0,5,6);
//	SetScrollArea(0,24,640,24+24*4*2);


//	m_document = pDocument;
//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_HSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_selObj = new CSelectObjectOnly();


//	SetWindowText(m_hWnd,"フィルム-なんたらかんたら");

	m_komaPrintX = 0;
	m_komaPrintY = 24;
	m_komaNextX = 72;
	m_komaNextY = 0;

	m_overrapButtonPrintX = 208;

	m_printKomaKosuu = 6;

	m_filmPicKosuu = 0;
	m_lpHBitmapDIB = NULL;
	m_lplpDIBBuffer = NULL;
	m_lpDIBNumberTable = NULL;

//	m_lpHBitmap = new HBITMAP[m_filmPicKosuuMax];

	m_scrollFlag[1] = TRUE;
	SetScrollArea(m_komaPrintX,m_komaPrintY,m_komaNextX * m_printKomaKosuu,m_komaPrintY + 24+48+24);

//	SetBlockSize(72,24+48+24);
	SetScrollPara(0,30,6);	//dummy

	m_filmBitmap = new CMyBitmap("nnndir\\setup\\bmp\\film64x48.bmp");

	m_filmPartsBitmap = new CMyBitmap("nnndir\\setup\\bmp\\film.bmp");
	m_overrapBitmap = new CMyBitmap("nnndir\\setup\\bmp\\overrap_mini.bmp");
	m_fontBitmap = new CMyBitmap("nnndir\\setup\\bmp\\nekofont_s.bmp");
	m_meswinMiniBitmap = new CMyBitmap("nnndir\\setup\\bmp\\meswinMini.bmp");
	m_onpuBitmap = new CMyBitmap("nnndir\\setup\\bmp\\onpu.bmp");
	m_kyufuBitmap = new CMyBitmap("nnndir\\setup\\bmp\\kyufu.bmp");


	m_cutinBitmap = new CMyBitmap("nnndir\\setup\\bmp\\cutin.bmp");
	m_preloadBitmap = new CMyBitmap("nnndir\\setup\\bmp\\preload.bmp");

	m_expStatusBitmap = new CMyBitmap("nnndir\\setup\\bmp\\expStatus.bmp");//dummy
	m_varControlBitmap = new CMyBitmap("nnndir\\setup\\bmp\\varcontrol.bmp");



	m_hBitmapFilm64x48 = m_filmBitmap->GetHBitmap();
	m_hBitmapFilmParts = m_filmPartsBitmap->GetHBitmap();

	//m_hBitmapFilm64x48 = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_FILM64X48));

//	HWND hwnd = m_view->GetHWND();


//	HWND hwnd0 = GetDesktopWindow();
//	HDC hdc0 = GetDC(hwnd0);
//	int bpp = GetDeviceCaps(hdc0,BITSPIXEL);
//	ReleaseDC(hwnd0,hdc0);
//
//	m_directX = new CMyDirectX(m_hWnd,m_hInstance,640,480,bpp,FALSE);

	AddFilmPicBuffer(64);


	m_buttonNew = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NEW),0,0);

	m_buttonCut = new CMyButton(1,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),32,0);
	m_buttonCopy = new CMyButton(2,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_COPY),56,0);
	m_buttonPaste = new CMyButton(3,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_PASTE),80,0);

	m_buttonDelete = new CMyButton(4,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_DELETE),112,0);

	m_buttonUndo = new CMyButton(5,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_UNDO),144,0);

	m_buttonOverrapType = new CMyButton(6,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_OVERRAPTYPE),m_overrapButtonPrintX,0,32,24);
	m_buttonOverrap = new CMyButton(7,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_OVERRAP),m_overrapButtonPrintX+32,0);

	m_buttonSetCG = new CMyButton(8,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SETCG),208+32+36,0);


//	m_buttonNoClearEffect = new CMyButton(9,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NOCLEAREFFECT),208+24+96,0);

	m_buttonBGM = new CMyButton(10,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_BGM),208+24+96 + 48,0);

	m_buttonFrame = new CMyButton(11,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_FRAMESPEED),208+24+96 + 48 + 48,0);

	m_buttonWindowOff = new CMyButton(12,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_WINDOWONOFF),208+24+96 + 48 + 48 + 48,0);


	m_buttonDemoBitmap = new CMyBitmap("nnndir\\setup\\bmp\\demoButton.bmp");
	m_buttonDemo = new CMyButton(13,m_hWnd,m_buttonDemoBitmap->GetHBitmap(),208+24+96 + 48 + 48 + 48+48,0);


	m_buttonWindowNumberBitmap = new CMyBitmap("nnndir\\setup\\bmp\\windownumber.bmp");
	m_buttonCursorNumberBitmap = new CMyBitmap("nnndir\\setup\\bmp\\cursornumber.bmp");
	m_buttonMouseNumberBitmap = new CMyBitmap("nnndir\\setup\\bmp\\mousenumber.bmp");

	m_buttonWindowNumber = new CMyButton(14,m_hWnd,m_buttonWindowNumberBitmap->GetHBitmap(),208+24+96 + 48 + 48 + 48+48+48,0);
	m_buttonCursorNumber = new CMyButton(15,m_hWnd,m_buttonCursorNumberBitmap->GetHBitmap(),208+24+96 + 48 + 48 + 48+48+48+24,0);
	m_buttonMouseNumber = new CMyButton(16,m_hWnd,m_buttonMouseNumberBitmap->GetHBitmap(),208+24+96 + 48 + 48 + 48+48+48+24*2,0);


	m_buttonAutoMessageBitmap = new CMyBitmap("nnndir\\setup\\bmp\\autoMessage.bmp");
	m_buttonCannotClickBitmap = new CMyBitmap("nnndir\\setup\\bmp\\cannotClick.bmp");
	m_buttonCannotSkipBitmap = new CMyBitmap("nnndir\\setup\\bmp\\cannotSkip.bmp");

	m_buttonOptionOffBitmap = new CMyBitmap("nnndir\\setup\\bmp\\optionOff.bmp");

	m_buttonAutoMessage = new CMyButton(17,m_hWnd,m_buttonAutoMessageBitmap->GetHBitmap(),660,0);
	m_buttonCannotClick = new CMyButton(18,m_hWnd,m_buttonCannotClickBitmap->GetHBitmap(),684,0);
	m_buttonCannotSkip = new CMyButton(19,m_hWnd,m_buttonCannotSkipBitmap->GetHBitmap(),708,0);

	m_buttonOptionOff = new CMyButton(20,m_hWnd,m_buttonOptionOffBitmap->GetHBitmap(),740,0);

	m_buttonCutin = new CMyButton(21,m_hWnd,m_cutinBitmap->GetHBitmap(),772,0);
	m_buttonPreload = new CMyButton(22,m_hWnd,m_preloadBitmap->GetHBitmap(),804,0);

	m_buttonExpStatus = new CMyButton(23,m_hWnd,m_expStatusBitmap->GetHBitmap(),836,0);
	m_buttonVarControl = new CMyButton(24,m_hWnd,m_varControlBitmap->GetHBitmap(),868,0);

	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_KOMA));



	AddBalloonCheckButton(m_buttonNew);
	AddBalloonCheckButton(m_buttonCut);
	AddBalloonCheckButton(m_buttonCopy);
	AddBalloonCheckButton(m_buttonPaste);
	AddBalloonCheckButton(m_buttonDelete);
	AddBalloonCheckButton(m_buttonUndo);
	AddBalloonCheckButton(m_buttonOverrapType);
	AddBalloonCheckButton(m_buttonOverrap);
	AddBalloonCheckButton(m_buttonSetCG);
	AddBalloonCheckButton(m_buttonBGM);
	AddBalloonCheckButton(m_buttonFrame);
	AddBalloonCheckButton(m_buttonWindowOff);
	AddBalloonCheckButton(m_buttonDemo);
	AddBalloonCheckButton(m_buttonWindowNumber);
	AddBalloonCheckButton(m_buttonCursorNumber);
	AddBalloonCheckButton(m_buttonMouseNumber);
	AddBalloonCheckButton(m_buttonAutoMessage);
	AddBalloonCheckButton(m_buttonCannotClick);
	AddBalloonCheckButton(m_buttonCannotSkip);
	AddBalloonCheckButton(m_buttonOptionOff);
	AddBalloonCheckButton(m_buttonCutin);
	AddBalloonCheckButton(m_buttonPreload);
	AddBalloonCheckButton(m_buttonExpStatus);
	AddBalloonCheckButton(m_buttonVarControl);




	ReCalcuScrollPara();

}



CFilmView::~CFilmView()
{
	End();
}

void CFilmView::End(void)
{
//	for (int i=0;i<FILM_PIC_KOSUU_MAX;i++)
//	{
//		DELETEGDIOBJECT(m_hBitmapDIB[i]);
//	}


	if (m_lpHBitmapDIB != NULL)
	{
		for (int i=0;i<m_filmPicKosuu;i++)
		{
			DELETEGDIOBJECT(m_lpHBitmapDIB[i]);
		}
		DELETEARRAY(m_lpHBitmapDIB);
	}
	DELETEARRAY(m_lpDIBNumberTable);

	DELETEARRAY(m_lplpDIBBuffer);

	ENDDELETECLASS(m_buttonOptionOff);
	ENDDELETECLASS(m_buttonAutoMessage);
	ENDDELETECLASS(m_buttonCannotClick);
	ENDDELETECLASS(m_buttonCannotSkip);

	ENDDELETECLASS(m_buttonOptionOffBitmap);
	ENDDELETECLASS(m_buttonAutoMessageBitmap);
	ENDDELETECLASS(m_buttonCannotClickBitmap);
	ENDDELETECLASS(m_buttonCannotSkipBitmap);

	ENDDELETECLASS(m_buttonMouseNumber);
	ENDDELETECLASS(m_buttonCursorNumber);
	ENDDELETECLASS(m_buttonWindowNumber);

	ENDDELETECLASS(m_buttonMouseNumberBitmap);
	ENDDELETECLASS(m_buttonCursorNumberBitmap);
	ENDDELETECLASS(m_buttonWindowNumberBitmap);
	ENDDELETECLASS(m_cutinBitmap);
	ENDDELETECLASS(m_preloadBitmap);
	ENDDELETECLASS(m_expStatusBitmap);
	ENDDELETECLASS(m_varControlBitmap);


	ENDDELETECLASS(m_buttonDemo);
	ENDDELETECLASS(m_buttonDemoBitmap);

	ENDDELETECLASS(m_buttonNew);
	ENDDELETECLASS(m_buttonCut);
	ENDDELETECLASS(m_buttonCopy);
	ENDDELETECLASS(m_buttonPaste);
	ENDDELETECLASS(m_buttonDelete);
	ENDDELETECLASS(m_buttonUndo);
	ENDDELETECLASS(m_buttonOverrap);
	ENDDELETECLASS(m_buttonOverrapType);
	ENDDELETECLASS(m_buttonSetCG);
//	ENDDELETECLASS(m_buttonNoClearEffect);
	ENDDELETECLASS(m_buttonBGM);
	ENDDELETECLASS(m_buttonFrame);
	ENDDELETECLASS(m_buttonWindowOff);
	ENDDELETECLASS(m_buttonCutin);
	ENDDELETECLASS(m_buttonPreload);
	ENDDELETECLASS(m_buttonExpStatus);
	ENDDELETECLASS(m_buttonVarControl);

//	DELETEGDIOBJECT(m_hBitmapFilm64x48);
	ENDDELETECLASS(m_kyufuBitmap);
	ENDDELETECLASS(m_onpuBitmap);
	ENDDELETECLASS(m_meswinMiniBitmap);
	ENDDELETECLASS(m_fontBitmap);
	ENDDELETECLASS(m_overrapBitmap);
	ENDDELETECLASS(m_filmPartsBitmap);
	ENDDELETECLASS(m_filmBitmap);

//	ENDDELETECLASS(m_directX);
}



LRESULT CFilmView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
int n;

	CFilmDoc* pDoc = (CFilmDoc*)m_document;
	int x,y;

	switch (message)
	{
	case WM_MOUSEWHEEL:
		OnWheelScroll(message,wParam,lParam);
		return TRUE;
		break;
	case WM_HSCROLL:
		OnScroll(message,wParam,lParam);
		break;
	case WM_LBUTTONDOWN:
		if (m_buttonNew != NULL) m_buttonNew->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCut != NULL) m_buttonCut->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCopy != NULL) m_buttonCopy->CalcuLButtonDown(wParam,lParam);
		if (m_buttonPaste != NULL) m_buttonPaste->CalcuLButtonDown(wParam,lParam);
		if (m_buttonDelete != NULL) m_buttonDelete->CalcuLButtonDown(wParam,lParam);
		if (m_buttonUndo != NULL) m_buttonUndo->CalcuLButtonDown(wParam,lParam);
		if (m_buttonOverrap != NULL) m_buttonOverrap->CalcuLButtonDown(wParam,lParam);
		if (m_buttonOverrapType != NULL) m_buttonOverrapType->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSetCG != NULL) m_buttonSetCG->CalcuLButtonDown(wParam,lParam);
//		if (m_buttonNoClearEffect != NULL) m_buttonNoClearEffect->CalcuLButtonDown(wParam,lParam);
		if (m_buttonBGM != NULL) m_buttonBGM->CalcuLButtonDown(wParam,lParam);
		if (m_buttonFrame != NULL) m_buttonFrame->CalcuLButtonDown(wParam,lParam);
		if (m_buttonWindowOff != NULL) m_buttonWindowOff->CalcuLButtonDown(wParam,lParam);
		if (m_buttonDemo != NULL) m_buttonDemo->CalcuLButtonDown(wParam,lParam);
		if (m_buttonWindowNumber != NULL) m_buttonWindowNumber->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCursorNumber != NULL) m_buttonCursorNumber->CalcuLButtonDown(wParam,lParam);
		if (m_buttonMouseNumber != NULL) m_buttonMouseNumber->CalcuLButtonDown(wParam,lParam);
		if (m_buttonAutoMessage != NULL) m_buttonAutoMessage->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCannotClick != NULL) m_buttonCannotClick->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCannotSkip != NULL) m_buttonCannotSkip->CalcuLButtonDown(wParam,lParam);
		if (m_buttonOptionOff != NULL) m_buttonOptionOff->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCutin != NULL) m_buttonCutin->CalcuLButtonDown(wParam,lParam);
		if (m_buttonPreload != NULL) m_buttonPreload->CalcuLButtonDown(wParam,lParam);
		if (m_buttonExpStatus != NULL) m_buttonExpStatus->CalcuLButtonDown(wParam,lParam);
		if (m_buttonVarControl != NULL) m_buttonVarControl->CalcuLButtonDown(wParam,lParam);

		x = LOWORD(lParam);
		y = HIWORD(lParam);
		n = GetContentsNumber(x,y);
		if (n != -1)
		{
			pDoc->OnClickFilm(n,wParam);
		}

		break;
	case WM_RBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		n = GetContentsNumber(x,y);
		if (n != -1)
		{
			int pp = OpenPopupMenu(wParam,lParam);
			if (pp == ID_MENU_SETKOMANAME)
			{
				pDoc->OnChangeKomaName(n);
			}
		}
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_USER_LBUTTONDOWN:
			switch (lParam)
			{
			case 0:
				pDoc->OnNewKoma();
				break;
			case 1:
				pDoc->OnCutKoma();
				break;
			case 2:
				pDoc->OnCopyKoma();
				break;
			case 3:
				pDoc->OnPasteKoma();
				break;
			case 4:
				pDoc->OnDelete();
				break;
			case 5:
				pDoc->OnUndo();
				break;
			case 6:
				pDoc->OnOverrapType();
				break;
			case 7:
				pDoc->OnOverrap();
				break;
			case 8:
				pDoc->OnSetCG();
				break;
//			case 9:
//				pDoc->OnNoClearEffect();
//				break;
			case 10:
				pDoc->OnBGM();
				break;
			case 11:
				pDoc->OnFrame();
				break;
			case 12:
				pDoc->OnWindowOff();
				break;
			case 13:
				pDoc->OnDemo();
				break;
			case 14:
				pDoc->OnWindowNumber();
				break;
			case 15:
				pDoc->OnCursorNumber();
				break;
			case 16:
				pDoc->OnMouseNumber();
				break;
			case 17:
				pDoc->OnAutoMessage();
				break;
			case 18:
				pDoc->OnCannotClick();
				break;
			case 19:
				pDoc->OnCannotSkip();
				break;
			case 20:
				pDoc->OnOptionOff();
				break;
			case 21:
				pDoc->OnCutin();
				break;
			case 22:
				pDoc->OnPreload();
				break;
			case 23:
				pDoc->OnExpStatus();
				break;
			case 24:
				pDoc->OnVarControl();
				break;
			}
			break;
		}
		break;

	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;
	case WM_ERASEBKGND:
		EraseSplit((HDC)wParam);
		return TRUE;
		break;

	case WM_PAINT:
		OnPaint(hWnd,wParam,lParam);

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

void CFilmView::OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	HDC src;
	HGDIOBJ old;
	int i;
	//int j;

	CFilmDoc* pDoc = (CFilmDoc*)m_document;

	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd,&ps);

	CFilmData* pFilm;
	pFilm = pDoc->GetNowSelectFilm();


	if (pFilm == NULL)
	{
		RECT rcClient;
		GetClientRect(hWnd,&rcClient);

		HGDIOBJ old = SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));
		PatBlt(hdc,0,24,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 24,PATCOPY);
		SelectObject(hdc,old);

		EndPaint(hWnd,&ps);
		return;
	}



	src = CreateCompatibleDC(hdc);

		if (m_buttonNew != NULL) m_buttonNew->Print(hdc,src,0,&ps.rcPaint);
				
		if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonCopy != NULL) m_buttonCopy->Print(hdc,src,0,&ps.rcPaint);
		
		if (m_buttonPaste != NULL)
		{
			int md = 2;
			if (pDoc->CheckBufferExist()) md = 0;
			m_buttonPaste->Print(hdc,src,md,&ps.rcPaint);
		}

		if (m_buttonDelete != NULL) m_buttonDelete->Print(hdc,src,0,&ps.rcPaint);

		if (m_buttonUndo != NULL)
		{
			int umd = 2;
			if (pDoc->CheckExistUndo()) umd = 0;
			m_buttonUndo->Print(hdc,src,umd,&ps.rcPaint);
		}

	CKomaData* pNowKoma = (CKomaData*)(pFilm->GetNowSelectObject());



	if (m_buttonOverrapType != NULL)
	{
		int overrapType = -2;
		if (pNowKoma != NULL)
		{
			if (pNowKoma->GetOverrapCount() > 0)
			{
				overrapType = pNowKoma->GetOverrapType();
			}
			else
			{
				overrapType = -1;
			}
		}

		if (overrapType == -2)
		{
			m_buttonOverrapType->Print(hdc,src,2,&ps.rcPaint);
		}
		else if (overrapType == -1)
		{
			m_buttonOverrapType->Print(hdc,src,0,&ps.rcPaint);
		}
		else
		{
			int putX = m_overrapButtonPrintX;
			int putY = 0;

			int sizeX = 32;
			int sizeY = 24;
			int srcX = (overrapType % 10) * sizeX;
			int srcY = (overrapType / 10) * sizeY;

			m_overrapBitmap->Blt(hdc,putX,putY,srcX,srcY,sizeX,sizeY);
		}
	}


	if (m_buttonOverrap != NULL)
	{
		int md = 2;
		if (pNowKoma != NULL)
		{
			if (pNowKoma->GetOverrapCount() > 0)
			{
				md = 1;
			}
			else
			{
				md = 0;
			}
		}

		m_buttonOverrap->Print(hdc,src,md,&ps.rcPaint);
	}


		
	if (m_buttonBGM != NULL)
	{
		int md = 0;
		int bgm = pDoc->GetBGM();

		if (bgm > 0) md = 1;
//		if (bgm == 0) md = 0;	//nothing
		if (bgm == -1) md = 3;	//off
		if (bgm == -2) md = 2;	//vol

		m_buttonBGM->Print(hdc,src,md,&ps.rcPaint);
	}


	if (m_buttonFrame != NULL)
	{
		int md = 0;
		int frame = pDoc->GetFrameTime();
		if (frame != 0) md = 1;

		m_buttonFrame->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonSetCG != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			if (pDoc->GetSetCG() != 0) md = 1;
		}
		m_buttonSetCG->Print(hdc,src,md,&ps.rcPaint);
	}

		
	if (m_buttonDemo != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetDemoFlag();
		}
		m_buttonDemo->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonWindowNumber != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			if (pDoc->GetWindowNumber() != 0) md = 1;
		}
		m_buttonWindowNumber->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonCursorNumber != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			if (pDoc->GetCursorNumber() != 0) md = 1;
		}
		m_buttonCursorNumber->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonMouseNumber != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			if (pDoc->GetMouseNumber() != 0) md = 1;
		}
		m_buttonMouseNumber->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonAutoMessage != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetAutoMessage();
			if (md>1) md = 1;
			if (md == -1) md = 2;
		}
		m_buttonAutoMessage->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonCannotClick != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetCannotClick();
			if (md == -1) md = 2;
		}
		m_buttonCannotClick->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonCannotSkip != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetCannotSkip();
			if (md == -1) md = 2;
		}
		m_buttonCannotSkip->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonOptionOff != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetOptionOff();
			if (md == -1) md = 2;
		}
		m_buttonOptionOff->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonCutin != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetCutinFlag();
			if (md == -1) md = 2;
		}
		m_buttonCutin->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonPreload != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetPreload();
		}
		m_buttonPreload->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonExpStatus != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->GetExpStatus();
		}
		m_buttonExpStatus->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonVarControl != NULL)
	{
		int md = 0;
		if (pDoc != NULL)
		{
			md = pDoc->CheckVarControl();
			if (md != 0) md = 1;

		}
		m_buttonVarControl->Print(hdc,src,md,&ps.rcPaint);

	}


/*
		if (m_buttonNoClearEffect != NULL)
		{
			int md = 1;
			if (pFilm != NULL)
			{
				if (pFilm->GetNoClearEffect()) md = 0;
			}
			m_buttonNoClearEffect->Print(hdc,src,md,&ps.rcPaint);
		}
*/


		if (pFilm != NULL)
		{
			int nowSelect = pFilm->GetNowSelectNumber();
			int kosuuMax = pFilm->GetObjectKosuu();
			int subSelect = pFilm->GetNowSelectSubNumber();





			if (m_buttonWindowOff != NULL)
			{
				int md = 2;
				if (nowSelect < kosuuMax)
				{
					CKomaData* pKoma00 = (CKomaData*)(pFilm->GetObjectData(nowSelect));
					int md0 = pKoma00->GetWindowOffFlag();

					if (md0 == 0) md = 0;
					if (md0 == 1) md = 3;
					if (md0 == 2) md = 1;
				}
				m_buttonWindowOff->Print(hdc,src,md,&ps.rcPaint);
			}



/*
			old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));

			RECT rc;

			for (i=0;i<GetBlockKosuu();i++)
			{
				int x,y;
				x = m_komaPrintX+m_komaNextX*i;
				y = m_komaPrintY;

				SetRect(&rc,x,y,x+m_komaNextX,y+20+48+20);
				if (IntersectRect(&ps.rcPaint,&rc,&rc) != 0)
				{
					for (j=0;j<2;j++)
					{
						for (int ii=0;ii<4;ii++)
						{
							PatBlt(hdc,x+18*ii+3,y+j*(12+2+2+48+2+2)+4,12,12,PATCOPY);
						}
					}

				}
			}

			for (j=0;j<2;j++)
			{
				for (i=0;i<32;i++)
				{
	//				PatBlt(hdc,i*72/4 ,8+j*(16+48+4),12,12,PATCOPY);
				}
			}

			SelectObject(hdc,old);
*/

			//わく1
			//わく2


//			old = SelectObject(src,m_lpHBitmapDIB[0]);
			old = m_document->SelObjOnly(src,m_lpHBitmapDIB[0]);	//@@@

	//		SelectObject(hdc,GetStockObject(GRAY_BRUSH));

			int pg;
			pg = GetMyScrollPos();

			for (i=0;i<GetBlockKosuu();i++)
			{
				int x,y;
				x = m_komaPrintX+m_komaNextX*i;
				y = m_komaPrintY;

				RECT rc;
				SetRect(&rc,x,y,x+72,y+96);
				if (IntersectRect(&ps.rcPaint,&rc,&rc) != 0)
				{
					int k = pg + i;
//					if (k>63) k = 63;

					if ((k>=0) && (k<kosuuMax))
					{
						HBITMAP hbitmap = pDoc->GetHBitmap(k);
						if (hbitmap != NULL)
						{
//						CKomaData* pKoma = pFilm->GetKoma(k);
//
//						//つくるか??
//						int bufNum = m_lpDIBNumberTable[k];
//
//						if (pKoma->CheckLargePic() == FALSE)
//						{
//							int* lpBuffer = m_lplpDIBBuffer[bufNum];
//							pKoma->Make64x48Pic(lpBuffer);
//						}
//
							SelectObject(src,hbitmap);
						}
					}




					if (k<0)
					{
						//error
						PutFilmParts(hdc,x,y,6,src);


//						HGDIOBJ old = SelectObject(src,m_hBitmapFilm64x48);
//						BitBlt(hdc,x,y,64,48,src,0,96,SRCCOPY);
//						SelectObject(src,old);
					}
					else if (k == kosuuMax)
					{
						//new
						if (k == nowSelect)
						{
							PutFilmParts(hdc,x,y,2,src);

//							HGDIOBJ old3 = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
//							PatBlt(hdc,x-3,y-3,64+6,48+6,PATCOPY);					
//							SelectObject(hdc,old3);

//							HGDIOBJ old = SelectObject(src,m_hBitmapFilm64x48);
//							HGDIOBJ old = m_document->SelObjOnly(src,m_hBitmapFilm64x48);	//@@@
//							BitBlt(hdc,x,y,64,48,src,0,0,NOTSRCCOPY);
//							SelectObject(src,old);
						}
						else
						{
							PutFilmParts(hdc,x,y,1,src);
//							HGDIOBJ old = SelectObject(src,m_hBitmapFilm64x48);
//							HGDIOBJ old = m_document->SelObjOnly(src,m_hBitmapFilm64x48);	//@@@
//							BitBlt(hdc,x,y,64,48,src,0,0,SRCCOPY);
//							SelectObject(src,old);
						}
					}
					else if (k>kosuuMax)
					{
						PutFilmParts(hdc,x,y,0,src);

						//aki
//						HGDIOBJ old = SelectObject(src,m_hBitmapFilm64x48);
//						HGDIOBJ old = m_document->SelObjOnly(src,m_hBitmapFilm64x48);	//@@@
//						BitBlt(hdc,x,y,64,48,src,0,48,SRCCOPY);
//						SelectObject(src,old);
					}
					else
					{
						if (k != nowSelect)
						{
//							if ((subSelect != -1) && (k>nowSelect) && (k<=subSelect))
							if (pFilm->CheckInSubArea(k))
							{
//								HGDIOBJ old4 = SelectObject(hdc,GetStockObject(GRAY_BRUSH));
//								PatBlt(hdc,x-3,y-3,64+6,48+6,PATCOPY);					
//								SelectObject(hdc,old4);
							
//								BitBlt(hdc,x,y,64,48,src,0,0,SRCCOPY);

						//		PatBlt(hdc,x-4,y-20,64+8,20,DSTINVERT);
						//		PatBlt(hdc,x-4,y+48,64+8,20,DSTINVERT);

								PutFilmParts(hdc,x,y,5,src);

							}
							else
							{
								PutFilmParts(hdc,x,y,3,src);
							}

							BitBlt(hdc,x+4,y+24,64,48,src,0,0,SRCCOPY);
						}
						else
						{
							PutFilmParts(hdc,x,y,4,src);

//							HGDIOBJ old3 = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
	///						PatBlt(hdc,x-3,y-3,64+6,48+6,PATCOPY);					
//							SelectObject(hdc,old3);
							
	///						BitBlt(hdc,x,y,64,48,src,0,0,NOTSRCCOPY);
							BitBlt(hdc,x+4,y+24,64,48,src,0,0,SRCCOPY);




//							PatBlt(hdc,x-4,y-20,64+8,20,DSTINVERT);
//							PatBlt(hdc,x-4,y+48,64+8,20,DSTINVERT);
						}


						//miniwin

						//overrrapflag,bgm
						
						CKomaData* pKoma = pFilm->GetKoma(k);
						if (pKoma != NULL)
						{
							int winoff = pKoma->GetWindowOffFlag();
							int winsw = 1;
							if (winoff == 1) winsw = 0;
							if (winoff == 2) winsw = 1;


							if (winsw)
							{
								m_meswinMiniBitmap->Blt(hdc,x+4+4,y+24+48-4-12,0,0,56,12,NULL,1);	//and
								m_meswinMiniBitmap->Blt(hdc,x+4+4,y+24+48-4-12,0,12,56,12,NULL,2);	//or
							}


							int overrapcount = pKoma->GetOverrapCount();
							if (overrapcount > 0)
							{
								//print yajirusi
								HBRUSH br = CreateSolidBrush(COLORREF(0x0000ff));
								HGDIOBJ old = SelectObject(hdc,br);

								POINT pt[3];
								pt[0].x = x-3;
								pt[0].y = y+24-4;
								pt[1].x = x-3;
								pt[1].y = y+24+4;
								pt[2].x = x+5;
								pt[2].y = y+24;

								Polygon(hdc,pt,3);


								//print time
								PrintMiniSuuji(hdc,x+9,y+24-6,overrapcount);

								SelectObject(hdc,old);
								DeleteObject(br);
							}

							if (m_buttonBGM != NULL)
							{
								int md = pKoma->GetBGMNumber();
								if (md>1) md = 1;
								if (md<0) md = 3;
								m_buttonBGM->Print(hdc,NULL,md,&ps.rcPaint);
							}

							if (1)
							{
								int md = pKoma->GetBGMNumber();
								if (md > 0)
								{
									m_onpuBitmap->Blt(hdc,x+48,y+18,16,0,16,16,NULL,1);
									m_onpuBitmap->Blt(hdc,x+48,y+18,0,0,16,16,NULL,2);
								}

								if (md<0)
								{
									m_kyufuBitmap->Blt(hdc,x+48,y+18,16,0,16,16,NULL,1);
									m_kyufuBitmap->Blt(hdc,x+48,y+18,0,0,16,16,NULL,2);
								}
							}

						}
					}
				}

			}
			SelectObject(src,old);
		}


		DeleteDC(src);
		EndPaint(hWnd,&ps);

}
/*
void CFilmView::ReCalcuScrollPara(void)
{
	CFilmDoc* pDoc = (CFilmDoc*)m_document;
	CFilmData* pFilm = pDoc->GetNowSelectFilm();
	if (pFilm == NULL)
	{
		SetScrollPara(0,0);
		return;
	}

	int kosuu = pFilm->GetObjectKosuu();
	int block = GetBlockKosuu();


	int pgMax = (kosuu + 1) - 1;
	int pos = GetMyScrollPos();
	if (pgMax<block-1) pgMax = block-1;

	int n = pFilm->GetNowSelectNumber();
	if (n != -1)
	{
		pos = n-(block-1);
		if (pos<0) pos = 0;
	}

	SetScrollPara(pos,pgMax,block);
}
*/


BOOL CFilmView::AddFilmPicBuffer(int n)
{
//	if ((place<0) || (place>m_filmPicKosuu)) place = m_filmPicKosuu - 1;

	HDC hdc = GetDC(m_clientHWND);
	BITMAPINFO bmi;

	ZeroMemory(&bmi,sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = 64;
	bmi.bmiHeader.biHeight = -48;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	HDC src = CreateCompatibleDC(hdc);

	HBITMAP* lpHBitmapDIB = new HBITMAP[m_filmPicKosuu+n];
	int** lplpDIBBuffer = new int*[m_filmPicKosuu+n];
	int* lpDIBNumberTable = new int[m_filmPicKosuu+n];

	int i;
	for (i=0;i<m_filmPicKosuu;i++)
	{
		*(lpHBitmapDIB+i) = m_lpHBitmapDIB[i];
		*(lplpDIBBuffer+i) = m_lplpDIBBuffer[i];
		*(lpDIBNumberTable+i) = m_lpDIBNumberTable[i];
	}

	for (i=m_filmPicKosuu;i<m_filmPicKosuu+n;i++)
	{
		*(lpHBitmapDIB+i) = CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,(void**)&lplpDIBBuffer[i],NULL,NULL);
		*(lpDIBNumberTable+i) = i;
//		int r1 = (rand() % 256);
//		for (int ii=0;ii<64*48;ii++)
//		{
//			*(m_dibBuffer[i] + ii) = r1;
//		}
//
//		HGDIOBJ old = SelectObject(src,m_hBitmapDIB[i]);
//
//		Ellipse(src,8,0,56,48);
//		char mes[256];
//		sprintf(mes,"%d",i);
//		TextOut(src,24,12,mes,strlen(mes));
//		SelectObject(src,old);
	}


	DELETEARRAY(m_lpHBitmapDIB);
	m_lpHBitmapDIB = lpHBitmapDIB;
	DELETEARRAY(m_lplpDIBBuffer);
	m_lplpDIBBuffer = lplpDIBBuffer;
	DELETEARRAY(m_lpDIBNumberTable);
	m_lpDIBNumberTable = lpDIBNumberTable;

	m_filmPicKosuu += n;

	DeleteDC(src);
	ReleaseDC(m_hWnd,hdc);

	return TRUE;
}


void CFilmView::PutFilmParts(HDC hdc,int x,int y,int n,HDC src)
{
	int sizeX = 72;
	int sizeY = 96;
	int srcX = n * sizeX;
	int srcY = 0;
	m_filmPartsBitmap->Blt(hdc,x,y,srcX,srcY,sizeX,sizeY);
}

void CFilmView::PrintMiniSuuji(HDC hdc,int x,int y,int d)
{
	int dv = 1000;
	int keta = 4;

	BOOL flg = FALSE;

	int sizeX = 12;
	int sizeY = 12;

	for (int i=0;i<keta;i++)
	{
		int c = d / dv;
		if ((c>0) || (i == keta-1) || (flg))
		{
			int srcX = (c % 16) * sizeX;
			int srcY = (c / 16) * sizeY;
			srcY += sizeY;

			m_fontBitmap->Blt(hdc,x,y,srcX,srcY,sizeX,sizeY);

			flg = TRUE;
			x += 12;
		}

		d = d % dv;
		dv /= 10;

	}
}

BOOL CFilmView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = FILM_WINDOW;
	int subType = CheckOnBalloonButton(x,y);

	if (subType == -1)
	{
		type = -1;
	}


	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}

BOOL CFilmView::OnControlKey(WPARAM wParam,LPARAM lParam)
{
	CFilmDoc* pDoc = (CFilmDoc*)m_document;

	if (wParam == 6)
	{
		//F
		//pDoc->OnSearchButton(FALSE);
		//return TRUE;
	}
	if (wParam == 19)
	{
		//S
//		pDoc->OnSaveButton();
//		return TRUE;
	}
	if (wParam == 3)
	{
		//C
		pDoc->OnCopyKoma();
		return TRUE;
	}
	if (wParam == 24)
	{
		//X
		pDoc->OnCutKoma();
		return TRUE;
	}
	if (wParam == 22)
	{
		//V
		pDoc->OnPasteKoma();
		return TRUE;
	}
	if (wParam == 14)
	{
		//N
		pDoc->OnNewKoma();
		return TRUE;
	}
	if (wParam == 15)
	{
		//O
	}
	if (wParam == 26)
	{
		return pDoc->OnUndo(); 
	}

	return FALSE;

}

/*_*/




