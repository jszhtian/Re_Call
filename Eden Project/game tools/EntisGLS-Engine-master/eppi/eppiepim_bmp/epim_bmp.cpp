
/*****************************************************************************
                             EPPI プラグイン
              BMP (Window Bitmap) format 画像インポーター
 -----------------------------------------------------------------------------
          Copyright (C) 2002 Leshade Entis. All rights reserved.
 *****************************************************************************/


#include "epim_bmp.h"


//////////////////////////////////////////////////////////////////////////////
// グローバル変数
//////////////////////////////////////////////////////////////////////////////

HINSTANCE	g_hInst = NULL ;

extern	"C" __declspec(dllexport)
		EPPI_DESCRIPTOR	_eppi_descriptor =
{
	EPPI_VERSION,				// バージョン
	EPPI_NORMAL_PRIORITY,		// 優先度
	EPPI_IMAGE_IMPORTER,		// プラグインの種別
	&EPPI_BMP_Initialize,		// 初期化関数
	&EPPI_BMP_Destroy,			// 終了関数
	&EPPI_BMP_Information,		// プラグイン情報関数
	(EPPI_COMMON_INTERFACE*)
	&g_imgimp					// プラグイン・インターフェース
} ;

EPPI_IMAGE_IMPORT	g_imgimp =
{
	"BMPインポーター",					// プラグイン名
	"Window Bitmap (*.bmp)",			// フィルタ名
	"bmp",								// 対応拡張子
	0,									// 対応機能
	0,									// 予約＝０
	&BMP_Create,						// インスタンス作成
	&BMP_Destroy,						// インスタンス破棄
	&BMP_Open,							// ファイルを開く
	&BMP_Close,							// ファイルを閉じる
	&BMP_GetInformation,				// 画像情報取得
	&BMP_SetInformation,				// 画像情報書き出し（対応している場合のみ）
	&BMP_HasPreviewImage,				// プレビュー画像を持っているか？
	&BMP_GetPreview,					// プレビュー画像取得
	&BMP_GetImage						// 画像取得
} ;


//////////////////////////////////////////////////////////////////////////////
// グローバル関数
//////////////////////////////////////////////////////////////////////////////

// DLL エントリポイント
//////////////////////////////////////////////////////////////////////////////
BOOL WINAPI main
	( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
	case	DLL_PROCESS_ATTACH:
		g_hInst = hinstDLL ;
		return	TRUE ;

	case	DLL_PROCESS_DETACH:
		break ;

	case	DLL_THREAD_ATTACH:
		break ;

	case	DLL_THREAD_DETACH:
		break ;
	}

	return	FALSE ;
}

void * operator new ( size_t stBytes )
{
	return	::GlobalAlloc( GMEM_FIXED, stBytes ) ;
}

void operator delete ( void * ptrObj )
{
	::GlobalFree( ptrObj ) ;
}


//////////////////////////////////////////////////////////////////////////////
// プラグイン・インターフェース
//////////////////////////////////////////////////////////////////////////////

//
// プラグイン初期化関数
//////////////////////////////////////////////////////////////////////////////
EPPI_INSTANCE __stdcall EPPI_BMP_Initialize( EPPI_GLOBAL_FUNC global )
{
	MY_EPPI_INSTANCE *	pInst = new MY_EPPI_INSTANCE ;
	pInst->global = global ;
	return	pInst ;
}

//
// プラグイン終了関数
//////////////////////////////////////////////////////////////////////////////
void __stdcall EPPI_BMP_Destroy( EPPI_INSTANCE instance )
{
	delete	(MY_EPPI_INSTANCE*) instance ;
}

//
// プラグイン情報表示
//////////////////////////////////////////////////////////////////////////////
static BOOL CALLBACK MyPluginInfoDlg
	( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ) ;

void __stdcall EPPI_BMP_Information( EPPI_INSTANCE instance, void * hParentWnd )
{
	MY_EPPI_INSTANCE *	pInst = (MY_EPPI_INSTANCE*) instance ;
	HWND	hMainWnd = (HWND) hParentWnd ;
	::DialogBox
		( g_hInst,
			MAKEINTRESOURCE(IDD_INFO_DLG),
			hMainWnd, &MyPluginInfoDlg ) ;
}

