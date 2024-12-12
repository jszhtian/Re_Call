// dllmain.cpp : 定义 DLL 应用程序的入口点。
/*
Ever tried. Ever failed.
No matter. Try again.
Fail again. Fail better.
*/
#include "framework.h"
HINSTANCE hInst;

typedef LCID(WINAPI* fnGetUserDefaultLCID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultLangID)(void);
typedef LANGID(WINAPI* fnGetSystemDefaultUILanguage)(void);
typedef bool (WINAPI* fnSetWindowTextA)(HWND hWnd, LPCSTR lpString);
typedef HFONT(WINAPI* fnCreateFontA)(_In_ int     nHeight,
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
	_In_ LPCTSTR lpszFace);
typedef int (WINAPI* fnEnumFontFamiliesExA)(
	HDC           hdc,
	LPLOGFONTA    lpLogfont,
	FONTENUMPROCA lpProc,
	LPARAM        lParam,
	DWORD         dwFlags
	);
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
PVOID g_pOldCreateFileA = CreateFileA;
typedef HANDLE(WINAPI* PfuncCreateFileA)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);
fnMessageboxA MessageBoxAOLD;
fnCreateFontA pCreateFontA;
fnGetSystemDefaultLangID pGetSystemDefaultLangID;
fnGetSystemDefaultUILanguage pGetSystemDefaultUILanguage;
fnGetUserDefaultLCID pGetUserDefaultLCID;
fnSetWindowTextA pSetWindowTextA;
fnEnumFontFamiliesExA pEnumFontFamiliesExA;
PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar);
wchar_t* ctow(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(932, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(932, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}

wchar_t* ctow936(const char* str)
{
	wchar_t* buffer = nullptr;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(936, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n];
		memset(buffer, 0, sizeof(wchar_t) * (n));
		MultiByteToWideChar(936, 0, (const char*)str, int(nu), buffer, int(n));
	}
	return buffer;
}

LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

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
	fdwCharSet = ANSI_CHARSET;
	char buffer[0x100] = { 0 };
	memset(buffer, 0, 0x100);
	memcpy(buffer, lpszFace, strlen(lpszFace));
	wcs = ctow936(buffer);
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, wcs);
}

LCID WINAPI newGetUserDefaultLCID()
{
	return 0x411;
}

LANGID WINAPI newGetSystemDefaultLangID()
{
	return 0x411;
}

LANGID WINAPI newGetSystemDefaultUILanguage()
{
	return 0x411;
}

