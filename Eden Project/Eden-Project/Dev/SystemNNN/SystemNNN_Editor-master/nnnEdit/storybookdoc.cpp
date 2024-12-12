//
// storybookdoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "windowlist.h"

#include "case.h"


#include "storydata.h"
#include "storybookdata.h"

#include "scrollview.h"
#include "storybookview.h"
#include "storybookdoc.h"

#include "myapplicationBase.h"

#include "myfileopen.h"
#include "myinputdialog.h"

#include "undoMemoryObject.h"
//#include "configparalist.h"



CStoryBookDoc::CStoryBookDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = STORYBOOK_WINDOW;

//	m_storyDataArray = NULL;
//	m_storyKosuu = 0;
//	m_storyKosuuMax = 0;
//	m_nowSelectNumber = -1;

	m_storyBookData = new CStoryBookData();

//	CreateStoryArray(256);

	m_view = new CStoryBookView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

//	m_view->SetViewWindowText("StoryBook");
//	m_view->MoveViewWindow(652,456,GetSystemMetrics(SM_CXVSCROLL)+480,24+24*5);

	m_inputDialog = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance);
	m_file = new CMyFileOpen(m_app->GetFrameHWND(),m_app->GetMyInstance(),"sty");
}



CStoryBookDoc::~CStoryBookDoc()
{
	End();
}

void CStoryBookDoc::End(void)
{
	ENDDELETECLASS(m_storyBookData);
	ENDDELETECLASS(m_inputDialog);
	ENDDELETECLASS(m_file);


//	if (m_storyDataArray != NULL)
//	{
//		for (int i=0;i<m_storyKosuuMax;i++)
//		{
//			ENDDELETECLASS(m_storyDataArray[i]);
//		}
//	}
//
//	DELETEARRAY(m_storyDataArray);
}

//void CStoryBookDoc::OnCloseButton(void)
//{
//
//}


//BOOL CStoryBookDoc::CreateStoryArray(int n)
//{
	/*
	CStoryData** lpArray = new CStoryData*[n];

	for (int i=0;i<m_storyKosuu;i++)
	{
		lpArray[i] = m_storyDataArray[i];
	}
	for (i=m_storyKosuu;i<n;i++)
	{
		lpArray[i] = NULL;
	}

	DELETEARRAY(m_storyDataArray);
	m_storyDataArray = lpArray;
	m_storyKosuuMax = n;
*/
//	return TRUE;
//}


CStoryData* CStoryBookDoc::GetStory(int n)
{
	return (CStoryData*)(m_storyBookData->GetObjectData(n));

//	if ((n<0) || (n>=m_storyKosuu)) return NULL;
//	return m_storyDataArray[n];
}

CStoryData* CStoryBookDoc::GetNowSelectStory(void)
{
	return (CStoryData*)(m_storyBookData->GetObjectData(m_storyBookData->GetNowSelectNumber()));
//	return GetStory(m_nowSelectNumber);
}


int CStoryBookDoc::GetStoryKosuu(void)
{
	return m_storyBookData->GetObjectKosuu();
}

int CStoryBookDoc::GetNowSelectNumber(void)
{
	return m_storyBookData->GetNowSelectNumber();
//	return m_nowSelectNumber;
}


