//
// layerview.cpp
//


#include <windows.h>
#include <stdio.h>

#include "dlgs.h"

#include "resource.h"


#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nyanEffectLib\effectList.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "windowlist.h"

#include "bitmapnumber.h"


#include "myPopupMenu.h"


#include "case.h"

#include "myBitmap.h"

#include "myDocument.h"
#include "layerdoc.h"
#include "layerview.h"

#include "mybutton.h"



#include "komadata.h"

#include "messageData.h"

//#include "..\cyclib\scriptcommand.h"

#include "myfileopen.h"

#include "myapplicationBase.h"

#include "minipiccache.h"

#define NNNEDITCOMMAND_DELETEPIC 0
#define NNNEDITCOMMAND_SELECTPIC 1
#define NNNEDITCOMMAND_SELECTPICBYNAME 5
#define NNNEDITCOMMAND_SETVARCONTROL 6

#define NNNEDITCOMMAND_SAMEEFFECT 0
#define NNNEDITCOMMAND_DELETEEFFECT 1
#define NNNEDITCOMMAND_SELECTEFFECT 2


#define NNNEDITCOMMAND_LOADEFFECT 0
#define NNNEDITCOMMAND_SAVEEFFECT 1


CLayerView::BUTTONZAHYO CLayerView::m_buttonZahyo[5]=
{
	{0,0,0,24,32,24},
	{32,0,0,24,24,24},
	{32+24,0,0,24,24,24},
	{32+24*2,0,0,24,24,24},
	{32+24*3,0,0,24,32,24},
};

/*
int CLayerView::m_commandToEffectTable[]=
{
//	ID_SETEFFECT_NOP,		EFFECT_NOP,
	ID_SETEFFECT_MAE,		EFFECT_MAE,

	ID_SETEFFECT_PUT,		EFFECT_PUT,

	ID_SETEFFECT_ANIME,	EFFECT_ANIME,

	ID_SETEFFECT_RAIN,		EFFECT_RAIN,
	ID_SETEFFECT_SNOW,		EFFECT_SNOW,
	ID_SETEFFECT_FLOWER,	EFFECT_FLOWER,
	ID_SETEFFECT_SAN,		EFFECT_SAN,
	ID_SETEFFECT_TAIYO,		EFFECT_TAIYO,
	ID_SETEFFECT_WATER,		EFFECT_WATER,
	ID_SETEFFECT_KAGEROU,	EFFECT_KAGEROU,
	ID_SETEFFECT_TRANS,		EFFECT_TRANS,
	ID_SETEFFECT_CTRANS,	EFFECT_CTRANS,
	ID_SETEFFECT_BOSSDEATH,	EFFECT_BOSSDEATH,
	ID_SETEFFECT_CHANGE,	EFFECT_CHANGE,
//	ID_SETEFFECT_MOVE,		EFFECT_MOVE,
	ID_SETEFFECT_FADEIN,	EFFECT_FADEIN,
	ID_SETEFFECT_FADEOUT,	EFFECT_FADEOUT,
	ID_SETEFFECT_FLASH,		EFFECT_FLASH,
	ID_SETEFFECT_SHAKE,		EFFECT_SHAKE,
	ID_SETEFFECT_LIGHT,		EFFECT_LIGHT,
	ID_SETEFFECT_RASTER,	EFFECT_RASTER,
	ID_SETEFFECT_GRADATION,	EFFECT_GRADATION,
	ID_SETEFFECT_BGDARK,	EFFECT_BGDARK,
	ID_SETEFFECT_SCROLL,	EFFECT_SCROLL,
	ID_SETEFFECT_BGZOOMIN,	EFFECT_BGZOOMIN,
//	ID_SETEFFECT_STRETCH,	EFFECT_STRETCH,
	ID_SETEFFECT_RELEAF,	EFFECT_RELEAF,
	ID_SETEFFECT_MELTDOWN,	EFFECT_MELTDOWN,
	ID_SETEFFECT_NEGA,		EFFECT_NEGA,
	ID_SETEFFECT_SUNA,		EFFECT_SUNA,
	ID_SETEFFECT_CIRCLE,	EFFECT_CIRCLE,
	ID_SETEFFECT_REMEMBER,	EFFECT_REMEMBER,
//	ID_SETEFFECT_NIJIMASU,	EFFECT_NIJIMASU,
//	ID_SETEFFECT_NAGASHI,	EFFECT_NAGASHI,
//	ID_SETEFFECT_SURA,		EFFECT_SURA,
	ID_SETEFFECT_MOZAIC,	EFFECT_MOZAIC,
	ID_SETEFFECT_MOUSEMOZAIC,	EFFECT_MOUSEMOZAIC,
	ID_SETEFFECT_KIRA,		EFFECT_KIRA,
//	ID_SETEFFECT_SOFTFOCUS,	EFFECT_SOFTFOCUS,
	ID_SETEFFECT_KIRA,		EFFECT_KIRA,
//	ID_SETEFFECT_KE,		EFFECT_KE,
//	ID_SETEFFECT_WARP,		EFFECT_WARP,
//	ID_SETEFFECT_HANABI,	EFFECT_HANABI,
//	ID_SETEFFECT_TARE,		EFFECT_TARE,
	ID_SETEFFECT_FILL,		EFFECT_FILL,
//	ID_SETEFFECT_OLDFILM,	EFFECT_OLDFILM,
//	ID_SETEFFECT_WIPE,		EFFECT_WIPE,

	ID_SETEFFECT_SIMPLEWIPE,EFFECT_SIMPLEWIPE,
	ID_SETEFFECT_MASKWIPE,	EFFECT_MASKWIPE,

	ID_SETEFFECT_MOVE,		EFFECT_MOVE,

	ID_SETEFFECT_KOMAANIME,	EFFECT_KOMAANIME,
	ID_SETEFFECT_TURN,		EFFECT_TURN,
	ID_SETEFFECT_STRETCHBLT,EFFECT_STRETCHBLT,
	ID_SETEFFECT_BUFFER,	EFFECT_BUFFER,
	ID_SETEFFECT_PERS,		EFFECT_PERS,
	ID_SETEFFECT_CURTAIN,	EFFECT_CURTAIN,

	ID_SETEFFECT_HAMON,		EFFECT_HAMON,
	ID_SETEFFECT_HANABIRA,	EFFECT_HANABIRA,
	ID_SETEFFECT_HOTARU,	EFFECT_HOTARU,

	ID_SETEFFECT_MEISAI,	EFFECT_MEISAI,

	ID_SETEFFECT_GET,		EFFECT_GET,

	ID_SETEFFECT_BURA,		EFFECT_BURA,

	ID_SETEFFECT_ONCE,		EFFECT_ONCE,

	ID_SETEFFECT_SLASH,		EFFECT_SLASH,

	ID_SETEFFECT_BOKASHI,	EFFECT_BOKASHI,
	ID_SETEFFECT_KOMOREBI,	EFFECT_KOMOREBI,

	ID_SETEFFECT_FULLANIME,	EFFECT_FULLANIME,

	ID_SETEFFECT_STAFFROLL,	EFFECT_STAFFROLL,

	ID_SETEFFECT_HAHEN,		EFFECT_HAHEN,
	ID_SETEFFECT_CLOUD,		EFFECT_CLOUD,

	ID_SETEFFECT_BEACH,		EFFECT_BEACH,

	ID_SETEFFECT_FUSUMA,	EFFECT_FUSUMA,




	ID_SETEFFECT_SPEEDLINE,	EFFECT_SPEEDLINE,

	ID_SETEFFECT_ZANTETSU,	EFFECT_ZANTETSU,

	ID_SETEFFECT_GEO_CIRCLE,EFFECT_GEO_CIRCLE,
	ID_SETEFFECT_GEO_POLYGON,EFFECT_GEO_POLYGON,
	ID_SETEFFECT_GEO_TORUS,	EFFECT_GEO_TORUS,

	ID_SETEFFECT_CHARAFLASH,EFFECT_CHARAFLASH,

	ID_SETEFFECT_WATERFALL,	EFFECT_WATERFALL,

	ID_SETEFFECT_SOYOSOYO,	EFFECT_SOYOSOYO,
	ID_SETEFFECT_GRASSPLANE,EFFECT_GRASSPLANE,

	// pollScroll
	// waterDrop

	ID_SETEFFECT_RAINBOW,	EFFECT_RAINBOW,
	ID_SETEFFECT_BATHWINDOW,	EFFECT_BATHWINDOW,

	ID_SETEFFECT_MEPACHIKUCHIPAKU,	EFFECT_MEPACHIKUCHIPAKU,

	ID_SETEFFECT_COLORADDSUB,	EFFECT_COLORADDSUB,

	ID_SETEFFECT_HAIKEIYURE,	EFFECT_HAIKEIYURE,


	-1,-1,
};
*/

















