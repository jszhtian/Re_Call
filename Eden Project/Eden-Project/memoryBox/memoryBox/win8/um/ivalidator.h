

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

#ifndef __IValidator_h__
#define __IValidator_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IValidator_FWD_DEFINED__
#define __IValidator_FWD_DEFINED__
typedef interface IValidator IValidator;

#endif 	/* __IValidator_FWD_DEFINED__ */


#ifndef __ICLRValidator_FWD_DEFINED__
#define __ICLRValidator_FWD_DEFINED__
typedef interface ICLRValidator ICLRValidator;

#endif 	/* __ICLRValidator_FWD_DEFINED__ */


/* header files for imported files */
#include "ivehandler.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_IValidator_0000_0000 */
/* [local] */ 

#pragma warning(push)
#pragma warning(disable:28718)    



enum ValidatorFlags
    {
        VALIDATOR_EXTRA_VERBOSE	= 0x1,
        VALIDATOR_SHOW_SOURCE_LINES	= 0x2,
        VALIDATOR_CHECK_ILONLY	= 0x4,
        VALIDATOR_CHECK_PEFORMAT_ONLY	= 0x8,
        VALIDATOR_NOCHECK_PEFORMAT	= 0x10,
        VALIDATOR_TRANSPARENT_ONLY	= 0x20
    } ;


extern RPC_IF_HANDLE __MIDL_itf_IValidator_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_IValidator_0000_0000_v0_0_s_ifspec;

#ifndef __IValidator_INTERFACE_DEFINED__
#define __IValidator_INTERFACE_DEFINED__

/* interface IValidator */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IValidator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("63DF8730-DC81-4062-84A2-1FF943F59FAC")
    IValidator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Validate( 
            /* [in] */ __RPC__in_opt IVEHandler *veh,
            /* [in] */ __RPC__in_opt IUnknown *pAppDomain,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ unsigned long ulMaxError,
            /* [in] */ unsigned long token,
            /* [in] */ __RPC__in LPWSTR fileName,
            /* [size_is][in] */ __RPC__in_ecount_full(ulSize) BYTE *pe,
            /* [in] */ unsigned long ulSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FormatEventInfo( 
            /* [in] */ HRESULT hVECode,
            /* [in] */ VEContext Context,
            /* [out][in] */ __RPC__inout LPWSTR msg,
            /* [in] */ unsigned long ulMaxLength,
            /* [in] */ __RPC__in SAFEARRAY * psa) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IValidatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IValidator * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IValidator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IValidator * This);
        
        HRESULT ( STDMETHODCALLTYPE *Validate )( 
            __RPC__in IValidator * This,
            /* [in] */ __RPC__in_opt IVEHandler *veh,
            /* [in] */ __RPC__in_opt IUnknown *pAppDomain,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ unsigned long ulMaxError,
            /* [in] */ unsigned long token,
            /* [in] */ __RPC__in LPWSTR fileName,
            /* [size_is][in] */ __RPC__in_ecount_full(ulSize) BYTE *pe,
            /* [in] */ unsigned long ulSize);
        
        HRESULT ( STDMETHODCALLTYPE *FormatEventInfo )( 
            __RPC__in IValidator * This,
            /* [in] */ HRESULT hVECode,
            /* [in] */ VEContext Context,
            /* [out][in] */ __RPC__inout LPWSTR msg,
            /* [in] */ unsigned long ulMaxLength,
            /* [in] */ __RPC__in SAFEARRAY * psa);
        
        END_INTERFACE
    } IValidatorVtbl;

    interface IValidator
    {
        CONST_VTBL struct IValidatorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IValidator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IValidator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IValidator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IValidator_Validate(This,veh,pAppDomain,ulFlags,ulMaxError,token,fileName,pe,ulSize)	\
    ( (This)->lpVtbl -> Validate(This,veh,pAppDomain,ulFlags,ulMaxError,token,fileName,pe,ulSize) ) 

#define IValidator_FormatEventInfo(This,hVECode,Context,msg,ulMaxLength,psa)	\
    ( (This)->lpVtbl -> FormatEventInfo(This,hVECode,Context,msg,ulMaxLength,psa) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IValidator_INTERFACE_DEFINED__ */


#ifndef __ICLRValidator_INTERFACE_DEFINED__
#define __ICLRValidator_INTERFACE_DEFINED__

/* interface ICLRValidator */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICLRValidator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("63DF8730-DC81-4062-84A2-1FF943F59FDD")
    ICLRValidator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Validate( 
            /* [in] */ __RPC__in_opt IVEHandler *veh,
            /* [in] */ unsigned long ulAppDomainId,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ unsigned long ulMaxError,
            /* [in] */ unsigned long token,
            /* [in] */ __RPC__in LPWSTR fileName,
            /* [size_is][in] */ __RPC__in_ecount_full(ulSize) BYTE *pe,
            /* [in] */ unsigned long ulSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FormatEventInfo( 
            /* [in] */ HRESULT hVECode,
            /* [in] */ VEContext Context,
            /* [out][in] */ __RPC__inout LPWSTR msg,
            /* [in] */ unsigned long ulMaxLength,
            /* [in] */ __RPC__in SAFEARRAY * psa) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICLRValidatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in ICLRValidator * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in ICLRValidator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in ICLRValidator * This);
        
        HRESULT ( STDMETHODCALLTYPE *Validate )( 
            __RPC__in ICLRValidator * This,
            /* [in] */ __RPC__in_opt IVEHandler *veh,
            /* [in] */ unsigned long ulAppDomainId,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ unsigned long ulMaxError,
            /* [in] */ unsigned long token,
            /* [in] */ __RPC__in LPWSTR fileName,
            /* [size_is][in] */ __RPC__in_ecount_full(ulSize) BYTE *pe,
            /* [in] */ unsigned long ulSize);
        
        HRESULT ( STDMETHODCALLTYPE *FormatEventInfo )( 
            __RPC__in ICLRValidator * This,
            /* [in] */ HRESULT hVECode,
            /* [in] */ VEContext Context,
            /* [out][in] */ __RPC__inout LPWSTR msg,
            /* [in] */ unsigned long ulMaxLength,
            /* [in] */ __RPC__in SAFEARRAY * psa);
        
        END_INTERFACE
    } ICLRValidatorVtbl;

    interface ICLRValidator
    {
        CONST_VTBL struct ICLRValidatorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICLRValidator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICLRValidator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICLRValidator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICLRValidator_Validate(This,veh,ulAppDomainId,ulFlags,ulMaxError,token,fileName,pe,ulSize)	\
    ( (This)->lpVtbl -> Validate(This,veh,ulAppDomainId,ulFlags,ulMaxError,token,fileName,pe,ulSize) ) 

#define ICLRValidator_FormatEventInfo(This,hVECode,Context,msg,ulMaxLength,psa)	\
    ( (This)->lpVtbl -> FormatEventInfo(This,hVECode,Context,msg,ulMaxLength,psa) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICLRValidator_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_IValidator_0000_0002 */
/* [local] */ 

#pragma warning(pop)


extern RPC_IF_HANDLE __MIDL_itf_IValidator_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_IValidator_0000_0002_v0_0_s_ifspec;

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


