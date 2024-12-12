
/*****************************************************************************
                         E R I S A - L i b r a r y
													last update 2003/01/28
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2203 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <xerisa.h>

#if	!defined(COMPACT_NOA_DECODER)


//
// �萔
//////////////////////////////////////////////////////////////////////////////

static const double	ERI_PI = 3.141592653589 ;	// = ��


/*****************************************************************************
                            �������k�I�u�W�F�N�g
 *****************************************************************************/

IMPLEMENT_CLASS_INFO( MIOEncoder, ESLObject )

//
// MIOEncoder::PARAMETER �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
MIOEncoder::PARAMETER::PARAMETER( void )
{
	rLowWeight = 4.0 ;
	rMiddleWeight = 3.0 ;
	rPowerScale = 0.5 ;
	nOddWeight = 1 ;
	nPreEchoThreshold = 2 ;
}

//
// MIOEncoder::PARAMETER �v���Z�b�g�l�ݒ�
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::PARAMETER::LoadPresetParam
	( MIOEncoder::PresetParameter ppIndex, MIO_INFO_HEADER & infhdr )
{
	struct	PRESET_PARAMETER
	{
		double		low_weight ;
		double		middle_weight ;
		int			odd_weight ;
		int			pe_threshold ;
		signed int	power_scale ;
		int			matrix_degree ;
		int			use_mss ;
	} ;
	static const PRESET_PARAMETER	preset[ppMax] =
	{
		{	4.0,	3.0,	0,	2,	256,	 9,	0	},			// 1/6
		{	4.0,	3.0,	0,	2,	140,	10,	0	},			// 1/8
		{	4.0,	3.0,	0,	2,	108,	10,	1	},			// 1/9
		{	4.0,	3.0,	1,	2,	95,		10,	1	},			// 1/10
		{	4.0,	3.0,	1,	2,	85,		10,	1	},			// 1/11
		{	4.0,	3.0,	1,	2,	75,		10,	1	},			// 1/12
		{	4.0,	3.0,	2,	4,	57,		11,	1	},			// 1/15
		{	4.0,	3.0,	2,	6,	49,		11,	1	},			// 1/18
		{	4.0,	3.0,	2,	8,	42,		12,	1	}			// 1/20
	} ;
	//
	rLowWeight = preset[ppIndex].low_weight ;
	rMiddleWeight = preset[ppIndex].middle_weight ;
	rPowerScale = preset[ppIndex].power_scale / 256.0 ;
	nOddWeight = preset[ppIndex].odd_weight ;
	nPreEchoThreshold = preset[ppIndex].pe_threshold ;
	//
	infhdr.dwVersion = 0x00020300 ;
	if ( preset[ppIndex].use_mss && (infhdr.dwChannelCount == 2) )
		infhdr.fdwTransformation = CVTYPE_LOT_ERI_MSS ;
	else
		infhdr.fdwTransformation = CVTYPE_LOT_ERI ;
	infhdr.dwArchitecture = ERI_RUNLENGTH_GAMMA ;
	infhdr.dwSubbandDegree = preset[ppIndex].matrix_degree ;
	infhdr.dwLappedDegree = 1 ;
}

//
// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
MIOEncoder::MIOEncoder( void )
{
	m_nBufLength = 0 ;
	m_ptrBuffer1 = NULL ;
	m_ptrBuffer2 = NULL ;
	m_ptrBuffer3 = NULL ;
	m_ptrSamplingBuf = NULL ;
	m_ptrInternalBuf = NULL ;
	m_ptrDstBuf = NULL ;
	m_ptrWorkBuf = NULL ;
	m_ptrWeightTable = NULL ;
	m_ptrLastDCT = NULL ;
	m_pRevolveParam = NULL ;
}

//
// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
MIOEncoder::~MIOEncoder( void )
{
	Delete( ) ;
}

//
// �������i�p�����[�^�̐ݒ�j
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::Initialize( const MIO_INFO_HEADER & infhdr )
{
	//
	// �ȑO�̃��\�[�X�����
	Delete( ) ;
	//
	// �������w�b�_���R�s�[
	m_mioih = infhdr ;
	//
	// �p�����[�^�̃`�F�b�N
	if ( m_mioih.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		if ( m_mioih.dwArchitecture != ERI_RUNLENGTH_HUFFMAN )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̈��k�t�H�[�}�b�g�j
		}
		if ( (m_mioih.dwChannelCount != 1) && (m_mioih.dwChannelCount != 2) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃`���l�����j
		}
		if ( (m_mioih.dwBitsPerSample != 8) && (m_mioih.dwBitsPerSample != 16) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃T���v�����O����\�j
		}
	}
	else if ( (m_mioih.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_mioih.fdwTransformation == CVTYPE_LOT_ERI_MSS) )
	{
		if ( (m_mioih.dwArchitecture != ERI_RUNLENGTH_GAMMA)
			&& (m_mioih.dwArchitecture != ERI_RUNLENGTH_HUFFMAN)
			&& (m_mioih.dwArchitecture != ERISA_NEMESIS_CODE) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̈��k�t�H�[�}�b�g�j
		}
		if ( (m_mioih.dwChannelCount != 1) && (m_mioih.dwChannelCount != 2) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃`���l�����j
		}
		if ( m_mioih.dwBitsPerSample != 16 )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃T���v�����O����\�j
		}
		if ( (m_mioih.dwSubbandDegree < 8) ||
				(m_mioih.dwSubbandDegree > MAX_DCT_DEGREE) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή���DCT�����j
		}
		if ( m_mioih.dwLappedDegree != 1 )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̏d���W���j
		}
		//
		// ���k�p�����[�^������
		PARAMETER	parameter ;
		SetCompressionParameter( parameter ) ;
		//
		// DCT �p�o�b�t�@�m��
		m_ptrBuffer1 =
			::eslHeapAllocate
				( NULL, m_mioih.dwChannelCount *
					(sizeof(REAL32) << m_mioih.dwSubbandDegree), 0 ) ;
		m_ptrSamplingBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_mioih.dwChannelCount
					* (sizeof(REAL32) << m_mioih.dwSubbandDegree), 0 ) ;
		m_ptrInternalBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, sizeof(REAL32) << m_mioih.dwSubbandDegree, 0 ) ;
		m_ptrDstBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_mioih.dwChannelCount
					* (sizeof(REAL32) << m_mioih.dwSubbandDegree), 0 ) ;
		m_ptrWorkBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, sizeof(REAL32) << m_mioih.dwSubbandDegree, 0 ) ;
		//
		// �d�݃e�[�u�����m��
		m_ptrWeightTable =
			(REAL32*) ::eslHeapAllocate
				( NULL, sizeof(REAL32) << m_mioih.dwSubbandDegree, 0 ) ;
		//
		// LOT �p�o�b�t�@�m��
		unsigned int	i, nBlocksetSamples, nLappedSamples ;
		nBlocksetSamples = m_mioih.dwChannelCount << m_mioih.dwSubbandDegree ;
		nLappedSamples = nBlocksetSamples * m_mioih.dwLappedDegree ;
		if ( nLappedSamples > 0 )
		{
			m_ptrLastDCT =
				(REAL32*) ::eslHeapAllocate
					( NULL, sizeof(REAL32) * nLappedSamples, 0 ) ;
			for ( i = 0; i < nLappedSamples; i ++ )
			{
				m_ptrLastDCT[i] = 0.0F ;
			}
		}
		//
		// �p�����[�^�ݒ�
		InitializeWithDegree( m_mioih.dwSubbandDegree ) ;
	}
	else
	{
		return	eslErrGeneral ;		// �G���[�i���Ή��̈��k�t�H�[�}�b�g�j
	}
	//
	// ����I��
	return	eslErrSuccess ;
}

//
// �I���i�������̉���Ȃǁj
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::Delete( void )
{
	if ( m_ptrBuffer1 != NULL )
	{
		::eslHeapFree( NULL, m_ptrBuffer1 ) ;
		m_ptrBuffer1 = NULL ;
	}
	if ( m_ptrBuffer2 != NULL )
	{
		::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		m_ptrBuffer2 = NULL ;
	}
	if ( m_ptrBuffer3 != NULL )
	{
		::eslHeapFree( NULL, m_ptrBuffer3 ) ;
		m_ptrBuffer3 = NULL ;
	}
	if ( m_ptrSamplingBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrSamplingBuf ) ;
		m_ptrSamplingBuf = NULL ;
	}
	if ( m_ptrInternalBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrInternalBuf ) ;
		m_ptrInternalBuf = NULL ;
	}
	if ( m_ptrDstBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrDstBuf ) ;
		m_ptrDstBuf = NULL ;
	}
	if ( m_ptrWorkBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrWorkBuf ) ;
		m_ptrWorkBuf = NULL ;
	}
	if ( m_ptrWeightTable != NULL )
	{
		::eslHeapFree( NULL, m_ptrWeightTable ) ;
		m_ptrWeightTable = NULL ;
	}
	if ( m_ptrLastDCT != NULL )
	{
		::eslHeapFree( NULL, m_ptrLastDCT ) ;
		m_ptrLastDCT = NULL ;
	}
	if ( m_pRevolveParam != NULL )
	{
		::eslHeapFree( NULL, m_pRevolveParam ) ;
		m_pRevolveParam = NULL ;
	}
	m_nBufLength = 0 ;
}

//
// ���������k
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeSound
	( ERISAEncodeContext & context,
		const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf )
{
	if ( m_mioih.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		if ( m_mioih.dwBitsPerSample == 8 )
		{
			return	EncodeSoundPCM8( context, datahdr, ptrWaveBuf ) ;
		}
		else if ( m_mioih.dwBitsPerSample == 16 )
		{
			return	EncodeSoundPCM16( context, datahdr, ptrWaveBuf ) ;
		}
	}
	else if ( (m_mioih.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_mioih.fdwTransformation == CVTYPE_LOT_ERI_MSS) )
	{
		if ( m_mioih.dwBitsPerSample == 16 )
		{
			if ( (m_mioih.dwChannelCount != 2) ||
					(m_mioih.fdwTransformation == CVTYPE_LOT_ERI) )
			{
				return	EncodeSoundDCT( context, datahdr, ptrWaveBuf ) ;
			}
			else
			{
				return	EncodeSoundDCT_MSS( context, datahdr, ptrWaveBuf ) ;
			}
		}
	}

	return	eslErrGeneral ;		// �G���[
}

//
// ���k�I�v�V������ݒ�
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::SetCompressionParameter
	( const MIOEncoder::PARAMETER & parameter )
{
	m_parameter = parameter ;
}

//
// 8�r�b�g��PCM�����k
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeSoundPCM8
	( ERISAEncodeContext & context,
		const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf )
{
	//
	// ���Z�p�o�b�t�@���m��
	unsigned int	nSampleCount = datahdr.dwSampleCount ;
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer1 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer1 ) ;
		}
		m_ptrBuffer1 = ::eslHeapAllocate
			( NULL, nSampleCount * m_mioih.dwChannelCount, 0 ) ;
		m_nBufLength = nSampleCount ;
	}
	//
	// �`���l�����ƂɃT���v�����O���č�������
	PBYTE	ptrDstBuf = (PBYTE) m_ptrBuffer1 ;
	PBYTE	ptrSrcBuf ;
	unsigned int	nStep = m_mioih.dwChannelCount ;
	unsigned int	i, j ;
	for ( i = 0; i < m_mioih.dwChannelCount; i ++ )
	{
		ptrSrcBuf = (PBYTE) ptrWaveBuf ;
		ptrSrcBuf += i ;
		//
		BYTE	bytLeftVal = 0 ;
		for ( j = 0; j < nSampleCount; j ++ )
		{
			BYTE	bytCurVal = *ptrSrcBuf ;
			ptrSrcBuf += nStep ;
			*(ptrDstBuf ++) = bytCurVal - bytLeftVal ;
			bytLeftVal = bytCurVal ;
		}
	}
	//
	// �n�t�}�������ŕ�����
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		context.PrepareToEncodeERINACode( ) ;
	}
	ULONG	nBytes = nSampleCount * m_mioih.dwChannelCount ;
	if ( context.EncodeSymbolBytes
			( (const SBYTE *) m_ptrBuffer1, nBytes ) < nBytes )
	{
		return	eslErrGeneral ;			// �G���[
	}
	context.Flushout( ) ;
	return	eslErrSuccess ;
}

//
// 16�r�b�g��PCM�����k
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeSoundPCM16
	( ERISAEncodeContext & context,
		const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf )
{
	//
	// ���Z�p�o�b�t�@���m��
	//
	unsigned int	i, j ;
	unsigned int	nSampleCount = datahdr.dwSampleCount ;
	unsigned int	nChannelCount = m_mioih.dwChannelCount ;
	unsigned int	nAllSampleCount = nSampleCount * nChannelCount ;
	//
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer1 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer1 ) ;
		}
		if ( m_ptrBuffer2 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		}
		m_ptrBuffer1 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_ptrBuffer2 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		//
		m_nBufLength = nSampleCount ;
	}
	//
	// �`���l�����ƂɃT���v�����O���č�������
	//
	SWORD *	ptrDstBuf = (SWORD*) m_ptrBuffer1 ;
	SWORD *	ptrSrcBuf ;
	unsigned int	nStep = nChannelCount ;
	for ( i = 0; i < nChannelCount; i ++ )
	{
		ptrSrcBuf = (SWORD*) ptrWaveBuf ;
		ptrSrcBuf += i ;
		//
		SWORD	wLeftVal = 0 ;
		SWORD	wLastDelta = 0 ;
		for ( j = 0; j < nSampleCount; j ++ )
		{
			SWORD	wCurVal = *ptrSrcBuf ;
			ptrSrcBuf += nStep ;
			SWORD	wCurDelta = wCurVal - wLeftVal ;
			ptrDstBuf[j] = wCurDelta - wLastDelta ;
			wLeftVal = wCurVal ;
			wLastDelta = wCurDelta ;
		}
		ptrDstBuf += nSampleCount ;
	}
	//
	// ���[�h�̏�ʂƉ��ʂ𕪗����Đ���
	//
	PBYTE	pbytDstBuf1, pbytDstBuf2, pbytSrcBuf ;
	for ( i = 0; i < nChannelCount; i ++ )
	{
		unsigned int	nOffset = i * nSampleCount * sizeof(SWORD) ;
		pbytDstBuf1 = ((PBYTE) m_ptrBuffer2) + nOffset ;
		pbytDstBuf2 = pbytDstBuf1 + nSampleCount ;
		pbytSrcBuf = ((PBYTE) m_ptrBuffer1) + nOffset ;
		//
		for ( j = 0; j < nSampleCount; j ++ )
		{
			SBYTE	bytLow = pbytSrcBuf[j * sizeof(SWORD) + 0] ;
			SBYTE	bytHigh = pbytSrcBuf[j * sizeof(SWORD) + 1] ;
			pbytDstBuf2[j] = bytLow ;
			pbytDstBuf1[j] = bytHigh ^ (bytLow >> 7) ;
		}
	}
	//
	// �n�t�}�������ŕ�����
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		context.PrepareToEncodeERINACode( ) ;
	}
	ULONG	nBytes = nSampleCount * nChannelCount * sizeof(SWORD) ;
	if ( context.EncodeSymbolBytes
			( (const SBYTE *) m_ptrBuffer2, nBytes ) < nBytes )
	{
		return	eslErrGeneral ;			// �G���[
	}
	context.Flushout( ) ;
	return	eslErrSuccess ;
}

