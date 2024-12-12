//
// filmdoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "windowlist.h"

#include "case.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"

#include "..\..\systemNNN\nnnUtilLib\namelist.h"


#include "miniPic.h"

#include "filmview.h"
#include "filmdoc.h"

#include "filmdata.h"
#include "komadata.h"

#include "myinputdialog.h"
#include "selectdialog.h"

#include "myapplicationBase.h"
#include "undoMemoryObject.h"

//#include "configparamlist.h"

char CFilmDoc::m_defaultKomaName[] = "名前をいれてくださいにゃ";


CFilmDoc::CFilmDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = FILM_WINDOW;

	m_miniPic = NULL;
	m_miniPicKosuu = 0;
	m_miniPicNumberTable = NULL;



//	m_miniPicHBitmap = NULL;
//	m_miniPicFlag = NULL;
//	m_lplpBuffer = NULL;

	m_komaBufferFlag = FALSE;

	m_view = new CFilmView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

//	m_view->SetViewWindowText("フイルム");
//	m_view->MoveViewWindow(5,528,72*6,112+GetSystemMetrics(SM_CYHSCROLL));

	AddMiniPicBuffer(64);
//	m_view->Show();

	m_input = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance,512);
	m_selectDialog = new CSelectDialog(m_app->GetFrameHWND(),m_hInstance);
	m_overrapDialog = new CSelectDialog(m_app->GetFrameHWND(),m_hInstance);
	m_overrapName = new CNameList();
	m_overrapName->LoadFile("nnndir\\setup\\list\\wipenamelist.txt");
/*
	m_overrapName->AddName("0");
	m_overrapName->AddName("なし");
	m_overrapName->AddName("1");
	m_overrapName->AddName("ノーマル");
	m_overrapName->AddName("2");
	m_overrapName->AddName("カーテン(右)");
	m_overrapName->AddName("3");
	m_overrapName->AddName("カーテン(下)");
	m_overrapName->AddName("4");
	m_overrapName->AddName("カーテン(上下)");
	m_overrapName->AddName("5");
	m_overrapName->AddName("スライド(斜め左)");
	m_overrapName->AddName("6");
	m_overrapName->AddName("引きずり(左)");
	m_overrapName->AddName("7");
	m_overrapName->AddName("円拡大");
	m_overrapName->AddName("8");
	m_overrapName->AddName("円縮小");
	m_overrapName->AddName("9");
	m_overrapName->AddName("円引きずり(左)");
	m_overrapName->AddName("10");
	m_overrapName->AddName("時計");
*/





	m_bgmTypeList0 = new CNameList();
	m_bgmTypeList1 = new CNameList();
	m_bgmTypeList2 = new CNameList();

	m_bgmTypeList0->AddName("0");
	m_bgmTypeList0->AddName("削除");
	m_bgmTypeList0->AddName("1");
	m_bgmTypeList0->AddName("停止");
	m_bgmTypeList0->AddName("2");
	m_bgmTypeList0->AddName("曲選択");
	m_bgmTypeList0->AddName("7");
	m_bgmTypeList0->AddName("ボリューム設定のみ");

	m_bgmTypeList1->AddName("0");
	m_bgmTypeList1->AddName("削除");
	m_bgmTypeList1->AddName("1");
	m_bgmTypeList1->AddName("停止");
	m_bgmTypeList1->AddName("2");
	m_bgmTypeList1->AddName("曲選択");
	m_bgmTypeList1->AddName("6");
	m_bgmTypeList1->AddName("フェードアウト設定");

	m_bgmTypeList2->AddName("0");
	m_bgmTypeList2->AddName("削除");
	m_bgmTypeList2->AddName("1");
	m_bgmTypeList2->AddName("停止");
	m_bgmTypeList2->AddName("2");
	m_bgmTypeList2->AddName("曲選択");
	m_bgmTypeList2->AddName("3");
	m_bgmTypeList2->AddName("ループ設定");
	m_bgmTypeList2->AddName("4");
	m_bgmTypeList2->AddName("ボリューム設定");
	m_bgmTypeList2->AddName("5");
	m_bgmTypeList2->AddName("フェードイン設定");
	m_bgmTypeList2->AddName("6");
	m_bgmTypeList2->AddName("フェードアウト設定");


//	m_bgmNameList = new CNameList();
//	m_bgmNameList->LoadFile("nnndir\\bgmlist.txt");
}


CFilmDoc::~CFilmDoc()
{
	End();
}

