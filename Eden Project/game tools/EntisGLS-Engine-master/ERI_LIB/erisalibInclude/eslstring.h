
/*****************************************************************************
                   Entis Standard Library declarations
 ----------------------------------------------------------------------------

	In this file, the strings classes declarations.

	Copyright (C) 1998-2004 Leshade Entis. All rights reserved.

 ****************************************************************************/


#if	!defined(__ESLSTRING_H__)
#define	__ESLSTRING_H__	1


/*****************************************************************************
                         ������N���X�e���v���[�g
 ****************************************************************************/

template <class _Type, class _Obj>	class	EGenString	: public	ESLObject
{
protected:
	// �f�[�^�����o
	_Type *			m_pszString ;
	unsigned int	m_nLength ;
	unsigned int	m_nBufLimit ;

public:
	// �\�z�֐�
	EGenString( void )
		: m_pszString( NULL ), m_nLength( 0 ), m_nBufLimit( 0 ) { }
	EGenString( _Type chrCode )
		: m_pszString( NULL ), m_nLength( 1 )
		{
			AllocString( 0x0F ) ;
			m_pszString[0] = chrCode ;
			m_pszString[1] = 0 ;
		}
	EGenString( const EGenString<_Type,_Obj> & strObject )
		:	m_pszString( NULL ), m_nLength( strObject.m_nLength )
		{
			AllocString( strObject.m_nBufLimit ) ;
			unsigned int	i ;
			for ( i = 0; i < m_nLength; i ++ )
				m_pszString[i] = strObject.m_pszString[i] ;
			m_pszString[i] = 0 ;
		}
	EGenString( const _Type * pszString, int nLength )
		: m_pszString( NULL ), m_nLength( nLength ), m_nBufLimit( 0 )
		{
			if ( nLength < 0 )
			{
				nLength = 0 ;
				if ( pszString != NULL )
				{
					while ( pszString[nLength] )
						nLength ++ ;
				}
				m_nLength = nLength ;
			}
			if ( nLength > 0 )
			{
				AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
				unsigned int	i ;
				for ( i = 0; i < m_nLength; i ++ )
					m_pszString[i] = pszString[i] ;
				m_pszString[i] = 0 ;
			}
		}
	EGenString( const _Type * pszString )
		: m_pszString( NULL ), m_nBufLimit( 0 )
		{
			m_nLength = 0 ;
			if ( pszString != NULL )
			{
				while( pszString[m_nLength] )
					m_nLength ++ ;
				AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
				unsigned int	i ;
				for ( i = 0; i < m_nLength; i ++ )
					m_pszString[i] = pszString[i] ;
				m_pszString[i] = 0 ;
			}
		}
	~EGenString( void )
		{
			if ( m_pszString != NULL )
				FreeString( ) ;
		}

public:
	// �������A���P�[�V����
	virtual void AllocString( unsigned int nLength )
		{
			if ( m_pszString != NULL )
			{
				m_pszString = (_Type*)
					::eslHeapReallocate
						( NULL, m_pszString, ((nLength + 1) * sizeof(_Type)), 0 ) ;
			}
			else
			{
				m_pszString = (_Type*)
					::eslHeapAllocate( NULL, ((nLength + 1) * sizeof(_Type)), 0 ) ;
				m_pszString[0] = '\0' ;
			}
			m_nBufLimit = nLength ;
		}
	virtual void FreeString( void )
		{
			if ( m_pszString != NULL )
			{
				::eslHeapFree( NULL, m_pszString ) ;
				m_pszString = NULL ;
				m_nLength = 0 ;
				m_nBufLimit = 0 ;
			}
		}

public:
	// �����擾
	unsigned int GetLength( void ) const
		{
			return	m_nLength ;
		}
	bool IsEmpty( void ) const
		{
			return	(m_nLength == 0) ;
		}

public:
	// ������擾
	_Type GetAt( int nIndex ) const
		{
			if ( (m_pszString != NULL) &&
					((unsigned int) nIndex < m_nLength) )
				return	m_pszString[nIndex] ;
			else
				return	(_Type) 0 ;
		}
	void SetAt( int nIndex, _Type chrCode )
		{
			ESLAssert( (m_pszString != NULL) && ((unsigned int) nIndex < m_nLength) ) ;
			m_pszString[nIndex] = chrCode ;
		}
	_Type operator [] ( int nIndex ) const
		{
			if ( (m_pszString != NULL) &&
					((unsigned int) nIndex < m_nLength) )
				return	m_pszString[nIndex] ;
			else
				return	(_Type) 0 ;
		}
	_Type & operator [] ( int nIndex )
		{
			ESLAssert( (unsigned int) nIndex < m_nLength ) ;
			return	m_pszString[nIndex] ;
		}
	operator const _Type * ( void ) const
		{
			return	m_pszString ;
		}
	const _Type * CharPtr( void ) const
		{
			return	m_pszString ;
		}

public:
	// �������r
	int Compare( const _Type * pszString ) const
		{
			if ( m_nLength == 0 )
				return	((pszString != NULL) ? - (int) pszString[0] : 0) ;
			else if ( pszString == NULL )
				return	(int) m_pszString[0] ;
			unsigned int	i ;
			for ( i = 0; i < m_nLength; i ++ )
			{
				int	nCompare = (int) m_pszString[i] - (int) pszString[i] ;
				if ( nCompare != 0 )
					return	nCompare ;
			}
			return	 - (int) pszString[i] ;
		}
	int CompareNoCase( const _Type * pszString ) const
		{
			if ( m_nLength == 0 )
				return	((pszString != NULL) ? - (int) pszString[0] : 0) ;
			else if ( pszString == NULL )
				return	(int) m_pszString[0] ;
			unsigned int	i ;
			for ( i = 0; i < m_nLength; i ++ )
			{
				_Type	c1 = m_pszString[i] ;
				_Type	c2 = pszString[i] ;
				if ( c1 != c2 )
				{
					if ( ('a' <= c1) && (c1 <= 'z') )
						c1 += ('A' - 'a') ;
					if ( ('a' <= c2) && (c2 <= 'z') )
						c2 += ('A' - 'a') ;
					int	nCompare = (int) c1 - (int) c2 ;
					if ( nCompare != 0 )
					{
						return	nCompare ;
					}
				}
			}
			return	 - (int) pszString[i] ;
		}
	bool operator == ( const _Type * pszString ) const
		{
			return	!Compare( pszString ) ;
		}
	bool operator != ( const _Type * pszString ) const
		{
			return	(Compare( pszString ) != 0) ;
		}
	bool operator < ( const _Type * pszString ) const
		{
			return	(Compare(pszString) < 0) ;
		}
	bool operator > ( const _Type * pszString ) const
		{
			return	(Compare(pszString) > 0) ;
		}
	bool operator <= ( const _Type * pszString ) const
		{
			return	(Compare(pszString) <= 0) ;
		}
	bool operator >= ( const _Type * pszString ) const
		{
			return	(Compare(pszString) >= 0) ;
		}

public:
	// �����񑀍�i�������o�j
	_Obj Middle
		( unsigned int nFirst, unsigned int nCount = -1 ) const
		{
			if ( m_pszString == NULL )
				return	_Obj( ) ;
			if ( nCount == -1 )
			{
				if ( nFirst < m_nLength )
					nCount = m_nLength - nFirst ;
			}
			if ( m_nLength < (nFirst + nCount) )
			{
				if ( m_nLength <= nFirst )
					return	_Obj( ) ;
				nCount = (m_nLength - nFirst) ;
			}
			return	_Obj( (m_pszString + nFirst), nCount ) ;
		}
	_Obj Left( unsigned int nCount ) const
		{
			if ( m_pszString == NULL )
				return	_Obj( ) ;
			if ( m_nLength < nCount )
				return	_Obj( m_pszString, m_nLength ) ;
			else
				return	_Obj( m_pszString, nCount ) ;
		}
	_Obj Right( unsigned int nCount ) const
		{
			if ( m_pszString == NULL )
				return	_Obj( ) ;
			if ( m_nLength < nCount )
				return	_Obj( m_pszString, m_nLength ) ;
			else
				return	_Obj( (m_pszString + m_nLength - nCount), nCount ) ;
		}
	// ������u������
	_Obj Replace( const _Type * pszOld, const _Type * pszNew )
		{
			_Obj	strReplaced ;
			strReplaced.AllocString( m_nLength + 1 ) ;
			unsigned int	iLast = 0 ;
			int				cDummy = 0 ;
			if ( pszNew == NULL )
			{
				pszNew = (const _Type *) &cDummy ;
			}
			for ( unsigned int i = 0; i < m_nLength; i ++ )
			{
				unsigned int	j ;
				for ( j = 0; pszOld[j] && (m_pszString[i+j] == pszOld[j]); j ++ ) ;
				if ( pszOld[j] == 0 )
				{
					strReplaced +=
						EGenString<_Type,_Obj>
							( m_pszString + iLast, i - iLast ) ;
					strReplaced += pszNew ;
					iLast = i + j ;
					i = iLast - 1 ;
				}
			}
			if ( iLast < m_nLength )
			{
				strReplaced +=
					EGenString<_Type,_Obj>
						( m_pszString + iLast, m_nLength - iLast ) ;
			}
			return	strReplaced ;
		}

public:
	// �t�@�C�����iURL�j����
	const _Type * GetFileNamePart( void ) const
		{
			unsigned int	l = 0 ;
			if ( m_pszString == NULL )
			{
				((EGenString<_Type,_Obj>*)this)->AllocString( 0x10 ) ;
			}
			for ( unsigned int i = 0; i < m_nLength; i ++ )
			{
				_Type	c = m_pszString[i] ;
				if ( (c == '\\') || (c == '/') )
				{
					l = i + 1 ;
				}
			}
			return	m_pszString + l ;
		}
	const _Type * GetFileExtensionPart( void ) const
		{
			const _Type *	pszFileName = GetFileNamePart( ) ;
			for ( int i = 0; pszFileName[i]; i ++ )
			{
				if ( pszFileName[i] == '.' )
				{
					return	pszFileName + i + 1 ;
				}
			}
			return	m_pszString + m_nLength ;
		}
	_Obj GetFileTitlePart( void ) const
		{
			const _Type *	pszFileName = GetFileNamePart( ) ;
			for ( int i = 0; pszFileName[i]; i ++ )
			{
				if ( pszFileName[i] == '.' )
				{
					return	_Obj( pszFileName, i ) ;
				}
			}
			return	pszFileName ;
		}
	_Obj GetFileDirectoryPart( void ) const
		{
			unsigned int	l = 0 ;
			for ( unsigned int i = 0; i < m_nLength; i ++ )
			{
				_Type	c = m_pszString[i] ;
				if ( (c == '\\') || (c == '/') )
				{
					l = i + 1 ;
				}
			}
			return	_Obj( m_pszString, l ) ;
		}
	// ���l�ϊ�
	long int AsInteger( bool fSign = true, bool * pError = NULL )
		{
			long int	nValue = 0x80000000 ;
			long int	nSign = 0 ;
			bool		fErr = true ;
			for ( unsigned int i = 0; i < m_nLength; i ++ )
			{
				unsigned int	c = m_pszString[i] ;
				if ( (c >= '0') & (c <= '9') )
				{
					nValue = nValue * 10 + (c - '0') ;
					fErr = false ;
				}
				else if ( fSign && ((c == '+') | (c == '-')) )
				{
					fSign = false ;
					nSign = - (int) (c == '-') ;
				}
				else if ( c > (unsigned int) ' ' )
				{
					break ;
				}
			}
			if ( pError != NULL )
			{
				*pError = (fErr || (m_nLength == 0)) ;
			}
			return	(nValue ^ nSign) - nSign ;
		}
	// �A���t�@�x�b�g��啶���ɕϊ�
	void MakeUpper( void )
		{
			for ( unsigned int i = 0; i < m_nLength; i ++ )
			{
				_Type	c = m_pszString[i] ;
				if ( ('a' <= c) && (c <= 'z') )
				{
					m_pszString[i] = c - ('a' - 'A') ;
				}
			}
		}
	// �A���t�@�x�b�g���������ɕϊ�
	void MakeLower( void )
		{
			for ( unsigned int i = 0; i < m_nLength; i ++ )
			{
				_Type	c = m_pszString[i] ;
				if ( ('A' <= c) && (c <= 'Z') )
				{
					m_pszString[i] = c + ('a' - 'A') ;
				}
			}
		}
	// ������̍��E���]
	void MakeReverse( void )
		{
			int	nLength = m_nLength ;
			int	nHalfLength = nLength / 2 ;
			nLength -- ;
			//
			for ( int i = 0; i < nHalfLength; i ++ )
			{
				wchar_t	wch = m_pszString[i] ;
				m_pszString[i] = m_pszString[nLength - i] ;
				m_pszString[nLength - i] = wch ;
			}
		}

public:
	// ��������
	int Find( _Type chrCode, int nIndex = 0 ) const
		{
			while ( (unsigned int) nIndex < m_nLength )
			{
				if ( m_pszString[nIndex] == chrCode )
				{
					return	nIndex ;
				}
				nIndex ++ ;
			}
			return	-1 ;
		}
	int Find( const _Type * pszString, int nIndex = 0 ) const
		{
			for ( unsigned int i = nIndex; i < m_nLength; i ++ )
			{
				unsigned int	j ;
				for ( j = 0; pszString[j]; j ++ )
				{
					if ( m_pszString[i + j] != pszString[j] )
						break ;
				}
				if ( pszString[j] == 0 )
				{
					return	i ;
				}
			}
			return	-1 ;
		}

public:
	// �󔒏���
	void TrimLeft( void )
		{
			if ( m_pszString != NULL )
			{
				unsigned int	i ;
				for ( i = 0; i < m_nLength; i ++ )
				{
					if ( m_pszString[i] > ' ' )
					{
						if ( i > 0 )
						{
							m_nLength -= i ;
							unsigned int	j ;
							for( j = 0; j < m_nLength; j ++ )
							{
								m_pszString[j] = m_pszString[i + j] ;
							}
							m_pszString[j] = 0 ;
						}
						return ;
					}
				}
				if ( i >= m_nLength )
				{
					m_nLength = 0 ;
					m_pszString[0] = 0 ;
				}
			}
		}
	void TrimRight( void )
		{
			if ( m_pszString != NULL )
			{
				int	i ;
				for ( i = (m_nLength - 1); i >= 0; i -- )
				{
					if ( m_pszString[i] > ' ' )
					{
						m_nLength = (++ i) ;
						m_pszString[i] = 0 ;
						return ;
					}
				}
				if ( i < 0 )
				{
					m_nLength = 0 ;
					m_pszString[0] = 0 ;
				}
			}
		}

public:
	// �������
	const EGenString<_Type,_Obj> & operator = ( _Type chrCode )
		{
			m_nLength = 1 ;
			AllocString( 0x0F ) ;
			m_pszString[0] = chrCode ;
			m_pszString[1] = 0 ;
			return	*this ;
		}
	const EGenString<_Type,_Obj> & operator =
			( const EGenString<_Type,_Obj> & strObject )
		{
			if ( this == &strObject )
				return	*this ;
			m_nLength = strObject.m_nLength ;
			if ( m_nLength == 0 )
			{
				if ( m_pszString != NULL )
					m_pszString[0] = 0 ;
			}
			else
			{
				AllocString( strObject.m_nBufLimit ) ;
				unsigned int	i ;
				for ( i = 0; i < m_nLength; i ++ )
					m_pszString[i] = strObject.m_pszString[i] ;
				m_pszString[i] = 0 ;
			}
			return	*this ;
		}
	const EGenString<_Type,_Obj> & operator = ( const _Type * pszString )
		{
			if ( pszString == m_pszString )
				return	*this ;
			if ( pszString == NULL )
			{
				if ( m_nLength == 0 )
					return	*this ;
				m_nLength = 0 ;
			}
			else
				for ( m_nLength = 0; pszString[m_nLength]; m_nLength ++ ) ;
			AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
			unsigned int	i ;
			for ( i = 0; i < m_nLength; i ++ )
				m_pszString[i] = pszString[i] ;
			m_pszString[i] = 0 ;
			return	*this ;
		}

public:
	// �����񌋍�
	const EGenString<_Type,_Obj> & operator +=
			( const EGenString<_Type,_Obj> & strObject )
		{
			unsigned int	nLength = m_nLength + strObject.m_nLength ;
			if ( nLength >= m_nBufLimit )
				AllocString( nLength + (nLength >> 1) + 1 ) ;
			for ( unsigned int i = 0; i < strObject.m_nLength; i ++ )
				m_pszString[m_nLength + i] = strObject.m_pszString[i] ;
			m_pszString[(m_nLength = nLength)] = 0 ;
			return	*this ;
		}
	const EGenString<_Type,_Obj> & operator += ( _Type chrCode )
		{
			if ( (++ m_nLength) >= m_nBufLimit )
				AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
			m_pszString[m_nLength - 1] = chrCode ;
			m_pszString[m_nLength] = 0 ;
			return	*this ;
		}
	const EGenString<_Type,_Obj> & operator += ( const _Type * ptrString )
		{
			int	nLength = 0 ;
			if ( ptrString != NULL )
			{
				while( ptrString[nLength] )
					nLength ++ ;
			}
			unsigned int	nBase = m_nLength ;
			m_nLength += nLength ;
			if( m_nLength >= m_nBufLimit )
				AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
			for( int i = 0; i < nLength; i ++ )
				m_pszString[nBase + i] = ptrString[i] ;
			m_pszString[m_nLength] = 0 ;
			return	*this ;
		}
	_Obj operator + ( const EGenString<_Type,_Obj> & strObject ) const
		{
			_Obj	objString( *this ) ;
			objString += strObject ;
			return	objString ;
		}
	_Obj operator + ( _Type chrCode ) const
		{
			_Obj	strObject( *this ) ;
			strObject += chrCode ;
			return	strObject ;
		}
	_Obj operator + ( const _Type * pszString ) const
		{
			_Obj	strObject( *this ) ;
			strObject += pszString ;
			return	strObject ;
		}
	friend _Obj operator +
		( const _Type * pszString, const EGenString<_Type,_Obj> & strObject )
		{
			_Obj	objString( pszString ) ;
			objString += strObject ;
			return	objString ;
		}
	_Obj operator * ( int nRepeatCount ) const
		{
			_Obj	objString ;
			_Obj	objSource = *this ;
			if ( nRepeatCount < 0 )
			{
				nRepeatCount = - nRepeatCount ;
				objSource.MakeReverse( ) ;
			}
			objString.AllocString( m_nLength * nRepeatCount ) ;
			for ( int i = 0; i < nRepeatCount; i ++ )
				objString += objSource ;
			return	objString ;
		}

public:
	// �����V�[�P���X�ւ̃A�N�Z�X
	_Type * GetBuffer( unsigned int nBufferSize )
		{
			if ( nBufferSize >= m_nBufLimit )
				AllocString( nBufferSize + 1 ) ;
			return	m_pszString ;
		}
	void ReleaseBuffer( int nLength = -1 )
		{
			if ( nLength < 0 )
			{
				unsigned int	i ;
				for ( i = 0; m_pszString[i] && (i < m_nBufLimit); i ++ ) ;
				m_nLength = i - (int) (i >= m_nBufLimit) ;
				m_pszString[m_nLength] = 0 ;
			}
			else
			{
				ESLAssert( (unsigned int) nLength < m_nBufLimit ) ;
				m_pszString[nLength] = 0 ;
				m_nLength = nLength ;
			}
		}

} ;