static BOOL CALLBACK MyPluginInfoDlg
	( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case	WM_INITDIALOG:
		{
			RECT	rcClient, rcWindow ;
			HWND	hwndParent = ::GetParent( hDlg ) ;
			::GetClientRect( hwndParent, &rcClient ) ;
			::GetWindowRect( hDlg, &rcWindow ) ;
			SIZE	sizeWindow ;
			sizeWindow.cx = rcWindow.right - rcWindow.left ;
			sizeWindow.cy = rcWindow.bottom - rcWindow.top ;
			POINT	posWindow ;
			posWindow.x = (rcClient.right - sizeWindow.cx) >> 1 ;
			posWindow.y = (rcClient.bottom - sizeWindow.cy) >> 1 ;
			::ClientToScreen( hwndParent, &posWindow ) ;
			posWindow.x = max( posWindow.x, 0 ) ;
			posWindow.y = max( posWindow.y, 0 ) ;
			//
			::SetWindowPos
				(
					hDlg, NULL,
					posWindow.x, posWindow.y,
					0, 0,
					(SWP_NOSIZE | SWP_NOZORDER)
				) ;
		}
		return	FALSE ;

	case	WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case	IDOK:
		case	IDCANCEL:
			EndDialog( hDlg, LOWORD(wParam) ) ;
			return	TRUE ;
		}
		break ;
	}

	return	FALSE ;
}

//
// 画像インスタンス作成
//////////////////////////////////////////////////////////////////////////////
void * __stdcall BMP_Create( EPPI_INSTANCE instance )
{
	MY_BMP_INSTANCE *	pInst = new MY_BMP_INSTANCE ;
	::memset( pInst, 0, sizeof(MY_BMP_INSTANCE) ) ;
	return	pInst ;
}

//
// 画像インスタンス破棄
//////////////////////////////////////////////////////////////////////////////
void __stdcall BMP_Destroy( void * instance )
{
	delete	(MY_BMP_INSTANCE*) instance ;
}

//
// 画像ファイルを開く
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Open
	( void * instance, const EPPI_FILE_INTERFACE * pfile )
{
	MY_BMP_INSTANCE *	pInst = (MY_BMP_INSTANCE*) instance ;
	pInst->pfile = pfile ;
	//
	// ファイルヘッダを読み込む
	//
	DWORD	dwReadBytes ;
	dwReadBytes = pfile->Read( &(pInst->bmfh), sizeof(BITMAPFILEHEADER) ) ;
	if ( dwReadBytes < sizeof(BITMAPFILEHEADER) )
	{
		return	"ファイルヘッダを読み込めませんでした。" ;
	}
	if ( pInst->bmfh.bfType != *((WORD*)"BM") )
	{
		return	"不正な BMP ファイルです。" ;
	}
	//
	// BMP 情報ヘッダを読み込む
	//
	dwReadBytes = pfile->Read( &(pInst->bmih), sizeof(BITMAPINFOHEADER) ) ;
	if ( dwReadBytes < sizeof(BITMAPINFOHEADER) )
	{
		return	"BMP 情報ヘッダを読み込めませんでした。" ;
	}
	if ( pInst->bmih.biSize != sizeof(BITMAPINFOHEADER) )
	{
		return	"未対応の BMP ファイルです。" ;
	}
	//
	// パレット数を設定する
	//
	if ( pInst->bmih.biBitCount <= 8 )
	{
		if ( pInst->bmih.biClrUsed == 0 )
		{
			pInst->bmih.biClrUsed = (1 << pInst->bmih.biBitCount) ;
		}
		else if ( pInst->bmih.biClrUsed > 0x100 )
		{
			pInst->bmih.biClrUsed = 0x100 ;
		}
	}
	else
	{
		pInst->bmih.biClrUsed = 0 ;
	}
	//
	// パレット情報を読み込む
	//
	if ( pInst->bmih.biClrUsed != 0 )
	{
		DWORD	dwPalBytes = sizeof(RGBQUAD) * pInst->bmih.biClrUsed ;
		dwReadBytes = pfile->Read( pInst->rgb, dwPalBytes ) ;
		if ( dwReadBytes < dwPalBytes )
		{
			return	"パレットテーブルを読み込めませんでした。" ;
		}
	}
	//
	// 画像情報を設定する
	//
	pInst->info.fdwFormat =
		(pInst->bmih.biBitCount <= 8) ? EPPI_INDEXED_IMAGE : EPPI_RGB_IMAGE ;
	pInst->info.dwWidth = pInst->bmih.biWidth ;
	pInst->info.dwHeight = pInst->bmih.biHeight ;
	pInst->info.dwBitsPerPixel = pInst->bmih.biBitCount ;
	pInst->info.dwClipperPalette = 0 ;
	pInst->info.pszCopyright = NULL ;
	pInst->info.pszDescription = NULL ;
	//
	return	NULL ;
}

