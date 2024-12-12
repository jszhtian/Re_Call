
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>


/*****************************************************************************
                       ライブラリ初期化・終了関数
 *****************************************************************************/

//
// ライブラリ初期化関数
//////////////////////////////////////////////////////////////////////////////
void eriInitializeLibrary( void )
{
#if	defined(ERI_INTEL_X86)
	//
	// MMX を搭載していれば有効に
	eriEnableMMX( 0 ) ;
	//
	// FPUを初期化
	eriInitializeFPU( ) ;
#endif
	//
	// DCT 行列演算用のテーブルを初期化
	eriInitializeCodec( ) ;
}

//
// ライブラリ終了関数
//////////////////////////////////////////////////////////////////////////////
void eriCloseLibrary( void )
{
}

//
// スレッド初期化関数
//////////////////////////////////////////////////////////////////////////////
void eriInitializeTask( void )
{
#if	defined(ERI_INTEL_X86)
	//
	// FPUを初期化
	eriInitializeFPU( ) ;
#endif
}

//
// スレッド終了関数
//////////////////////////////////////////////////////////////////////////////
void eriCloseTask( void )
{
}

//
// CODEC 初期化
//////////////////////////////////////////////////////////////////////////////
void eriInitializeCodec( void )
{
#if	!defined(COMPACT_NOA_DECODER)
	eriInitializeMatrix( ) ;
#endif
}


/*****************************************************************************
                            アニメーション用関数
 *****************************************************************************/

#if	!defined(COMPACT_NOA_DECODER)

//
// 可逆圧縮のためのフレーム差分
//////////////////////////////////////////////////////////////////////////////
ESLError eriLLSubtractionOfFrame
	( const EGL_IMAGE_INFO & eiiDst,
		const EGL_IMAGE_INFO & eiiSrc )
{
	if ( (eiiDst.dwImageWidth != eiiSrc.dwImageWidth)
		|| (eiiDst.dwImageHeight != eiiSrc.dwImageHeight)
		|| (eiiDst.dwBitsPerPixel != eiiSrc.dwBitsPerPixel) )
	{
		return	eslErrGeneral ;
	}
	if ( (eiiDst.dwBitsPerPixel != 24)
		&& (eiiDst.dwBitsPerPixel != 32) )
	{
		return	eslErrGeneral ;
	}
	//
	int		nPixelBytes = eiiDst.dwBitsPerPixel >> 3 ;
	int		nDstLineBytes = eiiDst.dwBytesPerLine ;
	int		nSrcLineBytes = eiiSrc.dwBytesPerLine ;
	BYTE *	pbytDstLine = (BYTE*) eiiDst.ptrImageArray ;
	BYTE *	pbytSrcLine = (BYTE*) eiiSrc.ptrImageArray ;
	DWORD	dwWidth = eiiDst.dwImageWidth ;
	DWORD	dwHeight = eiiDst.dwImageHeight ;
	//
	for ( DWORD y = 0; y < dwHeight; y ++ )
	{
		if ( nPixelBytes == 4 )
		{
			BYTE *	pbytDst = pbytDstLine ;
			BYTE *	pbytSrc = pbytSrcLine ;
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				pbytDst[0] -= pbytSrc[0] ;
				pbytDst[1] -= pbytSrc[1] ;
				pbytDst[2] -= pbytSrc[2] ;
				pbytDst[3] -= pbytSrc[3] ;
				pbytSrc += 4 ;
				pbytDst += 4 ;
			}
		}
		else
		{
			BYTE *	pbytDst = pbytDstLine ;
			BYTE *	pbytSrc = pbytSrcLine ;
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				pbytDst[0] -= pbytSrc[0] ;
				pbytDst[1] -= pbytSrc[1] ;
				pbytDst[2] -= pbytSrc[2] ;
				pbytSrc += 3 ;
				pbytDst += 3 ;
			}
		}
		pbytDstLine += nDstLineBytes ;
		pbytSrcLine += nSrcLineBytes ;
	}
	//
	return	eslErrSuccess ;
}

