//
// myfileopen.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\picture.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "myfileopen.h"

#define DIBSIZEX 256
#define DIBSIZEY 192
#define DIBSIZEX2 128
#define DIBSIZEY2 96

void* CMyFileOpen::m_readBuffer = NULL;
//int CMyFileOpen::m_readBufferSize = 0;

HBITMAP CMyFileOpen::m_dib1 = NULL;
HBITMAP CMyFileOpen::m_dib2 = NULL;

LPVOID CMyFileOpen::m_dibBuffer1 = NULL;
LPVOID CMyFileOpen::m_dibBuffer2 = NULL;

BOOL CMyFileOpen::m_dibExistFlag1 = NULL;
BOOL CMyFileOpen::m_dibExistFlag2 = NULL;

char CMyFileOpen::m_infoText[4096];
char CMyFileOpen::m_currentDir[1024];
char CMyFileOpen::m_filterName[1024];

int CMyFileOpen::m_256x192Pic[256*192];

CPicture* CMyFileOpen::m_pic = NULL;

BOOL CMyFileOpen::m_sameZahyoFlag = FALSE;


CMyFileOpen::CMyFileOpen(HWND parentHWND,HINSTANCE hinstance,LPSTR lpStrDefExt,LPSTR lpStrFilter)
{
	m_parentHWND = parentHWND;
	m_hInstance = hinstance;

//	m_sameZahyoFlag = FALSE;
	m_sameEnableFlag = TRUE;

	m_fileName[0] = 0;
	m_fileTitle[0] = 0;
	m_fileNameOnly[0] = 0;

	m_defaultExt[0] = 0;
	m_defaultFilter[0] = 0;

	if (m_readBuffer == NULL)
	{
		m_readBuffer = new char[1024*1024*8];//8M
	}

	if (m_pic == NULL)
	{
		m_pic = new CPicture();
	}

	if (m_dibBuffer1 == NULL) CreateMyDIB(&m_dib1,&m_dibBuffer1,DIBSIZEX,DIBSIZEY);
	if (m_dibBuffer2 == NULL) CreateMyDIB(&m_dib2,&m_dibBuffer2,DIBSIZEX2,DIBSIZEY2);

	SetDefaultExt(lpStrDefExt);
	SetDefaultFilter(lpStrFilter);
}



CMyFileOpen::~CMyFileOpen()
{
	End();
}


void CMyFileOpen::End(void)
{
	DELETEGDIOBJECT(m_dib1);
	DELETEGDIOBJECT(m_dib2);

	ENDDELETECLASS(m_pic);

	DELETEARRAY(m_readBuffer);
}



void CMyFileOpen::SetDefaultExt(LPSTR lpStrDefExt)
{
	if (lpStrDefExt == NULL)
	{
		m_defaultExt[0] = 0;
		return;
	}
//	sprintf(m_defaultExt,"",lpStrDefExt);
	int ln = strlen(lpStrDefExt);
	if (ln>1022) ln = 1022;
	memcpy(m_defaultExt,lpStrDefExt,ln);
	m_defaultExt[ln] = 0;
	m_defaultExt[ln+1] = 0;
}

void CMyFileOpen::SetDefaultFilter(LPSTR lpStrDefFilter)
{
	char tmp[1024];
	char tmp2[1024];
	if (lpStrDefFilter == NULL)
	{
		wsprintf(tmp,"all(*.%s)",m_defaultExt,m_defaultExt);
		wsprintf(tmp2,"*.%s",m_defaultExt,m_defaultExt);
	}
	else
	{
		wsprintf(tmp,"all(*.%s)",m_defaultExt,lpStrDefFilter);
		wsprintf(tmp2,"*.%s",m_defaultExt,lpStrDefFilter);
	}

	int ln1 = strlen(tmp);
	int ln2 = strlen(tmp2);

	if ((ln1+ln2)>1021)
	{
		MessageBox(NULL,"SetDefaultFilter()","ERROR",MB_OK);
		return;
	}

	memcpy(m_defaultFilter,tmp,ln1+1);
	memcpy(m_defaultFilter+ln1+1,tmp2,ln2+1);
	m_defaultFilter[ln1+1+ln2+1] = 0;
}