/*****************************************************************************
                       ������N���X�̃C���X�^���X��
 ****************************************************************************/

class	EString : public	EGenString<char,EString>
{
public:
	// �\�z�֐�
	EString( void )
		: EGenString<char,EString>( ) { }
	EString( char chrCode )
		: EGenString<char,EString>( chrCode ) { }
	EString( const EGenString<char,EString> & strObject )
		: EGenString<char,EString>( strObject ) { }
	EString( const char * pszString, unsigned int nLength )
		: EGenString<char,EString>( pszString, nLength ) { }
	EString( const char * pszString )
		: EGenString<char,EString>( pszString ) { }
	EString( const wchar_t * pwszString, unsigned int nLength = -1 ) ;
	EString( int nValue, int nPrec = 0 ) ;
	EString( DWORD dwValue, int nPrec = 0 ) ;
	EString( double rValue, int nPrec = 0 ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EString, ESLObject )

public:
	// �������r
	int Compare( const char * pszString ) const ;
	int CompareNoCase( const char * pszString ) const ;
	static int Compare( const char * pstr1, const char * pstr2 ) ;
	static int CompareNoCase( const char * pstr1, const char * pstr2 ) ;

	// �������
	const EString & operator = ( char chrCode )
		{
			EGenString<char,EString>::operator = ( chrCode ) ;
			return	*this ;
		}
	const EString & operator = ( const char * pszString )
		{
			EGenString<char,EString>::operator = ( pszString ) ;
			return	*this ;
		}
	const EString & operator = ( const EGenString<char,EString> & strObject )
		{
			EGenString<char,EString>::operator = ( strObject ) ;
			return	*this ;
		}
	const EString & operator = ( const EString & strObject )
		{
			EGenString<char,EString>::operator = ( strObject ) ;
			return	*this ;
		}
	const EString & operator = ( const wchar_t * pwszString ) ;

