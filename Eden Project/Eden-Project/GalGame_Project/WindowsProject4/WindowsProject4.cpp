// WindowsProject4.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject4.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT4));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;// MAKEINTRESOURCEW(IDC_WINDOWSPROJECT4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, WS_POPUP | WS_VISIBLE,
       CW_USEDEFAULT, CW_USEDEFAULT, 600, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_SHOWNORMAL);
   //UpdateWindow(hWnd); // 已经没用了

   return TRUE;
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
    assert(g_hDIBDC);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = rc.right;
    bi.biHeight = rc.bottom;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    // 内存DIB，即画纸
    HBITMAP hDIB = CreateDIBSection(g_hDIBDC, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &g_pBits, NULL, 0);
    assert(hDIB);

    SelectObject(g_hDIBDC, hDIB); // 把画纸装进画架
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: // 这个消息会在窗口创建成功后收到一次，实际上是在CreateWindow()里Call过来的。
    {
        //
        // 创建内存DC
        //

        InitDIB(hWnd);

        assert(g_hDC);
        assert(g_hDIBDC);

        //SetTimer(hWnd, 1001, 100, NULL);


        Gdiplus::Status status;

        auto pszText = L"倒是把文本给draw出来了";

        Gdiplus::Graphics g(g_hDIBDC);
        Gdiplus::FontFamily fm(L"黑体");
        Gdiplus::Point ptDraw{ 20, 20 };

        // 调整绘图质量
        g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
        g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        g.SetCompositingQuality(Gdiplus::CompositingQuality::CompositingQualityHighQuality);

        //Gdiplus::SolidBrush br(Gdiplus::Color(200, 200, 200, 200));
        //g.FillRectangle(&br, 0, 0, 600, 600);

        //
        // 绘图
        //

        Gdiplus::GraphicsPath path;
        status = path.AddString(pszText, wcslen(pszText), &fm, Gdiplus::FontStyleBold, 36, ptDraw, Gdiplus::StringFormat::GenericDefault());
        if (status == Gdiplus::Ok)
        {
            Gdiplus::Color clrOutline(60, 0, 128, 255);

            // 描绘路径
            for (int i = 1; i <= 10; ++i)
            {
                Gdiplus::Pen pen(clrOutline, i);
                pen.SetLineJoin(Gdiplus::LineJoinRound);
                status = g.DrawPath(&pen, &path);
            }

            // 填充路径
            Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
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

        BOOL result = UpdateLayeredWindow(hWnd, g_hDC, NULL, &size, g_hDIBDC, &pt, 0, &bf, ULW_ALPHA);

        break;
    }
    case WM_TIMER:
    {
        

        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            /*PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            auto pszText = L"倒是把文本给draw出来了";

            Gdiplus::Graphics g(hdc);
            Gdiplus::FontFamily fm(L"黑体");
            Gdiplus::Point ptDraw{ 20, 20 };

            g.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
            g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
            g.SetCompositingQuality(Gdiplus::CompositingQuality::CompositingQualityHighQuality);

            Gdiplus::GraphicsPath path;
            Gdiplus::Status status = path.AddString(pszText, wcslen(pszText), &fm, Gdiplus::FontStyleBold, 36, ptDraw, Gdiplus::StringFormat::GenericDefault());
            if (status == Gdiplus::Ok)
            {
                Gdiplus::Color clrOutline(60, 0, 128, 255);

                for (int i = 1; i <= 10; ++i)
                {
                    Gdiplus::Pen pen(clrOutline, i);
                    pen.SetLineJoin(Gdiplus::LineJoinRound);
                    g.DrawPath(&pen, &path);
                }

                Gdiplus::SolidBrush brush(Gdiplus::Color(255,255,255,255));
                g.FillPath(&brush, &path);
            }

            EndPaint(hWnd, &ps);*/
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
