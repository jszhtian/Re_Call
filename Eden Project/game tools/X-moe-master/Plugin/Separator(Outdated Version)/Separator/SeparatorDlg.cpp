
// SeparatorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Separator.h"
#include "SeparatorDlg.h"
#include "afxdialogex.h"
#include "pStart.h"

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	CDC *pDC=GetDC();
	OnEraseBkgnd(pDC);
}

void CAboutDlg::OnPaint()
{
		
		CPaintDC   dc(this);
		CRect      rect;
		GetClientRect(&rect);
		CDC        dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP2);   //IDB_BITMAP��Ӧ��ID                    
		BITMAP   bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);
		//dcMem.BitBlt(0,0,bitmap.bmWidth, bitmap.bmHeight, &dc, 0, 0, SRCCOPY);
		//dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
}


BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 
{
   // CDialog::OnEraseBkgnd(pDC);//����Ҫ��Ҫִ�и���������.;
    CDC  memdc;
    memdc.CreateCompatibleDC(pDC);
    CBitmap bmp;
    CBitmap *pOldBmp;
    BITMAP bm;//��������ͼƬ�ĳ������Ϣ

	CRect      rect;
	GetClientRect(&rect);
    bmp.LoadBitmap(IDB_BITMAP2);
    bmp.GetBitmap(&bm);

    //��ͼƬѡ��������
    pOldBmp = (CBitmap *)memdc.SelectObject(&bmp);

    //���ͼƬ���Ի���Ŀͻ���һ����ֱ����BitBlt�Ϳ���
    //pDC->BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &memdc, 0, 0, SRCCOPY);

    //���ͼƬ�ȶԻ���ı���С����Ҫ��StrechBlt������ͼƬ
	CPoint po = rect.TopLeft();
	pDC->BitBlt(po.x, po.y, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
    //pDC->StrechBlt(0, 0, rect.Width, rect.Height(), &memdc, 0, 0, bm.bmWidth,bm.bmHeight, SRCCOPY);

   //�ͷ�DC
    memdc.SelectObject(pOldBmp);
    memdc.DeleteDC();

    return TRUE; 
}



void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSeparatorDlg �Ի���




CSeparatorDlg::CSeparatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSeparatorDlg::IDD, pParent)
	, lpExtractAddr(_T(""))
	, lpExtractDic(_T(""))
	, lpEngineName(_T(""))
	, bRestoreTmpData(_T(""))
	, lpCmd(_T(""))
	//ExtractStart(NULL)
{
	//ExtractStart = new pStart;
	EngineStrUpdate = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSeparatorDlg::DoDataExchange(CDataExchange* pDX)
{
	////��ȡ�ĵ�ַ���������
	DDX_Text(pDX, IDC_EDIT1, lpExtractAddr);
	DDV_MaxChars(pDX, lpExtractAddr, 2048);
	////��ȡ���ļ���,������
	DDX_Text(pDX, IDC_EDIT2, lpExtractDic);
	DDV_MaxChars(pDX, lpExtractDic, 2048);
	////ָ���������֣�����ܶ�����ѹ�����ĺ�׺��Ϊarc��Ϊ�����ʶ���ʣ���Ҫ��д���
	DDX_Text(pDX, IDC_EDIT3, lpEngineName);
	DDV_MaxChars(pDX, lpEngineName, 2048);
	////�Ƿ񱣴��м����ݣ�����krkr2��tlgͼƬ
	DDX_Text(pDX, IDC_EDIT4, bRestoreTmpData);
	DDV_MaxChars(pDX, bRestoreTmpData, 2048);
	////��������
	DDX_Text(pDX, IDC_EDIT5, lpCmd);
	DDV_MaxChars(pDX, lpCmd, 2048);

	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSeparatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSeparatorDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CSeparatorDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CSeparatorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CSeparatorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSeparatorDlg::OnBnClickedButton3)
	ON_NOTIFY(BCN_DROPDOWN, IDOK, &CSeparatorDlg::OnDropdownIdok)
	ON_EN_CHANGE(IDC_EDIT5, &CSeparatorDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT3, &CSeparatorDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT2, &CSeparatorDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, &CSeparatorDlg::OnEnChangeEdit4)
	ON_EN_UPDATE(IDC_EDIT3, &CSeparatorDlg::OnUpdateEdit3)
END_MESSAGE_MAP()


// CSeparatorDlg ��Ϣ�������

BOOL CSeparatorDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_SHOW);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSeparatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSeparatorDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		
		CPaintDC   dc(this);
		CRect      rect;
		GetClientRect(&rect);
		CDC        dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP4);   //IDB_BITMAP��Ӧ��ID                    
		BITMAP   bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSeparatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSeparatorDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CSeparatorDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg *info = new CAboutDlg;
	info->Create(IDD_ABOUTBOX, this);
	info->ShowWindow(SW_SHOW);
	
}


void CSeparatorDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemTextW(IDC_EDIT1, lpEngineName);
}

/*
HBRUSH CSeparatorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
 
	if(nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);//���ñ���͸��
		pDC->SetTextColor(RGB(255,255,255));//��������Ϊ��ɫ
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}*/

/*
BOOL CSeparatorDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnEraseBkgnd(pDC);
	HBITMAP   m_hBitmap;
	HDC           m_hBkDC;
	m_hBitmap   =   ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP4));
	m_hBkDC     =   ::CreateCompatibleDC(pDC-> m_hDC);
	CRect      rect;
	GetClientRect(&rect);
	if(m_hBitmap   &&   m_hBkDC)
	{
		::SelectObject(m_hBkDC,m_hBitmap);
		::StretchBlt(pDC-> m_hDC,0,0,rect.Width(),rect.Height(),m_hBkDC,0,0,600,375,SRCCOPY);
		::DeleteObject(m_hBitmap);
		::DeleteDC(m_hBkDC);
	}
	//0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY
	return TRUE;
}
*/

void CAboutDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CAboutDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

/*************************/

void CSeparatorDlg::ParseCmd()
{
	if(lpCmd.IsEmpty())
		return;

	wchar_t *pCmdBuffer = new wchar_t[lpCmd.GetLength()+1];
	wmemset(pCmdBuffer, 0, (lpCmd.GetLength()+1)/sizeof(wchar_t));
	wmemcpy(pCmdBuffer, lpCmd.GetBuffer(), lpCmd.GetLength());
	
	wistringstream ss(pCmdBuffer);

	wchar_t Buffer[512];
	wmemset(Buffer, 0, sizeof(Buffer)/sizeof(wchar_t));
	while(!ss.eof())
	{
		ss>>Buffer;
		if(!StrCmpICW(Buffer, L"-using"))
		{
			wmemset(Buffer, 0, sizeof(Buffer)/sizeof(wchar_t));
			ss>>Buffer;
			//
		}
		else if (!StrCmpICW(Buffer, L"-engine"))
		{
			wmemset(Buffer, 0, sizeof(Buffer) / sizeof(wchar_t));
			ss >> Buffer;
			MessageBox(Buffer, L"", MB_OK);
			//
		}
		else if(!StrCmpICW(Buffer, L"-run"))
		{
			wmemset(Buffer, 0, sizeof(Buffer)/sizeof(wchar_t));
			ss>>Buffer;
			//run Script
		}
		else if(!StrCmpICW(Buffer, L"-cmd"))
		{
			/*
			decode utf16le cp936 cp932
			*/
			wmemset(Buffer, 0, sizeof(Buffer)/sizeof(wchar_t));
			ss>>Buffer;
			if(!StrCmpICW(Buffer, L"decode"))
			{
				if(!StrCmpICW(Buffer, L"unicode"))
				{
				}
				else if(!StrCmpICW(Buffer, L"cp932"))
				{
				}
				else if(!StrCmpICW(Buffer, L"cp936"))
				{
				}
				else
				{
					wchar_t Info[1024];
					wmemset(Info, 0, sizeof(Info)/sizeof(wchar_t));
					wsprintf(Info, L"�޷������Ĳ���: %s", Buffer);
					MessageBoxW(Info, L"Separator::Parse", MB_OK);
					break;
				}
			}
			else
			{
				wchar_t Info[1024];
				wmemset(Info, 0, sizeof(Info)/sizeof(wchar_t));
				wsprintf(Info, L"�޷������ĸ�ָ��: %s", Buffer);
				MessageBoxW(Info, L"Separator::Parse", MB_OK);
				break;
			}
		}
		else
		{
			wchar_t Info[1024];
			wmemset(Info, 0, sizeof(Info)/sizeof(wchar_t));
			wsprintf(Info, L"�޷�������ָ��: %s", Buffer);
			MessageBoxW(Info, L"Separator::Parse", MB_OK);
			break;
		}
		wmemset(Buffer, 0, sizeof(Buffer)/sizeof(wchar_t));
	}

	delete[] pCmdBuffer;
}


