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
	_asm pushad;
	_asm pushfd;
	//TXT << lpMultiByteStr << endl;
	switch (CodePage)
	{
	case CP_ACP:
	case CP_OEMCP:
	case CP_THREAD_ACP:
		CodePage = 932;
		dwFlags &= ~MB_ERR_INVALID_CHARS;
		break;
	default:
		break;
	}
	_asm popfd;
	_asm popad;
	return MultiByteToWideChar(CodePage, dwFlags,
		lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
	LOGFONTA lf;
	memcpy(&lf, lplf, sizeof(LOGFONTA));
	strcpy(lf.lfFaceName, "SimHei");
	lf.lfCharSet = GB2312_CHARSET;
	return CreateFontIndirectA(&lf);
}

void IATHook()
{
	HMODULE result = GetModuleHandleW(NULL);
	if (result == 0)
	{
		MessageBoxW(0, L"GetModuleHandleW returned error code。", L"IATHook Error", 0);
		return;
	}
	if (!IATPatch(result, "Kernel32.dll", GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "MultiByteToWideChar"), (PROC)HookMultiByteToWideChar))
	{
		MessageBoxW(0, L"MultiByteToWideChar Hook Error。", L"IATHook Error", 0);
		return;
	}
	if (!IATPatch(result, "Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectA"), (PROC)HookCreateFontIndirectA))
	{
		MessageBoxW(0, L"CreateFontIndirectA Hook Error。", L"IATHook Error", 0);
		return;
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
		IATHook();
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
