
/*****************************************************************************
                    Entis Standard Library declarations
 ----------------------------------------------------------------------------

	In this file, the array classes definitions.

	Copyright (C) 1998-2003 Leshade Entis. All rights reserved.

 ****************************************************************************/


#if	!defined(__ESLARRAY_H__)
#define	__ESLARRAY_H__	1


/****************************************************************************
                          �|�C���^�z��N���X
 ****************************************************************************/

class	EPtrArray	: public	ESLObject
{
protected:
	void **			m_ptrArray ;
	unsigned int	m_nLength ;
	unsigned int	m_nMaxSize ;
	unsigned int	m_nGrowAlign ;

public:
	// �\�z�֐�
	EPtrArray( void )
		: m_ptrArray( NULL ), m_nLength( 0 ),
			m_nMaxSize( 0 ), m_nGrowAlign( 0 ) { }
	EPtrArray
		( const EPtrArray & array,
			unsigned int nFirst = 0, unsigned int nCount = -1 ) ;
	// ���Ŋ֐�
	~EPtrArray( void )
		{
			if ( m_ptrArray != NULL )
				::eslHeapFree( NULL, m_ptrArray ) ;
		}
	// �N���X���
	DECLARE_CLASS_INFO( EPtrArray, ESLObject )

protected:
	// �������m��
	virtual void AlloBuffer( unsigned int nSize ) ;
	// �������J��
	virtual void FreeBuffer( void ) ;

public:
	// �z��ւ̃|�C���^���擾
	void ** const GetData( void ) const
		{
			return	m_ptrArray ;
		}
	// �z��̒������擾
	unsigned int GetSize( void ) const
		{
			return m_nLength ;
		}
	// �z��̓����o�b�t�@�̒������擾
	unsigned int GetLimit( void ) const
		{
			return	m_nMaxSize ;
		}
	// �z��̃T�C�Y��ݒ�
	void SetSize( unsigned int nSize, unsigned int nGrowAlign = 0 ) ;
	// �z��̓����o�b�t�@�̃T�C�Y��ݒ�
	void SetLimit( unsigned int nLimit, unsigned int nGrowAlign = 0 ) ;
	// �v�f���擾
	void * GetAt( unsigned int nIndex ) const
		{
			if ( nIndex < m_nLength )
				return	m_ptrArray[nIndex] ;
			else
				return	NULL ;
		}
	// �v�f��ݒ�
	void SetAt( unsigned int nIndex, void * ptrData )
		{
			if ( nIndex >= m_nLength )
				SetSize( nIndex + 1 ) ;
			m_ptrArray[nIndex] = ptrData ;
		}
	// �v�f��}��
	void InsertAt( unsigned int nIndex, void * ptrData ) ;
	// �w��͈̗͂v�f���폜
	void RemoveBetween( unsigned int nFirst, unsigned int nCount ) ;
	// �w��̗v�f���폜
	void RemoveAt( unsigned int nIndex )
		{
			RemoveBetween( nIndex, 1 ) ;
		}
	// �S�v�f���폜
	void RemoveAll( void ) ;
	// �w��̗v�f�����ւ���
	void Swap( unsigned int nIndex1, unsigned int nIndex2 ) ;
	// �z��̏I�[�ɗv�f��ǉ�
	unsigned int Add( void * ptrData )
		{
			unsigned int	nIndex = m_nLength ;
			SetAt( nIndex, ptrData ) ;
			return	nIndex ;
		}
	// �z�����������
	void Merge
		( int nIndex, const EPtrArray & array,
			unsigned int nFirst = 0, unsigned int nCount = -1 ) ;

public:
	// �X�^�b�N�փv�b�V��
	unsigned int Push( void * ptrData )
		{
			unsigned int	nIndex = m_nLength ;
			SetAt( nIndex, ptrData ) ;
			return	nIndex ;
		}
	// �X�^�b�N����|�b�v
	void * Pop( void )
		{
			if ( m_nLength > 0 )
				return	m_ptrArray[-- m_nLength] ;
			return	NULL ;
		}
	// �X�^�b�N��̔z��A�N�Z�X
	void * GetLastAt( unsigned int nIndex = 0 ) const
		{
			if ( nIndex < m_nLength )
				return	m_ptrArray[m_nLength - nIndex - 1] ;
			else
				return	NULL ;
		}

} ;


