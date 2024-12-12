// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;

void BypassInstallCheck()
{
	BYTE InstallCheckJMP[] = { 0xEB };
	int CheckedJMP = BaseAddr + 0x40383;
	memcopy((void*)CheckedJMP, InstallCheckJMP, sizeof(InstallCheckJMP));
}

void MyCharSet()
{
	BYTE CharSet[] = { 0xFF };
	/*
	int MyCharSet1 = BaseAddr + 0x155AD3;
	int MyCharSet2 = BaseAddr + 0x155ADB;
	int MyCharSet3 = BaseAddr + 0x155AE6;
	int MyCharSet4 = BaseAddr + 0x155AEE;
	*/
	int MyCharSet1 = BaseAddr + 0x273EA3;
	int MyCharSet2 = BaseAddr + 0x273EAB;
	int MyCharSet3 = BaseAddr + 0x273EB6;
	int MyCharSet4 = BaseAddr + 0x273EBE;
	memcopy((void*)MyCharSet1, CharSet, sizeof(CharSet));
	memcopy((void*)MyCharSet2, CharSet, sizeof(CharSet));
	memcopy((void*)MyCharSet3, CharSet, sizeof(CharSet));
	memcopy((void*)MyCharSet4, CharSet, sizeof(CharSet));
}

void CatSystemInitPatch()
{
	//BypassInstallCheck();
	MyCharSet();
}

/*************************************************************************************************************************************/

HFONT WINAPI newCreateFontA(_In_ int     nHeight,
	_In_ int     nWidth,
	_In_ int     nEscapement,
	_In_ int     nOrientation,
	_In_ int     fnWeight,
	_In_ DWORD   fdwItalic,
	_In_ DWORD   fdwUnderline,
	_In_ DWORD   fdwStrikeOut,
	_In_ DWORD   fdwCharSet,
	_In_ DWORD   fdwOutputPrecision,
	_In_ DWORD   fdwClipPrecision,
	_In_ DWORD   fdwQuality,
	_In_ DWORD   fdwPitchAndFamily,
	_In_ LPCSTR lpszFace)
{
	wchar_t* wcs = nullptr;
	fdwCharSet = GB2312_CHARSET;
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lpszFace, strlen(lpszFace));
	wcs = ctow936(buffer);
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, wcs);
}

int WINAPI newEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	LOGFONTA Lfont;
	memcpy(&Lfont, lpLogfont, sizeof(LOGFONTA));
	Lfont.lfCharSet = GB2312_CHARSET;
	memcpy(lpLogfont, &Lfont, sizeof(LOGFONTA));
	return ((fnEnumFontFamiliesExA)pEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	__asm
	{
		pushad
		pushfd
	}
	CRC32 crc;
	int ret = 0;
	ret = ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
	if (cp == 932)
	{
		ret = ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(936, dwFg, lpMBS, cbMB, lpWCS, ccWC);
	}
	__asm
	{
		popfd
		popad
	}
	return ret;
}

int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	return MessageBoxAOLD(hWnd, Pstr, lpCaption, uType);
}

HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	//strcpy(lplf->lfFaceName, "黑体");

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

HFONT WINAPI newCreateFontIndirectW(LOGFONTW* lplf)
{
	LOGFONTW lf;
	memcpy(&lf, lplf, sizeof(LOGFONTW));
	wcscpy(lf.lfFaceName, L"黑体");
	lf.lfCharSet = GB2312_CHARSET;
	return pCreateFontIndirectW(&lf);
}

bool WINAPI newSetWindowTextW(HWND hw, LPCWSTR lps)
{
	wchar_t newtitle[] = L"密语-体验版汉化";
	return SetWindowTextW(hw, newtitle);
}

HANDLE WINAPI NewCreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	if (!lpFileName)
	{
		return INVALID_HANDLE_VALUE;
	}
	
	string strOldName((LPCSTR)lpFileName);
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string strNewName;
	strNewName = strOldName;
	if (strName == "scene.int") {
		strNewName == strDirName + "cnpack.cpk";
	}
	return ((PfuncCreateFileW)(g_pOldCreateFileW))(
		(LPCWSTR)strNewName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

void EdenInitHook()
{
	//pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	//pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
	//pSetWindowTextW = (fnSetWindowTextW)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextW");
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	pCreateFontIndirectW = (fnCreateFontIndirectW)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontIndirectW");
	DetourTransactionBegin();
	//DetourAttach((void**)& pCreateFontA, newCreateFontA);
	//DetourAttach((void**)& pSetWindowTextW, newSetWindowTextW);
	//DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	//DetourAttach((void**)& pEnumFontFamiliesExA, newEnumFontFamiliesExA);
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach((void**)& pCreateFontIndirectW, newCreateFontIndirectW);
	//DetourAttach(&g_pOldCreateFileW, NewCreateFileW);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);

	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"API Function Hook Error.", L"EdenInitHook", MB_OK | MB_ICONERROR);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//make_console();
		CatSystemInitPatch();
		EdenInitHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

