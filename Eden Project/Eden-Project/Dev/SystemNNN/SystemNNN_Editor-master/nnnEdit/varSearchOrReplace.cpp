#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "filmCaseData.h"
#include "filmData.h"
#include "komaData.h"
#include "messageData.h"

#include "storyBookData.h"

#include "myVarSearchDialog.h"

#include "varSearchOrReplace.h"

#include "windowlist.h"

CVarSearchOrReplace::CVarSearchOrReplace(CMyApplicationBase* app)
{
	m_app = app;
	m_projectList = m_app->GetProjectList();

	m_search = new CMyVarSearchDialog(m_app->GetFrameHWND(),m_app->GetHInstance(),1024);
}

CVarSearchOrReplace::~CVarSearchOrReplace()
{
	End();
}

void CVarSearchOrReplace::End(void)
{
	ENDDELETECLASS(m_search);
}

void CVarSearchOrReplace::SearchOrReplace(BOOL replaceFlag,int nowSelectNumber,int blockStartNumber,int blockEndNumber)
{
//	char lastFileName[256];



	BOOL startFlag = TRUE;

	int cmd = m_search->GetCommand(replaceFlag,startFlag);
	if (cmd == -1) return;

	int radio = m_search->GetRadio();
	int radio2 = m_search->GetRadio2();

	LPSTR searchString = m_search->GetSearchText();
	LPSTR replaceString = m_search->GetReplaceText();


	int startVarNumber = 0;
	int endVarNumber = 999;

	int selectBlock = 0;

	//block
	if (radio == 0)
	{
		startVarNumber = blockStartNumber;
		endVarNumber = blockEndNumber;
	}

	if (radio2 == 0)
	{
		if ((nowSelectNumber >= blockStartNumber) && (nowSelectNumber <= blockEndNumber))
		{
			startVarNumber = nowSelectNumber;
		}
	}
	else if (radio == 2)
	{
		if ((nowSelectNumber >= blockStartNumber) && (nowSelectNumber <= blockEndNumber))
		{
			endVarNumber = nowSelectNumber;
		}
	}


	BOOL allReplaceFlag = FALSE;
	if (cmd == 2) allReplaceFlag = TRUE;

	BOOL quitFlag = FALSE;

	while (startVarNumber <= endVarNumber)
	{
		LPSTR varName = m_app->GetVarName(startVarNumber);
		LPSTR found = strstr(varName,searchString);
		if (found != NULL)
		{
			startFlag = FALSE;

			if (cmd == 2)
			{
				m_app->ReplaceVarName(startVarNumber,searchString,replaceString);
			}

			if (allReplaceFlag == FALSE)
			{
				//set select number
				m_app->SetSelectVarNumber(startVarNumber);

				m_app->UpdateMyWindow(-1);
				m_search->SetDefaultRadio2(0);

				cmd = m_search->GetCommand(replaceFlag,startFlag);
				searchString = m_search->GetSearchText();
				replaceString = m_search->GetReplaceText();

				if (cmd == 0)
				{
//					startPointer += srcLength;
				}

				if ((cmd == 1) || (cmd == 2))
				{
					//replace
					m_app->ReplaceVarName(startVarNumber,searchString,replaceString);

					if (cmd == 2)
					{
						allReplaceFlag = TRUE;
					}

					if (cmd == -1)
					{
						quitFlag = TRUE;
						break;
					}
				}
			}

			if (cmd == -1)
			{
				quitFlag = TRUE;
				break;
			}
		}
		startVarNumber++;
		if (quitFlag) break;
	}
}


/*_*/



