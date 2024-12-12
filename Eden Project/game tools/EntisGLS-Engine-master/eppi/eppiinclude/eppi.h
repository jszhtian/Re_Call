
/*****************************************************************************
                      e-Player plug-in declaration
 -----------------------------------------------------------------------------
         Copyright (c) 2001-2002 Leshade Entis. All rights reserved.
 *****************************************************************************/


#if	!defined(__EPPI_DEF__)
#define	__EPPI_DEF__


//////////////////////////////////////////////////////////////////////////////
// 基本定義
//////////////////////////////////////////////////////////////////////////////

typedef	void *			EPPI_INSTANCE ;
typedef	const char *	EPPIError ;

struct	EPPI_SIZE
{
	long int	w ;
	long int	h ;
} ;


//////////////////////////////////////////////////////////////////////////////
// グローバル関数
//////////////////////////////////////////////////////////////////////////////

typedef	void * (__stdcall *EPPI_GLOBAL_FUNC)( const char * pszFunction ) ;

#define	EPPI_GLOBAL_MAINWND		"GetMainWindow"
#define	EPPI_MEMORY_MANAGER		"MemoryManager"
#define	EPPI_GLOBAL_VARIABLE	"ScriptVariable"


//
// メモリ管理
//

struct	EPPI_MEMORY_INTERFACE
{
	void * (__stdcall *pfnAllocate)( DWORD dwBytes ) ;
	void * (__stdcall *pfnReallocate)( void * ptrMemBlock, DWORD dwNewBytes ) ;
	void (__stdcall *pfnFree)( void * ptrMemBlock ) ;
	DWORD (__stdcall *pfnGetSize)( void * ptrMemBlock ) ;
	int (__stdcall *pfnIsValid)( void * ptrMemBlock ) ;
} ;


//
// スクリプト・インターフェース
//

class	EASVariableObject ;
typedef	EASVariableObject *	HVARIABLE ;
enum	ASVariableType
{
	asvtObject,
	asvtTemplate,
	asvtReference,
	asvtArray,
	asvtInteger,
	asvtReal,
	asvtString
} ;

struct	EPPI_VARIABLE_INTERFACE
{
	HVARIABLE (__stdcall *pfnCreateArray)( void ) ;
	HVARIABLE (__stdcall *pfnCreateInteger)( void ) ;
	HVARIABLE (__stdcall *pfnCreateReal)( void ) ;
	HVARIABLE (__stdcall *pfnCreateString)( void ) ;
	HVARIABLE (__stdcall *pfnCreateObject)
		( const wchar_t * pwszType,
			const struct EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;
	HVARIABLE (__stdcall *pfnDuplicate)( HVARIABLE hVarOriginal ) ;

	void (__stdcall *pfnDestroy)( HVARIABLE instance ) ;
	HVARIABLE (__stdcall *pfnGetReference)( HVARIABLE instance ) ;
	ASVariableType (__stdcall *pfnGetType)( HVARIABLE instance ) ;
	const wchar_t * (__stdcall *pfnGetTypeName)( HVARIABLE instance ) ;
	void * (__stdcall *pfnGetInstance)( HVARIABLE instance ) ;
	const struct EPPI_CLASS_INTERFACE *
		(__stdcall *pfnGetInterface)( HVARIABLE instance ) ;

	void (__stdcall *pfnSetReference)( HVARIABLE instance, HVARIABLE hVar ) ;

	long int (__stdcall *pfnGetInteger)( HVARIABLE instance ) ;
	void (__stdcall *pfnSetInteger)( HVARIABLE instance, long int nValue ) ;

	double (__stdcall *pfnGetReal)( HVARIABLE instance ) ;
	void (__stdcall *pfnSetReal)( HVARIABLE instance, double rValue ) ;

	const wchar_t * (__stdcall *pfnGetString)( HVARIABLE instance ) ;
	void (__stdcall *pfnSetString)
		( HVARIABLE instance, const wchar_t * pwszValue, int nLength ) ;

	long int (__stdcall *pfnGetLength)( HVARIABLE instance ) ;
	void (__stdcall *pfnSetLength)( HVARIABLE instance, unsigned int nLength ) ;
	HVARIABLE (__stdcall *pfnGetElementAt)
		( HVARIABLE instance, unsigned int index ) ;
	void (__stdcall *pfnSetElementAt)
		( HVARIABLE instance, unsigned int index, HVARIABLE hVarNew ) ;
	HVARIABLE (__stdcall *pfnDetachElementAt)
		( HVARIABLE instance, unsigned int index ) ;
	void (__stdcall *pfnInsertElementAt)
		( HVARIABLE instance, unsigned int index, HVARIABLE hVarNew ) ;
} ;



//////////////////////////////////////////////////////////////////////////////
// 共通インターフェース
//////////////////////////////////////////////////////////////////////////////

struct	EPPI_COMMON_INTERFACE
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void * (__stdcall *pfnDestroy)( void * instance ) ;
} ;

