

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Mar 28 11:57:28 2012
 */
/* Compiler settings for .\DTKANPR.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __DTKANPR_h__
#define __DTKANPR_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILicManager_FWD_DEFINED__
#define __ILicManager_FWD_DEFINED__
typedef interface ILicManager ILicManager;
#endif 	/* __ILicManager_FWD_DEFINED__ */


#ifndef __IPlate_FWD_DEFINED__
#define __IPlate_FWD_DEFINED__
typedef interface IPlate IPlate;
#endif 	/* __IPlate_FWD_DEFINED__ */


#ifndef __IPlateCollection_FWD_DEFINED__
#define __IPlateCollection_FWD_DEFINED__
typedef interface IPlateCollection IPlateCollection;
#endif 	/* __IPlateCollection_FWD_DEFINED__ */


#ifndef __IANPREngine_FWD_DEFINED__
#define __IANPREngine_FWD_DEFINED__
typedef interface IANPREngine IANPREngine;
#endif 	/* __IANPREngine_FWD_DEFINED__ */


#ifndef __IVideoCapture_FWD_DEFINED__
#define __IVideoCapture_FWD_DEFINED__
typedef interface IVideoCapture IVideoCapture;
#endif 	/* __IVideoCapture_FWD_DEFINED__ */


#ifndef ___IANPREngineEvents_FWD_DEFINED__
#define ___IANPREngineEvents_FWD_DEFINED__
typedef interface _IANPREngineEvents _IANPREngineEvents;
#endif 	/* ___IANPREngineEvents_FWD_DEFINED__ */


#ifndef __ANPREngine_FWD_DEFINED__
#define __ANPREngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class ANPREngine ANPREngine;
#else
typedef struct ANPREngine ANPREngine;
#endif /* __cplusplus */

#endif 	/* __ANPREngine_FWD_DEFINED__ */


#ifndef __Plate_FWD_DEFINED__
#define __Plate_FWD_DEFINED__

#ifdef __cplusplus
typedef class Plate Plate;
#else
typedef struct Plate Plate;
#endif /* __cplusplus */

#endif 	/* __Plate_FWD_DEFINED__ */


#ifndef __PlateCollection_FWD_DEFINED__
#define __PlateCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class PlateCollection PlateCollection;
#else
typedef struct PlateCollection PlateCollection;
#endif /* __cplusplus */

#endif 	/* __PlateCollection_FWD_DEFINED__ */


#ifndef ___IVideoCaptureEvents_FWD_DEFINED__
#define ___IVideoCaptureEvents_FWD_DEFINED__
typedef interface _IVideoCaptureEvents _IVideoCaptureEvents;
#endif 	/* ___IVideoCaptureEvents_FWD_DEFINED__ */


#ifndef __VideoCapture_FWD_DEFINED__
#define __VideoCapture_FWD_DEFINED__

#ifdef __cplusplus
typedef class VideoCapture VideoCapture;
#else
typedef struct VideoCapture VideoCapture;
#endif /* __cplusplus */

#endif 	/* __VideoCapture_FWD_DEFINED__ */


#ifndef __LicManager_FWD_DEFINED__
#define __LicManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class LicManager LicManager;
#else
typedef struct LicManager LicManager;
#endif /* __cplusplus */

#endif 	/* __LicManager_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DTKANPR_0000_0000 */
/* [local] */ 

typedef /* [public][v1_enum] */ 
enum ConfidenceLevelEnum
    {	CL_Unknown	= 0,
	CL_Excellent	= 1,
	CL_Good	= 2,
	CL_Moderate	= 3,
	CL_Fair	= 4,
	CL_Poor	= 5
    } 	ConfidenceLevelEnum;

typedef /* [public][v1_enum] */ 
enum PlateColorSchemaEnum
    {	PCS_Unknown	= 0,
	PCS_BlackOnWhite	= 0x1,
	PCS_WhiteOnBlack	= 0x2,
	PCS_All	= 0x3
    } 	PlateColorSchemaEnum;

typedef /* [public][v1_enum] */ 
enum ImageContrastLevelEnum
    {	ICL_Unknown	= 0,
	ICL_Low	= 0x1,
	ICL_Medium	= 0x2,
	ICL_High	= 0x3
    } 	ImageContrastLevelEnum;

typedef /* [public][v1_enum] */ 
enum PreciseModeEnum
    {	PM_Normal	= 0,
		PM_Mode1	= 0x1,
		PM_Mode2	= 0x2,
		PM_Night	= 0x3
    } 	PreciseModeEnum;