LPSTR CMyFileOpen::SelectFile(LPSTR defaultName, LPSTR defaultDirName,LPSTR ext,int filterLength)
{
	Init();

	OPENFILENAME ofn;
	MYOPENFILENAME myofn;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));

	m_fileName[0] = 0;
	if (defaultName != NULL)
	{
		int ln = strlen(defaultName);
		if (ln>1022) ln = 1022;
		memcpy(m_fileName,defaultName,ln);
		m_fileName[ln] = 0;
		m_fileName[ln+1] = 0;
/*
		if (filterLength > 0)
		{
			if (ln >= filterLength)
			{
				if (_stricmp(ext,"dwq") == 0)
				{
					memcpy(&m_fileName[filterLength],"*.dwq",6);
				}
			}
		}
*/
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_parentHWND;

	ofn.hInstance = m_hInstance;

//	ofn.lpstrFilter = m_defaultFilter;

	ofn.lpstrFile = m_fileName;
	ofn.nMaxFile = 1024;

	char defaultFilter[1024];
	if (ext != NULL)
	{
		ofn.lpstrDefExt = ext;
		wsprintf(defaultFilter,"dwq_*.%s",ext);
		int zz = strlen(defaultFilter);
		defaultFilter[3] = 0;
		defaultFilter[zz] = 0;
		defaultFilter[zz+1] = 0;
		ofn.lpstrFilter = defaultFilter;
	}
	else
	{
		ofn.lpstrDefExt = m_defaultExt;
		ofn.lpstrFilter = m_defaultFilter;
	}


	ofn.lpstrTitle = "ファイルオープン";

	ofn.lpstrFileTitle = m_fileTitle;
	ofn.nMaxFileTitle = 256;

	ofn.Flags = OFN_HIDEREADONLY | OFN_ENABLEHOOK | OFN_EXPLORER | OFN_ENABLETEMPLATE;// | OFN_ENABLEINCLUDENOTIFY ;//@@@@@@@@@@@@@
	ofn.lpfnHook = OFNHookProc;

	ofn.lpstrInitialDir = defaultDirName;

	ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DIALOG3);

//	char tmp[1024];
	GetCurrentDirectory(1024,m_currentDir);

	ofn.lCustData = (DWORD)&myofn;

	myofn.enableFlag = TRUE;
	myofn.checkFlag = m_sameZahyoFlag;
	myofn.filterLength = filterLength;
	myofn.filename = m_fileName;

	if (m_sameEnableFlag == FALSE) myofn.checkFlag = FALSE;

	if (GetOpenFileName(&ofn) == 0)
	{
		SetCurrentDirectory(m_currentDir);
		return NULL;
	}

	m_sameZahyoFlag = myofn.checkFlag;
	m_filterLength = myofn.filterLength;


//filename set etc

//	FILE* file = fopen(m_fileName,"rb");
	SetCurrentDirectory(m_currentDir);

	MakeFileNameOnly();

	return m_fileNameOnly;
//	return file;
}


FILE* CMyFileOpen::OpenLoad(LPSTR defaultName, LPSTR defaultDirName)
{
	Init();

	OPENFILENAME ofn;
	MYOPENFILENAME myofn;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));

	m_fileName[0] = 0;
	if (defaultName != NULL)
	{
		int ln = strlen(defaultName);
		if (ln>1022) ln = 1022;
		memcpy(m_fileName,defaultName,ln);
		m_fileName[ln] = 0;
		m_fileName[ln+1] = 0;
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_parentHWND;

	ofn.hInstance = m_hInstance;

	ofn.lpstrFilter = m_defaultFilter;

	ofn.lpstrFile = m_fileName;
	ofn.nMaxFile = 1024;

	ofn.lpstrDefExt = m_defaultExt;
	ofn.lpstrTitle = "ファイルオープン";

	ofn.lpstrFileTitle = m_fileTitle;
	ofn.nMaxFileTitle = 256;

	ofn.Flags = OFN_HIDEREADONLY | OFN_ENABLEHOOK | OFN_EXPLORER | OFN_ENABLETEMPLATE;
	ofn.lpfnHook = OFNHookProc;

	ofn.lpstrInitialDir = defaultDirName;

	ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DIALOG3);

