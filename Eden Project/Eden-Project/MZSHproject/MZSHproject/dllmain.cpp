// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include<fstream>
#include<iostream>
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;

COLORREF rgb;
HWND ghWnd;
HBITMAP hBitmap;
int bitheight;
int bitwidth;
int CALLBACK TimerProc()
{
	static int wndAlp = 0;
	static int flag = 0;

	if (flag)
	{
		if (flag == 1)
		{
			Sleep(5000);
			flag = 2;
		}
		wndAlp -= 3;
		if (wndAlp == 0)
			DestroyWindow(ghWnd);
		SetLayeredWindowAttributes(ghWnd, -1, wndAlp, LWA_ALPHA);
	}
	else
	{
		wndAlp += 5;
		if (wndAlp == 255)
			flag = 1;
		SetLayeredWindowAttributes(ghWnd, -1, wndAlp, LWA_ALPHA);
	}
	return 0;
}
void DrawBmp(HDC hDC, HBITMAP bitmap, int nWidth, int nHeight)
{
	BITMAP			bm;
	HDC hdcImage;
	HDC hdcMEM;
	hdcMEM = CreateCompatibleDC(hDC);
	hdcImage = CreateCompatibleDC(hDC);
	HBITMAP bmp = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	GetObject(bitmap, sizeof(bm), (LPSTR)&bm);
	SelectObject(hdcMEM, bmp);
	SelectObject(hdcImage, bitmap);
	StretchBlt(hdcMEM, 0, 0, nWidth, nHeight, hdcImage, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	StretchBlt(hDC, 0, 0, nWidth, nHeight, hdcMEM, 0, 0, nWidth, nHeight, SRCCOPY);

	DeleteObject(hdcImage);
	DeleteDC(hdcImage);
	DeleteDC(hdcMEM);
}
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	static HDC compDC = 0;
	static RECT rect;
	if (uMsg == WM_CREATE)
	{
		ghWnd = hwnd;
		SetLayeredWindowAttributes(hwnd, -1, 0, LWA_ALPHA);
		SetTimer(hwnd, 5003, 1, (TIMERPROC)TimerProc);

		int scrWidth, scrHeight;

		scrWidth = GetSystemMetrics(SM_CXSCREEN);
		scrHeight = GetSystemMetrics(SM_CYSCREEN);
		GetWindowRect(hwnd, &rect);
		rect.left = (scrWidth - rect.right) / 2;
		rect.top = (scrHeight - rect.bottom) / 2;
		SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

		DrawBmp(GetDC(hwnd), hBitmap, 1280, 720);
	}
	if (uMsg == WM_PAINT)
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);

	}
	if (uMsg == WM_CLOSE)
	{
		DestroyWindow(hwnd);
	}
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
HINSTANCE hInst;

bool __stdcall LogoShow()
{
	if (!fopen("About.aym", "rb"))
	{
		MessageBox(0, L"Miss About.aym.", 0, 0);
		return false;
	}
	hBitmap = (HBITMAP)LoadImageA(NULL, "About.aym", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInst;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = "LYC";
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.cbWndExtra = DLGWINDOWEXTRA;
	rgb = 0xFFFFFFFF;
	RegisterClassExA(&wcex);
	HWND hWnd = CreateWindowExA(WS_EX_LAYERED | WS_EX_TOPMOST, "LYC", "LYC", WS_POPUP | WS_SYSMENU | WS_SIZEBOX, 0, 0, 1600, 900, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

