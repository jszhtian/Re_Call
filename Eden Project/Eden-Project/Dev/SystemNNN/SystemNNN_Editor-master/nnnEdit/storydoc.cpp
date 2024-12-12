//
// storydoc.cpp
//


#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"


#include "windowlist.h"

#include "case.h"

#include "storyview.h"

#include "storydoc.h"

#include "storydata.h"
#include "storyBookData.h"

#include "filmData.h"
#include "filmCaseData.h"

#include "myapplicationBase.h"

#include "myinputdialog.h"

#include "commanddata.h"
#include "commanddatatype.h"

#include "undoMemoryObject.h"
#include "selectdialog.h"


int CStoryDoc::m_commandToTypeTable[]=
{
	ID_MENU_COMMAND_NOP,				COMMANDDATATYPE_NOP,
	ID_MENU_COMMAND_IF,					COMMANDDATATYPE_IF,
	ID_MENU_COMMAND_ELSIF,				COMMANDDATATYPE_ELSIF,
	ID_MENU_COMMAND_ELSE,				COMMANDDATATYPE_ELSE,

	ID_MENU_COMMAND_SELECT1,			COMMANDDATATYPE_SELECT,
	ID_MENU_COMMAND_SELECT2,			COMMANDDATATYPE_SELECT,
	ID_MENU_COMMAND_SELECT3,			COMMANDDATATYPE_SELECT,
	ID_MENU_COMMAND_SELECT4,			COMMANDDATATYPE_SELECT,

	ID_MENU_COMMAND_STORY,				COMMANDDATATYPE_STORY,
	ID_MENU_COMMAND_FILM,				COMMANDDATATYPE_FILM,

//	ID_MENU_COMMAND_MIDI_START,			COMMANDDATATYPE_BGM_MIDI,
//	ID_MENU_COMMAND_MIDI_STARTONCE,		COMMANDDATATYPE_BGM_MIDI,
//	ID_MENU_COMMAND_MIDI_STOP,			COMMANDDATATYPE_BGM_MIDI,
//	ID_MENU_COMMAND_CD_START,			COMMANDDATATYPE_BGM_CD,
//	ID_MENU_COMMAND_CD_STARTONCE,		COMMANDDATATYPE_BGM_CD,
//	ID_MENU_COMMAND_CD_STOP,			COMMANDDATATYPE_BGM_CD,

//	ID_MENU_COMMAND_SYSTEM_NORMAL,		COMMANDDATATYPE_SYSTEM,
//	ID_MENU_COMMAND_SYSTEM_ENDING,		COMMANDDATATYPE_SYSTEM,
//	ID_MENU_COMMAND_SYSTEM_STAFFROLL,	COMMANDDATATYPE_SYSTEM,

	ID_MENU_COMMAND_CALCU,				COMMANDDATATYPE_CALCU,

	ID_MENU_COMMAND_RET,				COMMANDDATATYPE_RET,
	ID_MENU_COMMAND_NEXT,				COMMANDDATATYPE_NEXT,
	ID_MENU_COMMAND_END,				COMMANDDATATYPE_END,
	ID_MENU_COMMAND_EXIT,				COMMANDDATATYPE_EXIT,

	ID_MENU_COMMAND_SCRIPT,				COMMANDDATATYPE_SCRIPT,
	
	ID_MENU_COMMAND_FUNCTION,			COMMANDDATATYPE_SYSTEM,
	ID_MENU_COMMAND_SYSTEMCOMMAND,		COMMANDDATATYPE_SYSTEM,

	ID_MENU_COMMAND_WHILE,				COMMANDDATATYPE_WHILE,

	ID_MENU_COMMAND_DEBUG,				COMMANDDATATYPE_DEBUG,

	ID_MENU_COMMAND_JUMP,				COMMANDDATATYPE_JUMP,
	ID_MENU_COMMAND_SUBSCRIPT,			COMMANDDATATYPE_SUBSCRIPT,
//	ID_MENU_COMMAND_SUBSUBSCRIPT,		COMMANDDATATYPE_SUBSUBSCRIPT,
	-1,-1,
};






CStoryDoc::CStoryDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = STORY_WINDOW;

	m_view = new CStoryView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();
//	m_view->SetViewWindowText("シナリオ");
//	m_view->MoveViewWindow(836,480,480+GetSystemMetrics(SM_CXVSCROLL),24+32*10);

	m_mesBufferKosuu = 0;
	m_ppMesBuffer = NULL;
	CreateMessageBuffer(256);

	m_inputDialog = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance,4096);
	m_selectDialog = new CSelectDialog(m_app->GetFrameHWND(),m_hInstance);
}



CStoryDoc::~CStoryDoc()
{
	End();
}

void CStoryDoc::End(void)
{
	ENDDELETECLASS(m_selectDialog);
	ENDDELETECLASS(m_inputDialog);
	DELETEARRAY(m_ppMesBuffer);
}




