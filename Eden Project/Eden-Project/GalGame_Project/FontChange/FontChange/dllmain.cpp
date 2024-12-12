// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include <D3D9.h>


HFONT(WINAPI* OldCreateFontIndirectA)(_In_ CONST LOGFONTA* lplf) = CreateFontIndirectA;
HFONT WINAPI NewCreateFontIndirectA(_In_ /* CONST */ LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    lplf->lfWeight = FW_BOLD;
    lplf->lfQuality = ANTIALIASED_QUALITY;
    //StringCbCopyA(lplf->lfFaceName, 32, "\xBA\xDA\xCC\xE5"); // 黑体
    lstrcpyA(lplf->lfFaceName, "YouYuan");
    return OldCreateFontIndirectA(lplf);
}



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
fnCreateFontA pCreateFontA;

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
    _In_ LPCTSTR lpszFace)
{
    return pCreateFontA(nHeight, nWidth, nEscapement, nOrientation, FW_BOLD, fdwItalic, fdwUnderline, fdwStrikeOut,
        GB2312_CHARSET, fdwOutputPrecision, fdwClipPrecision, ANTIALIASED_QUALITY, fdwPitchAndFamily, lpszFace);
}

void WINAPI APIHook()
{
    pCreateFontA = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)OldCreateFontIndirectA, NewCreateFontIndirectA);

    DetourAttach((void**)&pCreateFontA, newCreateFontA);
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
        APIHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI func()
{
}