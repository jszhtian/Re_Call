
// SeparatorDlg.h : ͷ�ļ�
//

#pragma once
#include "pStart.h"
#include <list>

using std::list;

// CSeparatorDlg �Ի���
class CSeparatorDlg : public CDialogEx
{
// ����
public:
	CSeparatorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SEPARATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	//�趨͸��ɫ
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();

	void ParseCmd();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
	CString lpExtractAddr;
	CString lpExtractDic;
	CString lpEngineName;
	CString bRestoreTmpData;
	CString lpCmd;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnDropdownIdok(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnUpdateEdit3();

	bool EngineStrUpdate;


//private:
	list<pStart*> WorkList;
};