CLayerView::CLayerView(CMyDocument* pDocument,HWND clientHWND,HINSTANCE hinstance) : CMyView(pDocument, clientHWND, hinstance)
{
	m_upperAreaSizeY = 24;

	m_leftPicSizeX = 32;
	m_leftPicSizeY = 24;
	m_leftButtonSizeX = 24;
	m_leftButtonSizeY = 24;
	m_eyeSizeX = 24;
	m_eyeSizeY = 24;
	m_rightButtonSizeX = 24;
	m_rightButtonSizeY = 24;
	m_effectSizeX = 32;
	m_effectSizeY = 24;


	SetMyViewWindowName("layerWindow");
	HICON hicon = LoadIcon(hinstance,MAKEINTRESOURCE(IDI_ICON_MDICHILDWINDOW));
	SendMessage(m_hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hicon);

	m_captureCopyType = 0;
	m_captureLayerEffect = -1;

	m_captureCursorCopy = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_LAYERCOPY));
	m_captureCursorMove = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_LAYERMOVE));
	m_captureCursorExchange = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_LAYEREXCHANGE));
	m_captureCursorBad = LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_NODROP));
	m_defaultCursor = LoadCursor(NULL,IDC_ARROW);


//	SetResizeFlag();

	int layerMax = CKomaData::m_layerMax;

	SetBlockSize(m_leftPicSizeX+m_leftButtonSizeX+m_eyeSizeX+m_rightButtonSizeX+m_effectSizeX,m_leftPicSizeY*layerMax);
	SetBlockKosuuMin(1,1);
	SetBlockKosuuMax(1,1);
	SetNonBlockArea(0,m_upperAreaSizeY,0,0);

//	SetFreeSizeFlag(1);
//	SetScrollFlag(TRUE,FALSE);

	SetWindowStyle();



	SetViewWindowText("レイヤー");

//	m_view->MoveViewWindow(651,23,24+32+24+24+32,24*16+24);



	m_picTypeKosuu = ((CLayerDoc*)m_document)->GetPicTypeKosuu();

	m_miniPic = new CMiniPicCache();

//	m_hBitmapEye0 = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_EYE0));
//	m_hBitmapEye1 = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_EYE1));
//	m_hBitmapWrite0 = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_WRITE0));
//	m_hBitmapWrite1 = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_WRITE1));

	m_bitmapEffectMini = new CMyBitmap("nnndir\\setup\\bmp\\effect_mini.bmp");
	m_bitmapVarControlMark = new CMyBitmap("nnndir\\setup\\bmp\\varcontrolmark.bmp");
	m_bitmap = new CMyBitmap("nnndir\\setup\\bmp\\layer.bmp");

	m_hBitmapEffectMini = m_bitmapEffectMini->GetHBitmap();
	m_hBitmap = m_bitmap->GetHBitmap();
	m_hBitmapVarControl = m_bitmapVarControlMark->GetHBitmap();

