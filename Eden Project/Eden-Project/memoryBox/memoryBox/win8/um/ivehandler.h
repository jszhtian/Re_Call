

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0594 */
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IVEHandler_h__
#define __IVEHandler_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __VEHandlerClass_FWD_DEFINED__
#define __VEHandlerClass_FWD_DEFINED__

#ifdef __cplusplus
typedef class VEHandlerClass VEHandlerClass;
#else
typedef struct VEHandlerClass VEHandlerClass;
#endif /* __cplusplus */

#endif 	/* __VEHandlerClass_FWD_DEFINED__ */


#ifndef __IVEHandler_FWD_DEFINED__
#define __IVEHandler_FWD_DEFINED__
typedef interface IVEHandler IVEHandler;

#endif 	/* __IVEHandler_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_IVEHandler_0000_0000 */
/* [local] */ 

typedef struct tag_VerError
    {
    unsigned long flags;
    unsigned long opcode;
    unsigned long uOffset;
    unsigned long Token;
    unsigned long item1_flags;
    int *item1_data;
    unsigned long item2_flags;
    int *item2_data;
    } 	_VerError;

typedef _VerError VEContext;




extern RPC_IF_HANDLE __MIDL_itf_IVEHandler_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_IVEHandler_0000_0000_v0_0_s_ifspec;


#ifndef __VEHandlerLib_LIBRARY_DEFINED__
#define __VEHandlerLib_LIBRARY_DEFINED__

/* library VEHandlerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_VEHandlerLib;

EXTERN_C const CLSID CLSID_VEHandlerClass;

#ifdef __cplusplus

class DECLSPEC_UUID("856CA1B1-7DAB-11d3-ACEC-00C04F86C309")
VEHandlerClass;
#endif
#endif /* __VEHandlerLib_LIBRARY_DEFINED__ */

#ifndef __IVEHandler_INTERFACE_DEFINED__
#define __IVEHandler_INTERFACE_DEFINED__

/* interface IVEHandler */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IVEHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("856CA1B2-7DAB-11d3-ACEC-00C04F86C309")
    IVEHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE VEHandler( 
            /* [in] */ HRESULT VECode,
            /* [in] */ VEContext Context,
            /* [in] */ __RPC__in SAFEARRAY * psa) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetReporterFtn( 
            /* [in] */ __int64 lFnPtr) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IVEHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IVEHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IVEHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IVEHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *VEHandler )( 
            __RPC__in IVEHandler * This,
            /* [in] */ HRESULT VECode,
            /* [in] */ VEContext Context,
            /* [in] */ __RPC__in SAFEARRAY * psa);
        
        HRESULT ( STDMETHODCALLTYPE *SetReporterFtn )( 
            __RPC__in IVEHandler * This,
            /* [in] */ __int64 lFnPtr);
        
        END_INTERFACE
    } IVEHandlerVtbl;

    interface IVEHandler
    {
        CONST_VTBL struct IVEHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVEHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVEHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVEHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVEHandler_VEHandler(This,VECode,Context,psa)	\
    ( (This)->lpVtbl -> VEHandler(This,VECode,Context,psa) ) 

#define IVEHandler_SetReporterFtn(This,lFnPtr)	\
    ( (This)->lpVtbl -> SetReporterFtn(This,lFnPtr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVEHandler_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     __RPC__in unsigned long *, unsigned long            , __RPC__in LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  __RPC__in unsigned long *, __RPC__inout_xcount(0) unsigned char *, __RPC__in LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(__RPC__in unsigned long *, __RPC__in_xcount(0) unsigned char *, __RPC__out LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     __RPC__in unsigned long *, __RPC__in LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