//
// 非可逆圧縮のためのフレーム差分
//////////////////////////////////////////////////////////////////////////////
int eriLSSubtractionOfFrame
	( const EGL_IMAGE_INFO & eiiDst,
		const EGL_IMAGE_INFO & eiiSrc )
{
	if ( (eiiDst.dwImageWidth != eiiSrc.dwImageWidth)
		|| (eiiDst.dwImageHeight != eiiSrc.dwImageHeight)
		|| (eiiDst.dwBitsPerPixel != eiiSrc.dwBitsPerPixel) )
	{
		return	0x200 ;
	}
	if ( (eiiDst.dwBitsPerPixel != 24)
		&& (eiiDst.dwBitsPerPixel != 32) )
	{
		return	0x200 ;
	}
	//
	int		nPixelBytes = eiiDst.dwBitsPerPixel >> 3 ;
	int		nDstLineBytes = eiiDst.dwBytesPerLine ;
	int		nSrcLineBytes = eiiSrc.dwBytesPerLine ;
	BYTE *	pbytDstLine = (BYTE*) eiiDst.ptrImageArray ;
	BYTE *	pbytSrcLine = (BYTE*) eiiSrc.ptrImageArray ;
	DWORD	dwWidth = eiiDst.dwImageWidth ;
	DWORD	dwHeight = eiiDst.dwImageHeight ;
	int		dr = 0, dg = 0, db = 0, da = 0 ;
	int		lr = 0, lg = 0, lb = 0, la = 0 ;
	int		f ;
	//
	for ( DWORD y = 0; y < dwHeight; y ++ )
	{
		BYTE *	pbytDst = pbytDstLine ;
		BYTE *	pbytSrc = pbytSrcLine ;
		if ( nPixelBytes == 4 )
		{
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				int		b = (int) pbytDst[0] - (int) pbytSrc[0] ;
				int		g = (int) pbytDst[1] - (int) pbytSrc[1] ;
				int		r = (int) pbytDst[2] - (int) pbytSrc[2] ;
				int		a = (int) pbytDst[3] - (int) pbytSrc[3] ;
				pbytSrc += 4 ;
				pbytDst[0] = (BYTE) (b >> 1) ;
				pbytDst[1] = (BYTE) (g >> 1) ;
				pbytDst[2] = (BYTE) (r >> 1) ;
				pbytDst[3] = (BYTE) (a >> 1) ;
				pbytDst += 4 ;
				//
				lb -= b ;
				lg -= g ;
				lr -= r ;
				la -= a ;
				//
				f = lb >> 31 ;
				lb = (lb ^ f) - f ;
				f = lg >> 31 ;
				lg = (lg ^ f) - f ;
				f = lr >> 31 ;
				lr = (lr ^ f) - f ;
				f = la >> 31 ;
				la = (la ^ f) - f ;
				//
				f = (lb - db) >> 31 ;
				db = (db & f) | (lb & ~f) ;
				f = (lg - dg) >> 31 ;
				dg = (dg & f) | (lg & ~f) ;
				f = (lr - dr) >> 31 ;
				dr = (dr & f) | (lr & ~f) ;
				f = (la - da) >> 31 ;
				da = (da & f) | (la & ~f) ;
				//
				lb = b ;
				lg = g ;
				lr = r ;
				la = a ;
			}
		}
		else
		{
			for ( DWORD x = 0; x < dwWidth; x ++ )
			{
				int		b = (int) pbytDst[0] - (int) pbytSrc[0] ;
				int		g = (int) pbytDst[1] - (int) pbytSrc[1] ;
				int		r = (int) pbytDst[2] - (int) pbytSrc[2] ;
				pbytSrc += 3 ;
				pbytDst[0] = (BYTE) (b >> 1) ;
				pbytDst[1] = (BYTE) (g >> 1) ;
				pbytDst[2] = (BYTE) (r >> 1) ;
				pbytDst += 3 ;
				//
				lb -= b ;
				lg -= g ;
				lr -= r ;
				//
				f = lb >> 31 ;
				lb = (lb ^ f) - f ;
				f = lg >> 31 ;
				lg = (lg ^ f) - f ;
				f = lr >> 31 ;
				lr = (lr ^ f) - f ;
				//
				f = (lb - db) >> 31 ;
				db = (db & f) | (lb & ~f) ;
				f = (lg - dg) >> 31 ;
				dg = (dg & f) | (lg & ~f) ;
				f = (lr - dr) >> 31 ;
				dr = (dr & f) | (lr & ~f) ;
				//
				lb = b ;
				lg = g ;
				lr = r ;
			}
		}
		pbytDstLine += nDstLineBytes ;
		pbytSrcLine += nSrcLineBytes ;
	}
	//
	if ( db < dg )
	{
		db = dg ;
	}
	if ( db < dr )
	{
		db = dr ;
	}
	if ( eiiDst.fdwFormatType & EIF_WITH_ALPHA )
	{
		if ( db < da )
		{
			db = da ;
		}
	}
	//
	return	db ;
}

