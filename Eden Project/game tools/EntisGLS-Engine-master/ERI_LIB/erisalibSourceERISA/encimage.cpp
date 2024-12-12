
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2004 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>
#include <math.h>

 #if	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                            �摜���k�I�u�W�F�N�g
 *****************************************************************************/

#define	FREE_BUFFER(buf)	\
	if ( buf != NULL ) {  ::eslHeapFree( NULL, buf ) ;  buf = NULL ;  }

const ERISAEncoder::PTR_PROCEDURE
	ERISAEncoder::m_pfnColorOperation[0x10] =
{
	&ERISAEncoder::ColorOperation0000,
/*	&ERISAEncoder::ColorOperation0001,
	&ERISAEncoder::ColorOperation0010,
	&ERISAEncoder::ColorOperation0011,
	&ERISAEncoder::ColorOperation0100,*/
		&ERISAEncoder::ColorOperation0000,
		&ERISAEncoder::ColorOperation0000,
		&ERISAEncoder::ColorOperation0000,
		&ERISAEncoder::ColorOperation0000,
	&ERISAEncoder::ColorOperation0101,
	&ERISAEncoder::ColorOperation0110,
	&ERISAEncoder::ColorOperation0111,
//	&ERISAEncoder::ColorOperation1000,
		&ERISAEncoder::ColorOperation0000,
	&ERISAEncoder::ColorOperation1001,
	&ERISAEncoder::ColorOperation1010,
	&ERISAEncoder::ColorOperation1011,
//	&ERISAEncoder::ColorOperation1100,
		&ERISAEncoder::ColorOperation0000,
	&ERISAEncoder::ColorOperation1101,
	&ERISAEncoder::ColorOperation1110,
	&ERISAEncoder::ColorOperation1111
} ;

ERISAEncoder::PARAMETER::PARAMETER( void )
{
	m_dwFlags = 0 ;
	m_rYScaleDC = 0.5F ;
	m_rCScaleDC = 0.3F ;
	m_rYScaleLow = 0.25F ;
	m_rCScaleLow = 0.2F ;
	m_rYScaleHigh = 0.25F ;
	m_rCScaleHigh = 0.2F ;
	m_nYThreshold = 0 ;
	m_nCThreshold = 0 ;
	m_nYLPFThreshold = 64 ;
	m_nCLPFThreshold = 64 ;
	m_nAMDFThreshold = 0x120 ;
	m_rPFrameScale = 1.0F ;
	m_rBFrameScale = 0.9F ;
	m_nMaxFrameSize = 0 ;
	m_nMinFrameSize = 0 ;
}

void ERISAEncoder::PARAMETER::LoadPresetParam
	( ERISAEncoder::PresetParameter ppIndex, ERI_INFO_HEADER & infhdr )
{
	struct	PRESET_PARAMETER
	{
		DWORD		dwFlags ;
		DWORD		fdwTransformation ;
		REAL32		rYScaleDC ;
		REAL32		rYScaleLow ;
		REAL32		rYScaleHigh ;
		REAL32		rCScaleDC ;
		REAL32		rCScaleLow ;
		REAL32		rCScaleHigh ;
		REAL32		rPFrameScale ;
		REAL32		rBFrameScale ;
		ULONG		nMaxFrameSize ;
		ULONG		nMinFrameSize ;
		int			nYThreshold ;
		int			nCThreshold ;
		int			nYLPFThreshold ;
		int			nCLPFThreshold ;
		int			nAMDFThreshold ;
		DWORD		dwSamplingFlags ;
	} ;
	static const PRESET_PARAMETER	preset[ppMax] =
	{
		{	// ���t���k
			0, CVTYPE_DCT_ERI,
			0.5F, 0.3F, 0.2F,
			0.35F, 0.25F, 0.2F,
			1.5F, 1.0F, 0, 0,
			0, 0, 64, 64,
			0x120, ERISF_YUV_4_4_4
		},
		{	// ���掿�P
			0, CVTYPE_DCT_ERI,
			0.25F, 0.06F, 0.03F,
			0.2F, 0.05F, 0.025F,
			1.5F, 1.0F, 0, 0,
			0, 0, 64, 64,
			0x120, ERISF_YUV_4_1_1
		},
		{	// ���掿�Q
			pfLocalLoopFilter,
			CVTYPE_DCT_ERI,
			0.2F, 0.05F, 0.025F,
			0.18F, 0.04F, 0.02F,
			1.2F, 0.85F, 0, 0,
			1, 1, 63, 63,
			0x120, ERISF_YUV_4_1_1
		},
		{	// �W���P
			pfUseLoopFilter | pfLocalLoopFilter,
			CVTYPE_DCT_ERI,
			0.15F, 0.04F, 0.02F,
			0.1F, 0.025F, 0.01F,
			1.2F, 0.85F, 8000, 6000,
			1, 1, 43, 43,
			0x140, ERISF_YUV_4_1_1
		},
		{	// �W���Q
			pfUseLoopFilter | pfLocalLoopFilter,
			CVTYPE_DCT_ERI,
			0.15F, 0.03F, 0.02F,
			0.1F, 0.02F, 0.01F,
			1.0F, 0.7F, 6000, 4000,
			1, 1, 36, 36,
			0x180, ERISF_YUV_4_1_1
		},
		{	// ��掿�P
			pfUseLoopFilter | pfLocalLoopFilter,
			CVTYPE_LOT_ERI,
			0.12F, 0.02F, 0.01F,
			0.09F, 0.02F, 0.01F,
			0.9F, 0.6F, 4000, 3000,
			1, 1, 28, 28,
			0x180, ERISF_YUV_4_1_1
		},
		{	// ��掿�Q
			pfUseLoopFilter | pfLocalLoopFilter,
			CVTYPE_LOT_ERI,
			0.12F, 0.02F, 0.01F,
			0.09F, 0.02F, 0.01F,
			0.9F, 0.6F, 3000, 2000,
			2, 2, 28, 28,
			0x180, ERISF_YUV_4_1_1
		}
	} ;
	//
	m_dwFlags = preset[ppIndex].dwFlags ;
	m_rYScaleDC = preset[ppIndex].rYScaleDC ;
	m_rYScaleLow = preset[ppIndex].rYScaleLow ;
	m_rYScaleHigh = preset[ppIndex].rYScaleHigh ;
	m_rCScaleDC = preset[ppIndex].rCScaleDC ;
	m_rCScaleLow = preset[ppIndex].rCScaleLow ;
	m_rCScaleHigh = preset[ppIndex].rCScaleHigh ;
	m_nYThreshold = preset[ppIndex].nYThreshold ;
	m_nCThreshold = preset[ppIndex].nCThreshold ;
	m_nYLPFThreshold = preset[ppIndex].nYLPFThreshold ;
	m_nCLPFThreshold = preset[ppIndex].nCLPFThreshold ;
	m_nAMDFThreshold = preset[ppIndex].nAMDFThreshold ;
	m_rPFrameScale = preset[ppIndex].rPFrameScale ;
	m_rBFrameScale = preset[ppIndex].rBFrameScale ;
	m_nMaxFrameSize = preset[ppIndex].nMaxFrameSize ;
	m_nMinFrameSize = preset[ppIndex].nMinFrameSize ;
	//
	infhdr.dwVersion = 0x00020300 ;
	infhdr.fdwTransformation = preset[ppIndex].fdwTransformation ;
	infhdr.dwArchitecture = ERI_RUNLENGTH_GAMMA ;
	infhdr.dwSamplingFlags = preset[ppIndex].dwSamplingFlags ;
	infhdr.dwBlockingDegree = 3 ;
}

// �N���X���
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ERISAEncoder, ESLObject )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERISAEncoder::ERISAEncoder( void )
{
	m_ptrColumnBuf = NULL ;
	m_ptrLineBuf = NULL ;
	m_ptrEncodeBuf = NULL ;
	m_ptrArrangeBuf = NULL ;
	m_pArrangeTable[0] = NULL ;
	m_ptrVertBufLOT = NULL ;
	m_ptrHorzBufLOT = NULL ;
	m_ptrBlocksetBuf[0] = NULL ;
	m_ptrMatrixBuf[0] = NULL ;
	m_pQuantumizeScale[0] = NULL ;
	m_pQuantumizeTable = NULL ;
	m_nMovingVector = 0 ;
	m_pMoveVecFlags = NULL ;
	m_pMovingVector = NULL ;
	m_fPredFrameType = 0 ;
	m_nIntraBlockCount = 0 ;
	m_rDiffDeflectBlock = 0 ;
	m_rMaxDeflectBlock = 0 ;
	m_ptrCoefficient = NULL ;
	m_ptrImageDst = NULL ;
	m_ptrSignalBuf = NULL ;
	m_pHuffmanTree = NULL ;
	m_pProbERISA = NULL ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERISAEncoder::~ERISAEncoder( void )
{
	Delete( ) ;
}

// �������i�p�����[�^�̐ݒ�j
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAEncoder::Initialize( const ERI_INFO_HEADER & infhdr )
{
	//
	// �ȑO�̃f�[�^������
	//
	Delete( ) ;
	//
	// �摜���w�b�_���R�s�[
	//
	m_eihInfo = infhdr ;
	//
	// �p�����[�^�̃`�F�b�N
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		//
		// �t���k
		//////////////////////////////////////////////////////////////////////
		if ( (m_eihInfo.dwArchitecture != ERI_RUNLENGTH_GAMMA)
				&& (m_eihInfo.dwArchitecture != ERI_RUNLENGTH_HUFFMAN)
				&& (m_eihInfo.dwArchitecture != ERISA_NEMESIS_CODE) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̈��k�t�H�[�}�b�g�j
		}
		//
		switch ( (m_eihInfo.fdwFormatType & ERI_TYPE_MASK) )
		{
		case	ERI_RGB_IMAGE:
			if ( m_eihInfo.dwBitsPerPixel <= 8 )
				m_nChannelCount = 1 ;
			else if ( !(m_eihInfo.fdwFormatType & ERI_WITH_ALPHA) )
				m_nChannelCount = 3 ;
			else
				m_nChannelCount = 4 ;
			break ;

		case	ERI_GRAY_IMAGE:
			m_nChannelCount = 1 ;
			break;

		default:
			return	eslErrGeneral ;		// �G���[�i���Ή��̉摜�t�H�[�}�b�g�j
		}
		//
		// �e�萔���v�Z
		//
		if ( m_eihInfo.dwBlockingDegree == 0 )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̉摜�t�H�[�}�b�g�j
		}
		//
		m_nBlockSize = (UINT) (1 << m_eihInfo.dwBlockingDegree) ;
		m_nBlockArea = (ULONG) (1 << (m_eihInfo.dwBlockingDegree * 2)) ;
		m_nBlockSamples = m_nBlockArea * m_nChannelCount ;
		m_nWidthBlocks =
			(ULONG) ((m_eihInfo.nImageWidth + m_nBlockSize - 1)
								>> m_eihInfo.dwBlockingDegree) ;
		if ( m_eihInfo.nImageHeight < 0 )
		{
			m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
		}
		else
		{
			m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
		}
		m_nHeightBlocks =
			(m_nHeightBlocks + m_nBlockSize - 1)
								>> m_eihInfo.dwBlockingDegree ;
		//
		// ���[�L���O���������m��
		//
		m_ptrColumnBuf =
			(SBYTE*) ::eslHeapAllocate
				( NULL, m_nBlockSize * m_nChannelCount, 0 ) ;
		m_ptrLineBuf =
			(SBYTE*) ::eslHeapAllocate( NULL,
				m_nChannelCount *
					(m_nWidthBlocks << m_eihInfo.dwBlockingDegree), 0 ) ;
		m_ptrEncodeBuf =
			(SBYTE*) ::eslHeapAllocate( NULL, m_nBlockSamples, 0 ) ;
		m_ptrArrangeBuf =
			(SBYTE*) ::eslHeapAllocate( NULL, m_nBlockSamples, 0 ) ;
		//
		// �o�[�W�����̃`�F�b�N�ƃT���v�����O�e�[�u���̏���
		//
		InitializeSamplingTable( ) ;
		//
		if ( m_eihInfo.dwVersion == 0x00020200 )
		{
			if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN )
			{
				m_pHuffmanTree = new ERINA_HUFFMAN_TREE ;
			}
			else if ( m_eihInfo.dwArchitecture == ERISA_NEMESIS_CODE )
			{
				m_pProbERISA = new ERISA_PROB_MODEL ;
			}
		}
		else if ( m_eihInfo.dwVersion != 0x00020100 )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃o�[�W�����j
		}
	}
	else if ( (m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI) )
	{
		//
		// ��t���k
		//////////////////////////////////////////////////////////////////////
		if ( m_eihInfo.dwArchitecture != ERI_RUNLENGTH_GAMMA )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̈��k�t�H�[�}�b�g�j
		}
		//
		switch ( (m_eihInfo.fdwFormatType & ERI_TYPE_MASK) )
		{
		case	ERI_RGB_IMAGE:
			if ( m_eihInfo.dwBitsPerPixel <= 8 )
				m_nChannelCount = 1 ;
			else if ( !(m_eihInfo.fdwFormatType & ERI_WITH_ALPHA) )
				m_nChannelCount = 3 ;
			else
				m_nChannelCount = 4 ;
			break ;

		case	ERI_GRAY_IMAGE:
			m_nChannelCount = 1 ;
			break;

		default:
			return	eslErrGeneral ;		// �G���[�i���Ή��̉摜�t�H�[�}�b�g�j
		}
		//
		// �e�萔���v�Z
		//
		if ( m_eihInfo.dwBlockingDegree != 3 )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̉摜�t�H�[�}�b�g�j
		}
		//
		m_nBlockSize = (UINT) (1 << m_eihInfo.dwBlockingDegree) ;
		m_nBlockArea = (ULONG) (1 << (m_eihInfo.dwBlockingDegree * 2)) ;
		m_nBlockSamples = m_nBlockArea * m_nChannelCount ;
		if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
		{
			m_nWidthBlocks =
				(ULONG) ((m_eihInfo.nImageWidth + m_nBlockSize * 2 - 1)
									>> (m_eihInfo.dwBlockingDegree + 1)) ;
			if ( m_eihInfo.nImageHeight < 0 )
			{
				m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
			}
			else
			{
				m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
			}
			m_nHeightBlocks =
				(m_nHeightBlocks + m_nBlockSize * 2 - 1)
								>> (m_eihInfo.dwBlockingDegree + 1) ;
			//
			m_nWidthBlocks += 1  ;
			m_nHeightBlocks += 1 ;
			//
			if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
			{
				m_nBlocksetCount = m_nChannelCount * 4 ;
			}
			else if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
			{
				switch ( m_nChannelCount )
				{
				case	1:
					m_nBlocksetCount = 4 ;
					break ;
				case	3:
					m_nBlocksetCount = 6 ;
					break ;
				case	4:
					m_nBlocksetCount = 10 ;
					break ;
				default:
					return	eslErrGeneral ;
				}
			}
			else
			{
				return	eslErrGeneral ;		// �G���[�i���Ή��̉摜�t�H�[�}�b�g�j
			}
		}
		else
		{
			m_nWidthBlocks =
				(ULONG) ((m_eihInfo.nImageWidth + (m_nBlockSize * 2 - 1))
									>> (m_eihInfo.dwBlockingDegree + 1)) ;
			if ( m_eihInfo.nImageHeight < 0 )
			{
				m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
			}
			else
			{
				m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
			}
			m_nHeightBlocks =
				(m_nHeightBlocks + (m_nBlockSize * 2 - 1))
								>> (m_eihInfo.dwBlockingDegree + 1) ;
			//
			if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
			{
				m_nBlocksetCount = m_nChannelCount * 4 ;
			}
			else if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
			{
				switch ( m_nChannelCount )
				{
				case	1:
					m_nBlocksetCount = 4 ;
					break ;
				case	3:
					m_nBlocksetCount = 6 ;
					break ;
				case	4:
					m_nBlocksetCount = 10 ;
					break ;
				default:
					return	eslErrGeneral ;
				}
			}
			else
			{
				return	eslErrGeneral ;		// �G���[�i���Ή��̉摜�t�H�[�}�b�g�j
			}
		}
		//
		// ���[�L���O���������m��
		//
		m_ptrEncodeBuf =
			(SBYTE*) ::eslHeapAllocate( NULL, m_nBlockSamples * 4, 0 ) ;
		m_ptrVertBufLOT =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockSamples * 2
					* m_nWidthBlocks * sizeof(REAL32), 0 ) ;
		m_ptrHorzBufLOT =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockSamples * 2 * sizeof(REAL32), 0 ) ;
		m_ptrBlocksetBuf[0] =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 36 * sizeof(REAL32), 0 ) ;
		m_ptrMatrixBuf[0] =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 16 * sizeof(REAL32), 0 ) ;
		m_pQuantumizeScale[0] =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 2 * sizeof(REAL32), 0 ) ;
		m_pQuantumizeTable =
			(BYTE*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 2 * sizeof(BYTE), 0 ) ;
		//
		DWORD	dwTotalBlocks = m_nWidthBlocks * m_nHeightBlocks ;
		m_nMovingVector = 0 ;
		m_pMoveVecFlags =
			(BYTE*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * sizeof(BYTE), 0 ) ;
		m_pMovingVector = 
			(SBYTE*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * 4 * sizeof(SBYTE), 0 ) ;
		m_ptrCoefficient =
			(SBYTE*) ::eslHeapAllocate( NULL, dwTotalBlocks * 2, 0 ) ;
		m_ptrImageDst =
			(BYTE*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * m_nBlockArea * m_nBlocksetCount, 0 ) ;
		m_ptrSignalBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * m_nBlockArea
							* m_nBlocksetCount * sizeof(REAL32), 0 ) ;
		//
		int	i ;
		for ( i = 1; i < 36; i ++ )
		{
			m_ptrBlocksetBuf[i] = m_ptrBlocksetBuf[0] + (m_nBlockArea * i) ;
		}
		for ( i = 1; i < 16; i ++ )
		{
			m_ptrMatrixBuf[i] = m_ptrMatrixBuf[0] + (m_nBlockArea * i) ;
		}
		m_pQuantumizeScale[1] = m_pQuantumizeScale[0] + m_nBlockArea ;
		//
		// �T���v�����O�e�[�u���̏���
		//
		InitializeZigZagTable( ) ;
		//
		m_pHuffmanTree = new ERINA_HUFFMAN_TREE ;
	}
	else
	{
		return	eslErrGeneral ;			// �G���[�i���Ή��̃t�H�[�}�b�g�j
	}
	//
	// ����I��
	//
	return	eslErrSuccess ;
}

