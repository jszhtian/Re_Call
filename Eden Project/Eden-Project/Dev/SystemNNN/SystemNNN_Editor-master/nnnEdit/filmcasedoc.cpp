//
// filmcasedoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "windowlist.h"
#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\myFile.h"


#include "case.h"

#include "filmdata.h"
#include "filmcasedata.h"

#include "filmcaseview.h"
#include "filmcasedoc.h"

#include "myapplicationBase.h"

#include "myfileopen.h"

#include "myinputdialog.h"

#include "undoMemoryObject.h"

//#include "configparalist.h"


CFilmCaseDoc::CFilmCaseDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = FILMCASE_WINDOW;

//	m_nowSelectNumber = -1;

//	m_filmKosuu = 0;
//	m_filmKosuuMax = 0;
//	m_filmDataArray = NULL;

//	CreateFilmArray(256);
//	m_filmCaseData = new CFilmCaseData(m_app->GetEffect(),m_app);
	m_filmCaseData = new CFilmCaseData(m_app->GetEffect());

	m_view = new CFilmCaseView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

//	m_view->SetViewWindowText("フィルムケース");
//	m_view->MoveViewWindow(1031,287,GetSystemMetrics(SM_CXVSCROLL)+480,24*6);

	m_inputDialog = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance);

	m_file = new CMyFileOpen(m_app->GetFrameHWND(), m_app->GetMyInstance(),"flm");

	m_filmBufferFlag = FALSE;

}




CFilmCaseDoc::~CFilmCaseDoc()
{
	End();
}

void CFilmCaseDoc::End(void)
{
//	if (m_filmDataArray != NULL)
//	{
//		for (int i=0;i<m_filmKosuuMax;i++)
//		{
//			ENDDELETECLASS(m_filmDataArray[i]);
//		}
//	}
//	DELETEARRAY(m_filmDataArray);

	ENDDELETECLASS(m_filmCaseData);
	ENDDELETECLASS(m_inputDialog);
	ENDDELETECLASS(m_file);
}



//void CFilmCaseDoc::OnCloseButton(void)
//{
//
//}

void CFilmCaseDoc::OnNewFilm(int n)
{
	//nはつかわにゃい???

	int k;
	if (1)
	{
		int kosuu = m_filmCaseData->GetObjectKosuu();
		k = m_filmCaseData->GetNowSelectNumber();
		if ((k<0) || (k>=kosuu)) k = kosuu;
	}

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_FILM,UNDO_DATA_INSERT,k,k);
	}


	m_filmCaseData->CreateObjectData(k);

	CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(k));
	pFilm->Init(m_app->GetNewFilmName());
