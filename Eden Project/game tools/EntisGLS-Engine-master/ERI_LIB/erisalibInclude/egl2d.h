
/*****************************************************************************
                          Entis Graphic Library
 -----------------------------------------------------------------------------
     Copyright (c) 2002-2004 Leshade Entis, Entis-soft. Al rights reserved.
 *****************************************************************************/


#if	!defined(__EGL2D_H__)
#define	__EGL2D_H__

//
// �p���b�g���p��
//
union	EGL_PALETTE
{
	DWORD	dwPixelCode ;			// �p���b�g�ԍ��^�s�N�Z���R�[�h
	REAL32	rZOrder32 ;				// Z �l
	struct
	{
		BYTE	Blue ;
		BYTE	Green ;
		BYTE	Red ;
		BYTE	Reserved ;
	}		rgb ;					// RGB24 �t�H�[�}�b�g
	struct
	{
		BYTE	Blue ;
		BYTE	Green ;
		BYTE	Red ;
		BYTE	Alpha ;
	}		rgba ;					// RGBA32 �t�H�[�}�b�g
	struct
	{
		BYTE	Y ;
		BYTE	U ;
		BYTE	V ;
		BYTE	Reserved ;
	}		yuv ;					// YUV24 �t�H�[�}�b�g
	struct
	{
		BYTE	Brightness ;
		BYTE	Saturation ;
		BYTE	Hue ;
		BYTE	Reserved ;
	}		hsb ;					// HSB24 �t�H�[�}�b�g
} ;

class	EGLPalette
{
public:
	EGL_PALETTE	m_palette ;
public:
	EGLPalette( void ) { }
	EGLPalette( const EGL_PALETTE & palette )
		{ m_palette = palette ; }
	EGLPalette( DWORD dwRGBA )
		{ m_palette.dwPixelCode = dwRGBA ; }
	operator EGL_PALETTE ( void ) const
		{	return	m_palette ;	}
	operator EGL_PALETTE & ( void )
		{	return	m_palette ;	}
	operator const EGL_PALETTE * ( void ) const
		{	return	&m_palette ;	}
	operator EGL_PALETTE * ( void )
		{	return	&m_palette ;	}
	const EGLPalette & operator = ( const EGL_PALETTE & palette )
		{
			m_palette = palette ;
			return	*this ;
		}
	const EGLPalette & operator += ( const EGL_PALETTE & rgbColor )
		{
			DWORD	aria = (m_palette.dwPixelCode & 0x00FF00FF)
							+ (rgbColor.dwPixelCode & 0x00FF00FF) ;
			DWORD	maria = ((m_palette.dwPixelCode >> 8) & 0x00FF00FF)
							+ ((rgbColor.dwPixelCode >> 8) & 0x00FF00FF) ;
			DWORD	arisa = (((0x00FF00FF - aria) & 0xFF00FF00) >> 8)
								| ((0x00FF00FF - maria) & 0xFF00FF00) ;
			m_palette.dwPixelCode =
				(aria & 0x00FF00FF) | ((maria & 0x00FF00FF) << 8) | arisa ;
			return	*this ;
		}
	const EGLPalette & operator *= ( unsigned int x )
		{
			if ( x >= 0x100 )
			{
				x = 0x100 ;
			}
			DWORD	aria = (m_palette.dwPixelCode & 0x00FF00FF) * x ;
			DWORD	maria = ((m_palette.dwPixelCode >> 8) & 0x00FF00FF) * x ;
			m_palette.dwPixelCode =
				((aria & 0xFF00FF00) >> 8) | (maria & 0xFF00FF00) ;
			return	*this ;
		}
	EGLPalette operator + ( const EGL_PALETTE & rgbColor ) const
		{
			EGLPalette	t = *this ;
			t += rgbColor ;
			return	t ;
		}
	EGLPalette operator * ( unsigned int x ) const
		{
			EGLPalette	t = *this ;
			t *= x ;
			return	t ;
		}
} ;

typedef	EGL_PALETTE *	PEGL_PALETTE ;
typedef	const EGL_PALETTE *	PCEGL_PALETTE ;

//
// �摜���\����
//
struct	EGL_IMAGE_INFO
{
	DWORD				dwInfoSize ;
	DWORD				fdwFormatType ;
	DWORD				ptrOffsetPixel ;
	void *				ptrImageArray ;
	PEGL_PALETTE		pPaletteEntries ;
	DWORD				dwPaletteCount ;
	DWORD				dwImageWidth ;
	DWORD				dwImageHeight ;
	DWORD				dwBitsPerPixel ;
	SDWORD				dwBytesPerLine ;
	SDWORD				dwSizeOfImage ;
	DWORD				dwClippedPixel ;
} ;

typedef	EGL_IMAGE_INFO *	PEGL_IMAGE_INFO ;
typedef	const EGL_IMAGE_INFO *	PCEGL_IMAGE_INFO ;

#define	EIF_RGB_BITMAP		0x00000001
#define	EIF_RGBA_BITMAP		0x04000001
#define	EIF_GRAY_BITMAP		0x00000002
#define	EIF_YUV_BITMAP		0x00000004
#define	EIF_HSB_BITMAP		0x00000006
#define	EIF_Z_BUFFER_R4		0x00002005
#define	EIF_TYPE_MASK		0x00FFFFFF
#define	EIF_WITH_PALETTE	0x01000000
#define	EIF_WITH_CLIPPING	0x02000000
#define	EIF_WITH_ALPHA		0x04000000

//
// ���W�E�T�C�Y�E��`
//
struct	EGL_POINT
{
	SDWORD	x ;
	SDWORD	y ;
} ;
typedef	EGL_POINT *	PEGL_POINT ;
typedef	const EGL_POINT *	PCEGL_POINT ;

class	EGLPoint	: public	EGL_POINT
{
public:
	EGLPoint( void ) { }
	EGLPoint( SDWORD xPos, SDWORD yPos ) { x = xPos ;  y = yPos ; }
	EGLPoint( const EGL_POINT & point ) { x = point.x ;  y = point.y ; }
	const EGLPoint & operator = ( const EGL_POINT & point )
		{
			x = point.x ;
			y = point.y ;
			return	*this ;
		}
	bool operator == ( const EGL_POINT & point ) const
		{
			return	(x == point.x) && (y == point.y) ;
		}
	bool operator != ( const EGL_POINT & point ) const
		{
			return	(x != point.x) || (y != point.y) ;
		}
	const EGLPoint & operator += ( const EGL_POINT & point )
		{
			x += point.x ;
			y += point.y ;
			return	*this ;
		}
	const EGLPoint & operator -= ( const EGL_POINT & point )
		{
			x -= point.x ;
			y -= point.y ;
			return	*this ;
		}
	EGLPoint operator + ( const EGL_POINT & point ) const
		{
			EGLPoint	ptResult = *this ;
			ptResult += point ;
			return	ptResult ;
		}
	EGLPoint operator - ( const EGL_POINT & point ) const
		{
			EGLPoint	ptResult = *this ;
			ptResult -= point ;
			return	ptResult ;
		}
} ;

struct	EGL_SIZE
{
	SDWORD	w ;
	SDWORD	h ;
} ;
typedef	EGL_SIZE *	PEGL_SIZE ;
typedef	const EGL_SIZE *	PCEGL_SIZE ;

class	EGLSize	: public	EGL_SIZE
{
public:
	EGLSize( void ) { }
	EGLSize( SDWORD width, SDWORD height ) { w = width ;  h = height ; }
	EGLSize( const EGL_SIZE & size ) { w = size.w ;  h = size.h ; }
	const EGLSize & operator = ( const EGL_SIZE & size )
		{
			w = size.w ;
			h = size.h ;
			return	*this ;
		}
	bool operator == ( const EGL_SIZE & size ) const
		{
			return	(w == size.w) && (h == size.h) ;
		}
	bool operator != ( const EGL_SIZE & size ) const
		{
			return	(w != size.w) || (h != size.h) ;
		}
	const EGLSize & operator += ( const EGL_SIZE & size )
		{
			w += size.w ;
			h += size.h ;
			return	*this ;
		}
	const EGLSize & operator -= ( const EGL_SIZE & size )
		{
			w -= size.w ;
			h -= size.h ;
			return	*this ;
		}
	EGLSize operator + ( const EGL_SIZE & size ) const
		{
			EGLSize	sizeResult = *this ;
			sizeResult += size ;
			return	sizeResult ;
		}
	EGLSize operator - ( const EGL_SIZE & size ) const
		{
			EGLSize	sizeResult = *this ;
			sizeResult -= size ;
			return	sizeResult ;
		}
} ;

