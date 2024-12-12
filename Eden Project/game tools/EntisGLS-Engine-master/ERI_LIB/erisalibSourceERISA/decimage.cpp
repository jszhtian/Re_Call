
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2004 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>

#if	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                            画像展開オブジェクト
 *****************************************************************************/

#define	FREE_BUFFER(buf)	\
	if ( buf != NULL ) {  ::eslHeapFree( NULL, buf ) ;  buf = NULL ;  }

const ERISADecoder::PTR_PROCEDURE
	ERISADecoder::m_pfnColorOperation[0x10] =
{
	&ERISADecoder::ColorOperation0000,
	&ERISADecoder::ColorOperation0000,
	&ERISADecoder::ColorOperation0000,
	&ERISADecoder::ColorOperation0000,
	&ERISADecoder::ColorOperation0000,
	&ERISADecoder::ColorOperation0101,
	&ERISADecoder::ColorOperation0110,
	&ERISADecoder::ColorOperation0111,
	&ERISADecoder::ColorOperation0000,
	&ERISADecoder::ColorOperation1001,
	&ERISADecoder::ColorOperation1010,
	&ERISADecoder::ColorOperation1011,
	&ERISADecoder::ColorOperation0000,
	&ERISADecoder::ColorOperation1101,
	&ERISADecoder::ColorOperation1110,
	&ERISADecoder::ColorOperation1111
} ;

// クラス情報
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ERISADecoder, ESLObject )

// 構築関数
//////////////////////////////////////////////////////////////////////////////
ERISADecoder::ERISADecoder( void )
{
	m_ptrDstBlock = NULL ;
	m_ptrOperations = NULL ;
	m_ptrColumnBuf = NULL ;
	m_ptrLineBuf = NULL ;
	m_ptrDecodeBuf = NULL ;
	m_ptrArrangeBuf = NULL ;
	m_pArrangeTable[0] = NULL ;
	m_ptrVertBufLOT = NULL ;
	m_ptrHorzBufLOT = NULL ;
	m_ptrBlocksetBuf[0] = NULL ;
	m_ptrMatrixBuf = NULL ;
	m_ptrIQParamBuf = NULL ;
	m_ptrIQParamTable = NULL ;
	m_ptrBlockLineBuf = NULL ;
	m_ptrImageBuf = NULL ;
	m_ptrYUVImage = NULL ;
	m_ptrMovingVector = NULL ;
	m_ptrMoveVecFlags = NULL ;
	m_ptrMovePrevBlocks = NULL ;
	m_pPrevImageRef = NULL ;
	m_pNextImageRef = NULL ;
	m_pFilterImageBuf = NULL ;
	m_pHuffmanTree = NULL ;
	m_pProbERISA = NULL ;
}

// 消滅関数
//////////////////////////////////////////////////////////////////////////////
ERISADecoder::~ERISADecoder( void )
{
	Delete( ) ;
}

// 初期化（パラメータの設定）
//////////////////////////////////////////////////////////////////////////////
ESLError ERISADecoder::Initialize( const ERI_INFO_HEADER & infhdr )
{
	//
	// 以前のデータを消去
	//
	Delete( ) ;
	//
	// 画像情報ヘッダをコピー
	//
	m_eihInfo = infhdr ;
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		//
		// パラメータのチェック
		//
		if ( (m_eihInfo.dwArchitecture != ERI_RUNLENGTH_GAMMA)
				&& (m_eihInfo.dwArchitecture != ERI_RUNLENGTH_HUFFMAN)
				&& (m_eihInfo.dwArchitecture != ERISA_NEMESIS_CODE) )
		{
			return	eslErrGeneral ;		// エラー（未対応の圧縮フォーマット）
		}
		//
		switch ( (m_eihInfo.fdwFormatType & ERI_TYPE_MASK) )
		{
		case	ERI_RGB_IMAGE:
			if ( m_eihInfo.dwBitsPerPixel <= 8 )
				m_nChannelCount = 1 ;
			else if ( !(m_eihInfo.fdwFormatType & ERI_WITH_ALPHA) )
				m_nChannelCount = 3 ;
			else
				m_nChannelCount = 4 ;
			break ;

		case	ERI_GRAY_IMAGE:
			m_nChannelCount = 1 ;
			break;

		default:
			return	eslErrGeneral ;		// エラー（未対応の画像フォーマット）
		}
		//
		// 各定数を計算
		//
		if ( m_eihInfo.dwBlockingDegree == 0 )
		{
			return	eslErrGeneral ;		// エラー（未対応の画像フォーマット）
		}
		//
		m_nBlockSize = (ULONG) (1 << m_eihInfo.dwBlockingDegree) ;
		m_nBlockArea = (ULONG) (1 << (m_eihInfo.dwBlockingDegree * 2)) ;
		m_nBlockSamples = m_nBlockArea * m_nChannelCount ;
		m_nWidthBlocks =
			(ULONG) ((m_eihInfo.nImageWidth + m_nBlockSize - 1)
								>> m_eihInfo.dwBlockingDegree) ;
		if ( m_eihInfo.nImageHeight < 0 )
		{
			m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
		}
		else
		{
			m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
		}
		m_nHeightBlocks =
			(m_nHeightBlocks + m_nBlockSize - 1)
								>> m_eihInfo.dwBlockingDegree ;
		//
		// ワーキングメモリを確保
		//
		m_ptrOperations =
			(PBYTE) ::eslHeapAllocate
				( NULL, m_nWidthBlocks * m_nHeightBlocks, 0 ) ;
		m_ptrColumnBuf =
			(SBYTE*) ::eslHeapAllocate
				( NULL, m_nBlockSize * m_nChannelCount, 0 ) ;
		m_ptrLineBuf =
			(SBYTE*) ::eslHeapAllocate
				( NULL, m_nChannelCount
					* (m_nWidthBlocks << m_eihInfo.dwBlockingDegree), 0 ) ;
		m_ptrDecodeBuf =
			(SBYTE*) ::eslHeapAllocate( NULL, m_nBlockSamples, 0 ) ;
		m_ptrArrangeBuf =
			(SBYTE*) ::eslHeapAllocate( NULL, m_nBlockSamples, 0 ) ;
		//
		if ( !m_ptrOperations | !m_ptrColumnBuf
			| !m_ptrLineBuf | !m_ptrDecodeBuf | !m_ptrArrangeBuf )
		{
			return	eslErrGeneral ;		// エラー（メモリ不足）
		}
		//
		// バージョンのチェック
		//
		if ( m_eihInfo.dwVersion == 0x00020100 )
		{
			// 標準フォーマット
			m_fEnhancedMode = 0 ;
			//
			InitializeArrangeTable( ) ;
		}
		else if ( m_eihInfo.dwVersion == 0x00020200 )
		{
			// 拡張フォーマット
			m_fEnhancedMode = 2 ;
			//
			InitializeArrangeTable( ) ;
			//
			if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN )
			{
				m_pHuffmanTree = new ERINA_HUFFMAN_TREE ;
			}
			else if ( m_eihInfo.dwArchitecture == ERISA_NEMESIS_CODE )
			{
				m_pProbERISA = new ERISA_PROB_MODEL ;
			}
		}
		else
		{
			return	eslErrGeneral ;		// エラー（未対応のバージョン）
		}
	}
	else if ( (m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI) )
	{
		//
		// 非可逆圧縮
		//////////////////////////////////////////////////////////////////////
		if ( (m_eihInfo.dwArchitecture != ERI_RUNLENGTH_GAMMA)
				&& (m_eihInfo.dwArchitecture != ERI_RUNLENGTH_HUFFMAN)
				&& (m_eihInfo.dwArchitecture != ERISA_NEMESIS_CODE) )
		{
			return	eslErrGeneral ;		// エラー（未対応の圧縮フォーマット）
		}
		//
		switch ( (m_eihInfo.fdwFormatType & ERI_TYPE_MASK) )
		{
		case	ERI_RGB_IMAGE:
			if ( m_eihInfo.dwBitsPerPixel <= 8 )
				m_nChannelCount = 1 ;
			else if ( !(m_eihInfo.fdwFormatType & ERI_WITH_ALPHA) )
				m_nChannelCount = 3 ;
			else
				m_nChannelCount = 4 ;
			break ;

		case	ERI_GRAY_IMAGE:
			m_nChannelCount = 1 ;
			break;

		default:
			return	eslErrGeneral ;		// エラー（未対応の画像フォーマット）
		}
		//
		// 各定数を計算
		//
		if ( m_eihInfo.dwBlockingDegree != 3 )
		{
			return	eslErrGeneral ;		// エラー（未対応の画像フォーマット）
		}
		//
		m_nBlockSize = (UINT) (1 << m_eihInfo.dwBlockingDegree) ;
		m_nBlockArea = (ULONG) (1 << (m_eihInfo.dwBlockingDegree * 2)) ;
		m_nBlockSamples = m_nBlockArea * m_nChannelCount ;
		if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
		{
			m_nWidthBlocks =
				(ULONG) ((m_eihInfo.nImageWidth + m_nBlockSize * 2 - 1)
									>> (m_eihInfo.dwBlockingDegree + 1)) ;
			if ( m_eihInfo.nImageHeight < 0 )
			{
				m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
			}
			else
			{
				m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
			}
			m_nHeightBlocks =
				(m_nHeightBlocks + m_nBlockSize * 2 - 1)
							>> (m_eihInfo.dwBlockingDegree + 1) ;
			//
			m_nWidthBlocks += 1  ;
			m_nHeightBlocks += 1 ;
			//
			if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
			{
				m_nBlocksetCount = m_nChannelCount * 4 ;
			}
			else if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
			{
				switch ( m_nChannelCount )
				{
				case	1:
					m_nBlocksetCount = 4 ;
					break ;
				case	3:
					m_nBlocksetCount = 6 ;
					break ;
				case	4:
					m_nBlocksetCount = 10 ;
					break ;
				default:
					return	eslErrGeneral ;
				}
			}
			else
			{
				return	eslErrGeneral ;		// エラー（未対応の画像フォーマット）
			}
		}
		else
		{
			m_nWidthBlocks =
				(ULONG) ((m_eihInfo.nImageWidth + (m_nBlockSize * 2 - 1))
										>> (m_eihInfo.dwBlockingDegree + 1)) ;
			if ( m_eihInfo.nImageHeight < 0 )
			{
				m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
			}
			else
			{
				m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
			}
			m_nHeightBlocks =
				(m_nHeightBlocks + (m_nBlockSize * 2 - 1))
									>> (m_eihInfo.dwBlockingDegree + 1) ;
			//
			if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_4_4 )
			{
				m_nBlocksetCount = m_nChannelCount * 4 ;
			}
			else if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
			{
				switch ( m_nChannelCount )
				{
				case	1:
					m_nBlocksetCount = 4 ;
					break ;
				case	3:
					m_nBlocksetCount = 6 ;
					break ;
				case	4:
					m_nBlocksetCount = 10 ;
					break ;
				default:
					return	eslErrGeneral ;
				}
			}
			else
			{
				return	eslErrGeneral ;		// エラー（未対応の画像フォーマット）
			}
		}
		//
		// ワーキングメモリを確保
		//
		m_ptrDecodeBuf =
			(SBYTE*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 16 * sizeof(SBYTE), 0 ) ;
		m_ptrVertBufLOT =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockSamples * 2
					* m_nWidthBlocks * sizeof(REAL32), 0 ) ;
		m_ptrHorzBufLOT =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockSamples * 2 * sizeof(REAL32), 0 ) ;
		m_ptrBlocksetBuf[0] =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 16 * sizeof(REAL32), ESL_HEAP_ZERO_INIT ) ;
		m_ptrMatrixBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockArea * sizeof(REAL32) * 16, 0 ) ;
		m_ptrIQParamBuf =
			(REAL32*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 2 * sizeof(REAL32), 0 ) ;
		m_ptrIQParamTable =
			(BYTE*) ::eslHeapAllocate
				( NULL, m_nBlockArea * 2 * sizeof(BYTE), 0 ) ;
		//
		DWORD	dwTotalBlocks = m_nWidthBlocks * m_nHeightBlocks ;
		m_ptrOperations =
			(BYTE*) ::eslHeapAllocate( NULL, dwTotalBlocks * 2, 0 ) ;
		m_ptrImageBuf =
			(SBYTE*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * m_nBlockArea * m_nBlocksetCount, 0 ) ;
		m_ptrMovingVector =
			(SBYTE*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * 4 * sizeof(SBYTE), 0 ) ;
		m_ptrMoveVecFlags =
			(SBYTE*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * sizeof(BYTE), 0 ) ;
		m_ptrMovePrevBlocks =
			(PVOID*) ::eslHeapAllocate
				( NULL, dwTotalBlocks * 4 * sizeof(PVOID), 0 ) ;
		m_pPrevImageRef = NULL ;
		//
		for ( int i = 1; i < 16; i ++ )
		{
			m_ptrBlocksetBuf[i] = m_ptrBlocksetBuf[0] + (m_nBlockArea * i) ;
		}
		//
		// 中間画像バッファを生成
		//
		m_nYUVPixelBytes = m_nChannelCount ;
		if ( m_nYUVPixelBytes == 3 )
		{
			m_nYUVPixelBytes = 4 ;
		}
		m_nYUVLineBytes =
			((m_nYUVPixelBytes
				* m_nWidthBlocks * m_nBlockSize * 2) + 0x0F) & (~0x0F) ;
		ULONG	nYUVImageSize =
			m_nYUVLineBytes * m_nHeightBlocks * m_nBlockSize * 2 ;
		m_ptrBlockLineBuf = (SBYTE*)
			::eslHeapAllocate( NULL, m_nYUVLineBytes * 16, 0 ) ;
		m_ptrYUVImage = (SBYTE*) ::eslHeapAllocate( NULL, nYUVImageSize, 0 ) ;
		//
		if ( !m_ptrDecodeBuf | !m_ptrVertBufLOT | !m_ptrHorzBufLOT
			| !m_ptrBlocksetBuf[0] | !m_ptrMatrixBuf | !m_ptrIQParamBuf
			| !m_ptrIQParamTable | !m_ptrOperations | !m_ptrImageBuf
			| !m_ptrMovingVector | !m_ptrMovePrevBlocks )
		{
			return	eslErrGeneral ;		// エラー（メモリ不足）
		}
		//
		// サンプリングテーブルの準備
		//
		InitializeZigZagTable( ) ;
		//
		// 圧縮コンテキストの初期化
		//
		m_pHuffmanTree = new ERINA_HUFFMAN_TREE ;
		m_pProbERISA = new ERISA_PROB_MODEL ;
	}
	else
	{
		return	eslErrGeneral ;			// エラー（未対応のフォーマット）
	}

	//
	// 正常終了
	//
	return	eslErrSuccess ;
}