//
// �s��T�C�Y�̕ύX�ɔ����p�����[�^�̍Čv�Z
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::InitializeWithDegree( unsigned int nSubbandDegree )
{
	//
	// ��]�p�����[�^����
	if ( m_pRevolveParam != NULL )
	{
		::eslHeapFree( NULL, m_pRevolveParam ) ;
	}
	m_pRevolveParam =
		::eriCreateRevolveParameter( nSubbandDegree ) ;
	//
	// �ʎq���p�p�����[�^�ݒ�
	static const int	freq_width[7] =
	{
		-6, -6, -5, -4, -3, -2, -1
	} ;
	for ( int i = 0, j = 0; i < 7; i ++ )
	{
		m_nFrequencyWidth[i] = 1 << (nSubbandDegree + freq_width[i]) ;
		m_nFrequencyPoint[i] = j + (m_nFrequencyWidth[i] / 2) ;
		j += m_nFrequencyWidth[i] ;
	}
	//
	// ���[�J���p�����[�^��ݒ�
	m_nSubbandDegree = nSubbandDegree ;
	m_nDegreeNum = (1 << nSubbandDegree) ;
}

//
// �w��T���v����̉��ʂ����߂�
//////////////////////////////////////////////////////////////////////////////
double MIOEncoder::EvaluateVolume( const REAL32 * ptrWave, int nCount )
{
	double	rVolume = 0.0 ;
	for ( int i = 1; i < nCount; i ++ )
	{
		rVolume += fabs( ptrWave[i] - ptrWave[i - 1] ) ;
	}
	rVolume /= (nCount - 1) ;
	return	rVolume ;
}

//
// �����R�[�h���擾����
//////////////////////////////////////////////////////////////////////////////
int MIOEncoder::GetDivisionCode( const REAL32 * ptrSamples )
{
	double	rSum = 0.0 ;
	double	rAvg = 0.0 ;
	int		nDegreeWidth = (1 << m_mioih.dwSubbandDegree) ;
	int		nCount = nDegreeWidth / 64 ;
	//
	for ( int i = 0; i < nCount; i ++ )
	{
		double	rVol = EvaluateVolume( ptrSamples, 64 ) ;
		if ( (i >= 1) && (rVol >= rAvg * m_parameter.nPreEchoThreshold) )
		{
			return	2 ;
		}
		ptrSamples += 64 ;
		rSum += rVol ;
		rAvg = rSum / (i + 1) ;
	}
	//
	return	0 ;
}

//
// 16�r�b�g�̔�t���k
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeSoundDCT
	( ERISAEncodeContext & context,
		const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf )
{
	//
	// �o�b�t�@���m��
	//
	unsigned int	nDegreeWidth = (1 << m_mioih.dwSubbandDegree) ;
	unsigned int	nSampleCount =
		(datahdr.dwSampleCount + nDegreeWidth - 1) & ~(nDegreeWidth - 1) ;
	unsigned int	nSubbandCount =
		(nSampleCount >> m_mioih.dwSubbandDegree) ;
	unsigned int	nChannelCount = m_mioih.dwChannelCount ;
	unsigned int	nAllSampleCount = nSampleCount * nChannelCount ;
	//
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer2 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		}
		if ( m_ptrBuffer3 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer3 ) ;
		}
		m_ptrBuffer2 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_ptrBuffer3 = (SBYTE*)
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_nBufLength = nSampleCount ;
	}
	//
	m_ptrNextDstBuf = (SWORD*) m_ptrBuffer2 ;
	//
	// �\��r�b�g�𑗏o
	//
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
		{
			context.PrepareToEncodeERINACode( ) ;
		}
		else
		{
			context.PrepareToEncodeERISACode( ) ;
		}
	}
	context.OutNBits( 0, 1 ) ;
	//
	// �T�u�o���h�P�ʂŕ������A�e�XDCT�ϊ����{��
	//
	unsigned int	i, j, k ;
	const SWORD *	ptrSrcBuf = (const SWORD *) ptrWaveBuf ;
	unsigned int *	pLastDivision = new unsigned int [nChannelCount] ;
	for ( i = 0; i < nChannelCount; i ++ )
	{
		pLastDivision[i] = -1 ;
	}
	unsigned int	nCurrentDivision = -1 ;
	//
	for ( i = 0; i < nSubbandCount; i ++ )
	{
		const SWORD *	ptrSubbandHead =
			ptrSrcBuf + (nDegreeWidth * nChannelCount) * i ;
		//
		unsigned int	nCopySamples =
			datahdr.dwSampleCount - (i * nDegreeWidth) ;
		if ( nCopySamples > nDegreeWidth )
		{
			nCopySamples = nDegreeWidth ;
		}
		//
		for ( j = 0; j < nChannelCount; j ++ )
		{
			//
			// �T���v�����O
			//
			const SWORD *	ptrSrcHead = ptrSubbandHead + j ;
			//
			for ( k = 0; k < nCopySamples; k ++ )
			{
				m_ptrSamplingBuf[k] = (REAL32) *ptrSrcHead ;
				ptrSrcHead += nChannelCount ;
			}
			while ( k < nDegreeWidth )
			{
				m_ptrSamplingBuf[k ++] = 0.0F ;
			}
			//
			// �d�������p�o�b�t�@���擾
			//
			int		nChannelStep = nDegreeWidth * m_mioih.dwLappedDegree * j ;
			m_ptrLastDCTBuf = m_ptrLastDCT + nChannelStep ;
			//
			// �����R�[�h���擾
			//
			REAL32			rPowerScale = (REAL32) m_parameter.rPowerScale ;
			unsigned int	nDivisionCode = GetDivisionCode( m_ptrSamplingBuf ) ;
			unsigned int	nDivisionCount = (1 << nDivisionCode) ;
			//
			for ( k = 0; k < nDivisionCode; k ++ )
			{
				rPowerScale *= 1.0625F ;
			}
			//
			context.OutNBits( (((DWORD) nDivisionCode) << 30), 2 ) ;
			//
			// �s��T�C�Y���ω�����ۂ̏���
			//
			bool	fLeadBlock = false ;
			//
			if ( pLastDivision[j] != nDivisionCode )
			{
				//
				// ���O�܂ł̍s�������������
				//
				if ( i != 0 )
				{
					if ( nCurrentDivision != pLastDivision[j] )
					{
						InitializeWithDegree
							( m_mioih.dwSubbandDegree - pLastDivision[j] ) ;
						nCurrentDivision = pLastDivision[j] ;
					}
					if ( EncodePostBlock
						( context, (REAL32) m_parameter.rPowerScale ) )
					{
						return	eslErrGeneral ;
					}
				}
				//
				// �s��T�C�Y��ω������邽�߂̃p�����[�^���Z�b�g�A�b�v
				//
				pLastDivision[j] = nDivisionCode ;
				fLeadBlock = true ;
			}
			if ( nCurrentDivision != nDivisionCode )
			{
				InitializeWithDegree
					( m_mioih.dwSubbandDegree - nDivisionCode ) ;
				nCurrentDivision = nDivisionCode ;
			}
			//
			// ���� LOT �ϊ����{��
			//
			REAL32 *	ptrNextSamples = m_ptrSamplingBuf ;
			//
			for ( k = 0; k < nDivisionCount; k ++ )
			{
				if ( fLeadBlock )
				{
					//
					// ���[�h�u���b�N���o�͂���
					//
					if ( EncodeLeadBlock
							( context, ptrNextSamples, rPowerScale ) )
					{
						return	eslErrGeneral ;
					}
					//
					fLeadBlock = false ;
				}
				else
				{
					//
					// �ʏ�u���b�N���o�͂���
					//
					if ( EncodeInternalBlock
							( context, ptrNextSamples, rPowerScale ) )
					{
						return	eslErrGeneral ;
					}
				}
				//
				// ����
				//
				ptrNextSamples += m_nDegreeNum ;
			}
		}
	}
	//
	// �s�������������
	//
	if ( nSubbandCount > 0 )
	{
		for ( i = 0; i < nChannelCount; i ++ )
		{
			int		nChannelStep = nDegreeWidth * m_mioih.dwLappedDegree * i ;
			m_ptrLastDCTBuf = m_ptrLastDCT + nChannelStep ;
			//
			if ( nCurrentDivision != pLastDivision[i] )
			{
				InitializeWithDegree
					( m_mioih.dwSubbandDegree - pLastDivision[i] ) ;
				nCurrentDivision = pLastDivision[i] ;
			}
			//
			if ( EncodePostBlock( context, (REAL32) m_parameter.rPowerScale ) )
			{
				return	eslErrGeneral ;
			}
		}
	}
	//
	delete []	pLastDivision ;
	//
	// �C���^�[���[�u���ĕ�����
	//
	if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
	{
		SBYTE *	ptrHBuf = m_ptrBuffer3 ;
		SBYTE *	ptrLBuf = m_ptrBuffer3 + nAllSampleCount ;
		unsigned int	nAllSubbandCount = nSubbandCount * nChannelCount ;
		//
		for ( i = 0; i < nDegreeWidth; i ++ )
		{
			ptrSrcBuf = ((const SWORD *) m_ptrBuffer2) + i ;
			//
			for ( j = 0; j < nAllSubbandCount; j ++ )
			{
				SWORD	nValue = *ptrSrcBuf ;
				ptrSrcBuf += nDegreeWidth ;
				SBYTE	nLow = (SBYTE)((nValue << 8) >> 8) ;
				SBYTE	nHigh = (SBYTE)((nValue >> 8) ^ (nLow >> 7)) ;
				*(ptrHBuf ++) = nHigh ;
				*(ptrLBuf ++) = nLow ;
			}
		}
		//
		context.OutNBits( 0, 1 ) ;
		//
		if ( context.EncodeSymbolBytes
				( m_ptrBuffer3, nAllSampleCount * 2 ) < nAllSampleCount * 2 )
		{
			return	eslErrGeneral ;			// �G���[
		}
		//
		context.Flushout( ) ;
	}
	else
	{
		context.OutNBits( 0, 1 ) ;
		//
		if ( context.EncodeERISACodeWords
				( (const SWORD *) m_ptrBuffer2,
						nAllSampleCount ) < nAllSampleCount )
		{
			return	eslErrGeneral ;			// �G���[
		}
		//
		context.FinishERISACode( ) ;
	}

	return	eslErrSuccess ;
}

//
// LOT �ϊ����{��
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::PerformLOT
	( ERISAEncodeContext & context,
		REAL32 * ptrSamples, REAL32 rPowerScale )
{
	//
	// DCT �ϊ����{��
	//
	REAL32	rMatrixScale = (REAL32) sqrt(2.0 / m_nDegreeNum) ;
	::eriFastDCT
		( m_ptrInternalBuf, 1, ptrSamples,
			m_ptrWorkBuf, m_nSubbandDegree ) ;
	::eriScalarMultiply
		( m_ptrInternalBuf, rMatrixScale, m_nDegreeNum ) ;
	//
	// LOT ���{��
	//
	::eriFastPLOT
		( m_ptrInternalBuf, m_nSubbandDegree ) ;
	::eriFastLOT
		( m_ptrWorkBuf, m_ptrLastDCTBuf,
			m_ptrInternalBuf, m_nSubbandDegree ) ;
	::eriOddGivensMatrix
		( m_ptrWorkBuf, m_pRevolveParam, m_nSubbandDegree ) ;
	//
	for ( unsigned int i = 0; i < m_nDegreeNum; i ++ )
	{
		m_ptrLastDCTBuf[i] = m_ptrInternalBuf[i] ;
	}
	//
	// �ʎq��
	//
	DWORD	nWeightCode ;
	int		nCoefficient ;
	Quantumize
		( (PINT) m_ptrBuffer1, m_ptrWorkBuf,
			m_nDegreeNum, rPowerScale,
			&nWeightCode, &nCoefficient ) ;
	//
	// �ʎq���W�����o��
	//
	context.OutNBits( nWeightCode, 32 ) ;
	context.OutNBits( (((DWORD) nCoefficient) << 16), 16 ) ;
}

//
// �ʏ�̃u���b�N�𕄍������ďo�͂���
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeInternalBlock
	( ERISAEncodeContext & context,
		REAL32 * ptrSamples, REAL32 rPowerScale )
{
	//
	// LOT �ϊ����{��
	//
	PerformLOT( context, ptrSamples, rPowerScale ) ;
	//
	// ������
	//
	for ( unsigned int i = 0; i < m_nDegreeNum; i ++ )
	{
		*(m_ptrNextDstBuf ++) = (SWORD) (((const INT *) m_ptrBuffer1)[i]) ;
	}
	return	eslErrSuccess ;
}

