
/*****************************************************************************
                         E R I S A - L i b r a r y
 ----------------------------------------------------------------------------
    Copyright (C) 2000-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>


#if	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                    MIO�t�@�C���X�g���[���Đ��I�u�W�F�N�g
 *****************************************************************************/

IMPLEMENT_CLASS_INFO( MIODynamicPlayer, ESLObject )

//
// EPreloadBuffer �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::EPreloadBuffer::EPreloadBuffer( DWORD dwLength )
{
	m_ptrBuffer = (BYTE*) ::eslHeapAllocate( NULL, dwLength, 0 ) ;
	m_nKeySample = 0 ;
	EMemoryFile::Open( m_ptrBuffer, dwLength ) ;
}

//
// EPreloadBuffer ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::EPreloadBuffer::~EPreloadBuffer( void )
{
	::eslHeapFree( NULL, m_ptrBuffer ) ;
}

//
// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::MIODynamicPlayer( void )
{
	m_pmioc = NULL ;
	m_pmiod = NULL ;
	m_hThread = NULL ;
	m_hQueueFull = NULL ;
	m_hQueueNotEmpty = NULL ;
	m_hQueueSpace = NULL ;
	::InitializeCriticalSection( &m_cs ) ;
}

//
// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::~MIODynamicPlayer( void )
{
	Close( ) ;
	::DeleteCriticalSection( &m_cs ) ;
}

//
// MIO �t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError MIODynamicPlayer::Open
	( ESLFileObject * pFile, unsigned int nPreloadSize )
{
	Close( ) ;
	//
	// ERI�t�@�C�����J���i�X�g���[�����R�[�h�܂ŊJ���j
	//////////////////////////////////////////////////////////////////////////
	if ( m_erif.Open( pFile, m_erif.otOpenStream ) )
	{
		ESLTrace( "[MIODynamicPlayer::Open] �t�@�C�����J���܂���ł����B\n" ) ;
		return	ESLErrorMsg( "ERI �t�@�C�����J���܂���ł����B" ) ;
	}
	//
	// �W�J�I�u�W�F�N�g������������
	//////////////////////////////////////////////////////////////////////////
	if ( !(m_erif.m_fdwReadMask & ERIFile::rmSoundInfo) )
	{
		ESLTrace( "[MIODynamicPlayer::Open] ������񂪂���܂���B\n" ) ;
		return	ESLErrorMsg( "�t�@�C���ɉ�����񂪊܂܂�Ă��܂���B" ) ;
	}
	m_pmiod = CreateMIODecoder( ) ;
	if ( m_pmiod->Initialize( m_erif.m_MIOInfHdr ) )
	{
		ESLTrace( "[MIODynamicPlayer::Open] MIO �f�R�[�_�̏������Ɏ��s���܂����B\n" ) ;
		return	ESLErrorMsg( "MIO �f�R�[�_�̏������Ɏ��s���܂����B" ) ;
	}
	m_pmioc = new ERISADecodeContext( 0x10000 ) ;
	//
	// ��ǂ݃o�b�t�@�z����m��
	//////////////////////////////////////////////////////////////////////////
	if ( nPreloadSize == 0 )
	{
		m_queueSound.SetLimit( 8 ) ;
	}
	else if ( nPreloadSize == -1 )
	{
		m_queueSound.SetLimit( 2 ) ;
	}
	else
	{
		m_queueSound.SetLimit( nPreloadSize ) ;
	}
	//
	// �X���b�h�𐶐�
	//////////////////////////////////////////////////////////////////////////
	m_hQueueFull = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	m_hQueueNotEmpty = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	m_hQueueSpace = ::CreateEvent( NULL, TRUE, TRUE, NULL ) ;
	//
	if ( nPreloadSize == -1 )
	{
		m_nCurrentSample = 0 ;
		EPreloadBuffer *	pBuffer =
			LoadSoundStream( m_nCurrentSample ) ;
		if ( pBuffer != NULL )
		{
			AddPreloadBuffer( pBuffer ) ;
			m_nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
		}
	}
	else
	{
		m_hThread = ::CreateThread
			( NULL, 0, &MIODynamicPlayer::ThreadProc, this, 0, &m_idThread ) ;
		if ( !(m_hQueueNotEmpty && m_hQueueSpace && m_hThread) )
		{
			return	eslErrGeneral ;
		}
		HANDLE	hEventList[2] =
		{
			m_hQueueFull, m_hThread
		} ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		if ( dwWaitResult != WAIT_OBJECT_0 )
		{
			return	eslErrGeneral ;
		}
	}

	return	eslErrSuccess ;
}