// 終了（メモリの解放など）
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::Delete( void )
{
	FREE_BUFFER(m_ptrOperations)
	FREE_BUFFER(m_ptrColumnBuf)
	FREE_BUFFER(m_ptrLineBuf)
	FREE_BUFFER(m_ptrDecodeBuf)
	FREE_BUFFER(m_ptrArrangeBuf)
	FREE_BUFFER(m_pArrangeTable[0])
	FREE_BUFFER(m_ptrVertBufLOT)
	FREE_BUFFER(m_ptrHorzBufLOT)
	FREE_BUFFER(m_ptrBlocksetBuf[0])
	FREE_BUFFER(m_ptrMatrixBuf)
	FREE_BUFFER(m_ptrIQParamBuf)
	FREE_BUFFER(m_ptrIQParamTable)
	FREE_BUFFER(m_ptrBlockLineBuf)
	FREE_BUFFER(m_ptrImageBuf)
	FREE_BUFFER(m_ptrYUVImage)
	FREE_BUFFER(m_ptrMovingVector)
	FREE_BUFFER(m_ptrMoveVecFlags)
	FREE_BUFFER(m_ptrMovePrevBlocks)
	//
	if ( m_pHuffmanTree != NULL )
	{
		delete	m_pHuffmanTree ;
		m_pHuffmanTree = NULL ;
	}
	if ( m_pProbERISA != NULL )
	{
		delete	m_pProbERISA ;
		m_pProbERISA = NULL ;
	}
}

// 画像を展開
//////////////////////////////////////////////////////////////////////////////
ESLError ERISADecoder::DecodeImage
	( const EGL_IMAGE_INFO & dstimginf,
		ERISADecodeContext & context, DWORD fdwFlags )
{
	//
	// 出力画像情報をコピーする
	//
	EGL_IMAGE_INFO	imginf = dstimginf ;
	bool	fReverse = ((fdwFlags & dfTopDown) != 0) ;
	if ( m_eihInfo.nImageHeight < 0 )
	{
		fReverse = ! fReverse ;
	}
	if ( fReverse )
	{
		imginf.ptrImageArray =
			((PBYTE)imginf.ptrImageArray)
				+ (imginf.dwImageHeight - 1) * imginf.dwBytesPerLine ;
		imginf.dwBytesPerLine = - imginf.dwBytesPerLine ;
	}
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOSSLESS_ERI )
	{
		// 可逆画像フォーマット
		return	DecodeLosslessImage( imginf, context, fdwFlags ) ;
	}
	else if ( (m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI)
			|| (m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI) )
	{
		// 非可逆画像フォーマット
		return	DecodeLossyImage( imginf, context, fdwFlags ) ;
	}

	return	eslErrGeneral ;			// 未対応のフォーマット
}

// 直前フレームへの参照を設定
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::SetRefPreviousFrame
	( PEGL_IMAGE_INFO pPrevFrame, PEGL_IMAGE_INFO pNextFrame )
{
	m_pPrevImageRef = pPrevFrame ;
	if ( pPrevFrame != NULL )
	{
		m_dwPrevLineBytes = pPrevFrame->dwBytesPerLine ;
	}
	m_pNextImageRef = pNextFrame ;
	if ( pNextFrame != NULL )
	{
		m_dwNextLineBytes = pNextFrame->dwBytesPerLine ;
	}
}

// フィルタ処理画像を受け取るバッファを設定する
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::SetFilteredImageBuffer( PEGL_IMAGE_INFO pImageBuf )
{
	m_pFilterImageBuf = pImageBuf ;
}

