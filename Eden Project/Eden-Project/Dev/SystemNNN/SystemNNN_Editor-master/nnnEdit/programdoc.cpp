//
// programdoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "windowlist.h"

#include "case.h"

#include "programview.h"

#include "programdoc.h"

#include "myinputdialog.h"

#include "myapplicationBase.h"

#include "commanddata.h"
#include "storydata.h"

CProgramDoc::CProgramDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = PROGRAM_WINDOW;	//dummy


	m_view = new CProgramView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();

//	m_view->SetViewWindowText("Program");
//	m_view->MoveViewWindow(934,456,320+GetSystemMetrics(SM_CXVSCROLL),24*16);

	m_inputDialog = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance);
}



CProgramDoc::~CProgramDoc()
{
	End();
}

void CProgramDoc::End(void)
{
	ENDDELETECLASS(m_inputDialog);
}

//void CProgramDoc::OnCloseButton(void)
//{
//
//}

void CProgramDoc::OnEditText(int n)
{
//	CStoryData* pStory = GetNowSelectStory();
//	if (pStory == NULL) return;

//	int kosuu = pStory->GetObjectKosuu();
//	if ((n<0) || (n>=kosuu)) return;




//	CCommandData* pCommand = (CCommandData*)(pStory->GetObjectData(n));
	CCommandData* pCommand = GetNowSelectCommand();
	if (pCommand == NULL) return;

	LPSTR mes = pCommand->GetTextBuffer();
	LPSTR newText = m_inputDialog->GetText(mes);

//	int typ = pCommand->GetCommandType();
//	Get
	if (newText != NULL)
	{
		pCommand->SetTextBuffer(newText);
		m_app->SetModify();
		m_app->CommandIsChanged();
	}
}



/*_*/

