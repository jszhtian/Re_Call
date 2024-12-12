
/*****************************************************************************
                   Entis Standard Library declarations
 ----------------------------------------------------------------------------
	Copyright (C) 2002-2004 Leshade Entis, Entis-soft. All rights reserved.
 ****************************************************************************/


// Include esl.h
//////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<eritypes.h>
#include	<esl.h>


#if	!defined(COMPACT_NOA_DECODER)

/*****************************************************************************
                        構造物記述オブジェクト
 *****************************************************************************/

// クラス情報
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( EDescription, ESLObject )

// 構築関数
//////////////////////////////////////////////////////////////////////////////
EDescription::EDescription( void )
{
	m_pParent = NULL ;
}

EDescription::EDescription( const EDescription & desc )
{
	m_pParent = NULL ;
	EDescription::operator = ( desc ) ;
}

// 消滅関数
//////////////////////////////////////////////////////////////////////////////
EDescription::~EDescription( void )
{
}

// 現在の文字列（区切り記号は無視）を通過する
//////////////////////////////////////////////////////////////////////////////
void EDescription::EStreamXMLString::PassEnclosedString
	( wchar_t wchClose, int flagCtrlCode )
{
	if ( (flagCtrlCode == dftXMLandCEsc)
		&& ((wchClose == L'\"') || (wchClose == L'\'')) )
	{
		while ( m_nIndex < m_nLength )
		{
			if ( m_pszString[m_nIndex] == L'\\' )
			{
				if ( ++ m_nIndex >= m_nLength )
					break ;
			}
			else if ( m_pszString[m_nIndex] == wchClose )
			{
				break ;
			}
			m_nIndex ++ ;
		}
	}
	else
	{
		EStreamWideString::PassEnclosedString( wchClose, flagCtrlCode ) ;
	}
}

// 現在のトークンを通過する（XML仕様）
//////////////////////////////////////////////////////////////////////////////
void EDescription::EStreamXMLString::PassAToken( int * pTokenType )
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
			wch = m_pszString[m_nIndex] ;
			if( IsPunctuation( wch )
					&& ((wch != L'.') && (wch != L'-') && (wch != L':')) )
			{
				break ;
			}
			m_nIndex ++ ;
		}
	}
}

// 読みこみ
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::ReadDescription
	( EStreamBuffer & buf,
		EDescription::DataFormatType dftType,
		EDescription::CharacterEncoding ceType )
{
	//
	// データ取得
	//
	ESLError			err ;
	EString				strSrc ;
	EStreamXMLString	sxsDesc ;
	//
	EPtrBuffer			ptrbuf = buf.GetBuffer( ) ;
	::eslMoveMemory
		( strSrc.GetBuffer( ptrbuf.GetLength() ),
			ptrbuf.GetBuffer(), ptrbuf.GetLength() ) ;
	strSrc.ReleaseBuffer( ptrbuf.GetLength() ) ;
	buf.Release( ptrbuf.GetLength() ) ;
	//
	// 書式・文字エンコーディング取得
	//
	if ( (dftType == dftAuto) && (strSrc.Left(5) == "<?xml") )
	{
		int	iFind = strSrc.Find( "?>" ) ;
		if ( iFind > 0 )
		{
			EStreamXMLString	sxsXML ;
			sxsXML = strSrc.Middle( 5, iFind - 5 ) ;
			while ( !sxsXML.DisregardSpace() )
			{
				EWideString	wstrName = sxsXML.GetAToken( ) ;
				if ( sxsXML.HasToComeChar( L"=" ) != L'=' )
				{
					break ;
				}
				EWideString	wstrValue ;
				if ( sxsXML.HasToComeChar( L"\"" ) == L'\"' )
				{
					wstrValue = sxsXML.GetEnclosedString( L'\"' ) ;
				}
				else
				{
					wstrValue = sxsXML.GetString( ) ;
				}
				if ( wstrName == L"encoding" )
				{
					ceType = GetCharaEncodingType( EString(wstrValue) ) ;
				}
			}
		}
		dftType = dftXML ;
	}
	else if ( dftType == dftAuto )
	{
		dftType = dftXMLandCEsc ;
	}
	//
	// 文字エンコーディング変換
	//
	err = DecodeText( sxsDesc, strSrc, ceType ) ;
	if ( err )
		return	err ;
	//
	// 読み込み
	//
	RemoveAllAttribute( ) ;
	RemoveAllContentTag( ) ;
	//
	return	ReadDescription( sxsDesc, dftType ) ;
}