void CStoryBookDoc::OnNewStory(int n)
{
	int kosuu = m_storyBookData->GetObjectKosuu();

	int k;
	if (1)
	{
		k = m_storyBookData->GetNowSelectNumber();
		if ((k<0) || (k>=kosuu)) k = kosuu;
	}

	if (k == 0)
	{
		if (kosuu != 0)
		{
			MessageBox(m_app->GetFrameHWND(),"この位置には挿入できにゃいよ","警告",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
			return;
		}
	}

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_STORY,UNDO_DATA_INSERT,k,k);
	}




	m_storyBookData->CreateObjectData(k);
	
	CStoryData* pStory = (CStoryData*)(m_storyBookData->GetObjectData(k));
	pStory->Init(m_app->GetNewStoryName());

	((CStoryBookView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->StoryIsChanged();
	return;
}


void CStoryBookDoc::OnOpenStory(int n)
{
	n = m_storyBookData->GetNowSelectNumber();
	int kosuu = m_storyBookData->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	int kk = m_storyBookData->GetNowSelectNumber();
	if ((kk<0) || (kk>=kosuu)) kk = kosuu;

	if (kk == 0)
	{
		if (kosuu != 0)
		{
			MessageBox(m_app->GetFrameHWND(),"この位置には読み込めにゃいよ","警告",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
			return;
		}
	}


	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_STORY,UNDO_DATA_INSERT,kk,kk);
	}


	FILE* file = m_file->OpenLoad("nnndir\\story\\*.sty");
	if (file != NULL)
	{


		if (m_storyBookData->CreateObjectData(n))
		{
			CStoryData* pStory = (CStoryData*)(m_storyBookData->GetObjectData(n));
			if (pStory != NULL)
			{
				pStory->Load(file);
			}

			LPSTR newName = pStory->GetMyName();

			//int k = m_app->SearchStory(newName,n,n);
			int k = m_storyBookData->SearchStory(newName,n,n);
			if (k != -1)
			{
				LPSTR makedName = m_app->GetNewStoryName();
				char mes[128];
				wsprintf(mes,"%s_%s",makedName,newName);
				int ln = strlen(mes);
				if (ln>62) ln = 62;
				mes[ln] = 0;
				mes[ln+1] = 0;

				pStory->SetMyName(mes);

				//config?
//@@@				if (m_app->GetConfigData(CONFIGPARA_STORYNAMECHANGED_DIALOG))
				{
					MessageBox(m_app->GetFrameHWND(),"同じ名前が存在しています。\n名前を変更しましたにゃ","警告",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
				}
			}
		}

		fclose(file);


		((CStoryBookView*)m_view)->ReCalcuScrollPara();
		m_app->SetModify();
		m_app->StoryIsChanged();
	}
}



void CStoryBookDoc::OnSaveStory(int n)
{
	n = m_storyBookData->GetNowSelectNumber();
	int kosuu = m_storyBookData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	ClearUndo();

	FILE* file = m_file->OpenSave("nnndir\\story\\*.sty");
	if (file != NULL)
	{
		CStoryData* pStory = (CStoryData*)(m_storyBookData->GetObjectData(n));
		if (pStory != NULL)
		{
			pStory->Save(file);
		}

		fclose(file);

		m_app->StoryIsChanged();
	}
}


void CStoryBookDoc::OnDelete(int n)
{
	n = m_storyBookData->GetNowSelectNumber();
	int kosuu = m_storyBookData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;
	
	CStoryData* pStory = (CStoryData*)(m_storyBookData->GetObjectData(n));
	if (pStory == NULL) return;
	if (pStory->CheckIamTop())
	{
		MessageBox(m_frameHWND,"トップシナリオは削除できにゃいの","禁止事項",MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return;
	}



	if (m_app->GetWarningDelScene())
	{
		int rt = MessageBox(m_frameHWND,"本当に削除しますか","削除の確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (rt != IDYES) return;
	}


	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_STORY,UNDO_DATA_DELETE,n,n);
		pStory->Save(NULL,undo);
	}





	m_storyBookData->DeleteObjectData(n);

	((CStoryBookView*)m_view)->ReCalcuScrollPara();
	m_app->SetModify();
	m_app->StoryIsChanged();

}



void CStoryBookDoc::OnSearch(int n)
{
	LPSTR name = m_inputDialog->GetText("検索するシナリオ名");
	if (name == NULL) return;

	int kosuu = m_storyBookData->GetObjectKosuu();
	int found = -1;

	for (int i=0;i<kosuu;i++)
	{
		CStoryData* pStory = (CStoryData*)(m_storyBookData->GetObjectData(i));
		if (pStory != NULL)
		{
			LPSTR storyname = pStory->GetMyName();
			if (strcmp(name,storyname) == 0)
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

	m_storyBookData->SetSelectNumber(found);
	m_app->StoryIsChanged();
//	UpdateMyWindow();

}



void CStoryBookDoc::OnChangeName(int n)
{
	int kosuu = m_storyBookData->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	//name change?
	CStoryData* pStory = (CStoryData*)(m_storyBookData->GetObjectData(n));
	if (pStory == NULL) return;

	//check top
	if (pStory->CheckIamTop())
	{
		MessageBox(m_frameHWND,"トップシナリオの名前は変更できにゃいの","禁止事項",MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return;
	}

	CheckAndGetUndo(m_storyBookData,n,n);


	LPSTR newName = m_inputDialog->GetText(pStory->GetMyName());

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
//		int k = m_app->SearchStory(newName,n,n);
		int k = m_storyBookData->SearchStory(newName,n,n);
		if (k == -1)
		{
			//ok
			pStory->SetMyName(newName);
		}
		else
		{
			LPSTR makedName = m_app->GetNewStoryName();
			char mes[128];
			wsprintf(mes,"%s_%s",makedName,newName);
			int ln = strlen(mes);
			if (ln>62) ln = 62;
			mes[ln] = 0;
			mes[ln+1] = 0;

			pStory->SetMyName(mes);

			//config?
//			if (m_app->GetConfigData(CONFIGPARA_STORYNAMECHANGED_DIALOG))
			{
				MessageBox(m_app->GetFrameHWND(),"同じ名前が存在しています。\n名前を変更しましたにゃ","警告",MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
			}
		}


//		m_view->MyInvalidateRect();
		m_app->SetModify();
		m_app->StoryIsChanged();
	}

}

void CStoryBookDoc::OnSelectStory(int n)
{
	ClearUndo();
	m_storyBookData->SetSelectNumber(n);
	m_app->StoryIsChanged();

}

void CStoryBookDoc::OnSelectNumber(int n)
{
	ClearUndo();
	m_app->StoryIsChanged();
}





//BOOL CStoryBookDoc::LoadAllStory(FILE* file, int kosuu)
//{
//	return TRUE;
//}


//BOOL CStoryBookDoc::SaveAllStory(FILE* file)
//{
/*
	for (int i=0;i<m_storyKosuu;i++)
	{
		CStoryData* lpStory = m_storyDataArray[i];
		if (lpStory == NULL)
		{
			MessageBox(NULL,"no CStoryData in CStoryBookDoc::Save()","致命的ERROR",MB_OK);
		}
		else
		{
			lpStory->Save(file);
		}
	}
*/

//	return TRUE;
//}

CStoryBookData* CStoryBookDoc::GetStoryBookData(void)
{
	return m_storyBookData;
}

/*
int CStoryBookDoc::SearchStory(LPSTR storyName,int searchStart,int noCheckNumber)
{
	int kosuu = m_storyBookData->GetObjectKosuu();
	if (kosuu<1) return -1;

	int loop = kosuu / 2 + 1;
	int n1 = (searchStart + kosuu) % kosuu;
	int n2 = (searchStart-1 + kosuu) % kosuu;

	for (int i=0;i<loop;i++)
	{
		CStoryData* pStory;
		if (n1 != noCheckNumber)
		{
			pStory = (CStoryData*)(m_storyBookData->GetObjectData(n1));
			if (pStory != NULL)
			{
				if (strcmp(storyName,pStory->GetMyName()) == 0) return n1;
			}
		}

		if (n2 != noCheckNumber)
		{
			pStory = (CStoryData*)(m_storyBookData->GetObjectData(n2));
			if (pStory != NULL)
			{
				if (strcmp(storyName,pStory->GetMyName()) == 0) return n2;
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


void CStoryBookDoc::InitLoaded(void)
{
	if (m_view == NULL) return;
	((CStoryBookView*)m_view)->ReCalcuScrollPara();
}


void CStoryBookDoc::ClearAllStory(void)
{
	m_storyBookData->DeleteAll();
}

void CStoryBookDoc::ReCalcuScrollPara(void)
{
	if (m_view != NULL) 
	{
		((CStoryBookView*)m_view)->ReCalcuScrollPara();
	}
}


int CStoryBookDoc::GetScrollObjectKosuu(int vh)
{
//	if (m_filmCaseData != NULL) return m_filmCaseData->GetObjectKosuu();
//
//	return 0;

//int k = GetStoryKosuu();
//char mes[256];
//sprintf(mes,"[story=%d]",k);
//OutputDebugString(mes);

	return GetStoryKosuu() + 1;
}



CCase* CStoryBookDoc::GetNowSelectCaseObject(void)
{
	return m_storyBookData;
}


void CStoryBookDoc::OnDeleteKey(void)
{
	if (m_app->GetConfig("deleteKeyStory") == 0) return;
	OnDelete();
}

void CStoryBookDoc::OnEnterKey(void)
{
	int n = GetNowSelectNumber();
	int kosuu = m_storyBookData->GetObjectKosuu();
	if (n == kosuu)
	{
		OnNewStory();
	}
	else
	{
		OnChangeName(n);
	}
}

void CStoryBookDoc::OnInsertKey(void)
{
	OnNewStory();
}

void CStoryBookDoc::OnSpaceKey(void)
{
	m_app->ChangeWindowIfCan(STORY_WINDOW);
}



BOOL CStoryBookDoc::CheckExistUndo(void)
{
	CUndoMemoryObject* undo = m_app->GetUndoObject();
	if (undo != NULL)
	{
		int undoType = undo->GetUndoType();
		if (undoType == UNDO_TYPE_STORY)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CStoryBookDoc::OnUndo(int n)
{
	BOOL f = FALSE;

	if (m_app->GetUndoMode())
	{
		CStoryBookData* pStoryBook = m_app->GetStoryBookData();
		if (pStoryBook == NULL) return FALSE;

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
					pStoryBook->DeleteObjectData(startN,numN);
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_DELETE)
				{
					for (int i=startN;i<=endN;i++)
					{
						pStoryBook->CreateObjectData(i);
						CStoryData* pStory = (CStoryData*)(pStoryBook->GetObjectData(i));
						pStory->Init();
						pStory->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_MODIFY)
				{
					for (int i=startN;i<=endN;i++)
					{
						CStoryData* pStory = (CStoryData*)(pStoryBook->GetObjectData(i));
						pStory->Init();
						pStory->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
			}
		}
	}




	if (f)
	{
		((CStoryBookView*)m_view)->ReCalcuScrollPara();
		m_app->SetModify();
		m_app->StoryIsChanged();
		m_view->MyInvalidateRect();
	}


	return f;
}

void CStoryBookDoc::CheckAndGetUndo(CStoryBookData* pStoryBook,int start,int end)
{
	if (pStoryBook == NULL) return;

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_STORY,UNDO_DATA_MODIFY,start,end);
		for (int i=start;i<=end;i++)
		{
			CStoryData* pStory = (CStoryData*)(pStoryBook->GetObjectData(i));
			if (pStory != NULL)
			{
				pStory->Save(NULL,undo);
			}
		}
	}
}

void CStoryBookDoc::ClearUndo(void)
{
	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear();
	}
}

/*_*/

