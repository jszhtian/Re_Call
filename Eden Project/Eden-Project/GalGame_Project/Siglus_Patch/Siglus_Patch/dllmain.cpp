// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#define WorkPath L"_Einstein_WorkPath\\"
#define G00WorkPath L"_Einstein_WorkPath\\g00\\"
//g00 out reader support
WCHAR BaseDir[MAX_PATH] = { 0 };

PVOID g_pOldCreateFileW = CreateFileW;
typedef HANDLE(WINAPI* PfuncCreateFileW)(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    );
HANDLE WINAPI HookCreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    if(!wcsstr(lpFileName, BaseDir))
        return ((PfuncCreateFileW)(g_pOldCreateFileW))(
            lpFileName,
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile);
    wstring strOldName(lpFileName);
    wstring strName = strOldName.substr(strOldName.find_last_of(L"\\") + 1);
    wstring strNameAfter = strName.substr(strName.find_last_of(L"."));
    cout << wtoc(lpFileName) << endl;
    auto IsFileEx = [](wstring FileName) -> BOOL
    {
        if (GetFileAttributesW(FileName.c_str()) == INVALID_FILE_ATTRIBUTES)
            return FALSE;
        return TRUE;
    };

    //G00

    if (strNameAfter == L".G00" or strNameAfter == L".g00")
    {
        wstring Path = G00WorkPath + strName;
        if (!IsFileEx(Path))
            goto OLD_RETURN;
        lpFileName = Path.c_str();
    }

    //Scene.pck
    if (strName == L"Scene.pck")
    {
        wstring Path = WorkPath + strName;
        if (!IsFileEx(Path))
            goto OLD_RETURN;
        lpFileName = Path.c_str();
    }

    //Gameexe.dat
    else if (strName == L"Gameexe.dat")
    {
        wstring Path = WorkPath + strName;
        if (!IsFileEx(Path))
            goto OLD_RETURN;
        lpFileName = Path.c_str();
    }

    //op2.wmv
    /*
    else if (strName == L"op2.wmv")
    {
        wstring Movie = L"op2.mp4";
        wstring Path = WorkPath + Movie;
        if (!IsFileEx(Path))
            goto OLD_RETURN;
        lpFileName = Path.c_str();
    }
    */

OLD_RETURN:
    //cout << wtoc(lpFileName) << endl;
    return ((PfuncCreateFileW)(g_pOldCreateFileW))(
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile);
}

void FASTCALL g00OutReader()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&g_pOldCreateFileW, HookCreateFileW);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(0, L"g00OutReader set error.", L"g00OutReader", 0);
}
//Loacale Check
char (*sub_62CB20)();
char ByppassLocaleCheckMain()
{
    return 1;
}

void FASTCALL BypassLocaleCheck()
{
    *(DWORD*)&sub_62CB20 = BaseAddr + 0x22CB20;
    DetourTransactionBegin();
    DetourAttach((void**)&sub_62CB20, ByppassLocaleCheckMain);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(0, L"Bypass Locale Check error.", L"BypassLocaleCheck", 0);
}

//Font and other
//Useless
typedef HFONT(WINAPI* fnCreateFontIndirectW)(LOGFONTW* lplf);
fnCreateFontIndirectW pCreateFontIndirectW;

HFONT WINAPI newCreateFontIndirectW(LOGFONTW* lplf)
{
    LOGFONTW lf;
    memcpy(&lf, lplf, sizeof(LOGFONTW));
    if (lf.lfCharSet == SHIFTJIS_CHARSET)
    {
        lf.lfCharSet = DEFAULT_CHARSET;

        wcscpy(lf.lfFaceName, L"黑体");
    }

    //wcscpy(lf.lfFaceName, L"Font");

    return pCreateFontIndirectW(&lf);
}

typedef BOOL (WINAPI* fnGetTextMetrics)(
    HDC hdc,
    LPTEXTMETRIC lptm
);
fnGetTextMetrics pGetTextMetrics;

BOOL WINAPI HookGetTextMetrics(
    HDC hdc,
    LPTEXTMETRIC lptm
)
{
    cout << "----------------INFO----------------" << endl;
    cout << "tmCharSet:" << (unsigned int)lptm->tmCharSet << endl;
    cout << "tmFirstChar:" << lptm->tmFirstChar << endl;
    cout << "tmLastChar:" << lptm->tmLastChar << endl;
    cout << "tmAveCharWidth:" << (unsigned int)lptm->tmAveCharWidth << endl;
    cout << "tmMaxCharWidth:" << (unsigned int)lptm->tmMaxCharWidth << endl;
    cout << "----------------INFO----------------" << endl;
    return pGetTextMetrics(hdc, lptm);
}

void FASTCALL OtherHook()
{
    pCreateFontIndirectW = (fnCreateFontIndirectW)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontIndirectW");
    pGetTextMetrics = (fnGetTextMetrics)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "GetTextMetricsW");
    DetourTransactionBegin();
    DetourAttach((void**)&pCreateFontIndirectW, newCreateFontIndirectW);
    //DetourAttach((void**)&pGetTextMetrics, HookGetTextMetrics);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(0, L"CreateFontIndirectW Error.", L"OtherHook", 0);
}

void FASTCALL InitAllFunc()
{
    make_console();

    GetCurrentDirectoryW(MAX_PATH, BaseDir);
    BypassLocaleCheck();
    g00OutReader();
    OtherHook();

}

void FASTCALL InitAllPatch()
{
    BYTE CharSet[] = { 0x86 };
    BYTE SetLocale[] = { 'C','\x00', 
                         'h','\x00', 
                         'i','\x00',
                         'n','\x00',
                         'e','\x00',
                         's','\x00',
                         'e','\x00',
                         '\x00','\x00' };//Unicode string
    mmpatch((void*)(BaseAddr + 0x2b1933), CharSet, sizeof(CharSet));//FontCharset
    mmpatch((void*)(BaseAddr + 0x701290), SetLocale, sizeof(SetLocale));//_wsetlocale
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        InitAllFunc();
        InitAllPatch();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

