#pragma once

#if !defined(__cplusplus)
#error "This Project Only Support C++ Compiler!!"
#endif

#define ANZ_LIL_ENDIAN  1234
#define ANZ_BIG_ENDIAN  4321

#ifndef ANZ_BYTEORDER
#ifdef __linux__
#include <endian.h>
#define ANZ_BYTEORDER  __BYTE_ORDER
#else /* __linux__ */
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define ANZ_BYTEORDER   ANZ_BIG_ENDIAN
#else
#define ANZ_BYTEORDER   ANZ_LIL_ENDIAN
#endif
#endif
#endif


#if defined(_AIX)
#undef __AIX__
#define __AIX__     1
#endif
#if defined(__HAIKU__)
#undef __HAIKU__
#define __HAIKU__   1
#endif
#if defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
#undef __BSDI__
#define __BSDI__    1
#endif
#if defined(_arch_dreamcast)
#undef __DREAMCAST__
#define __DREAMCAST__   1
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
#undef __FREEBSD__
#define __FREEBSD__ 1
#endif
#if defined(hpux) || defined(__hpux) || defined(__hpux__)
#undef __HPUX__
#define __HPUX__    1
#endif
#if defined(sgi) || defined(__sgi) || defined(__sgi__) || defined(_SGI_SOURCE)
#undef __IRIX__
#define __IRIX__    1
#endif
#if (defined(linux) || defined(__linux) || defined(__linux__))
#undef __LINUX__
#define __LINUX__   1
#endif
#if defined(ANDROID) || defined(__ANDROID__)
#undef __ANDROID__
#undef __LINUX__
#define __ANDROID__ 1
#endif

#if defined(__APPLE__)
#include "AvailabilityMacros.h"
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#undef __IPHONEOS__
#define __IPHONEOS__ 1
#undef __MACOSX__
#else
#undef __MACOSX__
#define __MACOSX__  1
#if MAC_OS_X_VERSION_MIN_REQUIRED < 1050
# error SDL for Mac OS X only supports deploying on 10.5 and above.
#endif 
#endif
#endif 

#if defined(__NetBSD__)
#undef __NETBSD__
#define __NETBSD__  1
#endif
#if defined(__OpenBSD__)
#undef __OPENBSD__
#define __OPENBSD__ 1
#endif
#if defined(__OS2__)
#undef __OS2__
#define __OS2__     1
#endif
#if defined(osf) || defined(__osf) || defined(__osf__) || defined(_OSF_SOURCE)
#undef __OSF__
#define __OSF__     1
#endif
#if defined(__QNXNTO__)
#undef __QNXNTO__
#define __QNXNTO__  1
#endif
#if defined(riscos) || defined(__riscos) || defined(__riscos__)
#undef __RISCOS__
#define __RISCOS__  1
#endif
#if defined(__sun) && defined(__SVR4)
#undef __SOLARIS__
#define __SOLARIS__ 1
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#if (defined(_MSC_VER) && (_MSC_VER >= 1700) && !_USING_V110_SDK71_)
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#undef __WINDOWS__
#define __WINDOWS__   1
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#undef __WINRT__
#define __WINRT__ 1
#endif
#else
#undef __WINDOWS__
#define __WINDOWS__   1
#endif
#endif

#if defined(__WINDOWS__)
#undef __WIN32__
#define __WIN32__ 1
#endif
#if defined(__PSP__)
#undef __PSP__
#define __PSP__ 1
#endif


#if defined(__native_client__)
#undef __LINUX__
#undef __NACL__
#define __NACL__ 1
#endif
#if defined(__pnacl__)
#undef __LINUX__
#undef __PNACL__
#define __PNACL__ 1
#endif

typedef int          AnzInt;
typedef unsigned int AnzUInt;
typedef long         AnzLong;
typedef long         AnzResult;

typedef __int8       AnzChar;
typedef __int8       AnzInt8;
typedef __int16      AnzShort;
typedef __int16      AnzInt16;
typedef __int32      AnzInt32;
typedef __int64      AnzInt64;
typedef __int64      AnzLongLong;

typedef size_t       AnzSizeT;

typedef unsigned __int8  AnzUChar;
typedef unsigned __int8  AnzUInt8;
typedef unsigned __int16 AnzUShort;
typedef unsigned __int16 AnzWord;
typedef unsigned __int16 AnzUInt16;
typedef unsigned __int32 AnzUInt32;
typedef unsigned __int32 AnzULong32;
typedef unsigned __int32 AnzDWord;
typedef unsigned __int64 AnzUInt64;
typedef unsigned __int64 AnzULongLong;
typedef unsigned __int64 AnzQWord;

typedef wchar_t  AnzWChar;
typedef char     AnzNChar;
typedef void*    AnzPtr;
typedef wchar_t* AnzWStr;
typedef char*    AnzAStr;
typedef wchar_t* AnzUTF16Str;
typedef char*    AnzUTF8Str;
typedef AnzInt   AnzBool;
typedef void     AnzVoid;


#define AnzTrue  1
#define AnzFalse 0

#ifndef NULL
#define NULL 0
#endif

#if defined(__GNUC__) && defined(__i386__) && \
   !(__GNUC__ == 2 && __GNUC_MINOR__ == 95)
inline AnzUInt16
AnzSwap16(AnzUInt16 x)
{
	__asm__("xchgb %b0,%h0": "=q"(x) : "0"(x));
	return x;
}

