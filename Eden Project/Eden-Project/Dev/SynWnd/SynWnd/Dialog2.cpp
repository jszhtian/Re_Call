// Dialog2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SynWnd.h"
#include "Dialog2.h"
#include "afxdialogex.h"


// CDialog2 �Ի���

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


// CDialog2 ��Ϣ�������


void CDialog2::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDialog2::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

//	CDialogEx::OnOK();
}


void CDialog2::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}


void CDialog2::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	//CDialogEx::OnClose();
}


void CDialog2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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
dc.TextOut(5, 5, _T("�ո��ߡ�CSDN"), 8);
dc.SetBkMode(OPAQUE);
dc.SelectObject(def_font);

// Done with the font.  Delete the font object.
font.DeleteObject(); 

}
