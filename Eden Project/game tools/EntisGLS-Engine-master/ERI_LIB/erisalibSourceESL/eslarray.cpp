
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
                          �|�C���^�z��N���X
 ****************************************************************************/

IMPLEMENT_CLASS_INFO( EPtrArray, ESLObject )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
EPtrArray::EPtrArray
	( const EPtrArray & array, unsigned int nFirst, unsigned int nCount )
{
	//
	// �p�����[�^�𐳋K��
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
	// ������
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
	// �z�񕡐�
	//
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		m_ptrArray[i] = array.m_ptrArray[nFirst + i] ;
	}
}

// �������m��
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

// �������J��
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

// �z��̒�����ύX
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::SetSize( unsigned int nSize, unsigned int nGrowAlign )
{
	//
	// �z��̒����̊g���T�C�Y�𐳋K��
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
	// �z��̃T�C�Y��ݒ�
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

// �z��̓����������̃T�C�Y��ݒ�
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::SetLimit( unsigned int nLimit, unsigned int nGrowAlign )
{
	//
	// �z��̊g���T�C�Y��ݒ�
	//
	if( nGrowAlign != 0 )
	{
		m_nGrowAlign = nGrowAlign ;
	}
	//
	// �z��̃��~�b�g��ݒ�
	//
	if( m_nLength > nLimit )
	{
		return ;
	}
	AlloBuffer( nLimit ) ;
}

// �v�f��}��
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::InsertAt( unsigned int nIndex, void * ptrData )
{
	//
	// �}���ׂ̈ɔz��̒������g��
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
	// �v�f���V�t�g
	//
	ESLAssert( m_nLength >= 1 ) ;
	for ( unsigned int i = m_nLength - 1; i > nIndex; i -- )
	{
		m_ptrArray[i] = m_ptrArray[i - 1] ;
	}
	//
	// �v�f��ݒ�
	//
	m_ptrArray[nIndex] = ptrData ;
}

// �w��͈̗͂v�f���폜
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::RemoveBetween
	( unsigned int nFirst, unsigned int nCount )
{
	//
	// �폜���鐔�𐳋K��
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
	// �폜
	//
	if ( nCount > 0 )
	{
		//
		// �v�f���V�t�g����
		//
		for ( unsigned int i = nFirst + nCount; i < m_nLength; i ++ )
		{
			m_ptrArray[i - nCount] = m_ptrArray[i] ;
		}
		//
		// �z��̒������k��
		//
		ESLAssert( m_nLength >= nCount ) ;
		SetSize( m_nLength - nCount ) ;
	}
}

// �z����폜
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::RemoveAll( void )
{
	m_nLength = 0 ;
	FreeBuffer( ) ;
}

// �w��̗v�f�����ւ���
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::Swap( unsigned int nIndex1, unsigned int nIndex2 )
{
	ESLAssert( (nIndex1 < m_nLength) && (nIndex2 < m_nLength) ) ;
	void *	ptrTemp = m_ptrArray[nIndex1] ;
	m_ptrArray[nIndex1] = m_ptrArray[nIndex2] ;
	m_ptrArray[nIndex2] = ptrTemp ;
}

// �z�����������
//////////////////////////////////////////////////////////////////////////////
void EPtrArray::Merge
	( int nIndex, const EPtrArray & array,
		unsigned int nFirst, unsigned int nCount )
{
	//
	// �p�����[�^�𐳋K��
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
	// �z����g�����ėv�f���V�t�g
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
	// �v�f�𕡐�
	//
	for ( i = 0; i < nCount; i ++ )
	{
		m_ptrArray[nIndex + i] = array.m_ptrArray[nFirst + i] ;
	}
}


