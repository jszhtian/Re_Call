#pragma once


// CMyBT

class CMyBT : public CButton
{
	DECLARE_DYNAMIC(CMyBT)

public:
	CMyBT();
	virtual ~CMyBT();

public:
	RECT rect;//按钮的大小和位置

	int Movex;
	int Movey;

	int Width;
	int Height;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