typedef /* [public][v1_enum] */ 
enum LicenseTypeEnum
    {	LT_Undefined	= 0,
	LT_Developer	= 0x1,
	LT_DeveloperSIM	= 0x2,
	LT_Runtime	= 0x4,
	LT_RuntimeSIM	= 0x8,
	LT_URuntime	= 0x10,
	LT_URuntimeSIM	= 0x20,
	LT_Runtime2	= 0x40
    } 	LicenseTypeEnum;

typedef /* [public][v1_enum] */ 
enum CameraTypeEnum
    {	CT_Analog	= 0,
	CT_IPCamera	= 0x1
    } 	CameraTypeEnum;



extern RPC_IF_HANDLE __MIDL_itf_DTKANPR_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DTKANPR_0000_0000_v0_0_s_ifspec;

#ifndef __ILicManager_INTERFACE_DEFINED__
#define __ILicManager_INTERFACE_DEFINED__

/* interface ILicManager */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILicManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("11F82B1C-CBAB-4AB2-9870-7B1EF80321BB")
    ILicManager : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLicenseKey( 
            BSTR licenseKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ActivateLicenseOnline( 
            BSTR LicenseKey,
            BSTR *errorMessage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetActivationLink( 
            BSTR licenseKey,
            BSTR *ActivationLink) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetActivationCode( 
            BSTR ActivationCode) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsLicensed( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDeveloperLicenseKey( 
            BSTR devLicKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRuntimeLicenseExpDate( 
            /* [retval][out] */ DATE *date) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RequestTrialLicenseKey( 
            /* [retval][out] */ BSTR *licenseKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RequestTrialLicenseKeyEmail( 
            /* [out] */ BSTR *licenseKey,
            /* [in] */ BSTR email) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LicenseType( 
            /* [retval][out] */ LicenseTypeEnum *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeactivateLicense( 
            BSTR LicenseKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXValue( 
            LONG valueID,
            BSTR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILicManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILicManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILicManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILicManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILicManager * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILicManager * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILicManager * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILicManager * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddLicenseKey )( 
            ILicManager * This,
            BSTR licenseKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ActivateLicenseOnline )( 
            ILicManager * This,
            BSTR LicenseKey,
            BSTR *errorMessage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetActivationLink )( 
            ILicManager * This,
            BSTR licenseKey,
            BSTR *ActivationLink);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetActivationCode )( 
            ILicManager * This,
            BSTR ActivationCode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsLicensed )( 
            ILicManager * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDeveloperLicenseKey )( 
            ILicManager * This,
            BSTR devLicKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRuntimeLicenseExpDate )( 
            ILicManager * This,
            /* [retval][out] */ DATE *date);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RequestTrialLicenseKey )( 
            ILicManager * This,
            /* [retval][out] */ BSTR *licenseKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RequestTrialLicenseKeyEmail )( 
            ILicManager * This,
            /* [out] */ BSTR *licenseKey,
            /* [in] */ BSTR email);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LicenseType )( 
            ILicManager * This,
            /* [retval][out] */ LicenseTypeEnum *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeactivateLicense )( 
            ILicManager * This,
            BSTR LicenseKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXValue )( 
            ILicManager * This,
            LONG valueID,
            BSTR *value);
        
        END_INTERFACE
    } ILicManagerVtbl;

    interface ILicManager
    {
        CONST_VTBL struct ILicManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILicManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILicManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILicManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILicManager_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILicManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILicManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILicManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILicManager_AddLicenseKey(This,licenseKey)	\
    ( (This)->lpVtbl -> AddLicenseKey(This,licenseKey) ) 

#define ILicManager_ActivateLicenseOnline(This,LicenseKey,errorMessage)	\
    ( (This)->lpVtbl -> ActivateLicenseOnline(This,LicenseKey,errorMessage) ) 

#define ILicManager_GetActivationLink(This,licenseKey,ActivationLink)	\
    ( (This)->lpVtbl -> GetActivationLink(This,licenseKey,ActivationLink) ) 

#define ILicManager_SetActivationCode(This,ActivationCode)	\
    ( (This)->lpVtbl -> SetActivationCode(This,ActivationCode) ) 

#define ILicManager_get_IsLicensed(This,pVal)	\
    ( (This)->lpVtbl -> get_IsLicensed(This,pVal) ) 

#define ILicManager_SetDeveloperLicenseKey(This,devLicKey)	\
    ( (This)->lpVtbl -> SetDeveloperLicenseKey(This,devLicKey) ) 

#define ILicManager_GetRuntimeLicenseExpDate(This,date)	\
    ( (This)->lpVtbl -> GetRuntimeLicenseExpDate(This,date) ) 

#define ILicManager_RequestTrialLicenseKey(This,licenseKey)	\
    ( (This)->lpVtbl -> RequestTrialLicenseKey(This,licenseKey) ) 

#define ILicManager_RequestTrialLicenseKeyEmail(This,licenseKey,email)	\
    ( (This)->lpVtbl -> RequestTrialLicenseKeyEmail(This,licenseKey,email) ) 

#define ILicManager_get_LicenseType(This,pVal)	\
    ( (This)->lpVtbl -> get_LicenseType(This,pVal) ) 

#define ILicManager_DeactivateLicense(This,LicenseKey)	\
    ( (This)->lpVtbl -> DeactivateLicense(This,LicenseKey) ) 

#define ILicManager_GetXValue(This,valueID,value)	\
    ( (This)->lpVtbl -> GetXValue(This,valueID,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILicManager_INTERFACE_DEFINED__ */


#ifndef __IPlate_INTERFACE_DEFINED__
#define __IPlate_INTERFACE_DEFINED__

/* interface IPlate */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPlate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40A38BF9-BDA1-48FB-9BEB-2E59F141A1B1")
    IPlate : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Right( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Bottom( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ConfidenceLevel( 
            /* [retval][out] */ ConfidenceLevelEnum *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner1X( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner1Y( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner2X( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner2Y( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner3X( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner3Y( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner4X( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Corner4Y( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CustomData( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Bitmap( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CountryCode( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TrendX( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TrendY( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_State( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPlateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPlate * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPlate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPlate * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPlate * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPlate * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPlate * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPlate * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Text )( 
            IPlate * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Right )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Bottom )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConfidenceLevel )( 
            IPlate * This,
            /* [retval][out] */ ConfidenceLevelEnum *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner1X )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner1Y )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner2X )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner2Y )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner3X )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner3Y )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner4X )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Corner4Y )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CustomData )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Bitmap )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CountryCode )( 
            IPlate * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TrendX )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TrendY )( 
            IPlate * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_State )( 
            IPlate * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IPlateVtbl;

    interface IPlate
    {
        CONST_VTBL struct IPlateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPlate_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPlate_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPlate_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPlate_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPlate_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPlate_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPlate_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPlate_get_Text(This,pVal)	\
    ( (This)->lpVtbl -> get_Text(This,pVal) ) 

#define IPlate_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IPlate_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IPlate_get_Right(This,pVal)	\
    ( (This)->lpVtbl -> get_Right(This,pVal) ) 

#define IPlate_get_Bottom(This,pVal)	\
    ( (This)->lpVtbl -> get_Bottom(This,pVal) ) 

#define IPlate_get_ConfidenceLevel(This,pVal)	\
    ( (This)->lpVtbl -> get_ConfidenceLevel(This,pVal) ) 

#define IPlate_get_Corner1X(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner1X(This,pVal) ) 

#define IPlate_get_Corner1Y(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner1Y(This,pVal) ) 

#define IPlate_get_Corner2X(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner2X(This,pVal) ) 

#define IPlate_get_Corner2Y(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner2Y(This,pVal) ) 

#define IPlate_get_Corner3X(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner3X(This,pVal) ) 

#define IPlate_get_Corner3Y(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner3Y(This,pVal) ) 

#define IPlate_get_Corner4X(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner4X(This,pVal) ) 

#define IPlate_get_Corner4Y(This,pVal)	\
    ( (This)->lpVtbl -> get_Corner4Y(This,pVal) ) 

#define IPlate_get_CustomData(This,pVal)	\
    ( (This)->lpVtbl -> get_CustomData(This,pVal) ) 

#define IPlate_get_Bitmap(This,pVal)	\
    ( (This)->lpVtbl -> get_Bitmap(This,pVal) ) 

#define IPlate_get_CountryCode(This,pVal)	\
    ( (This)->lpVtbl -> get_CountryCode(This,pVal) ) 

#define IPlate_get_TrendX(This,pVal)	\
    ( (This)->lpVtbl -> get_TrendX(This,pVal) ) 

#define IPlate_get_TrendY(This,pVal)	\
    ( (This)->lpVtbl -> get_TrendY(This,pVal) ) 

#define IPlate_get_State(This,pVal)	\
    ( (This)->lpVtbl -> get_State(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPlate_INTERFACE_DEFINED__ */


#ifndef __IPlateCollection_INTERFACE_DEFINED__
#define __IPlateCollection_INTERFACE_DEFINED__

/* interface IPlateCollection */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPlateCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6B821FDF-2CDF-42BE-B088-EA4E4F3C9562")
    IPlateCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ LONG index,
            /* [retval][out] */ IPlate **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ LONG *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPlateCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPlateCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPlateCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPlateCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPlateCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPlateCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPlateCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPlateCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IPlateCollection * This,
            /* [in] */ LONG index,
            /* [retval][out] */ IPlate **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IPlateCollection * This,
            /* [retval][out] */ LONG *pVal);
        
        END_INTERFACE
    } IPlateCollectionVtbl;

    interface IPlateCollection
    {
        CONST_VTBL struct IPlateCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPlateCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPlateCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPlateCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPlateCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPlateCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPlateCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPlateCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPlateCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#define IPlateCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPlateCollection_INTERFACE_DEFINED__ */


#ifndef __IANPREngine_INTERFACE_DEFINED__
#define __IANPREngine_INTERFACE_DEFINED__

/* interface IANPREngine */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IANPREngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6792EB63-44F3-436D-AF22-790727FA15F6")
    IANPREngine : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadFromFile( 
            BSTR fileName,
            LONG customData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadFromBitmap( 
            LONG hBmp,
            LONG customData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadFromDIB( 
            LONG hDIB,
            LONG customData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadFromMemFile( 
            VARIANT fileData,
            LONG dataLen,
            LONG customData) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxCharHeight( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxCharHeight( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MinCharHeight( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MinCharHeight( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LicenseManager( 
            /* [retval][out] */ ILicManager **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetScanRectangle( 
            LONG left,
            LONG top,
            LONG right,
            LONG bottom) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Deinterlace( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Deinterlace( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlateColorSchema( 
            /* [retval][out] */ PlateColorSchemaEnum *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PlateColorSchema( 
            /* [in] */ PlateColorSchemaEnum newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCountryCode( 
            BSTR countryCode) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ContrastLevel( 
            /* [retval][out] */ ImageContrastLevelEnum *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ContrastLevel( 
            /* [in] */ ImageContrastLevelEnum newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            VARIANT_BOOL bVideo) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Plates( 
            /* [retval][out] */ IPlateCollection **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DeinterlacedSource( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DeinterlacedSource( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotateAngle( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RotateAngle( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PreciseMode( 
            /* [retval][out] */ PreciseModeEnum *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PreciseMode( 
            /* [in] */ PreciseModeEnum newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLastErrorText( 
            BSTR *errorText) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadFromBuffer( 
            LONG pBuffer,
            LONG width,
            LONG height,
            LONG stride,
            LONG bpp,
            LONG customData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMaskFromFile( 
            BSTR imageFileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMaskFromBitmap( 
            LONG hBitmap) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HistogramEqualization( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_HistogramEqualization( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MotionDetectionTriggering( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MotionDetectionTriggering( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Fps( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Fps( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RecognitionTime( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviationAngle( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DeviationAngle( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlatePresenceTime( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PlatePresenceTime( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetXOption( 
            BSTR optionName,
            BSTR value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IANPREngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IANPREngine * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IANPREngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IANPREngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IANPREngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IANPREngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IANPREngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IANPREngine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadFromFile )( 
            IANPREngine * This,
            BSTR fileName,
            LONG customData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadFromBitmap )( 
            IANPREngine * This,
            LONG hBmp,
            LONG customData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadFromDIB )( 
            IANPREngine * This,
            LONG hDIB,
            LONG customData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadFromMemFile )( 
            IANPREngine * This,
            VARIANT fileData,
            LONG dataLen,
            LONG customData);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxCharHeight )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MaxCharHeight )( 
            IANPREngine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MinCharHeight )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MinCharHeight )( 
            IANPREngine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LicenseManager )( 
            IANPREngine * This,
            /* [retval][out] */ ILicManager **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetScanRectangle )( 
            IANPREngine * This,
            LONG left,
            LONG top,
            LONG right,
            LONG bottom);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Deinterlace )( 
            IANPREngine * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Deinterlace )( 
            IANPREngine * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlateColorSchema )( 
            IANPREngine * This,
            /* [retval][out] */ PlateColorSchemaEnum *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PlateColorSchema )( 
            IANPREngine * This,
            /* [in] */ PlateColorSchemaEnum newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCountryCode )( 
            IANPREngine * This,
            BSTR countryCode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ContrastLevel )( 
            IANPREngine * This,
            /* [retval][out] */ ImageContrastLevelEnum *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ContrastLevel )( 
            IANPREngine * This,
            /* [in] */ ImageContrastLevelEnum newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            IANPREngine * This,
            VARIANT_BOOL bVideo);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Plates )( 
            IANPREngine * This,
            /* [retval][out] */ IPlateCollection **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeinterlacedSource )( 
            IANPREngine * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DeinterlacedSource )( 
            IANPREngine * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotateAngle )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RotateAngle )( 
            IANPREngine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PreciseMode )( 
            IANPREngine * This,
            /* [retval][out] */ PreciseModeEnum *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PreciseMode )( 
            IANPREngine * This,
            /* [in] */ PreciseModeEnum newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLastErrorText )( 
            IANPREngine * This,
            BSTR *errorText);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadFromBuffer )( 
            IANPREngine * This,
            LONG pBuffer,
            LONG width,
            LONG height,
            LONG stride,
            LONG bpp,
            LONG customData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMaskFromFile )( 
            IANPREngine * This,
            BSTR imageFileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMaskFromBitmap )( 
            IANPREngine * This,
            LONG hBitmap);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HistogramEqualization )( 
            IANPREngine * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HistogramEqualization )( 
            IANPREngine * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MotionDetectionTriggering )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MotionDetectionTriggering )( 
            IANPREngine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Fps )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Fps )( 
            IANPREngine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RecognitionTime )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IANPREngine * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviationAngle )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DeviationAngle )( 
            IANPREngine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlatePresenceTime )( 
            IANPREngine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PlatePresenceTime )( 
            IANPREngine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetXOption )( 
            IANPREngine * This,
            BSTR optionName,
            BSTR value);
        
        END_INTERFACE
    } IANPREngineVtbl;

    interface IANPREngine
    {
        CONST_VTBL struct IANPREngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IANPREngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IANPREngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IANPREngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IANPREngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IANPREngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IANPREngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IANPREngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IANPREngine_ReadFromFile(This,fileName,customData)	\
    ( (This)->lpVtbl -> ReadFromFile(This,fileName,customData) ) 

#define IANPREngine_ReadFromBitmap(This,hBmp,customData)	\
    ( (This)->lpVtbl -> ReadFromBitmap(This,hBmp,customData) ) 

#define IANPREngine_ReadFromDIB(This,hDIB,customData)	\
    ( (This)->lpVtbl -> ReadFromDIB(This,hDIB,customData) ) 

#define IANPREngine_ReadFromMemFile(This,fileData,dataLen,customData)	\
    ( (This)->lpVtbl -> ReadFromMemFile(This,fileData,dataLen,customData) ) 

#define IANPREngine_get_MaxCharHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_MaxCharHeight(This,pVal) ) 

#define IANPREngine_put_MaxCharHeight(This,newVal)	\
    ( (This)->lpVtbl -> put_MaxCharHeight(This,newVal) ) 

#define IANPREngine_get_MinCharHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_MinCharHeight(This,pVal) ) 

#define IANPREngine_put_MinCharHeight(This,newVal)	\
    ( (This)->lpVtbl -> put_MinCharHeight(This,newVal) ) 

#define IANPREngine_get_LicenseManager(This,pVal)	\
    ( (This)->lpVtbl -> get_LicenseManager(This,pVal) ) 

#define IANPREngine_SetScanRectangle(This,left,top,right,bottom)	\
    ( (This)->lpVtbl -> SetScanRectangle(This,left,top,right,bottom) ) 

#define IANPREngine_get_Deinterlace(This,pVal)	\
    ( (This)->lpVtbl -> get_Deinterlace(This,pVal) ) 

#define IANPREngine_put_Deinterlace(This,newVal)	\
    ( (This)->lpVtbl -> put_Deinterlace(This,newVal) ) 

#define IANPREngine_get_PlateColorSchema(This,pVal)	\
    ( (This)->lpVtbl -> get_PlateColorSchema(This,pVal) ) 

#define IANPREngine_put_PlateColorSchema(This,newVal)	\
    ( (This)->lpVtbl -> put_PlateColorSchema(This,newVal) ) 

#define IANPREngine_SetCountryCode(This,countryCode)	\
    ( (This)->lpVtbl -> SetCountryCode(This,countryCode) ) 

#define IANPREngine_get_ContrastLevel(This,pVal)	\
    ( (This)->lpVtbl -> get_ContrastLevel(This,pVal) ) 

#define IANPREngine_put_ContrastLevel(This,newVal)	\
    ( (This)->lpVtbl -> put_ContrastLevel(This,newVal) ) 

#define IANPREngine_Init(This,bVideo)	\
    ( (This)->lpVtbl -> Init(This,bVideo) ) 

#define IANPREngine_get_Plates(This,pVal)	\
    ( (This)->lpVtbl -> get_Plates(This,pVal) ) 

#define IANPREngine_get_DeinterlacedSource(This,pVal)	\
    ( (This)->lpVtbl -> get_DeinterlacedSource(This,pVal) ) 

#define IANPREngine_put_DeinterlacedSource(This,newVal)	\
    ( (This)->lpVtbl -> put_DeinterlacedSource(This,newVal) ) 

#define IANPREngine_get_RotateAngle(This,pVal)	\
    ( (This)->lpVtbl -> get_RotateAngle(This,pVal) ) 

#define IANPREngine_put_RotateAngle(This,newVal)	\
    ( (This)->lpVtbl -> put_RotateAngle(This,newVal) ) 

#define IANPREngine_get_PreciseMode(This,pVal)	\
    ( (This)->lpVtbl -> get_PreciseMode(This,pVal) ) 

#define IANPREngine_put_PreciseMode(This,newVal)	\
    ( (This)->lpVtbl -> put_PreciseMode(This,newVal) ) 

#define IANPREngine_GetLastErrorText(This,errorText)	\
    ( (This)->lpVtbl -> GetLastErrorText(This,errorText) ) 

#define IANPREngine_ReadFromBuffer(This,pBuffer,width,height,stride,bpp,customData)	\
    ( (This)->lpVtbl -> ReadFromBuffer(This,pBuffer,width,height,stride,bpp,customData) ) 

#define IANPREngine_SetMaskFromFile(This,imageFileName)	\
    ( (This)->lpVtbl -> SetMaskFromFile(This,imageFileName) ) 

#define IANPREngine_SetMaskFromBitmap(This,hBitmap)	\
    ( (This)->lpVtbl -> SetMaskFromBitmap(This,hBitmap) ) 

#define IANPREngine_get_HistogramEqualization(This,pVal)	\
    ( (This)->lpVtbl -> get_HistogramEqualization(This,pVal) ) 

#define IANPREngine_put_HistogramEqualization(This,newVal)	\
    ( (This)->lpVtbl -> put_HistogramEqualization(This,newVal) ) 

#define IANPREngine_get_MotionDetectionTriggering(This,pVal)	\
    ( (This)->lpVtbl -> get_MotionDetectionTriggering(This,pVal) ) 

#define IANPREngine_put_MotionDetectionTriggering(This,newVal)	\
    ( (This)->lpVtbl -> put_MotionDetectionTriggering(This,newVal) ) 

#define IANPREngine_get_Fps(This,pVal)	\
    ( (This)->lpVtbl -> get_Fps(This,pVal) ) 

#define IANPREngine_put_Fps(This,newVal)	\
    ( (This)->lpVtbl -> put_Fps(This,newVal) ) 

#define IANPREngine_get_RecognitionTime(This,pVal)	\
    ( (This)->lpVtbl -> get_RecognitionTime(This,pVal) ) 

#define IANPREngine_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#define IANPREngine_get_DeviationAngle(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviationAngle(This,pVal) ) 

#define IANPREngine_put_DeviationAngle(This,newVal)	\
    ( (This)->lpVtbl -> put_DeviationAngle(This,newVal) ) 

#define IANPREngine_get_PlatePresenceTime(This,pVal)	\
    ( (This)->lpVtbl -> get_PlatePresenceTime(This,pVal) ) 

#define IANPREngine_put_PlatePresenceTime(This,newVal)	\
    ( (This)->lpVtbl -> put_PlatePresenceTime(This,newVal) ) 

#define IANPREngine_SetXOption(This,optionName,value)	\
    ( (This)->lpVtbl -> SetXOption(This,optionName,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IANPREngine_INTERFACE_DEFINED__ */


#ifndef __IVideoCapture_INTERFACE_DEFINED__
#define __IVideoCapture_INTERFACE_DEFINED__

/* interface IVideoCapture */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVideoCapture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1D8ED214-92A2-4238-9601-1E976E4727EE")
    IVideoCapture : public IDispatch
    {
    public:
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONG_PTR *pHWND) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VideoSources( 
            /* [in] */ LONG ind,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VideoSourceCount( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurVideoSourceName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurVideoSourceName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurVideoSourceIndex( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurVideoSourceIndex( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartCapture( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopCapture( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowDeviceProperties( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowOutputProperties( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartCaptureFromFile( 
            BSTR fileName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VideoCodecs( 
            /* [in] */ LONG index,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VideoCodecCount( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurVideoCodecName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurVideoCodecName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurVideoCodecIndex( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurVideoCodecIndex( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowVideoCodecProperties( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OutputFileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_OutputFileName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IPCameraURL( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IPCameraURL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IPCameraUser( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IPCameraUser( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IPCameraPassword( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IPCameraPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IPCameraFrameRate( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IPCameraFrameRate( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CameraType( 
            /* [retval][out] */ CameraTypeEnum *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CameraType( 
            /* [in] */ CameraTypeEnum newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVideoCaptureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVideoCapture * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVideoCapture * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVideoCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVideoCapture * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVideoCapture * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVideoCapture * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVideoCapture * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IVideoCapture * This,
            /* [retval][out] */ LONG_PTR *pHWND);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoSources )( 
            IVideoCapture * This,
            /* [in] */ LONG ind,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoSourceCount )( 
            IVideoCapture * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurVideoSourceName )( 
            IVideoCapture * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurVideoSourceName )( 
            IVideoCapture * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurVideoSourceIndex )( 
            IVideoCapture * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurVideoSourceIndex )( 
            IVideoCapture * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartCapture )( 
            IVideoCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopCapture )( 
            IVideoCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowDeviceProperties )( 
            IVideoCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowOutputProperties )( 
            IVideoCapture * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartCaptureFromFile )( 
            IVideoCapture * This,
            BSTR fileName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoCodecs )( 
            IVideoCapture * This,
            /* [in] */ LONG index,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoCodecCount )( 
            IVideoCapture * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurVideoCodecName )( 
            IVideoCapture * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurVideoCodecName )( 
            IVideoCapture * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurVideoCodecIndex )( 
            IVideoCapture * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurVideoCodecIndex )( 
            IVideoCapture * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowVideoCodecProperties )( 
            IVideoCapture * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OutputFileName )( 
            IVideoCapture * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_OutputFileName )( 
            IVideoCapture * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IPCameraURL )( 
            IVideoCapture * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IPCameraURL )( 
            IVideoCapture * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IPCameraUser )( 
            IVideoCapture * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IPCameraUser )( 
            IVideoCapture * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IPCameraPassword )( 
            IVideoCapture * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IPCameraPassword )( 
            IVideoCapture * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IPCameraFrameRate )( 
            IVideoCapture * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IPCameraFrameRate )( 
            IVideoCapture * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CameraType )( 
            IVideoCapture * This,
            /* [retval][out] */ CameraTypeEnum *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CameraType )( 
            IVideoCapture * This,
            /* [in] */ CameraTypeEnum newVal);
        
        END_INTERFACE
    } IVideoCaptureVtbl;

    interface IVideoCapture
    {
        CONST_VTBL struct IVideoCaptureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVideoCapture_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVideoCapture_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVideoCapture_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVideoCapture_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVideoCapture_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVideoCapture_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVideoCapture_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVideoCapture_get_HWND(This,pHWND)	\
    ( (This)->lpVtbl -> get_HWND(This,pHWND) ) 

#define IVideoCapture_get_VideoSources(This,ind,pVal)	\
    ( (This)->lpVtbl -> get_VideoSources(This,ind,pVal) ) 

#define IVideoCapture_get_VideoSourceCount(This,pVal)	\
    ( (This)->lpVtbl -> get_VideoSourceCount(This,pVal) ) 

#define IVideoCapture_get_CurVideoSourceName(This,pVal)	\
    ( (This)->lpVtbl -> get_CurVideoSourceName(This,pVal) ) 

#define IVideoCapture_put_CurVideoSourceName(This,newVal)	\
    ( (This)->lpVtbl -> put_CurVideoSourceName(This,newVal) ) 

#define IVideoCapture_get_CurVideoSourceIndex(This,pVal)	\
    ( (This)->lpVtbl -> get_CurVideoSourceIndex(This,pVal) ) 

#define IVideoCapture_put_CurVideoSourceIndex(This,newVal)	\
    ( (This)->lpVtbl -> put_CurVideoSourceIndex(This,newVal) ) 

#define IVideoCapture_StartCapture(This)	\
    ( (This)->lpVtbl -> StartCapture(This) ) 

#define IVideoCapture_StopCapture(This)	\
    ( (This)->lpVtbl -> StopCapture(This) ) 

#define IVideoCapture_ShowDeviceProperties(This)	\
    ( (This)->lpVtbl -> ShowDeviceProperties(This) ) 

#define IVideoCapture_ShowOutputProperties(This)	\
    ( (This)->lpVtbl -> ShowOutputProperties(This) ) 

#define IVideoCapture_StartCaptureFromFile(This,fileName)	\
    ( (This)->lpVtbl -> StartCaptureFromFile(This,fileName) ) 

#define IVideoCapture_get_VideoCodecs(This,index,pVal)	\
    ( (This)->lpVtbl -> get_VideoCodecs(This,index,pVal) ) 

#define IVideoCapture_get_VideoCodecCount(This,pVal)	\
    ( (This)->lpVtbl -> get_VideoCodecCount(This,pVal) ) 

#define IVideoCapture_get_CurVideoCodecName(This,pVal)	\
    ( (This)->lpVtbl -> get_CurVideoCodecName(This,pVal) ) 

#define IVideoCapture_put_CurVideoCodecName(This,newVal)	\
    ( (This)->lpVtbl -> put_CurVideoCodecName(This,newVal) ) 

#define IVideoCapture_get_CurVideoCodecIndex(This,pVal)	\
    ( (This)->lpVtbl -> get_CurVideoCodecIndex(This,pVal) ) 

#define IVideoCapture_put_CurVideoCodecIndex(This,newVal)	\
    ( (This)->lpVtbl -> put_CurVideoCodecIndex(This,newVal) ) 

#define IVideoCapture_ShowVideoCodecProperties(This)	\
    ( (This)->lpVtbl -> ShowVideoCodecProperties(This) ) 

#define IVideoCapture_get_OutputFileName(This,pVal)	\
    ( (This)->lpVtbl -> get_OutputFileName(This,pVal) ) 

#define IVideoCapture_put_OutputFileName(This,newVal)	\
    ( (This)->lpVtbl -> put_OutputFileName(This,newVal) ) 

#define IVideoCapture_get_IPCameraURL(This,pVal)	\
    ( (This)->lpVtbl -> get_IPCameraURL(This,pVal) ) 

#define IVideoCapture_put_IPCameraURL(This,newVal)	\
    ( (This)->lpVtbl -> put_IPCameraURL(This,newVal) ) 

#define IVideoCapture_get_IPCameraUser(This,pVal)	\
    ( (This)->lpVtbl -> get_IPCameraUser(This,pVal) ) 

#define IVideoCapture_put_IPCameraUser(This,newVal)	\
    ( (This)->lpVtbl -> put_IPCameraUser(This,newVal) ) 

#define IVideoCapture_get_IPCameraPassword(This,pVal)	\
    ( (This)->lpVtbl -> get_IPCameraPassword(This,pVal) ) 

#define IVideoCapture_put_IPCameraPassword(This,newVal)	\
    ( (This)->lpVtbl -> put_IPCameraPassword(This,newVal) ) 

#define IVideoCapture_get_IPCameraFrameRate(This,pVal)	\
    ( (This)->lpVtbl -> get_IPCameraFrameRate(This,pVal) ) 

#define IVideoCapture_put_IPCameraFrameRate(This,newVal)	\
    ( (This)->lpVtbl -> put_IPCameraFrameRate(This,newVal) ) 

#define IVideoCapture_get_CameraType(This,pVal)	\
    ( (This)->lpVtbl -> get_CameraType(This,pVal) ) 

#define IVideoCapture_put_CameraType(This,newVal)	\
    ( (This)->lpVtbl -> put_CameraType(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVideoCapture_INTERFACE_DEFINED__ */



#ifndef __DTKANPRLib_LIBRARY_DEFINED__
#define __DTKANPRLib_LIBRARY_DEFINED__

/* library DTKANPRLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DTKANPRLib;

#ifndef ___IANPREngineEvents_DISPINTERFACE_DEFINED__
#define ___IANPREngineEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IANPREngineEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IANPREngineEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("50AC6D97-A68A-4FD0-AAEF-B407217CC580")
    _IANPREngineEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IANPREngineEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IANPREngineEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IANPREngineEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IANPREngineEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IANPREngineEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IANPREngineEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IANPREngineEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IANPREngineEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IANPREngineEventsVtbl;

    interface _IANPREngineEvents
    {
        CONST_VTBL struct _IANPREngineEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IANPREngineEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IANPREngineEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IANPREngineEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IANPREngineEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IANPREngineEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IANPREngineEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IANPREngineEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IANPREngineEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ANPREngine;

#ifdef __cplusplus

class DECLSPEC_UUID("E5B05BDF-0472-4235-9653-368A2E749FF3")
ANPREngine;
#endif

EXTERN_C const CLSID CLSID_Plate;

#ifdef __cplusplus

class DECLSPEC_UUID("7B4BFF6E-009D-4EE2-9CCE-E7A711C5EE3E")
Plate;
#endif

EXTERN_C const CLSID CLSID_PlateCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("1AF9CB0A-40C5-4C87-947B-A88C1E3EEB06")
PlateCollection;
#endif

#ifndef ___IVideoCaptureEvents_DISPINTERFACE_DEFINED__
#define ___IVideoCaptureEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IVideoCaptureEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IVideoCaptureEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DC826BE0-E835-41DA-BB82-F3ABBAADC5E6")
    _IVideoCaptureEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IVideoCaptureEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IVideoCaptureEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IVideoCaptureEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IVideoCaptureEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IVideoCaptureEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IVideoCaptureEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IVideoCaptureEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IVideoCaptureEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IVideoCaptureEventsVtbl;

    interface _IVideoCaptureEvents
    {
        CONST_VTBL struct _IVideoCaptureEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IVideoCaptureEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IVideoCaptureEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IVideoCaptureEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IVideoCaptureEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IVideoCaptureEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IVideoCaptureEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IVideoCaptureEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IVideoCaptureEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_VideoCapture;

#ifdef __cplusplus

class DECLSPEC_UUID("4A85D6AC-F3A7-4892-807D-F453B0D153B1")
VideoCapture;
#endif

EXTERN_C const CLSID CLSID_LicManager;

#ifdef __cplusplus

class DECLSPEC_UUID("9C01C32C-16D2-4024-8A5F-77452571D2F3")
LicManager;
#endif
#endif /* __DTKANPRLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