// �I���i�������̉���Ȃǁj
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::Delete( void )
{
	FREE_BUFFER(m_ptrColumnBuf)
	FREE_BUFFER(m_ptrLineBuf)
	FREE_BUFFER(m_ptrEncodeBuf)
	FREE_BUFFER(m_ptrArrangeBuf)
	FREE_BUFFER(m_pArrangeTable[0])
	FREE_BUFFER(m_ptrVertBufLOT)
	FREE_BUFFER(m_ptrHorzBufLOT)
	FREE_BUFFER(m_ptrBlocksetBuf[0])
	FREE_BUFFER(m_ptrMatrixBuf[0])
	FREE_BUFFER(m_pQuantumizeScale[0])
	FREE_BUFFER(m_pQuantumizeTable)
	FREE_BUFFER(m_pMoveVecFlags)
	FREE_BUFFER(m_pMovingVector)
	FREE_BUFFER(m_ptrCoefficient)
	FREE_BUFFER(m_ptrImageDst)
	FREE_BUFFER(m_ptrSignalBuf)
	//
	m_nMovingVector = 0 ;
	//
	if ( m_pHuffmanTree != NULL )
	{
		delete	m_pHuffmanTree ;
		m_pHuffmanTree = NULL ;
	}
	if ( m_pProbERISA != NULL )
	{
		delete	m_pProbERISA ;
		m_pProbERISA = NULL ;
	}
}

// �摜�����k
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAEncoder::EncodeImage
	( const EGL_IMAGE_INFO & srcimginf,
		ERISAEncodeContext & context, DWORD fdwFlags )
{
	//
	// �o�͉摜�����R�s�[����
	//
	EGL_IMAGE_INFO	imginf = srcimginf ;
	if ( m_eihInfo.nImageHeight < 0 )
	{
		imginf.ptrImageArray =
			((PBYTE)imginf.ptrImageArray)
				+ (imginf.dwImageHeight - 1) * imginf.dwBytesPerLine ;
		imginf.dwBytesPerLine = - imginf.dwBytesPerLine ;
	}
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		// �t���k�t�H�[�}�b�g
		return	EncodeLosslessImage( imginf, context, fdwFlags ) ;
	}
	else if ( (m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI) )
	{
		// ��t���k�t�H�[�}�b�g
		return	EncodeLossyImage( imginf, context, fdwFlags ) ;
	}

	return	eslErrGeneral ;			// ���Ή��̃t�H�[�}�b�g
}

// ���k�I�v�V������ݒ�
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::SetCompressionParameter( const PARAMETER & prmCmprOpt )
{
	m_prmCmprOpt = prmCmprOpt ;
}

// �����⏞�p�����[�^���v�Z����
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAEncoder::ProcessMovingVector
	( const EGL_IMAGE_INFO & nextimginf,
		const EGL_IMAGE_INFO & previmginf,
		int & nAbsMaxDiff, const EGL_IMAGE_INFO * ppredimg2 )
{
	//
	// �摜���̌���
	//////////////////////////////////////////////////////////////////////////
	if ( (nextimginf.dwImageWidth != previmginf.dwImageWidth)
		|| (nextimginf.dwImageHeight != previmginf.dwImageHeight)
		|| (nextimginf.dwBitsPerPixel != previmginf.dwBitsPerPixel) )
	{
		return	eslErrGeneral ;
	}
	const int	nBlockSize = 16 ;
	ESLAssert( m_nBlockSize == 8 ) ;
	int		nImageWidth = nextimginf.dwImageWidth ;
	int		nImageHeight = nextimginf.dwImageHeight ;
	//
	if ( (nImageWidth != m_eihInfo.nImageWidth)
		|| ((nImageHeight != m_eihInfo.nImageHeight)
			&& (nImageHeight != - m_eihInfo.nImageHeight)) )
	{
		return	eslErrGeneral ;
	}
	//
	// �e�u���b�N���̓����x�N�g�����Z�o����
	//////////////////////////////////////////////////////////////////////////
	//
	int		x, y ;
	SBYTE *	ptrNextVector = m_pMovingVector ;
	BYTE *	ptrNextVecFlag = m_pMoveVecFlags ;
	int	nWidthBlocks = (nImageWidth + (nBlockSize - 1)) / nBlockSize ;
	int	nHeightBlocks = (nImageHeight + (nBlockSize - 1)) / nBlockSize ;
	double	rSumDiffDeflect = 0 ;
	m_fPredFrameType = (ppredimg2 != NULL) ? 2 : 1 ;
	nAbsMaxDiff = 0 ;
	m_nIntraBlockCount = 0 ;
	m_rMaxDeflectBlock = 0 ;
	//
	EGL_IMAGE_INFO	nextimg = nextimginf ;
	EGL_IMAGE_INFO	previmg = previmginf ;
	EGL_IMAGE_INFO	predimg ;
	if ( ppredimg2 != NULL )
	{
		predimg = *ppredimg2 ;
	}
	if ( m_eihInfo.nImageHeight < 0 )
	{
		nextimg.ptrImageArray =
			((BYTE*) nextimg.ptrImageArray)
				+ (nextimg.dwImageHeight - 1)
						* nextimg.dwBytesPerLine ;
		nextimg.dwBytesPerLine = - nextimg.dwBytesPerLine ;
		//
		previmg.ptrImageArray =
			((BYTE*) previmg.ptrImageArray)
				+ (previmg.dwImageHeight - 1)
						* previmg.dwBytesPerLine ;
		previmg.dwBytesPerLine = - previmg.dwBytesPerLine ;
		//
		if ( ppredimg2 != NULL )
		{
			predimg.ptrImageArray =
				((BYTE*) predimg.ptrImageArray)
					+ (predimg.dwImageHeight - 1)
							* predimg.dwBytesPerLine ;
			predimg.dwBytesPerLine = - predimg.dwBytesPerLine ;
			ppredimg2 = &predimg ;
		}
	}
	//
	for ( y = 0; y < nImageHeight; y += nBlockSize )
	{
		for ( x = 0; x < nImageWidth; x += nBlockSize )
		{
			//
			// �����⏞�x�N�g�����v�Z����
			//
			EGL_POINT	ptMoveVec[2] ;
			double	rDeflectBlock ;
			int		nPredType = PredictMovingVector
				( nextimg, previmg, x, y,
					ptMoveVec, nAbsMaxDiff, rDeflectBlock, ppredimg2 ) ;
			//
			m_nIntraBlockCount += (nPredType == 0) ;
			*(ptrNextVecFlag ++) = nPredType ;
			rSumDiffDeflect += rDeflectBlock ;
			if ( m_rMaxDeflectBlock < rDeflectBlock )
			{
				m_rMaxDeflectBlock = (REAL32) rDeflectBlock ;
			}
			ptrNextVector[0] = (SBYTE) ptMoveVec[0].x ;
			ptrNextVector[1] = (SBYTE) ptMoveVec[0].y ;
			ptrNextVector[2] = (SBYTE) ptMoveVec[1].x ;
			ptrNextVector[3] = (SBYTE) ptMoveVec[1].y ;
			ptrNextVector += 4 ;
		}
	}
	//
	// �e�u���b�N���Ƃ̓����x�N�g����������������
	//////////////////////////////////////////////////////////////////////////
	//
	// ������������
	//
	ptrNextVector = m_pMovingVector ;
	for ( y = 0; y < nHeightBlocks; y ++ )
	{
		for ( x = nWidthBlocks - 2; x >= 0; x -- )
		{
			ptrNextVector[x * 4 + 4] -= ptrNextVector[x * 4] ;
			ptrNextVector[x * 4 + 5] -= ptrNextVector[x * 4 + 1] ;
			ptrNextVector[x * 4 + 6] -= ptrNextVector[x * 4 + 2] ;
			ptrNextVector[x * 4 + 7] -= ptrNextVector[x * 4 + 3] ;
		}
		ptrNextVector += nWidthBlocks * 4 ;
	}
	//
	// ������������
	//
	SBYTE *	ptrLastVector =
		m_pMovingVector + (nHeightBlocks - 1) * nWidthBlocks * 4 ;
	ptrNextVector = ptrLastVector - nWidthBlocks * 4 ;
	for ( y = 1; y < nHeightBlocks; y ++ )
	{
		for ( x = 0; x < nWidthBlocks; x ++ )
		{
			ptrLastVector[x * 4]     -= ptrNextVector[x * 4] ;
			ptrLastVector[x * 4 + 1] -= ptrNextVector[x * 4 + 1] ;
			ptrLastVector[x * 4 + 2] -= ptrNextVector[x * 4 + 2] ;
			ptrLastVector[x * 4 + 3] -= ptrNextVector[x * 4 + 3] ;
		}
		ptrLastVector = ptrNextVector ;
		ptrNextVector -= nWidthBlocks * 4 ;
	}
	//
	m_nMovingVector = nWidthBlocks * nHeightBlocks ;
	m_rDiffDeflectBlock =
		(REAL32) (rSumDiffDeflect / (m_nMovingVector - m_nIntraBlockCount + 1)) ;
	//
	if ( m_nIntraBlockCount == m_nMovingVector )
	{
		nAbsMaxDiff = 0x7FFFFFFF ;
	}
	//
	return	eslErrSuccess ;
}

// �����⏞���v�Z���A�\���摜�Ƃ̍������v�Z����
//////////////////////////////////////////////////////////////////////////////
int ERISAEncoder::PredictMovingVector
	( const EGL_IMAGE_INFO & nextimg,
		const EGL_IMAGE_INFO & previmg,
		int xBlock, int yBlock, EGL_POINT ptMoveVec[],
		int & nAbsMaxDiff, double & rDeflectBlock, PCEGL_IMAGE_INFO ppredimg )
{
	ESLAssert( m_nBlockSize == 8 ) ;
	const int	nBlockSize = 16 ;
	EGL_IMAGE_INFO	nextblock ;
	EGL_IMAGE_INFO	predblock[2] ;
	DWORD			imgbuf[3][256] ;
	bool		fDiffBlock[3] = { true, true, false } ;	// �����u���b�N���H
	long int	nSumDeflect, nSumSqrDiff[3] ;			// �΍��l
	DWORD		dwSumAbsDiff[3] ;
	//
	// �ߋ��t���[������̓����x�N�g�����v�Z����
	//////////////////////////////////////////////////////////////////////////
	predblock[0].ptrImageArray = &(imgbuf[0][0]) ;
	SearchMovingVector
		( nextblock, predblock[0],
			nextimg, previmg, xBlock, yBlock, ptMoveVec[0] ) ;
	//
	// �����t���[���𗘗p���邩�Ɨ��t���[���𗘗p���邩����
	//
	rDeflectBlock = 0 ;
	nSumDeflect = CalcSumDeflectBlock( nextblock ) ;
	nSumSqrDiff[0] = CalcSumSqrDifferenceBlock( nextblock, predblock[0] ) ;
	dwSumAbsDiff[0] = ::eriSumAbsDifferenceOfBlock( nextblock, predblock[0] ) ;
	//
	rDeflectBlock +=
		sqrt( (double) nSumSqrDiff[0]
			/ (int) (nextblock.dwImageWidth * nextblock.dwImageHeight) ) ;
	//
	if ( (nSumSqrDiff[0] > 8192*64*3) ||
		((nSumDeflect <= nSumSqrDiff[0])
				&& (nSumSqrDiff[0] > 4096*64*3)) )
	{
		fDiffBlock[0] = false ;
	}
	//
	// �����t���[������̓����x�N�g�����v�Z����
	//////////////////////////////////////////////////////////////////////////
	EGL_IMAGE_INFO	halfblock ;
	if ( ppredimg != NULL )
	{
		predblock[1].ptrImageArray = &(imgbuf[1][0]) ;
		SearchMovingVector
			( nextblock, predblock[1],
				nextimg, *ppredimg, xBlock, yBlock, ptMoveVec[1] ) ;
		//
		// �����t���[���𗘗p���邩�Ɨ��t���[���𗘗p���邩����
		//
		nSumSqrDiff[1] =
			CalcSumSqrDifferenceBlock( nextblock, predblock[1] ) ;
		dwSumAbsDiff[1] =
			::eriSumAbsDifferenceOfBlock( nextblock, predblock[1] ) ;
		//
		rDeflectBlock +=
			sqrt( (double) nSumSqrDiff[1]
				/ (int) (nextblock.dwImageWidth * nextblock.dwImageHeight) ) ;
		rDeflectBlock *= 0.5 ;
		//
		if ( (nSumSqrDiff[1] > 8192*64*3) ||
			((nSumDeflect <= nSumSqrDiff[1])
					&& (nSumSqrDiff[1] > 4096*64*3)) )
		{
			fDiffBlock[1] = false ;
		}
		//
		if ( fDiffBlock[0] && fDiffBlock[1] )
		{
			//
			// �������\���u���b�N
			//
			::eslFillMemory( &halfblock, 0, sizeof(halfblock) ) ;
			halfblock.dwInfoSize = sizeof(EGL_IMAGE_INFO) ;
			halfblock.fdwFormatType = nextblock.fdwFormatType ;
			halfblock.dwImageWidth = nextblock.dwImageWidth ;
			halfblock.dwImageHeight = nextblock.dwImageHeight ;
			halfblock.dwBitsPerPixel = nextblock.dwBitsPerPixel ;
			halfblock.dwBytesPerLine =
				((halfblock.dwImageWidth
					* halfblock.dwBitsPerPixel / 8) + 0x03) & ~0x03 ;
			halfblock.dwSizeOfImage =
				halfblock.dwBytesPerLine * halfblock.dwImageHeight ;
			halfblock.ptrImageArray = &(imgbuf[2][0]) ;
			//
			::eriBlendHalfImage( halfblock, predblock[0], predblock[1] ) ;
			//
			nSumSqrDiff[2] =
				CalcSumSqrDifferenceBlock( nextblock, halfblock ) ;
			dwSumAbsDiff[2] =
				::eriSumAbsDifferenceOfBlock( nextblock, halfblock ) ;
			//
			if ( !((nSumSqrDiff[2] > 8192*64*3) ||
				((nSumDeflect <= nSumSqrDiff[2])
						&& (nSumSqrDiff[2] > 4096*64*3)))
				&& (dwSumAbsDiff[2] <= dwSumAbsDiff[0])
				&& (dwSumAbsDiff[2] <= dwSumAbsDiff[1])
				&& (dwSumAbsDiff[0] * 3 > dwSumAbsDiff[1])
				&& (dwSumAbsDiff[1] * 3 < dwSumAbsDiff[0]) )
			{
				fDiffBlock[0] = false ;
				fDiffBlock[1] = false ;
				fDiffBlock[2] = true ;
			}
			else //if ( fDiffBlock[0] && fDiffBlock[1] )
			{
				if ( dwSumAbsDiff[0] <= dwSumAbsDiff[1] )
				{
					fDiffBlock[1] = false ;
				}
				else
				{
					fDiffBlock[0] = false ;
				}
			}
		}
	}
	else
	{
		ptMoveVec[1].x = 0 ;
		ptMoveVec[1].y = 0 ;
		fDiffBlock[1] = false ;
	}
	if ( (xBlock + nBlockSize > (int) nextimg.dwImageWidth)
		|| (yBlock + nBlockSize > (int) nextimg.dwImageHeight) )
	{
		fDiffBlock[0] = false ;
		fDiffBlock[1] = false ;
		fDiffBlock[2] = false ;
	}
	//
	// �����摜�𐶐�����
	//////////////////////////////////////////////////////////////////////////
	PEGL_IMAGE_INFO	pRefBlock = NULL ;
	int				nPredType = 0 ;
	if ( fDiffBlock[2] )
	{
		// �o�����Q�ƃu���b�N
		pRefBlock = &halfblock ;
		nPredType = 2 ;
	}
	else if ( fDiffBlock[0] )
	{
		// �O�����Q�ƃu���b�N
		pRefBlock = &predblock[0] ;
		ptMoveVec[1].x = 0 ;
		ptMoveVec[1].y = 0 ;
		nPredType = 1 ;
	}
	else if ( fDiffBlock[1] )
	{
		// ������Q�ƃu���b�N
		pRefBlock = &predblock[1] ;
		ptMoveVec[0].x = 0 ;
		ptMoveVec[0].y = 0 ;
		nPredType = -1 ;
	}
	else
	{
		// �Ɨ��u���b�N
		MakeBlockValueHalf( nextblock ) ;
		return	0 ;
	}
	int	nAbsDiff = ::eriLSSubtractionOfFrame( nextblock, *pRefBlock ) ;
	if ( nAbsDiff > nAbsMaxDiff )
	{
		nAbsMaxDiff = nAbsDiff ;
	}
	return	nPredType ;
}