//	m_hBitmapEffectMini = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_EFFECTMINI));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_LAYER));



	HDC hdc = GetDC(m_clientHWND);
	BITMAPINFO bmi;

	ZeroMemory(&bmi,sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biHeight = -24;
	bmi.bmiHeader.biWidth = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	int i;
	for (i=0;i<layerMax;i++)
	{
		m_hBitmapDIB[i] = CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,(void**)&m_dibBuffer[i],NULL,NULL);
//		m_hBitmapDIB2[i] = CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,(void**)&m_dibBuffer2[i],NULL,NULL);

		int r1 = (rand() % 256);
		int r2 = (rand() % 256) << 8;

		for (int ii=0;ii<32*24;ii++)
		{
			*(m_dibBuffer[i] + ii) = r1;
//			*(m_dibBuffer2[i] + ii) = r2;
		}
	}


	ReleaseDC(m_hWnd,hdc);

	for (i=0;i<layerMax;i++) m_lastPictureBufferNumber[i] = 0;

	for (i=0;i<layerMax;i++)
	{
		m_lpFileNameBuffer[i] = new char[64];
		*(m_lpFileNameBuffer[i]) = 0;
	}

//	m_writeButtonPrintX = 0;
//	m_writeButtonPrintY = 0;
//	m_writeButtonNextX = 0;
//	m_writeButtonNextY = 24;
//	m_writeButtonSizeX = 24;
//	m_writeButtonSizeY = 24;


//	m_buttonZahyo[0].x = m_writeButtonPrintX;
//	m_buttonZahyo[0].y = m_writeButtonPrintY;
//	m_buttonZahyo[0].nextX = m_writeButtonNextX;
//	m_buttonZahyo[0].nextY = m_writeButtonNextY;
//	m_buttonZahyo[0].sizeX = m_writeButtonSizeX;
//	m_buttonZahyo[0].sizeY = m_writeButtonSizeY;



//	HWND hwnd0 = GetDesktopWindow();
//	HDC hdc0 = GetDC(hwnd0);
//	int bpp = GetDeviceCaps(hdc0,BITSPIXEL);
//	ReleaseDC(hwnd0,hdc0);
//
//	m_directX = new CMyDirectX(m_hWnd,m_hInstance,160,120,bpp,FALSE);



	m_pictureMenu = new CMyPopupMenu("pictureMenu",NNNEDITCOMMAND_SELECTPIC);
	m_pictureMenu->SetMyMenu(m_hWnd);
	m_effectMenu = new CMyPopupMenu("effectMenu",NNNEDITCOMMAND_SELECTEFFECT);
	m_effectMenu->SetMyMenu(m_hWnd);
	m_layerMenu = new CMyPopupMenu("layerMenu",NNNEDITCOMMAND_LOADEFFECT);
	m_layerMenu->SetMyMenu(m_hWnd);

//	m_leftButtonPic = new CMyBitmap("nnndir\\setup\\bmp\\layerLeftButton",hdc);
//	m_rightButtonPic = new CMyBitmap("nnndir\\setup\\bmp\\layerRightButton",hdc);
	m_eyeButtonPic = new CMyBitmap("nnndir\\setup\\bmp\\eye.bmp");
	m_effectMiniPic = new CMyBitmap("nnndir\\setup\\bmp\\effect_mini.bmp");


	//m_popupMenu[0] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_PIC));
	//m_popupMenu[1] = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU_EFFECT));







//	m_buttonCut = new CMyButton(0,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_CUT),0,0);

	
	m_buttonOpen = new CMyButton(1,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_OPEN),0,0);
	m_buttonSave = new CMyButton(2,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_SAVE),24,0);

//	m_buttonDelete = new CMyButton(3,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_DELETE),56,0);
//	m_buttonUndo = new CMyButton(4,m_hWnd,m_document->GetCommonBitmap(BITMAPNUMBER_UNDO),88,0);

	m_copyPreEffectBitmap = new CMyBitmap("nnndir\\setup\\bmp\\copypreeffect.bmp");
	m_copyPreEffect2Bitmap = new CMyBitmap("nnndir\\setup\\bmp\\copypreeffect2.bmp");
	m_buttonCopyPreEffect = new CMyButton(3,m_hWnd,m_copyPreEffectBitmap->GetHBitmap(),56,0);
	m_buttonCopyPreEffect2 = new CMyButton(4,m_hWnd,m_copyPreEffect2Bitmap->GetHBitmap(),80,0);

	m_file = new CMyFileOpen((m_document->GetApp())->GetFrameHWND(),(m_document->GetApp())->GetMyInstance(),"dwq","*.dwq");
}


CLayerView::~CLayerView()
{
	End();
}

void CLayerView::End(void)
{
	ENDDELETECLASS(m_file);

	ENDDELETECLASS(m_buttonCopyPreEffect2);
	ENDDELETECLASS(m_buttonCopyPreEffect);

	ENDDELETECLASS(m_copyPreEffect2Bitmap);
	ENDDELETECLASS(m_copyPreEffectBitmap);

	ENDDELETECLASS(m_effectMiniPic);
	ENDDELETECLASS(m_eyeButtonPic);
//	ENDDELETECLASS(m_rightButtonPic);
//	ENDDELETECLASS(m_leftButtonPic);
	ENDDELETECLASS(m_layerMenu);
	ENDDELETECLASS(m_effectMenu);
	ENDDELETECLASS(m_pictureMenu);
//	ENDDELETECLASS(m_picTypeList);

//	if (m_popupMenu != NULL)
//	{
//		DestroyMenu(m_popupMenu);
//		m_popupMenu = NULL;
//	}

	int layerMax = CKomaData::m_layerMax;

	int i;
	for (i=0;i<layerMax;i++)
	{
		if (m_hBitmapDIB[i] != NULL)
		{
			DeleteObject(m_hBitmapDIB[i]);
		}
	//	DELETEGDIOBJECT(m_hBitmapDIB[i]);
//		DELETEGDIOBJECT(m_hBitmapDIB2[i]);
	}


	for (i=0;i<layerMax;i++)
	{
		DELETEARRAY(m_lpFileNameBuffer[i]);
	}


//	DELETEGDIOBJECT(m_hBitmapEye0);
//	DELETEGDIOBJECT(m_hBitmapEye1);
//	DELETEGDIOBJECT(m_hBitmapWrite0);
//	DELETEGDIOBJECT(m_hBitmapWrite1);

//	DELETEGDIOBJECT(m_hBitmapEffectMini);
//	ENDDELETECLASS(m_directX);

//	ENDDELETECLASS(m_buttonCut);
	ENDDELETECLASS(m_buttonOpen);
	ENDDELETECLASS(m_buttonSave);
//	ENDDELETECLASS(m_buttonDelete);
//	ENDDELETECLASS(m_buttonUndo);

//	DELETEGDIOBJECT(m_hBitmap);

	ENDDELETECLASS(m_bitmapVarControlMark);
	ENDDELETECLASS(m_bitmapEffectMini);
	ENDDELETECLASS(m_bitmap);
	ENDDELETECLASS(m_miniPic);
}



