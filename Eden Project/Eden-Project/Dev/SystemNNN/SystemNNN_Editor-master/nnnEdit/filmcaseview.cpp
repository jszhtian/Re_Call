//
// filmcaseview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nnnUtilLib\wheelmouse.h"

#include "bitmapnumber.h"
#include "mybutton.h"

#include "myBitmap.h"
#include "mydib.h"

#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"


#include "filmdata.h"

#include "filmcasedoc.h"
#include "filmcaseview.h"


int CFilmCaseView::m_filmColorTable[]=
{
	ID_MENU_FILMCOLOR_BLACK,0,
	ID_MENU_FILMCOLOR_BLUE,0xC00000,
	ID_MENU_FILMCOLOR_RED,0x0000B0,
	ID_MENU_FILMCOLOR_PURPLE,0xC000B0,
	ID_MENU_FILMCOLOR_GREEN,0x00A000,
	ID_MENU_FILMCOLOR_CYAN,0xC0A000,
	ID_MENU_FILMCOLOR_YELLOW,0x00A0B0,
};



CFilmCaseView::CFilmCaseView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("filmCaseWindow");

	SetResizeFlag();
	SetBlockSize(480,24);
	SetNonBlockArea(0,24,0,0);

	SetBlockKosuuMin(1,2);

	//windowsizeをひろってくる
	//最大サイズを調整する
	SetBlockKosuuMax(1,64);

	SetFreeSizeFlag(1);
	SetScrollFlag(FALSE,TRUE);

	SetWindowStyle();

	SetViewWindowText("フィルムケース");

//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_VSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

	m_hBitmap1 = NULL;
	m_hBitmap2 = NULL;


//	m_hBitmap1 = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_FILMCASEPIC1));

	m_dib = new CMyDIB(m_hWnd,32,24);


	m_scrollFlag[0] = TRUE;


	SetScrollPara(0,4,5);
	SetScrollArea(0,24,480,24*24*5);



	m_captureCopyType = 0;
	m_captureFilm = -1;

	//m_captureCursorCopy = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_LAYERCOPY));
	m_captureCursorMoveAndInsert = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_LAYERMOVE));
	m_captureCursorExchange = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_LAYEREXCHANGE));
	m_captureCursorBad = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_NODROP));
	m_defaultCursor = LoadCursor(NULL,IDC_ARROW);



//	SetBlockSize(480,24);

//	HWND w = CreateWindow("BUTTON","",WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_PUSHBUTTON, 0,0,24,24,m_hWnd,(HMENU)ID_BUTTON_NEW,m_hInstance,NULL);
//	SendMessage(w,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_NEW)));
//	w = CreateWindow("BUTTON","",WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_PUSHBUTTON, 24,0,24,24,m_hWnd,(HMENU)ID_BUTTON_OPEN,m_hInstance,NULL);
//	SendMessage(w,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_OPEN)));
//	w = CreateWindow("BUTTON","",WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_PUSHBUTTON, 48,0,24,24,m_hWnd,(HMENU)ID_BUTTON_SAVE,m_hInstance,NULL);
//	SendMessage(w,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_SAVE)));
	m_buttonNew = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NEW),0,0);
	m_buttonOpen = new CMyButton(1,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_OPEN),24,0);
	m_buttonSave = new CMyButton(2,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SAVE),48,0);


	m_buttonCut = new CMyButton(10,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),80,0);
	m_buttonCopy = new CMyButton(11,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_COPY),104,0);
	m_buttonPaste = new CMyButton(12,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_PASTE),128,0);


	m_buttonDelete = new CMyButton(3,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_DELETE),160,0);

	m_buttonUndo = new CMyButton(4,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_UNDO),192,0);

	m_buttonSearch = new CMyButton(5,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SEARCH),224,0);

	m_buttonNoClearEffect = new CMyButton(6,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NOCLEAREFFECT),256,0);

	m_typeTimeBitmap = new CMyBitmap("nnndir\\setup\\bmp\\typetime.bmp");
	m_configMaskBitmap = new CMyBitmap("nnndir\\setup\\bmp\\configmask.bmp");
	m_taikenLevelBitmap = new CMyBitmap("nnndir\\setup\\bmp\\taikenLevel.bmp");
	m_cutinBitmap = new CMyBitmap("nnndir\\setup\\bmp\\cutin.bmp");

	m_skipFilmBitmap = new CMyBitmap("nnndir\\setup\\bmp\\skipfilm.bmp");

	m_renameLayerBitmap = NULL;
