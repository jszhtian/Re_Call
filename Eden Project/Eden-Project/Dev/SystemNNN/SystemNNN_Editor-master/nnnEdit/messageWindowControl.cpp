#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "messageWindowControl.h"


CMessageWindowControl::CMessageWindowControl(CMyApplicationBase* app)
{
	m_app = app;
}

CMessageWindowControl::~CMessageWindowControl()
{
	End();
}

void CMessageWindowControl::End(void)
{
}




/*_*/