LRESULT CLayerView::ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int x,y;
	int xNum;
	int yNum;
	POINT pt;

	CLayerDoc* pDoc = (CLayerDoc*)m_document;

	switch (message)
	{
///	case WM_LBUTTONDBLCLK:
///		MessageBox(m_hWnd,"doucle click","aaa",MB_OK);
///		break;

	case WM_SETCURSOR:
//		SetMyCursor(wParam,lParam);
		break;

	case WM_RBUTTONDOWN:
		MyReleaseCapture();
//OutputDebugString("Release\n");

		x = LOWORD(lParam);
		y = HIWORD(lParam);


		xNum = GetAreaNumberX(x,y);
		yNum = GetAreaNumberY(x,y);

		if ((x == -1) || (y == -1)) break;

		if ((xNum == 0) || (xNum==4))
		{
			pt.x = x;
			pt.y = y;
			ClientToScreen(hWnd, &pt);

//@@@??			HMENU submenu;
			int layer = yNum;

			int cmd;

			if (xNum==0)
			{
				cmd = m_pictureMenu->OpenMenu(pt);
				if (cmd == NNNEDITCOMMAND_DELETEPIC) pDoc->DeletePic(layer);
				if ((cmd >= NNNEDITCOMMAND_SELECTPIC) && (cmd < NNNEDITCOMMAND_SELECTPIC + m_picTypeKosuu)) pDoc->SelectPictureFile(layer,cmd - NNNEDITCOMMAND_SELECTPIC,hWnd);
				
				if (cmd == NNNEDITCOMMAND_SELECTPICBYNAME) pDoc->SelectPictureFileByName(layer,cmd - NNNEDITCOMMAND_SELECTPIC,hWnd);
				if (cmd == NNNEDITCOMMAND_SETVARCONTROL) pDoc->SetVarControl(layer,cmd - NNNEDITCOMMAND_SELECTPIC,hWnd);



//				submenu = GetSubMenu(m_popupMenu[0],0);
//				int cmd = TrackPopupMenu(submenu,TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,pt.x,pt.y,0,m_hWnd,NULL);
//				if ((cmd == ID_SELECTPIC_TA) || (cmd == ID_SELECTPIC_BG) || (cmd == ID_SELECTPIC_EV) || (cmd == ID_SELECTPIC_ETC)) SelectPic(layer,cmd);
//				if (cmd == ID_DELETEPIC) pDoc->DeletePic(layer);
			}

			if (xNum==4)
			{
				cmd = m_effectMenu->OpenMenu(pt);

				//submenu = GetSubMenu(m_popupMenu[1],0);
				//int cmd = TrackPopupMenu(submenu,TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,pt.x,pt.y,0,m_hWnd,NULL);
				if (cmd == NNNEDITCOMMAND_DELETEEFFECT) pDoc->ChangeEffect(layer,-1);
				if (cmd == NNNEDITCOMMAND_SAMEEFFECT) pDoc->ChangeEffect(layer,-2);	//same
				if (cmd >= NNNEDITCOMMAND_SELECTEFFECT) pDoc->ChangeEffect(layer,cmd-NNNEDITCOMMAND_SELECTEFFECT);

				
//				if ((cmd != 0) && (cmd != ID_DELETE_EFFECT) && (cmd != ID_SETEFFECT_EX)) pDoc->ChangeEffect(layer, CommandToEffect(cmd));
				//if (cmd == ID_DELETE_EFFECT) pDoc->ChangeEffect(layer,-1);
//				if (cmd == ID_SETEFFECT_EX) pDoc->ChangeEffect(layer, CommandToEffect(cmd));
			}
		}

		if (xNum == 2)
		{
			pt.x = x;
			pt.y = y;
			ClientToScreen(hWnd, &pt);

//@@@??			HMENU submenu;
			int layer = yNum;
			int cmd;

			cmd = m_layerMenu->OpenMenu(pt);
			if (cmd == NNNEDITCOMMAND_LOADEFFECT) pDoc->OnLoadLayerEffect(layer);
			if (cmd == NNNEDITCOMMAND_SAVEEFFECT) pDoc->OnSaveLayerEffect(layer);
		}


		break;

	case WM_LBUTTONDOWN:
		GetCapture();
//		m_captureFlag = TRUE;
//OutputDebugString("Capture\n");
//		SetCursor(m_captureCursorCopy);
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		if (m_buttonOpen != NULL) m_buttonOpen->CalcuLButtonDown(wParam,lParam);
		if (m_buttonSave != NULL) m_buttonSave->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCopyPreEffect != NULL) m_buttonCopyPreEffect->CalcuLButtonDown(wParam,lParam);
		if (m_buttonCopyPreEffect2 != NULL) m_buttonCopyPreEffect2->CalcuLButtonDown(wParam,lParam);

