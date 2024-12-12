
/*****************************************************************************
                         E R I S A - L i b r a r y
 ----------------------------------------------------------------------------
    Copyright (C) 2000-2004 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>

#if	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                    ERI�A�j���[�V�����t�@�C���I�u�W�F�N�g
 *****************************************************************************/

IMPLEMENT_CLASS_INFO( ERIAnimation, ESLObject )

//
// EPreloadBuffer �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EPreloadBuffer::EPreloadBuffer( DWORD dwLength )
{
	m_ptrBuffer = (BYTE*) ::eslHeapAllocate( NULL, dwLength, 0 ) ;
	m_ptrMemory = m_ptrBuffer ;
	m_iFrameIndex = 0 ;
	m_ui64RecType = 0 ;
	EMemoryFile::Open( m_ptrBuffer, dwLength ) ;
}

//
// EPreloadBuffer ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EPreloadBuffer::~EPreloadBuffer( void )
{
	::eslHeapFree( NULL, m_ptrBuffer ) ;
}

//
// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::ERIAnimation( void )
{
	m_fWaveOutput = false ;
	m_fWaveStreaming = false ;
	m_peric = NULL ;
	m_perid = NULL ;
	m_pmioc = NULL ;
	m_pmiod = NULL ;
	m_iDstBufIndex = 0 ;
	m_pDstImage[0] = NULL ;
	m_pDstImage[1] = NULL ;
	m_pDstImage[2] = NULL ;
	m_pDstImage[3] = NULL ;
	m_pDstImage[4] = NULL ;
	m_hThread = NULL ;
	m_hQueueNotEmpty = NULL ;
	m_hQueueSpace = NULL ;
	::InitializeCriticalSection( &m_cs ) ;
}

//
// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::~ERIAnimation( void )
{
	Close( ) ;
	::DeleteCriticalSection( &m_cs ) ;
}

//
// �摜�W�J�o�̓o�b�t�@�v��
//////////////////////////////////////////////////////////////////////////////
EGL_IMAGE_INFO * ERIAnimation::CreateImageBuffer
	( DWORD format, SDWORD width, SDWORD height, DWORD bpp )
{
	DWORD	dwLineBytes = ((width * bpp / 8) + 0x03) & ~0x03 ;
	DWORD	dwImageBytes = dwLineBytes * height ;
	DWORD	dwPaletteLength = 0 ;
	if ( bpp <= 8 )
	{
		dwPaletteLength = (1 << bpp) ;
	}
	DWORD	dwPaletteBytes = dwPaletteLength * sizeof(EGL_PALETTE) ;
	DWORD	dwBufSize =
		sizeof(EGL_IMAGE_INFO) + dwImageBytes + dwPaletteBytes ;
	BYTE *	ptrBuffer = (BYTE*) ::eslHeapAllocate( NULL, dwBufSize, 0 ) ;
	//
	EGL_IMAGE_INFO *	peii = (EGL_IMAGE_INFO*) ptrBuffer ;
	peii->dwInfoSize = sizeof(EGL_IMAGE_INFO) ;
	peii->fdwFormatType = format ;
	peii->ptrOffsetPixel = 0 ;
	peii->ptrImageArray = ptrBuffer + sizeof(EGL_IMAGE_INFO) + dwPaletteBytes ;
	if ( dwPaletteLength == 0 )
	{
		peii->pPaletteEntries = NULL ;
		peii->dwPaletteCount = 0 ;
	}
	else
	{
		peii->pPaletteEntries =
			(EGL_PALETTE*) (ptrBuffer + sizeof(EGL_IMAGE_INFO)) ;
		peii->dwPaletteCount = dwPaletteLength ;
	}
	peii->dwImageWidth = width ;
	peii->dwImageHeight = height ;
	peii->dwBitsPerPixel = bpp ;
	peii->dwBytesPerLine = dwLineBytes ;
	peii->dwSizeOfImage = dwImageBytes ;
	peii->dwClippedPixel = 0 ;
	//
	return	peii ;
}

//
// �摜�W�J�o�̓o�b�t�@����
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::DeleteImageBuffer( EGL_IMAGE_INFO * peii )
{
	if ( peii != NULL )
	{
		::eslHeapFree( NULL, peii ) ;
	}
}

//
// �摜�W�J�I�u�W�F�N�g����
//////////////////////////////////////////////////////////////////////////////
ERISADecoder * ERIAnimation::CreateERIDecoder( void )
{
	return	new ERISADecoder ;
}

//
// �����W�J�I�u�W�F�N�g����
//////////////////////////////////////////////////////////////////////////////
MIODecoder * ERIAnimation::CreateMIODecoder( void )
{
	return	new MIODecoder ;
}

//
// �����o�͗v��
//////////////////////////////////////////////////////////////////////////////
bool ERIAnimation::RequestWaveOut
	( DWORD channels, DWORD frequency, DWORD bps )
{
	return	false ;
}

//
// �����o�͏I��
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::CloseWaveOut( void )
{
}

//
// �����f�[�^�o��
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::PushWaveBuffer( void * ptrWaveBuf, DWORD dwBytes )
{
	DeleteWaveBuffer( ptrWaveBuf ) ;
}

//
// �����o�b�t�@�m��
//////////////////////////////////////////////////////////////////////////////
void * ERIAnimation::AllocateWaveBuffer( DWORD dwBytes )
{
	return	::eslHeapAllocate( NULL, dwBytes, 0 ) ;
}

//
// �����f�[�^�j������
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::DeleteWaveBuffer( void * ptrWaveBuf )
{
	::eslHeapFree( NULL, ptrWaveBuf ) ;
}

//
// �����X�g���[�~���O�J�n
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::BeginWaveStreaming( void )
{
	if ( m_fWaveOutput )
	{
		m_fWaveStreaming = true ;
		//
		if ( m_hThread != NULL )
		{
			HANDLE	hEventList[2] ;
			hEventList[0] = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
			hEventList[1] = m_hThread ;
			::PostThreadMessage
				( m_idThread, tmSeekSound,
					m_iCurrentFrame, (LPARAM) hEventList[0] ) ;
			DWORD	dwWaitResult =
				::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
			::CloseHandle( hEventList[0] ) ;
		}
		else
		{
			SeekKeyWave( m_listKeyWave, m_iCurrentFrame ) ;
		}
	}
}

//
// �����X�g���[�~���O�I��
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::EndWaveStreaming( void )
{
	m_fWaveStreaming = false ;
}

