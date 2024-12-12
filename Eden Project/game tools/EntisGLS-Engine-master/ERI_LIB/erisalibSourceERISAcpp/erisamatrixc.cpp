
/*****************************************************************************
                         E R I S A - L i b r a r y
 ----------------------------------------------------------------------------
         Copyright (C) 2000-2004 Leshade Entis. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <math.h>
#include <xerisa.h>


//
// 定数テーブル
//////////////////////////////////////////////////////////////////////////////

static const double	ERI_PI = 3.141592653589 ;	// = π
static const REAL32	ERI_rHalf = 0.5F ;			// = 1/2
static const REAL32	ERI_r2 = 2.0F ;				// = 2.0
static REAL32		ERI_rCosPI4 ;				// = cos(pi/4)
static REAL32		ERI_r2CosPI4 ;				// = 2*cos(pi/4)

//
// 行列係数配列 : k(n,i) = cos( (2*i+1) / (4*n) )
//
static REAL32	ERI_DCTofK2[2] ;			// = cos( (2*i+1) / 8 )
static REAL32	ERI_DCTofK4[4] ;			// = cos( (2*i+1) / 16 )
static REAL32	ERI_DCTofK8[8] ;			// = cos( (2*i+1) / 32 )
static REAL32	ERI_DCTofK16[16] ;			// = cos( (2*i+1) / 64 )
static REAL32	ERI_DCTofK32[32] ;			// = cos( (2*i+1) / 128 )
static REAL32	ERI_DCTofK64[64] ;			// = cos( (2*i+1) / 256 )
static REAL32	ERI_DCTofK128[128] ;		// = cos( (2*i+1) / 512 )
static REAL32	ERI_DCTofK256[256] ;		// = cos( (2*i+1) / 1024 )
static REAL32	ERI_DCTofK512[512] ;		// = cos( (2*i+1) / 2048 )
static REAL32	ERI_DCTofK1024[1024] ;		// = cos( (2*i+1) / 4096 )
static REAL32	ERI_DCTofK2048[2048] ;		// = cos( (2*i+1) / 8192 )

//
// 行列係数配列へのテーブル
//
static REAL32 *	ERI_pMatrixDCTofK[MAX_DCT_DEGREE] =
{
	NULL,
	ERI_DCTofK2,
	ERI_DCTofK4,
	ERI_DCTofK8,
	ERI_DCTofK16,
	ERI_DCTofK32,
	ERI_DCTofK64,
	ERI_DCTofK128,
	ERI_DCTofK256,
	ERI_DCTofK512,
	ERI_DCTofK1024,
	ERI_DCTofK2048
} ;


//
// 行列テーブルの初期化
//////////////////////////////////////////////////////////////////////////////
void eriInitializeMatrix( void )
{
	//
	// 特殊条件の定数を準備
	//
	ERI_rCosPI4 = (REAL32) cos( ERI_PI * 0.25 ) ;
	ERI_r2CosPI4 = 2.0F * ERI_rCosPI4 ;
	//
	// 行列係数配列初期化
	//
	for ( int i = 1; i < MAX_DCT_DEGREE; i ++ )
	{
		int			n = (1 << i) ;
		REAL32 *	pDCTofK = ERI_pMatrixDCTofK[i] ;
		double		nr = ERI_PI / (4.0 * n) ;
		double		dr = nr + nr ;
		double		ir = nr ;
		//
		for ( int j = 0; j < n; j ++ )
		{
			pDCTofK[j] = (REAL32) cos( ir ) ;
			ir += dr ;
		}
	}
}


//
// 32 ビット浮動小数点を 32 ビット整数に変換
//////////////////////////////////////////////////////////////////////////////
int eriRoundR32ToInt( REAL32 r )
{
#if	defined(ERI_INTEL_X86)
	int		n ;
	__asm
	{
		fld		r
		fistp	n
	}
	return	n ;
#else
	if ( r >= 0.0 )
	{
		return	(int) floor( r + 0.5 ) ;
	}
	else
	{
		return	(int) ceil( r - 0.5 ) ;
	}
#endif
}

//
// 64 ビット浮動小数点を 64 ビット整数に変換
//////////////////////////////////////////////////////////////////////////////
INT64 eriRoundR64ToLInt( REAL64 r )
{
#if	defined(ERI_INTEL_X86)
	INT64	n ;
	__asm
	{
		fld		r
		fistp	n
	}
	return	n ;
#else
	if ( r >= 0.0 )
	{
		return	(INT64) floor( r + 0.5 ) ;
	}
	else
	{
		return	(INT64) ceil( r - 0.5 ) ;
	}
#endif
}

//
// 32 ビット浮動小数点配列を 16 ビット整数配列に変換
//////////////////////////////////////////////////////////////////////////////
void eriRoundR32ToWordArray
	( SWORD * ptrDst, int nStep, const REAL32 * ptrSrc, int nCount )
{
	for ( int i = 0; i < nCount; i ++ )
	{
		int		nValue = ::eriRoundR32ToInt( ptrSrc[i] ) ;
		if ( nValue <= -0x8000 )
		{
			*ptrDst = -0x8000 ;
		}
		else if ( nValue >= 0x7FFF )
		{
			*ptrDst = 0x7FFF ;
		}
		else
		{
			*ptrDst = (SWORD) nValue ;
		}
		ptrDst += nStep ;
	}
}

//
// 8 ビット符号無し整数配列を 32 ビット浮動小数点配列に変換
//////////////////////////////////////////////////////////////////////////////
void eriConvertArrayByteToR32
	( REAL32 * ptrDst, const BYTE * ptrSrc, int nCount )
{
	for ( int i = 0; i < nCount; i ++ )
	{
		ptrDst[i] = ptrSrc[i] ;
	}
}

//
// 8 ビット符号有り整数配列を 32 ビット浮動小数点配列に変換
//////////////////////////////////////////////////////////////////////////////
void eriConvertArraySByteToR32
	( REAL32 * ptrDst, const SBYTE * ptrSrc, int nCount )
{
	for ( int i = 0; i < nCount; i ++ )
	{
		ptrDst[i] = ptrSrc[i] ;
	}
}

//
// 32 ビット浮動小数点配列を 8 ビット符号なし整数配列に変換
//////////////////////////////////////////////////////////////////////////////
void eriConvertArrayR32ToByte
	( BYTE * ptrDst, const REAL32 * ptrSrc, int nCount )
{
#if	defined(ERI_INTEL_X86)
	int		n ;
	__asm
	{
		mov		ecx, nCount
		mov		esi, ptrSrc
		mov		edi, ptrDst
		test	ecx, ecx
		jz		LoopEnd
LoopBegin:
			fld		DWORD PTR [esi]
			fistp	n
			mov		eax, n
			add		esi, 4
			cmp		eax, 0FFH
			jbe		Label1
				not		eax
				sar		eax, 31
				and		eax, 0FFH
Label1:		mov		BYTE PTR [edi], al
			inc		edi
		dec		ecx
		jnz		LoopBegin
LoopEnd:
	}
#else
	for ( int i = 0; i < nCount; i ++ )
	{
		int	n = ::eriRoundR32ToInt( ptrSrc[i] ) ;
		if ( (unsigned int) n > 0xFF )
		{
			n = (~n >> 31) & 0xFF ;
		}
		ptrDst[i] = (SBYTE) n ;
	}
#endif
}

//
// 32 ビット浮動小数点配列を 8 ビット符号あり整数配列に変換
//////////////////////////////////////////////////////////////////////////////
void eriConvertArrayR32ToSByte
	( SBYTE * ptrDst, const REAL32 * ptrSrc, int nCount )
{
#if	defined(ERI_INTEL_X86)
	int		n ;
	__asm
	{
		mov		ecx, nCount
		mov		esi, ptrSrc
		mov		edi, ptrDst
		test	ecx, ecx
		jz		LoopEnd
LoopBegin:
			fld		DWORD PTR [esi]
			fistp	n
			mov		eax, n
			add		eax, 80H
			add		esi, 4
			cmp		eax, 0FFH
			jbe		Label1
				not		eax
				sar		eax, 31
				and		eax, 0FFH
Label1:		sub		eax, 80H
			mov		BYTE PTR [edi], al
			inc		edi
		dec		ecx
		jnz		LoopBegin
LoopEnd:
	}
#else
	for ( int i = 0; i < nCount; i ++ )
	{
		int	n = ::eriRoundR32ToInt( ptrSrc[i] ) ;
		if ( (unsigned int) n > 0x7F )
		{
			if ( n < -0x80 )
				n = -0x80 ;
			else
				n = 0x7F ;
		}
		ptrDst[i] = (SBYTE) n ;
	}
#endif
}

//
// スカラ乗算
//////////////////////////////////////////////////////////////////////////////
void eriScalarMultiply
		( REAL32 * ptrDst, REAL32 rScalar, unsigned int nCount )
{
	for ( unsigned int i = 0; i < nCount; i ++ )
	{
		ptrDst[i] *= rScalar ;
	}
}

//
// ベクトル乗算
//////////////////////////////////////////////////////////////////////////////
void eriVectorMultiply
	( REAL32 * ptrDst, const REAL32 * ptrSrc, unsigned int nCount )
{
	for ( unsigned int i = 0; i < nCount; i ++ )
	{
		ptrDst[i] *= ptrSrc[i] ;
	}
}

//
// 高速 DCT 変換
//////////////////////////////////////////////////////////////////////////////
void eriFastDCT
	(
		REAL32 *		ptrDst,
		unsigned int	nDstInterval,
		REAL32 *		ptrSrc,
		REAL32 *		ptrWorkBuf,
		unsigned int	nDegreeDCT
	)
{
	//
	// DCT 次数検証
	//
	ESLAssert( (nDegreeDCT >= MIN_DCT_DEGREE)
				&& (nDegreeDCT <= MAX_DCT_DEGREE) ) ;
	//
	if ( nDegreeDCT == MIN_DCT_DEGREE )
	{
		//
		// 4次 DCT の時は特殊条件
		//////////////////////////////////////////////////////////////////////
		REAL32	r32Buf[4] ;
		//
		// 交差加減算
		//
		r32Buf[0] = ptrSrc[0] + ptrSrc[3] ;
		r32Buf[2] = ptrSrc[0] - ptrSrc[3] ;
		r32Buf[1] = ptrSrc[1] + ptrSrc[2] ;
		r32Buf[3] = ptrSrc[1] - ptrSrc[2] ;
		//
		// 前半 : A2 * DCT2
		//
		ptrDst[0]                = ERI_rHalf * (r32Buf[0] + r32Buf[1]) ;
		ptrDst[nDstInterval * 2] = ERI_rCosPI4 * (r32Buf[0] - r32Buf[1]) ;
		//
		// 後半 : R2 * 2 * A2 * DCT2 * K2
		//
		r32Buf[2] = ERI_DCTofK2[0] * r32Buf[2] ;
		r32Buf[3] = ERI_DCTofK2[1] * r32Buf[3] ;
		//
		r32Buf[0] =                 r32Buf[2] + r32Buf[3] ;
		r32Buf[1] = ERI_r2CosPI4 * (r32Buf[2] - r32Buf[3]) ;
		//
		r32Buf[1] -= r32Buf[0] ;
		//
		ptrDst[nDstInterval]     = r32Buf[0] ;
		ptrDst[nDstInterval * 3] = r32Buf[1] ;
	}
	else
	{
		//
		// 汎用 DCT 変換
		//////////////////////////////////////////////////////////////////////
		//              | I   J |
		// 交差加減算 = |       |
		//              | I  -J |
		unsigned int	i ;
		unsigned int	nDegreeNum = (1 << nDegreeDCT) ;
		unsigned int	nHalfDegree = (nDegreeNum >> 1) ;
		for ( i = 0; i < nHalfDegree; i ++ )
		{
			ptrWorkBuf[i] = ptrSrc[i] + ptrSrc[nDegreeNum - i - 1] ;
			ptrWorkBuf[i + nHalfDegree] =
							ptrSrc[i] - ptrSrc[nDegreeNum - i - 1] ;
		}
		//
		// 前半 DCT : A * DCT
		//
		unsigned int	nDstStep = (nDstInterval << 1) ;
		eriFastDCT( ptrDst, nDstStep,
					ptrWorkBuf, ptrSrc, (nDegreeDCT - 1) ) ;
		//
		// 後半 DCT-IV : R * 2 * A * DCT * K
		//
		REAL32 *	pDCTofK = ERI_pMatrixDCTofK[nDegreeDCT - 1] ;
		ptrSrc = ptrWorkBuf + nHalfDegree ;
		ptrDst += nDstInterval ;
		//
		for ( i = 0; i < nHalfDegree; i ++ )
		{
			ptrSrc[i] *= pDCTofK[i] ;
		}
		//
		eriFastDCT( ptrDst, nDstStep,
					ptrSrc, ptrWorkBuf, (nDegreeDCT - 1) ) ;
		//
		REAL32 *	ptrNext = ptrDst ;
		for ( i = 0; i < nHalfDegree; i ++ )
		{
			*ptrNext += *ptrNext ;
			ptrNext += nDstStep ;
		}
		//
		ptrNext = ptrDst ;
		for ( i = 1; i < nHalfDegree; i ++ )
		{
			ptrNext[nDstStep] -= *ptrNext ;
			ptrNext += nDstStep ;
		}
	}
}

//
// 高速 IDCT 変換
//////////////////////////////////////////////////////////////////////////////
void eriFastIDCT
	(
		REAL32 *		ptrDst,
		REAL32 *		ptrSrc,
		unsigned int	nSrcInterval,
		REAL32 *		ptrWorkBuf,
		unsigned int	nDegreeDCT
	)
{
	//
	// DCT 次数検証
	//
	ESLAssert( (nDegreeDCT >= MIN_DCT_DEGREE)
				&& (nDegreeDCT <= MAX_DCT_DEGREE) ) ;
	//
	if ( nDegreeDCT == MIN_DCT_DEGREE )
	{
		//
		// 4次 DCT の時は特殊条件
		//////////////////////////////////////////////////////////////////////
		REAL32	r32Buf1[2] ;
		REAL32	r32Buf2[4] ;
		//
		// 偶数行 : IDCT2
		//
		r32Buf1[0] = ptrSrc[0] ;
		r32Buf1[1] = ERI_rCosPI4 * ptrSrc[nSrcInterval * 2] ;
		//
		r32Buf2[0] = r32Buf1[0] + r32Buf1[1] ;
		r32Buf2[1] = r32Buf1[0] - r32Buf1[1] ;
		//
		// 奇数行 : R * 2 * A * DCT * K
		//
		r32Buf1[0] = ERI_DCTofK2[0] * ptrSrc[nSrcInterval] ;
		r32Buf1[1] = ERI_DCTofK2[1] * ptrSrc[nSrcInterval * 3] ;
		//
		r32Buf2[2] =                 r32Buf1[0] + r32Buf1[1] ;
		r32Buf2[3] = ERI_r2CosPI4 * (r32Buf1[0] - r32Buf1[1]) ;
		//
		r32Buf2[3] -= r32Buf2[2] ;
		//
		// 交差加減算
		//
		ptrDst[0] = r32Buf2[0] + r32Buf2[2] ;
		ptrDst[3] = r32Buf2[0] - r32Buf2[2] ;
		ptrDst[1] = r32Buf2[1] + r32Buf2[3] ;
		ptrDst[2] = r32Buf2[1] - r32Buf2[3] ;
	}
	else
	{
		//
		// 汎用 IDCT 変換
		//////////////////////////////////////////////////////////////////////
		//
		// 偶数行 : IDCT
		//
		unsigned int	i ;
		unsigned int	nDegreeNum = (1 << nDegreeDCT) ;
		unsigned int	nHalfDegree = (nDegreeNum >> 1) ;
		unsigned int	nSrcStep = (nSrcInterval << 1) ;
		eriFastIDCT( ptrDst, ptrSrc,
					nSrcStep, ptrWorkBuf, (nDegreeDCT - 1) ) ;
		//
		// 奇数行 : R * 2 * A * DCT * K
		//
		REAL32 *	pDCTofK = ERI_pMatrixDCTofK[nDegreeDCT - 1] ;
		REAL32 *	pOddSrc = ptrSrc + nSrcInterval ;
		REAL32 *	pOddDst = ptrDst + nHalfDegree ;
		//
		REAL32 *	ptrNext = pOddSrc ;
		for ( i = 0; i < nHalfDegree; i ++ )
		{
			ptrWorkBuf[i] = *ptrNext * pDCTofK[i] ;
			ptrNext += nSrcStep ;
		}
		//
		eriFastDCT( pOddDst, 1, ptrWorkBuf,
					(ptrWorkBuf + nHalfDegree), (nDegreeDCT - 1) ) ;
		//
		for ( i = 0; i < nHalfDegree; i ++ )
		{
			pOddDst[i] += pOddDst[i] ;
		}
		//
		for ( i = 1; i < nHalfDegree; i ++ )
		{
			pOddDst[i] -= pOddDst[i - 1] ;
		}
		//              | I   I |
		// 交差加減算 = |       |
		//              | J  -J |
		REAL32			r32Buf[4] ;
		unsigned int	nQuadDegree = (nHalfDegree >> 1) ;
		for ( i = 0; i < nQuadDegree; i ++ )
		{
			r32Buf[0] = ptrDst[i] + ptrDst[nHalfDegree + i] ;
			r32Buf[3] = ptrDst[i] - ptrDst[nHalfDegree + i] ;
			r32Buf[1] =
				ptrDst[nHalfDegree - 1 - i] + ptrDst[nDegreeNum - 1 - i] ;
			r32Buf[2] =
				ptrDst[nHalfDegree - 1 - i] - ptrDst[nDegreeNum - 1 - i] ;
			//
			ptrDst[i]                   = r32Buf[0] ;
			ptrDst[nHalfDegree - 1 - i] = r32Buf[1] ;
			ptrDst[nHalfDegree + i]     = r32Buf[2] ;
			ptrDst[nDegreeNum - 1 - i]  = r32Buf[3] ;
		}
	}
}

//
// 画像データ複製
//////////////////////////////////////////////////////////////////////////////
ESLError eriCopyImage
	( const EGL_IMAGE_INFO & eiiDst,
		const EGL_IMAGE_INFO & eiiSrc )
{
	if ( (eiiDst.dwImageWidth != eiiSrc.dwImageWidth)
		|| (eiiDst.dwImageHeight != eiiSrc.dwImageHeight) )
	{
		return	eslErrGeneral ;
	}
	if ( ((eiiDst.dwBitsPerPixel != 24)
			&& (eiiDst.dwBitsPerPixel != 32))
		|| ((eiiSrc.dwBitsPerPixel != 24)
			&& (eiiSrc.dwBitsPerPixel != 32)))
	{
		return	eslErrGeneral ;
	}
	//
	int		nDstPixelBytes = eiiDst.dwBitsPerPixel >> 3 ;
	int		nSrcPixelBytes = eiiSrc.dwBitsPerPixel >> 3 ;
	int		nDstLineBytes = eiiDst.dwBytesPerLine ;
	int		nSrcLineBytes = eiiSrc.dwBytesPerLine ;
	BYTE *	pbytDstLine = (BYTE*) eiiDst.ptrImageArray ;
	BYTE *	pbytSrcLine = (BYTE*) eiiSrc.ptrImageArray ;
	DWORD	dwWidth = eiiDst.dwImageWidth ;
	DWORD	dwHeight = eiiDst.dwImageHeight ;
	//
	if ( pbytSrcLine != NULL )
	{
		for ( DWORD y = 0; y < dwHeight; y ++ )
		{
			if ( (nDstPixelBytes == 4) && (nSrcPixelBytes == 4) )
			{
				DWORD *	pbytDst = (DWORD*) pbytDstLine ;
				DWORD *	pbytSrc = (DWORD*) pbytSrcLine ;
				for ( DWORD x = 0; x < dwWidth; x ++ )
				{
					pbytDst[x] = pbytSrc[x] ;
				}
			}
			else
			{
				BYTE *	pbytDst = pbytDstLine ;
				BYTE *	pbytSrc = pbytSrcLine ;
				for ( DWORD x = 0; x < dwWidth; x ++ )
				{
					pbytDst[0] = pbytSrc[0] ;
					pbytDst[1] = pbytSrc[1] ;
					pbytDst[2] = pbytSrc[2] ;
					pbytSrc += nSrcPixelBytes ;
					pbytDst += nDstPixelBytes ;
				}
			}
			pbytDstLine += nDstLineBytes ;
			pbytSrcLine += nSrcLineBytes ;
		}
	}
	else
	{
		DWORD	dwDstLineBytes = dwWidth * nDstPixelBytes ;
		for ( DWORD y = 0; y < dwHeight; y ++ )
		{
			::eslFillMemory( pbytDstLine, 0, dwDstLineBytes ) ;
			pbytDstLine += nDstLineBytes ;
		}
	}
	//
	return	eslErrSuccess ;
}

//
// 2つの画像を 1:1 で合成した画像を計算
//////////////////////////////////////////////////////////////////////////////
ESLError eriBlendHalfImage
	( const EGL_IMAGE_INFO & eiiDst,
		const EGL_IMAGE_INFO & eiiSrc1, const EGL_IMAGE_INFO & eiiSrc2 )
{
	if ( (eiiDst.dwImageWidth != eiiSrc1.dwImageWidth)
		|| (eiiDst.dwImageHeight != eiiSrc1.dwImageHeight)
		|| (eiiDst.dwImageWidth != eiiSrc2.dwImageWidth)
		|| (eiiDst.dwImageHeight != eiiSrc2.dwImageHeight)
	{
		return	eslErrGeneral ;
	}
	if ( ((eiiDst.dwBitsPerPixel != 24)
			&& (eiiDst.dwBitsPerPixel != 32))
		|| ((eiiSrc1.dwBitsPerPixel != 24)
			&& (eiiSrc1.dwBitsPerPixel != 32))
		|| ((eiiSrc2.dwBitsPerPixel != 24)
			&& (eiiSrc2.dwBitsPerPixel != 32)) )
	{
		return	eslErrGeneral ;
	}
	//
	int		nDstPixelBytes = eiiDst.dwBitsPerPixel >> 3 ;
	int		nSrc1PixelBytes = eiiSrc1.dwBitsPerPixel >> 3 ;
	int		nSrc1PixelBytes = eiiSrc2.dwBitsPerPixel >> 3 ;
	int		nDstLineBytes = eiiDst.dwBytesPerLine ;
	int		nSrc1LineBytes = eiiSrc1.dwBytesPerLine ;
	int		nSrc2LineBytes = eiiSrc2.dwBytesPerLine ;
	BYTE *	pbytDstLine = (BYTE*) eiiDst.ptrImageArray ;
	BYTE *	pbytSrc1Line = (BYTE*) eiiSrc1.ptrImageArray ;
	BYTE *	pbytSrc2Line = (BYTE*) eiiSrc2.ptrImageArray ;
	DWORD	dwWidth = eiiDst.dwImageWidth ;
	DWORD	dwHeight = eiiDst.dwImageHeight ;
	//
	for ( DWORD y = 0; y < dwHeight; y ++ )
	{
		if ( (nDstPixelBytes == 4)
			&& (nSrc1PixelBytes == 4) && (nSrc2PixelBytes == 4) )
		{
			BYTE *	pbytDst = pbytDstLine ;
			BYTE *	pbytSrc1 = pbytSrc1Line ;
			BYTE *	pbytSrc2 = pbytSrc2Line ;
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				pbytDst[0] = ((int) pbytSrc1[0] + (int) pbytSrc2[0]) >> 1 ;
				pbytDst[1] = ((int) pbytSrc1[1] + (int) pbytSrc2[1]) >> 1 ;
				pbytDst[2] = ((int) pbytSrc1[2] + (int) pbytSrc2[2]) >> 1 ;
				pbytDst[3] = ((int) pbytSrc1[3] + (int) pbytSrc2[3]) >> 1 ;
				pbytSrc1 += 4 ;
				pbytSrc2 += 4 ;
				pbytDst += 4 ;
			}
		}
		else
		{
			BYTE *	pbytDst = pbytDstLine ;
			BYTE *	pbytSrc1 = pbytSrc1Line ;
			BYTE *	pbytSrc2 = pbytSrc2Line ;
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				pbytDst[0] = ((int) pbytSrc1[0] + (int) pbytSrc2[0]) >> 1 ;
				pbytDst[1] = ((int) pbytSrc1[1] + (int) pbytSrc2[1]) >> 1 ;
				pbytDst[2] = ((int) pbytSrc1[2] + (int) pbytSrc2[2]) >> 1 ;
				pbytSrc1 += nSrc1PixelBytes ;
				pbytSrc2 += nSrc2PixelBytes ;
				pbytDst += nDstPixelBytes ;
			}
		}
		pbytDstLine += nDstLineBytes ;
		pbytSrcLine += nSrcLineBytes ;
	}
	//
	return	eslErrSuccess ;
}

//
// 2つの画像の絶対差の合計を計算
//////////////////////////////////////////////////////////////////////////////
DWORD eriSumAbsDifferenceOfBlock
	( const EGL_IMAGE_INFO & eiiDst,
		const EGL_IMAGE_INFO & eiiSrc )
{
	if ( (eiiDst.dwImageWidth != eiiSrc.dwImageWidth)
		|| (eiiDst.dwImageHeight != eiiSrc.dwImageHeight) )
	{
		return	0x7FFFFFFF ;
	}
	if ( ((eiiDst.dwBitsPerPixel != 24)
			&& (eiiDst.dwBitsPerPixel != 32))
		|| ((eiiSrc.dwBitsPerPixel != 24)
			&& (eiiSrc.dwBitsPerPixel != 32)))
	{
		return	0x7FFFFFFF ;
	}
	//
	int		nDstPixelBytes = eiiDst.dwBitsPerPixel >> 3 ;
	int		nSrcPixelBytes = eiiSrc.dwBitsPerPixel >> 3 ;
	int		nDstLineBytes = eiiDst.dwBytesPerLine ;
	int		nSrcLineBytes = eiiSrc.dwBytesPerLine ;
	BYTE *	pbytDstLine = (BYTE*) eiiDst.ptrImageArray ;
	BYTE *	pbytSrcLine = (BYTE*) eiiSrc.ptrImageArray ;
	DWORD	dwWidth = eiiDst.dwImageWidth ;
	DWORD	dwHeight = eiiDst.dwImageHeight ;
	DWORD	dwSumAbsDiff = 0 ;
	//
	for ( DWORD y = 0; y < dwHeight; y ++ )
	{
		BYTE *	pbytDst = pbytDstLine ;
		BYTE *	pbytSrc = pbytSrcLine ;
		//
		if ( (eiiDst.fdwFormatType & EIF_WITH_ALPHA)
				&& (nDstPixelBytes == 4) && (nSrcPixelBytes == 4) )
		{
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				int	db = (int) pbytDst[0] - (int) pbytSrc[0] ;
				int	dg = (int) pbytDst[1] - (int) pbytSrc[1] ;
				int	dr = (int) pbytDst[2] - (int) pbytSrc[2] ;
				int	da = (int) pbytDst[2] - (int) pbytSrc[2] ;
				int	fb = (db >> 31) ;
				int	fg = (dg >> 31) ;
				int	fr = (dr >> 31) ;
				int	fa = (da >> 31) ;
				dwSumAbsDiff += (db ^ fb) - fb ;
				dwSumAbsDiff += (dg ^ fg) - fg ;
				dwSumAbsDiff += (dr ^ fr) - fr ;
				dwSumAbsDiff + = (da ^ fa) - fa ;
				pbytSrc += 4 ;
				pbytDst += 4 ;
			}
		}
		else
		{
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				int	db = (int) pbytDst[0] - (int) pbytSrc[0] ;
				int	dg = (int) pbytDst[1] - (int) pbytSrc[1] ;
				int	dr = (int) pbytDst[2] - (int) pbytSrc[2] ;
				int	fb = (db >> 31) ;
				int	fg = (dg >> 31) ;
				int	fr = (dr >> 31) ;
				dwSumAbsDiff += (db ^ fb) - fb ;
				dwSumAbsDiff += (dg ^ fg) - fg ;
				dwSumAbsDiff += (dr ^ fr) - fr ;
				pbytSrc += nSrcPixelBytes ;
				pbytDst += nDstPixelBytes ;
			}
		}
		pbytDstLine += nDstLineBytes ;
		pbytSrcLine += nSrcLineBytes ;
	}
	//
	return	dwSumAbsDiff ;
}

void eriImageFilterLoop421( const EGL_IMAGE_INFO & eiiDst )
{
	// ダミー実装
}
