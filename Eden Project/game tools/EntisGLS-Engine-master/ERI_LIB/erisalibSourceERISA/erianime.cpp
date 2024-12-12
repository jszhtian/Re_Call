
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
                    ERIアニメーションファイルオブジェクト
 *****************************************************************************/

IMPLEMENT_CLASS_INFO( ERIAnimation, ESLObject )

//
// EPreloadBuffer 構築関数
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
// EPreloadBuffer 消滅関数
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EPreloadBuffer::~EPreloadBuffer( void )
{
	::eslHeapFree( NULL, m_ptrBuffer ) ;
}

//
// 構築関数
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
// 消滅関数
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::~ERIAnimation( void )
{
	Close( ) ;
	::DeleteCriticalSection( &m_cs ) ;
}

//
// 画像展開出力バッファ要求
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
// 画像展開出力バッファ消去
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::DeleteImageBuffer( EGL_IMAGE_INFO * peii )
{
	if ( peii != NULL )
	{
		::eslHeapFree( NULL, peii ) ;
	}
}

//
// 画像展開オブジェクト生成
//////////////////////////////////////////////////////////////////////////////
ERISADecoder * ERIAnimation::CreateERIDecoder( void )
{
	return	new ERISADecoder ;
}

//
// 音声展開オブジェクト生成
//////////////////////////////////////////////////////////////////////////////
MIODecoder * ERIAnimation::CreateMIODecoder( void )
{
	return	new MIODecoder ;
}

//
// 音声出力要求
//////////////////////////////////////////////////////////////////////////////
bool ERIAnimation::RequestWaveOut
	( DWORD channels, DWORD frequency, DWORD bps )
{
	return	false ;
}

//
// 音声出力終了
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::CloseWaveOut( void )
{
}

//
// 音声データ出力
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::PushWaveBuffer( void * ptrWaveBuf, DWORD dwBytes )
{
	DeleteWaveBuffer( ptrWaveBuf ) ;
}

//
// 音声バッファ確保
//////////////////////////////////////////////////////////////////////////////
void * ERIAnimation::AllocateWaveBuffer( DWORD dwBytes )
{
	return	::eslHeapAllocate( NULL, dwBytes, 0 ) ;
}

//
// 音声データ破棄許可
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::DeleteWaveBuffer( void * ptrWaveBuf )
{
	::eslHeapFree( NULL, ptrWaveBuf ) ;
}

//
// 音声ストリーミング開始
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
// 音声ストリーミング終了
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::EndWaveStreaming( void )
{
	m_fWaveStreaming = false ;
}

