//
// viewview.cpp
//

#include <windows.h>
#include <stdio.h>


#include "resource.h"

#include "bitmapnumber.h"
#include "mybutton.h"


#include "..\..\systemNNN\nnnUtilLib\wheelMouse.h"

#include "myBitmap.h"
#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"


#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"


#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "mydirectx.h"

//#include "varInitData.h"

//#include "myPopupMenu.h"

#include "vardoc.h"
#include "varview.h"


int CVarView::m_markColorTable[]=
{
	ID_MENU_BACK_YELLOW,-1,1,
	ID_MENU_BACK_CYAN,-1,2,
	ID_MENU_BACK_PURPLE,-1,3,

	ID_MENU_MOJI_BLUE,1,-1,
	ID_MENU_MOJI_RED,2,-1,
	ID_MENU_MOJI_GREEN,3,-1,

	-1,
};

int CVarView::m_textColorTable[]=
{
	0,	//黒デフォ
	0xc00000,	//青
	0x0000b0,	//赤
	0x00a000,	//緑

};

int CVarView::m_backColorTable[]=
{
	0xffffff,	//白デフォ
	0x60ffff,	//黄色
	0xffff80,	//水色
	0xffa0ff,	//紫
};




CVarView::CVarView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("varWindow");

//	m_varInitData = ((CVarDoc*)m_document)->GetVarInitData();

	m_modeButtonPrintX = 24*4 + 24+16;
	SetResizeFlag();
	SetBlockSize(512,24);
	SetNonBlockArea(0,24,0,0);

	SetBlockKosuuMin(1,2);
	SetBlockKosuuMax(1,64);
	SetFreeSizeFlag(1);
	SetScrollFlag(FALSE,TRUE);

	SetWindowStyle();

	SetViewWindowText("変数");


	HICON hicon = LoadIcon(hinstance,MAKEINTRESOURCE(IDI_ICON_VARWINDOW));

	SendMessage(m_hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hicon);

//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_VSCROLL;

	m_bitmap = new CMyBitmap("nnndir\\setup\\bmp\\var.bmp");
	m_hBitmap = m_bitmap->GetHBitmap();
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_VAR));



	m_varBlockKosuu = ((CVarDoc*)m_document)->GetVarBlockKosuu();


//	int startX = 152+(m_varBlockKosuu-1)*24;
	int startX = 0;

	m_buttonSave = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SAVE),startX,0);
	m_buttonCut = new CMyButton(1,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),startX+24,0);
	m_buttonCopy = new CMyButton(2,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_COPY),startX+2*24,0);
	m_buttonPaste = new CMyButton(3,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_PASTE),startX+24*3,0);

//	m_searchBitmap = new CMyBitmap("nnndir\\setup\\bmp\\cutin.bmp");
//	m_searchBitmap = new CMyBitmap("nnndir\\setup\\bmp\\cutin.bmp");
	m_buttonSearch = new CMyButton(4,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SEARCH),startX+24*4+8,0);

//	SetWindowLong(m_hWnd,GWL_STYLE,style);

	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_VAR));

	AddBalloonCheckButton(m_buttonSave);
	AddBalloonCheckButton(m_buttonCut);
	AddBalloonCheckButton(m_buttonCopy);
	AddBalloonCheckButton(m_buttonPaste);
	AddBalloonCheckButton(m_buttonSearch);



	m_scrollFlag[0] = TRUE;
	SetScrollPara(0,4,5);
	SetScrollArea(0,24,512,24+24*5);
	SetBlockSize(512,24);

	ReCalcuScrollPara();
}

CVarView::~CVarView()
{
	End();
}

void CVarView::End(void)
{
	ENDDELETECLASS(m_buttonSearch);
//	ENDDELETECLASS(m_searchBitmap);

	ENDDELETECLASS(m_buttonPaste);
	ENDDELETECLASS(m_buttonCopy);
	ENDDELETECLASS(m_buttonCut);
	ENDDELETECLASS(m_buttonSave);
//	DELETEGDIOBJECT(m_hBitmap);
	ENDDELETECLASS(m_bitmap);
}