//
// MIO �t�@�C�������
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::Close( void )
{
	//
	// �X���b�h���I��������
	if ( m_hThread != NULL )
	{
		::PostThreadMessage( m_idThread, tmQuit, 0, 0 ) ;
		::WaitForSingleObject( m_hThread, INFINITE ) ;
		::CloseHandle( m_hThread ) ;
		m_hThread = NULL ;
	}
	//
	// ��ǂ݃L���[���N���A����
	if ( m_hQueueFull != NULL )
	{
		::CloseHandle( m_hQueueFull ) ;
		m_hQueueFull = NULL ;
	}
	if ( m_hQueueNotEmpty != NULL )
	{
		::CloseHandle( m_hQueueNotEmpty ) ;
		m_hQueueNotEmpty = NULL ;
	}
	if ( m_hQueueSpace != NULL )
	{
		::CloseHandle( m_hQueueSpace ) ;
		m_hQueueSpace = NULL ;
	}
	m_queueSound.RemoveAll( ) ;
	//
	// �L�[�|�C���g�z����N���A����
	m_listKeySample.RemoveAll( ) ;
	//
	// �W�J�I�u�W�F�N�g���폜����
	if ( m_pmiod != NULL )
	{
		m_pmiod->Delete( ) ;
		delete	m_pmiod ;
		m_pmiod = NULL ;
	}
	if ( m_pmioc != NULL )
	{
		delete	m_pmioc ;
		m_pmioc = NULL ;
	}
	//
	// �t�@�C�������
	m_erif.Close( ) ;
}

//
// �w��T���v���ֈړ����A���߂̃u���b�N�̃f�[�^���擾����
//////////////////////////////////////////////////////////////////////////////
void * MIODynamicPlayer::GetWaveBufferFrom
	( ULONG nSample, DWORD & dwBytes, DWORD & dwOffsetBytes )
{
	//
	// �V�[�N���b�Z�[�W�𑗐M���A�V�[�N����������܂ő҂�
	//////////////////////////////////////////////////////////////////////////
	if ( m_hThread == NULL )
	{
		SeekKeySample( nSample, m_nCurrentSample ) ;
	}
	else
	{
		HANDLE	hEventList[2] ;
		hEventList[0] = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		hEventList[1] = m_hThread ;
		::PostThreadMessage
			( m_idThread, tmSeekSound, (WPARAM) nSample, (LPARAM) hEventList[0] ) ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		::CloseHandle( hEventList[0] ) ;
		if ( dwWaitResult != WAIT_OBJECT_0 )
		{
			return	NULL ;
		}
	}
	//
	// �擪�̃f�[�^���擾���ēW�J����
	//////////////////////////////////////////////////////////////////////////
	EPreloadBuffer *	pBuffer = GetPreloadBuffer( ) ;
	if ( pBuffer == NULL )
	{
		return	NULL ;
	}
	if ( (nSample < pBuffer->m_nKeySample) ||
		(nSample >= pBuffer->m_nKeySample + pBuffer->m_miodh.dwSampleCount) )
	{
		return	NULL ;
	}
	DWORD	dwSampleBytes = GetChannelCount() * GetBitsPerSample() / 8 ;
	dwBytes = pBuffer->m_miodh.dwSampleCount * dwSampleBytes ;
	dwOffsetBytes = (nSample - pBuffer->m_nKeySample) * dwSampleBytes ;
	//
	void *	ptrWaveBuf = AllocateWaveBuffer( dwBytes ) ;
	//
	m_pmioc->AttachInputFile( pBuffer ) ;
	//
	if ( m_pmiod->DecodeSound( *m_pmioc, pBuffer->m_miodh, ptrWaveBuf ) )
	{
		DeleteWaveBuffer( ptrWaveBuf ) ;
		delete	pBuffer ;
		return	NULL ;
	}
	delete	pBuffer ;

	return	ptrWaveBuf ;
}