void CSeparatorDlg::OnBnClickedOk()
{
	//this->ShowWindow(SW_HIDE);
	pStart *ExtractStart = new pStart;
	WorkList.push_back(ExtractStart);
	//this->ShowWindow(SW_HIDE);
	//ParseCmd();
	//MessageBox(lpEngineName.GetBuffer(), L"", MB_OK);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!lpExtractAddr.IsEmpty() && !lpExtractDic.IsEmpty())
	{
		AfxMessageBox(L"ֻ��ѡ��һ��ģʽ!");
	}
	//�����ļ�
	else if(!lpExtractAddr.IsEmpty() && lpExtractDic.IsEmpty())
	{
		//this->ShowWindow(SW_HIDE);
		ExtractStart->ParseFile(lpExtractAddr, lpEngineName);
		//this->ShowWindow(SW_SHOW);

	}
	else if(lpExtractAddr.IsEmpty() && !lpExtractDic.IsEmpty())
	{
		//this->ShowWindow(SW_HIDE);
		ExtractStart->ParseDic(lpExtractDic, lpEngineName);
		//this->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(L"��Ч������!");
	}
	WorkList.remove(ExtractStart);
	delete ExtractStart;
	return;
}


//ѡ��һ�����
void CSeparatorDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(FALSE);
	CString strFile = _T("");

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"), NULL);

    if (dlgFile.DoModal())  
    {  
		strFile = dlgFile.GetPathName(); 
		lpExtractAddr = strFile;
		//UpdateData(TRUE);
    }
	UpdateData(FALSE);

}

//ѡ��һ���ļ���
void CSeparatorDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(FALSE);
    TCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·��    
    CString str;  
    ZeroMemory(szPath, sizeof(szPath));     
      
    BROWSEINFO bi;     
    bi.hwndOwner = m_hWnd;     
    bi.pidlRoot = NULL;     
    bi.pszDisplayName = szPath;     
    bi.lpszTitle = L"��ѡ�������ڵ�Ŀ¼";     
    bi.ulFlags = 0;     
    bi.lpfn = NULL;     
    bi.lParam = 0;     
    bi.iImage = 0;     
    //����ѡ��Ŀ¼�Ի���   
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);     
      
    if(lp && SHGetPathFromIDList(lp, szPath))
    {  
		//Debugʱ�ڴ���
        //str.Format(L"ѡ���Ŀ¼Ϊ %s",  szPath);
        //AfxMessageBox(str);             
    }  
    else     
        AfxMessageBox(L"��Ч��Ŀ¼!");

	lpExtractDic = szPath;
	UpdateData(FALSE);
}


void CSeparatorDlg::OnDropdownIdok(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


//�������
/*
-using unicode
-force cp 936
-cmd   tmpfile
-run   script.as
*/
void CSeparatorDlg::OnEnChangeEdit5()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//ָ������ NULL
void CSeparatorDlg::OnEnChangeEdit3()
{
	//if (EngineStrUpdate == true)
	//{
		GetDlgItemTextW(IDC_EDIT3, lpEngineName);
		//SetDlgItemText(IDC_EDIT3, lpEngineName.Right(1));/////////////////////
		//EngineStrUpdate = false;
	//}
}


void CSeparatorDlg::OnEnChangeEdit2()
{
	//lpCmd
	GetDlgItemTextW(IDC_EDIT2, lpCmd);
}


void CSeparatorDlg::OnEnChangeEdit4()
{
	GetDlgItemTextW(IDC_EDIT4, bRestoreTmpData);
}


void CSeparatorDlg::OnUpdateEdit3()
{
	EngineStrUpdate = true;
}
