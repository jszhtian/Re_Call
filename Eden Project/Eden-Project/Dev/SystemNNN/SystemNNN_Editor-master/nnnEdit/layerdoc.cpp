//
// layerdoc.cpp
//

#include <windows.h>
#include <stdio.h>


#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\myGraphics.h"

#include "windowlist.h"

#include "case.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"
//

#include "myfileopen.h"


#include "messageData.h"


#include "myapplicationBase.h"
#include "myapplication.h"

#include "komadata.h"
#include "filmdata.h"

#include "layerview.h"

#include "myDocument.h"
#include "layerdoc.h"

#include "myinputdialog.h"
#include "selectdialog.h"

#include "undoMemoryObject.h"

#include "..\..\systemNNN\nyanLib\include\picture.h"

#define NNNEDITCOMMAND_DELETEPIC 0
#define NNNEDITCOMMAND_SELECTPIC 1

#define NNNEDITCOMMAND_SAMEEFFECT 0
#define NNNEDITCOMMAND_DELETEEFFECT 1
#define NNNEDITCOMMAND_SELECTEFFECT 2


#define NNNEDITCOMMAND_LOADEFFECT 0
#define NNNEDITCOMMAND_SAVEEFFECT 1

CLayerDoc::CLayerDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_picTypeList = new CNameList();
	m_picTypeList->LoadFile("nnndir\\setup\\list\\picTypeList.txt");
	m_picTypeKosuu = m_picTypeList->GetNameKosuu() / 2;


	m_windowNumber = LAYER_WINDOW;
	m_view = new CLayerView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();



//	m_view->SetViewWindowText("レイヤー");
//	m_view->MoveViewWindow(651,23,24+32+24+24+32,24*16+24);

	for (int i=0;i<32;i++)
	{
		m_writeStatus[i] = 0;
		m_eyeStatus[i] = 1;
	}

//	m_selectPictureLayer = -1;
//	m_selectEffectLayer = -1;

	m_file = new CMyFileOpen(m_app->GetFrameHWND(),m_app->GetMyInstance(),"eff","*.eff");
	m_file2 = new CMyFileOpen(m_app->GetFrameHWND(),m_app->GetMyInstance(),"lef","*.lef");


	m_copyPreEffectMode = m_app->GetConfig("layerCopyEffectMode");

	m_inputDialog = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance);
	m_selectDialog = new CSelectDialog(m_app->GetFrameHWND(),m_hInstance);

//	m_writeLayer = 2;
//	m_writeStatus[m_writeLayer] = 1;
}



CLayerDoc::~CLayerDoc()
{
	End();
}

void CLayerDoc::End(void)
{
	ENDDELETECLASS(m_selectDialog);
	ENDDELETECLASS(m_inputDialog);

	ENDDELETECLASS(m_file2);
	ENDDELETECLASS(m_file);
	ENDDELETECLASS(m_picTypeList);


//OutputDebugString("CLayerDoc::End()");
}

//void CLayerDoc::OnCloseButton(void)
//{
//
//	OutputDebugString("OnCLOSEButton() in LayerDoc\n");	//override test
//}


void CLayerDoc::OnClickPictureButton(int layer)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	int selectPictureLayer = pKoma->GetSelectLayer();

	if (selectPictureLayer == layer)
	{
		pKoma->SetSelectLayer(-1);
	}
	else
	{
		pKoma->SetSelectLayer(layer);
//		m_selectPictureLayer = layer;
	}
//	m_selectEffectLayer = -1;
	pKoma->SetSelectEffectLayer(-1);

	m_app->LayerIsChanged();
	m_app->SetModify();
	//m_app->
}


void CLayerDoc::OnClickEffectButton(int layer)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	int selectEffectLayer = pKoma->GetSelectEffectLayer();

	if (selectEffectLayer == layer)
	{
		pKoma->SetSelectEffectLayer(-1);
		//m_selectEffectLayer = -1;
	}
	else
	{
		pKoma->SetSelectEffectLayer(layer);
//		m_selectEffectLayer = layer;
	}
//	m_selectPictureLayer = -1;
	pKoma->SetSelectLayer(-1);

	m_app->LayerIsChanged();
	m_app->SetModify();

}




int CLayerDoc::GetSelectPictureLayer(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return -1;
	return pKoma->GetSelectLayer();
}

int CLayerDoc::GetSelectEffectLayer(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return -1;
	return pKoma->GetSelectEffectLayer();
//	return m_selectEffectLayer;
}


