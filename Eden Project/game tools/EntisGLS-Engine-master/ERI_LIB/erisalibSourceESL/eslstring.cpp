
/*****************************************************************************
                   Entis Standard Library declarations
 ----------------------------------------------------------------------------
          Copyright (c) 1998-2003 Leshade Entis. All rights reserved.
 ****************************************************************************/


// Include esl.h
//////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<eritypes.h>
#include	<esl.h>


/*****************************************************************************
                       文字列クラスのインスタンス化
 ****************************************************************************/

IMPLEMENT_CLASS_INFO( EString, ESLObject )
#if	!defined(COMPACT_NOA_DECODER)
IMPLEMENT_CLASS_INFO( EWideString, ESLObject )
#endif

// EString 構築
//////////////////////////////////////////////////////////////////////////////
EString::EString( const wchar_t * ptrString, unsigned int nLength )
	: EGenString<char,EString>( )
{
	if ( ptrString == NULL )
	{
		m_nLength = 0 ;
		AllocString( 1 ) ;
	}
	else
	{
		m_nLength = ::WideCharToMultiByte
			( CP_ACP, 0, ptrString, nLength, NULL, 0, NULL, NULL ) ;
		if ( nLength == -1 )
		{
			m_nLength -- ;
		}
		AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
		::WideCharToMultiByte
			( CP_ACP, 0, ptrString, nLength,
				m_pszString, m_nBufLimit, NULL, NULL ) ;
	}
	m_pszString[m_nLength] = 0 ;
}

EString::EString( int nValue, int nPrec )
	: EGenString<char,EString>( )
{
	int		i = 0, c = 1 ;
	INT64	v = nValue, r = 10 ;
	AllocString( (nPrec < 0x10) ? 0x10 : nPrec ) ;
	if ( v < 0 )
	{
		m_pszString[i ++] = '-' ;
		v = - v ;
	}
	if ( nPrec <= 0 )
	{
		while ( v >= r )
		{
			r *= 10 ;
			c ++ ;
		}
	}
	else
	{
		c = nPrec - i ;
	}
	m_nLength = i + c ;
	m_pszString[m_nLength] = '\0' ;
	//
	while ( c -- )
	{
		int		n = (int) (v % 10) ;
		m_pszString[i + c] = (char)('0' + n) ;
		v = (v - n) / 10 ;
	}
}

EString::EString( DWORD dwValue, int nPrec )
{
	int		i = 0, c = 1 ;
	UINT64	r = 16 ;
	AllocString( (nPrec < 0x10) ? 0x10 : nPrec ) ;
	if ( nPrec <= 0 )
	{
		while ( dwValue >= r )
		{
			r <<= 4 ;
			c ++ ;
		}
	}
	else
	{
		c = nPrec - i ;
	}
	m_nLength = i + c ;
	m_pszString[m_nLength] = '\0' ;
	//
	while ( c -- )
	{
		int		n = dwValue & 0x0F ;
		if ( n < 10 )
			m_pszString[i + c] = (char)('0' + n) ;
		else
			m_pszString[i + c] = (char)('A' + n - 10) ;
		dwValue >>= 4 ;
	}
}

EString::EString( double rValue, int nPrec )
{
	int		i = 0, c = 1 ;
	INT64	r = 10 ;
	INT64	nValue, nIntPart ;
	AllocString( 0x20 ) ;
	//
	// 整数部フォーマット
	//
	if ( rValue < 0 )
	{
		m_pszString[i ++] = '-' ;
		rValue = - rValue ;
	}
	int	nDecPrec = nPrec ;
	if ( nPrec <= 0 )
	{
		nPrec = 9 - m_nLength ;
	}
	INT64	d = 1 ;	
	for ( int j = 0; j < nPrec; j ++ )
	{
		d *= 10 ;
	}
	nIntPart = nValue = ((INT64)(rValue * d + 0.5)) / d ;
	if ( nValue < 0 )
	{
		m_pszString[i ++] = '0' ;
		m_pszString[i ++] = '.' ;
		m_pszString[i ++] = '0' ;
		m_pszString[i ++] = '\0' ;
		m_nLength = i ;
		return ;
	}
	while ( nValue >= r )
	{
		r *= 10 ;
		if ( ++ c >= 19 )
			break ;
	}
	m_nLength = i + c ;
	while ( c -- )
	{
		int		n = (int)(nValue % 10) ;
		m_pszString[i + c] = (char)('0' + n) ;
		nValue = (nValue - n) / 10 ;
	}
	//
	// 小数部フォーマット
	//
	m_pszString[m_nLength ++] = '.' ;
	if ( nPrec > 0 )
	{
		if ( m_nLength + nPrec >= m_nBufLimit )
		{
			AllocString( m_nLength + nPrec + 1 ) ;
		}
		nValue = (INT64)((rValue - nIntPart) * d + 0.5) ;
		for ( i = 0; i < nPrec; i ++ )
		{
			if ( (nValue == 0) && (nDecPrec <= 0) )
			{
				break ;
			}
			d /= 10 ;
			int		n = (int)(nValue / d) ;
			m_pszString[m_nLength ++] = (char)('0' + n) ;
			nValue -= n * d ;
		}
	}
	//
	m_pszString[m_nLength] = '\0' ;
}

// EString 代入
//////////////////////////////////////////////////////////////////////////////
const EString & EString::operator = ( const wchar_t * ptrString )
{
	if ( ptrString == NULL )
	{
		m_nLength = 0 ;
		AllocString( 1 ) ;
	}
	else
	{
		m_nLength = ::WideCharToMultiByte
			( CP_ACP, 0, ptrString, -1, NULL, 0, NULL, NULL ) - 1 ;
		AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
		::WideCharToMultiByte
			( CP_ACP, 0, ptrString, -1,
				m_pszString, m_nBufLimit, NULL, NULL ) ;
	}
	m_pszString[m_nLength] = 0 ;
	return	*this ;
}

// 文字列比較
//////////////////////////////////////////////////////////////////////////////
int EString::Compare( const char * pstr1, const char * pstr2 )
{
	if ( pstr1 == NULL )
	{
		return	((pstr2 && pstr2[0]) ? -1 : 0) ;
	}
	else if ( pstr2 == NULL )
	{
		return	(pstr1[0] ? 1 : 0) ;
	}
	unsigned int	i ;
	for ( i = 0; pstr1[i]; i ++ )
	{
		char	c1 = pstr1[i] ;
		char	c2 = pstr2[i] ;
		if ( c1 != c2 )
		{
			int	nCompare = ((int) c1 & 0xFF) - ((int) c2 & 0xFF) ;
			if ( nCompare != 0 )
			{
				return	nCompare ;
			}
		}
	}
	return	 - ((int) pstr2[i] & 0xFF) ;
}

int EString::CompareNoCase( const char * pstr1, const char * pstr2 )
{
	if ( pstr1 == NULL )
	{
		return	((pstr2 && pstr2[0]) ? -1 : 0) ;
	}
	else if ( pstr2 == NULL )
	{
		return	(pstr1[0] ? 1 : 0) ;
	}
	unsigned int	i ;
	for ( i = 0; pstr1[i]; i ++ )
	{
		char	c1 = pstr1[i] ;
		char	c2 = pstr2[i] ;
		if ( c1 != c2 )
		{
			if ( (L'a' <= c1) && (c1 <= L'z') )
				c1 += (L'A' - L'a') ;
			if ( (L'a' <= c2) && (c2 <= L'z') )
				c2 += (L'A' - L'a') ;
			int	nCompare = ((int) c1 & 0xFF) - ((int) c2 & 0xFF) ;
			if ( nCompare != 0 )
			{
				return	nCompare ;
			}
		}
	}
	return	 - ((int) pstr2[i] & 0xFF) ;
}

int EString::Compare( const char * pszString ) const
{
	return	EGenString<char,EString>::Compare( pszString ) ;
}

int EString::CompareNoCase( const char * pszString ) const
{
	if ( m_pszString == NULL )
	{
		return	((pszString != NULL) ? -1 : 0) ;
	}
	else if ( pszString == NULL )
	{
		return	((m_nLength == 0) ? -1 : 0) ;
	}
	unsigned int	i ;
	for ( i = 0; i < m_nLength; i ++ )
	{
		char	c1 = m_pszString[i] ;
		char	c2 = pszString[i] ;
		if ( c1 != c2 )
		{
			if ( (L'a' <= c1) && (c1 <= L'z') )
				c1 += (L'A' - L'a') ;
			if ( (L'a' <= c2) && (c2 <= L'z') )
				c2 += (L'A' - L'a') ;
			int	nCompare = ((int) c1 & 0xFF) - ((int) c2 & 0xFF) ;
			if ( nCompare != 0 )
			{
				return	nCompare ;
			}
		}
	}
	return	 - (int) pszString[i] ;
}

// ファイル名（URL）操作
//////////////////////////////////////////////////////////////////////////////
const char * EString::GetFileNamePart( void ) const
{
	unsigned int	l = 0 ;
	if ( m_pszString == NULL )
	{
		((EString*)this)->AllocString( 0x10 ) ;
	}
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		char	c = m_pszString[i] ;
		if ( ::IsDBCSLeadByte( c ) )
		{
			i ++ ;
		}
		else if ( (c == '\\') || (c == '/') )
		{
			l = i + 1 ;
		}
	}
	return	m_pszString + l ;
}

const char * EString::GetFileExtensionPart( void ) const
{
	unsigned int	l = 0 ;
	if ( m_pszString == NULL )
	{
		((EString*)this)->AllocString( 0x10 ) ;
	}
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		char	c = m_pszString[i] ;
		if ( ::IsDBCSLeadByte( c ) )
		{
			i ++ ;
		}
		else if ( (c == '\\') || (c == '/') )
		{
			l = 0 ;
		}
		else if ( c == '.' )
		{
			l = i + 1 ;
		}
	}
	if ( l == 0 )
	{
		l = m_nLength ;
	}
	return	m_pszString + l ;
}

EString EString::GetFileTitlePart( void ) const
{
	const char *	pszFileName = GetFileNamePart( ) ;
	int		i = 0, l = -1 ;
	while ( pszFileName[i] )
	{
		if ( ::IsDBCSLeadByte( pszFileName[i] ) )
		{
			i ++ ;
		}
		else if ( pszFileName[i] == '.' )
		{
			l = i ;
		}
		i ++ ;
	}
	return	EString( pszFileName, l ) ;
}

