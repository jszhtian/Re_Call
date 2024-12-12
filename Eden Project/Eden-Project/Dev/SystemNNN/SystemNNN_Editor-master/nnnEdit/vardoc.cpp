//
// vardoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "windowlist.h"

#include "myinputdialog.h"


#include "case.h"

#include "varInitData.h"

#include "varview.h"

#include "vardoc.h"

#include "myapplicationBase.h"

#include "varSearchOrReplace.h"

#include "..\..\systemNNN\nnnUtilLib\namelist.h"


CVarDoc::CVarDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = VAR_WINDOW;

	m_varInitData = m_app->GetVarInitData();

	m_varMode = 3;
	m_scrapData[0] = 0;
	m_scrapColor = 0;

	m_scrapFlag = FALSE;

	wsprintf(m_varTypeNameSys,"sys");
	wsprintf(m_varTypeNameCom,"com");
	wsprintf(m_varTypeNameGame,"Game");
	wsprintf(m_varTypeNamePlay,"Play");
	wsprintf(m_varTypeNameWork,"worksys");



	for (int i=0;i<10;i++)
	{
		m_nowSelectNumber[i] = 0;
	}


	ReCalcuVarKosuu();


//	int varType = 1;
	int varKosuu = m_app->GetVarKosuu();
	//load mark color
	m_markChangeFlag = FALSE;
	int varType = m_app->GetVarType();
	
	
	
	LPSTR varmarkFilename = m_app->GetVarMarkFilename();

	FILE* file = CMyFile::Open(varmarkFilename,"rb");
	if (file == NULL)
	{
		LPSTR varmarkSetupFilename = m_app->GetVarMarkSetupFilename();
		file = CMyFile::Open(varmarkSetupFilename,"rb");
		m_markChangeFlag = TRUE;
	}

	if (file != NULL)
	{
		fread(m_markColor,sizeof(int),varKosuu,file);
		fclose(file);
	}
	else
	{
		for (int i=0;i<varKosuu;i++)
		{
			m_markColor[i] = 0;
		}

		m_markChangeFlag = TRUE;
	}


	m_view = new CVarView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

//	m_view->SetViewWindowText("変数");
//	m_view->MoveViewWindow(443,528,GetSystemMetrics(SM_CXVSCROLL)+512,24+24*5);

	m_input = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance,64);
	m_search = new CVarSearchOrReplace(m_app);
}



CVarDoc::~CVarDoc()
{
	End();
}

void CVarDoc::End(void)
{
	ENDDELETECLASS(m_search);
	ENDDELETECLASS(m_input);

//	if (m_markChangeFlag) SaveMarkColor();
}


//void CVarDoc::OnCloseButton(void)
//{
//
//}


void CVarDoc::OnClickModeButton(int md)
{
	if ((md<0) || (md>=(3+m_varBlockKosuu))) return;
	if (md == m_varMode) return;

	m_varMode = md;
//OutputDebugString("[ClickModeButton]");
	((CVarView*)m_view)->ReCalcuScrollPara();
	m_view->MyInvalidateRect();
}

void CVarDoc::OnClickVar(int n)
{
	int md = GetVarMode();
	if (n<0) return;
	if (n>GetVarKosuu(md)) return;

	if (n == GetNowSelectNumber0(md)) return;

	m_nowSelectNumber[md] = n;

	m_view->MyInvalidateRect();
}


int CVarDoc::GetVarMode(void)
{
	return m_varMode;
}


int CVarDoc::GetVarKosuu(int md)
{
	if (md == -1) md = GetVarMode();
	return m_varKosuu[md];
}

int CVarDoc::GetVarStart(int md)
{
	if (md == -1) md = GetVarMode();
	return m_varStart[md];
}


int CVarDoc::GetNowSelectNumber0(int md)
{
	if (md == -1) md = GetVarMode();
	return m_nowSelectNumber[md];
}


LPSTR CVarDoc::GetVarName(int n, int md)
{
	if (md == -1) md = GetVarMode();

	n += m_varStart[md];
	return m_app->GetVarName(n);
}

void CVarDoc::OnChangeVarName(int n, int md)
{
	if (md == -1) md = GetVarMode();
	if (md == 0) return;

	n += m_varStart[md];


	//dialogをひらいて入力させる
	LPSTR name = m_input->GetText(m_app->GetVarName(n));
	if (name == NULL) return;

	int ln = strlen(name);
	if (ln>62)
	{
		MessageBox(m_app->GetFrameHWND(),"名前が31文字を超えていますにゃ","ERROR",MB_OK);
		return;
	}

	if (ln > 0)
	{
		//同一名称チェック

		CNameList* var = m_app->GetNameList();

		int found = var->SearchName(name);
		if (found != -1)
		{
			MessageBox(m_frameHWND,"同じ変数名がありますにゃ","ERROR",MB_OK | MB_ICONEXCLAMATION);
			return;
		}
	}

	m_app->SetVarName(n,name);
	m_app->VarIsChanged();
}