#define	EPPI_CAP_ONLY_REAL_FILE		0x01
#define	EPPI_CAP_CAN_WRITE_INFO		0x02


//
// オプションダイアログ
//

#define	EPPI_OPTION_AS_WRITE	0x01

#define	EPPI_OPTION_OK			0x00
#define	EPPI_OPTION_CANCEL		0x01


//////////////////////////////////////////////////////////////////////////////
// プラグイン・ディスクリプタ
//////////////////////////////////////////////////////////////////////////////

typedef	EPPI_INSTANCE
	 (__stdcall *EPPI_PFUNC_INITILIZE)( EPPI_GLOBAL_FUNC global ) ;
typedef	void (__stdcall *EPPI_PFUNC_DESTROY)( EPPI_INSTANCE instance ) ;
typedef	void (__stdcall *EPPI_PFUNC_INFORMATION)
				( EPPI_INSTANCE instance, void * hParentWnd ) ;

struct	EPPI_DESCRIPTOR
{
	DWORD					dwVersion ;
	DWORD					dwPriority ;
	const char *			pszType ;
	EPPI_PFUNC_INITILIZE 	pfnInitialize ;
	EPPI_PFUNC_DESTROY		pfnDestroy ;
	EPPI_PFUNC_INFORMATION	pfnInformation ;
	EPPI_COMMON_INTERFACE *	ptrInterface ;
} ;

#define	EPPI_VERSION		1

#define	EPPI_LOWEST_PRIORITY	0
#define	EPPI_LOW_PRIORITY		1
#define	EPPI_NORMAL_PRIORITY	2
#define	EPPI_HIGH_PRIORITY		3
#define	EPPI_HIGHEST_PRIORITY	4

#define	EPPI_IMAGE_IMPORTER		"ImageImporter"
#define	EPPI_IMAGE_EXPORTER		"ImageExporter"
#define	EPPI_SOUND_IMPORTER		"SoundImporter"
#define	EPPI_SOUND_EXPORTER		"SoundExporter"
#define	EPPI_MOVIE_IMPORTER		"MovieImporter"
#define	EPPI_MOVIE_EXPORTER		"MovieExporter"
#define	EPPI_SCRIPT_IMPORTER	"ScriptImporter"


//////////////////////////////////////////////////////////////////////////////
// ファイルインターフェース
//////////////////////////////////////////////////////////////////////////////

struct	EPPI_FILE_INTERFACE
{
	void *	instance ;
	const char * (__stdcall *pfnGetFilePath)( void * instance ) ;
	int (__stdcall *pfnGetAccessFlag)( void * instance ) ;
	DWORD (__stdcall *pfnRead)
		( void * instance, void * ptrBuf, DWORD dwBytes ) ;
	DWORD (__stdcall *pfnWrite)
		( void * instance, const void * ptrBuf, DWORD dwBytes ) ;
	DWORD (__stdcall *pfnGetPosition)( void * instance ) ;
	DWORD (__stdcall *pfnGetLength)( void * instance ) ;
	DWORD (__stdcall *pfnSeek)( void * instance, DWORD dwPosition ) ;

