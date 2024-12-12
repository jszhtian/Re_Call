
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
   Copyright (C) 2003-2004 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#if	!defined(__ERISA_IMAGE_H__)
#define	__ERISA_IMAGE_H__	1


/*****************************************************************************
                            �摜�W�J�I�u�W�F�N�g
 *****************************************************************************/

class	ERISADecoder	: public	ESLObject
{
protected:
	ERI_INFO_HEADER			m_eihInfo ;				// �摜���w�b�_

	// �W�J�p�p�����[�^
	UINT					m_nBlockSize ;			// �u���b�L���O�T�C�Y
	ULONG					m_nBlockArea ;			// �u���b�N�ʐ�
	ULONG					m_nBlockSamples ;		// �u���b�N�̃T���v����
	UINT					m_nChannelCount ;		// �`���l����
	ULONG					m_nWidthBlocks ;		// �摜�̕��i�u���b�N���j
	ULONG					m_nHeightBlocks ;		// �摜�̍����i�u���b�N���j

	// �W�J��u���b�N�p�����[�^
	BYTE *					m_ptrDstBlock ;			// �o�͐�A�h���X
	LONG					m_nDstLineBytes ;		// �o�͐惉�C����
	UINT					m_nDstPixelBytes ;		// 1 �s�N�Z���̃o�C�g��
	UINT					m_nDstWidth ;			// �o�͐�u���b�N��
	UINT					m_nDstHeight ;			// �o�͐�u���b�N��
	DWORD					m_fdwDecFlags ;			// �����t���O

	// �t�W�J�p�o�b�t�@
	int						m_fEnhancedMode ;		// �g�����[�h�t���O
	PBYTE					m_ptrOperations ;		// �I�y���[�V�����e�[�u��
	SBYTE *					m_ptrColumnBuf ;		// ��o�b�t�@
	SBYTE *					m_ptrLineBuf ;			// �s�o�b�t�@
	SBYTE *					m_ptrDecodeBuf ;		// �W�J�o�b�t�@
	SBYTE *					m_ptrArrangeBuf ;		// �Ĕz��p�o�b�t�@
	PINT					m_pArrangeTable[4] ;	// �Ĕz��p�e�[�u��

	// ��t�W�J�p�o�b�t�@
	UINT					m_nBlocksetCount ;		// �T�u�u���b�N��
	REAL32 *				m_ptrVertBufLOT ;		// �d���ϊ��p�o�b�t�@
	REAL32 *				m_ptrHorzBufLOT ;
	REAL32 *				m_ptrBlocksetBuf[16] ;	// �u���b�N�Z�b�g�o�b�t�@
	REAL32 *				m_ptrMatrixBuf ;
	REAL32 *				m_ptrIQParamBuf ;		// �t�ʎq���p�����[�^
	BYTE *					m_ptrIQParamTable ;

	SBYTE *					m_ptrBlockLineBuf ;		// �u���b�N�s���ԃo�b�t�@
	SBYTE *					m_ptrNextBlockBuf ;
	SBYTE *					m_ptrImageBuf ;			// �摜�M���o�b�t�@
	SBYTE *					m_ptrYUVImage ;			// YUV �摜�o�̓o�b�t�@
	LONG					m_nYUVLineBytes ;		// YUV �摜���C����
	UINT					m_nYUVPixelBytes ;		// YUV �s�N�Z���̃o�C�g��

	SBYTE *					m_ptrMovingVector ;		// �����⏞�x�N�g��
	SBYTE *					m_ptrMoveVecFlags ;
	PVOID *					m_ptrMovePrevBlocks ;	// �Q�ƃu���b�N�ւ̃|�C���^
	PVOID *					m_ptrNextPrevBlocks ;
	PEGL_IMAGE_INFO			m_pPrevImageRef ;		// ���O�t���[���ւ̎Q��
	SDWORD					m_dwPrevLineBytes ;		// ���O�t���[�����C���X�e�b�v
	PEGL_IMAGE_INFO			m_pNextImageRef ;		// ����t���[���ւ̎Q��
	SDWORD					m_dwNextLineBytes ;		// ����t���[�����C���X�e�b�v
	PEGL_IMAGE_INFO			m_pFilterImageBuf ;		// �t�B���^�����p�o�b�t�@�Q��

	ERINA_HUFFMAN_TREE *	m_pHuffmanTree ;		// �n�t�}����
	ERISA_PROB_MODEL *		m_pProbERISA ;			// ���v���

public:
	typedef	void (ERISADecoder::*PTR_PROCEDURE)( void ) ;

protected:
	static const PTR_PROCEDURE	m_pfnColorOperation[0x10] ;

public:
	// �\�z�֐�
	ERISADecoder( void ) ;
	// ���Ŋ֐�
	virtual ~ERISADecoder( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERISADecoder, ESLObject )

public:
	enum	DecodeFlag
	{
		dfTopDown		= 0x0001,
		dfDifferential	= 0x0002,
		dfQuickDecode	= 0x0100,
		dfQualityDecode	= 0x0200,
		dfNoLoopFilter	= 0x0400,
		dfUseLoopFilter	= 0x0800,
		dfPreviewDecode	= 0x1000
	} ;
	// �������i�p�����[�^�̐ݒ�j
	virtual ESLError Initialize( const ERI_INFO_HEADER & infhdr ) ;
	// �I���i�������̉���Ȃǁj
	virtual void Delete( void ) ;
	// �摜��W�J
	virtual ESLError DecodeImage
		( const EGL_IMAGE_INFO & dstimginf,
			ERISADecodeContext & context, DWORD fdwFlags = dfTopDown ) ;
	// ���O�t���[���ւ̎Q�Ƃ�ݒ�
	void SetRefPreviousFrame
		( PEGL_IMAGE_INFO pPrevFrame, PEGL_IMAGE_INFO pNextFrame = NULL ) ;
	// �t�B���^�������ꂽ�摜���󂯎��
	PEGL_IMAGE_INFO GetFilteredImageBuffer( void ) const
		{
			return	m_pFilterImageBuf ;
		}
	// �t�B���^�����摜���󂯎��o�b�t�@��ݒ肷��
	void SetFilteredImageBuffer( PEGL_IMAGE_INFO pImageBuf ) ;

protected:
	// �W�J�i�s�󋵒ʒm�֐�
	virtual ESLError OnDecodedBlock
		( LONG line, LONG column, const EGL_IMAGE_RECT & rect ) ;

protected:
	// �t�摜�W�J
	ESLError DecodeLosslessImage
		( const EGL_IMAGE_INFO & imginf,
			ERISADecodeContext & context, DWORD fdwFlags ) ;
	// �A�����W�e�[�u���̏�����
	void InitializeArrangeTable( void ) ;
	// �I�y���[�V�������s
	void PerformOperation
		( DWORD dwOpCode, LONG nAllBlockLines, SBYTE * pNextLineBuf ) ;
	// �J���[�I�y���[�V�����֐��Q
	void ColorOperation0000( void ) ;
	void ColorOperation0101( void ) ;
	void ColorOperation0110( void ) ;
	void ColorOperation0111( void ) ;
	void ColorOperation1001( void ) ;
	void ColorOperation1010( void ) ;
	void ColorOperation1011( void ) ;
	void ColorOperation1101( void ) ;
	void ColorOperation1110( void ) ;
	void ColorOperation1111( void ) ;
	// �O���C�摜�^256�F�摜�̏o��
	void RestoreGray8( void ) ;
	// RGB �摜�i15�r�b�g�j�̏o��
	void RestoreRGB16( void ) ;
	// RGB �摜�̏o��
	void RestoreRGB24( void ) ;
	// RGBA �摜�̏o��
	void RestoreRGBA32( void ) ;
	// RGB �摜�̍����o��
	void LL_RestoreDeltaRGB24( void ) ;
	// RGBA �摜�̍����o��
	void LL_RestoreDeltaRGBA32( void ) ;
	// �摜�o�͊֐��擾
	virtual PTR_PROCEDURE GetLLRestoreFunc
		( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags ) ;

protected:
	// ��t�摜�W�J
	ESLError DecodeLossyImage
		( const EGL_IMAGE_INFO & imginf,
			ERISADecodeContext & context, DWORD fdwFlags ) ;
	// �u���b�N�P�ʂł̉�ʃT�C�Y���v�Z����
	void CalcImageSizeInBlocks( DWORD fdwTransformation ) ;
	// �T���v�����O�e�[�u���̏�����
	void InitializeZigZagTable( void ) ;
	// �����x�N�g�����Z�b�g�A�b�v����
	void SetupMovingVector( void ) ;
	// �t�ʎq��
	void ArrangeAndIQuantumize
		( const SBYTE * ptrSrcData, const SBYTE * ptrCoefficient ) ;
	// �t DCT �ϊ�
	void MatrixIDCT8x8( REAL32 * ) ;
	// �t LOT �ϊ�
	void MatrixILOT8x8( REAL32 * ptrVertBufLOT ) ;
	// 4:4:4 �X�P�[�����O
	void BlockScaling444( int x, int y, DWORD fdwFlags ) ;
	// 4:1:1 �X�P�[�����O
	void BlockScaling411( int x, int y, DWORD fdwFlags ) ;
	// ���ԉ摜�o�b�t�@�� 1 �`���l�������o��
	void StoreYUVImageChannel( int xBlock, int yBlock, int iChannel ) ;
	// ���ԉ摜�o�b�t�@�� 1 �`���l�������o���i�X�P�[�����O�j
	void StoreYUVImageChannelX2( int xBlock, int yBlock, int iChannel ) ;
	// ���ԃo�b�t�@�� YUV ���� RGB �`���֕ϊ�
	void ConvertImageYUVtoRGB( DWORD fdwFlags ) ;
	// �����⏞��K�p������ŉ摜�𕡐�����
	void MoveImageWithVector( void ) ;
	// �O���C�摜�̏o��
	void LS_RestoreGray8( void ) ;
	// RGB �摜�̏o��
	void LS_RestoreRGB24( void ) ;
	// RGBA �摜�̏o��
	void LS_RestoreRGBA32( void ) ;
	// �O���C�摜�̍����o��
	void LS_RestoreDeltaGray8( void ) ;
	// RGB �摜�̍����o��
	void LS_RestoreDeltaRGB24( void ) ;
	// RGBA �摜�̍����o��
	void LS_RestoreDeltaRGBA32( void ) ;
	// �摜�o�͊֐��擾
	virtual PTR_PROCEDURE GetLSRestoreFunc
		( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags ) ;

} ;