//
// �A�j���[�V�����t�@�C�����J��
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::Open
	( ESLFileObject * pFile, unsigned int nPreloadSize, DWORD fdwFlags )
{
	Close( ) ;
	//
	// ERI�t�@�C�����J���i�X�g���[�����R�[�h�܂ŊJ���j
	//////////////////////////////////////////////////////////////////////////
	if ( m_erif.Open( pFile, m_erif.otOpenStream ) )
	{
		ESLTrace( "[ERIAnimation::Open] "
				"ERI �t�@�C���w�b�_���J���܂���ł����B\n" ) ;
		return	ESLErrorMsg( "�t�@�C���w�b�_���J���܂���ł����B" ) ;
	}
	//
	// �W�J�I�u�W�F�N�g������������
	//////////////////////////////////////////////////////////////////////////
	m_perid = CreateERIDecoder( ) ;
	if ( m_perid->Initialize( m_erif.m_InfoHeader ) )
	{
		ESLTrace( "[ERIAnimation::Open] �f�R�[�_�[�̏������Ɏ��s���܂����B\n" ) ;
		return	ESLErrorMsg( "�f�R�[�_�[�̏������Ɏ��s���܂����B" ) ;
	}
	m_peric = new ERISADecodeContext( 0x10000 ) ;
	//
	m_fWaveOutput = false ;
	if ( m_erif.m_fdwReadMask & ERIFile::rmSoundInfo )
	{
		//
		// �T�E���h���܂܂�Ă���
		m_pmiod = CreateMIODecoder( ) ;
		if ( m_pmiod->Initialize( m_erif.m_MIOInfHdr ) == eslErrSuccess )
		{
			m_pmioc = new ERISADecodeContext( 0x10000 ) ;
			m_pmioc->AttachInputFile( &m_erif ) ;
			//
			// �����o�͗v��
			if ( RequestWaveOut
				( m_erif.m_MIOInfHdr.dwChannelCount,
					m_erif.m_MIOInfHdr.dwSamplesPerSec,
					m_erif.m_MIOInfHdr.dwBitsPerSample ) )
			{
				//
				// �����o�͗v�����󂯓����ꂽ
				m_fWaveOutput = true ;
			}
		}
	}
	//
	// �摜�o�b�t�@�𐶐�
	//////////////////////////////////////////////////////////////////////////
	m_fTopDown = ((fdwFlags & ERISADecoder::dfTopDown) != 0) ;
	m_fdwDecFlags = fdwFlags ;
	signed int	nImageHeight ;
	nImageHeight = m_erif.m_InfoHeader.nImageHeight ;
	if ( nImageHeight < 0 )
	{
		nImageHeight = - nImageHeight ;
	}
	unsigned int	nBitsPerPixel = m_erif.m_InfoHeader.dwBitsPerPixel ;
	if ( nBitsPerPixel == 24 )
	{
		nBitsPerPixel = 32 ;
	}
	int		i, nCount = 5 ;
	if ( m_erif.m_InfoHeader.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		nCount = 2 ;
	}
	for ( i = 0; i < nCount; i ++ )
	{
		m_pDstImage[i] = CreateImageBuffer
			( m_erif.m_InfoHeader.fdwFormatType,
				m_erif.m_InfoHeader.nImageWidth, nImageHeight, nBitsPerPixel ) ;
		if ( m_pDstImage[i] == NULL )
		{
			return	eslErrGeneral ;
		}
		m_iDstFrameIndex[i] = (unsigned int) -1 ;
	}
	for ( i = nCount; i < 5; i ++ )
	{
		m_iDstFrameIndex[i] = (unsigned int) -1 ;
	}
	//
	// ��ǂ݃o�b�t�@�z����m��
	//////////////////////////////////////////////////////////////////////////
	if ( nPreloadSize == 0 )
	{
		nPreloadSize = -30 ;
	}
	if ( (signed int) nPreloadSize >= 0 )
	{
		m_nPreloadLimit = nPreloadSize ;
	}
	else
	{
		m_nPreloadLimit = - (int) nPreloadSize ;
	}
	m_queueImage.SetLimit( m_nPreloadLimit ) ;
	//
	// �t���[���V�[�N�p�L�[�|�C���g�z��m��
	//////////////////////////////////////////////////////////////////////////
	m_listKeyFrame.SetLimit( m_erif.m_FileHeader.dwFrameCount ) ;
	m_listKeyWave.SetLimit( m_erif.m_FileHeader.dwFrameCount ) ;
	//
	// �X���b�h�𐶐�
	//////////////////////////////////////////////////////////////////////////
	m_hQueueNotEmpty = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	m_hQueueSpace = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	m_iCurrentFrame = 0 ;
	m_iDstBufIndex = 0 ;
	m_nCacheBFrames = 0 ;
	if ( m_erif.m_InfoHeader.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		m_nCacheBFrames = (unsigned int) -1 ;
	}
	if ( (signed int) nPreloadSize >= 0 )
	{
		m_hThread = ::CreateThread
			( NULL, 0, &ERIAnimation::ThreadProc, this, 0, &m_idThread ) ;
		if ( !(m_hQueueNotEmpty && m_hQueueSpace && m_hThread) )
		{
			return	eslErrGeneral ;
		}
		::WaitForSingleObject( m_hQueueSpace, INFINITE ) ;
		//
		DWORD	dwBeginTime = ::GetCurrentTime() ;
		while ( ::WaitForSingleObject( m_hQueueSpace, 0 ) == WAIT_OBJECT_0 )
		{
			::Sleep( 1 ) ;
			if ( ::GetCurrentTime() - dwBeginTime > 500 )
			{
				break ;
			}
		}
	}
	else
	{
		//
		// �X���b�h�𐶐����Ȃ��Ő�ǂ݂���
		//
		EKeyPoint	keypoint ;
		m_iPreloadFrame = 0 ;
		m_nPreloadWaveSamples = 0 ;
		keypoint.m_iKeyFrame = 0 ;
		keypoint.m_dwSubSample = 0 ;
		keypoint.m_dwRecOffset = m_erif.GetPosition( ) ;
		AddKeyPoint( m_listKeyFrame, keypoint ) ;
		m_hThread = NULL ;
		//
		EPreloadBuffer *	pBuffer ;
		do
		{
			pBuffer = LoadMovieStream( m_iPreloadFrame ) ;
			if ( pBuffer == NULL )
				break ;
			AddPreloadBuffer( pBuffer ) ;
		}
		while ( m_queueImage.GetSize() < m_nPreloadLimit ) ;
	}
	//
	// ��1�t���[����W�J����
	//////////////////////////////////////////////////////////////////////////
	if ( SeekToBegin( ) )
	{
		return	eslErrGeneral ;
	}

	return	eslErrSuccess ;
}

//
// �A�j���[�V�����t�@�C�������
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::Close( void )
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
	m_queueImage.RemoveAll( ) ;
	//
	// �L�[�t���[���|�C���g�z����N���A����
	m_listKeyFrame.RemoveAll( ) ;
	m_listKeyWave.RemoveAll( ) ;
	//
	// �摜�o�b�t�@���폜����
	for ( int i = 0; i < 5; i ++ )
	{
		if ( m_pDstImage[i] != NULL )
		{
			DeleteImageBuffer( m_pDstImage[i] ) ;
			m_pDstImage[i] = NULL ;
		}
	}
	//
	// �W�J�I�u�W�F�N�g���폜����
	if ( m_perid != NULL )
	{
		m_perid->Delete( ) ;
		delete	m_perid ;
		m_perid = NULL ;
	}
	if ( m_pmiod != NULL )
	{
		m_pmiod->Delete( ) ;
		delete	m_pmiod ;
		m_pmiod = NULL ;
	}
	if ( m_peric != NULL )
	{
		delete	m_peric ;
		m_peric = NULL ;
	}
	if ( m_pmioc != NULL )
	{
		delete	m_pmioc ;
		m_pmioc = NULL ;
	}
	//
	// �����o�͂̏I��
	if ( m_fWaveOutput )
	{
		CloseWaveOut( ) ;
		m_fWaveOutput = false ;
	}
	//
	// �t�@�C�������
	m_erif.Close( ) ;
}