EString EString::GetFileDirectoryPart( void ) const
{
	int		l = 0 ;
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		char	c = m_pszString[i] ;
		if ( ::IsDBCSLeadByte( c ) )
		{
			i ++ ;
		}
		else if ( (c == '\\') || (c == '/') )
		{
			l = i + 1 ;
		}
	}
	return	EString( m_pszString, l ) ;
}

// アルファベットを大文字に変換
//////////////////////////////////////////////////////////////////////////////
void EString::MakeUpper( void )
{
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		char	c = m_pszString[i] ;
		if ( ::IsDBCSLeadByte( c ) )
		{
			i ++ ;
		}
		else if ( ('a' <= c) && (c <= 'z') )
		{
			m_pszString[i] = c - ('a' - 'A') ;
		}
	}
}

// アルファベットを小文字に変換
//////////////////////////////////////////////////////////////////////////////
void EString::MakeLower( void )
{
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		char	c = m_pszString[i] ;
		if ( ::IsDBCSLeadByte( c ) )
		{
			i ++ ;
		}
		else if ( ('A' <= c) && (c <= 'Z') )
		{
			m_pszString[i] = c + ('a' - 'A') ;
		}
	}
}

// 文字列の左右反転
//////////////////////////////////////////////////////////////////////////////
void EString::MakeReverse( void )
{
	EString	strTemp = *this ;
	const char *	pszTemp = strTemp ;
	unsigned int	j = m_nLength ;
	for ( unsigned int i = 0; i < m_nLength; i ++ )
	{
		char	c = pszTemp[i] ;
		if ( ::IsDBCSLeadByte( c ) )
		{
			j -= 2 ;
			m_pszString[j] = c ;
			m_pszString[j + 1] = pszTemp[++ i] ;
		}
		else
		{
			m_pszString[-- j] = c ;
		}
	}
}

// 文字検索
//////////////////////////////////////////////////////////////////////////////
int EString::Find( char chrCode, int nIndex ) const
{
	while ( (unsigned int) nIndex < m_nLength )
	{
		if ( ::IsDBCSLeadByte( m_pszString[nIndex] ) )
		{
			nIndex ++ ;
		}
		else if ( m_pszString[nIndex] == chrCode )
		{
			return	nIndex ;
		}
		nIndex ++ ;
	}
	return	-1 ;
}

int EString::Find( const char * pszString, int nIndex ) const
{
	while ( (unsigned int) nIndex < m_nLength )
	{
		int		i ;
		for ( i = 0; pszString[i]; i ++ )
		{
			if ( m_pszString[nIndex + i] != pszString[i] )
				break ;
		}
		if ( pszString[i] == 0 )
		{
			return	nIndex ;
		}
		nIndex ++ ;
	}
	return	-1 ;
}

