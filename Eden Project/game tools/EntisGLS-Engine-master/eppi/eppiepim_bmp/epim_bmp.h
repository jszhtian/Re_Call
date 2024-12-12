
/*****************************************************************************
                             EPPI プラグイン
              BMP (Window Bitmap) format 画像インポーター
 -----------------------------------------------------------------------------
            Copyright (C) Leshade Entis. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>

#include "..\include\eppi.h"
#include "resource.h"


//
// グローバル変数
//////////////////////////////////////////////////////////////////////////////

extern	HINSTANCE	g_hInst ;
extern	"C" __declspec(dllexport)
		EPPI_DESCRIPTOR	_eppi_descriptor ;
extern	EPPI_IMAGE_IMPORT	g_imgimp ;


//
// グローバル関数
//////////////////////////////////////////////////////////////////////////////

// DLL エントリポイント
BOOL WINAPI main
	( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) ;


//
// プラグイン・インターフェース
//////////////////////////////////////////////////////////////////////////////

EPPI_INSTANCE __stdcall EPPI_BMP_Initialize( EPPI_GLOBAL_FUNC global ) ;
void __stdcall EPPI_BMP_Destroy( EPPI_INSTANCE instance ) ;
void __stdcall EPPI_BMP_Information( EPPI_INSTANCE instance, void * hParentWnd ) ;

void * __stdcall BMP_Create( EPPI_INSTANCE instance ) ;
void __stdcall BMP_Destroy( void * instance ) ;
EPPIError __stdcall BMP_Open
	( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
EPPIError __stdcall BMP_Close( void * instance ) ;
const EPPI_IMAGE_INFO *
	__stdcall BMP_GetInformation( void * instance ) ;
EPPIError __stdcall BMP_SetInformation
	( void * instance, const EPPI_IMAGE_INFO * pInfomation ) ;
const EPPI_IMAGE_INFO *
	 __stdcall BMP_HasPreviewImage( void * instance ) ;
EPPIError __stdcall BMP_GetPreview
	( void * instance, EPPI_IMAGE_BUFFER * pImageBuf ) ;
EPPIError __stdcall BMP_GetImage
	( void * instance,
		EPPI_IMAGE_BUFFER * pImageBuf,
		const EPPI_ADVANCE_STATE * pAdvState ) ;


//
// プラグイン・インスタンス
//////////////////////////////////////////////////////////////////////////////

struct	MY_EPPI_INSTANCE
{
	EPPI_GLOBAL_FUNC	global ;
} ;

struct	MY_BMP_INSTANCE
{
	const EPPI_FILE_INTERFACE *	pfile ;			// ファイルインターフェース
	BITMAPFILEHEADER			bmfh ;			// ファイルヘッダ
	BITMAPINFOHEADER			bmih ;			// 情報ヘッダ
	RGBQUAD						rgb[0x100] ;	// パレットテーブル
	EPPI_IMAGE_INFO				info ;			// 画像情報
} ;