//
// �擪�t���[���ֈړ�
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::SeekToBegin( void )
{
	//
	// �X���b�h�ɃV�[�N���b�Z�[�W�𑗂�
	//////////////////////////////////////////////////////////////////////////
	if ( m_hThread == NULL )
	{
		SeekKeyPoint( m_listKeyFrame, 0, m_iPreloadFrame ) ;
		EPreloadBuffer *	pBuffer ;
		while ( m_queueImage.GetSize() < m_nPreloadLimit )
		{
			pBuffer = LoadMovieStream( m_iPreloadFrame ) ;
			if ( pBuffer == NULL )
				break ;
			AddPreloadBuffer( pBuffer ) ;
		}
	}
	else
	{
		HANDLE	hEventList[2] ;
		hEventList[0] = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		hEventList[1] = m_hThread ;
		::PostThreadMessage
			( m_idThread, tmSeekFrame, 0, (LPARAM) hEventList[0] ) ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		::CloseHandle( hEventList[0] ) ;
		if ( dwWaitResult != WAIT_OBJECT_0 )
		{
			return	eslErrGeneral ;
		}
	}
	//
	// �擪�t���[����W�J
	//////////////////////////////////////////////////////////////////////////
	if ( SeekToNextFrame( ) )
	{
		return	eslErrGeneral ;
	}

	return	eslErrSuccess ;
}

//
// ���̃t���[���ֈړ�
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::SeekToNextFrame( int nSkipFrame )
{
	//
	// �X�L�b�v���镪�����ǂݔ�΂�
	//////////////////////////////////////////////////////////////////////////
	EObjArray<EPreloadBuffer>	queSkipFrames ;
	EPreloadBuffer *	pBuf ;
	int		i ;
	for ( i = 0; i < nSkipFrame; i ++ )
	{
		//
		// �X�L�b�v����t���[�������o��
		// �� GetPreloadBuffer �֐��͓W�J���ׂ����Ԃ�
		//    �Ĕz�񂵂ăt���[����Ԃ����Ƃɒ��ӁI
		//
		pBuf = GetPreloadBuffer( ) ;
		if ( pBuf != NULL )
		{
			int	nFrameType = GetFrameBufferType( pBuf ) ;
			if ( (nFrameType == ftIntraFrame)
					|| (nFrameType == ftPredictionalFrame) )
			{
				// I, P �s�N�`���͈�U�m�ۂ��Ă���
				queSkipFrames.Add( pBuf ) ;
			}
			else if ( nFrameType == ftBidirectionalFrame )
			{
				// B �s�N�`���͔j�����Ă��\��Ȃ�
				delete	pBuf ;
			}
			else
			{
				ApplyPaletteTable( pBuf ) ;
				delete	pBuf ;
				i -- ;
			}
		}
	}
	//
	// �W�J����ׂ��^�[�Q�b�g�t���[�����擾����
	//////////////////////////////////////////////////////////////////////////
	EPreloadBuffer *	pNextFrame ;
	for ( ; ; )
	{
		pNextFrame = GetPreloadBuffer( ) ;
		if ( pNextFrame == NULL )
		{
			// �t�@�C���I�[����
			m_iCurrentFrame = m_iDstFrameIndex[m_iDstBufIndex]
								= m_erif.m_FileHeader.dwFrameCount - 1 ;
			return	eslErrSuccess ;
		}
		if ( GetFrameBufferType( pNextFrame ) >= ftIntraFrame )
		{
			break ;
		}
		ApplyPaletteTable( pNextFrame ) ;
		delete	pNextFrame ;
	}
	//
	// I �s�N�`��������ł���ꍇ�ɂ͂����܂ł̃f�[�^��j������
	//////////////////////////////////////////////////////////////////////////
	if ( m_nCacheBFrames == (unsigned int) -1 )
	{
		if ( GetFrameBufferType( pNextFrame ) == ftIntraFrame )
		{
			queSkipFrames.RemoveAll( ) ;
		}
	}
	for ( i = 0; i < (int) queSkipFrames.GetSize(); i ++ )
	{
		pBuf = queSkipFrames.GetLastAt( i ) ;
		if ( GetFrameBufferType( pBuf ) == ftIntraFrame )
		{
			if ( m_nCacheBFrames == (unsigned int) -1 )
			{
				queSkipFrames.RemoveBetween( 0, i ) ;
			}
			else
			{
				if ( (i >= 1) ||
					(GetFrameBufferType( pNextFrame )
									!= ftBidirectionalFrame) )
				{
					DecodeFrame( pBuf, ERISADecoder::dfQuickDecode ) ;
					queSkipFrames.RemoveBetween( 0, i + 1 ) ;
				}
			}
			break ;
		}
	}
	//
	// �w��t���[���܂ŏ����W�J
	//////////////////////////////////////////////////////////////////////////
	ESLError	errResult = eslErrSuccess ;
	for ( i = 0; i < (int) queSkipFrames.GetSize(); i ++ )
	{
		pBuf = queSkipFrames.GetAt( i ) ;
		if ( pBuf != NULL )
		{
			m_fdwDecFlags |= ERISADecoder::dfQuickDecode ;
			DecodeFrame( pBuf ) ;
		}
	}
	//
	if ( nSkipFrame > 0 )
	{
		m_fdwDecFlags |= ERISADecoder::dfQuickDecode ;
	}
	errResult = DecodeFrame( pNextFrame ) ;
	//
	if ( m_nCacheBFrames == (unsigned int) -1 )
	{
		m_iCurrentFrame = m_iDstFrameIndex[m_iDstBufIndex] ;
	}
	else if ( GetFrameBufferType( pNextFrame ) == ftBidirectionalFrame )
	{
		m_iCurrentFrame = m_iDstFrameIndex[2] ;
	}
	else
	{
		m_iCurrentFrame = m_iDstFrameIndex[(m_iDstBufIndex ^ 0x01)] ;
	}
	delete	pNextFrame ;

	return	eslErrSuccess ;
}

