// dllmain.cpp : 定义 DLL 应用程序的入口点。
// Base AQUA [SORAHANE]
#include "framework.h"
#include "tp_stub.h"
#include "compact_enc_det/compact_enc_det.h"

// 早期kr2会出现问题，打开这个hook可以解决。
auto pfnGetProcAddress = GetProcAddress;
FARPROC WINAPI HookGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
    if (!strcmp(lpProcName, "GetSystemWow64DirectoryA"))
        return NULL;
    return pfnGetProcAddress(hModule, lpProcName);
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
    /*
    if (!Hook(pfnGetProcAddress, HookGetProcAddress)) {
        E(L"Hook GetProcAddress Error");
        return FALSE;
    }
    */
    ///*
    if (!Hook(pfnMultiByteToWideChar, HookMultiByteToWideChar)) {
        E(L"Hook MultiByteToWideChar Error");
        return FALSE;
    }
    //*/
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// pfnTVPCreateStream搜特征码 55 8B EC 81 C4 ?? ?? ?? ?? 53 56 57 89 95 ?? ?? ?? ?? 89 85 ?? ?? ?? ?? B8 ?? ?? ?? ?? C7 85 ?? ?? ?? ?? ?? ?? ?? ?? 89 65 80 89 85 ?? ?? ?? ?? 66 C7 45 ?? ?? ?? 33 D2 89 55 90 64 8B 0D ?? ?? ?? ?? 89 8D ?? ?? ?? ?? 8D 85 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 66 C7 45 ?? ?? ?? 8B 95 ?? ?? ?? ?? 8B 85 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 95 ?? ?? ?? ?? 64 89 15 ?? ?? ?? ?? E9 ?? ?? ?? ?? 6A 23 8B 8D ?? ?? ?? ?? 83 39 00 74 20 8B 85 ?? ?? ?? ?? 8B 00 85 C0 75 04 33 D2
// TVPFunctionExporter地址在TVPGetFunctionExporter里面，return的地址。

//PVOID pfnTVPCreateStream = (PVOID)(BaseAddr + 0x1FFB3C); // AQUA
PVOID pfnTVPCreateStream = (PVOID)(BaseAddr + 0x1FFB30);
//iTVPFunctionExporter* TVPFunctionExporter = (iTVPFunctionExporter*)(BaseAddr + 0x2F98E4); // AQUA
iTVPFunctionExporter* TVPFunctionExporter = (iTVPFunctionExporter*)(BaseAddr + 0x2F98C0);

BOOL inited = FALSE;

typedef void* (_cdecl* tKrkrCdeclNewProc)(size_t);
PVOID AddrOfKrkrNew = (PVOID)(BaseAddr + 0x16016C);
tKrkrCdeclNewProc pfnKrkrNew = (tKrkrCdeclNewProc)AddrOfKrkrNew;


typedef void(_cdecl* tKrkrCdeclFreeProc)(void*);
PVOID AddrOfKrkrFree = (PVOID)(BaseAddr + 0x1607BC);
tKrkrCdeclFreeProc pfnKrkrFree = (tKrkrCdeclFreeProc)AddrOfKrkrFree;

void* KrkrNew(size_t count)
{
    // TODO: Krkr2
    return pfnKrkrNew(count);
}


void KrkrFree(void* ptr)
{
    // TODO: Krkr2
    return pfnKrkrFree(ptr);
}



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


class tTJSBinaryStream
{
private:
public:
    virtual tjs_uint64 TJS_INTF_METHOD Seek(tjs_int64 offset, tjs_int whence) = 0;
    virtual tjs_uint TJS_INTF_METHOD Read(void* buffer, tjs_uint read_size) = 0;
    virtual tjs_uint TJS_INTF_METHOD Write(const void* buffer, tjs_uint write_size) = 0;
    virtual tjs_uint64 TJS_INTF_METHOD GetSize() = 0;
    virtual void TJS_INTF_METHOD SetEndOfStorage() = 0;
    virtual ~tTJSBinaryStream() { }
};

/*
class MyStream : public tTJSBinaryStream
{
    typedef tTJSBinaryStream inherited;

private:
    IStream* Stream;

public:
    static void* operator new(size_t count)
    {
        cout << "new" << endl;
        return KrkrNew(count);
    }


    static void operator delete(void* ptr)
    {
        cout << "delete" << endl;
        return KrkrFree(ptr);
    }

    MyStream(IStream* ref)
    {
        Stream = ref;
        Stream->AddRef();
    }

    ~MyStream()
    {
        Stream->Release();
    }

    MyStream(const MyStream&) = delete;
    MyStream& operator=(const MyStream&) = delete;


    tjs_uint64 TJS_INTF_METHOD Seek(tjs_int64 offset, tjs_int whence) override
    {
        DWORD origin;

        switch (whence)
        {
        case TJS_BS_SEEK_SET:			origin = STREAM_SEEK_SET;		break;
        case TJS_BS_SEEK_CUR:			origin = STREAM_SEEK_CUR;		break;
        case TJS_BS_SEEK_END:			origin = STREAM_SEEK_END;		break;
        default:						origin = STREAM_SEEK_SET;		break;
        }

        LARGE_INTEGER ofs;
        ULARGE_INTEGER newpos;

        ofs.QuadPart = 0;
        HRESULT hr = Stream->Seek(ofs, STREAM_SEEK_CUR, &newpos);
        bool orgpossaved;
        LARGE_INTEGER orgpos;
        if (FAILED(hr))
        {
            orgpossaved = false;
        }
        else
        {
            orgpossaved = true;
            *(LARGE_INTEGER*)&orgpos = *(LARGE_INTEGER*)&newpos;
        }

        ofs.QuadPart = offset;

        hr = Stream->Seek(ofs, origin, &newpos);
        if (FAILED(hr))
        {
            if (orgpossaved)
            {
                Stream->Seek(orgpos, STREAM_SEEK_SET, &newpos);
            }
            else
            {
                E(L"TVPSeekError");
            }
        }

        return newpos.QuadPart;
    }

    tjs_uint TJS_INTF_METHOD Read(void* buffer, tjs_uint read_size) override {
        ULONG cb = read_size;
        ULONG read;
        HRESULT hr = Stream->Read(buffer, cb, &read);
        if (FAILED(hr)) read = 0;
        return read;
    }

    tjs_uint TJS_INTF_METHOD Write(const void* buffer, tjs_uint write_size) override {
        ULONG cb = write_size;
        ULONG written;
        HRESULT hr = Stream->Write(buffer, cb, &written);
        if (FAILED(hr)) written = 0;
        return written;
    }

    tjs_uint64 TJS_INTF_METHOD GetSize() override {
        HRESULT hr;
        STATSTG stg;

        hr = Stream->Stat(&stg, STATFLAG_NONAME);
        if (FAILED(hr))
        {
            //return inherited::GetSize(); // use default routine
            E(L"GetSizeError");
            return 0;
        }

        return stg.cbSize.QuadPart;
    }

    void TJS_INTF_METHOD SetEndOfStorage() override {
        ULARGE_INTEGER pos;
        pos.QuadPart = Seek(0, TJS_BS_SEEK_CUR);
        HRESULT hr = Stream->SetSize(pos);
        if (FAILED(hr)) E(L"TVPTruncateError");
    }
};
*/

