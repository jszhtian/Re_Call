
// SynWndDlg.h : ͷ�ļ�
//

#pragma once

#include "Dialog2.h"
#include "MyBT.h"
// CSynWndDlg �Ի���
class CSynWndDlg : public CDialogEx
{
// ����
public:
	CSynWndDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SYNWND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