#endif


/*****************************************************************************
                         ハフマン・ツリー・構造体
 *****************************************************************************/

#if	!defined(COMPACT_NOA_DECODER)

//
// ツリーの初期化
//////////////////////////////////////////////////////////////////////////////
void ERINA_HUFFMAN_TREE::Initialize( void )
{
	int	i ;
	for ( i = 0; i < 0x100; i ++ )
	{
		m_iSymLookup[i] = ERINA_HUFFMAN_NULL ;
	}
	m_iEscape = ERINA_HUFFMAN_NULL ;
	m_iTreePointer = ERINA_HUFFMAN_ROOT ;
	m_hnTree[ERINA_HUFFMAN_ROOT].m_weight = 0 ;
	m_hnTree[ERINA_HUFFMAN_ROOT].m_parent = ERINA_HUFFMAN_NULL ;
	m_hnTree[ERINA_HUFFMAN_ROOT].m_child_code = ERINA_HUFFMAN_NULL ;
}

//
// 親の重みを再計算する
//////////////////////////////////////////////////////////////////////////////
void ERINA_HUFFMAN_TREE::RecountOccuredCount( int iParent )
{
	int	iChild = m_hnTree[iParent].m_child_code ;
	m_hnTree[iParent].m_weight =
		m_hnTree[iChild].m_weight
			+ m_hnTree[iChild + 1].m_weight ;
}

//
// 新しいフリーエントリを作成して追加
//////////////////////////////////////////////////////////////////////////////
void ERINA_HUFFMAN_TREE::AddNewEntry( int nNewCode )
{
	if ( m_iTreePointer > 0 )
	{
		//
		// 2つの領域を確保する
		int		i = m_iTreePointer = m_iTreePointer - 2 ;
		//
		// 新しいエントリを初期設定
		ERINA_HUFFMAN_NODE *	phnNew = &m_hnTree[i] ;
		phnNew->m_weight = 1 ;
		phnNew->m_child_code = ERINA_CODE_FLAG | nNewCode ;
		m_iSymLookup[nNewCode & 0xFF] = i ;
		//
		ERINA_HUFFMAN_NODE *	phnRoot = &m_hnTree[ERINA_HUFFMAN_ROOT] ;
		if ( phnRoot->m_child_code != ERINA_HUFFMAN_NULL )
		{
			//
			// 新しいエントリをツリーの末端に追加
			ERINA_HUFFMAN_NODE *	phnParent = &m_hnTree[i + 2] ;
			ERINA_HUFFMAN_NODE *	phnChild = &m_hnTree[i + 1] ;
			m_hnTree[i + 1] = m_hnTree[i + 2] ;
			//
			if ( phnChild->m_child_code & ERINA_CODE_FLAG )
			{
				int	nCode = phnChild->m_child_code & ~ERINA_CODE_FLAG ;
				if ( nCode != ERINA_HUFFMAN_ESCAPE )
					m_iSymLookup[nCode & 0xFF] = i + 1 ;
				else
					m_iEscape = i + 1 ;
			}
			//
			phnParent->m_weight =
				phnNew->m_weight + phnChild->m_weight ;
			phnParent->m_parent = phnChild->m_parent ;
			phnParent->m_child_code = i ;
			//
			phnNew->m_parent = phnChild->m_parent = i + 2 ;
			//
			// 親エントリの正規化
			Normalize( i + 2 ) ;
		}
		else
		{
			//
			// 初期状態のツリーを構築
			phnNew->m_parent = ERINA_HUFFMAN_ROOT ;
			//
			ERINA_HUFFMAN_NODE *
				phnEscape = &m_hnTree[m_iEscape = i + 1] ;
			phnEscape->m_weight = 1 ;
			phnEscape->m_parent = ERINA_HUFFMAN_ROOT ;
			phnEscape->m_child_code = ERINA_CODE_FLAG | ERINA_HUFFMAN_ESCAPE ;
			//
			phnRoot->m_weight = 2 ;
			phnRoot->m_child_code = i ;
		}
	}
	else
	{
		//
		// 最も出現頻度の低いシンボルを新しいシンボルで置き換える
		int		i = m_iTreePointer ;
		ERINA_HUFFMAN_NODE *	phnEntry = &m_hnTree[i] ;
		if ( phnEntry->m_child_code
				== (ERINA_CODE_FLAG | ERINA_HUFFMAN_ESCAPE) )
		{
			phnEntry = &m_hnTree[i + 1] ;
		}
		phnEntry->m_child_code = ERINA_CODE_FLAG | nNewCode ;
	}
}