	const char * GetFilePath( void ) const
		{
			return	(*pfnGetFilePath)( instance ) ;
		}
	int GetAccessFlag( void )  const
		{
			return	(*pfnGetAccessFlag)( instance ) ;
		}
	DWORD Read( void * ptrBuf, DWORD dwBytes ) const
		{
			return	(*pfnRead)( instance, ptrBuf, dwBytes ) ;
		}
	DWORD Write( const void * ptrBuf, DWORD dwBytes ) const
		{
			return	(*pfnWrite)( instance, ptrBuf, dwBytes ) ;
		}
	DWORD GetPosition( void ) const
		{
			return	(*pfnGetPosition)( instance ) ;
		}
	DWORD GetLength( void ) const
		{
			return	(*pfnGetLength)( instance ) ;
		}
	DWORD Seek( DWORD dwPosition ) const
		{
			return	(*pfnSeek)( instance, dwPosition ) ;
		}
} ;

#define	EPPI_FILE_READ_MODE		0x01
#define	EPPI_FILE_WRITE_MODE	0x02


//////////////////////////////////////////////////////////////////////////////
// 画像　インポーター／エクスポーター
//////////////////////////////////////////////////////////////////////////////

struct	EPPI_IMAGE_INFO
{
	DWORD			fdwFormat ;
	DWORD			dwWidth ;
	DWORD			dwHeight ;
	DWORD			dwBitsPerPixel ;
	DWORD			dwClipperPalette ;
	const char *	pszCopyright ;
	const char *	pszDescription ;
} ;

struct	EPPI_PALETTE
{
	BYTE	blue ;
	BYTE	green ;
	BYTE	red ;
	BYTE	reserved ;
} ;

struct	EPPI_IMAGE_BUFFER
{
	DWORD			fdwFormat ;
	DWORD			dwWidth ;
	DWORD			dwHeight ;
	DWORD			dwBitsPerPixel ;
	long int		nBytesPerLine ;
	BYTE *			ptrImageBuf ;
	DWORD			dwPaletteCount ;
	EPPI_PALETTE *	ptrPaletteTable ;
} ;

#define	EPPI_INDEXED_IMAGE	0
#define	EPPI_GRAY_IMAGE		1
#define	EPPI_RGB_IMAGE		2
#define	EPPI_RGBA_IMAGE		3
#define	EPPI_TYPE_MASK		0xFFFF
#define	EPPI_WITH_CLIPPER	0x10000

struct	EPPI_ADVANCE_STATE
{
	void *	instance ;
	int (__stdcall *pfnUpdate)
		( void * instance, DWORD dwCurrent, DWORD dwTotal ) ;

	int Update( DWORD dwCurrent, DWORD dwTotal ) const
		{
			return	(*pfnUpdate)( instance, dwCurrent, dwTotal ) ;
		}
} ;

struct	EPPI_IMAGE_IMPORT
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void (__stdcall *pfnDestroy)( void * instance ) ;
	EPPIError (__stdcall *pfnOpen)
		( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
	EPPIError (__stdcall *pfnClose)( void * instance ) ;
	const EPPI_IMAGE_INFO *
		(__stdcall *pfnGetInformation)( void * instance ) ;
	EPPIError (__stdcall *pfnSetInformation)
		( void * instance, const EPPI_IMAGE_INFO * pInformation ) ;
	const EPPI_IMAGE_INFO *
		(__stdcall *pfnHasPreviewImage)( void * instance ) ;
	EPPIError (__stdcall *pfnGetPreview)
		( void * instance, EPPI_IMAGE_BUFFER * pImageBuf ) ;
	EPPIError (__stdcall *pfnGetImage)
		( void * instance,
			EPPI_IMAGE_BUFFER * pImageBuf,
			const EPPI_ADVANCE_STATE * pAdvState ) ;
} ;