void CVarDoc::OnSaveButton(void)
{
	BOOL er1 = m_app->SaveVarName();
	BOOL er2 = m_app->SaveVarInitData();

	if (er1 && er2)
	{
		MessageBox(m_frameHWND,"変数名、初期値をSaveしましたにゃ","OK",MB_OK);
		return;
	}

	if (er1 == FALSE)
	{
		MessageBox(m_frameHWND,"変数名をSaveでエラー","OK",MB_OK | MB_ICONEXCLAMATION );
	}

	if (er2 == FALSE)
	{
		MessageBox(m_frameHWND,"変数初期値をSaveでエラー","OK",MB_OK | MB_ICONEXCLAMATION );
	}
}

void CVarDoc::OnCutButton(void)
{
	int md = GetVarMode();
	if (md == 0) return;

	//get data
	LPSTR name = GetVarName(GetNowSelectNumber0());
//	if (*name == 0) return;

	int ln = strlen(name);
	if (ln>62) ln = 62;
	if (ln>0) memcpy(m_scrapData,name,ln);
	m_scrapData[ln] = 0;
	m_scrapData[ln+1] = 0;

	//shift data
	int sel = GetNowSelectNumber0(GetVarMode());
	CNameList* var = m_app->GetNameList();

	int start = m_varStart[md];
	int blockKosuu = m_varKosuu[md];

	m_scrapColor = m_markColor[start+sel];

	if (sel<blockKosuu-1)
	{
		var->MoveBlock(start+sel+1,start+sel,blockKosuu-1-sel);
		memmove(&m_markColor[start+sel],&m_markColor[start+sel+1],(blockKosuu-1-sel)*sizeof(int));
	}


	//clear last
	m_app->SetVarName(start+blockKosuu-1,"");
	m_markColor[start+blockKosuu-1] = 0;

	m_scrapFlag = TRUE;
	m_view->MyInvalidateRect();
}


void CVarDoc::OnCopyButton(void)
{
	int md = GetVarMode();
	if (md == 0) return;

	LPSTR name = GetVarName(GetNowSelectNumber0());

	int start = m_varStart[md];
	int sel = GetNowSelectNumber0(GetVarMode());
	m_scrapColor = m_markColor[start+sel];


	int ln = strlen(name);
	if (ln>62) ln = 62;
	if (ln>0)memcpy(m_scrapData,name,ln);
	m_scrapData[ln] = 0;
	m_scrapData[ln+1] = 0;


	m_scrapFlag = TRUE;
	m_view->MyInvalidateRect();
}


void CVarDoc::OnPasteButton(void)
{
	int md = GetVarMode();

	if (md == 0) return;
	if (m_scrapFlag == FALSE) return;

	int start = m_varStart[md];
	int blockKosuu = m_varKosuu[md];

	int sel = GetNowSelectNumber0(GetVarMode());
	if ((sel<0) || (sel>=blockKosuu)) return;

	CNameList* var = m_app->GetNameList();

	if ((*m_scrapData) != 0)
	{
		int found = var->SearchName(m_scrapData);
		if (found != -1)
		{
			MessageBox(m_frameHWND,"同じ変数名がありますにゃ","ERROR",MB_OK | MB_ICONEXCLAMATION);
			return;
		}
	}

	if (sel<blockKosuu-1)
	{
		var->MoveBlock(start+sel,start+sel+1,blockKosuu-1-sel);
		memmove(&m_markColor[start+sel+1],&m_markColor[start+sel],(blockKosuu-1-sel)*sizeof(int));
	}

	m_app->SetVarName(start+sel,m_scrapData);
	m_markColor[start+sel] = m_scrapColor;

	m_view->MyInvalidateRect();
}


void CVarDoc::OnSearchButton(BOOL replaceFlag)
{
	int md = GetVarMode();
	int blockStartNumber = GetVarStart(md);
	int blockEndNumber = blockStartNumber + GetVarKosuu(md) - 1;
	int nowSelectNumber = m_nowSelectNumber[md] + blockStartNumber;
	m_search->SearchOrReplace(replaceFlag,nowSelectNumber,blockStartNumber,blockEndNumber);
}


LPSTR CVarDoc::GetScrap(void)
{
	if (m_scrapFlag == FALSE) return NULL;

	return m_scrapData;
}



