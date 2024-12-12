
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>


/*****************************************************************************
                            画像展開オブジェクト
 *****************************************************************************/

// オペレーション実行
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::PerformOperation
	( DWORD dwOpCode, LONG nAllBlockLines, SBYTE * pNextLineBuf )
{
	//
	// 再配列実行
	//
	INT		i, j, k ;
	UINT	nArrangeCode, nColorOperation, nDiffOperation ;
	nColorOperation = dwOpCode & 0x0F ;
	nArrangeCode = (dwOpCode >> 4) & 0x03 ;
	nDiffOperation = (dwOpCode >> 6) & 0x03 ;
	//
	if ( nArrangeCode == 0 )
	{
		::eslMoveMemory( m_ptrDecodeBuf, m_ptrArrangeBuf, m_nBlockSamples ) ;
		//
		if ( dwOpCode == 0 )
		{
			return ;
		}
	}
	else
	{
		PINT	pArrange = m_pArrangeTable[nArrangeCode] ;
		for ( i = 0; i < (INT) m_nBlockSamples; i ++ )
		{
			m_ptrDecodeBuf[pArrange[i]] = m_ptrArrangeBuf[i] ;
		}
	}
	//
	// カラーオペレーションを実行
	//
	(this->*m_pfnColorOperation[nColorOperation])( ) ;
	//
	// 差分処理を実行（水平方向）
	//
	SBYTE	* ptrNextBuf, * ptrNextColBuf, * ptrLineBuf ;
	if ( nDiffOperation & 0x01 )
	{
		ptrNextBuf = m_ptrDecodeBuf ;
		ptrNextColBuf = m_ptrColumnBuf ;
		for ( i = 0; i < nAllBlockLines; i ++ )
		{
			SBYTE	nLastVal = *ptrNextColBuf ;
			for ( j = 0; j < (INT) m_nBlockSize; j ++ )
			{
				nLastVal += *ptrNextBuf ;
				*(ptrNextBuf ++) = nLastVal ;
			}
			*(ptrNextColBuf ++) = nLastVal ;
		}
	}
	else
	{
		ptrNextBuf = m_ptrDecodeBuf ;
		ptrNextColBuf = m_ptrColumnBuf ;
		for ( i = 0; i < nAllBlockLines; i ++ )
		{
			*(ptrNextColBuf ++) = ptrNextBuf[m_nBlockSize - 1] ;
			ptrNextBuf += m_nBlockSize ;
		}
	}
	//
	// 差分処理を実行（垂直方向）
	//
	ptrLineBuf = pNextLineBuf ;
	ptrNextBuf = m_ptrDecodeBuf ;
	for ( k = 0; k < (INT) m_nChannelCount; k ++ )
	{
		SBYTE *	ptrLastLine = ptrLineBuf ;
		for ( i = 0; i < (INT) m_nBlockSize; i ++ )
		{
			SBYTE *	ptrCurrentLine = ptrNextBuf ;
			for ( j = 0; j < (INT) m_nBlockSize; j ++ )
			{
				*(ptrNextBuf ++) += *(ptrLastLine ++) ;
			}
			ptrLastLine = ptrCurrentLine ;
		}
		for ( j = 0; j < (INT) m_nBlockSize; j ++ )
		{
			*(ptrLineBuf ++) = *(ptrLastLine ++) ;
		}
	}
}

// カラーオペレーション関数群
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::ColorOperation0000( void )
{
}