	// �t�@�C�����iURL�j����
	const char * GetFileNamePart( void ) const ;
	const char * GetFileExtensionPart( void ) const ;
	EString GetFileTitlePart( void ) const ;
	EString GetFileDirectoryPart( void ) const ;

	// �A���t�@�x�b�g��啶���ɕϊ�
	void MakeUpper( void ) ;
	// �A���t�@�x�b�g���������ɕϊ�
	void MakeLower( void ) ;
	// ������̍��E���]
	void MakeReverse( void ) ;

public:
	// ��������
	int Find( char chrCode, int nIndex = 0 ) const ;
	int Find( const char * pszString, int nIndex = 0 ) const ;

public:
	// �󔒏���
	void TrimLeft( void ) ;
	void TrimRight( void ) ;

} ;


#if	!defined(COMPACT_NOA_DECODER)

class	EWideString : public	EGenString<wchar_t,EWideString>
{
public:
	// �\�z�֐�
	EWideString( void )
		: EGenString<wchar_t,EWideString>( ) { }
	EWideString( wchar_t wchCode )
		: EGenString<wchar_t,EWideString>( wchCode ) { }
	EWideString( const EGenString<wchar_t,EWideString> & strObject )
		: EGenString<wchar_t,EWideString>( strObject ) { }
	EWideString( const wchar_t * pwszString, unsigned int nLength )
		: EGenString<wchar_t,EWideString>( pwszString, nLength ) { }
	EWideString( const wchar_t * pwszString )
		: EGenString<wchar_t,EWideString>( pwszString ) { }
	EWideString( const char * pszString, unsigned int nLength = -1 ) ;
	EWideString( int nValue, int nPrec = 0 ) ;
	EWideString( DWORD dwValue, int nPrec = 0 ) ;
	EWideString( double rValue, int nPrec = 0 ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EWideString, ESLObject )

public:
	// �������
	const EWideString & operator = ( wchar_t wchCode )
		{
			EGenString<wchar_t,EWideString>::operator = ( wchCode ) ;
			return	*this ;
		}
	const EWideString & operator = ( const wchar_t * pwszString )
		{
			EGenString<wchar_t,EWideString>::operator = ( pwszString ) ;
			return	*this ;
		}
	const EWideString & operator = ( const EGenString<wchar_t,EWideString> & strObject )
		{
			EGenString<wchar_t,EWideString>::operator = ( strObject ) ;
			return	*this ;
		}
	const EWideString & operator = ( const EWideString & strObject )
		{
			EGenString<wchar_t,EWideString>::operator = ( strObject ) ;
			return	*this ;
		}
	const EWideString & operator = ( const char * pszString ) ;

} ;