void CFilmDoc::End(void)
{
//	ENDDELETECLASS(m_bgmNameList);
	ENDDELETECLASS(m_bgmTypeList2);
	ENDDELETECLASS(m_bgmTypeList1);
	ENDDELETECLASS(m_bgmTypeList0);
	ENDDELETECLASS(m_overrapName);
	ENDDELETECLASS(m_overrapDialog);
	ENDDELETECLASS(m_selectDialog);
	ENDDELETECLASS(m_input);
	DELETEARRAY(m_miniPicNumberTable);

	if (m_miniPic != NULL)
	{
		for (int i=0;i<m_miniPicKosuu;i++)
		{
			ENDDELETECLASS(m_miniPic[i]);
		}
		DELETEARRAY(m_miniPic);
	}

//	DELETEARRAY(m_miniPicFlag);
//	if (m_miniPicHBitmap != NULL)
//	{
//		for (int i=0;i<m_miniPicKosuu;i++)
//		{
//			DELETEGDIOBJECT(m_miniPicHBitmap[i]);
//		}
//		DELETEARRAY(m_miniPicHBitmap);
//	}

	//DELETEARRAY(m_lplpBuffer);	//なかみはHBITMAPが削除されるときに開放
}



void CFilmDoc::OnNewKoma(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;


	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_KOMA,UNDO_DATA_INSERT,n,n);
	}



	pFilm->CreateObjectData(n);

	InsertMiniPic(n);

	CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(n));
	if (pKoma == NULL) return;




	((CFilmView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->KomaIsChanged();
}



void CFilmDoc::OnCutKoma(int n)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(n));
	if (pKoma == NULL) return;


	int start = pFilm->GetSelectStart();
	int end = pFilm->GetSelectEnd();

	if ((start<0) || (start>=kosuu)) return;
	if ((end<0) || (end>=kosuu)) return;


	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_KOMA,UNDO_DATA_DELETE,start,end);
		for (int i=start;i<=end;i++)
		{
			CKomaData* pKoma000 = (CKomaData*)(pFilm->GetObjectData(i));
			pKoma000->Save(NULL,undo);
		}
	}


	BOOL errorFlag = FALSE;

	for (int i=start;i<=end;i++)
	{
		char filename[1024];
		wsprintf(filename,"nnndir\\tmp\\koma%d.kma",i-start+1);
		FILE* file = CMyFile::Open(filename,"wb");
		if (file != NULL)
		{
			pKoma = (CKomaData*)(pFilm->GetObjectData(i));
			if (pKoma != NULL)
			{
				pKoma->Save(file);
			}
			else
			{
				MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
				errorFlag = TRUE;
			}

			fclose(file);
		}
		else
		{
			MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
			errorFlag = TRUE;
		}
	}	

	FILE* file = CMyFile::Open("nnndir\\tmp\\komanum.dat","wb");
	if (file != NULL)
	{
		int komaKosuu = end - start + 1;
		if (errorFlag) komaKosuu = 0;
		fwrite(&komaKosuu,sizeof(int),1,file);
		fclose(file);
	}

	m_komaBufferFlag = !errorFlag;

	OnDelete(n);
}


void CFilmDoc::OnCopyKoma(int n)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(n));
	if (pKoma == NULL) return;

	int start = pFilm->GetSelectStart();
	int end = pFilm->GetSelectEnd();

	if ((start<0) || (start>=kosuu)) return;
	if ((end<0) || (end>=kosuu)) return;


	BOOL errorFlag = FALSE;

	for (int i=start;i<=end;i++)
	{
		char filename[1024];
		wsprintf(filename,"nnndir\\tmp\\koma%d.kma",i-start+1);
		FILE* file = CMyFile::Open(filename,"wb");
		if (file != NULL)
		{
			pKoma = (CKomaData*)(pFilm->GetObjectData(i));
			if (pKoma != NULL)
			{
				pKoma->Save(file);
			}
			else
			{
				MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
				errorFlag = TRUE;
			}

			fclose(file);
		}
		else
		{
			MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
			errorFlag = TRUE;
		}
	}	

	FILE* file = CMyFile::Open("nnndir\\tmp\\komanum.dat","wb");
	if (file != NULL)
	{
		int komaKosuu = end - start + 1;
		if (errorFlag) komaKosuu = 0;
		fwrite(&komaKosuu,sizeof(int),1,file);
		fclose(file);
	}

	m_app->SetModify();
	m_app->KomaIsChanged();

	m_komaBufferFlag = !errorFlag;
}


