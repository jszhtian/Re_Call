// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

class MalieMoji
{
public:
    wstring ProcessChsStr(WCHAR* str_in, unsigned int size);
    DWORD ret_len;
};
MalieMoji MJ;
wstring MalieMoji::ProcessChsStr(WCHAR* str_in, unsigned int size)
{
    wstring chs = wstring(str_in, size);
    wstring ret(0x1000, 0);
    unsigned int len = 0;

    auto x = chs.find_first_of(L'※');
    if (x != wstring::npos)
    {
        chs = wstring(&chs[x + 1], &chs[chs.size() - 1]);
    }
    for (unsigned int idx = 0; idx < size; idx++)
    {
        switch (chs[idx])
        {
        case EOPAR:
            ret[len++] = 0x0006;
            ret_len += 1;
            break;
        case TO_VOL:
            ret[len++] = 0x0007;
            ret[len++] = 0x0008;
            ret_len += 2;
            break;
        case EOVOL:
            ret[len++] = 0x0007;
            ret[len++] = 0x0009;
            ret_len += 2;
            break;
        case STRUB:
            ret[len++] = 0x000A;
            ret_len += 1;
            break;
        case EOSTR:
            ret[len++] = 0x0000;
            ret_len += 1;
            break;
        case NXL:
            ret[len++] = 0x0007;
            ret[len++] = 0x0004;
            ret_len += 2;
            break;
        case TO_RTN:
            ret[len++] = 0x0007;
            ret[len++] = 0x0006;
            ret_len += 2;
            break;
        case TO_RUB:
            ret[len++] = 0x0007;
            ret[len++] = 0x0001;
            ret_len += 2;
            break;
        default:
            ret[len++] = chs[idx];
            ret_len += 2;
        }
    }
    return move(ret);
}
WCHAR NormalText[0x1000] = { 0 };
DWORD szOfNormalText = 0;
WCHAR* TestText = (WCHAR*)L"刑一※▲v_aa00001▽はぁ、はぁ、はぁ……中文测试123ABCabc△◎◁";
DWORD __stdcall HookPushNormalStr(DWORD idx)
{
    memset(NormalText, 0, 0x1000 * sizeof(WCHAR));
    if (idx == 0) 
    {
        memcpy(NormalText, MJ.ProcessChsStr(TestText, wcslen(TestText)).c_str(), MJ.ret_len);
        szOfNormalText = MJ.ret_len * sizeof(WCHAR) + 2;
        cout << MJ.ret_len << endl;
        cout << &NormalText << endl;
        MJ.ret_len = 0;
        return (DWORD)(&NormalText);
    }
    return 0;
   
}
PVOID pHookPushNormalStr = (PVOID)0x5319C4;
PVOID eHookPushNormalStr = (PVOID)0x5319CC;

__declspec(naked)void gHookPushNormalStr()
{
    DWORD idx, Off;
    _asm
    {
        mov idx, esi;
        mov esi, DWORD PTR DS : [edx + 0x10] ;
        add esp, 0x8;
        add esi, ebx;

        pushad;
        pushfd;
        push idx;
        call HookPushNormalStr;
        mov Off, eax;
        popfd;
        popad;
        // temp
        mov idx, eax;
        xor eax, eax;
        cmp Off, eax;
        mov eax, idx;
        je do_not_copy;

        mov eax, szOfNormalText;
        mov ecx, szOfNormalText;
        mov esi, Off
    do_not_copy:
        jmp eHookPushNormalStr
    }
}

WCHAR NameText[0x1000] = { 0 };
DWORD szOfNameText = 0;
DWORD __stdcall HookPushNameText(WCHAR* Name)
{
    memset(NameText, 0, 0x1000 * sizeof(WCHAR));
    if (!wcscmp(Name, L"刑一")) 
    {
        szOfNameText = wcslen(L"刑一测试") * sizeof(WCHAR) + 2; //最后的00
        memcpy(NameText, L"刑一测试", szOfNameText);
        return (DWORD)(&NameText);
    }
    return 0;
}

PVOID pHookPushNameText = (PVOID)0x529F7C;
PVOID eHookPushNameText = (PVOID)0x529F83;

__declspec(naked)void gHookPushNameText()
{
    DWORD Name, Temp;
    _asm
    {
        mov edi, DWORD PTR SS : [esp + 0x10] ;
        lea ecx, DWORD PTR DS : [eax + 0x2] ;

        pushad;
        pushfd;
        push esi;
        call HookPushNameText;
        mov Name, eax;
        popfd;
        popad;

        mov Temp, eax;
        xor eax, eax;
        cmp Name, eax;
        mov eax, Temp;

        je do_not_copy;
        mov ecx, szOfNameText;
        mov esi, Name;
    do_not_copy:
        jmp eHookPushNameText
    }
}

