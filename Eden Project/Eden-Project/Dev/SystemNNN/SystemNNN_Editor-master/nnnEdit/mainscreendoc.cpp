//
// mainscreendoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "windowlist.h"


#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "..\cycGamelib\scriptcommand.h"
#include "..\..\systemNNN\nyanEffectLib\effectList.h"
#include "..\..\systemNNN\nyanLib\include\picture.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"

#include "effectParam.h"

#include "..\..\systemNNN\nyanlib\include\myGraphics.h"
#include "case.h"

#include "..\..\systemNNN\nnnUtilLib\myMessage.h"

#include "messageData.h"
#include "komadata.h"

#include "mainscreenview.h"
#include "mainscreendoc.h"

#include "myapplicationBase.h"





CMainScreenDoc::CMainScreenDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = MAINSCREEN_WINDOW;

	m_effectMenuPrintMode = m_app->GetConfig("effectMenuPrintMode");
	m_effectBarPrintMode = m_app->GetConfig("effectBarPrintMode");
	m_effectMenuPrintUpDown = m_app->GetConfig("effectMenuUpDown");
	m_effectPrintLeftRight = m_app->GetConfig("effectLeftRight");
	m_effectParamNamePrintMode = m_app->GetConfig("effectNamePrintFlag");
	m_effectDescPrintMode = m_app->GetConfig("effectDescPrintFlag");
	m_effectParamDescPrintMode = m_app->GetConfig("effectParamDescPrintFlag");



	GetEditFrameSetup();

	m_view = new CMainScreenView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

//	m_view->SetViewWindowText("画面");
//	m_view->MoveViewWindow(5,23,640,480);
}


CMainScreenDoc::~CMainScreenDoc()
{
	End();
}

void CMainScreenDoc::End(void)
{
}

void CMainScreenDoc::PrintLayerHint(int btn)
{
	((CMainScreenView*)m_view)->PrintLayerHint(btn);
}

void CMainScreenDoc::PrintEffectHint(int effectLayer)
{
	((CMainScreenView*)m_view)->PrintEffectHint(effectLayer);
}

void CMainScreenDoc::PrintPlayModeIcon(int playMode,int storyMode)
{
	((CMainScreenView*)m_view)->PrintPlayIcon(playMode,storyMode);
}

void CMainScreenDoc::FlipToScreen(void)
{
	((CMainScreenView*)m_view)->FlipToScreen();
}

void CMainScreenDoc::Moved(WPARAM wParam, LPARAM lParam)
{
	((CMainScreenView*)m_view)->Moved(wParam,lParam);
}

void CMainScreenDoc::OnLButtonDown(int x, int y)
{
	if (m_app->GetPlayMode())
	{
		m_app->OnGameClick();
	}


//	m_app->dmy2(x,y);
}


void CMainScreenDoc::MoveToByCenter(int x, int y)
{

}