//	char tmp[1024];
	GetCurrentDirectory(1024,m_currentDir);

	ofn.lCustData = (DWORD)&myofn;

	myofn.enableFlag = TRUE;
	myofn.checkFlag = m_sameZahyoFlag;
	myofn.filterLength = -1;
	myofn.filename = m_fileName;

	if (m_sameEnableFlag == FALSE) myofn.checkFlag = FALSE;

	if (GetOpenFileName(&ofn) == 0)
	{
		SetCurrentDirectory(m_currentDir);
		return NULL;
	}

	m_sameZahyoFlag = myofn.checkFlag;

//filename set etc

	FILE* file = CMyFile::Open(m_fileName,"rb");
	SetCurrentDirectory(m_currentDir);

	MakeFileNameOnly();

	return file;
}

HWND CMyFileOpen::m_filenameEditBox = NULL;

BOOL CALLBACK CMyFileOpen::MyEditBoxEnum(HWND hwnd,LPARAM lParam)
{
	char className[256];
	GetClassName(hwnd,className,256);

//	WINDOWINFO wi;
//	GwtWindowInfo(hwnd,&wi);

	char mes[1024];
	wsprintf(mes,"\nchild=%d %s",hwnd,className);
	OutputDebugString(mes);

	if (_stricmp(className,"Edit") == 0)
	{
		m_filenameEditBox = hwnd;
//		EnableWindow(editBox,FALSE);
		return FALSE;
	}

	return TRUE;
}


