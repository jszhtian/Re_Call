// dllmain.cpp

#include "util.h"
#include "util_detours.h"
#include <intrin.h>


//=============================================================================
// Hook Code
//=============================================================================


auto pfnRioLoadFileEx = (LPVOID(*)(LPCSTR, LPDWORD))0x00405940;

LPVOID RioLoadFileEx(LPCSTR lpName, LPDWORD lpSize)
{
    auto name = ShiftJisToUcs2(lpName);
    LogWrite(L"RioLoadFileEx: FILE=%s\r\n", (LPCWSTR)name);

    if (_stricmp(lpName, "start.scn") == 0)
    {
        FILE* fp;
        fopen_s(&fp, lpName, "rb");
        if (fp)
        {
            fseek(fp, 0, SEEK_END);
            auto size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            if (size > 0)
            {
                PVOID ptr = GlobalAlloc(GPTR, size);
                if (ptr)
                {
                    if (fread(ptr, size, 1, fp) == 1)
                    {
                        LogWrite("start.scn overrided\r\n");
                        fclose(fp);
                        *lpSize = size;
                        return ptr;
                    }
                    GlobalFree(ptr);
                }
            }
            fclose(fp);
        }
    }

    return pfnRioLoadFileEx(lpName, lpSize);
}


auto pfnMessageBoxA = MessageBoxA;

auto pfnRegCloseKey = RegCloseKey;
auto pfnRegQueryValueExA = RegQueryValueExA;
auto pfnRegOpenKeyExA = RegOpenKeyExA;
auto pfnRegFlushKey = RegFlushKey;
auto pfnRegSetValueExA = RegSetValueExA;
auto pfnRegCreateKeyExA = RegCreateKeyExA;
auto pfnRegEnumValueA = RegEnumValueA;
auto pfnRegOpenKeyA = RegOpenKeyA;


static auto gszRegPathGBK = "\x73\x6F\x66\x74\x77\x61\x72\x65\x5C\x51\x72\x75\x70\x70\x6F\x5C\x92\x69\xA4\xAD\xA5\xB2\xA9\x60\xA4\xDF\xA4\xBF\xA4\xA4\xA4\xCA\x8D\x75\xA4\xCB\xD7\xA1\xA4\xF3\xA4\xC7\xA4\xEB\xD8\x9A\xC8\xE9\xA4\xCF\xA4\xC9\xA4\xA6\xA4\xB9\xA4\xEA\xA4\xE3\xA4\xA4\xA4\xA4\xA4\xC7\xA4\xB9\xA4\xAB\xA3\xBF\xA3\xB2";
static auto gszRegPathSJS = "\x73\x6F\x66\x74\x77\x61\x72\x65\x5C\x51\x72\x75\x70\x70\x6F\x5C\x94\xB2\x82\xAB\x83\x51\x81\x5B\x82\xDD\x82\xBD\x82\xA2\x82\xC8\x93\x87\x82\xC9\x8F\x5A\x82\xF1\x82\xC5\x82\xE9\x95\x6E\x93\xFB\x82\xCD\x82\xC7\x82\xA4\x82\xB7\x82\xE8\x82\xE1\x82\xA2\x82\xA2\x82\xC5\x82\xB7\x82\xA9\x81\x48\x82\x51";


/* Hook sub_408280 + 47 */
static PVOID pCopyRegistryPathNextAddr;

_declspec(naked) void CopyRegistryPathDetour()
{
    _asm
    {
        PUSH    gszRegPathGBK
        JMP     pCopyRegistryPathNextAddr
    }
}

void CopyRegistryPath(LPSTR _Dst, LPCSTR _Src)
{
    lstrcpyA(_Dst, gszRegPathGBK);
}

/* Hook sub_4297D0 + 131F */
static PVOID pCopyRegistryPathNextAddr2;

_declspec(naked) void CopyRegistryPathDetour2()
{
    _asm
    {
        CALL    CopyRegistryPath
        ADD     ESP, 8
        JMP     pCopyRegistryPathNextAddr2
    }
}


LSTATUS APIENTRY MineRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
    return pfnRegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}


LSTATUS APIENTRY MineRegCloseKey(HKEY hKey)
{
    return pfnRegCloseKey(hKey);
}


