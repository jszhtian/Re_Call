// testabmpbutton.h : main header file for the TESTABMPBUTTON application
//

#if !defined(AFX_TESTABMPBUTTON_H__C6133A1A_9C78_42D9_972C_567EC15DCDEF__INCLUDED_)
#define AFX_TESTABMPBUTTON_H__C6133A1A_9C78_42D9_972C_567EC15DCDEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestabmpbuttonApp:
// See testabmpbutton.cpp for the implementation of this class
//

class CTestabmpbuttonApp : public CWinApp
{
public:
	CTestabmpbuttonApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestabmpbuttonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestabmpbuttonApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTABMPBUTTON_H__C6133A1A_9C78_42D9_972C_567EC15DCDEF__INCLUDED_)
