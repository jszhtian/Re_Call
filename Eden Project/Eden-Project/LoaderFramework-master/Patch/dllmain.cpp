// dllmain.cpp

#include "util.h"
#include <detours.h>


//=============================================================================
// Startup Code
//=============================================================================


void InstallPatches()
{
    LogWrite(L"补丁已安装\n");
}


void InstallHooks()
{
    LogWrite(L"钩子已安装\n");
}


//=============================================================================
// DLL Entry Point
//=============================================================================


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            // See https://github.com/microsoft/Detours/wiki/DetourRestoreAfterWith
            DetourRestoreAfterWith();

            // Open the log file
            LogInit(L"dev.log");

            InstallPatches();
            InstallHooks();

            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}


//=============================================================================
// Dummy Export Symbol
//=============================================================================


BOOL APIENTRY CreateObject()
{
    return TRUE;
}