//
// ���[�h�u���b�N�𕄍������ďo�͂���
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeLeadBlock
	( ERISAEncodeContext & context,
		REAL32 * ptrSamples, REAL32 rPowerScale )
{
	//
	// �d���W����������
	//
	unsigned int	i ;
	for ( i = 0; i < m_nDegreeNum; i ++ )
	{
		m_ptrLastDCTBuf[i] = 0.0F ;
	}
	//
	// LOT �ϊ����{��
	//
	PerformLOT( context, ptrSamples, rPowerScale ) ;
	//
	// ������
	//
	PINT	ptrSrcBuffer = (PINT) m_ptrBuffer1 ;
	unsigned int	nHalfDegree = m_nDegreeNum / 2 ;
	for ( i = 0; i < nHalfDegree; i ++ )
	{
		*(m_ptrNextDstBuf ++) = (SWORD) (ptrSrcBuffer[i * 2 + 1]) ;
	}
	return	eslErrSuccess ;
}

//
// �|�X�g�u���b�N�𕄍������ďo�͂���
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodePostBlock
	( ERISAEncodeContext & context, REAL32 rPowerScale )
{
	//
	// �_�~�[�̓��͐M���𑗐M����
	//
	unsigned int	i ;
	REAL32 *	ptrSamples = (REAL32*) m_ptrBuffer1 ;
	for ( i = 0; i < m_nDegreeNum; i ++ )
	{
		ptrSamples[i] = 0.0F ;
	}
	//
	// LOT �ϊ����{��
	//
	PerformLOT( context, ptrSamples, rPowerScale ) ;
	//
	// ������
	//
	PINT	ptrSrcBuffer = (PINT) m_ptrBuffer1 ;
	unsigned int	nHalfDegree = m_nDegreeNum / 2 ;
	for ( i = 0; i < nHalfDegree; i ++ )
	{
		*(m_ptrNextDstBuf ++) = (SWORD) (ptrSrcBuffer[i * 2 + 1]) ;
	}
	return	eslErrSuccess ;
}

//
// �ʎq��
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::Quantumize
	( PINT ptrQuantumized, const REAL32 * ptrSource,
		int nDegreeNum, REAL32 rPowerScale,
		DWORD * ptrWeightCode, int * ptrCoefficient )
{
	//
	// �֐������ݒ�
	//////////////////////////////////////////////////////////////////////////
	int		i, j ;
	int		nHalfDegree = nDegreeNum / 2 ;
	//
	// �e���g���т̃G�l���M�W���x������g�����Ƃ̗ʎq���W�����Z�o����
	//////////////////////////////////////////////////////////////////////////
	double	rPresetWeight[7] ;
	rPresetWeight[0] = rPresetWeight[1] =
		m_parameter.rLowWeight
			* pow( m_parameter.rMiddleWeight
					/ m_parameter.rLowWeight, 1.0 / 4.0 ) ;
	rPresetWeight[2] =
		m_parameter.rLowWeight
			* pow( m_parameter.rMiddleWeight
					/ m_parameter.rLowWeight, 3.0 / 4.0 ) ;
	for ( i = 3; i < 7; i ++ )
	{
		rPresetWeight[i] =
			pow( m_parameter.rMiddleWeight, ((6 - i) * 2) / 7.0 ) ;
	}
	//
	double	rAvgRatio[7] ;
	i = 0 ;
	for ( j = 0; j < 7; j ++ )
	{
		rAvgRatio[j] = 0.0 ;
		for ( int k = 0; k < m_nFrequencyWidth[j]; k ++ )
		{
			rAvgRatio[j] += sqrt( fabs( ptrSource[i ++] ) ) ;
		}
		rAvgRatio[j] = rAvgRatio[j] / m_nFrequencyWidth[j] ;
		rAvgRatio[j] *= rAvgRatio[j] ;
	}
	*ptrWeightCode = 0 ;
	double	rBaseAvg = 0.0 ;
	for ( i = 6; i >= 0; i -- )
	{
		rBaseAvg = rAvgRatio[i] ;
		if ( rBaseAvg >= 1.0 )
			break ;
	}
	for ( i = 0; i < 6; i ++ )
	{
		int		nLogRatio = 0 ;
		if ( rBaseAvg >= 1.0 )
		{
			double	rRatio =
				pow( rAvgRatio[i] / rBaseAvg / rPresetWeight[i], 0.85 ) ;
			if ( rRatio > 0.0 )
			{
				nLogRatio = ::eriRoundR32ToInt
					( (REAL32)(log( rRatio ) / log( 2.0 ) * 2.0) ) ;
				if ( nLogRatio < -15 )
					nLogRatio = -15 ;
				if ( nLogRatio > 16 )
					nLogRatio = 16 ;
			}
		}
		rAvgRatio[i] = 1.0 / pow( 2.0, nLogRatio * 0.5 ) ;
		*ptrWeightCode |= (nLogRatio + 15) << (i * 5) ;
	}
	rAvgRatio[6] = 1.0 ;
	//
	for ( i = 0; i < m_nFrequencyPoint[0]; i ++ )
	{
		m_ptrWeightTable[i] = (REAL32) rAvgRatio[0] ;
	}
	for ( j = 1; j < 7; j ++ )
	{
		double	a = rAvgRatio[j - 1] ;
		double	k = (rAvgRatio[j] - a)
					/ (m_nFrequencyPoint[j]
						- m_nFrequencyPoint[j - 1]) ;
		while ( i < m_nFrequencyPoint[j] )
		{
			m_ptrWeightTable[i ++] =
				(REAL32)(k * (i - m_nFrequencyPoint[j - 1]) + a) ;
		}
	}
	while ( i < nDegreeNum )
	{
		m_ptrWeightTable[i ++] = (REAL32) rAvgRatio[6] ;
	}
	//
	*ptrWeightCode |= ((DWORD)m_parameter.nOddWeight << 30) ;
	REAL32	rOddWeight = (REAL32)((m_parameter.nOddWeight + 2) * 0.5F) ;
	for ( i = 15; i < nDegreeNum; i += 16 )
	{
		m_ptrWeightTable[i] *= rOddWeight ;
	}
	//
	// ��Βl�̕��ς��Z�o
	//////////////////////////////////////////////////////////////////////////
	double	rAvg ;
	double	rMax = 0.0 ;
	double	rSum = 0.0 ;
	//
	for ( i = 0; i < nDegreeNum; i ++ )
	{
		double	r = fabs( ptrSource[i] * m_ptrWeightTable[i] ) ;
		if ( r > rMax )
		{
			rMax = r ;
		}
		if ( r >= 1.0 )
		{
			rSum += sqrt( r ) ;
		}
	}
	rAvg = (rSum / nDegreeNum) ;
	rAvg *= rAvg ;
	//
	// �W�����Z�o
	//////////////////////////////////////////////////////////////////////////
	double	rCoefficient ;
	double	rMinCoefficient = rMax / 0x7800 ;
	if ( rMinCoefficient < 1.0 )
	{
		rMinCoefficient = 1.0 ;
	}
	rCoefficient = rMinCoefficient ;
	if ( rAvg > rPowerScale )
	{
		rCoefficient = rAvg / rPowerScale ;
	}
	if ( rCoefficient < rMinCoefficient )
	{
		rCoefficient = rMinCoefficient ;
	}
	int		nCoefficient = ::eriRoundR32ToInt( (REAL32) rCoefficient ) ;
	if ( nCoefficient >= 0x10000 )
	{
		nCoefficient = 0xFFFF ;
	}
	*ptrCoefficient = nCoefficient ;
	//
	rCoefficient = 1.0 / nCoefficient ;
	//
	m_ptrWeightTable[nDegreeNum-1] = (REAL32) nCoefficient ;
	//
	// �ʎq��
	//////////////////////////////////////////////////////////////////////////
	for ( i = 0; i < nDegreeNum; i ++ )
	{
		int		nQuantumized = ::eriRoundR32ToInt
			( (REAL32) (ptrSource[i] * m_ptrWeightTable[i] * rCoefficient) ) ;
		if ( nQuantumized < -0x8000 )
		{
			nQuantumized = -0x8000 ;
		}
		else if ( nQuantumized > 0x7FFF )
		{
			nQuantumized = 0x7FFF ;
		}
		ptrQuantumized[i] = nQuantumized ;
	}
}

//
// 16�r�b�g�̔�t���k�i�~�h���T�C�h�X�e���I�j
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeSoundDCT_MSS
	( ERISAEncodeContext & context,
		const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf )
{
	//
	// �o�b�t�@���m��
	//
	unsigned int	nDegreeWidth = (1 << m_mioih.dwSubbandDegree) ;
	unsigned int	nSampleCount =
		(datahdr.dwSampleCount + nDegreeWidth - 1) & ~(nDegreeWidth - 1) ;
	unsigned int	nSubbandCount =
		(nSampleCount >> m_mioih.dwSubbandDegree) ;
	unsigned int	nChannelCount = m_mioih.dwChannelCount ;	// ��ɂQ
	unsigned int	nAllSampleCount = nSampleCount * nChannelCount ;
	//
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer2 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		}
		if ( m_ptrBuffer3 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer3 ) ;
		}
		m_ptrBuffer2 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_ptrBuffer3 = (SBYTE*)
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_nBufLength = nSampleCount ;
	}
	//
	m_ptrNextDstBuf = (SWORD*) m_ptrBuffer2 ;
	//
	// �\��r�b�g�𑗏o
	//
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
		{
			context.PrepareToEncodeERINACode( ) ;
		}
		else
		{
			context.PrepareToEncodeERISACode( ) ;
		}
	}
	context.OutNBits( 0, 1 ) ;
	//
	// �T�u�o���h�P�ʂŕ������A�e�XDCT�ϊ����{��
	//
	unsigned int	i, j, k ;
	const SWORD *	ptrSrcBuf = (const SWORD *) ptrWaveBuf ;
	unsigned int	nLastDivision = -1 ;
	//
	for ( i = 0; i < nSubbandCount; i ++ )
	{
		const SWORD *	ptrSubbandHead =
			ptrSrcBuf + (nDegreeWidth * nChannelCount) * i ;
		//
		unsigned int	nCopySamples =
			datahdr.dwSampleCount - (i * nDegreeWidth) ;
		if ( nCopySamples > nDegreeWidth )
		{
			nCopySamples = nDegreeWidth ;
		}
		//
		// �T���v�����O
		//
		unsigned int	nDivisionCode = 0 ;
		for ( j = 0; j < nChannelCount; j ++ )
		{
			const SWORD *	ptrSrcHead = ptrSubbandHead + j ;
			REAL32 *	ptrSamplingBuf = m_ptrSamplingBuf + j * nDegreeWidth ;
			//
			for ( k = 0; k < nCopySamples; k ++ )
			{
				ptrSamplingBuf[k] = (REAL32) *ptrSrcHead ;
				ptrSrcHead += nChannelCount ;
			}
			while ( k < nDegreeWidth )
			{
				ptrSamplingBuf[k ++] = 0.0F ;
			}
			//
			unsigned int	nDivCode = GetDivisionCode( ptrSamplingBuf ) ;
			if ( nDivCode > nDivisionCode )
			{
				nDivisionCode = nDivCode ;
			}
		}
		//
		// �����R�[�h���擾
		//
		REAL32			rPowerScale = (REAL32) m_parameter.rPowerScale ;
		unsigned int	nDivisionCount = (1 << nDivisionCode) ;
		//
		for ( k = 0; k < nDivisionCode; k ++ )
		{
			rPowerScale *= 1.0625F ;
		}
		//
		context.OutNBits( (((DWORD) nDivisionCode) << 30), 2 ) ;
		//
		// �s��T�C�Y���ω�����ۂ̏���
		//
		bool	fLeadBlock = false ;
		//
		if ( nLastDivision != nDivisionCode )
		{
			//
			// ���O�܂ł̍s�������������
			//
			if ( i != 0 )
			{
				if ( EncodePostBlock_MSS
						( context, (REAL32) m_parameter.rPowerScale ) )
				{
					return	eslErrGeneral ;
				}
			}
			//
			// �s��T�C�Y��ω������邽�߂̃p�����[�^���Z�b�g�A�b�v
			//
			nLastDivision = nDivisionCode ;
			InitializeWithDegree
				( m_mioih.dwSubbandDegree - nDivisionCode ) ;
			fLeadBlock = true ;
		}
		//
		// ���� LOT �ϊ����{��
		//
		REAL32 *	ptrNextSrc1 = m_ptrSamplingBuf ;
		REAL32 *	ptrNextSrc2 = m_ptrSamplingBuf + nDegreeWidth ;
		//
		for ( k = 0; k < nDivisionCount; k ++ )
		{
			if ( fLeadBlock )
			{
				//
				// ���[�h�u���b�N���o�͂���
				//
				if ( EncodeLeadBlock_MSS
						( context, ptrNextSrc1, ptrNextSrc2, rPowerScale ) )
				{
					return	eslErrGeneral ;
				}
				//
				fLeadBlock = false ;
			}
			else
			{
				//
				// �ʏ�u���b�N���o�͂���
				//
				if ( EncodeInternalBlock_MSS
						( context, ptrNextSrc1, ptrNextSrc2, rPowerScale ) )
				{
					return	eslErrGeneral ;
				}
			}
			//
			// ����
			//
			ptrNextSrc1 += m_nDegreeNum ;
			ptrNextSrc2 += m_nDegreeNum ;
		}
	}
	//
	// �s�������������
	//
	if ( nSubbandCount > 0 )
	{
		if ( EncodePostBlock_MSS
				( context, (REAL32) m_parameter.rPowerScale ) )
		{
			return	eslErrGeneral ;
		}
	}
	//
	// �C���^�[���[�u���ĕ�����
	//
	if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
	{
		SBYTE *	ptrHBuf = m_ptrBuffer3 ;
		SBYTE *	ptrLBuf = m_ptrBuffer3 + nAllSampleCount ;
		unsigned int	nAllSubbandCount = nSubbandCount ;
		//
		for ( i = 0; i < nDegreeWidth * 2; i ++ )
		{
			ptrSrcBuf = ((const SWORD *) m_ptrBuffer2) + i ;
			//
			for ( j = 0; j < nAllSubbandCount; j ++ )
			{
				SWORD	nValue = *ptrSrcBuf ;
				ptrSrcBuf += nDegreeWidth * 2 ;
				SBYTE	nLow = (SBYTE)((nValue << 8) >> 8) ;
				SBYTE	nHigh = (SBYTE)((nValue >> 8) ^ (nLow >> 7)) ;
				*(ptrHBuf ++) = nHigh ;
				*(ptrLBuf ++) = nLow ;
			}
		}
		//
		context.OutNBits( 0, 1 ) ;
		//
		if ( context.EncodeSymbolBytes
				( m_ptrBuffer3, nAllSampleCount * 2 ) < nAllSampleCount * 2 )
		{
			return	eslErrGeneral ;			// �G���[
		}
		//
		context.Flushout( ) ;
	}
	else
	{
		if ( context.EncodeERISACodeWords
				( (const SWORD *) m_ptrBuffer2,
						nAllSampleCount ) < nAllSampleCount )
		{
			return	eslErrGeneral ;			// �G���[
		}
		//
		context.FinishERISACode( ) ;
	}

	return	eslErrSuccess ;
}

