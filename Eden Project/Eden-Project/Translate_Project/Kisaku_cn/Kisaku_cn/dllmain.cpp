// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    lstrcpyA(lplf->lfFaceName, "黑体");
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

/*
CStringA*(__fastcall* sub_5059B0)(void* , void*, int);
CStringA* __fastcall Hook_sub_5059B0(void* This, void* null_ptr, int ScriptObject)
{
    CStringA* ret = sub_5059B0(This, null_ptr, ScriptObject);
    cout << "offset:0x" << hex << * (DWORD*)(ScriptObject + 4 * 4) << endl;
    //cout << ret->GetString() << endl;
    CHAR* str = (CHAR*)(ret);
    cout << *str << endl;
    return ret;
}
*/

string OldText;
int    ScriptObject = NULL;
PVOID  GetOldTextFunc = (PVOID)(BaseAddr + 0x105820);
PVOID  UpdateTextFunc = (PVOID)(BaseAddr + 0x143F0);

PVOID  GetText_ASM_Start = (PVOID)(BaseAddr + 0x105BAD);
PVOID  GetText_ASM_Out = (PVOID)(BaseAddr + 0x105BDF);

/*
00505BAD | 8B55 08           | MOV     EDX, DWORD PTR SS:[EBP + 8]                  | [ebp+8]:"T}}T"
00505BB0 | 52                | PUSH    EDX                                          | edx:"T}}T"
00505BB1 | 8B8D 48FFFFFF     | MOV     ECX, DWORD PTR SS:[EBP - B8]                 |
00505BB7 | E8 64FCFFFF       | CALL    <ai6win_old.sub_505820>                      |
00505BBC | 8885 6FFFFFFF     | MOV     BYTE PTR SS:[EBP - 91], AL                   |
00505BC2 | 0FBE85 6FFFFFFF   | MOVSX   EAX, BYTE PTR SS:[EBP - 91]                  |
00505BC9 | 85C0              | TEST    EAX, EAX                                     | eax:&"繭A"
00505BCB | 74 12             | JE      ai6win_old.505BDF                            |
00505BCD | 0FB68D 6FFFFFFF   | MOVZX   ECX, BYTE PTR SS:[EBP - 91]                  |
00505BD4 | 51                | PUSH    ECX                                          |
00505BD5 | 8D4D 84           | LEA     ECX, DWORD PTR SS:[EBP - 7C]                 | [ebp-7C]:L"讠B"
00505BD8 | E8 13E8F0FF       | CALL    <ai6win_old.sub_4143F0>                      |
00505BDD | EB CE             | JMP     ai6win_old.505BAD                            |
00505BDF | 68 00000080       | PUSH    80000000                                     |
*/
VOID WINAPI InitString()
{
    OldText.clear();
}

VOID WINAPI UpdateString(CHAR c)
{
    OldText += c;
}

VOID WINAPI ProcessString(int ThisPtrAddr)
{
    DWORD offset = *(DWORD*)(ScriptObject + 4 * 4);
    cout << hex << offset << "|" << OldText << endl;
    string newText = OldText;
    if (offset == 0x694)
    {
        newText = "乵婼嶌乶丗偭偨偔丄旤妛偺僇働儔傕偹偉巔傪嶯偟傗偑偭偰丅摨偠埳摢壠偺寣傪庴偗宲偖幰偲偟偰抪偢偐偟偄偤偉丄岥惿偟偄偤偉乧乧偊偊丄偍偄丠";
    }
    for (UCHAR str : newText)
    {
        INLINE_ASM
        {
            PUSH str;
            MOV ECX, ThisPtrAddr;
            CALL UpdateTextFunc;
        }
    }
}
ASM VOID GetText_ASM()
{
    INLINE_ASM
    {
        PUSHAD;
        PUSHFD;
        CALL InitString;
        POPFD;
        POPAD;

        MOV EDX, DWORD PTR SS : [EBP + 0x8] ;
        MOV ScriptObject, EDX;

    LOOP_START1:
        PUSH ScriptObject;
        MOV ECX, DWORD PTR SS : [EBP - 0xB8] ;
        CALL GetOldTextFunc;
        MOV BYTE PTR SS : [EBP - 0x91] , AL;
        MOVSX EAX, BYTE PTR SS : [EBP - 0x91] ;
        TEST EAX, EAX;
        JE LOOP_END1;
        MOVZX ECX, BYTE PTR SS : [EBP - 0x91] ;

        PUSHAD;
        PUSHFD;
        PUSH ECX;
        CALL UpdateString;
        POPFD;
        POPAD;

        JMP LOOP_START1;

    LOOP_END1:
        LEA ECX, DWORD PTR SS : [EBP - 0x7C] ;

        PUSHAD;
        PUSHFD;
        PUSH ECX;
        CALL ProcessString;
        POPFD;
        POPAD;
        JMP GetText_ASM_Out;
    }
}