UINT CALLBACK CMyFileOpen::OFNHookProc(HWND hdlg, UINT uiMsg,  WPARAM wParam,  LPARAM lParam)
{
	static OPENFILENAME* lpOfn = NULL;
	static MYOPENFILENAME* lpMyOfn = NULL;
	static HWND checkBox = NULL;
	static HWND filterCombo = NULL;
//	static int filterBoxList[16+1];
	static HWND filterButton = NULL;

	static int oldFilterCombo = 0;
	static int firstFlag = 0;


	if (uiMsg == WM_INITDIALOG)
	{
		firstFlag = 0;

		OPENFILENAME* lpOfn = (OPENFILENAME*)lParam;
		lpMyOfn = (MYOPENFILENAME*)(lpOfn->lCustData);


		if (1)
		{
			LPSTR orgName = lpMyOfn->filename;
			
			int ln1 = strlen(orgName);
			if (ln1>1022) ln1 = 1022;
			memcpy(m_filterName,orgName,ln1+1);
		}


		checkBox = GetDlgItem(hdlg,IDC_CHECK_SAMEZAHYO);
		if (checkBox != NULL)
		{
			EnableWindow(checkBox,lpMyOfn->enableFlag);
			
			if (lpMyOfn->checkFlag)
			{
				SendMessage(checkBox,BM_SETCHECK,(WPARAM)(BST_CHECKED),(LPARAM)0);
			}
			else
			{
				SendMessage(checkBox,BM_SETCHECK,(WPARAM)(BST_UNCHECKED),(LPARAM)0);
			}
		}

		filterButton = GetDlgItem(hdlg,IDC_DWQFILTERBUTTON);

		oldFilterCombo = lpMyOfn->filterLength;

		filterCombo = GetDlgItem(hdlg,IDC_FILELENGTHFILTER_COMBO);
		int filterLength = lpMyOfn->filterLength;
		if (filterCombo != NULL)
		{
			if (filterLength < 0)
			{
				EnableWindow(filterCombo,FALSE);
			}
			else
			{
				EnableWindow(filterCombo,TRUE);
				SendMessage(filterCombo,CB_INSERTSTRING,0,(LPARAM)"なし");
				for (int i=1;i<=16;i++)
				{
					char mes[32];
					wsprintf(mes,"%d",i);
					SendMessage(filterCombo,CB_INSERTSTRING,i,(LPARAM)mes);
				}

				int p = filterLength;
				if (p<0) p = 0;
				if (p>16) p = 16;

				SendMessage(filterCombo,CB_SETCURSEL,p,0);
				//enable window and set select
			}
		}

		if (filterButton != NULL)
		{
			if (filterLength < 0)
			{
				EnableWindow(filterButton,FALSE);
			}
			else
			{
				EnableWindow(filterButton,TRUE);
			}
		}




	}


	if (uiMsg == WM_PAINT)
	{


		HDC hdc = GetDC(hdlg);

//		TextOut(hdc,30,330,"TEST MESSAGE",strlen("TEST MESSAGE"));

		RECT dialogRect;
		GetWindowRect(hdlg,&dialogRect);

		HDC src = CreateCompatibleDC(hdc);

		int deltaY = dialogRect.bottom - dialogRect.top - DIBSIZEY - 32;

		HGDIOBJ old;
		if ((m_dib1 != NULL) && (m_dibExistFlag1))
		{
			old = SelectObject(src,m_dib1);
			BitBlt(hdc,30,deltaY,DIBSIZEX,DIBSIZEY,src,0,0,SRCCOPY);
			RECT rc0;
			SetRect(&rc0,30,deltaY,30+DIBSIZEX,deltaY+DIBSIZEY);
			ValidateRect(hdlg,&rc0);
			SelectObject(src,old);
		}

		if ((m_dib2 != NULL) && (m_dibExistFlag2))
		{
			old = SelectObject(src,m_dib2);
			BitBlt(hdc,288,96+deltaY,DIBSIZEX2,DIBSIZEY2,src,0,0,SRCCOPY);
			RECT rc0;
			SetRect(&rc0,288,96+deltaY,288+DIBSIZEX2,96+deltaY+DIBSIZEY2);
			ValidateRect(hdlg,&rc0);
			SelectObject(src,old);
		}

		RECT rc1;
		SetRect(&rc1,288,deltaY,288+128,96+deltaY);
		HBRUSH br = (HBRUSH)GetStockObject(WHITE_BRUSH);
		FillRect(hdc,&rc1,br);
		DrawText(hdc,m_infoText,-1,&rc1,0);
		ValidateRect(hdlg,&rc1);


		DeleteDC(src);

		ReleaseDC(hdlg,hdc);

	}

	if (uiMsg == WM_COMMAND)
	{
		WORD wID = LOWORD(wParam);

		if (checkBox != NULL)
		{
			if (SendMessage(checkBox,BM_GETCHECK,0,0) == BST_CHECKED)
			{
				lpMyOfn->checkFlag = TRUE;
			}
			else
			{
				lpMyOfn->checkFlag = FALSE;
			}
		}

		if (wID == IDC_FILELENGTHFILTER_COMBO)
		{
			if (filterCombo != NULL)
			{
				int newFilterCombo = SendMessage(filterCombo,CB_GETCURSEL,0,0);
				if (newFilterCombo != oldFilterCombo)
				{
					oldFilterCombo = newFilterCombo;
					//change!
					char mes[256];
					wsprintf(mes,"\nChange COMBO %d",newFilterCombo);
					OutputDebugString(mes);

					if (m_filenameEditBox != NULL)
					{
						lpMyOfn->filterLength = newFilterCombo;
						ChangeDWQFilter(lpMyOfn);
					}
				}
			}
		}

		if (filterButton != NULL)
		{
			if (wID == IDC_DWQFILTERBUTTON)
			{
//				OutputDebugString("\nButton");

				if (filterCombo != NULL)
				{
					int newFilterCombo = SendMessage(filterCombo,CB_GETCURSEL,0,0);
//					if (newFilterCombo != oldFilterCombo)
					if (1)
					{
						oldFilterCombo = newFilterCombo;
						//change!
						char mes[256];
						wsprintf(mes,"\nChange COMBO %d",newFilterCombo);
						OutputDebugString(mes);

						if (m_filenameEditBox != NULL)
						{
							lpMyOfn->filterLength = newFilterCombo;
							ChangeDWQFilter(lpMyOfn);
						}
					}
				}

				//endMessage(filterButton,BM_GETCHECK
			}
		}

	}

	if (uiMsg == WM_NOTIFY)
	{
		OFNOTIFY* of = (OFNOTIFY*)lParam;

//		if (of->hdr.code == CDN_INCLUDEITEM)
		if (of->hdr.code == CDN_INITDONE)
		{
			m_filenameEditBox = NULL;
			EnumChildWindows(of->hdr.hwndFrom,MyEditBoxEnum,NULL);




		}
		else if (of->hdr.code == CDN_FOLDERCHANGE)
		{
/*
			HWND stc = of->hdr.hwndFrom;
			listBox = NULL;
			EnumChildWindows(stc,MyEnum,NULL);

			if (listBox != NULL)
			{
				char mes[1024];
				int ct = SendMessage(listBox,LB_GETCOUNT,0,0);
				wsprintf(mes,"\n\n[%d] \n\n",ct);
				OutputDebugString(mes);
			}
*/

/*





			char bf[1024];
			for (int i=0;i<100;i++) bf[i] = 1;

//			HWND sb = GetDlgItem(stc,lst1);
			HWND sb = GetDlgItem(stc,stc1);

			int rd = SendMessage(stc,CDM_GETFOLDERIDLIST,1024,(LPARAM)bf);
			int d1 = 0xff & (int)(bf[0]);
			int d2 = 0xff & (int)(bf[1]);





//			SendMessage(sb,CDM_HIDECONTROL,0,0);
//			ShowWindow(sb,SW_HIDE);
//			EnableWindow(sb,FALSE);
//			SendMessage(sb,LB_SETSEL,TRUE,3);

			if (listBox != NULL)
			{
				int ct = SendMessage(stc,LB_GETCOUNT,0,0);

				wsprintf(mes,"\n\nCDN_INITDONE size=%d [%d] %d\n\n",rd,sb,ct);
//				EnableWindow(listBox,FALSE);
//				ShowWindow(listBox,SW_HIDE);
			}

			//SendMessage(sb,LB_SETSEL,TRUE,3);



			OutputDebugString(mes);
*/
		}
		else if (of->hdr.code == CDN_SELCHANGE)
		{





			m_infoText[0] = 0;
			m_dibExistFlag1 = FALSE;
			m_dibExistFlag2 = FALSE;

			char filename[1024];

			HWND stc = GetDlgItem(hdlg,of->hdr.idFrom);
			RECT rc1;
			GetWindowRect(of->hdr.hwndFrom,&rc1);
			RECT rc0;
			GetWindowRect(hdlg,&rc0);


	//	listBox = NULL;
	//	EnumChildWindows(stc,MyEnum,NULL);
	//	if (listBox != NULL)
	//	{
	//		EnableWindow(listBox,FALSE);
	//		ShowWindow(listBox,SW_HIDE);
	//	}


//			listBox = NULL;
//			EnumChildWindows(stc,MyEnum,NULL);
//			if (listBox != NULL)
//			{
//				char mes[1024];
//				int ct = SendMessage(stc,LB_GETCOUNT,0,0);
//				wsprintf(mes,"\n\nlistbox=%d\n\n",ct);
//				OutputDebugString(mes);
////				EnableWindow(listBox,FALSE);
//	//			ShowWindow(listBox,SW_HIDE);
//			}


//			int rt = SendMessage(of->hdr.hwndFrom,CDM_GETSPEC,(WPARAM)1024,(LPARAM)filename);
			int rt = SendMessage(of->hdr.hwndFrom,CDM_GETFILEPATH,(WPARAM)1024,(LPARAM)filename);

			if (rt>0)
			{
//				HANDLE h = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL);
				FILE* file = CMyFile::Open(filename,"rb");
				if (file != NULL)
				{
					fclose(file);

					LPSTR ext = NULL;

					int flen = strlen(filename);
					if (flen >= 3)
					{
						ext = filename + flen - 3;
					}

					if (flen != NULL)
					{

						if (_stricmp(ext,"dwq")==0)
						{
							// thumbnail 
							MakeDWQInfo(filename);
						}
						else if (_stricmp(ext,"nnn")==0)
						{
							MakeNNNInfo(filename);
						}
					}



				}
			}
//			InvalidateRect(of->hdr.hwndFrom,NULL,TRUE);
//			RECT rc;
//			SetRect(&rc,0,0,1000,1000);
			InvalidateRect(hdlg,NULL,TRUE);
		}
	}
	return 0;
}