//
// 各出現数を2分の1にして木を再構成
//////////////////////////////////////////////////////////////////////////////
void ERINA_HUFFMAN_TREE::HalfAndRebuild( void )
{
	//
	// 出現頻度を2分の1にする
	int		i ;
	int		iNextEntry = ERINA_HUFFMAN_ROOT ;
	for ( i = ERINA_HUFFMAN_ROOT - 1; i >= m_iTreePointer; i -- )
	{
		if ( m_hnTree[i].m_child_code & ERINA_CODE_FLAG )
		{
			m_hnTree[i].m_weight = (m_hnTree[i].m_weight + 1) >> 1 ;
			m_hnTree[iNextEntry --] = m_hnTree[i] ;
		}
	}
	++ iNextEntry ;
	//
	// ツリーを再構築
	int		iChild, nCode ;
	i = m_iTreePointer ;
	for ( ; ; )
	{
		//
		// 最も重みの小さい2つのエントリをハフマン木に組み込む
		m_hnTree[i] = m_hnTree[iNextEntry] ;
		m_hnTree[i + 1] = m_hnTree[iNextEntry + 1] ;
		iNextEntry += 2 ;
		ERINA_HUFFMAN_NODE *	phnChild1 = &m_hnTree[i] ;
		ERINA_HUFFMAN_NODE *	phnChild2 = &m_hnTree[i + 1] ;
		//
		if ( !(phnChild1->m_child_code & ERINA_CODE_FLAG) )
		{
			iChild = phnChild1->m_child_code ;
			m_hnTree[iChild].m_parent = i ;
			m_hnTree[iChild + 1].m_parent = i ;
		}
		else
		{
			nCode = phnChild1->m_child_code & ~ERINA_CODE_FLAG ;
			if ( nCode == ERINA_HUFFMAN_ESCAPE )
				m_iEscape = i ;
			else
				m_iSymLookup[nCode & 0xFF] = i ;
		}
		//
		if ( !(phnChild2->m_child_code & ERINA_CODE_FLAG) )
		{
			iChild = phnChild2->m_child_code ;
			m_hnTree[iChild].m_parent = i + 1 ;
			m_hnTree[iChild + 1].m_parent = i + 1 ;
		}
		else
		{
			nCode = phnChild2->m_child_code & ~ERINA_CODE_FLAG ;
			if ( nCode == ERINA_HUFFMAN_ESCAPE )
				m_iEscape = i + 1 ;
			else
				m_iSymLookup[nCode & 0xFF] = i + 1 ;
		}
		//
		WORD	weight = phnChild1->m_weight + phnChild2->m_weight ;
		//
		// 親エントリをリストに組み込む
		if ( iNextEntry <= ERINA_HUFFMAN_ROOT )
		{
			int		j = iNextEntry ;
			for ( ; ; )
			{
				if ( weight <= m_hnTree[j].m_weight )
				{
					m_hnTree[j - 1].m_weight = weight ;
					m_hnTree[j - 1].m_child_code = i ;
					break ;
				}
				m_hnTree[j - 1] = m_hnTree[j] ;
				if ( ++ j > ERINA_HUFFMAN_ROOT )
				{
					m_hnTree[ERINA_HUFFMAN_ROOT].m_weight = weight ;
					m_hnTree[ERINA_HUFFMAN_ROOT].m_child_code = i ;
					break ;
				}
			}
			-- iNextEntry ;
		}
		else
		{
			m_hnTree[ERINA_HUFFMAN_ROOT].m_weight = weight ;
			m_hnTree[ERINA_HUFFMAN_ROOT].m_parent = ERINA_HUFFMAN_NULL ;
			m_hnTree[ERINA_HUFFMAN_ROOT].m_child_code = i ;
			phnChild1->m_parent = ERINA_HUFFMAN_ROOT ;
			phnChild2->m_parent = ERINA_HUFFMAN_ROOT ;
			break ;
		}
		//
		i += 2 ;
	}
}