//
// アニメーションファイルを開く
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::Open
	( ESLFileObject * pFile, unsigned int nPreloadSize, DWORD fdwFlags )
{
	Close( ) ;
	//
	// ERIファイルを開く（ストリームレコードまで開く）
	//////////////////////////////////////////////////////////////////////////
	if ( m_erif.Open( pFile, m_erif.otOpenStream ) )
	{
		ESLTrace( "[ERIAnimation::Open] "
				"ERI ファイルヘッダを開けませんでした。\n" ) ;
		return	ESLErrorMsg( "ファイルヘッダを開けませんでした。" ) ;
	}
	//
	// 展開オブジェクトを初期化する
	//////////////////////////////////////////////////////////////////////////
	m_perid = CreateERIDecoder( ) ;
	if ( m_perid->Initialize( m_erif.m_InfoHeader ) )
	{
		ESLTrace( "[ERIAnimation::Open] デコーダーの初期化に失敗しました。\n" ) ;
		return	ESLErrorMsg( "デコーダーの初期化に失敗しました。" ) ;
	}
	m_peric = new ERISADecodeContext( 0x10000 ) ;
	//
	m_fWaveOutput = false ;
	if ( m_erif.m_fdwReadMask & ERIFile::rmSoundInfo )
	{
		//
		// サウンドが含まれている
		m_pmiod = CreateMIODecoder( ) ;
		if ( m_pmiod->Initialize( m_erif.m_MIOInfHdr ) == eslErrSuccess )
		{
			m_pmioc = new ERISADecodeContext( 0x10000 ) ;
			m_pmioc->AttachInputFile( &m_erif ) ;
			//
			// 音声出力要求
			if ( RequestWaveOut
				( m_erif.m_MIOInfHdr.dwChannelCount,
					m_erif.m_MIOInfHdr.dwSamplesPerSec,
					m_erif.m_MIOInfHdr.dwBitsPerSample ) )
			{
				//
				// 音声出力要求が受け入れられた
				m_fWaveOutput = true ;
			}
		}
	}
	//
	// 画像バッファを生成
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
	// 先読みバッファ配列を確保
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
	// フレームシーク用キーポイント配列確保
	//////////////////////////////////////////////////////////////////////////
	m_listKeyFrame.SetLimit( m_erif.m_FileHeader.dwFrameCount ) ;
	m_listKeyWave.SetLimit( m_erif.m_FileHeader.dwFrameCount ) ;
	//
	// スレッドを生成
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
		// スレッドを生成しないで先読みする
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
	// 第1フレームを展開する
	//////////////////////////////////////////////////////////////////////////
	if ( SeekToBegin( ) )
	{
		return	eslErrGeneral ;
	}

	return	eslErrSuccess ;
}

//
// アニメーションファイルを閉じる
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::Close( void )
{
	//
	// スレッドを終了させる
	if ( m_hThread != NULL )
	{
		::PostThreadMessage( m_idThread, tmQuit, 0, 0 ) ;
		::WaitForSingleObject( m_hThread, INFINITE ) ;
		::CloseHandle( m_hThread ) ;
		m_hThread = NULL ;
	}
	//
	// 先読みキューをクリアする
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
	// キーフレームポイント配列をクリアする
	m_listKeyFrame.RemoveAll( ) ;
	m_listKeyWave.RemoveAll( ) ;
	//
	// 画像バッファを削除する
	for ( int i = 0; i < 5; i ++ )
	{
		if ( m_pDstImage[i] != NULL )
		{
			DeleteImageBuffer( m_pDstImage[i] ) ;
			m_pDstImage[i] = NULL ;
		}
	}
	//
	// 展開オブジェクトを削除する
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
	// 音声出力の終了
	if ( m_fWaveOutput )
	{
		CloseWaveOut( ) ;
		m_fWaveOutput = false ;
	}
	//
	// ファイルを閉じる
	m_erif.Close( ) ;
}

//
// 先頭フレームへ移動
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::SeekToBegin( void )
{
	//
	// スレッドにシークメッセージを送る
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
	// 先頭フレームを展開
	//////////////////////////////////////////////////////////////////////////
	if ( SeekToNextFrame( ) )
	{
		return	eslErrGeneral ;
	}

	return	eslErrSuccess ;
}

