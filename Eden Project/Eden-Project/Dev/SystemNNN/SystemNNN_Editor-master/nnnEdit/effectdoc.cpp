//
// effectdoc.cpp
//

#include <windows.h>
#include <stdio.h>
#include "windowlist.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "case.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "effectParam.h"

#include "effectview.h"

#include "effectdoc.h"

#include "myapplicationBase.h"

#include "myinputdialog.h"

#include "komadata.h"

#include "effectParam.h"


CEffectDoc::CEffectDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = EFFECT_WINDOW;

	m_hBitmapEffectMini = m_app->GetCommonBitmap(0);
	m_hBitmapFont = m_app->GetCommonBitmap(1);

	m_editMode = 0;

	m_view = new CEffectView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

	m_inputDialog = new CMyInputDialog(m_app->GetFrameHWND(), m_hInstance);

//	m_view->SetViewWindowText("エフェクト");
//	m_view->MoveViewWindow(769+24,23,256-24,24*16);
}






CEffectDoc::~CEffectDoc()
{
	End();
}

void CEffectDoc::End(void)
{
	ENDDELETECLASS(m_inputDialog);
}

//void CEffectDoc::OnCloseButton(void)
//{
//
//	OutputDebugString("OnCLOSEButton() in DocDoc\n");	//override test
//}

HBITMAP CEffectDoc::GetEffectMini(void)
{
	return m_hBitmapEffectMini;
}

HBITMAP CEffectDoc::GetMyFont(void)
{
	return m_hBitmapFont;
}

int CEffectDoc::GetEditMode(void)
{
	return 1;

	return m_editMode;
}

void CEffectDoc::ClickSelButton(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;
//	int layer = GetNowSelectLayer();
	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return;

	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	CEffectParam* effectParam = m_app->GetEffectParam(eff);

	int kosuu = pKoma->GetEffectParaKosuu(layer);
//	int kosuuMin = CEffectParaList::m_para[eff].paraKosuuMin;
//	int kosuuMax = CEffectParaList::m_para[eff].paraKosuuMax;
	int kosuuMin = effectParam->GetNeedParaKosuu();
	int kosuuMax = effectParam->GetParaKosuu();

	if (n>=kosuuMax) return;
	if (n<kosuuMin) return;

	m_app->CheckAndGetKomaUndo();

	if (n>=kosuu)
	{
		//new kosuu?
		pKoma->SetEffectParaKosuu(layer,n+1);
		for (int i=kosuu;i<=n;i++)
		{
			pKoma->InitEffectDefaultPara(layer,i);
		}
	}
	else
	{
		BOOL delok = TRUE;

		if (m_app->GetWarningDelParam())
		{
			delok = FALSE;
			int rt00 = MessageBox(m_app->GetFrameHWND(),"減らしますか?","確認",MB_OKCANCEL | MB_APPLMODAL);
			if (rt00 == IDOK) delok = TRUE;
		}

		if (delok)
		{
			//kesu
			pKoma->SetEffectParaKosuu(layer,n);
		}
	}
	m_view->MyInvalidateRect();
	m_app->UpdateMainScreen();
}

void CEffectDoc::ClickDeltaButton0(int delta,BOOL wheelFlag)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return;

	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	int kosuu = pKoma->GetEffectParaKosuu(layer);
	int n = pKoma->GetSelectParam(layer);
	if ((n<0) || (n>=kosuu)) return;

	m_app->CheckAndGetKomaUndo();

	if ((wheelFlag == FALSE) && (delta == 0))
	{
		CenterRoutine(pKoma,layer,n);
	}
	else
	{
		DeltaRoutine(pKoma,layer,n,delta,wheelFlag);
	}
}


void CEffectDoc::CenterRoutine(CKomaData* pKoma,int layer,int n)
{
	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	CEffectParam* effectParam = m_app->GetEffectParam(eff);

//	int dataMin = effectParam->GetParaMin(n);
//	int dataMax = effectParam->GetParaMax(n);

//	int d = (dataMin+dataMax) / 2;

	int d = effectParam->GetParaDefault(n);

	int old = pKoma->GetEffectPara(layer,n);

	if (d == old) return;

	pKoma->SetEffectPara(layer,n,d);

	m_view->MyInvalidateRect();
	m_app->UpdateMainScreen();
	m_app->SetModify();
}