// 展開進行状況通知関数
//////////////////////////////////////////////////////////////////////////////
ESLError ERISADecoder::OnDecodedBlock
	( LONG line, LONG column, const EGL_IMAGE_RECT & rect )
{
	return	eslErrSuccess ;
}

// 可逆画像展開
//////////////////////////////////////////////////////////////////////////////
ESLError ERISADecoder::DecodeLosslessImage
	( const EGL_IMAGE_INFO & imginf,
		ERISADecodeContext & context, DWORD fdwFlags )
{
	//
	// 画像データヘッダを取得する
	//////////////////////////////////////////////////////////////////////////
	UINT	nERIVersion, fOpTable, fEncodeType, nBitCount ;
	//
	// ERI image data header (4 bytes) ;
	//	0000H : nERIVersion : ERI バージョン
	//		= 1 : ERI 互換フォーマット（第１水準）
	//		= 8 : ERINA フォーマット（第２水準）
	//		= 16 : ERISA フォーマット（第５水準）
	//	0001H : fOpTable : オペレーションテーブルの圧縮
	//		= 0 : 常に０
	//	0002H : fEncodeType : エンコード方式
	//		= 0 : ブロック独立型（推奨）
	//		= 1 : ブロック非独立型
	//	0003H : nBitCount : ビット深度
	//		= 0 : ERI 互換フォーマットでは予約（常に０）
	//		= 8 : ERINA, ERISA では符号のビット深度（常に８）
	//
	// 備考 ;
	// 　ERI ヘッダに続けてオペレーションテーブルが続く。
	// 　オペレーションコードは、ERI では固定長符号（4ビット）、
	// 　ERINA ではハフマン符号、ERISA では算術符号となる。
	// 　オペレーションコードの次に 1 ビットの '0' があり、
	// 　その後に実際の画像の符号が続く。
	// 　ただし、ERISA では常にブロック独立型が指定され、
	// 　オペレーションテーブルは存在しない。
	// 　また、ERISA 符号は先頭に 16 ビットの 0 が
	// 　付加されるので、ERI ヘッダの直後 17 ビットは
	// 　常に 0 である。
	//
	context.FlushBuffer( ) ;
	m_pFilterImageBuf = NULL ;
	//
	nERIVersion = context.GetNBits( 8 ) ;
	fOpTable = context.GetNBits( 8 ) ;
	fEncodeType = context.GetNBits( 8 ) ;
	nBitCount = context.GetNBits( 8 ) ;
	//
	if ( (fOpTable != 0) || (fEncodeType & 0xFE) )
	{
		return	eslErrGeneral ;			// 未対応のフォーマット
	}
	if ( nERIVersion == 1 )
	{
		// ERI 互換フォーマット（第１水準）
		if ( nBitCount != 0 )
		{
			return	eslErrGeneral ;		// 未対応のフォーマット
		}
	}
	else if ( nERIVersion == 8 )
	{
		// ERINA フォーマット（第２水準）
		if ( nBitCount != 8 )
		{
			return	eslErrGeneral ;		// 未対応のフォーマット
		}
	}
	else if ( nERIVersion == 16 )
	{
		// ERISA フォーマット（第５水準）
		if ( (nBitCount != 8) || (fEncodeType != 0) )
		{
			return	eslErrGeneral ;		// 未対応のフォーマット
		}
	}
	else
	{
		return	eslErrGeneral ;
	}
	if ( (fdwFlags & dfDifferential) && (m_pPrevImageRef != NULL) )
	{
		EGL_IMAGE_INFO	eiiPrev ;
		bool	fReverse = ((fdwFlags & dfTopDown) != 0) ;
		eiiPrev = *m_pPrevImageRef ;
		if ( m_eihInfo.nImageHeight < 0 )
		{
			fReverse = ! fReverse ;
		}
		if ( fReverse )
		{
			eiiPrev.ptrImageArray =
				((PBYTE)eiiPrev.ptrImageArray)
					+ (eiiPrev.dwImageHeight - 1) * eiiPrev.dwBytesPerLine ;
			eiiPrev.dwBytesPerLine = - eiiPrev.dwBytesPerLine ;
		}
		::eriCopyImage( imginf, eiiPrev ) ;
		m_pPrevImageRef = NULL ;
	}
	//
	// 関数を準備する
	//////////////////////////////////////////////////////////////////////////
	INT		i ;
	//
	// ストア関数取得
	//
	PTR_PROCEDURE	pfnRestoreFunc ;
	m_nDstPixelBytes = imginf.dwBitsPerPixel >> 3 ;
	m_nDstLineBytes = imginf.dwBytesPerLine ;
	pfnRestoreFunc = GetLLRestoreFunc
			( imginf.fdwFormatType, imginf.dwBitsPerPixel, fdwFlags ) ;
	if ( pfnRestoreFunc == NULL )
	{
		return	eslErrGeneral ;
	}
	//
	// オペレーションテーブルを取得する ;
	//	ERI 互換フォーマット :
	//		4 ビットでカラーオペレーション番号を指定する
	//		上位 4 ビットに 1100 を付加すると拡張フォーマットと同じ意味になる
	//	ERI 拡張フォーマット :
	//		DFC(2):ARC(2):COPN(4) のビットフォーマットで、
	//		DFC には差分コード、ARC にはアレンジコード、
	//		COPN にはカラーオペレーション番号を指定する。
	//
	if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN )
	{
		ESLAssert( m_pHuffmanTree != NULL ) ;
		m_pHuffmanTree->Initialize( ) ;
	}
	else if ( m_eihInfo.dwArchitecture == ERISA_NEMESIS_CODE )
	{
		ESLAssert( m_pProbERISA != NULL ) ;
		m_pProbERISA->Initialize( ) ;
	}
	PBYTE	ptrNextOperation = m_ptrOperations ;
	if ( (fEncodeType & 0x01) && (m_nChannelCount >= 3) )
	{
		ESLAssert( m_eihInfo.dwArchitecture != ERISA_NEMESIS_CODE ) ;
		LONG	nAllBlockCount = (LONG) (m_nWidthBlocks * m_nHeightBlocks) ;
		for ( i = 0; i < nAllBlockCount; i ++ )
		{
			if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA )
			{
				m_ptrOperations[i] = (BYTE) context.GetNBits(4) | 0xC0 ;
			}
			else
			{
				ESLAssert( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN ) ;
				m_ptrOperations[i] =
					(BYTE) context.GetHuffmanCode( m_pHuffmanTree ) ;
			}
		}
	}
	//
	// コンテキストを初期化する
	//
	if ( context.GetABit() != 0 )
	{
		return	eslErrGeneral ;			// 不正なフォーマット
	}
	if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA )
	{
		// ランレングス・ガンマ符号 (ERI 互換フォーマット)
		context.PrepareGammaCode( ) ;
		if ( fEncodeType & 0x01 )
		{
			context.InitGammaContext( ) ;
		}
	}
	else if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN )
	{
		// ランレングス・適応型１次ハフマン符号 (ERINA フォーマット)
		context.PrepareToDecodeERINACode( ) ;
	}
	else
	{
		// ERISA-N 符号 (ERISA フォーマット)
		ESLAssert( m_eihInfo.dwArchitecture == ERISA_NEMESIS_CODE ) ;
		context.PrepareToDecodeERISACode( ) ;
	}
	//
	// ラインバッファをクリアする
	//
	LONG	nWidthSamples = m_nChannelCount * m_nWidthBlocks * m_nBlockSize ;
	::eslFillMemory( m_ptrLineBuf, 0, nWidthSamples ) ;
	//
	// 各ブロックごとに復号して出力する反復処理
	//////////////////////////////////////////////////////////////////////////
	EGL_IMAGE_RECT	irBlockRect ;
	LONG	nPosX, nPosY ;
	LONG	nAllBlockLines = (LONG) (m_nBlockSize * m_nChannelCount) ;
	LONG	nLeftHeight = (LONG) imginf.dwImageHeight ;
	irBlockRect.y = 0 ;
	//
	for ( nPosY = 0; nPosY < (LONG) m_nHeightBlocks; nPosY ++ )
	{
		//
		// カラムバッファをクリアする
		//
		LONG	nColumnBufSamples = (LONG) (m_nBlockSize * m_nChannelCount) ;
		::eslFillMemory( m_ptrColumnBuf, 0, nColumnBufSamples ) ;
		//
		// 行の復号準備
		//
		if ( !(fdwFlags & dfPreviewDecode) )
		{
			m_ptrDstBlock = ((BYTE*) imginf.ptrImageArray)
							+ (nPosY * imginf.dwBytesPerLine * m_nBlockSize) ;
			m_nDstHeight = m_nBlockSize ;
			if ( (LONG) m_nDstHeight > nLeftHeight )
			{
				m_nDstHeight = nLeftHeight ;
			}
		}
		else
		{
			m_ptrDstBlock = ((BYTE*) imginf.ptrImageArray)
							+ (nPosY * imginf.dwBytesPerLine) ;
			m_nDstHeight = 1 ;
		}
		irBlockRect.h = m_nDstHeight ;
		//
		LONG	nLeftWidth = (LONG) imginf.dwImageWidth ;
		SBYTE *	ptrNextLineBuf = m_ptrLineBuf ;
		irBlockRect.x = 0 ;
		//
		for ( nPosX = 0; nPosX < (LONG) m_nWidthBlocks; nPosX ++ )
		{
			//
			// １つのブロックを復号して出力
			//////////////////////////////////////////////////////////////////
			//
			// ブロックの情報を正規化
			//
			if ( !(fdwFlags & dfPreviewDecode) )
			{
				m_nDstWidth = m_nBlockSize ;
				if ( (LONG) m_nDstWidth > nLeftWidth )
				{
					m_nDstWidth = nLeftWidth ;
				}
			}
			else
			{
				m_nDstWidth = 1 ;
			}
			irBlockRect.w = m_nDstWidth ;
			//
			// オペレーションコードを取得
			//
			DWORD	dwOperationCode ;
			if ( m_nChannelCount >= 3 )
			{
				if ( fEncodeType & 0x01 )
				{
					dwOperationCode = *(ptrNextOperation ++) ;
				}
				else if ( m_eihInfo.dwArchitecture == ERISA_NEMESIS_CODE )
				{
					dwOperationCode =
						context.DecodeERISACode( m_pProbERISA ) ;
				}
				else if ( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_HUFFMAN )
				{
					dwOperationCode =
						context.GetHuffmanCode( m_pHuffmanTree ) ;
				}
				else
				{
					ESLAssert( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA ) ;
					dwOperationCode = context.GetNBits(4) | 0xC0 ;
					context.InitGammaContext( ) ;
				}
			}
			else
			{
				if ( m_eihInfo.fdwFormatType == ERI_GRAY_IMAGE )
				{
					dwOperationCode = 0xC0 ;
				}
				else
				{
					dwOperationCode = 0x00 ;
				}
				if ( !(fEncodeType & 0x01)
					&& (m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA) )
				{
					context.InitGammaContext( ) ;
				}
			}
			//
			// 符号を復号する
			//
			if ( context.DecodeSymbolBytes
				( m_ptrArrangeBuf, m_nBlockSamples ) < m_nBlockSamples )
			{
				return	eslErrGeneral ;			// エラー（デコードに失敗）
			}
			//
			// オペレーションを実行する
			//
			PerformOperation
				( dwOperationCode, nAllBlockLines, ptrNextLineBuf ) ;
			ptrNextLineBuf += nColumnBufSamples ;
			//
			// 処理結果を出力バッファにストア
			//
			(this->*pfnRestoreFunc)( ) ;
			//
			// 展開の状況を通知
			//
			ESLError	errContinue ;
			errContinue = OnDecodedBlock( nPosY, nPosX, irBlockRect ) ;
			if ( errContinue )
			{
				return	errContinue ;
			}
			//
			// 次のブロックへ移動
			//
			if ( !(fdwFlags & dfPreviewDecode) )
			{
				m_ptrDstBlock += m_nDstPixelBytes * m_nBlockSize ;
			}
			else
			{
				m_ptrDstBlock += m_nDstPixelBytes ;
			}
			irBlockRect.x += m_nBlockSize ;
			nLeftWidth -= (LONG) m_nBlockSize ;
		}
		//
		irBlockRect.y += m_nBlockSize ;
		nLeftHeight -= (LONG) m_nBlockSize ;
	}
	//
	return	eslErrSuccess ;			// 正常終了
}