void CFilmDoc::OnPasteKoma(int n)
{
	if (m_komaBufferFlag == FALSE) return;

	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;


	int komaKosuu = 0;
	FILE* file = CMyFile::Open("nnndir\\tmp\\komanum.dat","rb");
	if (file == NULL) return;
	fread(&komaKosuu,sizeof(int),1,file);
	fclose(file);

	if (komaKosuu<=0) return;

	int insertNumber = n;

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_KOMA,UNDO_DATA_INSERT,n,n+komaKosuu-1);
	}





	for (int i=0;i<komaKosuu;i++)
	{
		pFilm->CreateObjectData(insertNumber+i);
		InsertMiniPic(insertNumber+i);

		CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(insertNumber+i));
		if (pKoma != NULL)
		{
			char filename[1024];
			wsprintf(filename,"nnndir\\tmp\\koma%d.kma",i+1);
			FILE* file2 = CMyFile::Open(filename,"rb");
			if (file2 != NULL)
			{
				pKoma->Load(file2);
				fclose(file2);
			}
			else
			{
				MessageBox(NULL,"LOAD KOMA ERROR","ERROR",MB_OK);
			}
		}
	}

	((CFilmView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->KomaIsChanged();
}

void CFilmDoc::OnDelete(int n)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return;

	n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;


	CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(n));
	if (pKoma == NULL) return;

	if (m_app->GetWarningDelKoma())
	{
		int rt = MessageBox(m_frameHWND,"本当に削除しますか","削除の確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (rt != IDYES) return;
	}

	int start = pFilm->GetSelectStart();
	int end = pFilm->GetSelectEnd();

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_KOMA,UNDO_DATA_DELETE,start,end);
		for (int i=start;i<=end;i++)
		{
			CKomaData* pKoma000 = (CKomaData*)(pFilm->GetObjectData(i));
			pKoma000->Save(NULL,undo);
		}
	}


	for (int i=end;i>=start;i--)
	{
		pFilm->DeleteObjectData(i);
		DeleteMiniPic(i);
	}


	pFilm->SetSelectNumber(start);
	pFilm->SetSelectSubNumber(-1);

	((CFilmView*)m_view)->ReCalcuScrollPara();
	m_app->SetModify();
	m_app->KomaIsChanged();
}




void CFilmDoc::OnOverrap(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

//	char oldText[32];
	int cnt = pKoma->GetOverrapCount();
	if (cnt == 0) cnt = m_app->GetConfig("defaultOverrapTime");
	int newData = cnt;

//	sprintf(oldText,"%d",cnt);

//	LPSTR newText = m_input->GetText(oldText,TRUE);
//	if (newText == NULL) return;

	if (m_input->GetNumber(cnt,&newData))
	{
//	int c = m_input->TextToSuuji();
		if ((newData >= 0) || (newData<9999))
		{

			CheckAndGetUndo(pFilm,n,n);

			pKoma->SetOverrapCount(newData);
			UpdateMyWindow();
		}
	}
}



void CFilmDoc::OnOverrapType(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

//	char oldText[32];
	int typ = pKoma->GetOverrapType();

	int c = m_overrapDialog->GetSelect(m_overrapName,typ+1);
	if (c == -1) return;

	c--;

	CheckAndGetUndo(pFilm,n,n);

	if (c == -1)
	{
		pKoma->SetOverrapCount(0);
	}
	else
	{
		pKoma->SetOverrapType(c);

		int cnt = pKoma->GetOverrapCount();
		if (cnt == 0)
		{
			pKoma->SetOverrapCount(m_app->GetConfig("defaultOverrapTime"));
		}
	}

	UpdateMyWindow();
}

void CFilmDoc::OnSetCG(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	int pl = pKoma->GetSetCGPlayer();
	int nm = pKoma->GetSetCGNumber();

	CNameList* plist = m_app->GetSetCGList();
	int rt = m_selectDialog->GetSelect(plist,pl);
	if (rt == -1) return;

	int c = 0;
	if (rt != 0)
	{
		char oldText[32];
		wsprintf(oldText,"%d",nm);

		LPSTR newText = m_input->GetText(oldText,TRUE);
		if (newText == NULL) return;

		c = m_input->TextToSuuji();
		if ((c<0) || (c>255)) return;
	}

	CheckAndGetUndo(pFilm,n,n);

	if (c == 0) rt = 0;

	pKoma->SetSetCG(rt,c);

	UpdateMyWindow();
}


int CFilmDoc::GetSetCG(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return ((pKoma->GetSetCGPlayer()) << 16) + (pKoma->GetSetCGNumber());
}



/*
void CFilmDoc::OnNoClearEffect(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

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
*/