/*****************************************************************************
                            �摜���k�I�u�W�F�N�g
 *****************************************************************************/

class	ERISAEncoder	: public	ESLObject
{
protected:
	ERI_INFO_HEADER		m_eihInfo ;				// �摜���w�b�_

	// ���k�p�����[�^
	UINT				m_nBlockSize ;			// �u���b�L���O�T�C�Y
	ULONG				m_nBlockArea ;			// �u���b�N�ʐ�
	ULONG				m_nBlockSamples ;		// �u���b�N�̃T���v����
	UINT				m_nChannelCount ;		// �`���l����
	ULONG				m_nWidthBlocks ;		// �摜�̕��i�u���b�N���j
	ULONG				m_nHeightBlocks ;		// �摜�̍����i�u���b�N���j

	// ���̓u���b�N�p�����[�^
	BYTE *				m_ptrSrcBlock ;			// ���͌��A�h���X
	LONG				m_nSrcLineBytes ;		// ���͌����C����
	UINT				m_nSrcPixelBytes ;		// 1 �s�N�Z���̃o�C�g��
	UINT				m_nSrcWidth ;			// ���͌��u���b�N��
	UINT				m_nSrcHeight ;			// ���͌��u���b�N��
	DWORD				m_fdwEncFlags ;			// �������t���O