// アレンジテーブルの初期化
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::InitializeArrangeTable( void )
{
	unsigned int	i, j, k, l, m ;
	//
	// サンプリングテーブル用バッファ確保
	//
	PINT	ptrTable =
		(PINT) ::eslHeapAllocate
			( NULL, m_nBlockSamples * 4 * sizeof(INT), 0 ) ;
	PINT	ptrNext ;
	m_pArrangeTable[0] = ptrTable ;
	m_pArrangeTable[1] = ptrTable + m_nBlockSamples ;
	m_pArrangeTable[2] = ptrTable + m_nBlockSamples * 2 ;
	m_pArrangeTable[3] = ptrTable + m_nBlockSamples * 3 ;
	//
	// 水平方向走査
	//
	ptrNext = m_pArrangeTable[0] ;
	for ( i = 0; i < m_nBlockSamples; i ++ )
	{
		ptrNext[i] = i ;
	}
	//
	// 垂直方向走査
	//
	ptrNext = m_pArrangeTable[1] ;
	l = 0 ;
	for ( i = 0; i < m_nChannelCount; i ++ )
	{
		for ( j = 0; j < m_nBlockSize; j ++ )
		{
			m = l + j ;
			for ( k = 0; k < m_nBlockSize; k ++ )
			{
				*(ptrNext ++) = m ;
				m += m_nBlockSize ;
			}
		}
		l += m_nBlockArea ;
	}
	//
	// 水平方向インターリーブ
	//
	ptrNext = m_pArrangeTable[2] ;
	for ( i = 0; i < m_nBlockArea; i ++ )
	{
		k = i ;
		for ( j = 0; j < m_nChannelCount; j ++ )
		{
			*(ptrNext ++) = k ;
			k += m_nBlockArea ;
		}
	}
	//
	// 垂直方向インターリーブ
	//
	ptrNext = m_pArrangeTable[3] ;
	for ( i = 0; i < m_nBlockSize; i ++ )
	{
		l = i ;
		for ( j = 0; j < m_nBlockSize; j ++ )
		{
			m = l ;
			l += m_nBlockSize ;
			for ( k = 0; k < m_nChannelCount; k ++ )
			{
				*(ptrNext ++) = m ;
				m += m_nBlockArea ;
			}
		}
	}
}

// グレイ画像／256色画像の出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::RestoreGray8( void )
{
	BYTE *	ptrDstLine = m_ptrDstBlock ;
	SBYTE *	ptrSrcLine = m_ptrDecodeBuf ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		for ( UINT x = 0; x < m_nDstWidth; x ++ )
		{
			ptrDstLine[x] = (BYTE) ptrSrcLine[x] ;
		}
		ptrSrcLine += m_nBlockSize ;
		ptrDstLine += m_nDstLineBytes ;
	}
}

// RGB 画像（15ビット）の出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::RestoreRGB16( void )
{
	BYTE *	ptrDstLine = m_ptrDstBlock ;
	SBYTE *	ptrSrcLine = m_ptrDecodeBuf ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		WORD *	ptrDstNext = (WORD*) ptrDstLine ;
		SBYTE *	ptrSrcNext = ptrSrcLine ;
		//
		for ( UINT x = 0; x < m_nDstWidth; x ++ )
		{
			*(ptrDstNext ++) =
				((WORD) *ptrSrcNext & 0x1F) |
				(((WORD) ptrSrcNext[nBlockSamples] & 0x1F) << 5) |
				(((WORD) ptrSrcNext[nBlockSamples * 2] & 0x1F) << 10) ;
			ptrSrcNext ++ ;
		}
		ptrSrcLine += m_nBlockSize ;
		ptrDstLine += m_nDstLineBytes ;
	}
}

// RGB 画像の出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::RestoreRGB24( void )
{
	BYTE *	ptrDstLine = m_ptrDstBlock ;
	SBYTE *	ptrSrcLine = m_ptrDecodeBuf ;
	UINT	nBytesPerPixel = m_nDstPixelBytes ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		BYTE *	ptrDstNext = ptrDstLine ;
		SBYTE *	ptrSrcNext = ptrSrcLine ;
		//
		for ( UINT x = 0; x < m_nDstWidth; x ++ )
		{
			ptrDstNext[0] = (BYTE) *ptrSrcNext ;
			ptrDstNext[1] = (BYTE) ptrSrcNext[nBlockSamples] ;
			ptrDstNext[2] = (BYTE) ptrSrcNext[nBlockSamples * 2] ;
			ptrSrcNext ++ ;
			ptrDstNext += nBytesPerPixel ;
		}
		ptrSrcLine += m_nBlockSize ;
		ptrDstLine += m_nDstLineBytes ;
	}
}

// RGBA 画像の出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::RestoreRGBA32( void )
{
	BYTE *	ptrDstLine = m_ptrDstBlock ;
	SBYTE *	ptrSrcLine = m_ptrDecodeBuf ;
	UINT	nBytesPerPixel = m_nDstPixelBytes ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	LONG	nBlockSamplesX3 = nBlockSamples * 3 ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		BYTE *	ptrDstNext = ptrDstLine ;
		SBYTE *	ptrSrcNext = ptrSrcLine ;
		//
		for ( UINT x = 0; x < m_nDstWidth; x ++ )
		{
			ptrDstNext[0] = (BYTE) *ptrSrcNext ;
			ptrDstNext[1] = (BYTE) ptrSrcNext[nBlockSamples] ;
			ptrDstNext[2] = (BYTE) ptrSrcNext[nBlockSamples * 2] ;
			ptrDstNext[3] = (BYTE) ptrSrcNext[nBlockSamplesX3] ;
			ptrSrcNext ++ ;
			ptrDstNext += 4 ;
		}
		ptrSrcLine += m_nBlockSize ;
		ptrDstLine += m_nDstLineBytes ;
	}
}

