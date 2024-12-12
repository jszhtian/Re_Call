//
// storyview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\wheelMouse.h"

#include "bitmapnumber.h"

#include "myBitmap.h"

#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"

#include "storydoc.h"
#include "storyview.h"

#include "storydata.h"

#include "mybutton.h"

#include "commanddata.h"

#include "mydib.h"

#include "commanddatatype.h"

#include "filmdata.h"

#include "..\..\systemNNN\nnnUtilLib\namelist.h"


int CStoryView::m_markColorTable[]=
{
	ID_MENU_CMDCOL_BACK_YELLOW,-1,1,0,
	ID_MENU_CMDCOL_BACK_CYAN,-1,2,0,
	ID_MENU_CMDCOL_BACK_PURPLE,-1,3,0,

	ID_MENU_CMDCOL_BLUE,1,-1,0,
	ID_MENU_CMDCOL_RED,2,-1,0,
	ID_MENU_CMDCOL_GREEN,3,-1,0,

	ID_MENU_CMDCOL_REL,0,0,0,

	ID_MENU_BLOCKCMDCOL_BACK_YELLOW,-1,1,1,
	ID_MENU_BLOCKCMDCOL_BACK_CYAN,-1,2,1,
	ID_MENU_BLOCKCMDCOL_BACK_PURPLE,-1,3,1,

	ID_MENU_BLOCKCMDCOL_BLUE,1,-1,1,
	ID_MENU_BLOCKCMDCOL_RED,2,-1,1,
	ID_MENU_BLOCKCMDCOL_GREEN,3,-1,1,

	ID_MENU_BLOCKCMDCOL_REL,0,0,1,

	-1,
};

int CStoryView::m_commandBackColorTable[]=
{
	0xffffff,	//白デフォ
	0x60ffff,	//黄色
	0xffff80,	//水色
	0xffa0ff,	//紫
};

int CStoryView::m_commandTextColorTable[]=
{
	0,	//黒デフォ
	0xc00000,	//青
	0x0000b0,	//赤
	0x00a000,	//緑
};


CStoryView::CStoryView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("storyWindow");

	m_danrakuX = 32;

	SetResizeFlag();
	SetBlockSize(480,24);
	SetNonBlockArea(0,24,0,0);

	SetBlockKosuuMin(1,2);
	SetBlockKosuuMax(1,64);

	SetFreeSizeFlag(1);
	SetScrollFlag(FALSE,TRUE);

	SetWindowStyle();

	SetViewWindowText("シナリオ");





//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_VSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_STORYCOMMAND));
	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_STORY));
	m_popupMenu[1] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_STORY_ELS));

	m_bitmap = new CMyBitmap("nnndir\\setup\\bmp\\story.bmp");
	m_hBitmap = m_bitmap->GetHBitmap();
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_STORY));


//	m_scrollFlag[0] = TRUE;
	SetScrollPara(0,1,10);
	SetScrollArea(0,24,480,24+16*10);
//	SetBlockSize(480,32);


	m_buttonNew = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NEW),0,0);

	m_buttonCut = new CMyButton(1,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),88,0);
	m_buttonCopy = new CMyButton(2,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_COPY),88+24,0);
	m_buttonPaste = new CMyButton(3,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_PASTE),88+24*2,0);

	m_buttonDelete = new CMyButton(4,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_DELETE),88+24*3+8,0);

	m_buttonUndo = new CMyButton(5,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_UNDO),88+96+8+8,0);

	m_buttonEdit = new CMyButton(6,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_EDIT),32,0);
	m_buttonChange = new CMyButton(7,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CHANGE),32+24,0);

	m_buttonBack = new CMyButton(8,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_BACK),88+96+8+8+32,0);
	m_buttonJump = new CMyButton(9,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_JUMP),88+96+8+8+32+24,0);
	m_buttonFoward = new CMyButton(10,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_FOWARD),88+96+8+8+32+24+24,0);

	AddBalloonCheckButton(m_buttonNew);
	AddBalloonCheckButton(m_buttonCut);
	AddBalloonCheckButton(m_buttonCopy);
	AddBalloonCheckButton(m_buttonPaste);
	AddBalloonCheckButton(m_buttonDelete);
	AddBalloonCheckButton(m_buttonUndo);
	AddBalloonCheckButton(m_buttonEdit);
