#include <windows.h> 
#include <iostream>
#include <gdiplus.h> 
#include <ctime>
#pragma  comment(lib,"Gdiplus.lib")
using namespace std;
using namespace Gdiplus;
ULONG_PTR gdiplusStartupToken;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

wchar_t* szClassName = (wchar_t*)L"YUIDialog";
HINSTANCE hThisInstance;
int main()
{
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&gdiplusStartupToken, &gdiInput, NULL);
	WNDCLASSEX wincl;       
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;     
	wincl.style = CS_DBLCLKS;                
	wincl.cbSize = sizeof(WNDCLASSEX);


	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 
	wincl.cbClsExtra = 0;                      
	wincl.cbWndExtra = 0;                      
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	
	if (!RegisterClassEx(&wincl))
		return 0;
	
	HWND hwnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST,                   
		szClassName,         
		L"Title",       
		WS_POPUP, 
		0,       
		0,       
		1280,                 
		720,                 
		NULL,        
		NULL,                
		hThisInstance,       
		NULL                 
	);

	ShowWindow(hwnd, SW_SHOW);

	MSG messages;
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	Gdiplus::GdiplusShutdown(gdiplusStartupToken);
	return messages.wParam;
}


HDC g_memDC = NULL;
HBITMAP g_memBitmap = NULL;
HWND ghWnd;
void copyToCurDC(HWND hwnd,wchar_t* file)
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

static char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE);
	char* bbb = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
	return bbb;
}
/*
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
*/
void OnPaint(HWND hwnd)
{
	//CDialog::OnPaint();
	HDC dc = GetDC(hwnd);
	RECT wndRect;
	::GetWindowRect(hwnd, &wndRect);
	SIZE wndSize = { wndRect.right - wndRect.left,wndRect.bottom - wndRect.top };

	Gdiplus::Graphics graphics(g_memDC);
	Gdiplus::SolidBrush bru(Gdiplus::Color(0, 0, 0, 0));
	Gdiplus::Rect re(0, 0, wndSize.cx, wndSize.cy);
	graphics.FillRectangle(&bru, re);

	FontFamily fontFamily(L"Arial");
	StringFormat strformat;
	wchar_t pszbuf[] = L"Text Designer";

	GraphicsPath path;
	path.AddString(pszbuf, wcslen(pszbuf), &fontFamily,
		FontStyleRegular, 48, Gdiplus::Point(10, 10), &strformat);

	for (int i = 1; i < 8; ++i)
	{
		Pen pen(Color(32, 0, 128, 192), i);
		pen.SetLineJoin(LineJoinRound);
		graphics.DrawPath(&pen, &path);
	}

	SolidBrush brush(Color(255, 255, 255));
	graphics.FillPath(&brush, &path);

	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;

	POINT pt = { 0,0 };
	SIZE sz = { 600,600 };
	UpdateLayeredWindow(hwnd, dc, NULL, &sz, g_memDC, &pt, 0, &bf, ULW_ALPHA);
}

HDC g_hDC = NULL;
HDC g_hDIBDC = NULL;
void* g_pBits;

void InitDIB(HWND hWnd)
{
	g_hDC = GetDC(hWnd);

	// 取得窗口用户区大小
	RECT rc;
	GetClientRect(hWnd, &rc);

	// 内存DC，即画架
	g_hDIBDC = CreateCompatibleDC(g_hDC);

	BITMAPINFOHEADER bi = { 0 };
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = rc.right;
	bi.biHeight = rc.bottom;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;

	// 内存DIB，即画纸
	HBITMAP hDIB = CreateDIBSection(g_hDIBDC, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &g_pBits, NULL, 0);

	SelectObject(g_hDIBDC, hDIB); // 把画纸装进画架
}

void Paint(HWND hwnd, wchar_t* text, int deep)
{
	Gdiplus::Status status;

	auto pszText = text;

	Graphics g(g_hDIBDC);
	FontFamily fm(L"黑体");
	Point ptDraw{ 20, 20 };

	// 调整绘图质量
	g.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.SetCompositingQuality(CompositingQualityHighQuality);


	//
	// 绘图
	//

	GraphicsPath path;
	status = path.AddString(pszText, wcslen(pszText), &fm, FontStyleRegular, 36, ptDraw, Gdiplus::StringFormat::GenericDefault());
	if (status == Ok)
	{
		Color clrOutline(32, 0, 128, 255);

		// 描绘路径
		for (int i = 1; i <= deep; ++i)
		{
			Pen pen(clrOutline, i);
			pen.SetLineJoin(LineJoinRound);
			status = g.DrawPath(&pen, &path);
		}

		// 填充路径
		SolidBrush brush(Color(255, 255, 255, 255));
		status = g.FillPath(&brush, &path);
	}

	//
	// 内存DC到窗口DC
	//

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;
	bf.AlphaFormat = AC_SRC_ALPHA;

	POINT pt = { 0, 0 };
	SIZE size = { 600, 600 };

	BOOL result = UpdateLayeredWindow(hwnd, g_hDC, NULL, &size, g_hDIBDC, &pt, 0, &bf, ULW_ALPHA);
}

HWND g_hwnd;
wchar_t* text = nullptr;
int CALLBACK TimerProc()
{
	Paint(g_hwnd, text, 8);
	return 0;
}


void delay_sec(int sec)//
{
	time_t start_time, cur_time;
	time(&start_time);
	do
	{
		time(&cur_time);
	} while ((cur_time - start_time) < sec);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  /* handle the messages */
	{
	case WM_CREATE:
	{
		InitDIB(hwnd);
		Paint(hwnd, (wchar_t*)L"Test",8);
		Paint(hwnd, (wchar_t*)L"Test2", 8);
		//DestroyWindow(hwnd);


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