std::wstring GetKrkrFileName(LPCWSTR Name){
    std::wstring Info(Name);

    if (Info.find_last_of(L">") != std::wstring::npos)
        Info = Info.substr(Info.find_last_of(L">") + 1, std::wstring::npos);

    if (Info.find_last_of(L"/") != std::wstring::npos)
        Info = Info.substr(Info.find_last_of(L"/") + 1, std::wstring::npos);

    return Info;
}

void FileNameToLower(std::wstring& FileName){
    std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

const tjs_char* TJSStringGetPtr(tTJSString* s){
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

BYTE* STARTUP_TJS = (BYTE*)"\x2F\x2F\x20\x73\x74\x61\x72\x74\x75\x70\x2E\x74\x6A\x73\x20\x2D\x20\x83\x58\x83\x5E\x81\x5B\x83\x67\x83\x41\x83\x62\x83\x76\x83\x58\x83\x4E\x83\x8A\x83\x76\x83\x67\x0D\x0A\x2F\x2F\x20\x43\x6F\x70\x79\x72\x69\x67\x68\x74\x20\x28\x43\x29\x20\x32\x30\x30\x31\x2C\x20\x57\x2E\x44\x65\x65\x20\x20\x89\xFC\x95\xCF\x81\x45\x94\x7A\x95\x7A\x82\xCD\x8E\xA9\x97\x52\x82\xC5\x82\xB7\x0D\x0A\x0D\x0A\x0D\x0A\x2F\x2F\x20\x82\xB1\x82\xCC\x83\x58\x83\x4E\x83\x8A\x83\x76\x83\x67\x82\xCD\x88\xEA\x94\xD4\x8D\xC5\x8F\x89\x82\xC9\x8E\xC0\x8D\x73\x82\xB3\x82\xEA\x82\xE9\x83\x58\x83\x4E\x83\x8A\x83\x76\x83\x67\x82\xC5\x82\xB7\x0D\x0A\x53\x63\x72\x69\x70\x74\x73\x2E\x65\x78\x65\x63\x53\x74\x6F\x72\x61\x67\x65\x28\x22\x73\x79\x73\x74\x65\x6D\x2F\x49\x6E\x69\x74\x69\x61\x6C\x69\x7A\x65\x2E\x74\x6A\x73\x22\x29\x3B\x20\x2F\x2F\x20\x73\x79\x73\x74\x65\x6D\x2F\x49\x6E\x69\x74\x69\x61\x6C\x69\x7A\x65\x2E\x74\x6A\x73\x20\x82\xF0\x8E\xC0\x8D\x73\x0D\x0A";

tTJSBinaryStream* _HookTVPCreateStream(ttstr* name, tjs_uint32 flags) {

    tTJSBinaryStream* Stream = TVPCreateStreamCallback(name, flags);
    //IStream* IStream;

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
        cout << wtoc(fnm.c_str(), 936) << endl;

        wstring wfnm = GetKrkrFileName(psz);
        FileNameToLower(wfnm);
        wstring NewFileName = L"#Project\\" + wfnm;

        FILE* fp = _wfopen(NewFileName.c_str(), L"rb");
        if (fp) {

            fseek(fp, 0, SEEK_END);
            DWORD FileSize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            BYTE* FileBuff = (BYTE*)KrkrNew(FileSize);
            fread(FileBuff, 1, FileSize, fp);
            fclose(fp);


            cout << "copy" << endl;
            Stream->Seek(0, TJS_BS_SEEK_SET);
            Stream->Write(FileBuff, FileSize);
            cout << "end" << endl;
            return Stream;
        }
        else
            return Stream;
        /*
        if (!wcscmp(wfnm.c_str(), L"startup.tjs")) {
            cout << "copying" << endl;
            Stream->Write(STARTUP_TJS, 233);
            cout << "end" << endl;
            return Stream;
        }
        else
            return Stream;
        */
    }
    else {
        return Stream;
    }

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

BOOL TVPHook(){
    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttach(&pfnTVPCreateStream, HookTVPCreateStream);
    if (DetourTransactionCommit() != NOERROR) {
        E(L"TVP Hook Error.");
        return FALSE;
    }
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
        if (!APIHook() || !TVPHook()) {
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

