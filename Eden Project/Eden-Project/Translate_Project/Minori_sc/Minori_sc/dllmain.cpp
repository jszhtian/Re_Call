// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#pragma comment(lib, "detours.lib")

using namespace std;

void __stdcall LumShowFileName(char* text)
{
    cout << text << endl; 
}

PVOID pGetFileName = NULL;
__declspec(naked)void GetFileName()
{
    __asm
    {
        pushad
        pushfd
        push edi
        call LumShowFileName
        popfd
        popad
        jmp pGetFileName
    }
}

void AymMainFunc()
{
    DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
    pGetFileName = (PVOID)(BaseAddr + 0x222ad);
    DetourTransactionBegin();
    DetourAttach((void**)&pGetFileName, GetFileName);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"ERROR", L"Lum", MB_OK);
    }
}

static void make_console()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    cout << "Debug mode\n" << endl;
}

////////////中文字符集////////////////////////////////////////////////////////

#define GBK_CODE_PAGE 932

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = ANSI_CHARSET;
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}


//貌似Win10中 CreateFontA 不再通过CreateFontIndirectA实现，所以单独Hook
PVOID g_pOldCreateFontA = CreateFontA;
typedef int (WINAPI* PfuncCreateFontA)(int nHeight,
    int nWidth,
    int nEscapement,
    int nOrientation,
    int fnWeight,
    DWORD fdwltalic,
    DWORD fdwUnderline,
    DWORD fdwStrikeOut,
    DWORD fdwCharSet,
    DWORD fdwOutputPrecision,
    DWORD fdwClipPrecision,
    DWORD fdwQuality,
    DWORD fdwPitchAndFamily,
    LPCTSTR lpszFace);
int WINAPI NewCreateFontA(int nHeight,
    int nWidth,
    int nEscapement,
    int nOrientation,
    int fnWeight,
    DWORD fdwltalic,
    DWORD fdwUnderline,
    DWORD fdwStrikeOut,
    DWORD fdwCharSet,
    DWORD fdwOutputPrecision,
    DWORD fdwClipPrecision,
    DWORD fdwQuality,
    DWORD fdwPitchAndFamily,
    LPCTSTR lpszFace)
{
    fdwCharSet = ANSI_CHARSET;
    return ((PfuncCreateFontA)g_pOldCreateFontA)(nHeight,
        nWidth,
        nEscapement,
        nOrientation,
        fnWeight,
        fdwltalic,
        fdwUnderline,
        fdwStrikeOut,
        fdwCharSet,
        fdwOutputPrecision,
        fdwClipPrecision,
        fdwQuality,
        fdwPitchAndFamily,
        lpszFace);
}

PVOID g_pOldEnumFontFamiliesExA = EnumFontFamiliesExA;
typedef int (WINAPI* PfuncEnumFontFamiliesExA)(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags);
int WINAPI NewEnumFontFamiliesExA(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags)
{
    lpLogfont->lfCharSet = GB2312_CHARSET;
    lpLogfont->lfFaceName[0] = '\0';
    return ((PfuncEnumFontFamiliesExA)g_pOldEnumFontFamiliesExA)(hdc, lpLogfont, lpEnumFontFamExProc, lParam, dwFlags);
}

//开启等宽字体

//00405E29 | > \6A 3C               push    0x3C
//00405E2B | .  8D45 C0             lea     eax, [local.16]
//00405E2E | .  6A 00               push    0x0
//00405E30 | .  50                  push    eax
//00405E31      C605 F82A5800 00    mov     byte ptr[0x582AF8], 0x0;  这里改成0，等宽字体
//00405E38 | .E8 03E70C00         call    004D4540
//00405E3D | .  8B15 04045800       mov     edx, dword ptr[0x580404]
//00405E43 | .  83C4 0C             add     esp, 0xC
//00405E46 | .  6A 00               push    0x0; / Flags = 0x0
//00405E48 | .  68 F82A5800         push    00582AF8; | lParam = 0x582AF8
//00405E4D | .  68 205D4000         push    <EnumFontFamExProc>; | Callback = <rendezvo.EnumFontFamExProc>
//00405E52 | .  8D4D C0             lea     ecx, [local.16]; |
//00405E55 | .  51                  push    ecx; | pLogfont
//00405E56 | .  52                  push    edx; | / hWnd = > 001B0E04('［罪之光 -|- Rendezvous］', class = 'Rendezvous - minori')
//00405E57 | .C645 D7 80          mov     byte ptr[ebp - 0x29], 0x80; ||
//00405E5B | .  66:C745 DB 0000     mov     word ptr[ebp - 0x25], 0x0; ||
//00405E61 | .FF15 68225000       call    dword ptr[<&USER32.GetDC>]; | \GetDC
//00405E67 | .  50                  push    eax; | hDC
//00405E68 | .FF15 20205000       call    dword ptr[<&GDI32.EnumFontFamiliesExA>]; \EnumFontFamiliesExA




//////////////////////////////////////////////////////////////////////////

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
    DWORD dwExStyle,
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam);
HWND WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    const char* szWndName = "罪之光Rendezvous（深柑blossom）【胖次汉化组】";

    HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    //SetWindowDisplayAffinity(hw, WDA_MONITOR);
    return hw;
}


typedef BOOL(WINAPI* PfuncSetWindowTextA)(HWND hwnd, LPCSTR lpString);
PfuncSetWindowTextA g_pOldSetWindowTextA = SetWindowTextA;
BOOL WINAPI NewSetWindowTextA(HWND hwnd, LPCSTR lpString)
{
    const char* szWndName = "罪之光Rendezvous（深柑blossom）【胖次汉化组】";
    const char* szOldName = "\x83\x67\x83\x8A\x83\x6D\x83\x89\x83\x43\x83\x93\x00";

    //if (strstr(lpString, "その日の獣には、"))
  //  {
        lpString = szWndName;
   // }
    return g_pOldSetWindowTextA(hwnd, lpString);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void Init()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
    DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
    DetourAttach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
    DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
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
        make_console();
        AymMainFunc();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

