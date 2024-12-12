// TextToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTextToolDlg �Ի���
class CTextToolDlg : public CDialog
{
// ����
public:
	CTextToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEXTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void GetTextData();
	void SetTextData();
	void OnOK();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_OrigText;
	CString m_NewText;
	CSpinButtonCtrl m_Spin_Count;
	CString m_IndexEdit;
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedButton5();
	CString m_MaxCount;
	afx_msg void OnBnClickedButton1();
	CEdit m_CountEdit;
	afx_msg void OnEnChangeEdit1();
	CString m_CountValue;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
};
