#include "DebugInfo.h"

VOID WINAPI OutputString(const CHAR* lpString)
{
	HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD nRet = 0;
	WriteConsoleA(hOutputHandle, lpString, lstrlenA(lpString), &nRet, NULL);
	WriteConsoleA(hOutputHandle, "\n", 1, &nRet, NULL);
}


VOID WINAPI OutputStringW(const WCHAR* lpString)
{
	HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD nRet = 0;
	WriteConsoleW(hOutputHandle, lpString, lstrlenW(lpString), &nRet, NULL);
	WriteConsoleW(hOutputHandle, L"\n", 1, &nRet, NULL);
}
