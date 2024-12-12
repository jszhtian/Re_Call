
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
                  ERIアニメーションファイル出力オブジェクト
 *****************************************************************************/

//
// クラス情報
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ERIAnimationWriter, ESLObject )

//
// ERIAnimationWriter::EEncodeContext 消滅関数
//////////////////////////////////////////////////////////////////////////////
ERIAnimationWriter::EEncodeContext::~EEncodeContext( void )
{
}

//
// 次のデータを書き出す
//////////////////////////////////////////////////////////////////////////////
ULONG ERIAnimationWriter::EEncodeContext::WriteNextData
					( const BYTE * ptrBuffer, ULONG nBytes )
{
	return	m_buf.Write( ptrBuffer, nBytes ) ;
}

//
// 構築関数
//////////////////////////////////////////////////////////////////////////////
ERIAnimationWriter::ERIAnimationWriter( void )
{
	m_wsStatus = wsNotOpened ;
	m_perie1 = NULL ;
	m_perie2 = NULL ;
	m_pmioc = NULL ;
	m_pmioe = NULL ;
	m_peiiLast = NULL ;
	m_peiiNext = NULL ;
	m_peiiBuf = NULL ;
	m_fDualEncoding = false ;
	m_hCompressed = NULL ;
	m_hThread = NULL ;
}

//
// 消滅関数
//////////////////////////////////////////////////////////////////////////////
ERIAnimationWriter::~ERIAnimationWriter( void )
{
	Close( ) ;
}

//
// ファイルを開く
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::Open
	( ESLFileObject * pFile, ERIAnimationWriter::FileIdentity fidType )
{
	//
	// 既にファイルを開いている場合には閉じる
	//
	Close( ) ;
	//
	// ERI ファイルを開く
	//
	static const char	szFileDetails[3][0x30] =
	{
		"Entis Rasterized Image",
		"Music Interleaved and Orthogonal transformed",
		"Moving Entis Image"
	} ;
	EMCFile::FILE_HEADER	fhdr ;
	EMCFile::SetFileHeader
		( fhdr, EMCFile::fidRasterizedImage, szFileDetails[fidType] ) ;
	if ( m_eriwf.Open( pFile, &fhdr ) )
	{
		return	ESLErrorMsg( "ファイルヘッダの書き出しに失敗しました。" ) ;
	}
	//
	// 成功
	//
	m_wsStatus = wsOpened ;

	return	eslErrSuccess ;
}

//
// ファイルを閉じる
//////////////////////////////////////////////////////////////////////////////
void ERIAnimationWriter::Close( void )
{
	//
	// レコードを閉じる
	//
	if ( m_wsStatus == wsWritingStream )
	{
		EndFileHeader( ) ;
	}
	else if ( m_wsStatus == wsWritingHeader )
	{
		EndStream( 0 ) ;
	}
	//
	// スレッド終了
	//
	if ( m_hThread != NULL )
	{
		::PostThreadMessage( m_idThread, tmQuit, 0, 0 ) ;
		::WaitForSingleObject( m_hThread, INFINITE ) ;
		::CloseHandle( m_hThread ) ;
		m_hThread = NULL ;
	}
	if ( m_hCompressed != NULL )
	{
		::CloseHandle( m_hCompressed ) ;
		m_hCompressed = NULL ;
	}
	//
	// オブジェクト解放
	//
	if ( m_perie1 != NULL )
	{
		m_perie1->Delete( ) ;
		delete	m_perie1 ;
		m_perie1 = NULL ;
	}
	if ( m_perie2 != NULL )
	{
		m_perie2->Delete( ) ;
		delete	m_perie2 ;
		m_perie2 = NULL ;
	}
	if ( m_pmioc != NULL )
	{
		delete	m_pmioc ;
		m_pmioc = NULL ;
	}
	if ( m_pmioe != NULL )
	{
		m_pmioe->Delete( ) ;
		delete	m_pmioe ;
		m_pmioe = NULL ;
	}
	//
	// バッファ解放
	//
	if ( m_peiiLast != NULL )
	{
		DeleteImageBuffer( m_peiiLast ) ;
		m_peiiLast = NULL ;
	}
	if ( m_peiiNext != NULL )
	{
		DeleteImageBuffer( m_peiiNext ) ;
		m_peiiNext = NULL ;
	}
	if ( m_peiiBuf != NULL )
	{
		DeleteImageBuffer( m_peiiBuf ) ;
		m_peiiBuf = NULL ;
	}
	for ( unsigned int i = 0; i < m_lstFrameBuf.GetSize(); i ++ )
	{
		DeleteImageBuffer( m_lstFrameBuf.GetAt(i) ) ;
	}
	m_lstEncFlags.RemoveAll( ) ;
	m_lstFrameBuf.RemoveAll( ) ;
	//
	// ファイルを閉じる
	//
	if ( m_wsStatus != wsNotOpened )
	{
		m_eriwf.Close( ) ;
		m_wsStatus = wsNotOpened ;
	}
}