//
// ���̉����f�[�^���X�g���[���̐擪�ł��邩�H
//////////////////////////////////////////////////////////////////////////////
bool MIODynamicPlayer::IsNextDataRewound( void )
{
	if ( m_hThread != NULL )
	{
		HANDLE	hEventList[2] ;
		hEventList[0] = m_hQueueNotEmpty ;
		hEventList[1] = m_hThread ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		if ( dwWaitResult != WAIT_OBJECT_0 )
		{
			return	false ;
		}
	}
	else if ( m_queueSound.GetSize() == 0 )
	{
		EPreloadBuffer *	pBuffer =
			LoadSoundStream( m_nCurrentSample ) ;
		if ( pBuffer != NULL )
		{
			AddPreloadBuffer( pBuffer ) ;
			m_nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
		}
	}
	bool	fRewound = false ;
	Lock( ) ;
	if ( m_queueSound.GetSize() != 0 )
	{
		EPreloadBuffer *	pBuffer = m_queueSound.GetAt( 0 ) ;
		if ( (pBuffer != NULL) && (pBuffer->m_nKeySample == 0) )
		{
			fRewound = true ;
		}
	}
	Unlock( ) ;
	//
	return	fRewound ;
}

//
// ���̉����f�[�^���擾
//////////////////////////////////////////////////////////////////////////////
void * MIODynamicPlayer::GetNextWaveBuffer( DWORD & dwBytes )
{
	EPreloadBuffer *	pBuffer = GetPreloadBuffer( ) ;
	if ( pBuffer == NULL )
	{
		return	NULL ;
	}
	DWORD	dwSampleBytes = GetChannelCount() * GetBitsPerSample() / 8 ;
	dwBytes = pBuffer->m_miodh.dwSampleCount * dwSampleBytes ;
	//
	void *	ptrWaveBuf = AllocateWaveBuffer( dwBytes ) ;
	//
	m_pmioc->AttachInputFile( pBuffer ) ;
	//
	if ( m_pmiod->DecodeSound( *m_pmioc, pBuffer->m_miodh, ptrWaveBuf ) )
	{
		DeleteWaveBuffer( ptrWaveBuf ) ;
		delete	pBuffer ;
		return	NULL ;
	}
	delete	pBuffer ;

	return	ptrWaveBuf ;
}

//
// �����o�b�t�@�m��
//////////////////////////////////////////////////////////////////////////////
void * MIODynamicPlayer::AllocateWaveBuffer( DWORD dwBytes )
{
	return	::eslHeapAllocate( NULL, dwBytes, 0 ) ;
}

//
// �����f�[�^�j������
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::DeleteWaveBuffer( void * ptrWaveBuf )
{
	::eslHeapFree( NULL, ptrWaveBuf ) ;
}

// �����W�J�I�u�W�F�N�g�𐶐�
//////////////////////////////////////////////////////////////////////////////
MIODecoder * MIODynamicPlayer::CreateMIODecoder( void )
{
	return	new MIODecoder ;
}

//
// ERIFile �I�u�W�F�N�g���擾����
//////////////////////////////////////////////////////////////////////////////
const ERIFile & MIODynamicPlayer::GetERIFile( void ) const
{
	return	m_erif ;
}

//
// �`���l�������擾����
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetChannelCount( void ) const
{
	return	m_erif.m_MIOInfHdr.dwChannelCount ;
}

//
// �T���v�����O���g�����擾����
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetFrequency( void ) const
{
	return	m_erif.m_MIOInfHdr.dwSamplesPerSec ;
}

//
// �T���v�����O�r�b�g����\���擾����
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetBitsPerSample( void ) const
{
	return	m_erif.m_MIOInfHdr.dwBitsPerSample ;
}

//
// �S�̂̒����i�T���v�����j���擾����
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetTotalSampleCount( void ) const
{
	return	m_erif.m_MIOInfHdr.dwAllSampleCount ;
}