ESLError EDescription::ReadDescription
	( EStreamWideString & swsDesc, EDescription::DataFormatType dftType )
{
	ESLAssert( (dftType == dftXML) || (dftType == dftXMLandCEsc) ) ;
	//
	ESLError	err = eslErrSuccess ;
	EString		strErrMsg ;
	bool		fEnterText = false ;
	EWideString	wstrContents ;
	//
	while ( !swsDesc.IsIndexOverflow() )
	{
		//
		// タグを判定
		//
		wchar_t	wch = swsDesc.GetCharacter( ) ;
		if ( wch == L'<' )
		{
			//
			// タグ
			//////////////////////////////////////////////////////////////////
			//
			// 直前の文字コンテンツを追加
			//
			if ( !wstrContents.IsEmpty() )
			{
				EDescription *	pDesc = CreateDescription( ) ;
				pDesc->m_wstrContents = wstrContents ;
				m_listDesc.Add( pDesc ) ;
			}
			//
			// 終了タグかチェック
			//
			if ( swsDesc.HasToComeChar( L"/" ) == L'/' )
			{
				EWideString	wstrTag = swsDesc.GetAToken( ) ;
				if ( m_wstrTag.CompareNoCase( wstrTag ) )
				{
					// エラー
					strErrMsg = "</" ;
					strErrMsg += EString( wstrTag ) ;
					strErrMsg += "> に対応するタグがありません。" ;
					OutputError( strErrMsg ) ;
					ESLTrace( "XML 構文エラー : "
						"</%s> に対応するタグがありません。\n",
							EString(wstrTag).CharPtr() ) ;
					swsDesc.PassEnclosedString( L'>', dftType ) ;
					err = eslErrGeneral ;
					continue ;
				}
				if ( swsDesc.HasToComeChar( L">" ) != L'>' )
				{
					strErrMsg = "</" ;
					strErrMsg += EString( wstrTag ) ;
					strErrMsg += "> の閉じ括弧がありません。" ;
					OutputError( strErrMsg ) ;
					err = eslErrGeneral ;
				}
				return	err ;
			}
			//
			// 特殊タグかチェック
			//
			wch = swsDesc.HasToComeChar( L"?!" ) ;
			if ( (wch == L'?') || (wch == L'!') )
			{
				if ( wch == L'!' )
				{
					unsigned int	iSave ;
					iSave = swsDesc.GetIndex( ) ;
					if ( (swsDesc.GetCharacter() == L'-')
						&& (swsDesc.GetCharacter() == L'-') )
					{
						//
						// コメントタグ
						//
						unsigned int	nHyphen = 0 ;
						while ( !swsDesc.IsIndexOverflow() )
						{
							wch = swsDesc.GetCharacter( ) ;
							if ( wch == L'-' )
							{
								nHyphen ++ ;
							}
							else if ( wch == L'>' )
							{
								if ( nHyphen >= 2 )
								{
									break ;
								}
								nHyphen = 0 ;
							}
							else
							{
								nHyphen = 0 ;
							}
						}
						EDescription *	pdscComment = new EDescription ;
						pdscComment->m_pParent = this ;
						pdscComment->m_wstrTag = L"!--" ;
						pdscComment->m_wstrContents =
							swsDesc.Middle( iSave - 1,
											swsDesc.GetIndex() - iSave + 1 ) ;
						m_listDesc.Add( pdscComment ) ;
						continue ;
					}
					swsDesc.MoveIndex( iSave ) ;
				}
				//
				swsDesc.PassEnclosedString( L'>', dftType ) ;
				swsDesc.HasToComeChar( L">" ) ;
				continue ;
			}
			//
			// タグの名前を取得
			//
			EDescription *	pDesc = CreateDescription( ) ;
			pDesc->m_pParent = this ;
			pDesc->m_wstrTag = swsDesc.GetAToken( ) ;
			if ( pDesc->m_wstrTag == L">" )
			{
				delete	pDesc ;
				continue ;
			}
			m_listDesc.Add( pDesc ) ;
			//
			// タグの属性を取得
			//
			bool	fTagClosed = false ;
			while ( !swsDesc.DisregardSpace() )
			{
				//
				// 終端チェック
				//
				wch = swsDesc.HasToComeChar( L"/>" ) ;
				if ( wch == L'>' )
				{
					break ;
				}
				if ( wch == L'/' )
				{
					if ( swsDesc.HasToComeChar( L">" ) != L'>' )
					{
						OutputError( "書式中に不正な / 記号を見つけました。" ) ;
						err = eslErrGeneral ;
					}
					fTagClosed = true ;
					break ;
				}
				//
				// 属性名取得
				//
				int	fTokenType ;
				EWideString	wstrAttr = swsDesc.GetAToken( &fTokenType ) ;
				if ( fTokenType != 0 )
				{
					strErrMsg = "<"	+ EString( pDesc->m_wstrTag )
						+ "> タグに不正なタグ属性 \'"
						+ EString(wstrAttr) + "\' が指定されています。" ;
					OutputError( strErrMsg ) ;
					swsDesc.PassEnclosedString( L'>', dftType ) ;
					err = eslErrGeneral ;
					break ;
				}
				if ( swsDesc.HasToComeChar( L"=" ) != L'=' )
				{
					pDesc->m_wstaAttr.SetAs( wstrAttr, new EWideString ) ;
/*					strErrMsg = '<' ;
					strErrMsg += EString( pDesc->m_wstrTag ) ;
					strErrMsg += "> タグの属性の書式が不正です。" ;
					OutputError( strErrMsg ) ;
					swsDesc.PassEnclosedString( L'>', dftType ) ;
					err = eslErrGeneral ;
					break ;*/
					continue ;
				}
				//
				// 属性値取得
				//
				EWideString	wstrValue ;
				if ( swsDesc.HasToComeChar( L"\"" ) == L'\"' )
				{
					wstrValue = swsDesc.GetEnclosedString( L'\"', dftType ) ;
				}
				else
				{
					wstrValue = swsDesc.GetString( ) ;
				}
				//
				// 属性追加
				//
				if ( dftType == dftXML )
				{
					DecodeTextContents( wstrValue ) ;
				}
				else
				{
					DecodeTextCEscSequence( wstrValue ) ;
				}
				pDesc->m_wstaAttr.SetAs( wstrAttr, new EWideString(wstrValue) ) ;
			}
			if ( !fTagClosed )
			{
				ESLError	err =
					pDesc->ReadDescription( swsDesc, dftType ) ;
				if ( err )
					return	err ;
			}
		}
		else if ( wch == L'&' )
		{
			//
			// 特殊文字
			//////////////////////////////////////////////////////////////////
			static const wchar_t *	pwszSpecChar[] =
			{
				L"lt", L"gt", L"quot", L"amp", L"nbsp", NULL
			} ;
			static const wchar_t	wchSpecChar[] =
			{
				L'<', L'>', L'\"', L'&', L' '
			} ;
			int			i ;
			if ( swsDesc.HasToComeChar( L"#" ) == L'#' )
			{
				int	nRadix = swsDesc.GetNumberRadix( ) ;
				if ( nRadix < 0 )
				{
					OutputWarning
						( "&#xxx; 構文で文字コードの指定が不正です。\n" ) ;
					wstrContents += L"&#" ;
				}
				else
				{
					wstrContents += (wchar_t) swsDesc.GetInteger( nRadix ) ;
					if ( swsDesc.HasToComeChar( L";" ) != L';' )
					{
						OutputWarning
							( "&#xxx; 構文がセミコロンで閉じられていません。\n" ) ;
					}
				}
			}
			else
			{
				EWideString	wstrToken = swsDesc.GetAToken( ) ;
				for ( i = 0; pwszSpecChar[i]; i ++ )
				{
					if ( wstrToken == pwszSpecChar[i] )
					{
						break ;
					}
				}
				if ( pwszSpecChar[i] )
				{
					if ( swsDesc.HasToComeChar( L";" ) != L';' )
					{
						OutputWarning
							( "特殊文字の終端は"
								"セミコロンで終わらなければなりません。\n" ) ;
					}
					wstrContents += wchSpecChar[i] ;
				}
				else
				{
					// エラー
					OutputWarning
						( "& 記号は &amp; と記述しなければなりません。\n" ) ;
					wstrContents += L'&' ;
					wstrContents += wstrToken ;
				}
			}
			fEnterText = true ;
		}
		else
		{
			//
			// 一般文字
			//////////////////////////////////////////////////////////////////
			if ( wch <= L' ' )
			{
				if ( (wch != L' ') && (wch != L'\t') )
				{
					fEnterText = false ;
				}
				else if ( fEnterText )
				{
					wstrContents += wch ;
				}
			}
			else
			{
				wstrContents += wch ;
				fEnterText = true ;
			}
		}
	}
	//
	// 最後の文字コンテンツを追加
	//
	if ( !wstrContents.IsEmpty() )
	{
		EDescription *	pDesc = CreateDescription( ) ;
		pDesc->m_wstrContents = wstrContents ;
		m_listDesc.Add( pDesc ) ;
	}
	//
	return	err ;
}

// 書き出し
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::WriteDescription
	( ESLFileObject & file, int nTabIndent,
		EDescription::DataFormatType dftType,
		EDescription::CharacterEncoding ceType )
{
	int			i ;
	EWideString	wstrTab ;
	EString		strText ;
	EWideString	wstrText ;
	//
	// 文書エンコーディング
	//
	for ( i = 0; i < nTabIndent; i ++ )
	{
		wstrTab += L'\t' ;
	}
	if ( dftType == dftAuto )
	{
		dftType = dftXML ;
		strText = "<?xml version=\"1.0\" encoding=\"" ;
		strText += GetCharaEncodingName( ceType ) ;
		strText += "\"?>\r\n" ;
		file.Write( strText.CharPtr(), strText.GetLength() ) ;
	}
	if ( m_wstrTag.IsEmpty() && (m_listDesc.GetSize() == 0) )
	{
		EWideString	wstrContents ;
		EncodeTextContents( wstrContents, m_wstrContents, nTabIndent ) ;
		EncodeText( strText, wstrTab + wstrContents, ceType ) ;
		file.Write( strText.CharPtr(), strText.GetLength() ) ;
		file.Write( "\r\n", 2 ) ;
		return	eslErrSuccess ;
	}
	//
	// タグ出力
	//
	bool	fTagContentString = false ;
	int		nNestIndent = nTabIndent ;
	if ( !m_wstrTag.IsEmpty() )
	{
		if ( m_wstrTag != L"!--" )
		{
			nNestIndent ++ ;
			wstrText = wstrTab + L"<" + m_wstrTag ;
			//
			for ( i = 0; i < (int) m_wstaAttr.GetSize(); i ++ )
			{
				ETaggedElement<EWideString,EWideString> *	pElement ;
				pElement = m_wstaAttr.GetAt( i ) ;
				ESLAssert( pElement != NULL ) ;
				ESLAssert( pElement->GetObject() != NULL ) ;
				//
				EWideString	wstrValue ;
				if ( dftType == dftXML )
				{
					EncodeTextContents( wstrValue, *(pElement->GetObject()), -1 ) ;
				}
				else
				{
					wstrValue = *(pElement->GetObject()) ;
					EncodeTextCEscSequence( wstrValue ) ;
				}
				//
				if ( (int) (wstrText.GetLength()
					+ pElement->Tag().GetLength()
					+ wstrValue.GetLength() + 4) >= (80 - nTabIndent * 7) )
				{
					wstrText += L"\r\n" ;
					EncodeText( strText, wstrText, ceType ) ;
					file.Write( strText.CharPtr(), strText.GetLength() ) ;
					wstrText = wstrTab + L" " ;
				}
				wstrText += L" " + pElement->Tag() + L"=\"" + wstrValue + L"\"" ;
			}
			//
			if ( m_listDesc.GetSize() == 0 )
			{
				wstrText += L"/>\r\n" ;
			}
			else
			{
				EDescription *	pDesc  ;
				if ( m_listDesc.GetSize() == 1 )
				{
					pDesc = m_listDesc.GetAt( 0 ) ;
					if ( (pDesc != NULL)
						&& pDesc->Tag().IsEmpty()
						&& (pDesc->m_listDesc.GetSize() == 0) )
					{
						if ( (pDesc->m_wstrContents.Find( '\n' ) < 0)
							&& (pDesc->m_wstrContents.Find( '\r' ) < 0) )
						{
							fTagContentString = true ;
						}
					}
				}
				if ( !fTagContentString )
				{
					wstrText += L">\r\n" ;
				}
				else
				{
					EWideString	wstrContents ;
					EncodeTextContents
						( wstrContents, pDesc->m_wstrContents, 0 ) ;
					wstrText += L">" ;
					wstrText += wstrContents ;
					wstrText += L"</" + m_wstrTag + L">\r\n" ;
				}
			}
		}
		else
		{
			wstrText = wstrTab + L"<" + m_wstrContents + L"\r\n" ;
		}
		EncodeText( strText, wstrText, ceType ) ;
		file.Write( strText.CharPtr(), strText.GetLength() ) ;
	}
	if ( !fTagContentString )
	{
		//
		// サブタグを出力
		//
		for ( i = 0; i < (int) m_listDesc.GetSize(); i ++ )
		{
			EDescription *	pDesc = m_listDesc.GetAt( i ) ;
			ESLAssert( pDesc != NULL ) ;
			pDesc->WriteDescription( file, nNestIndent, dftType, ceType ) ;
		}
		//
		// 閉じタグ出力
		//
		if ( !m_wstrTag.IsEmpty() && (m_listDesc.GetSize() > 0) )
		{
			wstrText = wstrTab + L"</" + m_wstrTag + L">\r\n" ;
			EncodeText( strText, wstrText, ceType ) ;
			file.Write( strText.CharPtr(), strText.GetLength() ) ;
		}
	}
	//
	return	eslErrSuccess ;
}