struct	EPPI_IMAGE_EXPORT
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void (__stdcall *pfnDestroy)( void * instance ) ;
	int (__stdcall *pfnIsSupported)
		( void * instance, const EPPI_IMAGE_INFO * pInformation ) ;
	int (__stdcall *pfnOption)
		( void * instance, void * hParentWnd, DWORD dwFlags ) ;
	EPPIError (__stdcall *pfnOpen)
		( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
	EPPIError (__stdcall *pfnClose)( void * instance ) ;
	EPPIError (__stdcall *pfnSetInformation)
		( void * instance,
			const EPPI_IMAGE_INFO * pInformation,
			const EPPI_IMAGE_INFO * pPreviewInfo ) ;
	EPPIError (__stdcall *pfnSetPreview)
		( void * instance, const EPPI_IMAGE_BUFFER * pPreviewBuf ) ;
	EPPIError (__stdcall *pfnSetImage)
		( void * instance,
			const EPPI_IMAGE_BUFFER * pImageBuf,
			const EPPI_ADVANCE_STATE * pAdvState ) ;
} ;


//////////////////////////////////////////////////////////////////////////////
// 音声　インポーター／エクスポーター
//////////////////////////////////////////////////////////////////////////////

struct	EPPI_SOUND_INFO
{
	DWORD			dwTotalSamples ;
	DWORD			dwFrequency ;
	DWORD			dwBitsPerSample ;
	DWORD			dwChannels ;
	const char *	pszTitle ;
	const char *	pszVocalPlayer ;
	const char *	pszComposer ;
	const char *	pszArranger ;
	const char *	pszSource ;
	const char *	pszTrack ;
	const char *	pszReleaseDate ;
	const char *	pszGenre ;
	const char *	pszRewindPoint ;
	const char *	pszCopyright ;
	const char *	pszDescription ;
	const char *	pszWords ;
} ;

struct	EPPI_SOUND_IMPORT
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void (__stdcall *pfnDestroy)( void * instance ) ;
	EPPIError (__stdcall *pfnOpen)
		( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
	EPPIError (__stdcall *pfnClose)( void * instance ) ;
	const EPPI_SOUND_INFO *
		(__stdcall *pfnGetInformation)( void * instance ) ;
	EPPIError (__stdcall *pfnSetInformation)
		( void * instance, const EPPI_SOUND_INFO * pInformation ) ;
	EPPIError (__stdcall *pfnSeek)( void * instance, DWORD dwPosition ) ;
	int (__stdcall *pfnIsEndOfStream)( void * instance ) ;
	EPPIError (__stdcall *pfnGetNextData)
		( void * instance, void ** ppWaveBuf, DWORD * pdwBytes ) ;
	EPPIError (__stdcall *pfnDeleteData)
		( void * instance, void * pWaveBuf, DWORD dwBytes ) ;
} ;

struct	EPPI_SOUND_EXPORT
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void (__stdcall *pfnDestroy)( void * instance ) ;
	int (__stdcall *pfnIsSupported)
		( void * instance, const EPPI_SOUND_INFO * pInformation ) ;
	int (__stdcall *pfnOption)
		( void * instance, void * hParentWnd, DWORD dwFlags ) ;
	EPPIError (__stdcall *pfnOpen)
		( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
	EPPIError (__stdcall *pfnClose)( void * instance ) ;
	EPPIError (__stdcall *pfnSetInformation)
		( void * instance, const EPPI_SOUND_INFO * pInformation ) ;
	DWORD (__stdcall *pfnEstimateNextSize)( void * instance ) ;
	EPPIError (__stdcall *pfnWriteNextData)
		( void * instance, const void * pWaveBuf, DWORD dwSamples ) ;
} ;


//////////////////////////////////////////////////////////////////////////////
// 動画像　インポーター／エクスポーター
//////////////////////////////////////////////////////////////////////////////

struct	EPPI_MOVIE_INFO
{
	DWORD					fdwFlags ;
	DWORD					dwTotalFrames ;
	double					rFramePerSec ;
	const EPPI_IMAGE_INFO *	pImageInfo ;
	const EPPI_SOUND_INFO *	pSoundInfo ;
} ;

#define	EPPI_CONTAIN_IMAGE	0x00000001
#define	EPPI_CONTAIN_SOUND	0x00000002