//
// ��]�p�����[�^���擾����
//////////////////////////////////////////////////////////////////////////////
int MIOEncoder::GetRevolveCode
	( const REAL32 * ptrBuf1, const REAL32 * ptrBuf2 )
{
	REAL32	r1, r2, r3 ;
	double	s1 = 0.0, s2 = 0.0 ;
	//
	for ( unsigned int i = 0; i < m_nDegreeNum; i += 2 )
	{
		r1 = ptrBuf1[i] ;
		r2 = ptrBuf2[i] ;
		//
		if ( r1 < 0.0F )
		{
			if ( r2 < 0.0F )
			{
				r1 = -r1 ;
				r2 = -r2 ;
			}
			else
			{
				r3 = r1 ;
				r1 = r2 ;
				r2 = -r3 ;
			}
		}
		else
		{
			if ( r2 < 0.0F )
			{
				r3 = r1 ;
				r1 = -r2 ;
				r2 = r3 ;
			}
		}
		//
		s1 += r1 ;
		s2 += r2 ;
	}
	//
	double	rs = ::atan2( s2, s1 ) ;
	int		nRevCode =
		::eriRoundR32ToInt( (REAL32)(rs * 4.0 / (ERI_PI * 0.5)) ) ;
	//
	return	(nRevCode & 0x03) ;
}

//
// LOT �ϊ����{��
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::PerformLOT_MSS
	( REAL32 * ptrDst, REAL32 * ptrLapBuf, REAL32 * ptrSrc )
{
	//
	// DCT �ϊ����{��
	//
	REAL32	rMatrixScale = (REAL32) sqrt(2.0 / m_nDegreeNum) ;
	::eriFastDCT
		( m_ptrInternalBuf, 1, ptrSrc,
			m_ptrWorkBuf, m_nSubbandDegree ) ;
	::eriScalarMultiply
		( m_ptrInternalBuf, rMatrixScale, m_nDegreeNum ) ;
	//
	// LOT �ϊ����{��
	//
	::eriFastPLOT
		( m_ptrInternalBuf, m_nSubbandDegree ) ;
	::eriFastLOT
		( ptrDst, ptrLapBuf,
			m_ptrInternalBuf, m_nSubbandDegree ) ;
	::eriOddGivensMatrix
		( ptrDst, m_pRevolveParam, m_nSubbandDegree ) ;
	//
	for ( unsigned int i = 0; i < m_nDegreeNum; i ++ )
	{
		ptrLapBuf[i] = m_ptrInternalBuf[i] ;
	}
}