// 副タグを生成
//////////////////////////////////////////////////////////////////////////////
EDescription * EDescription::CreateDescription( void )
{
	return	new EDescription ;
}

// エラー出力
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::OutputError( const char * pszErrMsg )
{
	ESLTrace( "XML 構文エラー : " ) ;
	ESLTrace( pszErrMsg ) ;
	ESLTrace( "\n" ) ;
	return	eslErrSuccess ;
}

// 警告出力
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::OutputWarning( const char * pszErrMsg )
{
	ESLTrace( "XML 構文エラー : " ) ;
	ESLTrace( pszErrMsg ) ;
	ESLTrace( "\n" ) ;
	return	eslErrSuccess ;
}

// 代入操作
//////////////////////////////////////////////////////////////////////////////
const EDescription & EDescription::operator = ( const EDescription & desc )
{
	int		i ;
	//
	m_wstrTag = desc.m_wstrTag ;
	m_wstrContents = desc.m_wstrContents ;
	//
	RemoveAllAttribute( ) ;
	RemoveAllContentTag( ) ;
	//
	for ( i = 0; i < (int) desc.m_wstaAttr.GetSize(); i ++ )
	{
		ETaggedElement<EWideString,EWideString> *	pElement ;
		pElement = desc.m_wstaAttr.GetAt( i ) ;
		ESLAssert( pElement != NULL ) ;
		ESLAssert( pElement->GetObject() != NULL ) ;
		m_wstaAttr.SetAs
			( pElement->Tag(), new EWideString(*(pElement->GetObject())) ) ;
	}
	//
	for ( i = 0; i < (int) desc.m_listDesc.GetSize(); i ++ )
	{
		EDescription *	pDesc = desc.m_listDesc.GetAt( i ) ;
		ESLAssert( pDesc != NULL ) ;
		EDescription *	pdscNew = CreateDescription( ) ;
		*pdscNew = *pDesc ;
		AddContentTag( pdscNew ) ;
	}
	//
	return	*this ;
}

// マージ操作
//////////////////////////////////////////////////////////////////////////////
const EDescription & EDescription::operator += ( const EDescription & desc )
{
	int		i ;
	m_wstrContents = desc.m_wstrContents ;
	for ( i = 0; i < (int) desc.m_wstaAttr.GetSize(); i ++ )
	{
		ETaggedElement<EWideString,EWideString> *	pElement ;
		pElement = desc.m_wstaAttr.GetAt( i ) ;
		ESLAssert( pElement != NULL ) ;
		ESLAssert( pElement->GetObject() != NULL ) ;
		m_wstaAttr.SetAs
			( pElement->Tag(), new EWideString(*(pElement->GetObject())) ) ;
	}
	for ( i = 0; i < (int) desc.m_listDesc.GetSize(); i ++ )
	{
		EDescription *	pDesc = desc.m_listDesc.GetAt( i ) ;
		ESLAssert( pDesc != NULL ) ;
		EDescription *	pdscNew = CreateContentTagAs( 0, pDesc->Tag() ) ;
		*pdscNew += *pDesc ;
	}
	return	*this ;
}

// 親取得
//////////////////////////////////////////////////////////////////////////////
EDescription * EDescription::GetParent( void ) const
{
	return	m_pParent ;
}

// タグ名取得
//////////////////////////////////////////////////////////////////////////////
const EWideString & EDescription::Tag( void ) const
{
	return	m_wstrTag ;
}

// タグ名設定
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetTag( const wchar_t * pwszTag )
{
	m_wstrTag = pwszTag ;
}

// コメントタグにする
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetCommentTag( const wchar_t * pwszComment )
{
	m_wstrTag = L"!--" ;
	m_wstrContents = L"!--" ;
	if ( pwszComment != NULL )
	{
		m_wstrContents += pwszComment ;
	}
	m_wstrContents += L"-->" ;
}

// 内容取得
//////////////////////////////////////////////////////////////////////////////
const EWideString & EDescription::Contents( void ) const
{
	return	m_wstrContents ;
}

// 内容設定
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetContents( const wchar_t * pwszContents )
{
	m_wstrContents = pwszContents ;
}

// タグ属性取得（整数）
//////////////////////////////////////////////////////////////////////////////
int EDescription::GetAttrInteger
	( const wchar_t * pwszAttr, int nDefValue ) const
{
	EWideString *	pwstrValue = m_wstaAttr.GetAs( pwszAttr ) ;
	if ( pwstrValue == NULL )
	{
		return	nDefValue ;
	}
	EStreamWideString	swsValue = *pwstrValue ;
	int		nRadix = swsValue.GetNumberRadix( ) ;
	if ( nRadix < 0 )
	{
		return	nDefValue ;
	}
	return	swsValue.GetInteger( nRadix ) ;
}

// タグ属性取得（実数）
//////////////////////////////////////////////////////////////////////////////
double EDescription::GetAttrReal
	( const wchar_t * pwszAttr, double rDefValue ) const
{
	EWideString *	pwstrValue = m_wstaAttr.GetAs( pwszAttr ) ;
	if ( pwstrValue == NULL )
	{
		return	rDefValue ;
	}
	EStreamWideString	swsValue = *pwstrValue ;
	int		nRadix = swsValue.GetNumberRadix( ) ;
	if ( nRadix < 0 )
	{
		return	rDefValue ;
	}
	return	swsValue.GetRealNumber( nRadix ) ;
}

// タグ属性取得（文字列）
//////////////////////////////////////////////////////////////////////////////
EWideString EDescription::GetAttrString
	( const wchar_t * pwszAttr, const wchar_t * pwszDefValue ) const
{
	EWideString *	pwstrValue = m_wstaAttr.GetAs( pwszAttr ) ;
	if ( pwstrValue == NULL )
	{
		return	pwszDefValue ;
	}
	return	*pwstrValue ;
}

// タグ属性設定（整数）
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetAttrInteger( const wchar_t * pwszAttr, int nValue )
{
	if ( (nValue >= -0x8000) && (nValue <= 0x8000) )
	{
		m_wstaAttr.SetAs( pwszAttr, new EWideString(nValue) ) ;
	}
	else
	{
		EWideString	wstrHex( (DWORD) nValue, 8 ) ;
		if ( (DWORD) nValue >= 0xA0000000 )
		{
			wstrHex = L"0" + wstrHex ;
		}
		wstrHex += L'H' ;
		m_wstaAttr.SetAs( pwszAttr, new EWideString(wstrHex) ) ;
	}
}

// タグ属性設定（実数）
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetAttrReal( const wchar_t * pwszAttr, double rValue )
{
	m_wstaAttr.SetAs( pwszAttr, new EWideString(rValue) ) ;
}

// タグ属性設定（文字列）
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetAttrString
	( const wchar_t * pwszAttr, const wchar_t * pwszValue )
{
	m_wstaAttr.SetAs( pwszAttr, new EWideString(pwszValue) ) ;
}

// タグ属性の総数を取得
//////////////////////////////////////////////////////////////////////////////
int EDescription::GetAttributeCount( void ) const
{
	return	m_wstaAttr.GetSize( ) ;
}

// タグ属性の指標を取得
//////////////////////////////////////////////////////////////////////////////
int EDescription::GetAttributeIndexAs( const wchar_t * pwszAttr )
{
	unsigned int	nIndex ;
	if ( m_wstaAttr.GetAs( pwszAttr, &nIndex ) == NULL )
	{
		return	-1 ;
	}
	return	nIndex ;
}