//	AddBalloonCheckButton(m_buttonChange);
	AddBalloonCheckButton(m_buttonBack);
	AddBalloonCheckButton(m_buttonJump);
	AddBalloonCheckButton(m_buttonFoward);

	m_dib = new CMyDIB(m_hWnd,32,24);

	ReCalcuScrollPara();
}


CStoryView::~CStoryView()
{
	End();
}

void CStoryView::End(void)
{
	ENDDELETECLASS(m_buttonNew);

	ENDDELETECLASS(m_buttonChange);
	ENDDELETECLASS(m_buttonEdit);

	ENDDELETECLASS(m_buttonCut);
	ENDDELETECLASS(m_buttonCopy);
	ENDDELETECLASS(m_buttonPaste);
	ENDDELETECLASS(m_buttonDelete);

	ENDDELETECLASS(m_buttonUndo);
//	ENDDELETECLASS(m_buttonDo);

	ENDDELETECLASS(m_buttonBack);
	ENDDELETECLASS(m_buttonJump);
	ENDDELETECLASS(m_buttonFoward);

//	DELETEGDIOBJECT(m_hBitmap);
	ENDDELETECLASS(m_bitmap);

	ENDDELETECLASS(m_dib);
}


LRESULT CStoryView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	int x,y;
	CStoryDoc* pDoc = (CStoryDoc*)m_document;
	int n;

	switch (message)
	{
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

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_USER_LBUTTONDOWN:
			switch (lParam)
			{
			case 0:
				pDoc->OnNewCommand();
				break;
			case 1:
				pDoc->OnCutCommand();
				break;
			case 2:
				pDoc->OnCopyCommand();
				break;
			case 3:
				pDoc->OnPasteCommand();
				break;
			case 4:
				pDoc->OnDelete();
				break;
			case 5:
				pDoc->OnUndo();
				break;
			case 6:
				pDoc->OnEditCommand();
				break;
			case 7:
//				pDoc->OnChangeCommand
				break;
			case 8:
				pDoc->OnBackCommand();
				break;
			case 9:
				pDoc->OnJumpCommand();
				break;
			case 10:
				pDoc->OnFowardCommand();
				break;
			}
//			pDoc->OnNewButton();
			break;
		}
		break;

	case WM_RBUTTONDOWN:
		CStoryData* pStory;
		pStory = pDoc->GetNowSelectStory();
		if (pStory != NULL)
		{
			int px = LOWORD(lParam);
			int py = HIWORD(lParam);

			n = GetContentsNumber(px,py);

//			if ((py>=24) && (py<24+32*10))
			if (n != -1)
			{
//				int pos = GetMyScrollPos();
//				int k = (py-24)/32;
//				k += pos;
				int kosuu = pStory->GetObjectKosuu();
//				if ((k>=0) && (k<kosuu))
				if ((n>=0) && (n<kosuu))
				{
						int cmd;
						CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));

						int type = pCommand->GetCommandType();

						if ((type == COMMANDDATATYPE_ELSIF) || (type == COMMANDDATATYPE_ELSE))
						{
							 cmd = OpenPopupMenu(wParam,lParam,0,-1,1);
						}
						else
						{
							 cmd = OpenPopupMenu(wParam,lParam);
						}

						if (cmd == ID_MENU_EDITCOMMAND)
						{
							pDoc->OnEditCommand(n);
						}
						else if (cmd == ID_MENU_SELECTCOMMAND)
						{
							pDoc->OnSelectCommand(n);
						}
						else if (cmd == ID_MENU_CHANGESELECTSTORY)
						{
							pDoc->OnChangeSelectStory(n);
						}
						else if (cmd == ID_MENU_COMMAND_SELECT_ID)
						{
							pDoc->OnChangeSelectID(n);
						}
						else if (cmd != 0)
						{
							CheckChangeMarkColor(n,cmd);
							pDoc->OnChangeType(n,cmd);
						}
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		if (m_buttonNew != NULL) m_buttonNew->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCut != NULL) m_buttonCut->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCopy != NULL) m_buttonCopy->CalcuLButtonDown(wParam,lParam);
		if (m_buttonPaste != NULL) m_buttonPaste->CalcuLButtonDown(wParam,lParam);
		if (m_buttonDelete != NULL) m_buttonDelete->CalcuLButtonDown(wParam,lParam);
		if (m_buttonUndo != NULL) m_buttonUndo->CalcuLButtonDown(wParam,lParam);