void ERISADecoder::ColorOperation0101( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = *ptrNext ;
		ptrNext[nChSamples] += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation0110( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea * 2 ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = *ptrNext ;
		ptrNext[nChSamples] += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation0111( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = *ptrNext ;
		ptrNext[nChSamples] += nBase ;
		ptrNext[nChSamples * 2] += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation1001( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = ptrNext[nChSamples] ;
		*ptrNext += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation1010( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = ptrNext[nChSamples] ;
		ptrNext[nChSamples * 2] += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation1011( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = ptrNext[nChSamples] ;
		*ptrNext += nBase ;
		ptrNext[nChSamples * 2] += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation1101( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea * 2 ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = ptrNext[nChSamples] ;
		*ptrNext += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation1110( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = ptrNext[nChSamples * 2] ;
		ptrNext[nChSamples] += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

void ERISADecoder::ColorOperation1111( void )
{
	SBYTE	nBase ;
	SBYTE *	ptrNext = m_ptrDecodeBuf ;
	LONG	nChSamples = m_nBlockArea ;
	LONG	nRepCount = m_nBlockArea ;
	//
	do
	{
		nBase = ptrNext[nChSamples * 2] ;
		*ptrNext += nBase ;
		ptrNext[nChSamples] += nBase ;
		ptrNext ++ ;
	}
	while ( -- nRepCount ) ;
}

// 逆量子化
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::ArrangeAndIQuantumize
	( const SBYTE * ptrSrcData, const SBYTE * ptrCoefficient )
{
	//
	// 逆量子化係数を算出
	//
	int		i, j ;
	REAL32	rMatrixScale = (REAL32) (256.0 * 2.0 / (LONG) m_nBlockSize) ;
	REAL32 *	pIQParamPtr[2] ;
	for ( i = 0; i < 2; i ++ )
	{
		REAL32	rScale = 1.0F ;
		if ( ptrCoefficient[i] & 0x01 )
		{
			rScale = 1.5F ;
		}
		rScale *= (REAL32) pow( 2.0, (ptrCoefficient[i] / 2) ) ;
		rScale *= rMatrixScale ;
		//
		pIQParamPtr[i] = m_ptrIQParamBuf + i * m_nBlockArea ;
		BYTE *	pIQParamTable = m_ptrIQParamTable + i * m_nBlockArea ;
		for ( j = 0; j < (int) m_nBlockArea; j ++ )
		{
			pIQParamPtr[i][j] = (REAL32) (rScale * (pIQParamTable[j] + 1)) ;
		}
	}
	//
	// 直流成分差分処理
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI )
	{
		ptrSrcData[m_nBlockArea]   += ptrSrcData[0] ;
		ptrSrcData[m_nBlockArea*2] += ptrSrcData[0] ;
		ptrSrcData[m_nBlockArea*3] += ptrSrcData[0] ;
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
			ptrSrcData[k + m_nBlockArea]   += ptrSrcData[k] ;
			ptrSrcData[k + m_nBlockArea*2] += ptrSrcData[k] ;
			ptrSrcData[k + m_nBlockArea*3] += ptrSrcData[k] ;
			k += m_nBlockArea * 4 ;
		}
	}
	//
	// 逆量子化＆ジグザグ走査
	//
	REAL32 *	pIQParam[16] ;
	pIQParam[0] = pIQParam[1] = pIQParam[2] = pIQParam[3] = pIQParamPtr[0] ;
	if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
	{
		for ( i = 4; i < 12; i ++ )
		{
			pIQParam[i] = pIQParamPtr[1] ;
		}
		for ( i = 12; i < (int) m_nBlocksetCount; i ++ )
		{
			pIQParam[i] = pIQParamPtr[0] ;
		}
	}
	else
	{
		pIQParam[4] = pIQParam[5] = pIQParamPtr[1] ;
		for ( i = 6; i < (int) m_nBlocksetCount; i ++ )
		{
			pIQParam[i] = pIQParamPtr[0] ;
		}
	}
	PINT	pArrange = m_pArrangeTable[0] ;
	for ( i = 0; i < (int) m_nBlocksetCount; i ++ )
	{
		REAL32 *	ptrDst = m_ptrBlocksetBuf[i] ;
		REAL32 *	ptrSrc = m_ptrMatrixBuf ;
		::eriConvertArraySByteToR32
			( ptrSrc, ptrSrcData, m_nBlockArea ) ;
		ptrSrcData += m_nBlockArea ;
		::eriVectorMultiply( ptrSrc, pIQParam[i], m_nBlockArea ) ;
		for ( j = 0; j < (int) m_nBlockArea; j ++ )
		{
			ptrDst[pArrange[j]] = ptrSrc[j] ;
		}
	}
}

// 逆 DCT 変換
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::MatrixIDCT8x8( REAL32 * )
{
	for ( i = 0; i < m_nBlocksetCount; i ++ )
	{
		::eriFastIDCT8x8( m_ptrBlocksetBuf[i] ) ;
	}
}

// 逆 LOT 変換
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::MatrixILOT8x8( REAL32 * ptrVertBufLOT )
{
	//
	// 輝度チャネルを処理
	//
	int		i, j, k, l = 0 ;
	REAL32 *	ptrHorzBufLOT = m_ptrHorzBufLOT ;
	for ( i = 0; i < 2; i ++ )
	{
		for ( j = 0; j < 2; j ++ )
		{
			::eriFastILOT8x8
				( m_ptrBlocksetBuf[l],
					ptrHorzBufLOT, ptrVertBufLOT + j * m_nBlockArea ) ;
			l ++ ;
		}
		ptrHorzBufLOT += m_nBlockArea ;
	}
	ptrVertBufLOT += m_nBlockArea * 2 ;
	//
	// 色差チャネルを処理
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
					::eriFastILOT8x8
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
			::eriFastILOT8x8
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
	// αチャネルを処理
	//
	if ( m_nChannelCount < 4 )
	{
		return ;
	}
	for ( i = 0; i < 2; i ++ )
	{
		for ( j = 0; j < 2; j ++ )
		{
			::eriFastILOT8x8
				( m_ptrBlocksetBuf[l],
					ptrHorzBufLOT, ptrVertBufLOT + j * m_nBlockArea ) ;
			l ++ ;
		}
		ptrHorzBufLOT += m_nBlockArea ;
	}
	ptrVertBufLOT += m_nBlockArea * 2 ;
}

// 4:4:4 スケーリング
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::BlockScaling444( int x, int y, DWORD fdwFlags )
{
	int	nBlockOffset = (m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI) ;
	for ( int i = 0; i < 2; i ++ )
	{
		int	yPos = y * 2 + i - nBlockOffset ;
		if ( yPos < 0 )
		{
			continue ;
		}
		for ( int j = 0; j < 2; j ++ )
		{
			int	xPos = x * 2 + j - nBlockOffset ;
			if ( xPos < 0 )
			{
				continue ;
			}
			//
			// 輝度チャネルを出力
			//
			int		k = i * 2 + j ;
			if ( fdwFlags & dfDifferential )
			{
				::eriConvertArrayR32ToSByte
					( m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k], m_nBlockArea ) ;
			}
			else
			{
				::eriConvertArrayR32ToByte
					( (BYTE*) m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k], m_nBlockArea ) ;
			}
			StoreYUVImageChannel( xPos, yPos, 0 ) ;
			//
			if ( m_nChannelCount < 3 )
			{
				continue ;
			}
			//
			// 色差チャネルを出力
			//
			::eriConvertArrayR32ToSByte
				( m_ptrDecodeBuf,
					m_ptrBlocksetBuf[k + 4], m_nBlockArea ) ;
			StoreYUVImageChannel( xPos, yPos, 1 ) ;
			//
			::eriConvertArrayR32ToSByte
				( m_ptrDecodeBuf,
					m_ptrBlocksetBuf[k + 8], m_nBlockArea ) ;
			StoreYUVImageChannel( xPos, yPos, 2 ) ;
			//
			if ( m_nChannelCount < 4 )
			{
				continue ;
			}
			//
			// αチャネルを出力
			//
			if ( fdwFlags & dfDifferential )
			{
				::eriConvertArrayR32ToSByte
					( m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k + 12], m_nBlockArea ) ;
			}
			else
			{
				::eriConvertArrayR32ToByte
					( (BYTE*) m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k + 12], m_nBlockArea ) ;
			}
			StoreYUVImageChannel( xPos, yPos, 3 ) ;
		}
	}
}

// 4:1:1 スケーリング
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::BlockScaling411( int x, int y, DWORD fdwFlags )
{
	int	nBlockOffset = (m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI) ;
	for ( int i = 0; i < 2; i ++ )
	{
		int	yPos = y * 2 + i - nBlockOffset * 2 ;
		if ( yPos < 0 )
		{
			continue ;
		}
		for ( int j = 0; j < 2; j ++ )
		{
			int	xPos = x * 2 + j - nBlockOffset * 2 ;
			if ( xPos < 0 )
			{
				continue ;
			}
			//
			// 輝度チャネルを出力
			//
			int		k = i * 2 + j ;
			if ( fdwFlags & dfDifferential )
			{
				::eriConvertArrayR32ToSByte
					( m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k], m_nBlockArea ) ;
			}
			else
			{
				::eriConvertArrayR32ToByte
					( (BYTE*) m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k], m_nBlockArea ) ;
			}
			StoreYUVImageChannel( xPos, yPos, 0 ) ;
			//
			if ( m_nChannelCount < 4 )
			{
				continue ;
			}
			//
			// αチャネルを出力
			//
			if ( fdwFlags & dfDifferential )
			{
				::eriConvertArrayR32ToSByte
					( m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k + 6], m_nBlockArea ) ;
			}
			else
			{
				::eriConvertArrayR32ToByte
					( (BYTE*) m_ptrDecodeBuf,
						m_ptrBlocksetBuf[k + 6], m_nBlockArea ) ;
			}
			StoreYUVImageChannel( xPos, yPos, 3 ) ;
		}
	}
	//
	// 色差チャネルを出力
	//
	if ( m_nChannelCount < 3 )
	{
		return ;
	}
	y -= nBlockOffset ;
	x -= nBlockOffset ;
	if ( (y < 0) || (x < 0) )
	{
		return ;
	}
	::eriConvertArrayR32ToSByte
		( m_ptrDecodeBuf,
			m_ptrBlocksetBuf[4], m_nBlockArea ) ;
	StoreYUVImageChannelX2( x, y, 1 ) ;
	//
	::eriConvertArrayR32ToSByte
		( m_ptrDecodeBuf,
			m_ptrBlocksetBuf[5], m_nBlockArea ) ;
	StoreYUVImageChannelX2( x, y, 2 ) ;
}

