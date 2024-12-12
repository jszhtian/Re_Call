// dllmain.cpp

#include "util.h"
#include "path.h"
#include "log.h"
#include "pe.h"
#include "stringhelper.h"
#include "detours.h"
#include "tp_stub.h"


static HMODULE g_hEXE;
static HMODULE g_hDLL;

static std::wstring g_exePath;
static std::wstring g_dllPath;

static Log::Logger g_logger;


template<class T>
void InlineHook(T& OriginalFunction, T DetourFunction)
{
    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourAttach(&(PVOID&)OriginalFunction, (PVOID&)DetourFunction);
    DetourTransactionCommit();
}


template<class T>
void UnInlineHook(T& OriginalFunction, T DetourFunction)
{
    DetourUpdateThread(GetCurrentThread());
    DetourTransactionBegin();
    DetourDetach(&(PVOID&)OriginalFunction, (PVOID&)DetourFunction);
    DetourTransactionCommit();
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
HRESULT _stdcall HookV2Link(iTVPFunctionExporter* exporter)
{
    try
    {
        TVPFunctionExporter = exporter;

        g_logger.WriteLine(L"Caught iTVPFunctionExporter(%p)", exporter);

        TVPInitImportStub(exporter);

        g_logger.WriteLine(L"Stub initialized");
    }
    catch (const std::exception& e)
    {
        g_logger.WriteLineAnsi(CP_ACP, "Failed to initialize stub, %s", e.what());
    }

    HRESULT result = pfnV2Link(exporter);

    try
    {
        // We don't need it anymore
        UnInlineHook(pfnV2Link, HookV2Link);
    }
    catch (const std::exception&)
    {
    }

    return result;
}


// Original
auto pfnGetProcAddress = GetProcAddress;
// Hooked
FARPROC WINAPI HookGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    FARPROC result = pfnGetProcAddress(hModule, lpProcName);

    try
    {
        if (result)
        {
            // Ignore function imported by ordinal
            if (HIWORD(lpProcName) != 0)
            {
                if (strcmp(lpProcName, "V2Link") == 0)
                {
                    pfnV2Link = (tTVPV2LinkProc)result;

                    // We don't need it anymore
                    UnInlineHook(pfnGetProcAddress, HookGetProcAddress);

                    auto path = Util::GetModulePathW(hModule);
                    auto name = Path::GetFileName(path);

                    g_logger.WriteLine(L"Caught V2Link(%p) from %s(%p)", result, name.c_str(), hModule);

                    // Now hook V2Link to get iTVPFunctionExporter

                    InlineHook(pfnV2Link, HookV2Link);

                    g_logger.WriteLine(L"Hook V2Link installed");
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        g_logger.WriteLineAnsi(CP_ACP, "Failed to hook V2Link, %s", e.what());
    }

    return result;
}


tjs_char* TVPGetStringPtr(ttstr* s)
{
    tTJSVariantString_S* v = *(tTJSVariantString_S **)s;
    return v->LongString ? v->LongString : v->ShortString;
}


class tTJSBinaryStream
{
public:
    virtual tjs_uint64 TJS_INTF_METHOD Seek(tjs_int64 offset, tjs_int whence) = 0;
    virtual tjs_uint TJS_INTF_METHOD Read(void* buffer, tjs_uint read_size) = 0;
    // virtual tjs_uint TJS_INTF_METHOD Write(const void* buffer, tjs_uint write_size) = 0;
    // virtual void TJS_INTF_METHOD SetEndOfStorage() = 0;
    // virtual tjs_uint64 TJS_INTF_METHOD GetSize() = 0;
    // virtual ~tTJSBinaryStream() { }
};


// 
// Version : KRKRZ (MSVC)
// 
#define TVPCREATESTREAM_SIG "\x55\x8B\xEC\x6A\xFF\x68\x2A\x2A\x2A\x2A\x64\xA1\x2A\x2A\x2A\x2A\x50\x83\xEC\x5C\x53\x56\x57\xA1\x2A\x2A\x2A\x2A\x33\xC5\x50\x8D\x45\xF4\x64\xA3\x2A\x2A\x2A\x2A\x89\x65\xF0\x89\x4D\xEC\xC7\x45\x2A\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x8B\x4D\xF4\x64\x89\x0D\x2A\x2A\x2A\x2A\x59\x5F\x5E\x5B\x8B\xE5\x5D\xC3"
#define TVPCREATESTREAM_SIG_LEN ( sizeof(TVPCREATESTREAM_SIG) - 1 )
// Prototype
typedef tTJSBinaryStream* (_fastcall* tKrkrzMsvcFastCallTVPCreateStreamProc)(ttstr*, tjs_uint32);


// Original
tKrkrzMsvcFastCallTVPCreateStreamProc pfnKrkrzMsvcFastCallTVPCreateStreamProc;
// Hooked
tTJSBinaryStream* _fastcall KrkrzMsvcFastCallTVPCreateStream(ttstr* name, tjs_uint32 flags)
{
    tTJSBinaryStream* result = pfnKrkrzMsvcFastCallTVPCreateStreamProc(name, flags);

    if (result)
    {
        tjs_char* pszname = TVPGetStringPtr(name);

        if (StringHelper::StartsWith(pszname, L"file://") == false)
        {
            g_logger.WriteLine(L"TVPCreateStream(\"%s\")", pszname);

            // Get the length of stream
            //result->Seek(0, TJS_BS_SEEK_END);
            //tjs_uint length = (tjs_uint)result->Seek(0, TJS_BS_SEEK_CUR);
            //result->Seek(0, TJS_BS_SEEK_SET);

            // Read stream
            //result->Read(buf, length);
        }
    }

    return result;
}


void InstallHooks()
{
    PVOID base = PE::GetModuleBase(g_hEXE);
    DWORD size = PE::GetModuleSize(g_hEXE);

    g_logger.WriteLine(L"Searching for TVPCreateStream");

    PVOID pfnTVPCreateStream = PE::SearchPattern(base, size, TVPCREATESTREAM_SIG, TVPCREATESTREAM_SIG_LEN);

    if (pfnTVPCreateStream)
    {
        pfnKrkrzMsvcFastCallTVPCreateStreamProc = (tKrkrzMsvcFastCallTVPCreateStreamProc)pfnTVPCreateStream;

        g_logger.WriteLine(L"Caught TVPCreateStream(%p)", pfnTVPCreateStream);

        InlineHook(pfnKrkrzMsvcFastCallTVPCreateStreamProc, KrkrzMsvcFastCallTVPCreateStream);

        g_logger.WriteLine(L"Hook TVPCreateStream installed");
    }
    else
    {
        g_logger.WriteLine(L"Signature for TVPCreateStream is not found");
    }

    // Get iTVPFunctionExporter by hooking V2Link
    //InlineHook(pfnGetProcAddress, HookGetProcAddress);
}


void OnStartup()
{
    std::wstring exePath = Util::GetModulePathW(g_hEXE);
    std::wstring dllPath = Util::GetModulePathW(g_hDLL);
    std::wstring logPath = Path::ChangeExtension(dllPath, L"log");

    Util::WriteDebugMessage(L"[KrkrDump] EXE Path = \"%s\"", exePath.c_str());
    Util::WriteDebugMessage(L"[KrkrDump] DLL Path = \"%s\"", dllPath.c_str());
    Util::WriteDebugMessage(L"[KrkrDump] Log Path = \"%s\"", logPath.c_str());

    g_logger.Open(logPath.c_str());

    g_logger.WriteLine(L"Startup");

    g_logger.WriteLine(L"Game Executable Path = \"%s\"", exePath.c_str());

    g_exePath = std::move(exePath);
    g_dllPath = std::move(dllPath);

    // Started

    try
    {
        InstallHooks();
        g_logger.WriteLine(L"Hooks Installed");
    }
    catch (const std::exception&)
    {
        g_logger.WriteLine(L"Failed to install hooks");
    }
}


void OnShutdown()
{
    g_logger.WriteLine(L"Shutdown");
    g_logger.Close();
}


// Create export function table.
extern "C" __declspec(dllexport) BOOL CreatePlugin() { return TRUE; }


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            g_hEXE = GetModuleHandle(NULL);
            g_hDLL = hModule;
            OnStartup();
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            break;
        }
        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            OnShutdown();
            break;
        }
    }

    return TRUE;
}