bool WINAPI newSetWindowTextA(HWND hw, LPCSTR lps)
{
	wchar_t newtitle[] = L"【天之圣杯汉化组】奇异恩典-What color is your attribute？";
	return SetWindowTextW(hw, newtitle);
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
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(936, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	char* Pstr2 = wtocGBK(ctowJIS((char*)lpCaption));
	return MessageBoxAOLD(hWnd, Pstr, Pstr2, uType);
}


void EdenInitHook()
{
	pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
	pGetSystemDefaultLangID = (fnGetSystemDefaultLangID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultLangID");
	pGetSystemDefaultUILanguage = (fnGetSystemDefaultUILanguage)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetSystemDefaultUILanguage");
	pGetUserDefaultLCID = (fnGetUserDefaultLCID)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetUserDefaultLCID");
	pSetWindowTextA = (fnSetWindowTextA)GetProcAddress(GetModuleHandle(L"User32.dll"), "SetWindowTextA");
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	DetourTransactionBegin();
	DetourAttach((void**)& pCreateFontA, newCreateFontA);
	DetourAttach((void**)& pGetSystemDefaultLangID, newGetSystemDefaultLangID);
	DetourAttach((void**)& pGetSystemDefaultUILanguage, newGetSystemDefaultUILanguage);
	DetourAttach((void**)& pGetUserDefaultLCID, newGetUserDefaultLCID);
	DetourAttach((void**)& pSetWindowTextA, newSetWindowTextA);
	DetourAttach((void**)& pEnumFontFamiliesExA, newEnumFontFamiliesExA);
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBoxW(NULL, L"SOME ERROR!!!", L"EdenFileSystem", MB_OK | MB_ICONWARNING);
	}
}
/**********************************************************************************************************************/
//中文显示
/*
因为默认是从231000开始的，所以写的时候需要重新计算offset。
002649F4  |.  3C 9F         cmp al,0x9F
002649F6  |.  76 0F         jbe short amegure.00264A07
002649F8  |>  3C E0         cmp al,0xE0
002649FA  |.  72 04         jb short amegure.00264A00
002649FC  |.  3C FC         cmp al,0xFC
002649FE  |.  76 07         jbe short amegure.00264A07
00264A00  |>  3C FF         cmp al,0xFF

002659B7  |> \3C A0         cmp al,0xA0
002659B9  |.  73 06         jnb short amegure.002659C1
002659BB  |.  B8 01000000   mov eax,0x1
002659C0  |.  C3            retn
002659C1  |>  3C E0         cmp al,0xE0

002DEC60  |> \3C A0         cmp al,0xA0
002DEC62  |.  73 09         jnb short amegure.002DEC6D
002DEC64  |.  B8 01000000   mov eax,0x1
002DEC69  |.  5D            pop ebp                                  ;  kernel32.754B6359
002DEC6A  |.  C2 0400       retn 0x4
002DEC6D  |>  3C E0         cmp al,0xE0

002643D9  |.  81FB 40810000 cmp ebx,0x8140
*/
void EdenCharCheck()
{
	BYTE Patch1[] = { 0xFE };
	BYTE Patch2[] = { 0xFF };
	BYTE Patch3[] = { 0xA1 };

	int CheckPoint1 = BaseAddr + 0x349f5;
	int CheckPoint2 = BaseAddr + 0x349f9;
	int CheckPoint3 = BaseAddr + 0x349fd;
	int CheckPoint4 = BaseAddr + 0x359b8;
	int CheckPoint5 = BaseAddr + 0x359c2;
	int CheckPoint6 = BaseAddr + 0xaec61;
	int CheckPoint7 = BaseAddr + 0xaec6e;
	int SpacePoint1 = BaseAddr + 0x343db;
	int SpacePoint2 = BaseAddr + 0x343dc;

	if (debug) 
	{
		cout << "BGI Check info" << endl;
		cout << hex << CheckPoint1 << " | " << hex << CheckPoint2 << " | " << hex << CheckPoint3 << " | " << hex << CheckPoint4 << " | " << hex << CheckPoint5 << " | " << hex << CheckPoint6 << " | " << hex << CheckPoint7 << " | " << hex << SpacePoint1 << " | " << hex << SpacePoint2 << endl;
	}

	memcopy((void*)CheckPoint1, Patch2, sizeof(Patch2));
	memcopy((void*)CheckPoint2, Patch1, sizeof(Patch1));
	memcopy((void*)CheckPoint3, Patch2, sizeof(Patch2));
	memcopy((void*)CheckPoint4, Patch2, sizeof(Patch2));
	memcopy((void*)CheckPoint5, Patch2, sizeof(Patch2));
	memcopy((void*)CheckPoint6, Patch2, sizeof(Patch2));
	memcopy((void*)CheckPoint7, Patch2, sizeof(Patch2));
	memcopy((void*)SpacePoint1, Patch3, sizeof(Patch3));
	memcopy((void*)SpacePoint2, Patch3, sizeof(Patch3));
}
/**********************************************************************************************************************/

HANDLE WINAPI NewCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	if (!lpFileName)
	{
		return INVALID_HANDLE_VALUE;
	}

	string strOldName(lpFileName);
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string strNewName;
	strNewName = strOldName;
	if (debug) 
	{
		cout << strName << endl;
		VFS << strNewName << endl;
	}
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		strNewName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

void LoadEdenPack()
{
	//FillMyNameDic();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	DetourTransactionCommit();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//hInst = (HINSTANCE)hModule;
		if (debug)
		{
			make_console();
		}
		EdenInitHook();
		EdenCharCheck();
		//EdenFileSystem_Main(hInst);
		if (!FileCopy())
		{
			MessageBox(0, L"汉化启动失败。", L"Ayaminokaze File System", 0);
			return false;
		}
		break;
    case DLL_THREAD_ATTACH:break;
    case DLL_THREAD_DETACH:break;
    case DLL_PROCESS_DETACH:break;
		//FileClose();
        break;
    }
    return TRUE;
}