// 中間バッファを YUV から RGB 形式へ変換
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::ConvertImageYUVtoRGB( DWORD fdwFlags )
{
	if ( m_nChannelCount < 3 )
	{
		return ;
	}
	//
	UINT	nPixelBytes = m_nYUVPixelBytes ;
	UINT	nWidth = m_nDstWidth ;
	UINT	nHeight = m_nDstHeight ;
	SBYTE *	ptrYUVLine = m_ptrYUVImage ;
	//
	for ( UINT y = 0; y < nHeight; y ++ )
	{
		SBYTE *	ptrYUVPixel = ptrYUVLine ;
		if ( fdwFlags & dfDifferential )
		{
			for ( UINT x = 0; x < nWidth; x ++ )
			{
				int	y = ptrYUVPixel[0] ;
				int	u = ptrYUVPixel[1] ;
				int	v = ptrYUVPixel[2] ;
				int	b = y + ((u * 7) >> 2) + 0x80 ;
				int	g = y - ((u * 3 + v * 6) >> 3) + 0x80 ;
				int	r = y + ((v * 3) >> 1) + 0x80 ;
				if ( (unsigned int) b > 0xFF )
				{
					b = (~b >> 31) & 0xFF ;
				}
				ptrYUVPixel[0] = (SBYTE) (b - 0x80) ;
				if ( (unsigned int) g > 0xFF )
				{
					g = (~g >> 31) & 0xFF ;
				}
				ptrYUVPixel[1] = (SBYTE) (g - 0x80) ;
				if ( (unsigned int) r > 0xFF )
				{
					r = (~r >> 31) & 0xFF ;
				}
				ptrYUVPixel[2] = (SBYTE) (r - 0x80) ;
				ptrYUVPixel += nPixelBytes ;
			}
		}
		else
		{
			for ( UINT x = 0; x < nWidth; x ++ )
			{
				int	y = *((BYTE*)ptrYUVPixel) ;
				int	u = ptrYUVPixel[1] ;
				int	v = ptrYUVPixel[2] ;
				int	b = y + ((u * 7) >> 2) ;
				int	g = y - ((u * 3 + v * 6) >> 3) ;
				int	r = y + ((v * 3) >> 1) ;
				if ( (unsigned int) b > 0xFF )
				{
					b = (~b >> 31) & 0xFF ;
				}
				ptrYUVPixel[0] = (SBYTE) b ;
				if ( (unsigned int) g > 0xFF )
				{
					g = (~g >> 31) & 0xFF ;
				}
				ptrYUVPixel[1] = (SBYTE) g ;
				if ( (unsigned int) r > 0xFF )
				{
					r = (~r >> 31) & 0xFF ;
				}
				ptrYUVPixel[2] = (SBYTE) r ;
				ptrYUVPixel += nPixelBytes ;
			}
		}
		ptrYUVLine += m_nYUVLineBytes ;
	}
}

