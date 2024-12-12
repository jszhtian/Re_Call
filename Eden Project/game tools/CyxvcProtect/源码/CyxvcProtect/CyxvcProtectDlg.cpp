
// CyxvcProtectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CyxvcProtect.h"
#include "CyxvcProtectDlg.h"
#include "afxdialogex.h"
#include <tchar.h>
#include "PACK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCyxvcProtectDlg::CCyxvcProtectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCyxvcProtectDlg::IDD, pParent)
	, m_strFilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCyxvcProtectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFilePath);
}

BEGIN_MESSAGE_MAP(CCyxvcProtectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCyxvcProtectDlg::OnBnClicked_OpenFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CCyxvcProtectDlg::OnBnClicked_Pack)
END_MESSAGE_MAP()

BOOL CCyxvcProtectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCyxvcProtectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CCyxvcProtectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

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
		CDialogEx::OnPaint();
	}
}

HCURSOR CCyxvcProtectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//************************************************************
// ��������:	OnBnClicked_OpenFile
// ����˵��:	���ļ���ť
// ��	��:	cyxvc
// ʱ	��:	2015/12/24
// �� ��	ֵ:	void
//************************************************************
void CCyxvcProtectDlg::OnBnClicked_OpenFile()
{
	CFileDialog dlg(TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
		m_strFilePath = dlg.GetPathName();
	else
		return;
	UpdateData(FALSE);
}


//************************************************************
// ��������:	OnBnClicked_Pack
// ����˵��:	�ӿǰ�ť
// ��	��:	cyxvc
// ʱ	��:	2015/12/24
// �� ��	ֵ:	void
//************************************************************
void CCyxvcProtectDlg::OnBnClicked_Pack()
{
	if (m_strFilePath.IsEmpty())
	{
		MessageBox(_T("��ѡ�񱻼ӿǵ��ļ���"), _T("��ʾ"), MB_OK);
		return;
	}
		
	CPACK objPACK;

	//��ȡ�Ƿ���ʾMessageBox
	int nStatus = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();

	if (objPACK.Pack(m_strFilePath, nStatus))
		MessageBox(_T("�ӿǳɹ���"), _T("��ʾ"), MB_OK);

}
