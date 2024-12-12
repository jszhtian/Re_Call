
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
                    MIOファイルストリーム再生オブジェクト
 *****************************************************************************/

IMPLEMENT_CLASS_INFO( MIODynamicPlayer, ESLObject )

//
// EPreloadBuffer 構築関数
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::EPreloadBuffer::EPreloadBuffer( DWORD dwLength )
{
	m_ptrBuffer = (BYTE*) ::eslHeapAllocate( NULL, dwLength, 0 ) ;
	m_nKeySample = 0 ;
	EMemoryFile::Open( m_ptrBuffer, dwLength ) ;
}

//
// EPreloadBuffer 消滅関数
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::EPreloadBuffer::~EPreloadBuffer( void )
{
	::eslHeapFree( NULL, m_ptrBuffer ) ;
}

//
// 構築関数
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
// 消滅関数
//////////////////////////////////////////////////////////////////////////////
MIODynamicPlayer::~MIODynamicPlayer( void )
{
	Close( ) ;
	::DeleteCriticalSection( &m_cs ) ;
}

//
// MIO ファイルを開く
//////////////////////////////////////////////////////////////////////////////
ESLError MIODynamicPlayer::Open
	( ESLFileObject * pFile, unsigned int nPreloadSize )
{
	Close( ) ;
	//
	// ERIファイルを開く（ストリームレコードまで開く）
	//////////////////////////////////////////////////////////////////////////
	if ( m_erif.Open( pFile, m_erif.otOpenStream ) )
	{
		ESLTrace( "[MIODynamicPlayer::Open] ファイルを開けませんでした。\n" ) ;
		return	ESLErrorMsg( "ERI ファイルを開けませんでした。" ) ;
	}
	//
	// 展開オブジェクトを初期化する
	//////////////////////////////////////////////////////////////////////////
	if ( !(m_erif.m_fdwReadMask & ERIFile::rmSoundInfo) )
	{
		ESLTrace( "[MIODynamicPlayer::Open] 音声情報がありません。\n" ) ;
		return	ESLErrorMsg( "ファイルに音声情報が含まれていません。" ) ;
	}
	m_pmiod = CreateMIODecoder( ) ;
	if ( m_pmiod->Initialize( m_erif.m_MIOInfHdr ) )
	{
		ESLTrace( "[MIODynamicPlayer::Open] MIO デコーダの初期化に失敗しました。\n" ) ;
		return	ESLErrorMsg( "MIO デコーダの初期化に失敗しました。" ) ;
	}
	m_pmioc = new ERISADecodeContext( 0x10000 ) ;
	//
	// 先読みバッファ配列を確保
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
	// スレッドを生成
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
// MIO ファイルを閉じる
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::Close( void )
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
	// キーポイント配列をクリアする
	m_listKeySample.RemoveAll( ) ;
	//
	// 展開オブジェクトを削除する
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
	// ファイルを閉じる
	m_erif.Close( ) ;
}