void CFilmDoc::OnBGM(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	int bgm = pKoma->GetBGMNumber();
	int rt = -1;

	CheckAndGetUndo(pFilm,n,n);

	if (bgm == 0)
	{
		char name[256];
		wsprintf(name,"ボリューム変化のみ");
		m_bgmTypeList0->SetName(7,name);

		rt = m_selectDialog->GetSelect(m_bgmTypeList0,2);
	}
	else if (bgm == -1)
	{
		//現状設定する
		int fadeOut = pKoma->GetExMusicParam(3);
		char name[256];

		if (fadeOut == -1)
		{
			wsprintf(name,"フェードアウト設定(無設定)");
		}
		else
		{
			if (fadeOut == 0)
			{
				wsprintf(name,"フェードアウト設定(なし)");
			}
			else
			{
				wsprintf(name,"フェードアウト設定(%d.%d秒)",fadeOut/10,fadeOut % 10);
			}
		}
		m_bgmTypeList1->SetName(7,name);

		rt = m_selectDialog->GetSelect(m_bgmTypeList1,2);
	}
	else if (bgm == -2)
	{
		char name[256];
		int vol = pKoma->GetExMusicParam(4);
		wsprintf(name,"ボリューム変化のみ(%d)",vol);
		m_bgmTypeList0->SetName(7,name);

		rt = m_selectDialog->GetSelect(m_bgmTypeList0,2);
	}
	else
	{
		int loop = pKoma->GetExMusicParam(0);
		int volume = pKoma->GetExMusicParam(1);
		int fadeIn = pKoma->GetExMusicParam(2);
		int fadeOut = pKoma->GetExMusicParam(3);
		char name[256];


		if (loop == -1)
		{
			wsprintf(name,"ループ設定(無設定)");
		}
		else if (loop == 0)
		{
			wsprintf(name,"ループ設定(無限ループ)");
		}
		else
		{
			wsprintf(name,"ループ設定(%d回)",loop);
		}
		m_bgmTypeList2->SetName(7,name);

		if (volume == 0)
		{
			wsprintf(name,"ボリューム増減値設定(無設定)");
		}
		else
		{
			wsprintf(name,"ボリューム増減値(%d)",volume);
		}
		m_bgmTypeList2->SetName(9,name);


		if (fadeIn == -1)
		{
			wsprintf(name,"フェードイン設定(無設定)");
		}
		else
		{
			if (fadeIn == 0)
			{
				wsprintf(name,"フェードイン設定(なし)");
			}
			else
			{
				wsprintf(name,"フェードイン設定(%d.%d秒)",fadeIn/10,fadeIn % 10);
			}
		}
		m_bgmTypeList2->SetName(11,name);

		if (fadeOut == -1)
		{
			wsprintf(name,"フェードアウト設定(無設定)");
		}
		else
		{
			if (fadeOut == 0)
			{
				wsprintf(name,"フェードアウト設定(なし)");
			}
			else
			{
				wsprintf(name,"フェードアウト設定(%d.%d秒)",fadeOut/10,fadeOut % 10);
			}
		}

		m_bgmTypeList2->SetName(13,name);
		rt = m_selectDialog->GetSelect(m_bgmTypeList2,2);
	}


	if (rt == -1) return;

	if (rt == 0)
	{
		pKoma->SetBGMNumber(0);
		m_app->KomaIsChanged();
		m_app->SetModify();
	}
	else if (rt == 1)
	{
		pKoma->SetBGMNumber(-1);
		m_app->KomaIsChanged();
		m_app->SetModify();
	}
	else if (rt == 2)
	{
		int bgmCursor = bgm-1;
		if (bgmCursor<0) bgmCursor = 0;
//		int rt2 = m_selectDialog->GetSelect(m_bgmNameList,bgmCursor,NULL,this);
		int rt2 = m_selectDialog->GetSelect(m_app->GetBGMList(),bgmCursor,NULL,this);
		if (rt2 != -1)
		{
			pKoma->SetBGMNumber(rt2+1);
			m_app->KomaIsChanged();
			m_app->SetModify();
		}
	}

	if (bgm == -1)
	{
		if (rt == 3) rt = 6;
	}

	if ((bgm == -2) || (bgm == 0))
	{
		if (rt == 3) rt = 7;
	}


	if (rt == 3)	//loop
	{
		int loop = pKoma->GetExMusicParam(0);
		int loop2 = loop;
		if (m_input->GetNumber(loop,&loop2))
		{
			pKoma->SetExMusicParam(0,loop2);
			m_app->KomaIsChanged();
			m_app->SetModify();
		}
	}
	else if (rt == 4)	//vol
	{
		int vol = pKoma->GetExMusicParam(1);
		int vol2 = vol;
		if (m_input->GetNumber(vol,&vol2))
		{
			if (vol2<-100) vol2 = -100;
			if (vol2>100) vol2 = 100;
			pKoma->SetExMusicParam(1,vol2);
			m_app->KomaIsChanged();
			m_app->SetModify();
		}
	}
	else if (rt == 5)	//fadein
	{
		int fadein = pKoma->GetExMusicParam(2);
		int fadein2 = fadein;
		if (m_input->GetNumber(fadein,&fadein2))
		{
			pKoma->SetExMusicParam(2,fadein2);
			m_app->KomaIsChanged();
			m_app->SetModify();
		}
	}
	else if (rt == 6)	//fadeout
	{
		int fadeout = pKoma->GetExMusicParam(3);
		int fadeout2 = fadeout;
		if (m_input->GetNumber(fadeout,&fadeout2))
		{
			pKoma->SetExMusicParam(3,fadeout2);
			m_app->KomaIsChanged();
			m_app->SetModify();
		}
	}
	else if (rt == 7)
	{
		int setVolume = pKoma->GetExMusicParam(4);
		int setVolume2 = setVolume;
		if (m_input->GetNumber(setVolume,&setVolume2))
		{
			pKoma->SetBGMNumber(-2);
			pKoma->SetExMusicParam(4,setVolume2);
			m_app->KomaIsChanged();
			m_app->SetModify();
		}
	}

	UpdateMyWindow();
}


