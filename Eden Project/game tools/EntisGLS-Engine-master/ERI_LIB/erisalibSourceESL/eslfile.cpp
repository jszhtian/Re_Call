
/*****************************************************************************
                   Entis Standard Library implementations
                                                    last update 2003/04/17
 ----------------------------------------------------------------------------

	In this file, the file object classes source codes.

	Copyright (C) 1998-2003 Leshade Entis.  All rights reserved.

 ****************************************************************************/


// Include esl.h
//////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<eritypes.h>
#include	<esl.h>


/*****************************************************************************
                           �t�@�C�����ۃN���X
 ****************************************************************************/

// �N���X�����C���v�������g
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ESLFileObject, ESLObject )
	IMPLEMENT_CLASS_INFO( ERawFile, ESLFileObject )
	IMPLEMENT_CLASS_INFO( EMemoryFile, ESLFileObject )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ESLFileObject::ESLFileObject( void )
	: m_nAttribute( 0 )
{
	::InitializeCriticalSection( &m_cs ) ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ESLFileObject::~ESLFileObject( void )
{
	::DeleteCriticalSection( &m_cs ) ;
}

// �X���b�h�r���A�N�Z�X
//////////////////////////////////////////////////////////////////////////////
void ESLFileObject::Lock( void ) const
{
	::EnterCriticalSection( (CRITICAL_SECTION*) &m_cs ) ;
}

void ESLFileObject::Unlock( void ) const
{
	::LeaveCriticalSection( (CRITICAL_SECTION*) &m_cs ) ;
}

// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
//////////////////////////////////////////////////////////////////////////////
ESLError ESLFileObject::SetEndOfFile( void )
{
	return	eslErrGeneral ;
}

// �t�@�C���̒������擾
//////////////////////////////////////////////////////////////////////////////
UINT64 ESLFileObject::GetLargeLength( void ) const
{
	return	GetLength( ) ;
}

// �t�@�C���|�C���^���ړ�
//////////////////////////////////////////////////////////////////////////////
UINT64 ESLFileObject::SeekLarge
	( INT64 nOffsetPos, ESLFileObject::SeekOrigin fSeekFrom )
{
	return	Seek( (long int) nOffsetPos, fSeekFrom ) ;
}

// �t�@�C���|�C���^���擾
//////////////////////////////////////////////////////////////////////////////
UINT64 ESLFileObject::GetLargePosition( void ) const
{
	return	GetPosition( ) ;
}


/*****************************************************************************
                           ���t�@�C���N���X
 ****************************************************************************/

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERawFile::ERawFile( void )
	: m_hFile( INVALID_HANDLE_VALUE ), m_pszFileTitle( NULL )
{
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERawFile::~ERawFile( void )
{
	Close( ) ;
}

// �t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError ERawFile::Open( const char * pszFileName, int nOpenFlags )
{
	//
	// ���݂̃t�@�C�������
	//
	Close( ) ;
	//
	// �t�@�C���I�[�v���t���O��ϊ�
	//
	DWORD	dwAccess = 0, dwShareMode = 0, dwCreate = OPEN_EXISTING ;
	if ( nOpenFlags & modeCreateFlag )	dwCreate = CREATE_ALWAYS ;
	if ( nOpenFlags & modeRead )		dwAccess |= GENERIC_READ ;
	if ( nOpenFlags & modeWrite )		dwAccess |= GENERIC_WRITE ;
	if ( nOpenFlags & shareRead )		dwShareMode |= FILE_SHARE_READ ;
	if ( nOpenFlags & shareWrite )		dwShareMode |= FILE_SHARE_WRITE ;
	//
	// �t�@�C�����J��
	//
	m_hFile = ::CreateFile
		( pszFileName, dwAccess, dwShareMode,
			NULL, dwCreate, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	//
	if ( m_hFile != INVALID_HANDLE_VALUE )
	{
		//
		// �t�@�C������ݒ�
		//
		m_nAttribute = nOpenFlags ;
		DWORD	dwNeededLength =
			::GetFullPathName( pszFileName, 0x100,
				m_strFilePath.GetBuffer( 0x100 ), (char**) &m_pszFileTitle ) ;
		//
		if( dwNeededLength >= 0x100 )
		{
			::GetFullPathName
				( pszFileName, dwNeededLength,
					m_strFilePath.GetBuffer( dwNeededLength + 1 ),
					(char**) &m_pszFileTitle ) ;
		}
		m_strFilePath.ReleaseBuffer( ) ;
		//
		return	eslErrSuccess ;
	}
	else
	{
		//
		// �t�@�C���̃I�[�v���Ɏ��s
		//
		m_nAttribute = 0 ;
		return	eslErrGeneral ;
	}
}

// �t�@�C���n���h���𕡐����ăt�@�C���I�u�W�F�N�g�Ɋ֘A�t����
//////////////////////////////////////////////////////////////////////////////
ESLError ERawFile::Create( HANDLE hFile, int nOpenFlags )
{
	//
	// ���݂̃t�@�C�������
	//
	Close( ) ;
	//
	// �I�[�v���t���O��ϊ�����
	//
	DWORD	dwAccess = 0 ;
	if ( nOpenFlags & modeRead )	dwAccess |= GENERIC_READ ;
	if ( nOpenFlags & modeWrite )	dwAccess |= GENERIC_WRITE ;
	//
	// �t�@�C���n���h���𕡐�����
	//
	m_pszFileTitle = m_strFilePath = EString( ) ;
	//
	if ( ::DuplicateHandle
		( ::GetCurrentProcess(), hFile,
			::GetCurrentProcess(), &m_hFile, dwAccess, TRUE, 0 ) )
	{
		//
		// �t�@�C������ݒ肷��
		//
		m_nAttribute = nOpenFlags ;
		return	eslErrSuccess ;
	}
	else
	{
		//
		// �t�@�C���̕����Ɏ��s
		//
		m_hFile = INVALID_HANDLE_VALUE ;
		m_nAttribute = 0 ;
		return	eslErrGeneral ;
	}
}

// �t�@�C�������
//////////////////////////////////////////////////////////////////////////////
void ERawFile::Close( void )
{
	Lock( ) ;
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( m_hFile ) ;
		m_hFile = INVALID_HANDLE_VALUE ;
	}
	Unlock( ) ;
}

// �t�@�C���I�u�W�F�N�g�𕡐�����
//////////////////////////////////////////////////////////////////////////////
ESLFileObject * ERawFile::Duplicate( void ) const
{
	ESLAssert( m_hFile != INVALID_HANDLE_VALUE ) ;
	Lock( ) ;
	ERawFile *	pRawFile = new ERawFile ;
	pRawFile->m_strFilePath = m_strFilePath ;
	pRawFile->m_pszFileTitle =
		((const char*)pRawFile->m_strFilePath)
			+ (DWORD)((const char*)m_strFilePath - (DWORD)m_pszFileTitle) ;
	pRawFile->m_hFile = INVALID_HANDLE_VALUE ;
	if ( (m_nAttribute & modeRead) && !m_strFilePath.IsEmpty() )
	{
		DWORD	dwAccess = 0, dwShareMode = 0, dwCreate = OPEN_EXISTING ;
		if ( m_nAttribute & modeRead )		dwAccess |= GENERIC_READ ;
		if ( m_nAttribute & modeWrite )		dwAccess |= GENERIC_WRITE ;
		if ( m_nAttribute & shareRead )		dwShareMode |= FILE_SHARE_READ ;
		if ( m_nAttribute & shareWrite )	dwShareMode |= FILE_SHARE_WRITE ;
		//
		pRawFile->m_hFile = ::CreateFile
			( m_strFilePath, dwAccess, dwShareMode,
				NULL, dwCreate, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		pRawFile->m_nAttribute = m_nAttribute ;
	}
	else
	{
		::DuplicateHandle
			( ::GetCurrentProcess(), m_hFile,
				::GetCurrentProcess(), &(pRawFile->m_hFile),
				0, TRUE, DUPLICATE_SAME_ACCESS );
		pRawFile->m_nAttribute = m_nAttribute ;
	}
	Unlock( ) ;
	return	pRawFile ;
}

// �t�@�C������f�[�^��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
unsigned long int ERawFile::Read
	( void * ptrBuffer, unsigned long int nBytes )
{
	ESLAssert( m_hFile != INVALID_HANDLE_VALUE ) ;
	ESLAssert( m_nAttribute & modeRead ) ;
	Lock( ) ;
	unsigned long int	nReadBytes = 0 ;
	::ReadFile( m_hFile, ptrBuffer, nBytes, &nReadBytes, NULL ) ;
	Unlock( ) ;
	return	nReadBytes ;
}

// �t�@�C���փf�[�^�������o��
//////////////////////////////////////////////////////////////////////////////
unsigned long int ERawFile::Write
	( const void * ptrBuffer, unsigned long int nBytes )
{
	ESLAssert( m_hFile != INVALID_HANDLE_VALUE ) ;
	ESLAssert( m_nAttribute & modeWrite ) ;
	Lock( ) ;
	unsigned long int	nWrittenBytes = 0 ;
	::WriteFile( m_hFile, ptrBuffer, nBytes, &nWrittenBytes, NULL ) ;
	Unlock( ) ;
	return	nWrittenBytes ;
}

// �t�@�C���̒������擾����
//////////////////////////////////////////////////////////////////////////////
unsigned long int ERawFile::GetLength( void ) const
{
	return	(unsigned long int) ERawFile::GetLargeLength( ) ;
}

// �t�@�C���|�C���^���ړ�����
//////////////////////////////////////////////////////////////////////////////
unsigned long int ERawFile::Seek
	( long int nOffsetPos, SeekOrigin fSeekFrom )
{
	return	(unsigned long int)
		ERawFile::SeekLarge( nOffsetPos, fSeekFrom ) ;
}

// �t�@�C���|�C���^���擾����
//////////////////////////////////////////////////////////////////////////////
unsigned long int ERawFile::GetPosition( void ) const
{
	return	(unsigned long int) ERawFile::GetLargePosition( ) ;
}

// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
//////////////////////////////////////////////////////////////////////////////
ESLError ERawFile::SetEndOfFile( void )
{
	ESLAssert( GetAttribute() & modeWrite ) ;
	ESLAssert( m_hFile != INVALID_HANDLE_VALUE ) ;
	if ( ::SetEndOfFile( m_hFile ) )
	{
		return	eslErrSuccess ;
	}
	return	eslErrGeneral ;
}

// �t�@�C���̒������擾
//////////////////////////////////////////////////////////////////////////////
UINT64 ERawFile::GetLargeLength( void ) const
{
	ESLAssert( m_hFile != INVALID_HANDLE_VALUE ) ;
	Lock( ) ;
	DWORD	dwHigh = 0 ;
	DWORD	dwSize = ::GetFileSize( m_hFile, &dwHigh ) ;
	Unlock( ) ;
	if ( GetLastError() != NO_ERROR )
	{
		dwSize = 0 ;
	}
	return	(((UINT64) dwHigh) << 32) | dwSize ;
}

// �t�@�C���|�C���^���ړ�
//////////////////////////////////////////////////////////////////////////////
UINT64 ERawFile::SeekLarge
	( INT64 nOffsetPos, SeekOrigin fSeekFrom )
{
	ESLAssert( m_hFile != INVALID_HANDLE_VALUE ) ;
	LONG	nHigh = (LONG) (nOffsetPos >> 32) ;
	DWORD	dwNewPos ;
	Lock( ) ;
	dwNewPos = ::SetFilePointer
		( m_hFile, (DWORD) nOffsetPos, &nHigh, fSeekFrom ) ;
	Unlock( ) ;
	if ( GetLastError() != NO_ERROR )
	{
		nHigh = 0 ;
		dwNewPos = 0 ;
	}
	return	(((UINT64) nHigh) << 32) | dwNewPos ;
}

// �t�@�C���|�C���^���擾
//////////////////////////////////////////////////////////////////////////////
UINT64 ERawFile::GetLargePosition( void ) const
{
	ESLAssert( m_hFile != INVALID_HANDLE_VALUE ) ;
	LONG	nHigh = 0 ;
	DWORD	dwNewPos ;
	Lock( ) ;
	dwNewPos = ::SetFilePointer( m_hFile, 0, &nHigh, FromCurrent ) ;
	Unlock( ) ;
	if ( GetLastError() != NO_ERROR )
	{
		nHigh = 0 ;
		dwNewPos = 0 ;
	}
	return	(((UINT64) nHigh) << 32) | dwNewPos ;
}


// �t�@�C���̃^�C���X�^���v���擾����
//////////////////////////////////////////////////////////////////////////////
ESLError ERawFile::GetFileTime
	( LPSYSTEMTIME lpCreationTime,
		LPSYSTEMTIME lpLastAccessTime, LPSYSTEMTIME lpLastWriteTime )
{
	if( m_hFile == INVALID_HANDLE_VALUE )
	{
		return	eslErrGeneral ;
	}
	//
	// �t�@�C���^�C�����擾
	//
	FILETIME	ftCreation, ftLastAccess, ftLastWrite ;
	if( !::GetFileTime
		( m_hFile, &ftCreation, &ftLastAccess, &ftLastWrite ) )
	{
		return	eslErrGeneral ;
	}
	//
	// �t�@�C���^�C���t�H�[�}�b�g��ϊ�����
	//
	FILETIME	ftLocalTime ;
	if( lpCreationTime )
	{
		::FileTimeToLocalFileTime( &ftCreation, &ftLocalTime ) ;
		::FileTimeToSystemTime( &ftLocalTime, lpCreationTime ) ;
	}
	if( lpLastAccessTime )
	{
		::FileTimeToLocalFileTime( &ftLastAccess, &ftLocalTime ) ;
		::FileTimeToSystemTime( &ftLocalTime, lpLastAccessTime ) ;
	}
	if( lpLastWriteTime )
	{
		::FileTimeToLocalFileTime( &ftLastWrite, &ftLocalTime ) ;
		::FileTimeToSystemTime( &ftLocalTime, lpLastWriteTime ) ;
	}
	return	eslErrSuccess ;
}

// �t�@�C���̃^�C���X�^���v��ݒ肷��
//////////////////////////////////////////////////////////////////////////////
ESLError ERawFile::SetFileTime
( const SYSTEMTIME * lpCreationTime,
	const SYSTEMTIME * lpLastAccessTime, const SYSTEMTIME * lpLastWriteTime )
{
	if( m_hFile == INVALID_HANDLE_VALUE )
	{
		return	eslErrGeneral ;
	}
	//
	// �t�@�C���^�C���t�H�[�}�b�g��ϊ�����
	//
	FILETIME	ftCreation, ftLastAccess, ftLastWrite ;
	FILETIME	ftLocalTime ;
	LPFILETIME	pftCreation = NULL, pftLastAccess = NULL, pftLastWrite = NULL ;
	if ( lpCreationTime != NULL )
	{
		::SystemTimeToFileTime( lpCreationTime, &ftLocalTime ) ;
		::LocalFileTimeToFileTime( &ftLocalTime, &ftCreation ) ;
		pftCreation = &ftCreation ;
	}
	if ( lpLastAccessTime != NULL )
	{
		::SystemTimeToFileTime( lpLastAccessTime, &ftLocalTime ) ;
		::LocalFileTimeToFileTime( &ftLocalTime, &ftLastAccess ) ;
		pftLastAccess = &ftLastAccess ;
	}
	if ( lpLastWriteTime != NULL )
	{
		::SystemTimeToFileTime( lpLastWriteTime, &ftLocalTime ) ;
		::LocalFileTimeToFileTime( &ftLocalTime, &ftLastWrite ) ;
		pftLastWrite = &ftLastWrite ;
	}
	//
	// �t�@�C���^�C����ݒ�
	//
	if ( !::SetFileTime
		( m_hFile, pftCreation, pftLastAccess, pftLastWrite ) )
	{
		return	eslErrGeneral ;
	}
	return	eslErrSuccess ;
}


/*****************************************************************************
                           �������t�@�C���N���X
 ****************************************************************************/

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
EMemoryFile::EMemoryFile( void )
	: m_ptrMemory( NULL )
{
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
EMemoryFile::~EMemoryFile( void )
{
	Delete( ) ;
}

// �ǂݏ����\�ȃ������t�@�C�����쐬����
//////////////////////////////////////////////////////////////////////////////
ESLError EMemoryFile::Create( unsigned long int nLength )
{
	//
	// ���݂̃������t�@�C�����������
	//
	Delete( ) ;
	//
	// ���������m�ۂ���
	//
	Lock( ) ;
	m_nLength = 0 ;
	m_nPosition = 0 ;
	m_nBufferSize = nLength ;
	m_ptrMemory = ::eslHeapAllocate( NULL, m_nBufferSize, 0 ) ;
	m_nAttribute = (modeRead | modeCreate) ;
	Unlock( ) ;

	return	eslErrSuccess ;
}

// �ǂݍ��ݐ�p�̃������t�@�C���I�u�W�F�N�g���쐬����
//////////////////////////////////////////////////////////////////////////////
ESLError EMemoryFile::Open
	( const void * ptrMemory, unsigned long int nLength )
{
	//
	// ���݂̃������t�@�C�����������
	//
	Delete( ) ;
	//
	// ���������֘A�t����
	//
	ESLAssert( ptrMemory != NULL ) ;
	Lock( ) ;
	m_nBufferSize = m_nLength = nLength ;
	m_nPosition = 0 ;
	m_ptrMemory = (void*) ptrMemory ;
	m_nAttribute = modeRead ;
	Unlock( ) ;

	return	eslErrSuccess ;
}

// �������t�@�C�����������
//////////////////////////////////////////////////////////////////////////////
void EMemoryFile::Delete( void )
{
	Lock( ) ;
	if( m_ptrMemory != NULL )
	{
		if( m_nAttribute & modeWrite )
		{
			::eslHeapFree( NULL, m_ptrMemory ) ;
		}
		m_ptrMemory = NULL ;
		m_nAttribute = 0 ;
	}
	Unlock( ) ;
}

// �������t�@�C���𕡐�����
//////////////////////////////////////////////////////////////////////////////
ESLFileObject * EMemoryFile::Duplicate( void ) const
{
	ESLAssert( m_ptrMemory != NULL ) ;
	Lock( ) ;
	EMemoryFile *	pMemFile = new EMemoryFile ;
	pMemFile->Create( m_nLength ) ;
	pMemFile->Write( m_ptrMemory, m_nLength ) ;
	pMemFile->Seek( 0, FromBegin ) ;
	Unlock( ) ;
	return	pMemFile ;
}

// �������t�@�C������f�[�^��]������
//////////////////////////////////////////////////////////////////////////////
unsigned long int EMemoryFile::Read
	( void * ptrBuffer, unsigned long int nBytes )
{
	ESLAssert( m_ptrMemory != NULL ) ;
	ESLAssert( m_nAttribute & modeRead ) ;
	Lock( ) ;
	unsigned long int	nReadBytes = nBytes ;
	if ( (nReadBytes + m_nPosition) > m_nLength )
	{
		nReadBytes = m_nLength - m_nPosition ;
	}
	::eslMoveMemory
		( ptrBuffer, (((BYTE*)m_ptrMemory) + m_nPosition), nReadBytes ) ;
	m_nPosition += nReadBytes ;
	Unlock( ) ;
	return	nReadBytes ;
}

// �������t�@�C���Ƀf�[�^��]������
//////////////////////////////////////////////////////////////////////////////
unsigned long int EMemoryFile::Write
	( const void * ptrBuffer, unsigned long int nBytes )
{
	ESLAssert( m_ptrMemory != NULL ) ;
	ESLAssert( m_nAttribute & modeWrite ) ;
	Lock( ) ;
	unsigned long int	nWrittenBytes = nBytes ;
	if ( nWrittenBytes + m_nPosition > m_nLength )
	{
		m_nLength = nWrittenBytes + m_nPosition ;
		if ( m_nLength > m_nBufferSize )
		{
			m_nBufferSize =
				(m_nLength + nWrittenBytes + 0xFFF) & (~0xFFF) ;
			m_ptrMemory = ::eslHeapReallocate
				( NULL, m_ptrMemory, m_nBufferSize, 0 ) ;
		}
	}
	::eslMoveMemory
		( (((BYTE*)m_ptrMemory) + m_nPosition), ptrBuffer, nWrittenBytes ) ;
	m_nPosition += nWrittenBytes ;
	Unlock( ) ;
	return	nWrittenBytes ;
}

// �������t�@�C���̒������擾����
//////////////////////////////////////////////////////////////////////////////
unsigned long int EMemoryFile::GetLength( void ) const
{
	ESLAssert( m_ptrMemory != NULL ) ;
	return	m_nLength ;
}

// �������t�@�C���̃|�C���^���ړ�����
//////////////////////////////////////////////////////////////////////////////
unsigned long int EMemoryFile::Seek
	( long int nOffsetPos, SeekOrigin fSeekFrom )
{
	ESLAssert( m_ptrMemory != NULL ) ;
	Lock( ) ;
	switch ( fSeekFrom )
	{
	case	FromBegin:
		m_nPosition = nOffsetPos ;
		break ;
	case	FromCurrent:
		m_nPosition += nOffsetPos ;
		break ;
	case	FromEnd:
		m_nPosition = m_nLength + nOffsetPos ;
		break ;
	default:
		ESLAssert( false ) ;
		break ;
	}
	if ( (signed long int) m_nPosition < 0 )
	{
		m_nPosition = 0 ;
	}
	else if ( m_nPosition > m_nLength )
	{
		if ( m_nAttribute & modeWrite )
		{
			m_nLength = m_nPosition ;
		}
	}
	Unlock( ) ;
	return	m_nPosition ;
}

// �������t�@�C���|�C���^���擾����
//////////////////////////////////////////////////////////////////////////////
unsigned long int EMemoryFile::GetPosition( void ) const
{
	ESLAssert( m_ptrMemory != NULL ) ;
	return	m_nPosition ;
}

// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
//////////////////////////////////////////////////////////////////////////////
ESLError EMemoryFile::SetEndOfFile( void )
{
	ESLAssert( m_ptrMemory != NULL ) ;
	ESLAssert( m_nAttribute & modeWrite ) ;
	m_nLength = m_nPosition ;
	return	eslErrSuccess ;
}


#if	!defined(COMPACT_NOA_DECODER)

/*****************************************************************************
                 �X�g���[�~���O�o�b�t�@�t�@�C���N���X
 ****************************************************************************/

// �N���X���
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO2( EStreamFileBuffer, ESLFileObject, EStreamBuffer )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
EStreamFileBuffer::EStreamFileBuffer( void )
{
	SetAttribute( modeReadWrite ) ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
EStreamFileBuffer::~EStreamFileBuffer( void )
{
}

// �t�@�C���I�u�W�F�N�g�𕡐�����
//////////////////////////////////////////////////////////////////////////////
ESLFileObject * EStreamFileBuffer::Duplicate( void ) const
{
	return	new EStreamFileBuffer ;
}

// �t�@�C������ǂݍ���
//////////////////////////////////////////////////////////////////////////////
unsigned long int EStreamFileBuffer::Read
	( void * ptrBuffer, unsigned long int nBytes )
{
	return	EStreamBuffer::Read( ptrBuffer, nBytes ) ;
}

// �t�@�C���֏����o��
//////////////////////////////////////////////////////////////////////////////
unsigned long int EStreamFileBuffer::Write
	( const void * ptrBuffer, unsigned long int nBytes )
{
	return	EStreamBuffer::Write( ptrBuffer, nBytes ) ;
}

// �t�@�C���̒������擾
//////////////////////////////////////////////////////////////////////////////
unsigned long int EStreamFileBuffer::GetLength( void ) const
{
	return	EStreamBuffer::GetLength( ) ;
}

// �t�@�C���|�C���^���ړ�
//////////////////////////////////////////////////////////////////////////////
unsigned long int EStreamFileBuffer::Seek
	( long int nOffsetPos, SeekOrigin fSeekFrom )
{
	if ( fSeekFrom == FromEnd )
	{
		nOffsetPos += EStreamBuffer::GetLength( ) ;
	}
	EStreamBuffer::GetBuffer( nOffsetPos ) ;
	EStreamBuffer::Release( nOffsetPos ) ;
	return	0 ;
}

// �t�@�C���|�C���^���擾
//////////////////////////////////////////////////////////////////////////////
unsigned long int EStreamFileBuffer::GetPosition( void ) const
{
	return	0 ;
}


/*****************************************************************************
                 �����X�g���[�~���O�o�b�t�@�t�@�C���N���X
 ****************************************************************************/

// �N���X���
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ESyncStreamFile, ESLFileObject )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ESyncStreamFile::ESyncStreamFile( void )
{
	m_hFinished = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	m_hWritten = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	m_nLength = -1 ;
	m_nPosition = 0 ;
	m_nBufLength = 0 ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ESyncStreamFile::~ESyncStreamFile( void )
{
	::CloseHandle( m_hFinished ) ;
	::CloseHandle( m_hWritten ) ;
}

// ���e��������
//////////////////////////////////////////////////////////////////////////////
void ESyncStreamFile::Initialize( unsigned long int nLength )
{
	Lock( ) ;
	::ResetEvent( m_hFinished ) ;
	::ResetEvent( m_hWritten ) ;
	m_tblBuffer.RemoveAll( ) ;
	m_nAttribute = (modeRead | modeCreate) ;
	m_nLength = nLength ;
	m_nPosition = 0 ;
	m_nBufLength = 0 ;
	Unlock( ) ;
}

// �o�b�t�@�ւ̏������݂�����
//////////////////////////////////////////////////////////////////////////////
void ESyncStreamFile::FinishStream( void )
{
	Lock( ) ;
	::SetEvent( m_hFinished ) ;
	m_nLength = m_nBufLength ;
	m_nAttribute = modeRead ;
	Unlock( ) ;
}

// �t�@�C���I�u�W�F�N�g�𕡐�����
//////////////////////////////////////////////////////////////////////////////
ESLFileObject * ESyncStreamFile::Duplicate( void ) const
{
	EMemoryFile *	pfile = new EMemoryFile ;
	pfile->Create( m_nBufLength ) ;
	//
	for ( unsigned int i = 0; i < m_tblBuffer.GetSize(); i ++ )
	{
		EBuffer *	pBuf = m_tblBuffer.GetAt( i ) ;
		ESLAssert( pBuf != NULL ) ;
		pfile->Write( pBuf->pbytBuf, pBuf->dwBytes ) ;
	}
	//
	pfile->Seek( m_nPosition, FromBegin ) ;
	return	pfile ;
}

// �t�@�C������ǂݍ���
//////////////////////////////////////////////////////////////////////////////
unsigned long int ESyncStreamFile::Read
	( void * ptrBuffer, unsigned long int nBytes )
{
	DWORD	dwResult ;
	HANDLE	hEvents[2] =
	{
		m_hWritten, m_hFinished
	} ;
	unsigned long int	nReadBytes = 0 ;
	BYTE *	pbytBuffer = (BYTE*) ptrBuffer ;
	Lock( ) ;
	while ( nBytes != 0 )
	{
		//
		// �o�b�t�@�����O����Ă���L���ȃo�C�g�����擾
		//
		unsigned long int	nLeftBytes ;
		for ( ; ; )
		{
			nLeftBytes = m_nBufLength - m_nPosition ;
			if ( nLeftBytes > 0 )
			{
				break ;
			}
			//
			// �o�b�t�@�����O����
			//
			Unlock( ) ;
			dwResult =
				::WaitForMultipleObjects
					( 2, hEvents, FALSE, INFINITE ) ;
			Lock( ) ;
			if ( dwResult == (WAIT_OBJECT_0 + 1) )
			{
				break ;
			}
			::ResetEvent( m_hWritten ) ;
		}
		//
		// �o�b�t�@����f�[�^��ǂݍ���
		//
		if ( nLeftBytes == 0 )
		{
			break ;
		}
		else if ( nLeftBytes > nBytes )
		{
			nLeftBytes = nBytes ;
		}
		unsigned int	iBuf = m_nPosition >> BUFFER_SCALE ;
		unsigned int	iOffset = m_nPosition & BUFFER_MASK ;
		EBuffer *	pBuf = m_tblBuffer.GetAt( iBuf ) ;
		ESLAssert( pBuf != NULL ) ;
		//
		if ( iOffset + nLeftBytes > pBuf->dwBytes )
		{
			nLeftBytes = pBuf->dwBytes - iOffset ;
		}
		BYTE *	pbytSrc = pBuf->pbytBuf + iOffset ;
		for ( unsigned long int i = 0; i < nLeftBytes; i ++ )
		{
			pbytBuffer[i] = pbytSrc[i] ;
		}
		//
		// �|�C���^��i�߂�
		//
		pbytBuffer += nLeftBytes ;
		nReadBytes += nLeftBytes ;
		nBytes -= nLeftBytes ;
		m_nPosition += nLeftBytes ;
	}
	Unlock( ) ;
	return	nReadBytes ;
}

// �t�@�C���֏����o��
//////////////////////////////////////////////////////////////////////////////
unsigned long int ESyncStreamFile::Write
	( const void * ptrBuffer, unsigned long int nBytes )
{
	unsigned long int	nWrittenBytes = 0 ;
	const BYTE *	pbytBuffer = (const BYTE *) ptrBuffer ;
	Lock( ) ;
	while ( nBytes != 0 )
	{
		//
		// �������ރo�b�t�@���擾����
		//
		unsigned int	iBuf = m_nBufLength >> BUFFER_SCALE ;
		unsigned int	iOffset = m_nBufLength & BUFFER_MASK ;
		EBuffer *	pBuf = m_tblBuffer.GetAt( iBuf ) ;
		if ( pBuf == NULL )
		{
			pBuf = new EBuffer ;
			m_tblBuffer.SetAt( iBuf, pBuf ) ;
		}
		//
		// �������݉\�ȃo�C�g�����v�Z���A��������
		//
		unsigned int	nWriteBytes = nBytes ;
		if ( nWriteBytes > BUFFER_SIZE - pBuf->dwBytes )
		{
			nWriteBytes = BUFFER_SIZE - pBuf->dwBytes ;
		}
		BYTE *	pbytDst = pBuf->pbytBuf + iOffset ;
		//
		for ( unsigned int i = 0; i < nWriteBytes; i ++ )
		{
			pbytDst[i] = pbytBuffer[i] ;
		}
		//
		// �|�C���^��i�߂�
		//
		pbytBuffer += nWriteBytes ;
		nWrittenBytes += nWriteBytes ;
		nBytes -= nWriteBytes ;
		pBuf->dwBytes += nWriteBytes ;
		m_nBufLength += nWriteBytes ;
	}
	::SetEvent( m_hWritten ) ;
	Unlock( ) ;
	return	nWrittenBytes ;
}

// �t�@�C���̒������擾
//////////////////////////////////////////////////////////////////////////////
unsigned long int ESyncStreamFile::GetLength( void ) const
{
	return	m_nLength ;
}

// �t�@�C���|�C���^���ړ�
//////////////////////////////////////////////////////////////////////////////
unsigned long int ESyncStreamFile::Seek
	( long int nOffsetPos, SeekOrigin fSeekFrom )
{
	Lock( ) ;
	switch ( fSeekFrom )
	{
	case	FromBegin:
		break ;
	case	FromCurrent:
		nOffsetPos += m_nPosition ;
		break ;
	case	FromEnd:
		if ( m_nLength != -1 )
			nOffsetPos += m_nLength ;
		else
			nOffsetPos += m_nBufLength ;
		break ;
	}
	//
	DWORD	dwResult ;
	HANDLE	hEvents[2] =
	{
		m_hWritten, m_hFinished
	} ;
	for ( ; ; )
	{
		if ( (unsigned long int) nOffsetPos <= m_nBufLength )
		{
			m_nPosition = nOffsetPos ;
			break ;
		}
		Unlock( ) ;
		dwResult =
			::WaitForMultipleObjects
				( 2, hEvents, FALSE, INFINITE ) ;
		Lock( ) ;
		if ( dwResult == (WAIT_OBJECT_0 + 1) )
		{
			break ;
		}
		::ResetEvent( m_hWritten ) ;
	}
	Unlock( ) ;
	return	m_nPosition ;
}

// �t�@�C���|�C���^���擾
//////////////////////////////////////////////////////////////////////////////
unsigned long int ESyncStreamFile::GetPosition( void ) const
{
	return	m_nPosition ;
}

// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
//////////////////////////////////////////////////////////////////////////////
ESLError ESyncStreamFile::SetEndOfFile( void )
{
	FinishStream( ) ;
	return	eslErrSuccess ;
}


#endif