void CStoryDoc::OnNewCommand(int n)
{


	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;
	int kosuu = pStory->GetObjectKosuu();

	if (n == -1) n = pStory->GetNowSelectNumber();
	if ((n<0) || (n>kosuu)) return;

//char mes[256];
//wsprintf(mes,"NewCommand :%d\x00d\x00a",n);
//m_app->DebugLog(mes);

	CheckAndGetUndoAll();
//	if (m_app->GetUndoMode())
//	{
//		CUndoMemoryObject* undo = m_app->GetUndoObject();
//		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
//	}


	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand != NULL)
	{
		int typ = pCommand->GetCommandType();
		int level = pCommand->GetLevel();

		int newType = COMMANDDATATYPE_NOP;

		if ((typ == COMMANDDATATYPE_ELSIF) || (typ == COMMANDDATATYPE_ELSE))
		{
			newType = COMMANDDATATYPE_ELSIF;
		}

		if (typ == COMMANDDATATYPE_ENDIF)
		{
			newType = COMMANDDATATYPE_ELSE;
			
			//change before type
			CCommandData* pCommand0 = pStory->GetBackSameLevel(n);
			if (pCommand0 != NULL)
			{
				if (pCommand0->GetCommandType() == COMMANDDATATYPE_ELSE)
				{
					pCommand0->ChangeCommandType(COMMANDDATATYPE_ELSIF);
				}
			}
		}


		int n2 = n;
		if (GetAsyncKeyState(VK_CONTROL) & 1)
		{
			if (newType == COMMANDDATATYPE_NOP)
			{
				if (pCommand->GetSubKosuu() == 0)
				{
					n2++;
				}
			}
		}





		pStory->CreateObjectData(n2);
		CCommandData* pCommand2 = (CCommandData*)(pStory->GetCommand(n2));
		pCommand2->ChangeCommandType(newType);
		pCommand2->Init((LPVOID)newType);
		pCommand2->SetLevel(level);

		if (newType != COMMANDDATATYPE_NOP)
		{
			pStory->CreateObjectData(n+1);
			CCommandData* pCommand3 = (CCommandData*)(pStory->GetCommand(n+1));
			pCommand3->ChangeCommandType(COMMANDDATATYPE_NOP);
			pCommand3->Init(COMMANDDATATYPE_NOP);
			pCommand3->SetLevel(level+1);

			pCommand2->SetSubKosuu(1);
		}


		if (level>0)
		{
			CCommandData* pParentCommand = pStory->GetParentObject(n);
			if (pParentCommand != NULL)
			{
				int subKosuu = pParentCommand->GetSubKosuu();
				pParentCommand->SetSubKosuu(subKosuu+1);
			}
		}


		((CStoryView*)m_view)->ReCalcuScrollPara();

		m_app->SetModify();
		m_app->CommandIsChanged();

		return;
	}


	pStory->CreateObjectData(n);
	pCommand = (CCommandData*)(pStory->GetObjectData(n));
	pCommand->Init(COMMANDDATATYPE_NOP);

	((CStoryView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->CommandIsChanged();

//	m_view->MyInvalidateRect();	//dummy
}


void CStoryDoc::OnCutCommand(int n)
{
}

void CStoryDoc::OnCopyCommand(int n)
{
}

void CStoryDoc::OnPasteCommand(int n)
{
}

void CStoryDoc::OnDelete(int n)
{

	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;
	int kosuu = pStory->GetObjectKosuu();

	if (n == -1) n = pStory->GetNowSelectNumber();

	if ((n<0) || (n>=kosuu)) return;

//char mes[256];
//wsprintf(mes,"DeleteCommand :%d\x00d\x00a",n);
//m_app->DebugLog(mes);

	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand == NULL) return;

	if (m_app->GetWarningDelCommand())
	{
		int rt = MessageBox(m_frameHWND,"本当に削除しますか","削除の確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (rt != IDYES) return;
	}


	int typ = pCommand->GetCommandType();
	if (typ == COMMANDDATATYPE_ENDIF) return;
	int level = pCommand->GetLevel();


	CheckAndGetUndoAll();
//	if (m_app->GetUndoMode())
//	{
//		CUndoMemoryObject* undo = m_app->GetUndoObject();
//		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
//	}



	//delete with child
	pStory->DeleteWithChild(n);

	//change parent have
	if (level > 0)
	{
		int p = SearchParent(n,level);
		if (p != -1)
		{
			SubParentHave(p);
		}
	}


/*
	int level = pCommand->GetLevel();
	if (level != 0)
	{
		CCommandData* pParentCommand = pStory->GetParentObject(n);
		if (pParentCommand == NULL) return;


		int subKosuu = pParentCommand->GetSubKosuu();
		if (subKosuu<=1) return;


		pStory->DeleteObjectData(n);
		pParentCommand->SetSubKosuu(subKosuu-1);

		((CStoryView*)m_view)->ReCalcuScrollPara();

		m_app->SetModify();
		m_app->CommandIsChanged();
		return;
	}

	//delete main
//	int typ = pCommand->GetCommandType();
	int subKosuu = pCommand->GetSubKosuu();

	pStory->DeleteObjectData(n,1 + subKosuu);
*/

	((CStoryView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->CommandIsChanged();
}

void CStoryDoc::SubParentHave(int n,int delta)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;
	int kosuuMax = pStory->GetObjectKosuu();
	if ((n<0) || (n>=kosuuMax)) return;

	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand == NULL) return;

	int subKosuu = pCommand->GetSubKosuu();
	subKosuu -= delta;
	if (subKosuu<0) subKosuu = 0;
	pCommand->SetSubKosuu(subKosuu);

	if (subKosuu == 0)
	{
		int level = pCommand->GetLevel();
		pStory->DeleteWithChild(n);

		if (level > 0)
		{
			int p = SearchParent(n,level);

			if (p != -1)
			{
				SubParentHave(p);
			}
		}
	}
}


int CStoryDoc::SearchParent(int n,int level)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return -1;

	int kosuuMax = pStory->GetObjectKosuu();
	if ((n<1) || (n>kosuuMax)) return -1;

	for (int i=n-1;i>=0;i--)
	{
		CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(i));
		if (pCommand != NULL)
		{
			int level2 = pCommand->GetLevel();
			if (level2 == level-1)
			{
				return i;
			}
		}
	}

	return -1;
}