void CFilmDoc::OnWindowOff(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int d = pKoma->GetWindowOffFlag();
	d++;
	d %= 3;
	pKoma->SetWindowOffFlag(d);
	UpdateMyWindow();
}


void CFilmDoc::OnFrame(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	int frame = pKoma->GetFrameTime();


	char oldText[32];
	wsprintf(oldText,"%d",frame);


	LPSTR newText = m_input->GetText(oldText,TRUE);
	if (newText == NULL) return;

	int c = m_input->TextToSuuji();
	if (c < -1) return;

	CheckAndGetUndo(pFilm,n,n);

	pKoma->SetFrameTime(c);

	UpdateMyWindow();
}

void CFilmDoc::OnDemo(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int md = pKoma->GetDemoFlag();
	md++;
	md %= 3;
	pKoma->SetDemoFlag(md);
	m_app->SetModify();
	UpdateMyWindow();
}

void CFilmDoc::OnWindowNumber(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	int nm = pKoma->GetWindowNumber();
	int nm2 = nm;
	if (m_input->GetNumber(nm,&nm2,"ウィンドウ番号"))
	{
		if (nm2>=0)
		{
			CheckAndGetUndo(pFilm,n,n);

			pKoma->SetWindowNumber(nm2);
			UpdateMyWindow();
		}
	}
}


void CFilmDoc::OnCursorNumber(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	int nm = pKoma->GetCursorNumber();
	int nm2 = nm;
	if (m_input->GetNumber(nm,&nm2,"送りカーソル番号"))
	{
		if (nm2>=0)
		{
			CheckAndGetUndo(pFilm,n,n);

			pKoma->SetCursorNumber(nm2);
			UpdateMyWindow();
		}
	}
}

void CFilmDoc::OnMouseNumber(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	int nm = pKoma->GetMouseNumber();
	int nm2 = nm;
	if (m_input->GetNumber(nm,&nm2,"マウス番号"))
	{
		if (nm2>=0)
		{
			CheckAndGetUndo(pFilm,n,n);

			pKoma->SetMouseNumber(nm2);
			UpdateMyWindow();
		}
	}
}

void CFilmDoc::OnAutoMessage(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	int nm = pKoma->GetAutoMessage();
	int nm2 = nm;
	if (m_input->GetNumber(nm,&nm2,"自動メッセージ速度0,1-5,[off=-1]"))
	{
		if ((nm2>=-1) && (nm2<=5))
		{
			CheckAndGetUndo(pFilm,n,n);

			pKoma->SetAutoMessage(nm2);
			UpdateMyWindow();
		}
	}
}

void CFilmDoc::OnCannotClick(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int nm = pKoma->GetCannotClick();
	nm++;
	if (nm >= 2) nm = -1;
	pKoma->SetCannotClick(nm);
	UpdateMyWindow();
}

void CFilmDoc::OnCannotSkip(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int nm = pKoma->GetCannotSkip();
	nm++;
	if (nm >= 2) nm = -1;
	pKoma->SetCannotSkip(nm);
	UpdateMyWindow();
}

void CFilmDoc::OnOptionOff(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int nm = pKoma->GetOptionOff();
	nm++;
	if (nm >= 2) nm = -1;
	pKoma->SetOptionOff(nm);
	UpdateMyWindow();
}

void CFilmDoc::OnCutin(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int nm = pKoma->GetCutinFlag();
	nm++;
	if (nm >= 2) nm = -1;
	pKoma->SetCutinFlag(nm);
	m_app->SetModify();
	UpdateMyWindow();
}

void CFilmDoc::OnPreload(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int nm = pKoma->GetPreload();
	nm++;
	if (nm >= 2) nm = 0;
	pKoma->SetPreload(nm);
	UpdateMyWindow();
}


void CFilmDoc::OnExpStatus(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;

	CheckAndGetUndo(pFilm,n,n);

	int nm = pKoma->GetExpStatus();
	int nm2 = nm;
	if (m_input->GetNumber(nm,&nm2,"拡張ステータス"))
	{
		if (nm2>=0)
		{
			pKoma->SetExpStatus(nm2);
			UpdateMyWindow();
		}
	}
}


