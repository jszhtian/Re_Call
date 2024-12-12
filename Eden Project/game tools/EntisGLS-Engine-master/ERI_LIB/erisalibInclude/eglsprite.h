
/*****************************************************************************
                          Entis Graphic Library
 -----------------------------------------------------------------------------
     Copyright (c) 2002-2004 Leshade Entis, Entis-soft. Al rights reserved.
 *****************************************************************************/


#if	!defined(__SPRITE_H__)
#define	__SPRITE_H__

#if	!defined(COMPACT_NOA_DECODER)


//////////////////////////////////////////////////////////////////////////////
// �x�W�F�Ȑ�
//////////////////////////////////////////////////////////////////////////////

template <class _T> class	EBezierCurve
{
protected:
	double	m_b[4] ;
	_T		m_cp[4] ;

public:
	// �p�����[�^�v�Z
	void set_t( double t )
		{
			double	ct = 1.0 - t ;
			m_b[0] = ct * ct * ct ;
			m_b[1] = 3.0 * t * ct * ct ;
			m_b[2] = 3.0 * t * t * ct ;
			m_b[3] = t * t * t ;
		}
	// ����_�A�N�Z�X
	_T operator [] ( int index ) const
		{
			ESLAssert( (index >= 0) && (index < 4) ) ;
			return	m_cp[index] ;
		}
	_T & operator [] ( int index )
		{
			ESLAssert( (index >= 0) && (index < 4) ) ;
			return	m_cp[index] ;
		}
	// �x�W�F�Ȑ���C�ӓ_�ŕ�������
	void DivideBezier
		( double t, EBezierCurve<_T> & bzFirst, EBezierCurve<_T> & bzLast )
		{
			_T	P = m_cp[0] + (m_cp[1] - m_cp[0]) * t ;
			_T	Q = m_cp[1] + (m_cp[2] - m_cp[1]) * t ;
			_T	R = m_cp[2] + (m_cp[3] - m_cp[2]) * t ;
			_T	S = P + (Q - P) * t ;
			_T	T = Q + (R - Q) * t ;
			_T	U = S + (T - S) * t ;
			bzFirst[0] = m_cp[0] ;
			bzFirst[1] = P ;
			bzFirst[2] = S ;
			bzFirst[3] = U ;
			bzLast[0] = U ;
			bzLast[1] = T ;
			bzLast[2] = R ;
			bzLast[3] = m_cp[3] ;
		}

} ;

class	EBezierR64	: public	EBezierCurve<double>
{
public:
	// �\�z�֐�
	EBezierR64( void ) { }
	EBezierR64( const EBezierR64 & bz )
		{
			for ( int i = 0; i < 4; i ++ )
				m_cp[i] = bz.m_cp[i] ;
		}
	// ������Z�q
	const EBezierR64 & operator = ( const EBezierR64 & bz )
		{
			for ( int i = 0; i < 4; i ++ )
				m_cp[i] = bz.m_cp[i] ;
			return	*this ;
		}
	// �Ȑ��v�Z P(t)
	double pt( double t ) ;

} ;

class	EBezier2D	: public	EBezierCurve<E3D_VECTOR_2D>
{
public:
	// �\�z�֐�
	EBezier2D( void ) { }
	EBezier2D( const EBezier2D & bz )
		{
			for ( int i = 0; i < 4; i ++ )
				m_cp[i] = bz.m_cp[i] ;
		}
	// ������Z�q
	const EBezier2D & operator = ( const EBezier2D & bz )
		{
			for ( int i = 0; i < 4; i ++ )
				m_cp[i] = bz.m_cp[i] ;
			return	*this ;
		}
	// �Ȑ��v�Z P(t)
	E3D_VECTOR_2D pt( double t ) ;
	EGL_POINT ipt( double t ) ;

} ;

