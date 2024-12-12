// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

#define RELEASE_MODE


struct PicInfo {
    BOOL IsRead;
    LONG Width;
    LONG Height;
    WORD Bpp;
    BYTE* DIB;
};
PicInfo PI;

void WINAPI InitPic(char* FileName)
{
    if (FileName)
    {
        char fnm[MAX_PATH] = { 0 };
        strcpy(fnm, "_pic\\");
        strcat(fnm, FileName);
        strcat(fnm, ".bmp");

        FILE* fp = fopen(fnm, "rb");
        if (!fp)
            return;

        fseek(fp, 0, SEEK_SET);
        BITMAPFILEHEADER bmf;
        BITMAPINFOHEADER bmi;
        fread(&bmf, sizeof(BITMAPFILEHEADER), 1, fp);
        fread(&bmi, sizeof(BITMAPINFOHEADER), 1, fp);

        DWORD DIBSize = bmf.bfSize - bmf.bfOffBits;
        BYTE* DIB = new BYTE[DIBSize];
        fread(DIB, DIBSize, 1, fp);
        fclose(fp);

        PI.IsRead = TRUE;
        PI.Width = bmi.biWidth;
        PI.Height = abs(bmi.biHeight);
        PI.Bpp = bmi.biBitCount;
        PI.DIB = DIB;

#ifdef DEBUG_MODE
        cout << "--------------------------------------" << endl;
        cout << "FileName:" << fnm << endl;
        cout << "Width:" << PI.Width << endl;
        cout << "Height:" << PI.Height << endl;
        cout << "Bpp:" << PI.Bpp << endl;
#endif
    }
}

PVOID pInitPic = (PVOID)(BaseAddr + 0x35E1D);
_declspec(naked) void gInitPic()
{
    _asm
    {
        pushad;
        pushfd;
        push eax;
        call InitPic;
        popfd;
        popad;
        jmp pInitPic;
    }
}

void WINAPI BppCheck(DWORD pOldBpp)
{
    if (PI.IsRead && pOldBpp != PI.Bpp)
    {
        MessageBox(0, L"Bpp Error", L"BppCheck", 0);
#ifdef DEBUG_MODE
        cout << "OldBpp:" << pOldBpp << endl;
        cout << "NewBpp:" << PI.Bpp << endl;
#endif
        PI.IsRead = FALSE;
        PI.Width = NULL;
        PI.Height = NULL;
        PI.Bpp = NULL;
        PI.DIB = nullptr;
    }
}

PVOID pBppCheck = (PVOID)(BaseAddr + 0x35FB1);
_declspec(naked) void gBppCheck()
{
    _asm
    {
        pushad;
        pushfd;
        push [ebx + 0x1C];
        call BppCheck;
        popfd;
        popad;
        jmp pBppCheck;
    }
}

void WINAPI CopyDib(BYTE* pOldDIB)
{
    if (PI.IsRead)
    {
        PI.IsRead = FALSE;

        DWORD Len = 0;
        if (PI.Bpp == 32)
            Len = PI.Width * PI.Height * 4;
        else if(PI.Bpp == 24)
            Len = PI.Width * PI.Height * 3;
        else if (PI.Bpp == 8)
            Len = PI.Width * PI.Height + 1024;
        else
        {
            MessageBox(0, L"Uncupport Bpp Mode", 0, 0);
            return;
        }

        memcpy(pOldDIB, PI.DIB, Len);

        delete[] PI.DIB;

        PI.Width = NULL;
        PI.Height = NULL;
        PI.Bpp = NULL;
        PI.DIB = nullptr;
#ifdef DEBUG_MODE
        cout << "DIB Copied" << endl;
#endif
    }
}

PVOID pCopyDib = (PVOID)(BaseAddr + 0x3603F);
_declspec(naked) void gCopyDib()
{
    _asm
    {
        pushad;
        pushfd;
        push esi;
        call CopyDib;
        popfd;
        popad;
        jmp pCopyDib;
    }
}

void WINAPI GetScriptName(char* fnm)
{
    cout << "--------------------------------------" << endl;
    cout << "当前脚本：" << fnm << ".scw" << endl;
}

PVOID pGetScriptName = (PVOID)(BaseAddr + 0x1A1BF);
_declspec(naked) void gGetScriptName()
{
    _asm
    {
        pushad;
        pushfd;
        push edi;
        call GetScriptName;
        popfd;
        popad;
        jmp pGetScriptName;
    }
}

void WINAPI SetPicHook()
{
    DetourTransactionBegin();
    DetourAttach((void**)&pInitPic, gInitPic);
    DetourAttach((void**)&pBppCheck, gBppCheck);
    DetourAttach((void**)&pCopyDib, gCopyDib);

#ifdef DEBUG_MODE
    DetourAttach((void**)&pGetScriptName, gGetScriptName);
#endif
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"SetPicHook Error", L"SetPicHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}

/*********************************************************************************************************************/

typedef int (WINAPI* fnEnumFontFamiliesExA)(
    HDC           hdc,
    LPLOGFONTA    lpLogfont,
    FONTENUMPROCA lpProc,
    LPARAM        lParam,
    DWORD         dwFlags
    );
fnEnumFontFamiliesExA pEnumFontFamiliesExA;
int WINAPI HookEnumFontFamiliesExA(
    HDC           hdc,
    LPLOGFONTA    lpLogfont,
    FONTENUMPROCA lpProc,
    LPARAM        lParam,
    DWORD         dwFlags
)
{
    lpLogfont->lfCharSet = GB2312_CHARSET;
    return ((fnEnumFontFamiliesExA)pEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

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
    const char* szWndName = "【颜月汉化组】无颜之月 - v1.0 [BuildTime: 220614]";

    HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    return hw;
}

void WINAPI APIHook()
{
    pEnumFontFamiliesExA = (fnEnumFontFamiliesExA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "EnumFontFamiliesExA");
    DetourTransactionBegin();
    DetourAttach((void**)&pEnumFontFamiliesExA, HookEnumFontFamiliesExA);
    DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
    DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBoxW(NULL, L"API Function Hook Error.", L"APIHook", MB_OK | MB_ICONERROR);
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
#ifdef DEBUG_MODE
        make_console();
#endif
        APIHook();
        SetPicHook();
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