void CFilmDoc::OnVarControl(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return;


	int block = 0;
	//search first block?





	LPSTR text = m_input->GetText("0",FALSE,"0:追加 -1:情報 行指定1-16");
	if (text == NULL) return;
	block = atoi(text);

	CheckAndGetUndo(pFilm,n,n);

	int varType = m_app->GetVarType();

	if (block == -1)
	{
		char mes[1024*16];
		char mes0[1024];

		mes[0] = 0;
		mes0[0] = 0;

		BOOL flg = FALSE;

		CNameList* list = m_app->GetVarControlList();

		for (int i=0;i<16;i++)
		{
			int d = pKoma->GetVarControl(i);
			if (d)
			{
				flg = TRUE;

				int varNumber;
				int varData;
				if (varType == 0)
				{
					varNumber = d & 0x3ff;
					varData = (d >> 10);
				}
				else
				{
					varNumber = d & 0xfff;
					varData = (d >> 12);

				}

				sprintf_s(mes0,sizeof(mes0),"[%d]%s=%d\n",i+1,list->GetName(varNumber*2+2+1),varData);
				strcat_s(mes,sizeof(mes),mes0);
			}
		}

		if (flg == FALSE)
		{
			sprintf_s(mes,sizeof(mes),"%s","設定はありません");
		}


		MessageBox(NULL,mes,"設定内容",MB_OK);

		return;
	}
	if ((block < 0) || (block > 16)) return;


	if (block == 0)
	{
		//search aki
		int found = -1;

		for (int i=0;i<16;i++)
		{
			if (pKoma->GetVarControl(i) == 0)
			{
				found = i;
				break;
			}
		}

		if (found == -1)
		{
			MessageBox(NULL,"空きがありません","Warning",MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		block = found;
	}
	else
	{
		block -= 1;
	}


	if ((block < 0) || (block > 15)) return;

	int nm = pKoma->GetVarControl(block);

//	int varNumber = nm  & 0x3ff;
//	int varData = (nm >> 10);
	int varNumber;
	int varData;
	if (varType == 0)
	{
		varNumber = nm & 0x3ff;
		varData = (nm >> 10);
	}
	else
	{
		varNumber = nm & 0xfff;
		varData = (nm >> 12);

	}


	int varKosuu = m_app->GetVarKosuu();
	if (varNumber >= varKosuu) varNumber = varKosuu - 1;

	int v = varNumber;
	if (v>0) v++;

	int rt2 = m_selectDialog->GetSelect(m_app->GetVarControlList(),v,NULL,this);
	if ((rt2 == 0) || (rt2 == 1))
	{
		pKoma->SetVarControl(block,0);
		UpdateMyWindow();
	}
	else if (rt2 > 1)
	{
		int nm2 = varData;

		if (m_input->GetNumber(varData,&nm2,"変数コントロール"))
		{
			int d;
			if (varType == 0)
			{
				d = (nm2 << 10) & 0xfffffc00;
			}
			else
			{
				d = (nm2 << 12) & 0xfffff000;
			}
			d += (rt2 - 1);

			pKoma->SetVarControl(block,d);
			UpdateMyWindow();
		}
	}
}




int CFilmDoc::GetBGM(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return -1;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return -1;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return -1;

	return pKoma->GetBGMNumber();
}


int CFilmDoc::GetFrameTime(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return -1;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return -1;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return -1;

	return pKoma->GetFrameTime();
}


void CFilmDoc::OnChangeKomaName(int n)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm != NULL)
	{
		int kosuu = pFilm->GetObjectKosuu();
		if (n == -1) n = pFilm->GetNowSelectNumber();

		if ((n>=0) && (n<kosuu))
		{
			CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(n));
			if (pKoma != NULL)
			{
				LPSTR oldText = pKoma->GetKomaName();
				if (oldText == NULL)
				{
					oldText = m_defaultKomaName;
				}

				LPSTR newText = m_input->GetText(oldText);
				if (newText != NULL)
				{

					CheckAndGetUndo(pFilm,n,n);

					pKoma->SetKomaName(newText);
					m_app->KomaIsChanged();
					m_app->FilmIsChanged();
					m_app->SetModify();
					UpdateMyWindow();
				}
			}
		}
	}
}


void CFilmDoc::OnClickFilm(int n,WPARAM wParam)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm != NULL)
	{
		int kosuu = pFilm->GetObjectKosuu();
		if ((n>=0) && (n<=kosuu))
		{
			ClearUndo();
//			CheckAndGetUndo(pFilm,n,n);

			if (wParam & (MK_CONTROL | MK_SHIFT))
			{
				pFilm->SetSelectSubNumber(n);
			}
			else
			{
				pFilm->SetSelectNumber(n);
			}

			m_app->KomaIsChanged();
		}
	}
}


CFilmData* CFilmDoc::GetNowSelectFilm(void)
{
	return m_app->GetNowSelectFilm();
}


void CFilmDoc::InitMiniPicTable(void)
{
	//たりないときはつくるにゃ
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm != NULL)
	{
		int k = pFilm->GetObjectKosuu();
		if (k>m_miniPicKosuu)
		{
			AddMiniPicBuffer(k-m_miniPicKosuu);
		}
	}


	for (int i=0;i<m_miniPicKosuu;i++)
	{
		m_miniPic[i]->SetFlag(FALSE);
	}
}