#endif


/*****************************************************************************
                             算術符号統計モデル
 *****************************************************************************/

const int	ERISA_PROB_BASE::m_nShiftCount[4] = { 1, 3, 4, 5 } ;
const int	ERISA_PROB_BASE::m_nNewProbLimit[4] = { 0x01, 0x08, 0x10, 0x20 } ;

//
// 生起確率を累積する
//////////////////////////////////////////////////////////////////////////////
int ERISA_PROB_MODEL::AccumulateProb( SWORD wSymbol )
{
	int	iSym = FindSymbol( wSymbol ) ;
	ESLAssert( iSym >= 0 ) ;
	DWORD	dwOccured = acsSymTable[iSym].wOccured ;
	int		i = 0 ;
	while ( dwOccured < dwTotalCount )
	{
		dwOccured <<= 1 ;
		i ++ ;
	}
	return	i ;
}

//
// シンボルの生起数を 1/2 にする
//////////////////////////////////////////////////////////////////////////////
void ERISA_PROB_MODEL::HalfOccuredCount( void )
{
	DWORD	 i ;
	dwTotalCount = 0 ;
	for ( i = 0; i < dwSymbolSorts; i ++ )
	{
		dwTotalCount +=
			acsSymTable[i].wOccured =
				((acsSymTable[i].wOccured + 1) >> 1) ;
	}
	for ( i = 0; i < ERISA_SUB_SORT_MAX; i ++ )
	{
		acsSubModel[i].wOccured >>= 1 ;
	}
}

//
// 指定のシンボルの指標を取得する
//////////////////////////////////////////////////////////////////////////////
int ERISA_PROB_MODEL::FindSymbol( SWORD wSymbol )
{
	int		iSym = 0 ;
	while ( acsSymTable[iSym].wSymbol != wSymbol )
	{
		if ( (DWORD) ++ iSym >= dwSymbolSorts )
			return	-1 ;
	}
	return	iSym ;
}

// 指定のシンボルを追加する
//////////////////////////////////////////////////////////////////////////////
int ERISA_PROB_MODEL::AddSymbol( SWORD wSymbol )
{
	int		iSym = dwSymbolSorts ++ ;
	dwTotalCount ++ ;
	acsSymTable[iSym].wSymbol = wSymbol ;
	acsSymTable[iSym].wOccured = 1 ;
	return	iSym ;
}


/*****************************************************************************
                             BSHF 符号バッファ
 *****************************************************************************/

//
// クラス情報
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ERIBshfBuffer, ESLObject )

//
// 構築関数
//////////////////////////////////////////////////////////////////////////////
ERIBshfBuffer::ERIBshfBuffer( void )
{
	m_dwPassOffset = 0 ;
}

