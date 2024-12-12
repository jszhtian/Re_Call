// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

//API Hook
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
    //0E50FC14   0E37FC90  |WindowName = "お家に帰るまでがましまろです"
    if(!memcmp(lpWindowName,"お家に帰るまでがましまろです",29))
    {
        strcpy((char*)(LPCTSTR)lpWindowName, "【绿茶汉化组】回家前的棉花糖 - v1.0");
    }

    HWND hw = ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    //SetWindowDisplayAffinity(hw, WDA_MONITOR);
    return hw;
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    lplf->lfHeight *= 0.65;
    strcpy(lplf->lfFaceName, "黑体");
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

void __fastcall APIHook()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
    DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"Can't set API hook", 0, MB_OK);
}
//Replace xml buffer
static char* Buff;
static DWORD XMLSize;
void __stdcall AymDumpFile()
{
    char* p1 = Buff;
    const char* p2 = g_xml;
    while (*p2 != '\0')
    {
        if (*p2 == '\n')
        {
            *p1++ = '\r';
        }
        *p1++ = *p2++;
    }
    *p1++ = '\0';

    //size = p1 - Buff;
    XMLSize = p1 - Buff;
}

PVOID pGetBuff = NULL;
PVOID pDump = NULL;//(PVOID)(0x10021C4D);
__declspec(naked)void GetBuff()
{
    __asm
    {
        pushad;
        pushfd;
        mov Buff, eax;
        popfd;
        popad;
        jmp pGetBuff;
    }
}

__declspec(naked)void Dump()
{
    __asm
    {
        pushad;
        pushfd;
        call AymDumpFile;
        popfd;
        popad;
        mov eax, XMLSize;
        jmp pDump;
    }
}

void __fastcall ReplaceXMLBuffer()
{
    DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
    pGetBuff = (PVOID)(BaseAddr + 0x36FAFF);//mashimaro
    pDump = (PVOID)(BaseAddr + 0x36FB0F);
    DetourTransactionBegin();
    DetourAttach((void**)&pGetBuff, GetBuff);
    DetourAttach((void**)&pDump, Dump);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Can't set hook",0, MB_OK);
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
        APIHook();
        ReplaceXMLBuffer();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