///	m_app->AddFilmVarName(pFilm->GetMyName());

	((CFilmCaseView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->FilmIsChanged();
}


void CFilmCaseDoc::OnOpenFilm(int n)
{
	n = m_filmCaseData->GetNowSelectNumber();
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;


	FILE* file = m_file->OpenLoad("nnndir\\film\\*.flm");
	if (file != NULL)
	{

		if (m_app->GetUndoMode())
		{
			CUndoMemoryObject* undo = m_app->GetUndoObject();
			undo->Clear(UNDO_TYPE_FILM,UNDO_DATA_INSERT,n,n);
		}

		if (m_filmCaseData->CreateObjectData(n))
		{
			CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
			if (pFilm != NULL)
			{
				pFilm->Load(file);
			}

			LPSTR newName = pFilm->GetMyName();

			//int k = m_app->SearchFilm(newName,n,n);
			int k = m_filmCaseData->SearchFilm(newName,n,n);
			if (k != -1)
			{
				LPSTR makedName = m_app->GetNewFilmName();
				char mes[128];
				wsprintf(mes,"%s_%s",makedName,newName);
				int ln = strlen(mes);
				if (ln>62) ln = 62;
				mes[ln] = 0;
				mes[ln+1] = 0;

				pFilm->SetMyName(mes);

				//config?
	//@@@			if (m_app->GetConfigData(CONFIGPARA_FILMNAMECHANGED_DIALOG))
				{
					MessageBox(m_app->GetFrameHWND(),"同じ名前が存在しています。\n名前を変更しましたにゃ","警告",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
				}
			}

			LPSTR varname = pFilm->GetMyName();
///			m_app->AddFilmVarName(varname);
		}

		fclose(file);


		((CFilmCaseView*)m_view)->ReCalcuScrollPara();
		m_app->SetModify();
		m_app->FilmIsChanged();
	}
}


void CFilmCaseDoc::OnSaveFilm(int n)
{
	n = m_filmCaseData->GetNowSelectNumber();
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	ClearUndo();

	FILE* file = m_file->OpenSave("nnndir\\film\\*.flm");
	if (file != NULL)
	{
		CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
		if (pFilm != NULL)
		{
			pFilm->Save(file);
		}

		fclose(file);

		m_app->FilmIsChanged();
	}
}


void CFilmCaseDoc::OnCut(int n)
{
	if (m_app->GetUndoMode())
	{
		int kosuu = GetFilmKosuu();
		if (n == -1) n = GetNowSelectNumber();
		if ((n<0) || (n>=kosuu)) return;
	
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_FILM,UNDO_DATA_DELETE,n,n);
		CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
		pFilm->Save(NULL,undo);
	}

	OnCopy(n,TRUE);
	OnDelete(n,TRUE);
}


void CFilmCaseDoc::OnCopy(int n,BOOL ignoreUndo)
{
	int kosuu = GetFilmKosuu();
	if (n == -1) n = GetNowSelectNumber();
	if ((n<0) || (n>=kosuu)) return;

	BOOL errorFlag = FALSE;

	FILE* file = CMyFile::Open("nnndir\\tmp\\film.flm","wb");
	if (file != NULL)
	{

		CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
		if (pFilm != NULL)
		{
			pFilm->Save(file);
		}

		fclose(file);

		m_app->FilmIsChanged();
	}
	else
	{
		errorFlag = TRUE;
	}

	m_filmBufferFlag = !errorFlag;
	m_app->SetModify();
}


void CFilmCaseDoc::OnPaste(int n)
{
	if (m_filmBufferFlag == FALSE) return;

	int kosuu = GetFilmKosuu();
	if (n == -1) n = GetNowSelectNumber();
	if ((n<0) || (n>kosuu)) return;



	FILE* file = CMyFile::Open("nnndir\\tmp\\film.flm","rb");
	if (file == NULL) return;


	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_FILM,UNDO_DATA_INSERT,n,n);
	}




	m_filmCaseData->CreateObjectData(n);

	CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
	if (pFilm != NULL)
	{
		pFilm->Load(file);
		fclose(file);

		LPSTR newName = pFilm->GetMyName();
		int k = m_filmCaseData->SearchFilm(newName,n,n);
		if (k != -1)
		{
			LPSTR makedName = m_app->GetNewFilmName();
			char mes[128];
			wsprintf(mes,"%s_%s",makedName,newName);
			int ln = strlen(mes);
			if (ln>62) ln = 62;
			mes[ln] = 0;
			mes[ln+1] = 0;

			pFilm->SetMyName(mes);

				//config?
	//@@@			if (m_app->GetConfigData(CONFIGPARA_FILMNAMECHANGED_DIALOG))
			if (1)
			{
				MessageBox(m_app->GetFrameHWND(),"同じ名前が存在しています。\n名前を変更しましたにゃ","警告",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
			}

			LPSTR varname = pFilm->GetMyName();
		}

	}
	else
	{
		MessageBox(NULL,"LOAD FILM ERROR","ERROR",MB_OK);
	}

	((CFilmCaseView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->FilmIsChanged();
}



void CFilmCaseDoc::OnDelete(int n,BOOL ignoreUndo)
{
	n = m_filmCaseData->GetNowSelectNumber();
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;


	if (m_app->GetWarningDelFilm())
	{
		int rt = MessageBox(m_frameHWND,"本当に削除しますか","削除の確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (rt != IDYES) return;
	}

	if (!ignoreUndo)
	{
		if (m_app->GetUndoMode())
		{
			CUndoMemoryObject* undo = m_app->GetUndoObject();
			undo->Clear(UNDO_TYPE_FILM,UNDO_DATA_DELETE,n,n);
			CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
			pFilm->Save(NULL,undo);
		}
	}



//varから名前の削除
	CFilmData* film = (CFilmData*)m_filmCaseData->GetObjectData(n);
///	LPSTR name = film->GetMyName();
///	m_app->DeleteFilmVarName(name);





	m_filmCaseData->DeleteObjectData(n);

	((CFilmCaseView*)m_view)->ReCalcuScrollPara();
	m_app->SetModify();
	m_app->FilmIsChanged();
}



void CFilmCaseDoc::OnSearch(int n)
{
	LPSTR name = m_inputDialog->GetText("検索するフィルム名前");
	if (name == NULL) return;

	int found = -1;
	int kosuu = m_filmCaseData->GetObjectKosuu();
	for (int i=0;i<kosuu;i++)
	{
		CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(i));
		if (pFilm != NULL)
		{
			LPSTR filmname = pFilm->GetMyName();
			if (strcmp(name,filmname) == 0)
			{
				found  = i;
				break;
			}
		}
	}

	if (found == -1)
	{
		MessageBox(m_app->GetFrameHWND(),"見つかりませんでしたにゃん",name,MB_OK);
		return;
	}

	m_filmCaseData->SetSelectNumber(found);
	m_app->FilmIsChanged();
//	UpdateMyWindow();
}


void CFilmCaseDoc::OnChangeColor(int n,int col)
{
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
	if (pFilm == NULL) return;

	CheckAndGetUndo(m_filmCaseData,n,n);

	pFilm->SetFilmColor(col);
	m_app->SetModify();
	m_app->FilmIsChanged();
}


int CFilmCaseDoc::GetFilmColor(int n)
{
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
	if (pFilm == NULL) return 0;

	return pFilm->GetFilmColor();
}


void CFilmCaseDoc::OnChangeName(int n)
{
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	//name change?
	CFilmData* pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n));
	if (pFilm == NULL) return;

	LPSTR newName = m_inputDialog->GetText(pFilm->GetMyName());

	if (newName != NULL)
	{
		if ((*newName) == 0)
		{
			MessageBox(m_app->GetFrameHWND(),"名前がありません","エラー",MB_OK | MB_APPLMODAL | MB_ICONSTOP);
			return;
		}

		if ((*newName) == '_')
		{
			MessageBox(m_app->GetFrameHWND(),"_で始まる名前は使えません","エラー",MB_OK | MB_APPLMODAL | MB_ICONSTOP);
			return;
		}

		//check same name!
//		int k = m_app->SearchFilm(newName,n,n);
		int k = m_filmCaseData->SearchFilm(newName,n,n);
////		int k2 = m_app->SearchFilmVarName(newName);


////		if ((k == -1) && (k2 == -1))
		if (k == -1)
		{
			//ok
///			m_app->DeleteFilmVarName(pFilm->GetMyName());
///			m_app->AddFilmVarName(newName);

			CheckAndGetUndo(m_filmCaseData,n,n);

			pFilm->SetMyName(newName);
		}
		else
		{
			MessageBox(m_app->GetFrameHWND(),"同じ名前が存在しています。","警告(ERROR)",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
			return;

			LPSTR makedName = m_app->GetNewFilmName();
			char mes[128];
			wsprintf(mes,"%s_%s",makedName,newName);
			int ln = strlen(mes);
			if (ln>62) ln = 62;
			mes[ln] = 0;
			mes[ln+1] = 0;

			pFilm->SetMyName(mes);

			//config?
//@@@			if (m_app->GetConfigData(CONFIGPARA_FILMNAMECHANGED_DIALOG))
			{
				MessageBox(m_app->GetFrameHWND(),"同じ名前が存在しています。\n名前を変更しましたにゃ","警告",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
			}
		}

//		m_view->MyInvalidateRect();
		m_app->SetModify();
		m_app->FilmIsChanged();
	}
}


void CFilmCaseDoc::OnSelectFilm(int n)
{
//	if (n<-1) n = -1;
//	if (n>m_obehctKosuu) n = m_objectKosuu;

	int old = m_filmCaseData->GetNowSelectNumber();
	if (n == old) return;

	ClearUndo();

	m_filmCaseData->SetSelectNumber(n);
	m_app->FilmIsChanged();
}

void CFilmCaseDoc::OnNoClearEffect(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	int n = m_filmCaseData->GetNowSelectNumber();
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if ((n>=0) || (n<kosuu))
	{
		CheckAndGetUndo(m_filmCaseData,n,n);
	}

	BOOL no = pFilm->GetNoClearEffect();
	if (no)
	{
		no = FALSE;
	}
	else
	{
		no = TRUE;
	}
	pFilm->SetNoClearEffect(no);

	m_app->SetModify();
	UpdateMyWindow();
}

void CFilmCaseDoc::OnSpecialType(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	int typeTime = pFilm->GetFilmSpecialTypeTime();
	int newData = typeTime;
	if (m_inputDialog->GetNumber(typeTime,&newData,"TypeTime -1:終了 0:なし 1-:設定値"))
	{
		if (newData != typeTime)
		{
			CheckAndGetUndo(m_filmCaseData,n,n);

			typeTime = newData;
			pFilm->SetFilmSpecialTypeTime(typeTime);

			m_app->SetModify();
			UpdateMyWindow();
		}
	}
}

void CFilmCaseDoc::OnConfigMask(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	int configMask = pFilm->GetConfigMask();
	int newData = configMask;
	if (m_inputDialog->GetNumber(configMask,&newData,"CONFIGマスク用の値,-1でマスククリアー"))
	{
		if (newData != configMask)
		{
			CheckAndGetUndo(m_filmCaseData,n,n);

			pFilm->SetConfigMask(newData);
	
			m_app->SetModify();
			UpdateMyWindow();
		}
	}
}

void CFilmCaseDoc::OnTaikenLevel(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	CheckAndGetUndo(m_filmCaseData,n,n);

	int taikenLevel = pFilm->GetTaikenLevel();
	taikenLevel++;
	taikenLevel %= 2;
	pFilm->SetTaikenLevel(taikenLevel);
	m_app->SetModify();
	UpdateMyWindow();
}

void CFilmCaseDoc::OnCutin(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	CheckAndGetUndo(m_filmCaseData,n,n);

	int cutinFlag = pFilm->GetCutinFlag();
	cutinFlag++;
	cutinFlag %= 2;
	pFilm->SetCutinFlag(cutinFlag);
	m_app->SetModify();
	UpdateMyWindow();
}

void CFilmCaseDoc::OnSkipFilm(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	CheckAndGetUndo(m_filmCaseData,n,n);

	int skipFilmFlag = pFilm->GetSkipToFilmEndEnable();
	skipFilmFlag++;
	skipFilmFlag %= 2;
	pFilm->SetSkipToFilmEndEnable(skipFilmFlag);
	m_app->SetModify();
	UpdateMyWindow();
}


void CFilmCaseDoc::OnRenameLayer(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	int renameLayer = pFilm->GetRenameLayer();
	int newData = renameLayer;

	if (m_inputDialog->GetNumber(renameLayer,&newData,"renameLayerData"))
	{
		if (newData != renameLayer)
		{
			CheckAndGetUndo(m_filmCaseData,n,n);

			renameLayer = newData;
			pFilm->SetRenameLayer(renameLayer);

			m_app->SetModify();
			UpdateMyWindow();
		}
	}
}


int CFilmCaseDoc::GetNowSelectNumber(void)
{
	return m_filmCaseData->GetNowSelectNumber();
}

void CFilmCaseDoc::SetSelectNumber(int n)
{
	ClearUndo();
	m_filmCaseData->SetSelectNumber(n);
}


int CFilmCaseDoc::GetFilmKosuu(void)
{
	return m_filmCaseData->GetObjectKosuu();
}


CFilmData* CFilmCaseDoc::GetNowSelectFilm(void)
{
	return (CFilmData*)(m_filmCaseData->GetObjectData(m_filmCaseData->GetNowSelectNumber()));

//	if (m_nowSelectNumber == -1) return NULL;
//	if ((m_nowSelectNumber<0) || (m_nowSelectNumber >= m_filmKosuu)) return NULL;
//	return m_filmDataArray[m_nowSelectNumber];
}


CFilmData* CFilmCaseDoc::GetFilm(int n)
{
	return (CFilmData*)(m_filmCaseData->GetObjectData(n));

//	if (n == -1) return NULL;
//	if ((n<0) || (n >= m_filmKosuu)) return NULL;
//	return m_filmDataArray[n];
}

/*
BOOL CFilmCaseDoc::CreateFilmArray(int n)
{

	CFilmData** lpArray = new CFilmData*[n];

	for (int i=0;i<m_filmKosuu;i++)
	{
		lpArray[i] = m_filmDataArray[i];
	}
	for (i=m_filmKosuu;i<n;i++)
	{
		lpArray[i] = NULL;
	}

	DELETEARRAY(m_filmDataArray);
	m_filmDataArray = lpArray;
	m_filmKosuuMax = n;

	return TRUE;
}
*/

/*
BOOL CFilmCaseDoc::LoadAllFilm(FILE* file,int kosuu)
{
	return TRUE;
}


BOOL CFilmCaseDoc::SaveAllFilm(FILE* file)
{
	for (int i=0;i<m_filmKosuu;i++)
	{
		CFilmData* lpFilm = m_filmDataArray[i];
		if (lpFilm == NULL)
		{
			MessageBox(NULL,"no CFilmData in CFilmCaseDoc::Save()","致命的ERROR",MB_OK);
		}
		else
		{
			lpFilm->Save(file);
		}
	}
	return TRUE;
}
*/


CFilmCaseData* CFilmCaseDoc::GetFilmCaseData(void)
{
	return m_filmCaseData;
}

/*
int CFilmCaseDoc::SearchFilm(LPSTR filmName,int searchStart,int noCheckNumber)
{
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if (kosuu<1) return -1;

	int loop = kosuu / 2 + 1;
	int n1 = (searchStart + kosuu) % kosuu;
	int n2 = (searchStart-1 + kosuu*2) % kosuu;

	for (int i=0;i<loop;i++)
	{
		CFilmData* pFilm;
		if (n1 != noCheckNumber)
		{
			pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n1));
			if (pFilm != NULL)
			{
				if (strcmp(filmName,pFilm->GetMyName()) == 0) return n1;
			}
		}

		if (n2 != noCheckNumber)
		{
			pFilm = (CFilmData*)(m_filmCaseData->GetObjectData(n2));
			if (pFilm != NULL)
			{
				if (strcmp(filmName,pFilm->GetMyName()) == 0) return n2;
			}
		}

		n1 += 1;
		n1 %= kosuu;
		n2 +=(kosuu*2-1);
		n2 %= kosuu;
	}

	return -1;
}
*/


void CFilmCaseDoc::InitLoaded(void)
{
	if (m_view == NULL) return;
	((CFilmCaseView*)m_view)->ReCalcuScrollPara();
}

void CFilmCaseDoc::ReCalcuScrollPara(void)
{
	if (m_view != NULL) 
	{
		((CFilmCaseView*)m_view)->ReCalcuScrollPara();
	}
}


BOOL CFilmCaseDoc::ClearAllFilm(void)
{
	return m_filmCaseData->ClearAllFilm();
}


int CFilmCaseDoc::GetScrollObjectKosuu(int vh)
{
	if (m_filmCaseData != NULL) return m_filmCaseData->GetObjectKosuu() + 1;

	return 1;
}

void CFilmCaseDoc::ScrollToSelect(void)
{
	int n = GetNowSelectNumber();
	CFilmCaseView* pView = (CFilmCaseView*)m_view;

	pView->ReCalcuScrollPara();
	pView->ScrollTo(n);

}

CCase* CFilmCaseDoc::GetNowSelectCaseObject(void)
{
	return m_filmCaseData;
}

void CFilmCaseDoc::OnSelectNumber(int n)
{
	ClearUndo();
	m_app->FilmIsChanged();
}

void CFilmCaseDoc::OnDeleteKey(void)
{
	if (m_app->GetConfig("deleteKeyFilm") == 0) return;
	OnDelete();
}

void CFilmCaseDoc::OnEnterKey(void)
{
	int n = GetNowSelectNumber();
	int kosuu = m_filmCaseData->GetObjectKosuu();
	if (n == kosuu)
	{
		OnNewFilm();
	}
	else
	{
		OnChangeName(n);
	}
}

void CFilmCaseDoc::OnInsertKey(void)
{
	OnNewFilm();
}

void CFilmCaseDoc::OnSpaceKey(void)
{
	m_app->ChangeWindowIfCan(FILM_WINDOW);
}




void CFilmCaseDoc::ExchangeFilm(int n1,int n2)
{
	if (n1 == n2) return;

	ClearUndo();

	int kosuu = GetFilmKosuu();
	if (n2>=kosuu)
	{
		MoveAndInsertFilm(n1,n2);
		return;
	}

	m_filmCaseData->ExchangeData(n1,n2);
	m_app->FilmIsChanged();
	m_app->SetModify();
	UpdateMyWindow();
}


void CFilmCaseDoc::MoveAndInsertFilm(int from,int to)
{
	if (from == to) return;

	int kosuu = GetFilmKosuu();
	if ((from < 0) || (from >= kosuu)) return;
	if ((to<0) || (to>kosuu)) return;

	m_filmCaseData->MoveAndInsertData(from,to);
	m_app->FilmIsChanged();
	m_app->SetModify();
	UpdateMyWindow();
}

BOOL CFilmCaseDoc::CheckExistUndo(void)
{
	CUndoMemoryObject* undo = m_app->GetUndoObject();
	if (undo != NULL)
	{
		int undoType = undo->GetUndoType();
		if (undoType == UNDO_TYPE_FILM)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFilmCaseDoc::OnUndo(int n)
{
	BOOL f = FALSE;

	if (m_app->GetUndoMode())
	{
		CFilmCaseData* pFilmCase = m_app->GetFilmCaseData();
		if (pFilmCase == NULL) return FALSE;

		CUndoMemoryObject* undo = m_app->GetUndoObject();
		if (undo != NULL)
		{
			if (CheckExistUndo())
			{
				int dataType = undo->GetUndoDataType();
				int startN = undo->GetUndoStartN();
				int endN = undo->GetUndoEndN();
				int numN = endN - startN + 1;
				

				if (dataType == UNDO_DATA_INSERT)
				{
					pFilmCase->DeleteObjectData(startN,numN);
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_DELETE)
				{
					for (int i=startN;i<=endN;i++)
					{
						pFilmCase->CreateObjectData(i);
						CFilmData* pFilm = (CFilmData*)(pFilmCase->GetObjectData(i));
						pFilm->Init();
						pFilm->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_MODIFY)
				{
					for (int i=startN;i<=endN;i++)
					{
						CFilmData* pFilm = (CFilmData*)(pFilmCase->GetObjectData(i));
						pFilm->Init();
						pFilm->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
			}
		}
	}




	if (f)
	{
		((CFilmCaseView*)m_view)->ReCalcuScrollPara();
		m_app->SetModify();
		m_app->FilmIsChanged();
		m_view->MyInvalidateRect();
	}


	return f;
}

void CFilmCaseDoc::CheckAndGetUndo(CFilmCaseData* pFilmCase,int start,int end)
{
	if (pFilmCase == NULL) return;

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_FILM,UNDO_DATA_MODIFY,start,end);
		for (int i=start;i<=end;i++)
		{
			CFilmData* pFilm = (CFilmData*)(pFilmCase->GetObjectData(i));
			if (pFilm != NULL)
			{
				pFilm->Save(NULL,undo);
			}
		}
	}
}

void CFilmCaseDoc::ClearUndo(void)
{
	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear();
	}
}


/*_*/

