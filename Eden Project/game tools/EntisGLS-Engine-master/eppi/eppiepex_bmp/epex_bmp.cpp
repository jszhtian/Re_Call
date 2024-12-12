
/*****************************************************************************
                             EPPI プラグイン
              BMP (Windows Bitmap) format 画像エクスポーター
 -----------------------------------------------------------------------------
          Copyright (C) 2002 Leshade Entis. All rights reserved.
 *****************************************************************************/


#include "epex_bmp.h"


//////////////////////////////////////////////////////////////////////////////
// グローバル変数
//////////////////////////////////////////////////////////////////////////////

HINSTANCE	g_hInst = NULL ;

extern	"C" __declspec(dllexport)
		EPPI_DESCRIPTOR	_eppi_descriptor =
{
	EPPI_VERSION,				// バージョン
	EPPI_NORMAL_PRIORITY,		// 優先度
	EPPI_IMAGE_EXPORTER,		// プラグインの種別
	&EPPI_BMP_Initialize,		// 初期化関数
	&EPPI_BMP_Destroy,			// 終了関数
	&EPPI_BMP_Information,		// プラグイン情報関数
	(EPPI_COMMON_INTERFACE*)
	&g_imgexp					// プラグイン・インターフェース
} ;

EPPI_IMAGE_EXPORT	g_imgexp =
{
	"BMP エクスポーター",				// プラグイン名
	"Window Bitmap (*.bmp)",			// フィルタ名
	"bmp",								// 対応拡張子
	0,									// 対応機能
	0,									// 予約＝０
	&BMP_Create,						// インスタンス作成
	&BMP_Destroy,						// インスタンス破棄
	&BMP_IsSupported,					// 対応しているフォーマットか？
	&BMP_Option,						// オプション設定
	&BMP_Open,							// ファイルを開く
	&BMP_Close,							// ファイルを閉じる
	&BMP_SetInformation,				// 画像情報を設定
	&BMP_SetPreview,					// プレビュー画像を書き出す
	&BMP_SetImage						// 画像を書き出す
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

// メモリ確保
//////////////////////////////////////////////////////////////////////////////
void * operator new ( size_t stBytes )
{
	return	::GlobalAlloc( GMEM_FIXED, stBytes ) ;
}

void operator delete( void * ptrObj )
{
	::GlobalFree( ptrObj ) ;
}


//////////////////////////////////////////////////////////////////////////////
// プラグイン・インターフェース
//////////////////////////////////////////////////////////////////////////////

// プラグインの初期化
//////////////////////////////////////////////////////////////////////////////
EPPI_INSTANCE __stdcall EPPI_BMP_Initialize( EPPI_GLOBAL_FUNC global )
{
	MY_EPPI_INSTANCE *	pInst = new MY_EPPI_INSTANCE ;
	pInst->global = global ;
	return	pInst ;
}

// プラグインの終了
//////////////////////////////////////////////////////////////////////////////
void __stdcall EPPI_BMP_Destroy( EPPI_INSTANCE instance )
{
	delete	(MY_EPPI_INSTANCE*) instance ;
}

// プラグインの情報
//////////////////////////////////////////////////////////////////////////////
static BOOL CALLBACK MyInfoDlgProc
	( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ) ;
void __stdcall EPPI_BMP_Information( EPPI_INSTANCE instance, void * hParentWnd )
{
	MY_EPPI_INSTANCE *	pInst = (MY_EPPI_INSTANCE*) instance ;
	HWND	hMainWnd = (HWND) hParentWnd ;
	//
	::DialogBox
		( g_hInst,
			MAKEINTRESOURCE(IDD_INFO_DLG),
			hMainWnd, &MyInfoDlgProc ) ;
}

BOOL CALLBACK MyInfoDlgProc
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
		switch ( LOWORD( wParam ) )
		{
		case	IDOK:
		case	IDCANCEL:
			EndDialog( hDlg, LOWORD(wParam) ) ;
		}
		break ;
	}

	return	FALSE ;
}

// 書き出しインスタンスの生成
//////////////////////////////////////////////////////////////////////////////
void * __stdcall BMP_Create( EPPI_INSTANCE instance )
{
	MY_IMAGE_WRITE *	pInst = new MY_IMAGE_WRITE ;
	pInst->m_global = ((MY_EPPI_INSTANCE*)instance)->global ;
	pInst->m_pfile = NULL ;
	return	pInst ;
}

// 書き出しインスタンスの破棄
//////////////////////////////////////////////////////////////////////////////
void __stdcall BMP_Destroy( void * instance )
{
	delete	(MY_IMAGE_WRITE*) instance ;
}

// 対応しているフォーマットか？
//////////////////////////////////////////////////////////////////////////////
int __stdcall BMP_IsSupported
	( void * instance, const EPPI_IMAGE_INFO * pInformation )
{
	return	1 ;
}

// オプション設定
//////////////////////////////////////////////////////////////////////////////
int __stdcall BMP_Option( void * instance, void * hParentWnd, DWORD dwFlags )
{
	return	EPPI_OPTION_OK ;
}

