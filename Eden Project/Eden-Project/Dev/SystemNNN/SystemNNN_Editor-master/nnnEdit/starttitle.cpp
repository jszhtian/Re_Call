//
// starttitle.cpp
//

#include <windows.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "define.h"

#include "myBitmap.h"


#include "starttitle.h"

CStartTitle* CStartTitle::m_this = NULL;



CStartTitle::CStartTitle(HWND clientHWND,HINSTANCE hinstance)
{
	m_clientHWND = clientHWND;
	m_hInstance = hinstance;

	m_bitmap = new CMyBitmap("nnndir\\setup\\bmp\\nekotitle.bmp");
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_TITLE));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_JOREMI));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_AEGI));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_BLACK));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_WA));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_NEKO));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_BURANE));

	m_hBitmap = m_bitmap->GetHBitmap();

	m_timerFlag = FALSE;

//	m_windowX = 0;
//	m_windowY = 0;
//	m_windowSizeX = 640;
//	m_windowSizeY = 480;

	m_hWnd = NULL;


	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = NULL;
	wc.hIcon = NULL;
	wc.hInstance = m_hInstance;
	wc.lpfnWndProc = staticWndProc;
	wc.lpszClassName = "starttitleclass";
	wc.lpszMenuName = NULL;
	wc.style = 0;

    RegisterClass(&wc);

	HWND hwnd0 = GetDesktopWindow();

	HDC hdc0 = GetDC(hwnd0);
	int dskX0 = GetDeviceCaps(hdc0,HORZRES);
	int dskY0 = GetDeviceCaps(hdc0,VERTRES);
	ReleaseDC(hwnd0,hdc0);


	RECT rc;
	GetWindowRect(clientHWND,&rc);

	int dskX = (rc.right - rc.left) / 2;
	int dskY = (rc.bottom - rc.top) / 2;

	SIZE size = m_bitmap->GetSize();

	int sx = size.cx;
	int sy = size.cy;

	int wx = dskX0 / 2 - sx / 2;
	int wy = dskY0 / 2 - sy / 2;


	m_hWnd = CreateWindow("starttitleclass","",WS_POPUP | WS_VISIBLE,
					wx,wy,
					sx,sy,

//					m_clientHWND,
					NULL,

					NULL,
					m_hInstance,NULL);

	m_this = this;
}





CStartTitle::~CStartTitle()
{
	End();
}

void CStartTitle::End(void)
{
	if (m_timerFlag)
	{
		m_timerFlag = FALSE;
		KillTimer(m_hWnd,555);
	}

	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
//	DELETEGDIOBJECT(m_hBitmap);
	ENDDELETECLASS(m_bitmap);
}


LRESULT CALLBACK CStartTitle::staticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_this != NULL)
	{
		return m_this->MyWndProc(hWnd,message,wParam,lParam);
	}



	return DefWindowProc(hWnd,message,wParam,lParam);
}

LRESULT CStartTitle::MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

//char mes1[256] = "アドレス 0x02719029で不正なアクセスが発生しました。";
//char mes2[256] = "EAX=8855648 EBX=7342345 ECX=7410643 EDX=6298882";
//char mes3[256] = "CTRL + ALT + DELを押してマシンを再起動してください。";

	switch (message)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;
		HDC src;
		HGDIOBJ old;

		hdc = BeginPaint(hWnd,&ps);
/*
HBRUSH br;
br = CreateSolidBrush(0xff0000);
RECT rc;
SetRect(&rc,0,0,1600,1200);
FillRect(hdc,&rc,br);
DeleteObject(br);
*/

		src = CreateCompatibleDC(hdc);
		old = SelectObject(src,m_hBitmap);
		BitBlt(hdc,0,0,430,270,src,0,0,SRCCOPY);
		SelectObject(src,old);
		DeleteDC(src);
/*
COLORREF col,bk;
int md;
col = GetTextColor(hdc);
bk = GetBkColor(hdc);
md = GetBkMode(hdc);

SetBkMode(hdc,TRANSPARENT);
SetTextColor(hdc,COLORREF(0xffffff));
SetBkColor(hdc,COLORREF(0));

TextOut(hdc,480,500,mes1,strlen(mes1));
TextOut(hdc,480,550,mes2,strlen(mes2));
TextOut(hdc,480,580,mes3,strlen(mes3));

SetBkMode(hdc,md);
SetTextColor(hdc,col);
SetBkColor(hdc,bk);
*/

		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		PostMessage(hWnd,WM_CLOSE,0,0);
		if (m_timerFlag)
		{
			KillTimer(hWnd,555);
			m_timerFlag = FALSE;
		}
		break;
	case WM_TIMER:
		m_timerFlag = FALSE;
		KillTimer(hWnd,555);
		PostMessage(hWnd,WM_CLOSE,0,0);
		break;
	case WM_DESTROY:
		m_hWnd = NULL;
		break;
//	case WM_CLOSE:
//		DestroyWindow(hWnd);
//		break;
	}

	return DefWindowProc(hWnd,message,wParam,lParam);
}

void CStartTitle::Show(void)
{
	ShowWindow(m_hWnd,SW_SHOW);
}

void CStartTitle::Start(void)
{
	SetTimer(m_hWnd,555,2000,NULL);
}

/*_*/