/****************************************************************************
                             ���l�z��N���X
 ****************************************************************************/

template <class T> class	ENumArray : public	EPtrArray
{
public:
	// �\�z�֐�
	ENumArray( void ) { }
	ENumArray
		( const ENumArray<T> & array,
			unsigned int nFirst = 0, unsigned int nCount = -1 )
		: EPtrArray( array, nFirst, nCount ) { }

public:
	// �w��v�f���擾
	T GetAt( unsigned int nIndex ) const
		{
			DWORD	n = (DWORD) EPtrArray::GetAt( nIndex ) ;
			return	*((T*)&n) ;
		}
	T operator [] ( unsigned int nIndex ) const
		{
			DWORD	n = (DWORD) EPtrArray::GetAt( nIndex ) ;
			return	*((T*)&n) ;
		}
	// �w��v�f�ɐݒ�
	void SetAt( unsigned int nIndex, T numData )
		{
			DWORD	n = *((DWORD*)&numData) ;
			EPtrArray::SetAt( nIndex, (void*) n ) ;
		}
	// �w��v�f�ɑ}��
	void InsertAt( unsigned int nIndex, T numData )
		{
			DWORD	n = *((DWORD*)&numData) ;
			EPtrArray::InsertAt( nIndex, (void*) n ) ;
		}
	// �z��̏I�[�ɒǉ�
	unsigned int Add( T numData )
		{
			DWORD	n = *((DWORD*)&numData) ;
			return	EPtrArray::Add( (void*) n ) ;
		}
	// �X�^�b�N�փv�b�V��
	unsigned int Push( T numData )
		{
			DWORD	n = *((DWORD*)&numData) ;
			return	EPtrArray::Push( (void*) n ) ;
		}
	// �X�^�b�N����|�b�v
	T Pop( void )
		{
			DWORD	n = (DWORD) EPtrArray::Pop() ;
			return	*((T*)&n) ;
		}
	// �X�^�b�N��̔z��A�N�Z�X
	T GetLastAt( unsigned int nIndex = 0 ) const
		{
			DWORD	n = (DWORD) EPtrArray::GetLastAt( nIndex ) ;
			return	*((T*)&n) ;
		}

} ;


/****************************************************************************
                      �I�u�W�F�N�g�Q�Ɣz��N���X
 ****************************************************************************/

template <class T> class	EPtrObjArray : public	EPtrArray
{
public:
	// �\�z�֐�
	EPtrObjArray( void ) { }
	EPtrObjArray
		( const EPtrObjArray<T> & array,
			unsigned int nFirst = 0, unsigned int nCount = -1 )
		: EPtrArray( array, nFirst, nCount ) { }

public:
	// �z��ւ̃|�C���^���擾
	T ** const GetData( void ) const
		{
			return	(T**) m_ptrArray ;
		}
	// �w��v�f���擾
	T * GetAt( unsigned int nIndex ) const
		{
			return	(T*) EPtrArray::GetAt( nIndex ) ;
		}
	T & operator [] ( unsigned int nIndex ) const
		{
			T *	ptrObj = (T*) EPtrArray::GetAt( nIndex ) ;
			ESLAssert( ptrObj != NULL ) ;
			return	*ptrObj ;
		}
	// �w��v�f�ɐݒ�
	void SetAt( unsigned int nIndex, T * ptrObj )
		{
			EPtrArray::SetAt( nIndex, (void*) ptrObj ) ;
		}
	// �w��v�f�ɑ}��
	void InsertAt( unsigned int nIndex, T * ptrObj )
		{
			EPtrArray::InsertAt( nIndex, (void*) ptrObj ) ;
		}
	// �z��̏I�[�ɒǉ�
	unsigned int Add( T * ptrObj )
		{
			return	EPtrArray::Add( (void*) ptrObj ) ;
		}
	// �X�^�b�N�փv�b�V��
	unsigned int Push( T * ptrObj )
		{
			return	EPtrArray::Push( (void*) ptrObj ) ;
		}
	// �X�^�b�N����|�b�v
	T * Pop( void )
		{
			return	(T*) EPtrArray::Pop( ) ;
		}
	// �X�^�b�N��̔z��A�N�Z�X
	T * GetLastAt( unsigned int nIndex = 0 ) const
		{
			return	(T*) EPtrArray::GetLastAt( nIndex ) ;
		}
	// ��̗v�f���폜
	void TrimEmpty( void )
		{
			unsigned int	i = 0, j = 0 ;
			while ( i < m_nLength )
			{
				if ( m_ptrArray[i] != NULL )
				{
					m_ptrArray[j ++] = m_ptrArray[i] ;
				}
				i ++ ;
			}
			m_nLength = j ;
		}

} ;