class	EBezier3D	: public	EBezierCurve<E3D_VECTOR>
{
public:
	// �\�z�֐�
	EBezier3D( void ) { }
	EBezier3D( const EBezier3D & bz )
		{
			for ( int i = 0; i < 4; i ++ )
				m_cp[i] = bz.m_cp[i] ;
		}
	// ������Z�q
	const EBezier3D & operator = ( const EBezier3D & bz )
		{
			for ( int i = 0; i < 4; i ++ )
				m_cp[i] = bz.m_cp[i] ;
			return	*this ;
		}
	// �Ȑ��v�Z P(t)
	E3D_VECTOR pt( double t ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �X�v���C�g���ۃN���X
//////////////////////////////////////////////////////////////////////////////

class	ESprite	: public	ESLObject
{
protected:
	int				m_priority ;		// �\���D��x
	ESprite *		m_parent ;			// �e�X�v���C�g
	bool			m_visible ;			// �\���t���O

public:
	// �\�z�֐�
	ESprite( void )
		: m_priority( 0 ), m_parent( NULL ), m_visible( false ) { }
	// ���Ŋ֐�
	virtual ~ESprite( void ) { }
	// �N���X���
	DECLARE_CLASS_INFO( ESprite, ESLObject )

	// �\����Ԏ擾
	virtual bool IsVisible( void ) ;
	// �\����Ԑݒ�
	virtual void SetVisible( bool fVisible ) ;
	// �O��(�ŏ�)��`�擾
	virtual EGL_RECT GetRectangle( void ) = 0 ;
	// �A�ɂȂ���ځi�ő�j��`�擾
	virtual bool GetHiddenRectangle( EGL_RECT & rect ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) = 0 ;
	// �X�v���C�g��̎w��̈�̍X�V�ʒm
	virtual bool UpdateRect( EGL_RECT * pUpdateRect = NULL ) ;

	// �\���D��x�ύX
	void ChangePriority( int nPriority ) ;
	// �q�X�v���C�g�̃v���C�I���e�B�ύX
	virtual void ChangedChildPriority( ESprite * pChild, int nPriority ) ;
	// �\���D��x�擾
	int GetPriority( void ) const
		{
			return	m_priority ;
		}
	// �e�X�v���C�g�擾
	ESprite * GetParent( void ) const
		{
			return	m_parent ;
		}
	// �e�X�v���C�g�ݒ�
	void SetParentAs( ESprite * pSprite )
		{
			ESLAssert( pSprite != NULL ) ;
			pSprite->m_parent = this ;
		}
	void SetSpriteParent( ESprite * pParent )
		{
			m_parent = pParent ;
		}
	// �\���D��x�ݒ�
	void SetSpritePriority( int nPriority )
		{
			m_priority = nPriority ;
		}

} ;


//////////////////////////////////////////////////////////////////////////////
// �摜�X�v���C�g
//////////////////////////////////////////////////////////////////////////////

class	EImageSprite	: public ESprite, public EGLImage
{
public:
	struct	PARAMETER
	{
		DWORD			dwFlags ;			// �`��t���O
		EGL_POINT		ptDstPos ;			// �o�͊���W
		EGL_POINT		ptRevCenter ;		// ��]���S���W
		REAL32			rHorzUnit ;			// �����g�嗦
		REAL32			rVertUnit ;			// �����g�嗦
		REAL32			rRevAngle ;			// ��]�p�x [deg]
		REAL32			rCrossingAngle ;	// �����������p [deg]
		EGL_PALETTE		rgbDimColor ;		// ���`��F
		EGL_PALETTE		rgbLightColor ;
		unsigned int	nTransparency ;		// �����x
		REAL32			rZOrder ;			// ���l
		EGL_PALETTE		rgbColorParam1 ;	// �F�p�����[�^
	} ;

protected:
	PARAMETER		m_ispParam ;	// �X�v���C�g�p�����[�^
	EGL_RECT		m_rectExt ;		// �O�ڋ�`
	EGL_DRAW_PARAM	m_eglParam ;	// �`��p�����[�^
	EGL_IMAGE_AXES	m_eglAxes ;

public:
	// �\�z�֐�
	EImageSprite( void ) ;
	// ���Ŋ֐�
	virtual ~EImageSprite( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO2( EImageSprite, ESprite, EGLImage )

protected:
	// �X�v���C�g�p�����[�^����`��p�p�����[�^�Z�b�g�A�b�v
	void SetParameterToDraw( void ) ;

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
	virtual ESLError ReadImageFile( ESLFileObject & file ) ;

public:
	// �p�����[�^�擾
	void GetParameter( PARAMETER & param ) const ;
	// �p�����[�^�ݒ�
	void SetParameter( const PARAMETER & param ) ;

	// �\����Ԃ��擾
	virtual bool IsVisible( void ) ;
	// �\������W�擾
	virtual EGL_POINT GetPosition( void ) ;
	// �\������W�ݒ�
	virtual void MovePosition( EGL_POINT ptDstPos ) ;
	// �����x�擾
	unsigned int GetTransparency( void ) const ;
	// �����x�ݒ�
	void SetTransparency( unsigned int nTransparency ) ;

public:
	// ���[�J�����W����O���[�o�����W�ɕϊ�
	static EGL_POINT LocalToGlobal
		( EGL_POINT ptLocal, const EGL_DRAW_PARAM & eglParam ) ;
	static EGL_RECT LocalToGlobal
		( const EGL_RECT & rect , const EGL_DRAW_PARAM & eglParam  ) ;
	EGL_POINT LocalToGlobal( EGL_POINT ptLocal ) const
		{
			return	LocalToGlobal( ptLocal, m_eglParam ) ;
		}
	EGL_RECT LocalToGlobal( const EGL_RECT & rect ) const
		{
			return	LocalToGlobal( rect, m_eglParam ) ;
		}
	// �O���[�o�����W���烍�[�J�����W�ɕϊ�
	EGL_POINT GlobalToLocal( EGL_POINT ptGlobal ) const ;
	// �摜�`��p�����[�^�擾
	void GetDrawParameter( EGL_DRAW_PARAM & dp ) const
		{
			dp = m_eglParam ;
		}

public:
	// �O�ڋ�`���擾
	virtual EGL_RECT GetRectangle( void ) ;
	// �A�ɂȂ���ځi�ő�j��`�擾
	virtual bool GetHiddenRectangle( EGL_RECT & rect ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;
	// �X�v���C�g��̎w��̈�̍X�V�ʒm
	virtual bool UpdateRect( EGL_RECT * pUpdateRect = NULL ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �t�B���^�X�v���C�g
//////////////////////////////////////////////////////////////////////////////

class	EFilterSprite	: public	ESprite
{
protected:
	EGL_RECT	m_rectEffect ;
	BYTE		m_bytBlue[0x100] ;
	BYTE		m_bytGreen[0x100] ;
	BYTE		m_bytRed[0x100] ;
	BYTE		m_bytAlpha[0x100] ;

public:
	// �\�z�֐�
	EFilterSprite( void ) ;
	// ���Ŋ֐�
	virtual ~EFilterSprite( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EFilterSprite, ESprite )

public:
	// ��`�擾
	virtual EGL_RECT GetRectangle( void ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;

public:
	// ��`�ݒ�
	void SetRectangle( const EGL_RECT & rect ) ;
	// �P�x��ݒ�
	void SetBrightness( int nBrightness ) ;
	// �`���l�����Ƃ̋P�x��ݒ�
	void SetColorTone( int nRed, int nGreen, int nBlue, int nAlpha ) ;
	// ���]�t�B���^��ݒ�
	void SetInversionTone( int nRed, int nGreen, int nBlue, int nAlpha ) ;
	// ���C�g�t�B���^��ݒ�
	void SetLightTone( int nRed, int nGreen, int nBlue, int nAlpha ) ;
	// ���G�ȃt�B���^��ݒ�
	void SetGeneralTone
		( int nRedTone, int nRedFlag, int nGreenTone, int nGreenFlag,
			int nBlueTone, int nBlueFlag, int nAlphaTone, int nAlphaFlag ) ;
	// �g�[���e�[�u���𕡐�
	void SetToneTables
		( const void * pRed, const void * pGreen,
			const void * pBlue, const void * pAlpha ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �`��X�v���C�g
//////////////////////////////////////////////////////////////////////////////

class	EShapeSprite	: public	ESprite
{
protected:
	EGLPalette		m_rgbaColor ;
	unsigned int	m_nTransparency ;
	DWORD			m_dwFlags ;

public:
	// �\�z�֐�
	EShapeSprite( void )
		: m_rgbaColor((DWORD)0xFF000000), m_nTransparency(0),
			m_dwFlags(EGL_DRAW_BLEND_ALPHA) { }
	// ���Ŋ֐�
	virtual ~EShapeSprite( void ) { }
	// �N���X���
	DECLARE_CLASS_INFO( EShapeSprite, ESprite )

public:
	// �\����Ԏ擾
	virtual bool IsVisible( void ) ;
	// �`��F�擾
	EGL_PALETTE GetColor( void ) const
		{
			return	(EGL_PALETTE) m_rgbaColor ;
		}
	// �����x�擾
	unsigned int GetTransparency( void ) const
		{
			return	m_nTransparency ;
		}
	// �`��t���O�擾
	DWORD GetDrawFlag( void ) const
		{
			return	m_dwFlags ;
		}
	// �`��F�ݒ�
	void SetColor( EGL_PALETTE rgbaColor ) ;
	// �����x�ݒ�
	void SetTransparency( unsigned int nTransparency ) ;
	// �`��t���O�ݒ�
	void SetDrawFlag( DWORD dwFlags ) ;

} ;

class	ELineSprite	: public	EShapeSprite
{
protected:
	EGLRect					m_rectExt ;
	EObjArray<EGL_POINT>	m_lines ;

public:
	// �\�z�֐�
	ELineSprite( void )
		: m_rectExt(0,0,-1,-1) { }
	// ���Ŋ֐�
	virtual ~ELineSprite( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ELineSprite, EShapeSprite )

public:
	// �O�ڋ�`�擾
	virtual EGL_RECT GetRectangle( void ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;

public:
	// ������ݒ�
	void SetLines( const EGL_POINT * pLines, unsigned int nCount ) ;

} ;

class	ERectangleSprite	: public	EShapeSprite
{
protected:
	EGLRect		m_rectFill ;

public:
	// �\�z�֐�
	ERectangleSprite( void )
		: m_rectFill(0,0,0,0) { }
	// ���Ŋ֐�
	virtual ~ERectangleSprite( void ) { }
	// �N���X���
	DECLARE_CLASS_INFO( ERectangleSprite, EShapeSprite )

public:
	// �O�ڋ�`�擾
	virtual EGL_RECT GetRectangle( void ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;

public:
	// ��`��ݒ�
	void SetRectangle( const EGL_RECT & rectFill ) ;

} ;

class	EEllipseSprite	: public	EShapeSprite
{
protected:
	EGLPoint	m_ptCenter ;
	EGLSize		m_sizeRadius ;

public:
	// �\�z�֐�
	EEllipseSprite( void )
		: m_ptCenter(0,0), m_sizeRadius(0,0) { }
	// ���Ŋ֐�
	virtual ~EEllipseSprite( void ) { }
	// �N���X���
	DECLARE_CLASS_INFO( EEllipseSprite, EShapeSprite )

public:
	// �O�ڋ�`�擾
	virtual EGL_RECT GetRectangle( void ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;

public:
	// ���S�_���擾
	EGL_POINT GetPosition( void ) const
		{
			return	m_ptCenter ;
		}
	// ���a���擾
	EGL_SIZE GetRadiusSize( void ) const
		{
			return	m_sizeRadius ;
		}
	// ���S�_��ݒ�
	void MovePosition( EGL_POINT ptCenter ) ;
	// ���a��ݒ�
	void SetRadiusSize( EGL_SIZE sizeRaidus ) ;

} ;

class	EPolygonSprite	: public	EShapeSprite
{
protected:
	EGLRect			m_rectExt ;
	unsigned int	m_nVertexes ;
	EGL_POINT *		m_pPolygon ;

public:
	// �\�z�֐�
	EPolygonSprite( void )
		: m_rectExt(0,0,-1,-1), m_nVertexes(0), m_pPolygon(NULL) { }
	// ���Ŋ֐�
	virtual ~EPolygonSprite( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EPolygonSprite, EShapeSprite )

public:
	// �O�ڋ�`�擾
	virtual EGL_RECT GetRectangle( void ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;

public:
	// ���p�`��ݒ�
	void SetPolygon( const EGL_POINT * pPolygon, unsigned int nCount ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// 3D �\���X�v���C�g
//////////////////////////////////////////////////////////////////////////////

class	E3DRenderSprite	: public ESprite, public E3DRenderPolygon
{
public:
	// �\�z�֐�
	E3DRenderSprite( void ) ;
	// ���Ŋ֐�
	virtual ~E3DRenderSprite( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO2( E3DRenderSprite, ESprite, E3DRenderPolygon )

protected:
	EGL_RECT	m_rectExt ;

public:
	// �O�ڋ�`�擾
	virtual EGL_RECT GetRectangle( void ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;

public:
	// �����_�����O����
	virtual ESLError PrepareRendering( void ) ;

} ;


//////////////////////////////////////////////////////////////////////////////
// �X�v���C�g�T�[�o�[
//////////////////////////////////////////////////////////////////////////////

class	ESpriteServer	: public EImageSprite
{
protected:
	EGLImage				m_imgZBuf ;
	E3D_VECTOR				m_vScreen ;
	//
	HEGL_RENDER_POLYGON		m_hRenderPoly ;
	DWORD					m_dwRenderFlags ;
	DWORD					m_dwDrawFlags ;
	EIntTagArray<ESprite>	m_itaSprite ;
	//
	enum	UpdateStatus
	{
		usEmpty, usMulti, usFull
	} ;
	UpdateStatus			m_usStatus ;
	EObjArray<EGL_RECT>		m_listUpdateRect ;
	//
	enum
	{
		HideTestLimit = 16
	} ;
	EObjArray<EGL_RECT>		m_listHideRect ;
	//
	bool					m_fFillBack ;
	EGL_PALETTE				m_rgbBackColor ;

public:
	// �\�z�֐�
	ESpriteServer( void ) ;
	// ���Ŋ֐�
	virtual ~ESpriteServer( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ESpriteServer, EImageSprite )

public:
	// �摜�o�b�t�@�쐬
	virtual PEGL_IMAGE_INFO CreateImage
			( DWORD fdwFormat, DWORD dwWidth, DWORD dwHeight,
					DWORD dwBitsPerPixel, DWORD dwFlags = 0 ) ;
	// �摜�o�b�t�@����
	virtual void DeleteImage( void ) ;

public:
	// ���X�v���C�g�����擾
	unsigned int GetSpriteCount( void ) const ;
	// �X�v���C�g���擾
	ESprite * GetSpriteAt( unsigned int index ) ;
	// �w��X�v���C�g�̎w�W���擾����
	int GetSpriteIndex( ESprite * pSprite ) ;
	// �X�v���C�g��ǉ�
	virtual void AddSprite( int nPriority, ESprite * pSprite ) ;

public:
	// �X�v���C�g�𕪗�
	virtual ESLError DetachSprite( ESprite * pSprite ) ;
	// �S�ẴX�v���C�g�𕪗�
	virtual void DetachAllSprite( void ) ;
	// �X�v���C�g���폜
	virtual ESLError RemoveSprite( ESprite * pSprite ) ;
	// �S�ẴX�v���C�g���폜
	virtual void RemoveAllSprite( void ) ;

public:
	// �A�ɂȂ���ځi�ő�j��`�擾
	virtual bool GetHiddenRectangle( EGL_RECT & rect ) ;
	// �X�v���C�g�`��
	virtual void Draw( HEGL_RENDER_POLYGON hRenderPoly ) ;
	// �X�v���C�g��̎w��̈�̍X�V�ʒm
	virtual bool UpdateRect( EGL_RECT * pUpdateRect = NULL ) ;
	// �X�V�̈���ĕ`��
	virtual void Refresh( void ) ;
protected:
	// �̈�ĕ`��
	virtual void RefreshRect( const EGL_RECT & rectRefresh ) ;
public:
	// �X�V�̈���폜
	void FlushUpdatedRect( void ) ;
	// �q�X�v���C�g�̃v���C�I���e�B�ύX
	virtual void ChangedChildPriority( ESprite * pChild, int nPriority ) ;

public:
	// �w�i�F�擾
	EGL_PALETTE GetBackColor( void ) const ;
	// �w�i�F�ݒ�
	void SetBackColor( EGL_PALETTE rgbBack, bool fEnableBack ) ;
	// �w�i�F�͗L�����H
	bool IsEnabledFillBack( void ) const ;
	// �w�i�F��L���ɂ���
	void EnableFillBack( bool fEnableBack ) ;

public:
	// �X�N���[�����W���擾
	const E3D_VECTOR & GetScreenPosition( void ) const ;
	// �X�N���[�����W��ݒ�
	void SetScreenPosition( const E3D_VECTOR & vScreen ) ;
	// �`��@�\�t���O���擾����
	DWORD GetDrawFunctionFlags( void ) const ;
	DWORD GetRenderFunctionFlags( void ) const ;
	// �`��@�\�t���O��ݒ肷��
	void SetDrawFunctionFlags( DWORD dwFlags ) ;
	void SetRenderFunctionFlags( DWORD dwFlags ) ;
	// Z �o�b�t�@���쐬
	void CreateZBuffer( void ) ;
	// Z �o�b�t�@���폜
	void DeleteZBuffer( void ) ;
	// Z �o�b�t�@���擾
	EGLImage & GetZBuffer( void ) ;

} ;


#endif	//	!defined(COMPACT_NOA_DECODER)

#endif