UINT CMyFileOpen::OFNHookProcOldStyle(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_LBUTTONDOWN)
	{
	}
	return 0;
}

UINT CMyFileOpen::MyDialogProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}


FILE* CMyFileOpen::OpenSave(LPSTR defaultName, LPSTR defaultDirName,BOOL backupFlag)
{
	Init();

	MYOPENFILENAME myofn;
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));

	m_fileName[0] = 0;
	if (defaultName != NULL)
	{
		int ln = strlen(defaultName);
		if (ln>1022) ln = 1022;
		memcpy(m_fileName,defaultName,ln);
		m_fileName[ln] = 0;
		m_fileName[ln+1] = 0;
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_parentHWND;

	ofn.lpstrFilter = m_defaultFilter;

	ofn.lpstrFile = m_fileName;
	ofn.nMaxFile = 1024;

	ofn.lpstrDefExt = m_defaultExt;
	ofn.lpstrTitle = "保存";

	ofn.lpstrFileTitle = m_fileTitle;
	ofn.nMaxFileTitle = 256;

	ofn.lpstrInitialDir = defaultDirName;

	ofn.hInstance = m_hInstance;

	ofn.Flags = OFN_HIDEREADONLY | OFN_ENABLEHOOK | OFN_EXPLORER | OFN_ENABLETEMPLATE | OFN_OVERWRITEPROMPT;
	ofn.lpfnHook = OFNHookProc;

	ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DIALOG3);

	myofn.enableFlag = FALSE;
	