void CEffectDoc::DeltaRoutine(CKomaData* pKoma,int layer,int n,int delta,BOOL wheelFlag)
{
	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	CEffectParam* effectParam = m_app->GetEffectParam(eff);

//	int dataMin = CEffectParaList::m_para[eff].effectParaData[n].paraMin;
//	int dataMax = CEffectParaList::m_para[eff].effectParaData[n].paraMax;
	int dataMin = effectParam->GetParaMin(n);
	int dataMax = effectParam->GetParaMax(n);

	if (wheelFlag)
	{
		int delta2 = (dataMax - dataMin) / 20;
		if (delta2<1) delta2 = 1;
		delta *= delta2;
	}


	int old = pKoma->GetEffectPara(layer,n);

	int ans = old + delta;
	if (ans<dataMin) ans = dataMin;
	if (ans>dataMax) ans = dataMax;

	if (ans == old) return;

	pKoma->SetEffectPara(layer,n,ans);

	m_view->MyInvalidateRect();
	m_app->UpdateMainScreen();
	m_app->SetModify();
}


void CEffectDoc::ClickDeltaButton(int n, int delta,BOOL wheelFlag)
{
	if ((n<0) || (n>255)) return;

	m_app->CheckAndGetKomaUndo();

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;
//	int layer = GetNowSelectLayer();
	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return;

	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	int kosuu = pKoma->GetEffectParaKosuu(layer);
	if (n>=kosuu) return;

	DeltaRoutine(pKoma,layer,n,delta,wheelFlag);
}



int CEffectDoc::GetNowSelectEffectLayer(void)
{
	return m_app->GetNowSelectEffectLayer();
}

CEffectParam* CEffectDoc::GetEffectParam(int n)
{
	return m_app->GetEffectParam(n);
}

int CEffectDoc::GetNowSelectParam(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return -1;

	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return -1;

	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return -1;

	return pKoma->GetNowSelectNumber();
}



void CEffectDoc::ChangeSelectParam(int selectParam)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return;

	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	m_app->CheckAndGetKomaUndo();

	int kosuu = pKoma->GetEffectParaKosuu(layer);
	
	if ((selectParam >= 0) && (selectParam < kosuu))
	{
		pKoma->SetSelectParam(layer,selectParam);

		//ウィンドウ書き直し要求
		m_app->UpdateMyWindow(EFFECT_WINDOW);
		m_app->UpdateMainScreen();
	}
}

void CEffectDoc::OnEnterKey(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return;

	int eff = pKoma->GetEffect(layer);
	if (eff == -1) return;

	int kosuu = pKoma->GetEffectParaKosuu(layer);
	int n = pKoma->GetSelectParam(layer);
	if ((n < 0) || (n >= kosuu)) return;


	m_app->CheckAndGetKomaUndo();

	CEffectParam* effectParam = m_app->GetEffectParam(eff);

	//	int dataMin = CEffectParaList::m_para[eff].effectParaData[n].paraMin;
	//	int dataMax = CEffectParaList::m_para[eff].effectParaData[n].paraMax;
	int dataMin = effectParam->GetParaMin(n);
	int dataMax = effectParam->GetParaMax(n);


	int old = pKoma->GetEffectPara(layer, n);

	int newData = old;
	if (m_inputDialog->GetNumber(old, &newData, "エフェクトパラメーター"))
	{
		int ans = newData;
		if (ans < dataMin) ans = dataMin;
		if (ans > dataMax) ans = dataMax;

		if (ans == old) return;

		pKoma->SetEffectPara(layer, n, ans);

		m_view->MyInvalidateRect();
		m_app->UpdateMainScreen();
		m_app->SetModify();
	}
}


void CEffectDoc::OnLeftKey(void)
{
	m_app->CheckAndGetKomaUndo();

	int delta = -1;
	if (GetAsyncKeyState(VK_CONTROL)) delta *= 10;
	if (GetAsyncKeyState(VK_SHIFT)) delta *= 100;
	ClickDeltaButton0(delta);
}

void CEffectDoc::OnRightKey(void)
{
	m_app->CheckAndGetKomaUndo();

	int delta = 1;
	if (GetAsyncKeyState(VK_CONTROL)) delta *= 10;
	if (GetAsyncKeyState(VK_SHIFT)) delta *= 100;
	ClickDeltaButton0(delta);
}

void CEffectDoc::OnUpKey(void)
{
	OnUpDownKey(-1);
}

void CEffectDoc::OnDownKey(void)
{
	OnUpDownKey(1);
}

void CEffectDoc::OnUpDownKey(int delta)
{
	m_app->CheckAndGetKomaUndo();

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = m_app->GetNowSelectEffectLayer();
	if (layer == -1) return;

	int para = pKoma->GetSelectParam(layer);
	if (para == -1) para = 0;

	para += delta;
	int paraKosuu = pKoma->GetEffectParaKosuu(layer);
	if (paraKosuu <= 0) return;

	if (para >= paraKosuu) para = paraKosuu-1;
	ChangeSelectParam(para);
}


void CEffectDoc::OnEscapeKey(void)
{
	m_app->ChangeWindowIfCan(LAYER_WINDOW);
}




/*_*/