//
// ファイルヘッダを開く
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::BeginFileHeader
	( DWORD dwKeyFrame, DWORD dwKeyWave, DWORD dwBidirectKey )
{
	//
	// パラメータの検証と設定
	//
	if ( m_wsStatus != wsOpened )
	{
		return	eslErrGeneral ;
	}
	m_dwKeyFrame = dwKeyFrame ;
	m_dwBidirectKey = dwBidirectKey ;
	m_dwKeyWave = dwKeyWave ;
	if ( m_dwBidirectKey == 0 )
	{
		m_dwBidirectKey = 1 ;
	}
	//
	// ヘッダレコードを開く
	//
	ESLError	err ;
	err = m_eriwf.DescendRecord( (UINT64*) "Header  " ) ;
	if ( err )
	{
		return	err ;
	}
	//
	// ファイルヘッダを書き出す（ダミー出力）
	//
	err = m_eriwf.DescendRecord( (UINT64*)"FileHdr " ) ;
	if ( err )
	{
		return	err ;
	}
	m_efh.dwVersion = 0x00020100 ;
	m_efh.dwContainedFlag = 0 ;
	m_efh.dwKeyFrameCount = dwKeyFrame ;
	m_efh.dwFrameCount = 0 ;
	m_efh.dwAllFrameTime = 0 ;
	m_eriwf.Write( &m_efh, sizeof(m_efh) ) ;
	m_eriwf.AscendRecord( ) ;
	//
	m_fWithSeqTable = false ;
	m_dwFrameCount = 0 ;
	m_dwWaveCount = 0 ;
	m_dwMioHeaderPos = 0 ;
	m_dwOutputWaveSamples = 0 ;
	m_fKeyWaveBlock = true ;
	m_dwWaveBufSamples = 0 ;
	m_bufWaveBuffer.Delete( ) ;
	//
	// 成功
	//
	m_wsStatus = wsWritingHeader ;

	return	eslErrSuccess ;
}

//
// プレビュー画像情報ヘッダを書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WritePreviewInfo( const ERI_INFO_HEADER & eih )
{
	if ( m_wsStatus != wsWritingHeader )
	{
		return	eslErrGeneral ;
	}
	ESLError	err ;
	err = m_eriwf.DescendRecord( (UINT64*)"PrevwInf" ) ;
	if ( err )
	{
		return	err ;
	}
	m_prvwih = eih ;
	m_eriwf.Write( &m_prvwih, sizeof(m_prvwih) ) ;
	m_eriwf.AscendRecord( ) ;

	return	eslErrSuccess ;
}

//
// 画像情報ヘッダを書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteEriInfoHeader( const ERI_INFO_HEADER & eih )
{
	if ( m_wsStatus != wsWritingHeader )
	{
		return	eslErrGeneral ;
	}
	ESLError	err ;
	err = m_eriwf.DescendRecord( (UINT64*)"ImageInf" ) ;
	if ( err )
	{
		return	err ;
	}
	m_eih = eih ;
	m_eriwf.Write( &m_eih, sizeof(m_eih) ) ;
	m_eriwf.AscendRecord( ) ;
	//
	m_efh.dwContainedFlag |= EFH_CONTAIN_IMAGE ;

	return	eslErrSuccess ;
}

//
// 音声情報ヘッダを書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteMioInfoHeader( const MIO_INFO_HEADER & mih )
{
	if ( m_wsStatus != wsWritingHeader )
	{
		return	eslErrGeneral ;
	}
	m_dwMioHeaderPos = m_eriwf.GetPosition( ) ;
	//
	ESLError	err ;
	err = m_eriwf.DescendRecord( (UINT64*)"SoundInf" ) ;
	if ( err )
	{
		return	err ;
	}
	m_mih = mih ;
	m_eriwf.Write( &m_mih, sizeof(m_mih) ) ;
	m_eriwf.AscendRecord( ) ;
	//
	m_efh.dwContainedFlag |= EFH_CONTAIN_WAVE ;

	return	eslErrSuccess ;
}

//
// 著作権情報を書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteCopyright
	( const void * ptrCopyright, DWORD dwBytes )
{
	if ( m_wsStatus != wsWritingHeader )
	{
		return	eslErrGeneral ;
	}
	ESLError	err ;
	err = m_eriwf.DescendRecord( (UINT64*)"cpyright" ) ;
	if ( err )
	{
		return	err ;
	}
	m_eriwf.Write( ptrCopyright, dwBytes ) ;
	m_eriwf.AscendRecord( ) ;

	return	eslErrSuccess ;
}

//
// コメントを書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteDescription
	( const void * ptrDescription, DWORD dwBytes )
{
	if ( m_wsStatus != wsWritingHeader )
	{
		return	eslErrGeneral ;
	}
	ESLError	err ;
	err = m_eriwf.DescendRecord( (UINT64*)"descript" ) ;
	if ( err )
	{
		return	err ;
	}
	m_eriwf.Write( ptrDescription, dwBytes ) ;
	m_eriwf.AscendRecord( ) ;

	return	eslErrSuccess ;
}

//
// シーケンステーブルを書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteSequenceTable
	( ERIFile::SEQUENCE_DELTA * pSequence, DWORD dwLength )
{
	if ( m_wsStatus != wsWritingHeader )
	{
		return	eslErrGeneral ;
	}
	ESLError	err ;
	err = m_eriwf.DescendRecord( (UINT64*)"Sequence" ) ;
	if ( err )
	{
		return	err ;
	}
	m_eriwf.Write( pSequence, dwLength * sizeof(ERIFile::SEQUENCE_DELTA) ) ;
	m_eriwf.AscendRecord( ) ;
	//
	m_efh.dwFrameCount = dwLength ;
	m_fWithSeqTable = true ;

	return	eslErrSuccess ;
}

