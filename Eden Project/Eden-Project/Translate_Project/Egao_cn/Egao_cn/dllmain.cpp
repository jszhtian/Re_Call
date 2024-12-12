// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

char* TitleName = (char*)"ROOM NO.9 - v1.5 | 【三辉汉化组】2020.8.2  严谨商业用途，仅供学习交流，禁止直播及各种形式发至外网";

PVOID pTitle = (PVOID)(BaseAddr + 0x47C8B);
PVOID pTitleOut = (PVOID)(BaseAddr + 0x47C90);
__declspec(naked) void Title()
{
    _asm
    {
        mov eax, TitleName
        push eax
        push 0
        push 0xC
        jmp pTitleOut
    }
}
static char* wtocGBK(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

static LPWSTR ctowJIS(char* str)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
    return out;
}

typedef int (WINAPI* fnMessageboxA)(
    _In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType
    );
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType)
{
    char* Pstr = wtocGBK(ctowJIS((char*)lpText));
    char* Pstr2 = wtocGBK(ctowJIS((char*)lpCaption));
    return MessageBoxAOLD(hWnd, Pstr, Pstr2, uType);
}

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    lstrcpyA(lplf->lfFaceName, "黑体");
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
    return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)TitleName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void Init()
{
    DetourTransactionBegin();
    //DetourAttach((void**)&pTitle, Title);
    DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);

    MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
    DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);

    g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
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
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
__declspec(dllexport)void WINAPI Dummy()
{
}