// RGB 画像の出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LS_RestoreRGB24( void )
{
	ConvertImageYUVtoRGB( 0 ) ;
	//
	LONG	nSrcLineBytes = m_nYUVLineBytes ;
	UINT	nSrcPixelBytes = m_nYUVPixelBytes ;
	SBYTE *	ptrSrcImage = m_ptrYUVImage ;
	LONG	nDstLineBytes = m_nDstLineBytes ;
	UINT	nDstPixelBytes = m_nDstPixelBytes ;
	BYTE *	ptrDstImage = m_ptrDstBlock ;
	UINT	nWidth = m_nDstWidth ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		SBYTE *	ptrSrcLine = ptrSrcImage ;
		BYTE *	ptrDstLine = ptrDstImage ;
		for ( UINT x = 0; x < nWidth; x ++ )
		{
			ptrDstLine[0] = (BYTE) ptrSrcLine[0] ;
			ptrDstLine[1] = (BYTE) ptrSrcLine[1] ;
			ptrDstLine[2] = (BYTE) ptrSrcLine[2] ;
			ptrSrcLine += nSrcPixelBytes ;
			ptrDstLine += nDstPixelBytes ;
		}
		ptrSrcImage += nSrcLineBytes ;
		ptrDstImage += nDstLineBytes ;
	}
}

