// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "drawtext.h"
#include "tools.h"
#include "crc32.h"
#include "detours.h"

#include <iostream>
#pragma comment(lib, "detours.lib")

HWND hw;
HDC hdc;

HDC g_hDC = NULL;
HDC g_hDIBDC = NULL;
void* g_pBits;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


static char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE);
	char* bbb = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
	return bbb;
}

wchar_t* Text = nullptr;
int ID;
int CALLBACK TimerProc()
{
    cout << wtoc(Text) << endl;
    Paint(hw, Text, 8);
    KillTimer(hw, ID);
    return 0;
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
    InitDIB(hw);
    Text = (wchar_t*)L"Hello world.";
    ID = 1;
    SetTimer(hw, 5000, ID, (TIMERPROC)TimerProc);

    return MessageBoxAOLD(hWnd, lpText, lpCaption, uType);
}

void HookInit()
{

    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
    MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
    DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"ERROR", L"Aym", MB_OK);
}

static void make_console()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    cout << "Debug mode\n" << endl;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        make_console();
        HookInit();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

