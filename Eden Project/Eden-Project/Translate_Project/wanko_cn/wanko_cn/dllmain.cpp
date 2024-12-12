// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

uint16_t SjisUnicodeTable[65536];
void init_fnt_table()
{
    auto fs = fopen("font.tbl", "rb");
    if (!fs)
    {
        MessageBox(0, L"Can't find font table!", 0, 0);
        //LOGERROR("Can't find font table!");
        exit(-1);
        //return;
    }
    fread(SjisUnicodeTable, 1, 65536 * 2, fs);
    fclose(fs);
}

PVOID AddrOfCWEW = CreateWindowExW;
typedef HWND(WINAPI* pfCreateWindowExW)(
    DWORD     dwExStyle,
    LPCWSTR   lpClassName,
    LPCWSTR   lpWindowName,
    DWORD     dwStyle,
    int       X,
    int       Y,
    int       nWidth,
    int       nHeight,
    HWND      hWndParent,
    HMENU     hMenu,
    HINSTANCE hInstance,
    LPVOID    lpParam
    );
HWND WINAPI MyCreateWindowExW(DWORD     dwExStyle,
    LPCWSTR   lpClassName,
    LPCWSTR   lpWindowName,
    DWORD     dwStyle,
    int       X,
    int       Y,
    int       nWidth,
    int       nHeight,
    HWND      hWndParent,
    HMENU     hMenu,
    HINSTANCE hInstance,
    LPVOID    lpParam)
{
    const wchar_t* Title = L"汪酱出嫁～欢迎光临！狗尾巴 - v1.0";
    HWND ret = ((pfCreateWindowExW)AddrOfCWEW)(dwExStyle, lpClassName, Title, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    return ret;
}

PVOID g_pOldGetGlyphOutlineW = NULL;
typedef int(WINAPI* PfuncGetGlyphOutlineW)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2);
int WINAPI NewGetGlyphOutlineW(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
    if (SjisUnicodeTable[uChar] != 0)
        uChar = SjisUnicodeTable[uChar];

    wchar_t* fn = (wchar_t* )L"SimHei";
    DWORD CharSet = GB2312_CHARSET;
    if (LOWORD(uChar) == (WORD)0x2468)
    {
        uChar = 0x266A;
        fn = (wchar_t*)L"MS Gothic";
        CharSet = SHIFTJIS_CHARSET;
    }
    else if (LOWORD(uChar) == (WORD)0x2467)
    {
        uChar = 0x2200;
        fn = (wchar_t*)L"MS Gothic";
        CharSet = SHIFTJIS_CHARSET;
    }

    LOGFONTW newlogfont;
    LOGFONTW oldlogfont;
    HFONT newhFont;
    HFONT oldhFont;
    newlogfont.lfHeight = 24;
    newlogfont.lfWidth = 0;
    newlogfont.lfWeight = 0;
    newlogfont.lfEscapement = 0;
    newlogfont.lfOrientation = 0;
    newlogfont.lfItalic = 0;
    newlogfont.lfUnderline = 0;
    newlogfont.lfStrikeOut = 0;
    newlogfont.lfCharSet = CharSet;
    newlogfont.lfOutPrecision = 0;
    newlogfont.lfClipPrecision = 0;
    newlogfont.lfQuality = 0;
    newlogfont.lfPitchAndFamily = 0;
    wsprintf(newlogfont.lfFaceName, L"%s", fn);
    newhFont = CreateFontIndirectW(&newlogfont);
    oldhFont = (HFONT)SelectObject(hdc, newhFont);
    GetObjectW(oldhFont, sizeof(LOGFONTW), &oldlogfont);
    wsprintf(oldlogfont.lfFaceName, L"%s", fn);
    oldhFont = CreateFontIndirectW(&oldlogfont);
    //newhFont = (HFONT)SelectObject(hdc, oldhFont);
    DeleteObject(newhFont);

    return ((PfuncGetGlyphOutlineW)g_pOldGetGlyphOutlineW)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
    _In_      UINT   CodePage,
    _In_      DWORD  dwFlags,
    _In_      LPCSTR lpMultiByteStr,
    _In_      int    cbMultiByte,
    _Out_opt_ LPWSTR lpWideCharStr,
    _In_      int    cchWideChar);

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
    cout << lpMBS << endl;
    return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);;
}

