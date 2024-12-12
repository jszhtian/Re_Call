//
// gamemessageview.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "mydirectx.h"

#include "..\..\systemNNN\nnnUtilLib\wheelMouse.h"
#include "case.h"

#include "messageData.h"
#include "myapplicationBase.h"
#include "windowList.h"

//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "myBitmap.h"

#include "gamemessagedoc.h"
#include "gamemessageview.h"

#include "bitmapnumber.h"

#include "mybutton.h"

#include "komadata.h"

#include "messagedata.h"

int CGameMessageView::m_messageColorTable[]=
{
	ID_MENU_MESSAGECOLOR_BLACK,0,
	ID_MENU_MESSAGECOLOR_BLUE,0xC00000,
	ID_MENU_MESSAGECOLOR_RED,0x0000B0,
	ID_MENU_MESSAGECOLOR_PURPLE,0xC000B0,
	ID_MENU_MESSAGECOLOR_GREEN,0x00A000,
	ID_MENU_MESSAGECOLOR_CYAN,0xC0A000,
	ID_MENU_MESSAGECOLOR_YELLOW,0x00A0B0,
};


CGameMessageView::CGameMessageView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyScrollView(pDocument, clientHWND, hinstance)
{
	SetMyViewWindowName("gameMessageWindow");

	m_editHWND[0] = NULL;
	m_editHWND[1] = NULL;

	SetEditMode(FALSE);

	SetResizeFlag();
	m_editorMessageGyo = ((CGameMessageDoc*)m_document)->GetEditorMessageGyo();
	m_widthGuideLine = ((CGameMessageDoc*)m_document)->GetMessageWidthGuideLine();
	SetBlockSize(640,24+18*m_editorMessageGyo);
	SetNonBlockArea(0,24,0,0);

	SetBlockKosuuMin(1,2);
	SetBlockKosuuMax(1,20);
	SetFreeSizeFlag(1);
	SetScrollFlag(FALSE,TRUE);

	SetWindowStyle();

	SetViewWindowText("コマ");




//	LONG style = GetWindowLong(m_hWnd,GWL_STYLE);
//	style &= ~(WS_MINIMIZEBOX | WS_SIZEBOX);
//	style |= WS_HSCROLL;
//	style |= WS_VSCROLL;
//	SetWindowLong(m_hWnd,GWL_STYLE,style);

//	m_scrollFlag[0] = TRUE;
	SetScrollPara(0,1,2);
	SetScrollArea(0,24,640,24+24+18*m_editorMessageGyo*2);
//	SetBlockSize(640,24*4);

	m_modeButtonPrintX = 0;
	m_modeButtonPrintY = 0;
	m_voiceButtonPrintX = 24+12*4;
	m_voiceButtonPrintY = 0;
	m_seButtonPrintX = 512-128;
	m_seButtonPrintY = 0;


	m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_MESSAGE));
	m_popupMenu[1] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_MESSAGE_VOICE));
	m_popupMenu[2] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_MESSAGE_SE));
	m_popupMenu[3] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_MESSAGE_EDIT));
	m_popupMenu[4] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_MESSAGE_VOICE2));

//EM_GETMODIFY



/*
	m_editHWND[0] = CreateWindow("EDIT",//エジットコントロール
	   "ここに入力\x00d\x00aつぎのぎょう",//最初に表示される文字列
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN,//ウィンドウスタイル
		32,32,//表示位置
		300,80,//幅、高さ
		m_hWnd,//親ウィンドウのハンドル
		(HMENU)IDC_EDIT_DUMMY,//エジットコントロールのＩＤ
		m_hInstance,//インスタンス・ハンドル
		NULL);
*/

	m_buttonNew = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_NEW),0,0);

	m_buttonCut = new CMyButton(1,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),32,0);
	m_buttonCopy = new CMyButton(2,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_COPY),56,0);
	m_buttonPaste = new CMyButton(3,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_PASTE),80,0);

	m_buttonDelete = new CMyButton(4,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_DELETE),112,0);

	m_buttonUndo = new CMyButton(5,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_UNDO),144,0);