	// �t���k�p�p�����[�^
	SBYTE *				m_ptrColumnBuf ;		// ��o�b�t�@
	SBYTE *				m_ptrLineBuf ;			// �s�o�b�t�@
	SBYTE *				m_ptrEncodeBuf ;		// ���k�o�b�t�@
	SBYTE *				m_ptrArrangeBuf ;		// �Ĕz��p�o�b�t�@
	PINT				m_pArrangeTable[4] ;	// �Ĕz��p�e�[�u��

	// ��t���k�p�o�b�t�@
	UINT				m_nBlocksetCount ;		// �T�u�u���b�N��
	REAL32 *			m_ptrVertBufLOT ;		// �d���ϊ��p�o�b�t�@
	REAL32 *			m_ptrHorzBufLOT ;
	REAL32 *			m_ptrBlocksetBuf[36] ;	// �u���b�N�Z�b�g�o�b�t�@
	REAL32 *			m_ptrMatrixBuf[16] ;	// �s��ϊ��p�o�b�t�@
	REAL32 *			m_pQuantumizeScale[2] ;	// �ʎq���e�[�u��
	BYTE *				m_pQuantumizeTable ;

	LONG				m_nMovingVector ;		// �����⏞�x�N�g���t���O
	BYTE *				m_pMoveVecFlags ;		// �����⏞�x�N�g���t���O
	SBYTE *				m_pMovingVector ;		// �����⏞�x�N�g��
	INT					m_fPredFrameType ;		// �t���[���^�C�v
	LONG				m_nIntraBlockCount ;	// �C���g���u���b�N��
	REAL32				m_rDiffDeflectBlock ;	// ���ύ����΍��l
	REAL32				m_rMaxDeflectBlock ;	// �ő卷���΍��l

	SBYTE *				m_ptrCoefficient ;		// �ʎq���p�����[�^�o�̓o�b�t�@
	BYTE *				m_ptrImageDst ;			// �摜�M���o�̓o�b�t�@
	REAL32 *			m_ptrSignalBuf ;

	ERINA_HUFFMAN_TREE *	m_pHuffmanTree ;	// �n�t�}����
	ERISA_PROB_MODEL *		m_pProbERISA ;		// ���v���

public:
	typedef	void (ERISAEncoder::*PTR_PROCEDURE)( void ) ;

protected:
	static const PTR_PROCEDURE	m_pfnColorOperation[0x10] ;

public:
	enum	EncodeFlag
	{
		efTopDown		= 0x0001,
		efDifferential	= 0x0002,
		efNoMoveVector	= 0x0004,
		efBestCmpr		= 0x0000,
		efHighCmpr		= 0x0010,
		efNormalCmpr	= 0x0020,
		efLowCmpr		= 0x0030,
		efCmprModeMask	= 0x0030
	} ;
	enum	PresetParameter
	{
		ppClearQuality,
		ppHighQuality,
		ppAboveQuality,
		ppStandardQuality,
		ppBelowQuality,
		ppLowQuality,
		ppLowestQuality,
		ppMax
	} ;
	enum	ParameterFlag
	{
		pfUseLoopFilter		= 0x0001,
		pfLocalLoopFilter	= 0x0002
	} ;
	struct	PARAMETER
	{
		DWORD			m_dwFlags ;				// �t���O
		REAL32			m_rYScaleDC ;			// �P�x���������̗ʎq���W��
		REAL32			m_rCScaleDC ;			// �F�����������̗ʎq���W��
		REAL32			m_rYScaleLow ;			// �P�x����g�����̗ʎq���W��
		REAL32			m_rCScaleLow ;			// �F������g�����̗ʎq���W��
		REAL32			m_rYScaleHigh ;			// �P�x�����g�����̗ʎq���W��
		REAL32			m_rCScaleHigh ;			// �F�������g�����̗ʎq���W��
		int				m_nYThreshold ;			// �P�x������臒l
		int				m_nCThreshold ;			// �F��������臒l
		int				m_nYLPFThreshold ;		// �P�x���� LPF �w�W
		int				m_nCLPFThreshold ;		// �F������ LPF �w�W
		int				m_nAMDFThreshold ;		// �t���[���ԍő卷��臒l
		REAL32			m_rPFrameScale ;		// P �t���[���̗ʎq���W����
		REAL32			m_rBFrameScale ;		// B �t���[���̗ʎq���W����
		ULONG			m_nMaxFrameSize ;		// �ő�o�C�g��
		ULONG			m_nMinFrameSize	;		// �ŏ��o�C�g��

