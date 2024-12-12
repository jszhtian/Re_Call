
// SynWndDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SynWnd.h"
#include "SynWndDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSynWndDlg 对话框




CSynWndDlg::CSynWndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSynWndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	dlg2=0;
	moveX1=0;
	moveY1=0;
	moveX2=0;
	moveY2=0;
	IsNcDown=0;
}

void CSynWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSynWndDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CSynWndDlg 消息处理程序

BOOL CSynWndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	dlg2=new CDialog2;
	dlg2->Create(IDD_DIALOGB,this);
	dlg2->ShowWindow(SW_SHOW);
	dlg2->UpdateWindow();

	bt.Create(L"收割者",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,0,100,100),this,1121);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSynWndDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		 // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CFont font;
VERIFY(font.CreateFont(
   30,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_NORMAL,                 // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   _T("Arial")));                 // lpszFacename

// Do something with the font just created...
CFont* def_font = dc.SelectObject(&font);
dc.SetBkMode(TRANSPARENT);
dc.TextOut(100, 50, _T("收割者—CSDN"), 8);
dc.SetBkMode(OPAQUE);
dc.SelectObject(def_font);

// Done with the font.  Delete the font object.
font.DeleteObject(); 
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSynWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSynWndDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CSynWndDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CSynWndDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	this->SetCapture();
	/////计算相对位置
	POINT CurPos;
	::GetCursorPos(&CurPos);//光标位置
    //////子窗口位置
	
	dlg2->GetWindowRect(&rect1);
	////
	moveX1=CurPos.x-rect1.left;
	moveY1=CurPos.y-rect1.top;
	////////主窗口位置
	
	this->GetWindowRect(&rect2);
	/////
	moveX2=CurPos.x-rect2.left;
	moveY2=CurPos.y-rect2.top;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSynWndDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nFlags==MK_LBUTTON)
	{
		POINT curpos;
		::GetCursorPos(&curpos);

		dlg2->MoveWindow(curpos.x-moveX1,curpos.y-moveY1,rect1.right-rect1.left,rect1.bottom-rect1.top);
		this->MoveWindow(curpos.x-moveX2,curpos.y-moveY2,rect2.right-rect2.left,rect2.bottom-rect2.top);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CSynWndDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();
	CDialogEx::OnLButtonUp(nFlags, point);
}


