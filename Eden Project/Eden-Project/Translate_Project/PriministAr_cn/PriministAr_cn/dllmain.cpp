// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

HFONT(WINAPI* OldCreateFontIndirectA)(_In_ CONST LOGFONTA* lplf) = CreateFontIndirectA;
HFONT WINAPI NewCreateFontIndirectA(_In_ /* CONST */ LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    //StringCbCopyA(lplf->lfFaceName, 32, "\xBA\xDA\xCC\xE5"); // 黑体
    lstrcpyA(lplf->lfFaceName, "SimHei");
    return OldCreateFontIndirectA(lplf);
}

int(WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;
int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
    WCHAR* lpTextW = ctow((CHAR*)lpText, 932);
    WCHAR* lpCaptionW = ctow((CHAR*)lpCaption, 932);
    return MessageBoxW(hWnd, lpTextW, lpCaptionW, uType);
}

void WINAPI APIHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)OldCreateFontIndirectA, NewCreateFontIndirectA);
    DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);

    DetourTransactionCommit();
}

#ifndef ASM
#define ASM __declspec(naked)
#endif /* ASM */

#ifndef INLINE_ASM
#define INLINE_ASM __asm
#endif

DWORD ImmAlloc = (BaseAddr + 0x65131);
DWORD ImmFree = (BaseAddr + 0x6739C);

ASM PVOID CDECL HostAlloc(LONG Size)
{
    INLINE_ASM
    {
        push dword ptr[esp + 4]
        call ImmAlloc
        pop ecx
        retn
    }
}


ASM VOID CDECL HostFree(PVOID lpMem)
{
    INLINE_ASM
    {
        push dword ptr[esp + 4]
        call ImmFree
        pop ecx
        retn
    }
}

WCHAR fnm[MAX_PATH] = { 0 };
char (*sub_42AB80)(char*, int&, int&);
char FileProc(char* FileName, int& Buffer, int& Size)
{
    wsprintfW(fnm, L"#Project\\%s\0", ctow(FileName, 932));
    //cout << wtoc(fnm, 936) << endl;
    FILE* fp = nullptr;
    _wfopen_s(&fp, fnm, L"rb");
    if (!fp)
        return sub_42AB80(FileName, Buffer, Size);

    cout << wtoc(fnm, 936) << endl;

    fseek(fp, 0, SEEK_END);
    Size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* FileBuff = (BYTE*)HostAlloc(Size);
    fread(FileBuff, 1, Size, fp);
    fclose(fp);

    Buffer = (int&)FileBuff;

    return 1;
}

void WINAPI InlienHook()
{
    *(DWORD*)&sub_42AB80 = BaseAddr + 0x2AB80;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach((void**)&sub_42AB80, FileProc);

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
        make_console();
        APIHook();
        InlienHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

