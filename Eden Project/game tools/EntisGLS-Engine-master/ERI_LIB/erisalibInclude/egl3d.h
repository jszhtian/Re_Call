
/*****************************************************************************
                          Entis Graphic Library
 -----------------------------------------------------------------------------
    Copyright (c) 2002-2004 Leshade Entis, Entis-soft. Al rights reserved.
 *****************************************************************************/


#if	!defined(__EGL3D_H__)
#define	__EGL3D_H__

#if	!defined(COMPACT_NOA_DECODER)

//////////////////////////////////////////////////////////////////////////////
// �摜�I�u�W�F�N�g
//////////////////////////////////////////////////////////////////////////////

class	EGLImage	: public	ESLObject
{
protected:
	enum	ImageOwnerFlag
	{
		iofRefInfo		= 0,
		iofRefBuffer,
		iofOwnBuffer
	} ;
	PEGL_IMAGE_INFO	m_pImage ;
	ImageOwnerFlag	m_iofOwnerFlag ;

public:
	// �摜�W�J�R�[���o�b�N
	typedef ESLError
		(__stdcall *PFUNC_CALLBACK_DECODE)
			( EGLImage * ptrImage, void * ptrData,
				DWORD dwDecoded, DWORD dwTotal ) ;
public:
	// �摜�W�J�I�u�W�F�N�g
	class	EGLImageDecoder	: public	ERISADecoder
	{
	public:
		EGLImage *				m_ptrImage ;
		PFUNC_CALLBACK_DECODE	m_pfnCallback ;
		void *					m_ptrCallbackData ;
	public:
		EGLImageDecoder
			( EGLImage * ptrImage,
				PFUNC_CALLBACK_DECODE pfnCallback, void * ptrData )
			: m_ptrImage( ptrImage ),
				m_pfnCallback( pfnCallback ), m_ptrCallbackData( ptrData ) { }
		virtual ESLError OnDecodedBlock
			( LONG line, LONG column, const EGL_IMAGE_RECT & rect ) ;
	} ;

public:
	// �\�z�֐�
	EGLImage( void ) : m_pImage(NULL), m_iofOwnerFlag(iofRefInfo) { }
	// ���Ŋ֐�
	virtual ~EGLImage( void )
		{
			if ( m_pImage && m_iofOwnerFlag )
				DeleteImage( ) ;
		}
	// �N���X���
	DECLARE_CLASS_INFO( EGLImage, ESLObject )

public:
	// �摜�o�b�t�@�֘A�t��
	virtual void AttachImage( PEGL_IMAGE_INFO pImage ) ;
	// �摜�o�b�t�@�̎Q�Ƃ�ݒ�
	virtual void SetImageView
		( PEGL_IMAGE_INFO pImage, PCEGL_RECT pViewRect = NULL ) ;
	// �摜�o�b�t�@�쐬
	virtual PEGL_IMAGE_INFO CreateImage
		( DWORD fdwFormat, DWORD dwWidth, DWORD dwHeight,
				DWORD dwBitsPerPixel, DWORD dwFlags = 0 ) ;
	// �摜�o�b�t�@����
	virtual PEGL_IMAGE_INFO DuplicateImage
		( PCEGL_IMAGE_INFO pImage, DWORD dwFlags = 0 ) ;

	// �摜�t�@�C����ǂݍ���
	virtual ESLError ReadImageFile
		( ESLFileObject & file,
			PFUNC_CALLBACK_DECODE pfnCallback = NULL,
			void * ptrCallbackData = NULL ) ;
	// �摜�o�b�t�@����
	virtual void DeleteImage( void ) ;

	// �v���r���[�摜�W�J�t���O
	enum	PreviewFlags
	{
		pfForcePreview		= 0x0001,
		pfResizePreview		= 0x0002
	} ;
	// �v���r���[�摜��ǂݍ���
	ESLError ReadPreviewImage
		( ESLFileObject & file,
			DWORD dwMaxWidth, DWORD dwMaxHeight,
			DWORD dwPreviewFlags = pfForcePreview,
			EGL_SIZE * pOriginalSize = NULL,
			PFUNC_CALLBACK_DECODE pfnCallback = NULL,
			void * ptrCallbackData = NULL ) ;

