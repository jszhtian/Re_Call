
/*****************************************************************************
                         E R I S A - L i b r a r y
													last update 2003/03/07
 -----------------------------------------------------------------------------
	Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>


/*****************************************************************************
                  EMC (Entis Media Complex) �t�@�C���`��
 *****************************************************************************/

char	EMCFile::m_cDefSignature[8] = { 'E', 'n', 't', 'i', 's', 0x1A, 0, 0 } ; 

// �N���X���
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( EMCFile, ESLFileObject )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
EMCFile::EMCFile( void )
{
	m_pFile = NULL ;
	m_pRecord = NULL ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
EMCFile::~EMCFile( void )
{
	Close( ) ;
}

// �t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError EMCFile::Open( ESLFileObject * pfile, const FILE_HEADER * pfhHeader )
{
	//
	// ���݂̃t�@�C�������
	//
	Close( ) ;
	//
	// �t�@�C�����֘A�t���ĊJ��
	//
	ESLAssert( pfile != NULL ) ;
	m_pFile = pfile ;
	SetAttribute( m_pFile->GetAttribute() ) ;
	//
	if ( (GetAttribute() & modeCreateFlag) && (pfhHeader != NULL) )
	{
		//
		// �t�@�C���w�b�_�������o��
		//
		if ( m_pFile->Write
			( pfhHeader, sizeof(FILE_HEADER) ) < sizeof(FILE_HEADER) )
		{
			ESLTrace( "�t�@�C���w�b�_�̏����o���Ɏ��s���܂����B\n" ) ;
			return	eslErrGeneral ;
		}
		//
		// ���[�g���R�[�h��ݒ肷��
		//
		m_pRecord = new RECORD_INFO ;
		m_pRecord->pParent = NULL ;
		m_pRecord->dwWriteFlag = 1 ;
		m_pRecord->qwBasePos = m_pFile->GetLargePosition( ) ;
		m_pRecord->rechdr.nRecordID = 0 ;
		m_pRecord->rechdr.nRecLength = 0 ;
	}
	else
	{
		//
		// �t�@�C���w�b�_��ǂݍ���
		//
		SetAttribute( m_pFile->GetAttribute() & ~modeCreateFlag ) ;
		if ( m_pFile->Read
			( &m_fhHeader, sizeof(FILE_HEADER) ) < sizeof(FILE_HEADER) )
		{
			ESLTrace( "�t�@�C���w�b�_�̓ǂݍ��݂Ɏ��s���܂����B\n" ) ;
			return	eslErrGeneral ;
		}
		if ( (m_fhHeader.dwReserved != 0) ||
				(*((UINT64*)&m_fhHeader.cHeader[0])
					!= *((UINT64*)m_cDefSignature)) )
		{
			ESLTrace( "�s���ȃt�@�C���w�b�_�ł��B\n" ) ;
			return	eslErrGeneral ;
		}
		//
		// ���[�g���R�[�h��ǂݍ���
		//
		m_pRecord = new RECORD_INFO ;
		m_pRecord->pParent = NULL ;
		m_pRecord->dwWriteFlag = 0 ;
		m_pRecord->qwBasePos = m_pFile->GetLargePosition( ) ;
		m_pRecord->rechdr.nRecordID = 0 ;
		m_pRecord->rechdr.nRecLength =
			m_pFile->GetLargeLength() - m_pRecord->qwBasePos ;
	}
	//
	return	eslErrSuccess ;
}

// �t�@�C�������
//////////////////////////////////////////////////////////////////////////////
void EMCFile::Close( void )
{
	if ( m_pFile )
	{
		while ( m_pRecord != NULL )
		{
			if ( AscendRecord( ) )
			{
				break ;
			}
		}
	}
	while ( m_pRecord != NULL )
	{
		RECORD_INFO *	pParent = m_pRecord->pParent ;
		delete	m_pRecord ;
		m_pRecord = pParent ;
	}
	m_pFile = NULL ;
}

// EMC �t�@�C���𕡐�
//////////////////////////////////////////////////////////////////////////////
ESLFileObject * EMCFile::Duplicate( void ) const
{
	EMCFile *	pDupFile = new EMCFile ;
	//
	pDupFile->SetAttribute( GetAttribute() ) ;
	pDupFile->m_pFile = m_pFile->Duplicate( ) ;
	pDupFile->m_pFile->SeekLarge( m_pFile->GetLargePosition(), FromBegin ) ;
	memcpy( &pDupFile->m_fhHeader, &m_fhHeader, sizeof(FILE_HEADER) ) ;
	//
	pDupFile->m_pRecord = NULL ;
	if ( m_pRecord != NULL )
	{
		RECORD_INFO *	pSrcRec = m_pRecord ;
		RECORD_INFO *	pDstRec = new RECORD_INFO ;
		pDupFile->m_pRecord = pDstRec ;
		//
		for ( ; ; )
		{
			pDstRec->dwWriteFlag = pSrcRec->dwWriteFlag ;
			pDstRec->qwBasePos = pSrcRec->qwBasePos ;
			pDstRec->rechdr = pSrcRec->rechdr ;
			//
			if ( pSrcRec->pParent == NULL )
			{
				pDstRec->pParent = NULL ;
				break ;
			}
			else
			{
				pDstRec->pParent = new RECORD_INFO ;
				pSrcRec = pSrcRec->pParent ;
				pDstRec = pDstRec->pParent ;
			}
		}
	}
	//
	return	pDupFile ;
}

// �t�@�C���w�b�_������
//////////////////////////////////////////////////////////////////////////////
void EMCFile::SetFileHeader
	( EMCFile::FILE_HEADER & fhHeader, DWORD dwFileID, const char * pszDesc )
{
	static const char * const	pszFileDesc[] =
	{
		"ERISA-Archive file",
		"Entis Rasterized Image format",
		"EGL-3D surface attribute file",
		"EGL-3D model object file",
		"undefined EMC file"
	} ;
	static const DWORD	dwFileIDs[] =
	{
		(DWORD) fidArchive,
		(DWORD) fidRasterizedImage,
		(DWORD) fidEGL3DSurface,
		(DWORD) fidEGL3DModel,
		(DWORD) fidUndefinedEMC
	} ;
	//
	*((UINT64*)fhHeader.cHeader) = *((UINT64*)m_cDefSignature) ;
	fhHeader.dwFileID = dwFileID ;
	fhHeader.dwReserved = 0 ;
	//
	int		i ;
	if ( pszDesc == NULL )
	{
		for ( i = 0; dwFileIDs[i] != (DWORD) fidUndefinedEMC; i ++ )
		{
			if ( dwFileIDs[i] == dwFileID )
			{
				break ;
			}
		}
		pszDesc = pszFileDesc[i] ;
	}
	//
	for ( i = 0; (i < 0x29) && (pszDesc[i] != '\0'); i ++ )
	{
		fhHeader.cFormatDesc[i] = pszDesc[i] ;
	}
	while ( i < 0x30 )
	{
		fhHeader.cFormatDesc[i ++] = '\0' ;
	}
}

// �f�t�H���g�t�@�C���V�O�l�`�����擾
//////////////////////////////////////////////////////////////////////////////
void EMCFile::GetFileSignature( char cHeader[8] )
{
	*((UINT64*)cHeader) = *((UINT64*)m_cDefSignature) ;
}

// �f�t�H���g�t�@�C���V�O�l�`����ݒ�
//////////////////////////////////////////////////////////////////////////////
void EMCFile::SetFileSignature( const char cHeader[8] )
{
	*((UINT64*)m_cDefSignature) = *((UINT64*)cHeader) ;
}

// ���R�[�h���J��
//////////////////////////////////////////////////////////////////////////////
ESLError EMCFile::DescendRecord( const UINT64 * pRecID )
{
	if ( GetAttribute() & modeCreateFlag )
	{
		//
		// �V�K�Ƀ��R�[�h�𐶐�����
		//
		ESLAssert( pRecID != NULL ) ;
		RECORD_HEADER	rechdr ;
		rechdr.nRecordID = *pRecID ;
		rechdr.nRecLength = 0 ;
		if ( m_pFile->Write( &rechdr, sizeof(rechdr) ) < sizeof(rechdr) )
		{
			ESLTrace( "���R�[�h�w�b�_�̏����o���Ɏ��s���܂����B\n" ) ;
			return	eslErrGeneral ;
		}
		//
		RECORD_INFO *	pRec = new RECORD_INFO ;
		pRec->pParent = m_pRecord ;
		pRec->dwWriteFlag = 1 ;
		pRec->qwBasePos = m_pFile->GetLargePosition( ) ;
		pRec->rechdr = rechdr ;
		m_pRecord = pRec ;
	}
	else
	{
		//
		// �����̃��R�[�h���J��
		//
		RECORD_HEADER	rechdr ;
		for ( ; ; )
		{
			if ( Read( &rechdr, sizeof(rechdr) ) < sizeof(rechdr) ) 
			{
				return	eslErrGeneral ;
			}
/*			if ( (rechdr.nRecLength != 0xFFFFFFFFFFFFFFFF)
					&& (rechdr.nRecLength & 0xFFFFFFFF00000000) )
			{
				ESLTrace( "�s���ȃ��R�[�h���ł��B\n" ) ;
				return	eslErrGeneral ;
			}
*/			if ( pRecID == NULL )
				break ;
			if ( *pRecID == rechdr.nRecordID )
				break ;
			//
			m_pFile->SeekLarge( rechdr.nRecLength, FromCurrent ) ;
		}
		//
		RECORD_INFO *	pRec = new RECORD_INFO ;
		pRec->pParent = m_pRecord ;
		pRec->dwWriteFlag = 0 ;
		pRec->qwBasePos = m_pFile->GetLargePosition( ) ;
		pRec->rechdr = rechdr ;
		m_pRecord = pRec ;
		//
		if ( rechdr.nRecLength == 0xFFFFFFFFFFFFFFFF )
		{
			pRec->rechdr.nRecLength
				= m_pFile->GetLargeLength() - pRec->qwBasePos ;
		}
	}
	//
	return	eslErrSuccess ;
}

// ���R�[�h�����
//////////////////////////////////////////////////////////////////////////////
ESLError EMCFile::AscendRecord( void )
{
	if ( m_pRecord != NULL )
	{
		if ( m_pRecord->dwWriteFlag )
		{
			ESLAssert( GetAttribute() & modeWrite ) ;
			UINT64	nPos = GetLargePosition( ) ;
			if ( nPos > m_pRecord->rechdr.nRecLength )
			{
				m_pRecord->rechdr.nRecLength = nPos ;
			}
			m_pFile->SeekLarge
				( m_pRecord->qwBasePos - sizeof(RECORD_HEADER), FromBegin ) ;
			if ( m_pFile->Write
				( &(m_pRecord->rechdr),
					sizeof(RECORD_HEADER) ) < sizeof(RECORD_HEADER) )
			{
				ESLTrace( "���R�[�h�w�b�_�̏����o���Ɏ��s���܂����B\n" ) ;
			}
		}
		//
		SeekLarge( GetLargeLength(), FromBegin ) ;
		//
		RECORD_INFO *	pRec = m_pRecord ;
		m_pRecord = m_pRecord->pParent ;
		delete	pRec ;
	}
	//
	return	eslErrSuccess ;
}

// �f�[�^��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
unsigned long int EMCFile::Read
	( void * ptrBuffer, unsigned long int nBytes )
{
	ESLAssert( m_pFile != NULL ) ;
	ESLAssert( m_pRecord != NULL ) ;
	//
	INT64	nPos = m_pFile->GetLargePosition() - m_pRecord->qwBasePos ;
	if ( (UINT64) nPos + nBytes > m_pRecord->rechdr.nRecLength )
	{
		nBytes = (DWORD) (m_pRecord->rechdr.nRecLength - nPos) ;
	}
	//
	if ( (signed long int) nBytes < 0 )
	{
		nBytes = 0 ;
	}
	return	m_pFile->Read( ptrBuffer, nBytes ) ;
}

// �f�[�^�������o��
//////////////////////////////////////////////////////////////////////////////
unsigned long int EMCFile::Write
	( const void * ptrBuffer, unsigned long int nBytes )
{
	ESLAssert( m_pFile != NULL ) ;
	ESLAssert( m_pRecord != NULL ) ;
	//
	if ( !(GetAttribute() & modeCreateFlag) )
	{
		INT64	nPos = m_pFile->GetLargePosition() - m_pRecord->qwBasePos ;
		if ( (UINT64) nPos + nBytes > m_pRecord->rechdr.nRecLength )
		{
			nBytes = (DWORD) (m_pRecord->rechdr.nRecLength - nPos) ;
		}
	}
	//
	if ( (signed long int) nBytes < 0 )
	{
		nBytes = 0 ;
	}
	return	m_pFile->Write( ptrBuffer, nBytes ) ;
}

// ���R�[�h�̒������擾����
//////////////////////////////////////////////////////////////////////////////
UINT64 EMCFile::GetLargeLength( void ) const
{
	ESLAssert( m_pRecord != NULL ) ;
	return	m_pRecord->rechdr.nRecLength ;
}

unsigned long int EMCFile::GetLength( void ) const
{
	ESLAssert( m_pRecord != NULL ) ;
	return	(unsigned long int) m_pRecord->rechdr.nRecLength ;
}

// �t�@�C���|�C���^���ړ�����
//////////////////////////////////////////////////////////////////////////////
UINT64 EMCFile::SeekLarge
	( INT64 nOffsetPos, ESLFileObject::SeekOrigin fSeekFrom )
{
	ESLAssert( m_pFile != NULL ) ;
	ESLAssert( m_pRecord != NULL ) ;
	UINT64	nPos = m_pFile->GetLargePosition() - m_pRecord->qwBasePos ;
	if ( m_pRecord->dwWriteFlag )
	{
		if ( nPos > m_pRecord->rechdr.nRecLength )
		{
			m_pRecord->rechdr.nRecLength = nPos ;
		}
	}
	//
	unsigned long int	nRecLength =
		(unsigned long int) m_pRecord->rechdr.nRecLength ;
	//
	switch ( fSeekFrom )
	{
	case	FromBegin:
	default:
		break ;
	case	FromCurrent:
		nOffsetPos += nPos ;
		break ;
	case	FromEnd:
		nOffsetPos += nRecLength ;
		break ;
	}
	//
	if ( m_pRecord->dwWriteFlag )
	{
		if ( (unsigned long int) nOffsetPos
					> m_pRecord->rechdr.nRecLength )
		{
			m_pRecord->rechdr.nRecLength = nOffsetPos ;
		}
	}
	else
	{
		if ( (unsigned long int) nOffsetPos
					> m_pRecord->rechdr.nRecLength )
		{
			nOffsetPos = (long int) m_pRecord->rechdr.nRecLength ;
		}
	}
	//
	return	m_pFile->SeekLarge
				( nOffsetPos + m_pRecord->qwBasePos, FromBegin )
											- m_pRecord->qwBasePos ;
}

unsigned long int EMCFile::Seek
	( long int nOffsetPos, ESLFileObject::SeekOrigin fSeekFrom )
{
	return	(unsigned long int) EMCFile::SeekLarge( nOffsetPos, fSeekFrom ) ;
}

// �t�@�C���|�C���^���擾����
//////////////////////////////////////////////////////////////////////////////
UINT64 EMCFile::GetLargePosition( void ) const
{
	ESLAssert( m_pFile != NULL ) ;
	ESLAssert( m_pRecord != NULL ) ;
	return	m_pFile->GetLargePosition() - m_pRecord->qwBasePos ;
}

unsigned long int EMCFile::GetPosition( void ) const
{
	return	(unsigned long int) EMCFile::GetLargePosition( ) ;
}

// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
//////////////////////////////////////////////////////////////////////////////
ESLError EMCFile::SetEndOfFile( void )
{
	ESLAssert( m_pFile != NULL ) ;
	ESLAssert( m_pRecord != NULL ) ;
	ESLAssert( GetAttribute() & modeWrite ) ;
	m_pRecord->rechdr.nRecLength =
		m_pFile->GetLargePosition() - m_pRecord->qwBasePos ;
	return	m_pFile->SetEndOfFile( ) ;
}


#if	!defined(COMPACT_NOA_DECODER)

/*****************************************************************************
                          �^�O����̓N���X
 *****************************************************************************/

//
// ETagObject �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERIFile::ETagObject::ETagObject( void )
{
}

//
// ETagObject ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERIFile::ETagObject::~ETagObject( void )
{
}

//
// ETagInfo �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERIFile::ETagInfo::ETagInfo( void )
{
}

//
// ETagInfo ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERIFile::ETagInfo::~ETagInfo( void )
{
}

//
// �^�O�����
//////////////////////////////////////////////////////////////////////////////
void ERIFile::ETagInfo::CreateTagInfo( const wchar_t * pwszDesc )
{
	//
	// ���݂̏����N���A
	//
	DeleteContents( ) ;
	//
	// ��������
	//
	if ( pwszDesc == NULL )
	{
		return ;
	}
	if ( pwszDesc[0] != L'#' )
	{
		ETagObject *	pTag = new ETagObject ;
		pTag->m_tag = L"comment" ;
		pTag->m_contents = pwszDesc ;
		m_lstTags.Add( pTag ) ;
		return ;
	}
	//
	while ( *pwszDesc != L'\0' )
	{
		//
		// �^�O�����擾
		//
		WORD	wch ;
		ETagObject *	pTag = new ETagObject ;
		int		i = 1 ;
		while ( pwszDesc[i] != L'\0' )
		{
			wch = pwszDesc[i] ;
			if ( wch > L' ' )
				break ;
			++ i ;
		}
		//
		int		iBegin = i ;
		while ( pwszDesc[i] != L'\0' )
		{
			wch = pwszDesc[i] ;
			if ( wch <= L' ' )
				break ;
			++ i ;
		}
		//
		pTag->m_tag = EWideString( pwszDesc + iBegin, i - iBegin ) ;
		//
		// ���̍s��
		//
		while ( pwszDesc[i] != L'\0' )
		{
			wch = pwszDesc[i ++] ;
			if ( (wch == L'\r') && (pwszDesc[i] == L'\n') )
			{
				++ i ;
				break ;
			}
		}
		pwszDesc += i ;
		//
		// �^�O�̓��e���擾
		//
		while ( *pwszDesc != L'\0' )
		{
			//
			// �P�s�擾
			//
			iBegin = 0 ;
			if ( pwszDesc[0] == L'#' )
			{
				if ( pwszDesc[1] != L'#' )
					break ;
				iBegin = 1 ;
			}
			i = iBegin ;
			while ( pwszDesc[i] != L'\0' )
			{
				wch = pwszDesc[i ++] ;
				if ( (wch == L'\r') && (pwszDesc[i] == L'\n') )
				{
					++ i ;
					break ;
				}
			}
			//
			// �P�s�ǉ�
			//
			pTag->m_contents += EWideString( pwszDesc + iBegin, i - iBegin ) ;
			//
			pwszDesc += i ;
		}
		//
		pTag->m_contents.TrimRight( ) ;
		//
		// �^�O��ǉ�
		//
		m_lstTags.Add( pTag ) ;
	}
}

// �^�O�����t�H�[�}�b�g
//////////////////////////////////////////////////////////////////////////////
void ERIFile::ETagInfo::FormatDescription( EWideString & wstrDesc )
{
	wstrDesc.GetBuffer(1)[0] = (wchar_t) 0xFEFF ;
	wstrDesc.ReleaseBuffer( 1 ) ;
	//
	for ( unsigned int i = 0; i < m_lstTags.GetSize(); i ++ )
	{
		ETagObject *	pTag = m_lstTags.GetAt( i ) ;
		ESLAssert( pTag != NULL ) ;
		//
		// �^�O��
		//
		wstrDesc += L'#' ;
		wstrDesc += pTag->m_tag ;
		wstrDesc += L"\r\n" ;
		//
		// ���e
		//
		int		iLine = 0 ;
		while ( iLine < (int) pTag->m_contents.GetLength() )
		{
			if ( pTag->m_contents.GetAt(iLine) == L'#' )
			{
				wstrDesc += L'#' ;
			}
			//
			int	nNextLine = pTag->m_contents.Find( L"\r\n", iLine ) ;
			if ( nNextLine < 0 )
				break ;
			//
			nNextLine += 2 ;
			wstrDesc += pTag->m_contents.Middle( iLine, nNextLine - iLine ) ;
			iLine = nNextLine ;
		}
		wstrDesc += pTag->m_contents.Middle( iLine ) ;
		if ( wstrDesc.Right(2) != L"\r\n" )
		{
			wstrDesc += L"\r\n" ;
		}
	}
}

// �^�O��ǉ�����
//////////////////////////////////////////////////////////////////////////////
void ERIFile::ETagInfo::AddTag
		( TagIndex tagIndex, const wchar_t * pwszContents )
{
	ETagObject *	pTag = new ETagObject ;
	pTag->m_tag = m_pwszTagName[tagIndex] ;
	pTag->m_contents = pwszContents ;
	m_lstTags.Add( pTag ) ;
}

//
// �^�O���̃N���A
//////////////////////////////////////////////////////////////////////////////
void ERIFile::ETagInfo::DeleteContents( void )
{
	m_lstTags.RemoveAll( ) ;
}

//
// �^�O���擾
//////////////////////////////////////////////////////////////////////////////
const wchar_t *
	ERIFile::ETagInfo::GetTagContents( const wchar_t * pwszTag ) const
{
	for ( unsigned int i = 0; i < m_lstTags.GetSize(); i ++ )
	{
		ETagObject *	pTag = m_lstTags.GetAt( i ) ;
		ESLAssert( pTag != NULL ) ;
		if ( pTag->m_tag == pwszTag )
		{
			return	pTag->m_contents ;
		}
	}
	return	NULL ;
}

const wchar_t * ERIFile::ETagInfo::GetTagContents( TagIndex tagIndex ) const
{
	return	GetTagContents( ERIFile::m_pwszTagName[tagIndex] ) ;
}

//
// �g���b�N�ԍ����擾
//////////////////////////////////////////////////////////////////////////////
int ERIFile::ETagInfo::GetTrackNumber( void ) const
{
	const wchar_t *	pwszTrack = GetTagContents( tagTrack ) ;
	if ( pwszTrack != NULL )
	{
		EStreamWideString	sws = pwszTrack ;
		return	sws.GetInteger( ) ;
	}
	return	0 ;
}

//
// �����[�X�N�������擾
//////////////////////////////////////////////////////////////////////////////
ESLError ERIFile::ETagInfo::
			GetReleaseDate( int & year, int & month, int & day ) const
{
	year = 0 ;
	month = 0 ;
	day = 0 ;
	//
	const wchar_t *	pwszDate = GetTagContents( tagReleaseDate ) ;
	if ( pwszDate == NULL )
		return	eslErrGeneral ;
	//
	int	radix ;
	EStreamWideString	sws = pwszDate ;
	radix = sws.GetNumberRadix( ) ;
	if ( radix < 0 )
		return	eslErrGeneral ;
	year = sws.GetInteger( radix ) ;
	if ( sws.HasToComeChar( L"/" ) != L'/' )
		return	eslErrGeneral ;
	//
	radix = sws.GetNumberRadix( ) ;
	if ( radix < 0 )
		return	eslErrGeneral ;
	month = sws.GetInteger( radix ) ;
	if ( sws.HasToComeChar( L"/" ) != L'/' )
		return	eslErrGeneral ;
	//
	radix = sws.GetNumberRadix( ) ;
	if ( radix < 0 )
		return	eslErrGeneral ;
	day = sws.GetInteger( radix ) ;

	return	eslErrSuccess ;
}

//
// ���[�v�|�C���g���擾
//////////////////////////////////////////////////////////////////////////////
int ERIFile::ETagInfo::GetRewindPoint( void ) const
{
	const wchar_t *	pwszRewindPoint = GetTagContents( tagRewindPoint ) ;
	if ( pwszRewindPoint != NULL )
	{
		EStreamWideString	sws = pwszRewindPoint ;
		return	sws.GetInteger( ) ;
	}
	return	0 ;
}

// �z�b�g�X�|�b�g���擾
//////////////////////////////////////////////////////////////////////////////
EGL_POINT ERIFile::ETagInfo::GetHotSpot( void ) const
{
	EGL_POINT	ptHotSpot ;
	ptHotSpot.x = 0 ;
	ptHotSpot.y = 0 ;
	//
	const wchar_t *	pwszHotSpot = GetTagContents( tagHotSpot ) ;
	if ( pwszHotSpot == NULL )
		return	ptHotSpot ;
	//
	int	radix ;
	EStreamWideString	sws = pwszHotSpot ;
	radix = sws.GetNumberRadix( ) ;
	if ( radix < 0 )
		return	ptHotSpot ;
	ptHotSpot.x = sws.GetInteger( radix ) ;
	if ( sws.HasToComeChar( L"," ) != L',' )
		return	ptHotSpot ;
	//
	radix = sws.GetNumberRadix( ) ;
	if ( radix < 0 )
		return	ptHotSpot ;
	ptHotSpot.y = sws.GetInteger( radix ) ;
	//
	return	ptHotSpot ;
}

// �𑜓x���擾
//////////////////////////////////////////////////////////////////////////////
long int ERIFile::ETagInfo::GetResolution( void ) const
{
	const wchar_t *	pwszResolution = GetTagContents( tagResolution ) ;
	if ( pwszResolution != NULL )
	{
		EStreamWideString	sws = pwszResolution ;
		return	sws.GetInteger( ) ;
	}
	return	0 ;
}


/*****************************************************************************
                        ERI �t�@�C���C���^�[�t�F�[�X
 *****************************************************************************/

//
// �^�O��񕶎���
//////////////////////////////////////////////////////////////////////////////
const wchar_t *	ERIFile::m_pwszTagName[ERIFile::tagMax] =
{
	L"title", L"vocal-player", L"composer", L"arranger",
	L"source", L"track", L"release-date", L"genre",
	L"rewind-point", L"hot-spot", L"resolution",
	L"comment", L"words"
} ;

// �N���X���
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ERIFile, EMCFile )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERIFile::ERIFile( void )
{
	m_dwSeqLength = 0 ;
	m_pSequence = NULL ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERIFile::~ERIFile( void )
{
	if ( m_pSequence != NULL )
	{
		::eslHeapFree( NULL, m_pSequence ) ;
	}
}

// ERI �t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError ERIFile::Open( ESLFileObject * pFile, ERIFile::OpenType type )
{
	//
	// �ǂݍ��݃t���O���N���A
	//////////////////////////////////////////////////////////////////////////
	m_fdwReadMask = 0 ;
	//
	m_dwSeqLength = 0 ;
	if ( m_pSequence != NULL )
	{
		::eslHeapFree( NULL, m_pSequence ) ;
		m_pSequence = NULL ;
	}
	//
	// EMC�w�b�_��ǂݍ���
	//////////////////////////////////////////////////////////////////////////
	ESLError	err ;
	err = EMCFile::Open( pFile, NULL ) ;
	if ( err )
		return	err ;
	if ( type == otOpenRoot )
		return	eslErrSuccess ;
	//
	// ���w�b�_���R�[�h��ǂݍ���
	//////////////////////////////////////////////////////////////////////////
	typedef	const UINT64 *	PCUINT64 ;
	err = DescendRecord( (PCUINT64)"Header  " ) ;
	if ( err )
		return	err ;
	//
	// �t�@�C���w�b�_��ǂݍ���
	err = DescendRecord( (PCUINT64)"FileHdr " ) ;
	if ( err )
		return	err ;
	if ( Read( &m_FileHeader,
			sizeof(ERI_FILE_HEADER) ) < sizeof(ERI_FILE_HEADER) )
	{
		return	ESLErrorMsg( "ERI �t�@�C���w�b�_��ǂݍ��߂܂���ł����B" ) ;
	}
	AscendRecord( ) ;
	//
	m_fdwReadMask |= rmFileHeader ;
	//
	// �o�[�W�������̃`�F�b�N
	if ( m_FileHeader.dwVersion > 0x00020100 )
	{
		return	ESLErrorMsg( "ERI �t�@�C���o�[�W�������s���ł��B" ) ;
	}
	//
	// �摜���w�b�_��ǂݍ���
	for ( ; ; )
	{
		if ( DescendRecord( ) )
			break ;
		//
		UINT64	ui64RecID = GetRecordID( ) ;
		if ( ui64RecID == *((PCUINT64)"PrevwInf") )
		{
			//
			// �v���r���[�摜���w�b�_
			if ( Read( &m_PreviewInfo,
					sizeof(ERI_INFO_HEADER) ) == sizeof(ERI_INFO_HEADER) )
			{
				m_fdwReadMask |= rmPreviewInfo ;
			}
		}
		else if ( ui64RecID == *((PCUINT64)"ImageInf") )
		{
			//
			// �摜���w�b�_
			if ( Read( &m_InfoHeader,
					sizeof(ERI_INFO_HEADER) ) == sizeof(ERI_INFO_HEADER) )
			{
				m_fdwReadMask |= rmImageInfo ;
			}
		}
		else if ( ui64RecID == *((PCUINT64)"SoundInf") )
		{
			//
			// �������w�b�_
			if ( Read( &m_MIOInfHdr,
					sizeof(MIO_INFO_HEADER) ) == sizeof(MIO_INFO_HEADER) )
			{
				m_fdwReadMask |= rmSoundInfo ;
			}
		}
		else if ( ui64RecID == *((PCUINT64)"Sequence") )
		{
			//
			// �V�[�P���X�e�[�u�����R�[�h
			DWORD	dwBytes ;
			m_dwSeqLength = GetLength() / sizeof(SEQUENCE_DELTA) ;
			dwBytes = m_dwSeqLength * sizeof(SEQUENCE_DELTA) ;
			m_pSequence =
				(SEQUENCE_DELTA*) ::eslHeapAllocate( NULL, dwBytes, 0 ) ;
			//
			if ( Read( m_pSequence, dwBytes ) == dwBytes )
			{
				m_fdwReadMask |= rmSequenceTable ;
			}
		}
		else
		{
			//
			// ���쌠���E�R�����g�Ȃ�
			int	nType = -1 ;
			if ( ui64RecID == *((PCUINT64)"cpyright") )
			{
				nType = 0 ;
				m_fdwReadMask |= rmCopyright ;
			}
			else if ( ui64RecID == *((PCUINT64)"descript") )
			{
				nType = 1 ;
				m_fdwReadMask |= rmDescription ;
			}
			if ( nType >= 0 )
			{
				int		nLength = GetLength( ) ;
				EStreamBuffer	bufDesc ;
				char *	pszBuf = (char*) bufDesc.PutBuffer( nLength + 2 ) ;
				Read( pszBuf, nLength ) ;
				//
				if ( (nLength >= 2) &&
					(pszBuf[0] == '\xff') && (pszBuf[1] == '\xfe') )
				{
					wchar_t *	pwszBuf = (wchar_t *) (pszBuf + 2) ;
					int		nWStrLen = (nLength - 2) / 2 ;
					pwszBuf[nWStrLen] = L'\0' ;
					if ( nType == 0 )
						m_wstrCopyright = pwszBuf ;
					else
						m_wstrDescription = pwszBuf ;
				}
				else
				{
					pszBuf[nLength] = '\0' ;
					if ( nType == 0 )
						m_wstrCopyright = pszBuf ;
					else
						m_wstrDescription = pszBuf ;
				}
			}
		}
		//
		// ���̃��R�[�h��
		AscendRecord( ) ;
	}
	//
	AscendRecord( ) ;
	//
	// ���k�I�v�V�����̃`�F�b�N
	if ( !(m_fdwReadMask & rmImageInfo) && !(m_fdwReadMask & rmSoundInfo) )
	{
		return	ESLErrorMsg( "�摜���w�b�_��������܂���ł����B" ) ;
	}
	if ( type == otReadHeader )
	{
		return	eslErrSuccess ;
	}
	//
	// �X�g���[�����R�[�h���J��
	//////////////////////////////////////////////////////////////////////////
	err = DescendRecord( (PCUINT64)"Stream  " ) ;
	if ( err )
		return	err ;
	if ( type == otOpenStream )
		return	eslErrSuccess ;
	//
	// �摜�f�[�^���R�[�h��{��
	for ( ; ; )
	{
		err = DescendRecord( ) ;
		if ( err )
		{
			return	err ;
		}
		UINT64	nRecID = GetRecordID( ) ;
		if ( nRecID == *((PCUINT64)"ImageFrm") )
		{
			break ;
		}
		if ( nRecID == *((PCUINT64)"Palette ") )
		{
			//
			// �p���b�g�e�[�u���ǂݍ���
			::eslFillMemory( m_PaletteTable, 0, sizeof(m_PaletteTable) ) ;
			Read( m_PaletteTable, sizeof(m_PaletteTable) ) ;
			//
			m_fdwReadMask |= rmPaletteTable ;
		}
		AscendRecord( ) ;
	}

	return	eslErrSuccess ;
}

// �V�[�P���X�e�[�u�����擾����
//////////////////////////////////////////////////////////////////////////////
const ERIFile::SEQUENCE_DELTA *
	 ERIFile::GetSequenceTable( DWORD * pdwLength ) const
{
	if ( pdwLength != NULL )
	{
		*pdwLength = m_dwSeqLength ;
	}
	return	m_pSequence ;
}

#endif	// !defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                    ERISA �A�[�J�C�u�`�� (NOA �`��)
 *****************************************************************************/

// ERISAArchive::FILE_INFO �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERISAArchive::FILE_INFO::FILE_INFO( void )
{
	nBytes = 0 ;
	dwAttribute = attrNormal ;
	dwEncodeType = etRaw ;
	nOffsetPos = 0 ;
	ftFileTime.nSecond = 0 ;
	ftFileTime.nMinute = 0 ;
	ftFileTime.nHour = 0 ;
	ftFileTime.nWeek = 0 ;
	ftFileTime.nDay = 0 ;
	ftFileTime.nMonth = 0 ;
	ftFileTime.nYear = 0 ;
	dwExtraInfoBytes = 0 ;
	ptrExtraInfo = NULL ;
	dwFileNameLen = 0 ;
	ptrFileName = NULL ;
}

ERISAArchive::FILE_INFO::FILE_INFO( const FILE_INFO & finfo )
{
	nBytes = finfo.nBytes ;
	dwAttribute = finfo.dwAttribute ;
	dwEncodeType = finfo.dwEncodeType ;
	nOffsetPos = finfo.nOffsetPos ;
	ftFileTime = finfo.ftFileTime ;
	dwExtraInfoBytes = 0 ;
	ptrExtraInfo = NULL ;
	dwFileNameLen = 0 ;
	ptrFileName = NULL ;
	//
	if ( finfo.dwExtraInfoBytes && finfo.ptrExtraInfo )
	{
		SetExtraInfo( finfo.ptrExtraInfo, finfo.dwExtraInfoBytes ) ;
	}
	if ( finfo.dwFileNameLen && finfo.ptrFileName )
	{
		dwFileNameLen = finfo.dwFileNameLen ;
		ptrFileName = (char*) ::eslHeapAllocate( NULL, dwFileNameLen, 0 ) ;
		for ( int i = 0; i < (int) dwFileNameLen; i ++ )
		{
			ptrFileName[i] = finfo.ptrFileName[i] ;
		}
	}
}

// ERISAArchive::FILE_INFO ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERISAArchive::FILE_INFO::~FILE_INFO( void )
{
	if ( ptrExtraInfo != NULL )
	{
		::eslHeapFree( NULL, ptrExtraInfo ) ;
	}
	if ( ptrFileName != NULL )
	{
		::eslHeapFree( NULL, ptrFileName ) ;
	}
}

// �G�N�X�g������ݒ�
//////////////////////////////////////////////////////////////////////////////
void ERISAArchive::FILE_INFO::SetExtraInfo( const void * pInfo, DWORD dwBytes )
{
	if ( ptrExtraInfo != NULL )
	{
		::eslHeapFree( NULL, ptrExtraInfo ) ;
	}
	dwExtraInfoBytes = dwBytes ;
	ptrExtraInfo = ::eslHeapAllocate( NULL, dwBytes, 0 ) ;
	//
	BYTE *	ptrDst = (BYTE*) ptrExtraInfo ;
	const BYTE *	ptrSrc = (const BYTE*) pInfo ;
	for ( DWORD i = 0; i < dwBytes; i ++ )
	{
		ptrDst[i] = ptrSrc[i] ;
	}
}

// �t�@�C������ݒ�
//////////////////////////////////////////////////////////////////////////////
void ERISAArchive::FILE_INFO::SetFileName( const char * pszFileName )
{
	if ( ptrFileName != NULL )
	{
		::eslHeapFree( NULL, ptrFileName ) ;
	}
	//
	int		nLen = 0 ;
	while ( pszFileName[nLen] != '\0' )
		++ nLen ;
	//
	dwFileNameLen = nLen + 1 ;
	ptrFileName = (char*) ::eslHeapAllocate( NULL, dwFileNameLen, 0 ) ;
	for ( int i = 0; i <= nLen; i ++ )
	{
		ptrFileName[i] = pszFileName[i] ;
	}
}

// �f�B���N�g���G���g���𕡐�
//////////////////////////////////////////////////////////////////////////////
void ERISAArchive::EDirectory::CopyDirectory( const EDirectory & dir )
{
	int		i, nCount ;
	nCount = dir.GetSize( ) ;
	for ( i = 0; i < nCount; i ++ )
	{
		FILE_INFO *	pInfo = dir.GetAt( i ) ;
		if ( pInfo != NULL )
		{
			Add( new FILE_INFO( *pInfo ) ) ;
		}
	}
}

// �w��t�@�C�����̃t�@�C�������擾
//////////////////////////////////////////////////////////////////////////////
ERISAArchive::FILE_INFO *
	ERISAArchive::EDirectory::GetFileInfo( const char * pszFileName )
{
	for ( unsigned int i = 0; i < GetSize(); i ++ )
	{
		FILE_INFO *	pInfo = GetAt( i ) ;
		if ( pInfo != NULL )
		{
			if ( !EString::CompareNoCase
				( pInfo->ptrFileName, pszFileName ) )
			{
				return	pInfo ;
			}
		}
	}
	return	NULL ;
}

// �t�@�C���G���g����ǉ�
//////////////////////////////////////////////////////////////////////////////
void ERISAArchive::EDirectory::AddFileEntry
	( const char * pszFileName, DWORD dwAttribute,
			DWORD dwEncodeType, FILE_TIME * pTime )
{
	FILE_INFO *	pInfo = new FILE_INFO ;
	pInfo->dwAttribute = dwAttribute ;
	pInfo->dwEncodeType = dwEncodeType ;
	if ( pTime != NULL )
	{
		pInfo->ftFileTime = *pTime ;
	}
	pInfo->SetFileName( pszFileName ) ;
	Add( pInfo ) ;
}

// �N���X���
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ERISAArchive, EMCFile )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERISAArchive::ERISAArchive( void )
{
	m_pCurDir = NULL ;
	m_pCurFile = NULL ;
	m_pBufFile = NULL ;
	m_pDecERISA = NULL ;
	m_pDecBSHF = NULL ;
#if	!defined(COMPACT_NOA_DECODER)
	m_pEncERISA = NULL ;
	m_pEncBSHF = NULL ;
#endif
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERISAArchive::~ERISAArchive( void )
{
	Close( ) ;
}

// �A�[�J�C�u�t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::Open
	( ESLFileObject * pfile, const EDirectory * pRootDir )
{
	//
	// ���݂̃t�@�C�������
	//
	Close( ) ;
	//
	// �t�@�C�����֘A�t���ĊJ��
	//
#if	!defined(COMPACT_NOA_DECODER)
	if ( pfile->GetAttribute() & modeCreateFlag )
	{
		//
		// �t�@�C�����J��
		//
		FILE_HEADER *	pfhHeader = NULL ;
		FILE_HEADER	fhHeader ;
		SetFileHeader( fhHeader, fidArchive ) ;
		ESLError	err = EMCFile::Open( pfile, &fhHeader ) ;
		if ( err )	
		{
			return	err ;
		}
		//
		// ���[�g�f�B���N�g����ݒ肷��
		//
		ESLAssert( pRootDir != NULL ) ;
		m_pCurDir = new EDirectory ;
		m_pCurDir->m_dwWriteFlag = 1 ;
		m_pCurDir->CopyDirectory( *pRootDir ) ;
		if ( WriteinDirectoryEntry( ) )
		{
			ESLTrace( "���[�g�f�B���N�g���̏����o���Ɏ��s���܂����B\n" ) ;
			return	eslErrGeneral ;
		}
	}
	else
#endif
	{
		//
		// �t�@�C�����J��
		//
		ESLError	err = EMCFile::Open( pfile, NULL ) ;
		if ( err )	
		{
			return	err ;
		}
		//
		// ���[�g�f�B���N�g����ǂݍ���
		//
		m_pCurDir = new EDirectory ;
		if ( LoadDirectoryEntry( ) )
		{
			ESLTrace( "���[�g�f�B���N�g����ǂݍ��߂܂���ł����B\n" ) ;
			return	eslErrGeneral ;
		}
	}
	//
	return	eslErrSuccess ;
}

// �A�[�J�C�u�t�@�C�������
//////////////////////////////////////////////////////////////////////////////
void ERISAArchive::Close( void )
{
	if ( m_pCurFile != NULL )
	{
		if ( AscendFile( ) )
		{
			ESLTrace( "AscendFile �Ɏ��s���܂����B\n" ) ;
		}
	}
	if ( m_pBufFile != NULL )
	{
		delete	m_pBufFile ;
		m_pBufFile = NULL ;
	}
	//
	while ( m_pCurDir != NULL )
	{
#if	!defined(COMPACT_NOA_DECODER)
		if ( m_pCurDir->m_dwWriteFlag )
		{
			SeekLarge( 0, FromBegin ) ;
			if ( WriteinDirectoryEntry( ) )
			{
				break ;
			}
		}
#endif
		EDirectory *	pDir = m_pCurDir ;
		m_pCurDir = m_pCurDir->m_pParent ;
		delete	pDir ;
		//
		if ( m_pCurDir == NULL )
		{
			break ;
		}
		if ( AscendRecord( ) )
		{
			break ;
		}
	}
	//
	EMCFile::Close( ) ;
}

// ���R�[�h���J��
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::DescendRecord( const UINT64 * pRecID )
{
	return	EMCFile::DescendRecord( pRecID ) ;
}

// ���R�[�h�����
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::AscendRecord( void )
{
	return	EMCFile::AscendRecord( ) ;
}

// �J�����g�f�B���N�g���̃t�@�C���G���g�����擾����
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::GetFileEntries( EDirectory & dirFiles )
{
	if ( m_pCurDir == NULL )
	{
		return	eslErrGeneral ;
	}
	//
	dirFiles.CopyDirectory( *m_pCurDir ) ;
	//
	return	eslErrSuccess ;
}

// �w��̃f�B���N�g�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::DescendDirectory
	( const char * pszDirName, const EDirectory * pDir )
{
	//
	// ���݊J���Ă���t�@�C�������
	//
	if ( m_pCurFile != NULL )
	{
		AscendFile( ) ;
	}
	//
	// �t�@�C���G���g������������
	//
	ESLAssert( m_pCurDir != NULL ) ;
	FILE_INFO *	pDirInfo = m_pCurDir->GetFileInfo( pszDirName ) ;
	if ( (pDirInfo == NULL) ||
		!(pDirInfo->dwAttribute & attrDirectory) )
	{
		ESLTrace
			( "\"%s\" �͌��݂̃f�B���N�g��"
				"�G���g���ɂ͑��݂��܂���B\n", pszDirName ) ;
		return	eslErrGeneral ;
	}
	//
#if	!defined(COMPACT_NOA_DECODER)
	if ( pDir != NULL )
	{
		//
		// �V�K�ɃT�u�f�B���N�g�����쐬����
		//
		ESLAssert( GetAttribute() & modeWrite ) ;
		//
		pDirInfo->nOffsetPos = GetLargePosition( ) ;
		//
		if ( DescendRecord( (const UINT64 *) "filedata" ) )
		{
			return	eslErrGeneral ;
		}
		//
		EDirectory *	pCurDir = new EDirectory ;
		pCurDir->m_pParent = m_pCurDir ;
		pCurDir->m_dwWriteFlag = 1 ;
		pCurDir->CopyDirectory( *pDir ) ;
		m_pCurDir = pCurDir ;
		//
		if ( WriteinDirectoryEntry( ) )
		{
			return	eslErrGeneral ;
		}
	}
	else
#endif
	{
		//
		// �w��f�B���N�g����ǂݍ��ׂ݂̈ɊJ��
		//
		ESLAssert( GetAttribute() & modeRead ) ;
		//
		SeekLarge( pDirInfo->nOffsetPos, FromBegin ) ;
		if ( DescendRecord( ) )
		{
			return	eslErrGeneral ;
		}
		if ( LoadDirectoryEntry( ) )
		{
			return	eslErrGeneral ;
		}
	}
	//
	return	eslErrSuccess ;
}

// �f�B���N�g�������
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::AscendDirectory( void )
{
	if ( m_pCurFile != NULL )
	{
		if ( AscendFile( ) )
		{
			ESLTrace( "AscendFile �Ɏ��s���܂����B\n" ) ;
		}
	}
	if ( m_pCurDir != NULL )
	{
#if	!defined(COMPACT_NOA_DECODER)
		if ( m_pCurDir->m_dwWriteFlag )
		{
			SeekLarge( 0, FromBegin ) ;
			if ( WriteinDirectoryEntry( ) )
			{
				return	eslErrGeneral ;
			}
		}
#endif
		EDirectory *	pDir = m_pCurDir ;
		m_pCurDir = m_pCurDir->m_pParent ;
		delete	pDir ;
		//
		return	AscendRecord( ) ;
	}
	//
	return	eslErrSuccess ;
}

// �t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::DescendFile
	( const char * pszFileName,
		const char * pszPassword, ERISAArchive::OpenType otType )
{
	//
	// ���݊J���Ă���t�@�C�������
	//
	if ( m_pCurFile != NULL )
	{
		AscendFile( ) ;
	}
	//
	// �t�@�C���G���g������������
	//
	ESLAssert( m_pCurDir != NULL ) ;
	FILE_INFO *	pfinfo = m_pCurDir->GetFileInfo( pszFileName ) ;
	if ( (pfinfo == NULL) ||
		(pfinfo->dwAttribute & attrDirectory) )
	{
		ESLTrace
			( "\"%s\" �͌��݂̃f�B���N�g��"
				"�G���g���ɂ͑��݂��܂���B\n", pszFileName ) ;
		return	eslErrGeneral ;
	}
	//
#if	!defined(COMPACT_NOA_DECODER)
	if ( m_pCurDir->m_dwWriteFlag )
	{
		//
		// �w��̃t�@�C���������o�����[�h�ŊJ��
		//
		ESLAssert( GetAttribute() & modeWrite ) ;
		//
		pfinfo->nOffsetPos = GetLargePosition( ) ;
		//
		if ( DescendRecord( (const UINT64 *) "filedata" ) )
		{
			return	eslErrGeneral ;
		}
		//
		// �������ݐݒ�
		//
		m_qwWrittenBytes = 0 ;
		m_bufCRC[0] = 0 ;
		m_bufCRC[1] = 0 ;
		m_bufCRC[2] = 0 ;
		m_bufCRC[3] = 0 ;
		m_iCRC = 0 ;
		//
		if ( pfinfo->dwEncodeType == etRaw )
		{
		}
		else if ( pfinfo->dwEncodeType == etERISACode )
		{
			m_pEncERISA = new ERISAEncodeContext( 0x4000 ) ;
			m_pEncERISA->AttachOutputFile( m_pFile ) ;
			m_pEncERISA->PrepareToEncodeERISANCode( ) ;
		}
		else if ( pfinfo->dwEncodeType == etBSHFCrypt )
		{
			m_pEncBSHF = new ERISAEncodeContext( 0x4000 ) ;
			m_pEncBSHF->AttachOutputFile( m_pFile ) ;
			m_pEncBSHF->PrepareToEncodeBSHFCode( pszPassword ) ;
		}
		else if ( pfinfo->dwEncodeType == etERISACrypt )
		{
			m_pEncBSHF = new ERISAEncodeContext( 0x4000 ) ;
			m_pEncBSHF->AttachOutputFile( m_pFile ) ;
			m_pEncBSHF->PrepareToEncodeBSHFCode( pszPassword ) ;
			m_pEncERISA = new ERISAEncodeContext( 0x4000 ) ;
			m_pEncERISA->AttachOutputContext( m_pEncBSHF ) ;
			m_pEncERISA->PrepareToEncodeERISANCode( ) ;
		}
		else
		{
			ESLTrace( "�s���ȃG���R�[�h�^�C�v�ł��B\n" ) ;
			return	eslErrGeneral ;
		}
	}
	else
#endif
	{
		//
		// �w��̃t�@�C����ǂݍ��݃��[�h�ŊJ��
		//
		ESLAssert( GetAttribute() & modeRead ) ;
		//
		SeekLarge( pfinfo->nOffsetPos, FromBegin ) ;
		if ( DescendRecord( ) )
		{
			return	eslErrGeneral ;
		}
		//
		if ( pfinfo->dwEncodeType != etRaw )
		{
			//
			// ��������
			//
			ERISADecodeContext *	pBSHF = NULL ;
			ERISADecodeContext *	pERISA = NULL ;
			//
			if ( pfinfo->dwEncodeType == etERISACode )
			{
				pERISA = new ERISADecodeContext( 0x10000 ) ;
				pERISA->AttachInputFile( m_pFile ) ;
				pERISA->PrepareToDecodeERISANCode( ) ;
			}
			else if ( pfinfo->dwEncodeType == etBSHFCrypt )
			{
				pBSHF = new ERISADecodeContext( 0x10000 ) ;
				pBSHF->AttachInputFile( m_pFile ) ;
				pBSHF->PrepareToDecodeBSHFCode( pszPassword ) ;
			}
			else if ( pfinfo->dwEncodeType == etERISACrypt )
			{
				pBSHF = new ERISADecodeContext( 0x10000 ) ;
				pBSHF->AttachInputFile( m_pFile ) ;
				pBSHF->PrepareToDecodeBSHFCode( pszPassword ) ;
				pERISA = new ERISADecodeContext( 0x10000 ) ;
				pERISA->AttachInputContext( pBSHF ) ;
				pERISA->PrepareToDecodeERISANCode( ) ;
			}
			else
			{
				ESLTrace( "�s���ȃG���R�[�h�^�C�v�ł��B\n" ) ;
				return	eslErrGeneral ;
			}
			//
			if ( otType == otNormal )
			{
				//
				// �����J�n
				//
				EStreamBuffer	buf ;
				unsigned int	nDecodedBytes = 0 ;
				unsigned int	nTotalBytes = (unsigned int) pfinfo->nBytes ;
				const unsigned int	nBufSize = 0x4000 ;
				BYTE *	ptrBuf = (BYTE*) buf.PutBuffer( nBufSize ) ;
				BYTE			bufCRC[4] = { 0, 0, 0, 0 } ;
				int				iCRC = 0 ;
				//
				EMemoryFile *	pBufFile = new EMemoryFile ;
				pBufFile->Create( nTotalBytes ) ;
				//
				while ( nDecodedBytes < nTotalBytes )
				{
					//
					// �����T�C�Y���Z�o
					//
					unsigned int	nDecodeBytes = nTotalBytes - nDecodedBytes ;
					if ( nDecodeBytes > nBufSize )
					{
						nDecodeBytes = nBufSize ;
					}
					//
					// ����
					//
					unsigned int	nResult = 0 ;
					if ( pERISA != NULL )
					{
						nResult = pERISA->DecodeERISANCodeBytes
									( (SBYTE*) ptrBuf, nDecodeBytes ) ;
					}
					else
					{
						nResult = pBSHF->DecodeBSHFCodeBytes
									( (SBYTE*) ptrBuf, nDecodeBytes ) ;
					}
					if ( nResult < nDecodeBytes )
					{
						ESLTrace( "�����Ɏ��s���܂����B\n" ) ;
						delete	pBSHF ;
						delete	pERISA ;
						delete	pBufFile ;
						return	eslErrGeneral ;
					}
					//
					// �`�F�b�N�T��
					//
					for ( unsigned int i = 0; i < nResult; i ++ )
					{
						bufCRC[iCRC] ^= ptrBuf[i] ;
						iCRC = (iCRC + 1) & 0x03 ;
					}
					//
					// �f�[�^��]��
					//
					pBufFile->Write( ptrBuf, nResult ) ;
					nDecodedBytes += nResult ;
					//
					ESLError	errExit =
						OnProcessFileData( nDecodedBytes, nTotalBytes ) ;
					if ( errExit )
					{
						delete	pBSHF ;
						delete	pERISA ;
						delete	pBufFile ;
						return	errExit ;
					}
				}
				//
				// �`�F�b�N�T��
				//
				delete	pBSHF ;
				delete	pERISA ;
				//
				BYTE	orgCRC[4] ;
				EMCFile::SeekLarge( -4, FromEnd ) ;
				EMCFile::Read( orgCRC, sizeof(orgCRC) ) ;
				//
				if ( (orgCRC[0] != bufCRC[0])
					|| (orgCRC[1] != bufCRC[1])
					|| (orgCRC[2] != bufCRC[2])
					|| (orgCRC[3] != bufCRC[3]) )
				{
					ESLTrace( "�`�F�b�N�T������v���܂���B\n" ) ;
					delete	pBufFile ;
					return	eslErrGeneral ;
				}
				//
				// �����I��
				//
				pBufFile->SeekLarge( 0, FromBegin ) ;
				m_pBufFile = pBufFile ;
			}
			else
			{
				//
				// �X�g���[�����[�h
				//
				m_qwWrittenBytes = 0 ;
				m_pDecBSHF = pBSHF ;
				m_pDecERISA = pERISA ;
				m_bufCRC[0] = 0 ;
				m_bufCRC[1] = 0 ;
				m_bufCRC[2] = 0 ;
				m_bufCRC[3] = 0 ;
				m_iCRC = 0 ;
			}
		}
	}
	//
	m_pCurFile = pfinfo ;
	return	eslErrSuccess ;
}

// �t�@�C�������
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::AscendFile( void )
{
	ESLError	errResult = eslErrSuccess ;
	//
	if ( m_pCurFile != NULL )
	{
		ESLAssert( m_pRecord != NULL ) ;
#if	!defined(COMPACT_NOA_DECODER)
		if ( m_pRecord->dwWriteFlag )
		{
			//
			// �����o������
			//
			if ( m_pCurFile->dwEncodeType != etRaw )
			{
				if ( m_pEncERISA != NULL )
				{
					m_pEncERISA->FinishERISACode( ) ;
					delete	m_pEncERISA ;
					m_pEncERISA = NULL ;
				}
				if ( m_pEncBSHF != NULL )
				{
					m_pEncBSHF->FinishBSHFCode( ) ;
					delete	m_pEncBSHF ;
					m_pEncBSHF = NULL ;
				}
				m_pFile->Write( m_bufCRC, sizeof(m_bufCRC) ) ;
				//
				m_pCurFile->nBytes = m_qwWrittenBytes ;
			}
			else
			{
				m_pCurFile->nBytes = GetLargeLength( ) ;
			}
		}
		else
#endif
		{
			//
			// �ǂݍ��݊���
			//
			if ( (m_pDecERISA != NULL) || (m_pDecBSHF != NULL) )
			{
				delete	m_pDecERISA ;
				delete	m_pDecBSHF ;
				m_pDecERISA = NULL ;
				m_pDecBSHF = NULL ;
				//
				BYTE	orgCRC[4] ;
				EMCFile::SeekLarge( -4, FromEnd ) ;
				EMCFile::Read( orgCRC, sizeof(orgCRC) ) ;
				//
				if ( (orgCRC[0] != m_bufCRC[0])
					|| (orgCRC[1] != m_bufCRC[1])
					|| (orgCRC[2] != m_bufCRC[2])
					|| (orgCRC[3] != m_bufCRC[3]) )
				{
					ESLTrace( "�`�F�b�N�T������v���܂���B\n" ) ;
					errResult = eslErrGeneral ;
				}
			}
		}
		//
		m_pCurFile = NULL ;
		AscendRecord( ) ;
	}
	//
	if ( m_pBufFile != NULL )
	{
		delete	m_pBufFile ;
		m_pBufFile = NULL ;
	}
	//
	return	errResult ;
}

// �f�B���N�g�����J���i�p�X�w��j
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::OpenDirectory( const char * pszDirPath )
{
	//
	// ���[�g�f�B���N�g���ֈڍs
	//
	if ( m_pCurFile != NULL )
	{
		AscendFile( ) ;
	}
	while ( (m_pCurDir != NULL)
			&& (m_pCurDir->m_pParent != NULL)
			&& (m_pCurDir->m_pParent->m_pParent != NULL) )
	{
		AscendDirectory( ) ;
	}
	//
	// �f�B���N�g���ڍs
	//
	EString	strDirPath = pszDirPath ;
	int		iDir = 0 ;
	for ( ; ; )
	{
		EString	strDirName ;
		int		iNext = strDirPath.Find( '\\', iDir ) ;
		if ( iNext < 0 )
		{
			strDirName = strDirPath.Middle( iDir ) ;
		}
		else
		{
			strDirName = strDirPath.Middle( iDir, iNext - iDir ) ;
		}
		if ( !strDirName.IsEmpty() )
		{
			if ( DescendDirectory( strDirName ) )
			{
				return	eslErrGeneral ;
			}
		}
		if ( iNext < 0 )
		{
			break ;
		}
		iDir = iNext + 1 ;
	}
	//
	return	eslErrSuccess ;
}

// �t�@�C�����J���i�p�X�w��j
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::OpenFile
	( const char * pszFilePath,
		const char * pszPassword, ERISAArchive::OpenType otType )
{
	EString	strFilePath = pszFilePath ;
	if ( OpenDirectory( strFilePath.GetFileDirectoryPart() ) )
	{
		return	eslErrGeneral ;
	}
	return	DescendFile
				( strFilePath.GetFileNamePart(), pszPassword, otType ) ;
}

// ���݂̈ʒu����f�B���N�g���G���g�����R�[�h�������o��
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::WriteinDirectoryEntry( void )
{
	ESLAssert( m_pCurDir != NULL ) ;
	if ( DescendRecord( (const UINT64 *) "DirEntry" ) )
	{
		return	eslErrGeneral ;
	}
	//
	DWORD	dwEntryCount = m_pCurDir->GetSize( ) ;
	Write( &dwEntryCount, sizeof(DWORD) ) ;
	//
	for ( DWORD i = 0; i < dwEntryCount; i ++ )
	{
		FILE_INFO *	pfinfo = m_pCurDir->GetAt( i ) ;
		ESLAssert( pfinfo != NULL ) ;
		Write( &(pfinfo->nBytes), sizeof(pfinfo->nBytes) ) ;
		Write( &(pfinfo->dwAttribute), sizeof(pfinfo->dwAttribute) ) ;
		Write( &(pfinfo->dwEncodeType), sizeof(pfinfo->dwEncodeType) ) ;
		Write( &(pfinfo->nOffsetPos), sizeof(pfinfo->nOffsetPos) ) ;
		Write( &(pfinfo->ftFileTime), sizeof(pfinfo->ftFileTime) ) ;
		//
		Write( &(pfinfo->dwExtraInfoBytes), sizeof(pfinfo->dwExtraInfoBytes) ) ;
		if ( pfinfo->dwExtraInfoBytes > 0 )
		{
			Write( pfinfo->ptrExtraInfo, pfinfo->dwExtraInfoBytes ) ;
		}
		Write( &(pfinfo->dwFileNameLen), sizeof(pfinfo->dwFileNameLen) ) ;
		if ( pfinfo->dwFileNameLen > 0 )
		{
			Write( pfinfo->ptrFileName, pfinfo->dwFileNameLen ) ;
		}
	}
	//
	if ( AscendRecord( ) )
	{
		return	eslErrGeneral ;
	}
	//
	return	eslErrSuccess ;
}

// ���݂̈ʒu����f�B���N�g���G���g�����R�[�h��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::LoadDirectoryEntry( void )
{
	ESLAssert( m_pCurDir != NULL ) ;
	if ( DescendRecord( ) )
	{
		return	eslErrGeneral ;
	}
	if ( m_pRecord->rechdr.nRecordID != *((UINT64*)"DirEntry") )
	{
		AscendRecord( ) ;
		return	eslErrGeneral ;
	}
	//
	EDirectory *	pParentDir = m_pCurDir ;
	m_pCurDir = new EDirectory ;
	m_pCurDir->m_pParent = pParentDir ;
	//
	DWORD	dwEntryCount ;
	if ( Read( &dwEntryCount, sizeof(DWORD) ) < sizeof(DWORD) )
	{
		AscendRecord( ) ;
		return	eslErrGeneral ;
	}
	//
	for ( DWORD i = 0; i < dwEntryCount; i ++ )
	{
		FILE_INFO *	pfinfo = new FILE_INFO ;
		Read( &(pfinfo->nBytes), sizeof(pfinfo->nBytes) ) ;
		Read( &(pfinfo->dwAttribute), sizeof(pfinfo->dwAttribute) ) ;
		Read( &(pfinfo->dwEncodeType), sizeof(pfinfo->dwEncodeType) ) ;
		Read( &(pfinfo->nOffsetPos), sizeof(pfinfo->nOffsetPos) ) ;
		Read( &(pfinfo->ftFileTime), sizeof(pfinfo->ftFileTime) ) ;
		//
		DWORD	dwExtraInfoBytes ;
		Read( &dwExtraInfoBytes, sizeof(dwExtraInfoBytes) ) ;
		if ( dwExtraInfoBytes > 0 )
		{
			EStreamBuffer	buf ;
			void *	ptrBuf = buf.PutBuffer( dwExtraInfoBytes ) ;
			Read( ptrBuf, dwExtraInfoBytes ) ;
			pfinfo->SetExtraInfo( ptrBuf, dwExtraInfoBytes ) ;
		}
		DWORD	dwFileNameLen ;
		Read( &dwFileNameLen, sizeof(dwFileNameLen) ) ;
		if ( dwFileNameLen > 0 )
		{
			EString	strFileName ;
			char *	pszFileName = strFileName.GetBuffer( dwFileNameLen ) ;
			Read( pszFileName, dwFileNameLen ) ;
			strFileName.ReleaseBuffer( dwFileNameLen ) ;
			pfinfo->SetFileName( strFileName ) ;
		}
		//
		m_pCurDir->Add( pfinfo ) ;
	}
	//
	if ( AscendRecord( ) )
	{
		return	eslErrGeneral ;
	}
	//
	return	eslErrSuccess ;
}

// �W�J�E�����̐i�s�󋵂�ʒm����
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchive::OnProcessFileData( DWORD dwCurrent, DWORD dwTotal )
{
	return	eslErrSuccess ;
}

// �t�@�C���I�u�W�F�N�g�𕡐�����
//////////////////////////////////////////////////////////////////////////////
ESLFileObject * ERISAArchive::Duplicate( void ) const
{
	if ( m_pBufFile != NULL )
	{
		return	m_pBufFile->Duplicate( ) ;
	}
	return	EMCFile::Duplicate( ) ;
}

// �A�[�J�C�u����f�[�^��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
unsigned long int ERISAArchive::Read
	( void * ptrBuffer, unsigned long int nBytes )
{
	if ( m_pBufFile != NULL )
	{
		//
		// �W�J���ꂽ��������̃f�[�^
		//
		return	m_pBufFile->Read( ptrBuffer, nBytes ) ;
	}
	else if ( (m_pDecERISA != NULL) || (m_pDecBSHF != NULL) )
	{
		//
		// �X�g���[������
		//
		unsigned long int	nReadBytes = nBytes ;
		SBYTE *		ptrDst = (SBYTE *) ptrBuffer ;
		if ( m_qwWrittenBytes + nReadBytes >= m_pCurFile->nBytes )
		{
			nReadBytes =
				(unsigned long int) (m_pCurFile->nBytes - m_qwWrittenBytes) ;
		}
		//
		if ( m_pDecERISA != NULL )
		{
			nReadBytes =
				m_pDecERISA->DecodeERISANCodeBytes( ptrDst, nReadBytes ) ;
		}
		else
		{
			nReadBytes =
				m_pDecBSHF->DecodeBSHFCodeBytes( ptrDst, nReadBytes ) ;
		}
		//
		// �`�F�b�N�T��
		//
		for ( unsigned long int i = 0; i < nReadBytes; i ++ )
		{
			m_bufCRC[m_iCRC] ^= ptrDst[i] ;
			m_iCRC = (m_iCRC + 1) & 0x03 ;
		}
		m_qwWrittenBytes += nReadBytes ;
		//
		return	nReadBytes ;
	}
	else
	{
		//
		// ������
		//
		return	EMCFile::Read( ptrBuffer, nBytes ) ;
	}
}

// �A�[�J�C�u�Ƀf�[�^�������o��
//////////////////////////////////////////////////////////////////////////////
unsigned long int ERISAArchive::Write
	( const void * ptrBuffer, unsigned long int nBytes )
{
	unsigned long int	nWrittenBytes = 0 ;
	//
#if	!defined(COMPACT_NOA_DECODER)
	if ( (m_pEncERISA != NULL) || (m_pEncBSHF != NULL) )
	{
		//
		// ������
		//
		const SBYTE *		ptrSrc = (const SBYTE *) ptrBuffer ;
		//
		if ( m_pEncERISA != NULL )
		{
			nWrittenBytes =
				m_pEncERISA->EncodeERISANCodeBytes( ptrSrc, nBytes ) ;
		}
		else
		{
			nWrittenBytes =
				m_pEncBSHF->EncodeBSHFCodeBytes( ptrSrc, nBytes ) ;
		}
		//
		// �`�F�b�N�T��
		//
		for ( unsigned long int i = 0; i < nWrittenBytes; i ++ )
		{
			m_bufCRC[m_iCRC] ^= ptrSrc[i] ;
			m_iCRC = (m_iCRC + 1) & 0x03 ;
		}
		//
		m_qwWrittenBytes += nWrittenBytes ;
	}
	else
#endif
	{
		//
		// �ʏ�̏����o��
		//
		nWrittenBytes = EMCFile::Write( ptrBuffer, nBytes ) ;
	}
	//
	return	nWrittenBytes ;
}

// �t�@�C���̒������擾����
//////////////////////////////////////////////////////////////////////////////
UINT64 ERISAArchive::GetLargeLength( void ) const
{
	ESLAssert( m_pRecord != NULL ) ;
	if ( m_pBufFile != NULL )
	{
		return	m_pBufFile->GetLargeLength( ) ;
	}
	if ( m_pRecord->dwWriteFlag )
	{
#if	!defined(COMPACT_NOA_DECODER)
		if ( (m_pEncERISA != NULL) || (m_pEncBSHF != NULL) )
		{
			return	m_qwWrittenBytes ;
		}
#endif
		UINT64	nPos = m_pFile->GetLargePosition() - m_pRecord->qwBasePos ;
		if ( nPos > m_pRecord->rechdr.nRecLength )
		{
			m_pRecord->rechdr.nRecLength = nPos ;
		}
	}
	else if ( (m_pCurFile != NULL)
			|| (m_pDecERISA != NULL) || (m_pDecBSHF != NULL) )
	{
		return	m_pCurFile->nBytes ;
	}
	return	m_pRecord->rechdr.nRecLength ;
}

unsigned long int ERISAArchive::GetLength( void ) const
{
	return	(unsigned long int) ERISAArchive::GetLargeLength( ) ;
}

// �t�@�C���|�C���^���ړ�����
//////////////////////////////////////////////////////////////////////////////
UINT64 ERISAArchive::SeekLarge
	( INT64 nOffsetPos, SeekOrigin fSeekFrom )
{
	ESLAssert( m_pRecord != NULL ) ;
	INT64	nPos = m_pFile->GetLargePosition() - m_pRecord->qwBasePos ;
	if ( m_pBufFile != NULL )
	{
		return	m_pBufFile->SeekLarge( nOffsetPos, fSeekFrom ) ;
	}
#if	!defined(COMPACT_NOA_DECODER)
	ESLAssert( !m_pEncERISA && !m_pEncBSHF && !m_pDecERISA && !m_pDecBSHF ) ;
#else
	ESLAssert( !m_pDecERISA && !m_pDecBSHF ) ;
#endif
	//
	return	EMCFile::SeekLarge( nOffsetPos, fSeekFrom ) ;
}

unsigned long int ERISAArchive::Seek
	( long int nOffsetPos, SeekOrigin fSeekFrom )
{
	return	(unsigned long int)
		ERISAArchive::SeekLarge( nOffsetPos, fSeekFrom ) ;
}

// �t�@�C���|�C���^���擾����
//////////////////////////////////////////////////////////////////////////////
UINT64 ERISAArchive::GetLargePosition( void ) const
{
	ESLAssert( m_pRecord != NULL ) ;
	if ( m_pBufFile != NULL )
	{
		return	m_pBufFile->GetLargePosition( ) ;
	}
#if	!defined(COMPACT_NOA_DECODER)
	if ( m_pRecord->dwWriteFlag )
	{
		if ( (m_pEncERISA != NULL) || (m_pEncBSHF != NULL) )
		{
			return	m_qwWrittenBytes ;
		}
	}
#endif
	if ( (m_pDecERISA != NULL) || (m_pDecBSHF != NULL) )
	{
		return	m_qwWrittenBytes ;
	}
	return	EMCFile::GetLargePosition( ) ;
}

unsigned long int ERISAArchive::GetPosition( void ) const
{
	return	(unsigned long int) ERISAArchive::GetLargePosition( ) ;
}


#if	!defined(COMPACT_NOA_DECODER)

/*****************************************************************************
                    ERISA �A�[�J�C�u���t�@�C�����X�g
 *****************************************************************************/

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERISAArchiveList::ERISAArchiveList( void )
{
	m_pCurDir = &m_dirRoot ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERISAArchiveList::~ERISAArchiveList( void )
{
	DeleteContents( ) ;
}

// �t�@�C�����X�g��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchiveList::LoadFileList( ESLFileObject & file )
{
	EDescription	desc ;
	EStreamBuffer	buf ;
	ESLError	err ;
	//
	// �t�@�C����ǂݍ���
	//
	void *			ptrBuf = buf.PutBuffer( file.GetLength() ) ;
	buf.Flush( file.Read( ptrBuf, file.GetLength() ) ) ;
	err = desc.ReadDescription( buf ) ;
	if ( err )
		return	err ;
	//
	// ������ǂݍ���
	//
	DeleteContents( ) ;
	//
	return	ReadListOnDirectory( m_dirRoot, desc ) ;
}

// �t�@�C�����X�g�������o��
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchiveList::SaveFileList( ESLFileObject & file )
{
	EDescription	desc ;
	ESLError	err ;
	err = WriteListOnDirectory( desc, m_dirRoot ) ;
	if ( err )
		return	err ;
	//
	return	desc.WriteDescription( file ) ;
}

// ���e��S�č폜
//////////////////////////////////////////////////////////////////////////////
void ERISAArchiveList::DeleteContents( void )
{
	m_pCurDir = &m_dirRoot ;
	m_dirRoot.RemoveAll( ) ;
}

// �w��̃f�B���N�g����ǂݍ���
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchiveList::ReadListOnDirectory
	( ERISAArchiveList::EDirectory & flist, EDescription & desc )
{
	for ( int i = 0; i < desc.GetContentTagCount(); i ++ )
	{
		//
		// �^�O���擾
		//
		EDescription *	pDesc = desc.GetContentTagAt( i ) ;
		if ( pDesc == NULL )
			continue ;
		//
		if ( !pDesc->Tag().CompareNoCase( L"file" ) )
		{
			//
			// �t�@�C���G���g���ǉ�
			//
			EFileEntry *	pfile = new EFileEntry ;
			pfile->m_strFileName = pDesc->GetAttrString( L"name", L"" ) ;
			pfile->m_dwAttribute = ERISAArchive::attrNormal ;
			pfile->m_dwEncodeType = ERISAArchive::etRaw ;
			//
			// �G���R�[�f�B���O�w��
			//
			EWideString	wstrEncoding =
				pDesc->GetAttrString( L"encoding", L"" ) ;
			if ( !wstrEncoding.IsEmpty() )
			{
				static const wchar_t *	pwszEncodingType[4] =
				{
					L"raw", L"erisa_code", L"bshf_crypt", L"erisa_crypt"
				} ;
				static const ERISAArchive::EncodeType	etEncodingType[4] =
				{
					ERISAArchive::etRaw, ERISAArchive::etERISACode,
					ERISAArchive::etBSHFCrypt, ERISAArchive::etERISACrypt
				} ;
				//
				for ( int j = 0; j < 4; j ++ )
				{
					if ( !wstrEncoding.CompareNoCase( pwszEncodingType[j] ) )
					{
						pfile->m_dwEncodeType = etEncodingType[j] ;
						break ;
					}
				}
			}
			//
			// �p�X���[�h�w��
			//
			pfile->m_strPassword = pDesc->GetAttrString( L"password", L"" ) ;
			//
			// �ǉ�
			//
			flist.Add( pfile ) ;
		}
		else if ( !pDesc->Tag().CompareNoCase( L"directory" ) )
		{
			//
			// �f�B���N�g���G���g���ǉ�
			//
			EFileEntry *	pdir = new EFileEntry ;
			pdir->m_strFileName = pDesc->GetAttrString( L"name", L"" ) ;
			pdir->m_dwAttribute = ERISAArchive::attrDirectory ;
			pdir->m_dwEncodeType = ERISAArchive::etRaw ;
			pdir->m_pSubDir = new EDirectory ;
			pdir->m_pSubDir->m_pParentDir = &flist ;
			flist.Add( pdir ) ;
			//
			if ( ReadListOnDirectory( *(pdir->m_pSubDir), *pDesc ) )
			{
				return	eslErrGeneral ;
			}
		}
		else
		{
			ESLTrace( "�����ȃ^�O�𔭌����܂����B\n" ) ;
		}
	}
	//
	return	eslErrSuccess ;
}

// �w��̃f�B���N�g���������o��
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchiveList::WriteListOnDirectory
	( EDescription & desc, EDirectory & flist )
{
	//
	// �t�@�C���G���g�������������o��
	//
	for ( int i = 0; i < (int) flist.GetSize(); i ++ )
	{
		EString	strLine ;
		EFileEntry *	pfentry = flist.GetAt( i ) ;
		ESLAssert( pfentry != NULL ) ;
		//
		if ( pfentry->m_dwAttribute & ERISAArchive::attrDirectory )
		{
			//
			// �T�u�f�B���N�g��
			//
			ESLAssert( pfentry->m_pSubDir != NULL ) ;
			EDescription *	pDescSub = new EDescription ;
			pDescSub->SetTag( L"directory" ) ;
			pDescSub->SetAttrString
				( L"name", EWideString(pfentry->m_strFileName) ) ;
			//
			desc.AddContentTag( pDescSub ) ;
			//
			if ( WriteListOnDirectory( *pDescSub, *(pfentry->m_pSubDir) ) )
			{
				return	eslErrGeneral ;
			}
		}
		else
		{
			//
			// �t�@�C���G���g��
			//
			EDescription *	pDescFile = new EDescription ;
			pDescFile->SetTag( L"file" ) ;
			pDescFile->SetAttrString
				( L"name", EWideString(pfentry->m_strFileName) ) ;
			//
			desc.AddContentTag( pDescFile ) ;
			//
			// �G���R�[�f�B���O�^�C�v�������o��
			//
			static const wchar_t *	pwszEncodingType[4] =
			{
				L"raw", L"erisa_code", L"bshf_crypt", L"erisa_crypt"
			} ;
			static const ERISAArchive::EncodeType	etEncodingType[4] =
			{
				ERISAArchive::etRaw, ERISAArchive::etERISACode,
				ERISAArchive::etBSHFCrypt, ERISAArchive::etERISACrypt
			} ;
			for ( int j = 0; j < 4; j ++ )
			{
				if ( pfentry->m_dwEncodeType == (DWORD) etEncodingType[j] )
				{
					pDescFile->SetAttrString( L"encoding", pwszEncodingType[j] ) ;
				}
			}
			//
			// �p�X���[�h�������o��
			//
			if ( !pfentry->m_strPassword.IsEmpty() )
			{
				pDescFile->SetAttrString
					( L"password", EWideString(pfentry->m_strPassword) ) ;
			}
		}
	}
	return	eslErrSuccess ;
}

// ���݂̃f�B���N�g���Ƀt�@�C����ǉ�
//////////////////////////////////////////////////////////////////////////////
void ERISAArchiveList::AddFileEntry
	( const char * pszFilePath, DWORD dwEncodeType, const char * pszPassword )
{
	ESLAssert( m_pCurDir != NULL ) ;
	EFileEntry *	pfentry = new EFileEntry ;
	pfentry->m_strFileName = pszFilePath ;
	pfentry->m_dwAttribute = ERISAArchive::attrNormal ;
	pfentry->m_dwEncodeType = dwEncodeType ;
	if ( pszPassword != NULL )
		pfentry->m_strPassword = pszPassword ;
	//
	m_pCurDir->Add( pfentry ) ;
}

// ���݂̃f�B���N�g���ɃT�u�f�B���N�g����ǉ�
//////////////////////////////////////////////////////////////////////////////
void ERISAArchiveList::AddSubDirectory( const char * pszDirName )
{
	ESLAssert( m_pCurDir != NULL ) ;
	EFileEntry *	pfentry = new EFileEntry ;
	pfentry->m_strFileName = pszDirName ;
	pfentry->m_dwAttribute = ERISAArchive::attrDirectory ;
	pfentry->m_dwEncodeType = ERISAArchive::etRaw ;
	pfentry->m_pSubDir = new EDirectory ;
	pfentry->m_pSubDir->m_pParentDir = m_pCurDir ;
	m_pCurDir->Add( pfentry ) ;
}

// ���[�g�f�B���N�g���̃t�@�C���G���g�����擾
//////////////////////////////////////////////////////////////////////////////
ERISAArchiveList::EDirectory *
	ERISAArchiveList::GetRootFileEntries( void )
{
	return	&m_dirRoot ;
}

// ���݂̃f�B���N�g���̃t�@�C���G���g�����擾
//////////////////////////////////////////////////////////////////////////////
ERISAArchiveList::EDirectory *
	ERISAArchiveList::GetCurrentFileEntries( void )
{
	return	m_pCurDir ;
}

// �T�u�f�B���N�g���ֈړ�
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchiveList::DescendDirectory( const char * pszDirName )
{
	ESLAssert( m_pCurDir != NULL ) ;
	for ( unsigned int i = 0; i < m_pCurDir->GetSize(); i ++ )
	{
		EFileEntry *	pfentry = m_pCurDir->GetAt( i ) ;
		ESLAssert( pfentry != NULL ) ;
		if ( !pfentry->m_strFileName.CompareNoCase( pszDirName ) )
		{
			ESLAssert( pfentry->m_pSubDir != NULL ) ;
			m_pCurDir = pfentry->m_pSubDir ;
			return	eslErrSuccess ;
		}
	}
	return	eslErrGeneral ;
}

// �e�f�B���N�g���ֈړ�
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchiveList::AscendDirectory( void )
{
	ESLAssert( m_pCurDir != NULL ) ;
	ESLAssert( m_pCurDir->m_pParentDir != NULL ) ;
	m_pCurDir = m_pCurDir->m_pParentDir ;
	return	eslErrSuccess ;
}

// ���݂̐�΃f�B���N�g���p�X���擾
//////////////////////////////////////////////////////////////////////////////
EString ERISAArchiveList::GetCurrentDirectoryPath( void ) const
{
	ESLAssert( m_pCurDir != NULL ) ;
	EDirectory *	pDir = m_pCurDir ;
	EString			strDirPath = "" ;
	//
	while ( pDir->m_pParentDir != NULL )
	{
		unsigned int	i ;
		EDirectory *	pParent = pDir->m_pParentDir ;
		for ( i = 0; i < pParent->GetSize(); i ++ )
		{
			EFileEntry *	pfentry = pParent->GetAt( i ) ;
			ESLAssert( pfentry != NULL ) ;
			if ( pfentry->m_pSubDir == pDir )
			{
				strDirPath = pfentry->m_strFileName + "\\" + strDirPath ;
				break ;
			}
		}
		ESLAssert( i < pParent->GetSize() ) ;
		pDir = pParent ;
	}
	//
	return	strDirPath ;
}

// ���݂̃f�B���N�g�����΃p�X���w�肵�Ĉړ�
//////////////////////////////////////////////////////////////////////////////
ESLError ERISAArchiveList::MoveCurrentDirectory( const char * pszDirPath )
{
	EString		strDirPath = pszDirPath ;
	int			i = 0 ;
	//
	m_pCurDir = &m_dirRoot ;
	//
	while ( i < (int) strDirPath.GetLength() )
	{
		int	j = strDirPath.Find( '\\', i ) ;
		if ( j < 0 )
		{
			j = strDirPath.GetLength( ) ;
		}
		if ( DescendDirectory( strDirPath.Middle( i, j - i ) ) )
		{
			return	eslErrGeneral ;
		}
		i = j + 1 ;
	}
	//
	return	eslErrSuccess ;
}


#endif