//
// ファイルヘッダを閉じる
//////////////////////////////////////////////////////////////////////////////
void ERIAnimationWriter::EndFileHeader( void )
{
	if ( m_wsStatus == wsWritingHeader )
	{
		m_dwHeaderBytes = m_eriwf.GetPosition( ) ;
		m_eriwf.AscendRecord( ) ;
		m_wsStatus = wsOpened ;
	}
}

//
// ストリームを開始する
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::BeginStream( void )
{
	//
	// ステータスチェック
	//
	if ( m_wsStatus != wsOpened )
	{
		return	eslErrGeneral ;
	}
	//
	// 圧縮オブジェクト初期化
	//
	if ( m_efh.dwContainedFlag & EFH_CONTAIN_IMAGE )
	{
		m_perie1 = CreateERIEncoder( ) ;
		m_perie2 = CreateERIEncoder( ) ;
		if ( m_perie1->Initialize( m_eih ) || m_perie2->Initialize( m_eih ) )
		{
			return	ESLErrorMsg( "画像エンコーダの初期化に失敗しました" ) ;
		}
		m_perie1->SetCompressionParameter( m_eriep_i ) ;
		m_perie2->SetCompressionParameter( m_eriep_p ) ;
	}
	if ( m_efh.dwContainedFlag & EFH_CONTAIN_WAVE )
	{
		m_pmioc = new ERISAEncodeContext( 0x10000 ) ;
		m_pmioc->AttachOutputFile( &m_eriwf ) ;
		m_pmioe = CreateMIOEncoder( ) ;
		if ( m_pmioe->Initialize( m_mih ) )
		{
			return	ESLErrorMsg( "音声エンコーダの初期化に失敗しました" ) ;
		}
		m_pmioe->SetCompressionParameter( m_mioep ) ;
	}
	//
	// 差分処理用バッファ生成
	//
	if ( m_efh.dwContainedFlag & EFH_CONTAIN_IMAGE )
	{
		m_peiiLast = CreateImageBuffer( m_eih ) ;
		m_peiiNext = CreateImageBuffer( m_eih ) ;
		m_peiiBuf = CreateImageBuffer( m_eih ) ;
	}
	//
	// 圧縮スレッド準備
	//
	if ( m_fDualEncoding )
	{
		m_hCompressed = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		if ( m_hCompressed == NULL )
		{
			return	eslErrGeneral ;
		}
		m_hThread = ::CreateThread
			( NULL, 0, &ERIAnimationWriter::ThreadProc, this, 0, &m_idThread ) ;
		if ( m_hThread == NULL )
		{
			return	eslErrGeneral ;
		}
		HANDLE	hEventList[2] ;
		hEventList[0] = m_hThread ;
		hEventList[1] = m_hCompressed ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		if ( dwWaitResult == WAIT_OBJECT_0 )
		{
			::CloseHandle( m_hThread ) ;
			::CloseHandle( m_hCompressed ) ;
			m_hThread = NULL ;
			m_hCompressed = NULL ;
			return	eslErrGeneral ;
		}
	}
	m_wsStatus = wsWritingStream ;
	//
	// ストリームレコードを開く
	//
	if ( m_eriwf.DescendRecord( (UINT64*)"Stream  " ) )
	{
		EndStream( 0 ) ;
		return	eslErrGeneral ;
	}

	return	eslErrSuccess ;
}

//
// 画像の圧縮パラメータを設定する
//////////////////////////////////////////////////////////////////////////////
void ERIAnimationWriter::SetImageCompressionParameter
	( const ERISAEncoder::PARAMETER & eriep )
{
	m_eriep_i = eriep ;
	m_eriep_p = eriep ;
	m_eriep_p.m_rYScaleDC *= eriep.m_rPFrameScale ;
	m_eriep_p.m_rCScaleDC *= eriep.m_rPFrameScale ;
	m_eriep_p.m_rYScaleLow = m_eriep_p.m_rYScaleDC * 0.75F ;
	m_eriep_p.m_rCScaleLow = m_eriep_p.m_rCScaleDC * 0.75F ;
	m_eriep_p.m_rYScaleHigh = m_eriep_p.m_rYScaleDC * 0.5F ;
	m_eriep_p.m_rCScaleHigh = m_eriep_p.m_rCScaleDC * 0.5F ;
	m_eriep_p.m_nYLPFThreshold = 64 - (64 - m_eriep_p.m_nYLPFThreshold) / 3 ;
	m_eriep_p.m_nCLPFThreshold = 64 - (64 - m_eriep_p.m_nCLPFThreshold) / 3 ;
	m_eriep_p.m_nMaxFrameSize =
		(ULONG) (eriep.m_nMaxFrameSize * eriep.m_rPFrameScale * 0.65) ;
	m_eriep_p.m_nMinFrameSize =
		(ULONG) (eriep.m_nMinFrameSize * eriep.m_rPFrameScale * 0.4) ;
	m_eriep_b = eriep ;
	m_eriep_b.m_rYScaleDC *= eriep.m_rBFrameScale ;
	m_eriep_b.m_rCScaleDC *= eriep.m_rBFrameScale ;
	m_eriep_b.m_rYScaleLow = m_eriep_b.m_rYScaleDC * 0.75F ;
	m_eriep_b.m_rCScaleLow = m_eriep_b.m_rCScaleDC * 0.75F ;
	m_eriep_b.m_rYScaleHigh = m_eriep_b.m_rYScaleDC * 0.5F ;
	m_eriep_b.m_rCScaleHigh = m_eriep_b.m_rCScaleDC * 0.5F ;
	m_eriep_b.m_nYLPFThreshold = (m_eriep_p.m_nYLPFThreshold + 64) / 2 ;
	m_eriep_b.m_nCLPFThreshold = (m_eriep_p.m_nCLPFThreshold + 64) / 2 ;
	m_eriep_b.m_nMaxFrameSize =
		(ULONG) (eriep.m_nMaxFrameSize * eriep.m_rBFrameScale * 0.5) ;
	m_eriep_b.m_nMinFrameSize =
		(ULONG) (eriep.m_nMinFrameSize * eriep.m_rBFrameScale * 0.333) ;
	//
	if ( m_perie1 != NULL )
	{
		m_perie1->SetCompressionParameter( m_eriep_i ) ;
	}
	if ( m_perie2 != NULL )
	{
		m_perie2->SetCompressionParameter( m_eriep_p ) ;
	}
}