// 空白除去
//////////////////////////////////////////////////////////////////////////////
void EString::TrimLeft( void )
{
	if ( m_pszString != NULL )
	{
		unsigned int	i ;
		for ( i = 0; i < m_nLength; i ++ )
		{
			if ( (unsigned char) m_pszString[i] > (unsigned char) ' ' )
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
				i = 0 ;
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

void EString::TrimRight( void )
{
	if ( m_pszString != NULL )
	{
		int	i ;
		for ( i = (m_nLength - 1); i >= 0; i -- )
		{
			if ( (unsigned char) m_pszString[i] > (unsigned char) ' ' )
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


#if	!defined(COMPACT_NOA_DECODER)

// EWideString 構築
//////////////////////////////////////////////////////////////////////////////
EWideString::EWideString( const char * ptrString, unsigned int nLength )
	: EGenString<wchar_t,EWideString>( )
{
	if ( ptrString == NULL )
	{
		m_nLength = 0 ;
		AllocString( 1 ) ;
	}
	else
	{
		m_nLength = ::MultiByteToWideChar
			( CP_ACP, MB_PRECOMPOSED, ptrString, nLength, NULL, 0 ) ;
		if ( nLength == -1 )
		{
			m_nLength -- ;
		}
		AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
		::MultiByteToWideChar
			( CP_ACP, MB_PRECOMPOSED,
				ptrString, nLength, m_pszString, m_nBufLimit ) ;
	}
	m_pszString[m_nLength] = 0 ;
}

EWideString::EWideString( int nValue, int nPrec )
	: EGenString<wchar_t,EWideString>( )
{
	EString	strValue( nValue, nPrec ) ;
	operator = ( strValue ) ;
}

EWideString::EWideString( DWORD dwValue, int nPrec )
	: EGenString<wchar_t,EWideString>( )
{
	EString	strValue( dwValue, nPrec ) ;
	operator = ( strValue ) ;
}

EWideString::EWideString( double rValue, int nPrec )
	: EGenString<wchar_t,EWideString>( )
{
	EString	strValue( rValue, nPrec ) ;
	operator = ( strValue ) ;
}

// EWideString 代入
//////////////////////////////////////////////////////////////////////////////
const EWideString & EWideString::operator = ( const char * ptrString )
{
	if ( ptrString == NULL )
	{
		m_nLength = 0 ;
		AllocString( 1 ) ;
	}
	else
	{
		m_nLength = ::MultiByteToWideChar
			( CP_ACP, MB_PRECOMPOSED, ptrString, -1, NULL, 0 ) - 1 ;
		AllocString( m_nLength + (m_nLength >> 1) + 1 ) ;
		::MultiByteToWideChar
			( CP_ACP, MB_PRECOMPOSED,
				ptrString, -1, m_pszString, m_nBufLimit ) ;
	}
	m_pszString[m_nLength] = 0 ;
	return	*this ;
}


/*****************************************************************************
                        構文解析用文字列クラス
 ****************************************************************************/

//
// デフォルト区切り記号
//////////////////////////////////////////////////////////////////////////////

const EStreamWideString::PUNCTUATION_MASK
		EStreamWideString::m_defPunctuation =
{
	0xFFFFFFFF,		// All control code is punctuation.
	0x7C00FFFF,		// " !"#$%&'()*+,-./" and ":;<=>" are punctuation.
	0x78000000,		// "[\]^" are punctuation.
	0xF8000001		// '`' and "{|}~ " are punctuation.
} ;

const EStreamWideString::PUNCTUATION_MASK
		EStreamWideString::m_defSpecialPunc =
{
	0x00000000,		//
	0x58001384,		// ""'(),;<>" are special punctuation.
	0x28000000,		// "[]" are special punctuation.
	0x28000000		// "{}" are special punctuation.
} ;

IMPLEMENT_CLASS_INFO( EStreamWideString, EWideString )

//
// 構築関数
//////////////////////////////////////////////////////////////////////////////

EStreamWideString::EStreamWideString( void )
	: EWideString( ), m_nIndex( 0 ),
		m_maskPunctuation( m_defPunctuation ),
		m_maskSpecialPunc( m_defSpecialPunc )
{
}

EStreamWideString::EStreamWideString
( const EStreamWideString & swidestr )
	: EWideString( swidestr ),
		m_nIndex( swidestr.m_nIndex ),
		m_maskPunctuation( swidestr.m_maskPunctuation ),
		m_maskSpecialPunc( m_defSpecialPunc )
{
}

EStreamWideString::EStreamWideString
( const EGenString<wchar_t,EWideString> & widestr )
	: EWideString( widestr ), m_nIndex( 0 ),
		m_maskPunctuation( m_defPunctuation ),
		m_maskSpecialPunc( m_defSpecialPunc )
{
}

EStreamWideString::EStreamWideString
( const wchar_t * ptrString, unsigned int nLength )
	: EWideString( ptrString, nLength ), m_nIndex( 0 ),
		m_maskPunctuation( m_defPunctuation ),
		m_maskSpecialPunc( m_defSpecialPunc )
{
}

EStreamWideString::EStreamWideString( const wchar_t * ptrString )
	: EWideString( ptrString ), m_nIndex( 0 ),
		m_maskPunctuation( m_defPunctuation ),
		m_maskSpecialPunc( m_defSpecialPunc )
{
}

EStreamWideString::EStreamWideString
( const char * ptrString, unsigned int nLength )
	: EWideString( ptrString, nLength ), m_nIndex( 0 ),
		m_maskPunctuation( m_defPunctuation ),
		m_maskSpecialPunc( m_defSpecialPunc )
{
}

//
// 消滅関数
//////////////////////////////////////////////////////////////////////////////

EStreamWideString::~EStreamWideString( void )
{
}

void EStreamWideString::DeleteContents( void )
{
	EWideString::operator = ( L'\0' ) ;
	m_nIndex = 0 ;
}

//
// 代入操作
//////////////////////////////////////////////////////////////////////////////

const EStreamWideString &
	EStreamWideString::operator = ( const EStreamWideString & swidestr )
{
	EWideString::operator = ( swidestr ) ;
	m_nIndex = swidestr.m_nIndex ;
	m_maskPunctuation = swidestr.m_maskPunctuation ;
	m_maskSpecialPunc = swidestr.m_maskSpecialPunc ;
	return	*this ;
}

const EStreamWideString &
	EStreamWideString::operator =
		( const EGenString<wchar_t,EWideString> & widestr )
{
	EWideString::operator = ( widestr ) ;
	m_nIndex = 0 ;
	return	*this ;
}

const EStreamWideString &
	EStreamWideString::operator = ( const wchar_t * ptrString )
{
	EWideString::operator = ( ptrString ) ;
	m_nIndex = 0 ;
	return	*this ;
}

const EStreamWideString &
	EStreamWideString::operator = ( const char * ptrString )
{
	EWideString::operator = ( ptrString ) ;
	m_nIndex = 0 ;
	return	*this ;
}

//
// ファイルから読み込む
//////////////////////////////////////////////////////////////////////////////

ESLError EStreamWideString::ReadTextFile( ESLFileObject & file )
{
	//
	// ShiftJIS ファイルか Unicode ファイルかを判定
	//
	WORD	wCheckUnicode ;
	bool	fUnicode = false ;
	unsigned long int	nBeginPos = file.GetPosition( ) ;
	if ( file.Read( &wCheckUnicode,
					sizeof(wCheckUnicode) ) == sizeof(wCheckUnicode) )
	{
		fUnicode = (wCheckUnicode == 0xFEFF) ;
	}
	//
	// テキストファイルを読み込む
	//
	if ( fUnicode )
	{
		return	ReadUnicode( file ) ;
	}
	else
	{
		file.Seek( nBeginPos, ESLFileObject::FromBegin ) ;
		return	ReadJiscode( file ) ;
	}
}

ESLError EStreamWideString::ReadJiscode( ESLFileObject & file )
{
	//
	// ファイルを読み込む
	//
	EStreamBuffer	buf ;
	unsigned int	nLength = file.GetLength() - file.GetPosition() ;
	char *	pstrBuf = (char*) buf.PutBuffer( nLength + 1 ) ;
	//
	if ( file.Read( pstrBuf, nLength ) < nLength )
	{
		return	eslErrGeneral ;
	}
	pstrBuf[nLength] = '\0' ;
	//
	// Unicode へ変換
	//
	EStreamWideString::operator = ( pstrBuf ) ;
	m_nIndex = 0 ;

	return	eslErrSuccess ;
}

ESLError EStreamWideString::ReadUnicode( ESLFileObject & file )
{
	m_nLength =
		(file.GetLength() - file.GetPosition()) / sizeof(wchar_t) ;
	AllocString( m_nLength + 1 ) ;
	//
	if( file.Read( m_pszString,
		(m_nLength * sizeof(wchar_t)) ) < (m_nLength * sizeof(wchar_t)) )
	{
		return	eslErrGeneral ;
	}
	m_pszString[m_nLength] = L'\0';
	m_nIndex = 0 ;

	return	eslErrSuccess ;
}

//
// 空白文字を読み飛ばす
//////////////////////////////////////////////////////////////////////////////

int EStreamWideString::DisregardSpace( void )
{
	while ( m_nIndex < m_nLength )
	{
		if( !IsCharacterSpace( m_pszString[m_nIndex] ) )
		{
			return	0 ;
		}
		m_nIndex ++ ;
	}
	return	1 ;
}

//
// 次の行へ移動
//////////////////////////////////////////////////////////////////////////////

int EStreamWideString::MoveToNextLine( void )
{
	while ( m_nIndex < m_nLength )
	{
		wchar_t	wch = m_pszString[(m_nIndex ++)] ;
		if ( wch == L'\n' )
		{
			if ( m_nIndex < m_nLength )
			{
				if ( m_pszString[m_nIndex] == L'\r' )
					return	((++ m_nIndex) >= m_nLength) ;
				else
					return	0 ;
			}
			else
			{
				return	1 ;
			}
		}
		else if ( wch == L'\r' )
		{
			if ( m_nIndex < m_nLength )
			{
				if( m_pszString[m_nIndex] == L'\n' )
					return	((++ m_nIndex) >= m_nLength) ;
				else
					return	0 ;
			}
			else
			{
				return	1 ;
			}
		}
	}
	return	1 ;
}

//
// 現在の文字列（区切り記号は無視）を通過する
//////////////////////////////////////////////////////////////////////////////

void EStreamWideString::PassString( void )
{
	while ( m_nIndex < m_nLength )
	{
		if ( IsCharacterSpace( m_pszString[m_nIndex] ) )
		{
			break ;
		}
		m_nIndex ++ ;
	}
}

//
// 囲まれた文字列を通過する
//////////////////////////////////////////////////////////////////////////////

void EStreamWideString::PassEnclosedString
	( wchar_t wchClose, int flagCtrlCode )
{
	while ( m_nIndex < m_nLength )
	{
		if ( m_pszString[m_nIndex] == wchClose )
		{
			break ;
		}
		m_nIndex ++ ;
	}
}

//
// 現在のトークンを通過する
//////////////////////////////////////////////////////////////////////////////

void EStreamWideString::PassAToken( int * pTokenType )
{
	//
	// 先頭の文字を取得
	//
	wchar_t	wch = GetCharacter( ) ;
	if ( IsSpecialPunctuaion( wch ) )
	{
		// 特殊区切り記号
		if( pTokenType != NULL )
		{
			*pTokenType = 2 ;
		}
		return ;
	}
	if ( IsPunctuation( wch ) )
	{
		// 区切り記号
		if( pTokenType != NULL )
		{
			*pTokenType = 1 ;
		}
		while ( m_nIndex < m_nLength )
		{
			wch = m_pszString[m_nIndex] ;
			if( IsCharacterSpace( wch ) ||
					IsSpecialPunctuaion( wch ) || !IsPunctuation( wch ) )
				break ;
			m_nIndex ++ ;
		}
	}
	else
	{
		// 非区切り記号
		if ( pTokenType != NULL )
		{
			*pTokenType = 0 ;
		}
		while ( m_nIndex < m_nLength )
		{
			if( IsPunctuation( m_pszString[m_nIndex] ) )
				break ;
			m_nIndex ++ ;
		}
	}
}

void EStreamWideString::PassAExpressionTerm( void )
{
	PassAToken( ) ;
}

//
// 次の文字列を取得（区切り記号は無視）
//////////////////////////////////////////////////////////////////////////////

EWideString EStreamWideString::GetString( void )
{
	if( DisregardSpace() )
	{
		return	EWideString( ) ;
	}

	unsigned int	nBeginIndex = m_nIndex ;

	PassString( ) ;

	return	EWideString
		( (m_pszString + nBeginIndex), (m_nIndex - nBeginIndex) ) ;
}

//
// 囲まれた文字列を取得
//////////////////////////////////////////////////////////////////////////////

EWideString EStreamWideString::GetEnclosedString
	( wchar_t wchClose, int flagCtrlCode )
{
	unsigned int	nBeginIndex = m_nIndex ;

	PassEnclosedString( wchClose, flagCtrlCode ) ;

	unsigned int	nEndIndex = m_nIndex ;
	if ( CurrentCharacter() == wchClose )
	{
		m_nIndex ++ ;
	}
	return	EWideString
		( (m_pszString + nBeginIndex), (nEndIndex - nBeginIndex) ) ;
}

//
// 次のトークンを取得
//////////////////////////////////////////////////////////////////////////////

EWideString EStreamWideString::GetAToken( int * pTokenType )
{
	if ( pTokenType != NULL )
		*pTokenType = -1 ;

	if ( DisregardSpace() )
	{
		return	EWideString( ) ;
	}

	unsigned int	nBeginIndex = m_nIndex ;

	PassAToken( pTokenType ) ;

	return	EWideString
		( (m_pszString + nBeginIndex), (m_nIndex - nBeginIndex) ) ;
}

//
// 次の数値の基底を取得
//////////////////////////////////////////////////////////////////////////////

int EStreamWideString::GetNumberRadix( void )
{
	// Get the number radix
	// The number format ;
	//    [{ + | - }] [nnnnnn][.nnnnnn]{ B | O | [T] | H }
	// B : binary ; radix = 2
	// O : octagonal ; radix = 8
	// T : ten ; radix = 10
	// H : hexagonal ; radix = 16

	// Save begining index
	unsigned int	nSaveIndex = m_nIndex;
	if( DisregardSpace() )	return	-1;

	// Get sign mark
	wchar_t	wchNext = GetCharacter();
	if( (wchNext == L'+') || (wchNext == L'-') )
	{
		if( DisregardSpace() )
		{
			m_nIndex = nSaveIndex ;
			return	-1 ;
		}
		wchNext = GetCharacter();
	}
	if ( (wchNext != L'.') && ((wchNext < L'0') || (wchNext > L'9')) )
	{
		m_nIndex = nSaveIndex ;
		return	-1 ;
	}

	// Progress the index while number string
	int	nMaxNumber = -1, nNextMaxNumber = -1 ;
	int	nDecimalFlag = 0 ;
	wchar_t	wchLast = -1 ;
	while(	((wchNext >= L'0') && (wchNext <= L'9')) ||
			((wchNext >= L'A') && (wchNext <= L'F')) ||
			((wchNext >= L'a') && (wchNext <= L'f')) ||
			(wchNext == L'.')	)
	{
		if( wchNext == L'.' )
		{
			if( (++ nDecimalFlag) > 1 )
				break ;
		}
		int		nNum = -1 ;
		if ( (wchNext >= L'0') && (wchNext <= L'9') )
			nNum = wchNext - L'0' ;
		else if ( (wchNext >= L'A') && (wchNext <= L'F') )
			nNum = wchNext - L'A' + 10 ;
		else if ( (wchNext >= L'a') && (wchNext <= L'f') )
			nNum = wchNext - L'a' + 10 ;
		nMaxNumber = nNextMaxNumber ;
		if ( nNum > nNextMaxNumber )
			nNextMaxNumber = nNum ;
		//
		wchLast = wchNext ;
		if( m_nIndex >= m_nLength )
			break ;
		wchNext = GetCharacter( ) ;
	}
	m_nIndex = nSaveIndex ;
	//
	if ( (nMaxNumber < 0) && (nNextMaxNumber < 0) )
		return	-1 ;

	if ( (nMaxNumber < 8) && ((wchNext == L'O') || (wchNext == L'o')) )
		return	8 ;		// octagonal
	if ( (nMaxNumber < 10) && ((wchNext == L'T') || (wchNext == L't')) )
		return	10 ;	// 10 radix
	if ( (nMaxNumber < 16) && ((wchNext == L'H') || (wchNext == L'h')) )
		return	16 ;	// hexagonal
	if ( (nMaxNumber < 2) && ((wchLast == L'B') || (wchLast == L'b')) )
		return	2 ;		// binary
	if ( ((wchLast >= L'0') && (wchLast <= L'9')) || (wchLast == L'.') )
	{
		if ( nMaxNumber < 10 )
			return	0 ;		// default radix
	}

	return	-1 ;		// error!
}

//
// 次の数値（実数）を取得
//////////////////////////////////////////////////////////////////////////////

REAL64 EStreamWideString::GetRealNumber( int nRadix )
{
	//
	// 基底を取得
	//
	if ( nRadix == 0 )
	{
		nRadix = GetNumberRadix() ;
	}
	if ( nRadix < 0 )
	{
		return	0.0 ;
	}
	else if	( nRadix == 0 )
	{
		nRadix = 10 ;
	}
	//
	// 符号を取得
	//
	bool	boolSignFlag = false ;
	unsigned int	nLastIndex ;
	if ( DisregardSpace() )
	{
		return	0.0 ;
	}
	nLastIndex = m_nIndex ;
	wchar_t	wchNext = GetCharacter( ) ;
	if( (wchNext == L'+') || (wchNext == L'-') )
	{
		if ( wchNext == L'-' )
		{
			boolSignFlag = true ;
		}
		if ( DisregardSpace() )
		{
			return	0.0 ;
		}
		nLastIndex = m_nIndex ;
		wchNext = GetCharacter( ) ;
	}
	//
	// 数値化
	//
	int		nDecimalFlag = 0 ;
	double	dblNumber = 0.0 ;
	double	dblDecimal = 1.0, dblIRadix = 1.0 / nRadix ;
	for ( ; ; )
	{
		int		numCurrent ;
		if		( (wchNext >= L'0') && (wchNext <= L'9') )
		{
			numCurrent = wchNext - L'0' ;
		}
		else if	( (wchNext >= L'A') && (wchNext <= L'F') )
		{
			numCurrent = wchNext - (L'A' - 10) ;
		}
		else if	( (wchNext >= L'a') && (wchNext <= L'f') )
		{
			numCurrent = wchNext - (L'a' - 10) ;
		}
		else if ( wchNext != L'.' )
		{
			break ;
		}
		if ( wchNext == L'.' )
		{
			if ( nDecimalFlag ++ )
				break ;
		}
		else
		{
			if ( numCurrent >= nRadix )
				break ;

			if ( !nDecimalFlag )
			{
				dblNumber = dblNumber * nRadix + numCurrent ;
			}
			else
			{
				dblNumber +=
					((dblDecimal = dblDecimal * dblIRadix) * numCurrent) ;
			}
		}

		nLastIndex = m_nIndex ;
		if( m_nIndex >= m_nLength )
			break ;
		wchNext = GetCharacter( ) ;
	}

	if ( (wchNext != L'B') && (wchNext != L'b') &&
			(wchNext != L'O') && (wchNext != L'o') &&
			(wchNext != L'T') && (wchNext != L't') &&
			(wchNext != L'H') && (wchNext != L'h') )
	{
		m_nIndex = nLastIndex ;
	}

	if ( boolSignFlag )
	{
		return	- dblNumber ;
	}
	else
	{
		return	dblNumber ;
	}
}

INT64 EStreamWideString::GetLargeInteger( int nRadix )
{
	//
	// 基底を取得
	//
	if ( nRadix == 0 )
	{
		nRadix = GetNumberRadix() ;
	}
	if ( nRadix < 0 )
	{
		return	0 ;
	}
	else if	( nRadix == 0 )
	{
		nRadix = 10 ;
	}
	//
	// 符号を取得
	//
	bool	boolSignFlag = false ;
	unsigned int	nLastIndex ;
	if ( DisregardSpace() )
	{
		return	0 ;
	}
	nLastIndex = m_nIndex ;
	wchar_t	wchNext = GetCharacter( ) ;
	if( (wchNext == L'+') || (wchNext == L'-') )
	{
		if ( wchNext == L'-' )
		{
			boolSignFlag = true ;
		}
		if ( DisregardSpace() )
		{
			return	0 ;
		}
		nLastIndex = m_nIndex ;
		wchNext = GetCharacter( ) ;
	}
	//
	// 数値化
	//
	INT64	nNumber = 0 ;
	for ( ; ; )
	{
		int		nCurrent ;
		if		( (wchNext >= L'0') && (wchNext <= L'9') )
		{
			nCurrent = wchNext - L'0' ;
		}
		else if	( (wchNext >= L'A') && (wchNext <= L'F') )
		{
			nCurrent = wchNext - (L'A' - 10) ;
		}
		else if	( (wchNext >= L'a') && (wchNext <= L'f') )
		{
			nCurrent = wchNext - (L'a' - 10) ;
		}
		else
		{
			break ;
		}
		nNumber = nNumber * nRadix + nCurrent ;
		nLastIndex = m_nIndex ;
		if( m_nIndex >= m_nLength )
			break ;
		wchNext = GetCharacter( ) ;
	}

	if ( (wchNext != L'B') && (wchNext != L'b') &&
			(wchNext != L'O') && (wchNext != L'o') &&
			(wchNext != L'T') && (wchNext != L't') &&
			(wchNext != L'H') && (wchNext != L'h') )
	{
		m_nIndex = nLastIndex ;
	}

	if ( boolSignFlag )
	{
		return	- nNumber ;
	}
	else
	{
		return	nNumber ;
	}
}

int EStreamWideString::GetInteger( int nRadix )
{
	return	(int) EStreamWideString::GetLargeInteger( nRadix ) ;
}

//
// 次に来るべき文字を通過する
//////////////////////////////////////////////////////////////////////////////

wchar_t EStreamWideString::HasToComeChar( const wchar_t * pwszNext )
{
	if ( DisregardSpace( ) )
	{
		return	L'\0' ;
	}
	wchar_t	wchNext = CurrentCharacter( ) ;
	while ( *pwszNext != L'\0' )
	{
		if ( *(pwszNext ++) == wchNext )
		{
			++ m_nIndex ;
			return	wchNext ;
		}
	}
	return	L'\0' ;
}

bool EStreamWideString::HasToComeToken( const wchar_t * pwszNext )
{
	unsigned int	nLastIndex = m_nIndex ;
	if ( GetAToken() == pwszNext )
	{
		return	true ;
	}
	m_nIndex = nLastIndex ;
	return	false ;
}

//
// パラメータ数の取得
//////////////////////////////////////////////////////////////////////////////
int EStreamWideString::EUsage::GetUsageParamCount( void ) const
{
	if ( m_type == utBeginParam )
	{
		return	1 ;
	}
	int	nCount = 0 ;
	for ( int i = 0; i < (int) GetSize(); i ++ )
	{
		ESLAssert( GetAt(i) != NULL ) ;
		nCount += GetAt(i)->GetUsageParamCount( ) ;
	}
	return	nCount ;
}

// 子オブジェクトの指標を取得する
//////////////////////////////////////////////////////////////////////////////
int EStreamWideString::EUsage::GetChildIndex( EUsage * pUsage ) const
{
	int	i, nCount = GetSize( ) ;
	for ( i = 0; i < nCount; i ++ )
	{
		if ( GetAt(i) == pUsage )
		{
			return	i ;
		}
	}
	return	-1 ;
}

//
// 次の書式判定
//////////////////////////////////////////////////////////////////////////////
ESLError EStreamWideString::IsMatchUsage
	( EStreamWideString & swsUsage, EString & strErrMsg,
						EObjArray<EWideString> * plstParam )
{
	//
	// 書式構文の解析
	//
	ESLError	err ;
	EUsage		lstUsage ;
	err = ParseUsage( lstUsage, swsUsage, strErrMsg ) ;
	if ( err )
	{
		strErrMsg = "書式構文のエラー：" + strErrMsg ;
		err = ESLErrorMsg( strErrMsg ) ;
		return	err ;
	}
	//
	// 書式を判定する
	//
	if ( plstParam != NULL )
	{
		plstParam->RemoveAll( ) ;
	}
	return	IsMatchUsageList( lstUsage, 0, strErrMsg, plstParam ) ;
}

// 書式の検索
//////////////////////////////////////////////////////////////////////////////
int EStreamWideString::FindMatchUsage
	( EStreamWideString & swsUsage,
		EStreamWideString::UsageType utWildCardType )
{
	//
	// 書式構文の解析
	//
	ESLError	err ;
	EUsage		lstUsage ;
	EString		strErrMsg ;
	err = ParseUsage( lstUsage, swsUsage, strErrMsg ) ;
	if ( err )
	{
		return	-1 ;
	}
	//
	// 書式を判定する
	//
	return	FindMatchUsageList( lstUsage, 0, utWildCardType ) ;
}

// 書式の置き換え
//////////////////////////////////////////////////////////////////////////////
ESLError EStreamWideString::ReplaceUsage
	( EWideString & wstrReplaced, EStreamWideString & swsUsage,
		const wchar_t * pwszReplace,
		EStreamWideString::UsageType utWildCardType )
{
	//
	// 書式構文の解析
	//
	ESLError	err ;
	EUsage		lstUsage ;
	EString		strErrMsg ;
	err = ParseUsage( lstUsage, swsUsage, strErrMsg ) ;
	if ( err )
	{
		return	ESLErrorMsg( "書式構文のエラー" ) ;
	}
	//
	// 一致部分を順次置き換え
	//
	ESLError		errResult = eslErrSuccess ;
	unsigned int	iLast = GetIndex() ;
	EWideString		wstrReplace ;
	wstrReplaced = L"" ;
	for ( ; ; )
	{
		//
		// 書式の先頭一致部分検出
		//
		int	iFind = FindMatchUsageList( lstUsage, 0, utWildCardType ) ;
		if ( iFind < 0 )
		{
			break ;
		}
		//
		// 書式の完全一致判定
		//
		EObjArray<EWideString>	lstParam ;
		int	iNext = GetIndex( ) ;
		MoveIndex( iFind ) ;
		if ( IsMatchUsageList( lstUsage, 0, strErrMsg, &lstParam ) )
		{
			MoveIndex( iNext ) ;
			continue ;
		}
		//
		// 置き換えない部分を追加
		//
		wstrReplaced += Middle( iLast, iFind - iLast ) ;
		iLast = GetIndex( ) ;
		//
		// 置き換える文字列を正規化する
		//
		wstrReplace = pwszReplace ;
		errResult = OnReplaceUsage( wstrReplace, lstParam ) ;
		if ( err )
		{
			break ;
		}
		wstrReplaced += wstrReplace ;
	}
	if ( !errResult )
	{
		wstrReplaced += Middle( iLast ) ;
	}
	return	errResult ;
}

// 書式の置き換え判定
//////////////////////////////////////////////////////////////////////////////
ESLError EStreamWideString::OnReplaceUsage
	( EWideString & wstrReplace,
		const EObjArray<EWideString> & lstParam )
{
	int	iFind = wstrReplace.Find( L'%' ) ;
	if ( iFind >= 0 )
	{
		EWideString	wstrTemp ;
		int			iLast = 0 ;
		do
		{
			int	iEnd = wstrReplace.Find( L'%', iFind + 1 ) ;
			if ( iEnd < 0 )
			{
				break ;
//				return	ESLErrorMsg
//					( "置き換え文字列の書式に誤りがあります。" ) ;
			}
			wstrTemp += wstrReplace.Middle( iLast, iFind - iLast ) ;
			if ( iEnd > iFind + 1 )
			{
				int	num = wstrReplace.Middle
					( iFind + 1, iEnd - iFind - 1 ).AsInteger() ;
				if ( (num >= 0) && (num < (int) lstParam.GetSize()) )
				{
					wstrTemp += lstParam[num] ;
				}
				iLast = iEnd + 1 ;
			}
			else
			{
				iLast = iEnd ;
			}
			iFind = wstrReplace.Find( L'%', iEnd + 1 ) ;
		}
		while ( iFind >= 0 ) ;
		//
		wstrTemp += wstrReplace.Middle( iLast ) ;
		wstrReplace = wstrTemp ;
	}
	return	eslErrSuccess ;
}

// 書式の一致判定
//////////////////////////////////////////////////////////////////////////////
ESLError EStreamWideString::IsMatchAUsage
	( EUsage * pUsage, EString * pstrErrMsg,
			EObjArray<EWideString> * plstParam )
{
	ESLError		err = eslErrSuccess ;
	int				iOrgIndex = GetIndex( ) ;
	const wchar_t *	pwszCmp ;
	const wchar_t *	pwszStr ;
	int				nCmpStrLen, iLastIndex ;
	int				i, nCount ;
	//
	ESLAssert( pUsage != NULL ) ;
	switch ( pUsage->m_type )
	{
	case	utToken:
	case	utString:
		pwszCmp = m_pszString + m_nIndex ;
		pwszStr = pUsage->m_wstr ;
		if ( pUsage->m_type == utToken )
		{
			iLastIndex = m_nIndex ;
			PassAToken( ) ;
			nCmpStrLen = m_nIndex - iLastIndex ;
		}
		else
		{
			nCmpStrLen = pUsage->m_wstr.GetLength() ;
			m_nIndex += nCmpStrLen ;
			if ( m_nIndex > (int) m_nLength )
			{
				m_nIndex = m_nLength ;
			}
		}
		if ( pUsage->m_flags & ufNoCase )
		{
			int		i, nCmpStr = -1 ;
			if ( pwszCmp && pwszStr
				&& ((int) pUsage->m_wstr.GetLength() == nCmpStrLen) )
			{
				for ( i = 0; i < nCmpStrLen; i ++ )
				{
					wchar_t	wch1 = pwszCmp[i] ;
					wchar_t	wch2 = pwszStr[i] ;
					if ( (wch1 >= L'a') && (wch1 <= L'z') )
					{
						wch1 -= L'a' - L'A' ;
					}
					if ( (wch2 >= L'a') && (wch2 <= L'z') )
					{
						wch2 -= L'a' - L'A' ;
					}
					if ( wch1 != wch2 )
					{
						break ;
					}
				}
				nCmpStr = (i < nCmpStrLen) ;
			}
			if ( nCmpStr )
			{
				if ( pstrErrMsg != NULL )
				{
					if ( pwszCmp != NULL )
					{
						for ( i = 0; i < nCmpStrLen; i ++ )
						{
							if ( pwszCmp[i] == L'\0' )
							{
								nCmpStrLen = i ;
								break ;
							}
						}
					}
					else
					{
						nCmpStrLen = 0 ;
					}
					*pstrErrMsg = EString(pwszCmp,nCmpStrLen) + " が "
							+ EString(pUsage->m_wstr) + " と一致しません。" ;
					err = ESLErrorMsg( *pstrErrMsg ) ;
				}
				else
				{
					err = ESLErrorMsg( "文字列が一致しません。" ) ;
				}
			}
		}
		else
		{
			int		i, nCmpStr = -1 ;
			if ( pwszCmp && pwszStr
				&& ((int) pUsage->m_wstr.GetLength() == nCmpStrLen) )
			{
				for ( i = 0; i < nCmpStrLen; i ++ )
				{
					wchar_t	wch1 = pwszCmp[i] ;
					wchar_t	wch2 = pwszStr[i] ;
					if ( wch1 != wch2 )
					{
						break ;
					}
				}
				nCmpStr = (i < nCmpStrLen) ;
			}
			if ( nCmpStr )
			{
				if ( pstrErrMsg != NULL )
				{
					if ( pwszCmp != NULL )
					{
						for ( i = 0; i < nCmpStrLen; i ++ )
						{
							if ( pwszCmp[i] == L'\0' )
							{
								nCmpStrLen = i ;
								break ;
							}
						}
					}
					else
					{
						nCmpStrLen = 0 ;
					}
					*pstrErrMsg = EString(pwszCmp,nCmpStrLen) + " が "
							+ EString(pUsage->m_wstr) + " と一致しません。" ;
					err = ESLErrorMsg( *pstrErrMsg ) ;
				}
				else
				{
					err = ESLErrorMsg( "文字列が一致しません。" ) ;
				}
			}
		}
		break ;
	case	utSpace:
		DisregardSpace( ) ;
		break ;
	case	utEnclosed:
		PassEnclosedString( pUsage->m_wstr.GetAt(0) ) ;
		if ( pUsage->m_wstr.GetAt(0) != CurrentCharacter() )
		{
			if ( pstrErrMsg != NULL )
			{
				*pstrErrMsg = EString(pUsage->m_wstr) + " 記号が見つかりません。" ;
				err = ESLErrorMsg( *pstrErrMsg ) ;
			}
			else
			{
				err = ESLErrorMsg( "閉じ記号が見つかりません。" ) ;
			}
		}
		break ;
	case	utWildCardToken:
		PassAToken( ) ;
		break ;
	case	utWildCardUsage:
	case	utWildCardExpression:
		break ;
	case	utNumberUsage:
		if ( !DisregardSpace() )
		{
			err = ESLErrorMsg( "10進数字列ではありません。" ) ;
			iLastIndex = m_nIndex ;
			HasToComeChar( L"+-" ) ;
			DisregardSpace( ) ;
			for ( ; ; )
			{
				wchar_t	wch = CurrentCharacter( ) ;
				if ( (wch < L'0') || (wch > L'9') )
				{
					break ;
				}
				err = eslErrSuccess ;
				GetCharacter( ) ;
			}
			if ( err && pstrErrMsg )
			{
				*pstrErrMsg = EString( GetESLErrorMsg(err) ) ;
				err = ESLErrorMsg( *pstrErrMsg ) ;
			}
		}
		break ;
	case	utEndOfUsage:
		if ( !DisregardSpace() )
		{
			if ( pstrErrMsg != NULL )
			{
				*pstrErrMsg = "末尾に不正な文字を発見しました。" ;
				err = ESLErrorMsg( *pstrErrMsg ) ;
			}
			else
			{
				err = ESLErrorMsg( "末尾に不正な文字を発見しました。" ) ;
			}
		}
		break ;
	case	utCharacters:
		{
			bool	fLogic ;
			bool	fNegative = ((pUsage->m_flags & ufNegative) != 0) ;
			int		nRepeatCount = 0 ;
			while ( !IsIndexOverflow() )
			{
				wchar_t	wchNext = CurrentCharacter( ) ;
				if ( wchNext < 0x80 )
				{
					fLogic =
						((pUsage->m_mask[(wchNext >> 5)]
							& (1 << (wchNext & 0x1F))) != 0) ;
				}
				else
				{
					fLogic = false ;
					for ( int i = 0; i < (int) pUsage->m_wstr.GetLength(); i ++ )
					{
						if ( pUsage->m_wstr.GetAt(i) == wchNext )
						{
							fLogic = true ;
							break ;
						}
					}
				}
				if ( !(fLogic ^ fNegative) )
				{
					break ;
				}
				GetCharacter( ) ;
				nRepeatCount ++ ;
				if ( !(pUsage->m_flags & ufRepeat) )
				{
					break ;
				}
			}
			if ( nRepeatCount == 0 )
			{
				if ( pstrErrMsg != NULL )
				{
					*pstrErrMsg = "文字コードが適合しません。" ;
					err = ESLErrorMsg( *pstrErrMsg ) ;
				}
				else
				{
					err = ESLErrorMsg( "文字コードが適合しません。" ) ;
				}
			}
		}
		break ;
	case	utRepeat:
	case	utBeginParam:
	case	utEndParam:
		err = eslErrGeneral ;
		break ;
	case	utOmittable:
		{
			EObjArray<EWideString>	lstParam ;
			EString					strErrMsg ;
			err = IsMatchUsageList( *pUsage, 0, strErrMsg, &lstParam ) ;
			if ( !err && plstParam )
			{
				plstParam->Merge( plstParam->GetSize(), lstParam ) ;
			}
		}
		break ;
	case	utSelectable:
		err = eslErrGeneral ;
		nCount = pUsage->GetSize( ) ;
		for ( i = 0; i < nCount; i ++ )
		{
			ESLAssert( pUsage->GetAt(i) != NULL ) ;
			err = IsMatchAUsage( pUsage->GetAt(i), NULL, plstParam ) ;
			if ( !err )
				break ;
		}
		break ;
	}
	if ( err )
	{
		MoveIndex( iOrgIndex ) ;
	}
	return	err ;
}

ESLError EStreamWideString::IsMatchUsageList
( EStreamWideString::EUsage & usage, int iStart,
		EString & strErrMsg, EObjArray<EWideString> * plstParam )
{
	ESLError	err = eslErrSuccess ;
	EWideString	wstrToken ;
	EString		strError ;
	int			iOrgIndex = GetIndex( ) ;
	int			iParamBegin = -1 ;
	int			i, nCount ;
	bool		fSelected ;
	EObjArray<EWideString>	lstParam ;
	//
	for ( int iUsage = iStart; iUsage < (int) usage.GetSize(); iUsage ++ )
	{
		EUsage *	pUsage = usage.GetAt( iUsage ) ;
		ESLAssert( pUsage != NULL ) ;
		switch ( pUsage->m_type )
		{
		case	utWildCardUsage:
		case	utWildCardExpression:
			i = FindMatchUsageList
				( usage, iUsage + 1,
					(EStreamWideString::UsageType) pUsage->m_type ) ;
			if ( i < 0 )
			{
				strErrMsg = "適合する書式が見つかりません。" ;
				err = ESLErrorMsg( strErrMsg ) ;
				break ;
			}
			MoveIndex( i ) ;
			break ;
		case	utBeginParam:
			ESLAssert( iParamBegin < 0 ) ;
			iParamBegin = GetIndex( ) ;
			break ;
		case	utEndParam:
//			ESLAssert( iParamBegin >= 0 ) ;
			if ( iParamBegin < 0 )
			{
//				strErrMsg = "パラメータの開始位置が見つかりません。" ;
//				err = ESLErrorMsg( strErrMsg ) ;
			}
			else if ( plstParam != NULL )
			{
				plstParam->Add
					( new EWideString( Middle
						( iParamBegin, GetIndex() - iParamBegin ) ) ) ;
				iParamBegin = -1 ;
			}
			break ;
		case	utRepeat:
			pUsage = usage.GetAt( iUsage - 1 ) ;
			if ( pUsage != NULL )
			{
				for ( ; ; )
				{
					EObjArray<EWideString>	lstParam ;
					if ( IsMatchAUsage( pUsage, NULL, &lstParam ) )
					{
						break ;
					}
					if ( plstParam != NULL )
					{
						plstParam->Merge( plstParam->GetSize(), lstParam ) ;
					}
				}
			}
			break ;
		case	utOmittable:
			lstParam.RemoveAll( ) ;
			err = IsMatchUsageList( *pUsage, 0, strError, &lstParam ) ;
			if ( plstParam != NULL )
			{
				if ( err )
				{
					nCount = pUsage->GetUsageParamCount( ) ;
					for ( i = 0; i < nCount; i ++ )
					{
						plstParam->Add( new EWideString ) ;
					}
				}
				else
				{
					plstParam->Merge( plstParam->GetSize(), lstParam ) ;
				}
			}
			err = eslErrSuccess ;
			break ;
		case	utSelectable:
			fSelected = false ;
			nCount = pUsage->GetSize( ) ;
			for ( i = 0; i < nCount; i ++ )
			{
				EUsage *	pSub = pUsage->GetAt( i ) ;
				ESLAssert( pSub != NULL ) ;
				if ( !fSelected )
				{
					lstParam.RemoveAll( ) ;
					if ( !IsMatchUsageList( *pSub, 0, strErrMsg, &lstParam ) )
					{
						if ( plstParam != NULL )
						{
							plstParam->Merge
								( plstParam->GetSize(), lstParam ) ;
						}
						fSelected = true ;
						continue ;
					}
				}
				if ( plstParam != NULL )
				{
					int	j, m = pSub->GetUsageParamCount( ) ;
					for ( j = 0; j < m; j ++ )
					{
						plstParam->Add( new EWideString ) ;
					}
				}
			}
			if ( fSelected )
			{
				strErrMsg = "" ;
			}
			break ;
		case	utList:
			lstParam.RemoveAll( ) ;
			err = IsMatchUsageList( *pUsage, 0, strErrMsg, plstParam ) ;
			if ( !err )
			{
				if ( plstParam != NULL )
				{
					plstParam->Merge
						( plstParam->GetSize(), lstParam ) ;
				}
			}
			break ;
		default:
			err = IsMatchAUsage( pUsage, &strErrMsg ) ;
			break ;
		}
		if ( err )
		{
			break ;
		}
	}
	if ( err )
	{
		MoveIndex( iOrgIndex ) ;
	}
	return	err ;
}

int EStreamWideString::FindMatchUsageList
	( EStreamWideString::EUsage & usage, int iStart,
		EStreamWideString::UsageType utWildCardType )
{
	//
	// 次の有効な書式を見つける
	//
	EUsage *	pUsage ;
	for ( ; ; )
	{
		pUsage = usage.GetAt( iStart ) ;
		if ( pUsage == NULL )
		{
			EUsage *	pParent = usage.m_parent ;
			EUsage *	pChild = &usage ;
			while ( pParent != NULL )
			{
				int	nIndex = pParent->GetChildIndex( pChild ) ;
				ESLAssert( nIndex >= 0 ) ;
				if ( (pParent->m_type == utList)
					|| (pParent->m_type == utOmittable) )
				{
					return	FindMatchUsageList
						( *pParent, nIndex + 1, utWildCardType ) ;
				}
				else if ( pParent->m_parent == NULL )
				{
					return	-1 ;
				}
				pChild = pParent ;
				pParent = pParent->m_parent ;
			}
			return	-1 ;
		}
		if ( pUsage->m_type == utRepeat )
		{
			EUsage *	pPrevUsage = usage.GetAt( iStart - 1 ) ;
			if ( pPrevUsage != NULL )
			{
				int	iFind[2] ;
				iFind[0] = FindMatchAUsage
					( NULL, -1, pPrevUsage, utWildCardType ) ;
				if ( iFind >= 0 )
				{
					iFind[1] = FindMatchUsageList
						( usage, iStart + 1, utWildCardType ) ;
					if ( iFind[1] < 0 )
					{
						return	iFind[0] ;
					}
					return	(iFind[0] < iFind[1]) ? iFind[0] : iFind[1] ;
				}
			}
		}
		if ( (pUsage->m_type != utSpace)
			&& (pUsage->m_type != utRepeat)
			&& (pUsage->m_type != utBeginParam)
			&& (pUsage->m_type != utEndParam) )
		{
			break ;
		}
		iStart ++ ;
	}
	return	FindMatchAUsage( &usage, iStart, pUsage, utWildCardType ) ;
}

int EStreamWideString::FindMatchAUsage
	( EUsage * pParent, int nIndex,
			EUsage * pUsage, UsageType utWildCardType )
{
	//
	// 書式の一致判定を行う
	//
	if ( pUsage->m_type == utOmittable )
	{
		int	iFind = FindMatchUsageList( *pUsage, 0, utWildCardType ) ;
		if ( iFind >= 0 )
		{
			return	iFind ;
		}
		if ( pParent == NULL )
		{
			return	-1 ;
		}
		return	FindMatchUsageList( *pParent, nIndex + 1, utWildCardType ) ;
	}
	else if ( pUsage->m_type == utSelectable )
	{
		int	iFirstFind = GetLength() ;
		for ( int i = 0; i < (int) pUsage->GetSize(); i ++ )
		{
			EUsage *	pSelectUsage = pUsage->GetAt( i ) ;
			ESLAssert( pSelectUsage != NULL ) ;
			int	iFind = FindMatchUsageList( *pSelectUsage, 0, utWildCardType ) ;
			if ( (iFind < iFirstFind) && (iFind >= 0) )
			{
				iFirstFind = iFind ;
			}
		}
		if ( iFirstFind < (int) GetLength() )
		{
			return	iFirstFind ;
		}
		return	-1 ;
	}
	//
	// 単純な書式の一致判定
	//
	ESLError	err = eslErrSuccess ;
	int			iOrgIndex = GetIndex( ) ;
	int			iFindIndex = -1 ;
	for ( ; ; )
	{
		iFindIndex = GetIndex( ) ;
		err = IsMatchAUsage( pUsage, NULL ) ;
		if ( !err )
		{
			break ;
		}
		if ( IsIndexOverflow() )
		{
			iFindIndex = -1 ;
			break ;
		}
		if ( (pUsage->m_type == utCharacters)
				|| (pUsage->m_type == utString) )
		{
			GetCharacter( ) ;
		}
		else if ( utWildCardType != utWildCardExpression )
		{
			PassAExpressionTerm( ) ;
			DisregardSpace( ) ;
		}
		else
		{
			PassAToken( ) ;
			DisregardSpace( ) ;
		}
	}
	MoveIndex( iOrgIndex ) ;
	return	iFindIndex ;
}

// 書式構文の解析
//////////////////////////////////////////////////////////////////////////////
ESLError EStreamWideString::ParseUsage
	( EStreamWideString::EUsage & lstUsage,
		EStreamWideString & swsUsage, EString & strErrMsg ) const
{
	ESLError	err = eslErrSuccess ;
	EUsage *	pUsage ;
	int			iOrgUsage = swsUsage.GetIndex( ) ;
	int			iParamBegin = -1 ;
	//
	while ( !swsUsage.IsIndexOverflow() )
	{
		//
		// 次の書式を判定する
		//
		int	iCurrentUsage = swsUsage.GetIndex( ) ;
		wchar_t	wchNext = swsUsage.GetCharacter( ) ;
		if ( IsCharacterSpace( wchNext ) )
		{
			//
			// 空白：トークンの切れ目
			//
			lstUsage.Add( new EUsage( utSpace, &lstUsage ) ) ;
			swsUsage.DisregardSpace( ) ;
			continue ;
		}
		else if ( wchNext == L'%' )
		{
			//
			// 任意タイプのトークン
			//
			wchNext = swsUsage.GetCharacter() ;
			if ( wchNext == L't' )
			{
				//
				// 任意トークン
				//
				lstUsage.Add( new EUsage( utWildCardToken, &lstUsage ) ) ;
			}
			else if ( wchNext == L'n' )
			{
				//
				// 10進数字文字列
				//
				pUsage = new EUsage( utNumberUsage, &lstUsage ) ;
				lstUsage.Add( pUsage ) ;
			}
			else if ( wchNext == L'c' )
			{
				//
				// 指定文字コードまで
				//
				wchNext = swsUsage.GetCharacter( ) ;
				pUsage = new EUsage( utEnclosed, &lstUsage ) ;
				pUsage->m_wstr = wchNext ;
				lstUsage.Add( pUsage ) ;
			}
			else if ( wchNext == L's' )
			{
				//
				// 任意の書式
				//
				lstUsage.Add( new EUsage( utWildCardUsage, &lstUsage ) ) ;
			}
			else if ( wchNext == L'x' )
			{
				//
				// 任意の式
				//
				lstUsage.Add( new EUsage( utWildCardExpression, &lstUsage ) ) ;
			}
			else if ( ((wchNext >= L'0') && (wchNext <= L'9'))
					|| ((wchNext >= L'A') && (wchNext <= L'F'))
					|| ((wchNext >= L'a') && (wchNext <= L'f')) )
			{
				//
				// 任意の文字コード
				//
				wchar_t	wchCode = 0 ;
				if ( (wchNext >= L'0') && (wchNext <= L'9') )
					wchCode = (wchar_t) (wchNext - L'0') ;
				else if ( (wchNext >= L'A') && (wchNext <= L'F') )
					wchCode = (wchar_t) (wchNext - L'A' + 10) ;
				else //if ( (wchNext >= L'A') && (wchNext <= L'F') )
					wchCode = (wchar_t) (wchNext - L'a' + 10) ;
				//
				wchNext = swsUsage.GetCharacter() ;
				wchCode <<= 4 ;
				if ( (wchNext >= L'0') && (wchNext <= L'9') )
					wchCode += (wchar_t) (wchNext - L'0') ;
				else if ( (wchNext >= L'A') && (wchNext <= L'F') )
					wchCode += (wchar_t) (wchNext - L'A' + 10) ;
				else if ( (wchNext >= L'A') && (wchNext <= L'F') )
					wchCode += (wchar_t) (wchNext - L'a' + 10) ;
				else
				{
					strErrMsg = "%## による文字コードの書式が不正です。" ;
					err = eslErrGeneral ;
					break ;
				}
				pUsage = new EUsage( utString, &lstUsage ) ;
				pUsage->m_wstr = wchNext ;
				lstUsage.Add( pUsage ) ;
			}
			else
			{
				strErrMsg = "%" + EString( EWideString( wchNext ) )
						+ " は定義されていないトークンタイプの指定です。" ;
				err = eslErrGeneral ;
				break ;
			}
		}
		else if ( wchNext == L'\\' )
		{
			//
			// 任意の1文字
			//
			wchNext = swsUsage.GetCharacter( ) ;
			if ( wchNext != L'\0' )
			{
				pUsage = new EUsage( utString, &lstUsage ) ;
				pUsage->m_wstr = wchNext ;
				lstUsage.Add( pUsage ) ;
			}
			else
			{
				lstUsage.Add( new EUsage( utEndOfUsage, &lstUsage ) ) ;
				break ;
			}
		}
		else if ( wchNext == L'<' )
		{
			//
			// 任意の文字：合致文字コード取得
			//
			pUsage = new EUsage( utCharacters, &lstUsage ) ;
			lstUsage.Add( pUsage ) ;
			pUsage->m_mask[0] = 0 ;
			pUsage->m_mask[1] = 0 ;
			pUsage->m_mask[2] = 0 ;
			pUsage->m_mask[3] = 0 ;
			//
			wchNext = swsUsage.GetCharacter( ) ;
			if ( wchNext == L'-' )
			{
				wchNext = swsUsage.GetCharacter( ) ;
				pUsage->m_flags |= ufNegative ;
			}
			wchar_t	wchLastCode = L'\0' ;
			while ( !swsUsage.IsIndexOverflow() && (wchNext != L'>') )
			{
				if ( wchNext == L'\\' )
				{
					wchNext = swsUsage.GetCharacter( ) ;
				}
				else if ( wchNext == L'%' )
				{
					wchar_t	wch ;
					int		i ;
					wchNext = 0 ;
					for ( i = 0; i < 2; i ++ )
					{
						wch = swsUsage.GetCharacter( ) ;
						wchNext <<= 4 ;
						if ( (wch >= L'0') && (wch <= L'9') )
							wchNext += (wchar_t) (wch - L'0') ;
						else if ( (wch >= L'A') && (wch <= L'F') )
							wchNext += (wchar_t) (wch - L'A' + 10) ;
						else if ( (wch >= L'A') && (wch <= L'F') )
							wchNext += (wchar_t) (wch - L'a' + 10) ;
						else
						{
							swsUsage.MoveIndex( swsUsage.GetIndex() - 1 ) ;
							break ;
						}
					}
					if ( i == 0 )
					{
						wchNext = L'%' ;
					}
				}
				else if ( wchNext == L'-' )
				{
					wchNext = swsUsage.GetCharacter( ) ;
					if ( wchNext < 0x80 )
					{
						while ( wchLastCode < wchNext )
						{
							pUsage->m_mask[(wchLastCode >> 5)]
									|= (1 << (wchLastCode & 0x1F)) ;
							wchLastCode ++ ;
						}
					}
				}
				if ( wchNext < 0x80 )
				{
					pUsage->m_mask[(wchNext >> 5)]
							|= (1 << (wchNext & 0x1F)) ;
				}
				else
				{
					pUsage->m_wstr += wchNext ;
				}
				wchLastCode = wchNext ;
				wchNext = swsUsage.GetCharacter( ) ;
			}
			if ( swsUsage.CurrentCharacter() == L'*' )
			{
				swsUsage.GetCharacter( ) ;
				pUsage->m_flags |= ufRepeat ;
			}
		}
		else if ( wchNext == L'&' )
		{
			//
			// 文字の完全一致
			//
			pUsage = new EUsage( utString, &lstUsage ) ;
			lstUsage.Add( pUsage ) ;
			wchNext = swsUsage.GetCharacter( ) ;
			if ( wchNext == L'+' )
			{
				pUsage->m_flags |= ufNoCase ;
				wchNext = swsUsage.GetCharacter( ) ;
			}
			else if ( wchNext == L'!' )
			{
				pUsage->m_type = utToken ;
				pUsage->m_flags |= ufNoCase ;
				wchNext = swsUsage.GetCharacter( ) ;
			}
			while ( !swsUsage.IsIndexOverflow() && (wchNext != L'&') )
			{
				if ( wchNext == L'\\' )
				{
					wchNext = swsUsage.GetCharacter( ) ;
				}
				pUsage->m_wstr += wchNext ;
				wchNext = swsUsage.GetCharacter( ) ;
			}
		}
		else if ( wchNext == L'(' )
		{
			//
			// パラメータの開始
			//
			if ( iParamBegin >= 0 )
			{
				strErrMsg = "() 括弧が二重に定義されています。" ;
				err = eslErrGeneral ;
				break ;
			}
			iParamBegin = 0 ;
			lstUsage.Add( new EUsage( utBeginParam, &lstUsage ) ) ;
		}
		else if ( wchNext == L')' )
		{
			//
			// パラメータの終了
			//
			if ( iParamBegin < 0 )
			{
				strErrMsg = "\')\' 括弧に対応する \'(\' 括弧がありません。" ;
				err = eslErrGeneral ;
				break ;
			}
			iParamBegin = -1 ;
			lstUsage.Add( new EUsage( utEndParam, &lstUsage ) ) ;
		}
		else if ( (wchNext == L'[') || (wchNext == L'{') )
		{
			//
			// 省略・選択書式の判定
			//
			EUsage *	pSubUsage ;
			int		iNestUsage = swsUsage.GetIndex( ) ;
			int		iEndUsage ;
			int		nNestCount = 1 ;
			int		nUsageParams = 0 ;
			wchar_t	wchBegin = wchNext, wchClose ;
			if ( wchNext == L'[' )
			{
				pUsage = new EUsage( utOmittable, &lstUsage ) ;
				wchClose = L']' ;
			}
			else
			{
				pUsage = new EUsage( utSelectable, &lstUsage ) ;
				wchClose = L'}' ;
			}
			lstUsage.Add( pUsage ) ;
			do
			{
				iEndUsage = swsUsage.GetIndex( ) ;
				wchNext = swsUsage.GetCharacter( ) ;
				if ( wchNext == wchClose )
				{
					if ( (-- nNestCount) == 0 )
					{
						break ;
					}
				}
				else if ( wchNext == L'\\' )
				{
					swsUsage.GetCharacter( ) ;
				}
				else if ( wchNext == L'%' )
				{
					if ( swsUsage.GetCharacter() == L'c' )
					{
						swsUsage.GetCharacter( ) ;
					}
				}
				else if ( wchNext == wchBegin )
				{
					nNestCount ++ ;
				}
				else if ( (wchBegin == L'{') && (wchNext == L'|') )
				{
					EStreamWideString	swsSubUsage =
						swsUsage.Middle( iNestUsage, iEndUsage - iNestUsage ) ;
					EUsage *	pSub = new EUsage( utList, pUsage ) ;
					pUsage->Add( pSub ) ;
					err = ParseUsage( *pSub, swsSubUsage, strErrMsg ) ;
					if ( err )
					{
						break ;
					}
					iNestUsage = swsUsage.GetIndex( ) ;
				}
				else if ( wchNext == L'(' )
				{
					nUsageParams ++ ;
				}
			}
			while ( !swsUsage.IsIndexOverflow() ) ;
			if ( err )
			{
				break ;
			}
			EStreamWideString	swsSubUsage =
				swsUsage.Middle( iNestUsage, iEndUsage - iNestUsage ) ;
			if ( pUsage->m_type == utOmittable )
			{
				pSubUsage = pUsage ;
			}
			else
			{
				EUsage *	pSub = new EUsage( utList, pUsage ) ;
				pUsage->Add( pSub ) ;
				pSubUsage = pSub ;
			}
			err = ParseUsage( *pSubUsage, swsSubUsage, strErrMsg ) ;
			if ( err )
			{
				break ;
			}
		}
		else if ( wchNext == L'*' )
		{
			//
			// 書式の反復
			//
			lstUsage.Add( new EUsage( utRepeat, &lstUsage ) ) ;
		}
		else
		{
			//
			// 任意文字列
			//
			pUsage = new EUsage( utToken, &lstUsage ) ;
			swsUsage.MoveIndex( iCurrentUsage ) ;
			pUsage->m_wstr = swsUsage.GetAToken( ) ;
			lstUsage.Add( pUsage ) ;
		}
	}
	if ( err )
	{
		swsUsage.MoveIndex( iOrgUsage ) ;
	}
	return	err ;
}

#endif	//	!defined(COMPACT_NOA_DECODER)



/*****************************************************************************
                    ストリーミングバッファクラス
 ****************************************************************************/

// クラス情報
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( EStreamBuffer, ESLObject )

// 消滅関数
//////////////////////////////////////////////////////////////////////////////
EStreamBuffer::EBuffer::~EBuffer( void )
{
	if ( m_ptrBuf != NULL )
	{
		::eslHeapFree( NULL, m_ptrBuf ) ;
	}
}

// バッファ取得
//////////////////////////////////////////////////////////////////////////////
EPtrBuffer EStreamBuffer::EBuffer::GetBuffer( unsigned int nLength )
{
	if ( nLength > m_nLength )
	{
		nLength = m_nLength ;
	}
	return	EPtrBuffer( m_ptrBuf + m_nBeginPos, nLength ) ;
}

// バッファ解放
//////////////////////////////////////////////////////////////////////////////
void EStreamBuffer::EBuffer::Release( unsigned int nLength )
{
	if ( nLength > m_nLength )
	{
		nLength = m_nLength ;
	}
	m_nBeginPos += nLength ;
	m_nLength -= nLength ;
}

// 出力のためにバッファ確保
//////////////////////////////////////////////////////////////////////////////
void * EStreamBuffer::EBuffer::PutBuffer( unsigned int nLength )
{
	if ( (m_nLength + nLength) > m_nBufLimit )
	{
		//
		// メモリを再確保
		//
		m_nBufLimit = ((m_nLength + nLength + 0xFFF) & (~0xFFF)) ;
		BYTE *	ptrBuf =
			(BYTE*) ::eslHeapAllocate( NULL, m_nBufLimit, 0 ) ;
		if ( m_ptrBuf != NULL )
		{
			::eslMoveMemory( ptrBuf, m_ptrBuf + m_nBeginPos, m_nLength ) ;
			::eslHeapFree( NULL, m_ptrBuf ) ;
		}
		m_ptrBuf = ptrBuf ;
		m_nBeginPos = 0 ;
	}
	else
	{
		//
		// メモリの再確保無しにバッファを確保
		//
		if ( (m_nBeginPos > 0) && (m_nLength > 0) )
		{
			::eslMoveMemory( m_ptrBuf, m_ptrBuf + m_nBeginPos, m_nLength ) ;
		}
		m_nBeginPos = 0 ;
	}
	//
	return	(m_ptrBuf + m_nBeginPos + m_nLength) ;
}

// メモリ確定
//////////////////////////////////////////////////////////////////////////////
void EStreamBuffer::EBuffer::Flush( unsigned int nLength )
{
	if ( m_nBeginPos + m_nLength + nLength > m_nBufLimit )
	{
		nLength = m_nBufLimit - (m_nBeginPos + m_nLength) ;
	}
	m_nLength += nLength ;
}

// 構築関数
//////////////////////////////////////////////////////////////////////////////
EStreamBuffer::EStreamBuffer( void )
	: m_pFirstBuf( NULL ), m_pLastBuf( NULL ), m_nLength( 0 )
{
}

EStreamBuffer::EStreamBuffer( const void * ptrBuffer, unsigned int nLength )
	: m_pFirstBuf( NULL ), m_pLastBuf( NULL ), m_nLength( 0 )
{
	Write( ptrBuffer, nLength ) ;
}

EStreamBuffer::EStreamBuffer( const EPtrBuffer & ptrbuf )
	: m_pFirstBuf( NULL ), m_pLastBuf( NULL ), m_nLength( 0 )
{
	Write( ptrbuf, ptrbuf.GetLength() ) ;
}

// 消滅関数
//////////////////////////////////////////////////////////////////////////////
EStreamBuffer::~EStreamBuffer( void )
{
	Delete( ) ;
}

// リソースを解放
//////////////////////////////////////////////////////////////////////////////
void EStreamBuffer::Delete( void )
{
	EBuffer *	pLastBuf = m_pFirstBuf ;
	while ( pLastBuf != NULL )
	{
		EBuffer *	pNextBuf = pLastBuf->GetNextBuffer( ) ;
		delete	pLastBuf ;
		pLastBuf = pNextBuf ;
	}
	m_pFirstBuf = NULL ;
	m_pLastBuf = NULL ;
	m_nLength = 0 ;
}

// バッファの長さを取得
//////////////////////////////////////////////////////////////////////////////
unsigned int EStreamBuffer::GetLength( void ) const
{
	return	m_nLength ;
}

// バッファから取り出すためのメモリを確保
//////////////////////////////////////////////////////////////////////////////
EPtrBuffer EStreamBuffer::GetBuffer( unsigned int nLength )
{
	//
	// 取り出すバッファの長さを計算
	//
	if ( m_pFirstBuf == NULL )
	{
		return	EPtrBuffer( NULL, 0 ) ;
	}
	if ( (nLength == 0) || (nLength > m_nLength) )
	{
		nLength = m_nLength ;
	}
	//
	// 先頭のバッファから取り出す
	//
	if ( m_pFirstBuf->GetLength() >= nLength )
	{
		return	m_pFirstBuf->GetBuffer( nLength ) ;
	}
	//
	// バッファを結合する
	//
	EBuffer *	pBuf = new EBuffer ;
	void *	ptrDst = pBuf->PutBuffer( nLength ) ;
	unsigned int	nSize = Read( ptrDst, nLength ) ;
	m_nLength += nSize ;
	pBuf->Flush( nSize ) ;
	pBuf->SetNextBuffer( m_pFirstBuf ) ;
	m_pFirstBuf = pBuf ;
	if ( m_pLastBuf == NULL )
	{
		m_pLastBuf = pBuf ;
	}
	//
	return	m_pFirstBuf->GetBuffer( nLength ) ;
}

// GetBuffer によってロックされたメモリを解放
//////////////////////////////////////////////////////////////////////////////
void EStreamBuffer::Release( unsigned int nLength )
{
	if ( nLength != 0 )
	{
		ESLAssert( m_pFirstBuf != NULL ) ;
		m_pFirstBuf->Release( nLength ) ;
		m_nLength -= nLength ;
	}
}

// バッファからデータを読み出す
//////////////////////////////////////////////////////////////////////////////
unsigned int EStreamBuffer::Read( void * ptrBuffer, unsigned int nLength )
{
	BYTE *	ptrDst = (BYTE*) ptrBuffer ;
	unsigned int	i = 0 ;
	while ( i < nLength )
	{
		if ( m_pFirstBuf == NULL )
		{
			break ;
		}
		EPtrBuffer	ptrbuf =
			m_pFirstBuf->GetBuffer( m_pFirstBuf->GetLength() ) ;
		unsigned int	nCopyLen = ptrbuf.GetLength( ) ;
		if ( i + nCopyLen > nLength )
		{
			nCopyLen = nLength - i ;
		}
		::eslMoveMemory( ptrDst + i, ptrbuf, nCopyLen ) ;
		m_pFirstBuf->Release( nCopyLen ) ;
		m_nLength -= nCopyLen ;
		i += nCopyLen ;
		//
		if ( m_pFirstBuf->GetLength() == 0 )
		{
			EBuffer *	pNext = m_pFirstBuf->GetNextBuffer( ) ;
			delete	m_pFirstBuf ;
			m_pFirstBuf = pNext ;
			if ( pNext == NULL )
			{
				m_pLastBuf = NULL ;
			}
		}
	}
	return	i ;
}

// バッファへ書き出すためのメモリを確保
//////////////////////////////////////////////////////////////////////////////
void * EStreamBuffer::PutBuffer( unsigned int nLength )
{
	if ( m_pLastBuf != NULL )
	{
		if ( m_pLastBuf->GetMaxWritableLength() >= nLength )
		{
			return	m_pLastBuf->PutBuffer( nLength ) ;
		}
	}
	EBuffer *	pBuf = new EBuffer ;
	void *	ptrPut = pBuf->PutBuffer( nLength ) ;
	if ( m_pLastBuf == NULL )
	{
		ESLAssert( m_pFirstBuf == NULL ) ;
		m_pFirstBuf = m_pLastBuf = pBuf ;
	}
	else
	{
		m_pLastBuf->SetNextBuffer( pBuf ) ;
		m_pLastBuf = pBuf ;
	}
	return	ptrPut ;
}

// PutBuffer によってロックされたメモリを確定
//////////////////////////////////////////////////////////////////////////////
void EStreamBuffer::Flush( unsigned int nLength )
{
	ESLAssert( m_pLastBuf != NULL ) ;
	m_pLastBuf->Flush( nLength ) ;
	m_nLength += nLength ;
}

// バッファへデータを書き込む
//////////////////////////////////////////////////////////////////////////////
unsigned int EStreamBuffer::Write
	( const void * ptrBuffer, unsigned int nLength )
{
	::eslMoveMemory( PutBuffer( nLength ), ptrBuffer, nLength ) ;
	Flush( nLength ) ;
	return	nLength ;
}

// バッファの一部を参照
//////////////////////////////////////////////////////////////////////////////
void * EStreamBuffer::ModifyBuffer
	( unsigned int nPosition, unsigned int nLength )
{
	//
	// バッファの位置を検索
	//
	EBuffer *	pBuffer = m_pFirstBuf ;
	while ( pBuffer != NULL )
	{
		if ( nPosition < pBuffer->GetLength() )
			break ;
		nPosition -= pBuffer->GetLength( ) ;
		pBuffer = pBuffer->GetNextBuffer( ) ;
	}
	if ( pBuffer == NULL )
	{
		return	NULL ;
	}
	//
	// バッファの結合
	//
	int	nFixed = 0 ;
	int	nLeftBytes = nPosition + nLength - pBuffer->GetLength() ;
	if ( nLeftBytes > 0 )
	{
		BYTE *	pbytBuf = (BYTE*) pBuffer->PutBuffer( nLeftBytes ) ;
		while ( nFixed < nLeftBytes )
		{
			EBuffer *	pNextBuf = pBuffer->GetNextBuffer( ) ;
			if ( pNextBuf == NULL )
			{
				break ;
			}
			int	nBufLen = pNextBuf->GetLength( ) ;
			if ( nBufLen > (nLeftBytes - nFixed) )
			{
				nBufLen = nLeftBytes - nFixed ;
			}
			//
			EPtrBuffer	ptrbuf = pNextBuf->GetBuffer( nBufLen ) ;
			::eslMoveMemory( pbytBuf, ptrbuf.GetBuffer(), ptrbuf.GetLength() ) ;
			pbytBuf += ptrbuf.GetLength( ) ;
			nFixed += ptrbuf.GetLength( ) ;
			pNextBuf->Release( nBufLen ) ;
			//
			if ( pNextBuf->GetLength() == 0 )
			{
				pBuffer->SetNextBuffer( pNextBuf->GetNextBuffer() ) ;
				delete	pNextBuf ;
				if ( pBuffer->GetNextBuffer() == NULL )
				{
					m_pLastBuf = pBuffer ;
				}
			}
		}
		pBuffer->Flush( nFixed ) ;
	}
	//
	if ( nPosition + nLength <= pBuffer->GetLength() )
	{
		return	pBuffer->ModifyBuffer( nPosition ) ;
	}
	return	NULL ;
}

// ファイルの終端まで読み込む
//////////////////////////////////////////////////////////////////////////////
ESLError EStreamBuffer::ReadFromFile( ESLFileObject & file )
{
	const DWORD	dwBlockSize = 0x2000 ;
	DWORD	dwReadBytes ;
	do
	{
		void *	pbuf = PutBuffer( dwBlockSize ) ;
		dwReadBytes = file.Read( pbuf, dwBlockSize ) ;
		Flush( dwReadBytes ) ;
	}
	while ( dwReadBytes == dwBlockSize ) ;
	return	eslErrSuccess ;
}

// バッファの内容をファイルに書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError EStreamBuffer::WriteToFile( ESLFileObject & file )
{
	const DWORD	dwBlockSize = 0x2000 ;
	for ( ; ; )
	{
		EPtrBuffer	ptrbuf = GetBuffer( dwBlockSize ) ;
		if ( ptrbuf.GetLength() == 0 )
		{
			break ;
		}
		if ( file.Write
			( ptrbuf, ptrbuf.GetLength() ) < ptrbuf.GetLength() )
		{
			return	eslErrGeneral ;
		}
		Release( ptrbuf.GetLength() ) ;
	}
	return	eslErrSuccess ;
}