void CStoryDoc::OnBackCommand(int n)
{
}

void CStoryDoc::OnFowardCommand(int n)
{
}

void CStoryDoc::OnJumpCommand(int n)
{
}


//void CStoryDoc::OnCloseButton(void)
//{
//
//	OutputDebugString("OnCLOSEButton() in DocDoc\n");	//override test
//}

void CStoryDoc::ReCalcuScrollPara(void)
{
	((CStoryView*)m_view)->ReCalcuScrollPara();
}

void CStoryDoc::OnClickCommand(int n)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	int kosuu = pStory->GetObjectKosuu();
	
	if ((n>=0) && (n<=kosuu))
	{
		ClearUndo();
		pStory->SetSelectNumber(n);
		m_app->CommandIsChanged();
	}
}


void CStoryDoc::OnChangeType(int n, int cmd)
{


	int typ = CommandToType(cmd);
	if (typ == -1) return;

	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	int kosuu = pStory->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand == NULL) return;

	int oldType = pCommand->GetCommandType();

	//変更できないもの
	if (oldType == COMMANDDATATYPE_ENDIF) return;
	if (oldType == COMMANDDATATYPE_IF) return;
	if (oldType == COMMANDDATATYPE_SELECT) return;
	if (oldType == COMMANDDATATYPE_WHILE) return;



	//特定のものからしか変更できないもの

	if (oldType == COMMANDDATATYPE_ELSIF)
	{
		if (typ != COMMANDDATATYPE_ELSE) return;

		//最後の１このelsifみ許可
		CCommandData* pCommand99 = pStory->GetNextSameLevel(n);
		if (pCommand99 == NULL) return;

		if (pCommand99->GetCommandType() != COMMANDDATATYPE_ENDIF) return;
	}


	if (typ == COMMANDDATATYPE_ELSE)
	{
		if (oldType != COMMANDDATATYPE_ELSIF) return;

		//最後の１このelsifみ許可
		CCommandData* pCommand99 = pStory->GetNextSameLevel(n);
		if (pCommand99 == NULL) return;

		if (pCommand99->GetCommandType() != COMMANDDATATYPE_ENDIF) return;
	}


	if (oldType == COMMANDDATATYPE_ELSE)
	{
		if (typ != COMMANDDATATYPE_ELSIF) return;
	}

	if (typ == COMMANDDATATYPE_ELSIF)
	{
		if (oldType != COMMANDDATATYPE_ELSE) return;
	}



	if ((typ == COMMANDDATATYPE_SELECT) || (typ == COMMANDDATATYPE_IF) || (typ == COMMANDDATATYPE_WHILE))
	{
		if (oldType == COMMANDDATATYPE_ELSIF) return;
		if (oldType == COMMANDDATATYPE_ELSE) return;
	}




	if ((typ == COMMANDDATATYPE_SCRIPT) || (typ == COMMANDDATATYPE_END))
	{
		if (m_app->CheckIamSystemScript() == FALSE)
		{
			return;
		}
	}

	if (typ == COMMANDDATATYPE_JUMP)
	{
		if (m_app->CheckIamSystemScript())
		{
			return;
		}
	}

	if ((typ == COMMANDDATATYPE_NEXT) || (typ == COMMANDDATATYPE_FILM))
	{
		if (m_app->CheckIamSystemScript())
		{
			return;
		}
	}

	if (typ ==COMMANDDATATYPE_EXIT)
	{
		if (m_app->CheckIamSystemScript())
		{
//			return;
		}
	}

	if (cmd == ID_MENU_COMMAND_SYSTEMCOMMAND)
	{
		if (m_app->CheckIamSystemScript())
		{
//			return;
		}
	}

	if (typ == COMMANDDATATYPE_SELECT)
	{
		if (m_app->CheckIamSystemScript())
		{
//			return;/test SELECT
		}
	}


	if (typ == COMMANDDATATYPE_RET)
	{
		if (pStory->CheckIamTop()) return;
	}