CONST WCHAR* DatNames[7] = {
    L".\\data\\chs.dat",
    L".\\data\\game.dat",
    L".\\data\\chara.dat",
    L".\\data\\cg.dat",
    L".\\data\\sound.dat",
    L".\\data\\voice.dat",
    L".\\data\\movie.dat",
};
DWORD __stdcall HookDatNames(DWORD idx)
{
    return (DWORD) (DatNames[idx / 4]);
}

PVOID pHookDatNames = (PVOID)0x52ABC6;
PVOID eHookDatNames = (PVOID)0x52ABE3;

PVOID DatCall = (PVOID)0x52E5E0;

__declspec(naked)void gHookDatNames()
{
    DWORD DatAddr;
    _asm
    {
    LoopStart:
        pushad;
        pushfd;
        push esi;
        call HookDatNames;
        mov DatAddr, eax;
        popfd;
        popad;
        
        mov eax, DatAddr;
        push eax;
        call DatCall;
        mov DWORD PTR DS : [esi + 0x6E8290] , eax;
        add esi, 4;
        add esp, 4;
        cmp esi, 0x1C;

        jb LoopStart;
        jmp eHookDatNames
    }
}

WCHAR ReadName[0x1000] = { 0 };
WCHAR LastReadName[0x1000] = { 0 };
DWORD* (__cdecl *IoOpen)(WCHAR* FileName, WCHAR* Mode);
DWORD* __cdecl HookIoOpen(WCHAR* FileName, WCHAR* Mode)
{
    wcscpy(ReadName, FileName);
    return IoOpen(FileName, Mode);
}

int (*__cdecl IoRead)(DWORD* IoHandle, BYTE* FileBuff, int ReadSize);
int __cdecl HookIoRead(DWORD* IoHandle, BYTE* FileBuff, int ReadSize)
{
    /*
    if(!wcscmp(LastReadName, ReadName))
        return IoRead(IoHandle, FileBuff, FileSize);
    wcscpy(LastReadName, ReadName);
    cout << wtoc(LastReadName) << endl;
    */
    /**/
    if (!wcscmp(ReadName, L".\\chs.cpk"))
    {
        DWORD f = *IoHandle;
        cout << "copy mae" << endl;
        int ret = fread(FileBuff, ReadSize, 1, *(FILE**)f);
        cout << "copy end" << endl;

        return ret;
    }
    return IoRead(IoHandle, FileBuff, ReadSize);
}


void ScriptPatch()
{
    DetourTransactionBegin();

    DetourAttach(&pHookPushNormalStr, gHookPushNormalStr);
    DetourAttach(&pHookPushNameText, gHookPushNameText);

    DetourAttach(&pHookDatNames, gHookDatNames);

    /*
    *(DWORD*)&IoOpen = 0x525FE0;
    *(DWORD*)&IoRead = 0x526130;

    DetourAttach((void**)&IoOpen, HookIoOpen);
    DetourAttach((void**)&IoRead, HookIoRead);
    */
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Patch hook error", L"Patch", MB_OK);
        ExitProcess(-1);
    }
}

bool IsOnce = false;
PVOID g_pOldGetCurrentDirectoryW = GetCurrentDirectoryW;
typedef DWORD(WINAPI* PfuncGetCurrentDirectoryW)(DWORD nBufferLength, LPTSTR lpBuffer);
DWORD WINAPI NewGetCurrentDirectoryW(DWORD nBufferLength, LPTSTR lpBuffer)
{
    if (!IsOnce)
    {
        ScriptPatch();
        IsOnce = true;
    }
    return ((PfuncGetCurrentDirectoryW)g_pOldGetCurrentDirectoryW)(nBufferLength, lpBuffer);
}

PVOID g_pOldSetWindowTextW = SetWindowTextW;
typedef int (WINAPI* PfuncSetWindowTextW)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextW(HWND hwnd, LPWSTR lpString)
{
    return ((PfuncSetWindowTextW)g_pOldSetWindowTextW)(hwnd, lpString);
}

PVOID g_pOldCreateFontIndirectW = CreateFontIndirectW;
typedef int (WINAPI* PfuncCreateFontIndirectW)(LOGFONTW* lplf);
int WINAPI NewCreateFontIndirectW(LOGFONTW* lplf)
{
    //这里由于是竖排显示，所以需要加@。横排不需要加
    lplf->lfCharSet = GB2312_CHARSET;
    lplf->lfWeight = FW_BOLD;
    lplf->lfQuality = ANTIALIASED_QUALITY;
    wcscpy_s(lplf->lfFaceName, L"SimHei");
    return ((PfuncCreateFontIndirectW)g_pOldCreateFontIndirectW)(lplf);
}

void Init()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldSetWindowTextW, NewSetWindowTextW);
    DetourAttach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
    DetourAttach(&g_pOldGetCurrentDirectoryW, NewGetCurrentDirectoryW);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Init hook error", L"Init", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}

static void make_console()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    cout << "Console\n" << endl;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LoadLibraryW(L"kDays.dll");
        make_console();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