//	m_renameLayerBitmap = new CMyBitmap("nnndir\\setup\\bmp\\renamelayer.bmp");

//	m_buttonSpecialType = new CMyButton(7,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NOCLEAREFFECT),160+24+24,0);
//	m_buttonConfigMask = new CMyButton(8,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NOCLEAREFFECT),160+24+24*2,0);
	m_buttonSpecialType = new CMyButton(7,m_hWnd,m_typeTimeBitmap->GetHBitmap(),288,0);
	m_buttonConfigMask = new CMyButton(8,m_hWnd,m_configMaskBitmap->GetHBitmap(),312,0);

	m_buttonTaikenLevel = new CMyButton(9,m_hWnd,m_taikenLevelBitmap->GetHBitmap(),344,0);
	m_buttonCutin = new CMyButton(13,m_hWnd,m_cutinBitmap->GetHBitmap(),376,0);
	m_buttonSkipFilm = new CMyButton(14,m_hWnd,m_skipFilmBitmap->GetHBitmap(),408,0);

	m_buttonRenameLayer = NULL;
//	m_buttonRenameLayer = new CMyButton(15,m_hWnd,m_renameLayerBitmap->GetHBitmap(),440,0);

	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_FILMCASE));




	AddBalloonCheckButton(m_buttonNew);
	AddBalloonCheckButton(m_buttonOpen);
	AddBalloonCheckButton(m_buttonSave);
	
	AddBalloonCheckButton(m_buttonCut);
	AddBalloonCheckButton(m_buttonCopy);
	AddBalloonCheckButton(m_buttonPaste);
	AddBalloonCheckButton(m_buttonDelete);
	AddBalloonCheckButton(m_buttonUndo);
	AddBalloonCheckButton(m_buttonSearch);
	AddBalloonCheckButton(m_buttonNoClearEffect);
	AddBalloonCheckButton(m_buttonSpecialType);
	AddBalloonCheckButton(m_buttonConfigMask);
	AddBalloonCheckButton(m_buttonTaikenLevel);
	AddBalloonCheckButton(m_buttonCutin);
	AddBalloonCheckButton(m_buttonSkipFilm);




	ReCalcuScrollPara();
}

CFilmCaseView::~CFilmCaseView()
{
	End();
}



void CFilmCaseView::End(void)
{
//	DELETEGDIOBJECT(m_hBitmap1);
	DELETEGDIOBJECT(m_hBitmap2);

	ENDDELETECLASS(m_renameLayerBitmap);
	ENDDELETECLASS(m_taikenLevelBitmap);
	ENDDELETECLASS(m_configMaskBitmap);
	ENDDELETECLASS(m_typeTimeBitmap);
	ENDDELETECLASS(m_cutinBitmap);
	ENDDELETECLASS(m_skipFilmBitmap);

	ENDDELETECLASS(m_buttonRenameLayer);
	ENDDELETECLASS(m_buttonNew);
	ENDDELETECLASS(m_buttonOpen);
	ENDDELETECLASS(m_buttonSave);
	ENDDELETECLASS(m_buttonDelete);
	ENDDELETECLASS(m_buttonUndo);
	ENDDELETECLASS(m_buttonSearch);
	ENDDELETECLASS(m_buttonNoClearEffect);
	ENDDELETECLASS(m_buttonSpecialType);
	ENDDELETECLASS(m_buttonConfigMask);
	ENDDELETECLASS(m_buttonTaikenLevel);
	ENDDELETECLASS(m_buttonCutin);
	ENDDELETECLASS(m_buttonSkipFilm);

	ENDDELETECLASS(m_buttonCut);
	ENDDELETECLASS(m_buttonCopy);
	ENDDELETECLASS(m_buttonPaste);

	ENDDELETECLASS(m_dib);
}