int CVarDoc::GetScrollObjectKosuu(int vh)
{
	return m_varKosuu[m_varMode];


	return 0;
}

int CVarDoc::GetNowSelectNumber(void)
{
	return GetNowSelectNumber0();
}


void CVarDoc::OnDeleteMark(int n)
{
	int md = GetVarMode();
	n += m_varStart[md];
	m_markColor[n] = 0;
	m_view->MyInvalidateRect();
}

void CVarDoc::OnChangeMarkColor(int n,int color,int backColor)
{
	int md = GetVarMode();
	n += m_varStart[md];

	int orgColor = m_markColor[n] & 0xff;
	int orgBackColor = (m_markColor[n] >> 8)& 0xff;

	if (color != -1) orgColor = color;
	if (backColor != -1) orgBackColor = backColor;


	m_markColor[n] = orgColor | (orgBackColor << 8);

	m_view->MyInvalidateRect();
}


void CVarDoc::SaveMarkColor(void)
{
	int varKosuu = m_app->GetVarKosuu();

	LPSTR varmarkFilename = m_app->GetVarMarkFilename();

	FILE* file = CMyFile::Open(varmarkFilename,"wb");
	if (file != NULL)
	{
		fwrite(m_markColor,sizeof(int),varKosuu,file);
		fclose(file);
	}
	m_markChangeFlag = FALSE;
}

int CVarDoc::GetMarkColor(int n,int md)
{
	if (md == -1) md = GetVarMode();
	n += m_varStart[md];
	return m_markColor[n];
}


int CVarDoc::GetBlockKosuu(void)
{
	if (m_view == NULL) return 1;
	return m_view->GetBlockKosuu();
}


void CVarDoc::ReCalcuVarKosuu(void)
{
	int varType = 1;
	int varStart = 300;
	int varKosuu = 700;

	if (varType == 0)
	{
		m_varStart[0] = 0;
		m_varKosuu[0] = 100;
		m_varStart[1] = 100;
		m_varKosuu[1] = 100;
		m_varStart[2] = 200;
		m_varKosuu[2] = 100;
	}
	else
	{
		m_varStart[0] = 0;
		m_varKosuu[0] = 100;
		m_varStart[1] = 100;
		m_varKosuu[1] = 100;
		m_varStart[2] = 200;
		m_varKosuu[2] = 1000;
		varStart = 1200;
		varKosuu = 1000;
	}

	m_varBlockKosuu = m_app->GetConfig("varBlockNumber");
	if (m_varBlockKosuu < 1)
	{
		m_varBlockKosuu = 1;
		m_varStart[3] = varStart;
		m_varKosuu[3] = varKosuu;
		m_nowSelectNumber[3] = 0;
	}
	else
	{
		if (m_varBlockKosuu>7) m_varBlockKosuu = 7;

		int start = varStart;
		for (int j=0;j<m_varBlockKosuu;j++)
		{
			char name[256];
			wsprintf(name,"varBlock%d",j+1);
			int kosuu = m_app->GetConfig(name);

			m_varStart[3+j] = start;
			m_varKosuu[3+j] = kosuu;
			start += kosuu;
			m_nowSelectNumber[3+j] = 0;
		}
	}

	if (m_varMode > m_varBlockKosuu+2) m_varMode = m_varBlockKosuu+2;
	
	if (m_view != NULL)
	{
		((CVarView*)m_view)->VarBlockIsChanged();
	}
}




void CVarDoc::SelectPrevObject(int prev)
{
	int md = GetVarMode();
	int kosuu = m_varKosuu[md];
	int sel = GetNowSelectNumber();
	sel += prev;
	if (sel<0) sel = 0;
//	if (sel > 0) sel--;
	OnClickVar(sel);
}


void CVarDoc::SelectNextObject(int next)
{
	int md = GetVarMode();
	int kosuu = m_varKosuu[md];
	int sel = GetNowSelectNumber();
	sel += next;
	if (sel>=kosuu) sel = kosuu-1;
//	if (sel < kosuu-1) sel++;
	OnClickVar(sel);
}

void CVarDoc::OnEnterKey(void)
{
	int n = GetNowSelectNumber();
	OnChangeVarName(n);
}

void CVarDoc::OnDeleteKey(void)
{
	if (m_app->GetConfig("deleteKeyVar") == 0) return;
	OnDelete();
}

void CVarDoc::OnInsertKey(void)
{
}

void CVarDoc::OnLeftKey(void)
{
	int md = GetVarMode();
	if (md>0)
	{
		md--;
		OnClickModeButton(md);
	}
}

void CVarDoc::OnRightKey(void)
{
	int md = GetVarMode();
	md++;
	if ((md<0) || (md>=(3+m_varBlockKosuu))) return;
	OnClickModeButton(md);
}