// ファイルを開く
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Open
	( void * instance, const EPPI_FILE_INTERFACE * pfile )
{
	MY_IMAGE_WRITE *	pInst = (MY_IMAGE_WRITE*) instance ;
	pInst->m_pfile = pfile ;
	return	NULL ;
}

// ファイルを閉じる
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Close( void * instance )
{
	return	NULL ;
}

// 画像情報を設定する
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetInformation
	( void * instance,
		const EPPI_IMAGE_INFO * pInformation,
		const EPPI_IMAGE_INFO * pPreviewInfo )
{
	return	NULL ;
}

// プレビュー画像を書き出す
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetPreview
	( void * instance, const EPPI_IMAGE_BUFFER * pPreviewBuf )
{
	return	NULL ;
}

// 画像を書き出す
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetImage
	( void * instance,
		const EPPI_IMAGE_BUFFER * pImageBuf,
		const EPPI_ADVANCE_STATE * pAdvState )
{
	MY_IMAGE_WRITE *	pInst = (MY_IMAGE_WRITE*) instance ;
	pInst->m_pfile->Seek( 0 ) ;
	//
	// ビットマップ情報ヘッダを生成
	//
	BITMAPINFOHEADER	bmih ;
	::memset( &bmih, 0, sizeof(bmih) ) ;
	bmih.biSize = sizeof(bmih) ;
	bmih.biWidth = pImageBuf->dwWidth ;
	bmih.biHeight = pImageBuf->dwHeight ;
	bmih.biPlanes = 1 ;
	bmih.biBitCount = (WORD) pImageBuf->dwBitsPerPixel ;
	//
	int		nLineBytes =
		((pImageBuf->dwWidth * pImageBuf->dwBitsPerPixel + 0x1F) & ~0x1F) / 8 ;
	DWORD	dwImageBytes = nLineBytes * pImageBuf->dwHeight ;
	//
	// パレットテーブルを取得
	//
	unsigned int	nPalCount = 0 ;
	RGBQUAD			rgbTable[0x100] ;
	if ( pImageBuf->dwBitsPerPixel <= 8 )
	{
		nPalCount = (1 << pImageBuf->dwBitsPerPixel) ;
		if ( pImageBuf->fdwFormat == EPPI_INDEXED_IMAGE )
		{
			unsigned int	nCount = pImageBuf->dwPaletteCount ;
			if ( nCount > nPalCount )
			{
				nCount = nPalCount ;
			}
			::memmove
				( rgbTable, pImageBuf->ptrPaletteTable,
						nCount * sizeof(EPPI_PALETTE) ) ;
		}
		else if ( pImageBuf->fdwFormat == EPPI_GRAY_IMAGE )
		{
			for ( int i = 0; i < 0x100; i ++ )
			{
				rgbTable[i].rgbBlue     = (BYTE) i ;
				rgbTable[i].rgbGreen    = (BYTE) i ;
				rgbTable[i].rgbRed      = (BYTE) i ;
				rgbTable[i].rgbReserved = 0 ;
			}
		}
	}
	//
	// ファイルヘッダを出力
	//
	BITMAPFILEHEADER	bmfh ;
	bmfh.bfType = *((WORD*)"BM") ;
	bmfh.bfSize =
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
			+ nPalCount * sizeof(RGBQUAD) + dwImageBytes ;
	bmfh.bfReserved1 = 0 ;
	bmfh.bfReserved2 = 0 ;
	bmfh.bfOffBits =
		sizeof(BITMAPFILEHEADER)
			+ sizeof(BITMAPINFOHEADER)
			+ nPalCount * sizeof(RGBQUAD) ;
	//
	pInst->m_pfile->Write( &bmfh, sizeof(BITMAPFILEHEADER) ) ;
	pInst->m_pfile->Write( &bmih, sizeof(BITMAPINFOHEADER) ) ;
	if ( nPalCount != 0 )
	{
		pInst->m_pfile->Write( rgbTable, nPalCount * sizeof(RGBQUAD) ) ;
	}
	//
	// 画像データを順次書き出す
	//
	const BYTE *	ptrSrcLine =
		pImageBuf->ptrImageBuf
			+ (int)(pImageBuf->dwHeight - 1) * pImageBuf->nBytesPerLine ;
	BYTE *	ptrLineBuf = (BYTE*) ::GlobalAlloc( GPTR, nLineBytes ) ;
	//
	for ( int y = 0; y < (int) pImageBuf->dwHeight; y ++ )
	{
		::memmove( ptrLineBuf, ptrSrcLine, nLineBytes ) ;
		ptrSrcLine -= pImageBuf->nBytesPerLine ;
		//
		pInst->m_pfile->Write( ptrLineBuf, nLineBytes ) ;
		//
		if ( pAdvState != NULL )
		{
			pAdvState->Update( y + 1, pImageBuf->dwHeight ) ;
		}
	}
	//
	::GlobalFree( ptrLineBuf ) ;
	//
	return	NULL ;
}