// タグ属性の名前を取得
//////////////////////////////////////////////////////////////////////////////
EWideString EDescription::GetAttributeNameAt( int nIndex ) const
{
	ETaggedElement<EWideString,EWideString> *	pElement ;
	pElement = m_wstaAttr.GetAt( nIndex ) ;
	if ( pElement == NULL )
		return	EWideString( ) ;
	//
	return	pElement->Tag( ) ;
}

// タグ属性の値を取得
//////////////////////////////////////////////////////////////////////////////
EWideString EDescription::GetAttributeValueAt( int nIndex ) const
{
	ETaggedElement<EWideString,EWideString> *	pElement ;
	pElement = m_wstaAttr.GetAt( nIndex ) ;
	if ( (pElement == NULL) || (pElement->GetObject() == NULL) )
		return	EWideString( ) ;
	//
	return	*(pElement->GetObject()) ;
}

// タグ属性を削除
//////////////////////////////////////////////////////////////////////////////
void EDescription::RemoveAttributeAt( int nIndex )
{
	m_wstaAttr.RemoveAt( nIndex ) ;
}

void EDescription::RemoveAttributeAs( const wchar_t * pwszAttr )
{
	m_wstaAttr.RemoveAs( pwszAttr ) ;
}

// 全てのタグ属性を削除
//////////////////////////////////////////////////////////////////////////////
void EDescription::RemoveAllAttribute( void )
{
	m_wstaAttr.RemoveAll( ) ;
}

// 含まれているタグの数を取得
//////////////////////////////////////////////////////////////////////////////
int EDescription::GetContentTagCount( void ) const
{
	return	m_listDesc.GetSize( ) ;
}

// 含まれているタグを取得
//////////////////////////////////////////////////////////////////////////////
EDescription * EDescription::GetContentTagAt( int nIndex ) const
{
	return	m_listDesc.GetAt( nIndex ) ;
}

// タグを条件を指定して検索
//////////////////////////////////////////////////////////////////////////////
EDescription * EDescription::SearchTagAs
	( int nFirst, const wchar_t * pwszAttr,
		const wchar_t * pwszValue,
		const wchar_t * pwszTag, int * pNext ) const
{
	for ( ; ; )
	{
		int		iFind = nFirst ;
		if ( pwszTag != NULL )
		{
			iFind = FindContentTag( nFirst, pwszTag ) ;
		}
		else if ( GetContentTagAt(iFind) == NULL )
		{
			iFind = -1 ;
		}
		if ( iFind < 0 )
		{
			if ( pNext != NULL )
				*pNext = -1 ;
			return	NULL ;
		}
		nFirst = iFind + 1 ;
		//
		EDescription *	pTag = GetContentTagAt( iFind ) ;
		if ( pwszAttr && pwszValue )
		{
			if ( pTag->GetAttrString(pwszAttr,NULL) != pwszValue )
			{
				continue ;
			}
		}
		if ( pNext != NULL )
		{
			*pNext = nFirst ;
		}
		return	pTag ;
	}
}

// タグを検索して取得
//////////////////////////////////////////////////////////////////////////////
EDescription * EDescription::GetContentTagAs
	( int nFirst, const wchar_t * pwszTag, int * pNext ) const
{
	int	iFind = FindContentTag( nFirst, pwszTag ) ;
	if ( iFind < 0 )
	{
		if ( pNext != NULL )
			*pNext = -1 ;
		return	NULL ;
	}
	if ( pNext != NULL )
	{
		*pNext = iFind + 1 ;
	}
	return	GetContentTagAt( iFind ) ;
}

// タグを生成
//////////////////////////////////////////////////////////////////////////////
EDescription * EDescription::CreateContentTagAs
			( int nFirst, const wchar_t * pwszTag )
{
	EDescription *	pTag = GetContentTagAs( nFirst, pwszTag ) ;
	if ( pTag == NULL )
	{
		pTag = CreateDescription( ) ;
		pTag->SetTag( pwszTag ) ;
		AddContentTag( pTag ) ;
	}
	return	pTag ;
}

// タグを検索
//////////////////////////////////////////////////////////////////////////////
int EDescription::FindContentTag( int nFirst, const wchar_t * pwszTag ) const
{
	for ( int i = nFirst; i < (int) m_listDesc.GetSize(); i ++ )
	{
		EDescription *	pDesc = m_listDesc.GetAt( i ) ;
		ESLAssert( pDesc != NULL ) ;
		if ( pwszTag == NULL )
		{
			if ( pDesc->m_wstrTag.IsEmpty() )
			{
				return	i ;
			}
		}
		else
		{
			if ( !pDesc->m_wstrTag.CompareNoCase( pwszTag ) )
			{
				return	i ;
			}
		}
	}
	return	-1 ;
}

// タグを追加
//////////////////////////////////////////////////////////////////////////////
void EDescription::AddContentTag( EDescription * pDesc )
{
	pDesc->m_pParent = this ;
	m_listDesc.Add( pDesc ) ;
}

// タグを挿入
//////////////////////////////////////////////////////////////////////////////
void EDescription::InsertContentTag( int nIndex, EDescription * pDesc )
{
	if ( nIndex < 0 )
		nIndex = 0 ;
	else if ( nIndex > GetContentTagCount() )
		nIndex = GetContentTagCount( ) ;
	//
	pDesc->m_pParent = this ;
	m_listDesc.InsertAt( nIndex, pDesc ) ;
}

// 文字列属性タグを取得
//////////////////////////////////////////////////////////////////////////////
EWideString EDescription::GetContentString
	( const wchar_t * pwszTag, const wchar_t * pwszDefValue ) const
{
	const EDescription *	pTag = this ;
	if ( pwszTag != NULL )
	{
		int	iTag = FindContentTag( 0, pwszTag ) ;
		pTag = GetContentTagAt( iTag ) ;
	}
	if ( pTag == NULL )
	{
		return	pwszDefValue ;
	}
	EDescription *	pContents = pTag->GetContentTagAt( 0 ) ;
	if ( pContents == NULL )
	{
		return	pwszDefValue ;
	}
	if ( !pContents->Tag().IsEmpty()
			|| (pContents->GetContentTagCount() > 0) )
	{
		return	pwszDefValue ;
	}
	return	pContents->Contents( ) ;
}

// 文字列属性タグを設定
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::SetContentString
	( const wchar_t * pwszTag, const wchar_t * pwszValue )
{
	EDescription *	pTag = this ;
	if ( pwszTag != NULL )
	{
		int	iTag = FindContentTag( 0, pwszTag ) ;
		pTag = GetContentTagAt( iTag ) ;
		if ( pTag == NULL )
		{
			pTag = CreateDescription( ) ;
			pTag->SetTag( pwszTag ) ;
			AddContentTag( pTag ) ;
		}
	}
	EDescription *	pContents = pTag->GetContentTagAt( 0 ) ;
	if ( pContents == NULL )
	{
		pContents = CreateDescription( ) ;
		pTag->AddContentTag( pContents ) ;
	}
	else if ( !pContents->Tag().IsEmpty()
			|| (pContents->GetContentTagCount() > 0) )
	{
		return	eslErrGeneral ;
	}
	pContents->SetContents( pwszValue ) ;
	return	eslErrSuccess ;
}

// タグを削除
//////////////////////////////////////////////////////////////////////////////
void EDescription::RemoveContentTagAt( int nIndex )
{
	m_listDesc.RemoveAt( nIndex ) ;
}

// 全てのタグを削除
//////////////////////////////////////////////////////////////////////////////
void EDescription::RemoveAllContentTag( void )
{
	m_listDesc.RemoveAll( ) ;
}

// タグを検索
//////////////////////////////////////////////////////////////////////////////
EDescription *
	EDescription::GetContentTag( const wchar_t * pwszTagPath ) const
{
	EWideString	wstrTagPath = pwszTagPath ;
	const EDescription *	pdscTag = this ;
	int	iNext = 0 ;
	int	iFind = wstrTagPath.Find( L'\\' ) ;
	while ( iFind >= iNext )
	{
		if ( iFind > iNext )
		{
			pdscTag = pdscTag->GetContentTagAs
				( 0, wstrTagPath.Middle( iNext, iFind - iNext ) ) ;
			if ( pdscTag == NULL )
			{
				return	NULL ;
			}
		}
		iNext = iFind + 1 ;
		iFind = wstrTagPath.Find( L'\\', iNext ) ;
	}
	if ( iNext >= (int) wstrTagPath.GetLength() )
	{
		return	(EDescription*) pdscTag ;
	}
	return	pdscTag->GetContentTagAs( 0, wstrTagPath.Middle( iNext ) ) ;
}