//
// ��ǂ݃o�b�t�@���擾����
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::EPreloadBuffer *
		MIODynamicPlayer::GetPreloadBuffer( void )
{
	if ( m_hThread == NULL )
	{
		while ( m_queueSound.GetSize() <= 1 )
		{
			EPreloadBuffer *	pBuffer =
				LoadSoundStream( m_nCurrentSample ) ;
			if ( pBuffer == NULL )
			{
				break ;
			}
			AddPreloadBuffer( pBuffer ) ;
			m_nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
		}
	}
	else
	{
		HANDLE	hEventList[2] ;
		hEventList[0] = m_hQueueNotEmpty ;
		hEventList[1] = m_hThread ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		if ( dwWaitResult != WAIT_OBJECT_0 )
		{
			return	NULL ;
		}
	}
	EPreloadBuffer *	pBuffer = NULL ;
	Lock( ) ;
	if ( m_queueSound.GetSize() != 0 )
	{
		pBuffer = m_queueSound.GetAt( 0 ) ;
		m_queueSound.DetachAt( 0 ) ;
		if ( m_queueSound.GetSize() == 0 )
		{
			::ResetEvent( m_hQueueNotEmpty ) ;
		}
		if ( m_queueSound.GetSize() < m_queueSound.GetLimit() )
		{
			::SetEvent( m_hQueueSpace ) ;
			::ResetEvent( m_hQueueFull ) ;
		}
	}
	Unlock( ) ;
	return	pBuffer ;
}

//
// ��ǂ݃o�b�t�@�ɒǉ�����
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::AddPreloadBuffer( EPreloadBuffer * pBuffer )
{
	Lock( ) ;
	if ( m_queueSound.GetSize() < m_queueSound.GetLimit() )
	{
		m_queueSound.Add( pBuffer ) ;
		if ( m_queueSound.GetSize() != 0 )
		{
			::SetEvent( m_hQueueNotEmpty ) ;
		}
		if ( m_queueSound.GetSize() >= m_queueSound.GetLimit() )
		{
			::ResetEvent( m_hQueueSpace ) ;
			::SetEvent( m_hQueueFull ) ;
		}
	}
	Unlock( ) ;
}

//
// �X���b�h�֐�
//////////////////////////////////////////////////////////////////////////////
DWORD WINAPI MIODynamicPlayer::ThreadProc( LPVOID parameter )
{
	::eriInitializeTask( ) ;
	DWORD	dwResult =
		((MIODynamicPlayer*)parameter)->LoadingThreadProc( ) ;
	::eriCloseTask( ) ;
	return	dwResult ;
}

DWORD MIODynamicPlayer::LoadingThreadProc( void )
{
	//
	// ���b�Z�[�W�L���[���쐬
	//////////////////////////////////////////////////////////////////////////
	MSG		msg ;
	::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ;
	//
	// �ϐ�������
	//////////////////////////////////////////////////////////////////////////
	unsigned int	nCurrentSample = 0 ;
	//
	// �e���R�[�h��ǂݍ���
	//////////////////////////////////////////////////////////////////////////
	for ( ; ; )
	{
		//
		// ���R�[�h���J��
		//////////////////////////////////////////////////////////////////////
		unsigned int	nLastSample = nCurrentSample ;
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		EPreloadBuffer *	pBuffer = LoadSoundStream( nCurrentSample ) ;
		if ( pBuffer == NULL )
		{
			break ;
		}
		//
		// ���b�Z�[�W����M���E�F�C�g
		//////////////////////////////////////////////////////////////////////
		bool	fQuit = false ;
		do
		{
			//
			// �L���[���󂭂����b�Z�[�W����M����܂ő҂�
			DWORD	dwWaitResult =
				::MsgWaitForMultipleObjects
					( 1, &m_hQueueSpace, FALSE, INFINITE, QS_ALLINPUT ) ;
			if ( (pBuffer != NULL) && (dwWaitResult == WAIT_OBJECT_0) )
			{
				AddPreloadBuffer( pBuffer ) ;
				nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
				pBuffer = NULL ;
			}
			//
			if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				if ( msg.message == tmSeekSound )
				{
					//
					// �����f�[�^���V�[�N
					//
					if ( pBuffer != NULL )
					{
						m_erif.Seek( dwRecPosition, ESLFileObject::FromBegin ) ;
						delete	pBuffer ;
						nCurrentSample = nLastSample ;
						pBuffer = NULL ;
					}
					//
					SeekKeySample( (ULONG) msg.wParam, nCurrentSample ) ;
					//
					::SetEvent( (HANDLE) msg.lParam ) ;
				}
				else if ( msg.message == tmQuit )
				{
					//
					// �I��
					//
					fQuit = true ;
					break ;
				}
			}
		}
		while ( pBuffer != NULL ) ;
		//
		if ( fQuit )
		{
			delete	pBuffer ;
			break ;
		}
	}

	return	0 ;
}