//char mes[256];
//wsprintf(mes,"ChangeCommand :%d,%d\x00d\x00a",n,typ);
//m_app->DebugLog(mes);

	CheckAndGetUndoAll();
//	if (m_app->GetUndoMode())
//	{
//		CUndoMemoryObject* undo = m_app->GetUndoObject();
//		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
//	}


	int para = CommandToPara(cmd);
	int subKosuu = CommandToSubKosuu(cmd);


	int oldLevel = pCommand->GetLevel();
	int oldPara = pCommand->GetPara();
	int oldSubKosuu = pCommand->GetSubKosuu();


	//if,else,elsifの変換は同数に
	if ((oldType == COMMANDDATATYPE_ELSIF) || (oldType == COMMANDDATATYPE_ELSE))
	{
		if ((typ == COMMANDDATATYPE_ELSIF) || (typ == COMMANDDATATYPE_ELSE))
		{
			subKosuu = oldSubKosuu;
		}
	}



	//下位レベルの個数により増減
	if (subKosuu > oldSubKosuu)
	{
		//ついかの必要あり
		int have = pStory->GetSubTotalKosuu(n);
		int k = subKosuu-oldSubKosuu;
		int st = n + 1 + have;

		pStory->CreateObjectData(st,k);


		for (int i=0;i<k;i++)
		{
			CCommandData* pCommand1 = (CCommandData*)(pStory->GetObjectData(st+i));
			pCommand1->Init(COMMANDDATATYPE_NOP);
			pCommand1->SetLevel(oldLevel + 1);
		}
		pCommand->SetSubKosuu(subKosuu);
	}
	else if (subKosuu<oldSubKosuu)
	{
		for (int i=oldSubKosuu-1;i>=oldSubKosuu-subKosuu;i--)
		{
			pStory->DeleteChildBlock(n,i);
		}
		pCommand->SetSubKosuu(subKosuu);
	}

/*
	for (int p=0;p<pStory->GetObjectKosuu();p++)
	{
		CCommandData* pc = (CCommandData*)(pStory->GetObjectData(p));
		char mes[256];
		sprintf(mes,"[%d=level%d]",p,pc->GetLevel());
		OutputDebugString(mes);
	}
	OutputDebugString("\n");
*/

	pCommand->ChangeCommandType(typ,para);

	if (typ == COMMANDDATATYPE_IF)
	{
		int have2 = pStory->GetSubTotalKosuu(n);
		pStory->CreateObjectData(n+1+have2,1);

		CCommandData* pCommand2 = (CCommandData*)(pStory->GetObjectData(n+1+have2));
		pCommand2->Init((LPVOID)COMMANDDATATYPE_ENDIF);
		pCommand2->SetLevel(oldLevel);
	}

	pStory->SetSelectNumber(n);



	//film
	//story も選択させる？


	if (typ == COMMANDDATATYPE_SYSTEM)
	{
		if (cmd == ID_MENU_COMMAND_FUNCTION)
		{
			CNameList* plist = m_app->GetFunctionList();
			int rt = m_selectDialog->GetSelect(plist);
			if (rt == -1) rt = 0;

			pCommand->SetSystemFunction(rt);
		}


		if (cmd == ID_MENU_COMMAND_SYSTEMCOMMAND)
		{
			CNameList* plist = m_app->GetCommandList();
			int rt = m_selectDialog->GetSelect(plist);
			if (rt == -1) rt = 0;

			pCommand->SetSystemCommand(rt);
		}

	}

