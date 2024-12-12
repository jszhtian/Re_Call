
// SynWndDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SynWnd.h"
#include "SynWndDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSynWndDlg �Ի���




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


// CSynWndDlg ��Ϣ�������

BOOL CSynWndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	dlg2=new CDialog2;
	dlg2->Create(IDD_DIALOGB,this);
	dlg2->ShowWindow(SW_SHOW);
	dlg2->UpdateWindow();

	bt.Create(L"�ո���",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,0,100,100),this,1121);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSynWndDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		 // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
dc.TextOut(100, 50, _T("�ո��ߡ�CSDN"), 8);
dc.SetBkMode(OPAQUE);
dc.SelectObject(def_font);

// Done with the font.  Delete the font object.
font.DeleteObject(); 
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSynWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSynWndDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnOK();
}


void CSynWndDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}


void CSynWndDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	this->SetCapture();
	/////�������λ��
	POINT CurPos;
	::GetCursorPos(&CurPos);//���λ��
    //////�Ӵ���λ��
	
	dlg2->GetWindowRect(&rect1);
	////
	moveX1=CurPos.x-rect1.left;
	moveY1=CurPos.y-rect1.top;
	////////������λ��
	
	this->GetWindowRect(&rect2);
	/////
	moveX2=CurPos.x-rect2.left;
	moveY2=CurPos.y-rect2.top;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSynWndDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ReleaseCapture();
	CDialogEx::OnLButtonUp(nFlags, point);
}