	// ���k�����t���O
	enum	CompressTypeFlag
	{
		ctfCompatibleFormat,
		ctfExtendedFormat,
		ctfSuperiorArchitecure
	} ;
	// �摜�t�@�C���֏����o��
	ESLError WriteImageFile
		( ESLFileObject & file,
			CompressTypeFlag ctfType = ctfCompatibleFormat,
			DWORD dwFlags = ERISAEncoder::efNormalCmpr,
			const ERISAEncoder::PARAMETER * periep = NULL ) ;

public:
	// �摜���擾
	PEGL_IMAGE_INFO GetInfo( void ) const
		{
			return	m_pImage ;
		}
	operator PCEGL_IMAGE_INFO ( void ) const
		{
			return	m_pImage ;
		}
	operator PEGL_IMAGE_INFO ( void )
		{
			return	m_pImage ;
		}
	operator const EGL_IMAGE_INFO & ( void ) const
		{
			ESLAssert( m_pImage != NULL ) ;
			return	*m_pImage ;
		}
	// �摜�t�H�[�}�b�g�擾
	DWORD GetFormatType( void ) const
		{
			return	m_pImage ? m_pImage->fdwFormatType : 0 ;
		}
	// �摜�T�C�Y�擾
	EGLSize GetSize( void ) const
		{
			return	EGLSize( GetWidth(), GetHeight() ) ;
		}
	DWORD GetWidth( void ) const
		{
			return	m_pImage ? m_pImage->dwImageWidth : 0 ;
		}
	DWORD GetHeight( void ) const
		{
			return	m_pImage ? m_pImage->dwImageHeight : 0 ;
		}
	// �r�b�g�[�x�擾
	DWORD GetBitsPerPixel( void ) const
		{
			return	m_pImage ? m_pImage->dwBitsPerPixel : 0 ;
		}

public:
	// �摜�̏㉺���]
	ESLError ReverseVertically( void )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglReverseVertically( m_pImage ) ;
		}
	// �摜���f�o�C�X�R���e�L�X�g�ɕ`��
	ESLError DrawToDC( HDC hDstDC, int nPosX, int nPosY,
					PCEGL_SIZE pSizeToDraw, PCEGL_RECT pViewRect ) const
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglDrawToDC
				( hDstDC, m_pImage,
					nPosX, nPosY, pSizeToDraw, pViewRect ) ;
		}
	// �摜���w��̒l�œh��Ԃ�
	ESLError FillImage( EGL_PALETTE colorFill )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglFillImage( m_pImage, colorFill ) ;
		}
	// �w��s�N�Z�����擾
	EGL_PALETTE GetPixel( int nPosX, int nPosY ) const
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	(EGL_PALETTE) EGLPalette( 0UL ) ;
			return	::eglGetPixel( m_pImage, nPosX, nPosY ) ;
		}
	// �w��s�N�Z���ɐݒ�
	ESLError SetPixel( int nPosX, int nPosY, EGL_PALETTE colorPixel )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglSetPixel( m_pImage, nPosX, nPosY, colorPixel ) ;
		}

public:
	// �t�H�[�}�b�g�ϊ�
	ESLError ConvertFrom( PCEGL_IMAGE_INFO pSrcImage )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglConvertFormat( m_pImage, pSrcImage ) ;
		}
	// �g�[���t�B���^�K�p
	ESLError ApplyToneTable
		( PCEGL_IMAGE_INFO pSrcImage,
			const void * pBlueTone, const void * pGreenTone,
			const void * pRedTone, const void * pAlphaTone )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglApplyToneTable
				( m_pImage, pSrcImage,
					pBlueTone, pGreenTone, pRedTone, pAlphaTone ) ;
		}
	// �P�x�t�B���^�K�p
	ESLError SetColorTone
		( PCEGL_IMAGE_INFO pSrcImage,
			int nBlueTone, int nGreenTone, int nRedTone, int nAlphaTone )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglSetColorTone
				( m_pImage, pSrcImage,
					nBlueTone, nGreenTone, nRedTone, nAlphaTone ) ;
		}
	// 2�{�Ɋg�又��
	ESLError EnlargeDouble( PCEGL_IMAGE_INFO pSrcImage, DWORD dwFlags = 0 )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglEnlargeDouble( m_pImage, pSrcImage, dwFlags ) ;
		}
	// ���`���l������
	ESLError BlendAlphaChannel
		( PCEGL_IMAGE_INFO pSrcRGB, PCEGL_IMAGE_INFO pSrcAlpha,
			DWORD dwFlags = 0, SDWORD nAlphaBase = 0, DWORD nCoefficient = 0x1000 )
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglBlendAlphaChannel
				( m_pImage, pSrcRGB, pSrcAlpha,
					dwFlags, nAlphaBase, nCoefficient ) ;
		}
	// ���`���l������
	ESLError UnpackAlphaChannel
			( PEGL_IMAGE_INFO pDstRGB,
				PEGL_IMAGE_INFO pDstAlpha, DWORD dwFlags = 0 ) const
		{
			ESLAssert( m_pImage != NULL ) ;
			if ( m_pImage == NULL )
				return	eslErrGeneral ;
			return	::eglUnpackAlphaChannel
						( pDstRGB, pDstAlpha, m_pImage, dwFlags ) ;
		}

} ;


//////////////////////////////////////////////////////////////////////////////
// �A�j���[�V�����摜�I�u�W�F�N�g
//////////////////////////////////////////////////////////////////////////////

