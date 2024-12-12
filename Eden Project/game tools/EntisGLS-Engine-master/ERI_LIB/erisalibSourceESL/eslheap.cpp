
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
// �f�o�b�O�T�|�[�g�֐�
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
		"�� �ȉ��̌��ŃA�T�[�V�����G���[���������܂����B\n"
		"    %s\n"
		"    %d �s��\n\n"
		"�� �A�T�[�V��������\n"
		"    %s\n"
		"�����𑱍s���܂����H\n" ;
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
		return	"�֐��͐������܂����B" ;
	case	eslErrNotSupported:
		return	"���Ή��̋@�\�𗘗p���悤�Ƃ��܂����B" ;
	case	eslErrGeneral:
		return	"��ʓI�ȃG���[���������܂����B" ;
	case	eslErrAbort:
		return	"���������f����܂����B" ;
	case	eslErrInvalidParam:
		return	"�s���ȃp�����[�^���w�肳��܂����B" ;
	case	eslErrTimeout:
		return	"�֐��̓^�C���A�E�g���܂����B" ;
	case	eslErrPending:
		return	"�������y���f�B���O��Ԃł��B" ;
	case	eslErrContinue:
		return	"�����͌p�����ł��B" ;
	}
	if ( err & 0xFFFF0000 )
	{
		return	(const char *) err ;
	}
	else
	{
		return	"���m�̃G���[�R�[�h�ł��B" ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// �n���h������Ă��Ȃ���O����
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
			"�y�[�W�A�N�Z�X��O"
		},
		{
			EXCEPTION_DATATYPE_MISALIGNMENT,
			"�A���C�����g��O"
		},
		{
			EXCEPTION_ARRAY_BOUNDS_EXCEEDED,
			"�z�񋫊E��O"
		},
		{
			EXCEPTION_FLT_DENORMAL_OPERAND,
			"FPU �f�m�[�}���I�y�����h��O"
		},
		{
			EXCEPTION_FLT_DIVIDE_BY_ZERO,
			"FPU �[�����Z��O"
		},
		{
			EXCEPTION_FLT_INEXACT_RESULT,
			"FPU ���x��O"
		},
		{
			EXCEPTION_FLT_INVALID_OPERATION,
			"FPU �s���I�y�����h��O"
		},
		{
			EXCEPTION_FLT_OVERFLOW,
			"FPU �I�[�o�[�t���[��O"
		},
		{
			EXCEPTION_FLT_STACK_CHECK,
			"FPU �X�^�b�N�`�F�b�N��O"
		},
		{
			EXCEPTION_FLT_UNDERFLOW,
			"FPU �A���_�[�t���[��O"
		},
		{
			EXCEPTION_INT_DIVIDE_BY_ZERO,
			"�[�����Z��O"
		},
		{
			EXCEPTION_INT_OVERFLOW,
			"�I�[�o�[�t���[��O"
		},
		{
			EXCEPTION_PRIV_INSTRUCTION,
			"�������ߗ�O"
		},
		{
			EXCEPTION_IN_PAGE_ERROR,
			"�y�[�W��O"
		},
		{
			EXCEPTION_ILLEGAL_INSTRUCTION,
			"�������ߗ�O"
		},
		{
			EXCEPTION_STACK_OVERFLOW,
			"�X�^�b�N�I�[�o�[�t���[��O"
		},
		{
			EXCEPTION_INVALID_DISPOSITION,
			"�s���f�B�X�|�W�V������O"
		},
		{
			EXCEPTION_GUARD_PAGE,
			"�ی�y�[�W��O"
		},
		{
			(DWORD) -1,
			"������O"
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
		// �V���O���X�e�b�v���荞��
		//
		pcontx->EFlags &= ~0x100 ;
		return	EXCEPTION_CONTINUE_EXECUTION ;
	}
	else if ( pexcp->ExceptionCode == EXCEPTION_BREAKPOINT )
	{
		//
		// �u���[�N�|�C���g��O
		//
		static const char	szBreakMsg[] =
			"�u���[�N�|�C���g��O�� %08XH �Ŕ������܂����B\n"
			"�����𑱍s���܂����H" ;
		::sprintf( strBuf.GetBuffer(0x400),
					szBreakMsg, pexcp->ExceptionAddress ) ;
		strBuf.ReleaseBuffer( ) ;
		int		nResult = ::MessageBox
			( NULL, strBuf, "���@��O�G���[���@��",
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
		// �y�[�W��O
		//
		return	EXCEPTION_CONTINUE_EXECUTION ;
	}
	//
	// ��O�̖��O���擾����
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
	// ��O�G���[�̃��b�Z�[�W��\������
	//
	static const char	szExceptMsg[] =
		"�� �v���O�������s���Ɉȉ��̗�O�G���[���������܂����B\n"
		"    %s\n"
		"    CS:EIP = %04X:%08X\n\n"
		"�ڍׂ����O�t�@�C���ɕۑ����܂����H\n\n"
		"�����̃��O�t�@�C�����v���O�����J���҂ɑ���Ɖ��炩��"
		"�C����񂪓����邩������܂���B\n\n"
		"�⃍�O��ۑ����Ď��s���p������ꍇ�ɂ́u�͂��v���A\n"
		"�⃍�O��ۑ������Ɏ��s���p������ꍇ�ɂ́u�������v���A\n"
		"�⃍�O��ۑ������ɏI������ꍇ�ɂ́u�L�����Z���v��I�����Ă��������B" ;
	::sprintf( strBuf.GetBuffer(0x400),
				szExceptMsg, pszExceptName, pcontx->SegCs, pcontx->Eip ) ;
	strBuf.ReleaseBuffer( ) ;
	int		nResult = ::MessageBox
		( NULL, strBuf, "���@��O�G���[���@��",
			(MB_YESNOCANCEL | MB_DEFBUTTON1 | MB_SETFOREGROUND) ) ;
	//
	if ( nResult == IDCANCEL )
	{
		//
		// �v���Z�X�I��
		//
		::ExitProcess( -1 ) ;
		return	EXCEPTION_CONTINUE_SEARCH ;
	}
	else if ( nResult == IDNO )
	{
		//
		// ���s�p��
		//
		return	EXCEPTION_CONTINUE_EXECUTION ;
	}
	//
	// ���O�t�@�C��������͂���
	//
	OPENFILENAME	ofn ;
	::memset( &ofn, 0, sizeof(ofn) ) ;
	strBuf = "" ;
	//
	ofn.lStructSize = sizeof(ofn) ;
	ofn.lpstrFilter = "error log file (*.log)\0*.log\0\0" ;
	ofn.lpstrFile = strBuf.GetBuffer( 0x400 ) ;
	ofn.nMaxFile = 0x400 ;
	ofn.lpstrTitle = "���O�t�@�C���ۑ�" ;
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
	// �t�@�C���ɏ����o��
	//
	ERawFile	file ;
	if ( file.Open( strBuf, file.modeCreate ) )
	{
		return	EXCEPTION_CONTINUE_SEARCH ;
	}
	//
	// ��{���������o��
	//
	static const char	szBasicInfo[] =
		"���W���[���F%s\r\n"
		"��O�G���[���X���b�h %08XH �Ŕ������܂����B\r\n"
		"��O�G���[�R�[�h�F%08XH\r\n"
		"��O�G���[�̎�ށF%s\r\n"
		"\r\n\r\n"
		"���W�X�^�_���v�G\r\n"
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
	// �R�[�h�_���v
	//
	strBuf = "�R�[�h�_���v�G\r\n" ;
	file.Write( strBuf.CharPtr(), strBuf.GetLength() ) ;
	eslUnhandledExceptionDumpMemory
		( file, pcontx->Eip, "EIP+0000H", -0x100, 0x100 ) ;
	//
	// �X�^�b�N�_���v
	//
	strBuf = "�X�^�b�N�_���v�G\r\n" ;
	file.Write( strBuf.CharPtr(), strBuf.GetLength() ) ;
	eslUnhandledExceptionDumpMemory
		( file, pcontx->Esp, "ESP+0000H", 0, 0x200 ) ;
	//
	// �f�[�^�_���v
	//
	strBuf = "�f�[�^�_���v�G\r\n" ;
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
	// �I��
	//
	file.Close( ) ;
	//
	nResult = ::MessageBox
		( NULL, "���O�t�@�C����ۑ����܂����B\n"
			"�����𑱍s���܂����H", "���@��O�G���[���@��",
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
// ���N���X
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
// �X�^�b�N���q�[�v�֐�
//////////////////////////////////////////////////////////////////////////////

struct	tagHSTACKHEAP
{
	HSTACKHEAP		pNextHeap ;			// ���̃q�[�v
	HSTACKHEAP		pCurrentHeap ;		// �J�����g�q�[�v
	DWORD			dwGrowSize ;		// �g���T�C�Y
	BYTE *			pbytBeginBlock ;	// �������u���b�N�̐擪
	DWORD			dwLimitBytes ;		// �q�[�v�̌��E�T�C�Y
	DWORD			dwUsedBytes ;		// �g�p����Ă���o�C�g��
	DWORD			for_align[2] ;
} ;

typedef	tagHSTACKHEAP	STACK_HEAP_HEADER ;


#if	!defined(COMPACT_NOA_DECODER)

//////////////////////////////////////////////////////////////////////////////
// �X�^�b�N���q�[�v�𐶐�
//////////////////////////////////////////////////////////////////////////////

HSTACKHEAP eslStackHeapCreate
	( DWORD dwInitSize, DWORD dwGrowSize, DWORD dwFlags )
{
	//
	// ���������m�ۂ���
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
	// �q�[�v��������
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
// �X�^�b�N���q�[�v���폜
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
// �X�^�b�N���q�[�v���烁�������m��
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
// �X�^�b�N���q�[�v�Ɋm�ۂ���Ă��郁������S�ĉ��
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
// �ėp�q�[�v�w�b�_�\����
//////////////////////////////////////////////////////////////////////////////

struct	VAR_HEAP_BLOCK
{
	DWORD					dwSize ;		// �������u���b�N�̗L���ȃo�C�g��
	union
	{
		VAR_HEAP_BLOCK *	pPrevious ;		// ���O�̃������u���b�N�̃A�h���X
		HESLHEAP			hHeap ;			// ������q�[�v
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
	VAR_HEAP_BLOCK *	pPrevious ;		// �O�̖��m�ۃu���b�N
	VAR_HEAP_BLOCK *	pNext ;			// ���̖��m�ۃu���b�N
} ;

#define	VAR_FREE_BLOCK_BASE	\
	((sizeof(VAR_HEAP_BLOCK) + sizeof(VAR_FREE_BLOCK_INFO)) * 2)

struct	tagHESLHEAP
{
	HESLHEAP			pNextHeap ;		// ���̃q�[�v
	HESLHEAP			pPrevHeap ;		// �O�̃q�[�v

	VAR_HEAP_BLOCK *	pFirstSpace ;	// ���߂̖��m�ۃu���b�N
	DWORD				dwFreeSpace ;	// ���m�ۗ̈�̍��v
	DWORD				dwMaxFreeArea ;	// �A�������ő�̂݊m�ۗ̈�̃T�C�Y
										// �����̒l�͐��m�ł͂Ȃ�
										// �@�m�ۂł���q�[�v��T�����߂�
										// �@�p�����邪�A���m�Ȑ��ȏ�̒l��
										// �@�o���邾�������Ȓl���]�܂���

	DWORD				dwGrowSize ;	// �q�[�v�̊g���T�C�Y
	HESLHEAP			pLastHeap ;		// �Ō�̃q�[�v

	DWORD				dwDefFlags ;	// �f�t�H���g�t���O
	DWORD				dwHeapSize ;	// �q�[�v�̃T�C�Y

	DWORD				dwRefCount ;	// �Q�ƃJ�E���g
	HESLHEAP			hParentHeap ;	// �e�q�[�v
	PVOID				pOrgMemAddr ;	// �q�q�[�v�̌��̃������u���b�N�A�h���X

	CRITICAL_SECTION	crtc_sect ;		// �N���e�B�J���Z�N�V����

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

static HESLHEAP	g_hEslHeap = NULL ;			// �O���[�o���q�[�v


//////////////////////////////////////////////////////////////////////////////
// �q�[�v�`�F�[���̌���
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
// �O���[�o���q�[�v�擾
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
// �ėp�q�[�v����
//////////////////////////////////////////////////////////////////////////////

HESLHEAP eslHeapCreate
	( DWORD dwInitSize, DWORD dwGrowSize,
			DWORD dwFlags, HESLHEAP hParentHeap )
{
	//
	// ���������m�ۂ���
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
			"�������m�ۂɎ��s���܂����B\n"
			"���̃A�v���P�[�V�����Ŏg�p���Ă��郁������������Ă��������B\n"
			"�������m�ۂ��Ď��s���܂����H" ;
		int	nMBResult = ::MessageBox
			( NULL, szMsg, "�� ESL �������A���P�[�V���� ��", MB_YESNO ) ;
		if ( nMBResult != IDYES )
		{
			return	NULL ;
		}
	}
	//
	// �q�[�v��������
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
// �ėp�q�[�v�폜
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
// �ėp�q�[�v�Ƀ��������m��
//////////////////////////////////////////////////////////////////////////////

void * eslHeapAllocate
	( HESLHEAP hHeap, DWORD dwSize, DWORD dwFlags )
{
	if ( hHeap == NULL )
	{
		hHeap = eslGetGlobalHeap( ) ;
	}
	//
	// �T�C�Y�����؂���
	//
	bool				fLargeMemory = false ;
	HESLHEAP			hTarget = hHeap ;
	VAR_HEAP_BLOCK *	pBlock = NULL ;
	dwSize = ((dwSize + 0x07 + (dwSize == 0)) & (~0x07)) ;
#if	defined(_DEBUG)
	if ( dwSize & (VAR_HEAP_FLAG_MASK << HEAP_LEN_SCALE) )
	{
		ESLTrace( "%dMB �𒴂��郁�����u���b�N�͊m�ۂł��܂���B\n",
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
	// �A���P�[�V�����\�ȃq�[�v��T��
	//
	for ( ; ; )
	{
		ESLAssert( pBlock == NULL ) ;
		//
		// �A���P�[�V�����\�Ȗ��m�ۗ̈��T��
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
		// ���̃q�[�v��
		//
		hTarget = hTarget->pNextHeap ;
		//
		if ( (hTarget == NULL) || (hTarget->pFirstSpace == NULL) )
		{
			//
			// �q�[�v���g������
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
	// ���m�ۗ̈�Ƀ����������蓖�Ă�
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
		// �u���b�N�𕪊�����
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
		// �u���b�N�����̂܂܊��蓖�Ă�
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
// �ėp�q�[�v���烁�����u���b�N�����
//////////////////////////////////////////////////////////////////////////////

void eslHeapFree( HESLHEAP hHeap, void * ptrObj, DWORD dwFlags )
{
	//
	// �|�C���^�̗L�����̌���
	//
	ESLAssert( !(((DWORD)ptrObj) & (0x07 >> HEAP_LEN_SCALE)) ) ;
#if	defined(_DEBUG)
	if ( (ptrObj == NULL) || (((DWORD)ptrObj) & (0x07 >> HEAP_LEN_SCALE)) )
	{
		ESLTrace( "�����ȃ|�C���^��������悤�Ƃ��܂����B\n" ) ;
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
		ESLTrace( "�����ȃ|�C���^��������悤�Ƃ��܂����B\n" ) ;
		return ;
	}
	//
	// �������
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
	// �q�[�v�w�b�_���擾����
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
			ESLTrace( "�����ȃ������u���b�N���q�[�v���������悤�Ƃ��܂����B\n" ) ;
			return ;
		}
		hTarget = hNextHeap ;
	}
	//
	// ���݂̃u���b�N��������Ē��O�̖��m�ۃu���b�N�ƘA��
	//
	VAR_FREE_BLOCK_INFO	vfbi ;
	if ( pBlock->pPrevious->dwSize & VAR_HEAP_FREE_FLAG )
	{
		//
		// ���O�̃u���b�N�͖��m�ۗ̈� -> �A��
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
		// ���݂̃u���b�N�𖢊m�ۂɐݒ�
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
	// ���m�ۃu���b�N����ݒ�
	//
//	*(GetFreeBlockInfo(pBlock)) = vfbi ;
	//
	// ���݂̃u���b�N�ƒ���̖��m�ۃu���b�N�ƘA��
	//
	if ( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) )
	{
		VAR_HEAP_BLOCK *	pNext = GetNextBlock( pBlock ) ;
		if ( pNext->dwSize & VAR_HEAP_FREE_FLAG )
		{
			//
			// ����̃u���b�N�͖��m�ۗ̈� -> �A��
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
	// �q�[�v�̘A���ő�m�ۉ\�T�C�Y���X�V����
	//
	DWORD	dwSize = GetBlockLength( pBlock ) ;
	SDWORD	dwMaxMask = ((SDWORD)(hTarget->dwMaxFreeArea - dwSize)) >> 31 ;
	hTarget->dwMaxFreeArea =
		(dwSize & dwMaxMask) | (hTarget->dwMaxFreeArea & ~dwMaxMask) ;
	//
	// �q�[�v���̂��̂̉��
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
				// �q�[�v�𑦎��ɉ�����邩���f
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
				// �q�[�v�`�F�[����ؒf
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
				// �q�[�v�����
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
			// �q�[�v�`�F�[�����œK��
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
// �������̍Ċm��
//////////////////////////////////////////////////////////////////////////////

void * eslHeapReallocate
	( HESLHEAP hHeap, void * ptrObj, DWORD dwSize, DWORD dwFlags )
{
	//
	// �|�C���^�̗L�����̌���
	//
	if ( ptrObj == NULL )
	{
		return	eslHeapAllocate( hHeap, dwSize, dwFlags ) ;
	}
	if ( ((DWORD)ptrObj) & (0x07 >> HEAP_LEN_SCALE) )
	{
		ESLTrace( "�����ȃ|�C���^��������悤�Ƃ��܂����B\n" ) ;
		return	NULL ;
	}
	VAR_HEAP_BLOCK *	pBlock =
		(VAR_HEAP_BLOCK*) (((DWORD) ptrObj) - sizeof(VAR_HEAP_BLOCK)) ;
	if ( (pBlock->dwSize & (VAR_HEAP_FREE_FLAG | VAR_HEAP_FIRST_FLAG))
		|| !(pBlock->dwSize & VAR_HEAP_SIGNATURE) )
	{
		ESLTrace( "�����ȃ|�C���^��������悤�Ƃ��܂����B\n" ) ;
		return	NULL ;
	}
	//
	// �q�[�v�w�b�_���擾����
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
			ESLTrace( "�����ȃ������u���b�N���q�[�v���������悤�Ƃ��܂����B\n" ) ;
			return	NULL ;
		}
		hTarget = hNextHeap ;
	}
	//
	// �Ċm�ۏ���
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
		// �������T�C�Y�̏k��
		//
		if ( !(pBlock->dwSize & VAR_HEAP_LAST_FLAG) )
		{
			VAR_HEAP_BLOCK *	pNext = GetNextBlock( pBlock ) ;
			if ( pNext->dwSize & VAR_HEAP_FREE_FLAG )
			{
				//
				// ����̖��m�ۗ̈�̃T�C�Y���g��
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
		// �������T�C�Y�̊g��
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
			// ����̖��m�ۗ̈���m��
			//
			DWORD	dwGrowSize = dwAlignSize - dwMemSize ;
			DWORD	dwOldSize = dwMemSize ;
			//
			if ( dwGrowSize < (GetBlockLength(pNext) / 2) )
			{
				//
				// �u���b�N�𕪊����čĊm��
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
				// �u���b�N�����̂܂܊��蓖�Ă�
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
			// �ʂ̗̈�Ƀ��������Ċm��
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
// �������T�C�Y�̎擾�i�������̗L�����̌��؁j
//////////////////////////////////////////////////////////////////////////////

DWORD eslHeapGetLength( HESLHEAP hHeap, void * ptrObj )
{
	try
	{
		//
		// �|�C���^�̗L�����̌���
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
		// �T�C�Y�̎擾
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
		// �|�C���^�͖���
		//
		return	ESL_INVALID_HEAP ;
	}
}


//////////////////////////////////////////////////////////////////////////////
// �q�[�v�ɑ΂��鑼�̃X���b�h����̑����ҋ@
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
// �q�[�v�ɑ΂��鑼�̃X���b�h����̑���̑ҋ@������
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
// �q�[�v�Ɋm�ۂ���Ă��郁�����u���b�N���f�o�b�O�o�͂փ_���v
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
							( "ESL �̉������Ă��Ȃ��������u���b�N������܂�\n" ) ;
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