/*****************************************************************************
                        �\����͗p������N���X
 ****************************************************************************/

class	EStreamWideString : public	EWideString
{
public:
	// �\�z�֐�
	EStreamWideString( void ) ;
	EStreamWideString( const EStreamWideString & swidestr ) ;
	EStreamWideString( const EGenString<wchar_t,EWideString> & widestr ) ;
	EStreamWideString( const wchar_t * pwszString, unsigned int nLength ) ;
	EStreamWideString( const wchar_t * pwszString ) ;
	EStreamWideString( const char * pszString, unsigned int nLength = -1 ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EStreamWideString, EWideString )

public:
	// ���Ŋ֐�
	virtual ~EStreamWideString( void ) ;
	virtual void DeleteContents( void ) ;

protected:
	// �f�[�^�����o
	unsigned int		m_nIndex ;
	struct	PUNCTUATION_MASK
	{
		DWORD	m_bit32[4] ;
	} ;
	PUNCTUATION_MASK	m_maskPunctuation ;
	PUNCTUATION_MASK	m_maskSpecialPunc ;

private:
	static const PUNCTUATION_MASK	m_defPunctuation ;
	static const PUNCTUATION_MASK	m_defSpecialPunc ;

public:
	// �������
	const EStreamWideString &
				operator = ( const EStreamWideString & swstr ) ;
	const EStreamWideString &
				operator = ( const EGenString<wchar_t,EWideString> & widestr ) ;
	const EStreamWideString &
				operator = ( const wchar_t * pwszString ) ;
	const EStreamWideString &
				operator = ( const char * pszString ) ;

public:
	// �t�@�C������ǂݍ���
	virtual ESLError ReadTextFile( ESLFileObject & file ) ;
	virtual ESLError ReadJiscode( ESLFileObject & file ) ;
	virtual ESLError ReadUnicode( ESLFileObject & file ) ;

public:
	// �w�W�ɑ΂��鑀��
	unsigned int GetIndex( void ) const
		{
			return	m_nIndex ;
		}
	unsigned int MoveIndex( unsigned int nIndex )
		{
			if ( nIndex > m_nLength )
				return	(m_nIndex = m_nLength) ;
			else
				return	(m_nIndex = nIndex) ;
		}

public:		// �X�g���[�~���O����
	// ����1�������擾
	wchar_t GetCharacter( void )
		{
			if ( m_nIndex >= m_nLength )
				return	L'\0' ;
			return	m_pszString[(m_nIndex ++)] ;
		}
	wchar_t CurrentCharacter( void ) const
		{
			if ( m_nIndex >= m_nLength )
				return	L'\0' ;
			return	m_pszString[m_nIndex] ;
		}