/*
	if (typ == COMMANDDATATYPE_BGM_MIDI)
	{
		if ((cmd == ID_MENU_COMMAND_MIDI_START) || (cmd == ID_MENU_COMMAND_MIDI_STARTONCE))
		{
			CNameList* plist = m_app->GetBGMList();
			int rt = m_selectDialog->GetSelect(plist);
			if (rt == -1) rt = 0;

			pCommand->SetBGMNumber(rt);
		}

//		int c = 0;
//		if (cmd == ID_MENU_COMMAND_MIDI_START) c = 0;
//		if (cmd == ID_MENU_COMMAND_MIDI_STARTONCE) c = 1;
//		if (cmd == ID_MENU_COMMAND_MIDI_STOP) c = -1;
//
//		pCommand->SetBgmKaisuu(c);
	}
*/

	if (typ == COMMANDDATATYPE_FILM)
	{
		int ft = m_app->GetFilmSelectDefault();
		if (ft == 1)
		{
			OnSelectCommand(n);
		}
		else if (ft == 2)
		{
			OnEditCommand(n);
		}
	}

	if (typ == COMMANDDATATYPE_STORY)
	{
		int st = m_app->GetStorySelectDefault();
		if (st == 1)
		{
			OnSelectCommand(n);
		}
		else if (st == 2)
		{
			OnEditCommand(n);
		}
	}

	if (typ == COMMANDDATATYPE_CALCU)
	{
		int st = m_app->GetCalcuSelectDefault();
		if (st == 1)
		{
			OnEditCommand(n);
		}
	}


	if (subKosuu != oldSubKosuu)
	{
		((CStoryView*)m_view)->ReCalcuScrollPara();
	}

	m_app->SetModify();
	m_app->CommandIsChanged();
}



void CStoryDoc::OnChangeSelectStory(int n)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	if (n == -1) n = pStory->GetNowSelectNumber();

	int kosuu = pStory->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand == NULL) return;

	int cmd = pCommand->GetCommandType();
	if (cmd != COMMANDDATATYPE_STORY) return;



	LPSTR text = pCommand->GetTextBuffer();
	if (text == NULL) return;

	ClearUndo();

	int s = m_app->SearchStory(text);
	if (s == -1)
	{
		//not found
		char mes[1024];
		wsprintf(mes,"存在していない名前ですにゃん。\n%s",text);
		MessageBox(m_app->GetFrameHWND(),mes,"ERROR",MB_OK);

		return;
	}

	m_app->JumpStory(s);
}


void CStoryDoc::OnChangeSelectID(int n)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	if (n == -1) n = pStory->GetNowSelectNumber();

	int kosuu = pStory->GetObjectKosuu();
	if ((n < 0) || (n >= kosuu)) return;

	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand == NULL) return;

	int cmd = pCommand->GetCommandType();
	if (cmd != COMMANDDATATYPE_SELECT) return;

	int oldID = pCommand->GetSelectMessageSerial();
	char old[256];
	sprintf_s(old, 256, "%d", oldID);

	int id = oldID;
	if (m_inputDialog->GetNumber(oldID,&id,"ID選択"))
	{
		if (id != oldID)
		{
			pCommand->SetSelectMessageSerial(id);
			m_app->SetModify();
			m_app->CommandIsChanged();
		}
	}
}


void CStoryDoc::OnEditCommand(int n)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	if (n == -1) n = pStory->GetNowSelectNumber();

	int kosuu = pStory->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;


	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand == NULL) return;


	CheckAndGetUndoAll();
//	if (m_app->GetUndoMode())
//	{
//		CUndoMemoryObject* undo = m_app->GetUndoObject();
//		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
//	}


	LPSTR mes = pCommand->GetTextBuffer();
	int commandType = pCommand->GetCommandType();

	if (commandType == COMMANDDATATYPE_ELSE)
	{
		CStoryView* pView = (CStoryView*)m_view;

		int cmd;

		int x = 0;
		int y = 0;

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(pView->GetHWND(),&pt);

		x = pt.x;
		y = pt.y;

		WPARAM wParam = 0;
		LPARAM lParam = (x & 0xffff) | ((y & 0xffff) << 16);

		cmd = pView->OpenPopupMenu(wParam,lParam,0,-1,1);
		
		if (cmd != 0)
		{
			pView->CheckChangeMarkColor(n,cmd);
			OnChangeType(n,cmd);
		}
		return;
	}



	int typ = pCommand->GetTextType();
	if (typ == 0) return;

	LPSTR newText = NULL;

	if ((typ == 1) || (typ == 2))
	{
		newText = m_inputDialog->GetText(mes);
	}
	else if (typ == 3)
	{
		newText = m_inputDialog->GetText2(mes);
	}


	if (newText != NULL)
	{
		if ((commandType == COMMANDDATATYPE_SCRIPT) || (commandType == COMMANDDATATYPE_JUMP) || (commandType == COMMANDDATATYPE_SUBSCRIPT))
		{
			if (m_app->SearchScript(newText) == -1)
			{
				MessageBox(m_frameHWND,newText,"プロジェクトに存在しません",MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}

		pCommand->SetTextBuffer(newText);
		m_app->SetModify();
		m_app->CommandIsChanged();
	}
}

void CStoryDoc::OnSelectCommand(int n)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	if (n == -1) n = pStory->GetNowSelectNumber();

	int kosuu = pStory->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;


	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pCommand == NULL) return;


	LPSTR mes = pCommand->GetTextBuffer();
	int commandType = pCommand->GetCommandType();
	int typ = pCommand->GetTextType();
	if (typ == 0) return;


	CheckAndGetUndoAll();