struct	EGL_RECT
{
	SDWORD	left ;
	SDWORD	top ;
	SDWORD	right ;
	SDWORD	bottom ;
} ;
typedef	EGL_RECT *	PEGL_RECT ;
typedef	const EGL_RECT *	PCEGL_RECT ;

struct	EGL_IMAGE_RECT
{
	SDWORD	x ;
	SDWORD	y ;
	SDWORD	w ;
	SDWORD	h ;
} ;
typedef	EGL_IMAGE_RECT *	PEGL_IMAGE_RECT ;
typedef	const EGL_IMAGE_RECT *	PCEGL_IMAGE_RECT ;

class	EGLRect	: public	EGL_RECT
{
public:
	EGLRect( void ) { }
	EGLRect( SDWORD x1, SDWORD y1, SDWORD x2, SDWORD y2 )
		{
			left = x1 ;
			top = y1 ;
			right = x2 ;
			bottom = y2 ;
		}
	EGLRect( const EGL_RECT & rect )
		{
			left = rect.left ;
			top = rect.top ;
			right = rect.right ;
			bottom = rect.bottom ;
		}
	EGLRect( const struct EGL_IMAGE_RECT & rect )
		{
			left = rect.x ;
			top = rect.y ;
			right = rect.x + rect.w - 1 ;
			bottom = rect.y + rect.h - 1 ;
		}
	const EGLRect & operator = ( const EGL_RECT & rect )
		{
			left = rect.left ;
			top = rect.top ;
			right = rect.right ;
			bottom = rect.bottom ;
			return	*this ;
		}
	const EGLRect & operator = ( const struct EGL_IMAGE_RECT & rect )
		{
			left = rect.x ;
			top = rect.y ;
			right = rect.x + rect.w - 1 ;
			bottom = rect.y + rect.h - 1 ;
			return	*this ;
		}
	bool operator &= ( const EGL_RECT & rect )
		{
			if ( (left > rect.right) || (top > rect.bottom)
					|| (right < rect.left) || (bottom < rect.top) )
			{
				left = top = 0 ;
				right = bottom = -1 ;
				return	false ;
			}
			else
			{
				if ( left < rect.left )
					left = rect.left ;
				if ( top < rect.top )
					top = rect.top ;
				if ( right > rect.right )
					right = rect.right ;
				if ( bottom > rect.bottom )
					bottom = rect.bottom ;
				return	true ;
			}
		}
	int operator == (const EGL_RECT & rect ) const
		{
			return	(left == rect.left) & (top == rect.top)
				& (right == rect.right) & (bottom == rect.bottom) ;
		}
	int operator != (const EGL_RECT & rect ) const
		{
			return	(left != rect.left) | (top != rect.top)
				| (right != rect.right) | (bottom != rect.bottom) ;
		}
} ;

class	EGLImageRect	: public	EGL_IMAGE_RECT
{
public:
	EGLImageRect( void ) { }
	EGLImageRect( SDWORD xPos, SDWORD yPos, SDWORD nWidth, SDWORD nHeight )
		{
			x = xPos ;
			y = yPos ;
			w = nWidth ;
			h = nHeight ;
		}
	EGLImageRect( const EGL_IMAGE_RECT & rect )
		{
			x = rect.x ;
			y = rect.y ;
			w = rect.w ;
			h = rect.h ;
		}
	EGLImageRect( const EGL_RECT & rect )
		{
			x = rect.left ;
			y = rect.top ;
			w = rect.right - rect.left + 1 ;
			h = rect.bottom - rect.top + 1 ;
		}
	const EGLImageRect & operator = ( const EGL_IMAGE_RECT & rect )
		{
			x = rect.x ;
			y = rect.y ;
			w = rect.w ;
			h = rect.h ;
			return	*this ;
		}
	const EGLImageRect & operator = ( const EGL_RECT & rect )
		{
			x = rect.left ;
			y = rect.top ;
			w = rect.right - rect.left + 1 ;
			h = rect.bottom - rect.top + 1 ;
			return	*this ;
		}
} ;

struct	EGL_IMAGE_AXES
{
	struct
	{
		REAL32	x ;
		REAL32	y ;
	}			xAxis ;
	struct
	{
		REAL32	x ;
		REAL32	y ;
	}			yAxis ;
} ;
typedef	EGL_IMAGE_AXES *	PEGL_IMAGE_AXES ;
typedef	const EGL_IMAGE_AXES *	PCEGL_IMAGE_AXES ;

//
// �V�F�[�f�B���O�F���
//
struct	E3D_COLOR
{
	EGL_PALETTE	rgbMul ;
	EGL_PALETTE	rgbAdd ;
} ;
typedef	E3D_COLOR *	PE3D_COLOR ;
typedef	const E3D_COLOR *	PCE3D_COLOR ;

//
// ���[�W����
//
struct	E3D_POLY_LINE_REGION
{
	SDWORD		nLeft ;
	DWORD		dwReserved1 ;
	SDWORD		nRight ;
	DWORD		dwReserved2 ;
	E3D_COLOR	rgbaLeft ;
	E3D_COLOR	rgbaRight ;
} ;

struct	E3D_POLYGON_REGION
{
	SDWORD					nTopLine ;
	SDWORD					nBottomLine ;
	E3D_POLY_LINE_REGION	plrLineRgn[1] ;
} ;
typedef	E3D_POLYGON_REGION *	PE3D_POLYGON_REGION ;

//
// �摜�o�b�t�@�Ǘ�
//
extern	"C"
{
	PEGL_IMAGE_INFO eglCreateImageBuffer
		( DWORD fdwFormat,
			DWORD dwWidth, DWORD dwHeight,
			DWORD dwBitsPerPixel, DWORD dwFlags = 0 ) ;
	PEGL_IMAGE_INFO eglCreateTextureInfo
		( PCEGL_IMAGE_INFO pImageInf,
			PCEGL_RECT pClipRect = NULL, DWORD dwFlags = 0 ) ;
	PEGL_IMAGE_INFO eglDuplicateImageBuffer
		( PCEGL_IMAGE_INFO pImageInf, DWORD dwFlags = 0 ) ;
	ESLError eglAddImageBufferRef( PEGL_IMAGE_INFO pImageInf ) ;
	ESLError eglDeleteImageBuffer( PEGL_IMAGE_INFO pImageInf ) ;
	ESLError eglDrawToDC
		( HDC hDstDC, PCEGL_IMAGE_INFO pImageInf,
			int nPosX, int nPosY,
			PCEGL_SIZE pSizeToDraw, PCEGL_RECT pViewRect ) ;
	HDC eglGetDC( PCEGL_IMAGE_INFO pImageInf ) ;
	ESLError eglFillImage
		( PEGL_IMAGE_INFO pImageInf, EGL_PALETTE colorFill ) ;
	ESLError eglGetClippedImageInfo
		( PEGL_IMAGE_INFO pClippedImage,
			PCEGL_IMAGE_INFO pOriginalImage,
			PCEGL_IMAGE_RECT pClippingRect ) ;
	PEGL_IMAGE_RECT eglGetOverlappedRectangle
		( PEGL_IMAGE_RECT pImageRect,
			PCEGL_RECT pDstViewRect,
			PCEGL_RECT pSrcViewRect,
			PCEGL_POINT pSrcViewOffset,
			PCEGL_SIZE pSrcViewMaxSize = NULL ) ;
	ESLError eglGetRevolvedAxes
		( PEGL_IMAGE_AXES pImageAxes,
			PEGL_POINT pBasePosition,
			PCEGL_POINT pCenterOffset,
			REAL32 rHorizontalRate = 1,
			REAL32 rVerticalRate = 1,
			REAL32 rRevolutionAngle = 0,
			REAL32 rCrossingAngle = 90,
			unsigned int nFlag = 0 ) ;
	ESLError eglReverseVertically( PEGL_IMAGE_INFO pImageInf ) ;
	EGL_PALETTE eglGetPixel
		( PCEGL_IMAGE_INFO pImageInf, int nPosX, int nPosY ) ;
	ESLError eglSetPixel
		( PEGL_IMAGE_INFO pImageInf,
			int nPosX, int nPosY, EGL_PALETTE colorPixel ) ;
} ;