/*
void CLayerDoc::ClickWriteButton(int layer)
{
//	if (GetWriteStatus(layer) == 0)
	if (1)
	{
		SetWriteStatus(m_writeLayer,0);
		m_writeLayer = layer;
		SetWriteStatus(m_writeLayer,1);
		if (GetEyeStatus(m_writeLayer) == 0)
		{
			SetEyeStatus(m_writeLayer,1);
		}
	}
}
*/




void CLayerDoc::ClickEyeButton(int layer)
{
	CheckAndGetUndo();

//	if (GetWriteStatus(layer) == 0)
	if (1)
	{
		if (GetEyeStatus(layer) == 0)
		{
			SetEyeStatus(layer,1);
		}
		else
		{
			SetEyeStatus(layer,0);
		}
	}
	m_app->LayerIsChanged();
	m_app->SetModify();
}


/*
int CLayerDoc::GetWriteStatus(int layer)
{
	if ((layer<0) || (layer>=LAYER_KOSUU_MAX)) return 0;
	return m_writeStatus[layer];
}
*/


int CLayerDoc::GetEyeStatus(int layer)
{
	if ((layer<0) || (layer>=CKomaData::m_layerMax)) return 0;
	return m_eyeStatus[layer];
}


void CLayerDoc::SetWriteStatus(int layer,int st)
{
	m_writeStatus[layer] = st;
	((CLayerView*)m_view)->UpdateWriteButton(layer);
}

void CLayerDoc::SetEyeStatus(int layer,int st)
{
	m_eyeStatus[layer] = st;
	((CLayerView*)m_view)->UpdateEyeButton(layer);

	m_app->SetEyeStatus(layer,st);
}


void CLayerDoc::ClearMiniPic(int n)
{
	CLayerView* pView = (CLayerView*)m_view;
	pView->ClearMiniPic(n);
}

void CLayerDoc::PictureToMini(int n,int* src, int x, int y)
{
	CLayerView* pView = (CLayerView*)m_view;
	pView->PictureToMini(n,src,x,y);
}


void CLayerDoc::DeletePic(int layer)
{
	CheckAndGetUndo();

	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int komaStart = pFilm->GetSelectStart();
	int komaEnd = pFilm->GetSelectEnd();
	for (int i=komaStart;i<=komaEnd;i++)
	{
		CKomaData* pKoma2 = pFilm->GetKoma(i);
		if (pKoma2 != NULL)
		{
			pKoma2->DeleteEffectPic(layer);
			int eff = pKoma2->GetEffect(layer);
			if ((eff == -1) || (eff == 0) || (0))	//EFFECT_MOVEONLY
			{
				pKoma2->SetEffectFlag(layer,FALSE);
			}
			pKoma2->SetAllEffect();
		}
	}

	pKoma->SetModifyFlag();
	m_app->LayerIsChanged();
	m_app->SetModify();
}




