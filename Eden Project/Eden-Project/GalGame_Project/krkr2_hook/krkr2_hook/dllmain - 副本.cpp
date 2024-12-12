// dllmain.cpp : 定义 DLL 应用程序的入口点。
// Base AQUA [SORAHANE]
#include "framework.h"
#include "tp_stub.h"
#include "compact_enc_det/compact_enc_det.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// pfnTVPCreateStream搜特征码 55 8B EC 81 C4 ?? ?? ?? ?? 53 56 57 89 95 ?? ?? ?? ?? 89 85 ?? ?? ?? ?? B8 ?? ?? ?? ?? C7 85 ?? ?? ?? ?? ?? ?? ?? ?? 89 65 80 89 85 ?? ?? ?? ?? 66 C7 45 ?? ?? ?? 33 D2 89 55 90 64 8B 0D ?? ?? ?? ?? 89 8D ?? ?? ?? ?? 8D 85 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 66 C7 45 ?? ?? ?? 8B 95 ?? ?? ?? ?? 8B 85 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 95 ?? ?? ?? ?? 64 89 15 ?? ?? ?? ?? E9 ?? ?? ?? ?? 6A 23 8B 8D ?? ?? ?? ?? 83 39 00 74 20 8B 85 ?? ?? ?? ?? 8B 00 85 C0 75 04 33 D2
// TVPFunctionExporter地址在TVPGetFunctionExporter里面，return的地址。

//PVOID pfnTVPCreateStream = (PVOID)(BaseAddr + 0x1FFB3C); // AQUA
PVOID pfnTVPCreateStream = (PVOID)(BaseAddr + 0x1FFB30);
//iTVPFunctionExporter* TVPFunctionExporter = (iTVPFunctionExporter*)(BaseAddr + 0x2F98E4); // AQUA
//iTVPFunctionExporter* TVPFunctionExporter = (iTVPFunctionExporter*)(BaseAddr + 0x2F98C0);
//BOOL inited = FALSE;

class tTJSCriticalSection {
    CRITICAL_SECTION CS;
public:
    tTJSCriticalSection() { InitializeCriticalSection(&CS); }
    ~tTJSCriticalSection() { DeleteCriticalSection(&CS); }

    void Enter() { EnterCriticalSection(&CS); }
    void Leave() { LeaveCriticalSection(&CS); }
};

class tTJSCriticalSectionHolder {
    tTJSCriticalSection* Section;
public:
    tTJSCriticalSectionHolder(tTJSCriticalSection& cs) {
        Section = &cs;
        Section->Enter();
    }

    ~tTJSCriticalSectionHolder() {
        Section->Leave();
    }

};

static tTJSCriticalSection LocalCreateStreamCS;


std::wstring GetKrkrFileName(LPCWSTR Name) {
    std::wstring Info(Name);

    if (Info.find_last_of(L">") != std::wstring::npos)
        Info = Info.substr(Info.find_last_of(L">") + 1, std::wstring::npos);

    if (Info.find_last_of(L"/") != std::wstring::npos)
        Info = Info.substr(Info.find_last_of(L"/") + 1, std::wstring::npos);

    return Info;
}

