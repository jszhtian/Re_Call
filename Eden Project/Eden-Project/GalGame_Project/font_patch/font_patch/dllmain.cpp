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
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectW"), (PROC)HookCreateFontIndirectW))
    {
        MessageBoxW(0, L"CreateFontIndirectW Hook Error。", L"IATHook Error", 0);
        return;
    }
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectA"), (PROC)HookCreateFontIndirectA))
    {
        MessageBoxW(0, L"CreateFontIndirectA Hook Error。", L"IATHook Error", 0);
        return;
    }
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    lplf->lfWeight = FW_BOLD;
    lplf->lfQuality = ANTIALIASED_QUALITY;
    strcpy(lplf->lfFaceName, "SimHei");

    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID g_pOldCreateFontIndirectW = CreateFontIndirectW;
typedef int (WINAPI* PfuncCreateFontIndirectW)(LOGFONTW* lplf);
int WINAPI NewCreateFontIndirectW(LOGFONTW* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    lplf->lfWeight = FW_BOLD;
    lplf->lfQuality = ANTIALIASED_QUALITY;
    wcscpy(lplf->lfFaceName, L"SimHei");

    return ((PfuncCreateFontIndirectW)g_pOldCreateFontIndirectW)(lplf);
}

typedef HFONT(WINAPI* fnCreateFontA)(
    int nHeight, // logical height of font height
    int nWidth, // logical average character width
    int nEscapement, // angle of escapement
    int nOrientation, // base-line orientation angle
    int fnWeight, // font weight
    DWORD fdwItalic, // italic attribute flag
    DWORD fdwUnderline, // underline attribute flag
    DWORD fdwStrikeOut, // strikeout attribute flag
    DWORD fdwCharSet, // character set identifier
    DWORD fdwOutputPrecision, // output precision
    DWORD fdwClipPrecision, // clipping precision
    DWORD fdwQuality, // output quality
    DWORD fdwPitchAndFamily, // pitch and family
    LPCSTR lpszFace // pointer to typeface name string
    );
fnCreateFontA CreateFontAOLD;
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
    fdwCharSet = 0x86;
    fnWeight = FW_SEMIBOLD;
    return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic,
        fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
}
void InlineHook()
{
    CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
    DetourAttach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
    //DetourAttach((void**)&CreateFontAOLD, CreateFontAEx);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Init hook error", L"Init", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
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
        //IATHook();
        InlineHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI FonrPatch()
{
}