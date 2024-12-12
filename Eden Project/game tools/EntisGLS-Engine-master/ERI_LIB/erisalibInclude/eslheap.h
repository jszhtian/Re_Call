
/*****************************************************************************
          ESL (Entis Standard Library) heap core declarrations
 ----------------------------------------------------------------------------
         Copyright (c) 2002-2004 Leshade Entis. All rights reserved.
 *****************************************************************************/


#if	!defined(__ESLHEAP_H__)
#define	__ESLHEAP_H__

//#if	sizeof(DWORD) != sizeof(void*)
//#error	ESL �q�[�v�֐��� 32 �r�b�g���łȂ���΂Ȃ�܂���B
//#endif


//////////////////////////////////////////////////////////////////////////////
// �^�̒�`
//////////////////////////////////////////////////////////////////////////////

typedef	struct tagHSTACKHEAP *	HSTACKHEAP ;
typedef	struct tagHESLHEAP *	HESLHEAP ;


//////////////////////////////////////////////////////////////////////////////
// �֐��̒�`
//////////////////////////////////////////////////////////////////////////////

//
// ����������֐�
//

extern	"C"
{
	GLSEXPORT void eslFillMemory
		( void * ptrMem, BYTE bytData, DWORD dwLength ) ;
	GLSEXPORT void eslMoveMemory
		( void * ptrDst, const void * ptrSrc, DWORD dwLength ) ;
} ;

//
// �X�^�b�N���q�[�v�֐�
//
extern	"C"
{
	GLSEXPORT HSTACKHEAP eslStackHeapCreate
		( DWORD dwInitSize, DWORD dwGrowSize, DWORD dwFlags ) ;
	GLSEXPORT void eslStackHeapDestroy( HSTACKHEAP hHeap ) ;
	GLSEXPORT void * eslStackHeapAllocate( HSTACKHEAP hHeap, DWORD dwSize ) ;
	GLSEXPORT void eslStackHeapFree( HSTACKHEAP hHeap ) ;
} ;

//
// �ėp�q�[�v�֐�
//
extern	"C"
{
	GLSEXPORT HESLHEAP eslGetGlobalHeap( void ) ;
	GLSEXPORT void eslFreeGlobalHeap( void ) ;
	GLSEXPORT HESLHEAP eslHeapCreate
		( DWORD dwInitSize = 0, DWORD dwGrowSize = 0,
			DWORD dwFlags = 0, HESLHEAP hParentHeap = NULL ) ;
	GLSEXPORT void eslHeapDestroy( HESLHEAP hHeap ) ;
	GLSEXPORT void * eslHeapAllocate
		( HESLHEAP hHeap, DWORD dwSize, DWORD dwFlags ) ;
	GLSEXPORT void eslHeapFree
		( HESLHEAP hHeap, void * ptrObj, DWORD dwFlags = 0 ) ;
	GLSEXPORT void * eslHeapReallocate
		( HESLHEAP hHeap, void * ptrObj, DWORD dwSize, DWORD dwFlags ) ;
	GLSEXPORT DWORD eslHeapGetLength( HESLHEAP hHeap, void * ptrObj ) ;
	GLSEXPORT ESLError eslVerifyHeapChain( HESLHEAP hHeap ) ;
	GLSEXPORT void eslHeapLock( HESLHEAP hHeap ) ;
	GLSEXPORT void eslHeapUnlock( HESLHEAP hHeap ) ;
	GLSEXPORT void eslHeapDump( HESLHEAP hHeap, int nLimit ) ;
} ;

#define	ESL_HEAP_ZERO_INIT		0x00000001
#define	ESL_HEAP_NO_SERIALIZE	0x00000002
#define	ESL_INVALID_HEAP		(0xFFFFFFFFUL)


#endif