// タグを生成
//////////////////////////////////////////////////////////////////////////////
EDescription * EDescription::CreateContentTag( const wchar_t * pwszTagPath )
{
	EWideString	wstrTagPath = pwszTagPath ;
	EDescription *	pdscTag = this ;
	int	iNext = 0 ;
	int	iFind = wstrTagPath.Find( L'\\' ) ;
	while ( iFind >= iNext )
	{
		if ( iFind > iNext )
		{
			pdscTag = pdscTag->CreateContentTagAs
				( 0, wstrTagPath.Middle( iNext, iFind - iNext ) ) ;
		}
		iNext = iFind + 1 ;
		iFind = wstrTagPath.Find( L'\\', iNext ) ;
	}
	if ( iNext >= (int) wstrTagPath.GetLength() )
	{
		return	pdscTag ;
	}
	return	pdscTag->CreateContentTagAs( 0, wstrTagPath.Middle( iNext ) ) ;
}

// 文字列データ取得
//////////////////////////////////////////////////////////////////////////////
EWideString EDescription::GetStringAt
	( const wchar_t * pwszTagPath,
		const wchar_t * pwszAttr, const wchar_t * pwszDefValue ) const
{
	EDescription *	pdscTag = GetContentTag( pwszTagPath ) ;
	if ( pdscTag != NULL )
	{
		int	iAttr = pdscTag->GetAttributeIndexAs( pwszAttr ) ;
		if ( iAttr >= 0 )
		{
			return	pdscTag->GetAttributeValueAt( iAttr ) ;
		}
		return	pdscTag->GetContentString( pwszAttr, pwszDefValue ) ;
	}
	return	pwszDefValue ;
}

// 整数データ取得
//////////////////////////////////////////////////////////////////////////////
int EDescription::GetIntegerAt
	( const wchar_t * pwszTagPath,
		const wchar_t * pwszAttr, int nDefValue ) const
{
	EStreamWideString	swsValue ;
	swsValue = GetStringAt( pwszTagPath, pwszAttr, NULL ) ;
	int	nRadix = swsValue.GetNumberRadix( ) ;
	if ( nRadix < 0 )
	{
		return	nDefValue ;
	}
	return	swsValue.GetInteger( nRadix ) ;
}

// 実数データ取得
//////////////////////////////////////////////////////////////////////////////
double EDescription::GetRealAt
	( const wchar_t * pwszTagPath,
		const wchar_t * pwszAttr, double rDefValue ) const
{
	EStreamWideString	swsValue ;
	swsValue = GetStringAt( pwszTagPath, pwszAttr, NULL ) ;
	int	nRadix = swsValue.GetNumberRadix( ) ;
	if ( nRadix < 0 )
	{
		return	rDefValue ;
	}
	return	swsValue.GetRealNumber( nRadix ) ;
}

// バイナリデータ取得
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::GetBinaryAt
	( EStreamBuffer & bufBinary,
		const wchar_t * pwszTagPath, const wchar_t * pwszAttr ) const
{
	EString	strValue ;
	strValue = GetStringAt( pwszTagPath, pwszAttr, NULL ) ;
	if ( !strValue.IsEmpty() )
	{
		return	DecodeBase64( bufBinary, strValue ) ;
	}
	return	eslErrSuccess ;
}

// 文字列データ設定
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetStringAt
	( const wchar_t * pwszTagPath,
			const wchar_t * pwszAttr, const wchar_t * pwszValue )
{
	EDescription *	pdscTag = CreateContentTag( pwszTagPath ) ;
	bool	fLongString = false ;
	if ( pwszValue != NULL )
	{
		for ( int i = 0; pwszValue[i]; i ++ )
		{
			if ( (pwszValue[i] < 0x20) || (i >= 32) )
			{
				fLongString = true ;
				break ;
			}
		}
	}
	if ( fLongString )
	{
		int	iAttr = pdscTag->GetAttributeIndexAs( pwszAttr ) ;
		while ( iAttr >= 0 )
		{
			pdscTag->RemoveAttributeAt( iAttr ) ;
			iAttr = pdscTag->GetAttributeIndexAs( pwszAttr ) ;
		}
		if ( pdscTag->SetContentString( pwszAttr, pwszValue ) )
		{
			pdscTag->SetAttrString( pwszAttr, pwszValue ) ;
		}
	}
	else
	{
		int	iTag = pdscTag->FindContentTag( 0, pwszAttr ) ;
		if ( iTag >= 0 )
		{
			EDescription *	pdscContent = pdscTag->GetContentTagAt( iTag ) ;
			if ( pdscContent != NULL )
			{
				if ( pdscContent->GetContentTagCount() <= 1 )
				{
					pdscContent->RemoveAllContentTag( ) ;
				}
				if ( pdscContent->GetAttributeCount() == 0 )
				{
					pdscTag->RemoveContentTagAt( iTag ) ;
				}
			}
		}
		pdscTag->SetAttrString( pwszAttr, pwszValue ) ;
	}
}

// 整数データ設定
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetIntegerAt
	( const wchar_t * pwszTagPath,
		const wchar_t * pwszAttr, int nValue )
{
	if ( (nValue >= -0x8000) && (nValue <= 0x8000) )
	{
		SetStringAt( pwszTagPath, pwszAttr, EWideString( nValue ) ) ;
	}
	else
	{
		EWideString	wstrHex( (DWORD) nValue, 8 ) ;
		if ( (DWORD) nValue >= 0xA0000000 )
		{
			wstrHex = L"0" + wstrHex ;
		}
		wstrHex += L'H' ;
		SetStringAt( pwszTagPath, pwszAttr, EWideString( wstrHex ) ) ;
	}
}

// 実数データ設定
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetRealAt
	( const wchar_t * pwszTagPath,
		const wchar_t * pwszAttr, double rValue )
{
	SetStringAt( pwszTagPath, pwszAttr, EWideString( rValue ) ) ;
}

// バイナリデータ設定
//////////////////////////////////////////////////////////////////////////////
void EDescription::SetBinaryAt
	( const wchar_t * pwszTagPath, const wchar_t * pwszAttr,
				const void * ptrData, unsigned int nBytes )
{
	EString	strText ;
	EncodeBase64( strText, ptrData, nBytes ) ;
	SetStringAt( pwszTagPath, pwszAttr, EWideString( strText ) ) ;
}

// 文字エンコーディングの種類
//////////////////////////////////////////////////////////////////////////////
static const char *	pszCharaEncodingType[5] =
{
	"shift_jis",
	"utf-8",
	"iso-2022-jp",
	"euc-jp",
	NULL
} ;

// 文字エンコーディング種類を判別
//////////////////////////////////////////////////////////////////////////////
EDescription::CharacterEncoding
	EDescription::GetCharaEncodingType( const char * pszType )
{
	for ( int i = 0; pszCharaEncodingType[i]; i ++ )
	{
		if ( !EString::CompareNoCase( pszType, pszCharaEncodingType[i] ) )
		{
			return	(EDescription::CharacterEncoding) i ;
		}
	}
	return	ceShiftJIS ;
}

// 文字エンコーディング種類文字列を取得
//////////////////////////////////////////////////////////////////////////////
const char * EDescription::GetCharaEncodingName
			( EDescription::CharacterEncoding ceType )
{
	if ( (ceType >= 0) && (ceType < 4) )
	{
		return	pszCharaEncodingType[ceType] ;
	}
	return	NULL ;
}