//
// 音声の圧縮パラメータを設定する
//////////////////////////////////////////////////////////////////////////////
void ERIAnimationWriter::SetSoundCompressionParameter
	( const MIOEncoder::PARAMETER & mioep )
{
	m_mioep = mioep ;
	//
	if ( m_pmioe != NULL )
		m_pmioe->SetCompressionParameter( mioep ) ;
}

//
// パレットテーブルを書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WritePaletteTable
	( const EGL_PALETTE * paltbl, unsigned int nLength )
{
	if ( m_wsStatus != wsWritingStream )
	{
		return	eslErrGeneral ;
	}
	if ( m_eriwf.DescendRecord( (UINT64*)"Palette " ) )
	{
		return	eslErrGeneral ;
	}
	m_eriwf.Write( paltbl, nLength * sizeof(EGL_PALETTE) ) ;
	m_eriwf.AscendRecord( ) ;
	//
	m_efh.dwContainedFlag |= EFH_CONTAIN_PALETTE ;

	return	eslErrSuccess ;
}

//
// プレビュー画像を出力する
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WritePreviewData
	( const EGL_IMAGE_INFO & rii, DWORD fdwFlags )
{
	if ( m_wsStatus != wsWritingStream )
	{
		return	eslErrGeneral ;
	}
	if ( m_eriwf.DescendRecord( (UINT64*)"Preview " ) )
	{
		return	eslErrGeneral ;
	}
	//
	// 画像圧縮
	//
	ESLError	errResult = eslErrGeneral ;
	do
	{
		ERISAEncodeContext	context( 0x10000 ) ;
		ERISAEncoder		encoder ;
		context.AttachOutputFile( &m_eriwf ) ;
		//
		if ( encoder.Initialize( m_prvwih ) )
		{
			break ;
		}
		if ( encoder.EncodeImage( rii, context, fdwFlags ) )
		{
			break ;
		}
		//
		errResult = eslErrSuccess ;
	}
	while ( false ) ;
	//
	m_eriwf.AscendRecord( ) ;

	return	errResult ;
}

//
// 音声データを出力する
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteWaveData
	( const void * ptrWaveBuf, DWORD dwSampleCount )
{
	if ( m_wsStatus != wsWritingStream )
	{
		return	eslErrGeneral ;
	}
	if ( m_efh.dwContainedFlag & EFH_CONTAIN_IMAGE )
	{
		DWORD	dwWaveBytes =
			dwSampleCount * (m_mih.dwChannelCount * m_mih.dwBitsPerSample / 8) ;
		m_bufWaveBuffer.Write( ptrWaveBuf, dwWaveBytes ) ;
		m_dwWaveBufSamples += dwSampleCount ;
		return	eslErrSuccess ;
	}
	if ( m_eriwf.DescendRecord( (UINT64*)"SoundStm" ) )
	{
		return	eslErrGeneral ;
	}
	//
	// 音声圧縮
	//
	ESLError	errResult = eslErrGeneral ;
	MIO_DATA_HEADER	miodh ;
	miodh.bytVersion = 1 ;
	miodh.bytFlags = 0 ;
	miodh.bytReserved1 = 0 ;
	miodh.bytReserved2 = 0 ;
	miodh.dwSampleCount = dwSampleCount ;
	if ( ((m_dwKeyWave == 0) && (m_dwWaveCount == 0))
				|| (m_dwWaveCount % m_dwKeyWave) == 0 )
	{
		miodh.bytFlags = MIO_LEAD_BLOCK ;
	}
	if ( m_eriwf.Write( &miodh, sizeof(miodh) ) == sizeof(miodh) )
	{
		if ( !m_pmioe->EncodeSound( *m_pmioc, miodh, ptrWaveBuf ) )
		{
			++ m_dwWaveCount ;
			m_dwOutputWaveSamples += dwSampleCount ;
			errResult = eslErrSuccess ;
		}
	}
	//
	m_eriwf.AscendRecord( ) ;

	return	errResult ;
}