void CLayerDoc::ChangePictureName(int layer, LPSTR name, CKomaData* pKoma,BOOL sameZahyoFlag)
{
	if (pKoma == NULL) pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	pKoma->SetPicFileName(layer,name,sameZahyoFlag);

	//縮小絵をつくりなおすか???

	int zahyoPrintType = ((CMyApplication*)m_app)->GetZahyoPrintType();


	CEffect* effect = m_app->GetEffect();


	CPicture* lpPic = effect->GetPicture(layer);
	if (lpPic != NULL)
	{
		BOOL charaFlag = FALSE;

		char filename[256];
		char filename2[256];
		char tagName[3];
		tagName[0] = *name;
		tagName[1] = *(name+1);
		tagName[2] = 0;

		if (_stricmp(tagName,"ta") == 0)
		{
			int ln = strlen(name);
			if ((*(name+ln-1)) == 'm')
			{
//				OutputDebugString("Delete m");
				*(name+ln-1) = 0;
				pKoma->SetPicFileName(layer,name);
			}

			wsprintf(filename,"ta\\%s",name);
			wsprintf(filename2,"ta\\%s",name);
			charaFlag = TRUE;
		}
		else if (_stricmp(tagName,"bg") == 0)
		{
			wsprintf(filename,"bg\\%s",name);
		}
		else if (_stricmp(tagName,"ev") == 0)
		{
			wsprintf(filename,"ev\\%s",name);
		}
		else
		{
			wsprintf(filename,"etc\\%s",name);
		}

//		if (lpPic->LoadPicture(filename))
		if (lpPic->LoadDWQ(filename))
		{

			//get size
			RECT rc;
			lpPic->GetPicSize(&rc);
			if (sameZahyoFlag == FALSE)
			{
			
				
				
				
				pKoma->SetEffectRect(&rc,layer);

				if (zahyoPrintType)
				{
					int screenSizeX = CMyGraphics::GetScreenSizeX();
					int screenSizeY = CMyGraphics::GetScreenSizeY();
					int xxx = screenSizeX / 2 - rc.right / 2;
					int yyy = screenSizeY / 2 - rc.bottom / 2;
					rc.left = xxx;
					rc.top = yyy;

					pKoma->SetEffectRect(&rc,layer,7);
				}

			}
			else
			{
				int k = 1;
				for (int i=0;i<6;i++)
				{
					RECT rc2;
					pKoma->GetEffectRect(&rc2,layer,i);
					rc2.right = rc.right;
					rc2.bottom = rc.bottom;
					pKoma->SetEffectRect(&rc2,layer,k);
					k <<= 1;
				}
			}

//			if (charaFlag)
//			{
//				lpPic->CutData();
//				lpPic->LoadAntiAlias2(filename2);
//			}

			int* buf = lpPic->GetPictureBuffer();
			PictureToMini(layer,buf,rc.right,rc.bottom);
		}
		else
		{
MessageBox(m_app->GetFrameHWND(),filename,"File not found",MB_OK | MB_ICONINFORMATION );
			//load default pic
//			lpPic->
		}
	}

	pKoma->SetAllEffect();
	pKoma->SetModifyFlag();


	m_app->LayerIsChanged();
	m_app->SetModify();
}


BOOL CLayerDoc::CheckPicExist(int layer,CKomaData* pKoma)
{
	if (pKoma == NULL) pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return FALSE;
	LPSTR filename = pKoma->GetPicFileName(layer);
	if (filename == NULL) return FALSE;
	if ((*filename) == 0) return FALSE;

	return TRUE;
}


//int CLayerDoc::GetNowWriteLayer(void)
//{
	//return m_writeLayer;
//}


void CLayerDoc::ChangeEffect(int layer,int eff)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();


	int komaStart = pFilm->GetSelectStart();
	int komaEnd = pFilm->GetSelectEnd();

	for (int i=komaStart;i<=komaEnd;i++)
	{
		CKomaData* pKoma2 = (CKomaData*)(pFilm->GetObjectData(i));
		if (pKoma2 != NULL)
		{
			if (eff != -1)
			{
				pKoma2->SetEffect(layer,eff);
			}
			else
			{
				pKoma2->DeleteEffect(layer);
			}
			pKoma2->SetAllEffect();
		}
	}

	m_app->LayerIsChanged();
	m_app->KomaIsChanged();
	m_app->FilmIsChanged();
	m_app->SetModify();
}

void CLayerDoc::OnLoadLayerEffect(int layer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	FILE* file = m_file2->OpenLoad("*.lef","nnndir\\effect");
	if (file != NULL)
	{
		pKoma->LoadLayerEffect(layer,file);
		fclose(file);
		pKoma->SetModifyFlag();
		m_app->KomaIsChanged();
		m_app->SetModify();
	}
}

void CLayerDoc::OnSaveLayerEffect(int layer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	FILE* file = m_file2->OpenSave("*.lef","nnndir\\effect");
	if (file != NULL)
	{
		pKoma->SaveLayerEffect(layer,file);
		fclose(file);
	}

}



void CLayerDoc::OnOpenEffect(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

//	if (n == -1) n = m_selectEffectLayer;
//	if (n == -1) return;

//	pKoma->GetEffectPara

	FILE* file = m_file->OpenLoad("*.eff","nnndir\\effect");
	if (file != NULL)
	{
		pKoma->LoadEffect(file);
		fclose(file);
		pKoma->SetModifyFlag();
		m_app->KomaIsChanged();
		m_app->SetModify();
	}
}

void CLayerDoc::OnCopyPreEffect(int n)
{
	if (CheckCopyPreEffect())
	{
		CheckAndGetUndo();

		m_app->CopyPreEffect();
	}
}

void CLayerDoc::OnCopyPreEffect2(int n)
{
	if (CheckCopyPreEffect())
	{
		CheckAndGetUndo();

		m_app->CopyPreEffect(2);
	}
}