#define	EGL_DRAW_RADIAN		0x0001

#define	EGL_IMAGE_HAS_DC	0x0001
#define	EGL_IMAGE_NO_DUP	0x0010


//
// �J���[�R�[�h�ϊ�
//
extern	"C"
{
	void eglRGBtoYUV( EGL_PALETTE * pColor ) ;
	void eglYUVtoRGB( EGL_PALETTE * pColor ) ;
	void eglRGBtoHSB( EGL_PALETTE * pColor ) ;
	void eglHSBtoRGB( EGL_PALETTE * pColor ) ;
	DWORD eglBlendRGBA( EGL_PALETTE rgba1, EGL_PALETTE rgba2 ) ;
} ;

//
// �t�B���^�����O�֐�
//
extern	"C"
{
	ESLError eglConvertFormat
		( PEGL_IMAGE_INFO pDstImage,
			PCEGL_IMAGE_INFO pSrcImage, DWORD dwFlags = 0 ) ;
	ESLError eglCalculateToneTable
		( void * pToneBuf, signed int nTone, int nFlag = 0 ) ;
	ESLError eglApplyToneTable
		( PEGL_IMAGE_INFO pDstImage, PCEGL_IMAGE_INFO pSrcImage,
			const void * pBlueTone, const void * pGreenTone,
			const void * pRedTone, const void * pAlphaTone ) ;
	ESLError eglSetColorTone
		( PEGL_IMAGE_INFO pDstImage, PCEGL_IMAGE_INFO pSrcImage,
			int nBlueTone, int nGreenTone, int nRedTone, int nAlphaTone ) ;
	ESLError eglEnlargeDouble
		( PEGL_IMAGE_INFO pDstImage,
			PCEGL_IMAGE_INFO pSrcImage, DWORD dwFlags = 0 ) ;
	ESLError eglBlendAlphaChannel
		( PEGL_IMAGE_INFO pRGBA32,
			PCEGL_IMAGE_INFO pSrcRGB,
			PCEGL_IMAGE_INFO pSrcAlpha,
			DWORD dwFlags = 0,
			SDWORD nAlphaBase = 0,
			DWORD nCoefficient = 0x10 ) ;
	ESLError eglUnpackAlphaChannel
		( PEGL_IMAGE_INFO pDstRGB, PEGL_IMAGE_INFO pDstAlpha,
			PCEGL_IMAGE_INFO pSrcRGBA32, DWORD dwFlags = 0 ) ;
} ;

#define	EGL_TONE_BRIGHTNESS		0x0000
#define	EGL_TONE_INVERSION		0x0001
#define	EGL_TONE_LIGHT			0x0002

#define	EGL_BAC_MULTIPLY		0x0001
#define	EGL_BAC_ADD_ALPHA		0x0002
#define	EGL_BAC_MULTIPLY_ALPHA	0x0004

//
// �摜�`��
//
struct	EGL_DRAW_DEST
{
	PEGL_IMAGE_INFO	pDstImage ;
	EGL_RECT		rectDstClip ;
	PEGL_IMAGE_INFO	pZBuffer ;
} ;

struct	EGL_DRAW_PARAM
{
	DWORD				dwFlags ;
	EGL_POINT			ptBasePos ;
	PEGL_IMAGE_INFO		pSrcImage ;
	PCEGL_RECT			pViewRect ;
	EGL_PALETTE			rgbDimColor ;
	EGL_PALETTE			rgbLightColor ;
	unsigned int		nTransparency ;
	REAL32				rZOrder ;
	PCEGL_IMAGE_AXES	pImageAxes ;
	EGL_PALETTE			rgbColorParam1 ;
} ;

#define	EGL_DRAW_BLEND_ALPHA	0x0001
#define	EGL_DRAW_GLOW_LIGHT		0x0002
#define	EGL_WITH_Z_ORDER		0x0004
#define	EGL_SMOOTH_STRETCH		0x0010
#define	EGL_UNSMOOTH_STRETCH	0x0020
#define	EGL_FIXED_POSITION		0x0040

inline DWORD EGL_DRAW_APPLY( int x )
{
	return	(x & 0x7F) << 16 ;
}
inline DWORD EGL_DRAW_FUNC( int x )
{
	return	(x & 0x7F) << 24 ;
}

#define	EGL_APPLY_C_ADD		0x00800000
#define	EGL_APPLY_C_MUL		0x00820000
#define	EGL_APPLY_A_MUL		0x00880000
#define	EGL_APPLY_C_MASK	0x00890000
#define	EGL_DRAW_F_ADD		0x80000000
#define	EGL_DRAW_F_SUB		0x81000000
#define	EGL_DRAW_F_MUL		0x82000000
#define	EGL_DRAW_F_DIV		0x83000000
#define	EGL_DRAW_A_MOVE		0x88000000
#define	EGL_DRAW_A_MUL		0x89000000

#define	EGL_FILL_INVERSION	0x00000100

typedef	struct EGL_DRAW_IMAGE *	HEGL_DRAW_IMAGE ;

struct	EGL_DRAW_IMAGE
{
	ESLError (*pfnRelease)( HEGL_DRAW_IMAGE hDrawImage ) ;
	ESLError (*pfnInitialize)
		( HEGL_DRAW_IMAGE hDrawImage, PEGL_IMAGE_INFO pDstImage,
					PCEGL_RECT pClipRect, PEGL_IMAGE_INFO pZBuffer ) ;
	ESLError (*pfnGetDestination)
		( HEGL_DRAW_IMAGE hDrawImage, EGL_DRAW_DEST * pDrawDest ) ;
	DWORD (*pfnGetFunctionFlags)( HEGL_DRAW_IMAGE hDrawImage ) ;
	DWORD (*pfnSetFunctionFlags)
		( HEGL_DRAW_IMAGE hDrawImage, DWORD dwFlags ) ;
	ESLError (*pfnPrepareDraw)
		( HEGL_DRAW_IMAGE hDrawImage, const EGL_DRAW_PARAM * pDrawParam ) ;
	ESLError (*pfnDrawImage)( HEGL_DRAW_IMAGE hDrawImage ) ;
	void *	ptrReserved ;
	ESLError (*pfnPrepareLine)
		( HEGL_DRAW_IMAGE hDrawImage,
			int x1, int y1, int x2, int y2,
			EGL_PALETTE colorDraw,
			unsigned int nTranparency, DWORD dwFlags ) ;
	ESLError (*pfnPrepareFillRect)
		( HEGL_DRAW_IMAGE hDrawImage,
			PCEGL_RECT pDrawRect, EGL_PALETTE colorDraw,
			unsigned int nTranparency, DWORD dwFlags ) ;
	ESLError (*pfnPrepareFillEllipse)
		( HEGL_DRAW_IMAGE hDrawImage,
			PCEGL_POINT pCenter, PCEGL_SIZE pRadius,
			EGL_PALETTE colorDraw,
			unsigned int nTranparency, DWORD dwFlags ) ;
	ESLError (*pfnPrepareFillPolygon)
		( HEGL_DRAW_IMAGE hDrawImage,
			PCEGL_POINT pVertexes, unsigned int nCount,
			EGL_PALETTE colorDraw,
			unsigned int nTranparency, DWORD dwFlags ) ;
	ESLError (*pfnFillRegion)( HEGL_DRAW_IMAGE hDrawImage ) ;
	ESLError (*pfnDrawRegion)( HEGL_DRAW_IMAGE hDrawImage ) ;

