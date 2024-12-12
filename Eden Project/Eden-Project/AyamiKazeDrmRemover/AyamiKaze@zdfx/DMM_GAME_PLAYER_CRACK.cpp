#include "framework.h"

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
    if (hm2)
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

BOOL DMM_GAME_PLAYER_CRACK()
{
    HANDLE ret = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Crack, NULL, 0, 0);
    if (!ret)
    {
        MessageBox(NULL, L"CRACK ERROR\nCODE: 1", L"DMM_GAME_PLAYER_CRACK", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}