	// �󔒕�����ǂݔ�΂�
	virtual int DisregardSpace( void ) ;

	// ���̍s�ֈړ�
	virtual int MoveToNextLine( void ) ;

	// ���݂̕�����i��؂�L���͖����j��ʉ߂���
	virtual void PassString( void ) ;
	virtual void PassEnclosedString
		( wchar_t wchClose, int flagCtrlCode = FALSE ) ;

	// ���݂̃g�[�N����ʉ߂���
	virtual void PassAToken( int * pTokenType = NULL ) ;
	virtual void PassAExpressionTerm( void ) ;

public:		// ������擾
	// ���̕�����擾�i��؂�L���͖����j
	virtual EWideString GetString( void ) ;
	virtual EWideString GetEnclosedString
		( wchar_t wchClose, int flagCtrlCode = FALSE ) ;

	// ���̃g�[�N�����擾
	virtual EWideString GetAToken( int * pTokenType = NULL ) ;

	// ���̐��l���擾
	virtual int GetNumberRadix( void ) ;
	virtual REAL64 GetRealNumber( int nRadix = 0 ) ;
	virtual INT64 GetLargeInteger( int nRadix = 0 ) ;
	virtual int GetInteger( int nRadix = 0 ) ;

	// ���ɗ���ׂ������i������j��ʉ߂���
	wchar_t HasToComeChar( const wchar_t * pwszNext ) ;
	bool HasToComeToken( const wchar_t * pwszNext ) ;

public:		// ���x�ȏ������
	// �����\���̍\����
	enum	UsageType
	{
		utToken,				// �g�[�N��
		utString,				// ������
		utSpace,				// �C�ӂ̋�
		utEnclosed,				// �w�蕶���R�[�h�ŕ���ꂽ�C�ӕ�����
		utWildCardToken,		// �C�ӂ̃g�[�N��
		utWildCardUsage,		// �C�ӂ̕�����
		utWildCardExpression,	// �C�ӂ̎�
		utNumberUsage,			// 10�i��������
		utEndOfUsage,			// ������̏I�[
		utCharacters,			// �w��̕���
		utRepeat,				// ����
		utBeginParam,			// �p�����[�^�̊J�n
		utEndParam,				// �p�����[�^�̏I��
		utOmittable,			// �ȗ��\
		utSelectable,			// �����̑I��
		utList
	} ;
	enum	UsageFlag
	{
		ufNoCase	= 0x0001,
		ufNegative	= 0x0002,
		ufRepeat	= 0x0004
	} ;
	class	EUsage	: public	EObjArray<EUsage>
	{
	public:
		int					m_type ;
		int					m_flags ;
		EUsage *			m_parent ;
		EWideString			m_wstr ;
		DWORD				m_mask[4] ;
	public:
		EUsage( int type = utList, EUsage * parent = NULL )
			: m_type(type), m_flags(0), m_parent(parent) { }
		~EUsage( void ) { }
		int GetUsageParamCount( void ) const ;
		int GetChildIndex( EUsage * pUsage ) const ;
	} ;