//	if (m_app->GetUndoMode())
//	{
//		CUndoMemoryObject* undo = m_app->GetUndoObject();
//		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
//	}

	LPSTR newText = NULL;

	if (commandType == COMMANDDATATYPE_STORY)
	{
		CStoryBookData* book = m_app->GetStoryBookData();
		int kosuu2 = book->GetObjectKosuu();
		CreateMessageBuffer(kosuu2);
	
		for (int i=0;i<kosuu2;i++)
		{
			CStoryData* story = (CStoryData*)(book->GetObjectData(i));
			*(m_ppMesBuffer+i) = story->GetMyName();
		}

		int sel = m_selectDialog->GetSelect(m_ppMesBuffer,kosuu2);
		if (sel == -1) return;
		newText = *(m_ppMesBuffer + sel);
	}
	else if (commandType == COMMANDDATATYPE_FILM)
	{
		CFilmCaseData* filmCase = m_app->GetFilmCaseData();
		int kosuu2 = filmCase->GetObjectKosuu();
		CreateMessageBuffer(kosuu2);
	
		for (int i=0;i<kosuu2;i++)
		{
			CFilmData* film = (CFilmData*)(filmCase->GetObjectData(i));
			*(m_ppMesBuffer+i) = film->GetMyName();
		}

		int sel = m_selectDialog->GetSelect(m_ppMesBuffer,kosuu2);
		if (sel == -1) return;
		newText = *(m_ppMesBuffer + sel);
	}
	else
	{
		if ((typ == 1) || (typ == 2))
		{
			newText = m_inputDialog->GetText(mes);
		}
		else if (typ == 3)
		{
			newText = m_inputDialog->GetText2(mes);
		}
	}


	if (newText != NULL)
	{
		if (pCommand->GetCommandType() == COMMANDDATATYPE_SCRIPT)
		{
			if (m_app->SearchScript(newText) == -1)
			{
				MessageBox(m_frameHWND,newText,"プロジェクトに存在しません",MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}

		pCommand->SetTextBuffer(newText);
		m_app->SetModify();
		m_app->CommandIsChanged();
	}
}


int CStoryDoc::CommandToType(int cmd)
{
	for (int i=0;i<100;i++)
	{
		int chk = m_commandToTypeTable[i*2];
		if (chk == -1) break;

		if (cmd == chk) return m_commandToTypeTable[i*2+1];
	}

	return -1;
}


int CStoryDoc::CommandToPara(int cmd)
{
	if (cmd == ID_MENU_COMMAND_SELECT1) return 1;
	if (cmd == ID_MENU_COMMAND_SELECT2) return 2;
	if (cmd == ID_MENU_COMMAND_SELECT3) return 3;
	if (cmd == ID_MENU_COMMAND_SELECT4) return 4;

//	if (cmd == ID_MENU_COMMAND_MIDI_START) return 0;
//	if (cmd == ID_MENU_COMMAND_MIDI_STARTONCE) return 1;
//	if (cmd == ID_MENU_COMMAND_MIDI_STOP) return 2;

//	if (cmd == ID_MENU_COMMAND_CD_START) return 0;
//	if (cmd == ID_MENU_COMMAND_CD_STARTONCE) return 1;
//	if (cmd == ID_MENU_COMMAND_CD_STOP) return 2;

	return 0;	//etc
}

int CStoryDoc::CommandToSubKosuu(int cmd)
{
	if (cmd == ID_MENU_COMMAND_SELECT1) return 1;
	if (cmd == ID_MENU_COMMAND_SELECT2) return 2;
	if (cmd == ID_MENU_COMMAND_SELECT3) return 3;
	if (cmd == ID_MENU_COMMAND_SELECT4) return 4;

	if (cmd == ID_MENU_COMMAND_IF) return 1;
	if (cmd == ID_MENU_COMMAND_ELSIF) return 1;
	if (cmd == ID_MENU_COMMAND_ELSE) return 1;
	if (cmd == ID_MENU_COMMAND_WHILE) return 1;

	return 0;
}


int CStoryDoc::SearchFilm(LPSTR filmName, int searchStart)
{
	return m_app->SearchFilm(filmName,searchStart);
}

CFilmData* CStoryDoc::GetFilmData(int n)
{
	return m_app->GetFilmData(n);
}

CNameList* CStoryDoc::GetFunctionList(void)
{
	return m_app->GetFunctionList();
}

CNameList* CStoryDoc::GetCommandList(void)
{
	return m_app->GetCommandList();
}

CNameList* CStoryDoc::GetBGMList(void)
{
	return m_app->GetBGMList();
}



int CStoryDoc::GetScrollObjectKosuu(int vh)
{
	CStoryData* pStory = m_app->GetNowSelectStory();
	if (pStory == NULL) return 1;

	return pStory->GetObjectKosuu() + 1;

//	char mes[256];
//	sprintf(mes,"rt=%d",rt);
//	OutputDebugString(mes);

//	return rt;
//	return pFilm->GetObjectKosuu();

	return 0;
}


int CStoryDoc::GetNowSelectNumber(void)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return -1;

	return pStory->GetNowSelectNumber();

	return -1;
}


