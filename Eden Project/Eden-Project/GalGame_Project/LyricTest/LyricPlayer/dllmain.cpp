// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include <strsafe.h>
#include <windowsx.h>
#include <synchapi.h>

static HHOOK hhk = NULL;
WINDOWPOS* ptrWndPos = nullptr;
ULONG_PTR gdiplusStartupToken;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
wchar_t* szClassName = (wchar_t*)L"YUIDialog";
HINSTANCE hThisInstance;

HWND hw;
int x, y, cx, cy = 0;

LRESULT CALLBACK TestProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        auto msg = (tagCWPSTRUCT*)lParam;
        switch (msg->message)
        {
        case WM_WINDOWPOSCHANGED:
            ptrWndPos = (WINDOWPOS*)msg->lParam;
            cout << ptrWndPos->x << "|" << ptrWndPos->y << endl;
			x = ptrWndPos->x;
			y = ptrWndPos->y;
			cx = ptrWndPos->cx;
			cy = ptrWndPos->cy;
            break;
        default:
            break;
        }
    }
    return CallNextHookEx(hhk, code, wParam, lParam);
}

void MyCreateWindow()
{
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&gdiplusStartupToken, &gdiInput, NULL);
	WNDCLASSEX wincl;        /* Data structure for the windowclass */

							 /* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;//+-69+ 
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
											   /* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return;
	/* The class is registered, let's create the program*/
	HWND hwnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		L"Title",       /* Title Text */
		WS_POPUP, /* default window */
		0,       /* Windows decides the position */
		0,       /* where the window ends up on the screen */
		1280,                 /* The programs width */
		720,                 /* and height in pixels */
		NULL,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);

	ShowWindow(hwnd, SW_SHOW);

	MSG messages;
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	Gdiplus::GdiplusShutdown(gdiplusStartupToken);
	return;
}

HDC g_memDC = NULL;
HBITMAP g_memBitmap = NULL;
HWND ghWnd;
void copyToCurDC(HWND hwnd, wchar_t* file)
{
	HDC dc = GetDC(hwnd);
	RECT wndRect;
	::GetWindowRect(hwnd, &wndRect);
	SIZE wndSize = { wndRect.right - wndRect.left,wndRect.bottom - wndRect.top };

	Gdiplus::Graphics graphics(g_memDC);
	Gdiplus::SolidBrush bru(Gdiplus::Color(0, 0, 0, 0));
	Gdiplus::Rect re(0, 0, wndSize.cx, wndSize.cy);
	graphics.FillRectangle(&bru, re);

	graphics.DrawImage(&Gdiplus::Image(file), 0, 0, wndSize.cx, wndSize.cy);

	//g_memDC;
	POINT ptDest = { wndRect.left,wndRect.top };
	BLENDFUNCTION blend = { AC_SRC_OVER, 0, (BYTE)(255.0), AC_SRC_ALPHA };
	POINT a = { 0,0 };
	UpdateLayeredWindow(hwnd, dc, &ptDest, &wndSize, g_memDC, &a, 0, &blend, ULW_ALPHA);
	::ReleaseDC(hwnd, dc);
}


int CALLBACK TimerProc()
{
	for (int i = 1; i <= 2545; i++)
	{
		wchar_t FileName[MAX_PATH];
		wsprintfW(FileName, L"pic\\%08d.png", i);
		std::cout << wtoc(FileName) << std::endl;
		copyToCurDC(ghWnd, FileName);
		//Sleep(17);
	}
	DestroyWindow(ghWnd);
	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  /* handle the messages */
	{
	case WM_CREATE:
	{
		//::SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	}
	break;
	case WM_SIZE:
	{
		::DeleteDC(g_memDC);
		::DeleteObject(g_memBitmap);

		int nW = LOWORD(lParam);
		int nH = HIWORD(lParam);

		HDC dc = GetDC(hwnd);
		g_memDC = ::CreateCompatibleDC(dc);

		BITMAPINFO bitmapinfo;
		bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfo.bmiHeader.biBitCount = 32;
		bitmapinfo.bmiHeader.biHeight = nH;
		bitmapinfo.bmiHeader.biWidth = nW;
		bitmapinfo.bmiHeader.biPlanes = 1;
		bitmapinfo.bmiHeader.biCompression = BI_RGB;
		bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
		bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
		bitmapinfo.bmiHeader.biClrUsed = 0;
		bitmapinfo.bmiHeader.biClrImportant = 0;
		bitmapinfo.bmiHeader.biSizeImage =
			bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight *
			bitmapinfo.bmiHeader.biBitCount / 8;
		g_memBitmap = ::CreateDIBSection(g_memDC, &bitmapinfo, 0, NULL, 0, 0);
		SelectObject(g_memDC, g_memBitmap);

		ReleaseDC(hwnd, dc);
		ghWnd = hwnd;
		
		static RECT rect;
		GetWindowRect(hw, &rect);// 目标窗口的hwnd
		rect.left = rect.right / 2;
		rect.top = rect.bottom / 2;
		SetWindowPos(hwnd, HWND_TOP, x, y, cx, cy, SWP_SHOWWINDOW);//播放窗口的hwnd

		SetTimer(hwnd, 84 * 1000, 1, (TIMERPROC)TimerProc);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	case WM_LBUTTONDOWN:
		::SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
	DWORD dwExStyle,
	LPCTSTR lpClassName,
	LPCTSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam);
HWND WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	const char* szWndName = "琥珀结晶 - v1.2";

	hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	return hw;
}

wchar_t szTitle[] = L"提示";
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	MyCreateWindow();
	return MessageBoxAOLD(hWnd, lpText, lpCaption, uType);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        make_console();
        hhk = SetWindowsHookEx(WH_CALLWNDPROC, TestProc, hModule, GetCurrentThreadId());
        cout << "Hook set" << endl;

		DetourTransactionBegin();
		DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
		MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
		DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
		DetourTransactionCommit();

    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        UnhookWindowsHookEx(hhk);
    }
    return TRUE;
}