//
// 画像データを出力する
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteImageData
	( const EGL_IMAGE_INFO & eii, DWORD fdwFlags )
{
	if ( m_wsStatus != wsWritingStream )
	{
		return	eslErrGeneral ;
	}
	//
	// キーフレームかどうかを判定
	//
	bool	fKeyFrame = (m_dwFrameCount == 0) ;
	if ( m_dwKeyFrame != 0 )
	{
		fKeyFrame = fKeyFrame || ((m_dwFrameCount % m_dwKeyFrame) == 0) ;
	}
	if ( !fKeyFrame && (m_eih.fdwTransformation != CVTYPE_LOSSLESS_ERI) )
	{
		if ( ((m_dwDiffFrames + 1) % m_dwBidirectKey) != 0 )
		{
			//
			// B ピクチャは一旦バッファに蓄積しておく
			//
			PEGL_IMAGE_INFO	peiiFrameBuf = CreateImageBuffer( m_eih ) ;
			::eriCopyImage( *peiiFrameBuf, eii ) ;
			m_lstEncFlags.Add( fdwFlags ) ;
			m_lstFrameBuf.Add( peiiFrameBuf ) ;
			m_dwFrameCount ++ ;
			m_dwDiffFrames ++ ;
			return	eslErrSuccess ;
		}
	}
	//
	// 差分処理フレームを圧縮
	//
	if ( !fKeyFrame )
	do
	{
		//
		// 差分処理
		//
		DWORD	fdwDifFlag = fdwFlags ;
		::eriCopyImage( *m_peiiBuf, eii ) ;
		if ( m_eih.fdwTransformation == CVTYPE_LOSSLESS_ERI )
		{
			::eriLLSubtractionOfFrame( *m_peiiBuf, *m_peiiLast ) ;
		}
		else
		{
			int		nAbsMaxDiff = 0x7FFFFFFF ;
			if ( fdwFlags & ERISAEncoder::efNoMoveVector )
			{
				nAbsMaxDiff =
					::eriLSSubtractionOfFrame( *m_peiiBuf, *m_peiiLast ) ;
			}
			else
			{
				m_perie2->ProcessMovingVector
					( *m_peiiBuf, *m_peiiLast, nAbsMaxDiff ) ;
			}
			if ( nAbsMaxDiff >= m_eriep_i.m_nAMDFThreshold )
			{
				fKeyFrame = true ;
				m_perie2->ClearMovingVector( ) ;
				break ;
			}
			fdwDifFlag |= ERISAEncoder::efDifferential ;
		}
		//
		// 圧縮開始
		//
		m_eric2.Delete( ) ;
		m_perie2->SetCompressionParameter( m_eriep_p ) ;
		if ( m_fDualEncoding )
		{
			::ResetEvent( m_hCompressed ) ;
			::PostThreadMessage
				( m_idThread, tmEncodeImage, fdwDifFlag, (LPARAM) m_peiiBuf ) ;
		}
		else
		{
			//
			// シングルスレッド処理の場合にはここで圧縮
			//
			if ( m_perie2->EncodeImage( *m_peiiBuf, m_eric2, fdwDifFlag ) )
			{
				return	eslErrGeneral ;
			}
		}
	}
	while ( false ) ;
	//
	// 非差分処理フレームを圧縮
	//
	m_eric1.Delete( ) ;
	ESLError	errResult ;
	m_perie1->SetCompressionParameter( m_eriep_i ) ;
	errResult = m_perie1->EncodeImage( eii, m_eric1, fdwFlags ) ;
	//
	// デュアルスレッド圧縮モードの時は、一方の圧縮が終了するのを待つ
	//
	if ( !fKeyFrame && m_fDualEncoding )
	{
		HANDLE	hEventList[2] ;
		hEventList[0] = m_hThread ;
		hEventList[1] = m_hCompressed ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		//
		if ( (dwWaitResult == WAIT_OBJECT_0) || !m_fCompressSuccessed )
		{
			errResult = eslErrGeneral ;
		}
	}
	if ( errResult )
	{
		return	errResult ;
	}
	//
	// 圧縮されたデータを取得する
	//
	EEncodeContext *	pEncodedData ;
	UINT64*			pRecID ;
	//
	if ( fKeyFrame )
	{
		pEncodedData = &m_eric1 ;
		pRecID = (UINT64*)"ImageFrm" ;
		m_dwDiffFrames = 0 ;
	}
	else
	{
		DWORD	dwSize1 = m_eric1.m_buf.GetLength(),
				dwSize2 = m_eric2.m_buf.GetLength() ;
		m_dwDiffFrames ++ ;
		if ( m_eih.fdwTransformation != CVTYPE_LOSSLESS_ERI )
		{
			dwSize2 = dwSize2 * ((m_dwDiffFrames / m_dwBidirectKey) + 7) / 8 ;
		}
		if ( dwSize1 < dwSize2 )
		{
			fKeyFrame = true ;
			pEncodedData = &m_eric1 ;
			pRecID = (UINT64*)"ImageFrm" ;
			m_dwDiffFrames = 0 ;
		}
		else
		{
			pEncodedData = &m_eric2 ;
			pRecID = (UINT64*)"DiffeFrm" ;
		}
	}
	//
	// フレームを１つ進める
	//
	EPtrBuffer	ptrbuf = pEncodedData->m_buf.GetBuffer( ) ;
	++ m_dwFrameCount ;
	if ( m_eih.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		::eriCopyImage( *m_peiiLast, eii ) ;
		//
		// 音声データを書き出す
		//
		m_fKeyWaveBlock |= fKeyFrame ;
		if ( m_dwWaveBufSamples > 0 )
		{
			WriteWaveBuffer( ) ;
		}
	}
	else
	{
		//
		// 非可逆圧縮の場合には、実際にデータを展開して
		// 圧縮して劣化した画像を取得する
		//
		EMemoryFile			memfile ;
		ERISADecodeContext	context( 0x10000 ) ;
		ERISADecoder		decoder ;
		memfile.Open( ptrbuf, ptrbuf.GetLength() ) ;
		context.AttachInputFile( &memfile ) ;
		do
		{
			errResult = decoder.Initialize( m_eih ) ;
			if ( errResult )
				break ;
			DWORD	fdwDifFlag = 0 ;
			if ( *pRecID == *((UINT64*)"DiffeFrm") )
			{
				fdwDifFlag = ERISADecoder::dfDifferential ;
				decoder.SetRefPreviousFrame( m_peiiLast ) ;
			}
			errResult = decoder.DecodeImage( *m_peiiNext, context, fdwDifFlag ) ;
		}
		while ( false ) ;
		//
		// B ピクチャがある場合には B ピクチャを圧縮する
		//
		::eriBlendHalfImage( *m_peiiNext, *m_peiiNext, eii ) ;
		if ( !errResult )
		{
			errResult = WriteBirectionalFrames( ) ;
		}
		::eriCopyImage( *m_peiiLast, *m_peiiNext ) ;
		//
		// 音声データを書き出す
		//
		m_fKeyWaveBlock |= fKeyFrame ;
		if ( m_dwWaveBufSamples > 0 )
		{
			WriteWaveBuffer( ) ;
		}
	}
	//
	// データを書き出す
	//
	if ( m_eriwf.DescendRecord( pRecID ) )
	{
		return	eslErrGeneral ;
	}
	m_eriwf.Write( ptrbuf, ptrbuf.GetLength() ) ;
	m_eriwf.AscendRecord( ) ;
	pEncodedData->m_buf.Release( ptrbuf.GetLength() ) ;
	//
	// 出力バッファを破棄する
	//
	m_eric1.Delete( ) ;
	//
	if ( !fKeyFrame )
	{
		m_eric2.Delete( ) ;
	}

	return	errResult ;
}