// RGBA 画像の出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LS_RestoreRGBA32( void )
{
	ConvertImageYUVtoRGB( 0 ) ;
	//
	LONG	nSrcLineBytes = m_nYUVLineBytes ;
	SBYTE *	ptrSrcImage = m_ptrYUVImage ;
	LONG	nDstLineBytes = m_nDstLineBytes ;
	BYTE *	ptrDstImage = m_ptrDstBlock ;
	UINT	nWidth = m_nDstWidth ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		DWORD *	pdwSrcLine = (DWORD*) ptrSrcImage ;
		DWORD *	pdwDstLine = (DWORD*) ptrDstImage ;
		for ( UINT x = 0; x < nWidth; x ++ )
		{
			pdwDstLine[x] = pdwSrcLine[x] ;
		}
		ptrSrcImage += nSrcLineBytes ;
		ptrDstImage += nDstLineBytes ;
	}
}

// RGB 画像の差分出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LS_RestoreDeltaRGB24( void )
{
	MoveImageWithVector( ) ;
	ConvertImageYUVtoRGB( dfDifferential ) ;
	//
	LONG	nSrcLineBytes = m_nYUVLineBytes ;
	UINT	nSrcPixelBytes = m_nYUVPixelBytes ;
	SBYTE *	ptrSrcImage = m_ptrYUVImage ;
	LONG	nDstLineBytes = m_nDstLineBytes ;
	UINT	nDstPixelBytes = m_nDstPixelBytes ;
	BYTE *	ptrDstImage = m_ptrDstBlock ;
	UINT	nWidth = m_nDstWidth ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		SBYTE *	ptrSrcLine = ptrSrcImage ;
		BYTE *	ptrDstLine = ptrDstImage ;
		for ( UINT x = 0; x < nWidth; x ++ )
		{
			register int	b =
				(int) ptrDstLine[0] + (((int) ptrSrcLine[0]) << 1) ;
			register int	g =
				(int) ptrDstLine[1] + (((int) ptrSrcLine[1]) << 1) ;
			register int	r =
				(int) ptrDstLine[2] + (((int) ptrSrcLine[2]) << 1) ;
			//
			if ( (unsigned int) b > 0xFF )
			{
				b = (~b >> 31) & 0xFF ;
			}
			if ( (unsigned int) g > 0xFF )
			{
				g = (~g >> 31) & 0xFF ;
			}
			if ( (unsigned int) r > 0xFF )
			{
				r = (~r >> 31) & 0xFF ;
			}
			ptrDstLine[0] = (BYTE) b ;
			ptrDstLine[1] = (BYTE) g ;
			ptrDstLine[2] = (BYTE) r ;
			ptrSrcLine += nSrcPixelBytes ;
			ptrDstLine += nDstPixelBytes ;
		}
		ptrSrcImage += nSrcLineBytes ;
		ptrDstImage += nDstLineBytes ;
	}
}