// RGB 画像の差分出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LL_RestoreDeltaRGB24( void )
{
	BYTE *	ptrDstLine = m_ptrDstBlock ;
	SBYTE *	ptrSrcLine = m_ptrDecodeBuf ;
	UINT	nBytesPerPixel = m_nDstPixelBytes ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		BYTE *	ptrDstNext = ptrDstLine ;
		SBYTE *	ptrSrcNext = ptrSrcLine ;
		//
		for ( UINT x = 0; x < m_nDstWidth; x ++ )
		{
			ptrDstNext[0] += (BYTE) *ptrSrcNext ;
			ptrDstNext[1] += (BYTE) ptrSrcNext[nBlockSamples] ;
			ptrDstNext[2] += (BYTE) ptrSrcNext[nBlockSamples * 2] ;
			ptrSrcNext ++ ;
			ptrDstNext += nBytesPerPixel ;
		}
		ptrSrcLine += m_nBlockSize ;
		ptrDstLine += m_nDstLineBytes ;
	}
}

// RGBA 画像の差分出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LL_RestoreDeltaRGBA32( void )
{
	BYTE *	ptrDstLine = m_ptrDstBlock ;
	SBYTE *	ptrSrcLine = m_ptrDecodeBuf ;
	UINT	nBytesPerPixel = m_nDstPixelBytes ;
	LONG	nBlockSamples = (LONG) m_nBlockArea ;
	LONG	nBlockSamplesX3 = nBlockSamples * 3 ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		BYTE *	ptrDstNext = ptrDstLine ;
		SBYTE *	ptrSrcNext = ptrSrcLine ;
		//
		for ( UINT x = 0; x < m_nDstWidth; x ++ )
		{
			ptrDstNext[0] += (BYTE) *ptrSrcNext ;
			ptrDstNext[1] += (BYTE) ptrSrcNext[nBlockSamples] ;
			ptrDstNext[2] += (BYTE) ptrSrcNext[nBlockSamples * 2] ;
			ptrDstNext[3] += (BYTE) ptrSrcNext[nBlockSamplesX3] ;
			ptrSrcNext ++ ;
			ptrDstNext += 4 ;
		}
		ptrSrcLine += m_nBlockSize ;
		ptrDstLine += m_nDstLineBytes ;
	}
}

// 画像出力関数取得
//////////////////////////////////////////////////////////////////////////////
ERISADecoder::PTR_PROCEDURE ERISADecoder::GetLLRestoreFunc
	( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags )
{
	switch ( dwBitsPerPixel )
	{
	case	32:
		if ( m_eihInfo.fdwFormatType == ERI_RGBA_IMAGE )
		{
			if ( !(fdwFlags & dfDifferential) )
				return	&ERISADecoder::RestoreRGBA32 ;
			else
				return	&ERISADecoder::LL_RestoreDeltaRGBA32 ;
		}
	case	24:
		if ( !(fdwFlags & dfDifferential) )
			return	&ERISADecoder::RestoreRGB24 ;
		else
			return	&ERISADecoder::LL_RestoreDeltaRGB24 ;
	case	16:
		return	&ERISADecoder::RestoreRGB16 ;
	case	8:
		return	&ERISADecoder::RestoreGray8 ;
	}
	return	NULL ;
}

