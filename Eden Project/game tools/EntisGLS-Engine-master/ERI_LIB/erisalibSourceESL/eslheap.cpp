
/*****************************************************************************
           ESL (Entis Standard Library) heap core implementation
 ----------------------------------------------------------------------------
        Copyright (c) 2002-2004 Leshade Entis. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <eritypes.h>
#include <esl.h>


//////////////////////////////////////////////////////////////////////////////
// デバッグサポート関数
//////////////////////////////////////////////////////////////////////////////

#if	defined(_DEBUG)

void ESLTrace( const char * pszTrace, ... )
{
	char	szBuf[0x400] ;
	va_list	vl ;
	va_start( vl, pszTrace ) ;
	::vsprintf( szBuf, pszTrace, vl ) ;
	::OutputDebugString( szBuf ) ;
}

void _ESLAssert
	( const char * pszExpr, const char * pszFile, int nLine )
{
	static const char	szMsg[] =
		"▼ 以下の個所でアサーションエラーが発生しました。\n"
		"    %s\n"
		"    %d 行目\n\n"
		"▼ アサーション条件\n"
		"    %s\n"
		"処理を続行しますか？\n" ;
	char	szBuf[0x1000] ;
	//
	int		i = 0 ;
	while ( pszFile[i] )
	{
		if ( ::IsDBCSLeadByte( pszFile[i] ) )
		{
			i += 2 ;
		}
		else if ( pszFile[i ++] == '\\' )
		{
			pszFile += i ;
			i = 0 ;
		}
	}
	//
	::sprintf( szBuf, szMsg, pszFile, nLine, pszExpr ) ;
	//
	int		nResult = ::MessageBox
		( NULL, szBuf, "ESL Assertion error !!!",
			(MB_YESNO | MB_DEFBUTTON2 | MB_SETFOREGROUND) ) ;
	if ( nResult != IDYES )
	{
		__asm
		{
			int		03H
		}
	}
}

#else

extern "C" void _ESLAssert
	( const char * pszExpr, const char * pszFile, int nLine )
{
}

#endif

const char * GetESLErrorMsg( ESLError err )
{
	switch ( err )
	{
	case	eslErrSuccess:
		return	"関数は成功しました。" ;
	case	eslErrNotSupported:
		return	"未対応の機能を利用しようとしました。" ;
	case	eslErrGeneral:
		return	"一般的なエラーが発生しました。" ;
	case	eslErrAbort:
		return	"処理が中断されました。" ;
	case	eslErrInvalidParam:
		return	"不正なパラメータが指定されました。" ;
	case	eslErrTimeout:
		return	"関数はタイムアウトしました。" ;
	case	eslErrPending:
		return	"処理がペンディング状態です。" ;
	case	eslErrContinue:
		return	"処理は継続中です。" ;
	}
	if ( err & 0xFFFF0000 )
	{
		return	(const char *) err ;
	}
	else
	{
		return	"未知のエラーコードです。" ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// ハンドルされていない例外処理
//////////////////////////////////////////////////////////////////////////////

#if	!defined(COMPACT_NOA_DECODER)

static void eslUnhandledExceptionDumpMemory
	( ESLFileObject & file, DWORD dwMemAddr,
		const char * pszPointer, int nStart, int nEnd ) ;

LONG WINAPI eslUnhandledExceptionFilter( EXCEPTION_POINTERS * excpinf )
{
	static struct
	{
		DWORD			dwCode ;
		const char *	pszName ;
	}		tblExceptionNames[] =
	{
		{
			EXCEPTION_ACCESS_VIOLATION,
			"ページアクセス例外"
		},
		{
			EXCEPTION_DATATYPE_MISALIGNMENT,
			"アライメント例外"
		},
		{
			EXCEPTION_ARRAY_BOUNDS_EXCEEDED,
			"配列境界例外"
		},
		{
			EXCEPTION_FLT_DENORMAL_OPERAND,
			"FPU デノーマルオペランド例外"
		},
		{
			EXCEPTION_FLT_DIVIDE_BY_ZERO,
			"FPU ゼロ除算例外"
		},
		{
			EXCEPTION_FLT_INEXACT_RESULT,
			"FPU 精度例外"
		},
		{
			EXCEPTION_FLT_INVALID_OPERATION,
			"FPU 不正オペランド例外"
		},
		{
			EXCEPTION_FLT_OVERFLOW,
			"FPU オーバーフロー例外"
		},
		{
			EXCEPTION_FLT_STACK_CHECK,
			"FPU スタックチェック例外"
		},
		{
			EXCEPTION_FLT_UNDERFLOW,
			"FPU アンダーフロー例外"
		},
		{
			EXCEPTION_INT_DIVIDE_BY_ZERO,
			"ゼロ除算例外"
		},
		{
			EXCEPTION_INT_OVERFLOW,
			"オーバーフロー例外"
		},
		{
			EXCEPTION_PRIV_INSTRUCTION,
			"特権命令例外"
		},
		{
			EXCEPTION_IN_PAGE_ERROR,
			"ページ例外"
		},
		{
			EXCEPTION_ILLEGAL_INSTRUCTION,
			"無効命令例外"
		},
		{
			EXCEPTION_STACK_OVERFLOW,
			"スタックオーバーフロー例外"
		},
		{
			EXCEPTION_INVALID_DISPOSITION,
			"不正ディスポジション例外"
		},
		{
			EXCEPTION_GUARD_PAGE,
			"保護ページ例外"
		},
		{
			(DWORD) -1,
			"無効例外"
		}
	} ;
	//
	PEXCEPTION_RECORD	pexcp = excpinf->ExceptionRecord ;
	PCONTEXT			pcontx = excpinf->ContextRecord ;
	EString	strBuf ;
	//
	if ( pexcp->ExceptionCode == EXCEPTION_SINGLE_STEP )
	{
		//
		// シングルステップ割り込み
		//
		pcontx->EFlags &= ~0x100 ;
		return	EXCEPTION_CONTINUE_EXECUTION ;
	}
	else if ( pexcp->ExceptionCode == EXCEPTION_BREAKPOINT )
	{
		//
		// ブレークポイント例外
		//
		static const char	szBreakMsg[] =
			"ブレークポイント例外が %08XH で発生しました。\n"
			"処理を続行しますか？" ;
		::sprintf( strBuf.GetBuffer(0x400),
					szBreakMsg, pexcp->ExceptionAddress ) ;
		strBuf.ReleaseBuffer( ) ;
		int		nResult = ::MessageBox
			( NULL, strBuf, "◆　例外エラー情報　◆",
				(MB_YESNO | MB_DEFBUTTON1 | MB_SETFOREGROUND) ) ;
		if ( nResult == IDYES )
		{
			try
			{
				if ( *((BYTE*)pcontx->Eip) == 0xCC )
				{
					pcontx->Eip ++ ;
				}
			}
			catch ( ... )
			{
			}
			return	EXCEPTION_CONTINUE_EXECUTION ;
		}
		else
		{
			return	EXCEPTION_CONTINUE_SEARCH ;
		}
	}
	else if ( pexcp->ExceptionCode == EXCEPTION_IN_PAGE_ERROR )
	{
		//
		// ページ例外
		//
		return	EXCEPTION_CONTINUE_EXECUTION ;
	}
	//
	// 例外の名前を取得する
	//
	int		i ;
	const char *	pszExceptName = NULL ;
	for ( i = 0; tblExceptionNames[i].dwCode != (DWORD) -1; i ++ )
	{
		if ( tblExceptionNames[i].dwCode == pexcp->ExceptionCode )
		{
			break ;
		}
	}
	pszExceptName = tblExceptionNames[i].pszName ;
	//
	// 例外エラーのメッセージを表示する
	//
	static const char	szExceptMsg[] =
		"▼ プログラム実行中に以下の例外エラーが発生しました。\n"
		"    %s\n"
		"    CS:EIP = %04X:%08X\n\n"
		"詳細をログファイルに保存しますか？\n\n"
		"※このログファイルをプログラム開発者に送ると何らかの"
		"修正情報が得られるかもしれません。\n\n"
		"≫ログを保存して実行を継続する場合には「はい」を、\n"
		"≫ログを保存せずに実行を継続する場合には「いいえ」を、\n"
		"≫ログを保存せずに終了する場合には「キャンセル」を選択してください。" ;
	::sprintf( strBuf.GetBuffer(0x400),
				szExceptMsg, pszExceptName, pcontx->SegCs, pcontx->Eip ) ;
	strBuf.ReleaseBuffer( ) ;
	int		nResult = ::MessageBox
		( NULL, strBuf, "◆　例外エラー情報　◆",
			(MB_YESNOCANCEL | MB_DEFBUTTON1 | MB_SETFOREGROUND) ) ;
	//
	if ( nResult == IDCANCEL )
	{
		//
		// プロセス終了
		//
		::ExitProcess( -1 ) ;
		return	EXCEPTION_CONTINUE_SEARCH ;
	}
	else if ( nResult == IDNO )
	{
		//
		// 実行継続
		//
		return	EXCEPTION_CONTINUE_EXECUTION ;
	}
	//
	// ログファイル名を入力する
	//
	OPENFILENAME	ofn ;
	::memset( &ofn, 0, sizeof(ofn) ) ;
	strBuf = "" ;
	//
	ofn.lStructSize = sizeof(ofn) ;
	ofn.lpstrFilter = "error log file (*.log)\0*.log\0\0" ;
	ofn.lpstrFile = strBuf.GetBuffer( 0x400 ) ;
	ofn.nMaxFile = 0x400 ;
	ofn.lpstrTitle = "ログファイル保存" ;
	ofn.lpstrDefExt = "log" ;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST ;
	//
	::InitCommonControls( ) ;
	if ( !::GetSaveFileName( &ofn ) )
	{
		return	EXCEPTION_CONTINUE_SEARCH ;
	}
	strBuf.ReleaseBuffer( ) ;
	//
	// ファイルに書き出す
	//
	ERawFile	file ;
	if ( file.Open( strBuf, file.modeCreate ) )
	{
		return	EXCEPTION_CONTINUE_SEARCH ;
	}
	//
	// 基本情報を書き出す
	//
	static const char	szBasicInfo[] =
		"モジュール：%s\r\n"
		"例外エラーがスレッド %08XH で発生しました。\r\n"
		"例外エラーコード：%08XH\r\n"
		"例外エラーの種類：%s\r\n"
		"\r\n\r\n"
		"レジスタダンプ；\r\n"
		"CS:EIP  = %04X:%08X\r\n"
		"EAX = %08X, EBX = %08X, ECX = %08X, EDX = %08X\r\n"
		"ESI = %08X, EDI = %08X, EBP = %08X, ESP = %08X\r\n"
		"DS = %04X, ES = %04X, FS = %04X, GS = %04X, SS = %04X\r\n"
		"\r\n\r\n" ;
	//
	EString	strModuleName ;
	::GetModuleFileName
		( ::GetModuleHandle(NULL), strModuleName.GetBuffer(0x400), 0x400 ) ;
	strModuleName.ReleaseBuffer( ) ;
	//
	::sprintf
		( strBuf.GetBuffer(0x400), szBasicInfo,
			strModuleName.CharPtr(), ::GetCurrentThreadId(),
			pexcp->ExceptionCode, pszExceptName,
			pcontx->SegCs, pcontx->Eip,
			pcontx->Eax, pcontx->Ebx, pcontx->Ecx, pcontx->Edx,
			pcontx->Esi, pcontx->Edi, pcontx->Ebp, pcontx->Esp,
			pcontx->SegDs, pcontx->SegEs,
			pcontx->SegFs, pcontx->SegGs, pcontx->SegSs ) ;
	strBuf.ReleaseBuffer( ) ;
	file.Write( strBuf.CharPtr(), strBuf.GetLength() ) ;
	//
	// コードダンプ
	//
	strBuf = "コードダンプ；\r\n" ;
	file.Write( strBuf.CharPtr(), strBuf.GetLength() ) ;
	eslUnhandledExceptionDumpMemory
		( file, pcontx->Eip, "EIP+0000H", -0x100, 0x100 ) ;
	//
	// スタックダンプ
	//
	strBuf = "スタックダンプ；\r\n" ;
	file.Write( strBuf.CharPtr(), strBuf.GetLength() ) ;
	eslUnhandledExceptionDumpMemory
		( file, pcontx->Esp, "ESP+0000H", 0, 0x200 ) ;
	//
	// データダンプ
	//
	strBuf = "データダンプ；\r\n" ;
	file.Write( strBuf.CharPtr(), strBuf.GetLength() ) ;
	//
	static const char *	pszRegNameList[7] =
	{
		"EAX+0000H", "EBX+0000H", "ECX+0000H", "EDX+0000H",
		"ESI+0000H", "EDI+0000H", "EBP+0000H"
	} ;
	DWORD	dwReg[7] =
	{
		pcontx->Eax, pcontx->Ebx, pcontx->Ecx, pcontx->Edx,
		pcontx->Esi, pcontx->Edi, pcontx->Ebp
	} ;
	//
	for ( i = 0; i < 7; i ++ )
	{
		if ( dwReg[i] & 0xFFFF0000 )
		{
			eslUnhandledExceptionDumpMemory
				( file, dwReg[i], pszRegNameList[i], 0, 0x100 ) ;
		}
	}
	for ( i = 0; i < 7; i ++ )
	{
		if ( dwReg[i] & 0xFFFF0000 )
		{
			DWORD *	pdwReg = (DWORD*) dwReg[i] ;
			for ( int j = 0; j < 8; j ++ )
			{
				try
				{
					if ( pdwReg[j] & 0xFFFF0000 )
					{
						EString	strAddr( pdwReg[j], 8 ) ;
						strAddr += 'H' ;
						eslUnhandledExceptionDumpMemory
							( file, pdwReg[j], strAddr, 0, 0x80 ) ;
					}
				}
				catch ( ... )
				{
				}
			}
		}
	}
	//
	// 終了
	//
	file.Close( ) ;
	//
	nResult = ::MessageBox
		( NULL, "ログファイルを保存しました。\n"
			"処理を続行しますか？", "◆　例外エラー情報　◆",
			(MB_YESNO | MB_DEFBUTTON1 | MB_SETFOREGROUND) ) ;
	//
	if ( nResult == IDYES )
	{
		return	EXCEPTION_CONTINUE_EXECUTION ;
	}
	else
	{
		return	EXCEPTION_CONTINUE_SEARCH ;
	}
}

static void eslUnhandledExceptionDumpMemory
	( ESLFileObject & file, DWORD dwMemAddr,
		const char * pszPointer, int nStart, int nEnd )
{
	const BYTE *	pbytMem = (const BYTE *) dwMemAddr ;
	for ( int i = nStart; i < nEnd; i += 0x10 )
	{
		EString	strAddr, strHex, strChar ;
		char *	pszAddr = strAddr.GetBuffer(0x10) ;
		char *	pszHex = strHex.GetBuffer(0x80) ;
		char *	pszChar = strChar.GetBuffer(0x20) ;
		//
		if ( i == 0 )
		{
			::sprintf( pszAddr, "%9s : ", pszPointer ) ;
		}
		else
		{
			::sprintf( pszAddr, "%08XH : ", dwMemAddr + i ) ;
		}
		strAddr.ReleaseBuffer( ) ;
		//
		for ( int j = 0; j < 0x10; j ++ )
		{
			try
			{
				BYTE	bytData = pbytMem[i + j] ;
				::sprintf( pszHex + j * 3, "%02X ", bytData ) ;
				//
				if ( bytData < 0x20 )
					pszChar[j] = '.' ;
				else
					pszChar[j] = bytData ;
			}
			catch ( ... )
			{
				::strcpy( pszHex + j * 3, "-- " ) ;
				pszChar[j] = ' ' ;
			}
		}
		//
		strHex.ReleaseBuffer( ) ;
		strChar.ReleaseBuffer( 0x10 ) ;
		//
		EString	strLine = strAddr + strHex + "    " + strChar + " \r\n" ;
		file.Write( strLine.CharPtr(), strLine.GetLength() ) ;
	}
	file.Write( "\r\n\r\n", 4 ) ;
}

#endif


//////////////////////////////////////////////////////////////////////////////
// 基底クラス
//////////////////////////////////////////////////////////////////////////////

const char *const	ESLObject::m_pszClassName = "ESLObject" ;

const char * ESLObject::GetClassName( void ) const
{
	return	ESLObject::m_pszClassName ;
}

int ESLObject::IsKindOf( const char * pszClassName ) const
{
	return	!EString::Compare( pszClassName, ESLObject::m_pszClassName ) ;
}

ESLObject * ESLObject::DynamicCast( const char * pszType ) const
{
	if ( ESLObject::IsKindOf( pszType ) )
		return	(ESLObject*) this ;
	return	NULL ;
}

int ESLObject::IsSameClassAs( ESLObject * pObj ) const
{
	return	!EString::Compare( GetClassName(), pObj->GetClassName() ) ;
}

void * ESLObject::operator new ( size_t stObj )
{
	return	::eslHeapAllocate( NULL, stObj, 0 ) ;
}

void * ESLObject::operator new ( size_t stObj, void * ptrObj )
{
	return	ptrObj ;
}

void * ESLObject::operator new
	( size_t stObj, const char * pszFileName, int nLine )
{
	return	::eslHeapAllocate( NULL, stObj, 0 ) ;
}

void ESLObject::operator delete ( void * ptrObj )
{
	::eslHeapFree( NULL, ptrObj, 0 ) ;
}


//////////////////////////////////////////////////////////////////////////////
// スタック式ヒープ関数
//////////////////////////////////////////////////////////////////////////////

struct	tagHSTACKHEAP
{
	HSTACKHEAP		pNextHeap ;			// 次のヒープ
	HSTACKHEAP		pCurrentHeap ;		// カレントヒープ
	DWORD			dwGrowSize ;		// 拡張サイズ
	BYTE *			pbytBeginBlock ;	// メモリブロックの先頭
	DWORD			dwLimitBytes ;		// ヒープの限界サイズ
	DWORD			dwUsedBytes ;		// 使用されているバイト数
	DWORD			for_align[2] ;
} ;

typedef	tagHSTACKHEAP	STACK_HEAP_HEADER ;


#if	!defined(COMPACT_NOA_DECODER)

//////////////////////////////////////////////////////////////////////////////
// スタック式ヒープを生成
//////////////////////////////////////////////////////////////////////////////

HSTACKHEAP eslStackHeapCreate
	( DWORD dwInitSize, DWORD dwGrowSize, DWORD dwFlags )
{
	//
	// メモリを確保する
	//
	if ( dwInitSize == 0 )
	{
		dwInitSize = 0x10000 ;
	}
	else
	{
		dwInitSize = (dwInitSize + 0x0FFF) & (~0x0FFF) ;
	}
	if ( dwGrowSize == 0 )
	{
		dwGrowSize = 0x10000 ;
	}
	//
	STACK_HEAP_HEADER *	pHeapHdr = (STACK_HEAP_HEADER*)
		::VirtualAlloc( NULL, dwInitSize, MEM_COMMIT, PAGE_READWRITE ) ;
	if ( pHeapHdr == NULL )
	{
		return	NULL ;
	}
	//
	// ヒープを初期化
	//
	pHeapHdr->pNextHeap = NULL ;
	pHeapHdr->pCurrentHeap = pHeapHdr ;
	pHeapHdr->dwGrowSize = dwGrowSize ;
	pHeapHdr->pbytBeginBlock =
		((BYTE*)pHeapHdr) + sizeof(STACK_HEAP_HEADER) ;
	pHeapHdr->dwLimitBytes = dwInitSize - sizeof(STACK_HEAP_HEADER) ;
	pHeapHdr->dwUsedBytes = 0 ;
	//
	return	pHeapHdr ;
}


//////////////////////////////////////////////////////////////////////////////
// スタック式ヒープを削除
//////////////////////////////////////////////////////////////////////////////

void eslStackHeapDestroy( HSTACKHEAP hHeap )
{
	if ( hHeap != NULL )
	{
		::eslStackHeapFree( hHeap ) ;
		::VirtualFree( hHeap, 0, MEM_RELEASE ) ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// スタック式ヒープからメモリを確保
//////////////////////////////////////////////////////////////////////////////

void * eslStackHeapAllocate( HSTACKHEAP hHeap, DWORD dwSize )
{
	HSTACKHEAP	hCurrent = hHeap->pCurrentHeap ;
	//
	dwSize = (dwSize + 0x0F) & (~0x0F) ;
	//
	for ( ; ; )
	{
		if ( hCurrent->dwUsedBytes + dwSize <= hCurrent->dwLimitBytes )
		{
			BYTE *	ptrObj =
				hCurrent->pbytBeginBlock + hCurrent->dwUsedBytes ;
			hCurrent->dwUsedBytes += dwSize ;
			//
			return	ptrObj ;
		}
		else
		{
			DWORD	dwInitSize = hCurrent->dwGrowSize ;
			if ( dwSize + sizeof(STACK_HEAP_HEADER) > dwInitSize )
			{
				dwInitSize = dwSize + sizeof(STACK_HEAP_HEADER) ;
			}
			dwInitSize = (dwInitSize + 0x0FFF) & (~0x0FFF) ;
			//
			hCurrent->pNextHeap =
				::eslStackHeapCreate( dwInitSize, hCurrent->dwGrowSize, 0 ) ;
			hCurrent = hCurrent->pNextHeap ;
			hHeap->pCurrentHeap = hCurrent ;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// スタック式ヒープに確保されているメモリを全て解放
//////////////////////////////////////////////////////////////////////////////

void eslStackHeapFree( HSTACKHEAP hHeap )
{
	HSTACKHEAP	hNext = hHeap->pNextHeap ;
	while ( hNext != NULL )
	{
		HSTACKHEAP	hCurrent = hNext ;
		hNext = hNext->pNextHeap ;
		::VirtualFree( hCurrent, 0, MEM_RELEASE ) ;
	}
	//
	hHeap->pNextHeap = NULL ;
	hHeap->pCurrentHeap = hHeap ;
	hHeap->dwUsedBytes = 0 ;
}


#endif

#if	!defined(COMPACT_NOA_DECODER)

//////////////////////////////////////////////////////////////////////////////
// 汎用ヒープヘッダ構造体
//////////////////////////////////////////////////////////////////////////////

struct	VAR_HEAP_BLOCK
{
	DWORD					dwSize ;		// メモリブロックの有効なバイト数
	union
	{
		VAR_HEAP_BLOCK *	pPrevious ;		// 直前のメモリブロックのアドレス
		HESLHEAP			hHeap ;			// 属するヒープ
	} ;
} ;

#define	VAR_HEAP_SIZE_MASK	(0x0FFFFFFFUL)
#define	VAR_HEAP_FLAG_MASK	(0xF0000000UL)
#define	VAR_HEAP_FREE_FLAG	(0x80000000UL)
#define	VAR_HEAP_LAST_FLAG	(0x40000000UL)
#define	VAR_HEAP_FIRST_FLAG	(0x20000000UL)
#define	VAR_HEAP_SIGNATURE	(0x10000000UL)
#define	HEAP_LEN_SCALE		2

struct	VAR_FREE_BLOCK_INFO
{
	VAR_HEAP_BLOCK *	pPrevious ;		// 前の未確保ブロック
	VAR_HEAP_BLOCK *	pNext ;			// 次の未確保ブロック
} ;

#define	VAR_FREE_BLOCK_BASE	\
	((sizeof(VAR_HEAP_BLOCK) + sizeof(VAR_FREE_BLOCK_INFO)) * 2)

struct	tagHESLHEAP
{
	HESLHEAP			pNextHeap ;		// 次のヒープ
	HESLHEAP			pPrevHeap ;		// 前のヒープ

	VAR_HEAP_BLOCK *	pFirstSpace ;	// 初めの未確保ブロック
	DWORD				dwFreeSpace ;	// 未確保領域の合計
	DWORD				dwMaxFreeArea ;	// 連続した最大のみ確保領域のサイズ
										// ※この値は正確ではない
										// 　確保できるヒープを探すために
										// 　用いられるが、正確な数以上の値で
										// 　出来るだけ小さな値が望ましい

	DWORD				dwGrowSize ;	// ヒープの拡張サイズ
	HESLHEAP			pLastHeap ;		// 最後のヒープ

	DWORD				dwDefFlags ;	// デフォルトフラグ
	DWORD				dwHeapSize ;	// ヒープのサイズ

	DWORD				dwRefCount ;	// 参照カウント
	HESLHEAP			hParentHeap ;	// 親ヒープ
	PVOID				pOrgMemAddr ;	// 子ヒープの元のメモリブロックアドレス

	CRITICAL_SECTION	crtc_sect ;		// クリティカルセクション

	BYTE				for_align[8 - (sizeof(CRITICAL_SECTION) % 8)] ;
} ;

typedef	tagHESLHEAP	VAR_HEAP_HEADER ;

inline void * GetBlockAddress( const VAR_HEAP_BLOCK * pBlock )
{
	return	((BYTE*) pBlock) + sizeof(VAR_HEAP_BLOCK) ;
}

inline DWORD GetBlockLength( const VAR_HEAP_BLOCK * pBlock )
{
	ESLAssert( !(pBlock->dwSize & (0x07 >> HEAP_LEN_SCALE)) ) ;
	return	((pBlock->dwSize & VAR_HEAP_SIZE_MASK) << HEAP_LEN_SCALE) ;
}

inline VAR_HEAP_BLOCK * GetNextBlock( const VAR_HEAP_BLOCK * pBlock )
{
	return	(VAR_HEAP_BLOCK*) (((BYTE*)pBlock)
				+ GetBlockLength(pBlock) + sizeof(VAR_HEAP_BLOCK)) ;
}

inline VAR_FREE_BLOCK_INFO * GetFreeBlockInfo( const VAR_HEAP_BLOCK * pBlock )
{
	return	(VAR_FREE_BLOCK_INFO*) (((BYTE*)pBlock) + sizeof(VAR_HEAP_BLOCK)) ;
//	return	(VAR_FREE_BLOCK_INFO*)
//		(((BYTE*)pBlock) + GetBlockLength(pBlock)
//				+ (sizeof(VAR_HEAP_BLOCK) - sizeof(VAR_FREE_BLOCK_INFO))) ;
}

static HESLHEAP	g_hEslHeap = NULL ;			// グローバルヒープ


//////////////////////////////////////////////////////////////////////////////
// ヒープチェーンの検証
//////////////////////////////////////////////////////////////////////////////

ESLError eslVerifyHeapChain( HESLHEAP hHeap )
{
	if ( hHeap == NULL )
	{
		hHeap = g_hEslHeap ;
	}
	HESLHEAP	hPrev = hHeap ;
	HESLHEAP	hNext ;
	__try
	{
		for ( ; ; )
		{
			hNext = hPrev->pNextHeap ;
			if ( hNext == NULL )
			{
//				ESLAssert( (hHeap->pLastHeap == hPrev) || (hPrev == hHeap) ) ;
				if ( (hHeap->pLastHeap == hPrev) || (hPrev == hHeap) )
				{
					break ;
				}
				__asm	int	3
				return	eslErrGeneral ;
			}
//			ESLAssert( hNext->pPrevHeap == hPrev ) ;
			if ( hNext->pPrevHeap != hPrev )
			{
				__asm	int	3
				return	eslErrGeneral ;
			}
			hPrev = hNext ;
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		return	eslErrGeneral ;
	}
	return	eslErrSuccess ;
}


//////////////////////////////////////////////////////////////////////////////
// グローバルヒープ取得
//////////////////////////////////////////////////////////////////////////////

HESLHEAP eslGetGlobalHeap( void )
{
	if ( g_hEslHeap == NULL )
	{
		g_hEslHeap = ::eslHeapCreate( 0, 0, 0 ) ;
	}
	return	g_hEslHeap ;
}

void eslFreeGlobalHeap( void )
{
	if ( g_hEslHeap )
	{
		::eslHeapDump( g_hEslHeap, 100 ) ;
		::eslHeapDestroy( g_hEslHeap ) ;
		g_hEslHeap = NULL ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// 汎用ヒープ生成
//////////////////////////////////////////////////////////////////////////////

HESLHEAP eslHeapCreate
	( DWORD dwInitSize, DWORD dwGrowSize,
			DWORD dwFlags, HESLHEAP hParentHeap )
{
	//
	// メモリを確保する
	//
	if ( dwInitSize == 0 )
	{
		dwInitSize = 0x10000 ;
	}
	else
	{
		dwInitSize = (dwInitSize + 0x0FFF) & (~0x0FFF) ;
	}
	if ( dwGrowSize == 0 )
	{
		dwGrowSize = 0x10000 ;
	}
	//
	VAR_HEAP_HEADER *	pHeapHdr ;
	for ( ; ; )
	{
		if ( hParentHeap == NULL )
		{
			pHeapHdr =
				(VAR_HEAP_HEADER*) ::VirtualAlloc
					( NULL, dwInitSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE ) ;
		}
		else
		{
			PVOID	pOrgMemAddr = ::eslHeapAllocate
				( hParentHeap, dwInitSize + 0x10, ESL_HEAP_ZERO_INIT ) ;
			pHeapHdr = (VAR_HEAP_HEADER*)
						((((DWORD)pOrgMemAddr) + 0x0F) & ~0x0F) ;
			pHeapHdr->pOrgMemAddr = pOrgMemAddr ;
		}
		if ( pHeapHdr != NULL )
		{
			break ;
		}
		static const char	szMsg[] =
			"メモリ確保に失敗しました。\n"
			"他のアプリケーションで使用しているメモリを解放してください。\n"
			"メモリ確保を再実行しますか？" ;
		int	nMBResult = ::MessageBox
			( NULL, szMsg, "◆ ESL メモリアロケーション ◆", MB_YESNO ) ;
		if ( nMBResult != IDYES )
		{
			return	NULL ;
		}
	}
	//
	// ヒープを初期化
	//
	pHeapHdr->pNextHeap = NULL ;
	pHeapHdr->pPrevHeap = NULL ;
	pHeapHdr->dwGrowSize = dwGrowSize ;
	pHeapHdr->pLastHeap = NULL ;
	pHeapHdr->dwDefFlags = dwFlags ;
	pHeapHdr->dwHeapSize = dwInitSize ;
	pHeapHdr->dwRefCount = 1 ;
	pHeapHdr->hParentHeap = hParentHeap ;
	//
	if ( hParentHeap != NULL )
	{
		hParentHeap->dwRefCount ++ ;
	}
	//
	if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::InitializeCriticalSection( &(pHeapHdr->crtc_sect) ) ;
	}
	//
	VAR_HEAP_BLOCK *	pBlock =
		(VAR_HEAP_BLOCK*) (((DWORD)pHeapHdr) + sizeof(VAR_HEAP_HEADER)) ;
	pHeapHdr->pFirstSpace = pBlock ;
	pHeapHdr->dwFreeSpace =
		dwInitSize - sizeof(VAR_HEAP_HEADER) - sizeof(VAR_HEAP_BLOCK) ;
	pHeapHdr->dwMaxFreeArea = pHeapHdr->dwFreeSpace ;
	//
	pBlock->dwSize =
		(pHeapHdr->dwFreeSpace >> HEAP_LEN_SCALE)
			| VAR_HEAP_SIGNATURE | VAR_HEAP_FREE_FLAG
			| VAR_HEAP_LAST_FLAG | VAR_HEAP_FIRST_FLAG ;
	pBlock->hHeap = pHeapHdr ;
	//
	VAR_FREE_BLOCK_INFO *	pvfbi = GetFreeBlockInfo( pBlock ) ;
	pvfbi->pNext = NULL ;
	pvfbi->pPrevious = NULL ;
	//
	return	pHeapHdr ;
}


//////////////////////////////////////////////////////////////////////////////
// 汎用ヒープ削除
//////////////////////////////////////////////////////////////////////////////

void eslHeapDestroy( HESLHEAP hHeap )
{
	if ( hHeap != NULL )
	{
		if ( (-- hHeap->dwRefCount) == 0 )
		{
			if ( !(hHeap->dwDefFlags & ESL_HEAP_NO_SERIALIZE) )
			{
				::DeleteCriticalSection( &(hHeap->crtc_sect) ) ;
			}
			//
			HESLHEAP	hLeadHeap = hHeap ;
			while ( hHeap )
			{
				HESLHEAP	hNext = hHeap->pNextHeap ;
				HESLHEAP	hParent = hHeap->hParentHeap ;
				if ( hParent != NULL )
				{
					::eslHeapFree( hParent, hHeap->pOrgMemAddr, 0 ) ;
					::eslHeapDestroy( hParent ) ;
				}
				else
				{
					::VirtualFree( hHeap, 0, MEM_RELEASE ) ;
				}
				hHeap = hNext ;
			}
			//
			if ( g_hEslHeap == hLeadHeap )
			{
				g_hEslHeap = NULL ;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// 汎用ヒープにメモリを確保
//////////////////////////////////////////////////////////////////////////////

void * eslHeapAllocate
	( HESLHEAP hHeap, DWORD dwSize, DWORD dwFlags )
{
	if ( hHeap == NULL )
	{
		hHeap = eslGetGlobalHeap( ) ;
	}
	//
	// サイズを検証する
	//
	bool				fLargeMemory = false ;
	HESLHEAP			hTarget = hHeap ;
	VAR_HEAP_BLOCK *	pBlock = NULL ;
	dwSize = ((dwSize + 0x07 + (dwSize == 0)) & (~0x07)) ;
#if	defined(_DEBUG)
	if ( dwSize & (VAR_HEAP_FLAG_MASK << HEAP_LEN_SCALE) )
	{
		ESLTrace( "%dMB を超えるメモリブロックは確保できません。\n",
											(256 << HEAP_LEN_SCALE) ) ;
		return	NULL ;
	}
#else
	dwSize &= ~(VAR_HEAP_FLAG_MASK << HEAP_LEN_SCALE) ;
#endif
	//
	dwFlags |= hHeap->dwDefFlags ;
	if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::EnterCriticalSection( &(hHeap->crtc_sect) ) ;
	}
	//
	ESLAssert( dwSize != 0 ) ;
	if ( (hHeap->hParentHeap == NULL) && (dwSize >= (hHeap->dwGrowSize / 2)) )
	{
		fLargeMemory = true ;
		DWORD	dwHeapSize =
			dwSize + sizeof(VAR_HEAP_HEADER)
				+ sizeof(VAR_HEAP_BLOCK)
				+ sizeof(VAR_FREE_BLOCK_BASE) * 2 + 0x20 ;
		dwHeapSize = (dwHeapSize + 0x0FFF) & (~0x0FFF) ;
		hTarget = eslHeapCreate( dwHeapSize, 0, ESL_HEAP_NO_SERIALIZE ) ;
		if ( hTarget == NULL )
		{
			if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
			{
				::LeaveCriticalSection( &(hHeap->crtc_sect) ) ;
			}
			return	NULL ;
		}
		if ( hHeap->pLastHeap != NULL )
		{
			ESLAssert( hHeap->pLastHeap->pNextHeap == NULL ) ;
			hTarget->pPrevHeap = hHeap->pLastHeap ;
			hHeap->pLastHeap->pNextHeap = hTarget ;
		}
		else
		{
			hHeap->pNextHeap = hTarget ;
			hTarget->pPrevHeap = hHeap ;
		}
		hHeap->pLastHeap = hTarget ;
	}
	//
	// アロケーション可能なヒープを探す
	//
	for ( ; ; )
	{
		ESLAssert( pBlock == NULL ) ;
		//
		// アロケーション可能な未確保領域を探す
		//
		VAR_HEAP_BLOCK *	pNext = hTarget->pFirstSpace ;
		DWORD	dwMaxFreeArea = hTarget->dwMaxFreeArea ;
		DWORD	dwFreeSize = hTarget->dwFreeSpace - VAR_FREE_BLOCK_BASE ;
		DWORD	dwNeedSize = dwSize + VAR_FREE_BLOCK_BASE ;
		//
		if ( dwMaxFreeArea >= dwSize )
		{
			DWORD	dwCurMaxFree = 0, dwBlockSize, dwCurrentNeed ;
			SDWORD	dwMaxMask ;
			//
			while ( dwFreeSize >= dwSize )
			{
				ESLAssert( pNext && (pNext->dwSize & VAR_HEAP_FREE_FLAG) ) ;
				//
				dwBlockSize = GetBlockLength( pNext ) ;
				dwMaxMask = ((SDWORD)(dwCurMaxFree - dwBlockSize)) >> 31 ;
				dwCurrentNeed = dwNeedSize
					- ((- (int) !(pNext->dwSize & VAR_HEAP_FIRST_FLAG))
							& (VAR_FREE_BLOCK_BASE - sizeof(VAR_HEAP_BLOCK))) ;
				dwCurMaxFree =
					(dwBlockSize & dwMaxMask) | (dwCurMaxFree & ~dwMaxMask) ;
				//
				if ( dwBlockSize >= dwCurrentNeed )
				{
					pBlock = pNext ;
					break ;
				}
				if ( pNext->dwSize & VAR_HEAP_LAST_FLAG )
				{
					break ;
				}
				pNext = GetFreeBlockInfo(pNext)->pNext ;
				if ( pNext == NULL )
				{
					break ;
				}
				dwFreeSize -= dwBlockSize ;
			}
			// dwCurMaxFree = max( dwFreeSize, dwCurMaxFree ) ;
			dwMaxMask = ((SDWORD)(dwFreeSize - dwCurMaxFree)) >> 31 ;
			dwCurMaxFree =
				(dwCurMaxFree & dwMaxMask) | (dwFreeSize & ~dwMaxMask) ;
			// hTarget->dwMaxFreeArea = min( dwMaxFreeArea, dwCurMaxFree ) ;
			dwMaxMask = ((SDWORD)(dwMaxFreeArea - dwCurMaxFree)) >> 31 ;
			hTarget->dwMaxFreeArea =
				(dwMaxFreeArea & dwMaxMask) | (dwCurMaxFree & ~dwMaxMask) ;
		}
		if ( pBlock != NULL )
		{
			break ;
		}
		//
		// 次のヒープへ
		//
		hTarget = hTarget->pNextHeap ;
		//
		if ( (hTarget == NULL) || (hTarget->pFirstSpace == NULL) )
		{
			//
			// ヒープを拡張する
			//
			ESLAssert( !fLargeMemory ) ;
			DWORD	dwHeapSize =
				dwSize + sizeof(VAR_HEAP_HEADER) * 2
						+ sizeof(VAR_FREE_BLOCK_BASE) * 2 ;
			if ( (hHeap->hParentHeap == NULL)
					&& (dwHeapSize < hHeap->dwGrowSize) )
			{
				dwHeapSize = hHeap->dwGrowSize ;
			}
			dwHeapSize = (dwHeapSize + 0x0FFF) & (~0x0FFF) ;
			//
			hTarget = eslHeapCreate
				( dwHeapSize, 0, ESL_HEAP_NO_SERIALIZE, hHeap->hParentHeap ) ;
			if ( hTarget == NULL )
			{
				if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
				{
					::LeaveCriticalSection( &(hHeap->crtc_sect) ) ;
				}
				return	NULL ;
			}
			if ( hHeap->pNextHeap != NULL )
			{
				hTarget->pPrevHeap = hHeap ;
				hTarget->pNextHeap = hHeap->pNextHeap ;
				hHeap->pNextHeap->pPrevHeap = hTarget ;
				hHeap->pNextHeap = hTarget ;
			}
			else
			{
				hHeap->pNextHeap = hHeap->pLastHeap = hTarget ;
				hTarget->pPrevHeap = hHeap ;
			}
		}
	}
	//
	// 未確保領域にメモリを割り当てる
	//
	BYTE *	ptrObj = NULL ;
	//
	if ( fLargeMemory )
	{
		DWORD	dwAllocSize =
			(GetBlockLength(pBlock) - VAR_FREE_BLOCK_BASE) & (~0x0F) ;
		ESLAssert( dwAllocSize >= dwSize ) ;
		ESLAssert( GetBlockLength(pBlock) >= dwAllocSize + VAR_FREE_BLOCK_BASE ) ;
		dwSize = dwAllocSize ;
		hTarget->pFirstSpace = NULL ;
	}
	if ( (pBlock->dwSize & VAR_HEAP_FIRST_FLAG) ||
			(GetBlockLength(pBlock) >= dwSize + VAR_FREE_BLOCK_BASE) )
	{
		//
		// ブロックを分割する
		//
		DWORD	dwBlockSize = GetBlockLength(pBlock) ;
		DWORD	dwNewSize =
			dwBlockSize - (dwSize + sizeof(VAR_HEAP_BLOCK)) ;
		//
		ESLAssert( (signed long int) dwNewSize > 0L ) ;
		//
//		VAR_FREE_BLOCK_INFO	vfbi = *(GetFreeBlockInfo(pBlock)) ;
		pBlock->dwSize = (dwNewSize >> HEAP_LEN_SCALE)
						| (pBlock->dwSize & VAR_HEAP_FLAG_MASK) ;
//		*(GetFreeBlockInfo(pBlock)) = vfbi ;
		//
		VAR_HEAP_BLOCK *	pNext = GetNextBlock( pBlock ) ;
		pNext->dwSize = (dwSize >> HEAP_LEN_SCALE) | VAR_HEAP_SIGNATURE ;
		pNext->pPrevious = pBlock ;
		//
		if ( pBlock->dwSize & VAR_HEAP_LAST_FLAG )
		{
			pBlock->dwSize &= ~VAR_HEAP_LAST_FLAG ;
			pNext->dwSize |= VAR_HEAP_LAST_FLAG ;
		}
		else
		{
			GetNextBlock(pNext)->pPrevious = pNext ;
		}
		//
		ptrObj = (BYTE*) GetBlockAddress( pNext ) ;
		hTarget->dwFreeSpace -= (dwSize + sizeof(VAR_HEAP_BLOCK)) ;
	}
	else
	{
		//
		// ブロックをそのまま割り当てる
		//
		VAR_FREE_BLOCK_INFO *	pvfbi = GetFreeBlockInfo( pBlock ) ;
		if ( pvfbi->pNext != NULL )
		{
			VAR_FREE_BLOCK_INFO *
				pvfbiNext = GetFreeBlockInfo( pvfbi->pNext ) ;
			pvfbiNext->pPrevious = pvfbi->pPrevious ;
		}
		GetFreeBlockInfo(pvfbi->pPrevious)->pNext = pvfbi->pNext ;
		//
		pBlock->dwSize &= ~VAR_HEAP_FREE_FLAG ;
		//
		ptrObj = (BYTE*) GetBlockAddress( pBlock ) ;
		hTarget->dwFreeSpace -= GetBlockLength( pBlock ) ;
	}
	//
	if ( dwFlags & ESL_HEAP_ZERO_INIT )
	{
		::eslFillMemory( ptrObj, 0, dwSize ) ;
	}
#if	defined(_DEBUG)
	else
	{
		::eslFillMemory( ptrObj, 0xCC, dwSize ) ;
	}
#endif
	//
	if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::LeaveCriticalSection( &(hHeap->crtc_sect) ) ;
	}
	//
	return	ptrObj ;
}


//////////////////////////////////////////////////////////////////////////////
// 汎用ヒープからメモリブロックを解放
//////////////////////////////////////////////////////////////////////////////

void eslHeapFree( HESLHEAP hHeap, void * ptrObj, DWORD dwFlags )
{
	//
	// ポインタの有効性の検証
	//
	ESLAssert( !(((DWORD)ptrObj) & (0x07 >> HEAP_LEN_SCALE)) ) ;
#if	defined(_DEBUG)
	if ( (ptrObj == NULL) || (((DWORD)ptrObj) & (0x07 >> HEAP_LEN_SCALE)) )
	{
		ESLTrace( "無効なポインタを解放しようとしました。\n" ) ;
#else
	if ( ptrObj == NULL )
	{
#endif
		return ;
	}
	VAR_HEAP_BLOCK *	pBlock =
		(VAR_HEAP_BLOCK*) (((DWORD) ptrObj) - sizeof(VAR_HEAP_BLOCK)) ;
	if ( (pBlock->dwSize & (VAR_HEAP_FREE_FLAG | VAR_HEAP_FIRST_FLAG))
		|| !(pBlock->dwSize & VAR_HEAP_SIGNATURE) )
	{
		ESLTrace( "無効なポインタを解放しようとしました。\n" ) ;
		return ;
	}
	//
	// 解放処理
	//
	if ( hHeap == NULL )
	{
		hHeap = g_hEslHeap ;
	}
	dwFlags |= hHeap->dwDefFlags ;
	if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::EnterCriticalSection( &(hHeap->crtc_sect) ) ;
	}
	//
	// ヒープヘッダを取得する
	//
	HESLHEAP	hTarget = NULL ;
	if ( pBlock->dwSize & VAR_HEAP_FIRST_FLAG )
	{
		hTarget = pBlock->hHeap ;
	}
	else if ( pBlock->pPrevious->dwSize & VAR_HEAP_FIRST_FLAG )
	{
		hTarget = pBlock->pPrevious->hHeap ;
	}
	else
	{
		HESLHEAP	hNextHeap = hHeap ;
		DWORD	dwBlockAddr = (DWORD) pBlock ;
		while ( hNextHeap != NULL )
		{
			DWORD	dwHeapAddr = (DWORD) hNextHeap ;
			if ( (dwHeapAddr <= dwBlockAddr)
				&& (dwBlockAddr < dwHeapAddr + hNextHeap->dwHeapSize) )
			{
				break ;
			}
			HESLHEAP	hPrevHeap = hNextHeap ;
			hNextHeap = hPrevHeap->pNextHeap ;
			ESLAssert( (hNextHeap != NULL) && (hNextHeap->pPrevHeap == hPrevHeap) ) ;
		}
		ESLAssert( hNextHeap != NULL ) ;
		if ( hNextHeap == NULL )
		{
			ESLTrace( "無効なメモリブロックをヒープから解放しようとしました。\n" ) ;
			return ;
		}
		hTarget = hNextHeap ;
	}
	//
	// 現在のブロックを解放して直前の未確保ブロックと連結
	//
	VAR_FREE_BLOCK_INFO	vfbi ;
	if ( pBlock->pPrevious->dwSize & VAR_HEAP_FREE_FLAG )
	{
		//
		// 直前のブロックは未確保領域 -> 連結
		//
		VAR_HEAP_BLOCK *	pPrev ;
		pPrev = pBlock->pPrevious ;
		ESLAssert( GetNextBlock(pPrev) == pBlock ) ;
//		vfbi = *(GetFreeBlockInfo(pPrev)) ;
		//
		DWORD	dwFlags = pPrev->dwSize & VAR_HEAP_FLAG_MASK ;
		DWORD	dwFreeSize =
			GetBlockLength( pBlock ) + sizeof(VAR_HEAP_BLOCK) ;
		hTarget->dwFreeSpace += dwFreeSize ;
		pPrev->dwSize =
			((GetBlockLength(pPrev) + dwFreeSize)
									>> HEAP_LEN_SCALE) | dwFlags ;
		if ( pBlock->dwSize & VAR_HEAP_LAST_FLAG )
		{
			pPrev->dwSize |= VAR_HEAP_LAST_FLAG ;
		}
		else
		{
			ESLAssert( !(pPrev->dwSize & VAR_HEAP_LAST_FLAG) ) ;
			GetNextBlock(pPrev)->pPrevious = pPrev ;
		}
		pBlock = pPrev ;
	}
	else
	{
		//
		// 現在のブロックを未確保に設定
		//
		ESLAssert( hTarget->pFirstSpace != NULL ) ;
		VAR_FREE_BLOCK_INFO *
			pFirst = GetFreeBlockInfo( hTarget->pFirstSpace ) ;
		vfbi.pPrevious = hTarget->pFirstSpace ;
		vfbi.pNext = pFirst->pNext ;
		pFirst->pNext = pBlock ;
		//
		if ( vfbi.pNext != NULL )
		{
			VAR_FREE_BLOCK_INFO *
				pNext = GetFreeBlockInfo( vfbi.pNext ) ;
			pNext->pPrevious = pBlock ;
		}
		//
		pBlock->dwSize |= VAR_HEAP_FREE_FLAG ;
		hTarget->dwFreeSpace += GetBlockLength(pBlock) ;
		*(GetFreeBlockInfo(pBlock)) = vfbi ;
	}
	//
	// 未確保ブロック情報を設定
	//
//	*(GetFreeBlockInfo(pBlock)) = vfbi ;
	//
	// 現在のブロックと直後の未確保ブロックと連結
	//
	if ( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) )
	{
		VAR_HEAP_BLOCK *	pNext = GetNextBlock( pBlock ) ;
		if ( pNext->dwSize & VAR_HEAP_FREE_FLAG )
		{
			//
			// 直後のブロックは未確保領域 -> 連結
			//
			VAR_FREE_BLOCK_INFO *	pvfbi = GetFreeBlockInfo(pNext) ;
			if ( pvfbi->pNext != NULL )
			{
				VAR_FREE_BLOCK_INFO *
					pvfbiNext = GetFreeBlockInfo( pvfbi->pNext ) ;
				pvfbiNext->pPrevious = pvfbi->pPrevious ;
			}
			GetFreeBlockInfo(pvfbi->pPrevious)->pNext = pvfbi->pNext ;
			//
//			vfbi = *(GetFreeBlockInfo(pBlock)) ;
			//
			DWORD	dwFlags = pBlock->dwSize & VAR_HEAP_FLAG_MASK ;
			DWORD	dwFreeSize =
				GetBlockLength( pNext ) + sizeof(VAR_HEAP_BLOCK) ;
			hTarget->dwFreeSpace += sizeof(VAR_HEAP_BLOCK) ;
			pBlock->dwSize =
				((GetBlockLength( pBlock ) + dwFreeSize)
								>> HEAP_LEN_SCALE) | dwFlags ;
			if ( pNext->dwSize & VAR_HEAP_LAST_FLAG )
			{
				pBlock->dwSize |= VAR_HEAP_LAST_FLAG ;
			}
			else
			{
				ESLAssert( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) ) ;
				GetNextBlock(pBlock)->pPrevious = pBlock ;
			}
			//
//			*(GetFreeBlockInfo(pBlock)) = vfbi ;
		}
	}
	//
	// ヒープの連続最大確保可能サイズを更新する
	//
	DWORD	dwSize = GetBlockLength( pBlock ) ;
	SDWORD	dwMaxMask = ((SDWORD)(hTarget->dwMaxFreeArea - dwSize)) >> 31 ;
	hTarget->dwMaxFreeArea =
		(dwSize & dwMaxMask) | (hTarget->dwMaxFreeArea & ~dwMaxMask) ;
	//
	// ヒープそのものの解放
	//
	if ( hTarget != hHeap )
	{
		if ( (pBlock->dwSize & (VAR_HEAP_LAST_FLAG | VAR_HEAP_FIRST_FLAG))
								== (VAR_HEAP_LAST_FLAG | VAR_HEAP_FIRST_FLAG) )
		{
			bool	fFreeHeap = (hTarget->pFirstSpace == NULL) ;
			if ( !fFreeHeap )
			{
				//
				// ヒープを即時に解放するか判断
				//
				HESLHEAP	hNext = hHeap->pNextHeap ;
				fFreeHeap =
					(hNext != NULL) && (hNext != hTarget)
					&& (hNext->pFirstSpace != NULL)
					&& ((hNext->pFirstSpace->dwSize
							& (VAR_HEAP_LAST_FLAG | VAR_HEAP_FIRST_FLAG))
								== (VAR_HEAP_LAST_FLAG | VAR_HEAP_FIRST_FLAG)) ;
				//
				ESLAssert( hTarget->pPrevHeap->pNextHeap == hTarget ) ;
				hTarget->pPrevHeap->pNextHeap = hTarget->pNextHeap ;
				if ( hTarget->pNextHeap != NULL )
				{
					hTarget->pNextHeap->pPrevHeap = hTarget->pPrevHeap ;
				}
				if ( hHeap->pLastHeap == hTarget )
				{
					if ( hTarget->pPrevHeap != hHeap )
						hHeap->pLastHeap = hTarget->pPrevHeap ;
					else
						hHeap->pLastHeap = NULL ;
				}
				if ( hHeap->pNextHeap != NULL )
				{
					hTarget->pPrevHeap = hHeap ;
					hTarget->pNextHeap = hHeap->pNextHeap ;
					hHeap->pNextHeap->pPrevHeap = hTarget ;
					hHeap->pNextHeap = hTarget ;
				}
				else
				{
					hHeap->pNextHeap = hHeap->pLastHeap = hTarget ;
					hTarget->pPrevHeap = hHeap ;
				}
				//
				if ( fFreeHeap )
				{
					hTarget = hNext ;
				}
			} 
			if ( fFreeHeap )
			{
				//
				// ヒープチェーンを切断
				//
				ESLAssert( hTarget->pPrevHeap->pNextHeap == hTarget ) ;
				hTarget->pPrevHeap->pNextHeap = hTarget->pNextHeap ;
				if ( hTarget->pNextHeap != NULL )
				{
					hTarget->pNextHeap->pPrevHeap = hTarget->pPrevHeap ;
				}
				if ( hHeap->pLastHeap == hTarget )
				{
					if ( hTarget->pPrevHeap != hHeap )
						hHeap->pLastHeap = hTarget->pPrevHeap ;
					else
						hHeap->pLastHeap = NULL ;
				}
				//
				// ヒープを解放
				//
#if	defined(_DEBUG)
				DWORD	dwFreeSpace = 0 ;
				pBlock = (VAR_HEAP_BLOCK*)
					(((BYTE*)hTarget) + sizeof(VAR_HEAP_HEADER)) ;
				for ( ; ; )
				{
					ESLAssert( pBlock->dwSize & VAR_HEAP_FREE_FLAG ) ;
					dwFreeSpace += GetBlockLength( pBlock ) ;
					if ( pBlock->dwSize & VAR_HEAP_LAST_FLAG )
						break ;
					pBlock = GetNextBlock( pBlock ) ;
				}
				ESLAssert( hTarget->dwFreeSpace == dwFreeSpace ) ;
#endif
				if ( hTarget->hParentHeap == NULL )
				{
					::VirtualFree( hTarget, 0, MEM_RELEASE ) ;
				}
				else
				{
//					HESLHEAP	hParent = hTarget->hParentHeap ;
					hTarget->pNextHeap = NULL ;
					hTarget->pPrevHeap = NULL ;
					hTarget->pLastHeap = NULL ;
					::eslHeapDestroy( hTarget ) ;
				}
			}
		}
		else if ( (hTarget->pPrevHeap != hHeap)
			&& (hTarget->pPrevHeap->dwFreeSpace < hTarget->dwFreeSpace) )
		{
			//
			// ヒープチェーンを最適化
			//
			HESLHEAP	hPrev = hTarget->pPrevHeap ;
			HESLHEAP	hNext = hTarget->pNextHeap ;
			ESLAssert( hPrev->pPrevHeap != NULL ) ;
			hTarget->pPrevHeap = hPrev->pPrevHeap ;
			hTarget->pNextHeap = hPrev ;
			hPrev->pPrevHeap->pNextHeap = hTarget ;
			hPrev->pNextHeap = hNext ;
			hPrev->pPrevHeap = hTarget ;
			if ( hNext != NULL )
			{
				hNext->pPrevHeap = hPrev ;
			}
			if ( hHeap->pLastHeap == hTarget )
			{
				ESLAssert( hNext == NULL ) ;
				ESLAssert( hPrev != hHeap ) ;
				hHeap->pLastHeap = hPrev ;
			}
		}
	}
	//
	if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::LeaveCriticalSection( &(hHeap->crtc_sect) ) ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// メモリの再確保
//////////////////////////////////////////////////////////////////////////////

void * eslHeapReallocate
	( HESLHEAP hHeap, void * ptrObj, DWORD dwSize, DWORD dwFlags )
{
	//
	// ポインタの有効性の検証
	//
	if ( ptrObj == NULL )
	{
		return	eslHeapAllocate( hHeap, dwSize, dwFlags ) ;
	}
	if ( ((DWORD)ptrObj) & (0x07 >> HEAP_LEN_SCALE) )
	{
		ESLTrace( "無効なポインタを解放しようとしました。\n" ) ;
		return	NULL ;
	}
	VAR_HEAP_BLOCK *	pBlock =
		(VAR_HEAP_BLOCK*) (((DWORD) ptrObj) - sizeof(VAR_HEAP_BLOCK)) ;
	if ( (pBlock->dwSize & (VAR_HEAP_FREE_FLAG | VAR_HEAP_FIRST_FLAG))
		|| !(pBlock->dwSize & VAR_HEAP_SIGNATURE) )
	{
		ESLTrace( "無効なポインタを解放しようとしました。\n" ) ;
		return	NULL ;
	}
	//
	// ヒープヘッダを取得する
	//
	if ( hHeap == NULL )
	{
		hHeap = g_hEslHeap ;
	}
	dwFlags |= hHeap->dwDefFlags ;
	if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::EnterCriticalSection( &(hHeap->crtc_sect) ) ;
	}
	//
	HESLHEAP	hTarget = NULL ;
	if ( pBlock->dwSize & VAR_HEAP_FIRST_FLAG )
	{
		hTarget = pBlock->hHeap ;
	}
	else if ( pBlock->pPrevious->dwSize & VAR_HEAP_FIRST_FLAG )
	{
		hTarget = pBlock->pPrevious->hHeap ;
	}
	else
	{
		HESLHEAP	hNextHeap = hHeap ;
		DWORD	dwBlockAddr = (DWORD) pBlock ;
		while ( hNextHeap != NULL )
		{
			DWORD	dwHeapAddr = (DWORD) hNextHeap ;
			if ( (dwHeapAddr <= dwBlockAddr)
				&& (dwBlockAddr < dwHeapAddr + hNextHeap->dwHeapSize) )
			{
				break ;
			}
			hNextHeap = hNextHeap->pNextHeap ;
		}
		ESLAssert( hNextHeap != NULL ) ;
		if ( hNextHeap == NULL )
		{
			ESLTrace( "無効なメモリブロックをヒープから解放しようとしました。\n" ) ;
			return	NULL ;
		}
		hTarget = hNextHeap ;
	}
	//
	// 再確保処理
	//
	DWORD	dwMemSize = GetBlockLength(pBlock) ;
	DWORD	dwAlignSize = ((dwSize + 0x07) & (~0x07)) ;
	if ( dwAlignSize == 0 )
	{
		dwAlignSize = 0x08 ;
	}
	if ( dwMemSize > dwAlignSize )
	{
		//
		// メモリサイズの縮小
		//
		if ( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) )
		{
			VAR_HEAP_BLOCK *	pNext = GetNextBlock( pBlock ) ;
			if ( pNext->dwSize & VAR_HEAP_FREE_FLAG )
			{
				//
				// 直後の未確保領域のサイズを拡大
				//
				VAR_FREE_BLOCK_INFO	vfbi = *(GetFreeBlockInfo(pNext)) ;
				DWORD	dwReduction =
					GetBlockLength(pBlock) - dwAlignSize ;
				pBlock->dwSize =
					(dwAlignSize >> HEAP_LEN_SCALE)
						| (pBlock->dwSize & VAR_HEAP_FLAG_MASK) ;
				//
				VAR_HEAP_BLOCK *	pNext2 = GetNextBlock(pBlock) ;
				pNext2->dwSize =
					((GetBlockLength(pNext) + dwReduction) >> HEAP_LEN_SCALE)
									| (pNext->dwSize & VAR_HEAP_FLAG_MASK) ;
				ESLAssert( pBlock == pNext->pPrevious ) ;
				pNext2->pPrevious = pBlock ;
				//
				if ( !(pNext2->dwSize & VAR_HEAP_LAST_FLAG) )
				{
					GetNextBlock(pNext2)->pPrevious = pNext2 ;
				}
				//
				*(GetFreeBlockInfo(pNext2)) = vfbi ;
//				VAR_FREE_BLOCK_INFO *	pvfbi = GetFreeBlockInfo(pNext2) ;
				if ( vfbi.pNext != NULL )
				{
					GetFreeBlockInfo(vfbi.pNext)->pPrevious = pNext2 ;
				}
				GetFreeBlockInfo(vfbi.pPrevious)->pNext = pNext2 ;
				//
				hTarget->dwFreeSpace += dwReduction ;
				dwMemSize = GetBlockLength(pBlock) ;
			}
		}
		//
		BYTE *	pbytObj = (BYTE*) GetBlockAddress( pBlock ) ;
		ptrObj = pbytObj ;
		//
		if ( dwFlags & ESL_HEAP_ZERO_INIT )
		{
			::eslFillMemory( pbytObj + dwSize, 0, dwMemSize - dwSize ) ;
		}
#if	defined(_DEBUG)
		else
		{
			::eslFillMemory( pbytObj + dwSize, 0xCC, dwMemSize - dwSize ) ;
		}
#endif
	}
	else if ( dwMemSize < dwAlignSize )
	{
		//
		// メモリサイズの拡大
		//
		VAR_HEAP_BLOCK *	pNext = NULL ;
		DWORD	dwLimitSize = dwMemSize ;
		//
		if ( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) )
		{
			pNext = GetNextBlock( pBlock ) ;
			if ( pNext->dwSize & VAR_HEAP_FREE_FLAG )
			{
				dwLimitSize +=
					GetBlockLength( pNext ) + sizeof(VAR_HEAP_BLOCK) ;
			}
		}
		//
		if ( dwLimitSize >= dwAlignSize )
		{
			//
			// 直後の未確保領域を確保
			//
			DWORD	dwGrowSize = dwAlignSize - dwMemSize ;
			DWORD	dwOldSize = dwMemSize ;
			//
			if ( dwGrowSize < (GetBlockLength(pNext) / 2) )
			{
				//
				// ブロックを分割して再確保
				//
				ESLAssert( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) ) ;
				VAR_HEAP_BLOCK *	pNext1 = GetNextBlock( pBlock ) ;
				VAR_FREE_BLOCK_INFO	vfbi = *(GetFreeBlockInfo(pNext1)) ;
				//
				dwMemSize = dwAlignSize ;
				pBlock->dwSize =
					(pBlock->dwSize & VAR_HEAP_FLAG_MASK)
							| (dwAlignSize >> HEAP_LEN_SCALE) ;
				//
				VAR_HEAP_BLOCK *	pNext2 = GetNextBlock( pBlock ) ;
				pNext2->dwSize =
					(pNext1->dwSize & VAR_HEAP_FLAG_MASK)
						| ((GetBlockLength(pNext1)
								- dwGrowSize) >> HEAP_LEN_SCALE) ;
				pNext2->pPrevious = pBlock ;
				//
				if ( !(pNext2->dwSize & VAR_HEAP_LAST_FLAG) )
				{
					GetNextBlock(pNext2)->pPrevious = pNext2 ;
				}
				//
				*(GetFreeBlockInfo(pNext2)) = vfbi ;
//				VAR_FREE_BLOCK_INFO *	pvfbi = GetFreeBlockInfo(pNext2) ;
				if ( vfbi.pNext != NULL )
				{
					GetFreeBlockInfo(vfbi.pNext)->pPrevious = pNext2 ;
				}
				GetFreeBlockInfo(vfbi.pPrevious)->pNext = pNext2 ;
				//
				hTarget->dwFreeSpace -= dwGrowSize ;
			}
			else
			{
				//
				// ブロックをそのまま割り当てる
				//
				ESLAssert( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) ) ;
				VAR_HEAP_BLOCK *	pNext = GetNextBlock( pBlock ) ;
				VAR_FREE_BLOCK_INFO *	pvfbi = GetFreeBlockInfo( pNext ) ;
				if ( pvfbi->pNext != NULL )
				{
					VAR_FREE_BLOCK_INFO *
						pvfbiNext = GetFreeBlockInfo( pvfbi->pNext ) ;
					pvfbiNext->pPrevious = pvfbi->pPrevious ;
				}
				GetFreeBlockInfo(pvfbi->pPrevious)->pNext = pvfbi->pNext ;
				//
				dwGrowSize = sizeof(VAR_HEAP_BLOCK) + GetBlockLength(pNext) ;
				dwMemSize += dwGrowSize ;
				pBlock->dwSize =
					(pBlock->dwSize & VAR_HEAP_FLAG_MASK)
								| (dwMemSize >> HEAP_LEN_SCALE) ;
				//
				if ( pNext->dwSize & VAR_HEAP_LAST_FLAG )
				{
					pBlock->dwSize |= VAR_HEAP_LAST_FLAG ;
				}
				else
				{
					ESLAssert( GetNextBlock(pBlock) == GetNextBlock(pNext) ) ;
					GetNextBlock(pBlock)->pPrevious = pBlock ;
				}
				//
				hTarget->dwFreeSpace -= dwGrowSize - sizeof(VAR_HEAP_BLOCK) ;
			}
			//
			BYTE *	pbytObj = (BYTE*) GetBlockAddress( pBlock ) ;
			ptrObj = pbytObj ;
			//
			if ( dwFlags & ESL_HEAP_ZERO_INIT )
			{
				::eslFillMemory( pbytObj + dwOldSize, 0, dwMemSize - dwOldSize ) ;
			}
#if	defined(_DEBUG)
			else
			{
				::eslFillMemory( pbytObj + dwOldSize, 0xCC, dwMemSize - dwOldSize ) ;
			}
#endif
		}
		else
		{
			//
			// 別の領域にメモリを再確保
			//
			BYTE *	ptrNew =
				(BYTE*) ::eslHeapAllocate
					( hHeap, dwSize, dwFlags | ESL_HEAP_NO_SERIALIZE ) ;
			BYTE *	pbytOld = (BYTE*) ptrObj ;
			ptrObj = ptrNew ;
			//
			::eslMoveMemory( ptrNew, pbytOld, dwMemSize ) ;
			//
			::eslHeapFree( hHeap, pbytOld, ESL_HEAP_NO_SERIALIZE ) ;
		}
	}
	else
	{
	}
	//
	if ( !(dwFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::LeaveCriticalSection( &(hHeap->crtc_sect) ) ;
	}
	//
	return	ptrObj ;
}


//////////////////////////////////////////////////////////////////////////////
// メモリサイズの取得（メモリの有効性の検証）
//////////////////////////////////////////////////////////////////////////////

DWORD eslHeapGetLength( HESLHEAP hHeap, void * ptrObj )
{
	try
	{
		//
		// ポインタの有効性の検証
		//
		if ( (ptrObj == NULL)
			|| (((DWORD)ptrObj) & (0x07 >> HEAP_LEN_SCALE)) )
		{
			return	ESL_INVALID_HEAP ;
		}
		VAR_HEAP_BLOCK *	pBlock =
			(VAR_HEAP_BLOCK*) (((DWORD) ptrObj) - sizeof(VAR_HEAP_BLOCK)) ;
		if ( (pBlock->dwSize & VAR_HEAP_FREE_FLAG)
			|| (pBlock->dwSize & VAR_HEAP_FIRST_FLAG)
			|| !(pBlock->dwSize & VAR_HEAP_SIGNATURE) )
		{
			return	ESL_INVALID_HEAP ;
		}
		//
		// サイズの取得
		//
		DWORD	dwSize = pBlock->dwSize ;
		if ( (dwSize & VAR_HEAP_FLAG_MASK) ==
			(VAR_HEAP_SIGNATURE | VAR_HEAP_LAST_FLAG | VAR_HEAP_FIRST_FLAG) )
		{
			dwSize = GetBlockLength( pBlock ) - 8 ;
		}
		else
		{
			dwSize = GetBlockLength( pBlock ) ;
		}
		return	dwSize ;
	}
	catch ( ... )
	{
		//
		// ポインタは無効
		//
		return	ESL_INVALID_HEAP ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// ヒープに対する他のスレッドからの操作を待機
//////////////////////////////////////////////////////////////////////////////

void eslHeapLock( HESLHEAP hHeap )
{
	if ( hHeap == NULL )
	{
		hHeap = ::eslGetGlobalHeap( ) ;
	}
	if ( hHeap && !(hHeap->dwDefFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::EnterCriticalSection( &(hHeap->crtc_sect) ) ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// ヒープに対する他のスレッドからの操作の待機を解除
//////////////////////////////////////////////////////////////////////////////

void eslHeapUnlock( HESLHEAP hHeap )
{
	if ( hHeap == NULL )
	{
		hHeap = g_hEslHeap ;
	}
	if ( hHeap && !(hHeap->dwDefFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::LeaveCriticalSection( &(hHeap->crtc_sect) ) ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// ヒープに確保されているメモリブロックをデバッグ出力へダンプ
//////////////////////////////////////////////////////////////////////////////

void eslHeapDump( HESLHEAP hHeap, int nLimit )
{
	char	szBuf[0x100] ;
	//
	if ( hHeap == NULL )
	{
		hHeap = g_hEslHeap ;
		if ( hHeap == NULL )
		{
			return ;
		}
	}
	//
	int		i = 0 ;
	if ( !(hHeap->dwDefFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::EnterCriticalSection( &(hHeap->crtc_sect) ) ;
	}
	//
	bool		fFirstDump = true ;
	HESLHEAP	hNextHeap = hHeap ;
	while ( (hNextHeap != NULL) && (i < nLimit) )
	{
		VAR_HEAP_BLOCK *	pBlock =
			(VAR_HEAP_BLOCK*) (((DWORD)hNextHeap) + sizeof(VAR_HEAP_HEADER)) ;
		__try
		{
			for ( ; ; )
			{
				if ( !(pBlock->dwSize & VAR_HEAP_FREE_FLAG) )
				{
					if ( fFirstDump )
					{
						::OutputDebugString
							( "ESL の解放されていないメモリブロックがあります\n" ) ;
						fFirstDump = false ;
					}
					BYTE *	ptrObj = ((BYTE*)pBlock) + sizeof(VAR_HEAP_BLOCK) ;
					int		j =
						::sprintf( szBuf, "block %08XH (%d bytes) :",
									ptrObj, GetBlockLength( pBlock ) ) ;
					__try
					{
						for ( int k = 0; k < 0x10; k ++ )
						{
							BYTE	bytBinary = ptrObj[k] ;
							j += ::sprintf( &szBuf[j], " %02X", bytBinary ) ;
						}
					}
					__except ( EXCEPTION_EXECUTE_HANDLER )
					{
					}
					//
					szBuf[j    ] = '\n' ;
					szBuf[j + 1] = '\0' ;
					::OutputDebugString( szBuf ) ;
					//
					if ( ++ i >= nLimit )
					{
						break ;
					}
				}
				if ( pBlock->dwSize & VAR_HEAP_LAST_FLAG )
				{
					break ;
				}
				pBlock = ::GetNextBlock( pBlock ) ;
			}
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			break ;
		}
		hNextHeap = hNextHeap->pNextHeap ;
	}
	//
	if ( !(hHeap->dwDefFlags & ESL_HEAP_NO_SERIALIZE) )
	{
		::LeaveCriticalSection( &(hHeap->crtc_sect) ) ;
	}
}


#else	// !defined(COMPACT_NOA_DECODER)

HESLHEAP eslGetGlobalHeap( void )
{
	return	(HESLHEAP) ::GetProcessHeap( ) ;
}

void eslFreeGlobalHeap( void )
{
}

HESLHEAP eslHeapCreate
	( DWORD dwInitSize, DWORD dwGrowSize,
		DWORD dwFlags, HESLHEAP hParentHeap )
{
	return	(HESLHEAP) ::HeapCreate( 0, dwInitSize, 0 ) ;
}

void eslHeapDestroy( HESLHEAP hHeap )
{
	::HeapDestroy( (HANDLE) hHeap ) ;
}

void * eslHeapAllocate
	( HESLHEAP hHeap, DWORD dwSize, DWORD dwFlags )
{
	DWORD	dwHeapFlag = 0 ;
	if ( dwFlags & ESL_HEAP_ZERO_INIT )
		dwHeapFlag = HEAP_ZERO_MEMORY ;
	return	::HeapAlloc( (HANDLE) hHeap, dwHeapFlag, dwSize ) ;
}

void eslHeapFree
	( HESLHEAP hHeap, void * ptrObj, DWORD dwFlags )
{
	::HeapFree( (HANDLE) hHeap, 0, ptrObj ) ;
}

void * eslHeapReallocate
	( HESLHEAP hHeap, void * ptrObj, DWORD dwSize, DWORD dwFlags )
{
	DWORD	dwHeapFlag = 0 ;
	if ( dwFlags & ESL_HEAP_ZERO_INIT )
		dwHeapFlag = HEAP_ZERO_MEMORY ;
	return	::HeapReAlloc( (HANDLE) hHeap, dwHeapFlag, ptrObj, dwSize ) ;
}


#endif