//	myofn.checkFlag = FALSE;
	myofn.checkFlag = m_sameZahyoFlag;


	myofn.filterLength = -1;
	myofn.filename = m_fileName;

	ofn.lCustData = (DWORD)&myofn;


	char tmp[1024];
	GetCurrentDirectory(1024,tmp);
	if (GetSaveFileName(&ofn) == 0)
	{
		SetCurrentDirectory(tmp);
		return NULL;
	}

	//bad name?
	//

	if (CheckFilenameHaveSpace(m_fileTitle))
	{
		SetCurrentDirectory(tmp);
		MessageBox(NULL,"system-NNNでは、スペース入りのファイル名は使用できません","open save error",MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}




	if (backupFlag)
	{
		char newFileName[1024];
		int ln5 = strlen(m_fileName);
		memcpy(newFileName,m_fileName,ln5+1);
		//
		if (ln5>=5)
		{
			newFileName[ln5-4] = '.';
			newFileName[ln5-3] = 'b';
			newFileName[ln5-2] = 'a';
			newFileName[ln5-1] = 'k';

			CopyFile(m_fileName,newFileName,FALSE);
		}
	}

//filename set etc





	FILE* file = CMyFile::Open(m_fileName,"wb");
	SetCurrentDirectory(tmp);

	MakeFileNameOnly();

	return file;
}

void CMyFileOpen::InitFileName(LPSTR filename)
{
	if (filename == NULL)
	{
		m_fileName[0] = 0;
		m_fileNameOnly[0] = 0;
		m_fileTitle[0] = 0;
		return;
	}

	int ln = strlen(filename);
	if (ln>1022) ln = 1022;
	memcpy(m_fileName,filename,ln);
	m_fileName[ln] = 0;
	m_fileName[ln+1] = 0;

	GetFileTitle(m_fileName,m_fileTitle,256);
	MakeFileNameOnly();
}



LPSTR CMyFileOpen::GetFileName(void)
{
	return m_fileName;
}

LPSTR CMyFileOpen::GetFileNameOnly(void)
{
	return m_fileNameOnly;
}

void CMyFileOpen::MakeFileNameOnly(void)
{
	int ln = strlen(m_fileTitle);
	if (ln>254) ln = 254;

	for (int i=0;i<ln;i++)
	{
		char c = m_fileTitle[i];
		m_fileNameOnly[i] = c;
		if ((c == 0) || (c == '.'))
		{
			m_fileNameOnly[i] = 0;
			break;
		}
	}

//	memcpy(m_fileNameOnly,m_fileTitle,ln);
	m_fileNameOnly[ln] = 0;
	m_fileNameOnly[ln+1] = 0;
}

void CMyFileOpen::Init(void)
{
	m_dibExistFlag1 = FALSE;
	m_dibExistFlag2 = FALSE;
	m_infoText[0] = 0;
}


