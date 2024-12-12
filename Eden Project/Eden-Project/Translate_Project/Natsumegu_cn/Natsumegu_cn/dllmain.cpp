// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
string fnm;

void WritLog(char* Msg)
{
    FILE* fp = fopen(fnm.c_str(), "ab+");
    if (!fp)
    {
        MessageBox(NULL, L"WritLog Error", L"WritLog", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
    fwrite(Msg, strlen(Msg), 1, fp);
    fwrite("\r\n", 2, 1, fp);
    fclose(fp);
}

void WINAPI PrintMsg(char* Msg)
{
    cout << wtoc(ctow(Msg, 932), 936) << endl;
    WritLog(Msg);
}

PVOID pPrintMsg = (PVOID)(BaseAddr + 0xA7C1);
_declspec(naked) void gPrintMsg()
{
    _asm
    {
        pushad;
        pushfd;
        push eax;
        call PrintMsg;
        popfd;
        popad;
        jmp pPrintMsg;
    }
}

void WINAPI CreateLogFile()
{
    time_t Now = time(0);
    char TimeNow[MAX_PATH] = { 0 };
    char* ptrNow = ctime(&Now);
    strncpy(TimeNow, ptrNow, strlen(ptrNow) - 1);

    char txtFileName[MAX_PATH] = { 0 };
    sprintf(txtFileName, "_%s.txt", TimeNow);

    for (char c : txtFileName)
    {
        if (c == ' ' || c == ':')
            fnm += "_";
        else
            fnm += c;
    }

    cout << fnm << endl;
}

void WINAPI GetScriptName()
{
    DetourTransactionBegin();
    DetourAttach((void**)&pPrintMsg, gPrintMsg);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"GetScriptName Error", L"GetScriptName", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
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
        CreateLogFile();
        GetScriptName();
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