	// ���̏�������
	virtual ESLError IsMatchUsage
		( EStreamWideString & swsUsage, EString & strErrMsg,
				EObjArray<EWideString> * plstParam = NULL ) ;
	// �����̌���
	virtual int FindMatchUsage
		( EStreamWideString & swsUsage, UsageType utWildCardType = utToken ) ;
	// �����̒u������
	virtual ESLError ReplaceUsage
		( EWideString & wstrReplaced, EStreamWideString & swsUsage,
			const wchar_t * pwszReplace, UsageType utWildCardType = utToken ) ;
protected:
	// �����̒u����������
	virtual ESLError OnReplaceUsage
		( EWideString & wstrReplace,
			const EObjArray<EWideString> & lstParam ) ;

public:
	// �����̈�v����
	virtual ESLError IsMatchAUsage
		( EUsage * pUsage, EString * pstrErrMsg,
			EObjArray<EWideString> * plstParam = NULL ) ;
	virtual ESLError IsMatchUsageList
		( EUsage & usage, int iStart,
			EString & strErrMsg, EObjArray<EWideString> * plstParam = NULL ) ;
	virtual int FindMatchUsageList
		( EUsage & usage, int iStart, UsageType utWildCardType ) ;
	virtual int FindMatchAUsage
		( EUsage * pParent, int nIndex,
			EUsage * pUsage, UsageType utWildCardType ) ;
	// �����\���̉��
	virtual ESLError ParseUsage
		( EUsage & lstUsage,
			EStreamWideString & swsUsage, EString & strErrMsg ) const ;

public:		// ��Ԗ₢���킹
	// �w�W���I�[�o�[�t���[���Ă��邩�H
	int IsIndexOverflow( void ) const
		{
			return	(m_nIndex >= m_nLength) ;
		}
	// �󔒋L�����H
	static int IsCharacterSpace( wchar_t wch )
		{
			return	(wch <= L' ') ;
		}
	// ��؂�L�����H
	int IsPunctuation( wchar_t wch ) const
		{
			if	( wch & (~0x7F) )	return	0 ;
			return	(m_maskPunctuation.m_bit32
						[(wch >> 5)] & (1 << (wch & 0x1F))) ;
		}
	// �����؂�L�����H
	int IsSpecialPunctuaion( wchar_t wch ) const
		{
			if	( wch & (~0x7F) )	return	0 ;
			return	(m_maskSpecialPunc.m_bit32
						[(wch >> 5)] & (1 << (wch & 0x1F))) ;
		}

} ;