//
// 消滅関数
//////////////////////////////////////////////////////////////////////////////
ERIBshfBuffer::~ERIBshfBuffer( void )
{
}

//
// 符号化
//////////////////////////////////////////////////////////////////////////////
void ERIBshfBuffer::EncodeBuffer( void )
{
	//
	// 暗号化の準備
	//
	int		i ;
	int		iPos = (int) m_dwPassOffset ++ ;
	int		nPassLen = m_strPassword.GetLength( ) ;
	const char *	pszPass = m_strPassword ;
	if ( (int) m_dwPassOffset >= nPassLen )
	{
		m_dwPassOffset = 0 ;
	}
	//
	for ( i = 0; i < 32; i ++ )
	{
		m_bufBSHF[i] = 0 ;
		m_maskBSHF[i] = 0 ;
	}
	//
	// データを暗号化
	//
	int		iBit = 0 ;
	for ( i = 0; i < 256; i ++ )
	{
		iBit = (iBit + pszPass[iPos ++]) & 0xFF ;
		if ( iPos >= nPassLen )
		{
			iPos = 0 ;
		}
		int		iOffset = (iBit >> 3) ;
		int		iMask = (0x80 >> (iBit & 0x07)) ;
		while ( m_maskBSHF[iOffset] == 0xFF )
		{
			iBit = (iBit + 8) & 0xFF ;
			iOffset = (iBit >> 3) ;
		}
		while ( m_maskBSHF[iOffset] & iMask )
		{
			iBit ++ ;
			iMask >>= 1 ;
			if ( iMask == 0 )
			{
				iBit = (iBit + 8) & 0xFF ;
				iOffset = (iBit >> 3) ;
				iMask = 0x80 ;
			}
		}
		ESLAssert( iMask != 0 ) ;
		m_maskBSHF[iOffset] |= (BYTE) iMask ;
		//
		if ( m_srcBSHF[iOffset] & iMask )
		{
			m_bufBSHF[(i >> 3)] |= (BYTE) (0x80 >> (i & 0x07)) ;
		}
	}
}

//
// 復号
//////////////////////////////////////////////////////////////////////////////
#if	!defined(ERI_INTEL_X86)
void ERIBshfBuffer::DecodeBuffer( void )
{
	//
	// 復号の準備
	//
	int		i ;
	int		iPos = (int) m_dwPassOffset ++ ;
	int		nPassLen = m_strPassword.GetLength( ) ;
	const char *	pszPass = m_strPassword ;
	if ( (int) m_dwPassOffset >= nPassLen )
	{
		m_dwPassOffset = 0 ;
	}
	//
	for ( i = 0; i < 32; i ++ )
	{
		m_bufBSHF[i] = 0 ;
		m_maskBSHF[i] = 0 ;
	}
	//
	// 暗号を復号
	//
	int		iBit = 0 ;
	for ( i = 0; i < 256; i ++ )
	{
		iBit = (iBit + pszPass[iPos ++]) & 0xFF ;
		if ( iPos >= nPassLen )
		{
			iPos = 0 ;
		}
		int		iOffset = (iBit >> 3) ;
		int		iMask = (0x80 >> (iBit & 0x07)) ;
		while ( m_maskBSHF[iOffset] == 0xFF )
		{
			iBit = (iBit + 8) & 0xFF ;
			iOffset = (iBit >> 3) ;
		}
		while ( m_maskBSHF[iOffset] & iMask )
		{
			iBit ++ ;
			iMask >>= 1 ;
			if ( iMask == 0 )
			{
				iBit = (iBit + 8) & 0xFF ;
				iOffset = (iBit >> 3) ;
				iMask = 0x80 ;
			}
		}
		ESLAssert( iMask != 0 ) ;
		m_maskBSHF[iOffset] |= (BYTE) iMask ;
		//
		if ( m_srcBSHF[(i >> 3)] & (0x80 >> (i & 0x07)) )
		{
			m_bufBSHF[iOffset] |= iMask ;
		}
	}
}
#endif