void CMainScreenDoc::AdjustPicture(int dx, int dy)
{
	if (m_app->GetPlayMode()) return;

	int sw = m_app->GetZahyoSwitch();
	if (sw == -1) return;


	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_app->GetNowSelectPictureLayer();
	if (layer == -1) return;

	int pic = pKoma->GetPictureNumber(layer);
	if (pic == -1) return;

	int effect = pKoma->GetEffect(layer);
	if (effect == -1) return;

	if (effect == EFFECT_NOP)
	{
		sw = 5;	//dst
	}
	if ((sw != 5) && (sw != 6) && (sw != 7)) return;



	RECT dstFrom;
	RECT dstTo;

	pKoma->GetEffectRect(&dstFrom,layer,1);
	pKoma->GetEffectRect(&dstTo,layer,2);


	int px = 0;
	int py = 0;


	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();


	int mask1 = 0x00000002;
	int mask2 = 0x00000004;
	switch (sw)
	{
	case 5:
		mask1 |= 0x00000001;
		mask2 |= 0x00000001;
		break;
	case 6:
		mask1 |= 0x00000001;
		break;
	case 7:
		mask2 |= 0x00000001;
		break;
	}

	if (mask1 & 0x00000001)
	{
		px = dstFrom.left;
		py = dstFrom.top;

		if (dx == -1) px = 0;
		if (dx == 1) px = screenSizeX-dstFrom.right;
		if (dy == -1) py = 0;
		if (dy == 1) py = screenSizeY-dstFrom.bottom;

		dstFrom.left = px;
		dstFrom.top = py;
	}

	if (mask2 & 0x00000001)
	{
		px = dstTo.left;
		py = dstTo.top;

		if (dx == -1) px = 0;
		if (dx == 1) px = screenSizeX-dstTo.right;
		if (dy == -1) py = 0;
		if (dy == 1) py = screenSizeY-dstTo.bottom;

		dstTo.left = px;
		dstTo.top = py;
	}

	pKoma->SetEffectRect(&dstFrom,layer,mask1);
	pKoma->SetEffectRect(&dstTo,layer,mask2);

	//data set


	m_app->SetNowKomaEffectRect(layer);

	if (mask1 & 0x00000001)
	{
		px = dstFrom.left;
		py = dstFrom.top;

		if (dx == -1) px = 0;
		if (dx == 1) px = screenSizeX-dstFrom.right;
		if (dy == -1) py = 0;
		if (dy == 1) py = screenSizeY-dstFrom.bottom;
		
		m_app->AdjustDstFrom(px,py);
	}

	if (mask2 & 0x00000001)
	{
		px = dstTo.left;
		py = dstTo.top;

		if (dx == -1) px = 0;
		if (dx == 1) px = screenSizeX-dstTo.right;
		if (dy == -1) py = 0;
		if (dy == 1) py = screenSizeY-dstTo.bottom;

		m_app->AdjustDstTo(px,py);
	}


//			m_app->MoveRectRoutine(layer,3,dx,dy);	//dummy
//			m_app->MoveDstTo(0,0);	//dummy

//						m_app->UpdateMyWindow(LAYER_WINDOW);
	m_app->UpdateMyWindow(ZAHYO_WINDOW);


	pKoma->SetModifyFlag();

//						m_app->dmy();
	m_app->SetNextPrint();
	m_app->SetModify();
}

void CMainScreenDoc::MovePicture(int dx, int dy)
{
	if (m_app->GetPlayMode()) return;

	int sw = m_app->GetZahyoSwitch();
	if (sw == -1) return;


	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_app->GetNowSelectPictureLayer();
	if (layer == -1) return;

	int pic = pKoma->GetPictureNumber(layer);
	if (pic == -1) return;

	int effect = pKoma->GetEffect(layer);
	if (effect == -1) return;

	if (effect == EFFECT_NOP)
	{
		sw = 5;	//dst
	}
	if ((sw != 5) && (sw != 6) && (sw != 7)) return;



	RECT dstFrom;
	RECT dstTo;

	pKoma->GetEffectRect(&dstFrom,layer,1);
	pKoma->GetEffectRect(&dstTo,layer,2);


	int mask1 = 0x00000002;
	int mask2 = 0x00000004;
	switch (sw)
	{
	case 5:
		mask1 |= 0x00000001;
		mask2 |= 0x00000001;
		break;
	case 6:
		mask1 |= 0x00000001;
		break;
	case 7:
		mask2 |= 0x00000001;
		break;
	}

	if (mask1 & 0x00000001)
	{
		dstFrom.left += dx;
		dstFrom.top += dy;
	}

	if (mask2 & 0x00000001)
	{
		dstTo.left += dx;
		dstTo.top += dy;
	}

	pKoma->SetEffectRect(&dstFrom,layer,mask1);
	pKoma->SetEffectRect(&dstTo,layer,mask2);

	//data set


	m_app->SetNowKomaEffectRect(layer);

	if (mask1 & 0x00000001)
	{
		m_app->MoveDstFrom(dx,dy);
	}

	if (mask2 & 0x00000001)
	{
		m_app->MoveDstTo(dx,dy);
	}

//			m_app->MoveRectRoutine(layer,3,dx,dy);	//dummy
//			m_app->MoveDstTo(0,0);	//dummy

//						m_app->UpdateMyWindow(LAYER_WINDOW);
	m_app->UpdateMyWindow(ZAHYO_WINDOW);
	m_app->SetModify();
	pKoma->SetModifyFlag();

//						m_app->dmy();
	m_app->SetNextPrint();
}