LSTATUS APIENTRY MineRegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
    return pfnRegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}


int WINAPI MineMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    auto strText = lpText ? ShiftJisToUcs2(lpText) : CStringW();
    auto strCaption = lpCaption ? ShiftJisToUcs2(lpCaption) : CStringW();

    return MessageBoxW(hWnd, strText, strCaption, uType);
}


LPVOID RIO_LoadResource(LPCSTR lpFileName, LPDWORD lpFileSize)
{
    typedef LPVOID(*PROC)(LPCSTR, LPDWORD);
    auto pfn = (PROC)0x00405940;

    #pragma region
    //LogWrite("RIO LoadResource = \"%s\"\r\n", lpFileName);

    const char* filelist[] =
    {
        "01_c00.txt",
        "01_c01.txt",
        "01_c02.txt",
        "01_c03.txt",
        "01_c04.txt",
        "01_c05.txt",
        "01_c06.txt",
        "01_c07.txt",
        "01_c08.txt",
        "01_c09.txt",
        "01_c09_io01.txt",
        "01_c09_rt01.txt",
        "01_c09_tr01.txt",
        "01_c10.txt",
        "01_c10_io02.txt",
        "01_c10_rt02.txt",
        "01_c10_tr02.txt",
        "01_c11_1.txt",
        "01_c11_2.txt",
        "01_c11_io03.txt",
        "01_c11_rt03.txt",
        "01_c11_tr03.txt",
        "01_c12_1.txt",
        "01_c12_2.txt",
        "01_c12_io04.txt",
        "01_c12_rt04.txt",
        "01_c12_tr04.txt",
        "01_c12_tr05.txt",
        "01_c13.txt",
        "02_c13_trialend.txt",
        "02_c14.txt",
        "02_c14_io05.txt",
        "02_c14_rt05.txt",
        "02_c14_tr06.txt",
        "02_c15.txt",
        "02_c15_io06.txt",
        "02_c15_rt06.txt",
        "02_c16_1.txt",
        "02_c16_2.txt",
        "02_c16_io07.txt",
        "02_c16_rt07.txt",
        "02_c16_tr08.txt",
        "02_c17.txt",
        "02_c17_io08.txt",
        "02_c18.txt",
        "02_c19_1.txt",
        "02_c19_2.txt",
        "02_c19_io09.txt",
        "02_c19_rt08.txt",
        "02_c19_tr09.txt",
        "02_c20.txt",
        "02_c21_1.txt",
        "02_c21_2.txt",
        "02_c21_io10.txt",
        "02_c21_rt09.txt",
        "02_c21_tr10.txt",
        "02_c22_1.txt",
        "02_c22_2.txt",
        "02_c22_io11.txt",
        "02_c22_rt10.txt",
        "02_c22_tr11.txt",
        "02_c23.txt",
        "02_c24.txt",
        "02_c24_io12.txt",
        "02_c24_ma01.txt",
        "02_c24_rt11.txt",
        "02_c24_tr12.txt",
        "03_tr13.txt",
        "03_tr14.txt",
        "03_tr15.txt",
        "03_tr16.txt",
        "03_tr17.txt",
        "03_tr18.txt",
        "03_tr19.txt",
        "03_tr20.txt",
        "04_rt12.txt",
        "04_rt13.txt",
        "04_rt14.txt",
        "04_rt15.txt",
        "04_rt16.txt",
        "04_rt17.txt",
        "04_rt18.txt",
        "04_rt19.txt",
        "04_rt20.txt",
        "05_io13.txt",
        "05_io14.txt",
        "05_io15.txt",
        "05_io16.txt",
        "05_io17.txt",
        "05_io18.txt",
        "06_at01.txt",
        "06_at02.txt",
        "06_at03.txt",
        "06_at04.txt",
        "06_at05.txt",
        "06_at06.txt",
        "06_at07.txt",
        "07_nk01.txt",
        "07_nk02.txt",
        "07_nk03.txt",
        "07_nk04.txt",
        "07_nk05.txt",
        "07_nk06.txt",
        "08_hw01.txt",
        "08_hw02.txt",
        "08_hw03.txt",
        "08_hw04.txt",
        "08_hw05.txt",
        "09_mh01.txt",
        "09_mh02.txt",
        "09_mh03.txt",
        "09_mh04.txt",
        "09_mh05.txt",
        "09_mh06.txt",
        "09_mh07.txt",
        "10_fk01.txt",
        "10_fk02.txt",
        "10_fk03.txt",
        "10_fk04.txt",
        "10_fk05.txt",
        "10_fk06.txt",
        "10_fk07.txt",
    };

    /*for (int i = 0; i < _countof(filelist); i++)
    {
        static char path[260];
        sprintf_s(path, "p\\%s", filelist[i]);
        DWORD size;
        PVOID ptr = pfn(path, &size);
        if (ptr)
        {
            FILE* fp;
            fopen_s(&fp, path, "wb");
            if (fp)
            {
                fwrite(ptr, size, 1, fp);
                fflush(fp);
                fclose(fp);
            }
            GlobalFree(ptr);
        }
    }*/

    #pragma endregion

    if (_stricmp(lpFileName, "p\\07_nk01.txt") == 0)
    {
        FILE* fp;
        fopen_s(&fp, lpFileName, "rb");
        if (fp)
        {
            fseek(fp, 0, SEEK_END);
            auto size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            if (size > 0)
            {
                PVOID ptr = GlobalAlloc(GPTR, size);
                if (ptr)
                {
                    if (fread(ptr, size, 1, fp) == 1)
                    {
                        fclose(fp);
                        *lpFileSize = size;
                        return ptr;
                    }
                    GlobalFree(ptr);
                }
            }
            fclose(fp);
        }
    }

    return pfn(lpFileName, lpFileSize);
}