/****************************************************************************
                      �I�u�W�F�N�g�z��N���X
 ****************************************************************************/

template <class T> class	EObjArray : public	EPtrObjArray<T>
{
public:
	// �\�z�֐�
	EObjArray( void ) { }
	EObjArray
		( EObjArray<T> & array,
			unsigned int nFirst = 0, unsigned int nCount = -1 )
		: EPtrObjArray<T>( (const EPtrObjArray<T> &) array, nFirst, nCount )
		{
			if ( nCount == -1 )
			{
				nCount = array.m_nLength ;
			}
			((EPtrArray&)array).RemoveBetween( nFirst, nCount ) ;
		}
	// ���Ŋ֐�
	~EObjArray( void )
		{
			RemoveAll( ) ;
		}

public:
	// �w��v�f�ɐݒ�
	void SetAt( unsigned int nIndex, T * ptrData )
		{
			delete	GetAt( nIndex ) ;
			EPtrObjArray<T>::SetAt( nIndex, ptrData ) ;
		}
	T & operator [] ( unsigned int nIndex ) const
		{
			T *	ptrObj = (T*) EPtrArray::GetAt( nIndex ) ;
			ESLAssert( ptrObj != NULL ) ;
			return	*ptrObj ;
		}
	T & operator [] ( unsigned int nIndex )
		{
			T *	ptrObj = (T*) EPtrArray::GetAt( nIndex ) ;
			if ( ptrObj == NULL )
			{
				ptrObj = new T ;
				EPtrArray::SetAt( nIndex, ptrObj ) ;
			}
			return	*ptrObj ;
		}
	// �z��̃T�C�Y��ݒ�
	void SetSize( unsigned int nSize, unsigned int nGrowAlign = 0 )
		{
			for ( unsigned int i = GetSize(); i > nSize; i -- )
				SetAt( i - 1, NULL ) ;
			EPtrObjArray<T>::SetSize( nSize, nGrowAlign ) ;
		}
	// �w���ԍ폜
	void RemoveBetween( unsigned int nFirst, unsigned int nCount )
		{
			for ( int i = nFirst + nCount - 1; i >= (int) nFirst;
					i = (i >= (int) m_nLength) ? (m_nLength - 1) : (i - 1) )
				delete	GetAt( i ) ;
			EPtrObjArray<T>::RemoveBetween( nFirst, nCount ) ;
		}
	// �w��v�f���폜
	void RemoveAt( unsigned int nIndex )
		{
			RemoveBetween( nIndex, 1 ) ;
		}
	// �S�Ă̗v�f���폜
	void RemoveAll( void )
		{
			for ( int i = GetSize() - 1; i >= 0;
					i = (i >= (int) m_nLength) ? (m_nLength - 1) : (i - 1) )
				delete	GetAt( i ) ;
			EPtrObjArray<T>::RemoveAll( ) ;
		}
	// �w���Ԃ𕪗�
	void DetachBetween( unsigned int nFirst, unsigned int nCount )
		{
			EPtrObjArray<T>::RemoveBetween( nFirst, nCount ) ;
		}
	// �w��v�f�𕪗�
	void DetachAt( unsigned int nIndex )
		{
			EPtrObjArray<T>::RemoveAt( nIndex ) ;
		}
	// �S�Ă̗v�f�𕪗�
	void DetachAll( void )
		{
			EPtrObjArray<T>::RemoveAll( ) ;
		}
	// �z�����������
	void Merge
		( int nIndex, EObjArray<T> & array,
			unsigned int nFirst = 0, unsigned int nCount = -1 )
		{
			EPtrObjArray<T>::Merge( nIndex, array, nFirst, nCount ) ;
			array.DetachBetween( nFirst, nCount ) ;
		}

} ;