//
// �w��̃t���[���Ɉړ�
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::SeekToFrame( unsigned int iFrameIndex )
{
	//
	// �������
	//////////////////////////////////////////////////////////////////////////
	if ( m_iCurrentFrame == iFrameIndex )
	{
		return	eslErrSuccess ;
	}
	if ( m_iCurrentFrame <= iFrameIndex )
	{
		unsigned int	nKeyFrame = GetKeyFrameCount( ) ;
		if ( nKeyFrame == 0 )
		{
			return	SeekToNextFrame( iFrameIndex - m_iCurrentFrame - 1 ) ;
		}
		if ( (iFrameIndex / nKeyFrame) == (m_iCurrentFrame / nKeyFrame) )
		{
			return	SeekToNextFrame( iFrameIndex - m_iCurrentFrame - 1 ) ;
		}
	}
	//
	// �X�g���[�~���O��~
	//////////////////////////////////////////////////////////////////////////
	if ( m_fWaveStreaming )
	{
		EndWaveStreaming( ) ;
		m_fWaveStreaming = false ;
	}
	//
	// �X���b�h�ɃV�[�N���b�Z�[�W�𑗂�
	//////////////////////////////////////////////////////////////////////////
	unsigned int	iKeyFrame = 0 ;
	if ( GetKeyFrameCount() > 0 )
	{
		iKeyFrame = iFrameIndex / GetKeyFrameCount() ;
		iKeyFrame *= GetKeyFrameCount() ;
	}
	if ( m_hThread == NULL )
	{
		SeekKeyPoint( m_listKeyFrame, iKeyFrame, m_iPreloadFrame ) ;
	}
	else
	{
		HANDLE	hEventList[2] ;
		hEventList[0] = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		hEventList[1] = m_hThread ;
		::PostThreadMessage
			( m_idThread, tmSeekFrame,
				(WPARAM) iKeyFrame, (LPARAM) hEventList[0] ) ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		::CloseHandle( hEventList[0] ) ;
		if ( dwWaitResult != WAIT_OBJECT_0 )
		{
			return	eslErrGeneral ;
		}
	}
	//
	// �L�[�t���[����W�J
	//////////////////////////////////////////////////////////////////////////
	if ( SeekToNextFrame( ) )
	{
		return	eslErrGeneral ;
	}
	//
	// �����t���[����W�J
	//////////////////////////////////////////////////////////////////////////
	if ( iFrameIndex > iKeyFrame )
	{
		if ( SeekToNextFrame( iFrameIndex - iKeyFrame - 1 ) )
		{
			return	eslErrGeneral ;
		}
	}

	return	eslErrSuccess ;
}

//
// �w��̃t���[���̓L�[�t���[�����H
//////////////////////////////////////////////////////////////////////////////
bool ERIAnimation::IsKeyFrame( unsigned int iFrameIndex )
{
	if ( m_erif.m_FileHeader.dwKeyFrameCount == 1 )
		return	true ;
	if ( iFrameIndex == 0 )
		return	true ;
	if ( m_erif.m_FileHeader.dwKeyFrameCount == 0 )
		return	false ;
	//
	return	(iFrameIndex % m_erif.m_FileHeader.dwKeyFrameCount) == 0 ;
}

//
// �œK�ȃt���[���X�L�b�v�����擾����
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::GetBestSkipFrames( unsigned int nCurrentTime )
{
	unsigned int	iFrameIndex = TimeToFrameIndex( nCurrentTime ) ;
	if ( iFrameIndex <= m_iCurrentFrame )
	{
		return	0 ;
	}
	int	i = iFrameIndex - m_iCurrentFrame ;
	int	nDefSkipCount = 0 ;
	if ( m_nCacheBFrames != (unsigned int) -1 )
	{
		//
		// B �t���[�����l������
		//
		nDefSkipCount = i ;
		if ( nDefSkipCount == (int) m_nCacheBFrames )
		{
			return	m_nCacheBFrames ;
		}
		if ( nDefSkipCount < (int) m_nCacheBFrames )
		{
			return	nDefSkipCount - 1 ;
		}
		nDefSkipCount = m_nCacheBFrames ;
	}
	//
	// I �t���[������������
	//
	if ( i > (int) m_queueImage.GetSize() )
	{
		i = (int) m_queueImage.GetSize() ;
	}
	while ( (-- i) >= 0 )
	{
		EPreloadBuffer *	pPreloaded = m_queueImage.GetAt( i ) ;
		if ( pPreloaded == NULL )
		{
			continue ;
		}
		if ( pPreloaded->m_ui64RecType == *((UINT64*)"ImageFrm") )
		{
			int	iSkipFrames = 0 ;
			while ( (-- i) >= 0 )
			{
				pPreloaded = m_queueImage.GetAt( i ) ;
				if ( pPreloaded == NULL )
				{
					continue ;
				}
				UINT64	recid = pPreloaded->m_ui64RecType ;
				if ( (recid == *((UINT64*)"ImageFrm")) ||
						(recid == *((UINT64*)"DiffeFrm")) )
				{
					iSkipFrames ++ ;
				}
			}
			return	iSkipFrames ;
		}
	}
	return	nDefSkipCount ;
}

//
// �t���[����W�J����
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::DecodeFrame( EPreloadBuffer * pFrame, DWORD fdwFlags )
{
	ESLError	errResult = eslErrSuccess ;
	int	nFrameType = GetFrameBufferType( pFrame ) ;
	if ( nFrameType >= ftIntraFrame )
	{
		//
		// �����t���[���̔���
		//
		int		iDstIndex ;
		DWORD	dwDecMask = 0xFFFFFFFF ;
		if ( m_fTopDown )
		{
			fdwFlags = ERISADecoder::dfTopDown ;
		}
		if ( nFrameType == ftBidirectionalFrame )
		{
			fdwFlags |= ERISADecoder::dfDifferential ;
			m_perid->SetRefPreviousFrame
				( m_pDstImage[(m_iDstBufIndex^0x01)],
								m_pDstImage[m_iDstBufIndex] ) ;
			iDstIndex = 2 ;
		}
		else
		{
			if ( nFrameType == ftPredictionalFrame )
			{
				fdwFlags |= ERISADecoder::dfDifferential ;
				m_perid->SetRefPreviousFrame( m_pDstImage[m_iDstBufIndex] ) ;
			}
			else
			{
				fdwFlags &= ~ERISADecoder::dfDifferential ;
				m_fdwDecFlags &= ~ERISADecoder::dfDifferential ;
				dwDecMask = ~ERISADecoder::dfQuickDecode ;
			}
			m_iDstBufIndex ^= 0x01 ;
			iDstIndex = m_iDstBufIndex ;
		}
		//
		// �W�J���s
		//
		int	iDstFiltered = (iDstIndex == 2) ? 2 : iDstIndex + 3 ;
		fdwFlags |= m_fdwDecFlags ;
		m_fdwDecFlags &= dwDecMask ;
		pFrame->Seek( 0, ESLFileObject::FromBegin ) ;
		m_peric->AttachInputFile( pFrame ) ;
		m_perid->SetFilteredImageBuffer( m_pDstImage[iDstFiltered] ) ;
		errResult = m_perid->DecodeImage
			( *(m_pDstImage[iDstIndex]), *m_peric, fdwFlags ) ;
		//
		if ( m_perid->GetFilteredImageBuffer() != NULL )
		{
			m_iDstFrameIndex[iDstFiltered] = pFrame->m_iFrameIndex ;
		}
		m_iDstFrameIndex[iDstIndex] = pFrame->m_iFrameIndex ;
	}
	else
	{
		ApplyPaletteTable( pFrame ) ;
	}
	return	errResult ;
}

//
// �p���b�g�e�[�u����K�p����
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::ApplyPaletteTable( EPreloadBuffer * pBuffer )
{
	if ( pBuffer != NULL )
	{
		UINT64	recid = pBuffer->m_ui64RecType ;
		if ( recid == *((UINT64*)"Palette ") )
		{
			unsigned int	nBytes = pBuffer->GetLength( ) ;
			if ( m_pDstImage[0]->pPaletteEntries != NULL )
			{
				unsigned int	nPaletteLimit =
					sizeof(EGL_PALETTE) * m_pDstImage[0]->dwPaletteCount ;
				if ( nBytes > nPaletteLimit )
					nBytes = nPaletteLimit ;
				pBuffer->Read( m_pDstImage[0]->pPaletteEntries, nBytes ) ;
				//
				for ( int i = 1; i < 5; i ++ )
				{
					if ( (m_pDstImage[i] != NULL)
						&& (m_pDstImage[i]->pPaletteEntries != NULL) )
					{
						::eslMoveMemory
							( m_pDstImage[i]->pPaletteEntries,
								m_pDstImage[0]->pPaletteEntries, nBytes ) ;
					}
				}
			}
		}
	}
}