//
// ファイルを閉じる
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Close( void * instance )
{
	return	NULL ;
}

//
// 画像情報を取得する
//////////////////////////////////////////////////////////////////////////////
const EPPI_IMAGE_INFO *
	__stdcall BMP_GetInformation( void * instance )
{
	return	& (((MY_BMP_INSTANCE*) instance)->info) ;
}

//
// 画像情報を設定する
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetInformation
	( void * instance, const EPPI_IMAGE_INFO * pInfomation )
{
	return	"BMP ファイルには付加的な画像情報を書き出せません。" ;
}

//
// プレビュー画像があるか？
//////////////////////////////////////////////////////////////////////////////
const EPPI_IMAGE_INFO *
	 __stdcall BMP_HasPreviewImage( void * instance )
{
	return	NULL ;
}

//
// プレビュー画像を取得
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_GetPreview
	( void * instance, EPPI_IMAGE_BUFFER * pImageBuf )
{
	return	"BMP ファイルにはプレビュー画像は含まれていません。" ;
}

//
// 画像を取得
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_GetImage
	( void * instance,
		EPPI_IMAGE_BUFFER * pImageBuf,
		const EPPI_ADVANCE_STATE * pAdvState )
{
	//
	// バッファの検証
	//
	MY_BMP_INSTANCE *	pInst = (MY_BMP_INSTANCE*) instance ;
	if ( (pImageBuf->dwWidth != pInst->info.dwWidth)
		|| (pImageBuf->dwHeight != pInst->info.dwHeight)
		|| (pImageBuf->dwBitsPerPixel != pInst->info.dwBitsPerPixel) )
	{
		return	"バッファの情報が画像の情報と一致しません。" ;
	}
	//
	// パレットテーブル複製
	//
	if ( (pInst->bmih.biClrUsed > 0) && pImageBuf->ptrPaletteTable )
	{
		unsigned int	nPaletteCount = pInst->bmih.biClrUsed ;
		if ( nPaletteCount > pImageBuf->dwPaletteCount )
			nPaletteCount = pImageBuf->dwPaletteCount ;
		//
		::memmove
			( pImageBuf->ptrPaletteTable,
				pInst->rgb, sizeof(RGBQUAD) * nPaletteCount ) ;
	}
	//
	// 画像読み込み準備
	//
	DWORD	dwLineBytes =
		(pInst->bmih.biWidth * pInst->bmih.biBitCount + 0x07) >> 3 ;
	DWORD	dwBytesPerLine =
		((pInst->bmih.biWidth *
			pInst->bmih.biBitCount + 0x1F) & (~0x1F)) >> 3 ;
	pInst->pfile->Seek( pInst->bmfh.bfOffBits ) ;
	//
	// 画像読み込み
	//
	DWORD	dwLastAdv = -1 ;
	BYTE *	pbytLine = pImageBuf->ptrImageBuf ;
	pbytLine += pImageBuf->nBytesPerLine * (pInst->info.dwHeight - 1) ;
	DWORD	dwPos = pInst->bmfh.bfOffBits ;
	//
	for ( DWORD y = 0; y < pInst->info.dwHeight; y ++ )
	{
		//
		// 1ライン読み込む
		//
		DWORD	dwReadBytes ;
		dwReadBytes = pInst->pfile->Read( pbytLine, dwLineBytes ) ;
		if ( dwReadBytes < dwLineBytes )
		{
			return	"画像を読み込めませんでした。" ;
		}
		//
		// 次のラインへ移動
		//
		dwPos += dwBytesPerLine ;
		if ( dwLineBytes != dwBytesPerLine )
		{
			pInst->pfile->Seek( dwPos ) ;
		}
		pbytLine -= pImageBuf->nBytesPerLine ;
		//
		// 進行状況更新
		//
		DWORD	dwCurrent ;
		dwCurrent = y * 100 / pInst->info.dwHeight ;
		if ( pAdvState && (dwCurrent != dwLastAdv) )
		{
			pAdvState->Update( dwCurrent, 100 ) ;
		}
	}
	//
	return	NULL ;
}

