
/*****************************************************************************
                         E R I S A - L i b r a r y
                                                      ç≈èIçXêV 2003/01/27
 ----------------------------------------------------------------------------
         Copyright (C) 2000-2003 Leshade Entis. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <math.h>
#include <xerisa.h>


#if	!defined(COMPACT_NOA_DECODER)

//
// íËêîÉeÅ[ÉuÉã
//////////////////////////////////////////////////////////////////////////////

static const double	ERI_PI = 3.141592653589 ;	// = ÉŒ
static const REAL32	ERI_rHalf = 0.5F ;			// = 1/2
static const REAL32	ERI_r2 = 2.0F ;				// = 2.0
static REAL32		ERI_rCosPI4 ;				// = cos(pi/4)
static REAL32		ERI_r2CosPI4 ;				// = 2*cos(pi/4)


//
// 2x2 âÒì]çsóÒ
//////////////////////////////////////////////////////////////////////////////
void eriRevolve2x2
	( REAL32 * ptrBuf1, REAL32 * ptrBuf2,
		REAL32 rSin, REAL32 rCos,
		unsigned int nStep, unsigned int nCount )
{
	unsigned int	i ;
	for ( i = 0; i < nCount; i ++ )
	{
		REAL32	r1 = *ptrBuf1 ;
		REAL32	r2 = *ptrBuf2 ;
		//
		REAL32	R1 = r1 * rCos - r2 * rSin ;
		REAL32	R2 = r1 * rSin + r2 * rCos ;
		//
		*ptrBuf1 = R1 ;
		*ptrBuf2 = R2 ;
		//
		ptrBuf1 += nStep ;
		ptrBuf2 += nStep ;
	}
}

//
// LOT ïœä∑ópâÒì]ÉpÉâÉÅÅ[É^Çê∂ê¨Ç∑ÇÈ
//////////////////////////////////////////////////////////////////////////////
ERI_SIN_COS * eriCreateRevolveParameter( unsigned int nDegreeDCT )
{
	signed int	i, nDegreeNum ;
	nDegreeNum = 1 << nDegreeDCT ;
	//
	signed int	lc = 1, n = nDegreeNum / 2 ;
	while ( n >= 8 )
	{
		n /= 8 ;
		++ lc ;
	}
	ERI_SIN_COS *	ptrRevolve = (ERI_SIN_COS*)
		::eslHeapAllocate( NULL, lc * 8 * sizeof(ERI_SIN_COS), 0 ) ;
	//
	double	k = ERI_PI / (nDegreeNum * 2) ;
	ERI_SIN_COS *	ptrNextRev = ptrRevolve ;
	signed int	nStep = 2 ;
	do
	{
		for ( i = 0; i < 7; i ++ )
		{
			double	ws = 1.0 ;
			double	a = 0.0 ;
			for ( signed int j = 0; j < i; j ++ )
			{
				a += nStep ;
				ws = ws * ptrNextRev[j].rSin
					+ ptrNextRev[j].rCos * cos( a * k ) ;
			}
			double	r = atan2( ws, cos( (a + nStep) * k ) ) ;
			ptrNextRev[i].rSin = (REAL32) sin( r ) ;
			ptrNextRev[i].rCos = (REAL32) cos( r ) ;
		}
		ptrNextRev += 7 ;
		nStep *= 8 ;
	}
	while ( nStep < nDegreeNum ) ;
	//
	return	ptrRevolve ;
}

//
// çÇë¨ LOT ïœä∑éñëOèàóù
//////////////////////////////////////////////////////////////////////////////
void eriFastPLOT
	(
		REAL32 *		ptrSrc,
		unsigned int	nDegreeDCT
	)
{
	signed int	i, nDegreeNum ;
	nDegreeNum = 1 << nDegreeDCT ;
	REAL32	r1, r2 ;
	//
	// ãÙêîé¸îgÇ∆äÔêîé¸îgÇçáéZÇ∑ÇÈ
	//
	for ( i = 0; i < nDegreeNum; i += 2 )
	{
		r1 = ptrSrc[i] ;
		r2 = ptrSrc[i + 1] ;
		ptrSrc[i]     = r1 + r2 ;
		ptrSrc[i + 1] = r1 - r2 ;
	}
}

//
// çÇë¨ LOT ïœä∑
//////////////////////////////////////////////////////////////////////////////
void eriFastLOT
	(
		REAL32 *		ptrDst,
		const REAL32 *	ptrSrc1,
		const REAL32 *	ptrSrc2,
		unsigned int	nDegreeDCT
	)
{
	unsigned int	i, nDegreeNum ;
	nDegreeNum = 1 << nDegreeDCT ;
	REAL32	r1, r2 ;
	//
	// èdï°âª
	//
	for ( i = 0; i < nDegreeNum; i += 2 )
	{
		r1 = ptrSrc2[i] ;
		r2 = ptrSrc1[i + 1] ;
		ptrDst[i]     = ERI_rHalf * (r1 + r2) ;
		ptrDst[i + 1] = ERI_rHalf * (r1 - r2) ;
	}
}

//
// ïΩñ âÒì]çsóÒ
//////////////////////////////////////////////////////////////////////////////
void eriOddGivensMatrix
	(
		REAL32 *			ptrDst,
		const ERI_SIN_COS *	ptrRevolve,
		unsigned int		nDegreeDCT
	)
{
	unsigned int	i, j, k, nDegreeNum ;
	nDegreeNum = 1 << nDegreeDCT ;
	REAL32	r1, r2 ;
	//
	// äÔêîé¸îgÇâÒì]ëÄçÏ
	//
	unsigned int	nStep, lc, index ;
	index = 1 ;
	nStep = 2 ;
	lc = (nDegreeNum / 2) / 8 ;
	for ( ; ; )
	{
		for ( i = 0; i < lc; i ++ )
		{
			k = i * (nStep * 8) + index ;
			for ( j = 0; j < 7; j ++ )
			{
				r1 = ptrDst[k] ;
				r2 = ptrDst[k + nStep] ;
				ptrDst[k] =
					r1 * ptrRevolve[j].rCos - r2 * ptrRevolve[j].rSin ;
				ptrDst[k + nStep] =
					r1 * ptrRevolve[j].rSin + r2 * ptrRevolve[j].rCos ;
				k += nStep ;
			}
		}
		ptrRevolve += 7 ;
		index += nStep * 7 ;
		nStep *= 8 ;
		if ( lc <= 8 )
			break ;
		lc /= 8 ;
	}
	k = index ;
	for ( j = 0; j < lc - 1; j ++ )
	{
		r1 = ptrDst[k] ;
		r2 = ptrDst[k + nStep] ;
		ptrDst[k] =
			r1 * ptrRevolve[j].rCos - r2 * ptrRevolve[j].rSin ;
		ptrDst[k + nStep] =
			r1 * ptrRevolve[j].rSin + r2 * ptrRevolve[j].rCos ;
		k += nStep ;
	}
}

//
// ãtïœä∑ïΩñ âÒì]çsóÒ
//////////////////////////////////////////////////////////////////////////////
void eriOddGivensInverseMatrix
	(
		REAL32 *			ptrSrc,
		const ERI_SIN_COS *	ptrRevolve,
		unsigned int		nDegreeDCT
	)
{
	signed int	i, j, k, nDegreeNum ;
	nDegreeNum = 1 << nDegreeDCT ;
	REAL32	r1, r2 ;
	//
	// äÔêîé¸îgÇâÒì]ëÄçÏ
	//
	signed int	nStep, lc, index ;
	index = 1 ;
	nStep = 2 ;
	lc = (nDegreeNum / 2) / 8 ;
	for ( ; ; )
	{
		ptrRevolve += 7 ;
		index += nStep * 7 ;
		nStep *= 8 ;
		if ( lc <= 8 )
			break ;
		lc /= 8 ;
	}
	k = index + nStep * (lc - 2) ;
	for ( j = lc - 2; j >= 0; j -- )
	{
		r1 = ptrSrc[k] ;
		r2 = ptrSrc[k + nStep] ;
		ptrSrc[k] =
			r1 * ptrRevolve[j].rCos + r2 * ptrRevolve[j].rSin ;
		ptrSrc[k + nStep] =
			r2 * ptrRevolve[j].rCos - r1 * ptrRevolve[j].rSin ;
		k -= nStep ;
	}
	for ( ; ; )
	{
		if ( lc > (nDegreeNum / 2) / 8 )
			break ;
		//
		ptrRevolve -= 7 ;
		nStep /= 8 ;
		index -= nStep * 7 ;
		//
		for ( i = 0; i < lc; i ++ )
		{
			k = i * (nStep * 8) + index + nStep * 6 ;
			for ( j = 6; j >= 0; j -- )
			{
				r1 = ptrSrc[k] ;
				r2 = ptrSrc[k + nStep] ;
				ptrSrc[k] =
					r1 * ptrRevolve[j].rCos + r2 * ptrRevolve[j].rSin ;
				ptrSrc[k + nStep] =
					r2 * ptrRevolve[j].rCos - r1 * ptrRevolve[j].rSin ;
				k -= nStep ;
			}
		}
		//
		lc *= 8 ;
	}
}

//
// çÇë¨ LOT ãtïœä∑éñëOèàóù
////////////////////////////////////////////////////////////////////////////// 
void eriFastIPLOT
	(
		REAL32 *		ptrSrc,
		unsigned int	nDegreeDCT
	)
{
	signed int	i, nDegreeNum ;
	nDegreeNum = 1 << nDegreeDCT ;
	REAL32	r1, r2 ;
	//
	// äÔêîé¸îgÇ∆ãÙêîé¸îgÇÃê¨ï™Çï™â
	//
	for ( i = 0; i < nDegreeNum; i += 2 )
	{
		r1 = ptrSrc[i] ;
		r2 = ptrSrc[i + 1] ;
		ptrSrc[i]     = ERI_rHalf * (r1 + r2) ;
		ptrSrc[i + 1] = ERI_rHalf * (r1 - r2) ;
	}
}

//
// çÇë¨ LOT ãtïœä∑
//////////////////////////////////////////////////////////////////////////////
void eriFastILOT
	(
		REAL32 *		ptrDst,
		const REAL32 *	ptrSrc1,
		const REAL32 *	ptrSrc2,
		unsigned int	nDegreeDCT
	)
{
	unsigned int	i, nDegreeNum ;
	nDegreeNum = 1 << nDegreeDCT ;
	REAL32	r1, r2 ;
	//
	// ãtèdï°âª
	//
	for ( i = 0; i < nDegreeNum; i += 2 )
	{
		r1 = ptrSrc1[i] ;
		r2 = ptrSrc2[i + 1] ;
		ptrDst[i]     = r1 + r2 ;
		ptrDst[i + 1] = r1 - r2 ;
	}
}

//
// çÇë¨2éüå≥DCT ïœä∑
//////////////////////////////////////////////////////////////////////////////
void eriFastDCT8x8( REAL32 * ptrDst )
{
	REAL32	rWork[8] ;
	REAL32	rTemp[64] ;
	int		i ;
	//
	for ( i = 0; i < 8; i ++ )
	{
		eriFastDCT( &rTemp[i], 8, ptrDst + i * 8, rWork, 3 ) ;
	}
	for ( i = 0; i < 8; i ++ )
	{
		eriFastDCT( ptrDst + i, 8, &rTemp[i * 8], rWork, 3 ) ;
	}
}

//
// çÇë¨2éüå≥ãtDCT ïœä∑
//////////////////////////////////////////////////////////////////////////////
void eriFastIDCT8x8( REAL32 * ptrDst )
{
	REAL32	rWork[8] ;
	REAL32	rTemp[64] ;
	int		i ;
	//
	for ( i = 0; i < 8; i ++ )
	{
		eriFastIDCT( &rTemp[i * 8], ptrDst + i, 8, rWork, 3 ) ;
	}
	for ( i = 0; i < 8; i ++ )
	{
		eriFastIDCT( ptrDst + i * 8, &rTemp[i], 8, rWork, 3 ) ;
	}
}

//
// çÇë¨2éüå≥ LOT ïœä∑
//////////////////////////////////////////////////////////////////////////////
void eriFastLOT8x8
	(
		REAL32 *		ptrDst,
		REAL32 *		ptrHorzCur,
		REAL32 *		ptrVertCur
	)
{
	static const ERI_SIN_COS	escRev[3] =
	{
		{	0.734510F, 0.678598F	},
		{	0.887443F, 0.460917F	},
		{	0.970269F, 0.242030F	}
	} ;
	REAL32	rWork[8] ;
	REAL32	rTemp[64] ;
	REAL32	s1, s2, r1, r2, r3 ;
	int		i, j, k ;
	//
	// 2éüå≥ 8x8 DCT ïœä∑
	//
	for ( i = 0; i < 8; i ++ )
	{
		eriFastDCT( &rTemp[i], 8, ptrDst + i * 8, rWork, 3 ) ;
	}
	for ( i = 0; i < 8; i ++ )
	{
		eriFastDCT( ptrDst + i, 8, &rTemp[i * 8], rWork, 3 ) ;
	}
	//
	// êÖïΩï˚å¸èdï°ïœä∑
	//
	for ( i = 0; i < 64; i += 8 )
	{
		for ( j = 0; j < 8; j += 2 )
		{
			k = i + j ;
			s1 = ptrDst[k] ;
			s2 = ptrDst[k + 1] ;
			r1 = s1 + s2 ;
			r2 = s1 - s2 ;
			//
			r3 = ptrHorzCur[k + 1] ;
			ptrHorzCur[k]     = r1 ;
			ptrHorzCur[k + 1] = r2 ;
			ptrDst[k]     = ERI_rHalf * (r1 + r3) ;
			ptrDst[k + 1] = ERI_rHalf * (r1 - r3) ;
		}
		for ( j = 0, k = i + 1; j < 3; j ++, k += 2 )
		{
			r1 = ptrDst[k] ;
			r2 = ptrDst[k + 2] ;
			ptrDst[k]     = r1 * escRev[j].rCos - r2 * escRev[j].rSin ;
			ptrDst[k + 2] = r1 * escRev[j].rSin + r2 * escRev[j].rCos ;
		}
	}
	//
	// êÇíºï˚å¸èdï°ïœä∑
	//
	for ( i = 0; i < 64; i += 16 )
	{
		for ( j = 0; j < 8; j ++ )
		{
			k = i + j ;
			s1 = ptrDst[k] ;
			s2 = ptrDst[k + 8] ;
			r1 = s1 + s2 ;
			r2 = s1 - s2 ;
			//
			r3 = ptrVertCur[k + 8] ;
			ptrVertCur[k]     = r1 ;
			ptrVertCur[k + 8] = r2 ;
			ptrDst[k]     = ERI_rHalf * (r1 + r3) ;
			ptrDst[k + 8] = ERI_rHalf * (r1 - r3) ;
		}
	}
	for ( i = 0; i < 8; i ++ )
	{
		for ( j = 0, k = i + 8; j < 3; j ++, k += 16 )
		{
			r1 = ptrDst[k] ;
			r2 = ptrDst[k + 16] ;
			ptrDst[k]      = r1 * escRev[j].rCos - r2 * escRev[j].rSin ;
			ptrDst[k + 16] = r1 * escRev[j].rSin + r2 * escRev[j].rCos ;
		}
	}
}

//
// çÇë¨2éüå≥ãt LOT ïœä∑
//////////////////////////////////////////////////////////////////////////////
void eriFastILOT8x8
	(
		REAL32 *		ptrDst,
		REAL32 *		ptrHorzCur,
		REAL32 *		ptrVertCur
	)
{
	static const ERI_SIN_COS	escRev[3] =
	{
		{	0.734510F, 0.678598F	},
		{	0.887443F, 0.460917F	},
		{	0.970269F, 0.242030F	}
	} ;
	REAL32	rWork[8] ;
	REAL32	rTemp[64] ;
	REAL32	s1, s2, r1, r2, r3 ;
	int		i, j, k ;
	//
	// êÇíºï˚å¸èdï°ïœä∑
	//
	for ( i = 0; i < 8; i ++ )
	{
		for ( j = 2, k = i + 40; j >= 0; j --, k -= 16 )
		{
			r1 = ptrDst[k] ;
			r2 = ptrDst[k + 16] ;
			ptrDst[k]      = r1 * escRev[j].rCos + r2 * escRev[j].rSin ;
			ptrDst[k + 16] = r2 * escRev[j].rCos - r1 * escRev[j].rSin ;
		}
	}
	for ( i = 0; i < 64; i += 16 )
	{
		for ( j = 0; j < 8; j ++ )
		{
			k = i + j ;
			s1 = ptrDst[k] ;
			s2 = ptrDst[k + 8] ;
			r1 = ERI_rHalf * (s1 + s2) ;
			r2 = ERI_rHalf * (s1 - s2) ;
			//
			r3 = ptrVertCur[k] ;
			ptrVertCur[k]     = r1 ;
			ptrVertCur[k + 8] = r2 ;
			ptrDst[k]     = r3 + r2 ;
			ptrDst[k + 8] = r3 - r2 ;
		}
	}
	//
	// êÖïΩï˚å¸èdï°ïœä∑
	//
	for ( i = 0; i < 64; i += 8 )
	{
		for ( j = 2, k = i + 5; j >= 0; j --, k -= 2 )
		{
			r1 = ptrDst[k] ;
			r2 = ptrDst[k + 2] ;
			ptrDst[k]     = r1 * escRev[j].rCos + r2 * escRev[j].rSin ;
			ptrDst[k + 2] = r2 * escRev[j].rCos - r1 * escRev[j].rSin ;
		}
		for ( j = 0; j < 8; j += 2 )
		{
			k = i + j ;
			s1 = ptrDst[k] ;
			s2 = ptrDst[k + 1] ;
			r1 = ERI_rHalf * (s1 + s2) ;
			r2 = ERI_rHalf * (s1 - s2) ;
			//
			r3 = ptrHorzCur[k] ;
			ptrHorzCur[k]     = r1 ;
			ptrHorzCur[k + 1] = r2 ;
			ptrDst[k]     = r3 + r2 ;
			ptrDst[k + 1] = r3 - r2 ;
		}
	}
	//
	// 2éüå≥ 8x8 IDCT ïœä∑
	//
	for ( i = 0; i < 8; i ++ )
	{
		eriFastIDCT( &rTemp[i * 8], ptrDst + i, 8, rWork, 3 ) ;
	}
	for ( i = 0; i < 8; i ++ )
	{
		eriFastIDCT( ptrDst + i * 8, &rTemp[i], 8, rWork, 3 ) ;
	}
}

//
// RGB-YUV êFãÛä‘ïœä∑
//////////////////////////////////////////////////////////////////////////////
void eriConvertRGBtoYUV
	( REAL32 * ptrBuf1, REAL32 * ptrBuf2,
		REAL32 * ptrBuf3, unsigned int nCount )
{
	static const double	rMatrixRGB2YUV[3][3] =
	{
		{
			7.0 / 24.0,		7.0 / 12.0,		1.0 / 8.0
		},
		{
			-1.0 / 6.0,		-1.0 / 3.0,		1.0 / 2.0
		},
		{
			17.0 / 36.0,	-7.0 / 18.0,	-1.0 / 12.0
		}
	} ;
	for ( unsigned int i = 0; i < nCount; i ++ )
	{
		REAL32	rBlue = ptrBuf1[i] ;
		REAL32	rGreen = ptrBuf2[i] ;
		REAL32	rRed = ptrBuf3[i] ;
		ptrBuf1[i] =
			(REAL32)(rMatrixRGB2YUV[0][0] * rRed
					+ rMatrixRGB2YUV[0][1] * rGreen
					+ rMatrixRGB2YUV[0][2] * rBlue) ;
		ptrBuf2[i] =
			(REAL32)(rMatrixRGB2YUV[1][0] * rRed
					+ rMatrixRGB2YUV[1][1] * rGreen
					+ rMatrixRGB2YUV[1][2] * rBlue) ;
		ptrBuf3[i] =
			(REAL32)(rMatrixRGB2YUV[2][0] * rRed
					+ rMatrixRGB2YUV[2][1] * rGreen
					+ rMatrixRGB2YUV[2][2] * rBlue) ;
	}
}


#endif