//
// �ʏ�̃u���b�N�𕄍������ďo�͂���
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeInternalBlock_MSS
	( ERISAEncodeContext & context,
		REAL32 * ptrSrc1, REAL32 * ptrSrc2, REAL32 rPowerScale )
{
	//
	// LOT �ϊ����{��
	//
	REAL32 *	ptrDstBuf2 = m_ptrDstBuf + m_nDegreeNum ;
	REAL32 *	ptrLapBuf2 = m_ptrLastDCT + m_nDegreeNum ;
	//
	PerformLOT_MSS( m_ptrDstBuf, m_ptrLastDCT, ptrSrc1 ) ;
	PerformLOT_MSS( ptrDstBuf2, ptrLapBuf2, ptrSrc2 ) ;
	//
	// ��]�p�����[�^���擾����
	//
	int		nRevCode1 = GetRevolveCode( m_ptrDstBuf, ptrDstBuf2 ) ;
	int		nRevCode2 = GetRevolveCode( m_ptrDstBuf + 1, ptrDstBuf2 + 1 ) ;
	DWORD	dwRevCode = (nRevCode1 << 2) | nRevCode2 ;
	//
	// ��]����
	//
	REAL32	rSin, rCos ;
	//
	rSin = (REAL32) ::sin( - nRevCode1 * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( - nRevCode1 * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( m_ptrDstBuf, ptrDstBuf2, rSin, rCos, 2, m_nDegreeNum / 2 ) ;
	//
	rSin = (REAL32) ::sin( - nRevCode2 * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( - nRevCode2 * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( m_ptrDstBuf + 1, ptrDstBuf2 + 1, rSin, rCos, 2, m_nDegreeNum / 2 ) ;
	//
	context.OutNBits( (dwRevCode << 28), 4 ) ;
	//
	// �ʎq��
	//
	DWORD	nWeightCode ;
	int		nCoefficient ;
	Quantumize_MSS
		( (PINT) m_ptrBuffer1, m_ptrDstBuf,
			m_nDegreeNum, rPowerScale,
			&nWeightCode, &nCoefficient ) ;
	//
	// �ʎq���W�����o��
	//
	context.OutNBits( nWeightCode, 32 ) ;
	context.OutNBits( (((DWORD) nCoefficient) << 16), 16 ) ;
	//
	// �o��
	//
	const INT *	ptrSrcBuf = (const INT *) m_ptrBuffer1 ;
	for ( unsigned int i = 0; i < m_nDegreeNum; i ++ )
	{
		m_ptrNextDstBuf[0] = (SWORD) ptrSrcBuf[0] ;
		m_ptrNextDstBuf[1] = (SWORD) ptrSrcBuf[1] ;
		m_ptrNextDstBuf += 2 ;
		ptrSrcBuf += 2 ;
	}
	return	eslErrSuccess ;
}

//
// ���[�h�u���b�N�𕄍������ďo�͂���
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodeLeadBlock_MSS
	( ERISAEncodeContext & context,
		REAL32 * ptrSrc1, REAL32 * ptrSrc2, REAL32 rPowerScale )
{
	//
	// �d���W����������
	//
	unsigned int	i ;
	for ( i = 0; i < m_nDegreeNum * 2; i ++ )
	{
		m_ptrLastDCT[i] = 0.0F ;
	}
	//
	// LOT �ϊ����{��
	//
	REAL32 *	ptrDstBuf2 = m_ptrDstBuf + m_nDegreeNum ;
	REAL32 *	ptrLapBuf2 = m_ptrLastDCT + m_nDegreeNum ;
	//
	PerformLOT_MSS( m_ptrDstBuf, m_ptrLastDCT, ptrSrc1 ) ;
	PerformLOT_MSS( ptrDstBuf2, ptrLapBuf2, ptrSrc2 ) ;
	//
	// ��]�p�����[�^���擾����
	//
	int		nRevCode2 = GetRevolveCode( m_ptrDstBuf + 1, ptrDstBuf2 + 1 ) ;
	//
	// ��]����
	//
	REAL32	rSin, rCos ;
	//
	rSin = (REAL32) ::sin( - nRevCode2 * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( - nRevCode2 * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( m_ptrDstBuf + 1, ptrDstBuf2 + 1, rSin, rCos, 2, m_nDegreeNum / 2 ) ;
	//
	for ( i = 0; i < m_nDegreeNum; i += 2 )
	{
		m_ptrDstBuf[i] = m_ptrDstBuf[i + 1] ;
		ptrDstBuf2[i] = ptrDstBuf2[i + 1] ;
	}
	//
	context.OutNBits( (nRevCode2 << 30), 2 ) ;
	//
	// �ʎq��
	//
	DWORD	nWeightCode ;
	int		nCoefficient ;
	Quantumize_MSS
		( (PINT) m_ptrBuffer1, m_ptrDstBuf,
			m_nDegreeNum, rPowerScale,
			&nWeightCode, &nCoefficient ) ;
	//
	// �ʎq���W�����o��
	//
	context.OutNBits( nWeightCode, 32 ) ;
	context.OutNBits( (((DWORD) nCoefficient) << 16), 16 ) ;
	//
	// �o��
	//
	const INT *	ptrSrcBuf = (const INT *) m_ptrBuffer1 ;
	for ( i = 0; i < m_nDegreeNum; i ++ )
	{
		m_ptrNextDstBuf[0] = (SWORD) ptrSrcBuf[1] ;
		m_ptrNextDstBuf ++ ;
		ptrSrcBuf += 2 ;
	}
	return	eslErrSuccess ;
}

//
// �|�X�g�u���b�N�𕄍������ďo�͂���
//////////////////////////////////////////////////////////////////////////////
ESLError MIOEncoder::EncodePostBlock_MSS
	( ERISAEncodeContext & context, REAL32 rPowerScale )
{
	//
	// �_�~�[�̓��͐M���𑗐M����
	//
	unsigned int	i ;
	REAL32 *	ptrSamples1 = (REAL32*) m_ptrBuffer1 ;
	for ( i = 0; i < m_nDegreeNum; i ++ )
	{
		ptrSamples1[i * 2]     = 0.0F ;
		ptrSamples1[i * 2 + 1] = 0.0F ;
	}
	//
	// LOT �ϊ����{��
	//
	REAL32 *	ptrDstBuf2 = m_ptrDstBuf + m_nDegreeNum ;
	REAL32 *	ptrLapBuf2 = m_ptrLastDCT + m_nDegreeNum ;
	REAL32 *	ptrSamples2 = ptrSamples1 + m_nDegreeNum ;
	//
	PerformLOT_MSS( m_ptrDstBuf, m_ptrLastDCT, ptrSamples1 ) ;
	PerformLOT_MSS( ptrDstBuf2, ptrLapBuf2, ptrSamples2 ) ;
	//
	// ��]�p�����[�^���擾����
	//
	int		nRevCode2 = GetRevolveCode( m_ptrDstBuf + 1, ptrDstBuf2 + 1 ) ;
	//
	// ��]����
	//
	REAL32	rSin, rCos ;
	//
	rSin = (REAL32) ::sin( - nRevCode2 * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( - nRevCode2 * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( m_ptrDstBuf + 1, ptrDstBuf2 + 1, rSin, rCos, 2, m_nDegreeNum / 2 ) ;
	//
	for ( i = 0; i < m_nDegreeNum; i += 2 )
	{
		m_ptrDstBuf[i] = m_ptrDstBuf[i + 1] ;
		ptrDstBuf2[i] = ptrDstBuf2[i + 1] ;
	}
	//
	context.OutNBits( (nRevCode2 << 30), 2 ) ;
	//
	// �ʎq��
	//
	DWORD	nWeightCode ;
	int		nCoefficient ;
	Quantumize_MSS
		( (PINT) m_ptrBuffer1, m_ptrDstBuf,
			m_nDegreeNum, rPowerScale,
			&nWeightCode, &nCoefficient ) ;
	//
	// �ʎq���W�����o��
	//
	context.OutNBits( nWeightCode, 32 ) ;
	context.OutNBits( (((DWORD) nCoefficient) << 16), 16 ) ;
	//
	// �o��
	//
	const INT *	ptrSrcBuf = (const INT *) m_ptrBuffer1 ;
	for ( i = 0; i < m_nDegreeNum; i ++ )
	{
		m_ptrNextDstBuf[0] = (SWORD) ptrSrcBuf[1] ;
		m_ptrNextDstBuf ++ ;
		ptrSrcBuf += 2 ;
	}
	return	eslErrSuccess ;
}

//
// �ʎq��
//////////////////////////////////////////////////////////////////////////////
void MIOEncoder::Quantumize_MSS
	( PINT ptrQuantumized, const REAL32 * ptrSource,
		int nDegreeNum, REAL32 rPowerScale,
		DWORD * ptrWeightCode, int * ptrCoefficient )
{
	//
	// �֐������ݒ�
	//////////////////////////////////////////////////////////////////////////
	int		i, j ;
	int		nHalfDegree = nDegreeNum / 2 ;
	//
	// �e���g���т̃G�l���M�W���x������g�����Ƃ̗ʎq���W�����Z�o����
	//////////////////////////////////////////////////////////////////////////
	double	rPresetWeight[7] ;
	rPresetWeight[0] = rPresetWeight[1] =
		m_parameter.rLowWeight
			* pow( m_parameter.rMiddleWeight
					/ m_parameter.rLowWeight, 1.0 / 4.0 ) ;
	rPresetWeight[2] =
		m_parameter.rLowWeight
			* pow( m_parameter.rMiddleWeight
					/ m_parameter.rLowWeight, 3.0 / 4.0 ) ;
	for ( i = 3; i < 7; i ++ )
	{
		rPresetWeight[i] =
			pow( m_parameter.rMiddleWeight, ((6 - i) * 2) / 7.0 ) ;
	}
	//
	double	rAvgRatio[7] ;
	i = 0 ;
	for ( j = 0; j < 7; j ++ )
	{
		rAvgRatio[j] = 0.0 ;
		for ( int k = 0; k < m_nFrequencyWidth[j]; k ++ )
		{
			rAvgRatio[j] += sqrt( fabs( ptrSource[i] ) ) ;
			rAvgRatio[j] += sqrt( fabs( ptrSource[m_nDegreeNum + i] ) ) ;
			++ i ;
		}
		rAvgRatio[j] = rAvgRatio[j] * 0.5 / m_nFrequencyWidth[j] ;
		rAvgRatio[j] *= rAvgRatio[j] ;
	}
	*ptrWeightCode = 0 ;
	double	rBaseAvg = 0.0 ;
	for ( i = 6; i >= 0; i -- )
	{
		rBaseAvg = rAvgRatio[i] ;
		if ( rBaseAvg >= 1.0 )
			break ;
	}
	for ( i = 0; i < 6; i ++ )
	{
		int		nLogRatio = 0 ;
		if ( rBaseAvg >= 1.0 )
		{
			double	rRatio =
				pow( rAvgRatio[i] / rBaseAvg / rPresetWeight[i], 0.85 ) ;
			if ( rRatio > 0.0 )
			{
				nLogRatio = ::eriRoundR32ToInt
					( (REAL32)(log( rRatio ) / log( 2.0 ) * 2.0) ) ;
				if ( nLogRatio < -15 )
					nLogRatio = -15 ;
				if ( nLogRatio > 16 )
					nLogRatio = 16 ;
			}
		}
		rAvgRatio[i] = 1.0 / pow( 2.0, nLogRatio * 0.5 ) ;
		*ptrWeightCode |= (nLogRatio + 15) << (i * 5) ;
	}
	rAvgRatio[6] = 1.0 ;
	//
	for ( i = 0; i < m_nFrequencyPoint[0]; i ++ )
	{
		m_ptrWeightTable[i] = (REAL32) rAvgRatio[0] ;
	}
	for ( j = 1; j < 7; j ++ )
	{
		double	a = rAvgRatio[j - 1] ;
		double	k = (rAvgRatio[j] - a)
					/ (m_nFrequencyPoint[j]
						- m_nFrequencyPoint[j - 1]) ;
		while ( i < m_nFrequencyPoint[j] )
		{
			m_ptrWeightTable[i ++] =
				(REAL32)(k * (i - m_nFrequencyPoint[j - 1]) + a) ;
		}
	}
	while ( i < nDegreeNum )
	{
		m_ptrWeightTable[i ++] = (REAL32) rAvgRatio[6] ;
	}
	//
	*ptrWeightCode |= ((DWORD)m_parameter.nOddWeight << 30) ;
	REAL32	rOddWeight = (REAL32)((m_parameter.nOddWeight + 2) * 0.5F) ;
	for ( i = 15; i < nDegreeNum; i += 16 )
	{
		m_ptrWeightTable[i] *= rOddWeight ;
	}
	//
	// ��Βl�̕��ς��Z�o
	//////////////////////////////////////////////////////////////////////////
	double	rAvg ;
	double	rMax = 0.0 ;
	double	rSum = 0.0 ;
	//
	for ( i = 0; i < nDegreeNum; i ++ )
	{
		double	r ;
		r = fabs( ptrSource[i] * m_ptrWeightTable[i] ) ;
		if ( r > rMax )
		{
			rMax = r ;
		}
		if ( r >= 1.0 )
		{
			rSum += sqrt( r ) ;
		}
		//
		r = fabs( ptrSource[m_nDegreeNum + i] * m_ptrWeightTable[i] ) ;
		if ( r > rMax )
		{
			rMax = r ;
		}
		if ( r >= 1.0 )
		{
			rSum += sqrt( r ) ;
		}
	}
	rAvg = (rSum / nDegreeNum / 2) ;
	rAvg *= rAvg ;
	//
	// �W�����Z�o
	//////////////////////////////////////////////////////////////////////////
	double	rCoefficient ;
	double	rMinCoefficient = rMax / 0x7800 ;
	if ( rMinCoefficient < 1.0 )
	{
		rMinCoefficient = 1.0 ;
	}
	rCoefficient = rMinCoefficient ;
	if ( rAvg > rPowerScale )
	{
		rCoefficient = rAvg / rPowerScale ;
	}
	if ( rCoefficient < rMinCoefficient )
	{
		rCoefficient = rMinCoefficient ;
	}
	int		nCoefficient = ::eriRoundR32ToInt( (REAL32) rCoefficient ) ;
	if ( nCoefficient >= 0x10000 )
	{
		nCoefficient = 0xFFFF ;
	}
	*ptrCoefficient = nCoefficient ;
	//
	rCoefficient = 1.0 / nCoefficient ;
	//
	m_ptrWeightTable[nDegreeNum-1] = (REAL32) nCoefficient ;
	//
	// �ʎq��
	//////////////////////////////////////////////////////////////////////////
	for ( j = 0; j < 2; j ++ )
	{
		for ( i = 0; i < nDegreeNum; i ++ )
		{
			int		nQuantumized = ::eriRoundR32ToInt
				( (REAL32) (*(ptrSource ++) * m_ptrWeightTable[i] * rCoefficient) ) ;
			if ( nQuantumized < -0x8000 )
			{
				nQuantumized = -0x8000 ;
			}
			else if ( nQuantumized > 0x7FFF )
			{
				nQuantumized = 0x7FFF ;
			}
			*(ptrQuantumized ++) = nQuantumized ;
		}
	}
}


/*****************************************************************************
                            �����W�J�I�u�W�F�N�g
 *****************************************************************************/

IMPLEMENT_CLASS_INFO( MIODecoder, ESLObject )

//
// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
MIODecoder::MIODecoder( void )
{
	m_nBufLength = 0 ;
	m_ptrBuffer1 = NULL ;
	m_ptrBuffer2 = NULL ;
	m_ptrBuffer3 = NULL ;
	m_ptrDivisionTable = NULL ;
	m_ptrRevolveCode = NULL ;
	m_ptrWeightCode = NULL ;
	m_ptrCoefficient = NULL ;
	m_ptrMatrixBuf = NULL ;
	m_ptrInternalBuf = NULL ;
	m_ptrWorkBuf = NULL ;
	m_ptrWorkBuf2 = NULL ;
	m_ptrWeightTable = NULL ;
	m_ptrLastDCT = NULL ;
	m_pRevolveParam = NULL ;
}

//
// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
MIODecoder::~MIODecoder( void )
{
	Delete( ) ;
}

//
// �������i�p�����[�^�̐ݒ�j
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::Initialize( const MIO_INFO_HEADER & infhdr )
{
	//
	// �ȑO�̃��\�[�X�����
	Delete( ) ;
	//
	// �������w�b�_���R�s�[
	m_mioih = infhdr ;
	//
	// �p�����[�^�̃`�F�b�N
	if ( m_mioih.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		if ( m_mioih.dwArchitecture != ERI_RUNLENGTH_HUFFMAN )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̈��k�t�H�[�}�b�g�j
		}
		if ( (m_mioih.dwChannelCount != 1) && (m_mioih.dwChannelCount != 2) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃`���l�����j
		}
		if ( (m_mioih.dwBitsPerSample != 8) && (m_mioih.dwBitsPerSample != 16) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃T���v�����O����\�j
		}
	}
	else if ( (m_mioih.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_mioih.fdwTransformation == CVTYPE_LOT_ERI_MSS) )
	{
		if ( (m_mioih.dwArchitecture != ERI_RUNLENGTH_GAMMA)
			&& (m_mioih.dwArchitecture != ERI_RUNLENGTH_HUFFMAN)
			&& (m_mioih.dwArchitecture != ERISA_NEMESIS_CODE) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̈��k�t�H�[�}�b�g�j
		}
		if ( (m_mioih.dwChannelCount != 1) && (m_mioih.dwChannelCount != 2) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃`���l�����j
		}
		if ( m_mioih.dwBitsPerSample != 16 )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̃T���v�����O����\�j
		}
		if ( (m_mioih.dwSubbandDegree < 8) ||
				(m_mioih.dwSubbandDegree > MAX_DCT_DEGREE) )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή���DCT�����j
		}
		if ( m_mioih.dwLappedDegree != 1 )
		{
			return	eslErrGeneral ;		// �G���[�i���Ή��̏d���W���j
		}
		//
		// DCT �p�o�b�t�@�m��
		m_ptrBuffer1 =
			::eslHeapAllocate
				( NULL, m_mioih.dwChannelCount *
					(sizeof(REAL32) << m_mioih.dwSubbandDegree), 0 ) ;
		m_ptrMatrixBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_mioih.dwChannelCount *
					(sizeof(REAL32) << m_mioih.dwSubbandDegree), 0 ) ;
		m_ptrInternalBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_mioih.dwChannelCount *
					(sizeof(REAL32) << m_mioih.dwSubbandDegree), 0 ) ;
		m_ptrWorkBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, sizeof(REAL32) << m_mioih.dwSubbandDegree, 0 ) ;
		m_ptrWorkBuf2 =
			(REAL32*) ::eslHeapAllocate
				( NULL, sizeof(REAL32) << m_mioih.dwSubbandDegree, 0 ) ;
		//
		// �d�݃e�[�u�����m��
		m_ptrWeightTable =
			(REAL32*) ::eslHeapAllocate
				( NULL, sizeof(REAL32) << m_mioih.dwSubbandDegree, 0 ) ;
		//
		// LOT �p�o�b�t�@�m��
		unsigned int	i, nBlocksetSamples, nLappedSamples ;
		nBlocksetSamples = m_mioih.dwChannelCount << m_mioih.dwSubbandDegree ;
		nLappedSamples = nBlocksetSamples * m_mioih.dwLappedDegree ;
		if ( nLappedSamples > 0 )
		{
			m_ptrLastDCT =
				(REAL32*) ::eslHeapAllocate
					( NULL, sizeof(REAL32) * nLappedSamples, 0 ) ;
			for ( i = 0; i < nLappedSamples; i ++ )
			{
				m_ptrLastDCT[i] = 0.0F ;
			}
		}
		//
		// �p�����[�^������
		InitializeWithDegree( m_mioih.dwSubbandDegree ) ;
	}
	else
	{
		return	eslErrGeneral ;		// �i���Ή��̈��k�t�H�[�}�b�g�j
	}
	//
	// ����I��
	return	eslErrSuccess ;
}

//
// �I���i�������̉���Ȃǁj
//////////////////////////////////////////////////////////////////////////////
void MIODecoder::Delete( void )
{
	if ( m_ptrBuffer1 != NULL )
	{
		::eslHeapFree( NULL, m_ptrBuffer1 ) ;
		m_ptrBuffer1 = NULL ;
	}
	if ( m_ptrBuffer2 != NULL )
	{
		::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		m_ptrBuffer2 = NULL ;
	}
	if ( m_ptrBuffer3 != NULL )
	{
		::eslHeapFree( NULL, m_ptrBuffer3 ) ;
		m_ptrBuffer3 = NULL ;
	}
	if ( m_ptrDivisionTable != NULL )
	{
		::eslHeapFree( NULL, m_ptrDivisionTable ) ;
		m_ptrDivisionTable = NULL ;
	}
	if ( m_ptrRevolveCode != NULL )
	{
		::eslHeapFree( NULL, m_ptrRevolveCode ) ;
		m_ptrRevolveCode = NULL ;
	}
	if ( m_ptrWeightCode != NULL )
	{
		::eslHeapFree( NULL, m_ptrWeightCode ) ;
		m_ptrWeightCode = NULL ;
	}
	if ( m_ptrCoefficient != NULL )
	{
		::eslHeapFree( NULL, m_ptrCoefficient ) ;
		m_ptrCoefficient = NULL ;
	}
	if ( m_ptrMatrixBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrMatrixBuf ) ;
		m_ptrMatrixBuf = NULL ;
	}
	if ( m_ptrInternalBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrInternalBuf ) ;
		m_ptrInternalBuf = NULL ;
	}
	if ( m_ptrWorkBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrWorkBuf ) ;
		m_ptrWorkBuf = NULL ;
	}
	if ( m_ptrWorkBuf2 != NULL )
	{
		::eslHeapFree( NULL, m_ptrWorkBuf2 ) ;
		m_ptrWorkBuf2 = NULL ;
	}
	if ( m_ptrWeightTable != NULL )
	{
		::eslHeapFree( NULL, m_ptrWeightTable ) ;
		m_ptrWeightTable = NULL ;
	}
	if ( m_ptrLastDCT != NULL )
	{
		::eslHeapFree( NULL, m_ptrLastDCT ) ;
		m_ptrLastDCT = NULL ;
	}
	if ( m_pRevolveParam != NULL )
	{
		::eslHeapFree( NULL, m_pRevolveParam ) ;
		m_pRevolveParam = NULL ;
	}
	m_nBufLength = 0 ;
}

//
// ���������k
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeSound
	( ERISADecodeContext & context,
		const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf )
{
	context.FlushBuffer( ) ;
	//
	if ( m_mioih.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		if ( m_mioih.dwBitsPerSample == 8 )
		{
			return	DecodeSoundPCM8( context, datahdr, ptrWaveBuf ) ;
		}
		else if ( m_mioih.dwBitsPerSample == 16 )
		{
			return	DecodeSoundPCM16( context, datahdr, ptrWaveBuf ) ;
		}
	}
	else if ( (m_mioih.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_mioih.fdwTransformation == CVTYPE_LOT_ERI_MSS) )
	{
		if ( (m_mioih.dwChannelCount != 2) ||
				(m_mioih.fdwTransformation == CVTYPE_LOT_ERI) )
		{
			return	DecodeSoundDCT( context, datahdr, ptrWaveBuf ) ;
		}
		else
		{
			return	DecodeSoundDCT_MSS( context, datahdr, ptrWaveBuf ) ;
		}
	}

	return	eslErrGeneral ;			// �G���[
}

//
// 8�r�b�g��PCM��W�J
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeSoundPCM8
	( ERISADecodeContext & context,
		const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf )
{
	//
	// ���Z�p�o�b�t�@���m��
	unsigned int	nSampleCount = datahdr.dwSampleCount ;
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer1 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer1 ) ;
		}
		m_ptrBuffer1 = ::eslHeapAllocate
			( NULL, nSampleCount * m_mioih.dwChannelCount, 0 ) ;
		m_nBufLength = nSampleCount ;
	}
	//
	// �n�t�}�������𕜍�
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		context.PrepareToDecodeERINACode( ) ;
	}
	ULONG	nBytes = nSampleCount * m_mioih.dwChannelCount ;
	if ( context.DecodeSymbolBytes
			( (SBYTE *) m_ptrBuffer1, nBytes ) < nBytes )
	{
		return	eslErrGeneral ;			// �G���[
	}
	//
	// �����������{���ďo��
	PBYTE	ptrSrcBuf = (PBYTE) m_ptrBuffer1 ;
	PBYTE	ptrDstBuf ;
	unsigned int	nStep = m_mioih.dwChannelCount ;
	unsigned int	i, j ;
	for ( i = 0; i < m_mioih.dwChannelCount; i ++ )
	{
		ptrDstBuf = (PBYTE) ptrWaveBuf ;
		ptrDstBuf += i ;
		//
		BYTE	bytValue = 0 ;
		for ( j = 0; j < nSampleCount; j ++ )
		{
			bytValue += *(ptrSrcBuf ++) ;
			*ptrDstBuf = bytValue ;
			ptrDstBuf += nStep ;
		}
	}
	//
	return	eslErrSuccess ;
}