// 文字列を Unicode に復元
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::DecodeText
	( EWideString & wstrText, const EString & strSrc,
			EDescription::CharacterEncoding ceEncoding )
{
	if ( ceEncoding == ceShiftJIS )
	{
		//
		// Shift_JIS -> Unicode 変換
		//
		wstrText = strSrc ;
		return	eslErrSuccess ;
	}
	else if ( ceEncoding == ceUTF8 )
	{
		//
		// UTF-8 -> Unicode 変換
		//
		int		i = 0 ;
		int		nSrcLen = (int) strSrc.GetLength( ) ;
		wstrText.AllocString( nSrcLen + 1 ) ;
		//
		while ( i < nSrcLen )
		{
			wchar_t			unicode ;
			unsigned char	utf8 = strSrc[i ++] ;
			//
			if ( utf8 & 0x80 )
			{
				unsigned char	mask = 0x20 ;
				int	count = 1 ;
				while ( utf8 & mask )
				{
					count ++ ;
					mask >>= 1 ;
					if ( mask == 0 )
						break ;
				}
				//
				unicode = utf8 & ((1 << (6 - count)) - 1) ;
				for ( int j = 0; (j < count) && (i + j < nSrcLen); j ++ )
				{
					unicode = (unicode << 6) | (strSrc[i ++] & 0x3F) ;
				}
			}
			else
			{
				unicode = utf8 ;
			}
			//
			wstrText += unicode ;
		}
		//
		return	eslErrSuccess ;
	}
	else if ( ceEncoding == ceISO2022JP )
	{
		//
		// ISO-2022-JP -> Unicode 変換
		//
		int		i = 0 ;
		int		nSrcLen = (int) strSrc.GetLength( ) ;
		bool	jismode = false ;
		EString	strText ;
		strText.AllocString( nSrcLen + 1 ) ;
		//
		while ( i < nSrcLen )
		{
			//
			// エスケープシーケンス
			while ( (strSrc[i] == 0x1B) && (i + 2 < nSrcLen) )
			{
				if ( ((strSrc[i + 1] == '$')
						&& (strSrc[i + 2] == 'B')) ||
					((strSrc[i + 1] == '$')
						&& (strSrc[i + 2] == '@')) ||
					((strSrc[i + 1] == '(')
						&& (strSrc[i + 2] == 'J')) )
				{
					jismode = true ;
					i += 3 ;
				}
				else if ( (strSrc[i + 1] == '(')
							&& (strSrc[i + 2] == 'B') )
				{
					jismode = false ;
					i += 3 ;
				}
				else
				{
					break ;
				}
			}
			if ( i >= nSrcLen )
			{
				break ;
			}
			//
			// ポインタを進める
			if ( jismode )
			{
				int	jiscode =
					((int)((unsigned char)strSrc[i]) << 8)
							| ((unsigned char)strSrc[i + 1]) ;
				i += 2 ;
				//
				BYTE	j1 = (BYTE) (jiscode >> 8) ;
				BYTE	j2 = (BYTE) jiscode ;
				if ( j1 & 0x01 )
					j2 += 0x1F ;
				else
					j2 += 0x7D ;
				if ( j2 >= 0x7F )
					j2 ++ ;
				j1 = ((j1 - 0x21) >> 1) + 0x81 ;
				if ( j1 > 0x9F )
					j1 += 0x40 ;
				//
				strText += (char) j1 ;
				strText += (char) j2 ;
			}
			else
			{
				strText += strSrc[i ++] ;
			}
		}
		//
		wstrText = strText ;
		return	eslErrSuccess ;
	}
	else if ( ceEncoding == ceEUCJP )
	{
		//
		// EUC-JP -> Unicode 変換
		//
		EString	strText ;
		int		i = 0 ;
		int		nSrcLen = (int) strSrc.GetLength( ) ;
		strText.AllocString( nSrcLen + 1 ) ;
		//
		while ( i < nSrcLen )
		{
			unsigned char	c = strSrc[i] ;
			if ( (c & 0x80) && (i + 1 < nSrcLen) )
			{
				unsigned char	c2 = strSrc[++ i] ;
				if ( c == 0x8E )
				{
					strText += (char) c2 ;
				}
				else if ( (c2 >= 0xA0) && (c2 <= 0xFF) )
				{
					BYTE	j1 = c - 0x80 ;
					BYTE	j2 = c2 - 0x80 ;
					//
					if ( j1 & 0x01 )
						j2 += 0x1F ;
					else
						j2 += 0x7D ;
					if ( j2 >= 0x7F )
						j2 ++ ;
					j1 = ((j1 - 0x21) >> 1) + 0x81 ;
					if ( j1 > 0x9F )
						j1 += 0x40 ;
					//
					strText += (char) j1 ;
					strText += (char) j2 ;
				}
				else
				{
					strText += (char) c ;
					strText += (char) c2 ;
				}
			}
			else
			{
				strText += (char) c ;
			}
			i ++ ;
		}
		//
		wstrText = strText ;
		return	eslErrSuccess ;
	}
	//
	return	ESLErrorMsg( "不正な文字エンコーディングが指定されました。" ) ;
}

// 文字列を Unicode から符号化
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::EncodeText
	( EString & strText, const EWideString & wstrSrc,
				EDescription::CharacterEncoding ceEncoding )
{
	if ( ceEncoding == ceShiftJIS )
	{
		//
		// Unicode -> ShiftJIS 変換
		//
		strText = wstrSrc ;
		return	eslErrSuccess ;
	}
	else if ( ceEncoding == ceUTF8 )
	{
		//
		// Unicode -> UTF-8 変換
		//
		int		i = 0 ;
		int		nSrcLen = (int) wstrSrc.GetLength( ) ;
		strText = "" ;
		strText.AllocString( nSrcLen * 3 / 2 ) ;
		//
		while ( i < nSrcLen )
		{
			DWORD	ucs4 = wstrSrc[i ++] ;
			//
			if ( ucs4 < 0x80U )
			{
				strText += (char) ucs4 ;
			}
			else
			{
				int	count = 1 ;
				signed char	first_char = -0x40 ;
				DWORD	range = 0x800 ;
				while ( ucs4 >= range )
				{
					count ++ ;
					range <<= 5 ;
					first_char >>= 1 ;
				}
				//
				int	first_width = 6 - count ;
				ucs4 <<= (32 - (count * 6 + first_width)) ;
				//
				strText += (char) (first_char | (ucs4 >> (32 - first_width))) ;
				ucs4 <<= first_width ;
				//
				for ( int i = 0; i < count; i ++ )
				{
					strText += (char) (0x80 | (ucs4 >> 26)) ;
					ucs4 <<= 6 ;
				}
			}
		}
		return	eslErrSuccess ;
	}
	else if ( ceEncoding == ceISO2022JP )
	{
		//
		// Unicode -> ISO-2022-JP 変換
		//
		EString	strSrc = wstrSrc ;
		int		i = 0 ;
		int		nSrcLen = (int) strSrc.GetLength( ) ;
		strText = "" ;
		strText.AllocString( nSrcLen * 3 / 2 ) ;
		//
		while ( i < nSrcLen )
		{
			unsigned char	c = strSrc[i] ;
			if ( (c >= 0xF0) || ((c >= 0x80) && (c < 0xA0)) )
			{
				strText += "\x1B$B" ;
				//
				while ( (i + 1 < nSrcLen)
						&& ((c >= 0xF0) || ((c >= 0x80) && (c < 0xA0))) )
				{
					BYTE	j1 = (BYTE) strSrc[i] ;
					BYTE	j2 = (BYTE) strSrc[i + 1] ;
					i += 2 ;
					//
					if ( j1 <= 0x9F )
						j1 -= 0x71 ;
					else
						j1 -= 0xB1 ;
					j1 = j1 * 2 + 1 ;
					if ( j2 > 0x7F )
						j2 -- ;
					if ( j2 >= 0x9E )
					{
						j2 -= 0x7D ;
						j1 ++ ;
					}
					else
						j2 -= 0x1F ;
					//
					strText += (char) j1 ;
					strText += (char) j2 ;
				}
				//
				strText += "\x1B(B" ;
			}
			else
			{
				i ++ ;
				strText += (char) c ;
			}
		}
		return	eslErrSuccess ;
	}
	else if ( ceEncoding == ceEUCJP )
	{
		//
		// Unicode -> EUC-JP 変換
		//
		EString	strSrc = wstrSrc ;
		int		i = 0 ;
		int		nSrcLen = (int) strSrc.GetLength( ) ;
		strText = "" ;
		strText.AllocString( nSrcLen ) ;
		//
		while ( i < nSrcLen )
		{
			unsigned char	c = strSrc[i ++] ;
			if ( c & 0x80 )
			{
				if ( ((c >= 0xA0) && (c < 0xE0)) || (c >= 0xEF) )
				{
					strText += (char) 0x8E ;
					strText += (char) c ;
				}
				else
				{
					BYTE	j1 = c ;
					BYTE	j2 = (BYTE) strSrc[i ++] ;
					//
					if ( j1 <= 0x9F )
						j1 -= 0x71 ;
					else
						j1 -= 0xB1 ;
					j1 = j1 * 2 + 1 ;
					if ( j2 > 0x7F )
						j2 -- ;
					if ( j2 >= 0x9E )
					{
						j2 -= 0x7D ;
						j1 ++ ;
					}
					else
						j2 -= 0x1F ;
					//
					strText += (char) (j1 + 0x80) ;
					strText += (char) (j2 + 0x80) ;
				}
			}
			else
			{
				strText += (char) c ;
			}
		}
		return	eslErrSuccess ;
	}
	//
	return	ESLErrorMsg( "不正な文字エンコーディングが指定されました。" ) ;
}