	// �摜�`��I�u�W�F�N�g���
	inline ESLError Release( void )
		{
			return	(*pfnRelease)( this ) ;
		}
	// �摜�`��I�u�W�F�N�g������
	inline ESLError Initialize
		( PEGL_IMAGE_INFO pDstImage,
			PCEGL_RECT pClipRect, PEGL_IMAGE_INFO pZBuffer )
		{
			return	(*pfnInitialize)( this, pDstImage, pClipRect, pZBuffer ) ;
		}
	// �摜�`���擾
	inline ESLError GetDestination( EGL_DRAW_DEST * pDrawDest )
		{
			return	(*pfnGetDestination)( this, pDrawDest ) ;
		}
	// �`��@�\�t���O�擾
	inline DWORD GetFunctionFlags( void )
		{
			return	(*pfnGetFunctionFlags)( this ) ;
		}
	// �`��@�\�t���O�ݒ�
	inline DWORD SetFunctionFlags( DWORD dwFlags )
		{
			return	(*pfnSetFunctionFlags)( this, dwFlags ) ;
		}
	// �摜�`�揀��
	inline ESLError PrepareDraw( const EGL_DRAW_PARAM * pDrawParam )
		{
			return	(*pfnPrepareDraw)( this, pDrawParam ) ;
		}
	// �摜�`��
	inline ESLError DrawImage( void )
		{
			return	(*pfnDrawImage)( this ) ;
		}
	// �����`�揀��
	inline ESLError PrepareLine
		( int x1, int y1, int x2, int y2,
			EGL_PALETTE colorDraw,
			unsigned int nTransparency, DWORD dwFlags = 0 )
		{
			return	(*pfnPrepareLine)
				( this, x1, y1, x2, y2, colorDraw, nTransparency, dwFlags ) ;
		}
	// ��`�`�揀��
	inline ESLError PrepareFillRect
		( PCEGL_RECT pDrawRect, EGL_PALETTE colorDraw,
			unsigned int nTransparency, DWORD dwFlags = 0 )
		{
			return	(*pfnPrepareFillRect)
				( this, pDrawRect, colorDraw, nTransparency, dwFlags ) ;
		}
	// �ȉ~�`�揀��
	inline ESLError PrepareFillEllipse
		( PCEGL_POINT pCenter, PCEGL_SIZE pRadius,
			EGL_PALETTE colorDraw,
			unsigned int nTransparency, DWORD dwFlags = 0 )
		{
			return	(*pfnPrepareFillEllipse)
				( this, pCenter, pRadius, colorDraw, nTransparency, dwFlags ) ;
		}
	// ���p�`�`�揀��
	inline ESLError PrepareFillPolygon
		( PCEGL_POINT pVertexes, unsigned int nCount,
			EGL_PALETTE colorDraw,
			unsigned int nTransparency, DWORD dwFlags = 0 )
		{
			return	(*pfnPrepareFillPolygon)
				( this, pVertexes, nCount, colorDraw, nTransparency, dwFlags ) ;
		}
	// �}�`�`��
	inline ESLError FillRegion( void )
		{
			return	(*pfnFillRegion)( this ) ;
		}
	// �}�`�֊s�`��
	inline ESLError DrawRegion( void )
		{
			return	(*pfnDrawRegion)( this ) ;
		}

} ;

extern	"C"
{
	HEGL_DRAW_IMAGE eglCreateDrawImage( void ) ;
} ;

//
// 2�����x�N�g��
//
struct	E3D_VECTOR_2D
{
public:
	REAL32	x ;
	REAL32	y ;
public:
	bool operator == ( const E3D_VECTOR_2D & vector ) const
		{
			return	(x == vector.x) && (y == vector.y) ;
		}
	bool operator != ( const E3D_VECTOR_2D & vector ) const
		{
			return	(x != vector.x) && (y != vector.y) ;
		}
	const E3D_VECTOR_2D & operator += ( const E3D_VECTOR_2D & vector )
		{
			x += vector.x ;
			y += vector.y ;
			return	*this ;
		}
	const E3D_VECTOR_2D & operator -= ( const E3D_VECTOR_2D & vector )
		{
			x -= vector.x ;
			y -= vector.y ;
			return	*this ;
		}
	const E3D_VECTOR_2D & operator *= ( double num )
		{
			x *= (REAL32) num ;
			y *= (REAL32) num ;
			return	*this ;
		}
	E3D_VECTOR_2D operator + ( const E3D_VECTOR_2D & vector ) const
		{
			E3D_VECTOR_2D	result = *this ;
			result += vector ;
			return	result ;
		}
	E3D_VECTOR_2D operator - ( void ) const
		{
			E3D_VECTOR_2D	result = { - x, - y } ;
			return	result ;
		}
	E3D_VECTOR_2D operator - ( const E3D_VECTOR_2D & vector ) const
		{
			E3D_VECTOR_2D	result = *this ;
			result -= vector ;
			return	result ;
		}
	E3D_VECTOR_2D operator * ( double num ) const
		{
			E3D_VECTOR_2D	result ;
			result.x = (REAL32) (x * num) ;
			result.y = (REAL32) (y * num) ;
			return	result ;
		}
	E3D_VECTOR_2D operator * ( const E3D_VECTOR_2D & vector ) const
		{
			E3D_VECTOR_2D	result ;
			result.x = x * vector.x - y * vector.y ;
			result.y = x * vector.y + y * vector.x ;
			return	result ;
		}
	E3D_VECTOR_2D operator / ( const E3D_VECTOR_2D & vector ) const
		{
			E3D_VECTOR_2D	result ;
			REAL32	temp_num
				= vector.x * vector.x + vector.y * vector.y ;
			result.x = (x * vector.x + y * vector.y) / temp_num ;
			result.y = (y * vector.x - y * vector.x) / temp_num ;
			return	result ;
		}
	const E3D_VECTOR_2D & operator *= ( const E3D_VECTOR_2D & vector )
		{
			return	(*this = *this * vector) ;
		}
	const E3D_VECTOR_2D & operator /= ( const E3D_VECTOR_2D & vector )
		{
			return	(*this = *this / vector) ;
		}
} ;

class	E3DVector2D	: public	E3D_VECTOR_2D
{
public:
	E3DVector2D( void ) { }
	E3DVector2D( double vx, double vy ) { x = (REAL32) vx;  y = (REAL32) vy ; }
	E3DVector2D( const E3D_VECTOR_2D & v ) { x = v.x ;  y = v.y ; }
	E3DVector2D( const EGL_POINT & point )
		{ x = (REAL32) point.x ;  y = (REAL32) point.y ; }
	const E3DVector2D & operator = ( const E3D_VECTOR_2D & vector )
		{
			x = vector.x ;
			y = vector.y ;
			return	*this ;
		}

} ;

typedef	E3D_VECTOR_2D *			PE3D_VECTOR_2D ;
typedef	const E3D_VECTOR_2D *	PCE3D_VECTOR_2D ;

