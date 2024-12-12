
/*****************************************************************************
                             EPPI プラグイン
              BMP (Windows Bitmap) format 画像エクスポーター
 -----------------------------------------------------------------------------
          Copyright (C) 2002 Leshade Entis. All rights reserved.
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
extern	EPPI_IMAGE_EXPORT	g_imgexp ;


//
// グローバル関数
//////////////////////////////////////////////////////////////////////////////

// DLL エントリポイント
BOOL WINAPI main
	( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) ;

// メモリ確保
void * operator new ( size_t stBytes ) ;
void operator delete ( void * ptrObj ) ;


//
// プラグイン・インターフェース
//////////////////////////////////////////////////////////////////////////////

EPPI_INSTANCE __stdcall EPPI_BMP_Initialize( EPPI_GLOBAL_FUNC global ) ;
void __stdcall EPPI_BMP_Destroy( EPPI_INSTANCE instance ) ;
void __stdcall EPPI_BMP_Information( EPPI_INSTANCE instance, void * hParentWnd ) ;

void * __stdcall BMP_Create( EPPI_INSTANCE instance ) ;
void __stdcall BMP_Destroy( void * instance ) ;
int __stdcall BMP_IsSupported
	( void * instance, const EPPI_IMAGE_INFO * pInformation ) ;
int __stdcall BMP_Option
	( void * instance, void * hParentWnd, DWORD dwFlags ) ;
EPPIError __stdcall BMP_Open
	( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
EPPIError __stdcall BMP_Close( void * instance ) ;
EPPIError __stdcall BMP_SetInformation
	( void * instance,
		const EPPI_IMAGE_INFO * pInformation,
		const EPPI_IMAGE_INFO * pPreviewInfo ) ;
EPPIError __stdcall BMP_SetPreview
	( void * instance, const EPPI_IMAGE_BUFFER * pPreviewBuf ) ;
EPPIError __stdcall BMP_SetImage
	( void * instance,
		const EPPI_IMAGE_BUFFER * pImageBuf,
		const EPPI_ADVANCE_STATE * pAdvState ) ;


//
// プラグイン・インスタンス
//////////////////////////////////////////////////////////////////////////////

struct	MY_EPPI_INSTANCE
{
	EPPI_GLOBAL_FUNC	global ;
} ;


//
// 画像保存インスタンス
//////////////////////////////////////////////////////////////////////////////

struct	MY_IMAGE_WRITE
{
	EPPI_GLOBAL_FUNC			m_global ;
	const EPPI_FILE_INTERFACE *	m_pfile ;
} ;