//
// ストリームを閉じる
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::EndStream( DWORD dwTotalTime )
{
	if ( m_wsStatus != wsWritingStream )
	{
		return	eslErrGeneral ;
	}
	//
	// B ピクチャが残っている場合には、
	// 最後のフレームを I ピクチャとしてエンコードしなおす
	//
	if ( m_lstFrameBuf.GetSize() > 0 )
	{
		DWORD	dwSaveKeyFrame = m_dwKeyFrame ;
		PEGL_IMAGE_INFO	pLastFrame = m_lstFrameBuf.Pop() ;
		m_dwKeyFrame = 1 ;
		//
		WriteImageData( *pLastFrame, 0 ) ;
		//
		DeleteImageBuffer( pLastFrame ) ;
		m_dwKeyFrame = dwSaveKeyFrame ;
	}
	//
	// 音声データが残っている場合にはエンコードして出力する
	//
	if ( m_dwWaveBufSamples > 0 )
	{
		WriteWaveBuffer( ) ;
	}
	//
	// スレッドを終了
	if ( m_hThread != NULL )
	{
		::PostThreadMessage( m_idThread, tmQuit, 0, 0 ) ;
		::WaitForSingleObject( m_hThread, INFINITE ) ;
		::CloseHandle( m_hThread ) ;
		m_hThread = NULL ;
	}
	if ( m_hCompressed != NULL )
	{
		::CloseHandle( m_hCompressed ) ;
		m_hCompressed = NULL ;
	}
	//
	// 圧縮オブジェクトの利用しているリソースを破棄
	if ( m_efh.dwContainedFlag & EFH_CONTAIN_IMAGE )
	{
		m_eric1.Delete( ) ;
		m_eric2.Delete( ) ;
		m_perie1->Delete( ) ;
		m_perie2->Delete( ) ;
	}
	if ( m_pmioc != NULL )
	{
		delete	m_pmioc ;
		m_pmioc = NULL ;
	}
	//
	// バッファを削除
	if ( m_peiiLast != NULL )
	{
		DeleteImageBuffer( m_peiiLast ) ;
		m_peiiLast = NULL ;
	}
	if ( m_peiiBuf != NULL )
	{
		DeleteImageBuffer( m_peiiBuf ) ;
		m_peiiBuf = NULL ;
	}
	//
	// ストリームレコードを閉じる
	m_eriwf.AscendRecord( ) ;
	m_wsStatus = wsOpened ;
	//
	// ファイルヘッダを再出力
	m_eriwf.Seek( 0, ESLFileObject::FromBegin ) ;
	//
	if ( m_eriwf.DescendRecord( (UINT64*)"Header  " )
		|| m_eriwf.DescendRecord( (UINT64*)"FileHdr " ) )
	{
		return	eslErrGeneral ;
	}
	if ( !m_fWithSeqTable )
	{
		m_efh.dwFrameCount = m_dwFrameCount ;
	}
	m_efh.dwAllFrameTime = dwTotalTime ;
	m_eriwf.Write( &m_efh, sizeof(m_efh) ) ;
	m_eriwf.AscendRecord( ) ;
	//
	// 音声情報ヘッダを再出力
	if ( m_efh.dwContainedFlag & EFH_CONTAIN_WAVE )
	{
		m_eriwf.Seek( m_dwMioHeaderPos, ESLFileObject::FromBegin ) ;
		m_eriwf.DescendRecord( (UINT64*)"SoundInf" ) ;
		m_mih.dwAllSampleCount = m_dwOutputWaveSamples ;
		m_eriwf.Write( &m_mih, sizeof(m_mih) ) ;
		m_eriwf.AscendRecord( ) ;
	}
	//
	m_eriwf.Seek( m_dwHeaderBytes, ESLFileObject::FromBegin ) ;
	m_eriwf.AscendRecord( ) ;

	return	eslErrSuccess ;
}