void CFilmDoc::InsertMiniPic(int place, int n)
{
	if (n<1) return;	//error

	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	int kosuu = pFilm->GetObjectKosuu();

	if ((place<0) || (place>kosuu)) return;	//error

	if (kosuu > m_miniPicKosuu)
	{
		int add = kosuu-m_miniPicKosuu;
		if (add<16) add = 16;
		AddMiniPicBuffer(add);
	}


	int i;
	for (i=m_miniPicKosuu-place-1;i>=0;i--)
	{
		int tmp = m_miniPicNumberTable[i];
		m_miniPicNumberTable[i] = m_miniPicNumberTable[i+place];
		m_miniPicNumberTable[i+place] = tmp;

		CMiniPic* tmpPic = m_miniPic[i];
		m_miniPic[i] = m_miniPic[i+place];
		m_miniPic[i+place] = tmpPic;
	}

	for (i=0;i<n;i++)
	{
		m_miniPic[place+i]->SetFlag(FALSE);
	}
}



void CFilmDoc::DeleteMiniPic(int place, int n)
{
	if (n<1) return;	//error

	if ((place<0) || (place>=m_miniPicKosuu)) return;	//error
	if ((place+n)>m_miniPicKosuu) return;	//cannot

	for (int i=0;i<n;i++)
	{
		int tmp = m_miniPicNumberTable[i];
		m_miniPicNumberTable[i] = m_miniPicNumberTable[place+i];
		m_miniPicNumberTable[place+i] = tmp;

		CMiniPic* tmpPic = m_miniPic[i];
		m_miniPic[i] = m_miniPic[place+i];
		m_miniPic[place+i] = tmpPic;
	}
}




void CFilmDoc::AddMiniPicBuffer(int n, int place)
{
	int i;

	if ((place <0) || (place>m_miniPicKosuu)) place = m_miniPicKosuu;

	int newKosuu = m_miniPicKosuu + n;

	int* lpTable = new int[newKosuu];
	CMiniPic** lpMiniPic = new CMiniPic*[newKosuu];

	HWND hwnd = m_app->GetFrameHWND();

	//copy first
	for (i=0;i<place;i++)
	{
		*(lpTable+i) = m_miniPicNumberTable[i];
		*(lpMiniPic+i) = m_miniPic[i];
	}

	//clear center
	for (i=0;i<n;i++)
	{
		*(lpTable+place+i) = m_miniPicKosuu+i;
		*(lpMiniPic+place+i) = new CMiniPic(hwnd);
	}

	//copy last
	int k = m_miniPicKosuu - place;
	for (i=0;i<k;i++)
	{
		*(lpTable+place+n+i) = m_miniPicNumberTable[place+i];
		*(lpMiniPic+place+n+i) = m_miniPic[place+i];
	}


	DELETEARRAY(m_miniPic);
	DELETEARRAY(m_miniPicNumberTable);

	m_miniPic = lpMiniPic;
	m_miniPicNumberTable = lpTable;

	m_miniPicKosuu += n;
}




HBITMAP CFilmDoc::GetHBitmap(int placeNumber)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return NULL;
	int kosuu = pFilm->GetObjectKosuu();

	if ((placeNumber<0) || (placeNumber>=kosuu)) return NULL;

	CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(placeNumber));
	if (pKoma == NULL) return NULL;

	if (placeNumber>=m_miniPicKosuu)
	{
		int mk = placeNumber - m_miniPicKosuu + 1;
		if (mk<16) mk = 16;
		AddMiniPicBuffer(mk);
	}

	int n = m_miniPicNumberTable[placeNumber];
	CMiniPic* miniPic = m_miniPic[n];	

	if (miniPic->GetFlag() == FALSE)
	{
		pKoma->Make64x48Pic(miniPic->GetBuffer());
		miniPic->SetFlag();
	}

	return miniPic->GetHBitmap();
}


/*
HBITMAP CFilmDoc::CreateMyBitmap(LPVOID* lplpBuffer)
{
	HWND hwnd = m_app->GetFrameHWND();
	HDC hdc = GetDC(hwnd);

	BITMAPINFO bmi;

	ZeroMemory(&bmi,sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = 64;
	bmi.bmiHeader.biHeight = -48;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	HBITMAP hbitmap = CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,lplpBuffer,NULL,NULL);
	if (hbitmap == NULL)
	{
		//error
	}

	ReleaseDC(hwnd,hdc);

	return hbitmap;
}
*/


void CFilmDoc::ReCalcuScrollPara(void)
{
	((CFilmView*)m_view)->ReCalcuScrollPara();
}

BOOL CFilmDoc::CheckBufferExist(void)
{
	return m_komaBufferFlag;
}


int CFilmDoc::GetScrollObjectKosuu(int vh)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

//int k = pFilm->GetObjectKosuu();
//char mes[256];
//sprintf(mes,"[koma=%d]",k);
//OutputDebugString(mes);

	return pFilm->GetObjectKosuu() + 1;

//	char mes[256];
//	sprintf(mes,"rt=%d",rt);
//	OutputDebugString(mes);