CEffectParam* CMainScreenDoc::GetEffectParam(int n)
{
	return m_app->GetEffectParam(n);
}

int CMainScreenDoc::GetNowEffect(void)
{
	return m_app->GetNowEffect();
}

BOOL CMainScreenDoc::GetPlayMode(void)
{
	return m_app->GetPlayMode();
}


void CMainScreenDoc::UpdateMainScreen(void)
{
	int cnt = GetEditFrameCount();
	m_app->UpdateMainScreen(cnt);
}

CMyMessage* CMainScreenDoc::GetMyMessage(void)
{
	return m_app->GetMyMessage();
}

int CMainScreenDoc::GetNowSelectParam(void)
{
	return m_app->GetNowSelectParam();
}


void CMainScreenDoc::PrintFrameTime(int nowFrame,int setFrame)
{
	((CMainScreenView*)m_view)->PrintFrameTime(nowFrame,setFrame);
}

void CMainScreenDoc::PrintShakin(int shakin,int skip,int totalSkip)
{
	((CMainScreenView*)m_view)->PrintShakin(shakin,skip,totalSkip);
}

void CMainScreenDoc::ChangeSelectParam(int selectParam)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return;

	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	int kosuu = pKoma->GetEffectParaKosuu(layer);
	
	if ((selectParam >= 0) && (selectParam < kosuu))
	{
		pKoma->SetSelectParam(layer,selectParam);
		m_app->SetModify();
		//ウィンドウ書き直し要求
		m_app->UpdateMyWindow(EFFECT_WINDOW);
	}
}


int CMainScreenDoc::GetEffectMenuPrintMode(void)
{
	return m_effectMenuPrintMode;
}

int CMainScreenDoc::GetEffectMenuPrintUpDown(void)
{
	return m_effectMenuPrintUpDown;
}

int CMainScreenDoc::GetEffectPrintLeftRight(void)
{
	return m_effectPrintLeftRight;
}

int CMainScreenDoc::GetEffectBarPrintMode(void)
{
	return m_effectBarPrintMode;
}

int CMainScreenDoc::GetEffectParamNamePrintMode(void)
{
	return m_effectParamNamePrintMode;
}

int CMainScreenDoc::GetEffectDescPrintMode(void)
{
	return m_effectDescPrintMode;
}

int CMainScreenDoc::GetEffectParamDescPrintMode(void)
{
	return m_effectParamDescPrintMode;
}

void CMainScreenDoc::OnClickEffectMenu(int n)
{
	m_app->CheckAndGetKomaUndo();

	if (n == 6)
	{
		m_effectMenuPrintMode = 1-m_effectMenuPrintMode;
	}
	else if (n == 0)
	{
		m_effectMenuPrintUpDown = 1-m_effectMenuPrintUpDown;
	}
	else if (n == 5)
	{
		m_effectPrintLeftRight = 1-m_effectPrintLeftRight;
	}
	else if (n == 1)
	{
		m_effectBarPrintMode = 1-m_effectBarPrintMode;
	}
	else if (n == 2)
	{
		m_effectParamNamePrintMode = 1-m_effectParamNamePrintMode;
	}
	else if (n == 3)
	{
		m_effectDescPrintMode = 1-m_effectDescPrintMode;
	}
	else if (n == 4)
	{
		m_effectParamDescPrintMode = 1-m_effectParamDescPrintMode;
	}

	m_app->SetConfig("effectMenuPrintMode",m_effectMenuPrintMode);
	m_app->SetConfig("effectBarPrintMode",m_effectBarPrintMode);
	m_app->SetConfig("effectMenuUpDown",m_effectMenuPrintUpDown);
	m_app->SetConfig("effectLeftRight",m_effectPrintLeftRight);
	m_app->SetConfig("effectNamePrintFlag",m_effectParamNamePrintMode);
	m_app->SetConfig("effectDescPrintFlag",m_effectDescPrintMode);
	m_app->SetConfig("effectParamDescPrintFlag",m_effectParamDescPrintMode);
}