BOOL CLayerDoc::CheckCopyPreEffect(void)
{
	int md = m_app->GetConfig("layerCopyEffectMode");
	if (md == 0) return FALSE;
	if (md == 1) return TRUE;

	int rt = MessageBox(m_app->GetFrameHWND(),"エフェクトをコピーします。\nよろしいですか","確認",MB_OKCANCEL);
	if (rt == IDOK)
	{
		return TRUE;
	}

	return FALSE;

}

void CLayerDoc::OnSaveEffect(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

//	if (n == -1) n = m_selectEffectLayer;
//	if (n == -1) return;

//	if (pKoma->GetEffect(n) == -1) return;

	FILE* file = m_file->OpenSave("*.eff","nnndir\\effect");

	if (file != NULL)
	{
		pKoma->SaveEffect(file);
		fclose(file);
	}
}




void CLayerDoc::OnUndoEffect(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;
}


BOOL CLayerDoc::CheckKomaSelected(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma != NULL) return TRUE;
	return FALSE;
}

void CLayerDoc::SelectPictureFileByName(int layer,int cmd,HWND hwnd)
{
	if (CheckKomaSelected() == FALSE) return;

	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;


	LPSTR oldName = pKoma->GetPicFileName(layer);

	LPSTR filename = m_inputDialog->GetText(oldName);
	if (filename == NULL) return;


	BOOL b = m_file->GetSameZahyoFlag();

	int startKoma = pFilm->GetSelectStart();
	int endKoma = pFilm->GetSelectEnd();


	for (int i=startKoma;i<=endKoma;i++)
	{
		CKomaData* pKoma2 = (CKomaData*)(pFilm->GetObjectData(i));
		if (pKoma2 != NULL)
		{
			BOOL b2 = b;
			if (CheckPicExist(layer,pKoma2) == FALSE) b2 = FALSE;
			ChangePictureName(layer,filename,pKoma2,b2);
		}
	}

	int fileLength = m_app->GetConfig("dwqFilterLength");
	int newFilterLength = m_file->GetFilterLength();
	if (newFilterLength >= 0)
	{
		if (newFilterLength != fileLength)
		{
			m_app->SetConfig("dwqFilterLength",newFilterLength);
		}
	}

	m_app->LayerIsChanged();
	m_app->KomaIsChanged();
	m_app->FilmIsChanged();
	m_app->SetModify();
}


void CLayerDoc::SetVarControl(int layer,int cmd,HWND hwnd)
{
	if (CheckKomaSelected() == FALSE) return;

	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int nm = pKoma->GetVarControlLayer(layer);//dummy

	int v = nm;
	if (nm == -1)
	{
		v = 1;
	}
	else if (nm > 0)
	{
		v++;
	}

	int rt2 = m_selectDialog->GetSelect(m_app->GetVarControlList(),v,NULL,NULL);
	if (rt2 == 0)
	{
		pKoma->SetVarControlLayer(layer,0);//dummy

		m_app->LayerIsChanged();
		m_app->KomaIsChanged();
		m_app->FilmIsChanged();
		m_app->SetModify();

	}
	else if (rt2 == 1)
	{
		pKoma->SetVarControlLayer(layer,-1);//削除命令

		m_app->LayerIsChanged();
		m_app->KomaIsChanged();
		m_app->FilmIsChanged();
		m_app->SetModify();
	}
	else if (rt2 > 1)
	{
		int nm2 = nm;

		pKoma->SetVarControlLayer(layer,rt2-1);//dummy

		m_app->LayerIsChanged();
		m_app->KomaIsChanged();
		m_app->FilmIsChanged();
		m_app->SetModify();
	}
}

int CLayerDoc::GetVarControlLayerCheckPre(int layer)
{
	if (CheckKomaSelected() == FALSE) return 0;

	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;

	int vc = pKoma->GetVarControlLayer(layer);
	if (vc != 0) return vc;

	int k = pFilm->GetNowSelectNumber();
	for (int i=k-1;i>=0;i--)
	{
		pKoma = pFilm->GetKoma(i);
		if (pKoma != NULL)
		{
			vc = pKoma->GetVarControlLayer(layer);
			if (vc > 0) return vc;
			if (vc == -1) return 0;
		}
	}


	return 0;
}