//
// 3�����x�N�g��
//
struct	E3D_VECTOR
{
public:
	REAL32	x ;
	REAL32	y ;
	REAL32	z ;
public:
	bool operator == ( const E3D_VECTOR & vector ) const
		{
			return	(x == vector.x) && (y == vector.y) && (z == vector.z) ;
		}
	bool operator != ( const E3D_VECTOR & vector ) const
		{
			return	(x != vector.x) || (y != vector.y) || (z != vector.z) ;
		}
	const E3D_VECTOR & operator += ( const E3D_VECTOR & vector )
		{
			x += vector.x ;
			y += vector.y ;
			z += vector.z ;
			return	*this ;
		}
	const E3D_VECTOR & operator -= ( const E3D_VECTOR & vector )
		{
			x -= vector.x ;
			y -= vector.y ;
			z -= vector.z ;
			return	*this ;
		}
	const E3D_VECTOR & operator *= ( double number )
		{
			x *= (REAL32) number ;
			y *= (REAL32) number ;
			z *= (REAL32) number ;
			return	*this ;
		}
	const E3D_VECTOR & operator *= ( const E3D_VECTOR & vector )
		{
			ExteriorProduct( vector ) ;
			return	*this ;
		}
	const E3D_VECTOR & operator /= ( double number )
		{
			x /= (REAL32) number ;
			y /= (REAL32) number ;
			z /= (REAL32) number ;
			return	*this ;
		}
	E3D_VECTOR operator + ( const E3D_VECTOR & vector ) const
		{
			E3D_VECTOR	result = *this ;
			result += vector ;
			return	result ;
		}
	E3D_VECTOR operator - ( void ) const
		{
			E3D_VECTOR	result = { - x, - y, - z } ;
			return	result ;
		}
	E3D_VECTOR operator - ( const E3D_VECTOR & vector ) const
		{
			E3D_VECTOR	result = *this ;
			result -= vector ;
			return	result ;
		}
	E3D_VECTOR operator * ( const E3D_VECTOR & vector ) const
		{
			E3D_VECTOR	result = *this ;
			result.ExteriorProduct( vector ) ;
			return	result ;
		}
	E3D_VECTOR operator * ( double number ) const
		{
			E3D_VECTOR	result = *this ;
			result *= number ;
			return	result ;
		}
	E3D_VECTOR operator / ( double number ) const
		{
			E3D_VECTOR	result = *this ;
			result /= number ;
			return	result ;
		}
	REAL64 Absolute( void ) const ;
	void ExteriorProduct( const E3D_VECTOR & vector ) ;
	REAL64 InnerProduct( const E3D_VECTOR & vector ) const ;
	void RoundTo1( void ) ;
} ;

class	E3DVector	: public	E3D_VECTOR
{
public:
	E3DVector( void ) { }
	E3DVector( double vx, double vy, double vz )
		{ x = (REAL32) vx ;  y = (REAL32) vy ;  z = (REAL32) vz ; }
	E3DVector( const E3D_VECTOR & v )
		{ x = v.x ;  y = v.y ;  z = v.z ; }
	const E3DVector & operator = ( const E3D_VECTOR & vector )
		{
			x = vector.x ;
			y = vector.y ;
			z = vector.z ;
			return	*this ;
		}

} ;

typedef	E3D_VECTOR *		PE3D_VECTOR ;
typedef	const E3D_VECTOR *	PCE3D_VECTOR ;

struct	E3D_VECTOR4	: public	E3D_VECTOR
{
	REAL32	d ;
} ;

class	E3DVector4	: public	E3D_VECTOR4
{
public:
	E3DVector4( void ) { }
	E3DVector4( double vx, double vy, double vz, double vd = 0 )
		{ x = (REAL32) vx ;  y = (REAL32) vy ;
			z = (REAL32) vz ;  d = (REAL32) vd ; }
	E3DVector4( const E3D_VECTOR & v )
		{ x = v.x ;  y = v.y ;  z = v.z ;  d = 0 ; }
	E3DVector4( const E3D_VECTOR4 & v )
		{ x = v.x ;  y = v.y ;  z = v.z ;  d = v.d ; }
	const E3DVector4 & operator = ( const E3D_VECTOR & vector )
		{
			x = vector.x ;
			y = vector.y ;
			z = vector.z ;
			d = 0 ;
			return	*this ;
		}
	const E3DVector4 & operator = ( const E3D_VECTOR4 & vector )
		{
			x = vector.x ;
			y = vector.y ;
			z = vector.z ;
			d = vector.d ;
			return	*this ;
		}

} ;

typedef	E3D_VECTOR4 *		PE3D_VECTOR4 ;
typedef	const E3D_VECTOR4 *	PCE3D_VECTOR4 ;


//
// 3x3 �ϊ��s��
// �i�A���C���̂��� 4x4 �j
//
struct	E3D_REV_MATRIX
{
public:
	REAL32	matrix[4][4] ;
public:
	void InitializeMatrix( const E3D_VECTOR & vector ) ;
	void RevolveOnX( double rSin, double rCos ) ;
	void RevolveOnY( double rSin, double rCos ) ;
	void RevolveOnZ( double rSin, double rCos ) ;
	void RevolveByAngleOn( const E3D_VECTOR & angle ) ;
	void RevolveForAngle( const E3D_VECTOR & angle ) ;
	void MagnifyByVector( const E3D_VECTOR & vector ) ;
	void RevolveMatrix( E3D_REV_MATRIX & matDst ) const ;
	void RevolveByMatrix( const E3D_REV_MATRIX & matSrc ) ;
	void RevolveVector( E3D_VECTOR & vector ) const ;
} ;

typedef	E3D_REV_MATRIX *		PE3D_REV_MATRIX ;
typedef	const E3D_REV_MATRIX *	PCE3D_REV_MATRIX ;

//
// ���}�b�s���O
//
struct	E3D_ENVIRONMENT_MAPPING
{
	PEGL_IMAGE_INFO	pRoundImage ;		// ���`�摜
	PEGL_IMAGE_INFO	pRoundLuminous ;
	PEGL_IMAGE_INFO	pUpperImage ;		// �㔼��
	PEGL_IMAGE_INFO	pUpperLuminous ;
	PEGL_IMAGE_INFO	pUnderImage ;		// ������
	PEGL_IMAGE_INFO	pUnderLuminous ;
} ;

//
// �e�N�X�`���[�}�b�s���O
//
struct	E3D_TEXTURE_MAPPING
{
	PEGL_IMAGE_INFO	pTextureImage ;		// �e�N�X�`���摜
	PEGL_IMAGE_INFO	pLuminousImage ;	// �����e�N�X�`��
	REAL32			rThresholdZ ;		// �k���摜�ɐ؂�ւ��邽�߂�臒l
	DWORD			nSmallScale ;		// �k���摜�̃X�P�[���i1/2^n�j
	PEGL_IMAGE_INFO	pSmallImage ;		// �k���e�N�X�`���摜
	PEGL_IMAGE_INFO	pSmallLuminous ;	// �k�������e�N�X�`��
} ;

//
// �|���S���̕\�ʑ���
//
struct	E3D_SURFACE_ATTRIBUTE
{
	DWORD			dwShadingFlags ;	// �V�F�[�f�B���O�t���O
	DWORD			dwReserved ;
	E3D_COLOR		rgbaColor ;			// ��{�F
	union
	{
		E3D_TEXTURE_MAPPING		txmap ;
		E3D_ENVIRONMENT_MAPPING	envmap ;
	} ;
	DWORD			nTextureApply ;		// �e�N�X�`���K�p�x�i���g�p�j
	DWORD			nLiminousApply ;	// �����e�N�X�`���K�p�x
	SDWORD			nAmbient ;			// ����
	SDWORD			nDiffusion ;		// �g�U��
	SDWORD			nSpecular ;			// ���ˌ�
	SDWORD			nSpecularSize ;
	SDWORD			nTransparency ;		// �����x
	SDWORD			nDeepness ;			// �����[�x�W��
} ;
typedef	E3D_SURFACE_ATTRIBUTE *			PE3D_SURFACE_ATTRIBUTE ;
typedef	const E3D_SURFACE_ATTRIBUTE *	PCE3D_SURFACE_ATTRIBUTE ;