//	return rt;
//	return pFilm->GetObjectKosuu();

	return 0;
}


int CFilmDoc::GetNowSelectNumber(void)
{
	CFilmData* pFilm = GetNowSelectFilm();
	if (pFilm == NULL) return -1;

	return pFilm->GetNowSelectNumber();

	return -1;
}

void CFilmDoc::ScrollToSelect(void)
{
	CFilmView* pView = (CFilmView*)m_view;
	CFilmData* pFilm = GetNowSelectFilm();

	int n = pFilm->GetNowSelectNumber();
	pView->ReCalcuScrollPara();
	pView->ScrollTo(n);
}

int CFilmDoc::GetDemoFlag(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	if (n == -1) n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetDemoFlag();
}


void CFilmDoc::PlayMusicTest(int n)
{
	m_app->PlayMusicTest(n);
}


int CFilmDoc::GetWindowNumber(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetWindowNumber();
}

int CFilmDoc::GetCursorNumber(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetCursorNumber();
}

int CFilmDoc::GetMouseNumber(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetMouseNumber();
}

int CFilmDoc::GetAutoMessage(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetAutoMessage();
}

int CFilmDoc::GetCannotClick(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetCannotClick();
}

int CFilmDoc::GetCannotSkip(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetCannotSkip();
}

int CFilmDoc::GetOptionOff(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetOptionOff();
}

int CFilmDoc::GetCutinFlag(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetCutinFlag();
}

int CFilmDoc::GetPreload(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetPreload();
}


int CFilmDoc::GetExpStatus(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(n);
	if (pKoma == NULL) return 0;

	return pKoma->GetExpStatus();
}

int CFilmDoc::GetVarControl(int n)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int koma = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((koma<0) || (koma>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(koma);
	if (pKoma == NULL) return 0;

	return pKoma->GetVarControl(n);
}

int CFilmDoc::CheckVarControl(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return 0;

	int koma = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();
	if ((koma<0) || (koma>=kosuu)) return 0;

	CKomaData* pKoma = pFilm->GetKoma(koma);
	if (pKoma == NULL) return 0;

	return pKoma->CheckVarControl();
}


CCase* CFilmDoc::GetNowSelectCaseObject(void)
{
	return GetNowSelectFilm();
}

void CFilmDoc::OnSelectNumber(int n)
{
	ClearUndo();

	m_app->KomaIsChanged();
}

void CFilmDoc::OnDeleteKey(void)
{
	if (m_app->GetConfig("deleteKeyKoma") == 0) return;
	OnDelete();
}

void CFilmDoc::OnEnterKey(void)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	int n = pFilm->GetNowSelectNumber();
	int kosuu = pFilm->GetObjectKosuu();

	if (n == kosuu)
	{
		OnNewKoma();
	}
	else
	{
		OnOverrap();
	}
}

void CFilmDoc::OnInsertKey(void)
{
	OnNewKoma();
}

void CFilmDoc::OnSpaceKey(void)
{
	m_app->ChangeWindowIfCan(GAMEMESSAGE_WINDOW);
}

void CFilmDoc::OnEscapeKey(void)
{
	m_app->ChangeWindowIfCan(FILMCASE_WINDOW);
}


BOOL CFilmDoc::CheckExistUndo(void)
{
	CUndoMemoryObject* undo = m_app->GetUndoObject();
	if (undo != NULL)
	{
		int undoType = undo->GetUndoType();
		if (undoType == UNDO_TYPE_KOMA)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFilmDoc::OnUndo(int n)
{
	BOOL f = FALSE;

	if (m_app->GetUndoMode())
	{
		CFilmData* pFilm = m_app->GetNowSelectFilm();
		if (pFilm == NULL) return FALSE;

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
					pFilm->DeleteObjectData(startN,numN);
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_DELETE)
				{
					for (int i=startN;i<=endN;i++)
					{
						pFilm->CreateObjectData(i);
						CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(i));
						pKoma->Init();
						pKoma->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_MODIFY)
				{
					for (int i=startN;i<=endN;i++)
					{
						CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(i));
						pKoma->Init();
						pKoma->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
			}
		}
	}




	if (f)
	{
		((CFilmView*)m_view)->ReCalcuScrollPara();
		m_app->SetModify();
		m_app->KomaIsChanged();
		m_view->MyInvalidateRect();
	}


	return f;
}

void CFilmDoc::CheckAndGetUndo(CFilmData* pFilm,int start,int end)
{
	if (pFilm == NULL) return;

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_KOMA,UNDO_DATA_MODIFY,start,end);
		for (int i=start;i<=end;i++)
		{
			CKomaData* pKoma = (CKomaData*)(pFilm->GetObjectData(i));
			if (pKoma != NULL)
			{
				pKoma->Save(NULL,undo);
			}
		}
	}
}

void CFilmDoc::ClearUndo(void)
{
	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear();
	}
}


/*_*/

