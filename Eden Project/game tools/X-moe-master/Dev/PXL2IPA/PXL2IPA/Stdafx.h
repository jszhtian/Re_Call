#pragma once

#define _WIN32_WINNT	0x0400
#define WINVER			0x0400

// Unicode 
#ifndef _UNICODE
#define _UNICODE
#endif

#ifdef _UNICODE
#ifndef UNICODE 
#define UNICODE
#pragma message("UNICODE macro is defined in the ConvertToIPA.h header.")
#endif	
#endif

#pragma warning(push, 4)
#include <windows.h>

#pragma warning(pop)
#pragma warning(push, 3)
#include <assert.h>
#include <tchar.h>
#include <process.h>

#if defined(_DEBUG) && !defined(NDEBUG)
#ifndef ASSERT
#define ASSERT(x)	assert(x)
#endif
#endif

#if !defined(_DEBUG) && defined(NDEBUG)
#ifndef ASSERT
#define ASSERT(x)	NULL
#endif
#endif

#include <string>
using namespace std;

#include <direct.h>
#include <io.h>