/****************************************************************************
                          �A�z�z��p�R���e�i
 ****************************************************************************/

template <class TagType, class ObjType> class	ETaggedElement
{
private:
	TagType		m_tag ;
	ObjType *	m_obj ;

public:
	// �\�z�֐�
	ETaggedElement( void )
		: m_obj( NULL ) { }
	ETaggedElement( TagType tag, ObjType * obj )
		: m_tag( tag ), m_obj( obj ) { }
	// ���Ŋ֐�
	~ETaggedElement( void )
		{
			delete	m_obj ;
		}
	// �^�O���擾
	TagType & Tag( void )
		{
			return	m_tag ;
		}
	// �I�u�W�F�N�g���擾
	ObjType * GetObject( void ) const
		{
			return	m_obj ;
		}
	// �I�u�W�F�N�g��ݒ�
	void SetObject( ObjType * obj )
		{
			delete	m_obj ;
			m_obj = obj ;
		}
	// �I�u�W�F�N�g�𕪗�
	ObjType * DetachObject( void )
		{
			ObjType *	obj = m_obj ;
			m_obj = NULL ;
			return	obj ;
		}

} ;


/****************************************************************************
                     �\�[�g�ɂ��A�z�z��N���X
 ****************************************************************************/

template < class TagType, class ObjType >	class	ETagSortArray
			: public	EObjArray< ETaggedElement<TagType,ObjType> >
{
public:
	// �\�z�֐�
	ETagSortArray( void ) { }
	// �v�f����
	ObjType * GetAs
		( TagType tag, unsigned int * pIndex = NULL ) const
		{
			int		iFirst, iEnd, iMiddle ;
			ETaggedElement<TagType,ObjType> *	pElement ;
			iFirst = 0 ;
			iEnd = GetSize() - 1 ;
			//
			while ( iFirst <= iEnd )
			{
				iMiddle = ((iFirst + iEnd) >> 1) ;
				pElement = GetAt( iMiddle ) ;
				ESLAssert( pElement != NULL ) ;
				//
				if ( tag < pElement->Tag() )
				{
					iEnd = iMiddle - 1 ;
				}
				else if ( tag > pElement->Tag() )
				{
					iFirst = iMiddle + 1 ;
				}
				else
				{
					if ( pIndex != NULL )
					{
						*pIndex = iMiddle ;
					}
					return	pElement->GetObject( ) ;
				}
			}
			//
			return	NULL ;
		}
	// �v�f�ǉ��i����v�f���������ꍇ�A�㏑������j
	unsigned int SetAs( TagType tag, ObjType * obj )
		{
			int		iFirst, iEnd, iMiddle = 0 ;
			ETaggedElement<TagType,ObjType> *	pElement ;
			iFirst = 0 ;
			iEnd = GetSize() - 1 ;
			//
			while ( iFirst <= iEnd )
			{
				iMiddle = ((iFirst + iEnd) >> 1) ;
				pElement = GetAt( iMiddle ) ;
				ESLAssert( pElement != NULL ) ;
				//
				if ( tag < pElement->Tag() )
				{
					iEnd = iMiddle - 1 ;
				}
				else if ( tag > pElement->Tag() )
				{
					iFirst = iMiddle + 1 ;
				}
				else
				{
					pElement->SetObject( obj ) ;
					return	iMiddle ;
				}
			}
			//
			pElement = new ETaggedElement<TagType,ObjType>( tag, obj ) ;
			InsertAt( iFirst, pElement ) ;
			//
			return	iFirst ;
		}
	// �v�f�ǉ��i����v�f���������ꍇ�A���������Ȃ��j
	unsigned int Add( TagType tag, ObjType * obj )
		{
			int		iFirst, iEnd, iMiddle = 0 ;
			ETaggedElement<TagType,ObjType> *	pElement ;
			iFirst = 0 ;
			iEnd = GetSize() - 1 ;
			//
			while ( iFirst <= iEnd )
			{
				iMiddle = ((iFirst + iEnd) >> 1) ;
				pElement = GetAt( iMiddle ) ;
				ESLAssert( pElement != NULL ) ;
				//
				if ( tag < pElement->Tag() )
				{
					iEnd = iMiddle - 1 ;
				}
				else
				{
					iFirst = iMiddle + 1 ;
				}
			}
			//
			pElement = new ETaggedElement<TagType,ObjType>( tag, obj ) ;
			InsertAt( iFirst, pElement ) ;
			//
			return	iFirst ;
		}
	// �v�f�t����
	ETaggedElement<TagType,ObjType> *
			SearchAs( ObjType * obj, unsigned int * pIndex = NULL )
		{
			ETaggedElement<TagType,ObjType> *	pElement ;
			for ( unsigned int i = 0; i < GetSize(); i ++ )
			{
				pElement = GetAt( i ) ;
				ESLAssert( pElement != NULL ) ;
				if ( pElement == NULL )
					continue ;
				//
				if ( pElement->GetObject() == obj )
				{
					if ( pIndex != NULL )
						*pIndex = i ;
					return	pElement ;
				}
			}
			return	NULL ;
		}
	// �v�f�폜
	void RemoveAs( TagType tag )
		{
			unsigned int	index = -1 ;
			GetAs( tag, &index ) ;
			if ( index != (unsigned int) -1 )
			{
				RemoveAt( index ) ;
			}
		}
	// �v�f����
	void DetachAs( TagType tag )
		{
			unsigned int	index ;
			if ( GetAs( tag, &index ) != NULL )
			{
				ETaggedElement<TagType,ObjType> *
							pElement = GetAt( index ) ;
				ESLAssert( pElement != NULL ) ;
				pElement->DetachObject( ) ;
				RemoveAt( index ) ;
			}
		}
	// �S�Ă̗v�f�𕪗�
	void DetachAll( void )
		{
			for ( unsigned int i = 0; i < GetSize(); i ++ )
			{
				ETaggedElement<TagType,ObjType> *
							pElement = GetAt( i ) ;
				pElement->DetachObject( ) ;
			}
			RemoveAll( ) ;
		}
	// �v�f�A�N�Z�X
	ObjType * GetObjectAt( unsigned int nIndex ) const
		{
			ETaggedElement<TagType,ObjType> * pElement = GetAt( nIndex ) ;
			if ( pElement == NULL )
			{
				return	NULL ;
			}
			return	pElement->GetObject( ) ;
		}
	const ObjType & operator [] ( TagType tag ) const
		{
			const ObjType *	ptrObj = GetAs( tag, NULL ) ;
			ESLAssert( ptrObj != NULL ) ;
			return	*ptrObj ;
		}
	ObjType & operator [] ( TagType tag )
		{
			ObjType *	ptrObj = GetAs( tag, NULL ) ;
			if ( ptrObj == NULL )
			{
				ptrObj = new ObjType ;
				Add( tag, ptrObj ) ;
			}
			return	*ptrObj ;
		}

} ;


//////////////////////////////////////////////////////////////////////////////
// ���l�ɂ��A�z�z��
//////////////////////////////////////////////////////////////////////////////

template <class T>	class	EIntTagArray
					: public	ETagSortArray<int,T>
{
public:
	// �\�z�֐�
	EIntTagArray( void ) { }

} ;


//////////////////////////////////////////////////////////////////////////////
// ������ɂ��A�z�z��
//////////////////////////////////////////////////////////////////////////////

template <class T>	class	EStrTagArray
					: public	ETagSortArray<EString,T>
{
public:
	// �\�z�֐�
	EStrTagArray( void ) { }

} ;

template <class T>	class	EWStrTagArray
					: public	ETagSortArray<EWideString,T>
{
public:
	// �\�z�֐�
	EWStrTagArray( void ) { }

} ;


#endif