/* Hook sub_4297D0 + E5A */
static PVOID pLoadResourceNextAddr;

_declspec(naked) void LoadResourceDetour()
{
    _asm
    {
        CALL    RIO_LoadResource
        JMP     pLoadResourceNextAddr
    }
}


void CopyFontName(LPSTR lpFontName)
{
    lstrcpyA(lpFontName, "Noto Sans SC");
}

/* Hook sub_438750 + 926 */
static PVOID pCopyFontNameNextAddr;

_declspec(naked) void CopyFontNameDetour()
{
    _asm
    {
        MOV     EDI, 1
        MOV     [ESI+0x5D8], EDI
        MOV     [ESI+0x1614], EDI
        LEA     EDX, [ESI+0x38]
        XOR     EDI, EDI
    loc_43908C:
        MOV     ECX, [ESI+0x168]
        MOV     AL, [ECX]
        INC     ECX
        CMP     AL, 0x2F
        MOV     [ESI+0x168], ECX
        JZ      loc_4390C3
        TEST    AL, AL
        JZ      loc_4390BC
        MOV     [EDX], AL
        INC     EDX
        INC     EDI
        CMP     EDI, 0xFE
        JL      loc_43908C
        JMP     loc_4390C3
    loc_4390BC:
        DEC     ECX
        MOV     [ESI+0x168], ECX
    loc_4390C3:
        MOV     BYTE PTR [EDX], 0
        //--------------------------
        ADD     ESI, 0x38
        PUSH    ESI
        CALL    CopyFontName
        ADD     ESP, 4
        //--------------------------
        POP     EDI
        POP     ESI
        POP     EBP
        ADD     ESP, 0x100
        RETN
    }
}


/* Hook sub_438750 + 980 */
static PVOID pReadFontCharSetNextAddr;

_declspec(naked) void ReadFontCharSetDetour()
{
    _asm
    {
        MOV     EBP, 0x004385C0     // S_GetVal
        MOV     EDI, 1
        MOV     [ESI+0x5D8], EDI
        MOV     [ESI+0x1614], EDI
        CMP     BYTE PTR [EAX], 0x2C
        JZ      loc_4390F4
        PUSH    ESI
        CALL    EBP
        ADD     ESP, 4
        MOV     [ESI+0x2C], 0x01    // tr->Font_CharSet
        JMP     loc_439102
    loc_4390F4:
        INC     EAX
        MOV     [ESI+0x168], EAX
        MOV     DWORD PTR [ESI+0x2C], 0x01  // tr->Font_CharSet
    loc_439102:
        MOV     EDX, [ESI+0x168]
        CMP     BYTE PTR [EDX-1], 0x2C
        JNZ     loc_439124
        PUSH    ESI
        CALL    EBP
        ADD     ESP, 4
        POP     EDI
        MOV     [ESI+0x34], EAX
        POP     ESI
        POP     EBP
        ADD     ESP, 0x100
        RETN
    loc_439124:
        MOV     [ESI+0x34], EDI
        POP     EDI
        POP     ESI
        POP     EBP
        ADD     ESP, 0x100
        RETN
    }
}