class	EGLAnimation	: public	EGLImage
{
protected:
	DWORD				m_dwTotalTime ;
	DWORD				m_dwCurrent ;
	ENumArray<UINT>		m_lstSequence ;
	EPtrObjArray<EGL_IMAGE_INFO>
						m_lstFrames ;
	EGL_POINT			m_ptHotSpot ;
	long int			m_nResolution ;

public:
	// �\�z�֐�
	EGLAnimation( void ) ;
	// ���Ŋ֐�
	virtual ~EGLAnimation( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EGLAnimation, EGLImage )

public:
	// �摜�o�b�t�@�̎Q�Ƃ�ݒ�
	virtual void SetImageView
		( PEGL_IMAGE_INFO pImage, PCEGL_RECT pViewRect = NULL ) ;
	// �摜�o�b�t�@�쐬
	virtual PEGL_IMAGE_INFO CreateImage
		( DWORD fdwFormat, DWORD dwWidth, DWORD dwHeight,
				DWORD dwBitsPerPixel, DWORD dwFlags = 0 ) ;
	// �摜�o�b�t�@����
	virtual PEGL_IMAGE_INFO DuplicateImage
		( PCEGL_IMAGE_INFO pImage, DWORD dwFlags = 0 ) ;
	// �摜�t�@�C����ǂݍ���
	virtual ESLError ReadImageFile
		( ESLFileObject & file,
			PFUNC_CALLBACK_DECODE pfnCallback = NULL,
			void * ptrCallbackData = NULL ) ;
	// �摜�o�b�t�@����
	virtual void DeleteImage( void ) ;
	// �摜�t�@�C���֏����o��
	ESLError WriteImageFile
		( ESLFileObject & file,
			CompressTypeFlag ctfType = ctfCompatibleFormat,
			DWORD dwFlags = ERISAEncoder::efNormalCmpr,
			const ERISAEncoder::PARAMETER * periep = NULL ) ;

public:
	// �V�[�P���X�e�[�u�����擾
	ENumArray<UINT> & GetSequenceTable( void )
		{
			return	m_lstSequence ;
		}
	// �t���[�����폜
	void RemoveFrameAt( int iFrame ) ;
	// �t���[����ǉ�
	void AddFrame( PEGL_IMAGE_INFO pImage ) ;
	// �t���[����}��
	void InsertFrame( int iFrame, PEGL_IMAGE_INFO pImage ) ;

public:
	// �S���Ԃ��擾
	DWORD GetTotalTime( void ) const
		{
			return	m_dwTotalTime ;
		}
	// �S�t���[������ݒ�
	void SetTotalTime( DWORD dwTotalTime )
		{
			m_dwTotalTime = dwTotalTime ;
		}
	// �S�t���[�������擾
	DWORD GetTotalFrameCount( void ) const
		{
			return	m_lstFrames.GetSize( ) ;
		}
	// �S�V�[�P���X�����擾
	DWORD GetSequenceLength( void ) const
		{
			return	m_lstSequence.GetSize( ) ;
		}
	// ���݂̃V�[�P���X�ԍ����擾
	DWORD GetCurrentSequence( void ) const
		{
			return	m_dwCurrent ;
		}
	// �z�b�g�X�|�b�g���擾
	EGL_POINT GetHotSpot( void ) const
		{
			return	m_ptHotSpot ;
		}
	// �z�b�g�X�|�b�g��ݒ�
	void SetHotSpot( EGL_POINT ptHotSpot )
		{
			m_ptHotSpot = ptHotSpot ;
		}
	// �𑜓x�擾
	long int GetResolution( void ) const
		{
			return	m_nResolution ;
		}
	// �𑜓x�ݒ�
	void SetResolution( long int nResolution )
		{
			m_nResolution = nResolution ;
		}
	// ���Ԃ���V�[�P���X�ԍ��ɕϊ�
	DWORD TimeToSequence( DWORD dwMilliSec ) const ;
	// �V�[�P���X�ԍ����玞�Ԃɕϊ�
	DWORD SequenceToTime( DWORD dwSequence ) const ;
	// �V�[�P���X�ԍ�����t���[���ԍ��ɕϊ�
	DWORD SequenceToFrame( DWORD dwSequence ) const ;
	// �w��t���[���̉摜���擾
	PEGL_IMAGE_INFO GetFrameAt( DWORD dwFrame ) ;
	// ���݂̃V�[�P���X�摜��ݒ肷��
	ESLError SetCurrentSequence( DWORD dwSequence ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �摜���f�B�A�ǂݍ��݃N���X
//////////////////////////////////////////////////////////////////////////////

namespace	Gdiplus
{
	class	GpBitmap ;
} ;
class	EGLMediaLoader	: public	EGLAnimation
{
public:
	// �\�z�֐�
	EGLMediaLoader( void ) ;
	// ���Ŋ֐�
	virtual ~EGLMediaLoader( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EGLMediaLoader, EGLAnimation )

public:
	// �摜�t�@�C����ǂݍ���
	ESLError LoadMediaFile
		( const char * pszFileName,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0,
			DWORD dwLimitFrames = 0, DWORD dwLimitSize = 0 ) ;
	ESLError ReadMediaFile
		( ESLFileObject & file,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0 ) ;
	// �v���r���[�摜��ǂݍ���
	ESLError LoadPreviewImage
		( const char * pszFileName,
			DWORD dwWidth, DWORD dwHeight,
			DWORD dwPreviewFlags = pfForcePreview,
			EGL_SIZE * pOriginalSize = NULL,
			PFUNC_CALLBACK_DECODE pfnCallback = NULL,
			void * ptrCallbackData = NULL ) ;

public:
	// Window Bitmap �t�@�C����ǂݍ���
	ESLError ReadBitmapFile
		( ESLFileObject & file,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0 ) ;
	// Photoshop PSD �t�@�C����ǂݍ���
	ESLError ReadPhotoshopPSDFile
		( ESLFileObject & file,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0,
			DWORD dwLimitFrames = 0, DWORD dwLimitSize = 0 ) ;
	// AVI �t�@�C����ǂݍ���
	ESLError LoadAviFile
		( const char * pszFileName,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0,
			DWORD dwLimitFrames = 0, DWORD dwLimitSize = 0 ) ;
	// GDI+ �𗘗p���ēǂݍ���
	ESLError LoadWithGDIplus
		( const char * pszFileName,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0 ) ;
	ESLError ReadWithGDIplus
		( ESLFileObject & file,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0 ) ;
	// �摜�t�H�[�}�b�g��ϊ�����
	ESLError ConvertFormatTo
		( DWORD fdwFormat, DWORD dwBitsPerPixel ) ;

public:
	// �r�b�g�}�b�v�f�[�^����摜�I�u�W�F�N�g���쐬����
	ESLError CreateFromBitmap
		( const BITMAPINFO * pbmi,
			void * ptrBitmap = NULL, DWORD dwFlags = 0 ) ;
	// �摜�I�u�W�F�N�g����p�b�N�hDIB���쐬����
	BITMAPINFO * CreatePackedDIB( void ** ppBitmap = NULL ) const ;
	// GDI+ �I�u�W�F�N�g����摜�I�u�W�F�N�g�֕ϊ�����
	ESLError ConvertFromGDIplus
		( Gdiplus::GpBitmap * pbitmap,
			DWORD fdwFormat = 0, DWORD dwBitsPerPixel = 0 ) ;
	// �摜�I�u�W�F�N�g���� GDI+ �I�u�W�F�N�g�֕ϊ�����
	ESLError CreateGDIplusBitmap( Gdiplus::GpBitmap *& pbitmap ) ;

public:
	// Windows Bitmap �t�@�C���������o��
	ESLError WriteBitmapFile( ESLFileObject & file ) ;
	// Photoshop PSD �t�@�C���������o��
	ESLError WritePhotoshopPSDFile( ESLFileObject & file ) ;
	// GDI+ �𗘗p���ď����o��
	ESLError SaveWithGDIplus
		( const char * pszFileName, const wchar_t * pwszMimeType ) ;
	ESLError WriteWithGDIplus
		( ESLFileObject & file, const wchar_t * pwszMimeType ) ;

protected:
	// MIME �^�C�v���� CLSID ���擾
	static ESLError GetEncoderClsid
		( const wchar_t * pwszMimeType, CLSID & clsidEncoder ) ;

public:
	struct	GDIP_CODEC
	{
		EWideString	wstrCodecName ;
		EWideString	wstrDllName ;
		EWideString	wstrDescription ;
		EWideString	wstrFileExtension ;
		EWideString	wstrMimeType ;
	} ;
	// GDI+ �ŗ��p�\�ȃf�R�[�_�[��񋓂���
	static ESLError EnumGDIplusDecoderList
		( EObjArray<GDIP_CODEC> & lstCodec ) ;
	// GDI+ �ŗ��p�\�ȃG���R�[�_�[��񋓂���
	static ESLError EnumGDIplusEncoderList
		( EObjArray<GDIP_CODEC> & lstCodec ) ;

public:
	// ������
	static void Initialize( bool fComMTA = true ) ;
	// �I��
	static void Close( void ) ;
	// GDI+ �͗L�����H
	static bool IsInstalledGDIplus( void ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �e�N�X�`���摜���C�u����
//////////////////////////////////////////////////////////////////////////////

class	E3DTextureLibrary	: public	EWStrTagArray<EGLImage>
{
protected:
	E3DTextureLibrary *	m_pParent ;

public:
	// �\�z�֐�
	E3DTextureLibrary( void ) : m_pParent( NULL ) { }
	// ���Ŋ֐�
	virtual ~E3DTextureLibrary( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DTextureLibrary, EPtrArray )

public:
	// �֘A�e���C�u�������擾
	E3DTextureLibrary * GetParent( void ) const
		{
			return	m_pParent ;
		}
	// �֘A�e���C�u������ݒ�
	void SetParent( E3DTextureLibrary * pParent )
		{
			m_pParent = pParent ;
		}
	// �e�N�X�`���摜���擾
	PEGL_IMAGE_INFO GetTextureAs
		( const wchar_t * pwszName, bool fSearchParent = true ) const ;
	// �摜�|�C���^����e�N�X�`�������擾
	ESLError GetTextureName
		( EWideString & wstrName,
			PEGL_IMAGE_INFO pTexture, bool fSearchParent = true ) const ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �\�ʑ������C�u����
//////////////////////////////////////////////////////////////////////////////

class	E3DSurfaceLibrary	: public	EWStrTagArray<E3D_SURFACE_ATTRIBUTE>
{
protected:
	E3DSurfaceLibrary *	m_pParent ;

public:
	// �\�z�֐�
	E3DSurfaceLibrary( void ) : m_pParent( NULL ) { }
	// ���Ŋ֐�
	virtual ~E3DSurfaceLibrary( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DSurfaceLibrary, EPtrArray )

public:
	// �֘A�e���C�u�������擾
	E3DSurfaceLibrary * GetParent( void ) const
		{
			return	m_pParent ;
		}
	// �֘A�e���C�u������ݒ�
	void SetParent( E3DSurfaceLibrary * pParent )
		{
			m_pParent = pParent ;
		}
	// �������擾����
	E3D_SURFACE_ATTRIBUTE * GetAttributeAs
		( const wchar_t * pszName, bool fSearchParent = true ) const ;
	// �����|�C���^���瑮�������擾
	ESLError GetAttributeName
		( EWideString & wstrName,
			E3D_SURFACE_ATTRIBUTE * pAttr, bool fSearchParent = true ) const ;

} ;


//////////////////////////////////////////////////////////////////////////////
// ���f���I�u�W�F�N�g
//////////////////////////////////////////////////////////////////////////////

class	E3DPolygonModel	: public	ESLObject
{
public:
	// �\�z�֐�
	E3DPolygonModel( void ) ;
	// ���Ŋ֐�
	virtual ~E3DPolygonModel( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DPolygonModel, ESLObject )

protected:
	E3DTextureLibrary			m_txlib ;
	E3DSurfaceLibrary			m_sflib ;

	unsigned int				m_nVertexCount ;
	PE3D_VECTOR4				m_pVertexes ;
	PE3D_VECTOR4				m_pVertexesBuf ;
	unsigned int				m_nNormalCount ;
	PE3D_VECTOR4				m_pNormals ;
	PE3D_VECTOR4				m_pNormalsBuf ;
	EPtrObjArray<E3D_PRIMITIVE_POLYGON>
								m_lstPrimitives ;

public:
	// ���f���f�[�^��ǂݍ���
	virtual ESLError ReadModel( ESLFileObject & file ) ;
protected:
	// �e�N�X�`���f�[�^��ǂݍ���
	virtual ESLError ReadTextureRecord( EMCFile & file ) ;
	// �\�ʑ����f�[�^��ǂݍ���
	virtual ESLError ReadSurfaceRecord( EMCFile & file ) ;
	// ���f���f�[�^��ǂݍ���
	virtual ESLError ReadModelRecord( EMCFile & file ) ;
	// ���[�U�[��`�̃��R�[�h��ǂݍ���
	virtual ESLError ReadUserRecord( EMCFile & file, UINT64 idRec ) ;

public:
	// ���f���f�[�^�������o��
	ESLError WriteModel( ESLFileObject & file ) ;
protected:
	// �e�N�X�`���f�[�^�������o��
	virtual ESLError WriteTextureRecord( EMCFile & file ) ;
	// �\�ʑ����f�[�^�������o��
	virtual ESLError WriteSurfaceRecord( EMCFile & file ) ;
	// ���f���f�[�^�������o��
	virtual ESLError WriteModelRecord( EMCFile & file ) ;
	// ���[�U�[��`�̃��R�[�h�������o��
	virtual ESLError WriteUserRecord( EMCFile & file ) ;

public:
	// ���f���f�[�^���폜����
	virtual void DeleteContents( void ) ;
	// �e�N�X�`���摜���C�u�������Q�Ƃ���
	E3DTextureLibrary & TextureLibrary( void )
		{
			return	m_txlib ;
		}
	// �\�ʑ������C�u�������Q�Ƃ���
	E3DSurfaceLibrary & SurfaceLibrary( void )
		{
			return	m_sflib ;
		}

public:
	// ���_�̐����擾����
	unsigned int GetVertexCount( void ) const
		{
			return	m_nVertexCount ;
		}
	// ���_�o�b�t�@�̃A�h���X���擾����
	PE3D_VECTOR4 GetVertexList( unsigned int nIndex = 0 )
		{
			ESLAssert( m_pVertexes != NULL ) ;
			ESLAssert( nIndex < m_nVertexCount ) ;
			return	m_pVertexes + nIndex ;
		}
	PE3D_VECTOR4 GetVertexBuffer( void )
		{
			return	m_pVertexesBuf ;
		}
	// �@���̐����擾����
	unsigned int GetNormalCount( void ) const
		{
			return	m_nNormalCount ;
		}
	// �@���o�b�t�@�̃A�h���X���擾����
	PE3D_VECTOR4 GetNormalList( unsigned int nIndex = 0 )
		{
			ESLAssert( m_pNormals != NULL ) ;
			ESLAssert( nIndex < m_nNormalCount ) ;
			return	m_pNormals + nIndex ;
		}
	PE3D_VECTOR4 GetNormalBuffer( void )
		{
			return	m_pNormalsBuf ;
		}
	// ���_�o�b�t�@���m�ۂ���
	virtual void AllocateVertexBuffer( unsigned int nCount ) ;
	// �@���o�b�t�@���m�ۂ���
	virtual void AllocateNormalBuffer( unsigned int nCount ) ;

public:
	// �v���~�e�B�u�����擾
	unsigned int GetPrimitiveCount( void ) const
		{
			return	m_lstPrimitives.GetSize( ) ;
		}
	// �v���~�e�B�u���擾
	E3D_PRIMITIVE_POLYGON * GetPrimitiveAt( unsigned int nIndex )
		{
			return	m_lstPrimitives.GetAt( nIndex ) ;
		}
	// �v���~�e�B�u�z����擾
	PE3D_PRIMITIVE_POLYGON * const GetPrimitiveList( void ) const
		{
			return	m_lstPrimitives.GetData( ) ;
		}
	// �|���S���v���~�e�B�u��ǉ�
	void AddPolygon
		( DWORD dwTypeFlag, PE3D_SURFACE_ATTRIBUTE pSurfAttr,
			DWORD dwVertexCount, const E3D_PRIMITIVE_VERTEX * pVertexes ) ;
	// �v���~�e�B�u��ǉ�
	void AddPrimitives
		( DWORD dwPrimitiveCount,
			const PE3D_PRIMITIVE_POLYGON * ppPrimitives ) ;
	// �v���~�e�B�u���폜
	void RemovePrimitives( unsigned int nFirst, unsigned int nCount ) ;
	// �v���~�e�B�u��S�č폜
	void RemoveAllPrimitive( void ) ;
	// �摜���f�����쐬����
	void CreateImagePolygon
		( PEGL_IMAGE_INFO pImage,
			PCEGL_RECT pView = NULL,
			const E3D_VECTOR_2D * pCenter = NULL,
			PCE3D_SURFACE_ATTRIBUTE pSurfAttr = NULL,
			REAL32 rFogDeepness = 0.0F,
			const EGL_PALETTE * pFogColor = NULL ) ;
	// �v���~�e�B�u�f�[�^�̃o�C�g�����v�Z����
	static DWORD CalcPrimitiveDataSize
		( const E3D_PRIMITIVE_POLYGON * pPrimitive ) ;

	friend	E3DModelJoint ;
} ;


//////////////////////////////////////////////////////////////////////////////
// ���f���W���C���g
//////////////////////////////////////////////////////////////////////////////

class	E3DModelJoint	: public	ESLObject
{
public:
	E3DModelJoint *	m_parent ;		// �e�W���C���g

	E3D_REV_MATRIX	m_rvmat ;		// ��]�s��i�K�w�����ꂽ�p�����[�^�j
	E3DVector		m_vmove ;		// ���s�ړ�

	EPtrObjArray<E3DPolygonModel>
					m_models ;		// ���f���f�[�^�ւ̎Q�ƃ��X�g
	EObjArray<E3DModelJoint>
					m_joints ;		// �T�u�W���C���g

protected:
	E3D_REV_MATRIX	m_rvmatParam ;	// ��]�s��i�K�w������Ă��Ȃ��p�����[�^�j
	E3DVector		m_vmoveParam ;	// ���s�ړ�

public:
	// �\�z�֐�
	E3DModelJoint( void ) ;
	E3DModelJoint( E3DPolygonModel * pmodel ) ;
	// ���Ŋ֐�
	virtual ~E3DModelJoint( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DModelJoint, ESLObject )

public:
	// �W���C���g����
	virtual E3DModelJoint * CreateJoint( void ) const ;
	// �p�����[�^���f
	virtual void RefreshJoint( void ) ;
	// �W���C���g��]����
	virtual void TransformJoint( const E3DModelJoint & mjParent ) ;
	// ���f����]����
	virtual void TransformModel
		( HEGL_RENDER_POLYGON hRender, E3DPolygonModel & model ) ;
	// �S�ẴW���C���g���폜
	void DeleteContents( void ) ;
	// �W���C���g����
	void CopyModelJoint( const E3DModelJoint & model ) ;

public:
	// ����W
	E3DVector & Position( void )
		{
			return	m_vmoveParam ;
		}
	// �ϊ��s��
	E3D_REV_MATRIX & Matrix( void )
		{
			return	m_rvmatParam ;
		}
	// �ϊ��s���������
	void InitializeMatrix( void )
		{
			m_rvmatParam.InitializeMatrix( E3DVector( 1, 1, 1 ) ) ;
		}
	// ������]
	void RevolveOnX( double rDeg ) ;
	// ������]
	void RevolveOnY( double rDeg ) ;
	// ������]
	void RevolveOnZ( double rDeg ) ;
	// �x�N�g���̕����Ɍ���
	void RevolveByAngleOn( const E3D_VECTOR & angle )
		{
			m_rvmatParam.RevolveByAngleOn( angle ) ;
		}
	void RevolveForAngle( const E3D_VECTOR & angle )
		{
			m_rvmatParam.RevolveForAngle( angle ) ;
		}
	// �g��
	void MagnifyByVector( const E3D_VECTOR & vector )
		{
			m_rvmatParam.MagnifyByVector( vector ) ;
		}

public:
	// ���f���f�[�^�ǉ�
	void AddModelRef( E3DPolygonModel * pmodel )
		{
			m_models.Add( pmodel ) ;
		}
	// ���f���f�[�^�z����Q��
	EPtrObjArray<E3DPolygonModel> & ModelList( void )
		{
			return	m_models ;
		}
	// �W���C���g�ǉ�
	void AddSubJoint( E3DModelJoint * pJoint )
		{
			pJoint->m_parent = this ;
			m_joints.Add( pJoint ) ;
		}
	// �W���C���g�z����Q��
	EObjArray<E3DModelJoint> & JointList( void )
		{
			return	m_joints ;
		}

} ;


//////////////////////////////////////////////////////////////////////////////
// ���g�ݍ��݃��f���I�u�W�F�N�g
//////////////////////////////////////////////////////////////////////////////

class	E3DBonePolygonModel	: public	E3DPolygonModel
{
public:
	// �\�z�֐�
	E3DBonePolygonModel( void ) ;
	// ���Ŋ֐�
	virtual ~E3DBonePolygonModel( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DBonePolygonModel, E3DPolygonModel )

public:
	// �{�[���W���C���g
	class	E3DBoneJoint	: public	E3DModelJoint
	{
	public:
		// �\�z�֐�
		E3DBoneJoint( void ) ;
		// ���Ŋ֐�
		virtual ~E3DBoneJoint( void ) ;
		// �N���X���
		DECLARE_CLASS_INFO( E3DBonePolygonModel::E3DBoneJoint, E3DModelJoint )
	public:
		EWideString		m_wstrName ;		// �{�[���̖��O
		E3DVector		m_vCenter ;			// �{�[���̊�_
		unsigned int	m_iRefVertex ;		// ���_�Q��
		unsigned int	m_nRefVertexCount ;
		REAL32 *		m_pVertexApply ;
		PE3D_VECTOR4	m_pVertexBuf ;
		unsigned int	m_iRefNormal ;		// �@���Q��
		unsigned int	m_nRefNormalCount ;
		REAL32 *		m_pNormalApply ;
		PE3D_VECTOR4	m_pNormalBuf ;
	public:
		// �W���C���g����
		virtual E3DModelJoint * CreateJoint( void ) const ;
		// �p�����[�^���f
		virtual void RefreshJoint( void ) ;
		// �W���C���g��]����
		virtual void TransformJoint( const E3DModelJoint & mjParent ) ;
		// ���f����]����
		virtual void TransformModel
			( HEGL_RENDER_POLYGON hRender, E3DPolygonModel & model ) ;
		// �S�ẴW���C���g���폜
		void DeleteContents( void ) ;
	public:
		// �{�[����ǂݍ���
		ESLError ReadBoneData( ESLFileObject & file ) ;
		// �{�[���������o��
		ESLError WriteBoneData( ESLFileObject & file ) ;
		// ���_�p�o�b�t�@�m��
		void AllocateVertexBuffer
			( unsigned int iFirst, unsigned int nCount ) ;
		// �@���p�o�b�t�@�m��
		void AllocateNormalBuffer
			( unsigned int iFirst, unsigned int nCount ) ;
		// �{�[���𖼑O�Ō���
		E3DBoneJoint * FindBoneAs( const wchar_t * pwszName ) ;
	} ;

public:
	EObjArray<E3DBoneJoint>
					m_lstBones ;	// �{�[���z��i���[�g�ɒ��ڑ�������́j
	PE3D_VECTOR4	m_pOrgVertexes ;
	PE3D_VECTOR4	m_pOrgNormals ;

public:
	// ���f���f�[�^���폜����
	virtual void DeleteContents( void ) ;
	// ���_�o�b�t�@���m�ۂ���
	virtual void AllocateVertexBuffer( unsigned int nCount ) ;
	// �@���o�b�t�@���m�ۂ���
	virtual void AllocateNormalBuffer( unsigned int nCount ) ;

public:
	// ���f���f�[�^��ǂݍ���
	virtual ESLError ReadModel( ESLFileObject & file ) ;
protected:
	// ���[�U�[��`�̃��R�[�h��ǂݍ���
	virtual ESLError ReadUserRecord( EMCFile & file, UINT64 idRec ) ;
public:
	// ���f���f�[�^�������o��
	ESLError WriteModel( ESLFileObject & file ) ;
protected:
	// ���[�U�[��`�̃��R�[�h�������o��
	virtual ESLError WriteUserRecord( EMCFile & file ) ;

public:
	// �{�[���̌��݂̃p�����[�^�����f���ɔ��f����
	virtual void TransformAccordingAsBone( void ) ;
	// �{�[������������
	E3DBoneJoint * FindBoneAs( const wchar_t * pwszName ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �J�����I�u�W�F�N�g
//////////////////////////////////////////////////////////////////////////////

class	E3DViewPointJoint	: public	E3DModelJoint
{
public:
	// �\�z�֐�
	E3DViewPointJoint( void ) ;
	// ���Ŋ֐�
	virtual ~E3DViewPointJoint( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DViewPointJoint, E3DModelJoint )

protected:
	E3DVector		m_vViewAngle ;		// �����x�N�g��
	E3DVector		m_vViewPoint ;		// ���_
	double			m_rRevAngleZ ;		// ������]

public:
	// �p�����[�^���f
	virtual void RefreshJoint( void ) ;

public:
	// �����x�N�g�����擾
	const E3DVector & GetViewAngle( void ) const
		{
			return	m_vViewAngle ;
		}
	// ���_���擾
	const E3DVector & GetViewPoint( void ) const
		{
			return	m_vViewPoint ;
		}
	// z ����]�p�x���擾
	double GetRevolveZ( void ) const
		{
			return	m_rRevAngleZ ;
		}
	// �����_�ݒ�
	void SetTarget( const E3D_VECTOR & vTarget ) ;
	// �����x�N�g����ݒ�
	void SetViewAngle( const E3D_VECTOR & vViewAngle ) ;
	// ���_��ݒ�
	void SetViewPoint( const E3D_VECTOR & vViewPoint ) ;
	// ������]�p�x��ݒ� [deg]
	void SetRevolveZ( double rDegAngle ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �J�����⊮�Ȑ�
//////////////////////////////////////////////////////////////////////////////

class	E3DViewAngleCurve	: public	ESLObject
{
public:
	// �\�z�֐�
	E3DViewAngleCurve( void ) ;
	// ���Ŋ֐�
	virtual ~E3DViewAngleCurve( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DViewAngleCurve, ESLObject )

public:
	// �⊮�t���O
	enum	CurveType
	{
		ctOnlyAngle		= 0x0001
	} ;

protected:
	E3DVector	m_vTarget[2] ;
	E3DVector	m_vViewPoint[2] ;
	double		m_rRevAngleZ[2] ;

public:
	// �J�����A���O���ݒ�
	void SetViewPoint
		( int nIndex, const E3D_VECTOR & vViewPoint,
			const E3D_VECTOR & vTarget, double rRevAngleZ = 0.0 ) ;
	// �J�����A���O���擾
	void GetViewPoint
		( double t, E3D_VECTOR & vViewPoint,
			E3D_VECTOR & vViewAngle, double & rRevAngleZ, DWORD dwFlags = 0 ) ;
	// �J�����A���O���ݒ�
	void SetViewCurveFor
		( E3DViewPointJoint & vpjoint, double t, DWORD dwFlags = 0 ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �����_�����O�I�u�W�F�N�g
//////////////////////////////////////////////////////////////////////////////

class	E3DRenderPolygon	: public	ESLObject
{
public:
	// �\�z�֐�
	E3DRenderPolygon( void ) ;
	// ���Ŋ֐�
	virtual ~E3DRenderPolygon( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( E3DRenderPolygon, ESLObject )

protected:
	// �X���b�h���b�Z�[�W
	enum	RenderThreadMessage
	{
		rtmQuit		= WM_QUIT,
		rtmRender	= WM_USER + 1
	} ;

public:
	// �񋓌^
	enum	MultiRenderingFlag
	{
		mrfAuto,
		mrfSingle,
		mrfDual
	} ;

protected:
	HEGL_RENDER_POLYGON		m_hRenderPoly ;		// �僌���_�����O�I�u�W�F�N�g
	DWORD					m_dwSortingFlags ;	// �\�[�g�t���O

	HESLHEAP				m_hLocalHeap ;		// ���[�J���q�[�v
	HSTACKHEAP				m_hStackHeap ;		// �X�^�b�N���q�[�v

	DWORD					m_dwPolyCount ;		// �|���S���G���g����
	DWORD					m_dwPolyLimit ;		// �|���S���G���g�����E��
	PE3D_POLYGON_ENTRY *	m_pPolyEntries ;	// �|���S���G���g��

	E3DViewPointJoint		m_vpjView ;			// �J�����I�u�W�F�N�g

	PEGL_IMAGE_INFO			m_pDstImage ;		// �����_�����O���
	PEGL_IMAGE_INFO			m_pZBuffer ;
	EGL_RECT				m_rectDst ;
	E3D_VECTOR				m_vScreenPos ;

	MultiRenderingFlag		m_mrfRenderingFlag ;
	HEGL_RENDER_POLYGON		m_hPrimaryRender ;
	HEGL_RENDER_POLYGON		m_hSecondaryRender ;

	HANDLE					m_hThread ;			// �����_�����O�p�X���b�h
	DWORD					m_dwThreadID ;
	HANDLE					m_hRenderEvent ;	// �����_�����O�����ʒm

public:
	// ������
	virtual ESLError Initialize
		( PEGL_IMAGE_INFO pDstImage, PCEGL_RECT pClipRect,
			PEGL_IMAGE_INFO pZBuffer, PCE3D_VECTOR pScreenPos,
			DWORD dwHeapSize = 0x100000, DWORD dwPolyLimit = 0x8000,
			MultiRenderingFlag mrfFlag = mrfAuto ) ;
	// ���\�[�X�J��
	virtual ESLError Release( void ) ;
	// �}���`�v���Z�b�T�Ή��擾
	MultiRenderingFlag GetMultiRenderingFlag( void ) const
		{
			return	m_mrfRenderingFlag ;
		}

public:
	// ���f���ǉ�
	virtual ESLError AddModel
		( E3DModelJoint & model,
			const E3D_COLOR * pColor = NULL,
			unsigned int nTransparency = 0 ) ;
	// �v���~�e�B�u�ǉ�
	virtual ESLError AddPrimitive
		( const E3D_PRIMITIVE_POLYGON * pPrimitive,
			const E3D_COLOR * pColor = NULL,
			unsigned int nTransparency = 0 ) ;
	// �����_�����O����
	virtual ESLError PrepareRendering( void ) ;
	// �����_�����O���s
	virtual ESLError
		RenderAllPolygon( HEGL_RENDER_POLYGON hRenderPoly = NULL ) ;
	// �|���S��������
	virtual ESLError FlushAllPolygon( void ) ;

protected:
	// �����_�����O�X���b�h
	static DWORD WINAPI RenderThreadProc( LPVOID param ) ;
	DWORD RenderingThread( void ) ;

public:
	// �J�����I�u�W�F�N�g�擾
	E3DViewPointJoint & ViewPoint( void )
		{
			return	m_vpjView ;
		}
	// �J�����ݒ�
	void SetViewPoint
		( const E3D_VECTOR & vViewPoint,
			const E3D_VECTOR & vTarget, double rDegAngle )
		{
			m_vpjView.SetViewPoint( vViewPoint ) ;
			m_vpjView.SetTarget( vTarget ) ;
			m_vpjView.SetRevolveZ( rDegAngle ) ;
			m_vpjView.RefreshJoint( ) ;
		}
	// �X�N���[�����W�擾
	const E3D_VECTOR & GetScreenPos( void ) const
		{
			return	m_vScreenPos ;
		}
	// �����_�����O�I�u�W�F�N�g�擾
	operator HEGL_RENDER_POLYGON ( void ) const
		{
			return	m_hRenderPoly ;
		}
	// �`��I�u�W�F�N�g�擾
	HEGL_DRAW_IMAGE GetDrawImage( void )
		{
			ESLAssert( m_hRenderPoly != NULL ) ;
			return	m_hRenderPoly->GetDrawImage( ) ;
		}
	// �@�\�t���O��ύX
	void ModifyFunctionFlags( DWORD dwAddFlags, DWORD dwRemoveFlags )
		{
			ESLAssert( m_hRenderPoly != NULL ) ;
			m_hRenderPoly->SetFunctionFlags
				( (m_hRenderPoly->GetFunctionFlags()
							& ~dwRemoveFlags) | dwAddFlags ) ;
		}
	// �@�\�t���O���擾
	DWORD GetFunctionFlags( void ) const
		{
			ESLAssert( m_hRenderPoly != NULL ) ;
			return	m_hRenderPoly->GetFunctionFlags( ) ;
		}
	// �@�\�t���O��ݒ�
	DWORD SetFunctionFlags( DWORD dwFlags )
		{
			ESLAssert( m_hRenderPoly != NULL ) ;
			return	m_hRenderPoly->SetFunctionFlags( dwFlags ) ;
		}
	// Z �N���b�v�l��ݒ�
	ESLError SetZClipRange( REAL32 rMin, REAL32 rMax )
		{
			ESLAssert( m_hRenderPoly != NULL ) ;
			return	m_hRenderPoly->SetZClipRange( rMin, rMax ) ;
		}
	// ���C�g��ݒ�
	ESLError SetLightEntries
			( unsigned int nLightCount, PCE3D_LIGHT_ENTRY pLightEntries ) ;
	// �ŏ��O�ڋ�`���擾
	ESLError GetExternalRect( EGL_RECT * pExtRect ) const
		{
			ESLAssert( m_hRenderPoly != NULL ) ;
			return	m_hRenderPoly->GetExternalRect
						( pExtRect, (PCE3D_POLYGON_ENTRY*)
										m_pPolyEntries, m_dwPolyCount ) ;
		}
	// �\�[�g�t���O���擾����
	DWORD GetSortingFlags( void ) const
		{
			return	m_dwSortingFlags ;
		}
	// �\�[�g�t���O��ݒ肷��
	void SetSortingFlags( DWORD dwSortingFlags )
		{
			m_dwSortingFlags = dwSortingFlags ;
		}

} ;


#endif	//	!defined(COMPACT_NOA_DECODER)

#endif
