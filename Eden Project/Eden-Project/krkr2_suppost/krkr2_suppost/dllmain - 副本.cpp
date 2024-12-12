// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
ofstream LOG("LOG.txt");
char* wtoc(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE);
    char* bbb = new char[dwMinSize];
    WideCharToMultiByte(CP_ACP, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
    return bbb;
}
char* wtocUTF(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

char* wtocGBK(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

char* wtocJIS(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(932, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(932, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

LPWSTR ctowUTF(char* str)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
    return out;
}

LPWSTR ctowGBK(char* str)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(936, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(936, 0, str, -1, out, dwMinSize);//转换
    return out;
}

LPWSTR ctowJIS(char* str)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
    return out;
}
LPCTSTR Name = NULL;

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;

    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

int memstr(char* oldmem, DWORD old_size, DWORD newmem)
{
    for (DWORD i = 0; i < old_size; i++)
    {
        if (oldmem[i] == newmem)
            return i;
    }
    return -1;
}

int memrender(char* mem, DWORD size)
{
    for (DWORD i = 0; i < size; i++)
    {
        if (0x9F > mem[i] > 0x7F || 0xFC > mem[i] > 0xE0)
            return 1;
    }
    return -1;
}

static int(WINAPI* OldMultiByteToWideChar)(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar) = MultiByteToWideChar;
int WINAPI newMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
    //wchar_t* tmp = ctowUTF(wtocJIS(lpWCS));
    //cout << sizeof(lpMBS) << endl;
    /*if (memstr((char*)lpMBS, ccWC, 0x7B) != -1 && memstr((char*)lpMBS, ccWC, 0x7D) != -1 &&
        memstr((char*)lpMBS, ccWC, 0x3D) != -1 && memstr((char*)lpMBS, ccWC, 0x22) != -1 &&
        memstr((char*)lpMBS, ccWC, 0x3B) != -1 && memstr((char*)lpMBS, ccWC, 0x5C) != -1 &&
        memstr((char*)lpMBS, ccWC, 0x01) == -1 && memstr((char*)lpMBS, ccWC, 0x02) == -1 ||
        memstr((char*)lpMBS, ccWC, 0x01) == -1 && memstr((char*)lpMBS, ccWC, 0x02) == -1 && memstr((char*)lpMBS, ccWC, 0x03) == -1 && memstr((char*)lpMBS, ccWC, 0x04) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x05) == -1 && memstr((char*)lpMBS, ccWC, 0x06) == -1 && memstr((char*)lpMBS, ccWC, 0x07) == -1 && memstr((char*)lpMBS, ccWC, 0x08) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x23) == -1 && memstr((char*)lpMBS, ccWC, 0x2C) == -1 ||
        memstr((char*)lpMBS, ccWC, 0x01) == -1 && memstr((char*)lpMBS, ccWC, 0x02) == -1 && memstr((char*)lpMBS, ccWC, 0x03) == -1 && memstr((char*)lpMBS, ccWC, 0x04) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x05) == -1 && memstr((char*)lpMBS, ccWC, 0x06) == -1 && memstr((char*)lpMBS, ccWC, 0x07) == -1 && memstr((char*)lpMBS, ccWC, 0x08) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x2C) == -1)*/
    /*if(memstr((char*)lpMBS, ccWC, 0x01) == -1 && memstr((char*)lpMBS, ccWC, 0x02) == -1 && memstr((char*)lpMBS, ccWC, 0x03) == -1 && memstr((char*)lpMBS, ccWC, 0x04) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x05) == -1 && memstr((char*)lpMBS, ccWC, 0x06) == -1 && memstr((char*)lpMBS, ccWC, 0x07) == -1 && memstr((char*)lpMBS, ccWC, 0x08) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x09) == -1 && memstr((char*)lpMBS, ccWC, 0x0A) == -1 && memstr((char*)lpMBS, ccWC, 0x0B) == -1 && memstr((char*)lpMBS, ccWC, 0x0C) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x0D) == -1 && memstr((char*)lpMBS, ccWC, 0x0E) == -1 && memstr((char*)lpMBS, ccWC, 0x0F) == -1 && memstr((char*)lpMBS, ccWC, 0x10) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x11) == -1 && memstr((char*)lpMBS, ccWC, 0x12) == -1 && memstr((char*)lpMBS, ccWC, 0x13) == -1 && memstr((char*)lpMBS, ccWC, 0x14) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x15) == -1 && memstr((char*)lpMBS, ccWC, 0x16) == -1 && memstr((char*)lpMBS, ccWC, 0x17) == -1 && memstr((char*)lpMBS, ccWC, 0x18) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x19) == -1 && memstr((char*)lpMBS, ccWC, 0x1A) == -1 && memstr((char*)lpMBS, ccWC, 0x1B) == -1 && memstr((char*)lpMBS, ccWC, 0x1C) == -1 &&
        memstr((char*)lpMBS, ccWC, 0x1D) == -1 && memstr((char*)lpMBS, ccWC, 0x1E) == -1 && memstr((char*)lpMBS, ccWC, 0x1F) == -1)*/
    //cout << ccWC << endl;
    //if(memrender((char*)lpMBS, ccWC)==1)
    if(cp=0)
    {
        cout << "patched" << endl;
        cp = 932;
        LOG << "______________________________" << endl;
        LOG << lpMBS << endl;
    }
    return OldMultiByteToWideChar(932, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

PVOID g_pOldSetWindowTextA = NULL;
typedef int (WINAPI* PfuncSetWindowTextA)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextA(HWND hwnd, LPCTSTR lpString)
{
    strcpy((char*)(LPCTSTR)lpString, "Little Sick Girls ～镜中的偶像～ | kirikiroid2吧  中文化制作 | bbs.zdfx.net  修改");
    return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hwnd, lpString);
}

bool IsInject = false;
void __stdcall AymShowFileName(LPCTSTR name)
{
    //cout << wtoc(name + 0x1) << endl;
    
    if (!IsInject && wtoc(name + 0x1)=="appconfig.tjs")
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)OldMultiByteToWideChar, newMultiByteToWideChar);
        if (DetourTransactionCommit() != NOERROR)
        {
            MessageBox(0, L"Hook mb2wc Error.", L"Attention", 0);
        }
        IsInject = true;
    }

}