//
// 指定サンプルへ移動し、初めのブロックのデータを取得する
//////////////////////////////////////////////////////////////////////////////
void * MIODynamicPlayer::GetWaveBufferFrom
	( ULONG nSample, DWORD & dwBytes, DWORD & dwOffsetBytes )
{
	//
	// シークメッセージを送信し、シークが完了するまで待つ
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
	// 先頭のデータを取得して展開する
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
// 次の音声データがストリームの先頭であるか？
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
// 次の音声データを取得
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
// 音声バッファ確保
//////////////////////////////////////////////////////////////////////////////
void * MIODynamicPlayer::AllocateWaveBuffer( DWORD dwBytes )
{
	return	::eslHeapAllocate( NULL, dwBytes, 0 ) ;
}

//
// 音声データ破棄許可
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::DeleteWaveBuffer( void * ptrWaveBuf )
{
	::eslHeapFree( NULL, ptrWaveBuf ) ;
}

// 音声展開オブジェクトを生成
//////////////////////////////////////////////////////////////////////////////
MIODecoder * MIODynamicPlayer::CreateMIODecoder( void )
{
	return	new MIODecoder ;
}

//
// ERIFile オブジェクトを取得する
//////////////////////////////////////////////////////////////////////////////
const ERIFile & MIODynamicPlayer::GetERIFile( void ) const
{
	return	m_erif ;
}

//
// チャネル数を取得する
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetChannelCount( void ) const
{
	return	m_erif.m_MIOInfHdr.dwChannelCount ;
}

//
// サンプリング周波数を取得する
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetFrequency( void ) const
{
	return	m_erif.m_MIOInfHdr.dwSamplesPerSec ;
}

//
// サンプリングビット分解能を取得する
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetBitsPerSample( void ) const
{
	return	m_erif.m_MIOInfHdr.dwBitsPerSample ;
}

//
// 全体の長さ（サンプル数）を取得する
//////////////////////////////////////////////////////////////////////////////
DWORD MIODynamicPlayer::GetTotalSampleCount( void ) const
{
	return	m_erif.m_MIOInfHdr.dwAllSampleCount ;
}

//
// 先読みバッファを取得する
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
// 先読みバッファに追加する
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
// スレッド関数
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
	// メッセージキューを作成
	//////////////////////////////////////////////////////////////////////////
	MSG		msg ;
	::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ;
	//
	// 変数初期化
	//////////////////////////////////////////////////////////////////////////
	unsigned int	nCurrentSample = 0 ;
	//
	// 各レコードを読み込む
	//////////////////////////////////////////////////////////////////////////
	for ( ; ; )
	{
		//
		// レコードを開く
		//////////////////////////////////////////////////////////////////////
		unsigned int	nLastSample = nCurrentSample ;
		DWORD	dwRecPosition = m_erif.GetPosition( ) ;
		EPreloadBuffer *	pBuffer = LoadSoundStream( nCurrentSample ) ;
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
					// 音声データをシーク
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
// 音声データレコードを読み込む
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
			// 1つも音声レコードが無い場合はエラー
			return	NULL ;
		}
		//
		// レコードの終端に到達したら
		// 自動的に先頭に移動
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
	// 音声データレコードを読み込む
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
	// キーポイントの設定
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
// キーフレームポイントを追加する
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::AddKeySample( const MIODynamicPlayer::EKeyPoint & key )
{
	Lock( ) ;
	m_listKeySample.Add( new EKeyPoint( key ) ) ;
	Unlock( ) ;
}

//
// 指定のキーフレームを検索する
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
// 指定のサンプルを含むブロックを読み込む
//////////////////////////////////////////////////////////////////////////////
void MIODynamicPlayer::SeekKeySample
	( ULONG nSample, unsigned int & nCurrentSample )
{
	EPreloadBuffer *	pBuffer ;
	//
	// 既に先読みキューに読み込まれていないか判断
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
	// 既に読み込まれている場合にはそこまでシークする
	//
	if ( iLoaded < m_queueSound.GetSize() )
	{
		//
		// 最も近いリードブロックまで破棄する
		unsigned int	i ;
		m_queueSound.RemoveBetween( 0, iLeadBlock ) ;
		//
		// 特定のブロックまでシークする
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
	// 既に読み込まれているブロックを破棄する
	//
	m_queueSound.SetSize( 0 ) ;
	Unlock( ) ;
	//
	// リストに指定のサンプルを含むキーが登録されているか検索し、
	// 登録されていない場合には、シークする
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
		// 各レコードを順次読み込む
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
			// キーポイントの設定
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
			// 位置の更新
			if ( (nCurrentSample <= nSample) &&
					((nSample - nCurrentSample) < miodh.dwSampleCount) )
			{
				break ;
			}
			nCurrentSample += miodh.dwSampleCount ;
		}
	}
	//
	// 指定のキーポイントからシークする
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
		// 音声データレコードを読み込む
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
		// 位置の更新
		if ( (nCurrentSample <= nSample) &&
			((nSample - nCurrentSample) < pBuffer->m_miodh.dwSampleCount) )
		{
			nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
			AddPreloadBuffer( pBuffer ) ;
			break ;
		}
		nCurrentSample += pBuffer->m_miodh.dwSampleCount ;
		//
		// データを展開して破棄する
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
// 排他処理（クリティカルセクション）
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
