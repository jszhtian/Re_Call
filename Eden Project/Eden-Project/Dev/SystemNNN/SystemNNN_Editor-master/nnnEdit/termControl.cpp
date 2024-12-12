#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "resource.h"

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
//#include "..\..\systemNNN\nnnUtilLib\scriptDefine.h"
//#include "commanddatatype.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "termControl.h"


CTermControl::CTermControl()
{
}

CTermControl::~CTermControl()
{
	End();
}

void CTermControl::End(void)
{
}

void CTermControl::SetTermList(CNameList* list)
{
	m_termList = list;
}

void CTermControl::Start(LPSTR mes)
{
	m_checkMessage = mes;
	m_nowCheck = 0;
	m_termKosuu = 0;
	if (m_termList != NULL)
	{
		m_termKosuu = m_termList->GetNameKosuu() / 4;
	}
}

int CTermControl::GetNextTerm(void)
{
	if (m_nowCheck >= m_termKosuu) return -1;

	while (m_nowCheck < m_termKosuu)
	{
		LPSTR term = m_termList->GetName(m_nowCheck*4);
		int rt = m_nowCheck;
		m_nowCheck++;

		if (strstr(m_checkMessage,term) != NULL)
		{
			return rt;
		}
	}

	return -1;
}

