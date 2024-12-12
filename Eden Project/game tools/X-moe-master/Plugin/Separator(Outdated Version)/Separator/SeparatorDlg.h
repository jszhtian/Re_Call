
// SeparatorDlg.h : 头文件
//

#pragma once
#include "pStart.h"
#include <list>

using std::list;

// CSeparatorDlg 对话框
class CSeparatorDlg : public CDialogEx
{
// 构造
public:
	CSeparatorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SEPARATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	//设定透明色
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