LRESULT CFilmCaseView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int x,y;
	int n;
	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (m_buttonNew != NULL) m_buttonNew->CalcuLButtonDown(wParam,lParam);
		if (m_buttonOpen != NULL) m_buttonOpen->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSave != NULL) m_buttonSave->CalcuLButtonDown(wParam,lParam);
		if (m_buttonDelete != NULL) m_buttonDelete->CalcuLButtonDown(wParam,lParam);
		if (m_buttonUndo != NULL) m_buttonUndo->CalcuLButtonDown(wParam,lParam);
//		if (m_buttonSearch != NULL) m_buttonSearch->CalcuLButtonDown(wParam,lParam);
		if (m_buttonNoClearEffect != NULL) m_buttonNoClearEffect->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSpecialType != NULL) m_buttonSpecialType->CalcuLButtonDown(wParam,lParam);
		if (m_buttonConfigMask != NULL) m_buttonConfigMask->CalcuLButtonDown(wParam,lParam);
		if (m_buttonTaikenLevel != NULL) m_buttonTaikenLevel->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCutin != NULL) m_buttonCutin->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSkipFilm != NULL) m_buttonSkipFilm->CalcuLButtonDown(wParam,lParam);

		if (m_buttonCut != NULL) m_buttonCut->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCopy != NULL) m_buttonCopy->CalcuLButtonDown(wParam,lParam);
		if (m_buttonPaste != NULL) m_buttonPaste->CalcuLButtonDown(wParam,lParam);
		if (m_buttonRenameLayer != NULL) m_buttonRenameLayer->CalcuLButtonDown(wParam,lParam);


		x = LOWORD(lParam);
		y = HIWORD(lParam);


		n = GetContentsNumber(x,y);

		if (n != -1)
		{
			pDoc->OnSelectFilm(n);

			if (CheckOnFilmPic(x,y))
			{
				if (n<pDoc->GetFilmKosuu())
				{
					m_captureFilm = n;

					if (wParam & MK_CONTROL)
					{
						m_captureCopyType = 1;
					}
					else if (wParam & MK_SHIFT)
					{
						m_captureCopyType = 1;
					}
					else
					{
						m_captureCopyType = 0;
					}
				}
			}
		}
		break;

	case WM_RBUTTONDOWN:
		int cmd;
		MyReleaseCapture();

		x = LOWORD(lParam);
		y = HIWORD(lParam);
		n = GetContentsNumber(x,y);
		if (y==-1) break;
		if (n>=pDoc->GetFilmKosuu()) break;

		cmd = OpenPopupMenu(wParam,lParam);
		if (cmd == ID_MENU_FILMCASE_CHANGENAME)
		{
			pDoc->OnChangeName(n);
		}
		else
		{
			CheckChangeColor(n,cmd);
		}

		break;

	case WM_LBUTTONUP:
		if (m_captureFilm != -1)
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			int n = GetContentsNumber(x,y);

			if (n != -1)
			{
				if (CheckOnFilmPic(x,y))
				{
					if (n != m_captureFilm)
					{
						if (m_captureCopyType == 0) pDoc->ExchangeFilm(m_captureFilm,n);
						if (m_captureCopyType == 1) pDoc->MoveAndInsertFilm(m_captureFilm,n);
					}
				}
			}
		}
		MyReleaseCapture();
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_USER_LBUTTONDOWN:
			switch (lParam)
			{
			case 0:
				pDoc->OnNewFilm();
				break;
			case 1:
				pDoc->OnOpenFilm();
				break;
			case 2:
				pDoc->OnSaveFilm();
				break;
			case 3:
				pDoc->OnDelete();
				break;
			case 4:
				pDoc->OnUndo();
				break;
			case 5:
				pDoc->OnSearch();
				break;
			case 6:
				pDoc->OnNoClearEffect();
				break;
			case 7:
				pDoc->OnSpecialType();
				break;
			case 8:
				pDoc->OnConfigMask();
				break;
			case 9:
				pDoc->OnTaikenLevel();
				break;
			case 10:
				pDoc->OnCut();
				break;
			case 11:
				pDoc->OnCopy();
				break;
			case 12:
				pDoc->OnPaste();
				break;
			case 13:
				pDoc->OnCutin();
				break;
			case 14:
				pDoc->OnSkipFilm();
				break;
			case 15:
				pDoc->OnRenameLayer();
				break;
			}
			break;
		}
		break;

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
		{
			if (m_captureFilm != -1)
			{
				m_captureFlag = TRUE;
			}
			SetMyCursor(wParam,lParam);
		}

		break;


	case WM_MOUSEWHEEL:
		OnWheelScroll(message,wParam,lParam);
		return TRUE;
		break;

	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;
	case WM_VSCROLL:
		OnScroll(message,wParam,lParam);
		break;
	case WM_ERASEBKGND:
		EraseSplit((HDC)wParam);
		return TRUE;
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);

