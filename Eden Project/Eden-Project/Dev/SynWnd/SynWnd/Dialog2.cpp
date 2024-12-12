// Dialog2.cpp : 实现文件
//

#include "stdafx.h"
#include "SynWnd.h"
#include "Dialog2.h"
#include "afxdialogex.h"


// CDialog2 对话框

IMPLEMENT_DYNAMIC(CDialog2, CDialogEx)

CDialog2::CDialog2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialog2::IDD, pParent)
{

}

CDialog2::~CDialog2()
{
}

void CDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog2, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDialog2 消息处理程序


void CDialog2::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDialog2::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialogEx::OnOK();
}


void CDialog2::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}


void CDialog2::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	//CDialogEx::OnClose();
}


void CDialog2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
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
dc.TextOut(5, 5, _T("收割者—CSDN"), 8);
dc.SetBkMode(OPAQUE);
dc.SelectObject(def_font);

// Done with the font.  Delete the font object.
font.DeleteObject(); 

}