#define	E3DSAF_NO_SHADING			0x00000000		// �V�F�[�f�B���O����
#define	E3DSAF_FLAT_SHADE			0x00000001		// �t���b�g�V�F�[�f�B���O�i���g�p�j
#define	E3DSAF_GOURAUD_SHADE		0x00000002		// �O�[���[�V�F�[�f�B���O
#define	E3DSAF_PHONG_SHADE			0x00000003		// �t�H���V�F�[�f�B���O�i���g�p�j
#define	E3DSAF_SHADING_MASK			0x000000FF
#define	E3DSAF_TEXTURE_TILING		0x00000100		// �e�N�X�`�����^�C�����O
#define	E3DSAF_TEXTURE_TRIM			0x00000200		// �g���~���O�e�N�X�`��
#define	E3DSAF_TEXTURE_SMOOTH		0x00000400		// �e�N�X�`���⊮�g��
#define	E3DSAF_TEXTURE_MAPPING		0x00001000		// �e�N�X�`���}�b�s���O
#define	E3DSAF_ENVIRONMENT_MAP		0x00002000		// ���}�b�s���O
#define	E3DSAF_GENVIRONMENT_MAP		0x00004000		// �O���[�o�����}�b�s���O
#define	E3DSAF_SINGLE_SIDE_PLANE	0x00010000		// �Жʃ|���S��
#define	E3DSAF_NO_ZBUFFER			0x00020000		// ����r���s��Ȃ��ŕ`��

//
// ����
//
struct	E3D_LIGHT_ENTRY
{
	DWORD		dwLightType ;
	EGL_PALETTE	rgbColor ;
	REAL32		rBrightness ;
	DWORD		dwReserved ;
	E3D_VECTOR4	vecLight ;
} ;
typedef	E3D_LIGHT_ENTRY *	PE3D_LIGHT_ENTRY ;
typedef	const E3D_LIGHT_ENTRY *	PCE3D_LIGHT_ENTRY ;

#define	E3D_FOG_LIGHT			0x00000000		// �[���t�H�b�O
#define	E3D_AMBIENT_LIGHT		0x00000001		// ����
#define	E3D_VECTOR_LIGHT		0x00000002		// ����������
#define	E3D_POINT_LIGHT			0x00000004		// �_����

//
// �e�N�X�`���}�b�s���O�p�p�����[�^
//
struct	E3D_TEXTURE_MAPINFO
{
	PEGL_IMAGE_INFO	pTextureImage ;		// �ʏ�e�N�X�`��
	PEGL_IMAGE_INFO	pLuminousImage ;	// �����e�N�X�`��
	DWORD			nTextureApply ;		// �e�N�X�`���K�p�x
	DWORD			nLiminousApply ;	// �����e�N�X�`���K�p�x
	E3D_VECTOR4		vOriginPos ;		// �}�b�s���O�p�����[�^
	E3D_VECTOR4		vAxisX ;
	E3D_VECTOR4		vAxisY ;
	REAL32			rFogDeepness ;		// �[���t�H�b�O�p
	EGL_PALETTE		rgbFogColor ;
} ;

//
// �|���S���v���~�e�B�u���
//
struct	E3D_PRIMITIVE_VERTEX
{
	PE3D_VECTOR4		vertex ;	// ���_���W
	PE3D_VECTOR4		normal ;	// �@���i�X���[�X�|���S���̂݁j
	union
	{
		E3D_VECTOR_2D	uv_map ;	// UV ���W�i�e�N�X�`���|���S���̂݁j
		E3D_COLOR		color ;		// ���_�F�i���_�F�|���S���̂݁j
	} ;
} ;

struct	E3D_PRIMITIVE_INFINITE_PLANE
{
	PE3D_VECTOR4	vertex ;		// ���ʊ�_
	PE3D_VECTOR4	xAxis ;			// �e�N�X�`������
	PE3D_VECTOR4	yAxis ;			// �e�N�X�`������
	REAL32			rFogDeepness ;	// �[���t�H�b�O�p
	EGL_PALETTE		rgbFogColor ;
} ;

struct	E3D_PRIMITIVE_IMAGE
{
	PE3D_VECTOR4	vCenter ;			// ���S���W
	PE3D_VECTOR4	vEnlarge ;			// �g�嗦
	E3D_VECTOR_2D	vImageBase ;		// �摜�̊���W
	DWORD			dwTransparency ;	// �����x
	REAL32			rRevolveAngle ;		// ��]�p [rad]
	PEGL_IMAGE_INFO	pImageInf ;			// �摜�o�b�t�@�i�K�v�ł���΁j
} ;

struct	E3D_PRIMITIVE_POLYGON
{
	DWORD					dwTypeFlag ;
	PE3D_SURFACE_ATTRIBUTE	pSurfaceAttr ;
	DWORD					dwVertexCount ;
	DWORD					dwReserved ;
	union
	{
		// �|���S��
		E3D_PRIMITIVE_VERTEX			polygon[1] ;
		// �������ʁiUV�t�j
		E3D_PRIMITIVE_INFINITE_PLANE	infinite_plane ;
		// �摜
		E3D_PRIMITIVE_IMAGE				image ;
	} ;
} ;
typedef	E3D_PRIMITIVE_POLYGON *	PE3D_PRIMITIVE_POLYGON ;
typedef	const E3D_PRIMITIVE_POLYGON *	PCE3D_PRIMITIVE_POLYGON ;

#define	E3D_FLAT_POLYGON			0x00000000
#define	E3D_SMOOTH_POLYGON			0x00000001
#define	E3D_TEXTURE_POLYGON			0x00000002
#define	E3D_VERTEX_COLOR_POLYGON	0x00000004
#define	E3D_POLYGON_PRIMITIVE_MASK	0x00000007
#define	E3D_INFINITE_PLANE			0x0000000A
#define	E3D_IMAGE_PRIMITIVE			0x00000100

//
// ���ʃp�����[�^
//
typedef	E3D_VECTOR4	E3D_PLANE_PARAMETER ;
typedef	E3D_PLANE_PARAMETER *		PE3D_PLANE_PARAMETER ;
typedef	const E3D_PLANE_PARAMETER *	PCE3D_PLANE_PARAMETER ;

//
// �����_�����O�|���S�����
//
struct	E3D_POLYGON_ENTRY
{
	DWORD				dwTypeFlag ;
	DWORD				dwTransparency ;
	union
	{
		struct
		{
			E3D_TEXTURE_MAPINFO		txmap ;
			PE3D_SURFACE_ATTRIBUTE	pAttr ;
		}					poly ;
		struct
		{
			PEGL_IMAGE_INFO			pInfo ;
			REAL32					rRevolveAngle ;
			E3D_VECTOR_2D			vCenter ;
			E3D_VECTOR_2D			vEnlarge ;
			E3D_VECTOR_2D			vImageBase ;
			PE3D_SURFACE_ATTRIBUTE	pAttr ;
		}					image ;
	}					surface ;
	E3D_PLANE_PARAMETER	plane ;
	E3D_VECTOR4			vCenter ;
	DWORD				dwVertexCount ;
	PE3D_VECTOR4		pVertexes ;
	PE3D_VECTOR4		pNormals ;
	DWORD				dwProjectedCount ;
	PE3D_VECTOR_2D		pProjVertexes ;
	PE3D_COLOR			pVertexColors ;
} ;
typedef	E3D_POLYGON_ENTRY *			PE3D_POLYGON_ENTRY ;
typedef	const E3D_POLYGON_ENTRY *	PCE3D_POLYGON_ENTRY ;

//
// 3D �`��p�����[�^
//
struct	E3D_RENDER_PARAM
{
	DWORD				dwFlags ;
	DWORD				dwTransparency ;
	E3D_COLOR			rgbaColor ;
	PEGL_IMAGE_INFO		pSrcImage ;
	PEGL_IMAGE_INFO		pLuminousImage ;
	DWORD				nTextureApply ;		// �e�N�X�`���K�p�x�i���g�p�j
	DWORD				nLiminousApply ;	// �����e�N�X�`���K�p�x
	union
	{
		struct
		{
			E3D_VECTOR			vRenderPos ;
			E3D_VECTOR_2D		vRevCenter ;
			int					nViewVertexes ;
			PCE3D_VECTOR_2D		pViewVertexes ;
			PCE3D_COLOR			pVertexColors ;
			PCE3D_REV_MATRIX	pRevMatrix ;
		}		rev ;
		struct
		{
			int					nViewVertexes ;
			PCE3D_VECTOR4		pVertexes ;
			PCE3D_VECTOR_2D		pViewVertexes ;
			PCE3D_COLOR			pVertexColors ;
		}		poly ;
	} ;
} ;
typedef	E3D_RENDER_PARAM *			PE3D_RENDER_PARAM ;
typedef	const E3D_RENDER_PARAM *	PCE3D_RENDER_PARAM ;