LRESULT CVarView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int x,y;
	int n;

	CVarDoc* pDoc = (CVarDoc*)m_document;

	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_USER_LBUTTONDOWN:
			switch (lParam)
			{
			case 0:
				pDoc->OnSaveButton();
				break;
			case 1:
				pDoc->OnCutButton();
				break;
			case 2:
				pDoc->OnCopyButton();
				break;
			case 3:
				pDoc->OnPasteButton();
				break;
			case 4:
				pDoc->OnSearchButton(FALSE);
				break;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		OnWheelScroll(message,wParam,lParam);
		return TRUE;
		break;
	case WM_VSCROLL:
		OnScroll(message,wParam,lParam);
		break;
	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window


		return 0;
		break;
	case WM_LBUTTONDOWN:
		if (m_buttonSave != NULL) m_buttonSave->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCut != NULL) m_buttonCut->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCopy != NULL) m_buttonCopy->CalcuLButtonDown(wParam,lParam);
		if (m_buttonPaste != NULL) m_buttonPaste->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSearch != NULL) m_buttonSearch->CalcuLButtonDown(wParam,lParam);

		x = LOWORD(lParam);
		y = HIWORD(lParam);
		n = GetContentsNumber(x,y);

		if ((y>=0) && (y<24))
		{
			if ((x>=m_modeButtonPrintX) && (x<m_modeButtonPrintX+32*(3+m_varBlockKosuu)))
			{
				pDoc->OnClickModeButton((x-m_modeButtonPrintX)/32);
			}
		}

//		if ((y>=24) && (y<24*6))
		if (n != -1)
		{
//			pDoc->OnClickVar(((y-24) / 24) + GetMyScrollPos());
			pDoc->OnClickVar(n);
		}
		break;

	case WM_RBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		n = GetContentsNumber(x,y);

//		if ((y>=24) && (y<24+24*5))
		if (x != -1)
		{
//			int ny = (y-24) / 24;
//			ny += GetMyScrollPos();

			int vm = pDoc->GetVarMode();

			if ((vm >= 1) && (vm <= 3+m_varBlockKosuu))
			{
				int cmd = OpenPopupMenu(wParam,lParam);


				if (cmd == ID_MENU_VAR_CHANGE)
				{
					pDoc->OnChangeVarName(n);
				}
				else if (cmd == ID_MENU_DELETEMARK)
				{
					pDoc->OnDeleteMark(n);
				}
				else if (cmd == ID_MENU_VAR_INITDATA)
				{
					pDoc->OnChangeInitData(n);
				}
				else
				{
					CheckChangeMarkColor(n,cmd);
				}

//@@@				if (cmd == ID_MENU_VAR_CHANGE) pDoc->OnChangeVarName(n);
//				if (cmd == ID_MENU_VAR_DELETE) pDoc->OnDeleteVarName(ny);

//				pDoc->OnClickModeButton(x/32);
			}
			//
		}
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
		
//		HGDIOBJ old;

//		UuidCreate(&uuid);

	//dummy
		if (m_hBitmap != NULL)
		{
			HGDIOBJ old66;
			old66 = SelectObject(src,m_hBitmap);
			int md = pDoc->GetVarMode();
			int i;
			for (i=0;i<3+m_varBlockKosuu;i++)
			{
				int srcX = 0;
				if (i == md) srcX += 32;
				int srcY = i*24;
				int dstX = m_modeButtonPrintX + 32*i;
				if (i>=4) srcY=3*24;
				int dstY = 0;
				BitBlt(hdc,dstX,dstY,32,24,src,srcX,srcY,SRCCOPY);
			}
			SelectObject(src,old66);
		}

		if (m_buttonSave != NULL) m_buttonSave->Print(hdc,src,0,&ps.rcPaint);

		int f = 0;
		if (pDoc->GetVarMode() == 0) f = 2;
		if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,f,&ps.rcPaint);
		if (m_buttonCopy != NULL) m_buttonCopy->Print(hdc,src,0,&ps.rcPaint);
		f = 0;
		if (pDoc->GetScrap() == NULL) f = 2;
		if (pDoc->GetVarMode() == 0) f = 2;
		if (m_buttonPaste != NULL) m_buttonPaste->Print(hdc,src,f,&ps.rcPaint);

		if (m_buttonSearch != NULL) m_buttonSearch->Print(hdc,src,0,&ps.rcPaint);

		int i;
		int md = pDoc->GetVarMode();

//		int varOffset = 0;
//		if (md == 1) varOffset = 100;
//		if (md == 2) varOffset = 200;
//		if (md == 3) varOffset = 300;


		int p = GetMyScrollPos();
//		p += varOffset;

		int kosuu;
		kosuu = GetBlockKosuu();

		int markColor = -1;
		COLORREF oldTextColor;
		COLORREF oldBackColor;
		int colorMode;

		oldTextColor = GetTextColor(hdc);
		oldBackColor = GetBkColor(hdc);
		colorMode = GetBkMode(hdc);

		SetBkMode(hdc,OPAQUE);