// 非可逆画像展開
//////////////////////////////////////////////////////////////////////////////
ESLError ERISADecoder::DecodeLossyImage
	( const EGL_IMAGE_INFO & imginf,
		ERISADecodeContext & context, DWORD fdwFlags )
{
	//
	// 画像データヘッダを取得する
	//////////////////////////////////////////////////////////////////////////
	UINT	nERIVersion, fOpTable, fEncodeType, nBitCount ;
	//
	// ERI image data header (4 bytes) ;
	//	0000H : nERIVersion : ERI バージョン
	//		= 21H : ガンマ符号（未使用）
	//		= 28H : ERINA 符号
	//		= 30H : ERISA 符号（未使用）
	//	0001H : fOpTable : オペレーションテーブル
	//		=  0H : デフォルト
	//		|  1H : 動き補償ベクトル（動画用：P フレーム）
	//		|  2H : 動き補償ベクトル（動画用：B フレーム）
	//		|  4H : 4-2-1 ループフィルタ（画面全体）
	//		|  8H : 4-2-1 ループフィルタ（イントラブロックのみ）
	//	0002H : fEncodeType : エンコード方式
	//		= 0 : デフォルト
	//		= 1 : DCT 変換を使用（差分フレーム用）
	//	0003H : nBitCount : ビット深度
	//		= 0 : ガンマ符号では予約（常に０）
	//		= 8 : ERINA, ERISA では符号のビット深度（常に８）
	//
	// 備考；
	//		ERI ヘッダに続けて次のデータ（テーブル）が続く
	//			・逆量子化係数テーブル
	//			・ブロックスケーリング係数
	//			・動き補償ベクトルテーブル
	//			・画像データ
	//		逆量子化テーブルはハフマン符号で、画像データは
	//		ランレングス付ハフマン符号で、それ以外は
	//		ランレングスガンマ符号で符号化される
	//
	context.FlushBuffer( ) ;
	//
	nERIVersion = context.GetNBits( 8 ) ;
	fOpTable = context.GetNBits( 8 ) ;
	fEncodeType = context.GetNBits( 8 ) ;
	nBitCount = context.GetNBits( 8 ) ;
	//
	// 関数を準備する
	//////////////////////////////////////////////////////////////////////////
	if ( fdwFlags & dfQualityDecode )
	{
		fdwFlags &= ~dfQuickDecode ;
	}
	//
	// LOT 変換・DCT 変換の切り替え
	//
	DWORD	fdwOrgTrans = m_eihInfo.fdwTransformation ;
	CalcImageSizeInBlocks
		( (fEncodeType == 1) ? CVTYPE_DCT_ERI : fdwOrgTrans ) ;
	//
	// 出力先画像バッファを設定
	//
	m_ptrDstBlock = (BYTE*) imginf.ptrImageArray ;
	m_nDstPixelBytes = imginf.dwBitsPerPixel >> 3 ;
	m_nDstLineBytes = imginf.dwBytesPerLine ;
	m_nDstWidth = imginf.dwImageWidth ;
	m_nDstHeight = imginf.dwImageHeight ;
	m_fdwDecFlags = fdwFlags ;
	//
	// ストア関数取得
	//
	PTR_PROCEDURE	pfnRestoreFunc ;
	pfnRestoreFunc = GetLSRestoreFunc
			( imginf.fdwFormatType, imginf.dwBitsPerPixel, fdwFlags ) ;
	if ( pfnRestoreFunc == NULL )
	{
		return	eslErrGeneral ;
	}
	//
	// コンテキストを初期化する
	//
	if ( context.GetABit() != 0 )
	{
		return	eslErrGeneral ;			// 不正なフォーマット
	}
	if ( nERIVersion == 0x28 )
	{
		ESLAssert( m_eihInfo.dwArchitecture == ERI_RUNLENGTH_GAMMA ) ;
		ESLAssert( m_pHuffmanTree != NULL ) ;
		m_pHuffmanTree->Initialize( ) ;
		context.PrepareToDecodeERINACode( context.efERINAOrder0 ) ;
	}
	else
	{
		return	eslErrGeneral ;			// 未対応のフォーマット
	}
	//
	// 符号を復号
	//////////////////////////////////////////////////////////////////////////
	//
	// 逆量子化テーブルを取得
	//
	int		i ;
	for ( i = 0; i < (int) m_nBlockArea * 2; i ++ )
	{
		m_ptrIQParamTable[i] =
			(BYTE) context.GetHuffmanCode( m_pHuffmanTree ) ;
	}
	//
	// ブロックスケーリング係数を取得
	//
	LONG	nTotalBlocks = m_nWidthBlocks * m_nHeightBlocks ;
	ULONG	nTotalSamples = nTotalBlocks * m_nBlockArea * m_nBlocksetCount ;
	context.InitGammaContext( ) ;
	if ( context.DecodeGammaCodeBytes
		( (SBYTE*) m_ptrOperations, nTotalBlocks * 2 )
								< (ULONG) nTotalBlocks * 2 )
	{
		return	eslErrGeneral ;
	}
	//
	// 動き補償ベクトルを取得
	//
	ESLAssert( m_nBlockSize == 8 ) ;
	const int	nBlockSize = 16 ;
	LONG	nWidthDivBlocks =
		(imginf.dwImageWidth + (nBlockSize - 1)) / nBlockSize ;
	LONG	nHeightDivBlocks =
		(imginf.dwImageHeight + (nBlockSize - 1)) / nBlockSize ;
	LONG	nTotalDivBlocks = nWidthDivBlocks * nHeightDivBlocks ;
	LONG	nPosX, nPosY ;
	//
	if ( fOpTable & 0x01 )
	{
		context.InitGammaContext( ) ;
		if ( context.DecodeGammaCodeBytes
			( m_ptrMoveVecFlags, nTotalDivBlocks )
									< (ULONG) nTotalDivBlocks )
		{
			return	eslErrGeneral ;
		}
		context.InitGammaContext( ) ;
		if ( context.DecodeGammaCodeBytes
			( m_ptrMovingVector, nTotalDivBlocks * 4 )
							< (ULONG) nTotalDivBlocks * 4 )
		{
			return	eslErrGeneral ;
		}
	}
	else if ( fdwFlags & dfDifferential )
	{
		::eslFillMemory( m_ptrMoveVecFlags, 1, nTotalDivBlocks ) ;
		::eslFillMemory( m_ptrMovingVector, 0, nTotalDivBlocks * 4 ) ;
	}
	if ( fdwFlags & dfDifferential )
	{
		ESLAssert( m_pPrevImageRef != NULL ) ;
		if ( m_pPrevImageRef == NULL )
		{
			m_pPrevImageRef = (PEGL_IMAGE_INFO) &imginf ;
		}
		SetupMovingVector( ) ;
	}
	//
	// 画像信号を復号
	//
/*	if ( context.DecodeSymbolBytes
		( m_ptrImageBuf, nTotalSamples ) < nTotalSamples )
	{
		return	eslErrGeneral ;
	}*/
	ULONG	nLineBlockSamples =
		m_nWidthBlocks * m_nBlockArea * m_nBlocksetCount ;
	//
	// 各ブロックごとに復号処理
	//////////////////////////////////////////////////////////////////////////
	EGL_IMAGE_RECT	irBlockRect = { 0, 0, 0, 0 } ;
	SBYTE *	ptrSrcData = m_ptrImageBuf ;
	SBYTE *	ptrQParam = (SBYTE*) m_ptrOperations ;
	//
	// バッファを初期化
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
	{
		::eslFillMemory
			( m_ptrVertBufLOT, 0,
				m_nBlockSamples * 2 * m_nWidthBlocks * sizeof(REAL32) ) ;
	}
	//
	// 展開アルゴリズムを選択する
	//
	void (ERISADecoder::*pfnBlockMatrix)( REAL32 * ) ;
	void (ERISADecoder::*pfnBlockScaling)( int x, int y, DWORD fdwFlags ) ;
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
	{
		pfnBlockMatrix = &ERISADecoder::MatrixILOT8x8 ;
	}
	else
	{
		pfnBlockMatrix = &ERISADecoder::MatrixIDCT8x8 ;
	}
	if ( m_eihInfo.dwSamplingFlags == ERISF_YUV_4_1_1 )
	{
		pfnBlockScaling = &ERISADecoder::BlockScaling411 ;
	}
	else
	{
		pfnBlockScaling = &ERISADecoder::BlockScaling444 ;
	}
	//
	m_ptrNextPrevBlocks = m_ptrMovePrevBlocks ;
	//
	for ( nPosY = 0; nPosY < (LONG) m_nHeightBlocks; nPosY ++ )
	{
		//
		// LOT 変換バッファをクリア
		//
		if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
		{
			::eslFillMemory
				( m_ptrHorzBufLOT, 0,
					m_nBlockSamples * 2 * sizeof(REAL32) ) ;
		}
		REAL32 *	ptrVertBufLOT = m_ptrVertBufLOT ;
		m_ptrNextBlockBuf = m_ptrBlockLineBuf ;
		//
		// 画像信号を復号
		//
		if ( context.DecodeSymbolBytes
			( m_ptrImageBuf, nLineBlockSamples ) < nLineBlockSamples )
		{
			return	eslErrGeneral ;
		}
		ptrSrcData = m_ptrImageBuf ;
		//
		for ( nPosX = 0; nPosX < (LONG) m_nWidthBlocks; nPosX ++ )
		{
			//
			// 逆量子化
			//
			ArrangeAndIQuantumize( ptrSrcData, ptrQParam ) ;
			ptrSrcData += m_nBlockArea * m_nBlocksetCount ;
			ptrQParam += 2 ;
			//
			// 逆 LOT/DCT 変換
			//
			(this->*pfnBlockMatrix)( ptrVertBufLOT ) ;
			ptrVertBufLOT += m_nBlockArea * 2 * m_nChannelCount ;
			//
			// 画像スケーリング
			//
			(this->*pfnBlockScaling)( nPosX, nPosY, fdwFlags ) ;
			//
			// 展開の状況を通知
			//
			ESLError	errContinue ;
			errContinue = OnDecodedBlock( nPosY, nPosX, irBlockRect ) ;
			if ( errContinue )
			{
				return	errContinue ;
			}
		}
		//
		// 逆 YUV 変換・画像出力
		//
#if	defined(ERI_INTEL_X86)
		if ( nPosY || (m_eihInfo.fdwTransformation == CVTYPE_DCT_ERI) )
		{
			int	nDstHeight = imginf.dwImageHeight - nPosY * 16 ;
			if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
			{
				nDstHeight += 16 ;
			}
			if ( nDstHeight > 16 )
			{
				nDstHeight = 16 ;
			}
			m_nDstHeight = nDstHeight ;
			//
			(this->*pfnRestoreFunc)( ) ;
			//
			m_ptrDstBlock += m_nDstLineBytes * 16 ;
			m_ptrNextPrevBlocks += m_nWidthBlocks * 4 ;
		}
#endif
	}
	//
	// 逆 YUV 変換・画像出力
	//
#if	!defined(ERI_INTEL_X86)
	(this->*pfnRestoreFunc)( ) ;
#endif
	//
	if ( !(fdwFlags & dfQuickDecode) && !(fdwFlags & dfNoLoopFilter) )
	{
		if ( (fOpTable & 0x04) || (fdwFlags & dfUseLoopFilter) )
		{
			// 画面全体への 4-2-1 ループフィルタ
			if ( m_pFilterImageBuf != NULL )
			{
				EGL_IMAGE_INFO	filterbuf = *m_pFilterImageBuf ;
				bool	fReverse = ((fdwFlags & dfTopDown) != 0) ;
				if ( m_eihInfo.nImageHeight < 0 )
				{
					fReverse = ! fReverse ;
				}
				if ( fReverse )
				{
					filterbuf.ptrImageArray =
						((PBYTE)filterbuf.ptrImageArray)
							+ (filterbuf.dwImageHeight - 1)
									* filterbuf.dwBytesPerLine ;
					filterbuf.dwBytesPerLine = - filterbuf.dwBytesPerLine ;
				}
				::eriImageFilterLoop421( filterbuf, &imginf ) ;
			}
		}
		else if ( fOpTable & 0x08 )
		{
			// イントラブロックのみのループフィルタ
			ESLAssert( m_nBlockSize == 8 ) ;
			::eriImageFilterLoop421
				( imginf, &imginf, m_ptrMoveVecFlags, 16 ) ;
			m_pFilterImageBuf = NULL ;
		}
		else
		{
			m_pFilterImageBuf = NULL ;
		}
	}
	else
	{
		m_pFilterImageBuf = NULL ;
	}
	//
#if	defined(ERI_INTEL_X86)
	if ( ERI_EnabledProcessorType & (ERI_USE_MMX_PENTIUM | ERI_USE_XMM_P3) )
	{
		__asm
		{
			emms
		}
	}
#endif
	m_eihInfo.fdwTransformation = fdwOrgTrans ;
	//
	return	eslErrSuccess ;
}

// ブロック単位での画面サイズを計算する
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::CalcImageSizeInBlocks( DWORD fdwTransformation )
{
	m_eihInfo.fdwTransformation = fdwTransformation ;
	//
	if ( m_eihInfo.fdwTransformation == CVTYPE_LOT_ERI )
	{
		m_nWidthBlocks =
			(ULONG) ((m_eihInfo.nImageWidth + (m_nBlockSize * 2 - 1))
									>> (m_eihInfo.dwBlockingDegree + 1)) ;
		if ( m_eihInfo.nImageHeight < 0 )
		{
			m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
		}
		else
		{
			m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
		}
		m_nHeightBlocks =
			(m_nHeightBlocks + (m_nBlockSize * 2 - 1))
								>> (m_eihInfo.dwBlockingDegree + 1) ;
		//
		m_nWidthBlocks += 1  ;
		m_nHeightBlocks += 1 ;
	}
	else
	{
		m_nWidthBlocks =
			(ULONG) ((m_eihInfo.nImageWidth + (m_nBlockSize * 2 - 1))
									>> (m_eihInfo.dwBlockingDegree + 1)) ;
		if ( m_eihInfo.nImageHeight < 0 )
		{
			m_nHeightBlocks = (ULONG) - m_eihInfo.nImageHeight ;
		}
		else
		{
			m_nHeightBlocks = (ULONG) m_eihInfo.nImageHeight ;
		}
		m_nHeightBlocks =
			(m_nHeightBlocks + (m_nBlockSize * 2 - 1))
								>> (m_eihInfo.dwBlockingDegree + 1) ;
	}
}

// サンプリングテーブルの初期化
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::InitializeZigZagTable( void )
{
	PINT	ptrArrange = (PINT)
		::eslHeapAllocate( NULL, m_nBlockArea * sizeof(INT), 0 ) ;
	m_pArrangeTable[0] = ptrArrange ;
	//
	unsigned int	i = 0 ;
	INT		x = 0, y = 0 ;
	for ( ; ; )
	{
		for ( ; ; )
		{
			ptrArrange[i ++] = x + y * m_nBlockSize ;
			if ( i >= m_nBlockArea )
				return ;
			++ x ;
			-- y ;
			if ( x >= (INT) m_nBlockSize )
			{
				-- x ;
				y += 2 ;
				break ;
			}
			else if ( y < 0 )
			{
				y = 0 ;
				break ;
			}
		}
		for ( ; ; )
		{
			ptrArrange[i ++] = x + y * m_nBlockSize ;
			if ( i >= m_nBlockArea )
				return ;
			++ y ;
			-- x ;
			if ( y >= (INT) m_nBlockSize )
			{
				-- y ;
				x += 2 ;
				break ;
			}
			else if ( x < 0 )
			{
				x = 0 ;
				break ;
			}
		}
	}
}

