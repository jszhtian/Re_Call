
/*****************************************************************************
                             EPPI �v���O�C��
              BMP (Window Bitmap) format �摜�C���|�[�^�[
 -----------------------------------------------------------------------------
          Copyright (C) 2002 Leshade Entis. All rights reserved.
 *****************************************************************************/


#include "epim_bmp.h"


//////////////////////////////////////////////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////

HINSTANCE	g_hInst = NULL ;

extern	"C" __declspec(dllexport)
		EPPI_DESCRIPTOR	_eppi_descriptor =
{
	EPPI_VERSION,				// �o�[�W����
	EPPI_NORMAL_PRIORITY,		// �D��x
	EPPI_IMAGE_IMPORTER,		// �v���O�C���̎��
	&EPPI_BMP_Initialize,		// �������֐�
	&EPPI_BMP_Destroy,			// �I���֐�
	&EPPI_BMP_Information,		// �v���O�C�����֐�
	(EPPI_COMMON_INTERFACE*)
	&g_imgimp					// �v���O�C���E�C���^�[�t�F�[�X
} ;

EPPI_IMAGE_IMPORT	g_imgimp =
{
	"BMP�C���|�[�^�[",					// �v���O�C����
	"Window Bitmap (*.bmp)",			// �t�B���^��
	"bmp",								// �Ή��g���q
	0,									// �Ή��@�\
	0,									// �\�񁁂O
	&BMP_Create,						// �C���X�^���X�쐬
	&BMP_Destroy,						// �C���X�^���X�j��
	&BMP_Open,							// �t�@�C�����J��
	&BMP_Close,							// �t�@�C�������
	&BMP_GetInformation,				// �摜���擾
	&BMP_SetInformation,				// �摜��񏑂��o���i�Ή����Ă���ꍇ�̂݁j
	&BMP_HasPreviewImage,				// �v���r���[�摜�������Ă��邩�H
	&BMP_GetPreview,					// �v���r���[�摜�擾
	&BMP_GetImage						// �摜�擾
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

void * operator new ( size_t stBytes )
{
	return	::GlobalAlloc( GMEM_FIXED, stBytes ) ;
}

void operator delete ( void * ptrObj )
{
	::GlobalFree( ptrObj ) ;
}


//////////////////////////////////////////////////////////////////////////////
// �v���O�C���E�C���^�[�t�F�[�X
//////////////////////////////////////////////////////////////////////////////

//
// �v���O�C���������֐�
//////////////////////////////////////////////////////////////////////////////
EPPI_INSTANCE __stdcall EPPI_BMP_Initialize( EPPI_GLOBAL_FUNC global )
{
	MY_EPPI_INSTANCE *	pInst = new MY_EPPI_INSTANCE ;
	pInst->global = global ;
	return	pInst ;
}

//
// �v���O�C���I���֐�
//////////////////////////////////////////////////////////////////////////////
void __stdcall EPPI_BMP_Destroy( EPPI_INSTANCE instance )
{
	delete	(MY_EPPI_INSTANCE*) instance ;
}

//
// �v���O�C�����\��
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
// �摜�C���X�^���X�쐬
//////////////////////////////////////////////////////////////////////////////
void * __stdcall BMP_Create( EPPI_INSTANCE instance )
{
	MY_BMP_INSTANCE *	pInst = new MY_BMP_INSTANCE ;
	::memset( pInst, 0, sizeof(MY_BMP_INSTANCE) ) ;
	return	pInst ;
}

//
// �摜�C���X�^���X�j��
//////////////////////////////////////////////////////////////////////////////
void __stdcall BMP_Destroy( void * instance )
{
	delete	(MY_BMP_INSTANCE*) instance ;
}

//
// �摜�t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Open
	( void * instance, const EPPI_FILE_INTERFACE * pfile )
{
	MY_BMP_INSTANCE *	pInst = (MY_BMP_INSTANCE*) instance ;
	pInst->pfile = pfile ;
	//
	// �t�@�C���w�b�_��ǂݍ���
	//
	DWORD	dwReadBytes ;
	dwReadBytes = pfile->Read( &(pInst->bmfh), sizeof(BITMAPFILEHEADER) ) ;
	if ( dwReadBytes < sizeof(BITMAPFILEHEADER) )
	{
		return	"�t�@�C���w�b�_��ǂݍ��߂܂���ł����B" ;
	}
	if ( pInst->bmfh.bfType != *((WORD*)"BM") )
	{
		return	"�s���� BMP �t�@�C���ł��B" ;
	}
	//
	// BMP ���w�b�_��ǂݍ���
	//
	dwReadBytes = pfile->Read( &(pInst->bmih), sizeof(BITMAPINFOHEADER) ) ;
	if ( dwReadBytes < sizeof(BITMAPINFOHEADER) )
	{
		return	"BMP ���w�b�_��ǂݍ��߂܂���ł����B" ;
	}
	if ( pInst->bmih.biSize != sizeof(BITMAPINFOHEADER) )
	{
		return	"���Ή��� BMP �t�@�C���ł��B" ;
	}
	//
	// �p���b�g����ݒ肷��
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
	// �p���b�g����ǂݍ���
	//
	if ( pInst->bmih.biClrUsed != 0 )
	{
		DWORD	dwPalBytes = sizeof(RGBQUAD) * pInst->bmih.biClrUsed ;
		dwReadBytes = pfile->Read( pInst->rgb, dwPalBytes ) ;
		if ( dwReadBytes < dwPalBytes )
		{
			return	"�p���b�g�e�[�u����ǂݍ��߂܂���ł����B" ;
		}
	}
	//
	// �摜����ݒ肷��
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
// �t�@�C�������
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_Close( void * instance )
{
	return	NULL ;
}

//
// �摜�����擾����
//////////////////////////////////////////////////////////////////////////////
const EPPI_IMAGE_INFO *
	__stdcall BMP_GetInformation( void * instance )
{
	return	& (((MY_BMP_INSTANCE*) instance)->info) ;
}

//
// �摜����ݒ肷��
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_SetInformation
	( void * instance, const EPPI_IMAGE_INFO * pInfomation )
{
	return	"BMP �t�@�C���ɂ͕t���I�ȉ摜���������o���܂���B" ;
}

//
// �v���r���[�摜�����邩�H
//////////////////////////////////////////////////////////////////////////////
const EPPI_IMAGE_INFO *
	 __stdcall BMP_HasPreviewImage( void * instance )
{
	return	NULL ;
}

//
// �v���r���[�摜���擾
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_GetPreview
	( void * instance, EPPI_IMAGE_BUFFER * pImageBuf )
{
	return	"BMP �t�@�C���ɂ̓v���r���[�摜�͊܂܂�Ă��܂���B" ;
}

//
// �摜���擾
//////////////////////////////////////////////////////////////////////////////
EPPIError __stdcall BMP_GetImage
	( void * instance,
		EPPI_IMAGE_BUFFER * pImageBuf,
		const EPPI_ADVANCE_STATE * pAdvState )
{
	//
	// �o�b�t�@�̌���
	//
	MY_BMP_INSTANCE *	pInst = (MY_BMP_INSTANCE*) instance ;
	if ( (pImageBuf->dwWidth != pInst->info.dwWidth)
		|| (pImageBuf->dwHeight != pInst->info.dwHeight)
		|| (pImageBuf->dwBitsPerPixel != pInst->info.dwBitsPerPixel) )
	{
		return	"�o�b�t�@�̏�񂪉摜�̏��ƈ�v���܂���B" ;
	}
	//
	// �p���b�g�e�[�u������
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
	// �摜�ǂݍ��ݏ���
	//
	DWORD	dwLineBytes =
		(pInst->bmih.biWidth * pInst->bmih.biBitCount + 0x07) >> 3 ;
	DWORD	dwBytesPerLine =
		((pInst->bmih.biWidth *
			pInst->bmih.biBitCount + 0x1F) & (~0x1F)) >> 3 ;
	pInst->pfile->Seek( pInst->bmfh.bfOffBits ) ;
	//
	// �摜�ǂݍ���
	//
	DWORD	dwLastAdv = -1 ;
	BYTE *	pbytLine = pImageBuf->ptrImageBuf ;
	pbytLine += pImageBuf->nBytesPerLine * (pInst->info.dwHeight - 1) ;
	DWORD	dwPos = pInst->bmfh.bfOffBits ;
	//
	for ( DWORD y = 0; y < pInst->info.dwHeight; y ++ )
	{
		//
		// 1���C���ǂݍ���
		//
		DWORD	dwReadBytes ;
		dwReadBytes = pInst->pfile->Read( pbytLine, dwLineBytes ) ;
		if ( dwReadBytes < dwLineBytes )
		{
			return	"�摜��ǂݍ��߂܂���ł����B" ;
		}
		//
		// ���̃��C���ֈړ�
		//
		dwPos += dwBytesPerLine ;
		if ( dwLineBytes != dwBytesPerLine )
		{
			pInst->pfile->Seek( dwPos ) ;
		}
		pbytLine -= pImageBuf->nBytesPerLine ;
		//
		// �i�s�󋵍X�V
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