//
// 16�r�b�g��PCM��W�J
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeSoundPCM16
	( ERISADecodeContext & context,
		const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf )
{
	//
	// ���Z�p�o�b�t�@���m��
	//
	unsigned int	i, j ;
	unsigned int	nSampleCount = datahdr.dwSampleCount ;
	unsigned int	nChannelCount = m_mioih.dwChannelCount ;
	unsigned int	nAllSampleCount = nSampleCount * nChannelCount ;
	//
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer1 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer1 ) ;
		}
		if ( m_ptrBuffer2 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		}
		m_ptrBuffer1 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_ptrBuffer2 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		//
		m_nBufLength = nSampleCount ;
	}
	//
	// �n�t�}�������𕜍�
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		context.PrepareToDecodeERINACode( ) ;
	}
	ULONG	nBytes = nSampleCount * nChannelCount * sizeof(SWORD) ;
	if ( context.DecodeSymbolBytes
			( (SBYTE *) m_ptrBuffer1, nBytes ) < nBytes )
	{
		return	eslErrGeneral ;			// �G���[
	}
	//
	// ��ʃo�C�g�Ɖ��ʃo�C�g���p�b�L���O
	PBYTE	pbytSrcBuf1, pbytSrcBuf2, pbytDstBuf ;
	for ( i = 0; i < m_mioih.dwChannelCount; i ++ )
	{
		unsigned int	nOffset = i * nSampleCount * sizeof(SWORD) ;
		pbytSrcBuf1 = ((PBYTE) m_ptrBuffer1) + nOffset ;
		pbytSrcBuf2 = pbytSrcBuf1 + nSampleCount ;
		pbytDstBuf = ((PBYTE) m_ptrBuffer2) + nOffset ;
		//
		for ( j = 0; j < nSampleCount; j ++ )
		{
			SBYTE	bytLow = pbytSrcBuf2[j] ;
			SBYTE	bytHigh = pbytSrcBuf1[j] ;
			pbytDstBuf[j * sizeof(SWORD) + 0] = bytLow ;
			pbytDstBuf[j * sizeof(SWORD) + 1] = bytHigh ^ (bytLow >> 7) ;
		}
	}
	//
	// �����������{���ďo��
	SWORD *	ptrSrcBuf = (SWORD*) m_ptrBuffer2 ;
	SWORD *	ptrDstBuf ;
	unsigned int	nStep = m_mioih.dwChannelCount ;
	for ( i = 0; i < m_mioih.dwChannelCount; i ++ )
	{
		ptrDstBuf = (SWORD*) ptrWaveBuf ;
		ptrDstBuf += i ;
		//
		SWORD	wValue = 0 ;
		SWORD	wDelta = 0 ;
		for ( j = 0; j < nSampleCount; j ++ )
		{
			wDelta +=  *(ptrSrcBuf ++) ;
			wValue += wDelta ;
			*ptrDstBuf = wValue ;
			ptrDstBuf += nStep ;
		}
	}
	//
	return	eslErrSuccess ;
}

//
// �s��T�C�Y�̕ύX�ɔ����p�����[�^�̍Čv�Z
//////////////////////////////////////////////////////////////////////////////
void MIODecoder::InitializeWithDegree( unsigned int nSubbandDegree )
{
	//
	// ��]�p�����[�^����
	if ( m_pRevolveParam != NULL )
	{
		::eslHeapFree( NULL, m_pRevolveParam ) ;
	}
	m_pRevolveParam = ::eriCreateRevolveParameter( nSubbandDegree ) ;
	//
	// �t�ʎq���p�p�����[�^����
	static const int	freq_width[7] =
	{
		-6, -6, -5, -4, -3, -2, -1
	} ;
	for ( int i = 0, j = 0; i < 7; i ++ )
	{
		int		nFrequencyWidth = 1 << (nSubbandDegree + freq_width[i]) ;
		m_nFrequencyPoint[i] = j + (nFrequencyWidth / 2) ;
		j += nFrequencyWidth ;
	}
	//
	// ���[�J���p�����[�^��ݒ�
	m_nSubbandDegree = nSubbandDegree ;
	m_nDegreeNum = (1 << nSubbandDegree) ;
}

//
// 16�r�b�g�̔�t�W�J
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeSoundDCT
	( ERISADecodeContext & context,
		const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf )
{
	//
	// �o�b�t�@���m��
	//
	unsigned int	i, j, k ;
	unsigned int	nDegreeWidth = (1 << m_mioih.dwSubbandDegree) ;
	unsigned int	nSampleCount =
		(datahdr.dwSampleCount + nDegreeWidth - 1) & ~(nDegreeWidth - 1) ;
	unsigned int	nSubbandCount =
		(nSampleCount >> m_mioih.dwSubbandDegree) ;
	unsigned int	nChannelCount = m_mioih.dwChannelCount ;
	unsigned int	nAllSampleCount = nSampleCount * nChannelCount ;
	unsigned int	nAllSubbandCount = nSubbandCount * nChannelCount ;
	//
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer2 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		}
		if ( m_ptrBuffer3 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer3 ) ;
		}
		if ( m_ptrDivisionTable != NULL )
		{
			::eslHeapFree( NULL, m_ptrDivisionTable ) ;
		}
		if ( m_ptrWeightCode != NULL )
		{
			::eslHeapFree( NULL, m_ptrWeightCode ) ;
		}
		if ( m_ptrCoefficient != NULL )
		{
			::eslHeapFree( NULL, m_ptrCoefficient ) ;
		}
		m_ptrBuffer2 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(INT), 0 ) ;
		m_ptrBuffer3 = (SBYTE*)
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_ptrDivisionTable = (PBYTE)
			::eslHeapAllocate( NULL, nAllSubbandCount * sizeof(BYTE), 0 ) ;
		m_ptrWeightCode = (SDWORD*)
			::eslHeapAllocate( NULL, nAllSubbandCount * 5 * sizeof(SDWORD), 0 ) ;
		m_ptrCoefficient = (PINT)
			::eslHeapAllocate( NULL, nAllSubbandCount * 5 * sizeof(INT), 0 ) ;
		//
		m_nBufLength = nSampleCount ;
	}
	//
	// �ʎq���W���e�[�u���𕜍�
	//
	if ( context.GetABit( ) != 0 )
	{
		return	eslErrGeneral ;
	}
	//
	unsigned int *	pLastDivision = new unsigned int [nChannelCount] ;
	m_ptrNextDivision = m_ptrDivisionTable ;
	m_ptrNextWeight = m_ptrWeightCode ;
	m_ptrNextCoefficient = m_ptrCoefficient ;
	//
	for ( i = 0; i < nChannelCount; i ++ )
	{
		pLastDivision[i] = -1 ;
	}
	for ( i = 0; i < nSubbandCount; i ++ )
	{
		for ( j = 0; j < nChannelCount; j ++ )
		{
			unsigned int	nDivisionCode = context.GetNBits( 2 ) ;
			*(m_ptrNextDivision ++) = (BYTE) nDivisionCode ;
			//
			if ( nDivisionCode != pLastDivision[j] )
			{
				if ( i != 0 )
				{
					*(m_ptrNextWeight ++) = context.GetNBits( 32 ) ;
					*(m_ptrNextCoefficient ++) = context.GetNBits( 16 ) ;
				}
				pLastDivision[j] = nDivisionCode ;
			}
			//
			unsigned int	nDivisionCount = (1 << nDivisionCode) ;
			for ( k = 0; k < nDivisionCount; k ++ )
			{
				*(m_ptrNextWeight ++) = context.GetNBits( 32 ) ;
				*(m_ptrNextCoefficient ++) = context.GetNBits( 16 ) ;
			}
		}
	}
	if ( nSubbandCount > 0 )
	{
		for ( i = 0; i < nChannelCount; i ++ )
		{
			*(m_ptrNextWeight ++) = context.GetNBits( 32 ) ;
			*(m_ptrNextCoefficient ++) = context.GetNBits( 16 ) ;
		}
	}
	//
	if ( context.GetABit( ) != 0 )
	{
		return	eslErrGeneral ;
	}
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
		{
			context.PrepareToDecodeERINACode( ) ;
		}
		else
		{
			context.PrepareToDecodeERISACode( ) ;
		}
	}
	else if ( m_mioih.dwArchitecture == ERISA_NEMESIS_CODE )
	{
		context.InitializeERISACode( ) ;
	}
	//
	// �������ċt�C���^�[���u
	//
	if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
	{
		if ( context.DecodeSymbolBytes
				( m_ptrBuffer3, nAllSampleCount * 2 ) < nAllSampleCount * 2 )
		{
			return	eslErrGeneral ;			// �G���[
		}
		//
		SBYTE *	ptrHBuf = m_ptrBuffer3 ;
		SBYTE *	ptrLBuf = m_ptrBuffer3 + nAllSampleCount ;
		//
		for ( i = 0; i < nDegreeWidth; i ++ )
		{
			PINT	ptrQuantumized = ((PINT) m_ptrBuffer2) + i ;
			//
			for ( j = 0; j < nAllSubbandCount; j ++ )
			{
				INT		nLow = *(ptrLBuf ++) ;
				INT		nHigh = *(ptrHBuf ++) ^ (nLow >> 8) ;
				*ptrQuantumized = (nLow & 0xFF) | (nHigh << 8) ;
				ptrQuantumized += nDegreeWidth ;
			}
		}
	}
	else
	{
		if ( context.DecodeERISACodeWords
				( (SWORD*) m_ptrBuffer3, nAllSampleCount ) < nAllSampleCount )
		{
			return	eslErrGeneral ;			// �G���[
		}
		for ( i = 0; i < nAllSampleCount; i ++ )
		{
			((PINT)m_ptrBuffer2)[i] = ((SWORD*)m_ptrBuffer3)[i] ;
		}
	}
	//
	// �T�u�o���h�P�ʂŋt DCT �ϊ����{��
	//
	unsigned int	nSamples ;
	unsigned int *	pRestSamples = new unsigned int [nChannelCount] ;
	SWORD **		ptrDstBuf = new SWORD * [nChannelCount] ;
	//
	m_ptrNextDivision = m_ptrDivisionTable ;
	m_ptrNextWeight = m_ptrWeightCode ;
	m_ptrNextCoefficient = m_ptrCoefficient ;
	m_ptrNextSource = (PINT) m_ptrBuffer2 ;
	//
	for ( i = 0; i < nChannelCount; i ++ )
	{
		pLastDivision[i] = -1 ;
		pRestSamples[i] = datahdr.dwSampleCount ;
		ptrDstBuf[i] = ((SWORD*) ptrWaveBuf) + i ;
	}
	unsigned int	nCurrentDivision = -1 ;
	//
	for ( i = 0; i < nSubbandCount; i ++ )
	{
		for ( j = 0; j < nChannelCount; j ++ )
		{
			//
			// �����R�[�h���擾
			//
			unsigned int	nDivisionCode = *(m_ptrNextDivision ++) ;
			unsigned int	nDivisionCount = (1 << nDivisionCode) ;
			//
			// �d�������p�o�b�t�@���擾
			//
			int		nChannelStep = nDegreeWidth * m_mioih.dwLappedDegree * j ;
			m_ptrLastDCTBuf = m_ptrLastDCT + nChannelStep ;
			//
			// �s��T�C�Y���ω�����ۂ̏���
			//
			bool	fLeadBlock = false ;
			//
			if ( pLastDivision[j] != nDivisionCode )
			{
				//
				// ���O�܂ł̍s�������������
				//
				if ( i != 0 )
				{
					if ( nCurrentDivision != pLastDivision[j] )
					{
						InitializeWithDegree
							( m_mioih.dwSubbandDegree - pLastDivision[j] ) ;
						nCurrentDivision = pLastDivision[j] ;
					}
					nSamples = pRestSamples[j] ;
					if ( nSamples > m_nDegreeNum )
					{
						nSamples = m_nDegreeNum ;
					}
					if ( DecodePostBlock( ptrDstBuf[j], nSamples ) )
					{
						return	eslErrGeneral ;
					}
					pRestSamples[j] -= nSamples ;
					ptrDstBuf[j] += nSamples * nChannelCount ;
				}
				//
				// �s��T�C�Y��ω������邽�߂̃p�����[�^���Z�b�g�A�b�v
				//
				pLastDivision[j] = nDivisionCode ;
				fLeadBlock = true ;
			}
			if ( nCurrentDivision != nDivisionCode )
			{
				InitializeWithDegree
					( m_mioih.dwSubbandDegree - nDivisionCode ) ;
				nCurrentDivision = nDivisionCode ;
			}
			//
			// �����t LOT �ϊ����{��
			//
			for ( k = 0; k < nDivisionCount; k ++ )
			{
				if ( fLeadBlock )
				{
					//
					// ���[�h�u���b�N�𕜍�����
					//
					if ( DecodeLeadBlock( ) )
					{
						return	eslErrGeneral ;
					}
					//
					fLeadBlock = false ;
				}
				else
				{
					//
					// �ʏ�u���b�N�𕜍�����
					//
					nSamples = pRestSamples[j] ;
					if ( nSamples > m_nDegreeNum )
					{
						nSamples = m_nDegreeNum ;
					}
					if ( DecodeInternalBlock
							( ptrDstBuf[j], nSamples ) )
					{
						return	eslErrGeneral ;
					}
					pRestSamples[j] -= nSamples ;
					ptrDstBuf[j] += nSamples * nChannelCount ;
				}
			}
		}
	}
	//
	// �s�������������
	//
	if ( nSubbandCount > 0 )
	{
		for ( i = 0; i < nChannelCount; i ++ )
		{
			int		nChannelStep = nDegreeWidth * m_mioih.dwLappedDegree * i ;
			m_ptrLastDCTBuf = m_ptrLastDCT + nChannelStep ;
			//
			if ( nCurrentDivision != pLastDivision[i] )
			{
				InitializeWithDegree
					( m_mioih.dwSubbandDegree - pLastDivision[i] ) ;
				nCurrentDivision = pLastDivision[i] ;
			}
			nSamples = pRestSamples[i] ;
			if ( nSamples > m_nDegreeNum )
			{
				nSamples = m_nDegreeNum ;
			}
			if ( DecodePostBlock( ptrDstBuf[i], nSamples ) )
			{
				return	eslErrGeneral ;
			}
			pRestSamples[i] -= nSamples ;
			ptrDstBuf[i] += nSamples * nChannelCount ;
		}
	}
	//
	delete []	pLastDivision ;
	delete []	pRestSamples ;
	delete []	ptrDstBuf ;

	return	eslErrSuccess ;
}

