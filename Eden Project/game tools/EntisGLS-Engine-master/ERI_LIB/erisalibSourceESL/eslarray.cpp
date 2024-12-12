
/*****************************************************************************
                    Entis Standard Library declarations
                                                    last update 2002/04/19
 ----------------------------------------------------------------------------

	In this file, the array classes implementation.

	Copyright (C) 1998-2002 Leshade Entis. All rights reserved.

 ****************************************************************************/


// Include esl.h
//////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<eritypes.h>
#include	<esl.h>


/****************************************************************************
                          ポインタ配列クラス
 ****************************************************************************/

IMPLEMENT_CLASS_INFO( EPtrArray, ESLObject )

// 構築関数
//////////////////////////////////////////////////////////////////////////////
EPtrArray::EPtrArray
	( const EPtrArray & array, unsigned int nFirst, unsigned int nCount )
{
	//
	// パラメータを正規化
	//
	if ( nCount == -1 )
	{
		m_nLength = array.GetSize( ) ;
	}
	if ( nFirst >= GetSize() )
	{
		m_nLength = 0 ;
	}
	else if ( (nFirst + m_nLength) >= array.GetSize() )
	{
		m_nLength = array.GetSize() - nFirst ;
	}
	//
	// 初期化
	//
	unsigned int	nGrowAlign = m_nGrowAlign = array.m_nGrowAlign ;
	if ( nGrowAlign == 0 )
	{
		if ( (nGrowAlign = (m_nLength >> 1)) == 0 )
			nGrowAlign = 1 ;
	}
	m_nMaxSize = m_nLength - (m_nLength % nGrowAlign) + nGrowAlign ;
	m_ptrArray = (void**)
		::eslHeapAllocate( NULL, m_nMaxSize * sizeof(void*), 0 ) ;
	//
	// 配列複製
	//
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		m_ptrArray[i] = array.m_ptrArray[nFirst + i] ;
	}
}

// メモリ確保
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::AlloBuffer( unsigned int nSize )
{
	if ( m_ptrArray == NULL )
	{
		m_ptrArray = (void**)
			::eslHeapAllocate( NULL, nSize * sizeof(void*), 0 ) ;
	}
	else
	{
		m_ptrArray = (void**) ::eslHeapReallocate
			( NULL, m_ptrArray, nSize * sizeof(void*), 0 ) ;
	}
	m_nMaxSize = nSize ;
}

// メモリ開放
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::FreeBuffer( void )
{
	if ( m_ptrArray != NULL )
	{
		::eslHeapFree( NULL, m_ptrArray ) ;
	}
	m_ptrArray = NULL ;
	m_nMaxSize = 0 ;
}

// 配列の長さを変更
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::SetSize( unsigned int nSize, unsigned int nGrowAlign )
{
	//
	// 配列の長さの拡張サイズを正規化
	//
	if ( nGrowAlign == 0 )
	{
		if ( m_nGrowAlign == 0 )
		{
			nGrowAlign = (m_nLength >> 1) ;
		}
		else
		{
			nGrowAlign = m_nGrowAlign ;
		}
		if ( nGrowAlign == 0 )
		{
			nGrowAlign = 1 ;
		}
	}
	else
	{
		m_nGrowAlign = nGrowAlign ;
	}
	//
	// 配列のサイズを設定
	//
	if ( (m_ptrArray == NULL) || (nSize > m_nMaxSize) )
	{
		AlloBuffer( nSize - (nSize % nGrowAlign) + nGrowAlign ) ;
	}
	if ( (m_ptrArray != NULL) && (nSize > m_nLength) )
	{
		for( unsigned int i = m_nLength; i < nSize; i ++ )
		{
			m_ptrArray[i] = NULL ;
		}
	}
	m_nLength = nSize ;
}

// 配列の内部メモリのサイズを設定
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::SetLimit( unsigned int nLimit, unsigned int nGrowAlign )
{
	//
	// 配列の拡張サイズを設定
	//
	if( nGrowAlign != 0 )
	{
		m_nGrowAlign = nGrowAlign ;
	}
	//
	// 配列のリミットを設定
	//
	if( m_nLength > nLimit )
	{
		return ;
	}
	AlloBuffer( nLimit ) ;
}

// 要素を挿入
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::InsertAt( unsigned int nIndex, void * ptrData )
{
	//
	// 挿入の為に配列の長さを拡張
	//
	ESLAssert( (signed int) nIndex >= 0 ) ;
	if ( nIndex >= m_nLength )
	{
		SetSize( nIndex + 1 ) ;
	}
	else
	{
		SetSize( m_nLength + 1 ) ;
	}
	//
	// 要素をシフト
	//
	ESLAssert( m_nLength >= 1 ) ;
	for ( unsigned int i = m_nLength - 1; i > nIndex; i -- )
	{
		m_ptrArray[i] = m_ptrArray[i - 1] ;
	}
	//
	// 要素を設定
	//
	m_ptrArray[nIndex] = ptrData ;
}

// 指定範囲の要素を削除
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::RemoveBetween
	( unsigned int nFirst, unsigned int nCount )
{
	//
	// 削除する数を正規化
	//
	if ( nFirst >= m_nLength )
	{
		return ;
	}
	if ( (nFirst + nCount) > m_nLength )
	{
		nCount = m_nLength - nFirst ;
	}
	//
	// 削除
	//
	if ( nCount > 0 )
	{
		//
		// 要素をシフトする
		//
		for ( unsigned int i = nFirst + nCount; i < m_nLength; i ++ )
		{
			m_ptrArray[i - nCount] = m_ptrArray[i] ;
		}
		//
		// 配列の長さを縮小
		//
		ESLAssert( m_nLength >= nCount ) ;
		SetSize( m_nLength - nCount ) ;
	}
}

// 配列を削除
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::RemoveAll( void )
{
	m_nLength = 0 ;
	FreeBuffer( ) ;
}

// 指定の要素を入れ替える
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::Swap( unsigned int nIndex1, unsigned int nIndex2 )
{
	ESLAssert( (nIndex1 < m_nLength) && (nIndex2 < m_nLength) ) ;
	void *	ptrTemp = m_ptrArray[nIndex1] ;
	m_ptrArray[nIndex1] = m_ptrArray[nIndex2] ;
	m_ptrArray[nIndex2] = ptrTemp ;
}

// 配列を結合する
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::Merge
	( int nIndex, const EPtrArray & array,
		unsigned int nFirst, unsigned int nCount )
{
	//
	// パラメータを正規化
	//
	if ( nCount == -1 )
	{
		nCount = array.GetSize( ) ;
	}
	if ( nFirst >= array.GetSize() )
	{
		return ;
	}
	else if ( (nFirst + nCount) >= array.GetSize() )
	{
		nCount = array.GetSize() - nFirst ;
	}
	if ( nCount == 0 )
	{
		return ;
	}
	//
	// 配列を拡張して要素をシフト
	//
	ESLAssert( (unsigned int) nIndex <= m_nLength ) ;
	unsigned int	i, len ;
	len = m_nLength ;
	//
	SetSize( m_nLength + nCount ) ;
	//
	for ( i = len - 1; (int) i >= nIndex; i -- )
	{
		m_ptrArray[i + nCount] = m_ptrArray[i] ;
	}
	//
	// 要素を複製
	//
	for ( i = 0; i < nCount; i ++ )
	{
		m_ptrArray[nIndex + i] = array.m_ptrArray[nFirst + i] ;
	}
}