//
// B ピクチャを圧縮して書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteBirectionalFrames( void )
{
	//
	// エンコーダをセットアップする
	//
	m_perie2->SetCompressionParameter( m_eriep_b ) ;
	//
	// 順次フレームを圧縮
	//
	for ( unsigned int i = 0; i < m_lstFrameBuf.GetSize(); i ++ )
	{
		//
		// フレームを取得
		//
		PEGL_IMAGE_INFO	pImageBuf = m_lstFrameBuf.GetAt( i ) ;
		ESLAssert( pImageBuf != NULL ) ;
		if ( pImageBuf == NULL )
		{
			continue ;
		}
		DWORD	fdwFlags =
			m_lstEncFlags.GetAt(i) | ERISAEncoder::efDifferential ;
		do
		{
			//
			// 差分画像を生成
			//
			int		nAbsMaxDiff = 0x7FFFFFFF ;
			if ( fdwFlags & ERISAEncoder::efNoMoveVector )
			{
				nAbsMaxDiff =
					::eriLSSubtractionOfFrame( *pImageBuf, *m_peiiLast ) ;
			}
			else
			{
				m_perie2->ProcessMovingVector
					( *pImageBuf, *m_peiiLast, nAbsMaxDiff, m_peiiNext ) ;
			}
			//
			// 圧縮
			//
			EEncodeContext	context ;
			if ( m_perie2->EncodeImage( *pImageBuf, context, fdwFlags ) )
			{
				break ;
			}
			//
			// 書き出し
			//
			EPtrBuffer	ptrbuf = context.m_buf.GetBuffer( ) ;
			if ( m_eriwf.DescendRecord( (UINT64*)"DiffeFrm" ) )
			{
				break ;
			}
			m_eriwf.Write( ptrbuf, ptrbuf.GetLength() ) ;
			m_eriwf.AscendRecord( ) ;
		}
		while ( false ) ;
		//
		DeleteImageBuffer( pImageBuf ) ;
	}
	//
	m_lstEncFlags.RemoveAll( ) ;
	m_lstFrameBuf.RemoveAll( ) ;
	//
	return	eslErrSuccess ;
}

//
// 音声データを圧縮して書き出す
//////////////////////////////////////////////////////////////////////////////
ESLError ERIAnimationWriter::WriteWaveBuffer( void )
{
	if ( m_dwWaveBufSamples == 0 )
	{
		return	eslErrSuccess ;
	}
	if ( m_eriwf.DescendRecord( (UINT64*)"SoundStm" ) )
	{
		return	eslErrGeneral ;
	}
	ESLError	errResult = eslErrGeneral ;
	MIO_DATA_HEADER	miodh ;
	miodh.bytVersion = 1 ;
	miodh.bytFlags = 0 ;
	miodh.bytReserved1 = 0 ;
	miodh.bytReserved2 = 0 ;
	miodh.dwSampleCount = m_dwWaveBufSamples ;
	if ( m_fKeyWaveBlock )
	{
		miodh.bytFlags = MIO_LEAD_BLOCK ;
		m_fKeyWaveBlock = false ;
	}
	if ( m_eriwf.Write( &miodh, sizeof(miodh) ) == sizeof(miodh) )
	{
		EPtrBuffer	ptrbuf = m_bufWaveBuffer.GetBuffer( ) ;
		if ( !m_pmioe->EncodeSound( *m_pmioc, miodh, ptrbuf ) )
		{
			m_dwOutputWaveSamples += m_dwWaveBufSamples ;
			errResult = eslErrSuccess ;
		}
		m_bufWaveBuffer.Release( ptrbuf.GetLength() ) ;
	}
	m_dwWaveBufSamples = 0 ;
	//
	m_eriwf.AscendRecord( ) ;
	//
	return	errResult ;
}