// �����⏞�x�N�g�����v�Z����
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::SearchMovingVector
	( EGL_IMAGE_INFO & nextblock, EGL_IMAGE_INFO & predblock,
		const EGL_IMAGE_INFO & nextimg, const EGL_IMAGE_INFO & predimg,
						int xBlock, int yBlock, EGL_POINT & ptMoveVec )
{
	//
	// �摜���̎擾
	//
	const int	nBlockSize = 16 ;
	ESLAssert( m_nBlockSize == 8 ) ;
	EGL_IMAGE_INFO	prvblock ;
	int		nImageWidth, nImageHeight, nNextPixelBytes, nPredPixelBytes ;
	nextblock = nextimg ;
	prvblock = predimg ;
	nImageWidth = nextimg.dwImageWidth ;
	nImageHeight = nextimg.dwImageHeight ;
	nNextPixelBytes = nextblock.dwBitsPerPixel / 8 ;
	nPredPixelBytes = prvblock.dwBitsPerPixel / 8 ;
	//
	// �u���b�N�̈�̃A�h���X���v�Z����
	//
	nextblock.dwImageWidth = nBlockSize ;
	nextblock.dwImageHeight = nBlockSize ;
	prvblock.dwImageWidth = nBlockSize ;
	prvblock.dwImageHeight = nBlockSize ;
	nextblock.dwSizeOfImage =
		nextblock.dwBytesPerLine * nBlockSize ;
	prvblock.dwSizeOfImage =
		prvblock.dwBytesPerLine * nBlockSize ;
	nextblock.ptrImageArray =
		((BYTE*) nextimg.ptrImageArray)
			+ xBlock * nNextPixelBytes
			+ yBlock * nextblock.dwBytesPerLine ;
	prvblock.ptrImageArray =
		((BYTE*) predimg.ptrImageArray)
			+ xBlock * nPredPixelBytes
			+ yBlock * prvblock.dwBytesPerLine ;
	//
	if ( (yBlock + nBlockSize > nImageHeight)
		|| (xBlock + nBlockSize > nImageWidth) )
	{
		//
		// ��ʒ[�̔��[�T�C�Y�̃u���b�N�͏������Ȃ�
		//
		ptMoveVec.x = 0 ;
		ptMoveVec.y = 0 ;
		//
		// �u���b�N���𐳋K������
		//
		if ( yBlock + nBlockSize > nImageHeight )
		{
			nextblock.dwImageHeight = nImageHeight - yBlock ;
			prvblock.dwImageHeight = nextblock.dwImageHeight ;
			nextblock.dwSizeOfImage =
				nextblock.dwBytesPerLine * nextblock.dwImageHeight ;
			prvblock.dwSizeOfImage =
				prvblock.dwBytesPerLine * prvblock.dwImageHeight ;
		}
		if ( xBlock + nBlockSize > nImageWidth )
		{
			nextblock.dwImageWidth = nImageWidth - xBlock ;
			prvblock.dwImageWidth = nextblock.dwImageWidth ;
		}
	}
	else
	{
		//
		// ��΍��̍��v���v�Z����
		//
		long int	nBaseDeflect =
			CalcSumSqrDifferenceBlock( nextblock, prvblock ) ;
		DWORD	dwBaseSAD =
			::eriSumAbsDifferenceOfBlock( nextblock, prvblock ) ;
		//
		// �̈�����߂Ȃ���œK�ȃx�N�g���̑I�����J��Ԃ�
		//
		static const EGL_POINT	ptVecList[8] =
		{
			{ -1, -1 }, {  0, -1 }, {  1, -1 },
			{ -1,  0 }, {  1,  0 },
			{ -1,  1 }, {  0,  1 }, {  1,  1 }
		} ;
		int	i, x, y ;
		EGL_POINT	ptBaseVec = { 0, 0 } ;
		EGL_POINT	ptBestVec ;
		DWORD		dwBestSAD ;
		long int	nBestDeflect = nBaseDeflect ;
		ptBestVec = ptBaseVec ;
		dwBestSAD = dwBaseSAD ;
		for ( y = -17; y <= 17; y += 2 )
		{
			for ( x = -17; x <= 17; x += 2 )
			{
				//
				// ��x�N�g���ƁA���̍����x�N�g���Ƃ��r����
				//
				EGL_POINT	ptDeltaVec ;
				ptDeltaVec.x = ptBaseVec.x + x ;
				ptDeltaVec.y = ptBaseVec.y + y ;
				//
				EGL_POINT	ptBlockPos = ptDeltaVec ;
				ptBlockPos.x += xBlock ;
				ptBlockPos.y += yBlock ;
				if ( ((ptBlockPos.x | ptBlockPos.y) < 0)
					|| (ptBlockPos.x + nBlockSize > nImageWidth)
					|| (ptBlockPos.y + nBlockSize > nImageHeight) )
				{
					continue ;
				}
				prvblock.ptrImageArray =
					((BYTE*) predimg.ptrImageArray)
						+ ptBlockPos.x * nPredPixelBytes
						+ ptBlockPos.y * prvblock.dwBytesPerLine ;
				//
				DWORD	dwDeltaSAD =
					::eriSumAbsDifferenceOfBlock
							( nextblock, prvblock ) ;
				//
				if ( dwDeltaSAD < dwBestSAD )
				{
					ptBestVec = ptDeltaVec ;
					dwBestSAD = dwDeltaSAD ;
				}
			}
		}
		if ( (ptBestVec.x != ptBaseVec.x)
			|| (ptBestVec.y != ptBaseVec.y) )
		{
			prvblock.ptrImageArray =
				((BYTE*) predimg.ptrImageArray)
					+ (xBlock + ptBestVec.x) * nPredPixelBytes
					+ (yBlock + ptBestVec.y) * prvblock.dwBytesPerLine ;
			long int	nDeltaDiff =
				CalcSumSqrDifferenceBlock( nextblock, prvblock ) ;
			if ( nDeltaDiff <= nBaseDeflect )
			{
				ptBaseVec = ptBestVec ;
				dwBaseSAD = dwBestSAD ;
				nBaseDeflect = nDeltaDiff ;
			}
		}
		ptMoveVec = ptBaseVec ;
		//
		// ����f���x�̓������o
		//
		DWORD		dwOptSAD = 0x7FFFFFFF ;
		EGL_POINT	ptOptVec = { 0, 0 } ;
		for ( i = 0; i < 8; i ++ )
		{
			EGL_POINT	ptBlockPos = ptMoveVec ;
			ptBlockPos.x += ptVecList[i].x + xBlock ;
			ptBlockPos.y += ptVecList[i].y + yBlock ;
			if ( ((ptBlockPos.x | ptBlockPos.y) < 0)
				|| (ptBlockPos.x + nBlockSize > nImageWidth)
				|| (ptBlockPos.y + nBlockSize > nImageHeight) )
			{
				continue ;
			}
			prvblock.ptrImageArray =
				((BYTE*) predimg.ptrImageArray)
					+ ptBlockPos.x * nPredPixelBytes
					+ ptBlockPos.y * prvblock.dwBytesPerLine ;
			//
			DWORD	dwDeltaSAD =
				::eriSumAbsDifferenceOfBlock
						( nextblock, prvblock ) ;
			//
			if ( dwDeltaSAD < dwOptSAD )
			{
				ptOptVec = ptVecList[i] ;
				dwOptSAD = dwDeltaSAD ;
			}
		}
		ptMoveVec.x *= 2 ;
		ptMoveVec.y *= 2 ;
		if ( dwOptSAD < dwBaseSAD * 5 / 4 )
		{
			ptMoveVec.x += ptOptVec.x ;
			ptMoveVec.y += ptOptVec.y ;
		}
	}
	//
	// �����x�N�g������
	//
	if ( ptMoveVec.x < -0x7F )
	{
		ptMoveVec.x = -0x7F ;
	}
	else if ( ptMoveVec.x > 0x7F )
	{
		ptMoveVec.x = 0x7F ;
	}
	if ( ptMoveVec.y < -0x7F )
	{
		ptMoveVec.y = -0x7F ;
	}
	else if ( ptMoveVec.y > 0x7F )
	{
		ptMoveVec.y = 0x7F ;
	}
	ESLAssert( xBlock + (ptMoveVec.x >> 1) >= 0 ) ;
	ESLAssert( yBlock + (ptMoveVec.y >> 1) >= 0 ) ;
	ESLAssert( xBlock + ((ptMoveVec.x + 1) >> 1)
				+ (int) prvblock.dwImageWidth <= nImageWidth ) ;
	ESLAssert( yBlock + ((ptMoveVec.y + 1) >> 1)
				+ (int) prvblock.dwImageHeight <= nImageHeight ) ;
	//
	// �摜����
	//
	EGL_IMAGE_INFO	prvblock2 ;
	predblock.dwInfoSize = sizeof(EGL_IMAGE_INFO) ;
	predblock.fdwFormatType = prvblock.fdwFormatType ;
	predblock.dwImageWidth = prvblock.dwImageWidth ;
	predblock.dwImageHeight = prvblock.dwImageHeight ;
	predblock.dwBitsPerPixel = prvblock.dwBitsPerPixel ;
	predblock.dwBytesPerLine = predblock.dwImageWidth * nPredPixelBytes ;
	predblock.dwSizeOfImage =
		predblock.dwImageHeight * predblock.dwBytesPerLine ;
	prvblock.ptrImageArray =
		((BYTE*) predimg.ptrImageArray)
			+ (xBlock + (ptMoveVec.x >> 1)) * nPredPixelBytes
			+ (yBlock + (ptMoveVec.y >> 1)) * prvblock.dwBytesPerLine ;
	//
	if ( ptMoveVec.y & 0x01 )
	{
		if ( ptMoveVec.x & 0x01 )
		{
			EGL_IMAGE_INFO	bufblock ;
			DWORD			buf[256] ;
			ESLAssert( m_nBlockSize == 8 ) ;
			//
			prvblock2 = prvblock ;
			prvblock2.ptrImageArray =
				((BYTE*)prvblock.ptrImageArray) + nPredPixelBytes ;
			::eriBlendHalfImage( predblock, prvblock, prvblock2 ) ;
			//
			bufblock = prvblock ;
			bufblock.ptrImageArray = buf ;
			bufblock.dwBytesPerLine =
				bufblock.dwImageWidth * nPredPixelBytes ;
			bufblock.dwSizeOfImage =
				bufblock.dwImageHeight * bufblock.dwBytesPerLine ;
			prvblock.ptrImageArray =
				((BYTE*)prvblock.ptrImageArray) + prvblock2.dwBytesPerLine ;
			prvblock2.ptrImageArray =
				((BYTE*)prvblock2.ptrImageArray) + prvblock2.dwBytesPerLine ;
			::eriBlendHalfImage( bufblock, prvblock, prvblock2 ) ;
			//
			::eriBlendHalfImage( predblock, predblock, bufblock ) ;
		}
		else
		{
			prvblock2 = prvblock ;
			prvblock2.ptrImageArray =
				((BYTE*)prvblock.ptrImageArray) + prvblock.dwBytesPerLine ;
			::eriBlendHalfImage( predblock, prvblock, prvblock2 ) ;
		}
	}
	else if ( ptMoveVec.x & 0x01 )
	{
		prvblock2 = prvblock ;
		prvblock2.ptrImageArray =
			((BYTE*)prvblock.ptrImageArray) + nPredPixelBytes ;
		::eriBlendHalfImage( predblock, prvblock, prvblock2 ) ;
	}
	else
	{
		::eriCopyImage( predblock, prvblock ) ;
	}
}

// �摜�u���b�N�̓��΍��i���v�j�����߂�
//////////////////////////////////////////////////////////////////////////////
long int ERISAEncoder::CalcSumDeflectBlock( const EGL_IMAGE_INFO & imgblock )
{
	int		n, x, y ;
	int		nSum[3] = { 0, 0, 0 } ;
	int		nAvg[3], nDeflect = 0 ;
	int		nWidth = imgblock.dwImageWidth ;
	int		nHeight = imgblock.dwImageHeight ;
	BYTE *	ptrNextLine = (BYTE*) imgblock.ptrImageArray ;
	int		nPixelBytes = imgblock.dwBitsPerPixel / 8 ;
	for ( y = 0; y < nHeight; y ++ )
	{
		BYTE *	ptrNextPixel = ptrNextLine ;
		for ( x = 0; x < nWidth; x ++ )
		{
			nSum[0] += (int) ptrNextPixel[0] ;
			nSum[1] += (int) ptrNextPixel[1] ;
			nSum[2] += (int) ptrNextPixel[2] ;
			ptrNextPixel += nPixelBytes ;
		}
		ptrNextLine += imgblock.dwBytesPerLine ;
	}
	//
	nAvg[0] = nSum[0] / (nHeight * nWidth) ;
	nAvg[1] = nSum[1] / (nHeight * nWidth) ;
	nAvg[2] = nSum[2] / (nHeight * nWidth) ;
	ptrNextLine = (BYTE*) imgblock.ptrImageArray ;
	//
	for ( y = 0; y < nHeight; y ++ )
	{
		BYTE *	ptrNextPixel = ptrNextLine ;
		for ( x = 0; x < nWidth; x ++ )
		{
			n = (int) ptrNextPixel[0] - nAvg[0] ;
			nDeflect += n * n ;
			n = (int) ptrNextPixel[1] - nAvg[1] ;
			nDeflect += n * n ;
			n = (int) ptrNextPixel[2] - nAvg[2] ;
			nDeflect += n * n ;
			ptrNextPixel += nPixelBytes ;
		}
		ptrNextLine += imgblock.dwBytesPerLine ;
	}
	//
	return	nDeflect ;
}