//		if (m_buttonChange != NULL) m_buttonChange->CalcuLButtonDown(wParam,lParam);
		if (m_buttonEdit != NULL) m_buttonEdit->CalcuLButtonDown(wParam,lParam);
		if (m_buttonBack != NULL) m_buttonBack->CalcuLButtonDown(wParam,lParam);
		if (m_buttonJump != NULL) m_buttonJump->CalcuLButtonDown(wParam,lParam);
		if (m_buttonFoward != NULL) m_buttonFoward->CalcuLButtonDown(wParam,lParam);
		OnClickLButton(wParam,lParam);
		break;

	case WM_LBUTTONDBLCLK:
		int cmd2;
		cmd2 = OpenPopupMenu(wParam,lParam,0,2);
		break;

	case WM_ERASEBKGND:
		EraseSplit((HDC)wParam);
		return TRUE;
		break;

	case WM_PAINT:
		OnPaint(hWnd,wParam,lParam);
		return 0;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}
/*
void CStoryView::ReCalcuScrollPara(void)
{
	CStoryDoc* pDoc = (CStoryDoc*)m_document;
	CStoryData* pStory = pDoc->GetNowSelectStory();
	if (pStory == NULL)
	{
		SetScrollPara(0,0);
		return;
	}

	int kosuu = pStory->GetObjectKosuu();
	int block = GetBlockKosuu();


	int pgMax = (kosuu + 1) - 1;
	int pos = GetMyScrollPos();
	if (pgMax<block-1) pgMax = block-1;

	int n = pStory->GetNowSelectNumber();
	if (n != -1)
	{
		pos = n-(block-1);
		if (pos<0) pos = 0;
	}

	SetScrollPara(pos,pgMax,block);
}
*/

void CStoryView::OnClickLButton(WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	int n = GetContentsNumber(x,y);

	if (n != -1)
//	if ((y>=24) && (y<24+32*10))
	{
		CStoryDoc* pDoc = (CStoryDoc*)m_document;
		if (pDoc == NULL) return;

		CStoryData* pStoryData = pDoc->GetNowSelectStory();
		if (pStoryData == NULL) return;

		int kosuu = pStoryData->GetObjectKosuu();

//		int k = (y-24) / 32;
//		k += GetMyScrollPos();

//		if ((k>=0) && (k<=kosuu))
		if ((n>=0) && (n<=kosuu))
		{
			pDoc->OnClickCommand(n);
		}
	}
}


