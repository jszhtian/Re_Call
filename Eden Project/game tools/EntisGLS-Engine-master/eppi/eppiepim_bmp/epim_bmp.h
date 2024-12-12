
/*****************************************************************************
                             EPPI �v���O�C��
              BMP (Window Bitmap) format �摜�C���|�[�^�[
 -----------------------------------------------------------------------------
            Copyright (C) Leshade Entis. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>

#include "..\include\eppi.h"
#include "resource.h"


//
// �O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////

extern	HINSTANCE	g_hInst ;
extern	"C" __declspec(dllexport)
		EPPI_DESCRIPTOR	_eppi_descriptor ;
extern	EPPI_IMAGE_IMPORT	g_imgimp ;


//
// �O���[�o���֐�
//////////////////////////////////////////////////////////////////////////////

// DLL �G���g���|�C���g
BOOL WINAPI main
	( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) ;


//
// �v���O�C���E�C���^�[�t�F�[�X
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
// �v���O�C���E�C���X�^���X
//////////////////////////////////////////////////////////////////////////////

struct	MY_EPPI_INSTANCE
{
	EPPI_GLOBAL_FUNC	global ;
} ;

struct	MY_BMP_INSTANCE
{
	const EPPI_FILE_INTERFACE *	pfile ;			// �t�@�C���C���^�[�t�F�[�X
	BITMAPFILEHEADER			bmfh ;			// �t�@�C���w�b�_
	BITMAPINFOHEADER			bmih ;			// ���w�b�_
	RGBQUAD						rgb[0x100] ;	// �p���b�g�e�[�u��
	EPPI_IMAGE_INFO				info ;			// �摜���
} ;