// 動きベクトルをセットアップする
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::SetupMovingVector( void )
{
	//
	// パラメータ計算
	//
	ESLAssert( m_nBlockSize == 8 ) ;
	const int	nBlockSize = 16 ;
	LONG	nWidthDivBlocks = 
				(m_nDstWidth + (nBlockSize - 1)) / nBlockSize ;
	LONG	nHeightDivBlocks =
				(m_nDstHeight + (nBlockSize - 1)) / nBlockSize ;
	LONG	nTotalDivBlocks = nWidthDivBlocks * nHeightDivBlocks ;
	LONG	nPosX, nPosY ;
	//
	// 垂直方向差分の復元
	//
	SBYTE *	ptrLastVector = m_ptrMovingVector ;
	SBYTE *	ptrNextVector = ptrLastVector + nWidthDivBlocks * 4 ;
	for ( nPosY = 1; nPosY < nHeightDivBlocks; nPosY ++ )
	{
		for ( nPosX = 0; nPosX < nWidthDivBlocks; nPosX ++ )
		{
			ptrNextVector[0] += ptrLastVector[0] ;
			ptrNextVector[1] += ptrLastVector[1] ;
			ptrNextVector[2] += ptrLastVector[2] ;
			ptrNextVector[3] += ptrLastVector[3] ;
			ptrLastVector += 4 ;
			ptrNextVector += 4 ;
		}
	}
	//
	// 水平方向差分の復元
	//
	ptrNextVector = m_ptrMovingVector ;
	for ( nPosY = 0; nPosY < nHeightDivBlocks; nPosY ++ )
	{
		for ( nPosX = 1; nPosX < nWidthDivBlocks; nPosX ++ )
		{
			ptrNextVector[4] += ptrNextVector[0] ;
			ptrNextVector[5] += ptrNextVector[1] ;
			ptrNextVector[6] += ptrNextVector[2] ;
			ptrNextVector[7] += ptrNextVector[3] ;
			ptrNextVector += 4 ;
		}
		ptrNextVector += 4 ;
	}
	//
	// 参照ブロックへのポインタを算出する
	//
	ESLAssert( m_pPrevImageRef != NULL ) ;
	PVOID *	ptrNextBlockAddr = m_ptrMovePrevBlocks ;
	BYTE *	ptrPrevImage = (BYTE*) m_pPrevImageRef->ptrImageArray ;
	SBYTE *	ptrNextVecFlag = m_ptrMoveVecFlags ;
	LONG	nPrevPixelBytes = m_pPrevImageRef->dwBitsPerPixel / 8 ;
	LONG	nPrevLineBytes = m_pPrevImageRef->dwBytesPerLine ;
	BYTE *	ptrNextImage = NULL ;
	LONG	nNextPixelBytes = 0, nNextLineBytes = 0 ;
	if ( m_pNextImageRef != NULL )
	{
		ptrNextImage = (BYTE*) m_pNextImageRef->ptrImageArray ;
		nNextPixelBytes = m_pNextImageRef->dwBitsPerPixel / 8 ;
		nNextLineBytes = m_pNextImageRef->dwBytesPerLine ;
	}
	bool	fReverse = ((m_fdwDecFlags & dfTopDown) != 0) ;
	if ( m_eihInfo.nImageHeight < 0 )
	{
		fReverse = ! fReverse ;
	}
	if ( fReverse )
	{
		ptrPrevImage +=
				(m_pPrevImageRef->dwImageHeight - 1) * nPrevLineBytes ;
		nPrevLineBytes = - nPrevLineBytes ;
		//
		if ( m_pNextImageRef != NULL )
		{
			ptrNextImage +=
				(m_pNextImageRef->dwImageHeight - 1) * nNextLineBytes ;
			nNextLineBytes = - nNextLineBytes ;
		}
	}
	m_dwPrevLineBytes = nPrevLineBytes ;
	m_dwNextLineBytes = nNextLineBytes ;
	ptrNextVector = m_ptrMovingVector ;
	//
	for ( nPosY = 0; nPosY < nHeightDivBlocks; nPosY ++ )
	{
		for ( nPosX = 0; nPosX < nWidthDivBlocks; nPosX ++ )
		{
			EGL_POINT	ptRefPos ;
			int			nPrevType = *(ptrNextVecFlag ++) ;
			ptrNextBlockAddr[0] = NULL ;
			ptrNextBlockAddr[1] = NULL ;
			//
			if ( (nPrevType == 1) || (nPrevType == 2) )
			{
				//
				// 直前フレーム参照
				//
				ptRefPos.x = nPosX * nBlockSize
								+ ((int) ptrNextVector[0] >> 1) ;
				ptRefPos.y = nPosY * nBlockSize
								+ ((int) ptrNextVector[1] >> 1) ;
				if ( ((ptRefPos.x | ptRefPos.y) < 0)
					|| (ptRefPos.y + nBlockSize > (LONG) m_nDstWidth)
					|| (ptRefPos.x + nBlockSize > (LONG) m_nDstWidth) )
				{
					ptRefPos.x = nPosX * nBlockSize ;
					ptRefPos.y = nPosY * nBlockSize ;
				}
				#if	defined(_DEBUG)
				EGL_SIZE	sizeBlock ;
				sizeBlock.w = m_nDstWidth - nPosX * nBlockSize ;
				sizeBlock.h = m_nDstHeight - nPosY * nBlockSize ;
				if ( sizeBlock.w > nBlockSize )
					sizeBlock.w = nBlockSize ;
				if ( sizeBlock.h > nBlockSize )
					sizeBlock.h = nBlockSize ;
				ESLAssert( ptRefPos.x >= 0 ) ;
				ESLAssert( ptRefPos.y >= 0 ) ;
				ESLAssert( ptRefPos.x + sizeBlock.w
						+ (ptrNextVector[0] & 0x01) <= (int) m_nDstWidth ) ;
				ESLAssert( ptRefPos.y + sizeBlock.h
						+ (ptrNextVector[1] & 0x01) <= (int) m_nDstHeight ) ;
				#endif
				ptrNextBlockAddr[0] =
					ptrPrevImage + ptRefPos.x * nPrevPixelBytes
									+ ptRefPos.y * nPrevLineBytes ;
			}
			if ( (ptrNextImage != NULL)
				&& ((nPrevType == -1) || (nPrevType == 2)) )
			{
				//
				// 直後フレーム参照
				//
				ptRefPos.x = nPosX * nBlockSize
								+ ((int) ptrNextVector[2] >> 1) ;
				ptRefPos.y = nPosY * nBlockSize
								+ ((int) ptrNextVector[3] >> 1) ;
				if ( ((ptRefPos.x | ptRefPos.y) < 0)
					|| (ptRefPos.y + nBlockSize > (LONG) m_nDstWidth)
					|| (ptRefPos.x + nBlockSize > (LONG) m_nDstWidth) )
				{
					ptRefPos.x = nPosX * nBlockSize ;
					ptRefPos.y = nPosY * nBlockSize ;
				}
				#if	defined(_DEBUG)
				EGL_SIZE	sizeBlock ;
				sizeBlock.w = m_nDstWidth - nPosX * nBlockSize ;
				sizeBlock.h = m_nDstHeight - nPosY * nBlockSize ;
				if ( sizeBlock.w > nBlockSize )
					sizeBlock.w = nBlockSize ;
				if ( sizeBlock.h > nBlockSize )
					sizeBlock.h = nBlockSize ;
				ESLAssert( ptRefPos.x >= 0 ) ;
				ESLAssert( ptRefPos.y >= 0 ) ;
				ESLAssert( ptRefPos.x + sizeBlock.w
						+ (ptrNextVector[2] & 0x01) <= (int) m_nDstWidth ) ;
				ESLAssert( ptRefPos.y + sizeBlock.h
						+ (ptrNextVector[3] & 0x01) <= (int) m_nDstHeight ) ;
				#endif
				ptrNextBlockAddr[1] =
					ptrNextImage + ptRefPos.x * nNextPixelBytes
									+ ptRefPos.y * nNextLineBytes ;
			}
			((DWORD*)ptrNextBlockAddr)[2] =
				(ptrNextVector[0] & 0x01) | ((ptrNextVector[1] & 0x01) << 1) ;
			((DWORD*)ptrNextBlockAddr)[3] =
				(ptrNextVector[2] & 0x01) | ((ptrNextVector[3] & 0x01) << 1) ;
			ptrNextVector += 4 ;
			ptrNextBlockAddr += 4 ;
		}
	}
}

// 中間画像バッファに 1 チャネル書き出す
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::StoreYUVImageChannel
	( int xBlock, int yBlock, int iChannel )
{
	LONG	nPixelBytes = m_nYUVPixelBytes ;
	int		nBlockSize = m_nBlockSize ;
	int		nBlockArea = m_nBlockArea ;
	SBYTE *	ptrDstYUV =
		m_ptrYUVImage
			+ (yBlock * nBlockSize * m_nYUVLineBytes)
					+ (xBlock * nBlockSize * nPixelBytes) + iChannel ;
	SBYTE *	ptrSrcYUV = m_ptrDecodeBuf ;
	//
	for ( int y = 0; y < nBlockSize; y ++ )
	{
		SBYTE *	ptrDstLine = ptrDstYUV ;
		for ( int x = 0; x < nBlockSize; x ++ )
		{
			*ptrDstLine = *(ptrSrcYUV ++) ;
			ptrDstLine += nPixelBytes ;
		}
		ptrDstYUV += m_nYUVLineBytes ;
	}
}

