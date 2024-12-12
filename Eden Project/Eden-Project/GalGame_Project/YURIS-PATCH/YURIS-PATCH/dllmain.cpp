// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

// Font & EnumFont
PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}


//貌似Win10中 CreateFontA 不再通过CreateFontIndirectA实现，所以单独Hook
PVOID g_pOldCreateFontA = CreateFontA;
typedef int (WINAPI* PfuncCreateFontA)(int nHeight,
    int nWidth,
    int nEscapement,
    int nOrientation,
    int fnWeight,
    DWORD fdwltalic,
    DWORD fdwUnderline,
    DWORD fdwStrikeOut,
    DWORD fdwCharSet,
    DWORD fdwOutputPrecision,
    DWORD fdwClipPrecision,
    DWORD fdwQuality,
    DWORD fdwPitchAndFamily,
    LPCTSTR lpszFace);
int WINAPI NewCreateFontA(int nHeight,
    int nWidth,
    int nEscapement,
    int nOrientation,
    int fnWeight,
    DWORD fdwltalic,
    DWORD fdwUnderline,
    DWORD fdwStrikeOut,
    DWORD fdwCharSet,
    DWORD fdwOutputPrecision,
    DWORD fdwClipPrecision,
    DWORD fdwQuality,
    DWORD fdwPitchAndFamily,
    LPCTSTR lpszFace)
{
    fdwCharSet = GB2312_CHARSET;
    return ((PfuncCreateFontA)g_pOldCreateFontA)(nHeight,
        nWidth,
        nEscapement,
        nOrientation,
        fnWeight,
        fdwltalic,
        fdwUnderline,
        fdwStrikeOut,
        fdwCharSet,
        fdwOutputPrecision,
        fdwClipPrecision,
        fdwQuality,
        fdwPitchAndFamily,
        lpszFace);
}

PVOID g_pOldEnumFontFamiliesExA = EnumFontFamiliesExA;
typedef int (WINAPI* PfuncEnumFontFamiliesExA)(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags);
int WINAPI NewEnumFontFamiliesExA(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags)
{
    lpLogfont->lfCharSet = GB2312_CHARSET;
    //lpLogfont->lfFaceName[0] = '\0';
    return ((PfuncEnumFontFamiliesExA)g_pOldEnumFontFamiliesExA)(hdc, lpLogfont, lpEnumFontFamExProc, lParam, dwFlags);
}

void Init()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
    DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
    DetourAttach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBoxW(0, L"Hook API error.", L"Init", MB_OK | MB_ICONERROR);
        ExitProcess(0);
    }
    else
        MessageBoxW(0, L"本补丁为【咩咕噜汉化组】制作\n禁止一切录播直播和商业行为。", L"使用须知", MB_OK | MB_ICONINFORMATION);
}

// FileSystemPatch

char* mBuffer = NULL;
unsigned int mSize = NULL;

unsigned int GetSize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    unsigned int ret = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return ret;
}

void __stdcall ReadFileFromDir(char* FileName)
{
    //cout << FileName << endl;
    char fnm[MAX_PATH] = { 0 };
    sprintf(fnm, "Kaze\\%s", FileName);
    FILE* fp = fopen(fnm, "rb");
    if (fp) 
    {
        cout << "ReadFileFromKaze:" << fnm << endl;
        unsigned int FileSize = GetSize(fp);
        char* FileBuff = (char*)malloc(FileSize);
        if (FileBuff)
        {
            fread(FileBuff, FileSize, 1, fp);
            fclose(fp);
            mBuffer = FileBuff;
            mSize = FileSize;
        }
        else
        {
            fclose(fp);
            char err[MAX_PATH] = { 0 };
            sprintf(err, "Can't read file:%s\nAlloc memory failed.", fnm);
            MessageBoxA(0, err, "ReadFileFromDir", MB_OK | MB_ICONERROR);
            ExitProcess(0);
        }
    }
}

void __stdcall CopyFileFromRead(char* oldBuffer, unsigned int oldSize)
{
    if (mSize && mBuffer)
    {
        memset(oldBuffer, 0, oldSize);
        memcpy(oldBuffer, mBuffer, mSize);
        free(mBuffer);
        mBuffer = NULL;
        mSize = NULL;
    }
}
PVOID pShowFileName = (PVOID)(BaseAddr + 0x405A6);
__declspec(naked) void gShowFileName()
{
    _asm
    {
        pushad;
        pushfd;
        push edi;
        call ReadFileFromDir;
        popfd;
        popad;
        jmp pShowFileName;
    }
}

PVOID pSetFileSize1 = (PVOID)(BaseAddr + 0x1B93E);
__declspec(naked) void gSetFileSize1()
{
    _asm
    {
        pushad;
        pushfd;
        cmp mSize, 0x00;
        je have;
        mov eax, mSize;
        mov dword ptr ss : [esp + 0x144] , eax;
    have:
        popfd;
        popad;
        jmp pSetFileSize1
    }
}
void FileSystemSet()
{
    DetourTransactionBegin();
    DetourAttach((void**)&pShowFileName, gShowFileName);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBoxW(0, L"FileSystem hook error.", L"FileSystemSet", MB_OK | MB_ICONERROR);
        ExitProcess(0);
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
        FileSystemSet();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

