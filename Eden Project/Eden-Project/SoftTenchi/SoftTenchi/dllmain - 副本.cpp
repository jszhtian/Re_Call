// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "crc32.h"
#include <iomanip>
#pragma comment(lib, "detours.lib")
using namespace std;
WNDPROC Func = NULL;

bool IsSuccess = false;
bool IsSuccess2 = false;
LRESULT CALLBACK MyWinBack(
    _In_ HWND   hwnd,
    _In_ UINT   uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    int result;
    if (!Func)
    {
        MessageBox(0, L"Can't find callback func.", L"Aym", 0);
        exit(-1);
    }
    if (uMsg == 1)
    {
        IsSuccess2 = true;
        result = Func(hwnd, 1045, 0, 0);
    }
    else
    {
        result = Func(hwnd, uMsg, wParam, lParam);
    }
    return result;
}


PVOID g_pOldRegisterClassExA = RegisterClassExA;
typedef ATOM (WINAPI* PfuncRegisterClassExA) (WNDCLASSEXA* Arg1);
ATOM WINAPI NewRegisterClassExA(WNDCLASSEXA* Arg1)
{
    if (Arg1 && Arg1->lpfnWndProc && !strcmp(Arg1->lpszClassName, "SDWRAP") && !IsSuccess)
    {
        Func = Arg1->lpfnWndProc;
        Arg1->lpfnWndProc = MyWinBack;
        IsSuccess = true;
        return ((PfuncRegisterClassExA)g_pOldRegisterClassExA)(Arg1);
    }

    return ((PfuncRegisterClassExA)g_pOldRegisterClassExA)(Arg1);
}

PVOID g_pOldCreateFileA = CreateFileA;
typedef HANDLE(WINAPI* PfuncCreateFileA)(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    );
/*
0019F1A8   00413AF0  |FileName = "D:\A_Workspace\BaiduNetdiskDownload\ニュートンと林檎の樹\setup\cs2.log"
0019F1AC   40000000  |Access = GENERIC_WRITE
0019F1B0   00000001  |ShareMode = FILE_SHARE_READ
0019F1B4   00000000  |pSecurity = NULL
0019F1B8   00000002  |Mode = CREATE_ALWAYS
0019F1BC   00000080  |Attributes = NORMAL
0019F1C0   00000000  \hTemplateFile = NULL

*/
char* WriteFileName = NULL;
HANDLE FileHand = NULL;
HANDLE WINAPI NewCreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    if (!lpFileName)
    {
        return INVALID_HANDLE_VALUE;
    }
    if (dwDesiredAccess == GENERIC_WRITE && dwShareMode == FILE_SHARE_READ && IsSuccess && IsSuccess2 && dwCreationDisposition == CREATE_ALWAYS)
    {
        string strOldName(lpFileName);
        string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
        string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
        WriteFileName = (char*)"dump.exe";
        FileHand = ((PfuncCreateFileA)(g_pOldCreateFileA))(
            lpFileName,
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile);
    }
    else
    {
        return ((PfuncCreateFileA)(g_pOldCreateFileA))(
            lpFileName,
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile);
    }
    return FileHand;
}

PVOID g_pOldWriteFile = WriteFile;
typedef BOOL(WINAPI* PfuncWriteFile) (
    HANDLE  hFile,//文件句柄
    LPCVOID lpBuffer,//数据缓存区指针
    DWORD   nNumberOfBytesToWrite,//你要写的字节数
    LPDWORD lpNumberOfBytesWritten,//用于保存实际写入字节数的存储区域的指针
    LPOVERLAPPED lpOverlapped//OVERLAPPED结构体指针
    );
BOOL WINAPI NewWriteFile(
    HANDLE  hFile,//文件句柄
    LPCVOID lpBuffer,//数据缓存区指针
    DWORD   nNumberOfBytesToWrite,//你要写的字节数
    LPDWORD lpNumberOfBytesWritten,//用于保存实际写入字节数的存储区域的指针
    LPOVERLAPPED lpOverlapped//OVERLAPPED结构体指针
)
{
    BOOL ret = ((PfuncWriteFile)g_pOldWriteFile)(
        hFile,//文件句柄
        lpBuffer,//数据缓存区指针
        nNumberOfBytesToWrite,//你要写的字节数
        lpNumberOfBytesWritten,//用于保存实际写入字节数的存储区域的指针
        lpOverlapped//OVERLAPPED结构体指针
        );
    if (IsSuccess && IsSuccess2 && hFile == FileHand)
    {
        cout <<"Create: " <<  WriteFileName << endl;
        FILE* fp = fopen(WriteFileName, "wb");
        fwrite(lpBuffer, nNumberOfBytesToWrite, 1, fp);
        fclose(fp);
        cout << "Create Success" << endl;
    }
    return ret;
}



static void make_console() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    cout << "Console\n" << endl;
}

void Init()
{
    make_console();
    DetourTransactionBegin();
    DetourAttach(&g_pOldRegisterClassExA, NewRegisterClassExA);
    DetourAttach((void**)&g_pOldCreateFileA, NewCreateFileA);
    DetourAttach((void**)&g_pOldWriteFile, NewWriteFile);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"ERROR", L"Aym", MB_OK);
    cout << "软电池通用破解工具。\n仅限终点论坛内使用，禁止一切形式的转载！\nCreate By AyamiKaze[SWIFT@zdfx]\n\n如果无法直接启动，请直接启动dump.exe。" << endl;
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