// �摜�u���b�N�̍��̓��΍��i���v�j�����߂�
//////////////////////////////////////////////////////////////////////////////
long int ERISAEncoder::CalcSumSqrDifferenceBlock
	( const EGL_IMAGE_INFO & dstimg, const EGL_IMAGE_INFO & srcimg )
{
	int		n, x, y ;
	int		nSum[3] = { 0, 0, 0 } ;
	int		nAvg[3], nDeflect = 0 ;
	int		nWidth = dstimg.dwImageWidth ;
	int		nHeight = dstimg.dwImageHeight ;
	BYTE *	ptrDstNextLine = (BYTE*) dstimg.ptrImageArray ;
	BYTE *	ptrSrcNextLine = (BYTE*) srcimg.ptrImageArray ;
	int		nDstPixelBytes = dstimg.dwBitsPerPixel / 8 ;
	int		nSrcPixelBytes = srcimg.dwBitsPerPixel / 8 ;
	//
	for ( y = 0; y < nHeight; y ++ )
	{
		BYTE *	ptrDstNextPixel = ptrDstNextLine ;
		BYTE *	ptrSrcNextPixel = ptrSrcNextLine ;
		for ( x = 0; x < nWidth; x ++ )
		{
			nSum[0] += (int) ptrDstNextPixel[0] - (int) ptrSrcNextPixel[0] ;
			nSum[1] += (int) ptrDstNextPixel[1] - (int) ptrSrcNextPixel[1] ;
			nSum[2] += (int) ptrDstNextPixel[2] - (int) ptrSrcNextPixel[2] ;
			ptrDstNextPixel += nDstPixelBytes ;
			ptrSrcNextPixel += nSrcPixelBytes ;
		}
	}
	//
	nAvg[0] = nSum[0] / (nHeight * nWidth) ;
	nAvg[1] = nSum[1] / (nHeight * nWidth) ;
	nAvg[2] = nSum[2] / (nHeight * nWidth) ;
	ptrDstNextLine = (BYTE*) dstimg.ptrImageArray ;
	ptrSrcNextLine = (BYTE*) srcimg.ptrImageArray ;
	//
	for ( y = 0; y < nHeight; y ++ )
	{
		BYTE *	ptrDstNextPixel = ptrDstNextLine ;
		BYTE *	ptrSrcNextPixel = ptrSrcNextLine ;
		for ( x = 0; x < nWidth; x ++ )
		{
			n = (int) ptrDstNextPixel[0] - (int) ptrSrcNextPixel[0] - nAvg[0] ;
			nDeflect += n * n ;
			n = (int) ptrDstNextPixel[1] - (int) ptrSrcNextPixel[1] - nAvg[1] ;
			nDeflect += n * n ;
			n = (int) ptrDstNextPixel[2] - (int) ptrSrcNextPixel[2] - nAvg[2] ;
			nDeflect += n * n ;
			ptrDstNextPixel += nDstPixelBytes ;
			ptrSrcNextPixel += nSrcPixelBytes ;
		}
	}
	return	nDeflect ;
}

// �摜�u���b�N�̋P�x�𔼕��ɂ���
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::MakeBlockValueHalf( const EGL_IMAGE_INFO & imgblock )
{
	int		x, y ;
	int		nWidth = imgblock.dwImageWidth ;
	int		nHeight = imgblock.dwImageHeight ;
	BYTE *	ptrNextLine = (BYTE*) imgblock.ptrImageArray ;
	int		nPixelBytes = imgblock.dwBitsPerPixel / 8 ;
	for ( y = 0; y < nHeight; y ++ )
	{
		if ( nPixelBytes == 32 )
		{
			DWORD *	pdwNextPixel = (DWORD*) ptrNextLine ;
			for ( x = 0; x < nWidth; x ++ )
			{
				*pdwNextPixel = (*pdwNextPixel >> 1) & 0x7F7F7F7F ;
				pdwNextPixel ++ ;
			}
		}
		else
		{
			BYTE *	ptrNextPixel = ptrNextLine ;
			for ( x = 0; x < nWidth; x ++ )
			{
				ptrNextPixel[0] >>= 1 ;
				ptrNextPixel[1] >>= 1 ;
				ptrNextPixel[2] >>= 1 ;
				ptrNextPixel += nPixelBytes ;
			}
		}
		ptrNextLine += imgblock.dwBytesPerLine ;
	}
}

// �����⏞�p�����[�^���N���A����
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::ClearMovingVector( void )
{
	m_nMovingVector = 0 ;
}

// �W�J�i�s�󋵒ʒm�֐�
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAEncoder::OnEncodedBlock( LONG line, LONG column )
{
	return	eslErrSuccess ;
}

// �t���J���[�摜�̈��k
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAEncoder::EncodeLosslessImage
	( const EGL_IMAGE_INFO & imginf,
		ERISAEncodeContext & context, DWORD fdwFlags )
{
	//
	// �֐��̏���
	//////////////////////////////////////////////////////////////////////////
	//
	// �T���v�����O�֐��擾
	//
	PTR_PROCEDURE	pfnSamplingFunc ;
	m_nSrcLineBytes = imginf.dwBytesPerLine ;
	m_nSrcPixelBytes = imginf.dwBitsPerPixel >> 3 ;
	pfnSamplingFunc = GetLLSamplingFunc
		( imginf.fdwFormatType, imginf.dwBitsPerPixel, fdwFlags ) ;
	if ( pfnSamplingFunc == NULL )
	{
		return	eslErrGeneral ;
	}
	//
	// �R���e�L�X�g���������� ERI �w�b�_���o��
	//
	if ( m_eihInfo.dwArchitecture == ERISA_NEMESIS_CODE )
	{
		//
		// ERISA �t�H�[�}�b�g
		//
		context.OutNBits( 0x10000008UL, 32 ) ;
		context.OutNBits( 0, 1 ) ;
		m_pProbERISA->Initialize( ) ;
		context.PrepareToEncodeERISACode( ) ;
	}
	else if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN )
	{
		//
		// ERINA �t�H�[�}�b�g
		//
		context.OutNBits( 0x08000008UL, 32 ) ;
		context.OutNBits( 0, 1 ) ;
		m_pHuffmanTree->Initialize( ) ;
		context.PrepareToEncodeERINACode( ) ;
	}
	else
	{
		//
		// ERI �݊��t�H�[�}�b�g
		//
		ESLAssert( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA ) ;
		context.OutNBits( 0x01000000UL, 32 ) ;
		context.OutNBits( 0, 1 ) ;
		context.PrepareToEncodeGammaCode( ) ;
	}
	//
	// �e�u���b�N�����k���ďo�͂��锽������
	//////////////////////////////////////////////////////////////////////////
	//
	// ���C���o�b�t�@���N���A
	//
	LONG	nWidthSamples = m_nChannelCount * m_nWidthBlocks * m_nBlockSize ;
	::eslFillMemory( m_ptrLineBuf, 0, nWidthSamples ) ;
	//
	LONG	nPosX, nPosY ;
	LONG	nAllBlockLines = (LONG) (m_nBlockSize * m_nChannelCount) ;
	LONG	nLeftHeight = (LONG) imginf.dwImageHeight ;
	//
	for ( nPosY = 0; nPosY < (LONG) m_nHeightBlocks; nPosY ++ )
	{
		//
		// �J�����o�b�t�@���N���A
		//
		LONG	nColumnBufSamples = (LONG) (m_nBlockSize * m_nChannelCount) ;
		::eslFillMemory( m_ptrColumnBuf, 0, nColumnBufSamples ) ;
		//
		// �s�̎��O����
		//
		m_ptrSrcBlock =
			((BYTE*)imginf.ptrImageArray)
				+ (nPosY * imginf.dwBytesPerLine * m_nBlockSize) ;
		m_nSrcHeight = m_nBlockSize ;
		if ( (LONG) m_nSrcHeight > nLeftHeight )
		{
			m_nSrcHeight = nLeftHeight ;
		}
		//
		LONG	nLeftWidth = (LONG) imginf.dwImageWidth ;
		SBYTE *	ptrNextLineBuf = m_ptrLineBuf ;
		//
		for ( nPosX = 0; nPosX < (LONG) m_nWidthBlocks; nPosX ++ )
		{
			//
			// �u���b�N���T���v�����O
			//
			m_nSrcWidth = m_nBlockSize ;
			if ( (LONG) m_nSrcWidth > nLeftWidth )
			{
				m_nSrcWidth = nLeftWidth ;
			}
			if ( (m_nSrcHeight < m_nBlockSize)
					|| (m_nSrcWidth < m_nBlockSize) )
			{
				::eslFillMemory( m_ptrArrangeBuf, 0, m_nBlockSamples ) ;
			}
			(this->*pfnSamplingFunc)( ) ;
			//
			if ( m_nChannelCount >= 3 )
			{
				//
				// �I�y���[�V�����R�[�h���擾
				//
				DWORD	dwOpCode =
					DecideOperationCode
						( fdwFlags, nAllBlockLines, ptrNextLineBuf ) ;
				ptrNextLineBuf += nColumnBufSamples ;
				//
				// �I�y���[�V�����R�[�h���o��
				//
				if ( m_eihInfo.dwArchitecture == ERISA_NEMESIS_CODE )
				{
					// ERISA �t�H�[�}�b�g
					context.EncodeERISACodeSymbol( m_pProbERISA, (BYTE) dwOpCode ) ;
				}
				else if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN )
				{
					// ERINA �t�H�[�}�b�g
					context.OutHuffmanCode( m_pHuffmanTree, (BYTE) dwOpCode ) ;
				}
				else
				{
					// ERI �݊��t�H�[�}�b�g
					ESLAssert( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA ) ;
					context.OutNBits( (dwOpCode << 28), 4 ) ;
				}
			}
			else
			{
				if ( m_eihInfo.fdwFormatType == ERI_GRAY_IMAGE )
				{
					DifferentialOperation( nAllBlockLines, ptrNextLineBuf ) ;
					ptrNextLineBuf += nColumnBufSamples ;
				}
			}
			//
			// �f�[�^�𕄍���
			//
			if ( context.EncodeSymbolBytes
					( m_ptrEncodeBuf, m_nBlockSamples ) < m_nBlockSamples )
			{
				return	eslErrGeneral ;		// �G���R�[�h�Ɏ��s
			}
			//
			// ���k�̏󋵂�ʒm
			//
			ESLError	errContinue ;
			errContinue = OnEncodedBlock( nPosY, nPosX ) ;
			if ( errContinue )
			{
				return	errContinue ;
			}
			//
			// ���̃u���b�N��
			//
			m_ptrSrcBlock += m_nSrcPixelBytes * m_nBlockSize ;
			nLeftWidth -= (LONG) m_nBlockSize ;
		}
		//
		nLeftHeight -= (LONG) m_nBlockSize ;
	}
	//
	// �I��
	//
	if ( context.FinishEncoding( ) )
	{
		return	eslErrGeneral ;
	}
	//
	return	eslErrSuccess ;
}

// �T���v�����O�e�[�u���̏�����
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::InitializeSamplingTable( void )
{
	unsigned int	i, j, k, l, m, n ;
	//
	// �T���v�����O�e�[�u���p�o�b�t�@�m��
	//
	PINT	ptrTable =
		(PINT) ::eslHeapAllocate
			( NULL, m_nBlockSamples * 4 * sizeof(INT), 0 ) ;
	PINT	ptrNext ;
	m_pArrangeTable[0] = ptrTable ;
	m_pArrangeTable[1] = ptrTable + m_nBlockSamples ;
	m_pArrangeTable[2] = ptrTable + m_nBlockSamples * 2 ;
	m_pArrangeTable[3] = ptrTable + m_nBlockSamples * 3 ;
	//
	// ������������
	//
	ptrNext = m_pArrangeTable[0] ;
	for ( i = 0; i < m_nBlockSamples; i ++ )
	{
		ptrNext[i] = i ;
	}
	//
	// ������������
	//
	ptrNext = m_pArrangeTable[1] ;
	l = 0 ;
	for ( i = 0; i < m_nChannelCount; i ++ )
	{
		for ( j = 0; j < m_nBlockSize; j ++ )
		{
			m = l + j ;
			for ( k = 0; k < m_nBlockSize; k ++ )
			{
				*(ptrNext ++) = m ;
				m += m_nBlockSize ;
			}
		}
		l += m_nBlockArea ;
	}
	//
	// ���������C���^�[���[�u
	//
	ptrNext = m_pArrangeTable[2] ;
	for ( i = 0; i < m_nChannelCount; i ++ )
	{
		k = i ;
		for ( j = 0; j < m_nBlockArea; j ++ )
		{
			*(ptrNext ++) = k ;
			k += m_nChannelCount ;
		}
	}
	//
	// ���������C���^�[���[�u
	//
	ptrNext = m_pArrangeTable[3] ;
	n = m_nBlockSize * m_nChannelCount ;
	for ( i = 0; i < m_nChannelCount; i ++ )
	{
		l = i ;
		for ( j = 0; j < m_nBlockSize; j ++ )
		{
			m = l ;
			l += m_nChannelCount ;
			for ( k = 0; k < m_nBlockSize; k ++ )
			{
				*(ptrNext ++) = m ;
				m += n ;
			}
		}
	}
}

// ��������
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::DifferentialOperation
	( LONG nAllBlockLines, SBYTE * pNextLineBuf )
{
	INT		i, j, k ;
	SBYTE	* ptrLineBuf, * ptrNextBuf, * ptrNextColBuf ;
	//
	// �������������s�i���������j
	//
	ptrLineBuf = pNextLineBuf ;
	ptrNextBuf = m_ptrEncodeBuf ;
	for ( k = 0; k < (INT) m_nChannelCount; k ++ )
	{
		for ( i = 0; i < (INT) m_nBlockSize; i ++ )
		{
			for ( j = 0; j < (INT) m_nBlockSize; j ++ )
			{
				SBYTE	nCurVal = *ptrNextBuf ;
				*(ptrNextBuf ++) -= ptrLineBuf[j] ;
				ptrLineBuf[j] = nCurVal ;
			}
		}
		ptrLineBuf += m_nBlockSize ;
	}
	//
	// �������������s�i���������j
	//
	ptrNextBuf = m_ptrEncodeBuf ;
	ptrNextColBuf = m_ptrColumnBuf ;
	for ( i = 0; i < nAllBlockLines; i ++ )
	{
		SBYTE	nRightVal = ptrNextBuf[m_nBlockSize - 1] ;
		j = m_nBlockSize ;
		while ( -- j )
		{
			ptrNextBuf[j] -= ptrNextBuf[j - 1] ;
		}
		ptrNextBuf[0] -= ptrNextColBuf[i] ;
		ptrNextBuf += m_nBlockSize ;
		ptrNextColBuf[i] = nRightVal ;
	}
}