void CStoryDoc::CreateMessageBuffer(int n)
{
	if (n<m_mesBufferKosuu) return;

	DELETEARRAY(m_ppMesBuffer);
	m_ppMesBuffer = new LPSTR [n];
	m_mesBufferKosuu = n;
}


void CStoryDoc::OnChangeMarkColor(int n,int color,int backColor,int blockFlag)
{
	CStoryData* pStory = m_app->GetNowSelectStory();
	if (pStory == NULL) return;

	if (n == -1) n = pStory->GetNowSelectNumber();

	int kosuu = pStory->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;


	CCommandData* pStartCommand = (CCommandData*)(pStory->GetObjectData(n));
	if (pStartCommand == NULL) return;


	CheckAndGetUndoAll();
//	if (m_app->GetUndoMode())
//	{
//		CUndoMemoryObject* undo = m_app->GetUndoObject();
//		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
//	}


	int startLevel = pStartCommand->GetLevel();


	int type = pStartCommand->GetCommandType();

	int ed = kosuu;
	if (blockFlag == 0) ed = n+1;

	for (int k=n;k<ed;k++)
	{
		CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(k));
		if (pCommand == NULL) break;

		int level = pCommand->GetLevel();
		if (level < startLevel) break;

		if (k>n)
		{
			if (level == startLevel)
			{
				if (type == COMMANDDATATYPE_ELSIF) break;
				if (type == COMMANDDATATYPE_ELSE) break;
				if (type == COMMANDDATATYPE_WHILE) break;
				if (type == COMMANDDATATYPE_SELECT) break;

				if (type == COMMANDDATATYPE_IF)
				{
					int type2 = pCommand->GetCommandType();
					if (type2 == COMMANDDATATYPE_ENDIF) break;
				}
			}
		}
		
		if (color >= 0)
		{
			pCommand->SetColor(color);
		}
		if (backColor >= 0)
		{
			pCommand->SetBackColor(backColor);
		}
	}

	m_view->MyInvalidateRect();
}


void CStoryDoc::OnEnterKey(void)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;
	int kosuu = pStory->GetObjectKosuu();

	int n = pStory->GetNowSelectNumber();

	if ((n<0) || (n>kosuu)) return;

	if (n == kosuu)
	{
		OnNewCommand();
		return;
	}


	CheckAndGetUndoAll();
//	if (m_app->GetUndoMode())
//	{
//		CUndoMemoryObject* undo = m_app->GetUndoObject();
//		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
//	}

	CCommandData* pCommand = pStory->GetCommand(n);
	if (pCommand->GetCommandType() == COMMANDDATATYPE_NOP)
	{
		CStoryView* pView = (CStoryView*)m_view;

		int cmd;

		if (CheckLeftShiftKey())
		{
			cmd = ID_MENU_COMMAND_FILM;
			pView->CheckChangeMarkColor(n,cmd);
			OnChangeType(n,cmd);
	//		OnEditCommand(n);
		}
		else if (CheckRightShiftKey())
		{
			cmd = ID_MENU_COMMAND_STORY;
			pView->CheckChangeMarkColor(n,cmd);
			OnChangeType(n,cmd);
		//	OnEditCommand(n);
		}
		else
		{

			int x = 0;
			int y = 0;
			WPARAM wParam = 0;
			LPARAM lParam = (x & 0xffff) | ((y & 0xffff) << 16);

			cmd = pView->OpenPopupMenu(wParam,lParam);

			if (cmd == ID_MENU_EDITCOMMAND)
			{
				OnEditCommand(n);
			}
			else if (cmd == ID_MENU_SELECTCOMMAND)
			{
				OnSelectCommand(n);
			}
			else if (cmd == ID_MENU_CHANGESELECTSTORY)
			{
				OnChangeSelectStory(n);
			}
			else if (cmd != 0)
			{
				pView->CheckChangeMarkColor(n,cmd);
				OnChangeType(n,cmd);
			}
		}
	}
	else
	{
		OnEditCommand(n);
	}
}


void CStoryDoc::OnDeleteKey(void)
{
	if (m_app->GetConfig("deleteKeyCommand") == 0) return;
	OnDelete(-1);
}

void CStoryDoc::OnInsertKey(void)
{
	OnNewCommand(-1);
}

