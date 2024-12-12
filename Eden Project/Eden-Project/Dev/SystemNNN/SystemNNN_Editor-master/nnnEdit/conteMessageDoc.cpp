//
// ConteMessagedoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"


#include "windowlist.h"

#include "case.h"

#include "ConteMessageview.h"

#include "ConteMessagedoc.h"

#include "myapplicationBase.h"

#include "komadata.h"

#include "myfileopen.h"








CConteMessageDoc::CConteMessageDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = CONTEMESSAGE_WINDOW;
	m_view = new CConteMessageView(this,m_clientHWND,m_hInstance);
	m_view->InitWindowZahyo();
}

CConteMessageDoc::~CConteMessageDoc()
{
	End();
}

void CConteMessageDoc::End(void)
{
}



/*_*/