// �I�y���[�V�����R�[�h���擾
//////////////////////////////////////////////////////////////////////////////
DWORD ERISAEncoder::DecideOperationCode
	( DWORD fdwFlags, LONG nAllBlockLines, SBYTE * pNextLineBuf )
{
	unsigned int	i, j, k ;
	int		nBestSize, nTrySize ;
	//
	// �œK�ȍ��������R�[�h��I��
	//
	int		iBestDifOp ;
	int		nCompressMode = fdwFlags & efCmprModeMask ;
	if ( (nCompressMode >= efNormalCmpr)
		|| (m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA) )
	{
		//
		// ���������̑I���͂��Ȃ�
		//
		iBestDifOp = 0x03 ;
		DifferentialOperation( nAllBlockLines, pNextLineBuf ) ;
		nBestSize = ERISAEncodeContext::EstimateGammaCodeBytes
							( m_ptrEncodeBuf, m_nBlockSamples ) ;
	}
	else // if ( nCompressMode <= efHighCmpr )
	{
		//
		// �������������s�i���������j
		//
		SBYTE	* ptrLineBuf, * ptrNextBuf, * ptrNextColBuf ;
		ptrLineBuf = pNextLineBuf ;
		ptrNextBuf = m_ptrEncodeBuf ;
		for ( k = 0; k < (INT) m_nChannelCount; k ++ )
		{
			for ( i = 0; i < (INT) m_nBlockSize; i ++ )
			{
				for ( j = 0; j < (INT) m_nBlockSize; j ++ )
				{
					SBYTE	nCurVal = *ptrNextBuf ;
					*(ptrNextBuf ++) -= ptrLineBuf[j] ;
					ptrLineBuf[j] = nCurVal ;
				}
			}
			ptrLineBuf += m_nBlockSize ;
		}
		//
		iBestDifOp = 0x02 ;
		nBestSize = ERISAEncodeContext::EstimateGammaCodeBytes
							( m_ptrEncodeBuf, m_nBlockSamples ) ;
		//
		// ���ԏ�Ԃ�ۑ�
		//
		::eslMoveMemory( m_ptrArrangeBuf, m_ptrEncodeBuf, m_nBlockSamples ) ;
		//
		// �������������s�i���������j
		//
		ptrNextBuf = m_ptrEncodeBuf ;
		ptrNextColBuf = m_ptrColumnBuf ;
		for ( i = 0; i < (ULONG) nAllBlockLines; i ++ )
		{
			SBYTE	nRightVal = ptrNextBuf[m_nBlockSize - 1] ;
			j = m_nBlockSize ;
			while ( -- j )
			{
				ptrNextBuf[j] -= ptrNextBuf[j - 1] ;
			}
			ptrNextBuf[0] -= ptrNextColBuf[i] ;
			ptrNextBuf += m_nBlockSize ;
			ptrNextColBuf[i] = nRightVal ;
		}
		//
		nTrySize = ERISAEncodeContext::EstimateGammaCodeBytes
							( m_ptrEncodeBuf, m_nBlockSamples ) ;
		if ( nTrySize < nBestSize )
		{
			iBestDifOp = 0x03 ;
			nBestSize = nTrySize ;
		}
		else
		{
			::eslMoveMemory
				( m_ptrEncodeBuf, m_ptrArrangeBuf, m_nBlockSamples ) ;
		}
	}
	//
	if ( nCompressMode == efLowCmpr )
	{
		// �J���[�I�y���[�V�����͎g�p���Ȃ�
		return	(DWORD) (iBestDifOp << 6) ;
	}
	//
	// �A���t�@�`���l�����R�s�[����
	//
	if ( m_nChannelCount >= 4 )
	{
		SBYTE *	ptrSrc = m_ptrEncodeBuf + m_nBlockArea * 3 ;
		SBYTE *	ptrDst = m_ptrArrangeBuf + m_nBlockArea * 3 ;
		::eslMoveMemory( ptrDst, ptrSrc, m_nBlockArea ) ;
	}
	//
	// �œK�ȃJ���[�I�y���[�V������I��
	//
/*	static const int	iClrOpFullEx[15] =
	{
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	} ;*/
	static const int	iClrOpFull[9] =
	{
		5, 6, 7, 9, 10, 11, 13, 14, 15
	} ;
	static const int	iClrOpHalf[3] =
	{
		7, 11, 15
	} ;
	int		iBestClrOp = 0 ;
	//
	const int *		pClrOp ;
	unsigned int	nClrOpCount ;
	if ( nCompressMode >= 1 )
	{
		pClrOp = iClrOpHalf ;
		nClrOpCount = 3 ;
	}
	else // if ( nCompressMode == 0 )
	{
//		if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA )
		{
			pClrOp = iClrOpFull ;
			nClrOpCount = 9 ;
		}
/*		else
		{
			pClrOp = iClrOpFullEx ;
			nClrOpCount = 15 ;
		}*/
	}
	for ( i = 0; i < nClrOpCount; i ++ )
	{
		(this->*m_pfnColorOperation[pClrOp[i]])( ) ;
		//
		nTrySize =
			ERISAEncodeContext::EstimateGammaCodeBytes
					( m_ptrArrangeBuf, m_nBlockSamples ) ;
		if ( nTrySize < nBestSize )
		{
			iBestClrOp = pClrOp[i] ;
			nBestSize = nTrySize ;
		}
	}
	//
	// �J���[�I�y���[�V���������s
	//
	(this->*m_pfnColorOperation[iBestClrOp])( ) ;
	//
	// �œK�ȃA�����W�R�[�h��I��
	//
	int		iBestArrange = 0 ;
	PINT	pArrange ;
	if ( (nCompressMode == efNormalCmpr)
		|| (m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA) )
	{
		//
		// �A�����W�R�[�h�͎g��Ȃ�
		//
	}
	else if ( nCompressMode == efHighCmpr )
	{
		//
		// �C���^�[���[�u�̗L����������
		//
		pArrange = m_pArrangeTable[2] ;
		for ( j = 0; j < (INT) m_nBlockSamples; j ++ )
		{
			m_ptrEncodeBuf[pArrange[j]] = m_ptrArrangeBuf[j] ;
		}
		nTrySize =
			ERISAEncodeContext::EstimateGammaCodeBytes
					( m_ptrEncodeBuf, m_nBlockSamples ) ;
		if ( nTrySize < nBestSize )
		{
			nBestSize = nTrySize ;
			iBestArrange |= 0x02 ;
		}
		//
		// ���������̗L����������
		//
		pArrange = m_pArrangeTable[iBestArrange | 0x01] ;
		for ( j = 0; j < (INT) m_nBlockSamples; j ++ )
		{
			m_ptrEncodeBuf[pArrange[j]] = m_ptrArrangeBuf[j] ;
		}
		nTrySize =
			ERISAEncodeContext::EstimateGammaCodeBytes
					( m_ptrEncodeBuf, m_nBlockSamples ) ;
		if ( nTrySize < nBestSize )
		{
			nBestSize = nTrySize ;
			iBestArrange |= 0x01 ;
		}
	}
	else // if ( nCompressMode == efBestCmpr )
	{
		//
		// ��������
		//
		for ( i = 1; i < 4; i ++ )
		{
			pArrange = m_pArrangeTable[i] ;
			for ( j = 0; j < (INT) m_nBlockSamples; j ++ )
			{
				m_ptrEncodeBuf[pArrange[j]] = m_ptrArrangeBuf[j] ;
			}
			nTrySize =
				ERISAEncodeContext::EstimateGammaCodeBytes
						( m_ptrEncodeBuf, m_nBlockSamples ) ;
			//
			if ( nTrySize < nBestSize )
			{
				nBestSize = nTrySize ;
				iBestArrange = i ;
			}
		}
	}
	pArrange = m_pArrangeTable[iBestArrange] ;
	for ( i = 0; i < m_nBlockSamples; i ++ )
	{
		m_ptrEncodeBuf[pArrange[i]] = m_ptrArrangeBuf[i] ;
	}
	//
	// �I�y���[�V�����R�[�h��Ԃ�
	//
	return	(DWORD) (iBestDifOp << 6) | (iBestArrange << 4) | iBestClrOp ;
}

// �J���[�I�y���[�V�����֐��Q
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::ColorOperation0000( void )
{
	::eslMoveMemory( m_ptrArrangeBuf, m_ptrEncodeBuf, m_nBlockArea * 3 ) ;
}