/* Hook sub_438750 + 9E1 */
static PVOID pReadFontHeightNextAddr;

_declspec(naked) void ReadFontHeightDetour()
{
    _asm
    {
        MOV     EBP, 0x004385C0     // S_GetVal
        MOV     EDI, 1
        PUSH    ESI
        MOV     [ESI+0x5D8], EDI
        MOV     [ESI+0x1614], EDI
        CALL    EBP
        ADD     ESP, 4
        POP     EDI
        MOV     [ESI+0x0C], EAX     // EAX
        POP     ESI
        POP     EBP
        ADD     ESP, 0x100
        RETN
    }
}


/* Hook sub_438750 + A08 */
static PVOID pReadFontWidthNextAddr;

_declspec(naked) void ReadFontWidthDetour()
{
    _asm
    {
        MOV     EBP, 0x004385C0     // S_GetVal
        PUSH    ESI
        CALL    EBP
        ADD     ESP, 4
        MOV     [ESI+0x10], EAX     // EAX
        MOV     EDI, 1
        MOV     [ESI+0x5D8], EDI
        MOV     [ESI+0x1614], EDI
        POP     EDI
        POP     ESI
        POP     EBP
        ADD     ESP, 0x100
        RETN
    }
}


/* Hook 0x004212D9 */
_declspec(naked) void OpCmpLessOrEqualDetour()
{
    _asm
    {
        CMP     EAX, 0x9F
        JNE     loc_5492929
        MOV     EAX, 0xFE
    loc_5492929:
        CMP     EDI, EAX
        JBE     def_4212A2
        MOV     [ESI+0x10], ECX
    def_4212A2:
        POP     EDI
        XOR     EAX, EAX
        POP     ESI
        RETN
    }
}


//=============================================================================
// Startup Code
//=============================================================================


void InstallCharCheckPatch()
{
    PatchWrite((PVOID)(0x00429D15 + 2), (BYTE)0x81); // c >= 0x81
    PatchWrite((PVOID)(0x00429D1A + 2), (BYTE)0xFE); // c <= 0x9F
    PatchWrite((PVOID)(0x00429D1F + 2), (BYTE)0x81); // c >= 0xE0
    PatchWrite((PVOID)(0x00429D24 + 2), (BYTE)0xFE); // c <= 0xFC

    PatchWrite((PVOID)(0x0043ABDF + 2), (BYTE)0x81); // c >= 0x81
    PatchWrite((PVOID)(0x0043ABE4 + 2), (BYTE)0xFE); // c <= 0x9F
    PatchWrite((PVOID)(0x0043ABE9 + 2), (BYTE)0x81); // c >= 0xE0
    PatchWrite((PVOID)(0x0043ABEE + 2), (BYTE)0xFE); // c <= 0xFC

    PatchWrite((PVOID)(0x0043ADCE + 1), (BYTE)0x81); // c >= 0x81
    PatchWrite((PVOID)(0x0043ADD2 + 1), (BYTE)0xFE); // c <= 0x9F
    PatchWrite((PVOID)(0x0043ADD6 + 1), (BYTE)0x81); // c >= 0xE0
    PatchWrite((PVOID)(0x0043ADDA + 1), (BYTE)0xFE); // c <= 0xFC

    PatchWrite((PVOID)(0x0043BDCD + 2), (BYTE)0x81); // c >= 0x81
    PatchWrite((PVOID)(0x0043BDD2 + 2), (BYTE)0xFE); // c <= 0x9F
    PatchWrite((PVOID)(0x0043BDD7 + 2), (BYTE)0x81); // c >= 0xE0
    PatchWrite((PVOID)(0x0043BDDC + 2), (BYTE)0xFE); // c <= 0xFC
}