/*	EGL_RENDER_PARAM::dwFlags �t���O�̎�肤��g�ݍ��킹
#define	EGL_WITH_Z_ORDER			0x0004
#define	E3DSAF_TEXTURE_TILING		0x00000100		// �e�N�X�`�����^�C�����O
#define	E3DSAF_TEXTURE_TRIM			0x00000200		// �g���~���O�e�N�X�`��
#define	E3DSAF_TEXTURE_SMOOTH		0x00000400		// �e�N�X�`���⊮�g��
*/
#define	E3DRP_RENDER_REV_IMAGE		0x0010
#define	E3DRP_RENDER_POLYGON		0x0000
#define	E3DRP_Z_ORDER_SCREEN		0x0020
#define	E3DRP_NO_SCREEN_ORIGIN		0x0040

//
// �|���S���`��
//
typedef	struct EGL_RENDER_POLYGON *	HEGL_RENDER_POLYGON ;

struct	EGL_RENDER_POLYGON
{
	ESLError (*pfnRelease)( HEGL_RENDER_POLYGON hRenderPoly ) ;
	ESLError (*pfnInitialize)
		( HEGL_RENDER_POLYGON hRenderPoly,
			PEGL_IMAGE_INFO pDstImage,
			PCEGL_RECT pClipRect,
			PEGL_IMAGE_INFO pZBuffer, PCE3D_VECTOR pScreenPos ) ;
	HEGL_DRAW_IMAGE (*pfnGetDrawImage)( HEGL_RENDER_POLYGON hRenderPoly ) ;
	DWORD (*pfnGetFunctionFlags)( HEGL_RENDER_POLYGON hRenderPoly ) ;
	DWORD (*pfnSetFunctionFlags)
		( HEGL_RENDER_POLYGON hRenderPoly, DWORD dwFlags ) ;
	ESLError (*pfnPrepareMatrix)
		( HEGL_RENDER_POLYGON hRenderPoly,
			const E3D_REV_MATRIX * matrix,
			PCE3D_VECTOR pOrigin, PCE3D_VECTOR pEnlarge ) ;
	ESLError (*pfnRevolveMatrix)
		( HEGL_RENDER_POLYGON hRenderPoly,
			PE3D_VECTOR4 pDst, PCE3D_VECTOR4 pSrc, unsigned int nVectorCount ) ;
	ESLError (*pfnSetZClipRange)
		( HEGL_RENDER_POLYGON hRenderPoly, REAL32 rMin, REAL32 rMax ) ;
	ESLError (*pfnSetEnvironmentMapping)
		( HEGL_RENDER_POLYGON hRenderPoly,
			const E3D_REV_MATRIX * matrix,
			const E3D_ENVIRONMENT_MAPPING * envmap ) ;
	ESLError (*pfnPrepareLight)
		( HEGL_RENDER_POLYGON hRenderPoly,
			HSTACKHEAP hStackHeap, unsigned int nLightCount,
								PCE3D_LIGHT_ENTRY pLightEntries ) ;
	E3D_POLYGON_ENTRY * (*pfnCreatePolygonEntry)
		( HEGL_RENDER_POLYGON hRenderPoly,
			HSTACKHEAP hStackHeap, PCE3D_PRIMITIVE_POLYGON pPrimitive ) ;
	E3D_POLYGON_ENTRY * (*pfnMakeUpPolygon)
		( HEGL_RENDER_POLYGON hRenderPoly,
			HSTACKHEAP hStackHeap, E3D_POLYGON_ENTRY * pPolyEntry ) ;
	ESLError (*pfnApplyAttribute)
		( HEGL_RENDER_POLYGON hRenderPoly,
			E3D_POLYGON_ENTRY * pPolyEntry,
				const E3D_COLOR * pColor, unsigned int nTransparency ) ;
	ESLError (*pfnGetExternalRect)
		( HEGL_RENDER_POLYGON hRenderPoly, EGL_RECT * pExtRect,
			PCE3D_POLYGON_ENTRY * pPolyEntry, unsigned int nCount ) ;
	ESLError (*pfnShadeReflectLights)
		( HEGL_RENDER_POLYGON hRenderPoly,
			PCE3D_SURFACE_ATTRIBUTE pSurfaceAttribute,
			PCE3D_PLANE_PARAMETER pPlaneParameter,
			PCE3D_VECTOR pFocusPoint, E3D_COLOR * pColorLooks ) ;
	ESLError (*pfnProjectScreen)
		( HEGL_RENDER_POLYGON hRenderPoly, PE3D_VECTOR_2D pDst,
				PCE3D_VECTOR4 pSrc, unsigned int nVertexCount ) ;
	ESLError (*pfnSortPolygonEntry)
		( HEGL_RENDER_POLYGON hRenderPoly,
			PE3D_POLYGON_ENTRY * ppPolygons,
			unsigned int nPolygonCount, DWORD dwSortingFlags ) ;
	ESLError (*pfnPrepareRender)
		( HEGL_RENDER_POLYGON hRenderPoly, PCE3D_POLYGON_ENTRY pPolyEntry ) ;
	ESLError (*pfnPrepareRenderParam)
		( HEGL_RENDER_POLYGON hRenderPoly, PCE3D_RENDER_PARAM pRenderParam ) ;
	ESLError (*pfnRenderPolygon)( HEGL_RENDER_POLYGON hRenderPoly ) ;