//
// 次のフレームへ移動
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::SeekToNextFrame( int nSkipFrame )
{
	//
	// スキップする分だけ読み飛ばす
	//////////////////////////////////////////////////////////////////////////
	EObjArray<EPreloadBuffer>	queSkipFrames ;
	EPreloadBuffer *	pBuf ;
	int		i ;
	for ( i = 0; i < nSkipFrame; i ++ )
	{
		//
		// スキップするフレームを取り出す
		// ※ GetPreloadBuffer 関数は展開すべき順番に
		//    再配列してフレームを返すことに注意！
		//
		pBuf = GetPreloadBuffer( ) ;
		if ( pBuf != NULL )
		{
			int	nFrameType = GetFrameBufferType( pBuf ) ;
			if ( (nFrameType == ftIntraFrame)
					|| (nFrameType == ftPredictionalFrame) )
			{
				// I, P ピクチャは一旦確保しておく
				queSkipFrames.Add( pBuf ) ;
			}
			else if ( nFrameType == ftBidirectionalFrame )
			{
				// B ピクチャは破棄しても構わない
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
	// 展開するべきターゲットフレームを取得する
	//////////////////////////////////////////////////////////////////////////
	EPreloadBuffer *	pNextFrame ;
	for ( ; ; )
	{
		pNextFrame = GetPreloadBuffer( ) ;
		if ( pNextFrame == NULL )
		{
			// ファイル終端処理
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
	// I ピクチャを挟んでいる場合にはそこまでのデータを破棄する
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
	// 指定フレームまで順次展開
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
// 指定のフレームに移動
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::SeekToFrame( unsigned int iFrameIndex )
{
	//
	// 特殊条件
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
	// ストリーミング停止
	//////////////////////////////////////////////////////////////////////////
	if ( m_fWaveStreaming )
	{
		EndWaveStreaming( ) ;
		m_fWaveStreaming = false ;
	}
	//
	// スレッドにシークメッセージを送る
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
	// キーフレームを展開
	//////////////////////////////////////////////////////////////////////////
	if ( SeekToNextFrame( ) )
	{
		return	eslErrGeneral ;
	}
	//
	// 差分フレームを展開
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
// 指定のフレームはキーフレームか？
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
// 最適なフレームスキップ数を取得する
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
		// B フレームを考慮する
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
	// I フレームを検索する
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
// フレームを展開する
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimation::DecodeFrame( EPreloadBuffer * pFrame, DWORD fdwFlags )
{
	ESLError	errResult = eslErrSuccess ;
	int	nFrameType = GetFrameBufferType( pFrame ) ;
	if ( nFrameType >= ftIntraFrame )
	{
		//
		// 差分フレームの判定
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
		// 展開実行
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
// パレットテーブルを適用する
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
// 先読みバッファを取得する
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EPreloadBuffer * ERIAnimation::GetPreloadBuffer( void )
{
	EPreloadBuffer *	pBuffer = NULL ;
	Lock( ) ;
	if ( m_hThread == NULL )
	{
		//
		// 先読みバッファにフレームを読み込む
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
		// B フレームキャッシュがある場合には
		// （又はその必要がない場合には）
		// 素直に直後の先読みデータを返す
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
		// B フレームキャッシュがない場合には
		// （B フレームをキャッシュする必要がある場合には）
		// 次の I, 又は P フレームを返す
		//
		for ( ; ; )
		{
			//
			// B フレーム以外を探す
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
			// B フレームしか見つからなかった場合には
			// 先読みバッファに見つかるまで読み込む
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
	// バッファを先読みする
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
// 先読みバッファに追加する
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
// 指定のフレームが I, P, B ピクチャか判定する
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
		return	ftIntraFrame ;					// I ピクチャ
	}
	if ( recid == *((UINT64*)"DiffeFrm") )
	{
		if ( m_erif.m_InfoHeader.fdwTransformation == CVTYPE_LOSSLESS_ERI )
		{
			return	ftPredictionalFrame ;		// P ピクチャ
		}
		if ( pBuffer->m_ptrBuffer[1] & 0x02 )
		{
			return	ftBidirectionalFrame ;		// B ピクチャ
		}
		return	ftPredictionalFrame ;
	}
	return	ftOtherData ;						// その他（パレット等）
}

//
// ERIFile オブジェクトを取得する
//////////////////////////////////////////////////////////////////////////////
const ERIFile & ERIAnimation::GetERIFile( void ) const
{
	return	m_erif ;
}

//
// カレントフレームのインデックスを取得する
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::CurrentIndex( void ) const
{
	return	m_iCurrentFrame ;
}

//
// カレントフレームの画像を取得
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
// パレットテーブル取得
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
// キーフレームを取得
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::GetKeyFrameCount( void ) const
{
	return	m_erif.m_FileHeader.dwKeyFrameCount ;
}

//
// 全フレーム数を取得
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::GetAllFrameCount( void ) const
{
	return	m_erif.m_FileHeader.dwFrameCount ;
}

//
// 全アニメーション時間を取得
//////////////////////////////////////////////////////////////////////////////
unsigned int ERIAnimation::GetTotalTime( void ) const
{
	return	m_erif.m_FileHeader.dwAllFrameTime ;
}

//
// フレーム番号から時間へ変換
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
// 時間からフレーム番号へ変換
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
// スレッド関数
//////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ERIAnimation::ThreadProc( LPVOID parameter )
{
	return	((ERIAnimation*)parameter)->LoadingThreadProc( ) ;
}

DWORD ERIAnimation::LoadingThreadProc( void )
{
	//
	// メッセージキューを作成
	//////////////////////////////////////////////////////////////////////////
	MSG		msg ;
	::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ;
	::SetEvent( m_hQueueSpace ) ;
	//
	// 変数初期化
	//////////////////////////////////////////////////////////////////////////
	unsigned int	iCurrentFrame = 0 ;
	m_nPreloadWaveSamples = 0 ;
	EKeyPoint	keypoint ;
	keypoint.m_iKeyFrame = 0 ;
	keypoint.m_dwSubSample = 0 ;
	keypoint.m_dwRecOffset = m_erif.GetPosition( ) ;
	AddKeyPoint( m_listKeyFrame, keypoint ) ;
	//
	// 各レコードを読み込む
	//////////////////////////////////////////////////////////////////////////
	for ( ; ; )
	{
		//
		// レコードを開く
		//////////////////////////////////////////////////////////////////////
		unsigned int	iLastFrame = iCurrentFrame ;
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		EPreloadBuffer *	pBuffer = LoadMovieStream( iCurrentFrame ) ;
		if ( pBuffer == NULL )
		{
			break ;
		}
		//
		// メッセージを受信＆ウェイト
		//////////////////////////////////////////////////////////////////////
		bool	fQuit = false ;
		do
		{
			//
			// キューが空くかメッセージを受信するまで待つ
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
					// キーフレームにシーク
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
					// 音声データをシーク
					//
					SeekKeyWave( m_listKeyWave, msg.wParam ) ;
					//
					::SetEvent( (HANDLE) msg.lParam ) ;
				}
				else if ( msg.message == tmQuit )
				{
					//
					// 終了
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
// 動画像ストリームを読み込む
//////////////////////////////////////////////////////////////////////////////
ERIAnimation::EPreloadBuffer *
	ERIAnimation::LoadMovieStream( unsigned int & iCurrentFrame )
{
	EKeyPoint	keypoint ;
	for ( ; ; )
	{
		//
		// レコードを開く
		//////////////////////////////////////////////////////////////////////
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		if ( m_erif.DescendRecord( ) )
		{
//			if ( iCurrentFrame == 0 )
			{
				// 1つも画像レコードが無い場合はエラー
				return	NULL ;
			}
			//
			// レコードの終端に到達したら
			// 自動的に先頭に移動
			iCurrentFrame = 0 ;
			m_nPreloadWaveSamples = 0 ;
			m_erif.Seek( 0, ESLFileObject::FromBegin ) ;
			continue ;
		}
		//
		// レコードの種類を判別
		//////////////////////////////////////////////////////////////////////
		UINT64	recid = m_erif.GetRecordID( ) ;
		EPreloadBuffer *	pBuffer = NULL ;
		if ( (recid == *((UINT64*)"ImageFrm")) ||
				(recid == *((UINT64*)"DiffeFrm")) )
		{
			//
			// 画像データレコード
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
			// パレットテーブルレコード
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
			// 音声ストリームレコード
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
				// 音声ストリーミングモード
				//		→ デコードして出力
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
			// レコードを閉じる
			//////////////////////////////////////////////////////////////////////
			m_erif.AscendRecord( ) ;
		}
	}
}

//
// キーフレームポイントを追加する
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::AddKeyPoint
	( ERIAnimation::EKeyPointList & list, const ERIAnimation::EKeyPoint & key )
{
	Lock( ) ;
	list.Add( new EKeyPoint( key ) ) ;
	Unlock( ) ;
}

//
// 指定のキーフレームを検索する
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
// 指定のフレームにシークする
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::SeekKeyPoint
	( ERIAnimation::EKeyPointList & list,
		unsigned int iFrame, unsigned int & iCurrentFrame )
{
	//
	// 先読みキューに読み込まれているか判断
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
		// リストに指定フレームのポインタが
		// 登録されているかどうかを調べる
		//
		EKeyPoint *	pKeyPoint = SearchKeyPoint( m_listKeyFrame, iFrame ) ;
		//
		if ( pKeyPoint != NULL )
		{
			//
			// ポインタにシーク
			//
			m_erif.Seek
				( pKeyPoint->m_dwRecOffset, ESLFileObject::FromBegin ) ;
			iCurrentFrame = pKeyPoint->m_iKeyFrame ;//iFrame ;
			m_nPreloadWaveSamples = pKeyPoint->m_dwSubSample ;
		}
		else
		{
			//
			// 指定のフレームを探す
			//
			do
			{
				//
				// 次のレコードを開く
				//
				DWORD	dwRecPosition = m_erif.GetPosition( ) ;
				if ( m_erif.DescendRecord( ) )
				{
					iCurrentFrame = 0 ;
					m_erif.Seek( 0, ESLFileObject::FromBegin ) ;
					break ;
				}
				//
				// レコードの種類を判別
				//
				UINT64	recid = m_erif.GetRecordID( ) ;
				if ( (recid == *((UINT64*)"ImageFrm")) ||
						(recid == *((UINT64*)"DiffeFrm")) )
				{
					//
					// 画像データレコード
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
					// 音声ストリームレコード
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
	// B ピクチャに対応したフォーマットでは
	// 先頭の I ピクチャはあらかじめ展開しておく
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
// 指定の音声データまでシークしてストリーミング出力する
//////////////////////////////////////////////////////////////////////////////
void ERIAnimation::SeekKeyWave
	( EKeyPointList & list, unsigned int iFrame )
{
	if ( !m_fWaveOutput || !m_fWaveStreaming )
	{
		return ;
	}
	//
	// フレーム番号を音声サンプル数へ変換
	//
	DWORD	dwSeekTime = FrameIndexToTime( iFrame ) ;
	DWORD	dwWaveSamples =
		(DWORD) ((__int64) dwSeekTime
			* m_erif.m_MIOInfHdr.dwSamplesPerSec / 1000) ;
	//
	// シークポイントより手間で、最も近いキーポイントを検索する
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
	// ファイルポインタを移動
	//
	DWORD	dwOriginalPointer = m_erif.GetPosition( ) ;
	m_nPreloadWaveSamples = pKeyPoint->m_iKeyFrame ;	
	unsigned int	iCurrentFrame = pKeyPoint->m_dwSubSample ;
	m_erif.Seek( pKeyPoint->m_dwRecOffset, ESLFileObject::FromBegin ) ;
	//
	// 指定の場所を探す
	//
	do
	{
		//
		// 次のレコードを開く
		//
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		if ( m_erif.DescendRecord( ) )
		{
			m_erif.Seek( 0, ESLFileObject::FromBegin ) ;
			break ;
		}
		//
		// レコードの種類を判別
		//
		UINT64	recid = m_erif.GetRecordID( ) ;
		if ( (recid == *((UINT64*)"ImageFrm")) ||
				(recid == *((UINT64*)"DiffeFrm")) )
		{
			//
			// 画像データレコード
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
			// 音声ストリームレコード
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
			// 音声ストリーミングモード
			//		→ デコードして出力
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
// 排他処理（クリティカルセクション）
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
