// testabmpbuttonDlg.h : header file
//

#if !defined(AFX_TESTABMPBUTTONDLG_H__87DA4AB3_8DCE_4B4C_ADC7_2BBAA474B025__INCLUDED_)
#define AFX_TESTABMPBUTTONDLG_H__87DA4AB3_8DCE_4B4C_ADC7_2BBAA474B025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestabmpbuttonDlg dialog

class CTestabmpbuttonDlg : public CDialog
{
// Construction
public:
	CTestabmpbuttonDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestabmpbuttonDlg)
	enum { IDD = IDD_TESTABMPBUTTON_DIALOG };
//	CButton	m_button;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestabmpbuttonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestabmpbuttonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTABMPBUTTONDLG_H__87DA4AB3_8DCE_4B4C_ADC7_2BBAA474B025__INCLUDED_)