//		if (m_buttonUndo != NULL) m_buttonUndo->CalcuLButtonDown(wParam,lParam);

		xNum = GetAreaNumberX(x,y);
		yNum = GetAreaNumberY(x,y);

		if (yNum != -1)
		{
			if (xNum == 2) pDoc->ClickEyeButton(yNum);
			if ((xNum == 0) || (xNum == 1)) pDoc->OnClickPictureButton(yNum);
			if ((xNum == 3) || (xNum == 4)) pDoc->OnClickEffectButton(yNum);

			m_captureLayerEffect = -1;
			if (xNum == 0) m_captureLayerEffect = 0;
			if (xNum == 4) m_captureLayerEffect = 4;
			m_captureLayer = yNum;

			if (wParam & MK_CONTROL)
			{
				m_captureCopyType = 1;
			}
			else if (wParam & MK_SHIFT)
			{
				m_captureCopyType = 0;
			}
			else
			{
				m_captureCopyType = 2;
			}
		}
		break;

	case WM_LBUTTONUP:
		if (m_captureLayerEffect != -1)
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			xNum = GetAreaNumberX(x,y);
			yNum = GetAreaNumberY(x,y);

			if (yNum != -1)
			{
				if (xNum == m_captureLayerEffect)
				{
					if ((yNum != -1) && (yNum != m_captureLayer))
					{
						if (m_captureLayerEffect == 0)
						{
							if (m_captureCopyType == 0) pDoc->MoveLayer(m_captureLayer,yNum);
							if (m_captureCopyType == 1) pDoc->CopyLayer(m_captureLayer,yNum);
							if (m_captureCopyType == 2) pDoc->ExchangeLayer(m_captureLayer,yNum);
						}
						else if (m_captureLayerEffect == 4)
						{
							if (m_captureCopyType == 0) pDoc->MoveEffect(m_captureLayer,yNum);
							if (m_captureCopyType == 1) pDoc->CopyEffect(m_captureLayer,yNum);
							if (m_captureCopyType == 2) pDoc->ExchangeEffect(m_captureLayer,yNum);
						}
					}
				}
			}
		}
		MyReleaseCapture();


		break;

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
		{
			if (m_captureLayerEffect != -1)
			{
				m_captureFlag = TRUE;
			}
			SetMyCursor(wParam,lParam);
		}

		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_USER_LBUTTONDOWN:
			switch (lParam)
			{
			case 1:
				pDoc->OnOpenEffect();
				break;
			case 2:
				pDoc->OnSaveEffect();
				break;
			case 3:
				pDoc->OnCopyPreEffect();
				break;
			case 4:
				pDoc->OnCopyPreEffect2();
				break;

//			case 4:
//				pDoc->OnUndoEffect();
//				break;
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
//	case WM_COMMAND:
	case WM_ERASEBKGND:
//		HGDIOBJ old5;
//		HDC hdc5;
//		hdc5 = (HDC)wParam;
//		old5 = SelectObject(hdc5,GetStockObject(LTGRAY_BRUSH));
//		PatBlt(hdc5,0,0,160,24,PATCOPY);
//		SelectObject(hdc5,old5);
//
		EraseUpper((HDC)wParam);

		return TRUE;
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);
		HDC hdc = ps.hdc;

		CKomaData* pKoma;
		pKoma = pDoc->GetNowSelectKoma();
//		if (pKoma == NULL)
		if (pDoc->CheckKomaSelected() == FALSE)
		{
			int layerMax = CKomaData::m_layerMax;

			HGDIOBJ old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
			PatBlt(hdc,0,24,32*2+24*3,24*layerMax,PATCOPY);
			SelectObject(hdc,old);
			EndPaint(hWnd,&ps);
			return 0;
			break;
		}


		RECT tmpRect;
		RECT rc;

		//ps.rcPaintとくらべてかきなおす IntersectRect IntersectClipRect


		HDC src = CreateCompatibleDC(hdc);

//		if (m_buttonCut != NULL) m_buttonCut->Print(hdc,src,2,&ps.rcPaint);
		if (m_buttonOpen != NULL) m_buttonOpen->Print(hdc,src,0,&ps.rcPaint);
		if (m_buttonSave != NULL) m_buttonSave->Print(hdc,src,0,&ps.rcPaint);

		if (1)
		{
			int md = pDoc->GetCopyPreEffectMode();
			if (md == 0)
			{
				md = 3;
			}
			else
			{
				md = 0;
			}

			if (md == 0)
			{
				if (m_buttonCopyPreEffect != NULL) m_buttonCopyPreEffect->Print(hdc,src,md,&ps.rcPaint);
				if (m_buttonCopyPreEffect2 != NULL) m_buttonCopyPreEffect2->Print(hdc,src,md,&ps.rcPaint);
			}
		}

