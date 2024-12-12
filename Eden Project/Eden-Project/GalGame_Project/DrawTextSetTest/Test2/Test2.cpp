#include <windows.h> 
#include <iostream>
#include <gdiplus.h> 
#pragma  comment(lib,"Gdiplus.lib")
using namespace Gdiplus;
using namespace std;
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
		WS_EX_LAYERED, 
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

void OnPaint(HDC hdc)
{
	//CDialog::OnPaint();
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

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
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  /* handle the messages */
	{
	case WM_CREATE:
	{
		HDC hdc = GetDC(hwnd);
		RECT rect;
		GetClientRect(hwnd, &rect);
		int m_nWidth = rect.left;
		int m_nHeight = rect.top;
		HDC DC = CreateCompatibleDC(hdc);
		
		BITMAPINFO bitmapinfo;
		bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfo.bmiHeader.biBitCount = 32;
		bitmapinfo.bmiHeader.biHeight = m_nHeight;
		bitmapinfo.bmiHeader.biWidth = m_nWidth;
		bitmapinfo.bmiHeader.biPlanes = 1;
		bitmapinfo.bmiHeader.biCompression = BI_RGB;
		bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
		bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
		bitmapinfo.bmiHeader.biClrUsed = 0;
		bitmapinfo.bmiHeader.biClrImportant = 0;
		bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
		HBITMAP hBitmap = CreateDIBSection(DC, &bitmapinfo, 0, NULL, 0, 0);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(DC, hBitmap);
		Graphics* pGraphics = new Graphics(DC);
		pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
		pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
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
			pGraphics->DrawPath(&pen, &path);
		}

		SolidBrush brush(Color(255, 255, 255));
		pGraphics->FillPath(&brush, &path);
		delete pGraphics;
		POINT pt = { 0,0 };
		SIZE sz = { m_nWidth ,m_nHeight };
		BLENDFUNCTION blendFunc32bpp;
		blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
		blendFunc32bpp.BlendFlags = 0;
		blendFunc32bpp.BlendOp = AC_SRC_OVER;
		blendFunc32bpp.SourceConstantAlpha = 255;
		HDC hDC = GetDC(hwnd);
		UpdateLayeredWindow(hwnd, hDC, NULL, &sz, DC, &pt, 0, &blendFunc32bpp, ULW_ALPHA);

		SelectObject(DC, hOldBitmap);
		DeleteObject(hBitmap);
		ReleaseDC(hwnd, hDC);

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