		PARAMETER( void ) ;
		void LoadPresetParam
			( PresetParameter ppIndex, ERI_INFO_HEADER & infhdr ) ;
	} ;

protected:
	PARAMETER			m_prmCmprOpt ;			// ���k�p�����[�^

public:
	// �\�z�֐�
	ERISAEncoder( void ) ;
	// ���Ŋ֐�
	virtual ~ERISAEncoder( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERISAEncoder, ESLObject )

	// �������i�p�����[�^�̐ݒ�j
	virtual ESLError Initialize( const ERI_INFO_HEADER & infhdr ) ;
	// �I���i�������̉���Ȃǁj
	virtual void Delete( void ) ;
	// �摜�����k
	virtual ESLError EncodeImage
		( const EGL_IMAGE_INFO & srcimginf,
			ERISAEncodeContext & context, DWORD fdwFlags = efNormalCmpr ) ;
	// ���k�I�v�V������ݒ�
	void SetCompressionParameter( const PARAMETER & prmCmprOpt ) ;

protected:
	// �W�J�i�s�󋵒ʒm�֐�
	virtual ESLError OnEncodedBlock( LONG line, LONG column ) ;

public:
	// �����⏞�p�����[�^���v�Z����
	ESLError ProcessMovingVector
		( const EGL_IMAGE_INFO & nextimg,
			const EGL_IMAGE_INFO & previmg, int & nAbsMaxDiff,
					const EGL_IMAGE_INFO * ppredimg2 = NULL ) ;
	// �����⏞�p�����[�^���N���A����
	void ClearMovingVector( void ) ;
protected:
	// �����⏞�x�N�g�����v�Z���A�\���摜�Ƃ̍������v�Z����
	int PredictMovingVector
		( const EGL_IMAGE_INFO & nextimg,
			const EGL_IMAGE_INFO & previmg,
			int xBlock, int yBlock, EGL_POINT ptMoveVec[],
			int & nAbsMaxDiff, double & rDeflectBlock,
			PCEGL_IMAGE_INFO ppredimg = NULL ) ;
	// �����⏞�x�N�g�����v�Z����
	void SearchMovingVector
		( EGL_IMAGE_INFO & nextblock, EGL_IMAGE_INFO & predblock,
			const EGL_IMAGE_INFO & nextimg, const EGL_IMAGE_INFO & predimg,
						int xBlock, int yBlock, EGL_POINT & ptMoveVec ) ;
	// �摜�u���b�N�̓��΍��i���v�j�����߂�
	static long int CalcSumDeflectBlock( const EGL_IMAGE_INFO & imgblock ) ;
	// �摜�u���b�N�̍��̓�捇�v�����߂�
	static long int CalcSumSqrDifferenceBlock
		( const EGL_IMAGE_INFO & dstimg, const EGL_IMAGE_INFO & srcimg ) ;
	// �摜�u���b�N�̋P�x�𔼕��ɂ���
	static void MakeBlockValueHalf( const EGL_IMAGE_INFO & imgblock ) ;

protected:
	// �t���J���[�摜�̈��k
	ESLError EncodeLosslessImage
		( const EGL_IMAGE_INFO & imginf,
			ERISAEncodeContext & context, DWORD fdwFlags ) ;
	// �T���v�����O�e�[�u���̏�����
	void InitializeSamplingTable( void ) ;
	// ��������
	void DifferentialOperation( LONG nAllBlockLines, SBYTE * pNextLineBuf ) ;
	// �I�y���[�V�����R�[�h���擾
	DWORD DecideOperationCode
		( DWORD fdwFlags, LONG nAllBlockLines, SBYTE * pNextLineBuf ) ;
	// �J���[�I�y���[�V�����֐��Q
	void ColorOperation0000( void ) ;
	void ColorOperation0001( void ) ;
	void ColorOperation0010( void ) ;
	void ColorOperation0011( void ) ;
	void ColorOperation0100( void ) ;
	void ColorOperation0101( void ) ;
	void ColorOperation0110( void ) ;
	void ColorOperation0111( void ) ;
	void ColorOperation1000( void ) ;
	void ColorOperation1001( void ) ;
	void ColorOperation1010( void ) ;
	void ColorOperation1011( void ) ;
	void ColorOperation1100( void ) ;
	void ColorOperation1101( void ) ;
	void ColorOperation1110( void ) ;
	void ColorOperation1111( void ) ;
	// �O���C�摜�̃T���v�����O
	void SamplingGray8( void ) ;
	// RGB �摜(15�r�b�g)�̃T���v�����O
	void SamplingRGB16( void ) ;
	// RGB �摜�̃T���v�����O
	void SamplingRGB24( void ) ;
	// RGBA �摜�̃T���v�����O
	void SamplingRGBA32( void ) ;
	// �摜���T���v�����O����֐��ւ̃|�C���^���擾����
	virtual PTR_PROCEDURE GetLLSamplingFunc
		( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags ) ;

protected:
	// �t���J���[�摜�̈��k
	ESLError EncodeLossyImage
		( const EGL_IMAGE_INFO & imginf,
			ERISAEncodeContext & context, DWORD fdwFlags ) ;
	// �u���b�N�P�ʂł̉�ʃT�C�Y���v�Z����
	void CalcImageSizeInBlocks( DWORD fdwTransformation ) ;
	// �T���v�����O�e�[�u���̏�����
	void InitializeZigZagTable( void ) ;
	// �ʎq���e�[�u���̐���
	void InitializeQuantumizeTable( double r = 1.0 ) ;
	// �}�N���u���b�N�̃T���v�����O�i4:4:4 �`���j���F��ԕϊ�
	void SamplingMacroBlock
		( int xBlock, int yBlock,
			int nLeftWidth, int nLeftHeight,
			DWORD dwBlockStepAddr, BYTE *& ptrSrcLineAddr,
			PTR_PROCEDURE pfnSamplingFunc ) ;
	// ���[�̈�ɕ��ϒl��ݒ�
	void FillBlockOddArea( DWORD dwFlags ) ;
	// 4:4:4 �X�P�[�����O
	void BlockScaling444( void ) ;
	// 4:1:1 �X�P�[�����O
	void BlockScaling411( void ) ;
	// DCT �ϊ����{��
	void MatrixDCT8x8( void ) ;
	// LOT �ϊ����{��
	void MatrixLOT8x8( REAL32 * ptrVertBufLOT ) ;
	// �ʎq�����{��
	void ArrangeAndQuantumize( SBYTE * ptrCoefficient, DWORD fdwFlags ) ;
	// �摜���T���v�����O����֐��ւ̃|�C���^���擾����
	virtual PTR_PROCEDURE GetLSSamplingFunc
		( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags ) ;

} ;


#endif
