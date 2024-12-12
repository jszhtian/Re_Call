// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

int WINAPI HookMultiByteToWideChar(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
)
{
    cout << CodePage << endl;
	return MultiByteToWideChar(CodePage, dwFlags,
		lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

int WINAPI HookWideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar)
{
    if (CodePage == CP_THREAD_ACP)
        CodePage = 932;
    return WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    strcpy(lplf->lfFaceName, "黑体");

    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}
void Init()
{
    if (!IATPatch("Kernel32.dll", GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "WideCharToMultiByte"), (PROC)HookWideCharToMultiByte))
    {
        MessageBoxW(0, L"WideCharToMultiByte Hook Error。", L"IATHook Error", 0);
    }
    /*
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"启动失败", L"启动失败", MB_OK);
    */
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
__declspec(dllexport)void WINAPI AyamiKaze()
{
}
