
/*****************************************************************************
                  Adobe(R) Photoshop(R) �t�@�C�����[�_�[
 *****************************************************************************/

namespace	PSD
{

//////////////////////////////////////////////////////////////////////////////
// �\����
//////////////////////////////////////////////////////////////////////////////

#pragma	pack( push, __PSDFILE_ALIGN__, 1 )

enum	ImageMode
{
	modeBitmap,
	modeGrayscale,
	modeIndexed,
	modeRGB,
	modeCMYK,
	modeMultichannel,
	modeDuotone,
	modeLab
} ;

struct	FILE_HEADER
{
	DWORD	dwSignature ;			// must be "8BPS"
	WORD	wVersion ;				// must be 1
	BYTE	bytReserved[6] ;		// must be zero
	WORD	wChannels ;				// �`���l���� [1,24]
	DWORD	dwHeight ;				// ����
	DWORD	dwWidth ;				// ��
	WORD	wDepth ;				// �r�b�g�[�x {1,8,16}
	WORD	wMode ;					// �摜���[�h (ImageMode)
} ;

struct	COLOR_MODE_DATA				// modeIndexed, modeDuotone �ȊO�ł͂O
{
	DWORD	dwLength ;
	BYTE	bytData[1] ;
} ;

struct	IMAGE_RESOURCE_SECTION
{
	DWORD	dwLength ;
	BYTE	bytData[1] ;
} ;

enum	ChannelID
{
	chidRed = 0, chidGreen = 1, chidBlue = 2,
	chidTransparencyMask = -1, chidUserSuppliedMask = -2
} ;

struct	CHANNEL_LENGTH_INFO
{
	WORD	wChannelID ;		// enum ChannelID
	DWORD	dwLength ;			// �`���l���f�[�^��
} ;

struct	ADJUST_LAYER_DATA
{
	DWORD	dwSize ;
	DWORD	dwTop ;				// Rectangle enclosing layer mask
	DWORD	dwLeft ;
	DWORD	dwBottom ;
	DWORD	dwRight ;
	BYTE	bytDefColor ;		// 0 or 255
	BYTE	Flags ;				// bit 0 = position relative to layer
								// bit 1 = layer mask disabled
								// bit 2 = inver layer mask when blending
	WORD	wPadding ;			// Zeros
} ;

enum	LayerBlendMode
{
	blendNormal		= 'norm',
	blendDarken		= 'dark',
	blendLighten	= 'lite',
	blendHue		= 'hue ',
	blendSat		= 'sat ',
	blendColor		= 'colr',
	blendLuminosity	= 'lum ',
	blendMultiply	= 'mul ',
	blendDivision	= 'div ',
	blendScreen		= 'scrn',
	blendDissolve	= 'diss',
	blendOverlay	= 'over',
	blendHardLight	= 'hLit',
	blendSoftLight	= 'sLit',
	blendDifference	= 'diff'
} ;

enum	LayerFlag
{
	flagTransparencyProtected	= 0x01,
	flagInvisible				= 0x02
} ;

struct	LAYER_RECORD
{
	DWORD				dwTop ;				// ���C���[�̈�
	DWORD				dwLeft ;
	DWORD				dwBottom ;
	DWORD				dwRight ;
	WORD				wChannels ;			// �`���l����
	CHANNEL_LENGTH_INFO	chlen[24] ;			// �`���l����
	DWORD				dwSignature ;		// must be "8BIM"
	DWORD				dwBlendMode ;		// 'norm' etc...
	BYTE				bytOpacity ;		// 0:���� �` 255:�s����
	BYTE				bytClipping ;		// 0:base, 1:����ȊO
	BYTE				bytFlags ;			// bit0 : ���������̕ی�
											// bit1 : �s��
	BYTE				bytFilter ;			// must be zero
	DWORD				dwExtraSize ;		// ����ȍ~�̃o�C�g��
	ADJUST_LAYER_DATA	adjdata ;
	EString				strLayerName ;
} ;

#pragma	pack( pop, __PSDFILE_ALIGN__ )


//////////////////////////////////////////////////////////////////////////////
// �G���f�B�A���ϊ��֐�
//////////////////////////////////////////////////////////////////////////////

inline DWORD dwswap( DWORD dwData )
{
#if	defined(ERI_INTEL_X86)
	__asm
	{
		mov	eax, dwData
		bswap	eax
		mov	dwData, eax
	}
	return	dwData ;
#else
	return	(dwData >> 24) | ((dwData >> 8) & 0xFF00)
			| ((dwData << 8) & 0x00FF0000) | (dwData << 24) ;
#endif
}

inline WORD wswap( WORD wData )
{
	return	(wData << 8) | (wData >> 8) ;
}


//////////////////////////////////////////////////////////////////////////////
// PSD �t�@�C��
//////////////////////////////////////////////////////////////////////////////

class	File	: public ESLObject
{
protected:
	ESLFileObject *			m_pfile ;			// PSD �t�@�C��
	FILE_HEADER				m_fhHeader ;		// �t�@�C���w�b�_
	EStreamBuffer			m_bufColorMode ;	// Color Mode Data
	EObjArray<LAYER_RECORD>	m_lstLayer ;		// ���C���[���
	DWORD					m_dwLayerDataPos ;	// ���C���[�摜�̈ʒu
	DWORD					m_dwBaseDataPos ;	// �x�[�X�摜�̈ʒu