// 適切な文字コードを選択する
//////////////////////////////////////////////////////////////////////////////
EDescription::CharacterEncoding
	EDescription::GetCharacterEncoding( const EString & strText )
{
	const int	maskShiftJIS	= 0x0001 ;
	const int	maskUTF8		= 0x0002 ;
	const int	maskISO2022JP	= 0x0004 ;
	const int	maskEUCJP		= 0x0008 ;
	const int	maskAll			= 0x000F ;
	int			i, nTextLen = strText.GetLength( ) ;
	DWORD		dwFlags = maskAll ;
	bool		fEscJIS = false ;
	bool		fSecondSJIS = false ;
	int			nUTFCount = 0 ;
	//
	for ( i = 0; dwFlags && (i < nTextLen); i ++ )
	{
		if ( (dwFlags & 1) + ((dwFlags >> 1) & 1)
			+ ((dwFlags >> 2) & 1) + ((dwFlags >> 3) & 1) <= 1 )
		{
			break ;
		}
		unsigned char	c = strText[i] ;
		if ( !(c & 0x80) )
		{
			if ( fSecondSJIS )
			{
				if ( c < 0x40 )
				{
					dwFlags &= ~maskShiftJIS ;
				}
				fSecondSJIS = false ;
			}
			if ( nUTFCount != 0 )
			{
				dwFlags &= ~maskUTF8 ;
			}
			if ( (dwFlags & maskISO2022JP)
					&& (c == 0x1B) && (i + 2 < nTextLen) )
			{
				if ( (strText[i + 1] == '$') && (strText[i + 2] == 'B') )
				{
					fEscJIS = true ;
				}
			}
			continue ;			// ASCII
		}
		//
		dwFlags &= ~maskISO2022JP ;
		//
		if ( (c < 0xA0) && (c != 0x8E) && (c != 0x8F) )
		{
			dwFlags &= ~maskEUCJP ;
		}
		if ( fSecondSJIS )
		{
			if ( c > 0xFC )
			{
				dwFlags &= ~maskShiftJIS ;
			}
			fSecondSJIS = false ;
		}
		else
		{
			if ( c >= 0xF0 )
			{
				dwFlags &= ~maskShiftJIS ;
			}
			fSecondSJIS = (c >= 0x80) && (c < 0xA0) || (c >= 0xE0) ;
		}
		if ( dwFlags & maskUTF8 )
		{
			if ( nUTFCount == 0 )
			{
				unsigned char	mask = 0x20 ;
				int		count = 1 ;
				while ( c & mask )
				{
					count ++ ;
					mask >>= 1 ;
					if ( mask == 0 )
						break ;
				}
				nUTFCount = count ;
			}
			else
			{
				nUTFCount -- ;
			}
		}
	}
	if ( (dwFlags & maskISO2022JP) && fEscJIS )
	{
		return	ceISO2022JP ;
	}
	if ( dwFlags & maskShiftJIS )
	{
		return	ceShiftJIS ;
	}
	if ( dwFlags & maskEUCJP )
	{
		return	ceEUCJP ;
	}
	if ( dwFlags & maskUTF8 )
	{
		return	ceUTF8 ;
	}
	return	ceUnknown ;
}

// C 言語エスケープシーケンスを復元
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::DecodeTextCEscSequence( EWideString & wstrText )
{
	int		iFind ;
	iFind = wstrText.Find( L'\\' ) ;
	if ( iFind < 0 )
	{
		return	eslErrSuccess ;
	}
	//
	int		iLast = 0 ;
	EWideString	wstrBuf ;
	do
	{
		wstrBuf += wstrText.Middle( iLast, iFind - iLast ) ;
		ESLAssert( wstrText[iFind] == '\\' ) ;
		//
		wchar_t	wch = wstrText.GetAt( iFind + 1 ) ;
		if ( (wch == L'x') || ((wch >= L'0') && (wch < L'8')) )
		{
			wchar_t	wchCode = 0 ;
			wchar_t	wchRadix = 8 ;
			if ( wch == L'x' )
			{
				wchRadix = 0x10 ;
				iFind += 2 ;
			}
			else
			{
				iFind ++ ;
			}
			for ( int i = 0; i < 3; i ++ )
			{
				wch = wstrText.GetAt( iFind ) ;
				wchar_t	wchNum = 16 ;
				if ( (wch >= L'0') && (wch <= L'9') )
					wchNum = (wch - L'0') ;
				else if ( (wch >= L'A') && (wch <= L'F') )
					wchNum = (wch - L'A' + 10) ;
				else if ( (wch >= L'a') && (wch <= L'f') )
					wchNum = (wch - L'a' + 10) ;
				if ( wchNum >= wchRadix )
					break ;
				wchCode = (wchCode * wchRadix) + wchNum ;
				++ iFind ;
			}
			//
			wstrBuf += wchCode ;
			iLast = iFind ;
			iFind = wstrText.Find( L'\\', iFind ) ;
		}
		else
		{
			static const wchar_t	wchEscChar[] =
			{
				L'n', L'r', L't', L'v', L'b', L'a',
				L'f', L'?', L'\\', '\"', '\'', 0
			} ;
			static const wchar_t	wchEscCode[] =
			{
				L'\n', L'\r', L'\t', L'\v', L'\b', L'\a',
				L'\f', L'?', L'\\', '\"', '\'', 0
			} ;
			int		i ;
			for ( i = 0; wchEscChar[i]; i ++ )
			{
				if ( wch == wchEscChar[i] )
					break ;
			}
			if ( wchEscChar[i] )
			{
				wstrBuf += wchEscCode[i] ;
				iFind ++ ;
			}
			iLast = ++ iFind ;
			iFind = wstrText.Find( L'\\', iFind ) ;
		}
	}
	while ( iFind >= 0 ) ;
	//
	wstrText = wstrBuf + wstrText.Middle( iLast ) ;
	return	eslErrSuccess ;
}

// C 言語エスケープシーケンスに変換
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::EncodeTextCEscSequence( EWideString & wstrText )
{
	static const wchar_t	wchEscChar[] =
	{
		L'n', L'r', L't', L'\\', 0
	} ;
	static const wchar_t	wchEscCode[] =
	{
		L'\n', L'\r', L'\t', L'\\', 0
	} ;
	int		i = 0, iLast = 0 ;
	int		nTextLen = (int) wstrText.GetLength( ) ;
	EWideString	wstrBuf ;
	//
	while ( i < nTextLen )
	{
		wchar_t	wch = wstrText[i] ;
		if ( (wch < L' ') || (wch == L'\\')
				|| (wch == L'\"') || (wch == L'\'') )
		{
			wstrBuf += wstrText.Middle( iLast, i - iLast ) ;
			iLast = i + 1 ;
			//
			int		j ;
			for ( j = 0; wchEscCode[j]; j ++ )
			{
				if ( wchEscCode[j] == wch )
					break ;
			}
			if ( wchEscChar[j] )
			{
				wstrBuf += L'\\' ;
				wstrBuf += wchEscChar[j] ;
			}
			else
			{
				wstrBuf += L"\\x" ;
				//
				wchar_t	wchNext = wstrText.GetAt( i + 1 ) ;
				if ( ((wchNext >= L'0') && (wchNext <= L'9'))
					|| ((wchNext >= L'A') && (wchNext <= L'F'))
					|| ((wchNext >= L'a') && (wchNext <= L'f')) )
				{
					wstrBuf += L'0' ;
				}
				//
				wchar_t	wch1 = (wch >> 4) & 0x0F ;
				wchar_t	wch2 = wch & 0x0F ;
				//
				if ( wch1 < 10 )
					wstrBuf += wch1 + L'0' ;
				else
					wstrBuf += wch1 - 10 + L'A' ;
				//
				if ( wch2 < 10 )
					wstrBuf += wch2 + L'0' ;
				else
					wstrBuf += wch2 - 10 + L'A' ;
			}
		}
		i ++ ;
	}
	//
	wstrText = wstrBuf + wstrText.Middle( iLast ) ;
	return	eslErrSuccess ;
}