#endif	//	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                           �Q�ƃo�b�t�@�N���X
 ****************************************************************************/

class	EPtrBuffer
{
protected:
	// �\�z�֐�
	EPtrBuffer( void ) { }
public:
	EPtrBuffer( const void * ptrBuffer, unsigned int nLength )
		: m_ptrBuffer( ptrBuffer ), m_nLength( nLength ) { }
	EPtrBuffer( const EPtrBuffer & ptrbuf )
		: m_ptrBuffer( ptrbuf.m_ptrBuffer ), m_nLength( ptrbuf.m_nLength ) { }
	// ���Ŋ֐�
	~EPtrBuffer( void ) { }

protected:
	// �f�[�^�����o
	const void *	m_ptrBuffer ;
	unsigned int	m_nLength ;

public:
	// �o�b�t�@�̒����擾
	unsigned int GetLength( void ) const
		{
			return	m_nLength ;
		}
	// �o�b�t�@�ւ̃A�h���X�擾
	const void * GetBuffer( void ) const
		{
			return	m_ptrBuffer ;
		}

public:
	// �����V�[�P���X�擾
	operator const void * ( void ) const
		{
			return	m_ptrBuffer ;
		}
	// �������
	EPtrBuffer & operator = ( const EPtrBuffer & ptrbuf )
		{
			m_ptrBuffer = ptrbuf.m_ptrBuffer ;
			m_nLength = ptrbuf.m_nLength ;
			return	*this ;
		}

} ;