//		RECT tmpRect;
//		RECT rc;

		//ps.rcPaintとくらべてかきなおす IntersectRect IntersectClipRect


		HDC hdc = ps.hdc;
		HDC src = CreateCompatibleDC(hdc);

		CFilmData* pFilm;
		pFilm = pDoc->GetNowSelectFilm();


		if (m_buttonNew != NULL) m_buttonNew->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonOpen != NULL) m_buttonOpen->Print(hdc,src,0,&ps.rcPaint);

		if (m_buttonSave != NULL)
		{
			int md = 2;
			if (pFilm != NULL) md = 0;
			m_buttonSave->Print(hdc,src,md,&ps.rcPaint);
		}

		if (m_buttonDelete != NULL)
		{
			int md = 2;
			if (pFilm != NULL) md = 0;
			m_buttonDelete->Print(hdc,src,md,&ps.rcPaint);
		}


		if (m_buttonUndo != NULL)
		{
			int umd = 2;
			if (pDoc->CheckExistUndo()) umd = 0;

			m_buttonUndo->Print(hdc,src,umd,&ps.rcPaint);
		}
//		if (m_buttonSearch != NULL) m_buttonSearch->Print(hdc,src,0,&ps.rcPaint);

		if (m_buttonNoClearEffect != NULL)
		{
			int md = 2;
			if (pFilm != NULL)
			{
				md = 1;
				if (pFilm->GetNoClearEffect()) md = 0;
			}
			m_buttonNoClearEffect->Print(hdc,src,md,&ps.rcPaint);
		}


		if (m_buttonSpecialType != NULL)
		{
			int md = 0;
			if (pFilm != NULL)
			{
				int typeTime = pFilm->GetFilmSpecialTypeTime();
				if (typeTime < 0) md = 1;
				if (typeTime > 0) md = 2;
			}
			m_buttonSpecialType->Print(hdc,src,md,&ps.rcPaint);
		}

		if (m_buttonConfigMask != NULL)
		{
			int md = 0;
			if (pFilm != NULL)
			{
				int cm = pFilm->GetConfigMask();
				if (cm>0)
				{
					md = 1;
				}
				else if (cm<0)
				{
					md = 2;
				}
			}
			m_buttonConfigMask->Print(hdc,src,md,&ps.rcPaint);
		}

		if (m_buttonTaikenLevel != NULL)
		{
			int md = 0;
			if (pFilm != NULL)
			{
				if (pFilm->GetTaikenLevel() != 0)
				{
					md = 1;
				}
			}
			m_buttonTaikenLevel->Print(hdc,src,md,&ps.rcPaint);
		}

		if (m_buttonCutin != NULL)
		{
			int md = 0;
			if (pFilm != NULL)
			{
				if (pFilm->GetCutinFlag() != 0)
				{
					md = 1;
				}
			}
			m_buttonCutin->Print(hdc,src,md,&ps.rcPaint);
		}


		if (m_buttonSkipFilm != NULL)
		{
			int md = 0;
			if (pFilm != NULL)
			{
				int sf = pFilm->GetSkipToFilmEndEnable();
				if (sf == 1)
				{
					md = 1;
				}
			}
			m_buttonSkipFilm->Print(hdc,src,md,&ps.rcPaint);
		}


		if (m_buttonRenameLayer != NULL)
		{
			int md = 0;
			if (pFilm != NULL)
			{
				int sf = pFilm->GetRenameLayer();
				if (sf != 0)
				{
					md = 1;
				}
			}
			m_buttonRenameLayer->Print(hdc,src,md,&ps.rcPaint);

		}

		if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonCopy != NULL) m_buttonCopy->Print(hdc,src,0,&ps.rcPaint);
	
		if (m_buttonPaste != NULL)
		{
			int pmd = 2;
			if (pDoc->CheckBufferExist()) pmd = 0;
			m_buttonPaste->Print(hdc,src,pmd,&ps.rcPaint);
		}