//
// �ʏ�̃u���b�N�𕜍�����
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeInternalBlock
	( SWORD * ptrDst, unsigned int nSamples )
{
	//
	// �t�ʎq��
	//
	SDWORD	nWeightCode = *(m_ptrNextWeight ++) ;
	int		nCoefficient = *(m_ptrNextCoefficient ++) ;
	IQuantumize
		( m_ptrMatrixBuf, m_ptrNextSource,
			m_nDegreeNum, nWeightCode, nCoefficient ) ;
	m_ptrNextSource += m_nDegreeNum ;
	//
	// �t LOT ���{��
	//
	::eriOddGivensInverseMatrix
		( m_ptrMatrixBuf, m_pRevolveParam, m_nSubbandDegree ) ;
	::eriFastIPLOT
		( m_ptrMatrixBuf, m_nSubbandDegree ) ;
	::eriFastILOT
		( m_ptrWorkBuf, m_ptrLastDCTBuf,
			m_ptrMatrixBuf, m_nSubbandDegree ) ;
	//
	for ( unsigned int i = 0; i < m_nDegreeNum; i ++ )
	{
		m_ptrLastDCTBuf[i] = m_ptrMatrixBuf[i] ;
		m_ptrMatrixBuf[i] = m_ptrWorkBuf[i] ;
	}
	//
	// �t DCT �ϊ����{��
	//
	::eriFastIDCT
		( m_ptrInternalBuf, m_ptrMatrixBuf,
			1, m_ptrWorkBuf, m_nSubbandDegree ) ;
	//
	// �X�g�A
	//
	if ( nSamples != 0 )
	{
		::eriRoundR32ToWordArray
			( ptrDst, m_mioih.dwChannelCount,
				m_ptrInternalBuf, nSamples ) ;
	}
	//
	return	eslErrSuccess ;
}

//
// ���[�h�u���b�N�𕜍�����
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeLeadBlock( void )
{
	//
	// �t�ʎq��
	//
	SDWORD	nWeightCode = *(m_ptrNextWeight ++) ;
	int		nCoefficient = *(m_ptrNextCoefficient ++) ;
	unsigned int	i ;
	unsigned int	nHalfDegree = m_nDegreeNum / 2 ;
	PINT	ptrSrcBuf = (PINT) m_ptrBuffer1 ;
	for ( i = 0; i < nHalfDegree; i ++ )
	{
		ptrSrcBuf[i * 2] = 0 ;
		ptrSrcBuf[i * 2 + 1] = *(m_ptrNextSource ++) ;
	}
	IQuantumize
		( m_ptrLastDCTBuf, ptrSrcBuf,
			m_nDegreeNum, nWeightCode, nCoefficient ) ;
	//
	// �d���p�����[�^��ݒ肷��
	//
	::eriOddGivensInverseMatrix
		( m_ptrLastDCTBuf, m_pRevolveParam, m_nSubbandDegree ) ;
	//
	for ( i = 0; i < m_nDegreeNum; i += 2 )
	{
		m_ptrLastDCTBuf[i] = m_ptrLastDCTBuf[i + 1] ;
	}
	//
	::eriFastIPLOT
		( m_ptrLastDCTBuf, m_nSubbandDegree ) ;
	//
	return	eslErrSuccess ;
}

//
// �|�X�g�u���b�N�𕜍�����
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodePostBlock
	( SWORD * ptrDst, unsigned int nSamples )
{
	//
	// �t�ʎq��
	//
	SDWORD	nWeightCode = *(m_ptrNextWeight ++) ;
	int		nCoefficient = *(m_ptrNextCoefficient ++) ;
	unsigned int	i ;
	unsigned int	nHalfDegree = m_nDegreeNum / 2 ;
	PINT	ptrSrcBuf = (PINT) m_ptrBuffer1 ;
	for ( i = 0; i < nHalfDegree; i ++ )
	{
		ptrSrcBuf[i * 2] = 0 ;
		ptrSrcBuf[i * 2 + 1] = *(m_ptrNextSource ++) ;
	}
	IQuantumize
		( m_ptrMatrixBuf, ptrSrcBuf,
			m_nDegreeNum, nWeightCode, nCoefficient ) ;
	//
	// �t LOT ���{��
	//
	::eriOddGivensInverseMatrix
		( m_ptrMatrixBuf, m_pRevolveParam, m_nSubbandDegree ) ;
	//
	for ( i = 0; i < m_nDegreeNum; i += 2 )
	{
		m_ptrMatrixBuf[i] = - m_ptrMatrixBuf[i + 1] ;
	}
	//
	::eriFastIPLOT
		( m_ptrMatrixBuf, m_nSubbandDegree ) ;
	::eriFastILOT
		( m_ptrWorkBuf, m_ptrLastDCTBuf,
			m_ptrMatrixBuf, m_nSubbandDegree ) ;
	//
	for ( i = 0; i < m_nDegreeNum; i ++ )
	{
		m_ptrMatrixBuf[i] = m_ptrWorkBuf[i] ;
	}
	//
	// �t DCT �ϊ����{��
	//
	::eriFastIDCT
		( m_ptrInternalBuf, m_ptrMatrixBuf,
			1, m_ptrWorkBuf, m_nSubbandDegree ) ;
	//
	// �X�g�A
	//
	if ( nSamples != 0 )
	{
		::eriRoundR32ToWordArray
			( ptrDst, m_mioih.dwChannelCount,
				m_ptrInternalBuf, nSamples ) ;
	}
	//
	return	eslErrSuccess ;
}

//
// �t�ʎq��
//////////////////////////////////////////////////////////////////////////////
void MIODecoder::IQuantumize
	( REAL32 * ptrDestination,
		const INT * ptrQuantumized, int nDegreeNum,
		SDWORD nWeightCode, int nCoefficient )
{
	//
	// �֐������ݒ�
	//
	int		i, j ;
	//
	// �W�����Z�o
	//
	double	rMatrixScale = sqrt(2.0 / nDegreeNum) ;
	double	rCoefficient = rMatrixScale * nCoefficient ;
	//
	// �d�݃e�[�u���𐶐�����
	//
	double	rAvgRatio[7] ;
	for ( i= 0; i < 6; i++ )
	{
		rAvgRatio[i] =
			1.0 / pow( 2.0, (((nWeightCode >> (i * 5)) & 0x1F) - 15) * 0.5 ) ;
	}
	rAvgRatio[6] = 1.0 ;
	//
	for ( i = 0; i < m_nFrequencyPoint[0]; i ++ )
	{
		m_ptrWeightTable[i] = (REAL32) rAvgRatio[0] ;
	}
	for ( j = 1; j < 7; j ++ )
	{
		double	a = rAvgRatio[j - 1] ;
		double	k = (rAvgRatio[j] - a)
					/ (m_nFrequencyPoint[j] - m_nFrequencyPoint[j - 1]) ;
		while ( i < m_nFrequencyPoint[j] )
		{
			m_ptrWeightTable[i ++] =
				(REAL32)(k * (i - m_nFrequencyPoint[j - 1]) + a) ;
		}
	}
	while ( i < nDegreeNum )
	{
		m_ptrWeightTable[i ++] = (REAL32) rAvgRatio[6] ;
	}
	//
	REAL32	rOddWeight =
		(REAL32)((((nWeightCode >> 30) & 0x03) + 0x02) / 2.0) ;
	for ( i = 15; i < nDegreeNum; i += 16 )
	{
		m_ptrWeightTable[i] *= rOddWeight ;
	}
	m_ptrWeightTable[nDegreeNum-1] = (REAL32) nCoefficient ;
	//
	for ( i = 0; i < nDegreeNum; i ++ )
	{
		m_ptrWeightTable[i] = 1.0F / m_ptrWeightTable[i] ;
	}
	//
	// �t�ʎq��
	//
	for ( i = 0; i < nDegreeNum; i ++ )
	{
		ptrDestination[i] =
			(REAL32) (rCoefficient * m_ptrWeightTable[i] * ptrQuantumized[i]) ;
	}
}