	// �|���S�������_�����O�I�u�W�F�N�g���
	inline ESLError Release( void )
		{
			return	(*pfnRelease)( this ) ;
		}
	// �|���S�������_�����O�I�u�W�F�N�g������
	inline ESLError Initialize
		( PEGL_IMAGE_INFO pDstImage, PCEGL_RECT pClipRect,
			PEGL_IMAGE_INFO pZBuffer, PCE3D_VECTOR pScreenPos )
		{
			return	(*pfnInitialize)
				( this, pDstImage, pClipRect, pZBuffer, pScreenPos ) ;
		}
	// �摜�`��I�u�W�F�N�g�擾
	inline HEGL_DRAW_IMAGE GetDrawImage( void )
		{
			return	(*pfnGetDrawImage)( this ) ;
		}
	// �@�\�t���O���擾����
	inline DWORD GetFunctionFlags( void )
		{
			return	(*pfnGetFunctionFlags)( this ) ;
		}
	// �@�\�t���O��ݒ肷��
	inline DWORD SetFunctionFlags( DWORD dwFlags )
		{
			return	(*pfnSetFunctionFlags)( this, dwFlags ) ;
		}
	// ���W�ϊ�����
	inline ESLError PrepareMatrix
		( const E3D_REV_MATRIX * matrix,
			PCE3D_VECTOR pOrigin, PCE3D_VECTOR pEnlarge )
		{
			return	(*pfnPrepareMatrix)( this, matrix, pOrigin, pEnlarge ) ;
		}
	// ���W�ϊ�
	inline ESLError RevolveMatrix
		( PE3D_VECTOR4 pDst, PCE3D_VECTOR4 pSrc, unsigned int nVectorCount )
		{
			return	(*pfnRevolveMatrix)( this, pDst, pSrc, nVectorCount ) ;
		}
	// Z �N���b�s���O�ݒ�
	inline ESLError SetZClipRange( REAL32 rMin, REAL32 rMax )
		{
			return	(*pfnSetZClipRange)( this, rMin, rMax ) ;
		}
	// ���}�b�s���O�ݒ�
	inline ESLError SetEnvironmentMapping
		( const E3D_REV_MATRIX * matrix,
			const E3D_ENVIRONMENT_MAPPING * envmap )
		{
			return	(*pfnSetEnvironmentMapping)( this, matrix, envmap ) ;
		}
	// ���C�g�ݒ�
	inline ESLError PrepareLight
			( HSTACKHEAP hStackHeap,
				unsigned int nLightCount, PCE3D_LIGHT_ENTRY pLightEntries )
		{
			return	(*pfnPrepareLight)
				( this, hStackHeap, nLightCount, pLightEntries ) ;
		}
	// �����_�����O�p�|���S�����쐬
	inline E3D_POLYGON_ENTRY * CreatePolygonEntry
			( HSTACKHEAP hStackHeap, PCE3D_PRIMITIVE_POLYGON pPrimitive )
		{
			return	(*pfnCreatePolygonEntry)( this, hStackHeap, pPrimitive ) ;
		}
	// �|���S���ɃV�F�[�f�B���O�E�����ϊ����{��
	inline E3D_POLYGON_ENTRY * MakeUpPolygon
			( HSTACKHEAP hStackHeap, E3D_POLYGON_ENTRY * pPolyEntry )
		{
			return	(*pfnMakeUpPolygon)( this, hStackHeap, pPolyEntry ) ;
		}
	// �|���S���ɑ�����ݒ肷��
	inline ESLError ApplyAttribute
			( E3D_POLYGON_ENTRY * pPolyEntry,
				const E3D_COLOR * pColor, unsigned int nTransparency )
		{
			return	(*pfnApplyAttribute)
				( this, pPolyEntry, pColor, nTransparency ) ;
		}
	// �ŏ��O�ڋ�`���擾����
	inline ESLError GetExternalRect
			( EGL_RECT * pExtRect,
				PCE3D_POLYGON_ENTRY * pPolyEntry, unsigned int nCount )
		{
			return	(*pfnGetExternalRect)
				( this, pExtRect, pPolyEntry, nCount ) ;
		}
	// �V�F�[�f�B���O
	inline ESLError ShadeReflectLights
		( PCE3D_SURFACE_ATTRIBUTE pSurfaceAttribute,
			PCE3D_PLANE_PARAMETER pPlaneParameter,
			PCE3D_VECTOR pFocusPoint, E3D_COLOR * pColorLooks )
		{
			return	(*pfnShadeReflectLights)
				( this, pSurfaceAttribute,
					pPlaneParameter, pFocusPoint, pColorLooks ) ;
		}
	// �����ϊ�
	inline ESLError ProjectScreen
			( PE3D_VECTOR_2D pDst,
				PCE3D_VECTOR4 pSrc, unsigned int nVertexCount )
		{
			return	(*pfnProjectScreen)( this, pDst, pSrc, nVertexCount ) ;
		}
	// �|���S�����\�[�g
	inline ESLError SortPolygonEntry
		( PE3D_POLYGON_ENTRY * ppPolygons,
			unsigned int nPolygonCount, DWORD dwSortingFlags )
		{
			return	(*pfnSortPolygonEntry)
				( this, ppPolygons, nPolygonCount, dwSortingFlags ) ;
		}
	// �����_�����O����
	inline ESLError PrepareRender( PCE3D_POLYGON_ENTRY pPolyEntry )
		{
			return	(*pfnPrepareRender)( this, pPolyEntry ) ;
		}
	// �����_�����O����
	inline ESLError PrepareRenderParam( PCE3D_RENDER_PARAM pRenderParam )
		{
			return	(*pfnPrepareRenderParam)( this, pRenderParam ) ;
		}
	// �����_�����O
	inline ESLError RenderPolygon( void )
		{
			return	(*pfnRenderPolygon)( this ) ;
		}

} ;

#define	E3D_FLAG_ANTIALIAS_SIDE_EDGE	0x0001
#define	E3D_FLAG_TEXTURE_SMOOTHING		0x0002

#define	E3D_SORT_TRANSPARENT	0x0001
#define	E3D_SORT_OPAQUE			0x0002


//
// ���f�������蔻��
//
typedef	struct EGL_MODEL_MATRIX *	HEGL_MODEL_MATRIX ;

struct	EGL_MODEL_MATRIX
{
	ESLError (*pfnRelease)( HEGL_MODEL_MATRIX hMatrix ) ;
	ESLError (*pfnInitialize)
		( HEGL_MODEL_MATRIX hMatrix,
			PCE3D_PRIMITIVE_POLYGON * pModel, unsigned int nPolyCount ) ;
	ESLError (*pfnIsHitInclusiveSphere)
		( HEGL_MODEL_MATRIX hMatrix,
			const E3D_VECTOR * pSphere, REAL32 rRadius, int * pHitResult ) ;
	ESLError (*pfnIsHitAgainstSphere)
		( HEGL_MODEL_MATRIX hMatrix,
			const E3D_VECTOR * pSphere, REAL32 rRadius,
				int * pHitResult, E3D_VECTOR * pHitPos,
				E3D_VECTOR * pHitNormal, E3D_VECTOR * pReflection ) ;
	ESLError (*pfnIsCrossingSegment)
		( HEGL_MODEL_MATRIX hMatrix,
			const E3D_VECTOR * pPos0, const E3D_VECTOR * pPos1,
				REAL32 rErrorGap, int * pHitResult, E3D_VECTOR * pHitPos,
				E3D_VECTOR * pHitNormal, E3D_VECTOR * pReflection ) ;

	// �����蔻��I�u�W�F�N�g���
	inline ESLError Release( void )
		{
			return	(*pfnRelease)( this ) ;
		}
	// �����蔻��I�u�W�F�N�g������
	inline ESLError Initialize
			( PCE3D_PRIMITIVE_POLYGON * pModel, unsigned int nPolyCount )
		{
			return	(*pfnInitialize)( this, pModel, nPolyCount ) ;
		}
	// �����ԂƋ��̂Ƃ̌�������
	inline ESLError IsHitInclusiveSphere
			( const E3D_VECTOR * pSphere, double rRadius, int * pHitResult )
		{
			return	(*pfnIsHitInclusiveSphere)
				( this, pSphere, (REAL32) rRadius, pHitResult ) ;
		}
	// �|���S���Ƌ��̂Ƃ̌�������
	inline ESLError IsHitAgainstSphere
		( const E3D_VECTOR * pSphere, double rRadius,
				int * pHitResult, E3D_VECTOR * pHitPos,
				E3D_VECTOR * pHitNormal, E3D_VECTOR * pReflection )
		{
			return	(*pfnIsHitAgainstSphere)
				( this, pSphere, (REAL32) rRadius,
					pHitResult, pHitPos, pHitNormal, pReflection ) ;
		}
	// �|���S���Ɛ����Ƃ̌�������
	inline ESLError IsCrossingSegment
		( const E3D_VECTOR * pPos0, const E3D_VECTOR * pPos1,
			double rErrorGap, int * pHitResult, E3D_VECTOR * pHitPos,
			E3D_VECTOR * pHitNormal, E3D_VECTOR * pReflection )
		{
			return	(*pfnIsCrossingSegment)
				( this, pPos0, pPos1, (REAL32) rErrorGap,
					pHitResult, pHitPos, pHitNormal, pReflection ) ;
		}
} ;

//
// EGL �֐�
//
extern	"C"
{
	PE3D_POLYGON_REGION eglNormalizePolygonRegion
		( PE3D_POLYGON_REGION pPolyRegion,
			PCEGL_RECT pClipRect,
			unsigned int nVertexCount,
			PCE3D_VECTOR_2D pPolyVertexes,
			PCE3D_COLOR pVertexColors = NULL ) ;
	HEGL_RENDER_POLYGON eglCreateRenderPolygon( void ) ;
	HEGL_RENDER_POLYGON eglCurrentRenderPolygon( void ) ;
	HEGL_MODEL_MATRIX eglCreateModelMatrix( void ) ;
} ;


#endif