struct	EPPI_MOVIE_IMPORT
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void (__stdcall *pfnDestroy)( void * instance ) ;
	EPPIError (__stdcall *pfnOpen)
		( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
	EPPIError (__stdcall *pfnClose)( void * instance ) ;
	const EPPI_MOVIE_INFO *
		(__stdcall *pfnGetInformation)( void * instance ) ;
	EPPIError (__stdcall *pfnSetInformation)
		( void * instance, const EPPI_MOVIE_INFO * pInformation ) ;
	EPPIError (__stdcall *pfnSeek)( void * instance, DWORD dwPosition ) ;
	unsigned int (__stdcall *pfnGetBestSkipFrames)
		( void * instance, unsigned int nCurrentTime ) ;
	EPPIError (__stdcall *pfnSeekNextFrame)
		( void * instance, unsigned int nSkipFrames ) ;
	unsigned int (__stdcall *pfnGetCurrentFrame)( void * instance ) ;
	EPPIError (__stdcall *pfnDrawImage)
		( void * instance, void * hDstDC,
			int xPos, int yPos, const EPPI_SIZE * pDstSize ) ;
	EPPIError (__stdcall *pfnGetImageData)
		( void * instance, EPPI_IMAGE_BUFFER * pImageBuf ) ;
	EPPIError (__stdcall *pfnBeginStreaming)( void * instance ) ;
	EPPIError (__stdcall *pfnEndStreaming)( void * instance ) ;
	EPPIError (__stdcall *pfnGetNextWaveStream)
		( void * instance, void ** ppWaveBuf, DWORD * pdwBytes ) ;
	EPPIError (__stdcall *pfnDeleteWaveData)
		( void * instance, void * pWaveBuf, DWORD dwBytes ) ;
} ;

struct	EPPI_MOVIE_EXPORT
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void (__stdcall *pfnDestroy)( void * instance ) ;
	int (__stdcall *pfnIsSupported)
		( void * instance, const EPPI_MOVIE_INFO * pMovieInfo ) ;
	int (__stdcall *pfnOption)
		( void * instance, void * hParentWnd, DWORD dwFlags ) ;
	EPPIError (__stdcall *pfnOpen)
		( void * instance, const EPPI_FILE_INTERFACE * pfile ) ;
	EPPIError (__stdcall *pfnClose)( void * instance ) ;
	EPPIError (__stdcall *pfnSetInformation)
		( void * instance, const EPPI_MOVIE_INFO * pMovieInfo ) ;
	DWORD (__stdcall *pfnEstimateNextWaveSize)( void * instance ) ;
	EPPIError (__stdcall *pfnWriteNextData)
		( void * instance,
			const EPPI_IMAGE_BUFFER * pImageBuf,
			const void * pWaveBuf, DWORD dwSamples ) ;
} ;


//////////////////////////////////////////////////////////////////////////////
// スクリプト・インポーター
//////////////////////////////////////////////////////////////////////////////

struct	EPPI_ACTIVE_SCRIPT_INTERFACE
{
	void *	instance ;
	const char * (__stdcall *pfnGetScriptPath)( void * instance ) ;
	void (__stdcall *pfnLock)( void * instance ) ;
	void (__stdcall *pfnUnlock)( void * instance ) ;
	EPPIError (__stdcall *pfnExpression)
		( void * instance, HVARIABLE * pVarResult, const wchar_t * pwszExpr ) ;
	EPPIError (__stdcall *pfnEnterFunction)
		( void * instance, const wchar_t * pwszFuncName, HVARIABLE hArgument ) ;
	EPPIError (__stdcall *pfnExecuteUntilLeave)
		( void * instance, HVARIABLE * pVarResult ) ;
	HVARIABLE (__stdcall *pfnSearchVariable)
		( void * instance, const wchar_t * pwszVarName ) ;

