//
// programview.cpp
//

#include <windows.h>
#include <stdio.h>


#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "mydirectx.h"

#include "windowlist.h"
#include "case.h"
#include "messageData.h"
#include "myapplicationBase.h"

#include "programdoc.h"
#include "programview.h"

#include "commanddatatype.h"

#include "storydata.h"
#include "commanddata.h"

#include "bitmapnumber.h"
#include "mybutton.h"


CProgramView::CProgramView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("programWindow");

	SetResizeFlag();
	SetBlockSize(512,512);
	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
	SetNonBlockArea(0,24,0,0);
	SetFreeSizeFlag(0);
	SetFreeSizeFlag(1);
//	SetScrollFlag(FALSE,FALSE);

	SetWindowStyle();

	SetViewWindowText("Program");


//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
///	style |= WS_VSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_buttonEdit = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_EDIT),0,0);
	ReCalcuScrollPara();
}

CProgramView::~CProgramView()
{
	End();
}

void CProgramView::End(void)
{
//	ENDDELETECLASS(m_buttonEdit);
}


LRESULT CProgramView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	int x,y;
	CProgramDoc* pDoc = (CProgramDoc*)m_document;

	switch (message)
	{
	case WM_CLOSE:
		Show(FALSE);
//		m_document->OnClose();
		//hide window

		return 0;
		break;
//	case WM_RBUTTONDOWN:
//		pDoc->OnEditText(-1);
//		break;
	case WM_LBUTTONDOWN:
//		if (m_buttonEdit != NULL) m_buttonEdit->CalcuLButtonDown(wParam,lParam);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_USER_LBUTTONDOWN:
			switch (lParam)
			{
			case 0:
				pDoc->OnEditText(-1);
				break;
			}
			break;
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

		//ps.rcPaint‚Æ‚­‚ç‚×‚Ä‚©‚«‚È‚¨‚· IntersectRect IntersectClipRect


		HDC hdc = ps.hdc;
		HDC src = CreateCompatibleDC(hdc);
		
//		if (m_buttonEdit != NULL) m_buttonEdit->Print(hdc,src,0,&ps.rcPaint);

//		HGDIOBJ old;

//		static char m1[] = "ƒAƒ‹ƒƒŠƒADˆÓ“x+=1;";
//		static char m2[] = "ŠG—¢‰ÔGŠpŽæ‚ê‚é=0;";
//		static char m3[] = "‚Í‚ç‚Û‚ñ‚±‚í‚ê‚½=1;";

//		TextOut(hdc,0,0,m1,strlen(m1));
//		TextOut(hdc,0,24,m2,strlen(m2));
//		TextOut(hdc,0,48,m3,strlen(m3));

		RECT rc;
		SetRect(&rc,0,24,320,24*15);

		CStoryData* pStory = m_document->GetNowSelectStory();
		if (pStory != NULL)
		{
			CCommandData* pCommand = (CCommandData*)(pStory->GetNowSelectObject());
			if (pCommand != NULL)
			{
				int typ = pCommand->GetCommandType();

				LPSTR mes = pCommand->GetTextBuffer();

				//dummy
				switch (typ)
				{
				case COMMANDDATATYPE_NOP:
					DrawText(hdc,"ynopz",-1,&rc,0);
					break;
				case COMMANDDATATYPE_IF:
				case COMMANDDATATYPE_ELSIF:
				case COMMANDDATATYPE_SELECT:
				case COMMANDDATATYPE_STORY:
				case COMMANDDATATYPE_FILM:
				case COMMANDDATATYPE_CALCU:
				case COMMANDDATATYPE_SCRIPT:
					if (mes != NULL)
					{
						DrawText(hdc,mes,-1,&rc,DT_NOPREFIX);
					}
					else
					{
						DrawText(hdc,"ERROR!",-1,&rc,0);
					}
					break;
				case COMMANDDATATYPE_ELSE:
					DrawText(hdc,"yelsez",-1,&rc,0);
					break;
				case COMMANDDATATYPE_BGM:
					DrawText(hdc,"BGM",-1,&rc,0);
					break;
				case COMMANDDATATYPE_BGM_CD:
					DrawText(hdc,"BGM",-1,&rc,0);
					break;
				case COMMANDDATATYPE_SYSTEM:
					DrawText(hdc,"SYSTEM",-1,&rc,0);
					break;
				case COMMANDDATATYPE_RET:
					DrawText(hdc,"yretz",-1,&rc,0);
					break;
				case COMMANDDATATYPE_NEXT:
					DrawText(hdc,"ynextz",-1,&rc,0);
					break;
				case COMMANDDATATYPE_END:
					DrawText(hdc,"yendz",-1,&rc,0);
					break;
				}




//			DrawText(hdc,"ƒAƒ‹ƒƒŠƒADˆÓ“x+=1;\nŠG—¢‰ÔGŠpŽæ‚ê‚é=0;‚»‚µ‚Ä‚±‚ê‚Í‚È‚ª[[[[[[[[[[[[‚¬‚å‚¤\n‚Í‚ç‚Û‚ñ‚±‚í‚ê‚½=1;",-1,&rc,0);


			}
		}

		DeleteDC(src);

		EndPaint(hWnd,&ps);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


BOOL CProgramView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 



	int subType = -1;
	int type = PROGRAM_WINDOW;


	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

//	OutputDebugString("*moveMouse layer*\x00f\x00a");
	return FALSE;
}

/*_*/