void CStoryView::OnPaint(HWND hWnd, WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd,&ps);

	BOOL printFlag = FALSE;

	CStoryDoc* pDoc = (CStoryDoc*)m_document;
	if (pDoc != NULL)
	{
		CStoryData* pStory = pDoc->GetNowSelectStory();
		if (pStory != NULL)
		{
			printFlag = TRUE;

			HDC hdc = ps.hdc;
			HDC src = CreateCompatibleDC(hdc);

			if (m_buttonNew != NULL) m_buttonNew->Print(hdc,src,0,&ps.rcPaint);

//			if (m_buttonChange != NULL) m_buttonChange->Print(hdc,src,0,&ps.rcPaint);
			if (m_buttonEdit != NULL) m_buttonEdit->Print(hdc,src,0,&ps.rcPaint);

			if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,2,&ps.rcPaint);
			if (m_buttonCopy != NULL) m_buttonCopy->Print(hdc,src,2,&ps.rcPaint);
			if (m_buttonPaste != NULL) m_buttonPaste->Print(hdc,src,2,&ps.rcPaint);
			if (m_buttonDelete != NULL) m_buttonDelete->Print(hdc,src,0,&ps.rcPaint);

			if (m_buttonUndo != NULL)
			{
				int umd = 2;
				if (pDoc->CheckExistUndo()) umd = 0;
				m_buttonUndo->Print(hdc,src,umd,&ps.rcPaint);

			}

			if (m_buttonBack != NULL) m_buttonBack->Print(hdc,src,2,&ps.rcPaint);
			if (m_buttonJump != NULL) m_buttonJump->Print(hdc,src,2,&ps.rcPaint);
			if (m_buttonFoward != NULL) m_buttonFoward->Print(hdc,src,2,&ps.rcPaint);

			HGDIOBJ old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));

			int pos = GetMyScrollPos();
			int kosuu = pStory->GetObjectKosuu();
			int nowSelect = pStory->GetNowSelectNumber();

			int oldBkMode = GetBkMode(hdc);
			COLORREF oldBkColor = GetBkColor(hdc);
			COLORREF oldTextColor = GetTextColor(hdc);

			SetBkMode(hdc,OPAQUE);

			int oldBackColorNumber = 0;
			int oldTextColorNumber = 0;

			for (int i=0;i<GetBlockKosuu();i++)
			{
				int k= pos + i;

				int x = 8;
				int y = 24 + 24 * i;

				if ((k>=0) && (k<kosuu))
				{
					CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(k));
					if (pCommand != NULL)
					{
						int typ = pCommand->GetCommandType();
						int level = pCommand->GetLevel();
						x += level * m_danrakuX;



						LPSTR mes = pCommand->GetTextBuffer();
						if (mes != NULL)
						{

							int newTextColorNumber = pCommand->GetColor(); 
							int newBackColorNumber = pCommand->GetBackColor();

							if ((newTextColorNumber != oldTextColorNumber) || (newBackColorNumber != oldBackColorNumber))
							{
								oldTextColorNumber = newTextColorNumber;
								oldBackColorNumber = newBackColorNumber;

								SetBkColor(hdc,(COLORREF)(m_commandBackColorTable[newBackColorNumber]));
								SetTextColor(hdc,(COLORREF)(m_commandTextColorTable[newTextColorNumber]));
							}

							if (typ == COMMANDDATATYPE_SYSTEM)
							{
								int ctype = pCommand->GetSystemCommandType();
								int cnm = pCommand->GetSystemCommandNumber();
								char mes9[1024];


								
								if (ctype == 0)
								{
									CNameList* plist = pDoc->GetFunctionList();
									wsprintf(mes9,"%s(%s);//%s",plist->GetName(cnm*2),mes,plist->GetName(cnm*2+1));
								}
								else
								{
									CNameList* plist = pDoc->GetCommandList();
									wsprintf(mes9,"%s(%s);//%s",plist->GetName(cnm*2),mes,plist->GetName(cnm*2+1));
								}

								TextOut(hdc,x+33,y,mes9,strlen(mes9));
							}
							else if(typ == COMMANDDATATYPE_BGM)
							{
								int para = pCommand->GetPara();
								char mes10[1024];

								if ((para == 0) || (para == 1))
								{
									int bnm = pCommand->GetBGMNumber();
									CNameList* plist = pDoc->GetBGMList();
									if  (para == 0)
									{
										wsprintf(mes10,"MIDI(%s);//%s",plist->GetName(bnm*2+1),plist->GetName(bnm*2));
									}
									else
									{
										wsprintf(mes10,"MIDI ONCE(%s);//%s",plist->GetName(bnm*2+1),plist->GetName(bnm*2));
									}
								}
								else
								{
									wsprintf(mes10,"STOP");
								}
								TextOut(hdc,x+33,y,mes10,strlen(mes10));
							}
							else
							{

								int deltaX = 0;
								if ((typ == COMMANDDATATYPE_ELSIF) || (typ == COMMANDDATATYPE_ELSE))
								{
								///	deltaX = m_danrakuX / 2;
								}

								if (typ == COMMANDDATATYPE_SELECT)
								{
									char selmes[2048];
									int selid = pCommand->GetSelectMessageSerial();

									sprintf_s(selmes, 2048, "[ID:%d]%s", selid,mes);
									TextOut(hdc, x + deltaX + 33, y, selmes, strlen(selmes));
								}
								else
								{
									TextOut(hdc, x + deltaX + 33, y, mes, strlen(mes));
								}
							}
						}

						BOOL filmFlag = FALSE;

						if (typ == COMMANDDATATYPE_FILM)
						{
							int para = pCommand->GetPara();	//last found film number (tmp)
							int f = pDoc->SearchFilm(mes,para);
							if (f != -1)
							{
								CFilmData* pFilm = pDoc->GetFilmData(f);
								if (pFilm != NULL)
								{

									pFilm->MakeMiniPic();

									m_dib->SetPic(pFilm->GetMiniPic());
									m_dib->Put(x,y,hdc,src);

									filmFlag = TRUE;
								}
							}

						}

						if (filmFlag == FALSE)
						{
							old = SelectObject(src,m_hBitmap);
							int deltaX = 0;
							if ((typ == COMMANDDATATYPE_ELSIF) || (typ == COMMANDDATATYPE_ELSE))
							{
//								deltaX = m_danrakuX / 2;
							}

							BitBlt(hdc,x+deltaX,y,32,24,src,0,typ*24,SRCCOPY);
							SelectObject(src,old);
						}
					}
					else
					{
						//error??
						TextOut(hdc,x+33,y,"NO DATA ERROR!!",strlen("NO DATA ERROR!!"));
					}
				}
				else if (k == kosuu)
				{
					SetBkColor(hdc,(COLORREF)0xffffff);
					SetTextColor(hdc,(COLORREF)0);
					TextOut(hdc,x+33,y,"新規",strlen("新規"));
				}
				else if (k>kosuu)
				{
					//aki
					SetBkColor(hdc,(COLORREF)0xffffff);
					SetTextColor(hdc,(COLORREF)0);
					TextOut(hdc,x+33,y,"--------",strlen("--------"));
				}
				else
				{
					//error
					TextOut(hdc,x+33,y,"ERROR",strlen("ERROR"));
				}


				//select 
				if (k == nowSelect)
				{
					RECT rc;
					GetWindowRect(hWnd,&rc);
					PatBlt(hdc,40,y,(rc.right - rc.left)-40,24,DSTINVERT);
				}
			}

			SetBkMode(hdc,oldBkMode);
			SetBkColor(hdc,oldBkColor);
			SetTextColor(hdc,oldTextColor);

			DeleteDC(src);

		}
	}

	if (printFlag == FALSE)
	{
		RECT rcClient;
		GetClientRect(hWnd,&rcClient);

		HGDIOBJ old = SelectObject(ps.hdc,GetStockObject(GRAY_BRUSH));
		PatBlt(ps.hdc,0,24,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 24,PATCOPY);
			
		SelectObject(ps.hdc,old);
	}


	EndPaint(hWnd,&ps);
}