// RGBA 画像の差分出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LS_RestoreDeltaRGBA32( void )
{
	MoveImageWithVector( ) ;
	ConvertImageYUVtoRGB( dfDifferential ) ;
	//
	LONG	nSrcLineBytes = m_nYUVLineBytes ;
	SBYTE *	ptrSrcImage = m_ptrYUVImage ;
	LONG	nDstLineBytes = m_nDstLineBytes ;
	BYTE *	ptrDstImage = m_ptrDstBlock ;
	UINT	nWidth = m_nDstWidth ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		SBYTE *	ptrSrcLine = ptrSrcImage ;
		BYTE *	ptrDstLine = ptrDstImage ;
		for ( UINT x = 0; x < nWidth; x ++ )
		{
			register int	b =
				(int) ptrDstLine[0] + (((int) ptrSrcLine[0]) << 1) ;
			register int	g =
				(int) ptrDstLine[1] + (((int) ptrSrcLine[1]) << 1) ;
			register int	r =
				(int) ptrDstLine[2] + (((int) ptrSrcLine[2]) << 1) ;
			register int	a =
				(int) ptrDstLine[3] + (((int) ptrSrcLine[3]) << 1) ;
			//
			if ( (unsigned int) b > 0xFF )
			{
				b = (~b >> 31) & 0xFF ;
			}
			if ( (unsigned int) g > 0xFF )
			{
				g = (~g >> 31) & 0xFF ;
			}
			if ( (unsigned int) r > 0xFF )
			{
				r = (~r >> 31) & 0xFF ;
			}
			if ( (unsigned int) a > 0xFF )
			{
				a = (~a >> 31) & 0xFF ;
			}
			ptrDstLine[0] = (BYTE) b ;
			ptrDstLine[1] = (BYTE) g ;
			ptrDstLine[2] = (BYTE) r ;
			ptrDstLine[3] = (BYTE) a ;
			ptrSrcLine += 4 ;
			ptrDstLine += 4 ;
		}
		ptrSrcImage += nSrcLineBytes ;
		ptrDstImage += nDstLineBytes ;
	}
}
