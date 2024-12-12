#include "stdafx.h"
#include <Windows.h>
#include "detours.h"
#include "tp_stub.h"
#include "MiniNtdll.h"
#include "PluginHook.h"
#include "DirectShowWorker.h"

#pragma comment(lib,"ntdll.lib")
#pragma comment(lib,"detours.lib")

///////////////////////////////////////////////////////////////
//On global

DWORD dwVersion = 0;
DWORD dwMajorVersion = 0;
DWORD dwMinorVersion = 0;
DWORD dwBuild = 0;

BOOL HasPatch4 = TRUE;

////////////////////////////////////////////////////////////////

BOOL StartHook(LPCSTR szDllName, PROC pfnOrg, PROC pfnNew)
{
	HMODULE hmod;
	LPCSTR szLibName;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk;
	DWORD dwOldProtect, dwRVA;
	PBYTE pAddr;

	hmod = GetModuleHandle(NULL);
	pAddr = (PBYTE)hmod;
	pAddr += *((DWORD*)&pAddr[0x3C]);
	dwRVA = *((DWORD*)&pAddr[0x80]);

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hmod + dwRVA);

	for (; pImportDesc->Name; pImportDesc++)
	{
		szLibName = (LPCSTR)((DWORD)hmod + pImportDesc->Name);

		if (!stricmp(szLibName, szDllName))
		{
			pThunk = (PIMAGE_THUNK_DATA)((DWORD)hmod + pImportDesc->FirstThunk);

			for (; pThunk->u1.Function; pThunk++)
			{
				if (pThunk->u1.Function == (DWORD)pfnOrg)
				{
					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);

					pThunk->u1.Function = (DWORD)pfnNew;

					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, dwOldProtect, &dwOldProtect);

					return TRUE;
				}

			}
		}
	}
	return FALSE;
}