void FileNameToLower(std::wstring& FileName) {
    std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

const tjs_char* TJSStringGetPtr(tTJSString* s) {
    if (!s)
        return L"";

    tTJSVariantString_S* v = *(tTJSVariantString_S**)s;

    if (!v)
        return L"";

    if (v->LongString)
        return v->LongString;

    return v->ShortString;
}

IStream* CreateLocalStream(LPCWSTR FileName) {

    tTJSCriticalSectionHolder CSHolder(LocalCreateStreamCS);

    wstring fnm = GetKrkrFileName(FileName);
    FileNameToLower(fnm);
    wstring NewFileName = L"#Project\\" + fnm;

    //cout << "Replace:" << wtoc(FileName, 936) << endl;

    IStream* pStream;
    auto hr = SHCreateStreamOnFileEx(NewFileName.c_str(), STGM_READ, 0, FALSE, NULL, &pStream);
    if (SUCCEEDED(hr)) {
        //cout << "Replace:" << wtoc(NewFileName.c_str(), 936) << endl;
        return pStream;
    }
    else {
        return NULL;
    }
}

_declspec(naked) tTJSBinaryStream* TVPCreateStreamCallback(ttstr* name, tjs_uint32 flags) {
    _asm {
        mov edx, flags;
        mov eax, name;
        call pfnTVPCreateStream;
        ret;
    }
}

tTJSBinaryStream* _HookTVPCreateStream(ttstr* name, tjs_uint32 flags) {
    tTJSBinaryStream* Stream;
    IStream* IStream;

    /*
    if (!inited) {
        TVPInitImportStub(TVPFunctionExporter);
        cout << "func list: 0x" << hex << (DWORD)TVPFunctionExporter << endl;
        inited = TRUE;
    }
    */

    if (flags == TJS_BS_READ) {
        const tjs_char* psz = TJSStringGetPtr(name);
        wstring fnm(psz);
        IStream = CreateLocalStream(fnm.c_str());
        cout << wtoc(fnm.c_str(), 936) << endl;
        if (IStream) {
            Stream = TVPCreateBinaryStreamAdapter(IStream);
            if (!Stream) {
                E(L"TVPCreateBinaryStreamAdapter Error.");
                Stream = TVPCreateStreamCallback(name, flags);
            }
            else {
                cout << "Replace:" << wtoc(fnm.c_str(), 936) << endl;
            }
        }
        else {
            Stream = TVPCreateStreamCallback(name, flags);
        }
    }
    else {
        Stream = TVPCreateStreamCallback(name, flags);
    }

    return Stream;
}

_declspec(naked) void HookTVPCreateStream() {
    _asm {
        push edx;
        push eax;
        call _HookTVPCreateStream;
        add esp, 8;
        ret;
    }
}

BOOL TVPHook() {
    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttach(&pfnTVPCreateStream, HookTVPCreateStream);
    if (DetourTransactionCommit() != NOERROR) {
        E(L"TVP Hook Error.");
        return FALSE;
    }
    return TRUE;
}

extern "C"
{
    typedef HRESULT(_stdcall* tTVPV2LinkProc)(iTVPFunctionExporter*);
    typedef HRESULT(_stdcall* tTVPV2UnlinkProc)();
}

static iTVPFunctionExporter* TVPFunctionExporter;


// Original
tTVPV2LinkProc pfnV2Link;
// Hooked
HRESULT _stdcall HookV2Link(iTVPFunctionExporter* exporter){

    TVPFunctionExporter = exporter;

    TVPInitImportStub(exporter);

    cout << "TVPFunctionExporter: 0x" << hex << (DWORD)exporter << endl;

    HRESULT ret = pfnV2Link(exporter);

    if (!UnHook(pfnV2Link, HookV2Link)) {
        E(L"Unhook V2Link Error");
        return ret;
    }

    TVPHook();

    return ret;
}

auto pfnGetProcAddress = GetProcAddress;
FARPROC WINAPI HookGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
    // 早期kr2会出现问题，打开这个hook可以解决。
    //if (!strcmp(lpProcName, "GetSystemWow64DirectoryA"))
    //    return NULL;
    FARPROC ret =  pfnGetProcAddress(hModule, lpProcName);
    if (ret) {
        if (HIWORD(lpProcName) != 0) {
            if (!strcmp(lpProcName, "V2Link")) {
                pfnV2Link = (tTVPV2LinkProc)ret;
                if (!UnHook(pfnGetProcAddress, HookGetProcAddress)) {
                    E(L"Unhook GetProcAddress Error");
                    return ret;
                }
                if (!Hook(pfnV2Link, HookV2Link)) {
                    E(L"Hook V2Link Error");
                    return ret;
                }
            }
        }
    }
    return ret;
}

// 不稳定，遇到问题建议单独测试处理
auto pfnMultiByteToWideChar = MultiByteToWideChar;
int WINAPI HookMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC){

    bool is_reliable;
    int bytes_consumed;
    Encoding encoding = CompactEncDet::DetectEncoding(lpMBS, strlen(lpMBS), nullptr, nullptr, nullptr, UNKNOWN_ENCODING, UNKNOWN_LANGUAGE, CompactEncDet::QUERY_CORPUS, true, &bytes_consumed, &is_reliable);
    UINT codepage = cp;
    if (encoding == JAPANESE_SHIFT_JIS && is_reliable){
        codepage = 932;
    }
    return pfnMultiByteToWideChar(codepage, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

BOOL APIHook(){
    ///*
    if (!Hook(pfnGetProcAddress, HookGetProcAddress)) {
        E(L"Hook GetProcAddress Error");
        return FALSE;
    }
    //*/
    ///*
    if (!Hook(pfnMultiByteToWideChar, HookMultiByteToWideChar)) {
        E(L"Hook MultiByteToWideChar Error");
        return FALSE;
    }
    //*/
    return TRUE;
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
        if (!APIHook()) {
            E(L"Hook Error.");
            ExitProcess(-1);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

