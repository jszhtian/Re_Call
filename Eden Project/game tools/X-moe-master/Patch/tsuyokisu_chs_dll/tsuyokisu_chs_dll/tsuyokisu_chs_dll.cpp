#include "stdafx.h"
#include <string>
#include "dxsdk\Include\d3dx9core.h"
#include "detours.h"
#include "Adler32.h"
#include "D3DFont.h"
#include "DebugInfo.h"

//0x97a02c9e
#include "BG370000.h"
//0x6a779546
#include "BG840000.h"
//0x0d7f6ab4
#include "BG840005.h"
//0xaebe1c97
#include "BG900008.h"
//0x5a36f06e
#include "BG900015.h"

#include "DummyLogo.h"
//0x19d7d32e
#include "BG840004.h"

using std::wstring;

#pragma comment(lib, "dxsdk\\Lib\\x86\\d3dx9.lib")
#pragma comment(lib, "detours.lib")
//stdcall:
/*
CPU Stack
Address   Value      Comments
0018FCB4  [00416FCE  ; /RETURN from tkfe.00451640 to tkfe.00416FCE
0018FCB8  /0018FD30  ; |Arg1 = 18FD30
0018FCBC  |0049E450  ; \Arg2 = ASCII "bmp.tbl"
*/

//Function Start :
/*
00451640 / $  55            push ebp; tkfe.00451640(guessed Arg1, Arg2)
*/

char* WINAPI LoadFile(unsigned long dwSize, const char* lpFileName)
{
	return nullptr;
}

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

int WINAPI HookMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	WCHAR* uTitle = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 1024 * 2);
	WCHAR* uInfo = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 1024 * 2);
	MultiByteToWideChar(932, 0, lpText, -1, uInfo, 1024);
	if (!strncmp(lpCaption, "[X'moe]", strlen("[X'moe]")))
	{
		MultiByteToWideChar(936, 0, lpCaption, -1, uTitle, 1024);
	}
	else
	{
		MultiByteToWideChar(932, 0, lpCaption, -1, uTitle, 1024);
	}
	int result = MessageBoxW(hWnd, uInfo, uTitle, uType);
	HeapFree(GetProcessHeap(), 0, uTitle);
	HeapFree(GetProcessHeap(), 0, uInfo);
	return result;
}


HANDLE WINAPI HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	//OutputString(lpFileName);

	if (!strcmp(lpFileName, "data.fpk"))
	{
		return CreateFileW(L"data_chs.fpk", dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
			hTemplateFile);
	}
	else if (!stricmp(lpFileName, "chip.fpk"))
	{
		return CreateFileW(L"chip_chs.fpk", dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
			hTemplateFile);
	}
	else
	{
		return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
			hTemplateFile);
	}
}

HFONT WINAPI HookCreateFontIndirectA(LOGFONTA *lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	LOGFONTW Info = {0};
	Info.lfHeight = lplf->lfHeight;
	Info.lfWidth = lplf->lfWidth;
	Info.lfEscapement = lplf->lfEscapement;
	Info.lfOrientation = lplf->lfOrientation;
	Info.lfWeight = lplf->lfWeight;
	Info.lfItalic = lplf->lfItalic;
	Info.lfUnderline = lplf->lfUnderline;
	Info.lfStrikeOut = Info.lfStrikeOut;
	Info.lfCharSet = GB2312_CHARSET;
	Info.lfOutPrecision = lplf->lfOutPrecision;
	Info.lfClipPrecision = lplf->lfClipPrecision;
	Info.lfQuality = lplf->lfQuality;
	Info.lfPitchAndFamily = lplf->lfPitchAndFamily;
	lstrcpyW(Info.lfFaceName, L"SimHei");

	//OutputString("called Gdi CreateFont Indirect\n");

	return CreateFontIndirectW(&Info);
}