//		if (m_buttonDelete != NULL) m_buttonDelete->Print(hdc,src,0,&ps.rcPaint);
//		if (m_buttonUndo != NULL) m_buttonUndo->Print(hdc,src,2,&ps.rcPaint);

		HGDIOBJ old;

		int picLayer;
		int effLayer;


		picLayer = pDoc->GetSelectPictureLayer();
		effLayer = pDoc->GetSelectEffectLayer();

		int layerMax = CKomaData::m_layerMax;
		for (int i=0;i<layerMax;i++)
		{
			//left picture
			GetUpdateArea(&rc,i,0);
			if (IntersectRect(&tmpRect,&rc,&ps.rcPaint))
			{
				if (pKoma->GetPictureNumber(i) != -1)
				{
					LPSTR name = pKoma->GetPicFileName(i);
					if (strcmp(name,m_lpFileNameBuffer[i]) != 0)
					{
						int nm = m_miniPic->GetBufferNumber(name,m_lastPictureBufferNumber[i]);
						int* buf = m_miniPic->GetPicture(name,nm);
						if (buf != NULL)
						{
							memcpy(m_dibBuffer[i],buf,sizeof(int)*32*24);
							//copy data
							m_lastPictureBufferNumber[i] = nm;

							//name copy
							int ln = strlen(name);
							if (ln>62) ln = 62;
							memcpy(m_lpFileNameBuffer[i],name,ln+1);
						}
					}

					old = SelectObject(src,m_hBitmapDIB[i]);
					BitBlt(hdc,0,i*24+24,32,24,src,0,0,SRCCOPY);
//					SelectObject(src,old);
				}
				else
				{
					old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
					PatBlt(hdc,0,i*24+24,32,24,PATCOPY);
//					SelectObject(hdc,old);
				}

				//var mark?
				int varControl = pDoc->GetVarControlLayerCheckPre(i);
				if (varControl > 0)
				{
					SelectObject(src,m_hBitmapVarControl);
					BitBlt(hdc,0,i*24+24,8,8,src,0,0,SRCCOPY);
				}
				else if (varControl == -1)//削除命令
				{
					SelectObject(src,m_hBitmapVarControl);
					BitBlt(hdc,0,i*24+24,8,8,src,8,0,SRCCOPY);
				}


				SelectObject(src,old);

			}



			//left button
			GetUpdateArea(&rc,i,1);
			if (IntersectRect(&tmpRect,&rc,&ps.rcPaint))
			{
				BOOL onFlag = FALSE;
				if (1)
				{
					if (i == picLayer) onFlag = TRUE;
				}
				PutLeftButton(hdc,src,i,onFlag);
			}

			//eye button
			GetUpdateArea(&rc,i,2);
			if (IntersectRect(&tmpRect,&rc,&ps.rcPaint))
			{
				BOOL onFlag = TRUE;
				if (pDoc->GetEyeStatus(i) == 0) onFlag = FALSE;
				PutEyeButton(hdc,src,i,onFlag);
			}


			//right button
			GetUpdateArea(&rc,i,3);
			if (IntersectRect(&tmpRect,&rc,&ps.rcPaint))
			{
				BOOL onFlag = FALSE;
				if (1)
				{
					if (i == effLayer) onFlag = TRUE;
				}
				PutRightButton(hdc,src,i,onFlag);
			}

			//right effect
			GetUpdateArea(&rc,i,4);
			if (IntersectRect(&tmpRect,&rc,&ps.rcPaint))
			{
				int eff = pKoma->GetEffect(i);
				PutMiniEffect(hdc,src,i,eff);
			}
		}

		DeleteDC(src);

		EndPaint(hWnd,&ps);
		return 0;
		break;
	}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


void CLayerView::UpdateWriteButton(int layer)
{
	UpdateButtonArea(layer,0);
}

void CLayerView::UpdateEyeButton(int layer)
{
	UpdateButtonArea(layer,1);
}

void CLayerView::UpdatePic(int layer)
{
}

//void CLayerView::UpdatePic2(int layer)
//{
//}


void CLayerView::UpdateButtonArea(int layer, int nm)
{
	RECT rc;
	GetUpdateArea(&rc,layer,nm);	
	InvalidateRect(m_hWnd,&rc,FALSE);
}


void CLayerView::GetUpdateArea(RECT* lpRect,int layer,int nm)
{
	int x = m_buttonZahyo[nm].x;
	int y = m_buttonZahyo[nm].y + 24;
	int nextX = m_buttonZahyo[nm].nextX;
	int nextY = m_buttonZahyo[nm].nextY;
	int sizeX = m_buttonZahyo[nm].sizeX;
	int sizeY = m_buttonZahyo[nm].sizeY;

	SetRect(lpRect,x+nextX*layer,y+nextY*layer,x+nextX*layer+sizeX,y+nextY*layer+sizeY);

}


void CLayerView::SetMiniPic(int picNumber, int* picData)
{
	int layerMax = CKomaData::m_layerMax;

	if ((picNumber<0) || (picNumber>=layerMax)) return;
	if (m_dibBuffer[picNumber] == NULL) return;

	memcpy(m_dibBuffer[picNumber],picData,sizeof(int)*32*24);
}

//void CLayerView::SetMiniPic2(int picNumber, int* picData)
//{
//	if ((picNumber<0) || (picNumber>=PIC_KOSUU_MAX)) return;
//	if (m_dibBuffer2[picNumber] == NULL) return;
//
//	memcpy(m_dibBuffer2[picNumber],picData,sizeof(int)*32*24);
//}


void CLayerView::ClearMiniPic(int n)
{
	int* dst = m_dibBuffer[n];
	for (int i=0;i<32*24;i++)
	{
		*dst = 0;
		dst++;
	}
}

void CLayerView::PictureToMini(int n, int* src, int x, int y)
{
	int* dst = m_dibBuffer[n];

	int loopX = x / 32;
	int loopY = y / 24;

	int div = loopX * loopY;
	if (div<1) div = 1;

	for (int j=0;j<24;j++)
	{
		for (int i=0;i<32;i++)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			int* ptr0 = src + i * loopX + j * loopY * x;

			for (int jj=0;jj<loopY;jj++)
			{
				int* ptr = ptr0;

				for (int ii=0;ii<loopX;ii++)
				{
					int d = *ptr;
					int rr = (d>>16) & 0xff;
					int gg = (d>>8) & 0xff;
					int bb = d & 0xff;

					r += rr;
					g += gg;
					b += bb;

					ptr++;
				}
				ptr0 += x;
			}

			r /= div;
			g /= div;
			b /= div;

			if (r>255) r = 255;
			if (g>255) g = 255;
			if (b>255) b = 255;

			*dst = (r<<16) | (g<<8) | b;
			dst++;
		}
	}
}