void CStoryView::CheckChangeMarkColor(int n,int cmd)
{
	CStoryDoc* pDoc = (CStoryDoc*)m_document;

	for (int i=0;i<100;i++)
	{
		int check = m_markColorTable[i*4];
		if (check == -1) return;

		if (cmd == check)
		{
			pDoc->OnChangeMarkColor(n,m_markColorTable[i*4+1],m_markColorTable[i*4+2],m_markColorTable[i*4+3]);
			return;
		}
	}
}

BOOL CStoryView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = STORY_WINDOW;
	int subType = CheckOnBalloonButton(x,y);

	if (subType == -1)
	{
		type = -1;
	}


	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}

BOOL CStoryView::OnControlKey(WPARAM wParam,LPARAM lParam)
{
	CStoryDoc* pDoc = (CStoryDoc*)m_document;
	if (wParam == 6)
	{
		//F
//		pDoc->OnSearch();
//		return TRUE;
	}
	if (wParam == 19)
	{
		//S
//		pDoc->OnSaveStory();
//		return TRUE;
	}
	if (wParam == 3)
	{
		//C
//		pDoc->OnCopyS();
//		return TRUE;
	}
	if (wParam == 24)
	{
		//X
//		pDoc->OnCut();
//		return TRUE;
	}
	if (wParam == 22)
	{
		//V
//		pDoc->OnPaste();
//		return TRUE;
	}
	if (wParam == 14)
	{
		//N
		pDoc->OnNewCommand();
		return TRUE;
	}
	if (wParam == 15)
	{
//		pDoc->OnOpenStory();
//		return TRUE;
		//O
	}
	if (wParam == 26)
	{
		return pDoc->OnUndo(); 
	}

	return FALSE;

}

/*_*/



