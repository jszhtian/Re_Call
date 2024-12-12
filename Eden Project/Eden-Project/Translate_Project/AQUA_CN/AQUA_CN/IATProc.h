#ifndef _IATProc_
#define _IATProc_

#include <Windows.h>

BOOL IATPatch(HMODULE hmod, LPCSTR szDllName, PROC pfnOrg, PROC pfnNew);
BOOL IATFinder(LPCSTR szDllName, PROC pfnOrg);

#endif
