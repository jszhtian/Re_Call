// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "IATProc.h"
void __stdcall AymShowTexW(wchar_t* text)
{
    if(wcslen(text)!=0)
        cout << wtoc(text) << endl;
}

PVOID pGetFileName = (PVOID)(BaseAddr + 0xEC5C4);
__declspec(naked)void mGetFileName()
{
    __asm
    {
        pushad
        pushfd
        push edi
        call AymShowTexW
        popfd
        popad
        jmp pGetFileName
    }
}



void InlineHook()
{
    DetourTransactionBegin();
    DetourAttach((void**)&pGetFileName, mGetFileName);
    DetourTransactionCommit();
}


void SupportOutFileRead()
{
    BYTE nop[] = { 0x90, 0x90 };
    BYTE nop2[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    memcopy((void*)(BaseAddr + 0xF0BE1), nop, sizeof(nop));
    memcopy((void*)(BaseAddr + 0xF0C05), nop, sizeof(nop));
    memcopy((void*)(BaseAddr + 0xF0D61), nop2, sizeof(nop2));
}

HFONT WINAPI HookCreateFontIndirectW(LOGFONTW* lplf)
{
    LOGFONTW lf;
    memcpy(&lf, lplf, sizeof(LOGFONTW));
    cout << "lfWeight:0x" << hex << lf.lfWeight << endl;
    //cout << "lfWidth:" << lf.lfWidth << endl;
    cout << "lfHeight:0x" << hex << lf.lfHeight << endl;
    if (lf.lfHeight == -112)
    {
        cout << "change " << endl;
        lf.lfHeight = -144;
    }
    return CreateFontIndirectW(&lf);
}

void IATHook()
{
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectW"), (PROC)HookCreateFontIndirectW))
    {
        MessageBoxW(0, L"CreateFontIndirectW Hook Error。", L"IATHook Error", 0);
        return;
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
        //InlineHook();
        //SupportOutFileRead();
        IATHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