void CMyFileOpen::CreateMyDIB(HBITMAP* lpBitmap, LPVOID* lplpBuffer, int sizeX, int sizeY)
{
	HDC hdc = GetDC(m_parentHWND);
	BITMAPINFO bmi;

	ZeroMemory(&bmi,sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biHeight = -sizeY;
	bmi.bmiHeader.biWidth = sizeX;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);


	*lpBitmap = CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,lplpBuffer,NULL,NULL);

	int* ptr = (int*)(*lplpBuffer);

	ReleaseDC(m_parentHWND,hdc);
}



void CMyFileOpen::MakeDWQInfo(LPSTR filename)
{
	int i,j;

	if (m_pic == NULL) return;
	if (m_dib1 == NULL) return;
	if (m_dib2 == NULL) return;
	if (m_dibBuffer1 == NULL) return;
	if (m_dibBuffer2 == NULL) return;

	//full path to dir\\filename (.dwq)

	//search dwq

	char* found = strstr(filename,"\\dwq\\");
	if (found == NULL)
	{
		found = strstr(filename,"\\DWQ\\");
	}

	if (found == NULL)
	{
		found = strstr(filename,"\\Dwq\\");
	}

	if (found == NULL) return;


	LPSTR filename2 = found + 5;

	char dirfilename[1024];
	int ln = strlen(filename2);
	for (i=0;i<ln;i++)
	{
		char c = *(filename2+i);
		if ((c == '.') || (c == 0))
		{
			dirfilename[i] = 0;
			break;
		}
		dirfilename[i] = c;
	}
	dirfilename[ln] = 0;




	char tmp[1024];
	GetCurrentDirectory(1024,tmp);
	SetCurrentDirectory(m_currentDir);
	BOOL b = m_pic->LoadDWQ(dirfilename);
	SetCurrentDirectory(tmp);

	if (b == FALSE) return;

	RECT rc;
	m_pic->GetPicSize(&rc);
	int sizeX = rc.right;
	int sizeY = rc.bottom;




	wsprintf(m_infoText,"sizeX=%d\r\nsizeY=%d\r\n",sizeX,sizeY);

	int* src = (int*)m_pic->GetBuffer();

	int* dst = (int*)m_dibBuffer1;

	int loopX = DIBSIZEX;
	int loopY = DIBSIZEY;

	if ((DIBSIZEX * sizeY) > (DIBSIZEY * sizeX))
	{
		loopX = (DIBSIZEY * sizeX) / sizeY;
	}
	else if ((DIBSIZEX * sizeY) < (DIBSIZEY * sizeX))
	{
		loopY = (DIBSIZEX * sizeY) / sizeX;
	}

	if (loopX==0) loopX = 1;
	if (loopY==0) loopY = 1;

	if ((loopX != DIBSIZEX) || (loopY != DIBSIZEY))
	{
		for (i=0;i<DIBSIZEX*DIBSIZEY;i++)
		{
			*(dst+i) = 0;
		}
	}

	int* dst0 = dst;
	for (j=0;j<loopY;j++)
	{
		dst = dst0;
		int y = (j * sizeY) / loopY;
		for (i=0;i<loopX;i++)
		{
			int x = (i * sizeX) / loopX;
			int d = *(src + x + y * sizeX);
			*dst = d;
			dst++;
		}
		dst0 += DIBSIZEX;
	}

	m_dibExistFlag1 = TRUE;

	if (m_pic->GetFileMode() != 2) return;

	char* mask = (char*)m_pic->GetMaskPic();

	dst = (int*)m_dibBuffer2;
	dst0 = dst;

	loopX = DIBSIZEX2;
	loopY = DIBSIZEY2;

	if ((DIBSIZEX2 * sizeY) > (DIBSIZEY2 * sizeX))
	{
		loopX = (DIBSIZEY2 * sizeX) / sizeY;
	}
	else if ((DIBSIZEX2 * sizeY) < (DIBSIZEY2 * sizeX))
	{
		loopY = (DIBSIZEX2 * sizeY) / sizeX;
	}

	if (loopX==0) loopX = 1;
	if (loopY==0) loopY = 1;

	if ((loopX != DIBSIZEX2) || (loopY != DIBSIZEY2))
	{
		for (i=0;i<DIBSIZEX2*DIBSIZEY2;i++)
		{
			*(dst+i) = 0;
		}
	}

	for (j=0;j<loopY;j++)
	{
		dst = dst0;
		int y = (j * sizeY) / loopY;
		for (i=0;i<loopX;i++)
		{
			int x = (i * sizeX) / loopX;
			int c = *(mask + x + y * sizeX);
			c &= 0xff;

			int d = (c<<16) | (c<<8) | c;
			*dst = d;
			dst++;
		}
		dst0 += DIBSIZEX2;
	}

	m_dibExistFlag2 = TRUE;
}




