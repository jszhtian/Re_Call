// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
	_In_     DWORD     dwExStyle,
	_In_opt_ LPCSTR   lpClassName,
	_In_opt_ LPCSTR   lpWindowName,
	_In_     DWORD     dwStyle,
	_In_     int       x,
	_In_     int       y,
	_In_     int       nWidth,
	_In_     int       nHeight,
	_In_opt_ HWND      hWndParent,
	_In_opt_ HMENU     hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID    lpParam);
HWND WINAPI NewCreateWindowExA(_In_     DWORD     dwExStyle,
	_In_opt_ LPCSTR   lpClassName,
	_In_opt_ LPCSTR   lpWindowName,
	_In_     DWORD     dwStyle,
	_In_     int       x,
	_In_     int       y,
	_In_     int       nWidth,
	_In_     int       nHeight,
	_In_opt_ HWND      hWndParent,
	_In_opt_ HMENU     hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID    lpParam)
{
	/*016D56A4   017A9ED8  |Class = "墛偺泂傑偣傕偭偲両敪堢偭両恎懱應掕俀"
016D56A8   017A9ED8  |WindowName = "墛偺泂傑偣傕偭偲両敪堢偭両恎懱應掕俀"

*/
	if(!lstrcmpA(lpClassName,"墛偺泂傑偣傕偭偲両敪堢偭両恎懱應掕俀") && !lstrcmpA(lpWindowName,"墛偺泂傑偣傕偭偲両敪堢偭両恎懱應掕俀"))
		return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, "【炎孕问题少女汉化组×脸肿汉化组】炎之孕身体测定２ - v1.0",
			dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, lpWindowName, 
		dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

wchar_t szTitle[] = L"提示";
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	/*
	if (strcmp(Pstr, "終了しますか？") == 0)
	{
		strcpy(Pstr, "结束游戏吗？");
	}
	*/
	return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}

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

HANDLE WINAPI NewCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	string strOldName(lpFileName);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string fstReadName = "Kaze\\" + strName;
	if (GetFileAttributesA(fstReadName.c_str()) != INVALID_FILE_ATTRIBUTES)
	{
		cout << "ReadFileFromKaze:" << fstReadName << endl;
		return ((PfuncCreateFileA)(g_pOldCreateFileA))(
			fstReadName.c_str(),
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);
	}
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	strcpy(lplf->lfFaceName, "SimHei");
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

void __fastcall Init()
{
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");

	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	if (DetourTransactionCommit() != NOERROR) { MessageBox(NULL, L"INIT FUNCTION ERROR", L"System", MB_OK | MB_ICONERROR); }
	else { MessageBox(NULL, L"本补丁由【炎孕问题少女汉化组×脸肿汉化组】制作\n禁止一切录播直播和商业行为", L"警告", MB_OK | MB_ICONINFORMATION); }
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
		Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) void AyamiKaze(void)
{
	return;
}