BOOL FindIAT(HMODULE hmod, LPCSTR szDllName, LPCSTR szFunc)
{
	LPCSTR szLibName;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk;
	DWORD dwRVA;
	PBYTE pAddr;

	HMODULE hMod = GetModuleHandleA(szDllName);
	DWORD pfnOrg = (DWORD)GetProcAddress(hMod, szFunc);

	pAddr = (PBYTE)hmod;
	pAddr += *((DWORD*)&pAddr[0x3C]);
	dwRVA = *((DWORD*)&pAddr[0x80]);

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hmod + dwRVA);

	for (; pImportDesc->Name; pImportDesc++)
	{
		szLibName = (LPCSTR)((DWORD)hmod + pImportDesc->Name);

		if (!stricmp(szLibName, szDllName))
		{
			pThunk = (PIMAGE_THUNK_DATA)((DWORD)hmod + pImportDesc->FirstThunk);

			for (; pThunk->u1.Function; pThunk++)
			{
				if (pThunk->u1.Function == (DWORD)pfnOrg)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


HFONT WINAPI HookCreateFontIndirectW(LOGFONT *lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	
	if (lstrcmpW(lplf->lfFaceName, L"ＭＳ Ｐゴシック") == 0)
	{
		RtlZeroMemory(lplf->lfFaceName, 64);
		lstrcpyW(lplf->lfFaceName, L"微软雅黑");
	}
	return CreateFontIndirectW(lplf);
}


int WINAPI HookEnumFontFamiliesExW(
	_In_ HDC           hdc,
	_In_ LPLOGFONTW    lpLogfont,
	_In_ FONTENUMPROCW lpEnumFontFamExProc,
	_In_ LPARAM        lParam,
	DWORD              dwFlags
	)
{
	lpLogfont->lfCharSet = GB2312_CHARSET;
	return HookEnumFontFamiliesExW(hdc, lpLogfont, lpEnumFontFamExProc, lParam, dwFlags);
}

BOOL WINAPI HookSetWindowTextW(HWND hWnd, LPCTSTR lpString)
{
	if (dwMajorVersion < 6)
	{
		return SetWindowTextW(hWnd, L"[X'moe女装学院]魔女的夜宴V1.0(XP兼容模式)");
	}
	else
	{
		return SetWindowTextW(hWnd, L"[X'moe女装学院]魔女的夜宴V1.0(请勿网络直播本补丁内容)");
	}
}


HWND WINAPI HookCreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
	DWORD dwStyle, int x, int y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (dwMajorVersion < 6)
	{
		return CreateWindowExW(dwExStyle, lpClassName, L"[X'moe女装学院]魔女的夜宴V1.0(XP兼容模式)",
			dwStyle, x, y, nWidth, nHeight,
			hWndParent, hMenu, hInstance, lpParam);
	}
	else
	{
		return CreateWindowExW(dwExStyle, lpClassName, L"[X'moe女装学院]魔女的夜宴V1.0(请勿网络直播本补丁内容)",
			dwStyle, x, y, nWidth, nHeight,
			hWndParent, hMenu, hInstance, lpParam);
	}
}

int WINAPI HookMultiByteToWideChar(
	UINT   CodePage,
	DWORD  dwFlags,
	LPCSTR lpMultiByteStr,
	int    cbMultiByte,
	LPWSTR lpWideCharStr,
	int    cchWideChar
	)
{
	if (CodePage == CP_ACP)
	{
		return
			MultiByteToWideChar(
			932,
			dwFlags,
			lpMultiByteStr,
			cbMultiByte,
			lpWideCharStr,
			cchWideChar
			);
	}
	else
	{
		return
			MultiByteToWideChar(
			CodePage,
			dwFlags,
			lpMultiByteStr,
			cbMultiByte,
			lpWideCharStr,
			cchWideChar
			);
	}
}

PVOID StubMultiByteToWideChar = NULL;
typedef int (WINAPI *PfunMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
	);

int WINAPI HookMultiByteToWideCharInline(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
	)
{
	if (CodePage == CP_ACP)
	{
		return (PfunMultiByteToWideChar(StubMultiByteToWideChar))(932, dwFlags, lpMultiByteStr,
			cbMultiByte, lpWideCharStr, cchWideChar);
	}
	else
	{
		return (PfunMultiByteToWideChar(StubMultiByteToWideChar))(CodePage, dwFlags, lpMultiByteStr,
			cbMultiByte, lpWideCharStr, cchWideChar);
	}
}

wstring GetFileName2(const WCHAR* lpString)
{
	wstring tmp(lpString);
	return tmp.substr(tmp.find_last_of(L"\\") + 1, wstring::npos);
}

HANDLE WINAPI HookCreateFileW(
	_In_     LPCWSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
	)
{
	wstring FileName = GetFileName2(lpFileName);
	if (!wcsicmp(FileName.c_str(), L"patch4.xp3"))
	{
		return INVALID_HANDLE_VALUE;
	}
	else
	{
		return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
			hTemplateFile);
	}
}


HMODULE WINAPI HookLoadLibraryW(LPCWSTR lpFileName)
{
	HMODULE result = nullptr;
	if (wcsstr(lpFileName, L"krmovie.dll") && /*(dwMajorVersion >= 6) && */ HasPatch4)
	{
		result = LoadLibraryW(L"krmovieEx.dll");
		//InitDirectShowFilter();
	}
	else if (wcsstr(lpFileName, L"PackinOne.dll") || wcsstr(lpFileName, L"psbfile.dll"))
	{
		result = LoadLibraryW(lpFileName);
		char* lpString = (char*)RtlAllocateHeap(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PATH * 4);
		WideCharToMultiByte(CP_ACP, NULL, lpFileName, -1, lpString, MAX_PATH * 4, NULL, NULL);

		PluginHook::InitHook(lpString, result);
		RtlFreeHeap(GetProcessHeap(), 0, lpString);
	}
	else
	{
		result = LoadLibraryW(lpFileName);
	}
	return result;
}


DWORD WINAPI HookGetGlyphOutlineW(
	_In_        HDC            hdc,
	_In_        UINT           uChar,
	_In_        UINT           uFormat,
	_Out_       LPGLYPHMETRICS lpgm,
	_In_        DWORD          cbBuffer,
	_Out_       LPVOID         lpvBuffer,
	_In_  const MAT2           *lpmat2
	)
{
	DWORD Result = GetGlyphOutlineW(hdc, uChar, uFormat, lpgm, cbBuffer, lpvBuffer, lpmat2);
	if (Result == GDI_ERROR)
	{
		LOGFONTW* lplf = (LOGFONTW*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(LOGFONTW));

		HFONT hOldFont = (HFONT)GetCurrentObject(hdc, OBJ_FONT);
		GetObject(hOldFont, sizeof(LOGFONTW), lplf);
		LOGFONTW Info = { 0 };

		Info.lfHeight = lplf->lfHeight;
		Info.lfWidth = lplf->lfWidth;
		Info.lfEscapement = lplf->lfEscapement;
		Info.lfOrientation = lplf->lfOrientation;
		Info.lfWeight = lplf->lfWeight;
		Info.lfItalic = lplf->lfItalic;
		Info.lfUnderline = lplf->lfUnderline;
		Info.lfStrikeOut = lplf->lfStrikeOut;
		Info.lfOutPrecision = lplf->lfOutPrecision;
		Info.lfClipPrecision = lplf->lfClipPrecision;
		Info.lfQuality = lplf->lfQuality;
		Info.lfPitchAndFamily = lplf->lfPitchAndFamily;
		lstrcpyW(Info.lfFaceName, L"MS Gothic");
		lplf->lfCharSet = SHIFTJIS_CHARSET;

		HFONT hFont = CreateFontIndirectW(&Info);

		hOldFont = (HFONT)SelectObject(hdc, hFont);
		Result = GetGlyphOutlineW(hdc, uChar, uFormat,
			lpgm, cbBuffer, lpvBuffer, lpmat2);

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
		HeapFree(GetProcessHeap(), 0, lplf);

		return Result;
	}
}



typedef IStream* (WINAPI *OriCreateStream)(const ttstr & _name, tjs_uint32 flags);
static OriCreateStream pOriCreateStream = (OriCreateStream)0x0041E030;

FARPROC pfCreateFontIndirectW = NULL;
FARPROC pfSetWindowTextW      = NULL;
FARPROC pfCreateWindowExW     = NULL;
FARPROC pfMultiByteToWideChar = NULL;
FARPROC pfLoadLibraryW = NULL;
FARPROC pfGetGlyphOutlineW = NULL;
FARPROC pfEnumFontFamiliesExW = NULL;
FARPROC pfCreateFileW = NULL;

VOID WINAPI Init()
{

	//先检查Krkrz.dll

	pfCreateFontIndirectW = GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectW");
	pfSetWindowTextW      = GetProcAddress(GetModuleHandleW(L"User32.dll"), "SetWindowTextW");
	pfCreateWindowExW     = GetProcAddress(GetModuleHandleW(L"User32.dll"), "CreateWindowExW");
	pfMultiByteToWideChar = GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "MultiByteToWideChar");
	pfLoadLibraryW        = GetProcAddress(GetModuleHandleW(L"KERNEL32.dll"), "LoadLibraryW");
	pfCreateFileW         = GetProcAddress(GetModuleHandleW(L"KERNEL32.dll"), "CreateFileW");
	pfGetGlyphOutlineW    = GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "GetGlyphOutlineW");
	pfEnumFontFamiliesExW = GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "EnumFontFamiliesExW");

	if (!StartHook("Gdi32.dll", pfCreateFontIndirectW, (PROC)HookCreateFontIndirectW))
	{
		MessageBoxW(NULL, L"启动失败:\nErrorCode : 0", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
		ExitProcess(-1);
	}

	if (!StartHook("User32.dll", pfSetWindowTextW, (PROC)HookSetWindowTextW))
	{
		MessageBoxW(NULL, L"启动失败:\nErrorCode : 1", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
		ExitProcess(-1);
	}

	if (!StartHook("User32.dll", pfCreateWindowExW, (PROC)HookCreateWindowExW))
	{
		MessageBoxW(NULL, L"启动失败:\nErrorCode : 2", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
		ExitProcess(-1);
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	StubMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	DetourAttach(&StubMultiByteToWideChar, HookMultiByteToWideCharInline);
	DetourTransactionCommit();

	/*
	if (!StartHook("Kernel32.dll", pfMultiByteToWideChar, (PROC)HookMultiByteToWideChar))
	{
		MessageBoxW(NULL, L"启动失败:\nErrorCode : 3", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
		ExitProcess(-1);
	}
	*/

	if (!StartHook("KERNEL32.dll", pfLoadLibraryW, (PROC)HookLoadLibraryW))
	{
		MessageBoxW(NULL, L"启动失败:\nErrorCode : 4", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
		ExitProcess(-1);
	}

	dwVersion = GetVersion();

	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	
	if (/*dwMajorVersion < 6*/ false)
	{
		if (!StartHook("KERNEL32.dll", pfCreateFileW, (PROC)HookCreateFileW))
		{
			MessageBoxW(NULL, L"启动失败:\nErrorCode : 5", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
			ExitProcess(-1);
		}
	}
	/*
	//GetGlyphOutlineW
	if (!StartHook("Gdi32.dll", pfGetGlyphOutlineW, (PROC)HookGetGlyphOutlineW))
	{
		MessageBoxW(NULL, L"启动失败:\nErrorCode : 5", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
		ExitProcess(-1);
	}
	// pfEnumFontFamiliesExW
	if (!StartHook("Gdi32.dll", pfEnumFontFamiliesExW, (PROC)HookEnumFontFamiliesExW))
	{
		MessageBoxW(NULL, L"启动失败:\nErrorCode : 6", L"[X'moe女装学院]魔女的夜宴V1.0", MB_OK);
		ExitProcess(-1);
	}
	*/

	WinFile Patch4Test;
	if (FAILED(Patch4Test.Open(L"patch4.xp3", WinFile::FileRead)))
		HasPatch4 = FALSE;
	else
		Patch4Test.Release();
}

static CHAR* AnthString = "XmoeSTmoe>_<";

__declspec(dllexport) BOOL WINAPI XmoeLink(const CHAR* lpAnthString)
{
	if (RtlCompareMemory(lpAnthString, AnthString, lstrlenA(AnthString)) == lstrlenA(AnthString))
	{
		return TRUE;
	}
	return FALSE;
}