void CLayerDoc::SelectPictureFile(int layer,int cmd,HWND hwnd)
{
	if (CheckKomaSelected() == FALSE) return;

	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));

	char fn[1024];
	char ft[256];
	char defname[256];
	LPSTR oldName = pKoma->GetPicFileName(layer);
	fn[0] = 0;
	if (oldName != NULL)
	{
		wsprintf(fn,"%s.dwq",oldName);
		wsprintf(defname,"%s.dwq",oldName);
	}
	else
	{
		wsprintf(defname,"*.dwq");
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "all(*.dwq)\0*.dwq\0";
	ofn.lpstrFile = fn;
	ofn.nMaxFile = 1024;

	ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLETEMPLATE;//OFN_FILEMUSTEXIST | OFN_READONLY;//OFN_HIDEREADONLY;
	ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DIALOG3);
	ofn.hInstance = m_hInstance;
//	ofn.lpfnHook = OFNHookProc;

	ofn.lpstrDefExt = "*.dwq";
	ofn.lpstrTitle = "絵の選択";

	ofn.lpstrFileTitle = ft;
	ofn.nMaxFileTitle = 256;

	if (cmd>=m_picTypeKosuu) cmd = m_picTypeKosuu-1;
	if (cmd<0) cmd = 0;


	LPSTR picTypeName = m_picTypeList->GetName(cmd*2);
	char dir[256];
	wsprintf(dir,"dwq\\%s",picTypeName);

	ofn.lpstrInitialDir = dir;



//	LPSTR filename = m_file->SelectFile("*.dwq",dir,"dwq");
	int fileLength = m_app->GetConfig("dwqFilterLength");
	LPSTR filename = m_file->SelectFile(defname,dir,"dwq",fileLength);
	if (filename != NULL)
	{
		BOOL b = m_file->GetSameZahyoFlag();

		int startKoma = pFilm->GetSelectStart();
		int endKoma = pFilm->GetSelectEnd();


		for (int i=startKoma;i<=endKoma;i++)
		{
			CKomaData* pKoma2 = (CKomaData*)(pFilm->GetObjectData(i));
			if (pKoma2 != NULL)
			{
				BOOL b2 = b;
				if (CheckPicExist(layer,pKoma2) == FALSE) b2 = FALSE;
				ChangePictureName(layer,filename,pKoma2,b2);
			}
		}

		int newFilterLength = m_file->GetFilterLength();
		if (newFilterLength >= 0)
		{
			if (newFilterLength != fileLength)
			{
				m_app->SetConfig("dwqFilterLength",newFilterLength);
			}
		}

		m_app->LayerIsChanged();
		m_app->KomaIsChanged();
		m_app->FilmIsChanged();
		m_app->SetModify();
	}

}


void CLayerDoc::MoveLayer(int fromLayer,int toLayer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	pKoma->MoveLayer(fromLayer,toLayer);

	pKoma->SetModifyFlag();
	m_app->KomaIsChanged();
	m_app->SetModify();
}

void CLayerDoc::CopyLayer(int fromLayer,int toLayer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	pKoma->CopyLayer(fromLayer,toLayer);

	pKoma->SetModifyFlag();
	m_app->KomaIsChanged();
	m_app->SetModify();
}

void CLayerDoc::ExchangeLayer(int fromLayer,int toLayer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	pKoma->ExchangeLayer(fromLayer,toLayer);

	pKoma->SetModifyFlag();
	m_app->KomaIsChanged();
	m_app->SetModify();
}

void CLayerDoc::MoveEffect(int fromLayer,int toLayer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	pKoma->MoveEffect(fromLayer,toLayer);

	pKoma->SetModifyFlag();
	m_app->KomaIsChanged();
	m_app->SetModify();
}

void CLayerDoc::CopyEffect(int fromLayer,int toLayer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	pKoma->CopyEffect(fromLayer,toLayer);

	pKoma->SetModifyFlag();
	m_app->KomaIsChanged();
	m_app->SetModify();
}

void CLayerDoc::ExchangeEffect(int fromLayer,int toLayer)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	pKoma->ExchangeEffect(fromLayer,toLayer);

	pKoma->SetModifyFlag();
	m_app->KomaIsChanged();
	m_app->SetModify();
}

void CLayerDoc::OnLeftKey(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	int selectPictureLayer = pKoma->GetSelectLayer();
	if (selectPictureLayer != -1) return;

	selectPictureLayer = pKoma->GetSelectEffectLayer();
	if (selectPictureLayer == -1)
	{
		selectPictureLayer = 0;
	}

	pKoma->SetSelectLayer(selectPictureLayer);
	pKoma->SetSelectEffectLayer(-1);

	m_app->LayerIsChanged();
	m_app->SetModify();
}

