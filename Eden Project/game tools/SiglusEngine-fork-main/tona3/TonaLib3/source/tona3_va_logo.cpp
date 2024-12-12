#include	"tona3_pch.h"
#include	"tona3_file.h"
#include	"tona3_filesys.h"
#include	"tona3_va_logo.h"
#include	"tona3_app.h"

namespace NT3
{

static HWND Gvalog_hwnd = NULL;

static HBITMAP Gvalog_bmphd = NULL;
static int Gvalog_bmp_xl;
static int Gvalog_bmp_yl;

static HDC Gvalogo_memdc = NULL;

static int Gvalogo_start_time;

typedef DWORD (WINAPI *FWINLAYER)(HWND hwin, DWORD key, BYTE alpha, DWORD flag);

static HINSTANCE Gvalogo_user32_dll = NULL;
static FWINLAYER Gvalogo_layered_func = NULL;

static int Gvalog_mode = 0;
static DWORD Gvalog_disp_time;
static DWORD Gvalog_erase_time;
static DWORD Gvalog_total_time;

#define		LARGE_VALOGO_DISP_TIME		1500
#define		LARGE_VALOGO_ERASE_TIME		1000
#define		LARGE_VALOGO_SLEEP_TIME		0
#define		LARGE_VALOGO_TOTAL_TIME		(LARGE_VALOGO_DISP_TIME + LARGE_VALOGO_ERASE_TIME)

#define		SMALL_VALOGO_DISP_TIME		1000
#define		SMALL_VALOGO_ERASE_TIME		500
#define		SMALL_VALOGO_TOTAL_TIME		(SMALL_VALOGO_DISP_TIME + SMALL_VALOGO_ERASE_TIME)

LRESULT CALLBACK valog_window_funk(HWND hwin, UINT message, WPARAM wParam, LPARAM lParam){

	switch(message){

		case WM_CREATE:
			Gvalog_hwnd = hwin;

			// ビットマップをメモリデバイスコンテキストに関連付ける
			if (Gvalog_bmphd)	{
				HDC hdc = GetDC(hwin);
				Gvalogo_memdc = CreateCompatibleDC(hdc);
				SelectObject(Gvalogo_memdc, Gvalog_bmphd);
				ReleaseDC(hwin, hdc);
			}

			// タイマー開始
			SetTimer(hwin, 999, 20, NULL);		// 識別子=999 タイマー間隔=20 タイマー関数=なし(WM_TIMER発生)

			// 半透明を２５５にする
			if (Gvalogo_user32_dll && Gvalogo_layered_func)	{
				Gvalogo_layered_func(hwin, 0, 255, 2);
			}

			// タイムスタート
			Gvalogo_start_time = GetTickCount();

			SetWindowPos(hwin, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
			UpdateWindow(hwin);
			break;

		case WM_PAINT:
			{
				PAINTSTRUCT	ps;
				HDC hdc = BeginPaint(hwin, &ps);
				BitBlt(hdc, 0, 0, Gvalog_bmp_xl, Gvalog_bmp_yl, Gvalogo_memdc, 0, 0, SRCCOPY);
				EndPaint(hwin, &ps);
			}
			break;

		case WM_TIMER:

			{
				DWORD keika = (DWORD)(GetTickCount() - Gvalogo_start_time);
				if (Gvalogo_user32_dll && Gvalogo_layered_func)	{
					if (keika >= Gvalog_disp_time)	{
						DWORD time = keika - Gvalog_disp_time;
						int alpha = (time < Gvalog_erase_time) ? (255 - (255 * time / Gvalog_erase_time)) : 0;
						Gvalogo_layered_func(hwin, 0, alpha, 2);
					}
				}
				if (keika >= Gvalog_total_time)	{
					valogo_destroy();
				}
			}

			break;

		case WM_DESTROY:
			DestroyWindow(hwin);
			break;

		default:
			return(DefWindowProc(hwin, message, wParam, lParam));
	}
	return(0);
}

void valogo_create(CTSTR& check_filepath, int resource_id_logo_large, int resource_id_logo_small)
{
	WNDCLASSEX wcl;					// ウィンドウクラスの定義
	HWND hwin;

	// ファイルチェック
	Gvalog_mode = file_get_type(check_filepath) == 1 ? 1 : 0;

	// リソースからビットマップを読み込み
	if (Gvalog_mode)	{
		Gvalog_bmphd = LoadBitmap(G_app.h_inst, (LPCTSTR)(PTR)resource_id_logo_small);
		Gvalog_disp_time = SMALL_VALOGO_DISP_TIME;
		Gvalog_erase_time = SMALL_VALOGO_ERASE_TIME;
		Gvalog_total_time = SMALL_VALOGO_TOTAL_TIME;
	}
	else	{
		Gvalog_bmphd = LoadBitmap(G_app.h_inst, (LPCTSTR)(PTR)resource_id_logo_large);
		Gvalog_disp_time = LARGE_VALOGO_DISP_TIME;
		Gvalog_erase_time = LARGE_VALOGO_ERASE_TIME;
		Gvalog_total_time = LARGE_VALOGO_TOTAL_TIME;
	}
	if (Gvalog_bmphd == NULL)	{	return;	}
	BITMAP bmp;
	GetObject(Gvalog_bmphd, sizeof(BITMAP), &bmp);
	Gvalog_bmp_xl = bmp.bmWidth;
	Gvalog_bmp_yl = bmp.bmHeight;

	// メインフレームウィンドクラスの定義
	wcl.style         = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc   = valog_window_funk;
	wcl.cbClsExtra    = 0;
	wcl.cbWndExtra    = sizeof(LONG);
	wcl.hInstance     = G_app.h_inst;
	wcl.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground = (HBRUSH)0;
//	wcl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcl.lpszMenuName  = NULL;
	wcl.lpszClassName = _T("VisualArtsLogo");
	wcl.cbSize        = sizeof(WNDCLASSEX);
	if (!RegisterClassEx(&wcl))
		return;

	// 半透明化用の関数を取得
#if 1
	Gvalogo_user32_dll = LoadLibrary(_T("user32.dll"));
	if (Gvalogo_user32_dll)	{
		Gvalogo_layered_func = (FWINLAYER)GetProcAddress(Gvalogo_user32_dll, "SetLayeredWindowAttributes");
	}
#endif

	DWORD layered_style = (Gvalogo_user32_dll && Gvalogo_layered_func) ? WS_EX_LAYERED : 0;

	// ウィンドウの作成
	int x, y;
#if 1
	if (Gvalog_mode)	{
		RECT rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
		x = rect.right  - Gvalog_bmp_xl - 8;
		y = rect.bottom - Gvalog_bmp_yl - 8;
	}
	else	{
		x = (GetSystemMetrics(SM_CXSCREEN) - Gvalog_bmp_xl) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - Gvalog_bmp_yl) / 2;
	}
#else
	x = (GetSystemMetrics(SM_CXSCREEN) - Gvalog_bmp_xl) / 2;
	y = (GetSystemMetrics(SM_CYSCREEN) - Gvalog_bmp_yl) / 2;
#endif

	hwin = CreateWindowEx(
		WS_EX_TOOLWINDOW | layered_style,
		wcl.lpszClassName, _T("VisualArtsLogo"),
		WS_POPUP,
		x, y, Gvalog_bmp_xl, Gvalog_bmp_yl,
		HWND_DESKTOP, NULL, G_app.h_inst, NULL
	);

	// 大きいロゴを表示した場合はスリープ
//	if (Gvalog_mode == 0)	{
//		Sleep(LARGE_VALOGO_SLEEP_TIME);
//	}
}

void valogo_destroy()
{
	if (Gvalog_hwnd)	{
		SendMessage(Gvalog_hwnd, WM_DESTROY, 0, 0);
		Gvalog_hwnd = NULL;
	}
	if (Gvalogo_memdc)	{
		DeleteDC(Gvalogo_memdc);
		Gvalogo_memdc = NULL;
	}
	if (Gvalog_bmphd)	{
		DeleteObject(Gvalog_bmphd);
		Gvalog_bmphd = NULL;
	}
	if (Gvalogo_user32_dll)	{
		FreeLibrary(Gvalogo_user32_dll);
	}
}

HWND valogo_get_handle()
{
	return Gvalog_hwnd;
}

}

