// MyBT.cpp : 实现文件
//

#include "stdafx.h"
#include "SynWnd.h"
#include "MyBT.h"


// CMyBT

IMPLEMENT_DYNAMIC(CMyBT, CButton)

CMyBT::CMyBT()
{
	Movex=0;
	Movey=0;
}

CMyBT::~CMyBT()
{
}


BEGIN_MESSAGE_MAP(CMyBT, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMyBT 消息处理程序




void CMyBT::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetWindowRect(&rect);
	POINT curpos;
	::GetCursorPos(&curpos);
	Movex=curpos.x-rect.left;
	Movey=curpos.y-rect.top;
	this->SetCapture();
	CButton::OnLButtonDown(nFlags, point);
}


void CMyBT::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();
	CButton::OnLButtonUp(nFlags, point);
}


void CMyBT::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nFlags==MK_LBUTTON)
	{
		POINT curpos;
		::GetCursorPos(&curpos);
		RECT lrect;
		lrect.left=curpos.x-Movex;
		lrect.right=lrect.left+100;
		lrect.top=curpos.y-Movey;
		lrect.bottom=lrect.top+100;
		this->GetParent()->ScreenToClient(&lrect);
		this->MoveWindow(lrect.left,lrect.top,100,100);
	}
	CButton::OnMouseMove(nFlags, point);
}


int CMyBT::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	GetWindowRect(&rect);
	return 0;
}
