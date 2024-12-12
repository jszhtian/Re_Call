// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include <iostream>
#include "detours.h"
#include "..\3rd\cryptopp-master\salsa.h"
#include "..\3rd\cryptopp-master\chacha.h"
#include "..\3rd\cryptopp-master\osrng.h"
#include "..\3rd\cryptopp-master\cryptlib.h"
#include "..\3rd\cryptopp-master\hex.h"
#include "..\3rd\cryptopp-master\files.h"
#include "..\3rd\cryptopp-master\secblock.h"

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "cryptlib.lib")
using namespace std;
using namespace CryptoPP;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
#define STDCALL __stdcall
#define FASTCALL __fastcall
const char* Key = "Creative";
const char* Salt = "TudoupeigengHanhuazu copyright 2020";

void DecHelper(const char* Buf, char** OutBuf, const char* Key, const char* Salt, int OriLen)
{
    try
    {
        CryptoPP::Salsa20::Decryption dec;
        dec.SetKeyWithIV((byte*)Key, CryptoPP::Salsa20::MAX_KEYLENGTH, (byte*)Salt, CryptoPP::Salsa20::IV_LENGTH);
        dec.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
    }
    catch (const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

BOOL IsEncrypt = FALSE;
CHAR* Buffer = nullptr;
VOID STDCALL LumShowFileName(CHAR* FileName)
{
    cout << FileName << endl;
    if (strstr(FileName, ".ast"))
        IsEncrypt = TRUE;
}

VOID STDCALL LumDecryptData(DWORD Size)
{
    if (IsEncrypt)
    {
        DecHelper((char*)Buffer, &Buffer, Key, Salt, Size);
        IsEncrypt = FALSE;
    }
}

PVOID FileNameHook = (PVOID)(BaseAddr + 0x124CCE);
PVOID FileBufferHook = (PVOID)(BaseAddr + 0x13AA4F);
PVOID FileSystemSet = (PVOID)(BaseAddr + 0x13AA5D);

__declspec(naked) void mFileNameHook()
{
    _asm
    {
        pushad
        pushfd
        push edi
        call LumShowFileName
        popfd
        popad
        jmp FileNameHook
    }
}

__declspec(naked) void GetFileBuffer()
{
    _asm
    {
        pushad
        pushfd
        mov Buffer, edi
        popfd
        popad
        jmp FileBufferHook
    }
}

__declspec(naked) void mFileSystem()
{
    _asm
    {
        pushad
        pushfd
        push eax
        call LumDecryptData
        popfd
        popad
        jmp FileSystemSet
    }
}

VOID FASTCALL InitMain()
{
    DetourTransactionBegin();
    DetourAttach((void**)&FileNameHook, mFileNameHook);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"mFileNameHook Error", 0, MB_OK);

    DetourTransactionBegin();
    DetourAttach((void**)&FileBufferHook, GetFileBuffer);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"GetFileBuffer Error", 0, MB_OK);

    DetourTransactionBegin();
    DetourAttach((void**)&FileSystemSet, mFileSystem);
    if (DetourTransactionCommit() != NOERROR)
        MessageBox(NULL, L"FileSystem Error", 0, MB_OK);
}

static VOID FASTCALL make_console()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    cout << "Debug mode\n" << endl;
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
        InitMain();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
__declspec(dllexport)void WINAPI 土豆培根汉化组()
{
}
