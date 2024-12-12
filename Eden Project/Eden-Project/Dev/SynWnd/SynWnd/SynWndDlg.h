
// SynWndDlg.h : 头文件
//

#pragma once

#include "Dialog2.h"
#include "MyBT.h"
// CSynWndDlg 对话框
class CSynWndDlg : public CDialogEx
{
// 构造
public:
	CSynWndDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SYNWND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CDialog2 *dlg2;
	CMyBT bt;
     int moveX1;
	 int moveY1;
	 int moveX2;
	 int moveY2;

	 RECT rect1;
	 RECT rect2;

	 BOOL IsNcDown;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