//	m_buttonWindow = new CMyButton(6,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_WINDOWONOFF),244,0);
	m_buttonWindow = NULL;

	m_buttonCommentTop = new CMyButton(7,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_COMMENT_TOP),276,0,32,24);
	m_buttonCommentBottom = new CMyButton(8,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_COMMENT_BOTTOM),308,0,32,24);

	m_addButtonBitmap = new CMyBitmap("nnndir\\setup\\bmp\\seButtonAdd.bmp");
	m_addButtonHBitmap = m_addButtonBitmap->GetHBitmap();

	m_expStatusBitmap = new CMyBitmap("nnndir\\setup\\bmp\\expStatus.bmp");
	m_buttonExpStatus = new CMyButton(9,m_hWnd,m_expStatusBitmap->GetHBitmap(),360,0,24,24);

	m_messageFontSizeTypeBitmap = new CMyBitmap("nnndir\\setup\\bmp\\messageFontSize.bmp");//
	m_buttonMessageFontSizeType = new CMyButton(10,m_hWnd,m_messageFontSizeTypeBitmap->GetHBitmap(),192,0,24,24);

	m_faceBitmap = new CMyBitmap("nnndir\\setup\\bmp\\window_face_number.bmp");
	m_buttonFace = new CMyButton(11,m_hWnd,m_faceBitmap->GetHBitmap(),392,0,24,24);

	m_mustFaceBitmap = new CMyBitmap("nnndir\\setup\\bmp\\window_face_must.bmp");
	m_buttonMustFace = new CMyButton(12,m_hWnd,m_mustFaceBitmap->GetHBitmap(),416,0,24,24);

	m_fixFaceBitmap = new CMyBitmap("nnndir\\setup\\bmp\\window_face_kotei.bmp");
	m_buttonFixFace = new CMyButton(14,m_hWnd,m_fixFaceBitmap->GetHBitmap(),440,0,24,24);



	m_musicFadeBitmap = new CMyBitmap("nnndir\\setup\\bmp\\common_bgm.bmp");
	m_buttonMusicFade = new CMyButton(13,m_hWnd,m_musicFadeBitmap->GetHBitmap(),472,0,24,24);

	m_messageEffectBitmap = new CMyBitmap("nnndir\\setup\\bmp\\messageEffect.bmp");
	m_buttonMessageEffect = new CMyButton(15,m_hWnd,m_messageEffectBitmap->GetHBitmap(),504,0,24,24);
	m_messageEffectTimeBitmap = new CMyBitmap("nnndir\\setup\\bmp\\messageEffectTime.bmp");
	m_buttonMessageEffectTime = new CMyButton(16,m_hWnd,m_messageEffectTimeBitmap->GetHBitmap(),528,0,24,24);




	AddBalloonCheckButton(m_buttonNew);
	AddBalloonCheckButton(m_buttonCut);
	AddBalloonCheckButton(m_buttonCopy);
	AddBalloonCheckButton(m_buttonPaste);
	AddBalloonCheckButton(m_buttonDelete);
	AddBalloonCheckButton(m_buttonUndo);
	AddBalloonCheckButton(m_buttonCommentTop);
	AddBalloonCheckButton(m_buttonCommentBottom);
	AddBalloonCheckButton(m_buttonExpStatus);
	AddBalloonCheckButton(m_buttonMessageFontSizeType);
	AddBalloonCheckButton(m_buttonFace);
	AddBalloonCheckButton(m_buttonMustFace);
	AddBalloonCheckButton(m_buttonMusicFade);
	AddBalloonCheckButton(m_buttonFixFace);
	AddBalloonCheckButton(m_buttonMessageEffect);
	AddBalloonCheckButton(m_buttonMessageEffectTime);



	ReCalcuScrollPara();
}


CGameMessageView::~CGameMessageView()
{
	End();
}


void CGameMessageView::End(void)
{
	ENDDELETECLASS(m_buttonMusicFade);
	ENDDELETECLASS(m_musicFadeBitmap);
	ENDDELETECLASS(m_buttonFixFace);
	ENDDELETECLASS(m_fixFaceBitmap);
	ENDDELETECLASS(m_buttonMustFace);
	ENDDELETECLASS(m_mustFaceBitmap);
	ENDDELETECLASS(m_buttonFace);
	ENDDELETECLASS(m_faceBitmap);
	ENDDELETECLASS(m_buttonMessageFontSizeType);
	ENDDELETECLASS(m_messageFontSizeTypeBitmap);

	ENDDELETECLASS(m_buttonMessageEffect);
	ENDDELETECLASS(m_messageEffectBitmap);
	ENDDELETECLASS(m_buttonMessageEffectTime);
	ENDDELETECLASS(m_messageEffectTimeBitmap);

	ENDDELETECLASS(m_buttonExpStatus);
	ENDDELETECLASS(m_expStatusBitmap);
	ENDDELETECLASS(m_addButtonBitmap);
	ENDDELETECLASS(m_buttonCommentBottom);
	ENDDELETECLASS(m_buttonCommentTop);
	ENDDELETECLASS(m_buttonNew);
	ENDDELETECLASS(m_buttonCut);
	ENDDELETECLASS(m_buttonCopy);
	ENDDELETECLASS(m_buttonPaste);
	ENDDELETECLASS(m_buttonDelete);
	ENDDELETECLASS(m_buttonUndo);
	ENDDELETECLASS(m_buttonWindow);
}


LRESULT CGameMessageView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	int x,y;
	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document;
