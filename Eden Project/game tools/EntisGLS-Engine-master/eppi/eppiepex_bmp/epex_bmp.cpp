
/*****************************************************************************
                             EPPI �v���O�C��
              BMP (Windows Bitmap) format �摜�G�N�X�|�[�^�[
 -----------------------------------------------------------------------------
          Copyright (C) 2002 Leshade Entis. All rights reserved.
 *****************************************************************************/


#include "epex_bmp.h"


//////////////////////////////////////////////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////

HINSTANCE	g_hInst = NULL ;

extern	"C" __declspec(dllexport)
		EPPI_DESCRIPTOR	_eppi_descriptor =
{
	EPPI_VERSION,				// �o�[�W����
	EPPI_NORMAL_PRIORITY,		// �D��x
	EPPI_IMAGE_EXPORTER,		// �v���O�C���̎��
	&EPPI_BMP_Initialize,		// �������֐�
	&EPPI_BMP_Destroy,			// �I���֐�
	&EPPI_BMP_Information,		// �v���O�C�����֐�
	(EPPI_COMMON_INTERFACE*)
	&g_imgexp					// �v���O�C���E�C���^�[�t�F�[�X
} ;

EPPI_IMAGE_EXPORT	g_imgexp =
{
	"BMP �G�N�X�|�[�^�[",				// �v���O�C����
	"Window Bitmap (*.bmp)",			// �t�B���^��
	"bmp",								// �Ή��g���q
	0,									// �Ή��@�\
	0,									// �\�񁁂O
	&BMP_Create,						// �C���X�^���X�쐬
	&BMP_Destroy,						// �C���X�^���X�j��
	&BMP_IsSupported,					// �Ή����Ă���t�H�[�}�b�g���H
	&BMP_Option,						// �I�v�V�����ݒ�
	&BMP_Open,							// �t�@�C�����J��
	&BMP_Close,							// �t�@�C�������
	&BMP_SetInformation,				// �摜����ݒ�
	&BMP_SetPreview,					// �v���r���[�摜�������o��
	&BMP_SetImage						// �摜�������o��
} ;


//////////////////////////////////////////////////////////////////////////////
// �O���[�o���֐�
//////////////////////////////////////////////////////////////////////////////

// DLL �G���g���|�C���g
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

// �������m��
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
// �v���O�C���E�C���^�[�t�F�[�X
//////////////////////////////////////////////////////////////////////////////

// �v���O�C���̏�����
//////////////////////////////////////////////////////////////////////////////
EPPI_INSTANCE __stdcall EPPI_BMP_Initialize( EPPI_GLOBAL_FUNC global )
{
	MY_EPPI_INSTANCE *	pInst = new MY_EPPI_INSTANCE ;
	pInst->global = global ;
	return	pInst ;
}

// �v���O�C���̏I��
//////////////////////////////////////////////////////////////////////////////
void __stdcall EPPI_BMP_Destroy( EPPI_INSTANCE instance )
{
	delete	(MY_EPPI_INSTANCE*) instance ;
}

// �v���O�C���̏��
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

// �����o���C���X�^���X�̐���
//////////////////////////////////////////////////////////////////////////////
void * __stdcall BMP_Create( EPPI_INSTANCE instance )
{
	MY_IMAGE_WRITE *	pInst = new MY_IMAGE_WRITE ;
	pInst->m_global = ((MY_EPPI_INSTANCE*)instance)->global ;
	pInst->m_pfile = NULL ;
	return	pInst ;
}

// �����o���C���X�^���X�̔j��
//////////////////////////////////////////////////////////////////////////////
void __stdcall BMP_Destroy( void * instance )
{
	delete	(MY_IMAGE_WRITE*) instance ;
}

// �Ή����Ă���t�H�[�}�b�g���H
//////////////////////////////////////////////////////////////////////////////
int __stdcall BMP_IsSupported
	( void * instance, const EPPI_IMAGE_INFO * pInformation )
{
	return	1 ;
}

// �I�v�V�����ݒ�
//////////////////////////////////////////////////////////////////////////////
int __stdcall BMP_Option( void * instance, void * hParentWnd, DWORD dwFlags )
{
	return	EPPI_OPTION_OK ;
}

// �t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Open
	( void * instance, const EPPI_FILE_INTERFACE * pfile )
{
	MY_IMAGE_WRITE *	pInst = (MY_IMAGE_WRITE*) instance ;
	pInst->m_pfile = pfile ;
	return	NULL ;
}

// �t�@�C�������
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Close( void * instance )
{
	return	NULL ;
}

// �摜����ݒ肷��
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetInformation
	( void * instance,
		const EPPI_IMAGE_INFO * pInformation,
		const EPPI_IMAGE_INFO * pPreviewInfo )
{
	return	NULL ;
}

// �v���r���[�摜�������o��
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetPreview
	( void * instance, const EPPI_IMAGE_BUFFER * pPreviewBuf )
{
	return	NULL ;
}

// �摜�������o��
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetImage
	( void * instance,
		const EPPI_IMAGE_BUFFER * pImageBuf,
		const EPPI_ADVANCE_STATE * pAdvState )
{
	MY_IMAGE_WRITE *	pInst = (MY_IMAGE_WRITE*) instance ;
	pInst->m_pfile->Seek( 0 ) ;
	//
	// �r�b�g�}�b�v���w�b�_�𐶐�
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
	// �p���b�g�e�[�u�����擾
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
	// �t�@�C���w�b�_���o��
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
	// �摜�f�[�^�����������o��
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