	const char * GetScriptPath( void ) const
		{
			return	(*pfnGetScriptPath)( instance ) ;
		}
	void Lock( void ) const
		{
			(*pfnLock)( instance ) ;
		}
	void Unlock( void ) const
		{
			(*pfnUnlock)( instance ) ;
		}
	EPPIError Expression( HVARIABLE * pVarResult, const wchar_t * pwszExpr ) const
		{
			return	(*pfnExpression)( instance, pVarResult, pwszExpr ) ;
		}
	EPPIError EnterFunction
		( const wchar_t * pwszFuncName, HVARIABLE hArgument ) const
		{
			return	(*pfnEnterFunction)( instance, pwszFuncName, hArgument ) ;
		}
	EPPIError ExecuteUntilLeave( HVARIABLE * pVarResult ) const
		{
			return	(*pfnExecuteUntilLeave)( instance, pVarResult ) ;
		}
	HVARIABLE SearchVariable( const wchar_t * pwszVarName ) const
		{
			return	(*pfnSearchVariable)( instance, pwszVarName ) ;
		}
} ;

typedef	EPPIError
	(__stdcall *EPPI_PFUNC_FUNCTION)
		( void * instance, HVARIABLE * hVarResult, HVARIABLE hArgument,
			const EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;

enum	ASOperatorType
{
	asopAdd,	asopSub,	asopMul,	asopDiv,	asopMod,
	asopAnd,	asopOr,		asopXor,
	asopMax
} ;

enum	ASUnaryOperatorType
{
	asuopPlus, asuopNegate, asuopBitNot, asuopLogicalNot,
	asuopMax,
} ;

enum	ASCompareType
{
	ascpEqual,		ascpNotEqual,
	ascpLessThan,	ascpGreaterThan,
	ascpLessEqual,	ascpGreaterEqual,
	ascpMax
} ;

struct	EPPI_CLASS_INTERFACE
{
	EPPIError (__stdcall *pfnCreate)
		( void ** pNewInstance, HVARIABLE hArgument,
			const EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;
	void * (__stdcall *pfnDestroy)( void * instance ) ;
	const wchar_t * (__stdcall *pfnMakeString)( void * instance ) ;
	const wchar_t * (__stdcall *pfnGetTypeName)( void * instance ) ;
	EPPIError (__stdcall *pfnDuplicate)
		( void * instance, void ** pNewInstance, 
			const EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;
	EPPIError (__stdcall *pfnMove)
		( void * instance, HVARIABLE hVar,
			const EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;
	EPPIError (__stdcall *pfnUnaryOperate)
		( void * instance, ASUnaryOperatorType asuopType,
			const EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;
	EPPIError (__stdcall *pfnOperate)
		( void * instance, ASOperatorType asopType, HVARIABLE hVar,
				const EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;
	EPPIError (__stdcall *pfnCompare)
		( void * instance, int * pResult,
			ASCompareType ascpType, HVARIABLE hVar,
			const EPPI_ACTIVE_SCRIPT_INTERFACE * pASInterface ) ;
	HVARIABLE (__stdcall *pfnIsVariable)
		( void * instance, const wchar_t * pwszSymbol ) ;
	EPPI_PFUNC_FUNCTION
		(__stdcall *pfnIsFunction)
			( void * instance, const wchar_t * pwszSymbol ) ;
} ;

struct	EPPI_SCRIPT_IMPORT
{
	const char *	pszPluginName ;
	const char *	pszFilterName ;
	const char *	pszExtensions ;
	DWORD			fdwCapacity ;
	DWORD			dwReserved ;
	void * (__stdcall *pfnCreate)( EPPI_INSTANCE instance ) ;
	void * (__stdcall *pfnDestroy)( void * instance ) ;
	const EPPI_CLASS_INTERFACE * (__stdcall *pfnIsType)
		( void * instance, const wchar_t * pwszSymbol ) ;
	HVARIABLE (__stdcall *pfnIsConstant)
		( void * instance, const wchar_t * pwszSymbol ) ;
	EPPI_PFUNC_FUNCTION
		(__stdcall *pfnIsFunction)
			( void * instance, const wchar_t * pwszSymbol ) ;
} ;


#endif