//	HGDIOBJ old;
	int n;

	switch (message)
	{
	case WM_MOUSEWHEEL:
		SetEditMode(FALSE);
		OnWheelScroll(message,wParam,lParam);
		return TRUE;
		break;

	case WM_VSCROLL:
		SetEditMode(FALSE);
		OnScroll(message,wParam,lParam);
		break;

//	case WM_NOTIFY:
//		NMHDR* nm;
//
//		nm = (NMHDR*)lParam;

//		break;

		
		
		//EM_GETLINE
		//EM_GETLINECOUNT

	case WM_LBUTTONDOWN:
		if (m_buttonNew != NULL) m_buttonNew->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCut != NULL) m_buttonCut->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCopy != NULL) m_buttonCopy->CalcuLButtonDown(wParam,lParam);
		if (m_buttonPaste != NULL) m_buttonPaste->CalcuLButtonDown(wParam,lParam);
		if (m_buttonDelete != NULL) m_buttonDelete->CalcuLButtonDown(wParam,lParam);
		if (m_buttonWindow != NULL) m_buttonWindow->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCommentTop != NULL) m_buttonCommentTop->CalcuLButtonDown(wParam,lParam);
		if (m_buttonExpStatus != NULL) m_buttonExpStatus->CalcuLButtonDown(wParam,lParam);
		if (m_buttonMessageFontSizeType != NULL) m_buttonMessageFontSizeType->CalcuLButtonDown(wParam,lParam);
		if (m_buttonFace != NULL) m_buttonFace->CalcuLButtonDown(wParam,lParam);
		if (m_buttonMustFace != NULL) m_buttonMustFace->CalcuLButtonDown(wParam,lParam);
		if (m_buttonFixFace != NULL) m_buttonFixFace->CalcuLButtonDown(wParam,lParam);
		if (m_buttonMusicFade != NULL) m_buttonMusicFade->CalcuLButtonDown(wParam,lParam);
		if (m_buttonMessageEffect != NULL) m_buttonMessageEffect->CalcuLButtonDown(wParam,lParam);
		if (m_buttonMessageEffectTime != NULL) m_buttonMessageEffectTime->CalcuLButtonDown(wParam,lParam);
		//if (m_buttonCommentBottom != NULL) m_buttonCommentBottom->CalcuLButtonDown(wParam,lParam);
		if (m_buttonUndo != NULL) m_buttonUndo->CalcuLButtonDown(wParam,lParam);
		OnLButtonDown(hWnd,wParam,lParam);
		break;

	case WM_RBUTTONDOWN:
		CKomaData* pKoma;
		pKoma = pDoc->GetNowSelectKoma();
		if (pKoma != NULL)
		{
			int px = LOWORD(lParam);
			int py = HIWORD(lParam);


			n = GetContentsNumber(px,py);
			if (n != -1)
//			if ((py>=24) && (py<24+24*4*2))
			{
//				int pos = GetMyScrollPos();
//				int k = (py-24)/(24*4);
//				k += pos;
				int kosuu = pKoma->GetObjectKosuu();
				if ((n>=0) && (n<kosuu))
				{
					int area = GetAreaNumber(px,py);
					switch (area)
					{
						int cmd;
					case 0:
						cmd = OpenPopupMenu(wParam,lParam);
						switch (cmd)
						{
						case ID_MENU_MESSAGE_PRINT:
							pDoc->OnChangeMessageType(n,0);
							break;
						case ID_MENU_MESSAGE_LPRINT:
							pDoc->OnChangeMessageType(n,1);
							break;
						case ID_MENU_MESSAGE_APPEND:
							pDoc->OnChangeMessageType(n,2);
							break;
						case ID_MENU_MESSAGE_DRAW:
							pDoc->OnChangeMessageType(n,3);
							break;
						case ID_MENU_MESSAGE_CHANGEID:
							pDoc->OnChangeID(n);
							break;
						}
						break;
					case 1:
						int voiceChannel;
						voiceChannel = CalcuVoiceChannel(px,py);

						if (voiceChannel < 2)
						{
							cmd = OpenPopupMenu(wParam,lParam,0,-1,1);
						}
						else
						{
							cmd = OpenPopupMenu(wParam,lParam,0,-1,4);
						}

						switch (cmd)
						{
						case ID_MENU_MESSAGE_VOICEPLAYER:
							pDoc->OnChangeVoicePlayer(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICENUMBER:
							break;
						case ID_MENU_MESSAGE_VOICEFILENAME:
							pDoc->OnChangeVoiceFileName(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICEDELETE:
							pDoc->OnDeleteVoice(n,voiceChannel);
							break;

						case ID_MENU_MESSAGE_VOICELOCK:
							pDoc->OnLockVoice(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICEUNLOCK:
							pDoc->OnUnlockVoice(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICE_VOLUME:
							pDoc->OnVoiceVolume(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICE_CONTINUE:
							pDoc->OnVoiceContinue(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICE_COMPLETE:
							pDoc->OnVoiceComplete(n, voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICE_NOWAITVOICE:
							pDoc->OnVoiceNoWaitSameCharaVoice(n, voiceChannel);
							break;

						case ID_MENU_MESSAGE_VOICE_EFFECT:
							pDoc->OnVoiceEffect(n,voiceChannel);
							break;

						case ID_MENU_MESSAGE_VOICE_LOOP:
							pDoc->OnVoiceLoop(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICE_STOP:
							pDoc->OnVoiceStop(n,voiceChannel);
							break;

						case ID_MENU_MESSAGE_VOICE_FADE:
							pDoc->OnVoiceFade(n,voiceChannel);
							break;
						case ID_MENU_MESSAGE_VOICE_VOLUMEONLY:
							pDoc->OnVoiceVolumeOnly(n,voiceChannel);
							break;
						}
						break;
					case 2:
						cmd = OpenPopupMenu(wParam,lParam,0,-1,2);
						int seChannel;
						seChannel = CalcuSeChannel(px,py);
						switch (cmd)
						{
						case ID_MENU_MESSAGE_SENUMBER:
							pDoc->OnChangeSeNumber(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SEFILENAME:
							break;
						case ID_MENU_MESSAGE_SEDELETE:
							pDoc->OnDeleteSe(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SE_STOP:
							pDoc->OnSeStop(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SE_VOLUME:
							pDoc->OnSeVolume(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SE_EFFECT:
							pDoc->OnSeEffect(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SE_LOOP:
							pDoc->OnSeLoop(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SE_FADE:
							pDoc->OnSeFade(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SE_VOLUMEONLY:
							pDoc->OnSeVolumeOnly(n,seChannel);
							break;
						case ID_MENU_MESSAGE_SE_SYSTEM:
							pDoc->OnSeSystem(n, seChannel);
							break;
						}
						break;
					case 3:
						cmd = OpenPopupMenu(wParam,lParam,0,-1,3);
						switch (cmd)
						{
						case ID_MENU_MESSAGE_EDIT:
							pDoc->OnClickModeButton(n);	//おなじしょり
							break;
						case ID_MENU_MESSAGECOLOR_BLACK:
						case ID_MENU_MESSAGECOLOR_BLUE:
						case ID_MENU_MESSAGECOLOR_RED:
						case ID_MENU_MESSAGECOLOR_PURPLE:
						case ID_MENU_MESSAGECOLOR_GREEN:
						case ID_MENU_MESSAGECOLOR_CYAN:
						case ID_MENU_MESSAGECOLOR_YELLOW:
							CheckChangeColor(n,cmd);
							break;

						}
						break;
					}
				}
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
				pDoc->OnNewMessage(-1);	//dummy
				break;
			case 1:
				pDoc->OnCut();
				break;
			case 2:
				pDoc->OnCopy();
				break;
			case 3:
				pDoc->OnPaste();
				break;
			case 4:
				pDoc->OnDelete(-1);
				break;
			case 5:
				pDoc->OnUndo(-1);
				break;
			case 6:
				pDoc->OnWindowOnOff(-1);
				break;
			case 7:
				pDoc->OnClickCommentTopButton();
				break;
			case 8:
				pDoc->OnClickCommentBottomButton();
				break;
			case 9:
				pDoc->OnClickExpStatusButton();
				break;
			case 10:
				pDoc->OnClickMessageFontSizeTypeButton();
				break;
			case 11:
				pDoc->OnClickFaceButton();
				break;
			case 12:
				pDoc->OnClickMustFaceButton();
				break;
			case 13:
				pDoc->OnClickMusicFadeButton();
				break;
			case 14:
				pDoc->OnClickFixFaceButton();
				break;
			case 15:
				pDoc->OnClickMessageEffectButton();
				break;
			case 16:
				pDoc->OnClickMessageEffectTimeButton();
				break;

			}
			break;
		}
/*
		HWND edt;
		edt = m_editHWND[0];

		if (edt != NULL)
		{
			if ((HWND)lParam == edt)
			{
				if (HIWORD(wParam) == EN_CHANGE)
				{
					if (SendMessage(edt,EM_GETMODIFY,0,0))
					{
						char mes[256];
						int lineCount = SendMessage(edt,EM_GETLINECOUNT,0,0);
						if (lineCount>4) lineCount = 4;
						for (int i=0;i<lineCount;i++)
						{
							int c = SendMessage(edt,EM_GETLINE,(WPARAM)i,(LPARAM)mes);
							mes[c+2] = 0;
							mes[c] = 0xd;
							mes[c+1] = 0xa;
							OutputDebugString(mes);
						}
//						GetWindowText(edt,mes,256);
//						OutputDebugString(mes);
//						OutputDebugString("あいうえお\nかきくけこ");
						SendMessage(edt,EM_SETMODIFY,(WPARAM)FALSE,(LPARAM)0);
					}
				}
			}
		}
*/


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
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


void CGameMessageView::OnPaint(HWND hWnd,WPARAM wParam, LPARAM lParam)
{
	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document;

	PAINTSTRUCT ps;
	BeginPaint(hWnd,&ps);

	HGDIOBJ old;
	HDC hdc = ps.hdc;

	CKomaData* pKoma = pDoc->GetNowSelectKoma();
	if (pKoma == NULL)
	{
		RECT rcClient;
		GetClientRect(hWnd,&rcClient);

		HGDIOBJ old = SelectObject(hdc,GetStockObject(GRAY_BRUSH));
		PatBlt(hdc,0,24,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 24,PATCOPY);
		SelectObject(hdc,old);

		EndPaint(hWnd,&ps);
		return;	//nothing to write
	}




//		RECT tmpRect;
//		RECT rc;

		//ps.rcPaintとくらべてかきなおす IntersectRect IntersectClipRect


	HDC src = CreateCompatibleDC(hdc);

	old = SelectObject(src,pDoc->GetMessagePic());

	//upper button???
//OutputDebugString("<");
	if (m_buttonNew != NULL) m_buttonNew->Print(hdc,src,0,&ps.rcPaint);

	if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,0,&ps.rcPaint);
	if (m_buttonCopy != NULL) m_buttonCopy->Print(hdc,src,0,&ps.rcPaint);
	
	if (m_buttonPaste != NULL)
	{
		int pmd = 2;
		if (pDoc->CheckBufferExist()) pmd = 0;
		m_buttonPaste->Print(hdc,src,pmd,&ps.rcPaint);
	}


	if (m_buttonDelete != NULL) m_buttonDelete->Print(hdc,src,0,&ps.rcPaint);
	if (m_buttonUndo != NULL)
	{
		int umd = 2;
		if (pDoc->CheckExistUndo()) umd = 0;
		m_buttonUndo->Print(hdc,src,umd,&ps.rcPaint);
	}
	
	if (m_buttonCommentTop != NULL)
	{
		int md = pDoc->GetCommentPrintMode(0);
		m_buttonCommentTop->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonCommentBottom != NULL)
	{
		int md = pDoc->GetCommentPrintMode(1);
		md = 2;
		m_buttonCommentBottom->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonExpStatus != NULL)
	{
		int md = 0;
		if (pDoc->GetExpStatus()) md = 1;
		m_buttonExpStatus->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonMessageEffect != NULL)
	{
		int md = 0;
		if (pDoc->GetMessageEffect()) md = 1;
		m_buttonMessageEffect->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonMessageEffectTime != NULL)
	{
		int md = 0;
		if (pDoc->GetMessageEffectTime()) md = 1;
		m_buttonMessageEffectTime->Print(hdc,src,md,&ps.rcPaint);
	}

//OutputDebugString(">\r\n");


	if (m_buttonMessageFontSizeType != NULL)
	{
		int md = pDoc->GetMessageFontSizeType();
		if (md > 0) md = 1;
		m_buttonMessageFontSizeType->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonFace != NULL)
	{
		int md = pDoc->GetFace();
		if (md>0)
		{
			md = 1;
		}
		else if (md < 0)
		{
			md = 2;
		}
		m_buttonFace->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonMustFace != NULL)
	{
		int md = pDoc->GetMustFace();
		if (md > 0)
		{
			md = 1;
		}
		m_buttonMustFace->Print(hdc,src,md,&ps.rcPaint);
	}

	if (m_buttonFixFace != NULL)
	{
		int md = pDoc->GetFixFace();
		if (md > 0)
		{
			md = 1;
		}
		m_buttonFixFace->Print(hdc,src,md,&ps.rcPaint);
	}


	if (m_buttonMusicFade != NULL)
	{
		int md = pDoc->GetMusicFadeVolume();
		if (md != 0)
		{
			if (md == 101)
			{
				md = 2;
			}
			else
			{
				md = 1;
			}
		}
		m_buttonMusicFade->Print(hdc,src,md,&ps.rcPaint);
	}


	int wf = 1;
	if (pKoma->GetWindowOffFlag())
	{
		wf = 0;
	}

//OutputDebugString("<<");
	if (m_buttonWindow != NULL) m_buttonWindow->Print(hdc,src,wf,&ps.rcPaint);
//OutputDebugString(">>\r\n");


	SelectObject(src,pDoc->GetMessagePic());


	int kosuu = pKoma->GetObjectKosuu();
	int nowSelect = pKoma->GetNowSelectNumber();
	int subSelect = pKoma->GetNowSelectSubNumber();

	SIZE sz = pDoc->GetDesktopSize();
	int windowSizeX = sz.cx;
	int windowSizeY = sz.cy;

	for (int i=0;i<GetBlockKosuu();i++)
	{
		int k = GetMyScrollPos() + i;
		RECT rc;
		RECT tmpRect;
		int x = 0;
		int y = 24+(24+18*m_editorMessageGyo)*i;
		if ((k>=0) && (k<kosuu))
		{

			SetRect(&rc,x,y,x+windowSizeX,y+24+18*m_editorMessageGyo);
			if (IntersectRect(&tmpRect,&rc,&ps.rcPaint))
			{
				//button

				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(k));

				int typ = pMessage->GetMessageMode();
				BitBlt(hdc,x+m_modeButtonPrintX,y+m_modeButtonPrintY,24,24,src,typ*24,0,SRCCOPY);


				int voiceChannel = pMessage->GetVoiceChannel();

				int ch;
				for (ch=0;ch<4;ch++)
				{
					int addPicFlag = 0;

					int sx = 24;
					int sy = 24;

					if (ch & 1) addPicFlag |= 1;


					BOOL fl = pMessage->CheckVoiceFlag(ch);
					if (fl == FALSE)
					{
						if (pMessage->CheckVoiceOff(ch))
						{
							//addPicFlag |= 0x10;
							fl = TRUE;
						}

						if (pMessage->CheckVoiceVolumeExist(ch))
						{
//							addPicFlag |= 4;
							fl = TRUE;
						}

						if (pMessage->GetVoiceVolumeOnly(ch))
						{
							fl = TRUE;
						}

						if (pMessage->GetVoiceFadeInOut(ch))
						{
							fl = TRUE;
						}


						if (ch<2)
						{
							if (pMessage->CheckVoiceContinue(ch))
							{
								fl = TRUE;
							}
						}

					}

//					if (pMessage->CheckVoiceFlag(ch) || offFlag)
					if (fl)
					{
						sx = 0;
						if (pMessage->CheckVoiceLock(ch))
						{
							sx += 48;
						}

						if (pMessage->GetVoiceVolumeOnly(ch))
						{
							sy = 7 * 24;

							if (pMessage->GetVoiceFadeInOut(ch))
							{
								if (pMessage->CheckVoiceOff(ch) == FALSE)
								{
									addPicFlag |= 0x40;
								}
								else
								{
									addPicFlag |= 0x80;
								}

							}

						}
						else
						{
							if (pMessage->CheckVoiceVolumeExist(ch))
							{
								addPicFlag |= 4;
							}

							if (pMessage->GetVoiceVolumeOnly(ch))
							{
								addPicFlag |= 4;
							}

							if (pMessage->CheckVoiceLoop(ch))
							{
								addPicFlag |= 2;
							}

							if (pMessage->CheckVoiceEffect(ch))	//check pan
							{
								addPicFlag |= 0x8;
							}

							if (pMessage->CheckVoiceComplete(ch))
							{
								addPicFlag |= 0x200;
							}
							if (pMessage->CheckVoiceNoWaitSameCharaVoice(ch))
							{
								addPicFlag |= 0x400;
							}

							if (ch<2)
							{
								if (pMessage->CheckVoiceContinue(ch))
								{
									addPicFlag |= 0x20;
								}
							}
							else
							{
								if (pMessage->CheckVoiceOff(ch))
								{
									addPicFlag |= 0x10;
								}
							}

							if (pMessage->GetVoiceFadeInOut(ch))
							{
								if (pMessage->CheckVoiceOff(ch) == FALSE)
								{
									addPicFlag |= 0x40;
								}
								else
								{
									addPicFlag |= 0x80;
								}

							}
						}

					}


					int putX = x+m_voiceButtonPrintX+ch*24;
					int putY = y+m_voiceButtonPrintY;

					if (ch>=2) putX += 8;

					if (ch != voiceChannel) sx += 24*3;
					BitBlt(hdc,putX,putY,24,24,src,sx,sy,SRCCOPY);


					if (addPicFlag)
					{
						PutAddButtonPic(hdc,src,putX,putY,addPicFlag);
					}
				}





				int seChannel = pMessage->GetSeChannel();
				for (ch=0;ch<8;ch++)
				{
					int sx2 = 24;
					int sy2 = 48;

					int addPicFlag = 0;


					int seMode = pMessage->GetSeMode(ch);

					if (seMode != 0)
					{
						sx2 = 0;

						if (seMode == 1)
						{
							if (pMessage->CheckSeVolumeExist(ch)) addPicFlag |= 4;
							if (pMessage->CheckSeLoop(ch)) addPicFlag |= 0x2;
							if (pMessage->CheckSeEffect(ch)) addPicFlag |= 0x8;
//							if (pMessage->CheckSeStop(ch)) addPicFlag |= 0x10;
						}
						else if (seMode == 2)
						{

//							if (pMessage->CheckSeVolumeExist(ch)) addPicFlag |= 4;
//							if (pMessage->CheckSeLoop(ch)) addPicFlag |= 0x2;
//							if (pMessage->CheckSeEffect(ch)) addPicFlag |= 0x8;
//							if (pMessage->CheckSeStop(ch)) addPicFlag |= 0x10;
							addPicFlag |= 0x10;

						}
						else if (seMode == 3)
						{
							sx2 = 0;
							sy2 = 8 * 24;
						}


						if (pMessage->GetSEFadeInOut(ch))
						{
							if (seMode != 2)
							{
								addPicFlag |= 0x40;
							}
							else
							{
								addPicFlag |= 0x80;
							}
						}
					}

					if (pMessage->CheckSEIsSystem(ch))
					{
						addPicFlag |= 0x100;
					}


					if (ch != seChannel) sx2  += 24*3;

					int putDelta = 0;
					if (ch>=4)
					{
						if (ch>=4) addPicFlag |= 1;
						putDelta = 4;
					}

					int putX = putDelta + x+m_seButtonPrintX+ch*24;
					int putY = y+m_seButtonPrintY;
					BitBlt(hdc,putX,putY,24,24,src,sx2,sy2,SRCCOPY);

	//		addPicFlag = rand() % 32;

					if (addPicFlag)
					{
						PutAddButtonPic(hdc,src,putX,putY,addPicFlag);
					}
				}


				//message serial
				char mes[256];
				int mid = pMessage->GetID();
				if (mid <= 0)
				{
					wsprintf(mes,"[ID]");
				}
				else
				{
					wsprintf(mes,"%d",mid);
				}
				TextOut(hdc,x+24,y+4,mes,strlen(mes));	//1-4095	0=nothing

				//player n(name)-serial [filename]


//				if ((typ == 3) || (	pMessage->GetVoiceFlag() == FALSE) )
				if (pMessage->GetVoiceFlag() == FALSE)
				{
					wsprintf(mes,"");
				}
				else
				{
					int vn = pMessage->GetVoicePlayerNumber(voiceChannel);
					int nn = pMessage->GetVoiceNumber();
					LPSTR cname = pDoc->GetCharaName(vn);
					int vcc = pMessage->GetVoiceChannel();
					LPSTR fname = pMessage->GetVoiceFileName(vcc);
					wsprintf(mes,"%d(%s)-%d [%s]",vn,cname,nn,fname);
				}

				TextOut(hdc,x+24+12*8 +24*2+12,y,mes,strlen(mes));
//				TextOut(hdc,x+24+12*8 +24,y,"PLAYER3()-23 [ar00123]",strlen("PLAYER3()-23 [ar00123]"));

	//			RECT rc2 = rc;
	//			rc2.left += (480 + 24);
	//			rc2.bottom = rc2.top + 24;
	//			DrawText(hdc,"abcde",-1,&rc2,0);
				//filename

				char se[256];
				se[0] = 0;
				if (pMessage->GetSEFlag())
				{
//OutputDebugString("S");
					int sec = pMessage->GetSeChannel();
//if (sec == 4) OutputDebugString("4");
					if (pMessage->CheckSEFlag(sec))
					{
//if (sec == 4) OutputDebugString("o");
						int senum = pMessage->GetSENumber(sec);
						LPSTR sename = pDoc->GetSEName(senum);
						if (sename != NULL)
						{
							LPSTR sefilename = pDoc->GetSEFileName(senum);
							wsprintf(se,"%d【%s】(%s)",senum,sename,sefilename);
						}
					}
					else
					{
//if (sec == 4) OutputDebugString("x");
					}

				}
				TextOut(hdc,x+m_seButtonPrintX+24*8+4,y,se,strlen(se));

				rc.top += 24;

				if (pKoma->CheckInSubArea(k))
//				if ((subSelect != -1) && (k>nowSelect) && (k<=subSelect))
				{
					RECT rc88;
					GetWindowRect(m_hWnd,&rc88);
					HGDIOBJ grey = GetStockObject(GRAY_BRUSH);
					HGDIOBJ old99 = SelectObject(hdc,grey);
		//					BitBlt(hdc,0,24+24+24*4*i,rc.right-rc.left,24*3,src,0,0,SRCCOPY);
					PatBlt(hdc,0,24+24+(24+18*m_editorMessageGyo)*i,rc88.right-rc88.left,18*m_editorMessageGyo,PATCOPY);//@@@@@

					SelectObject(hdc,old99);


				//BitBlt(
			//	PatBlt(hdc,0,24+24+24*4*i,rc.right-rc.left,24*3,DSTINVERT);


				}


				BOOL colChangeFlag = FALSE;
				int col = pDoc->GetMessageColor(k);
				int oldColor = 0;

				if (col != 0)
				{
					colChangeFlag = TRUE;
				}

				if (colChangeFlag)
				{
					oldColor = GetTextColor(hdc);
					SetTextColor(hdc,m_messageColorTable[col*2+1]);
				}


				if ((typ == 0) || (typ == 1) || (typ==2))
				{
					LPSTR text;
					if (pDoc->GetCommentPrintMode(0) == 0)
					{
						text = pMessage->GetMessageDataSkipComment();
					}
					else
					{
						text = pMessage->GetMessageData();
					}
					
					
					if ((subSelect != -1) && (k>nowSelect) && (k<=subSelect))
					{
					}
					
					int bkMode = GetBkMode(hdc);

					if ((subSelect != -1) && (k>nowSelect) && (k<=subSelect))
					{
						SetBkMode(hdc,TRANSPARENT);
					}

					DrawText(hdc,text,-1,&rc,DT_NOPREFIX);
					SetBkMode(hdc,bkMode);



				}
				else if (typ == 3)
				{
					char nm[16];
					wsprintf(nm,"DRAW=%d",pMessage->GetDrawCount());
					DrawText(hdc,nm,-1,&rc,0);
				}

				//				DrawText(hdc,"なまえとか\x00d\x00aいろいろ\x00d\x00aひょうじされるのこころよ\x00d\x00aよんぎょうめ",-1,&rc,0);
				
				if (colChangeFlag)
				{
					SetTextColor(hdc,oldColor);
				}


				//line
				HBRUSH br = CreateSolidBrush(COLORREF(0x00ff00));
				HGDIOBJ old = SelectObject(hdc,br);

				int x0 = x + 16*m_widthGuideLine;
				int y0 = y + 24;
				int x1 = x + 16*m_widthGuideLine;
				int y1 = y + 24 + 18*m_editorMessageGyo;

				MoveToEx(hdc,x0,y0,NULL);
				LineTo(hdc,x1,y1);
				
				SelectObject(hdc,old);
				DeleteObject(br);

			}
		}
		else if (k == kosuu)
		{
			TextOut(hdc,0,y+24,"新規",strlen("新規"));
			//新規
		}
		else
		{
			//aki
		}

		if (k == nowSelect)
		{
			GetWindowRect(m_hWnd,&rc);
			PatBlt(hdc,0,24+24+(24+18*m_editorMessageGyo)*i,rc.right-rc.left,18*m_editorMessageGyo,DSTINVERT);
		}

		//select
	}

	SelectObject(src,old);


//		HBITMAP bm;
//		bm = pDoc->GetMessagePic();
//
//		old = SelectObject(src,bm);
//
//		BitBlt(hdc,0,0,24,24,src,0,0,SRCCOPY);
//		BitBlt(hdc,0,24,24,24,src,0,24,SRCCOPY);
//		BitBlt(hdc,0,48,24,24,src,24,48,SRCCOPY);
//
//		SelectObject(src,old);
//
//		TextOut(hdc,24,0,"なまえとか",strlen("なまえとか"));
//		TextOut(hdc,24,16,"１ぎょうめのめっせーじが表示されるにゃ",strlen("１ぎょうめのめっせーじが表示されるにゃ"));
//		TextOut(hdc,24,32,"えっと",strlen("えっと"));
//		TextOut(hdc,24,48,"ここまでかな",strlen("ここまでかな"));

//		HGDIOBJ old;


	DeleteDC(src);

	EndPaint(hWnd,&ps);
}


void CGameMessageView::SetEditMode(BOOL flag, int n)
{
	m_editMode = flag;
}

/*
void CGameMessageView::ReCalcuScrollPara(void)
{
	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document;
	int kosuu = pDoc->GetMessageKosuu();

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

/*
void CGameMessageView::OnEnterKey(void)
{
}

void CGameMessageView::OnDeleteKey(void)
{
}
*/

void CGameMessageView::OnLButtonDown(HWND hWnd,WPARAM wParam, LPARAM lParam)
{
	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document;

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	int n = GetContentsNumber(x,y);


//	if ((y>=24) && (y<24+24*4*2))
	if (n != -1)
	{
//		int k = (y-24) / (24*4);
//
//		k += GetMyScrollPos();

		int area = GetAreaNumber(x,y);
		switch (area)
		{
		case 0:
			pDoc->OnClickModeButton(n);
			break;
		case 1:
			int voiceChannel;
			voiceChannel = CalcuVoiceChannel(x,y);
			pDoc->OnClickVoiceButton(n,voiceChannel);
			break;
		case 2:
			int seChannel;
			seChannel = CalcuSeChannel(x,y);
			pDoc->OnClickSeButton(n,seChannel);
			break;
		case 3:
			pDoc->OnSelectMessage(n,wParam);
			break;
		}
	}
}

int CGameMessageView::GetMessageButtonParam(int x,int y)
{

	y -= 24;
	if (y<0) return -1;

	y %= (24+18*m_editorMessageGyo);

	if (CheckInButton(x,y,m_modeButtonPrintX,m_modeButtonPrintY))
	{
		return 0;
	}

	int dx = x - m_voiceButtonPrintX;
	int dy = y - m_voiceButtonPrintY;
//	if ((dx>=0) && (dx<32*2) && (dy>=0) && (dy<24)) return 1;
	if ((dx>=0) &&(dy>=0) && (dy<24))
	{
		if (dx<48)
		{
			return dx / 24 + 1;
		}

		if ((dx>=48+8) && (dx<48+8+48))
		{
			return (dx-48-8) / 24 + 3;
		}
	}

	dx = x - m_seButtonPrintX;
	dy = y - m_seButtonPrintY;
	if ((dx>=0) && (dx<24*8+4) && (dy>=0) && (dy<24))
	{
		if (dx<24*4)
		{
			return dx / 24 + 5;
		}

		if (dx>=24*4+4)
		{
			return (dx-24*4-4) / 24 + 9;
		}
	}

	return -1;

}

int CGameMessageView::GetAreaNumber(int x, int y)
{
	y -= 24;
//	if ((y<0) || (y>=24*4*2)) return -1;
	if (y<0) return -1;

	y %= (24+18*m_editorMessageGyo);

	if (CheckInButton(x,y,m_modeButtonPrintX,m_modeButtonPrintY)) return 0;

	int dx = x - m_voiceButtonPrintX;
	int dy = y - m_voiceButtonPrintY;
//	if ((dx>=0) && (dx<32*2) && (dy>=0) && (dy<24)) return 1;
	if ((dx>=0) &&(dy>=0) && (dy<24))
	{
		if (dx<48) return 1;
		if ((dx>=48+8) && (dx<48+8+48)) return 1;
	}

	dx = x - m_seButtonPrintX;
	dy = y - m_seButtonPrintY;
	if ((dx>=0) && (dx<24*8+4) && (dy>=0) && (dy<24))
	{
		if ((dx<24*4) || (dx>=24*4+4)) return 2;
	}

//	if (CheckInButton(x,y,m_voiceButtonPrintX,m_voiceButtonPrintY)) return 1;
//	if (CheckInButton(x,y,m_seButtonPrintX,m_seButtonPrintY)) return 2;

	if (y>=24)
	{
		return 3;
	}

	return -1;
}

int CGameMessageView::CheckInButton(int x, int y, int bx, int by)
{
	if ((x>=bx) && (x<(bx+24)) && (y>=by) && (y<(by+24))) return TRUE;

	return FALSE;
}




void CGameMessageView::CheckChangeColor(int n,int cmd)
{
	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document;

	for (int i=0;i<7;i++)
	{
		if (cmd == m_messageColorTable[i*2])
		{
			pDoc->OnChangeMessageColor(n,i);//m_filmColorTable[i*2+1]);
			return;
		}
	}
}



int CGameMessageView::CalcuVoiceChannel(int x,int y)
{
	y -= 24;
	if (y<0) return 0;

	y %= (24+18*m_editorMessageGyo);

	int dx = x - m_voiceButtonPrintX;
	int dy = y - m_voiceButtonPrintY;
	
	if (dx<48)
	{
		return dx / 24;
	}

	dx -= (48+8);

	return (dx/24) + 2;
}

int CGameMessageView::CalcuSeChannel(int x, int y)
{
	y -= 24;
	if (y<0) return 0;

	y %= (24+18*m_editorMessageGyo);

	int dx = x - m_seButtonPrintX;
	int dy = y - m_seButtonPrintY;

	if (dx<24*4)
	{
		int channel = dx / 24;
		if ((channel >= 0) && (channel <=3)) return channel;
	}

	if (dx>=24*4+4)
	{
		int channel = (dx-(24*4+4)) / 24;
		channel += 4;

		if ((channel >= 4) && (channel <=7)) return channel;
	}

	return 0;
}


void CGameMessageView::PutAddButtonPic(HDC hdc,HDC src,int putX,int putY,int addPicFlag)
{
	HGDIOBJ old = SelectObject(src,m_addButtonHBitmap);
	
	for (int i=0;i<16;i++)
	{
		if (addPicFlag & 1)
		{
			int srcX = 24 * i;
			int srcY = 0;

//			if ((i==1) || (i == 3)) srcX += 8;
//			if (i == 4) srcX += 4;

			BitBlt(hdc,putX,putY,24,24,src,srcX,srcY,SRCAND);
			srcY = 24;
			BitBlt(hdc,putX,putY,24,24,src,srcX,srcY,SRCPAINT);

		}

		addPicFlag >>= 1;
	}


	SelectObject(src,old);
}


BOOL CGameMessageView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 

	int type = GAMEMESSAGE_WINDOW;
	int subType = CheckOnBalloonButton(x,y);

	if (subType == -1)
	{
		//search2
		subType = GetMessageButtonParam(x,y);
		if (subType != -1)
		{
			subType += 17;
		}
	}



	if (subType == -1)
	{
		type = -1;
	}


	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

	return FALSE;
}

BOOL CGameMessageView::OnControlKey(WPARAM wParam,LPARAM lParam)
{
	CGameMessageDoc* pDoc = (CGameMessageDoc*)m_document;

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
		pDoc->OnNewMessage(-1);
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