//		HGDIOBJ old;

		int kosuu = pDoc->GetFilmKosuu();
		int page = GetMyScrollPos();

		int nm = pDoc->GetNowSelectNumber();

		int i;

		COLORREF oldTextColor;
		oldTextColor = GetTextColor(hdc);

		int colorMode = -1;

		for (i=0;i<GetBlockKosuu();i++)
		{
			int k = page + i;
			if (k<kosuu)
			{
				CFilmData* pFilm = pDoc->GetFilm(k);
				int col;
				col = pFilm->GetFilmColor();
				if (col != colorMode)
				{
					colorMode = col;
					SetTextColor(hdc,m_filmColorTable[col*2+1]);
				}

				if (pFilm == NULL)
				{
					TextOut(hdc,32,24+4+i*24,"_エラー",strlen("_エラー"));
				}
				else
				{
					pFilm->MakeMiniPic();

					m_dib->SetPic(pFilm->GetMiniPic());
					m_dib->Put(0,24+i*24,hdc,src);
					LPSTR mes = pFilm->GetMyName();
					TextOut(hdc,32,24+4+i*24,mes,strlen(mes));
				}
			}
			else if (k == kosuu)
			{
				if (colorMode != 0)
				{
					SetTextColor(hdc,0);
					colorMode = 0;
				}

				TextOut(hdc,32,24+4+i*24,"_新規",strlen("_新規"));
			}
			else
			{
				if (colorMode != 0)
				{
					SetTextColor(hdc,0);
					colorMode = 0;
				}

				TextOut(hdc,32,24+4+i*24,"-----",strlen("-----"));
			}
			
			if (k == nm)
			{
				RECT rc;
				GetWindowRect(hWnd,&rc);
//				HGDIOBJ old99 = SelectObject(hdc,GetStockObject(WHITE_BRUSH));
				PatBlt(hdc,33,24+i*24,rc.right-rc.left-32-1,24,DSTINVERT);
//				SelectObject(hdc,old99);
			}
		}

		SetTextColor(hdc,oldTextColor);

//		old = SelectObject(src,m_hBitmap1);
//		BitBlt(hdc,0,0,24,24,src,0,0,SRCCOPY);
//		BitBlt(hdc,24,0,24,24,src,0,24,SRCCOPY);
//		BitBlt(hdc,48,0,24,24,src,0,48,SRCCOPY);

//		BitBlt(hdc,96,0,24,24,src,0,72,SRCCOPY);
//		BitBlt(hdc,120,0,24,24,src,0,96,SRCCOPY);
//		BitBlt(hdc,144,0,24,24,src,0,120,SRCCOPY);
//		BitBlt(hdc,168,0,24,24,src,0,144,SRCCOPY);

