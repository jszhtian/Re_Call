#ifndef _LCScriptEngine_
#define _LCScriptEngine_

#include <Windows.h>
#include <string>
#include <tchar.h>
#include <Shlwapi.h>

#pragma comment(lib,"Shlwapi.lib")

using std::string;
using std::wstring;

typedef DWORD u32;
typedef char s8;
typedef __int32 s32;


#pragma pack (1)
typedef struct
{
	u32 index_entries;
} lst_header_t;

typedef struct
{
	u32 offset;
	u32 length;
	s8 name[64];
	s32 type;
} lst_entry_t;

typedef struct
{
	u32 instructs;
	u32 text_length;
} snx_header_t;
#pragma pack ()

static const TCHAR *suffixW[6] =
{
	_T(".lst"),
	_T(".SNX"),
	_T(".BMP"),
	_T(".PNG"),
	_T(".WAV"),
	_T(".OGG")
};

static const char *suffixA[6] =
{
	(".lst"),
	(".SNX"),
	(".BMP"),
	(".PNG"),
	(".WAV"),
	(".OGG")
};

extern "C" _declspec (dllexport) void ShowInfo();

extern "C" _declspec (dllexport) BOOL Match(TCHAR *pszStr, TCHAR *pFile);

extern "C" _declspec (dllexport) BOOL Engine(TCHAR *pszStr);

extern "C" _declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output = NULL, const TCHAR *args = NULL);

#endif