/*****************************************************************************
                    �X�g���[�~���O�o�b�t�@�N���X
 ****************************************************************************/

class	EStreamBuffer	: public	ESLObject
{
public:
	// �\�z�֐�
	EStreamBuffer( void ) ;
	EStreamBuffer( const void * ptrBuffer, unsigned int nLength ) ;
	EStreamBuffer( const EPtrBuffer & ptrbuf ) ;
	// ���Ŋ֐�
	virtual ~EStreamBuffer( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EStreamBuffer, ESLObject )

protected:
	class	EBuffer
	{
	protected:
		EBuffer *		m_pNextBuf ;
		BYTE *			m_ptrBuf ;
		unsigned int	m_nBeginPos ;
		unsigned int	m_nLength ;
		unsigned int	m_nBufLimit ;
	public:
		// �\�z�֐�
		EBuffer( void )
			: m_pNextBuf( NULL ),
				m_ptrBuf( NULL ), m_nBeginPos( 0 ),
				m_nLength( 0 ), m_nBufLimit( 0 ) { }
		// ���Ŋ֐�
		~EBuffer( void ) ;
		// ���̃o�b�t�@�擾
		EBuffer * GetNextBuffer( void ) const
			{
				return	m_pNextBuf ;
			}
		void SetNextBuffer( EBuffer * pNext )
			{
				m_pNextBuf = pNext ;
			}
		// �o�b�t�@�̒������擾
		unsigned int GetLength( void ) const
			{
				return	m_nLength ;
			}
		// �o�b�t�@�擾
		EPtrBuffer GetBuffer( unsigned int nLength ) ;
		// �o�b�t�@���
		void Release( unsigned int nLength ) ;
		// �������Ċm�ۖ����Ŋm�ۉ\�ȍő�̒������擾
		unsigned int GetMaxWritableLength( void ) const
			{
				return	(m_nBufLimit - m_nLength) ;
			}
		// �o�͂̂��߂Ƀo�b�t�@�m��
		void * PutBuffer( unsigned int nLength ) ;
		// �������m��
		void Flush( unsigned int nLength ) ;
		// �o�b�t�@�Q��
		BYTE * ModifyBuffer( unsigned int nPosition )
			{
				return	m_ptrBuf + nPosition ;
			}
	} ;
	EBuffer *		m_pFirstBuf ;
	EBuffer *		m_pLastBuf ;
	unsigned int	m_nLength ;

public:
	// ���\�[�X�����
	void Delete( void ) ;

public:		// �o�b�t�@����̏o�͑���
	// �o�b�t�@�̒����擾
	unsigned int GetLength( void ) const ;
	// �o�b�t�@������o�����߂̃��������m��
	EPtrBuffer GetBuffer( unsigned int nLength = 0 ) ;
	// GetBuffer �ɂ���ă��b�N���ꂽ�����������
	void Release( unsigned int nLength ) ;
	// �o�b�t�@����f�[�^��ǂݏo��
	unsigned int Read( void * ptrBuffer, unsigned int nLength ) ;

public:		// �o�b�t�@�ւ̓��͑���
	// �o�b�t�@�֏����o�����߂̃��������m��
	void * PutBuffer( unsigned int nLength ) ;
	// PutBuffer �ɂ���ă��b�N���ꂽ���������m��
	void Flush( unsigned int nLength ) ;
	// �o�b�t�@�փf�[�^����������
	unsigned int Write( const void * ptrBuffer, unsigned int nLength ) ;

public:		// �o�b�t�@�ւ̕ύX����
	// �o�b�t�@�̈ꕔ���Q��
	void * ModifyBuffer( unsigned int nPosition, unsigned int nLength ) ;

public:
	// �t�@�C���̏I�[�܂œǂݍ���
	ESLError ReadFromFile( ESLFileObject & file ) ;
	// �o�b�t�@�̓��e���t�@�C���ɏ����o��
	ESLError WriteToFile( ESLFileObject & file ) ;

} ;


#endif