void CMyFileOpen::MakeNNNInfo(LPSTR filename)
{
	FILE* file = CMyFile::Open(filename,"rb");
	if (file != NULL)
	{
		fread(m_infoText,sizeof(char),16+64,file);	//skip
		fread(m_infoText,sizeof(char),256,file);

		int tmp[16];
		fread(tmp,sizeof(int),16,file);
		if (tmp[1] == 1)
		{
			fread(m_dibBuffer2,sizeof(int),128*96,file);
			m_dibExistFlag2 = TRUE;
		}
		else if (tmp[1] == 2)
		{
			fread(m_dibBuffer1,sizeof(int),256*192,file);
			m_dibExistFlag1 = TRUE;
		}

		fclose(file);
	}
}

void CMyFileOpen::SetSameEnable(BOOL b)
{
	m_sameEnableFlag = b;
}


void CMyFileOpen::SetSameZahyoFlag(BOOL b)
{
	m_sameZahyoFlag = b;
}

BOOL CMyFileOpen::GetSameZahyoFlag(void)
{
	return m_sameZahyoFlag;
}


BOOL CMyFileOpen::CheckFilenameHaveSpace(LPSTR fullName)
{
	int ln = strlen(fullName);
	if (ln>254) ln = 254;

	int i = 0;
	while (i<ln)
	{
		unsigned char c = fullName[i];

		if (c == ' ')
		{
//OutputDebugString("space!");
			return TRUE;
		}

		if ((c == 0) || (c == '.'))
		{
//OutputDebugString("end!");
			return FALSE;
		}

		int cc = ((int)c) & 0xff;

		//if (c>= (unsigned char)0x80)
		if (((cc >= 0x80) && (cc < 0xa0)) || ((cc >= 0xe0) && (cc < 0x100)))
		{
			i++;
		}

		//if (c >= (unsigned char)0x80)
		//{
			//i++;
		//}
		i++;
	}
//OutputDebugString("last!");

	return FALSE;
}


void CMyFileOpen::ChangeDWQFilter(MYOPENFILENAME* lpMy)
{
	//
	char editBofFileName[1024];
	editBofFileName[0] = 0;
	GetWindowText(m_filenameEditBox,editBofFileName,1022);
	//check have *
	int ln1 = strlen(editBofFileName);
	int n = 0;
	BOOL found = FALSE;

	while (n<ln1)
	{
		char c = editBofFileName[n];
		if (c == '*')
		{
			found = TRUE;
			break;
		}

		int cc = (int)c;
		cc &= 0xff;

		n++;

		if ((cc >= 0x80) && (cc<=0x9f)) n++;
		if ((cc >= 0xe0) && (cc<=0xff)) n++;
	}

	if (found == FALSE)
	{
		memcpy(m_filterName,editBofFileName,ln1+1);
	}

//	LPSTR orgFileName = lpMy->filename;
	LPSTR orgFileName = m_filterName;
	int ln = strlen(orgFileName);
	int filterLength = lpMy->filterLength;
	if (ln >= filterLength)
	{
		if (*orgFileName != '.')
		{
			char filterName[1024];
			memcpy(filterName,orgFileName,filterLength);
			memcpy(filterName + filterLength,"*.dwq",6);
			SetWindowText(m_filenameEditBox,filterName);
						SetFocus(m_filenameEditBox);
			PostMessage(m_filenameEditBox,WM_KEYDOWN,VK_RETURN,0);
		}
	}



}