/*
UINT CALLBACK CLayerView::OFNHookProc(HWND hdlg, UINT uiMsg,  WPARAM wParam,  LPARAM lParam)
{
	static CLayerView* myview;

	if (uiMsg == WM_INITDIALOG)
	{
		OPENFILENAME* lp = (OPENFILENAME*)lParam;
		myview = (CLayerView*)(lp->lCustData);
	}

	if (uiMsg == WM_PAINT)
	{
		HDC hdc = GetDC(hdlg);
//		HDC hdc = (HDC)wParam;
		TextOut(hdc,30,330,"abcde",strlen("abcde"));
		ReleaseDC(hdlg,hdc);
//		RECT rc;
//		SetRect(&rc,0,0,100,100);
//		ValidateRect(hdlg,&rc);
	}


	if (uiMsg == WM_NOTIFY)
	{
		OFNOTIFY* of = (OFNOTIFY*)lParam;
		if (of->hdr.code == CDN_SELCHANGE)
		{
			char filename[1024];

			HWND stc = GetDlgItem(hdlg,of->hdr.idFrom);
			RECT rc1;
			GetWindowRect(of->hdr.hwndFrom,&rc1);
			RECT rc0;
			GetWindowRect(hdlg,&rc0);


//			int rt = SendMessage(hdlg,CDM_GETFILEPATH,(WPARAM)1024,(LPARAM)filename);
			int rt = SendMessage(of->hdr.hwndFrom,CDM_GETSPEC,(WPARAM)1024,(LPARAM)filename);



			if (rt>0)
			{
				OutputDebugString(filename);
			}
			else
			{
				OutputDebugString("Error");
			}

			//サムネイルの表示

//			RECT rc;
//			SetRect(&rc,0,0,100,100);
//			InvalidateRect(hdlg,&rc,TRUE);


		}
		else
		{
//			OutputDebugString("[etc]");
		}
	}

	return 0;
}
*/





/*
int CLayerView::CommandToEffect(int cmd)
{
	for (int i=0;i<256;i++)
	{
		int checkCmd = m_commandToEffectTable[i*2];
		if (checkCmd == -1) return -1;
		if (checkCmd == cmd) return m_commandToEffectTable[i*2+1];

	}

	return -1;
}
*/


//public method
int CLayerView::GetAreaNumberX(int x, int y)
{
	int layerMax = CKomaData::m_layerMax;

	if ((x<0) || (x>=32*2+24*3)) return -1;
	if ((y<24) || (y>=24*(layerMax+1))) return -1;

	if ((x>=0) && (x<32)) return 0;
	if ((x>=32) && (x<32+24)) return 1;
	if ((x>=32+24) && (x<32+24*2)) return 2;
	if ((x>=32+24*2) && (x<32+24*3)) return 3;
	if ((x>=32+24*3) && (x<32+24*3+32)) return 4;

	return -1;
}


int CLayerView::GetAreaNumberY(int x,int y)
{
	int layerMax = CKomaData::m_layerMax;

	if ((x<0) || (x>=32*2+24*3)) return -1;
	if ((y<24) || (y>=24*(layerMax+1))) return -1;

	y -= 24;
	y /= 24;
	if ((y<0) || (y>=layerMax)) return -1;

	return y;
}


/////////////////////////////////
void CLayerView::PutLeftButton(HDC hdc,HDC src,int n,BOOL onFlag)
{
	int sizeX = m_leftButtonSizeX;
	int sizeY = m_leftButtonSizeY;

	int srcX = 0;
	int srcY = 48;
	
	if (onFlag) srcX += sizeX;
	if ((n < 4) || (n == 30)) srcY += sizeY;

	int putX = m_leftPicSizeX;
	int putY = m_upperAreaSizeY + n*sizeY;

	m_bitmap->Blt(hdc,putX,putY,srcX,srcY,sizeX,sizeY);
}

void CLayerView::PutRightButton(HDC hdc,HDC src,int n,BOOL onFlag)
{
	int sizeX = m_rightButtonSizeX;
	int sizeY = m_rightButtonSizeY;

	int srcX = 0;
	int srcY = 96;
	
	if (onFlag) srcX += sizeX;
	if ((n < 4) || (n == 30)) srcY += sizeY;

	int putX = m_leftPicSizeX + m_leftButtonSizeX + m_eyeSizeX;
	int putY = m_upperAreaSizeY + n*sizeY;

	m_bitmap->Blt(hdc,putX,putY,srcX,srcY,sizeX,sizeY);

//	m_rightButtonPic->Blt(hdc,putX,putY,srcX,srcY,sizeX,sizeY,src);
}

void CLayerView::PutEyeButton(HDC hdc,HDC src,int n,BOOL onFlag)
{
	int sizeX = m_eyeSizeX;
	int sizeY = m_eyeSizeY;

	int srcX = 0;
	int srcY = sizeY * n;
	
	if (onFlag) srcX += sizeX;

	int putX = m_leftPicSizeX + m_leftButtonSizeX;
	int putY = m_upperAreaSizeY + n*sizeY;

	m_eyeButtonPic->Blt(hdc,putX,putY,srcX,srcY,sizeX,sizeY);

//	m_eyeButtonPic->Blt(hdc,putX,putY,srcX,srcY,sizeX,sizeY,src);
}

void CLayerView::PutMiniEffect(HDC hdc,HDC src,int n,int eff)
{
	int sizeX = m_effectSizeX;
	int sizeY = m_effectSizeY;

	int srcX = (eff % 10) * sizeX;
	int srcY = (eff / 10) * sizeY;

	int putX = m_leftPicSizeX + m_leftButtonSizeX + m_eyeSizeX + m_rightButtonSizeX;
	int putY = m_upperAreaSizeY + n*sizeY;

	if (eff != -1)
	{
		m_effectMiniPic->Blt(hdc,putX,putY,srcX,srcY,sizeX,sizeY);
	}
	else
	{
		HGDIOBJ old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
		PatBlt(hdc,putX,putY,sizeX,sizeY,PATCOPY);
		SelectObject(hdc,old);
	}
}


void CLayerView::MyReleaseCapture(void)
{
	ReleaseCapture();

	m_captureFlag = FALSE;
	m_captureLayerEffect = -1;

	SetCursor(m_defaultCursor);
}