void CLayerDoc::OnRightKey(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	int selectEffectLayer = pKoma->GetSelectEffectLayer();
	if (selectEffectLayer != -1) return;

	selectEffectLayer = pKoma->GetSelectLayer();
	if (selectEffectLayer == -1)
	{
		selectEffectLayer = 0;
	}

	pKoma->SetSelectEffectLayer(selectEffectLayer);
	pKoma->SetSelectLayer(-1);

	m_app->LayerIsChanged();
	m_app->SetModify();
}

void CLayerDoc::OnUpKey(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	int layer = pKoma->GetSelectLayer();
	int effectLayer = -1;
	if (layer == -1)
	{
		effectLayer = pKoma->GetSelectEffectLayer();
		if (effectLayer == -1)
		{
			layer = 0;
		}
	}

	if (layer != -1)
	{
		layer--;
		if (layer<0) layer = 0;
	}
	if (effectLayer != -1)
	{
		effectLayer--;
		if (effectLayer<0) effectLayer = 0;
	}

	pKoma->SetSelectEffectLayer(effectLayer);
	pKoma->SetSelectLayer(layer);

	m_app->LayerIsChanged();
	m_app->SetModify();
}

void CLayerDoc::OnDownKey(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	CheckAndGetUndo();

	int layer = pKoma->GetSelectLayer();
	int effectLayer = -1;
	if (layer == -1)
	{
		effectLayer = pKoma->GetSelectEffectLayer();
		if (effectLayer == -1)
		{
			layer = 0;
		}
	}

	int layerMax = CKomaData::m_layerMax;

	if (layer != -1)
	{
		layer++;
		if (layer >= layerMax) layer = layerMax-1;
	}
	if (effectLayer != -1)
	{
		effectLayer++;
		if (effectLayer >= layerMax) effectLayer = layerMax-1;
	}

	pKoma->SetSelectEffectLayer(effectLayer);
	pKoma->SetSelectLayer(layer);

	m_app->LayerIsChanged();
	m_app->SetModify();
}


void CLayerDoc::OnSpaceKey(void)
{
	int layer = -1;
	int effectLayer = -1;

	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	layer = pKoma->GetSelectLayer();
	if (layer == -1)
	{
		effectLayer = pKoma->GetSelectEffectLayer();
		if (effectLayer == -1) return;
	}


	if (layer != -1)
	{
		m_app->ChangeWindowIfCan(ZAHYO_WINDOW);
		return;
	}

	if (effectLayer != -1)
	{
		m_app->ChangeWindowIfCan(EFFECT_WINDOW);
		return;
	}
}

void CLayerDoc::OnEnterKey(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	int layer = pKoma->GetSelectLayer();
	int effectLayer = -1;
	if (layer == -1)
	{
		effectLayer = pKoma->GetSelectEffectLayer();
		if (effectLayer == -1) return;
	}

	CheckAndGetUndo();

	CLayerView* pView = (CLayerView*)m_view;

	int x = 0;
	int y = 0;
	WPARAM wParam = 0;
	LPARAM lParam = (x & 0xffff) | ((y & 0xffff) << 16);


	if (layer != -1)
	{
		int cmd = pView->OpenPopupLayerMenu(wParam,lParam);

		if (cmd == NNNEDITCOMMAND_DELETEPIC) DeletePic(layer);
		if ((cmd >= NNNEDITCOMMAND_SELECTPIC) && (cmd < NNNEDITCOMMAND_SELECTPIC + m_picTypeKosuu)) SelectPictureFile(layer,cmd - NNNEDITCOMMAND_SELECTPIC,pView->GetHWND());
	}

	if (effectLayer != -1)
	{
		int cmd = pView->OpenPopupEffectMenu(wParam,lParam);

		if (cmd == NNNEDITCOMMAND_DELETEEFFECT) ChangeEffect(effectLayer,-1);
		if (cmd == NNNEDITCOMMAND_SAMEEFFECT) ChangeEffect(effectLayer,-2);	//same
		if (cmd >= NNNEDITCOMMAND_SELECTEFFECT) ChangeEffect(effectLayer,cmd-NNNEDITCOMMAND_SELECTEFFECT);
	}
}

int CLayerDoc::GetCopyPreEffectMode(void)
{
	return m_copyPreEffectMode;
}

void CLayerDoc::CheckAndGetUndo()
{
	m_app->CheckAndGetKomaUndo();


}

/*_*/