//
// ��ǂ݃o�b�t�@���擾����
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EPreloadBuffer * ERIAnimation::GetPreloadBuffer( void )
{
	EPreloadBuffer *	pBuffer = NULL ;
	Lock( ) ;
	if ( m_hThread == NULL )
	{
		//
		// ��ǂ݃o�b�t�@�Ƀt���[����ǂݍ���
		//
		if ( m_queueImage.GetSize() < m_nPreloadLimit )
		{
			EPreloadBuffer *	pPreload =
				LoadMovieStream( m_iPreloadFrame ) ;
			if ( pPreload != NULL )
			{
				AddPreloadBuffer( pPreload ) ;
			}
		}
	}
	if ( m_nCacheBFrames != 0 )
	{
		//
		// B �t���[���L���b�V��������ꍇ�ɂ�
		// �i���͂��̕K�v���Ȃ��ꍇ�ɂ́j
		// �f���ɒ���̐�ǂ݃f�[�^��Ԃ�
		//
		if ( m_queueImage.GetSize() != 0 )
		{
			pBuffer = m_queueImage.GetAt( 0 ) ;
			m_queueImage.DetachAt( 0 ) ;
			m_nCacheBFrames -= (m_nCacheBFrames != (unsigned int) -1) ;
		}
	}
	else
	{
		//
		// B �t���[���L���b�V�����Ȃ��ꍇ�ɂ�
		// �iB �t���[�����L���b�V������K�v������ꍇ�ɂ́j
		// ���� I, ���� P �t���[����Ԃ�
		//
		for ( ; ; )
		{
			//
			// B �t���[���ȊO��T��
			//
			unsigned int	i ;
			for ( i = 0; i < m_queueImage.GetSize(); i ++ )
			{
				int	nFrameType =
					GetFrameBufferType( m_queueImage.GetAt( i ) ) ;
				if ( nFrameType != ftBidirectionalFrame )
				{
					pBuffer = m_queueImage.GetAt( i ) ;
					break ;
				}
			}
			if ( pBuffer != NULL )
			{
				ESLAssert( pBuffer == m_queueImage.GetAt( i ) ) ;
				m_queueImage.DetachAt( i ) ;
				m_nCacheBFrames = i ;
				break ;
			}
			//
			// B �t���[������������Ȃ������ꍇ�ɂ�
			// ��ǂ݃o�b�t�@�Ɍ�����܂œǂݍ���
			//
			if ( (m_hThread == NULL)
				|| (::GetCurrentThreadId() == m_idThread) )
			{
				EPreloadBuffer *	pPreload =
					LoadMovieStream( m_iPreloadFrame ) ;
				if ( pPreload != NULL )
				{
					AddPreloadBuffer( pPreload ) ;
				}
				else
				{
					break ;
				}
			}
			else
			{
				::SetEvent( m_hQueueSpace ) ;
				Unlock( ) ;
				if ( ::WaitForSingleObject( m_hThread, 0 ) == WAIT_OBJECT_0 )
				{
					return	NULL ;
				}
				Lock( ) ;
			}
		}
	}
	//
	// �o�b�t�@���ǂ݂���
	//
	if ( m_queueImage.GetSize() == 0 )
	{
		::ResetEvent( m_hQueueNotEmpty ) ;
	}
	if ( m_queueImage.GetSize() < m_nPreloadLimit )
	{
		::SetEvent( m_hQueueSpace ) ;
		//
		if ( m_hThread == NULL )
		{
			EPreloadBuffer *	pPreload =
				LoadMovieStream( m_iPreloadFrame ) ;
			if ( pPreload != NULL )
			{
				AddPreloadBuffer( pPreload ) ;
			}
		}
	}
	Unlock( ) ;
	return	pBuffer ;
}

//
// ��ǂ݃o�b�t�@�ɒǉ�����
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::AddPreloadBuffer( EPreloadBuffer * pBuffer )
{
	Lock( ) ;
	m_queueImage.Add( pBuffer ) ;
	if ( m_queueImage.GetSize() != 0 )
	{
		::SetEvent( m_hQueueNotEmpty ) ;
	}
	if ( m_queueImage.GetSize() >= m_nPreloadLimit )
	{
		::ResetEvent( m_hQueueSpace ) ;
	}
	Unlock( ) ;
}

//
// �w��̃t���[���� I, P, B �s�N�`�������肷��
//////////////////////////////////////////////////////////////////////////////
int ERIAnimation::GetFrameBufferType( EPreloadBuffer * pBuffer )
{
	if ( pBuffer == NULL )
	{
		return	ftOtherData ;
	}
	UINT64	recid = pBuffer->m_ui64RecType ;
	if ( recid == *((UINT64*)"ImageFrm") )
	{
		return	ftIntraFrame ;					// I �s�N�`��
	}
	if ( recid == *((UINT64*)"DiffeFrm") )
	{
		if ( m_erif.m_InfoHeader.fdwTransformation == CVTYPE_LOSSLESS_ERI )
		{
			return	ftPredictionalFrame ;		// P �s�N�`��
		}
		if ( pBuffer->m_ptrBuffer[1] & 0x02 )
		{
			return	ftBidirectionalFrame ;		// B �s�N�`��
		}
		return	ftPredictionalFrame ;
	}
	return	ftOtherData ;						// ���̑��i�p���b�g���j
}

//
// ERIFile �I�u�W�F�N�g���擾����
//////////////////////////////////////////////////////////////////////////////
const ERIFile & ERIAnimation::GetERIFile( void ) const
{
	return	m_erif ;
}

//
// �J�����g�t���[���̃C���f�b�N�X���擾����
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::CurrentIndex( void ) const
{
	return	m_iCurrentFrame ;
}

//
// �J�����g�t���[���̉摜���擾
//////////////////////////////////////////////////////////////////////////////
const EGL_IMAGE_INFO * ERIAnimation::GetImageInfo( void ) const
{
	int	i = 4 ;
	do
	{
		if ( m_iDstFrameIndex[i] == m_iCurrentFrame )
		{
			return	m_pDstImage[i] ;
		}
	}
	while ( -- i ) ;
	return	m_pDstImage[0] ;
}

//
// �p���b�g�e�[�u���擾
//////////////////////////////////////////////////////////////////////////////
const EGL_PALETTE * ERIAnimation::GetPaletteEntries( void ) const
{
	if ( m_pDstImage[0] == NULL )
	{
		return	NULL ;
	}
	return	m_pDstImage[m_iDstBufIndex]->pPaletteEntries ;
}

//
// �L�[�t���[�����擾
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::GetKeyFrameCount( void ) const
{
	return	m_erif.m_FileHeader.dwKeyFrameCount ;
}