void CMainScreenDoc::EditFrameIsChanged(void)
{
	CMainScreenView* pView = (CMainScreenView*)m_view;
	GetEditFrameSetup();
}


void CMainScreenDoc::GetEditFrameSetup(void)
{
	m_editFrameType = m_app->GetConfig("editFrameType");
	m_editFrame = m_app->GetConfig("editFrame");
}


int CMainScreenDoc::GetEditFrameCount(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;

	int kosuu = pKoma->GetObjectKosuu();
	if (kosuu<1) return 0;	//error!??

	int type = m_editFrameType;
	int frame = 1;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(0));

	if (pMessage->GetMessageMode() == 3)
	{
		frame = pMessage->GetDrawCount();
	}
	else
	{
		if (type == 0) return m_editFrame;

		int layer = pKoma->GetSelectEffectLayer();
		if (layer == -1) return 0;

		int effect = pKoma->GetEffect(layer);
		if (effect == -1) return 0;

		if (pKoma->GetEffectParaKosuu(layer) == 0) return 0;

		frame = pKoma->GetEffectPara(layer,0);
	}


	int n = m_editFrame;
	if (type==0)
	{
		if (n<0) n = 0;
		if (n>frame) n = frame;
		return n;
	}

	n *= (frame-1);
	n /= 100;

	if (n<0) n = 0;
	if (n>frame) n = frame;

	return n;

}


void CMainScreenDoc::OnClickEffectType(void)
{
	m_app->CheckAndGetKomaUndo();

	int d = GetEditFrameType();
	m_editFrameType = 1-d;
	m_app->SetConfig("editFrameType",m_editFrameType);
	int cnt = GetEditFrameCount();
	m_app->UpdateMainScreen(cnt);
	((CMainScreenView*)m_view)->FlipToScreen();
//	m_app->UpdateMyWindow(MAINSCREEN_WINDOW);
}

void CMainScreenDoc::OnClickEffectGaze(int d)
{
	m_app->CheckAndGetKomaUndo();

	if (d<0) d = 0;
	int type = GetEditFrameType();
	if (type != 0)
	{
		if (d>100) d = 100;
	}
	m_editFrame = d;
	m_app->SetConfig("editFrame",m_editFrame);
	int cnt = GetEditFrameCount();
	m_app->UpdateMainScreen(cnt);
	((CMainScreenView*)m_view)->FlipToScreen();

//	m_app->UpdateMyWindow(MAINSCREEN_WINDOW);
}

void CMainScreenDoc::OnUpDownEffectGaze(int d)
{
	int f = GetEditFrame();
	f += d;
	if (f < 0) f = 0;
	OnClickEffectGaze(f);
}

void CMainScreenDoc::OnDragEffectGaze(int d)
{
	int f = GetEditFrame();
	f += d;
	OnClickEffectGaze(f);
}


CPicture* CMainScreenDoc::GetMesNameWinPic(int n)
{
	if (n == 0) return m_app->GetMesWinPic();

	return m_app->GetNameWinPic();
}

void CMainScreenDoc::SetMessageSetup(CMessageSetup* lpMessageSetup)
{
	CMainScreenView* view = (CMainScreenView*)m_view;
	view->SetMessageSetup(lpMessageSetup);
}


//int CMainScreenDoc::GetActiveEffectParaKosuu(void)
//{
//	return m_app->GetActiveEffectParaKosuu();
//}
