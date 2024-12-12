// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

#pragma pack (1)
struct AYMHeader {
    CHAR Magic[20];
    int Mode;
};
#pragma pack ()

BOOL WINAPI InitCrack()
{
    AYMHeader header;
    FILE* fp = fopen("Info.aym", "rb");
    if (!fp)
    {
        MessageBox(0, L"丢失Info.aym文件。", L"InitCrack", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    fseek(fp, 0, SEEK_SET);
    fread(&header, sizeof(AYMHeader), 1, fp);
    fclose(fp);
    if (lstrcmpA(header.Magic, "AyamiKazeDrmInfo"))
    {
        MessageBox(0, L"Info.aym文件损坏。请重新生成Info.aym。", L"InitCrack", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    if (header.Mode == 1)
        SDWRAP_CRACK();
    else if (header.Mode == 2)
        DMM_GAME_PLAYER_CRACK();
    else
    {
        MessageBox(0, L"模式不明确。", L"InitCrack", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}

BOOL RunOnce = FALSE;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!RunOnce)
        {
            if (!InitCrack())
                ExitProcess(-1);
        }
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