int WINAPI HookEnumFontFamiliesExA(
	_In_ HDC          hdc,
	_In_ LPLOGFONTA    lpLogfont,
	_In_ FONTENUMPROCA lpEnumFontFamExProc,
	_In_ LPARAM       lParam,
	DWORD        dwFlags
	)
{
	LOGFONTA Info = { 0 };
	Info.lfCharSet = GB2312_CHARSET;
	Info.lfPitchAndFamily = lpLogfont->lfPitchAndFamily;
	lstrcpyA(Info.lfFaceName, "SimHei");

	return EnumFontFamiliesExA(hdc, &Info, lpEnumFontFamExProc, lParam, dwFlags);
}



DWORD WINAPI HookGetGlyphOutlineA(HDC hdc, UINT uChar, UINT uFormat,
	LPGLYPHMETRICS lpgm, DWORD cbBuffer, LPVOID lpvBuffer, const MAT2 *lpmat2)
{
	//OutputString("Call Glyph");
	UINT sChar = uChar;
	int len;
	char mbchs[2];
	UINT cp = 936;
	if (IsDBCSLeadByteEx(cp, uChar >> 8))
	{
		len = 2;
		mbchs[0] = (uChar & 0xff00) >> 8;
		mbchs[1] = (uChar & 0xff);
	}
	else
	{
		len = 1;
		mbchs[0] = (uChar & 0xff);
	}
	uChar = 0;
	MultiByteToWideChar(cp, 0, mbchs, len, (LPWSTR)&uChar, 1);

	//A2 E1 
	if ((uChar) == (UINT)0x2468)
	{
		//6A 26 
		return GetGlyphOutlineW(hdc, (UINT)0x266A, uFormat,
			lpgm, cbBuffer, lpvBuffer, lpmat2);
	}
	else
	{
		return GetGlyphOutlineW(hdc, uChar, uFormat,
			lpgm, cbBuffer, lpvBuffer, lpmat2);
	}
}


HRESULT WINAPI HookD3DXCreateFontA(LPDIRECT3DDEVICE9 pDevice, INT Height,
	UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD CharSet,
	DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily,
	LPCSTR pFacename, LPD3DXFONT *ppFont)
{
	//OutputString("called D3D CreateFont\n");

	LPD3DXFONT FontHook = NULL;
	HRESULT Result =  D3DXCreateFontW(pDevice, Height,
		Width, Weight, MipLevels, Italic, GB2312_CHARSET,
		OutputPrecision, Quality, PitchAndFamily,
		L"SimHei", &FontHook);

	//XmoeD3DFont* Attach = (XmoeD3DFont*)HeapAlloc(GetProcessHeap(), 0, sizeof(XmoeD3DFont));
	XmoeD3DFont* Attach = new XmoeD3DFont;
	Attach->Init(FontHook);
	*ppFont = Attach;
	return Result;
}

HRESULT WINAPI HookD3DXCreateFontIndirectA(LPDIRECT3DDEVICE9 pDevice, D3DXFONT_DESCA* pDesc,
	LPD3DXFONT* pFont)
{
	//pDesc->CharSet = GB2312_CHARSET;

	D3DXFONT_DESCW Info = { 0 };
	Info.Height = pDesc->Height;
	Info.Width = pDesc->Width;
	Info.Weight = pDesc->Weight;
	Info.MipLevels = pDesc->MipLevels;
	Info.Italic = pDesc->Italic;
	Info.CharSet = GB2312_CHARSET;
	Info.OutputPrecision = pDesc->OutputPrecision;
	Info.Quality = pDesc->Quality;
	Info.PitchAndFamily = pDesc->PitchAndFamily;
	lstrcpyW(Info.FaceName, L"SimHei");

	//OutputString("D3D CreateFont Indirect\n");

	return D3DXCreateFontIndirectW(pDevice, &Info, pFont);
}
//D3DX9_42.dll			C:\Windows\SysWOW64\D3DX9_42.dll


