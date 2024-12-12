
/*****************************************************************************
           ESL (Entis Standard Library) heap core implementation
 ----------------------------------------------------------------------------
        Copyright (c) 2002-2003 Leshade Entis. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <eritypes.h>
#include <esl.h>


//////////////////////////////////////////////////////////////////////////////
// メモリアロケーション
//////////////////////////////////////////////////////////////////////////////

#if	!defined(_MFC_VER) && !defined(_DISABLE_ESL_NEW)

void * operator new ( size_t stObj )
{
	return	::eslHeapAllocate( NULL, stObj, 0 ) ;
}

void * operator new ( size_t stObj, const char * pszFileName, int nLine )
{
	return	::eslHeapAllocate( NULL, stObj, 0 ) ;
}

void operator delete ( void * ptrObj )
{
	if ( ptrObj != NULL )
	{
		::eslHeapFree( NULL, ptrObj, 0 ) ;
	}
}

#endif