#elif defined(__GNUC__) && defined(__x86_64__)
inline AnzUInt16
AnzSwap16(AnzUInt16 x)
{
	__asm__("xchgb %b0,%h0": "=Q"(x) : "0"(x));
	return x;
}

#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
inline AnzUInt16
AnzSwap16(AnzUInt16 x)
{
	int result;

	__asm__("rlwimi %0,%2,8,16,23": "=&r"(result) : "0"(x >> 8), "r"(x));
	return (AnzUInt16)result;
}

#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
inline AnzUInt16
AnzSwap16(Uint16 x)
{
	__asm__("rorw #8,%0": "=d"(x) : "0"(x) : "cc");
	return x;
}
#else
inline AnzUInt16
AnzSwap16(AnzUInt16 x)
{
	return static_cast<AnzUInt16>((x << 8) | (x >> 8));
}
#endif

#if defined(__GNUC__) && defined(__i386__)
inline AnzUInt32
AnzSwap32(Uint32 x)
{
	__asm__("bswap %0": "=r"(x) : "0"(x));
	return x;
}

#elif defined(__GNUC__) && defined(__x86_64__)
inline AnzUInt32
AnzSwap32(Uint32 x)
{
	__asm__("bswapl %0": "=r"(x) : "0"(x));
	return x;
}

#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
inline AnzUInt32
AnzSwap32(AnzUInt32 x)
{
	AnzUInt32 result;

	__asm__("rlwimi %0,%2,24,16,23": "=&r"(result) : "0"(x >> 24), "r"(x));
	__asm__("rlwimi %0,%2,8,8,15": "=&r"(result) : "0"(result), "r"(x));
	__asm__("rlwimi %0,%2,24,0,7": "=&r"(result) : "0"(result), "r"(x));
	return result;
}

#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
inline AnzUInt32
AnzSwap32(AnzUInt32 x)
{
	__asm__("rorw #8,%0\n\tswap %0\n\trorw #8,%0": "=d"(x) : "0"(x) : "cc");
	return x;
}

#else
inline AnzUInt32
AnzSwap32(AnzUInt32 x)
{
	return static_cast<AnzUInt32>((x << 24) | ((x << 8) & 0x00FF0000) |
		((x >> 8) & 0x0000FF00) | (x >> 24));
}
#endif

#if defined(__GNUC__) && defined(__i386__)
inline AnzUInt64
AnzSwap64(AnzUInt64 x)
{
	union
	{
		struct
		{
			AnzUInt32 a, b;
		} s;
		AnzUInt64 u;
	} v;
	v.u = x;
	__asm__("bswapl %0 ; bswapl %1 ; xchgl %0,%1": "=r"(v.s.a), "=r"(v.s.b) : "0"(v.s.a),
		"1"(v.s.
		b));
	return v.u;
}

#elif defined(__GNUC__) && defined(__x86_64__)
inline AnzUInt64
AnzSwap64(AnzUInt64 x)
{
	__asm__("bswapq %0": "=r"(x) : "0"(x));
	return x;
}

#else
inline AnzUInt64
AnzSwap64(AnzUInt64 x)
{
	AnzUInt32 hi, lo;

	lo = static_cast<AnzUInt32>(x & 0xFFFFFFFF);
	x >>= 32;
	hi = static_cast<AnzUInt32>(x & 0xFFFFFFFF);
	x = AnzSwap32(lo);
	x <<= 32;
	x |= AnzSwap32(hi);
	return (x);
}
#endif


inline float
AnzSwapFloat(float x)
{
	union
	{
		float f;
		AnzUInt32 ui32;
	} swapper;
	swapper.f = x;
	swapper.ui32 = AnzSwap32(swapper.ui32);
	return swapper.f;
}


#if ANZ_BYTEORDER == ANZ_LIL_ENDIAN
#define AnzSwapLE16(X) (X)
#define AnzSwapLE32(X) (X)
#define AnzSwapLE64(X) (X)
#define AnzSwapFloatLE(X)  (X)
#define AnzSwapBE16(X) AnzSwap16(X)
#define AnzSwapBE32(X) AnzSwap32(X)
#define AnzSwapBE64(X) AnzSwap64(X)
#define AnzSwapFloatBE(X)  AnzSwapFloat(X)
#else
#define AnzSwapLE16(X) AnzSwap16(X)
#define AnzSwapLE32(X) AnzSwap32(X)
#define AnzSwapLE64(X) AnzSwap64(X)
#define AnzSwapFloatLE(X)  AnzSwapFloat(X)
#define AnzSwapBE16(X) (X)
#define AnzSwapBE32(X) (X)
#define AnzSwapBE64(X) (X)
#define AnzSwapFloatBE(X)  (X)
#endif

#define CCALL __cdecl
#define SCALL __stdcall
#define FCALL __stdcall
#define AnzExpAPI __cdecl
#define AnzLuaAPI __cdecl

#define AnzAPI SCALL
#define AnzResultCall(FuncName) AnzResult AnzAPI FuncName

#if !defined(WIN32) && !defined(WIN64)
#define AnzExport __attribute__(dllexport)
#else
#define AnzExport __declspec(dllexport)
#endif

/*Error Code*/
#define AnzFailed(x) x & 0xFFFFFFFFUL

#define ANZ_OK           0
#define ANZ_NOTIMPL      1
#define ANZ_ERRORPARAM   2
#define ANZ_NOMEMORY     3
#define ANZ_UNK          4
#define ANZ_NOTINRANGE   5

