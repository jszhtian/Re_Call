// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
using namespace std;
void Crack()
{
    HMODULE hm = 0;
    HMODULE hm2 = 0;
    LPVOID lpAddress = 0;
    LPVOID lpAddress2 = 0;
    DWORD flOldProtect = 0;
    while (!hm)
        hm = LoadLibraryW(L"plugin.dll");
    lpAddress = GetProcAddress(hm, "executeAPI");
    hm2 = LoadLibraryW(L"sub.dll");
    if(hm2)
        lpAddress2 = GetProcAddress(hm2, "INITIAL");
    unsigned int* pos = (unsigned int*)lpAddress;
    if (lpAddress) 
    {
        while (*pos == 0xE9)
            pos += *(DWORD*)(pos + 1) + 5;
        if (hm2)
        {
            VirtualProtect(lpAddress, 0x10, PAGE_EXECUTE_READWRITE, &flOldProtect);
            *(BYTE*)lpAddress = 0xE9;
            *(DWORD*)((char*)lpAddress + 1) = (char*)hm2 - (char*)((BYTE*)lpAddress + 5);
        }
        else
        {
            VirtualProtect(lpAddress, 0x10, PAGE_EXECUTE_READWRITE, &flOldProtect);
            *(BYTE*)lpAddress = 0xC3;
            *((BYTE*)lpAddress + 1) = 0xC3;
        }
        VirtualProtect(lpAddress, 0x10, flOldProtect, &flOldProtect);

        for (int i = 0; i < 4096; ++i)
        {
            if (pos[i] == 0x75 && pos[i + 1] == 2 && pos[i + 2] == 0xEB)
            {
                VirtualProtect(&pos[i], 0x10, PAGE_EXECUTE_READWRITE, &flOldProtect);
                pos[i] == 0x90;
                pos[i + 1] == 0x90;
                VirtualProtect(&pos[i], 0x10, flOldProtect, &flOldProtect);
                return;
            }
        }
    }
}

void CrackMain()
{
   HANDLE ret = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Crack, NULL, 0, 0);
   if (!ret)
       MessageBox(0, L"Can't Create Thread.", L"CrackMain", 0);
}

static void make_console() {
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
        //make_console();
        CrackMain();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI Dummy()
{
}