//
// �����f�[�^���R�[�h��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::EPreloadBuffer *
	MIODynamicPlayer::LoadSoundStream( unsigned int & nCurrentSample )
{
	EKeyPoint	keypoint ;
	DWORD	dwRecPosition = m_erif.GetPosition( ) ;
	while ( m_erif.DescendRecord( (const UINT64 *)"SoundStm" ) )
	{
		if ( nCurrentSample == 0 )
		{
			// 1���������R�[�h�������ꍇ�̓G���[
			return	NULL ;
		}
		//
		// ���R�[�h�̏I�[�ɓ��B������
		// �����I�ɐ擪�Ɉړ�
		EKeyPoint *	pKeyPoint = SearchKeySample( nCurrentSample ) ;
		if ( pKeyPoint == NULL )
		{
			keypoint.m_nKeySample = nCurrentSample ;
			keypoint.m_dwRecOffset = dwRecPosition ;
			AddKeySample( keypoint ) ;
		}
		//
		nCurrentSample = 0 ;
		m_erif.Seek( 0, ESLFileObject::FromBegin ) ;
	}
	//
	// �����f�[�^���R�[�h��ǂݍ���
	DWORD	dwDataBytes = m_erif.GetLength( ) ;
	EPreloadBuffer *	pBuffer =
		new EPreloadBuffer( dwDataBytes - sizeof(MIO_DATA_HEADER) ) ;
	//
	pBuffer->m_nKeySample = nCurrentSample ;
	m_erif.Read( &(pBuffer->m_miodh), sizeof(MIO_DATA_HEADER) ) ;
	m_erif.Read( pBuffer->m_ptrBuffer,
					dwDataBytes - sizeof(MIO_DATA_HEADER) ) ;
	//
	m_erif.AscendRecord( ) ;
	//
	// �L�[�|�C���g�̐ݒ�
	if ( pBuffer->m_miodh.bytFlags & MIO_LEAD_BLOCK )
	{
		EKeyPoint *	pKeyPoint = SearchKeySample( nCurrentSample ) ;
		if ( pKeyPoint == NULL )
		{
			keypoint.m_nKeySample = nCurrentSample ;
			keypoint.m_dwRecOffset = dwRecPosition ;
			AddKeySample( keypoint ) ;
		}
	}
	return	pBuffer ;
}

//
// �L�[�t���[���|�C���g��ǉ�����
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::AddKeySample( const MIODynamicPlayer::EKeyPoint & key )
{
	Lock( ) ;
	m_listKeySample.Add( new EKeyPoint( key ) ) ;
	Unlock( ) ;
}