//
// �S�t���[�������擾
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::GetAllFrameCount( void ) const
{
	return	m_erif.m_FileHeader.dwFrameCount ;
}

//
// �S�A�j���[�V�������Ԃ��擾
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::GetTotalTime( void ) const
{
	return	m_erif.m_FileHeader.dwAllFrameTime ;
}

//
// �t���[���ԍ����玞�Ԃ֕ϊ�
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::FrameIndexToTime( unsigned int iFrameIndex ) const
{
	if ( m_erif.m_FileHeader.dwFrameCount == 0 )
		return	0 ;
	return	(unsigned int) ((UINT64) iFrameIndex
							* m_erif.m_FileHeader.dwAllFrameTime
							/ m_erif.m_FileHeader.dwFrameCount) ;
}

//
// ���Ԃ���t���[���ԍ��֕ϊ�
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::TimeToFrameIndex( unsigned int nMilliSec ) const
{
	if ( m_erif.m_FileHeader.dwAllFrameTime == 0 )
		return	0 ;
	return	(unsigned int) ((UINT64) nMilliSec
							* m_erif.m_FileHeader.dwFrameCount
							/ m_erif.m_FileHeader.dwAllFrameTime) ;
}

//
// �X���b�h�֐�
//////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ERIAnimation::ThreadProc( LPVOID parameter )
{
	return	((ERIAnimation*)parameter)->LoadingThreadProc( ) ;
}