//char mes2[256];
//sprintf(mes2,"%d %d\n",kosuu,ps.rcPaint.bottom);
//OutputDebugString(mes2);

		int vmd = pDoc->GetVarMode();

		for (i=0;i<kosuu;i++)
		{
			char tmp[1024];
			LPSTR name = pDoc->GetVarName(p+i);
			int sel = pDoc->GetNowSelectNumber0(md);

			if (name != NULL)
			{
				int col;
				col = pDoc->GetMarkColor(p+i);
				if (col != markColor)
				{
					markColor = col;

					int textColorNumber;
					int backColorNumber;

					textColorNumber = markColor & 0xff;
					backColorNumber = (markColor >> 8) & 0xff;

					COLORREF textColor;
					textColor = m_textColorTable[textColorNumber];

					if (textColorNumber == 0)
					{
						//change?
					}

					COLORREF backColor;
					backColor = m_backColorTable[backColorNumber];

					SetTextColor(hdc,textColor);
					SetBkColor(hdc,backColor);
				}


				wsprintf(tmp,"%s[%s]",name,pDoc->GetVarInitDataString(p+i));


				TextOut(hdc,0,24+24*i,tmp,strlen(tmp));
				if (sel == p+i)
				{
					RECT rc;
					GetWindowRect(hWnd,&rc);

					PatBlt(hdc,0,24+24*i,rc.right - rc.left,24,DSTINVERT);
				}
			}
			else
			{
				//
			}
		}

		SetBkMode(hdc,colorMode);
		SetTextColor(hdc,oldTextColor);
		SetBkColor(hdc,oldBackColor);

//		TextOut(hdc,0,24,"ミント好意度",strlen("ミント好意度"));
//		TextOut(hdc,0,48,"１３４",strlen("１３４"));

		DeleteDC(src);

		EndPaint(hWnd,&ps);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}

void CVarView::CheckChangeMarkColor(int n,int cmd)
{
	CVarDoc* pDoc = (CVarDoc*)m_document;

	for (int i=0;i<100;i++)
	{
		int check = m_markColorTable[i*3];
		if (check == -1) return;

		if (cmd == check)
		{
			pDoc->OnChangeMarkColor(n,m_markColorTable[i*3+1],m_markColorTable[i*3+2]);
			return;
		}
	}
}

void CVarView::VarBlockIsChanged(void)
{
	if (m_document == NULL) return;

	m_varBlockKosuu = ((CVarDoc*)m_document)->GetVarBlockKosuu();
	ReCalcuScrollPara();
}

/*
void CVarView::ReCalcuScrollPara(void)
{
	CVarDoc* pDoc = (CVarDoc*)m_document;
	int md = pDoc->GetVarMode();
	int kosuu = pDoc->GetVarKosuu(md);
	int block = GetBlockKosuu();

	int pgMax = (kosuu + 1) - 1 - 1;	//新規がないため
	int pos = GetMyScrollPos();
	if (pgMax<block-1) pgMax = block-1;

	int n = pDoc->GetNowSelectNumber(md);

	if (n != -1)
	{
		pos = n-(block-1);
		if (pos<0) pos = 0;
	}
	SetScrollPara(pos,pgMax);
}
*/

void CVarView::OnLeftKey(void)
{
	CVarDoc* pDoc = (CVarDoc*)m_document;
	pDoc->OnLeftKey();
}

void CVarView::OnRightKey(void)
{
	CVarDoc* pDoc = (CVarDoc*)m_document;
	pDoc->OnRightKey();
}


BOOL CVarView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = VAR_WINDOW;
	int subType = CheckOnBalloonButton(x,y);


	if (subType == -1)
	{
		if ((y>=0) && (y<24))
		{
			if ((x>=m_modeButtonPrintX) && (x<m_modeButtonPrintX+32*(3+m_varBlockKosuu)))
			{
				int n = (x - m_modeButtonPrintX) / 32;
				if (n<0) n = 0;
				if (n>9) n = 9;

				subType = 10 + n;
			}
		}
	}

	if (subType == -1)
	{
		type = -1;
	}


	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}

BOOL CVarView::OnControlKey(WPARAM wParam,LPARAM lParam)
{

	CVarDoc* pDoc = (CVarDoc*)m_document;

	if (wParam == 6)
	{
		pDoc->OnSearchButton(FALSE);
		return TRUE;
	}
	if (wParam == 19)
	{
		pDoc->OnSaveButton();
		return TRUE;
	}
	if (wParam == 3)
	{
		pDoc->OnCopyButton();
		return TRUE;
	}
	if (wParam == 24)
	{
		pDoc->OnCutButton();
		return TRUE;
	}
	if (wParam == 22)
	{
		pDoc->OnPasteButton();
		return TRUE;
	}
	if (wParam == 14)
	{
		//N
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