//
// �w��̃L�[�t���[������������
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::EKeyPoint *
	MIODynamicPlayer::SearchKeySample( unsigned int nKeySample )
{
	int		iFirst, iMiddle, iEnd ;
	EKeyPoint *	pFoundKey = NULL ;
	//
	Lock( ) ;
	//
	if ( m_listKeySample.GetSize() == 0 )
	{
		Unlock( ) ;
		return	NULL ;
	}
	//
	iFirst = 0 ;
	iMiddle = 0 ;
	iEnd = m_listKeySample.GetSize() - 1 ;
	//
	for ( ; ; )
	{
		if ( iFirst >= iEnd )
		{
			pFoundKey = NULL ;
			ESLAssert( iMiddle < (int) m_listKeySample.GetSize() ) ;
			if ( m_listKeySample[iMiddle].m_nKeySample == nKeySample )
			{
				pFoundKey = m_listKeySample.GetAt( iMiddle ) ;
			}
			else if ( m_listKeySample[iMiddle].m_nKeySample > nKeySample )
			{
				while ( iMiddle > 0 )
				{
					if ( m_listKeySample[-- iMiddle].m_nKeySample <= nKeySample )
					{
						pFoundKey = m_listKeySample.GetAt( iMiddle ) ;
						break ;
					}
				}
			}
			else
			{
				while ( iMiddle < (int) m_listKeySample.GetSize() - 1 )
				{
					if ( m_listKeySample[iMiddle + 1].m_nKeySample == nKeySample )
					{
						pFoundKey = m_listKeySample.GetAt( iMiddle + 1 ) ;
						break ;
					}
					else if ( m_listKeySample[iMiddle + 1].m_nKeySample > nKeySample )
					{
						pFoundKey = m_listKeySample.GetAt( iMiddle ) ;
						break ;
					}
					++ iMiddle ;
				}
			}
			break ;
		}
		//
		iMiddle = (iFirst + iEnd) / 2 ;
		pFoundKey = m_listKeySample.GetAt( iMiddle ) ;
		//
		if ( pFoundKey->m_nKeySample == nKeySample )
		{
			break ;
		}
		if ( pFoundKey->m_nKeySample > nKeySample )
		{
			iEnd = iMiddle - 1 ;
		}
		else
		{
			iFirst = iMiddle + 1 ;
		}
	}
	//
	Unlock( ) ;
	//
	return	pFoundKey ;
}