// 中間画像バッファに 1 チャネル書き出す（スケーリング）
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::StoreYUVImageChannelX2
	( int xBlock, int yBlock, int iChannel )
{
	LONG	nPixelBytes = m_nYUVPixelBytes ;
	LONG	nLineBytes = m_nYUVLineBytes ;
	int		nBlockSize = m_nBlockSize ;
	int		nBlockArea = m_nBlockArea ;
	SBYTE *	ptrDstYUV =
		m_ptrYUVImage
			+ (yBlock * nBlockSize * 2 * nLineBytes)
					+ (xBlock * nBlockSize * 2 * nPixelBytes) + iChannel ;
	SBYTE *	ptrSrcYUV = m_ptrDecodeBuf ;
	//
	for ( int y = 0; y < nBlockSize; y ++ )
	{
		SBYTE *	ptrDstLine = ptrDstYUV ;
		for ( int x = 0; x < nBlockSize; x ++ )
		{
			SBYTE	d = *(ptrSrcYUV ++) ;
			ptrDstLine[nLineBytes + nPixelBytes] =
				ptrDstLine[nLineBytes] =
				ptrDstLine[nPixelBytes] = ptrDstLine[0] = d ;
			ptrDstLine += nPixelBytes * 2 ;
		}
		ptrDstYUV += nLineBytes * 2 ;
	}
}

// 動き補償を適用した上で画像を複製する
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::MoveImageWithVector( void )
{
	ESLAssert( m_nBlockSize == 8 ) ;
	const int	nBlockSize = 16 ;
	LONG	nImageWidth = m_nDstWidth ;
	LONG	nImageHeight = m_nDstHeight ;
	LONG	nWidthDivBlocks =
		(nImageWidth + (nBlockSize - 1)) / nBlockSize ;
	LONG	nHeightDivBlocks =
		(nImageHeight + (nBlockSize - 1)) / nBlockSize ;
	//
	EGL_IMAGE_INFO	srcblock1, srcblock2, dstblock ;
	::eslFillMemory( &srcblock1, 0, sizeof(EGL_IMAGE_INFO) ) ;
	::eslFillMemory( &srcblock2, 0, sizeof(EGL_IMAGE_INFO) ) ;
	::eslFillMemory( &dstblock, 0, sizeof(EGL_IMAGE_INFO) ) ;
	//
	srcblock1.dwInfoSize = sizeof(EGL_IMAGE_INFO) ;
	if ( m_pPrevImageRef != NULL )
	{
		srcblock1.fdwFormatType = m_pPrevImageRef->fdwFormatType ;
		srcblock1.dwBitsPerPixel = m_pPrevImageRef->dwBitsPerPixel ;
	}
	srcblock1.dwBytesPerLine = m_dwPrevLineBytes ;
	//
	srcblock2.dwInfoSize = sizeof(EGL_IMAGE_INFO) ;
	if ( m_pNextImageRef != NULL )
	{
		srcblock2.fdwFormatType = m_pNextImageRef->fdwFormatType ;
		srcblock2.dwBitsPerPixel = m_pNextImageRef->dwBitsPerPixel ;
	}
	srcblock2.dwBytesPerLine = m_dwPrevLineBytes ;
	//
	dstblock.dwInfoSize = sizeof(EGL_IMAGE_INFO) ;
	dstblock.fdwFormatType = srcblock1.fdwFormatType ;
	dstblock.dwBitsPerPixel = m_nDstPixelBytes * 8 ;
	dstblock.dwBytesPerLine = m_nDstLineBytes ;
	LONG	nSrc1PixelBytes = srcblock1.dwBitsPerPixel / 8 ;
	LONG	nSrc2PixelBytes = srcblock2.dwBitsPerPixel / 8 ;
	LONG	nDstPixelBytes = m_nDstPixelBytes ;
	LONG	x, y ;
	//
#if	defined(ERI_INTEL_X86)
	PVOID *	ptrNextBlockAddr = m_ptrNextPrevBlocks ;
	y = 0 ;
#else
	PVOID *	ptrNextBlockAddr = m_ptrMovePrevBlocks ;
	for ( y = 0; y < nImageHeight; y += nBlockSize )
#endif
	{
		dstblock.dwImageHeight = nBlockSize ;
		if ( y + nBlockSize > nImageHeight )
		{
			dstblock.dwImageHeight = nImageHeight - y ;
		}
		srcblock1.dwImageHeight = dstblock.dwImageHeight ;
		srcblock2.dwImageHeight = dstblock.dwImageHeight ;
		dstblock.dwSizeOfImage =
			dstblock.dwImageHeight * dstblock.dwBytesPerLine ;
		srcblock1.dwSizeOfImage =
			srcblock1.dwImageHeight * srcblock1.dwBytesPerLine ;
		srcblock2.dwSizeOfImage =
			srcblock2.dwImageHeight * srcblock2.dwBytesPerLine ;
		//
		for ( x = 0; x < nImageWidth; x += nBlockSize )
		{
			dstblock.dwImageWidth = nBlockSize ;
			if ( x + nBlockSize > nImageWidth )
			{
				dstblock.dwImageWidth = nImageWidth - x ;
			}
			srcblock1.dwImageWidth = dstblock.dwImageWidth ;
			srcblock2.dwImageWidth = dstblock.dwImageWidth ;
			dstblock.ptrImageArray =
				m_ptrDstBlock
					+ x * nDstPixelBytes
					+ y * dstblock.dwBytesPerLine ;
			srcblock1.ptrImageArray = ptrNextBlockAddr[0] ;
			srcblock2.ptrImageArray = ptrNextBlockAddr[1] ;
			//
			if ( srcblock2.ptrImageArray == NULL )
			{
				::eriCopyImage( dstblock, srcblock1 ) ;
			}
			else if ( srcblock1.ptrImageArray != NULL )
			{
				::eriBlendHalfImage( dstblock, srcblock1, srcblock2 ) ;
			}
			else
			{
				::eriCopyImage( dstblock, srcblock2 ) ;
			}
			//
			ptrNextBlockAddr += 4 ;
		}
	}
}

// グレイ画像の出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LS_RestoreGray8( void )
{
	LONG	nSrcLineBytes = m_nYUVLineBytes ;
	SBYTE *	ptrSrcImage = m_ptrYUVImage ;
	LONG	nDstLineBytes = m_nDstLineBytes ;
	BYTE *	ptrDstImage = m_ptrDstBlock ;
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		::eslMoveMemory( ptrDstImage, ptrSrcImage, m_nDstWidth ) ;
		ptrSrcImage += nSrcLineBytes ;
		ptrDstImage += nDstLineBytes ;
	}
}

// グレイ画像の差分出力
//////////////////////////////////////////////////////////////////////////////
void ERISADecoder::LS_RestoreDeltaGray8( void )
{
	LONG	nSrcLineBytes = m_nYUVLineBytes ;
	SBYTE *	ptrSrcImage = m_ptrYUVImage ;
	LONG	nDstLineBytes = m_nDstLineBytes ;
	BYTE *	ptrDstImage = m_ptrDstBlock ;
	UINT	nWidth = m_nDstWidth ;
	//
	for ( UINT y = 0; y < m_nDstHeight; y ++ )
	{
		SBYTE *	ptrSrcLine = ptrSrcImage ;
		BYTE *	ptrDstLine = ptrDstImage ;
		for ( UINT x = 0; x < nWidth; x ++ )
		{
			int	g = (int) ptrDstLine[x] + (((int) ptrSrcLine[x]) << 1) ;
			if ( (unsigned int) g > 0xFF )
			{
				g = (~g >> 31) & 0xFF ;
			}
			ptrDstLine[x] = (BYTE) g ;
		}
		ptrSrcImage += nSrcLineBytes ;
		ptrDstImage += nDstLineBytes ;
	}
}

// 画像出力関数取得
//////////////////////////////////////////////////////////////////////////////
ERISADecoder::PTR_PROCEDURE ERISADecoder::GetLSRestoreFunc
	( DWORD fdwFormatType, DWORD dwBitsPerPixel, DWORD fdwFlags )
{
	switch ( dwBitsPerPixel )
	{
	case	32:
		if ( m_eihInfo.fdwFormatType == ERI_RGBA_IMAGE )
		{
			if ( !(fdwFlags & dfDifferential) )
				return	&ERISADecoder::LS_RestoreRGBA32 ;
			else
				return	&ERISADecoder::LS_RestoreDeltaRGBA32 ;
		}
	case	24:
		if ( !(fdwFlags & dfDifferential) )
			return	&ERISADecoder::LS_RestoreRGB24 ;
		else
			return	&ERISADecoder::LS_RestoreDeltaRGB24 ;
	case	8:
		if ( !(fdwFlags & dfDifferential) )
			return	&ERISADecoder::LS_RestoreGray8 ;
		else
			return	&ERISADecoder::LS_RestoreDeltaGray8 ;
	}
	return	NULL ;
}


#endif