PVOID pGetName = (PVOID)(BaseAddr + 0x20B392);
__declspec(naked)void GetName()
{
    __asm
    {
        pushad
        pushfd
        push esi
        call AymShowFileName
        //mov Name, esi
        popfd
        popad
        jmp pGetName
    }
}

void __stdcall AymTjsFunc(char* tjs)
{
    tjs = wtocUTF(ctowJIS(tjs));
    //LOG << "______________________________" << endl;
    //LOG << tjs << endl;
    WCHAR bom = *(WCHAR*)tjs;
    if (bom == 0xFEFF)
        return;
    DWORD Size = strlen(tjs);
    WCHAR* WideBuffer = new WCHAR[(Size * 4)];
    memset(WideBuffer, 0, Size * 8);

    MultiByteToWideChar(932, 0, tjs, Size, &WideBuffer[1], Size * 4 - 1);

    WideBuffer[0] = 0xFEFF;
    _asm {
        mov eax, WideBuffer;
    }
}

PVOID pGetTjs = (PVOID)(BaseAddr + 0x203B23);
__declspec(naked)void GetTjs()
{
    __asm
    {
        pushad
        pushfd
        push eax
        call AymTjsFunc
        //mov Name, esi
        popfd
        popad
        jmp pGetTjs
    }
}

void SetHook()
{
    
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    g_pOldSetWindowTextA = DetourFindFunction("USER32.dll", "SetWindowTextA");
    //DetourAttach(&g_pOldSetWindowTextA, NewSetWindowTextA);
    g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(0, L"Hook1 Error.", L"Attention", 0);
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //DetourAttach((void**)&pGetName, GetName);
    DetourAttach((void**)&pGetTjs, GetTjs);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(0, L"Hook2 Error.", L"Attention", 0);
    }

}

static void make_console() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    std::cout << "Open Console Success!" << std::endl;
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
        SetHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

