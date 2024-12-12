// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#define TITLE  "【花火汉化组】公主与秽欲祭品 - v1.0 [BuildTime: 20220713]"

struct ExtFileHandleInfo
{
    uint32_t isSubFile;
    HANDLE file;
    uint32_t filePos;
    uint32_t size;
};

struct ExtFileInfo
{
    uint32_t isMem;
    void* buff;
    uint32_t unk;
    uint32_t size;
    ExtFileHandleInfo* handleInfo;
};

ExtFileInfo*(CDECL* sub_510680)(CHAR*);
ExtFileInfo* CDECL  HookOpenFile(CHAR* FileName)
{
    ExtFileInfo* FileObject = sub_510680(FileName);

    cout << FileName << endl;
    auto fnm = "#Project\\" + string(FileName);
    FILE* fp = _wfopen(ctow((CHAR*)fnm.c_str(), 932), L"rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        DWORD FileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        HGLOBAL FileGlobalAllocInfo = GlobalAlloc(GPTR, FileSize);
        if(!FileGlobalAllocInfo)
        {
            MessageBox(NULL, L"Alloc Memory Error 1.", L"HookOpenFile", MB_OK | MB_ICONERROR);
            return FileObject;
        }
        void* FileBuff = GlobalLock(FileGlobalAllocInfo);
        if (!FileBuff)
        {
            MessageBox(NULL, L"Alloc Memory Error 2.", L"HookOpenFile", MB_OK | MB_ICONERROR);
            return FileObject;
        }
        fread(FileBuff, 1, FileSize, fp);
        fclose(fp);

        FileObject->buff = FileBuff;
        FileObject->size = FileSize;
    }
    return FileObject;
}

void InlineInit()
{
    *(DWORD*)&sub_510680 = BaseAddr + 0x110680;
    DetourTransactionBegin();
    DetourAttach((void**)&sub_510680, HookOpenFile);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBox(NULL, L"InlinePatch Error.", L"InlinePatch", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}



PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    //wchar_t* wcs = nullptr;
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
HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    // 2: [esp+8] 007750A0 "ESCUDE_GAMES_FRAME"
    if (!strcmp((CHAR*)lpClassName, "ESCUDE_GAMES_FRAME"))
    {
        return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)TITLE, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }
    return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldSetWindowTextA = SetWindowTextA;
typedef bool (WINAPI* PfuncSetWindowTextA)(HWND hWnd, LPCSTR lpString);
bool WINAPI HookSetWindowTextA(HWND hw, LPCSTR lpString)
{
    //cout << lpString << endl;
    if(!lstrcmpA(lpString,"昉偲鈗梸偺僒僋儕僼傽僀僗"))
        return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hw, TITLE);
    return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hw, lpString);
}

void APIInit()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
    DetourAttach(&g_pOldCreateWindowExA, HookCreateWindowExA);
    DetourAttach(&g_pOldSetWindowTextA, HookSetWindowTextA);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBox(NULL, L"APIHook Error.", L"APIInit", MB_OK | MB_ICONERROR);
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
        make_console();
        InlineInit();
        APIInit();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

