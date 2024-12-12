
// SeparatorDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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
		bmpBackground.LoadBitmap(IDB_BITMAP2);   //IDB_BITMAP对应的ID                    
		BITMAP   bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);
		//dcMem.BitBlt(0,0,bitmap.bmWidth, bitmap.bmHeight, &dc, 0, 0, SRCCOPY);
		//dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
}


BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 
{
   // CDialog::OnEraseBkgnd(pDC);//忘记要不要执行父类的这个了.;
    CDC  memdc;
    memdc.CreateCompatibleDC(pDC);
    CBitmap bmp;
    CBitmap *pOldBmp;
    BITMAP bm;//用来保存图片的长宽等信息

	CRect      rect;
	GetClientRect(&rect);
    bmp.LoadBitmap(IDB_BITMAP2);
    bmp.GetBitmap(&bm);

    //将图片选进缓冲区
    pOldBmp = (CBitmap *)memdc.SelectObject(&bmp);

    //如果图片跟对话框的客户区一样大，直接用BitBlt就可以
    //pDC->BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &memdc, 0, 0, SRCCOPY);

    //如果图片比对话框的背景小，就要用StrechBlt来拉伸图片
	CPoint po = rect.TopLeft();
	pDC->BitBlt(po.x, po.y, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
    //pDC->StrechBlt(0, 0, rect.Width, rect.Height(), &memdc, 0, 0, bm.bmWidth,bm.bmHeight, SRCCOPY);

   //释放DC
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


// CSeparatorDlg 对话框




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
	////提取的地址，单个封包
	DDX_Text(pDX, IDC_EDIT1, lpExtractAddr);
	DDV_MaxChars(pDX, lpExtractAddr, 2048);
	////提取的文件夹,多个封包
	DDX_Text(pDX, IDC_EDIT2, lpExtractDic);
	DDV_MaxChars(pDX, lpExtractDic, 2048);
	////指定引擎名字，比如很多引擎压缩包的后缀名为arc，为了提高识别率，需要填写这个
	DDX_Text(pDX, IDC_EDIT3, lpEngineName);
	DDV_MaxChars(pDX, lpEngineName, 2048);
	////是否保存中间数据，比如krkr2的tlg图片
	DDX_Text(pDX, IDC_EDIT4, bRestoreTmpData);
	DDV_MaxChars(pDX, bRestoreTmpData, 2048);
	////特殊命令
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


// CSeparatorDlg 消息处理程序

BOOL CSeparatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_SHOW);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSeparatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

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
		//CDialogEx::OnPaint();
		
		CPaintDC   dc(this);
		CRect      rect;
		GetClientRect(&rect);
		CDC        dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP4);   //IDB_BITMAP对应的ID                    
		BITMAP   bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSeparatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSeparatorDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSeparatorDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg *info = new CAboutDlg;
	info->Create(IDD_ABOUTBOX, this);
	info->ShowWindow(SW_SHOW);
	
}


void CSeparatorDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemTextW(IDC_EDIT1, lpEngineName);
}

/*
HBRUSH CSeparatorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
 
	if(nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);//设置背景透明
		pDC->SetTextColor(RGB(255,255,255));//设置字体为黄色
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}*/

/*
BOOL CSeparatorDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CAboutDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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
					wsprintf(Info, L"无法解析的参数: %s", Buffer);
					MessageBoxW(Info, L"Separator::Parse", MB_OK);
					break;
				}
			}
			else
			{
				wchar_t Info[1024];
				wmemset(Info, 0, sizeof(Info)/sizeof(wchar_t));
				wsprintf(Info, L"无法解析的副指令: %s", Buffer);
				MessageBoxW(Info, L"Separator::Parse", MB_OK);
				break;
			}
		}
		else
		{
			wchar_t Info[1024];
			wmemset(Info, 0, sizeof(Info)/sizeof(wchar_t));
			wsprintf(Info, L"无法解析的指令: %s", Buffer);
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
	// TODO: 在此添加控件通知处理程序代码
	if(!lpExtractAddr.IsEmpty() && !lpExtractDic.IsEmpty())
	{
		AfxMessageBox(L"只能选择一种模式!");
	}
	//单个文件
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
		AfxMessageBox(L"无效的输入!");
	}
	WorkList.remove(ExtractStart);
	delete ExtractStart;
	return;
}


//选择一个封包
void CSeparatorDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
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

//选择一个文件夹
void CSeparatorDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(FALSE);
    TCHAR szPath[MAX_PATH];     //存放选择的目录路径    
    CString str;  
    ZeroMemory(szPath, sizeof(szPath));     
      
    BROWSEINFO bi;     
    bi.hwndOwner = m_hWnd;     
    bi.pidlRoot = NULL;     
    bi.pszDisplayName = szPath;     
    bi.lpszTitle = L"请选择封包所在的目录";     
    bi.ulFlags = 0;     
    bi.lpfn = NULL;     
    bi.lParam = 0;     
    bi.iImage = 0;     
    //弹出选择目录对话框   
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);     
      
    if(lp && SHGetPathFromIDList(lp, szPath))
    {  
		//Debug时期处理
        //str.Format(L"选择的目录为 %s",  szPath);
        //AfxMessageBox(str);             
    }  
    else     
        AfxMessageBox(L"无效的目录!");

	lpExtractDic = szPath;
	UpdateData(FALSE);
}


void CSeparatorDlg::OnDropdownIdok(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


//特殊参数
/*
-using unicode
-force cp 936
-cmd   tmpfile
-run   script.as
*/
void CSeparatorDlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//指定引擎 NULL
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