void CStoryDoc::OnEscapeKey(void)
{
	m_app->ChangeWindowIfCan(STORYBOOK_WINDOW);
}

/*
void CStoryDoc::SelectPrevObject(void)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	int kosuu = pStory->GetObjectKosuu();
	int sel = pStory->GetNowSelectNumber();
	if (sel > 0) sel--;
	pStory->SetSelectNumber(sel);
	pStory->SetSelectSubNumber(-1);
}


void CStoryDoc::SelectNextObject(void)
{
	CStoryData* pStory = GetNowSelectStory();
	if (pStory == NULL) return;

	int kosuu = pStory->GetObjectKosuu();
	int sel = pStory->GetNowSelectNumber();
	if (sel < kosuu) sel++;
	pStory->SetSelectNumber(sel);
	pStory->SetSelectSubNumber(-1);
}
*/

CCase* CStoryDoc::GetNowSelectCaseObject(void)
{
	return GetNowSelectStory();
}

void CStoryDoc::OnSelectNumber(int n)
{
	ClearUndo();
	m_app->CommandIsChanged();
//	m_app->ProgramIsChanged();
}

BOOL CStoryDoc::CheckLeftShiftKey(void)
{
	if (GetAsyncKeyState(VK_LSHIFT) & 0x80000000) return TRUE;
	return FALSE;
}

BOOL CStoryDoc::CheckRightShiftKey(void)
{
	if (GetAsyncKeyState(VK_RSHIFT) & 0x80000000) return TRUE;
	return FALSE;
}




BOOL CStoryDoc::CheckExistUndo(void)
{
	CUndoMemoryObject* undo = m_app->GetUndoObject();
	if (undo != NULL)
	{
		int undoType = undo->GetUndoType();
		if (undoType == UNDO_TYPE_COMMAND)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CStoryDoc::OnUndo(int n)
{
	BOOL f = FALSE;

	if (m_app->GetUndoMode())
	{
		CStoryData* pStory = m_app->GetNowSelectStory();
		if (pStory == NULL) return FALSE;

		CUndoMemoryObject* undo = m_app->GetUndoObject();
		if (undo != NULL)
		{
			if (CheckExistUndo())
			{
				int dataType = undo->GetUndoDataType();
				int startN = undo->GetUndoStartN();
				int endN = undo->GetUndoEndN();
				int numN = endN - startN + 1;
				

				if (dataType == UNDO_DATA_ALL)//only 
				{
					int kosuu = pStory->GetObjectKosuu();

					int nowSelect = pStory->GetNowSelectNumber();

					if (kosuu>0)
					{
						pStory->DeleteObjectData(0,kosuu);
					}


					for (int i=startN;i<=endN;i++)
					{
						pStory->CreateObjectData(i);
						CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(i));
						pCommand->Init();
						pCommand->Load(NULL,undo);
					}

					if ((nowSelect >= 0) && (nowSelect<endN))
					{
						pStory->SetSelectNumber(nowSelect);
					}


					undo->Clear();
					f = TRUE;
				}

				/*
				if (dataType == UNDO_DATA_INSERT)
				{
					pStory->DeleteObjectData(startN,numN);
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_DELETE)
				{
					for (int i=startN;i<=endN;i++)
					{
						pStory->CreateObjectData(i);
						CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(i));
						pCommand->Init();
						pCommand->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_MODIFY)
				{
					for (int i=startN;i<=endN;i++)
					{
						CCommandData* pCommand = (CCommandData*)(pCommand->GetObjectData(i));
						pCommand->Init();
						pCommand->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
				*/

			}
		}
	}




	if (f)
	{
		((CStoryView*)m_view)->ReCalcuScrollPara();
		m_app->SetModify();
		m_app->CommandIsChanged();
		m_view->MyInvalidateRect();
	}


	return f;
}

void CStoryDoc::CheckAndGetUndoAll(void)
{
	if (m_app->GetUndoMode())
	{
		CStoryData* pStory = GetNowSelectStory();
		if (pStory == NULL) return;
		int kosuu = pStory->GetObjectKosuu();
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		if (kosuu>0)
		{
			undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_ALL,0,kosuu-1);
			for (int i=0;i<kosuu;i++)
			{
				CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(i));
				if (pCommand != NULL)
				{
					pCommand->Save(NULL,undo);
				}
			}

		}
	}
}

//dontUse
void CStoryDoc::CheckAndGetUndo(CStoryData* pStory,int start,int end)
{
	if (pStory == NULL) return;

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_COMMAND,UNDO_DATA_MODIFY,start,end);
		for (int i=start;i<=end;i++)
		{
			CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(i));
			if (pCommand != NULL)
			{
				pCommand->Save(NULL,undo);
			}
		}
	}
}

void CStoryDoc::ClearUndo(void)
{
	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear();
	}
}



/*_*/