BOOL WINAPI HookSetWindowTextA(HWND hWnd, LPCTSTR lpString)
{
#if 0
	UINT Acp = GetACP();
	if (Acp != 936)
	{
		return SetWindowTextW(hWnd, L"嬌蠻之吻Full Edition　famille漢化組V1.0");
	}
	else
	{
		return SetWindowTextW(hWnd, L"娇蛮之吻Full Edition　famille汉化组V1.0");
	}
#else
	return SetWindowTextW(hWnd, L"娇蛮之吻Full Edition　famille汉化组Ver1.0");
#endif

}


UINT WINAPI HookGetACP()
{
	return 936;
}

UINT WINAPI HookGetOEMCP()
{
	return 936;
}

/*
//0x97a02c9e
#include "BG370000.h"
//0x6a779546
#include "BG840000.h"
//0x0d7f6ab4
#include "BG840005.h"
//0xaebe1c97
#include "BG900008.h"
//0x5a36f06e
#include "BG900015.h"
*/

HRESULT WINAPI HookD3DXLoadSurfaceFromFileInMemory(
	_In_          LPDIRECT3DSURFACE9 pDestSurface,
	_In_    const PALETTEENTRY       *pDestPalette,
	_In_    const RECT               *pDestRect,
	_In_          LPCVOID            pSrcData,
	_In_          UINT               SrcData,
	_In_    const RECT               *pSrcRect,
	_In_          DWORD              Filter,
	_In_          D3DCOLOR           ColorKey,
	_Inout_       D3DXIMAGE_INFO     *pSrcInfo
	)
{
	HRESULT Result;

	if (SrcData == 0x0015f938)
	{
		ULONG Hash = adler32(1, (const PBYTE)pSrcData, SrcData);
		if (Hash == 0x97a02c9e)
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, BG370000, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
		else if (Hash == 0x6a779546)
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, BG840000, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
		else if (Hash == 0x0d7f6ab4)
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, BG840005, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
		else if (Hash == 0xaebe1c97)
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, BG900008, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
		else if (Hash == 0x5a36f06e)
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, BG900015, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
		else if (Hash == 0x19d7d32e)
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, BG840004, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
		//Dummy Test Logo
#if 0
		else if (Hash == 0x273e6c3c)
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, DummyLogo, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
#endif
		else
		{
			Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
				pDestRect, pSrcData, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
		}
	}
	else
	{
		Result = D3DXLoadSurfaceFromFileInMemory(pDestSurface, pDestPalette,
			pDestRect, pSrcData, SrcData, pSrcRect, Filter, ColorKey, pSrcInfo);
	}
	return Result;
}



wstring GetPackageName(wstring& fileName)
{
	wstring temp(fileName);
	wstring::size_type pos = temp.find_last_of(L"\\");

	if (pos != wstring::npos)
	{
		temp = temp.substr(pos + 1, temp.length());
	}

	wstring temp2(temp);
	wstring::size_type pos2 = temp2.find_last_of(L"\\");
	if (pos2 != wstring::npos)
	{
		temp2 = temp2.substr(pos + 1, temp2.length());
	}
	return temp2;
}

typedef HANDLE(WINAPI *PfunHooKCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess,
	DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile);