//
// 画像バッファを生成
//////////////////////////////////////////////////////////////////////////////
EGL_IMAGE_INFO *
	 ERIAnimationWriter::CreateImageBuffer( const ERI_INFO_HEADER & eih )
{
	//
	// EGL_IMAGE_INFO 構造体メモリを確保
	//
	DWORD	dwWidth, dwHeight ;
	DWORD	dwLineBytes, dwImageSize ;
	dwWidth = eih.nImageWidth ;
	dwHeight = (eih.nImageHeight < 0) ? - eih.nImageHeight : eih.nImageHeight ;
	dwLineBytes = ((dwWidth * eih.dwBitsPerPixel >> 3) + 0x03) & ~0x03 ;
	dwImageSize = dwLineBytes * dwHeight ;
	EGL_IMAGE_INFO *	peii =
		(EGL_IMAGE_INFO*) ::eslHeapAllocate
			( NULL, dwImageSize
				+ (sizeof(EGL_IMAGE_INFO) + 0x10), ESL_HEAP_ZERO_INIT ) ;
	//
	// パラメータコピー
	//
	peii->dwInfoSize = sizeof(EGL_IMAGE_INFO) ;
	peii->fdwFormatType = eih.fdwFormatType ;
	peii->dwImageWidth = dwWidth ;
	peii->dwImageHeight = dwHeight ;
	peii->dwBitsPerPixel = eih.dwBitsPerPixel ;
	peii->dwBytesPerLine = dwLineBytes ;
	peii->dwSizeOfImage = dwImageSize ;
	//
	// 画像バッファのアドレスを設定
	//
	peii->ptrImageArray =
		((BYTE*) peii) + ((sizeof(EGL_IMAGE_INFO) + 0x0F) & ~0x0F) ;

	return	peii ;
}

//
// 画像バッファを消去
//////////////////////////////////////////////////////////////////////////////
void ERIAnimationWriter::DeleteImageBuffer( EGL_IMAGE_INFO * peii )
{
	if ( peii != NULL )
	{
		::eslHeapFree( NULL, peii ) ;
	}
}

//
// 画像圧縮オブジェクトを生成
//////////////////////////////////////////////////////////////////////////////
ERISAEncoder * ERIAnimationWriter::CreateERIEncoder( void )
{
	return	new ERISAEncoder ;
}

//
// 音声圧縮オブジェクトを生成
//////////////////////////////////////////////////////////////////////////////
MIOEncoder * ERIAnimationWriter::CreateMIOEncoder( void )
{
	return	new MIOEncoder ;
}

//
// 出力された画像の枚数を取得する
//////////////////////////////////////////////////////////////////////////////
DWORD ERIAnimationWriter::GetWrittenFrameCount( void ) const
{
	return	m_dwFrameCount ;
}

//
// マルチスレッド圧縮を有効化
//////////////////////////////////////////////////////////////////////////////
void ERIAnimationWriter::EnableDualEncoding( bool fDualEncoding )
{
	m_fDualEncoding = fDualEncoding ;
	//
	if ( m_fDualEncoding && (m_hThread == NULL)
				&& (m_wsStatus == wsWritingStream) )
	{
		m_hCompressed = ::CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		if ( m_hCompressed == NULL )
		{
			return ;
		}
		m_hThread = ::CreateThread
			( NULL, 0, &ERIAnimationWriter::ThreadProc, this, 0, &m_idThread ) ;
		if ( m_hThread == NULL )
		{
			return ;
		}
		HANDLE	hEventList[2] ;
		hEventList[0] = m_hThread ;
		hEventList[1] = m_hCompressed ;
		DWORD	dwWaitResult =
			::WaitForMultipleObjects( 2, hEventList, FALSE, INFINITE ) ;
		if ( dwWaitResult == WAIT_OBJECT_0 )
		{
			::CloseHandle( m_hThread ) ;
			::CloseHandle( m_hCompressed ) ;
			m_hThread = NULL ;
			m_hCompressed = NULL ;
			return ;
		}
	}
}

//
// スレッド関数
//////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ERIAnimationWriter::ThreadProc( LPVOID parameter )
{
	return	((ERIAnimationWriter*)parameter)->EncodingThreadProc( ) ;
}

DWORD ERIAnimationWriter::EncodingThreadProc( void )
{
	::eriInitializeTask( ) ;
	//
	// メッセージキューを生成
	//
	MSG		msg ;
	::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ;
	m_fCompressSuccessed = false ;
	::SetEvent( m_hCompressed ) ;
	//
	while ( ::GetMessage( &msg, NULL, 0, 0 ) )
	{
		if ( msg.message == tmEncodeImage )
		{
			//
			// 画像を圧縮
			//
			EGL_IMAGE_INFO *	peii = (EGL_IMAGE_INFO*) msg.lParam ;
			DWORD				fdwFlags = msg.wParam ;
			//
			m_fCompressSuccessed =
				!m_perie2->EncodeImage( *peii, m_eric2, fdwFlags ) ;
			//
			::SetEvent( m_hCompressed ) ;
		}
		else if ( msg.message == tmQuit )
		{
			break ;
		}
	}
	//
	::eriCloseTask( ) ;

	return	0 ;
}


#endif
