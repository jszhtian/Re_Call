// TextHaloEffect.h : TextHaloEffect Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CTextHaloEffectApp:
// �йش����ʵ�֣������ TextHaloEffect.cpp
//

class CTextHaloEffectApp : public CWinApp
{
public:
	CTextHaloEffectApp();

	//GDI+
private:
	ULONG_PTR m_gdiplusToken;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTextHaloEffectApp theApp;