	EStreamBuffer				m_bufLayerInfo ;	// �����o���p���C���[���
	EObjArray<EStreamBuffer>	m_lstLayerData ;	// �����o���p���C���[�摜
	EStreamBuffer				m_bufBaseData ;		// �����o���p�x�[�X�摜

public:
	class	LayerInfo
	{
	public:
		EGL_IMAGE_RECT		irRect ;
		EGL_IMAGE_RECT		irMask ;
		int					nChannels ;
		CHANNEL_LENGTH_INFO	chlen[24] ;
		DWORD				dwBlendMode ;
		unsigned int		nTransparency ;
		DWORD				dwFlags ;
		EString				strLayerName ;
	} ;

public:
	// �\�z�֐�
	File( void ) ;
	// ���Ŋ֐�
	virtual ~File( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( PSD::File, ESLObject )

public:
	// �t�@�C�����J��
	ESLError Open( ESLFileObject & file ) ;
	// �t�@�C�������
	void Close( void ) ;

public:
	// �w�b�_�����擾����
	void GetFileHeader( FILE_HEADER & fhHeader ) const ;
	// �J���[���[�h�����擾����
	void GetColorModeData( EStreamBuffer bufColorMode ) const ;
	// ���C���[�̑������擾����
	unsigned int GetLayerCount( void ) const ;
	// ���C���[�����擾����
	ESLError GetLayerInfo
		( LayerInfo & liLayer, unsigned int iLayer ) const ;
	// ���C���[�摜���擾����
	ESLError LoadLayerImage
		( EGLImage & imgBuf, unsigned int iLayer ) ;
	// �x�[�X�摜���擾����
	ESLError LoadBaseImage( EGLImage & imgBuf ) ;

public:
	// RLE �̓W�J
	static void UnpackBits
		( BYTE * ptrBuf, int nBufSize,
			const BYTE * ptrRLE, int nRLESize ) ;

public:
	// PSD �t�@�C�������o���̏������n�߂�
	ESLError PrepareToWrite
		( ESLFileObject & file, PCEGL_IMAGE_INFO pBase ) ;
	// ���C���[��ǉ�����
	ESLError AddLayerImage
		( PCEGL_IMAGE_INFO pLayer,
			const char * pszName = NULL,
			int xPos = 0, int yPos = 0,
			unsigned int nTransparency = 0,
			DWORD dwBlendMode = blendNormal ) ;
	// �t�@�C���ւ̏����o�����I������
	ESLError FinishToWrite( void ) ;

public:
	// ���C���[�摜�����k����
	static ESLError PackLayerImage
		( EStreamBuffer & bufImage,
			LAYER_RECORD & lrLayer, PCEGL_IMAGE_INFO pLayer ) ;
	// �x�[�X�摜�����k����
	static ESLError PackBaseImage
		( EStreamBuffer & bufImage, PCEGL_IMAGE_INFO pBase ) ;
	// RLE �̈��k
	static int PackBits
		( BYTE * ptrRLE, int nRLESize,
			const BYTE * ptrBuf, int nBufSize ) ;

} ;

} ;