bool(__cdecl* sub_41AD70)(unsigned __int8* a1);
bool __cdecl Hook_sub_41AD70(unsigned __int8* a1)
{
    return *a1 >= 0x81u && *a1 <= 0xFEu || *a1 >= 0xE0u && *a1 <= 0xFEu;
}


BOOL IATPatch(LPCSTR szDllName, PROC pfnOrg, PROC pfnNew)
{
    HMODULE hmod;
    LPCSTR szLibName;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    PIMAGE_THUNK_DATA pThunk;
    DWORD dwOldProtect, dwRVA;
    PBYTE pAddr;

    hmod = GetModuleHandleW(NULL);
    pAddr = (PBYTE)hmod;
    pAddr += *((DWORD*)&pAddr[0x3C]);
    dwRVA = *((DWORD*)&pAddr[0x80]);

    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hmod + dwRVA);

    for (; pImportDesc->Name; pImportDesc++)
    {
        szLibName = (LPCSTR)((DWORD)hmod + pImportDesc->Name);
        if (!stricmp(szLibName, szDllName))
        {
            pThunk = (PIMAGE_THUNK_DATA)((DWORD)hmod + pImportDesc->FirstThunk);
            for (; pThunk->u1.Function; pThunk++)
            {
                if (pThunk->u1.Function == (DWORD)pfnOrg)
                {
                    VirtualProtect((LPVOID)&pThunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
                    pThunk->u1.Function = (DWORD)pfnNew;
                    VirtualProtect((LPVOID)&pThunk->u1.Function, 4, dwOldProtect, &dwOldProtect);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}



BOOL IATFinder(LPCSTR szDllName, PROC pfnOrg)
{
    HMODULE hmod;
    LPCSTR szLibName;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    PIMAGE_THUNK_DATA pThunk;
    DWORD dwOldProtect, dwRVA;
    PBYTE pAddr;

    hmod = GetModuleHandleW(NULL);
    pAddr = (PBYTE)hmod;
    pAddr += *((DWORD*)&pAddr[0x3C]);
    dwRVA = *((DWORD*)&pAddr[0x80]);

    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hmod + dwRVA);

    for (; pImportDesc->Name; pImportDesc++)
    {
        szLibName = (LPCSTR)((DWORD)hmod + pImportDesc->Name);
        if (!stricmp(szLibName, szDllName))
        {
            pThunk = (PIMAGE_THUNK_DATA)((DWORD)hmod + pImportDesc->FirstThunk);
            for (; pThunk->u1.Function; pThunk++)
            {
                if (pThunk->u1.Function == (DWORD)pfnOrg)
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}



int WINAPI HookMultiByteToWideChar(
    _In_      UINT   CodePage,
    _In_      DWORD  dwFlags,
    _In_      LPCSTR lpMultiByteStr,
    _In_      int    cbMultiByte,
    _Out_opt_ LPWSTR lpWideCharStr,
    _In_      int    cchWideChar
)
{
    //cout << CodePage << endl;
    CodePage = 932;
    return MultiByteToWideChar(CodePage, dwFlags,
        lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

int WINAPI HookWideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar)
{
    //if (CodePage == CP_THREAD_ACP)
        CodePage = 932;
    return WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
}

void WINAPI APIHook()
{
    /*(DWORD*)&sub_5059B0 = BaseAddr + 0x1059B0;
    *(DWORD*)&sub_41AD70 = BaseAddr + 0x1AD70;
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
    DetourAttach((void**)&sub_41AD70, Hook_sub_41AD70);
    //DetourAttach((void**)&GetText_ASM_Start, GetText_ASM);
    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBoxW(NULL, L"API Function Hook Error.", L"APIHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }*/

    if (!IATPatch("Kernel32.dll", GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "WideCharToMultiByte"), (PROC)HookWideCharToMultiByte))
    {
        MessageBoxW(0, L"WideCharToMultiByte Hook Error。", L"IATHook Error", 0);
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
        APIHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI AyamiKaze()
{
}