void CLayerView::SetMyCursor(WPARAM wParam,LPARAM lParam)
{
		if (m_captureFlag == FALSE)
		{
			SetCursor(m_defaultCursor);
		}
		else
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			int xNum = GetAreaNumberX(x,y);
			int yNum = GetAreaNumberY(x,y);

			if (m_captureLayerEffect == 0)
			{
				if (xNum != 0)
				{
					SetCursor(m_captureCursorBad);
				}
				else
				{
					if (m_captureCopyType == 0)
					{
						SetCursor(m_captureCursorMove);
					}
					else if (m_captureCopyType == 1)
					{
						SetCursor(m_captureCursorCopy);
					}
					else
					{
						SetCursor(m_captureCursorExchange);
					}
				}
			}
			else if (m_captureLayerEffect == 4)
			{
				if (xNum != 4)
				{
					SetCursor(m_captureCursorBad);
				}
				else
				{
					if (m_captureCopyType == 0)
					{
						SetCursor(m_captureCursorMove);
					}
					else if (m_captureCopyType == 1)
					{
						SetCursor(m_captureCursorCopy);
					}
					else
					{
						SetCursor(m_captureCursorExchange);
					}
				}
			}
			else
			{
				SetCursor(m_defaultCursor);
			}
		}

}

void CLayerView::OnLeftKey(void)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document;
	pDoc->OnLeftKey();
}

void CLayerView::OnRightKey(void)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document;
	pDoc->OnRightKey();
}

void CLayerView::OnUpKey(void)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document;
	pDoc->OnUpKey();
}

void CLayerView::OnDownKey(void)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document;
	pDoc->OnDownKey();
}

/*
void CLayerView::OnEnterKey(void)
{

		if ((xNum == 0) || (xNum==4))
		{
			pt.x = x;
			pt.y = y;
			ClientToScreen(hWnd, &pt);

//@@@??			HMENU submenu;
			int layer = yNum;

			int cmd;

			if (xNum==0)
			{
				cmd = m_pictureMenu->OpenMenu(pt);
				if (cmd == NNNEDITCOMMAND_DELETEPIC) pDoc->DeletePic(layer);
				if ((cmd >= NNNEDITCOMMAND_SELECTPIC) && (cmd < NNNEDITCOMMAND_SELECTPIC + m_picTypeKosuu)) pDoc->SelectPictureFile(layer,cmd - NNNEDITCOMMAND_SELECTPIC,hWnd);

//				submenu = GetSubMenu(m_popupMenu[0],0);
//				int cmd = TrackPopupMenu(submenu,TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,pt.x,pt.y,0,m_hWnd,NULL);
//				if ((cmd == ID_SELECTPIC_TA) || (cmd == ID_SELECTPIC_BG) || (cmd == ID_SELECTPIC_EV) || (cmd == ID_SELECTPIC_ETC)) SelectPic(layer,cmd);
//				if (cmd == ID_DELETEPIC) pDoc->DeletePic(layer);
			}

			if (xNum==4)
			{
				cmd = m_effectMenu->OpenMenu(pt);

				//submenu = GetSubMenu(m_popupMenu[1],0);
				//int cmd = TrackPopupMenu(submenu,TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,pt.x,pt.y,0,m_hWnd,NULL);
				if (cmd == NNNEDITCOMMAND_DELETEEFFECT) pDoc->ChangeEffect(layer,-1);
				if (cmd == NNNEDITCOMMAND_SAMEEFFECT) pDoc->ChangeEffect(layer,-2);	//same
				if (cmd >= NNNEDITCOMMAND_SELECTEFFECT) pDoc->ChangeEffect(layer,cmd-NNNEDITCOMMAND_SELECTEFFECT);

				
//				if ((cmd != 0) && (cmd != ID_DELETE_EFFECT) && (cmd != ID_SETEFFECT_EX)) pDoc->ChangeEffect(layer, CommandToEffect(cmd));
				//if (cmd == ID_DELETE_EFFECT) pDoc->ChangeEffect(layer,-1);
//				if (cmd == ID_SETEFFECT_EX) pDoc->ChangeEffect(layer, CommandToEffect(cmd));
			}
		}
*/

int CLayerView::OpenPopupLayerMenu(WPARAM wParam,LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	POINT pt;

	pt.x = x;
	pt.y = y;

	ClientToScreen(m_hWnd, &pt);
	return m_pictureMenu->OpenMenu(pt);
}

int CLayerView::OpenPopupEffectMenu(WPARAM wParam,LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	POINT pt;

	pt.x = x;
	pt.y = y;

	ClientToScreen(m_hWnd, &pt);
	return m_effectMenu->OpenMenu(pt);
}

BOOL CLayerView::MoveMouse(int x,int y,POINT screenPos)
{
	POINT pt;
	pt.x = x + m_windowX;
	pt.y = y + m_windowY; 



	int subType = -1;
	int type = LAYER_WINDOW;

	if (m_buttonOpen != NULL)
	{
		if (m_buttonOpen->CheckOn(x,y)) subType = 0;
	}
	if (m_buttonSave != NULL)
	{
		if (m_buttonSave->CheckOn(x,y)) subType = 1;
	}
	if (m_buttonCopyPreEffect != NULL)
	{
		if (m_buttonCopyPreEffect->CheckOn(x,y)) subType = 2;
	}
	if (m_buttonCopyPreEffect2 != NULL)
	{
		if (m_buttonCopyPreEffect2->CheckOn(x,y)) subType = 3;
	}

	if (subType == -1)
	{
		type = -1;
	}

	m_document->GetApp()->OnBalloonArea(type,pt,subType,screenPos);

//	OutputDebugString("*moveMouse layer*\x00f\x00a");
	return FALSE;
}


BOOL CLayerView::OnControlKey(WPARAM wParam,LPARAM lParam)
{
	CLayerDoc* pDoc = (CLayerDoc*)m_document;
	if (wParam == 6)
	{
		//F
//		pDoc->OnSearch();
//		return TRUE;
	}
	if (wParam == 19)
	{
		//S
		pDoc->OnSaveEffect();
		return TRUE;
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
//		pDoc->OnNewCommand();
//		return TRUE;
	}
	if (wParam == 15)
	{
		pDoc->OnOpenEffect();
		return TRUE;
		//O
	}
	if (wParam == 26)
	{
		return pDoc->OnUndo(); 
	}

	return FALSE;

}

/*_*/



