// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

HFONT WINAPI HookCreateFontIndirectW(LOGFONTW* lplf)
{
    LOGFONTW lf;
    memcpy(&lf, lplf, sizeof(LOGFONTW));
    wcscpy(lf.lfFaceName, L"SimHei");
    lf.lfCharSet = GB2312_CHARSET;
    return CreateFontIndirectW(&lf);
}

bool IsJis(wchar_t* Text)
{
	for (int i = 0; i < wcslen(Text); i++)
	{

		if ((unsigned int)Text[i] < 0x20)
			return false;
		if ((unsigned int)Text[i] > 0x80)
			return true;
	}
	return false;
}

int WINAPI HookMultiByteToWideChar(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
)
{

    int ret =  MultiByteToWideChar(CodePage, dwFlags,
        lpMultiByteStr, cbMultiByte, lpWideCharStr, 0x1000);
    //TXT << wtoc(lpWideCharStr) << endl;
    if (!strcmp(wtoc(lpWideCharStr), "俵俽 僑僔僢僋")) 
    {
        cout << "Hook" << endl;
        wcscpy(lpWideCharStr, L"MS UI Gothic");
    }
    return ret;
}

void IATHook()
{
	if (!IATPatch("Kernel32.dll", GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "MultiByteToWideChar"), (PROC)HookMultiByteToWideChar))
	{
		MessageBoxW(0, L"MultiByteToWideChar Hook Error。", L"IATHook Error", 0);
		return;
	}
    /*
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectW"), (PROC)HookCreateFontIndirectW))
    {
        MessageBoxW(0, L"CreateFontIndirectW Hook Error。", L"IATHook Error", 0);
        return;
    }
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
        make_console();
        IATHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