//
// �w��̃T���v�����܂ރu���b�N��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::SeekKeySample
	( ULONG nSample, unsigned int & nCurrentSample )
{
	EPreloadBuffer *	pBuffer ;
	//
	// ���ɐ�ǂ݃L���[�ɓǂݍ��܂�Ă��Ȃ������f
	//////////////////////////////////////////////////////////////////////////
	Lock( ) ;
	//
	unsigned int	iLoaded = 0 ;
	unsigned int	iLeadBlock = 0 ;
	while ( iLoaded < m_queueSound.GetSize() )
	{
		pBuffer = m_queueSound.GetAt( iLoaded ) ;
		if ( pBuffer != NULL )
		{
			if ( pBuffer->m_miodh.bytFlags & MIO_LEAD_BLOCK )
			{
				iLeadBlock = iLoaded ;
			}
			if ( (pBuffer->m_nKeySample <= nSample)
				&& ((nSample - pBuffer->m_nKeySample)
						< pBuffer->m_miodh.dwSampleCount) )
			{
				break ;
			}
		}
		++ iLoaded ;
	}
	//
	// ���ɓǂݍ��܂�Ă���ꍇ�ɂ͂����܂ŃV�[�N����
	//
	if ( iLoaded < m_queueSound.GetSize() )
	{
		//
		// �ł��߂����[�h�u���b�N�܂Ŕj������
		unsigned int	i ;
		m_queueSound.RemoveBetween( 0, iLeadBlock ) ;
		//
		// ����̃u���b�N�܂ŃV�[�N����
		for ( i = iLeadBlock; i < iLoaded; i ++ )
		{
			pBuffer = GetPreloadBuffer( ) ;
			if ( pBuffer == NULL )
				break ;
			//
			DWORD	dwSampleBytes =
				GetChannelCount() * GetBitsPerSample() / 8 ;
			DWORD	dwBytes =
				pBuffer->m_miodh.dwSampleCount * dwSampleBytes ;
			//
			void *	ptrWaveBuf = AllocateWaveBuffer( dwBytes ) ;
			//
			m_pmioc->AttachInputFile( pBuffer ) ;
			//
			m_pmiod->DecodeSound( *m_pmioc, pBuffer->m_miodh, ptrWaveBuf ) ;
			//
			DeleteWaveBuffer( ptrWaveBuf ) ;
			delete	pBuffer ;
		}
		Unlock( ) ;
		return ;
	}
	//
	// ���ɓǂݍ��܂�Ă���u���b�N��j������
	//
	m_queueSound.SetSize( 0 ) ;
	Unlock( ) ;
	//
	// ���X�g�Ɏw��̃T���v�����܂ރL�[���o�^����Ă��邩�������A
	// �o�^����Ă��Ȃ��ꍇ�ɂ́A�V�[�N����
	//////////////////////////////////////////////////////////////////////////
	EKeyPoint *	pKeyPoint = SearchKeySample( nSample ) ;
	if ( pKeyPoint == NULL )
	{
		Lock( ) ;
		if ( m_listKeySample.GetSize() > 0 )
		{
			pKeyPoint = m_listKeySample.GetAt( m_listKeySample.GetSize() - 1 ) ;
			m_erif.Seek( pKeyPoint->m_dwRecOffset, ESLFileObject::FromBegin ) ;
			nCurrentSample = pKeyPoint->m_nKeySample ;
		}
		Unlock( ) ;
		//
		// �e���R�[�h�������ǂݍ���
		for ( ; ; )
		{
			DWORD	dwRecPosition = m_erif.GetPosition( ) ;
			if ( m_erif.DescendRecord( (const UINT64 *)"SoundStm" ) )
			{
				return ;
			}
			//
			MIO_DATA_HEADER	miodh ;
			m_erif.Read( &miodh, sizeof(miodh) ) ;
			m_erif.AscendRecord( ) ;
			//
			// �L�[�|�C���g�̐ݒ�
			if ( miodh.bytFlags & MIO_LEAD_BLOCK )
			{
				pKeyPoint = SearchKeySample( nCurrentSample ) ;
				if ( pKeyPoint == NULL )
				{
					EKeyPoint	keypoint ;
					keypoint.m_nKeySample = nCurrentSample ;
					keypoint.m_dwRecOffset = dwRecPosition ;
					AddKeySample( keypoint ) ;
					//
					pKeyPoint =
						m_listKeySample.GetAt( m_listKeySample.GetSize() - 1 ) ;
				}
			}
			//
			// �ʒu�̍X�V
			if ( (nCurrentSample <= nSample) &&
					((nSample - nCurrentSample) < miodh.dwSampleCount) )
			{
				break ;
			}
			nCurrentSample += miodh.dwSampleCount ;
		}
	}
	//
	// �w��̃L�[�|�C���g����V�[�N����
	//////////////////////////////////////////////////////////////////////////
	if ( pKeyPoint == NULL )
	{
		return ;
	}
	nCurrentSample = pKeyPoint->m_nKeySample ;
	m_erif.Seek( pKeyPoint->m_dwRecOffset, ESLFileObject::FromBegin ) ;
	//
	for ( ; ; )
	{
		if ( m_erif.DescendRecord( (const UINT64 *)"SoundStm" ) )
		{
			return ;
		}
		//
		// �����f�[�^���R�[�h��ǂݍ���
		DWORD	dwDataBytes = m_erif.GetLength( ) ;
		EPreloadBuffer *	pBuffer =
			new EPreloadBuffer( dwDataBytes - sizeof(MIO_DATA_HEADER) ) ;
		//
		pBuffer->m_nKeySample = nCurrentSample ;
		m_erif.Read( &(pBuffer->m_miodh), sizeof(MIO_DATA_HEADER) ) ;
		m_erif.Read( pBuffer->m_ptrBuffer,
						dwDataBytes - sizeof(MIO_DATA_HEADER) ) ;
		//
		m_erif.AscendRecord( ) ;
		//
		// �ʒu�̍X�V
		if ( (nCurrentSample <= nSample) &&
			((nSample - nCurrentSample) < pBuffer->m_miodh.dwSampleCount) )
		{
			nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
			AddPreloadBuffer( pBuffer ) ;
			break ;
		}
		nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
		//
		// �f�[�^��W�J���Ĕj������
		DWORD	dwSampleBytes = GetChannelCount() * GetBitsPerSample() / 8 ;
		DWORD	dwBytes = pBuffer->m_miodh.dwSampleCount * dwSampleBytes ;
		//
		void *	ptrWaveBuf = AllocateWaveBuffer( dwBytes ) ;
		//
		m_pmioc->AttachInputFile( pBuffer ) ;
		//
		m_pmiod->DecodeSound( *m_pmioc, pBuffer->m_miodh, ptrWaveBuf ) ;
		//
		DeleteWaveBuffer( ptrWaveBuf ) ;
		delete	pBuffer ;
	}
}

//
// �r�������i�N���e�B�J���Z�N�V�����j
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::Lock( void )
{
	::EnterCriticalSection( &m_cs ) ;
}

void MIODynamicPlayer::Unlock( void )
{
	::LeaveCriticalSection( &m_cs ) ;
}


#endif