//		SelectObject(src,old);








//		TextOut(hdc,32,24*1,"あいうえお",strlen("あいうえお"));
//		TextOut(hdc,32,24*2,"あいうえお",strlen("あいうえお"));
//		TextOut(hdc,32,24*3,"あいうえお",strlen("あいうえお"));
//		TextOut(hdc,32,24*4,"あいうえお",strlen("あいうえお"));
//		TextOut(hdc,32,24*5,"あいうえお",strlen("あいうえお"));

		DeleteDC(src);

		EndPaint(hWnd,&ps);

		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


void CFilmCaseView::CheckChangeColor(int n,int cmd)
{
	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document;

	for (int i=0;i<7;i++)
	{
		if (cmd == m_filmColorTable[i*2])
		{
			pDoc->OnChangeColor(n,i);//m_filmColorTable[i*2+1]);
		}
	}
}





void CFilmCaseView::MyReleaseCapture(void)
{
	ReleaseCapture();

	m_captureFlag = FALSE;
	m_captureFilm = -1;

	SetCursor(m_defaultCursor);
}

void CFilmCaseView::SetMyCursor(WPARAM wParam,LPARAM lParam)
{
	if (m_captureFlag == FALSE)
	{
		SetCursor(m_defaultCursor);
	}
	else
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		int n = GetContentsNumber(x,y);

//		int xNum = GetAreaNumberX(x,y);
//		int yNum = GetAreaNumberY(x,y);

		if (m_captureFilm != -1)
		{
			if (CheckOnFilmPic(x,y) == FALSE)
			{
				SetCursor(m_captureCursorBad);
			}
			else
			{
				if (m_captureCopyType == 0)
				{
					SetCursor(m_captureCursorExchange);
				}
				else if (m_captureCopyType == 1)
				{
					SetCursor(m_captureCursorMoveAndInsert);
				}
				else
				{
					SetCursor(m_defaultCursor);//???
				}
			}
		}
		else
		{
			SetCursor(m_defaultCursor);
		}
	}
}


BOOL CFilmCaseView::CheckOnFilmPic(int x,int y)
{
	if (x<0) return FALSE;
	if (x>=32) return FALSE;

	return TRUE;
}


BOOL CFilmCaseView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = FILMCASE_WINDOW;
	int subType = CheckOnBalloonButton(x,y);

	if (subType == -1)
	{
		type = -1;
	}


	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}


BOOL CFilmCaseView::OnControlKey(WPARAM wParam,LPARAM lParam)
{
	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document;

	if (wParam == 6)
	{
		//F
//		pDoc->OnSearch();
		//return TRUE;
	}
	if (wParam == 19)
	{
		//S
		pDoc->OnSaveFilm();
		return TRUE;
	}
	if (wParam == 3)
	{
		//C
		pDoc->OnCopy();
		return TRUE;
	}
	if (wParam == 24)
	{
		//X
		pDoc->OnCut();
		return TRUE;
	}
	if (wParam == 22)
	{
		//V
		pDoc->OnPaste();
		return TRUE;
	}
	if (wParam == 14)
	{
		//N
		pDoc->OnNewFilm();
		return TRUE;
	}
	if (wParam == 15)
	{
		pDoc->OnOpenFilm();
		return TRUE;
		//O
	}
	if (wParam == 26)
	{
		return pDoc->OnUndo(); 
	}

	return FALSE;

}

/*
void CFilmCaseView::ReCalcuScrollPara(void)
{
	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document;
	int kosuu = pDoc->GetFilmKosuu();
	int block = GetBlockKosuu();

	int pgMax = (kosuu + 1) - 1;
	int pos = GetMyScrollPos();
	if (pgMax<block-1) pgMax = block-1;
	int n = pDoc->GetNowSelectNumber();
	if (n != -1)
	{
		pos = n-(block-1);
		if (pos<0) pos = 0;
	}
	SetScrollPara(pos,pgMax);
}
*/












/*_*/