// 文字列コンテンツのデコード
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::DecodeTextContents( EWideString & wstrText )
{
	static const wchar_t *	pwszSpecChar[] =
	{
		L"lt", L"gt", L"quot", L"amp", L"nbsp", NULL
	} ;
	static const wchar_t	wchSpecChar[] =
	{
		L'<', L'>', L'\"', L'&', L' ', L'\0'
	} ;
	//
	int		iFind ;
	iFind = wstrText.Find( L'&' ) ;
	if ( iFind < 0 )
	{
		return	eslErrSuccess ;
	}
	//
	wchar_t	wchCode ;
	int		i, iLast = 0 ;
	EWideString	wstrBuf ;
	EWideString	wstrToken ;
	do
	{
		wstrBuf += wstrText.Middle( iLast, iFind - iLast ) ;
		ESLAssert( wstrText[iFind] == '&' ) ;
		//
		iLast = ++ iFind ;
		iFind = wstrText.Find( L';', iFind ) ;
		if ( iFind < 0 )
		{
			iFind = wstrText.Find( L'&', iLast -- ) ;
			continue ;
		}
		//
		wstrToken = wstrText.Middle( iLast, iFind - iLast ) ;
		wstrToken.TrimRight( ) ;
		wstrToken.TrimLeft( ) ;
		//
		if ( wstrToken.GetAt(0) == L'#' )
		{
			wchCode = 0 ;
			for ( i = 1; i < (int) wstrToken.GetLength(); i ++ )
			{
				wchar_t	wch = wstrToken.GetAt(i) ;
				if ( (wch < L'0') || (wch > L'9') )
					break ;
				wchCode = (wchCode * 10) + (wch - L'0') ;
			}
		}
		else
		{
			for ( i = 0; pwszSpecChar[i] != NULL; i ++ )
			{
				if ( wstrToken == pwszSpecChar[i] )
					break ;
			}
			if ( pwszSpecChar[i] )
			{
				wchCode = wchSpecChar[i] ;
			}
			else
			{
				iFind = wstrText.Find( L'&', iLast -- ) ;
				continue ;
			}
		}
		//
		wstrBuf += wchCode ;
		iLast = ++ iFind ;
		iFind = wstrText.Find( L'&', iFind ) ;
	}
	while ( iFind >= 0 ) ;
	//
	wstrText = wstrBuf + wstrText.Middle( iLast ) ;
	return	eslErrSuccess ;
}

// 文字列コンテンツのエンコーディング
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::EncodeTextContents
	( EWideString & wstrText, const EWideString & wstrSrc, int nTabIndent )
{
	static const wchar_t *	pwszSpecChar[] =
	{
		L"&lt;", L"&gt;", L"&quot;", L"&amp;", L"&nbsp;", NULL
	} ;
	static const wchar_t	wchSpecChar[] =
	{
		L'<', L'>', L'\"', L'&', L' ', L'\0'
	} ;
	//
	wstrText = L"" ;
	wstrText.AllocString( wstrSrc.GetLength() + 0x10 ) ;
	//
	for ( unsigned int i = 0; i < wstrSrc.GetLength(); i ++ )
	{
		wchar_t	wch = wstrSrc.GetAt( i ) ;
		if ( wch <= L' ' )
		{
			wstrText += L"&#" + EWideString( (int) wch ) + L";" ;
			//
			if ( (wch == L'\n') && (i + 1 < wstrSrc.GetLength()) )
			{
				if ( nTabIndent >= 0 )
				{
					wstrText += L"\r\n" ;
					for ( int j = 0; j < nTabIndent; j ++ )
					{
						wstrText += '\t' ;
					}
				}
			}
		}
		else
		{
			int		j ;
			for ( j = 0; wchSpecChar[j]; j ++ )
			{
				if ( wch == wchSpecChar[j] )
					break ;
			}
			if ( wchSpecChar[j] )
			{
				wstrText += pwszSpecChar[j] ;
			}
			else
			{
				wstrText += wch ;
			}
		}
	}
	//
	return	eslErrSuccess ;
}

// base64 から復元
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::DecodeBase64
	( EStreamBuffer & buf, const char * pszText )
{
	//
	// 出力バッファを確保
	//
	int		nSrcLen = 0 ;
	while ( pszText[nSrcLen] )
		nSrcLen ++ ;
	//
	BYTE *	ptrDst = (BYTE*) buf.PutBuffer( nSrcLen * 3 / 4 ) ;
	//
	// 復号
	//
	int		iSrc = 0 ;
	int		iDst = 0 ;
	ESLError	err = eslErrSuccess ;
	//
	while ( iSrc < nSrcLen )
	{
		//
		// 空白を読み飛ばす
		//
		while ( iSrc < nSrcLen )
		{
			if ( (unsigned char) pszText[iSrc] > (unsigned char) ' ' )
				break ;
			iSrc ++ ;
		}
		if ( iSrc >= nSrcLen )
			break ;
		//
		// 4文字取得
		//
		BYTE	bin[4] ;	// 取得したバイナリ
		int		j = 0 ;		// 無効なバイト数
		for ( int i = 0; i < 4; i ++ )
		{
			if ( iSrc >= nSrcLen )
			{
				err = eslErrGeneral ;
				break ;
			}
			char	c = pszText[iSrc ++] ;
			if ( (c >= 'A') && (c <= 'Z') )
			{
				c -= 'A' ;
			}
			else if ( (c >= 'a') && (c <= 'z') )
			{
				c -= ('a' - 0x1A) ;
			}
			else if ( (c >= '0') && (c <= '9') )
			{
				c -= ('0' - 0x34) ;
			}
			else if ( c == '+' )
			{
				c = 0x3E ;
			}
			else if ( c == '/' )
			{
				c = 0x3f ;
			}
			else if ( c == '=' )
			{
				j ++ ;
				c = 0 ;
			}
			else
			{
				err = eslErrGeneral ;
				break ;
			}
			bin[i] = c ;
   		}
		if ( err )
			break ;
		//
		// 3バイト出力
		//
		if ( j == 0 )
		{
			// 末尾以外の処理
			ptrDst[iDst ++] = (bin[0] << 2) | (bin[1] >> 4) ;
			ptrDst[iDst ++] = ((bin[1] & 0x0F) << 4) | (bin[2] >> 2) ;
			ptrDst[iDst ++] = ((bin[2] & 0x03) << 6) | bin[3] ;
		}
		else
		{
			// 末尾処理
			ptrDst[iDst ++] = (bin[0] << 2) | (bin[1] >> 4) ;
			if ( j < 2 )
			{
				ptrDst[iDst ++] = ((bin[1] & 0x0F) << 4) | (bin[2] >> 2) ;
			}
			break ;
		}
	}
	//
	// バッファを確定
	//
	buf.Flush( iDst ) ;

	return	err ;
}

// base64 に符号化
//////////////////////////////////////////////////////////////////////////////
ESLError EDescription::EncodeBase64
	( EString & strText, const void * ptrBuf, unsigned int nBytes )
{
	//
	// base64 変換テーブル
	//
	static const char	chrBase64Tbl[] =
										"ABCDEFGHIJKLMNOP"
										"QRSTUVWXYZabcdef"
										"ghijklmnopqrstuv"
										"wxyz0123456789+/" ;
	//
	// 変換ループ
	//
	char *	pszDst = strText.GetBuffer( (nBytes * 4 + 2) / 3 + 1 ) ;
	const BYTE *	ptrData = (const BYTE *) ptrBuf ;
	int		iDst = 0, iSrc = 0 ;
	while ( nBytes != 0 )
	{
		if ( nBytes >= 3 )
		{
			//
			// 末尾以外処理
			//
			pszDst[iDst ++] =
				chrBase64Tbl[(ptrData[iSrc] >> 2)] ;
			pszDst[iDst ++] =
				chrBase64Tbl[((ptrData[iSrc] & 0x03) << 4) | (ptrData[iSrc+1] >> 4)] ;
			pszDst[iDst ++] =
				chrBase64Tbl[((ptrData[iSrc+1] & 0x0F) << 2) | (ptrData[iSrc+2] >> 6)] ;
			pszDst[iDst ++] =
				chrBase64Tbl[(ptrData[iSrc+2] & 0x3F)] ;
			iSrc += 3 ;
			nBytes -= 3 ;
		}
		else
		{
			//
			// 末尾用特殊処理
			//
			pszDst[iDst ++] =
				chrBase64Tbl[(ptrData[iSrc] >> 2)] ;
			if ( nBytes <= 1 )
			{
				pszDst[iDst ++] =
					chrBase64Tbl[((ptrData[iSrc] & 0x03) << 4)] ;
				pszDst[iDst ++] = '=' ;
				pszDst[iDst ++] = '=' ;
			}
			else
			{
				pszDst[iDst ++] = chrBase64Tbl
					[((ptrData[iSrc] & 0x03) << 4) | (ptrData[iSrc+1] >> 4)] ;
				pszDst[iDst ++] =
					chrBase64Tbl[((ptrData[iSrc+1] & 0x0F) << 2)] ;
				pszDst[iDst ++] = '=' ;
			}
			break ;
		}
	}
	strText.ReleaseBuffer( iDst ) ;
	return	eslErrSuccess ;
}

#endif
