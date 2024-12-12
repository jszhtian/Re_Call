//
// myclientwindow.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "define.h"

#include "myclientwindow.h"

CMyClientWindow* CMyClientWindow::m_this = NULL;

long CMyClientWindow::m_oldClientProc = NULL;


CMyClientWindow::CMyClientWindow(HWND hWndParent, HINSTANCE hinstance)
{
	m_this = this;

	m_hWndParent = hWndParent;
	m_hInstance = hinstance;


//	LPCREATESTRUCT lpcs;
	CLIENTCREATESTRUCT ccs;


//	lpcs  = (LPCREATESTRUCT)lParam;


	//			ccs.hWindowMenu  = GetSubMenu(lpcs->hMenu,IDMC_WINDOW);
	ccs.hWindowMenu  = NULL;
	ccs.idFirstChild = ID_MDICHILD;

	m_hWnd = CreateWindow(	"MDICLIENT",NULL,
							WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|
							WS_HSCROLL|WS_VSCROLL,
//						  0,0,0,0,hWnd,lpcs->hMenu,
							0,0,0,0,m_hWndParent,NULL,
							m_hInstance,(LPVOID)&ccs);

	m_oldClientProc =  GetWindowLong(m_hWnd,GWL_WNDPROC);
///	SetWindowLong(m_hWnd,GWL_WNDPROC,(long)(staticClientWndProc));

//	SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)staticClientWndProc);
}


CMyClientWindow::~CMyClientWindow()
{
}


void CMyClientWindow::End(void)
{
	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}


HWND CMyClientWindow::GetHWND(void)
{
	return m_hWnd;
}

/*
HWND CMyClientWindow::CreateMyWindow(void)
{

			
	LPCREATESTRUCT lpcs;
	CLIENTCREATESTRUCT ccs;
	HINSTANCE hInst;

			
	hInst = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);


			
	lpcs  = (LPCREATESTRUCT)lParam;


//			ccs.hWindowMenu  = GetSubMenu(lpcs->hMenu,IDMC_WINDOW);
			ccs.hWindowMenu  = NULL;


			ccs.idFirstChild = ID_MDICHILD;

			m_hWndClient = CreateWindow("MDICLIENT",NULL,
							  WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|
							  WS_HSCROLL|WS_VSCROLL,
							  0,0,0,0,hWnd,lpcs->hMenu,
							  hInst,(LPVOID)&ccs);

	return m_hWnd;
}
*/

/*
			LPCREATESTRUCT lpcs;
			CLIENTCREATESTRUCT ccs;
			HINSTANCE hInst;
//			LOGFONT   lf;

//			InitCommonControls();
//			hRtfLib = LoadLibrary("RICHED32.DLL");
			hInst = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);


			lpcs  = (LPCREATESTRUCT)lParam;


//			ccs.hWindowMenu  = GetSubMenu(lpcs->hMenu,IDMC_WINDOW);
			ccs.hWindowMenu  = NULL;


			ccs.idFirstChild = ID_MDICHILD;

			m_hWndClient = CreateWindow("MDICLIENT",NULL,
							  WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|
							  WS_HSCROLL|WS_VSCROLL,
							  0,0,0,0,hWnd,lpcs->hMenu,
							  hInst,(LPVOID)&ccs);
*/


LRESULT CALLBACK CMyClientWindow::staticClientWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


//return DefWindowProc(hWnd,message,wParam,lParam);

	if (m_this != NULL)
	{
		return m_this->ClientWndProc(hWnd,message,wParam,lParam);
	}

char mes[256];
if (message == WM_LBUTTONDOWN)
{
	wsprintf(mes,"[%d %d %d]",message,wParam,lParam);
	OutputDebugString(mes);
}

	if (m_oldClientProc != NULL)
	{
	}

	return DefWindowProc(hWnd,message,wParam,lParam);
}


LRESULT CMyClientWindow::ClientWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_oldClientProc != NULL)
	{
		LRESULT (*testFunc)(HWND,UINT,WPARAM,LPARAM);

		testFunc = (LRESULT(*)(HWND,UINT,WPARAM,LPARAM))m_oldClientProc;

		return testFunc(hWnd,message,wParam,lParam);
		//LRESULT (testFunc)(HWND,MESSAGE,WPARAM,LPARAM) = (LRESULT()(HWND,MESSAGE,WPARAM,LPARAM))m_oldClientProc;

		//return (LRESULT(*)(HWND,MESSAGE,WPARAM,LPARAM))(m_oldClientProc)(hWnd,message,wParam,lParam);
	}


	if ((hWnd != m_hWnd) && (hWnd != NULL) && (m_hWnd != NULL))
	{
		return DefWindowProc(hWnd,message,wParam,lParam);
	}

	HGDIOBJ old;
	HDC hdc;
	RECT rcClient;

//	return DefWindowProc(hWnd,message,wParam,lParam);

	switch (message)
	{
	case WM_ERASEBKGND:

		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd,&ps);
		hdc = ps.hdc;
		old = SelectObject(hdc,GetStockObject(BLACK_BRUSH));
		GetClientRect(hWnd,&rcClient);
		PatBlt(hdc,0,24,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 24,PATCOPY);
		SelectObject(hdc,old);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_LBUTTONDOWN:
		hdc = NULL;
		break;
	}


	if (m_oldClientProc != NULL)
	{
		LRESULT (*testFunc)(HWND,UINT,WPARAM,LPARAM);

		testFunc = (LRESULT(*)(HWND,UINT,WPARAM,LPARAM))m_oldClientProc;

		return testFunc(hWnd,message,wParam,lParam);
		//LRESULT (testFunc)(HWND,MESSAGE,WPARAM,LPARAM) = (LRESULT()(HWND,MESSAGE,WPARAM,LPARAM))m_oldClientProc;

		//return (LRESULT(*)(HWND,MESSAGE,WPARAM,LPARAM))(m_oldClientProc)(hWnd,message,wParam,lParam);
	}

	return DefWindowProc(hWnd,message,wParam,lParam);
}

void CMyClientWindow::ToTop(HWND hwnd)
{
	SetFocus(hwnd);
}