void InstallRegistryHooks()
{
    /*************************************************************************************************************/

    PVOID pCopyRegistryPathAddr = (PVOID)(0x00408280 + 0x47);
    PDETOUR_TRAMPOLINE pCopyRegistryPathTrampoline;

    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttachEx(&pCopyRegistryPathAddr, CopyRegistryPathDetour, &pCopyRegistryPathTrampoline, NULL, NULL);
    DetourTransactionCommit();

    pCopyRegistryPathNextAddr = pCopyRegistryPathTrampoline->pbRemain;

    /*************************************************************************************************************/

    PVOID pCopyRegistryPathAddr2 = (PVOID)(0x004297D0 + 0x131F);
    PDETOUR_TRAMPOLINE pCopyRegistryPathTrampoline2;

    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttachEx(&pCopyRegistryPathAddr2, CopyRegistryPathDetour2, &pCopyRegistryPathTrampoline2, NULL, NULL);
    DetourTransactionCommit();

    pCopyRegistryPathNextAddr2 = pCopyRegistryPathTrampoline2->pbRemain;
}


void InstallResourceHooks()
{
    PVOID pLoadResourceAddr = (PVOID)(0x004297D0 + 0xE5A);
    PDETOUR_TRAMPOLINE pLoadResourceTrampoline;

    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttachEx(&pLoadResourceAddr, LoadResourceDetour, &pLoadResourceTrampoline, NULL, NULL);
    DetourTransactionCommit();

    pLoadResourceNextAddr = pLoadResourceTrampoline->pbRemain;
}


void InstallFontHooks()
{
    /*************************************************************************************************************/

    PVOID pCopyFontNameAddr = (PVOID)(0x00438750 + 0x926);
    PDETOUR_TRAMPOLINE pCopyFontNameTrampoline;

    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttachEx(&pCopyFontNameAddr, CopyFontNameDetour, &pCopyFontNameTrampoline, NULL, NULL);
    DetourTransactionCommit();

    pCopyFontNameNextAddr = pCopyFontNameTrampoline->pbRemain;

    /*************************************************************************************************************/

    PVOID pReadFontCharSetAddr = (PVOID)(0x00438750 + 0x980);
    PDETOUR_TRAMPOLINE pReadFontCharSetTrampoline;

    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttachEx(&pReadFontCharSetAddr, ReadFontCharSetDetour, &pReadFontCharSetTrampoline, NULL, NULL);
    DetourTransactionCommit();

    pReadFontCharSetNextAddr = pReadFontCharSetTrampoline->pbRemain;

    /*************************************************************************************************************/

    //PVOID pReadFontHeightAddr = (PVOID)(0x00438750 + 0x9E1);
    //PDETOUR_TRAMPOLINE pReadFontHeightTrampoline;

    //DetourUpdateThread(GetCurrentThread());
    //DetourTransactionBegin();
    //DetourAttachEx(&pReadFontHeightAddr, ReadFontHeightDetour, &pReadFontHeightTrampoline, NULL, NULL);
    //DetourTransactionCommit();

    //pReadFontHeightNextAddr = pReadFontHeightTrampoline->pbRemain;

    /*************************************************************************************************************/

    //PVOID pReadFontWidthAddr = (PVOID)(0x00438750 + 0xA08);
    //PDETOUR_TRAMPOLINE pReadFontWidthTrampoline;

    //DetourUpdateThread(GetCurrentThread());
    //DetourTransactionBegin();
    //DetourAttachEx(&pReadFontWidthAddr, ReadFontWidthDetour, &pReadFontWidthTrampoline, NULL, NULL);
    //DetourTransactionCommit();

    //pReadFontWidthNextAddr = pReadFontWidthTrampoline->pbRemain;
}


void InstallPatches()
{
    InstallCharCheckPatch();
}


void InstallHooks()
{
    IATHook(NULL, "USER32.DLL", "MessageBoxA", MineMessageBoxA);

    InstallRegistryHooks();
    InstallResourceHooks();
    InstallFontHooks();

    //InlineHook(pfnRioLoadFile, RioLoadFile);
    InlineHook(pfnRioLoadFileEx, RioLoadFileEx);
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
