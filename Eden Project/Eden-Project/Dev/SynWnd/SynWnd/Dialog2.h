#pragma once


// CDialog2 �Ի���
#include "resource.h"
class CDialog2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog2)

public:
	CDialog2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialog2();

// �Ի�������
	enum { IDD = IDD_DIALOGB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnClose();
	afx_msg void OnPaint();
};