void InitAPIHook()
{
    DetourTransactionBegin();
    //Window
    DetourAttach(&AddrOfCWEW, MyCreateWindowExW);
    //Font
    //DetourAttach(&HookCFIW, HookCreateFontIndirectW);
    //DetourAttach(&HookCFW, HookCreateFontW);
    g_pOldGetGlyphOutlineW = DetourFindFunction("GDI32.dll", "GetGlyphOutlineW");
    DetourAttach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);

    //g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
    //DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"ERROR", 0, MB_OK);
}

int (*sub_6767D0)(int a1);
int AymChangeCharSet(int a1)
{
    int ret = sub_6767D0(a1);
    if (ret == 932)
        return 936;
    return ret;
}

unsigned int (*sub_6B2470)(const char* Src, int SrcStrLength, int SrcCharCodeFormat, char* Dest, size_t BufferBytes, int DestCharCodeFormat);
unsigned int AymConvStringRoutine(const char* Src, int SrcStrLength, int SrcCharCodeFormat, char* Dest, size_t BufferBytes, int DestCharCodeFormat)
{
    if (Src != nullptr && Dest != nullptr && SrcCharCodeFormat == 936 && DestCharCodeFormat == 0x4b0)
    {
        size_t dest_i = 0;
        auto src_len = SrcStrLength < 0 ? strlen(Src) : SrcStrLength;
        for (int i = 0; i < src_len; dest_i += 2)
        {
            if (dest_i >= BufferBytes)
            {
                //LOGERROR("error not enough buffer!");
                MessageBox(0, L"error not enough buffer!", 0, 0);
                if (dest_i >= 2)
                {
                    *(wchar_t*)(Dest + dest_i - 2) = 0;
                }
                return dest_i;
            }

            auto c = (uint8_t)Src[i];
            if (c >= 0x81 && c <= 0x9f || c >= 0xe0 && c <= 0xfc)
            {
                auto real_c = *(uint16_t*)(Src + i);
                if (real_c == 0)
                {
                    //LOGERROR("error char: %x %x", c, Src[i + 1]);
                    cout << "error char:" << hex << c << "|" << Src[i + 1] << endl;
                }
                *(wchar_t*)(Dest + dest_i) = real_c;//SjisUnicodeTable[real_c];
                i += 2;
            }
            else if (c >= 0xa0 && c < 0xe0)
            {
                *(wchar_t*)(Dest + dest_i) = (wchar_t)(c + 0xfec0);
                i++;
            }
            else
            {
                *(wchar_t*)(Dest + dest_i) = (wchar_t)c;
                i++;
            }
        }
        if (dest_i < BufferBytes)
        {
            *(wchar_t*)(Dest + dest_i) = 0;
            dest_i += 2;
        }
        return dest_i;
    }
    return sub_6B2470(Src, SrcStrLength, SrcCharCodeFormat, Dest, BufferBytes, DestCharCodeFormat);
}
void InitInlineHook()
{
    DetourTransactionBegin();
    //CharSet
    *(DWORD*)&sub_6767D0 = BaseAddr + 0x2767D0;
    cout << hex << sub_6767D0 << endl;
    DetourAttach((void**)&sub_6767D0, AymChangeCharSet);
    //MapStr
    *(DWORD*)&sub_6B2470 = BaseAddr + 0x2B2470;
    cout << hex << sub_6B2470 << endl;
    DetourAttach((void**)&sub_6B2470, AymConvStringRoutine);

    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"ERROR2", L"Aym", MB_OK);

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
        init_fnt_table();
        InitAPIHook();
        InitInlineHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

