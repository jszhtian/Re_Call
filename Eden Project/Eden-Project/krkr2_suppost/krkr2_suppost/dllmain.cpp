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

LPWSTR ctowUTF(char* str)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
    return out;
}
LPCTSTR Name = NULL;
DWORD DataOffset = 0;
DWORD RawSize = 0;
char* Dir = (char*)"ProjectDir\\";
PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    strcpy(lplf->lfFaceName, "黑体");
    //strcpy(lplf->lfFaceName, "楷体");
    //lplf->lfCharSet = ANSI_CHARSET;
    //lplf->lfCharSet = SHIFTJIS_CHARSET;

    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}
static int(WINAPI* OldMultiByteToWideChar)(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar) = MultiByteToWideChar;
static int(WINAPI* OldWideCharToMultiByte)(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar) = WideCharToMultiByte;
int WINAPI newMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
    __asm
    {
        pushfd
        pushad
    }

    int ret = 0;
    ret = OldMultiByteToWideChar(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
    if (Name && DataOffset!=0)
    {
        char* FileName = wtoc(Name + 0x1);
        LOG << "---------------------------------------------------------------------------------------------------" << endl;
        LOG << FileName << endl;
        LOG << wtocUTF(lpWCS) << endl;
        LOG << "---------------------------------------------------------------------------------------------------" << endl;
    }
    __asm
    {
        popad
        popfd
    }
    return ret;
}


PVOID g_pOldSetWindowTextA = NULL;
typedef int (WINAPI* PfuncSetWindowTextA)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextA(HWND hwnd, LPCTSTR lpString)
{
    strcpy((char*)(LPCTSTR)lpString, "Clover Day's");
    return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hwnd, lpString);
}

DWORD OutSize = 0;
void __stdcall ReplaceOutFile(DWORD Offset)
{
    if (Name)
    {
        char* FileName = wtoc(Name+0x1);
        //std::cout << "File Name:\n\t" << FileName << std::endl;
        //std::cout << "File Offset:\n\t0x" << std::hex << Offset << std::endl;
        
        std::cout << "Try to open file:" << FileName << std::endl;
        FILE* f = fopen(FileName, "rb");
        if (f == NULL)
        {
            std::cout << "failed" << std::endl;
            Name = NULL;
            goto retn;
        }
        std::cout << "success" << std::endl;
        fseek(f, 0, SEEK_END);
        DWORD msize = ftell(f);
        fseek(f, 0, SEEK_SET);
        void* data = malloc(msize);
        fread(data, msize, 1, f);
        fclose(f);
        memcpy((void*)Offset, data, msize);
        Name = NULL;
        OutSize = msize;
        goto retn;
    }
retn:
    return;
}

PVOID pGetName = (PVOID)(BaseAddr + 0x20B392);
__declspec(naked)void GetName()
{
    __asm
    {
        pushad
            pushfd
                mov Name, esi
            popfd
        popad
        jmp pGetName
    }
}

PVOID pDataOffset = (PVOID)(BaseAddr + 0x203781);
__declspec(naked)void GetDataOffset()
{
    __asm
    {
        pushad
            pushfd
                mov DataOffset, eax
            popfd
        popad
        jmp pDataOffset
    }
}

PVOID pGetRawDataSize = (PVOID)(BaseAddr + 0x2037AC);
__declspec(naked)void GetRawDataSize()
{
    __asm
    {
        mov edx, OutSize
        mov edi, OutSize
        mov dword ptr ss : [ebp - 0x28], edx
        jmp pGetRawDataSize
    }
}

PVOID pDataOffsetNext = (PVOID)(BaseAddr + 0x203786);
__declspec(naked)void GetDataOffsetNext()
{
    __asm
    {
        pushad
        pushfd
        push DataOffset
        call ReplaceOutFile
        popfd
        popad
        jmp pDataOffsetNext
    }
}

void SetHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    g_pOldSetWindowTextA = DetourFindFunction("USER32.dll", "SetWindowTextA");
   // DetourAttach(&g_pOldSetWindowTextA, NewSetWindowTextA);
    g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
    //DetourAttach(&(PVOID&)OldMultiByteToWideChar, newMultiByteToWideChar);
    //DetourAttach(&(PVOID&)OldWideCharToMultiByte, newWideCharToMultiByte);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(0, L"Hook1 Error.", L"Attention", 0);
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((void**)&pGetName, GetName);
    DetourAttach((void**)&pDataOffset, GetDataOffset);
    DetourAttach((void**)&pDataOffsetNext, GetDataOffsetNext);
    DetourAttach((void**)&pGetRawDataSize, GetRawDataSize);
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

