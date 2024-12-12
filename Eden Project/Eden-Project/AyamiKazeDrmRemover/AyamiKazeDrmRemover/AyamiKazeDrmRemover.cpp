// AyamiKazeDrmRemover.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shellapi.h>
#include <iostream>

#include "resource.h"
#include "..\3rd\detours.h"
using namespace std;
////////////////////////////////////////////////////////////// Error Messages.
//
VOID AssertMessage(PCSTR szMsg, PCSTR szFile, DWORD nLine)
{
    printf("ASSERT(%s) failed in %s, line %d.", szMsg, szFile, nLine);
}

#define ASSERT(x)   \
do { if (!(x)) { AssertMessage(#x, __FILE__, __LINE__); DebugBreak(); }} while (0)
;


//////////////////////////////////////////////////////////////////////////////
//
static BOOLEAN  s_fRemove = FALSE;
static CHAR     s_szDllPath[MAX_PATH] = "AyamiKaze@zdfx.dll";

//////////////////////////////////////////////////////////////////////////////
//
//  This code verifies that the named DLL has been configured correctly
//  to be imported into the target process.  DLLs must export a function with
//  ordinal #1 so that the import table touch-up magic works.
//
static BOOL CALLBACK ExportCallback(PVOID pContext,
    ULONG nOrdinal,
    PCHAR pszSymbol,
    PVOID pbTarget)
{
    (void)pContext;
    (void)pbTarget;
    (void)pszSymbol;

    if (nOrdinal == 1) {
        *((BOOL*)pContext) = TRUE;
    }
    return TRUE;
}

BOOL DoesDllExportOrdinal1(PCHAR pszDllPath)
{
    HMODULE hDll = LoadLibraryExA(pszDllPath, NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (hDll == NULL) {
        printf("setdll.exe: LoadLibraryEx(%s) failed with error %d.\n",
            pszDllPath,
            GetLastError());
        return FALSE;
    }

    BOOL validFlag = FALSE;
    DetourEnumerateExports(hDll, &validFlag, ExportCallback);
    FreeLibrary(hDll);
    return validFlag;
}

//////////////////////////////////////////////////////////////////////////////
//
static BOOL CALLBACK ListBywayCallback(PVOID pContext,
    PCHAR pszFile,
    PCHAR* ppszOutFile)
{
    (void)pContext;

    *ppszOutFile = pszFile;
    /*
    if (pszFile) {
        printf("    %s\n", pszFile);
    }
    */
    return TRUE;
}

static BOOL CALLBACK ListFileCallback(PVOID pContext,
    PCHAR pszOrigFile,
    PCHAR pszFile,
    PCHAR* ppszOutFile)
{
    (void)pContext;

    *ppszOutFile = pszFile;
    //printf("    %s -> %s\n", pszOrigFile, pszFile);
    return TRUE;
}

static BOOL CALLBACK AddBywayCallback(PVOID pContext,
    PCHAR pszFile,
    PCHAR* ppszOutFile)
{
    PBOOL pbAddedDll = (PBOOL)pContext;
    if (!pszFile && !*pbAddedDll) {                     // Add new byway.
        *pbAddedDll = TRUE;
        *ppszOutFile = s_szDllPath;
    }
    return TRUE;
}

BOOL SetFile(PCHAR pszPath)
{
    BOOL bGood = TRUE;
    HANDLE hOld = INVALID_HANDLE_VALUE;
    HANDLE hNew = INVALID_HANDLE_VALUE;
    PDETOUR_BINARY pBinary = NULL;

    CHAR szOrg[MAX_PATH];
    CHAR szNew[MAX_PATH];
    CHAR szOld[MAX_PATH];

    szOld[0] = '\0';
    szNew[0] = '\0';

#ifdef _CRT_INSECURE_DEPRECATE
    strcpy_s(szOrg, sizeof(szOrg), pszPath);
    strcpy_s(szNew, sizeof(szNew), szOrg);
    strcat_s(szNew, sizeof(szNew), "#");
    strcpy_s(szOld, sizeof(szOld), szOrg);
    strcat_s(szOld, sizeof(szOld), ".old");
#else
    strcpy(szOrg, pszPath);
    strcpy(szNew, szOrg);
    strcat(szNew, "#");
    strcpy(szOld, szOrg);
    strcat(szOld, "~");
#endif
    //printf("  %s:\n", pszPath);

    hOld = CreateFileA(szOrg,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hOld == INVALID_HANDLE_VALUE) {
        printf("Couldn't open input file: %s, error: %d\n",
            szOrg, GetLastError());
        bGood = FALSE;
        goto end;
    }

    hNew = CreateFileA(szNew,
        GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hNew == INVALID_HANDLE_VALUE) {
        printf("Couldn't open output file: %s, error: %d\n",
            szNew, GetLastError());
        bGood = FALSE;
        goto end;
    }

    if ((pBinary = DetourBinaryOpen(hOld)) == NULL) {
        printf("DetourBinaryOpen failed: %d\n", GetLastError());
        goto end;
    }

    if (hOld != INVALID_HANDLE_VALUE) {
        CloseHandle(hOld);
        hOld = INVALID_HANDLE_VALUE;
    }

    {
        BOOL bAddedDll = FALSE;

        DetourBinaryResetImports(pBinary);

        if (!s_fRemove) {
            if (!DetourBinaryEditImports(pBinary,
                &bAddedDll,
                AddBywayCallback, NULL, NULL, NULL)) {
                printf("DetourBinaryEditImports failed: %d\n", GetLastError());
            }
        }

        if (!DetourBinaryEditImports(pBinary, NULL,
            ListBywayCallback, ListFileCallback,
            NULL, NULL)) {

            printf("DetourBinaryEditImports failed: %d\n", GetLastError());
        }

        if (!DetourBinaryWrite(pBinary, hNew)) {
            printf("DetourBinaryWrite failed: %d\n", GetLastError());
            bGood = FALSE;
        }

        DetourBinaryClose(pBinary);
        pBinary = NULL;

        if (hNew != INVALID_HANDLE_VALUE) {
            CloseHandle(hNew);
            hNew = INVALID_HANDLE_VALUE;
        }

        if (bGood) {
            if (!DeleteFileA(szOld)) {
                DWORD dwError = GetLastError();
                if (dwError != ERROR_FILE_NOT_FOUND) {
                    printf("Warning: Couldn't delete %s: %d\n", szOld, dwError);
                    bGood = FALSE;
                }
            }
            if (!MoveFileA(szOrg, szOld)) {
                printf("Error: Couldn't back up %s to %s: %d\n",
                    szOrg, szOld, GetLastError());
                bGood = FALSE;
            }
            if (!MoveFileA(szNew, szOrg)) {
                printf("Error: Couldn't install %s as %s: %d\n",
                    szNew, szOrg, GetLastError());
                bGood = FALSE;
            }
        }

        DeleteFileA(szNew);
    }


end:
    if (pBinary) {
        DetourBinaryClose(pBinary);
        pBinary = NULL;
    }
    if (hNew != INVALID_HANDLE_VALUE) {
        CloseHandle(hNew);
        hNew = INVALID_HANDLE_VALUE;
    }
    if (hOld != INVALID_HANDLE_VALUE) {
        CloseHandle(hOld);
        hOld = INVALID_HANDLE_VALUE;
    }
    return bGood;
}

void PrintMessage()
{
    cout << "---------------------信息---------------------" << endl;
    cout << "AyamiKaze Drm Remover Ver 0.2" << endl;
    cout << "From zdfx" << endl;
    cout << "仅供终点论坛内部使用，谢绝转载！" << endl;
    cout << "支持DRM：" << endl;
    cout << "\t1. 软电池（SDWRAP）" << endl;
    cout << "\t2. DMM GAME PLAYER" << endl;
    cout << "---------------------信息---------------------" << endl;
}

BOOL CheckDll()
{
    FILE* fp = fopen(s_szDllPath, "rb");
    if (!fp)
        return FALSE;
    fclose(fp);
    return TRUE;
}

#pragma pack (1)
struct AYMHeader {
    CHAR Magic[20];
    int Mode;
};
#pragma pack ()

void WriteAYMInfoFile(int Mode)
{
    AYMHeader header;
    strcpy(header.Magic, "AyamiKazeDrmInfo");
    header.Mode = Mode;
    FILE* fp = fopen("Info.aym", "wb");
    fwrite(&header, sizeof(AYMHeader), 1, fp);
    fclose(fp);
}

int main(int argc, char** argv)
{
    PrintMessage();
    if (argc != 2)
    {
        cout << "\r\n没有发现需要破解的程序，终止运行。" << endl;
        system("pause");
        return 0;
    }

    if (!CheckDll())
    {
        cout << "\r\n丢失AyamiKaze@zdfx.dll，终止运行。" << endl;
        system("pause");
        return 0;
    }

    CHAR* ExeName = argv[1];
    int Mode = 0;
    cout << "\r\n当前破解程序：" << ExeName << endl;

    InputCrackMode:
    cout << "请选择破解方式：";
    cin >> Mode;
    cout << endl;
    if (Mode == 1) 
    {
        cout << "破解方式：软电池（SDWRAP）" << endl;
    }
    else if (Mode == 2)
    {
        cout << "破解方式：DMM GAME PLAYER" << endl;
    }
    else
    {
        cout << "不支持的破解模式，请重新输入。" << endl;
        Mode = 0;
        goto InputCrackMode;
    }
    //cout << Mode << endl;
    WriteAYMInfoFile(Mode);
    cout << "补丁制作中……" << endl;
    BOOL IsSuccess = SetFile(ExeName);
    if (!IsSuccess)
    {
        cout << "补丁制作失败！" << endl;
        system("pause");
        return 0;
    }
    cout << "补丁制作成功，请启动补丁文件。" << endl;
    system("pause");
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