PVOID pOldCreateFileW = NULL;
HANDLE WINAPI HooKCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess,
	DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	if (!wcscmp(GetPackageName(wstring(lpFileName)).c_str(), L"chip.fpk"))
	{
		return ((PfunHooKCreateFileW)pOldCreateFileW)(L"chip_chs.fpk", dwDesiredAccess,
			dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	else
	{
		return ((PfunHooKCreateFileW)pOldCreateFileW)(lpFileName, dwDesiredAccess,
			dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
}


BOOL WINAPI HookGetTextMetricsA(
	_In_  HDC          hdc,
	_Out_ LPTEXTMETRICA lptm
	)
{
	BOOL Result = GetTextMetricsA(hdc, lptm);
	lptm->tmCharSet = GB2312_CHARSET;

	//OutputString("called GetTextMetrics\n");
	return Result;
}




VOID WINAPI PreHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	pOldCreateFileW = DetourFindFunction("Kernel32.dll", "CreateFileW");
	DetourAttach(&pOldCreateFileW, HooKCreateFileW);
	DetourTransactionCommit();
}



/**********************************************/

FARPROC pfD3DXLoadSurfaceFromFileInMemory = NULL;
FARPROC pfCreateFontIndirectA = NULL;
FARPROC pfGetGlyphOutlineA = NULL;
FARPROC pfD3DXCreateFontA = NULL;
FARPROC pfD3DXCreateFontIndirectA = NULL;
FARPROC pfMessageBoxA = NULL;
FARPROC pfSetWindowTextA = NULL;
FARPROC pfRegOpenKeyExA = NULL;
FARPROC pfRegQueryValueExA = NULL;
FARPROC pfCreateFileA = NULL;
FARPROC pfEnumFontFamiliesExA = NULL;
FARPROC pfGetTextMetricsA = NULL;
FARPROC pfGetACP = NULL;
FARPROC pfGetOEMCP = NULL;


BOOL WINAPI Install()
{
	//AllocConsole();

	PreHook();

	pfCreateFileA = GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "CreateFileA");
	pfCreateFontIndirectA = GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectA");
	pfGetGlyphOutlineA = GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "GetGlyphOutlineA");
	pfD3DXCreateFontA = GetProcAddress(GetModuleHandleW(L"D3DX9_42.dll"), "D3DXCreateFontA");
	pfD3DXCreateFontIndirectA = GetProcAddress(GetModuleHandleW(L"D3DX9_42.dll"), "D3DXCreateFontIndirectA");
	pfMessageBoxA = GetProcAddress(GetModuleHandleW(L"User32.dll"), "MessageBoxA");
	pfSetWindowTextA = GetProcAddress(GetModuleHandleW(L"User32.dll"), "SetWindowTextA");
	pfRegOpenKeyExA = GetProcAddress(GetModuleHandleW(L"Advapi32.dll"), "RegOpenKeyExA");
	pfD3DXLoadSurfaceFromFileInMemory = GetProcAddress(GetModuleHandleW(L"D3DX9_42.dll"), "D3DXLoadSurfaceFromFileInMemory");
	pfEnumFontFamiliesExA = GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "EnumFontFamiliesExA");
	pfGetTextMetricsA = GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "GetTextMetricsA");
	pfGetACP = GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "GetACP");
	pfGetOEMCP = GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "GetOEMCP");

	if (!StartHook("Kernel32.dll", pfCreateFileA, (PROC)HookCreateFileA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000000]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("Gdi32.dll", pfCreateFontIndirectA, (PROC)HookCreateFontIndirectA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000001]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}
	
	if (!StartHook("Gdi32.dll", pfGetGlyphOutlineA, (PROC)HookGetGlyphOutlineA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000002]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("D3DX9_42.dll", pfD3DXCreateFontA, (PROC)HookD3DXCreateFontA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000003]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("D3DX9_42.dll", pfD3DXCreateFontIndirectA, (PROC)HookD3DXCreateFontIndirectA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000004]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("User32.dll", pfMessageBoxA, (PROC)HookMessageBoxA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000005]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("User32.dll", pfSetWindowTextA, (PROC)HookSetWindowTextA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000006]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("D3DX9_42.dll", pfD3DXLoadSurfaceFromFileInMemory, (PROC)HookD3DXLoadSurfaceFromFileInMemory))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000007]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	/*
	if (!StartHook("Gdi32.dll", pfEnumFontFamiliesExA, (PROC)HookEnumFontFamiliesExA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000008]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}
	*/

	if (!StartHook("Gdi32.dll", pfGetTextMetricsA, (PROC)HookGetTextMetricsA))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x00000009]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("Kernel32.dll", pfGetACP, (PROC)HookGetACP))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x0000000A]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	if (!StartHook("Kernel32.dll", pfGetOEMCP, (PROC)HookGetOEMCP))
	{
		MessageBoxW(NULL, L"娇蛮之吻启动失败[code : 0x0000000B]", L"错误", MB_OK);
		ExitProcess(-1);
		return FALSE;
	}

	return TRUE;
}

BOOL WINAPI Uninstall()
{
	return TRUE;
}