//
// 16�r�b�g�̔�t�W�J�i�~�h���T�C�h�X�e���I�j
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeSoundDCT_MSS
	( ERISADecodeContext & context,
		const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf )
{
	//
	// �o�b�t�@���m��
	//
	unsigned int	i, j, k ;
	unsigned int	nDegreeWidth = (1 << m_mioih.dwSubbandDegree) ;
	unsigned int	nSampleCount =
		(datahdr.dwSampleCount + nDegreeWidth - 1) & ~(nDegreeWidth - 1) ;
	unsigned int	nSubbandCount =
		(nSampleCount >> m_mioih.dwSubbandDegree) ;
	unsigned int	nChannelCount = m_mioih.dwChannelCount ;	// ��ɂQ
	unsigned int	nAllSampleCount = nSampleCount * nChannelCount ;
	unsigned int	nAllSubbandCount = nSubbandCount ;
	//
	if ( nSampleCount > m_nBufLength )
	{
		if ( m_ptrBuffer2 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer2 ) ;
		}
		if ( m_ptrBuffer3 != NULL )
		{
			::eslHeapFree( NULL, m_ptrBuffer3 ) ;
		}
		if ( m_ptrDivisionTable != NULL )
		{
			::eslHeapFree( NULL, m_ptrDivisionTable ) ;
		}
		if ( m_ptrRevolveCode != NULL )
		{
			::eslHeapFree( NULL, m_ptrRevolveCode ) ;
		}
		if ( m_ptrWeightCode != NULL )
		{
			::eslHeapFree( NULL, m_ptrWeightCode ) ;
		}
		if ( m_ptrCoefficient != NULL )
		{
			::eslHeapFree( NULL, m_ptrCoefficient ) ;
		}
		m_ptrBuffer2 =
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(INT), 0 ) ;
		m_ptrBuffer3 = (SBYTE*)
			::eslHeapAllocate( NULL, nAllSampleCount * sizeof(SWORD), 0 ) ;
		m_ptrDivisionTable = (PBYTE)
			::eslHeapAllocate( NULL, nAllSubbandCount * sizeof(BYTE), 0 ) ;
		m_ptrRevolveCode = (PBYTE)
			::eslHeapAllocate( NULL, nAllSubbandCount * 10 * sizeof(BYTE), 0 ) ;
		m_ptrWeightCode = (SDWORD*)
			::eslHeapAllocate( NULL, nAllSubbandCount * 10 * sizeof(SDWORD), 0 ) ;
		m_ptrCoefficient = (PINT)
			::eslHeapAllocate( NULL, nAllSubbandCount * 10 * sizeof(INT), 0 ) ;
		//
		m_nBufLength = nSampleCount ;
	}
	//
	// �ʎq���W���e�[�u���𕜍�
	//
	if ( context.GetABit( ) != 0 )
	{
		return	eslErrGeneral ;
	}
	//
	unsigned int	nLastDivision = -1 ;
	m_ptrNextDivision = m_ptrDivisionTable ;
	m_ptrNextRevCode = m_ptrRevolveCode ;
	m_ptrNextWeight = m_ptrWeightCode ;
	m_ptrNextCoefficient = m_ptrCoefficient ;
	//
	for ( i = 0; i < nSubbandCount; i ++ )
	{
		unsigned int	nDivisionCode = context.GetNBits( 2 ) ;
		*(m_ptrNextDivision ++) = (BYTE) nDivisionCode ;
		//
		bool	fLeadBlock = false ;
		if ( nDivisionCode != nLastDivision )
		{
			if ( i != 0 )
			{
				*(m_ptrNextRevCode ++) = context.GetNBits( 2 ) ;
				*(m_ptrNextWeight ++) = context.GetNBits( 32 ) ;
				*(m_ptrNextCoefficient ++) = context.GetNBits( 16 ) ;
			}
			fLeadBlock = true ;
			nLastDivision = nDivisionCode ;
		}
		//
		unsigned int	nDivisionCount = (1 << nDivisionCode) ;
		for ( k = 0; k < nDivisionCount; k ++ )
		{
			if ( fLeadBlock )
			{
				*(m_ptrNextRevCode ++) = context.GetNBits( 2 ) ;
				fLeadBlock = false ;
			}
			else
			{
				*(m_ptrNextRevCode ++) = context.GetNBits( 4 ) ;
			}
			*(m_ptrNextWeight ++) = context.GetNBits( 32 ) ;
			*(m_ptrNextCoefficient ++) = context.GetNBits( 16 ) ;
		}
	}
	if ( nSubbandCount > 0 )
	{
		*(m_ptrNextRevCode ++) = context.GetNBits( 2 ) ;
		*(m_ptrNextWeight ++) = context.GetNBits( 32 ) ;
		*(m_ptrNextCoefficient ++) = context.GetNBits( 16 ) ;
	}
	//
	if ( context.GetABit( ) != 0 )
	{
		return	eslErrGeneral ;
	}
	if ( datahdr.bytFlags & MIO_LEAD_BLOCK )
	{
		if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
		{
			context.PrepareToDecodeERINACode( ) ;
		}
		else
		{
			context.PrepareToDecodeERISACode( ) ;
		}
	}
	else if ( m_mioih.dwArchitecture == ERISA_NEMESIS_CODE )
	{
		context.InitializeERISACode( ) ;
	}
	//
	// �������ċt�C���^�[���u
	//
	if ( m_mioih.dwArchitecture != ERISA_NEMESIS_CODE )
	{
		if ( context.DecodeSymbolBytes
				( m_ptrBuffer3, nAllSampleCount * 2 ) < nAllSampleCount * 2 )
		{
			return	eslErrGeneral ;			// �G���[
		}
		//
		SBYTE *	ptrHBuf = m_ptrBuffer3 ;
		SBYTE *	ptrLBuf = m_ptrBuffer3 + nAllSampleCount ;
		//
		for ( i = 0; i < nDegreeWidth * 2; i ++ )
		{
			PINT	ptrQuantumized = ((PINT) m_ptrBuffer2) + i ;
			//
			for ( j = 0; j < nAllSubbandCount; j ++ )
			{
				INT		nLow = *(ptrLBuf ++) ;
				INT		nHigh = *(ptrHBuf ++) ^ (nLow >> 8) ;
				*ptrQuantumized = (nLow & 0xFF) | (nHigh << 8) ;
				ptrQuantumized += nDegreeWidth * 2 ;
			}
		}
	}
	else
	{
		if ( context.DecodeERISACodeWords
				( (SWORD*) m_ptrBuffer3, nAllSampleCount ) < nAllSampleCount )
		{
			return	eslErrGeneral ;			// �G���[
		}
		for ( i = 0; i < nAllSampleCount; i ++ )
		{
			((PINT)m_ptrBuffer2)[i] = ((SWORD*)m_ptrBuffer3)[i] ;
		}
	}
	//
	// �T�u�o���h�P�ʂŋt DCT �ϊ����{��
	//
	unsigned int	nSamples ;
	unsigned int	nRestSamples = datahdr.dwSampleCount ;
	SWORD *			ptrDstBuf = (SWORD*) ptrWaveBuf ;
	//
	nLastDivision = -1 ;
	m_ptrNextDivision = m_ptrDivisionTable ;
	m_ptrNextRevCode = m_ptrRevolveCode ;
	m_ptrNextWeight = m_ptrWeightCode ;
	m_ptrNextCoefficient = m_ptrCoefficient ;
	m_ptrNextSource = (PINT) m_ptrBuffer2 ;
	//
	for ( i = 0; i < nSubbandCount; i ++ )
	{
		//
		// �����R�[�h���擾
		//
		unsigned int	nDivisionCode = *(m_ptrNextDivision ++) ;
		unsigned int	nDivisionCount = (1 << nDivisionCode) ;
		//
		// �s��T�C�Y���ω�����ۂ̏���
		//
		bool	fLeadBlock = false ;
		//
		if ( nLastDivision != nDivisionCode )
		{
			//
			// ���O�܂ł̍s�������������
			//
			if ( i != 0 )
			{
				nSamples = nRestSamples ;
				if ( nSamples > m_nDegreeNum )
				{
					nSamples = m_nDegreeNum ;
				}
				if ( DecodePostBlock_MSS( ptrDstBuf, nSamples ) )
				{
					return	eslErrGeneral ;
				}
				nRestSamples -= nSamples ;
				ptrDstBuf += nSamples * nChannelCount ;
			}
			//
			// �s��T�C�Y��ω������邽�߂̃p�����[�^���Z�b�g�A�b�v
			//
			InitializeWithDegree
				( m_mioih.dwSubbandDegree - nDivisionCode ) ;
			nLastDivision = nDivisionCode ;
			fLeadBlock = true ;
		}
		//
		// �����t LOT �ϊ����{��
		//
		for ( k = 0; k < nDivisionCount; k ++ )
		{
			if ( fLeadBlock )
			{
				//
				// ���[�h�u���b�N�𕜍�����
				//
				if ( DecodeLeadBlock_MSS( ) )
				{
					return	eslErrGeneral ;
				}
				//
				fLeadBlock = false ;
			}
			else
			{
				//
				// �ʏ�u���b�N�𕜍�����
				//
				nSamples = nRestSamples ;
				if ( nSamples > m_nDegreeNum )
				{
					nSamples = m_nDegreeNum ;
				}
				if ( DecodeInternalBlock_MSS( ptrDstBuf, nSamples ) )
				{
					return	eslErrGeneral ;
				}
				nRestSamples -= nSamples ;
				ptrDstBuf += nSamples * nChannelCount ;
			}
		}
	}
	//
	// �s�������������
	//
	if ( nSubbandCount > 0 )
	{
		nSamples = nRestSamples ;
		if ( nSamples > m_nDegreeNum )
		{
			nSamples = m_nDegreeNum ;
		}
		if ( DecodePostBlock_MSS( ptrDstBuf, nSamples ) )
		{
			return	eslErrGeneral ;
		}
		nRestSamples -= nSamples ;
		ptrDstBuf += nSamples * nChannelCount ;
	}

	return	eslErrSuccess ;
}

//
// �ʏ�̃u���b�N�𕜍�����
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeInternalBlock_MSS
	( SWORD * ptrDst, unsigned int nSamples )
{
	unsigned int	i, j ;
	REAL32 *	ptrSrcBuf = m_ptrMatrixBuf ;
	REAL32 *	ptrLapBuf = m_ptrLastDCT ;
	//
	SDWORD	nWeightCode = *(m_ptrNextWeight ++) ;
	int		nCoefficient = *(m_ptrNextCoefficient ++) ;
	//
	for ( i = 0; i < 2; i ++ )
	{
		//
		// �t�ʎq��
		//
		IQuantumize
			( ptrSrcBuf, m_ptrNextSource,
				m_nDegreeNum, nWeightCode, nCoefficient ) ;
		m_ptrNextSource += m_nDegreeNum ;
		ptrSrcBuf += m_nDegreeNum ;
	}
	//
	// ��]�ϊ�
	//
	REAL32	rSin, rCos ;
	int		nRevCode = *(m_ptrNextRevCode ++) ;
	int		nRevCode1 = (nRevCode >> 2) & 0x03 ;
	int		nRevCode2 = (nRevCode & 0x03) ;
	//
	REAL32 *	ptrSrcBuf1 = m_ptrMatrixBuf ;
	REAL32 *	ptrSrcBuf2 = m_ptrMatrixBuf + m_nDegreeNum ;
	//
	rSin = (REAL32) ::sin( nRevCode1 * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( nRevCode1 * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( ptrSrcBuf1, ptrSrcBuf2, rSin, rCos, 2, m_nDegreeNum / 2 ) ;
	//
	rSin = (REAL32) ::sin( nRevCode2 * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( nRevCode2 * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( ptrSrcBuf1 + 1, ptrSrcBuf2 + 1, rSin, rCos, 2, m_nDegreeNum / 2 ) ;
	//
	ptrSrcBuf = m_ptrMatrixBuf ;
	//
	for ( i = 0; i < 2; i ++ )
	{
		//
		// �t LOT ���{��
		//
		::eriOddGivensInverseMatrix
			( ptrSrcBuf, m_pRevolveParam, m_nSubbandDegree ) ;
		::eriFastIPLOT
			( ptrSrcBuf, m_nSubbandDegree ) ;
		::eriFastILOT
			( m_ptrWorkBuf, ptrLapBuf, ptrSrcBuf, m_nSubbandDegree ) ;
		//
		for ( j = 0; j < m_nDegreeNum; j ++ )
		{
			ptrLapBuf[j] = ptrSrcBuf[j] ;
			ptrSrcBuf[j] = m_ptrWorkBuf[j] ;
		}
		//
		// �t DCT �ϊ����{��
		//
		::eriFastIDCT
			( m_ptrInternalBuf, ptrSrcBuf,
				1, m_ptrWorkBuf, m_nSubbandDegree ) ;
		//
		// �X�g�A
		//
		if ( nSamples != 0 )
		{
			::eriRoundR32ToWordArray
				( ptrDst + i, 2, m_ptrInternalBuf, nSamples ) ;
		}
		//
		ptrSrcBuf += m_nDegreeNum ;
		ptrLapBuf += m_nDegreeNum ;
	}
	//
	return	eslErrSuccess ;
}

//
// ���[�h�u���b�N�𕜍�����
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodeLeadBlock_MSS( void )
{
	unsigned int	i, j ;
	unsigned int	nHalfDegree = m_nDegreeNum / 2 ;
	SDWORD	nWeightCode = *(m_ptrNextWeight ++) ;
	int		nCoefficient = *(m_ptrNextCoefficient ++) ;
	REAL32 *	ptrLapBuf = m_ptrLastDCT ;
	//
	for ( i = 0; i < 2; i ++ )
	{
		//
		// �t�ʎq��
		//
		PINT	ptrSrcBuf = (PINT) m_ptrBuffer1 ;
		for ( j = 0; j < nHalfDegree; j ++ )
		{
			ptrSrcBuf[j * 2] = 0 ;
			ptrSrcBuf[j * 2 + 1] = *(m_ptrNextSource ++) ;
		}
		IQuantumize
			( ptrLapBuf, ptrSrcBuf,
				m_nDegreeNum, nWeightCode, nCoefficient ) ;
		//
		ptrLapBuf += m_nDegreeNum ;
	}
	//
	// ��]�ϊ�
	//
	REAL32	rSin, rCos ;
	int		nRevCode = *(m_ptrNextRevCode ++) ;
	//
	REAL32 *	ptrLapBuf1 = m_ptrLastDCT ;
	REAL32 *	ptrLapBuf2 = m_ptrLastDCT + m_nDegreeNum ;
	//
	rSin = (REAL32) ::sin( nRevCode * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( nRevCode * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( ptrLapBuf1, ptrLapBuf2, rSin, rCos, 1, m_nDegreeNum ) ;
	//
	ptrLapBuf = m_ptrLastDCT ;
	//
	for ( i = 0; i < 2; i ++ )
	{
		//
		// �d���p�����[�^��ݒ肷��
		//
		::eriOddGivensInverseMatrix
			( ptrLapBuf, m_pRevolveParam, m_nSubbandDegree ) ;
		//
		for ( j = 0; j < m_nDegreeNum; j += 2 )
		{
			ptrLapBuf[j] = ptrLapBuf[j + 1] ;
		}
		//
		::eriFastIPLOT( ptrLapBuf, m_nSubbandDegree ) ;
		//
		ptrLapBuf += m_nDegreeNum ;
	}
	//
	return	eslErrSuccess ;
}

//
// �|�X�g�u���b�N�𕜍�����
//////////////////////////////////////////////////////////////////////////////
ESLError MIODecoder::DecodePostBlock_MSS
	( SWORD * ptrDst, unsigned int nSamples )
{
	REAL32 *	ptrLapBuf = m_ptrLastDCT ;
	REAL32 *	ptrSrcBuf = m_ptrMatrixBuf ;
	//
	unsigned int	i, j ;
	unsigned int	nHalfDegree = m_nDegreeNum / 2 ;
	SDWORD	nWeightCode = *(m_ptrNextWeight ++) ;
	int		nCoefficient = *(m_ptrNextCoefficient ++) ;
	//
	for ( i = 0; i < 2; i ++ )
	{
		//
		// �t�ʎq��
		//
		PINT	ptrTempBuf = (PINT) m_ptrBuffer1 ;
		for ( j = 0; j < nHalfDegree; j ++ )
		{
			ptrTempBuf[j * 2] = 0 ;
			ptrTempBuf[j * 2 + 1] = *(m_ptrNextSource ++) ;
		}
		IQuantumize
			( ptrSrcBuf, ptrTempBuf,
				m_nDegreeNum, nWeightCode, nCoefficient ) ;
		//
		ptrSrcBuf += m_nDegreeNum ;
	}
	//
	// ��]�ϊ�
	//
	REAL32	rSin, rCos ;
	int		nRevCode = *(m_ptrNextRevCode ++) ;
	//
	REAL32 *	ptrSrcBuf1 = m_ptrMatrixBuf ;
	REAL32 *	ptrSrcBuf2 = m_ptrMatrixBuf + m_nDegreeNum ;
	//
	rSin = (REAL32) ::sin( nRevCode * ERI_PI / 8 ) ;
	rCos = (REAL32) ::cos( nRevCode * ERI_PI / 8 ) ;
	::eriRevolve2x2
		( ptrSrcBuf1, ptrSrcBuf2, rSin, rCos, 1, m_nDegreeNum ) ;
	//
	ptrSrcBuf = m_ptrMatrixBuf ;
	//
	for ( i = 0; i < 2; i ++ )
	{
		//
		// �t LOT ���{��
		//
		::eriOddGivensInverseMatrix
			( ptrSrcBuf, m_pRevolveParam, m_nSubbandDegree ) ;
		//
		for ( j = 0; j < m_nDegreeNum; j += 2 )
		{
			ptrSrcBuf[j] = - ptrSrcBuf[j + 1] ;
		}
		//
		::eriFastIPLOT
			( ptrSrcBuf, m_nSubbandDegree ) ;
		::eriFastILOT
			( m_ptrWorkBuf, ptrLapBuf, ptrSrcBuf, m_nSubbandDegree ) ;
		//
		for ( j = 0; j < m_nDegreeNum; j ++ )
		{
			ptrSrcBuf[j] = m_ptrWorkBuf[j] ;
		}
		//
		// �t DCT �ϊ����{��
		//
		::eriFastIDCT
			( m_ptrInternalBuf, ptrSrcBuf,
				1, m_ptrWorkBuf, m_nSubbandDegree ) ;
		//
		// �X�g�A
		//
		if ( nSamples != 0 )
		{
			::eriRoundR32ToWordArray
				( ptrDst + i, 2, m_ptrInternalBuf, nSamples ) ;
		}
		//
		ptrLapBuf += m_nDegreeNum ;
		ptrSrcBuf += m_nDegreeNum ;
	}
	//
	return	eslErrSuccess ;
}


#endif