DWORD ERIAnimation::LoadingThreadProc( void )
{
	//
	// ���b�Z�[�W�L���[���쐬
	//////////////////////////////////////////////////////////////////////////
	MSG		msg ;
	::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ;
	::SetEvent( m_hQueueSpace ) ;
	//
	// �ϐ�������
	//////////////////////////////////////////////////////////////////////////
	unsigned int	iCurrentFrame = 0 ;
	m_nPreloadWaveSamples = 0 ;
	EKeyPoint	keypoint ;
	keypoint.m_iKeyFrame = 0 ;
	keypoint.m_dwSubSample = 0 ;
	keypoint.m_dwRecOffset = m_erif.GetPosition( ) ;
	AddKeyPoint( m_listKeyFrame, keypoint ) ;
	//
	// �e���R�[�h��ǂݍ���
	//////////////////////////////////////////////////////////////////////////
	for ( ; ; )
	{
		//
		// ���R�[�h���J��
		//////////////////////////////////////////////////////////////////////
		unsigned int	iLastFrame = iCurrentFrame ;
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		EPreloadBuffer *	pBuffer = LoadMovieStream( iCurrentFrame ) ;
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
					( 1, &m_hQueueSpace, FALSE, 5, QS_ALLINPUT ) ;
			if ( (pBuffer != NULL) && (dwWaitResult == WAIT_OBJECT_0) )
			{
				AddPreloadBuffer( pBuffer ) ;
				pBuffer = NULL ;
			}
			//
			if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				if ( msg.message == tmSeekFrame )
				{
					//
					// �L�[�t���[���ɃV�[�N
					//
					if ( pBuffer != NULL )
					{
						m_erif.Seek
							( dwRecPosition, ESLFileObject::FromBegin ) ;
						delete	pBuffer ;
						iCurrentFrame = iLastFrame ;
						pBuffer = NULL ;
					}
					SeekKeyPoint
						( m_listKeyFrame, msg.wParam, iCurrentFrame ) ;
					//
					::SetEvent( (HANDLE) msg.lParam ) ;
				}
				else if ( msg.message == tmSeekSound )
				{
					//
					// �����f�[�^���V�[�N
					//
					SeekKeyWave( m_listKeyWave, msg.wParam ) ;
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
// ���摜�X�g���[����ǂݍ���
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EPreloadBuffer *
	ERIAnimation::LoadMovieStream( unsigned int & iCurrentFrame )
{
	EKeyPoint	keypoint ;
	for ( ; ; )
	{
		//
		// ���R�[�h���J��
		//////////////////////////////////////////////////////////////////////
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		if ( m_erif.DescendRecord( ) )
		{
//			if ( iCurrentFrame == 0 )
			{
				// 1���摜���R�[�h�������ꍇ�̓G���[
				return	NULL ;
			}
			//
			// ���R�[�h�̏I�[�ɓ��B������
			// �����I�ɐ擪�Ɉړ�
			iCurrentFrame = 0 ;
			m_nPreloadWaveSamples = 0 ;
			m_erif.Seek( 0, ESLFileObject::FromBegin ) ;
			continue ;
		}
		//
		// ���R�[�h�̎�ނ𔻕�
		//////////////////////////////////////////////////////////////////////
		UINT64	recid = m_erif.GetRecordID( ) ;
		EPreloadBuffer *	pBuffer = NULL ;
		if ( (recid == *((UINT64*)"ImageFrm")) ||
				(recid == *((UINT64*)"DiffeFrm")) )
		{
			//
			// �摜�f�[�^���R�[�h
			//
			DWORD	dwDataBytes = m_erif.GetLength( ) ;
			pBuffer = new EPreloadBuffer( dwDataBytes ) ;
			pBuffer->m_iFrameIndex = iCurrentFrame ;
			pBuffer->m_ui64RecType = recid ;
			m_erif.Read( pBuffer->m_ptrBuffer, dwDataBytes ) ;
			//
			m_erif.AscendRecord( ) ;
			//
			++ iCurrentFrame ;
			if ( IsKeyFrame( iCurrentFrame ) )
			{
				EKeyPoint *	pKeyPoint =
					SearchKeyPoint( m_listKeyFrame, iCurrentFrame ) ;
				if ( pKeyPoint == NULL )
				{
					keypoint.m_iKeyFrame = iCurrentFrame ;
					keypoint.m_dwSubSample = m_nPreloadWaveSamples ;
					keypoint.m_dwRecOffset = m_erif.GetPosition( ) ;
					AddKeyPoint( m_listKeyFrame, keypoint ) ;
				}
			}
			return	pBuffer ;
		}
		else if ( recid == *((UINT64*)"Palette ") )
		{
			//
			// �p���b�g�e�[�u�����R�[�h
			//
			DWORD	dwDataBytes = m_erif.GetLength( ) ;
			pBuffer = new EPreloadBuffer( dwDataBytes ) ;
			pBuffer->m_iFrameIndex = iCurrentFrame ;
			pBuffer->m_ui64RecType = recid ;
			m_erif.Read( pBuffer->m_ptrBuffer, dwDataBytes ) ;
			m_erif.AscendRecord( ) ;
			return	pBuffer ;
		}
		else if ( recid == *((UINT64*)"SoundStm") )
		{
			//
			// �����X�g���[�����R�[�h
			//
			MIO_DATA_HEADER	miodh ;
			m_erif.Read( &miodh, sizeof(MIO_DATA_HEADER) ) ;
			if ( miodh.bytFlags & MIO_LEAD_BLOCK )
			{
				EKeyPoint *	pKeyPoint = SearchKeyPoint
					( m_listKeyWave, m_nPreloadWaveSamples/*iCurrentFrame*/ ) ;
				if ( pKeyPoint == NULL )
				{
					keypoint.m_iKeyFrame = m_nPreloadWaveSamples ;//iCurrentFrame ;
					keypoint.m_dwSubSample = iCurrentFrame ;
					keypoint.m_dwRecOffset = dwRecPosition ;
					AddKeyPoint( m_listKeyWave, keypoint ) ;
				}
			}
			m_nPreloadWaveSamples += miodh.dwSampleCount ;
			//
			if ( m_fWaveStreaming && m_fWaveOutput )
			{
				//
				// �����X�g���[�~���O���[�h
				//		�� �f�R�[�h���ďo��
				//
				DWORD	dwBytes = miodh.dwSampleCount
							* m_erif.m_MIOInfHdr.dwChannelCount
							* (m_erif.m_MIOInfHdr.dwBitsPerSample / 8) ;
				void *	ptrWaveBuf = AllocateWaveBuffer( dwBytes ) ;
				if ( ptrWaveBuf != NULL )
				{
					if ( m_pmiod->DecodeSound
						( *m_pmioc, miodh, ptrWaveBuf ) == eslErrSuccess )
					{
						PushWaveBuffer( ptrWaveBuf, dwBytes ) ;
					}
					else
					{
						DeleteWaveBuffer( ptrWaveBuf ) ;
					}
				}
			}
			//
			m_erif.AscendRecord( ) ;
		}
		else
		{
			//
			// ���R�[�h�����
			//////////////////////////////////////////////////////////////////////
			m_erif.AscendRecord( ) ;
		}
	}
}

//
// �L�[�t���[���|�C���g��ǉ�����
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::AddKeyPoint
	( ERIAnimation::EKeyPointList & list, const ERIAnimation::EKeyPoint & key )
{
	Lock( ) ;
	list.Add( new EKeyPoint( key ) ) ;
	Unlock( ) ;
}

//
// �w��̃L�[�t���[������������
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EKeyPoint * ERIAnimation::SearchKeyPoint
	( ERIAnimation::EKeyPointList & list, unsigned int iKeyFrame )
{
	int		iFirst, iMiddle, iEnd ;
	EKeyPoint *	pFoundKey ;
	//
	Lock( ) ;
	//
	iFirst = 0 ;
	iEnd = list.GetSize() - 1 ;
	//
	for ( ; ; )
	{
		if ( iFirst > iEnd )
		{
			pFoundKey = NULL ;
			break ;
		}
		//
		iMiddle = (iFirst + iEnd) / 2 ;
		pFoundKey = list.GetAt( iMiddle ) ;
		//
		if ( pFoundKey->m_iKeyFrame == iKeyFrame )
		{
			break ;
		}
		if ( pFoundKey->m_iKeyFrame > iKeyFrame )
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
// �w��̃t���[���ɃV�[�N����
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::SeekKeyPoint
	( ERIAnimation::EKeyPointList & list,
		unsigned int iFrame, unsigned int & iCurrentFrame )
{
	//
	// ��ǂ݃L���[�ɓǂݍ��܂�Ă��邩���f
	//
	bool	fHaveSeeked = false ;
	EPreloadBuffer *	pBuffer ;
	Lock( ) ;
	for ( ; ; )
	{
		if ( m_queueImage.GetSize() == 0 )
		{
			::ResetEvent( m_hQueueNotEmpty ) ;
			break ;
		}
		pBuffer = m_queueImage.GetAt( 0 ) ;
		if ( (pBuffer != NULL)
				&& (pBuffer->m_iFrameIndex == iFrame) )
		{
			if ( GetFrameBufferType( pBuffer ) == ftIntraFrame )
			{
				fHaveSeeked = true ;
				break ;
			}
			else
			{
				ApplyPaletteTable( pBuffer ) ;
			}
		}
		m_queueImage.RemoveAt( 0 ) ;
	}
	Unlock( ) ;
	//
	if ( !fHaveSeeked )
	{
		//
		// ���X�g�Ɏw��t���[���̃|�C���^��
		// �o�^����Ă��邩�ǂ����𒲂ׂ�
		//
		EKeyPoint *	pKeyPoint = SearchKeyPoint( m_listKeyFrame, iFrame ) ;
		//
		if ( pKeyPoint != NULL )
		{
			//
			// �|�C���^�ɃV�[�N
			//
			m_erif.Seek
				( pKeyPoint->m_dwRecOffset, ESLFileObject::FromBegin ) ;
			iCurrentFrame = pKeyPoint->m_iKeyFrame ;//iFrame ;
			m_nPreloadWaveSamples = pKeyPoint->m_dwSubSample ;
		}
		else
		{
			//
			// �w��̃t���[����T��
			//
			do
			{
				//
				// ���̃��R�[�h���J��
				//
				DWORD	dwRecPosition = m_erif.GetPosition( ) ;
				if ( m_erif.DescendRecord( ) )
				{
					iCurrentFrame = 0 ;
					m_erif.Seek( 0, ESLFileObject::FromBegin ) ;
					break ;
				}
				//
				// ���R�[�h�̎�ނ𔻕�
				//
				UINT64	recid = m_erif.GetRecordID( ) ;
				if ( (recid == *((UINT64*)"ImageFrm")) ||
						(recid == *((UINT64*)"DiffeFrm")) )
				{
					//
					// �摜�f�[�^���R�[�h
					//
					m_erif.AscendRecord( ) ;
					++ iCurrentFrame ;
					if ( IsKeyFrame( iCurrentFrame ) )
					{
						EKeyPoint *	pKeyPoint =
							SearchKeyPoint( m_listKeyFrame, iCurrentFrame ) ;
						if ( pKeyPoint == NULL )
						{
							EKeyPoint	keypoint ;
							keypoint.m_iKeyFrame = iCurrentFrame ;
							keypoint.m_dwSubSample = m_nPreloadWaveSamples ;
							keypoint.m_dwRecOffset = m_erif.GetPosition( ) ;
							AddKeyPoint( m_listKeyFrame, keypoint ) ;
						}
					}
					if ( iCurrentFrame == iFrame )
					{
						fHaveSeeked = true ;
					}
				}
				else if ( recid == *((UINT64*)"SoundStm") )
				{
					//
					// �����X�g���[�����R�[�h
					//
					MIO_DATA_HEADER	miodh ;
					m_erif.Read( &miodh, sizeof(MIO_DATA_HEADER) ) ;
					if ( miodh.bytFlags & MIO_LEAD_BLOCK )
					{
						EKeyPoint *	pKeyPoint = SearchKeyPoint
							( m_listKeyWave, m_nPreloadWaveSamples/*iCurrentFrame*/ ) ;
						if ( pKeyPoint == NULL )
						{
							EKeyPoint	keypoint ;
							keypoint.m_iKeyFrame = m_nPreloadWaveSamples ;//iCurrentFrame ;
							keypoint.m_dwSubSample = iCurrentFrame ;
							keypoint.m_dwRecOffset = dwRecPosition ;
							AddKeyPoint( m_listKeyWave, keypoint ) ;
						}
					}
					m_nPreloadWaveSamples += miodh.dwSampleCount ;
					m_erif.AscendRecord( ) ;
				}
				else
				{
					m_erif.AscendRecord( ) ;
				}
			}
			while ( !fHaveSeeked ) ;
		}
	}
	//
	// B �s�N�`���ɑΉ������t�H�[�}�b�g�ł�
	// �擪�� I �s�N�`���͂��炩���ߓW�J���Ă���
	//
	if ( m_nCacheBFrames != (unsigned int) -1 )
	{
		m_nCacheBFrames = 0 ;
		pBuffer = GetPreloadBuffer( ) ;
		DecodeFrame( pBuffer ) ;
		delete	pBuffer ;
		ESLAssert( m_nCacheBFrames == 0 ) ;
	}
}

//
// �w��̉����f�[�^�܂ŃV�[�N���ăX�g���[�~���O�o�͂���
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::SeekKeyWave
	( EKeyPointList & list, unsigned int iFrame )
{
	if ( !m_fWaveOutput || !m_fWaveStreaming )
	{
		return ;
	}
	//
	// �t���[���ԍ��������T���v�����֕ϊ�
	//
	DWORD	dwSeekTime = FrameIndexToTime( iFrame ) ;
	DWORD	dwWaveSamples =
		(DWORD) ((__int64) dwSeekTime
			* m_erif.m_MIOInfHdr.dwSamplesPerSec / 1000) ;
	//
	// �V�[�N�|�C���g����ԂŁA�ł��߂��L�[�|�C���g����������
	//
	int		iFirst, iMiddle, iEnd ;
	EKeyPoint *	pKeyPoint ;
	Lock( ) ;
	iFirst = 0 ;
	iEnd = list.GetSize() - 1 ;
	for ( ; ; )
	{
		if ( iFirst > iEnd )
		{
			pKeyPoint = list.GetAt( iEnd ) ;
			break ;
		}
		//
		iMiddle = (iFirst + iEnd) / 2 ;
		pKeyPoint = list.GetAt( iMiddle ) ;
		//
		if ( pKeyPoint->m_iKeyFrame == dwWaveSamples )
		{
			break ;
		}
		if ( pKeyPoint->m_iKeyFrame > dwWaveSamples )
		{
			iEnd = iMiddle - 1 ;
		}
		else
		{
			iFirst = iMiddle + 1 ;
		}
	}
	Unlock( ) ;
	//
	// �t�@�C���|�C���^���ړ�
	//
	DWORD	dwOriginalPointer = m_erif.GetPosition( ) ;
	m_nPreloadWaveSamples = pKeyPoint->m_iKeyFrame ;	
	unsigned int	iCurrentFrame = pKeyPoint->m_dwSubSample ;
	m_erif.Seek( pKeyPoint->m_dwRecOffset, ESLFileObject::FromBegin ) ;
	//
	// �w��̏ꏊ��T��
	//
	do
	{
		//
		// ���̃��R�[�h���J��
		//
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		if ( m_erif.DescendRecord( ) )
		{
			m_erif.Seek( 0, ESLFileObject::FromBegin ) ;
			break ;
		}
		//
		// ���R�[�h�̎�ނ𔻕�
		//
		UINT64	recid = m_erif.GetRecordID( ) ;
		if ( (recid == *((UINT64*)"ImageFrm")) ||
				(recid == *((UINT64*)"DiffeFrm")) )
		{
			//
			// �摜�f�[�^���R�[�h
			//
			m_erif.AscendRecord( ) ;
			++ iCurrentFrame ;
			if ( IsKeyFrame( iCurrentFrame ) )
			{
				EKeyPoint *	pKeyPoint =
					SearchKeyPoint( m_listKeyFrame, iCurrentFrame ) ;
				if ( pKeyPoint == NULL )
				{
					EKeyPoint	keypoint ;
					keypoint.m_iKeyFrame = iCurrentFrame ;
					keypoint.m_dwSubSample = m_nPreloadWaveSamples ;
					keypoint.m_dwRecOffset = m_erif.GetPosition( ) ;
					AddKeyPoint( m_listKeyFrame, keypoint ) ;
				}
			}
		}
		else if ( recid == *((UINT64*)"SoundStm") )
		{
			//
			// �����X�g���[�����R�[�h
			//
			MIO_DATA_HEADER	miodh ;
			m_erif.Read( &miodh, sizeof(MIO_DATA_HEADER) ) ;
			if ( miodh.bytFlags & MIO_LEAD_BLOCK )
			{
				EKeyPoint *	pKeyPoint = SearchKeyPoint
					( m_listKeyWave, m_nPreloadWaveSamples/*iCurrentFrame*/ ) ;
				if ( pKeyPoint == NULL )
				{
					EKeyPoint	keypoint ;
					keypoint.m_iKeyFrame = iCurrentFrame ;
					keypoint.m_dwSubSample = m_nPreloadWaveSamples ;
					keypoint.m_dwRecOffset = dwRecPosition ;
					AddKeyPoint( m_listKeyWave, keypoint ) ;
				}
			}
			//
			// �����X�g���[�~���O���[�h
			//		�� �f�R�[�h���ďo��
			//
			DWORD	dwBytes = miodh.dwSampleCount
						* m_erif.m_MIOInfHdr.dwChannelCount
						* (m_erif.m_MIOInfHdr.dwBitsPerSample / 8) ;
			void *	ptrWaveBuf = AllocateWaveBuffer( dwBytes ) ;
			if ( ptrWaveBuf != NULL )
			{
				if ( m_pmiod->DecodeSound
					( *m_pmioc, miodh, ptrWaveBuf ) == eslErrSuccess )
				{
//					if ( iCurrentFrame >= iFrame )
					if ( m_nPreloadWaveSamples
							+ miodh.dwSampleCount > dwWaveSamples )
					{
						long int	nOffsetSamples
									= dwWaveSamples - m_nPreloadWaveSamples ;
						if ( nOffsetSamples <= 0 )
						{
							PushWaveBuffer( ptrWaveBuf, dwBytes ) ;
						}
						else
						{
							DWORD	dwBlockAlign =
								m_erif.m_MIOInfHdr.dwChannelCount
									* m_erif.m_MIOInfHdr.dwBitsPerSample / 8 ;
							DWORD	dwOffsetBytes =
										nOffsetSamples * dwBlockAlign ;
							DWORD	dwSubBytes = dwBytes - dwOffsetBytes ;
							void *	ptrSubBuf = AllocateWaveBuffer( dwSubBytes ) ;
							::eslMoveMemory
								( ptrSubBuf, ((BYTE*)ptrWaveBuf)
												+ dwOffsetBytes, dwSubBytes ) ;
							DeleteWaveBuffer( ptrWaveBuf ) ;
							PushWaveBuffer( ptrSubBuf, dwSubBytes ) ;
						}
					}
					else
					{
						DeleteWaveBuffer( ptrWaveBuf ) ;
					}
				}
				else
				{
					DeleteWaveBuffer( ptrWaveBuf ) ;
				}
			}
			m_nPreloadWaveSamples += miodh.dwSampleCount ;
			//
			m_erif.AscendRecord( ) ;
		}
		else
		{
			m_erif.AscendRecord( ) ;
		}
	}
	while ( m_erif.GetPosition() < dwOriginalPointer ) ;
}

//
// �r�������i�N���e�B�J���Z�N�V�����j
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::Lock( void )
{
	::EnterCriticalSection( &m_cs ) ;
}

void ERIAnimation::Unlock( void )
{
	::LeaveCriticalSection( &m_cs ) ;
}


#endif