void ERISAEncoder::ColorOperation0001( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[0] = ptrSrc[0] ;
		ptrDst[nChSamples] = ptrSrc[nChSamples] - nBase ;
		ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[0] -= ptrDst[nChSamples] ;
		ptrSrc ++ ;
		ptrDst ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation0010( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[nChSamples] -= ptrDst[0] ;
		ptrSrc ++ ;
		ptrDst ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation0011( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		ptrDst[nChSamples * 2] -= ptrDst[0] ;
		ptrSrc ++ ;
		ptrDst ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation0100( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[0] = ptrSrc[0] ;
		ptrDst[nChSamples] = ptrSrc[nChSamples] - nBase ;
		ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[0] -= ptrDst[nChSamples * 2] ;
		ptrSrc ++ ;
		ptrDst ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation0101( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[0] = ptrSrc[0] ;
		ptrDst[nChSamples] = ptrSrc[nChSamples] - nBase ;
		(ptrDst ++)[nChSamples * 2] = (ptrSrc ++)[nChSamples * 2] ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation0110( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[0] = ptrSrc[0] ;
		ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		(ptrDst ++)[nChSamples * 2] = (ptrSrc ++)[nChSamples * 2] - nBase ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation0111( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[0] = ptrSrc[0] ;
		ptrDst[nChSamples] = ptrSrc[nChSamples] - nBase ;
		(ptrDst ++)[nChSamples * 2] = (ptrSrc ++)[nChSamples * 2] - nBase ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1000( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[nChSamples] -= ptrDst[nChSamples * 2] ;
		ptrSrc ++ ;
		ptrDst ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1001( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		(ptrDst ++)[nChSamples * 2] = (ptrSrc ++)[nChSamples * 2] ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1010( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		ptrDst[0] = ptrSrc[0] ;
		(ptrDst ++)[nChSamples * 2] = (ptrSrc ++)[nChSamples * 2] - nBase ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1011( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		(ptrDst ++)[nChSamples * 2] = (ptrSrc ++)[nChSamples * 2] - nBase ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1100( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		ptrDst[nChSamples] = ptrSrc[nChSamples] ;
		ptrDst[nChSamples * 2] -= ptrDst[nChSamples] ;
		ptrSrc ++ ;
		ptrDst ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1101( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		(ptrDst ++)[nChSamples] = (ptrSrc ++)[nChSamples] ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1110( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[0] = ptrSrc[0] ;
		(ptrDst ++)[nChSamples] = (ptrSrc ++)[nChSamples] - nBase ;
	}
	while ( -- nRepCount ) ;
}

void ERISAEncoder::ColorOperation1111( void )
{
	SBYTE *	ptrSrc = m_ptrEncodeBuf ;
	SBYTE *	ptrDst = m_ptrArrangeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	SBYTE	nBase ;
	//
	do
	{
		nBase = ptrDst[nChSamples * 2] = ptrSrc[nChSamples * 2] ;
		ptrDst[0] = ptrSrc[0] - nBase ;
		(ptrDst ++)[nChSamples] = (ptrSrc ++)[nChSamples] - nBase ;
	}
	while ( -- nRepCount ) ;
}

// �O���C�摜�̃T���v�����O
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::SamplingGray8( void )
{
	BYTE *	ptrSrcLine = m_ptrSrcBlock ;
	SBYTE *	ptrDstLine = m_ptrEncodeBuf ;
	//
	for ( UINT y = 0; y < m_nSrcHeight; y ++ )
	{
		for ( UINT x = 0; x < m_nSrcWidth; x ++ )
		{
			ptrDstLine[x] = (SBYTE) ptrSrcLine[x] ;
		}
		ptrSrcLine += m_nSrcLineBytes ;
		ptrDstLine += m_nBlockSize ;
	}
}

// RGB �摜(15�r�b�g)�̃T���v�����O
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::SamplingRGB16( void )
{
	BYTE *	ptrSrcLine = m_ptrSrcBlock ;
	SBYTE *	ptrDstLine = m_ptrEncodeBuf ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	//
	for ( UINT y = 0; y < m_nSrcHeight; y ++ )
	{
		WORD *	ptrSrcNext = (WORD*) ptrSrcLine ;
		SBYTE *	ptrDstNext = ptrDstLine ;
		//
		for ( UINT x = 0; x < m_nSrcWidth; x ++ )
		{
			WORD	wSrcPixel = *(ptrSrcNext ++) ;
			*ptrDstNext = wSrcPixel & 0x1F ;
			ptrDstNext[nBlockSamples] = (wSrcPixel >> 5) & 0x1F ;
			ptrDstNext[nBlockSamples * 2] = (wSrcPixel >> 10) & 0x1F ;
			ptrDstNext ++ ;
		}
		ptrSrcLine += m_nSrcLineBytes ;
		ptrDstLine += m_nBlockSize ;
	}
}

// RGB �摜�̃T���v�����O
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::SamplingRGB24( void )
{
	BYTE *	ptrSrcLine = m_ptrSrcBlock ;
	SBYTE *	ptrDstLine = m_ptrEncodeBuf ;
	UINT	nBytesPerPixel = m_nSrcPixelBytes ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	//
	for ( UINT y = 0; y < m_nSrcHeight; y ++ )
	{
		BYTE *	ptrSrcNext = ptrSrcLine ;
		SBYTE *	ptrDstNext = ptrDstLine ;
		//
		for ( UINT x = 0; x < m_nSrcWidth; x ++ )
		{
			ptrDstNext[0] = ptrSrcNext[0] ;
			ptrDstNext[nBlockSamples] = ptrSrcNext[1] ;
			ptrDstNext[nBlockSamples * 2] = ptrSrcNext[2] ;
			ptrSrcNext += nBytesPerPixel ;
			ptrDstNext ++ ;
		}
		ptrSrcLine += m_nSrcLineBytes ;
		ptrDstLine += m_nBlockSize ;
	}
}

// RGBA �摜�̃T���v�����O
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::SamplingRGBA32( void )
{
	BYTE *	ptrSrcLine = m_ptrSrcBlock ;
	SBYTE *	ptrDstLine = m_ptrEncodeBuf ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	LONG	nBlockSamplesX3 = nBlockSamples * 3 ;
	//
	for ( UINT y = 0; y < m_nSrcHeight; y ++ )
	{
		BYTE *	ptrSrcNext = ptrSrcLine ;
		SBYTE *	ptrDstNext = ptrDstLine ;
		//
		for ( UINT x = 0; x < m_nSrcWidth; x ++ )
		{
			ptrDstNext[0] = ptrSrcNext[0] ;
			ptrDstNext[nBlockSamples] = ptrSrcNext[1] ;
			ptrDstNext[nBlockSamples * 2] = ptrSrcNext[2] ;
			ptrDstNext[nBlockSamplesX3] = ptrSrcNext[3] ;
			ptrSrcNext += 4 ;
			ptrDstNext ++ ;
		}
		ptrSrcLine += m_nSrcLineBytes ;
		ptrDstLine += m_nBlockSize ;
	}
}

// �摜���T���v�����O����֐��ւ̃|�C���^���擾����
//////////////////////////////////////////////////////////////////////////////
ERISAEncoder::PTR_PROCEDURE ERISAEncoder::GetLLSamplingFunc
	( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags )
{
	switch ( dwBitsPerPixel )
	{
	case	32:
		if ( m_eihInfo.fdwFormatType == ERI_RGBA_IMAGE )
		{
			return	&ERISAEncoder::SamplingRGBA32 ;
		}
	case	24:
		return	&ERISAEncoder::SamplingRGB24 ;
	case	16:
		return	&ERISAEncoder::SamplingRGB16 ;
	case	8:
		return	&ERISAEncoder::SamplingGray8 ;
	}
	return	NULL ;
}

// �t���J���[�摜�̈��k
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAEncoder::EncodeLossyImage
	( const EGL_IMAGE_INFO & imginf,
		ERISAEncodeContext & context, DWORD fdwFlags )
{
	//
	// �֐��̏���
	//////////////////////////////////////////////////////////////////////////
	//
	// LOT �ϊ��EDCT �ϊ��̐؂�ւ�
	//
	DWORD	fdwOrgTrans = m_eihInfo.fdwTransformation ;
	CalcImageSizeInBlocks
		( (fdwFlags & efDifferential) ? CVTYPE_DCT_ERI : fdwOrgTrans ) ;
	//
	// �T���v�����O�֐��̎擾
	//
	PTR_PROCEDURE	pfnSamplingFunc ;
	m_nSrcLineBytes = imginf.dwBytesPerLine ;
	m_nSrcPixelBytes = imginf.dwBitsPerPixel >> 3 ;
	m_fdwEncFlags = fdwFlags ;
	pfnSamplingFunc = GetLSSamplingFunc
		( imginf.fdwFormatType, imginf.dwBitsPerPixel, fdwFlags ) ;
	if ( pfnSamplingFunc == NULL )
	{
		return	eslErrGeneral ;
	}
	//
	// �ʎq���e�[�u���̐���
	//
	InitializeQuantumizeTable( ) ;
	//
	// �o�b�t�@�̏�����
	//
	::eslFillMemory
		( m_ptrVertBufLOT, 0,
			m_nBlockSamples * 2 * m_nWidthBlocks * sizeof(REAL32) ) ;
	//
	// �e�u���b�N���Ƃ̏���
	//////////////////////////////////////////////////////////////////////////
	INT		i, j, k ;
	LONG	nPosX, nPosY ;
	LONG	nLeftHeight = (LONG) imginf.dwImageHeight ;
	DWORD	dwBlockStepAddr = imginf.dwBytesPerLine * m_nBlockSize ;
	SBYTE *	ptrCoefficient = m_ptrCoefficient ;
	BYTE *	ptrImageDst = m_ptrImageDst ;
	REAL32	rMatrixScale = (REAL32) (2.0 / (LONG) m_nBlockSize) ;
	REAL32 *	ptrNextSignalBuf = m_ptrSignalBuf ;
	//
	for ( nPosY = 0; nPosY < (LONG) m_nHeightBlocks; nPosY ++ )
	{
		//
		// LOT �ϊ��o�b�t�@���N���A
		//
		::eslFillMemory
			( m_ptrHorzBufLOT, 0, m_nBlockSamples * 2 * sizeof(REAL32) ) ;
		//
		BYTE *	ptrSrcLineAddr =
			((BYTE*)imginf.ptrImageArray) + (nPosY * dwBlockStepAddr * 2) ;
		LONG	nLeftWidth = (LONG) imginf.dwImageWidth ;
		REAL32 *	ptrVertBufLOT = m_ptrVertBufLOT ;
		//
		for ( nPosX = 0; nPosX < (LONG) m_nWidthBlocks; nPosX ++ )
		{
			//
			// �u���b�N���T���v�����O�i4:4:4 �`���Łj���F��ԕϊ�
			//
			SamplingMacroBlock
				( nPosX, nPosY, nLeftWidth, nLeftHeight,
					dwBlockStepAddr, ptrSrcLineAddr, pfnSamplingFunc ) ;
			//
			// �X�P�[�����O
			//
			if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
			{
				BlockScaling444( ) ;
			}
			else // if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
			{
				BlockScaling411( ) ;
			}
			//
			// LOT �ϊ�
			//
			if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
			{
				MatrixLOT8x8( ptrVertBufLOT ) ;
			}
			else
			{
				MatrixDCT8x8( ) ;
			}
			::eriScalarMultiply
				( m_ptrBlocksetBuf[0],
					rMatrixScale, m_nBlockArea * m_nBlocksetCount ) ;
			ptrVertBufLOT += m_nBlockArea * 2 * m_nChannelCount ;
			//
			::eslMoveMemory
				( ptrNextSignalBuf, m_ptrBlocksetBuf[0],
					m_nBlockArea * m_nBlocksetCount * sizeof(REAL32) ) ;
			ptrNextSignalBuf += m_nBlockArea * m_nBlocksetCount ;
			//
			// �ʎq��
			//
			DWORD	fdwBlockFlags = fdwFlags ;
			if ( m_nMovingVector > 0 )
			{
				if ( m_pMoveVecFlags[nPosY * m_nWidthBlocks + nPosX] == 0 )
				{
					fdwBlockFlags &= ~efDifferential ;
				}
			}
			ArrangeAndQuantumize( ptrCoefficient, fdwBlockFlags ) ;
			for ( i = 0; i < (int) m_nBlockArea; i ++ )
			{
				for ( j = 0; j < (int) m_nBlocksetCount; j ++ )
				{
					ptrImageDst[i * m_nBlocksetCount + j] =
							m_ptrEncodeBuf[i + j * m_nBlockArea] ;
				}
			}
			ptrCoefficient += 2 ;
			ptrImageDst += m_nBlockArea * m_nBlocksetCount ;
			//
			// ���k�̏󋵂�ʒm
			//
			ESLError	errContinue ;
			errContinue = OnEncodedBlock( nPosY, nPosX ) ;
			if ( errContinue )
			{
				return	errContinue ;
			}
			//
			// ���̃u���b�N��
			//
			nLeftWidth -= (LONG) m_nBlockSize * 2 ;
		}
		//
		nLeftHeight -= (LONG) m_nBlockSize * 2 ;
	}
	//
	// ���[�g����
	//////////////////////////////////////////////////////////////////////////
	LONG	nTotalBlocks = m_nWidthBlocks * m_nHeightBlocks ;
	ULONG	nTotalSamples = nTotalBlocks * m_nBlockArea * m_nBlocksetCount ;
	//
	double	rRatio = 1.0 ;
	if ( m_prmCmprOpt.m_nMinFrameSize < m_prmCmprOpt.m_nMaxFrameSize )
	{
		do
		{
			//
			// ���[�g����
			//
			ULONG	nEstimatedSize =
				context.EstimateGammaCodeBytes
					( (const SBYTE *) m_ptrImageDst, nTotalSamples ) / 8 ;
			//
			double	r ;
			if ( m_prmCmprOpt.m_nMinFrameSize > nEstimatedSize )
			{
				r = pow( (double) (long int)
						m_prmCmprOpt.m_nMinFrameSize
								/ (long int) nEstimatedSize, 0.85 ) ;
				if ( r >= 32.0 )
				{
					r = 32.0 ;
				}
			}
			else if ( m_prmCmprOpt.m_nMaxFrameSize < nEstimatedSize )
			{
				r = pow( (double) (long int)
						m_prmCmprOpt.m_nMaxFrameSize
								/ (long int) nEstimatedSize, 0.8 ) ;
				if ( r <= 0.125 )
				{
					r = 0.125 ;
				}
			}
			else
			{
				break ;
			}
			//
			// �ėʎq���X�e�b�v
			//
			rRatio *= r ;
			ptrCoefficient = m_ptrCoefficient ;
			ptrImageDst = m_ptrImageDst ;
			ptrNextSignalBuf = m_ptrSignalBuf ;
			InitializeQuantumizeTable( rRatio ) ;
			//
			for ( k = 0; k < (int) nTotalBlocks; k ++ )
			{
				::eslMoveMemory
					( m_ptrBlocksetBuf[0], ptrNextSignalBuf,
						m_nBlockArea * m_nBlocksetCount * sizeof(REAL32) ) ;
				ptrNextSignalBuf += m_nBlockArea * m_nBlocksetCount ;
				//
				DWORD	fdwBlockFlags = fdwFlags ;
				if ( m_nMovingVector > 0 )
				{
					if ( m_pMoveVecFlags[k] == 0 )
					{
						fdwBlockFlags &= ~efDifferential ;
					}
				}
				ArrangeAndQuantumize( ptrCoefficient, fdwBlockFlags ) ;
				for ( i = 0; i < (int) m_nBlockArea; i ++ )
				{
					for ( j = 0; j < (int) m_nBlocksetCount; j ++ )
					{
						ptrImageDst[i * m_nBlocksetCount + j] =
								m_ptrEncodeBuf[i + j * m_nBlockArea] ;
					}
				}
				ptrCoefficient += 2 ;
				ptrImageDst += m_nBlockArea * m_nBlocksetCount ;
			}
		}
		while ( false ) ;
	}
	//
	// ���������ďo��
	//////////////////////////////////////////////////////////////////////////
	//
	// �R���e�L�X�g���������� ERI �w�b�_���o��
	//
	DWORD	dwHeader = 0 ;
	if ( m_nMovingVector > 0 )
	{
		// �����⏞
		dwHeader |= 0x00010000UL ;
		//
		if ( m_fPredFrameType == 2 )
		{
			// �����t���[�����Q��
			dwHeader |= 0x00020000UL ;
		}
	}
	//
	// ���[�v�t�B���^����
	//
	if ( m_prmCmprOpt.m_dwFlags & pfUseLoopFilter )
	{
		dwHeader |= 0x00040000UL ;
	}
	else if ( m_prmCmprOpt.m_dwFlags & pfLocalLoopFilter )
	{
		dwHeader |= 0x00080000UL ;
	}
/*	if ( !(fdwFlags & efDifferential) )
	{
		if ( (m_prmCmprOpt.m_dwFlags & pfUseLoopFilter) && (rRatio <= 2.0) )
		{
			dwHeader |= 0x00040000UL ;
		}
	}
	else
	{
		if ( (m_nIntraBlockCount * 4 > m_nMovingVector)
				&& (m_prmCmprOpt.m_dwFlags & pfUseLoopFilter) )
		{
			dwHeader |= 0x00040000UL ;
		}
		else if ( m_fPredFrameType == 2 )
		{
			if ( (m_prmCmprOpt.m_dwFlags & pfUseLoopFilter)
					&& (m_rDiffDeflectBlock > 14.5)
					&& (m_rMaxDeflectBlock >= 128.0) )
			{
				dwHeader |= 0x00040000UL ;
			}
		}
		else
		{
			if ( (m_prmCmprOpt.m_dwFlags & pfUseLoopFilter)
					&& (m_rDiffDeflectBlock > 15.5)
					&& (m_rMaxDeflectBlock >= 160.0) )
			{
				dwHeader |= 0x00040000UL ;
			}
		}
		if ( !(dwHeader & 0x00040000UL)
			&& (m_prmCmprOpt.m_dwFlags & pfLocalLoopFilter) )
		{
			dwHeader |= 0x00080000UL ;
		}
	}*/
	if ( m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI )
	{
		dwHeader |= 0x00000100UL ;
	}
	ESLAssert( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA ) ;
	dwHeader |= 0x28000008UL ;
	context.OutNBits( dwHeader, 32 ) ;
	context.OutNBits( 0, 1 ) ;
	m_pHuffmanTree->Initialize( ) ;
	context.PrepareToEncodeERINACode( context.efERINAOrder0 ) ;
	//
	// �t�ʎq���e�[�u�����o�͂���
	//
	for ( i = 0; i < (int) m_nBlockArea * 2; i ++ )
	{
		if ( context.OutHuffmanCode
			( m_pHuffmanTree, (BYTE) m_pQuantumizeTable[i] ) )
		{
			return	eslErrGeneral ;
		}
	}
	//
	// �u���b�N�X�P�[�����O�W�����o�͂���
	//
	if ( context.EncodeGammaCodeBytes
		( m_ptrCoefficient, nTotalBlocks * 2 ) < (ULONG) nTotalBlocks * 2 )
	{
		return	eslErrGeneral ;
	}
	//
	// �����⏞�x�N�g�����o�͂���
	//
	if ( m_nMovingVector > 0 )
	{
		if ( context.EncodeGammaCodeBytes
			( (SBYTE*) m_pMoveVecFlags, m_nMovingVector )
								< (ULONG) m_nMovingVector )
		{
			return	eslErrGeneral ;
		}
		if ( context.EncodeGammaCodeBytes
			( m_pMovingVector, m_nMovingVector * 4 )
							< (ULONG) m_nMovingVector * 4 )
		{
			return	eslErrGeneral ;
		}
		m_nMovingVector = 0 ;
		m_nIntraBlockCount = 0 ;
		m_rDiffDeflectBlock = 0 ;
		m_rMaxDeflectBlock = 0 ;
	}
	//
	// �摜�M�����o�͂���
	//
	if ( context.EncodeERINACodeBytes
		( (const SBYTE *) m_ptrImageDst, nTotalSamples ) < nTotalSamples )
	{
		return	eslErrGeneral ;
	}
	if ( context.FinishEncoding( ) )
	{
		return	eslErrGeneral ;
	}
	m_eihInfo.fdwTransformation = fdwOrgTrans ;
	//
	return	eslErrSuccess ;
}

// �u���b�N�P�ʂł̉�ʃT�C�Y���v�Z����
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::CalcImageSizeInBlocks( DWORD fdwTransformation )
{
	m_eihInfo.fdwTransformation = fdwTransformation ;
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
	{
		m_nWidthBlocks =
			(ULONG) ((m_eihInfo.nImageWidth + m_nBlockSize - 1)
								>> m_eihInfo.dwBlockingDegree) ;
		if ( m_eihInfo.nImageHeight < 0 )
		{
			m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
		}
		else
		{
			m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
		}
		m_nHeightBlocks =
			(m_nHeightBlocks + m_nBlockSize - 1)
								>> m_eihInfo.dwBlockingDegree ;
		//
		if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
		{
			m_nBlocksetCount = m_nChannelCount * 4 ;
			m_nWidthBlocks = (m_nWidthBlocks >> 1) + 1  ;
			m_nHeightBlocks = (m_nHeightBlocks >> 1) + 1 ;
		}
		else if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
		{
			m_nWidthBlocks = ((m_nWidthBlocks + 1) >> 1) + 1  ;
			m_nHeightBlocks = ((m_nHeightBlocks + 1) >> 1) + 1 ;
		}
	}
	else
	{
		m_nWidthBlocks =
			(ULONG) ((m_eihInfo.nImageWidth + (m_nBlockSize * 2 - 1))
								>> (m_eihInfo.dwBlockingDegree + 1)) ;
		if ( m_eihInfo.nImageHeight < 0 )
		{
			m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
		}
		else
		{
			m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
		}
		m_nHeightBlocks =
			(m_nHeightBlocks + (m_nBlockSize * 2 - 1))
							>> (m_eihInfo.dwBlockingDegree + 1) ;
	}
}

// �T���v�����O�e�[�u���̏�����
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::InitializeZigZagTable( void )
{
	PINT	ptrArrange = (PINT)
		::eslHeapAllocate( NULL, m_nBlockArea * 2 * sizeof(INT), 0 ) ;
	m_pArrangeTable[0] = ptrArrange ;
	m_pArrangeTable[1] = ptrArrange + m_nBlockArea ;
	//
	unsigned int	i = 0 ;
	INT		x = 0, y = 0 ;
	for ( ; ; )
	{
		for ( ; ; )
		{
			ptrArrange[i ++] = x + y * m_nBlockSize ;
			if ( i >= m_nBlockArea )
				goto	Label_Next ;
			++ x ;
			-- y ;
			if ( x >= (INT) m_nBlockSize )
			{
				-- x ;
				y += 2 ;
				break ;
			}
			else if ( y < 0 )
			{
				y = 0 ;
				break ;
			}
		}
		for ( ; ; )
		{
			ptrArrange[i ++] = x + y * m_nBlockSize ;
			if ( i >= m_nBlockArea )
				goto	Label_Next ;
			++ y ;
			-- x ;
			if ( y >= (INT) m_nBlockSize )
			{
				-- y ;
				x += 2 ;
				break ;
			}
			else if ( x < 0 )
			{
				x = 0 ;
				break ;
			}
		}
	}
Label_Next:
	//
	for ( i = 0; i < m_nBlockArea; i ++ )
	{
		m_pArrangeTable[1][ptrArrange[i]] = i ;
	}
}

// �ʎq���e�[�u���̐���
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::InitializeQuantumizeTable( double r )
{
	ESLAssert( m_nBlockSize == 8 ) ;
	double	q[2][3] =
	{
		{
			m_prmCmprOpt.m_rYScaleDC * r,
			m_prmCmprOpt.m_rYScaleLow * r,
			m_prmCmprOpt.m_rYScaleHigh * r
		},
		{
			m_prmCmprOpt.m_rCScaleDC * r,
			m_prmCmprOpt.m_rCScaleLow * r,
			m_prmCmprOpt.m_rCScaleHigh * r
		}
	} ;
	for ( int i = 0; i < 2; i ++ )
	{
		//
		// ����g�����̗ʎq���p�����[�^
		//
		int			j, k ;
		REAL32 *	pQScale = m_pQuantumizeScale[i] ;
		BYTE *		pQTable = m_pQuantumizeTable + i * m_nBlockArea ;
		REAL32		rq ;
		int			iq ;
		for ( j = 0; j < 7; j ++ )
		{
			for ( k = 0; k <= j; k ++ )
			{
				//
				// �ʎq���p�����[�^�̕⊮�Ɛ��K��
				//
				double	r = 1.0 ;
				if ( j != 0 )
				{
					r = pow( (double) (j - k) / j, 2.0 )
								+ pow( (double) k / j, 2.0 ) ;
				}
				rq = (REAL32)
					((q[i][0] + (q[i][1] - q[i][0])
							* pow( (j * 0.16666667), 0.7 )) * r) ;
				if ( rq > 0.0001 )
				{
					iq = ::eriRoundR32ToInt( 1.0F / rq ) ;
					if ( iq <= 0 )
					{
						iq = 1 ;
					}
					else if ( iq > 0x100 )
					{
						iq = 0x100 ;
					}
				}
				else
				{
					iq = 0x100 ;
				}
				rq = (REAL32) (1.0 / iq) ;
				iq -- ;
				//
				// �p�����[�^�ݒ�
				//
				pQScale[(j - k) * 8 + k] = rq ;
				pQTable[(j - k) * 8 + k] = (BYTE) iq ;
			}
		}
		//
		// �����g�����̗ʎq���p�����[�^
		//
		for ( j = 7; j < 15; j ++ )
		{
			for ( k = 7; k >= j - 7; k -- )
			{
				//
				// �ʎq���p�����[�^�̕⊮�Ɛ��K��
				//
				double	r = pow( (double) (j - k) / j, 2.0 )
								+ pow( (double) k / j, 2.0 ) ;
				rq = (REAL32)
					((q[i][1] + (q[i][2] - q[i][1]) * (j * 0.125F)) * r) ;
				if ( rq > 0.0001 )
				{
					iq = ::eriRoundR32ToInt( 1.0F / rq ) ;
					if ( iq <= 0 )
					{
						iq = 1 ;
					}
					else if ( iq > 0x100 )
					{
						iq = 0x100 ;
					}
				}
				else
				{
					iq = 0x100 ;
				}
				rq = (REAL32) (1.0 / iq) ;
				iq -- ;
				//
				// �p�����[�^�ݒ�
				//
				pQScale[(j - k) * 8 + k] = rq ;
				pQTable[(j - k) * 8 + k] = (BYTE) iq ;
			}
		}
		//
		if ( (m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI)
								&& (m_pArrangeTable[0] != NULL) )
		{
			PINT	pArrange = m_pArrangeTable[1] ;
			for ( int y = 1; y < 8; y += 2 )
			{
				for ( int x = 0; x < 8; x ++ )
				{
					j = pArrange[y * 8 + x] ;
					REAL32	rq = pQScale[j] ;
					if ( x & 0x01 )
						rq *= 1.75F ;
					else
						rq *= 1.4F ;
					//
					if ( rq > 0.0001 )
					{
						iq = ::eriRoundR32ToInt( 1.0F / rq ) ;
						if ( iq <= 0 )
						{
							iq = 1 ;
						}
						else if ( iq > 0x100 )
						{
							iq = 0x100 ;
						}
					}
					else
					{
						iq = 0x100 ;
					}
					rq = (REAL32) (1.0 / iq) ;
					iq -- ;
					pQScale[j] = rq ;
					pQTable[j] = (BYTE) iq ;
				}
			}
		}
	}
}

// �}�N���u���b�N�̃T���v�����O
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::SamplingMacroBlock
	( int nPosX, int nPosY,
			int nLeftWidth, int nLeftHeight,
			DWORD dwBlockStepAddr, BYTE *& ptrSrcLineAddr,
			ERISAEncoder::PTR_PROCEDURE pfnSamplingFunc )
{
	//
	// �u���b�N���T���v�����O�i4:4:4 �`���Łj
	//
	INT		i, j ;
	BYTE *	ptrSrcBlocks[9] ;
	INT		nWidths[9], nHeights[9] ;
	INT		nSubBlocks = 4 ;
//	if ( (m_nChannelCount >= 3) &&
//		(m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1) )
	{
		INT		xSub, ySub ;
		LONG	nSubLeftHeight = nLeftHeight + m_nBlockSize ;
		INT		nBlockWidthBytes = m_nBlockSize * m_nSrcPixelBytes ;
		BYTE *	ptrBlockLine =
			ptrSrcLineAddr - dwBlockStepAddr - nBlockWidthBytes ;
		nSubBlocks = 9 ;
		i = 0 ;
		for ( ySub = 0; ySub < 3; ySub ++ )
		{
			BYTE *	ptrNextBlock = ptrBlockLine ;
			LONG	nSubLeftWidth = nLeftWidth + m_nBlockSize ;
			for ( xSub = 0; xSub < 3; xSub ++ )
			{
				if ( ((xSub + nPosX) == 0) || ((ySub + nPosY) == 0) )
				{
					nWidths[i] = 0 ;
					nHeights[i] = 0 ;
				}
				else
				{
					ptrSrcBlocks[i] = ptrNextBlock ;
					nWidths[i] = nSubLeftWidth ;
					nHeights[i] = nSubLeftHeight ;
				}
				i ++ ;
				ptrNextBlock += nBlockWidthBytes ;
				nSubLeftWidth -= m_nBlockSize ;
			}
			ptrBlockLine += dwBlockStepAddr ;
			nSubLeftHeight -= m_nBlockSize ;
		}
	}
/*	else
	{
		ptrSrcBlocks[0] = ptrSrcLineAddr ;
		ptrSrcBlocks[1] =
			ptrSrcLineAddr + (m_nBlockSize * m_nSrcPixelBytes) ;
		ptrSrcBlocks[2] = ptrSrcLineAddr + dwBlockStepAddr ;
		ptrSrcBlocks[3] = ptrSrcBlocks[1] + dwBlockStepAddr ;
		nWidths[0] = nWidths[2] = nLeftWidth ;
		nWidths[1] = nWidths[3] = nLeftWidth - m_nBlockSize ;
		nHeights[0] = nHeights[1] = nLeftHeight ;
		nHeights[2] = nHeights[3] = nLeftHeight - m_nBlockSize ;
	}
*/	ptrSrcLineAddr += (m_nBlockSize * m_nSrcPixelBytes) * 2 ;
	//
	for ( i = 0; i < nSubBlocks; i ++ )
	{
		//
		// �e�T�u�u���b�N���T���v�����O
		//
		m_ptrSrcBlock = ptrSrcBlocks[i] ;
		if ( (nWidths[i] <= 0) || (nHeights[i] <= 0) )
		{
			// �摜�̈�O�F0�ŏ�����
			DWORD	dwBytes = m_nBlockArea * sizeof(REAL32) ;
			for ( j = 0; j < (INT) m_nChannelCount; j ++ )
			{
				::eslFillMemory
					( m_ptrBlocksetBuf[i + j * nSubBlocks], 0, dwBytes ) ;
			}
			ptrSrcBlocks[i] = NULL ;
			continue ;
		}
		m_nSrcWidth = nWidths[i] ;
		m_nSrcHeight = nHeights[i] ;
		if ( m_nSrcWidth > m_nBlockSize )
		{
			m_nSrcWidth = m_nBlockSize ;
		}
		if ( m_nSrcHeight > m_nBlockSize )
		{
			m_nSrcHeight = m_nBlockSize ;
		}
		//
		// �T���v�����O
		(this->*pfnSamplingFunc)( ) ;
		FillBlockOddArea( m_fdwEncFlags ) ;
		//
		// ���������_�`���֕ϊ�
		if ( m_fdwEncFlags & efDifferential )
		{
			SBYTE *	ptrEncodeBuf = m_ptrEncodeBuf ;
			for ( j = 0; j < (INT) m_nChannelCount; j ++ )
			{
				::eriConvertArraySByteToR32
					( m_ptrBlocksetBuf[i + j * nSubBlocks],
						ptrEncodeBuf, m_nBlockArea ) ;
				ptrEncodeBuf += m_nBlockArea ;
			}
		}
		else
		{
			BYTE *	ptrEncodeBuf = (BYTE*) m_ptrEncodeBuf ;
			for ( j = 0; j < (INT) m_nChannelCount; j ++ )
			{
				::eriConvertArrayByteToR32
					( m_ptrBlocksetBuf[i + j * nSubBlocks],
						ptrEncodeBuf, m_nBlockArea ) ;
				ptrEncodeBuf += m_nBlockArea ;
			}
		}
	}
	//
	// LOT �ϊ����̉�ʒ[�u���b�N�̂��߂̏���
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
	{
		for ( i = 0; i < nSubBlocks; i ++ )
		{
			if ( ptrSrcBlocks[i] != NULL )
			{
				continue ;
			}
			int	x, y, k = i ;
/*			if ( nSubBlocks == 4 )
			{
				if ( ptrSrcBlocks[(i ^ 0x01)] != NULL )
				{
					k = i ^ 0x01 ;
				}
				else if ( ptrSrcBlocks[(i ^ 0x02)] != NULL )
				{
					k = i ^ 0x02 ;
				}
				else
				{
					continue ;
				}
			}
			else
*/			{
				x = i % 3 ;
				y = i / 3 ;
				if ( ptrSrcBlocks[y * 3 + 1] != NULL )
				{
					k = y * 3 + 1 ;
				}
				else if ( ptrSrcBlocks[3 + x] != NULL )
				{
					k = 3 + x ;
				}
				else if ( ptrSrcBlocks[y * 3] != NULL )
				{
					k = y * 3 ;
				}
				else if ( ptrSrcBlocks[x] != NULL )
				{
					k = x ;
				}
				else if ( ptrSrcBlocks[y * 3 + 2] != NULL )
				{
					k = y * 3 + 2 ;
				}
				else if ( ptrSrcBlocks[6 + x] != NULL )
				{
					k = 6 + x ;
				}
				else if ( ptrSrcBlocks[3 + 1] != NULL )
				{
					k = 3 + 1 ;
				}
				else if ( (ptrSrcBlocks[0] != NULL)
							&& (ptrSrcBlocks[6 + 2] == NULL) )
				{
					k = 0 ;
				}
				else if ( (ptrSrcBlocks[6 + 2] != NULL)
							&& (ptrSrcBlocks[0] == NULL) )
				{
					k = 6 + 2 ;
				}
				else if ( (ptrSrcBlocks[2] != NULL)
							&& (ptrSrcBlocks[6] == NULL) )
				{
					k = 2 ;
				}
				else if ( (ptrSrcBlocks[6] != NULL)
							&& (ptrSrcBlocks[2] == NULL) )
				{
					k = 6 ;
				}
				else
				{
					continue ;
				}
			}
			//
			for ( j = 0; j < (int) m_nChannelCount; j ++ )
			{
				REAL32 *	pDstBlock = m_ptrBlocksetBuf[i + j * nSubBlocks] ;
				REAL32 *	pSrcBlock = m_ptrBlocksetBuf[k + j * nSubBlocks] ;
				REAL32	rAvgBlock = 0 ;
				for ( x = 0; x < 64; x ++ )
				{
					rAvgBlock += pSrcBlock[x] ;
				}
				rAvgBlock /= 64.0F ;
				//
				for ( x = 0; x < 64; x ++ )
				{
					pDstBlock[x] = rAvgBlock ;
				}
			}
		}
	}
	//
	// RGB->YUV �F��ԕϊ�
	//
	if ( m_nChannelCount >= 3 )
	{
		::eriConvertRGBtoYUV
			( m_ptrBlocksetBuf[0],
				m_ptrBlocksetBuf[nSubBlocks],
				m_ptrBlocksetBuf[nSubBlocks*2],
				m_nBlockArea * nSubBlocks ) ;
	}
}

// ���[�̈�ɓK���l��ݒ�
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::FillBlockOddArea( DWORD fdwFlags )
{
	if ( (m_nSrcWidth >= m_nBlockSize)
		&& (m_nSrcHeight >= m_nBlockSize) )
	{
		return ;
	}
	for ( UINT i = 0; i < m_nChannelCount; i ++ )
	{
		int		sum = 0 ;
		UINT	x, y ;
		BYTE *	ptrBuf ;
		BYTE *	ptrBuf1 ;
		BYTE *	ptrBuf2 ;
		ptrBuf = (BYTE*) m_ptrEncodeBuf + i * m_nBlockArea ;
		if ( m_nSrcWidth < m_nBlockSize )
		{
			for ( y = 0; y < m_nSrcHeight; y ++ )
			{
				ptrBuf1 = ptrBuf + y * m_nBlockSize ;
				SBYTE	p = ptrBuf1[m_nSrcWidth - 1] ;
				for ( x = m_nSrcWidth; x < m_nBlockSize; x ++ )
				{
					ptrBuf1[x] = p ;
				}
			}
		}
		ptrBuf1 = ptrBuf + (m_nSrcHeight - 1) * m_nBlockSize ;
		ptrBuf2 = ptrBuf1 + m_nBlockSize ;
		for ( y = m_nSrcHeight; y < m_nBlockSize; y ++ )
		{
			for ( x = 0; x < m_nBlockSize; x ++ )
			{
				ptrBuf1[x] = ptrBuf2[x] ;
			}
			ptrBuf1 += m_nBlockSize ;
			ptrBuf2 += m_nBlockSize ;
		}
	}
}

// 4:4:4 �X�P�[�����O
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::BlockScaling444( void )
{
	UINT	nBlockSize = m_nBlockSize ;
	UINT	nHalfSize = m_nBlockSize / 2 ;
	UINT	nHalfArea = m_nBlockArea / 2 ;
	int		i, j, k, p = 0 ;
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
	{
		for ( k = 0; k < (int) m_nChannelCount; k ++ )
		{
			for ( i = 0; i < 2; i ++ )
			{
				for ( j = 0; j < 2; j ++ )
				{
					::eslMoveMemory
						( m_ptrBlocksetBuf[p ++],
							m_ptrBlocksetBuf[k * 9 + i * 3 + j],
							m_nBlockArea * sizeof(REAL32) ) ;
				}
			}
		}
	}
	else
	{
		for ( k = 0; k < (int) m_nChannelCount; k ++ )
		{
			for ( i = 0; i < 2; i ++ )
			{
				for ( j = 0; j < 2; j ++ )
				{
					::eslMoveMemory
						( m_ptrBlocksetBuf[p ++],
							m_ptrBlocksetBuf[k * 9 + 4 + i * 3 + j],
							m_nBlockArea * sizeof(REAL32) ) ;
				}
			}
		}
	}
}

// 4:1:1 �X�P�[�����O
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::BlockScaling411( void )
{
	//
	// �P�x�M�����Ĕz��
	//
	UINT	nBlockSize = m_nBlockSize ;
	UINT	nHalfSize = m_nBlockSize / 2 ;
	UINT	nHalfArea = m_nBlockArea / 2 ;
	int		i, j ;
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
	{
		for ( i = 0, j = 2; i < 2; i ++, j ++ )
		{
			::eslMoveMemory
				( m_ptrBlocksetBuf[j],
					m_ptrBlocksetBuf[3 + i],
					m_nBlockArea * sizeof(REAL32) ) ;
		}
	}
	else
	{
		for ( i = 0; i < 2; i ++ )
		{
			for ( j = 0; j < 2; j ++ )
			{
				::eslMoveMemory
					( m_ptrBlocksetBuf[i * 2 + j],
						m_ptrBlocksetBuf[4 + i * 3 + j],
						m_nBlockArea * sizeof(REAL32) ) ;
			}
		}
	}
	if ( m_nChannelCount < 3 )
	{
		return ;
	}
	//
	// �F���M���� 1/2 �ɃX�P�[�����O
	//
	for ( i = 0; i < 2; i ++ )
	{
		REAL32 *	pDstBuf[4] ;
		pDstBuf[0] = m_ptrBlocksetBuf[4 + i] ;
		pDstBuf[1] = pDstBuf[0] + nHalfSize ;
		pDstBuf[2] = pDstBuf[0] + nHalfArea ;
		pDstBuf[3] = pDstBuf[1] + nHalfArea ;
		//
		for ( j = 0; j < 4; j ++ )
		{
			REAL32 *	ptrSrc =
				m_ptrBlocksetBuf[13 + i * 9 + (j >> 1) * 3 + (j & 0x01)] ;
			REAL32 *	ptrDst = pDstBuf[j] ;
			//
			for ( UINT y = 0; y < nBlockSize; y += 2 )
			{
				for ( UINT x = 0; x < nHalfSize; x ++ )
				{
					ptrDst[x] =
						(ptrSrc[0] + ptrSrc[1]
							+ ptrSrc[nBlockSize]
							+ ptrSrc[nBlockSize + 1]) * 0.25F ;
					ptrSrc += 2 ;
				}
				ptrDst += nBlockSize ;
				ptrSrc += nBlockSize ;
			}
		}
	}
	//
	// ���`���l���𕡐�
	//
	if ( m_nChannelCount >= 4 )
	{
		for ( i = 0; i < 2; i ++ )
		{
			for ( j = 0; j < 2; j ++ )
			{
				if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
				{
					::eslMoveMemory
						( m_ptrBlocksetBuf[6 + i * 2 + j],
							m_ptrBlocksetBuf[27 + i * 3 + j],
							m_nBlockArea * sizeof(REAL32) ) ;
				}
				else
				{
					::eslMoveMemory
						( m_ptrBlocksetBuf[6 + i * 2 + j],
							m_ptrBlocksetBuf[31 + i * 3 + j],
							m_nBlockArea * sizeof(REAL32) ) ;
				}
			}
		}
	}
}

// DCT �ϊ����{��
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::MatrixDCT8x8( void )
{
	UINT	i ;
	for ( i = 0; i < m_nBlocksetCount; i ++ )
	{
		::eriFastDCT8x8( m_ptrBlocksetBuf[i] ) ;
	}
}

// LOT �ϊ����{��
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::MatrixLOT8x8( REAL32 * ptrVertBufLOT )
{
	//
	// �P�x�`���l��������
	//
	int		i, j, k, l = 0 ;
	REAL32 *	ptrHorzBufLOT = m_ptrHorzBufLOT ;
	for ( i = 0; i < 2; i ++ )
	{
		for ( j = 0; j < 2; j ++ )
		{
			::eriFastLOT8x8
				( m_ptrBlocksetBuf[l],
					ptrHorzBufLOT, ptrVertBufLOT + j * m_nBlockArea ) ;
			l ++ ;
		}
		ptrHorzBufLOT += m_nBlockArea ;
	}
	ptrVertBufLOT += m_nBlockArea * 2 ;
	//
	// �F���`���l��������
	//
	if ( m_nChannelCount < 3 )
	{
		return ;
	}
	if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
	{
		for ( k = 0; k < 2; k ++ )
		{
			for ( i = 0; i < 2; i ++ )
			{
				for ( j = 0; j < 2; j ++ )
				{
					::eriFastLOT8x8
						( m_ptrBlocksetBuf[l],
							ptrHorzBufLOT, ptrVertBufLOT + j * m_nBlockArea ) ;
					l ++ ;
				}
				ptrHorzBufLOT += m_nBlockArea ;
			}
			ptrVertBufLOT += m_nBlockArea * 2 ;
		}
	}
	else if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
	{
		for ( k = 0; k < 2; k ++ )
		{
			::eriFastLOT8x8
				( m_ptrBlocksetBuf[l],
					ptrHorzBufLOT, ptrVertBufLOT ) ;
			l ++ ;
			ptrHorzBufLOT += m_nBlockArea ;
			ptrVertBufLOT += m_nBlockArea ;
		}
	}
	else
	{
		return ;
	}
	//
	// ���`���l��������
	//
	if ( m_nChannelCount < 4 )
	{
		return ;
	}
	for ( i = 0; i < 2; i ++ )
	{
		for ( j = 0; j < 2; j ++ )
		{
			::eriFastLOT8x8
				( m_ptrBlocksetBuf[l],
					ptrHorzBufLOT, ptrVertBufLOT + j * m_nBlockArea ) ;
			l ++ ;
		}
		ptrHorzBufLOT += m_nBlockArea ;
	}
	ptrVertBufLOT += m_nBlockArea * 2 ;
}

// �ʎq�����{��
//////////////////////////////////////////////////////////////////////////////
void ERISAEncoder::ArrangeAndQuantumize
		( SBYTE * ptrCoefficient, DWORD fdwFlags )
{
	//
	// �W�O�U�O����
	//
	int		i, j, k ;
	REAL32 *	ptrDst ;
	REAL32 *	ptrSrc ;
	PINT	pArrange = m_pArrangeTable[0] ;
	for ( i = 0; i < (int) m_nBlocksetCount; i ++ )
	{
		ptrDst = m_ptrMatrixBuf[i] ;
		ptrSrc = m_ptrBlocksetBuf[i] ;
		for ( j = 0; j < (int) m_nBlockArea; j ++ )
		{
			ptrDst[j] = ptrSrc[pArrange[j]] ;
		}
	}
	//
	// �e�u���b�N�̗ʎq���e�[�u���̎w�W���擾
	//
	int		iParamIndex[16] ;
	int		nThreshold[16] ;
	int		nLPFThreshold[16] ;
	iParamIndex[0] = iParamIndex[1] = iParamIndex[2] = iParamIndex[3] = 0 ;
	if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
	{
		for ( i = 4; i < 12; i ++ )
		{
			iParamIndex[i] = 1 ;
		}
		for ( i = 12; i < (int) m_nBlocksetCount; i ++ )
		{
			iParamIndex[i] = 0 ;
		}
	}
	else
	{
		iParamIndex[4] = iParamIndex[5] = 1 ;
		for ( i = 6; i < (int) m_nBlocksetCount; i ++ )
		{
			iParamIndex[i] = 0 ;
		}
	}
	//
	// �u���b�N�̒���g�E�����g�����̔䗦�𒲂ׂ�
	//
	double	rTAvgDC[2] = { 0, 0 } ;
	double	rTAvgAC2[2] = { 0, 0 } ;
	double	rTAvgLow[2] = { 0, 0 } ;
	double	rTAvgHigh[2] = { 0, 0 } ;
	int		nBlockCount[2] = { 0, 0 } ;
	int		nScalingHint[2] = { 0, 0 } ;
	for ( i = 0; i < (int) m_nBlocksetCount; i ++ )
	{
		nThreshold[i] =
			(iParamIndex[i] == 0) ?
				m_prmCmprOpt.m_nYThreshold
					: m_prmCmprOpt.m_nCThreshold ;
		nLPFThreshold[i] =
			(iParamIndex[i] == 0) ?
				m_prmCmprOpt.m_nYLPFThreshold
					: m_prmCmprOpt.m_nCLPFThreshold ;
		//
		double	rAvgDC = 0, rAvgAC2 = 0, rAvgLow = 0, rAvgHigh = 0 ;
		ptrSrc = m_ptrMatrixBuf[i] ;
		rAvgDC = fabs( ptrSrc[0] ) ;
		k = iParamIndex[i] ;
		rAvgAC2 = (fabs( ptrSrc[1] ) + fabs( ptrSrc[2] )) / 2 ;
		rTAvgAC2[k] += rAvgAC2 ;
		for ( j = 1; j < 28; j ++ )
		{
			rAvgLow += fabs( ptrSrc[j] ) ;
		}
		for ( j = 28; j < 64; j ++ )
		{
			rAvgHigh += fabs( ptrSrc[j] ) ;
		}
		rAvgLow /= (28 - 1) ;
		rAvgHigh /= (64 - 28) ;
		//
		rTAvgDC[k] += rAvgDC ;
		rTAvgLow[k] += rAvgLow ;
		rTAvgHigh[k] += rAvgHigh ;
		nBlockCount[k] ++ ;
		//
		if ( (rAvgLow >= rAvgHigh * 4.0)
			&& (rAvgDC >= rAvgLow * 64.0) )
		{
			if ( m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI )
			{
				nThreshold[i] -= (nThreshold[i] > 0) ;
			}
		}
		else if ( rAvgLow < rAvgHigh * 2.0 )
		{
			if ( fdwFlags & efDifferential )
			{
				if ( rAvgDC < rAvgLow * 3.0 )
				{
					nThreshold[i] /= 2 ;
					nLPFThreshold[i] = (nLPFThreshold[i] + 64) / 2 ;
				}
			}
			else if ( (rAvgDC < rAvgLow * 8.0)
				|| ((rAvgDC <= rAvgAC2 * 8.0) && (rAvgAC2 < rAvgLow * 3.0)) )
			{
				nThreshold[i] /= 2 ;
				nLPFThreshold[i] = 64 ;
				nScalingHint[iParamIndex[i]] -- ;
				//
				if ( rAvgLow < rAvgHigh * 2.0 )
				{
					nScalingHint[iParamIndex[i]] -- ;
				}
			}
			else if ( (rAvgDC < rAvgLow * 16.0)
				|| ((rAvgDC <= rAvgAC2 * 16.0) && (rAvgAC2 < rAvgLow * 3.0)) )
			{
				nThreshold[i] -= (nThreshold[i] > 0) ;
				nLPFThreshold[i] = (nLPFThreshold[i] + 64) / 2 ;
				//
				if ( rAvgLow < rAvgHigh * 2.0 )
				{
					nScalingHint[iParamIndex[i]] -- ;
				}
			}
		}
/*		if ( (rAvgDC <= rTAvgAC2[i] * 16.0)
				&& (rTAvgAC2[i] >= 8.0)
				&& (rTAvgAC2[i] > rAvgLow * 2.0)
				&& (rTAvgAC2[i] > rAvgHigh * 2.0)
				&& (rTAvgAC2[i] * m_pQuantumizeScale[i][1] < 1.0) )
		{
			nThreshold[i] -= (nThreshold[i] > 0) ;
		}*/
	}
	//
	// ���g�����z����X�P�[�����O�l�𒲐߂���
	//
	int		nCoefficient[2] = { 0, 0 } ;
	REAL32	rCoefficient[2] = { 1.0F, 1.0F } ;
	for ( i = 0; i < 2; i ++ )
	{
		rTAvgDC[i] /= nBlockCount[i] ;
		rTAvgAC2[i] /= nBlockCount[i] ;
		rTAvgLow[i] /= nBlockCount[i] ;
		rTAvgHigh[i] /= nBlockCount[i] ;
		//
		if ( nScalingHint[i] < 0 )
		{
			nCoefficient[i] = nScalingHint[i] + 1 ;
			if ( nCoefficient[i] < -4 )
			{
				nCoefficient[i] = -4 ;
			}
			rCoefficient[i] = 1.0F ;
			if ( nCoefficient[i] & 0x01 )
			{
				rCoefficient[i] = 1.5F ;
			}
			rCoefficient[i] =
				(REAL32) (1.0
					/ (rCoefficient[i] * pow( 2.0, (nCoefficient[i] >> 1) ))) ;
		}
		//
		if ( !(fdwFlags & efDifferential)
				&& (m_fdwEncFlags & efDifferential) )
		{
			// �����t���[���ł̓Ɨ��u���b�N�̕�����
			nCoefficient[i] = -3 ;
			rCoefficient[i] = (REAL32) (1.0 / 0.375) ;
		}
/*		else if ( fdwFlags & efDifferential )
		{
			// �����t���[��
			if ( (rTAvgDC[i] >= rTAvgAC2[i] * 16.0)
				&& (rTAvgAC2[i] * m_pQuantumizeScale[i][1] <= 0.5) )
			{
				if ( rTAvgDC[i] * m_pQuantumizeScale[i][1] <= 0.5 )
				{
					nCoefficient[i] = -2 ;
					rCoefficient[i] = (REAL32) (1.0 / 0.5) ;
				}
				else
				{
					nCoefficient[i] = -1 ;
					rCoefficient[i] = (REAL32) (1.0 / 0.75) ;
				}
			}
		}
/*		else //if ( !(fdwFlags & efDifferential) )
		{
			// �Ɨ��t���[��
			if ( (rTAvgDC[i] <= rTAvgAC2[i] * 16.0)
					&& (rTAvgAC2[i] >= 8.0)
					&& (rTAvgAC2[i] > rTAvgLow[i] * 2.0)
					&& (rTAvgAC2[i] > rTAvgHigh[i] * 2.0)
					&& (rTAvgAC2[i] * m_pQuantumizeScale[i][1] < 1.0) )
			{
				nCoefficient[i] = -2 ;
				rCoefficient[i] = (REAL32) (1.0 / 0.5) ;
				//
				for ( j = 0; j < (int) m_nBlocksetCount; j ++ )
				{
					if ( iParamIndex[j] == i )
					{
						nThreshold[j] ++ ;
					}
				}
			}
		}*/
	}
	//
	// �ʎq���W���̐��K���F�ő�l���͈͂Ɏ��܂�悤�ɕ␳
	//
	REAL32	rMax[2] = { 0.0F, 0.0F } ;
	for ( i = 0; i < (int) m_nBlocksetCount; i ++ )
	{
		ptrSrc = m_ptrMatrixBuf[i] ;
		k = iParamIndex[i] ;
		for ( j = 0; j < (int) m_nBlockArea; j ++ )
		{
			REAL32	y = (REAL32) fabs
				( ptrSrc[j] * m_pQuantumizeScale[k][j] * rCoefficient[k] ) ;
			if ( y > rMax[k] )
			{
				rMax[k] = y ;
			}
		}
	}
	for ( i = 0; i < 2; i ++ )
	{
		if ( rMax[i] > 128.0 )
		{
			if ( rMax[i] <= 192.0 )
			{
				nCoefficient[i] ++ ;
			}
			else
			{
				do
				{
					rMax[i] *= 0.5F ;
					nCoefficient[i] += 2 ;
				}
				while ( rMax[i] > 128.0 ) ;
				if ( rMax[i] < 96.0 )
				{
					nCoefficient[i] -- ;
				}
			}
		}
		rCoefficient[i] = 1.0F ;
		if ( nCoefficient[i] & 0x01 )
		{
			rCoefficient[i] = 1.5F ;
		}
		rCoefficient[i] =
			(REAL32) (1.0
				/ (rCoefficient[i] * pow( 2.0, (nCoefficient[i] >> 1) ))) ;
	}
	//
	// �u���b�N�X�P�[���W���o��
	//
	ptrCoefficient[0] = (SBYTE) nCoefficient[0] ;
	ptrCoefficient[1] = (SBYTE) nCoefficient[1] ;
	//
	// �ʎq�����s
	//
	SBYTE *		pbytDst = m_ptrEncodeBuf ;
	ptrSrc = m_ptrMatrixBuf[0] ;
	for ( i = 0; i < (int) m_nBlocksetCount; i ++ )
	{
		//
		// �ʎq��
		//
		int	k = iParamIndex[i] ;
		::eriVectorMultiply
			( ptrSrc, m_pQuantumizeScale[k], m_nBlockArea ) ;
		::eriScalarMultiply
			( ptrSrc, rCoefficient[k], m_nBlockArea ) ;
		::eriConvertArrayR32ToSByte( pbytDst, ptrSrc, m_nBlockArea ) ;
		//
		// 臒l����
		//
		int	nCurThreshold = nThreshold[i] ;
		int	nLPF = nLPFThreshold[i] ;
		if ( nLPF >= (int) m_nBlockArea )
		{
			nLPF = m_nBlockArea ;
		}
		if ( nCurThreshold != 0 )
		{
/*			if ( fdwFlags & efDifferential )
			{
				for ( j = 0; j < 3; j ++ )
				{
					if ( fabs( ptrSrc[j] ) < 0.7 )
					{
						pbytDst[j] = 0 ;
						break ;
					}
				}
			}*/
			double	rThreshold = nCurThreshold * 0.6 ;
			double	rThresholdStep = nCurThreshold * 0.8 / 64 ;
			j = 3 ;
			while ( j < (int) m_nBlockSize )
			{
				if ( fabs( ptrSrc[j] ) < rThreshold )
				{
					pbytDst[j] = 0 ;
				}
				rThreshold += rThresholdStep ;
				j ++ ;
			}
		}
		ESLAssert( m_nBlockSize == 8 ) ;
		for ( j = m_nBlockArea - 1; j >= nLPF; j -- )
		{
			pbytDst[j] = 0 ;
		}
		pbytDst += m_nBlockArea ;
		ptrSrc += m_nBlockArea ;
	}
	//
	// ����������������
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI )
	{
		m_ptrEncodeBuf[m_nBlockArea]   -= m_ptrEncodeBuf[0] ;
		m_ptrEncodeBuf[m_nBlockArea*2] -= m_ptrEncodeBuf[0] ;
		m_ptrEncodeBuf[m_nBlockArea*3] -= m_ptrEncodeBuf[0] ;
		//
		k = m_nBlockArea * 6 ;
		j = 3 ;
		if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
		{
			k = m_nBlockArea * 4 ;
			j = 1 ;
		}
		for ( i = j; i < (int) m_nChannelCount; i ++ )
		{
			m_ptrEncodeBuf[k + m_nBlockArea]   -= m_ptrEncodeBuf[k] ;
			m_ptrEncodeBuf[k + m_nBlockArea*2] -= m_ptrEncodeBuf[k] ;
			m_ptrEncodeBuf[k + m_nBlockArea*3] -= m_ptrEncodeBuf[k] ;
			k += m_nBlockArea * 4 ;
		}
	}
}

// �摜���T���v�����O����֐��ւ̃|�C���^���擾����
//////////////////////////////////////////////////////////////////////////////
ERISAEncoder::PTR_PROCEDURE ERISAEncoder::GetLSSamplingFunc
	( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags )
{
	return	GetLLSamplingFunc( fdwFormatType, dwBitsPerPixel, fdwFlags ) ;
}

#endif
