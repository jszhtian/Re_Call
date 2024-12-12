//
// zahyodoc.cpp
//


#include <windows.h>
#include <stdio.h>

#include "windowlist.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectList.h"
#include "case.h"

#include "zahyoview.h"

#include "zahyodoc.h"

#include "myapplicationBase.h"

#include "komadata.h"

CZahyoDoc::CZahyoDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = ZAHYO_WINDOW;
	m_screenSize.cx = m_app->GetConfig("mainScreenWindowSizeX1");
	m_screenSize.cy = m_app->GetConfig("mainScreenWindowSizeY1");


	m_view = new CZahyoView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

//	m_view->SetViewWindowText("À•W");
//	m_view->MoveViewWindow(1031,23,240,240);

	m_switchKosuu = 10;
	m_nowSelectSwitch = -1;
	for (int i=0;i<m_switchKosuu;i++)
	{
		m_switchFlag[i] = FALSE;
	}
	m_switchFlag[5] = TRUE;
	m_switchFlag[6] = TRUE;
	m_switchFlag[7] = TRUE;

	for (int i=0;i<7;i++)
	{
		char mes[256];
		sprintf_s(mes,256,"presetX_%d_%d",0,i);
		m_preset[i].x = m_app->GetConfig(mes);
		sprintf_s(mes,256,"presetY_%d_%d",0,i);
		m_preset[i].y = m_app->GetConfig(mes);
	}

	SetRect(&m_srcFromRect,0,0,640,480);
	SetRect(&m_srcToRect,0,0,640,480);
	SetRect(&m_dstFromRect,0,0,640,480);
	SetRect(&m_dstToRect,0,0,640,480);

//	m_view->Show();
}



CZahyoDoc::~CZahyoDoc()
{
	End();
}

void CZahyoDoc::End(void)
{
}

void CZahyoDoc::GetZahyoRect(int n, RECT* lpRect)
{
	CKomaData* pKoma = GetNowSelectKoma();
	int layer = GetNowSelectLayer();
	if ((pKoma == NULL) || (layer == -1))
	{
		SetRect(lpRect,0,0,0,0);
		return;
	}

	switch (n)
	{
	case 0:
		pKoma->GetEffectRect(lpRect,layer,4);
//		*lpRect = m_srcFromRect;
		break;
	case 1:
		pKoma->GetEffectRect(lpRect,layer,5);
//		*lpRect = m_srcToRect;
		break;
	case 2:
		pKoma->GetEffectRect(lpRect,layer,1);
//		*lpRect = m_dstFromRect;
		break;
	case 3:
		pKoma->GetEffectRect(lpRect,layer,2);
//		*lpRect = m_dstToRect;
		break;
	}
}

void CZahyoDoc::SetZahyoRect(int n, RECT* lpRect)
{
	switch (n)
	{
	case 0:
		m_srcFromRect = *lpRect;
		break;
	case 1:
		m_srcToRect = *lpRect;
		break;
	case 2:
		m_dstFromRect = *lpRect;
		break;
	case 3:
		m_dstToRect = *lpRect;
		break;
	}
	m_app->SetModify();
}

int CZahyoDoc::GetNowSelectLayer(void)
{
	return m_app->GetNowSelectPictureLayer();
}


BOOL CZahyoDoc::CheckSwitchFlag(int n)
{
	if ((n<0) || (n>=m_switchKosuu)) return FALSE;

	return m_switchFlag[n];
}

void CZahyoDoc::EnableSwitch(int n, BOOL flg)
{
	if ((n<0) || (n>=m_switchKosuu)) return;
	m_switchFlag[n] = flg;
}

int CZahyoDoc::GetSelectSwitch(void)
{
	return m_nowSelectSwitch;
}

void CZahyoDoc::SetSelectSwitch(int n)
{
	m_nowSelectSwitch = n;
}


int CZahyoDoc::GetSwitchKosuu(void)
{
	return m_switchKosuu;
}

void CZahyoDoc::OnClickSwitch(int n)
{
	if ((n<0) || (n>=m_switchKosuu)) return;
	if (CheckSwitchFlag(n) == FALSE) return;


	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma != NULL)
	{
		int layer = GetNowSelectLayer();
		if (layer != -1)
		{
			int eff = pKoma->GetEffect(layer);
			if (eff == EFFECT_NOP)
			{
				if (m_nowSelectSwitch == -1)
				{
					n = 5;
				}
				else
				{
					n = m_nowSelectSwitch;
				}
			}
		}
	}
	//


	if (m_nowSelectSwitch == n)
	{
		m_nowSelectSwitch = -1;
	}
	else
	{
		m_nowSelectSwitch = n;
	}
	m_view->MyInvalidateRect();
}

void CZahyoDoc::OnLeftKey(void)
{
}

void CZahyoDoc::OnRightKey(void)
{
}

void CZahyoDoc::OnUpKey(void)
{
}

void CZahyoDoc::OnDownKey(void)
{
}

void CZahyoDoc::OnSpaceKey(void)
{
	if (m_nowSelectSwitch == -1) return;

	m_app->ChangeWindowIfCan(MAINSCREEN_WINDOW);
}

void CZahyoDoc::OnEscapeKey(void)
{
	m_app->ChangeWindowIfCan(LAYER_WINDOW);
}

void CZahyoDoc::OnEnterKey(void)
{
}

int CZahyoDoc::GetPresetButtonEnable(void)
{
	return m_app->GetPresetButtonEnable();
}


POINT CZahyoDoc::GetPresetZahyo(int n)
{
	POINT pt;
	pt.x = 0;
	pt.y = 0;
	if ((n>=0) && (n<7))
	{
		pt = m_preset[n];
	}
	return pt;
}

void CZahyoDoc::SetPresetZahyo(int n,POINT pt)
{
	if ((n>=0) && (n<7))
	{
		m_preset[n] = pt;

		char mes[256];
		sprintf_s(mes,256,"presetX_%d_%d",0,n);
		m_app->SetConfig(mes,pt.x);
		sprintf_s(mes,256,"presetY_%d_%d",0,n);
		m_app->SetConfig(mes,pt.y);
	}
}

void CZahyoDoc::UpdateMainWindow(void)
{
	m_app->UpdateMainScreen();
}

/*_*/