void CVarDoc::OnDelete(void)
{
	if (m_app->GetWarningDelVar())
	{
		//警告表示と確認
	}



	//削除
}



void CVarDoc::SetSelectVarNumber(int varNumber)
{
	int varBlock = 0;
	for (int i=0;i<3+m_varBlockKosuu;i++)
	{
		int start = GetVarStart(i);
		int kosuu = GetVarKosuu(i);
		if (varNumber >= start)
		{
			if (varNumber < (start+kosuu))
			{
				varBlock = i;
				break;
			}
		}
	}

	m_varMode = varBlock;
	int start2 = GetVarStart(m_varMode);
	m_nowSelectNumber[m_varMode] = varNumber - start2;

//	ReCalcuVarKosuu();
	if (m_view != NULL)
	{
		((CVarView*)m_view)->VarBlockIsChanged();
		((CVarView*)m_view)->ScrollTo(varNumber - start2);
	}


	m_view->MyInvalidateRect();
}

void CVarDoc::ReplaceVarName(int varNumber,LPSTR searchString,LPSTR replaceString)
{
	if (strcmp(searchString,replaceString) == 0) return;
	if ((*searchString) == 0) return;

	CNameList* varList = m_app->GetNameList();

	LPSTR orgVarName = m_app->GetVarName(varNumber);

	char newVarName[1024];
	int ln = strlen(orgVarName);
	int targetStart = 0;
	int n = 0;
	BOOL startFlag = FALSE;

	int repLen = strlen(replaceString);
	int searchLen = strlen(searchString);

	while (n<ln)
	{
		LPSTR found = strstr(orgVarName+n,searchString);
		if (found != NULL)
		{
			if (startFlag == FALSE)
			{
				int startSize = found - orgVarName;
				if (startSize > 0)
				{
					memcpy(newVarName,orgVarName,startSize);
					targetStart += startSize;
				}
				startFlag = TRUE;
			}

			if (repLen > 0)
			{
				memcpy(newVarName+targetStart,replaceString,repLen);
				targetStart += repLen;
				n += searchLen;
			}

			//check size
			if (targetStart > 62)
			{
				MessageBox(m_frameHWND,"変換後の変数名が長すぎます","OK",MB_OK | MB_ICONEXCLAMATION );
				return;
			}
		}
		else
		{
			break;
		}
	}

	if (startFlag == FALSE) return;//error not found

	//nokori copy
	if (n<ln)
	{
		memcpy(newVarName+targetStart,orgVarName+n,(ln-n));
		targetStart += (ln-n);
	}
	newVarName[targetStart] = 0;


	//check size
	if (targetStart > 62)
	{
		MessageBox(m_frameHWND,"変換後の変数名が長すぎます","OK",MB_OK | MB_ICONEXCLAMATION );
		return;
	}

	//check same
	int check = varList->SearchName(newVarName);
	if (check != -1)
	{
		MessageBox(m_frameHWND,"変換後に同じ変数名がありますにゃ","ERROR",MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	m_app->SetVarName(varNumber,newVarName);
	m_app->VarIsChanged();
}



LPSTR CVarDoc::GetVarInitDataString(int n, int md)
{
	if (md == -1) md = GetVarMode();
	n += m_varStart[md];
	if (n < 100) return m_varTypeNameSys;
	int type = m_app->GetVarType();

	if (type == 0)
	{
		if ((n >= 200) && (n < 300)) return m_varTypeNameGame;

		wsprintf(m_varTypeNameWork, "%d", m_varInitData->GetData(n));
		return m_varTypeNameWork;

	}

	if ((n >= 200) && (n < 1200)) return m_varTypeNameGame;
	wsprintf(m_varTypeNameWork, "%d", m_varInitData->GetData(n));
	return m_varTypeNameWork;


}

void CVarDoc::OnChangeInitData(int n)
{
	BOOL f = TRUE;

	int md = GetVarMode();
	n += m_varStart[md];

	if (n<100) f = FALSE;

	int type = m_app->GetVarType();

	if (type == 0)
	{
		if ((n >= 200) && (n < 300)) f = FALSE;
	}
	else
	{
		if ((n >= 200) && (n < 1200)) f = FALSE;
	}

	if (f == FALSE)
	{
		MessageBox(m_frameHWND,"システム変数、ゲーム変数には初期値は設定できません","ERROR",MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int oldData = m_varInitData->GetData(n);
	int newData = oldData;

	if (m_input->GetNumber(oldData,&newData,"初期値設定"))
	{
		m_varInitData->SetData(n,newData);
		m_app->VarIsChanged();
	}
}


/*_*/


