//-------------------------------------------------------------------------------------------------------------------------------
// Written By MEGUMI
//-------------------------------------------------------------------------------------------------------------------------------

#include "pch.h"
#include <windows.h>
#include <shlwapi.h>
#include <winternl.h>
#include <strsafe.h>
#include "detours.h"
#include <codecvt>
#include <filesystem>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <thread>


#pragma warning(push)
#pragma warning(disable:4005)
#include <ntstatus.h>
#pragma warning(pop)


//-------------------------------------------------------------------------------------------------------------------------------
// Std Types
//-------------------------------------------------------------------------------------------------------------------------------


using std::endl;
using std::string;
using std::wstring;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::filesystem::directory_iterator;
using std::filesystem::recursive_directory_iterator;
using std::filesystem::path;
using std::filesystem::is_directory;
using std::filesystem::file_size;
using std::filesystem::copy_file;
using std::filesystem::remove;
using std::list;
using std::unique_ptr;
using std::make_unique;
using std::wstringstream;
using std::wstring_convert;
using std::codecvt_utf8;


//-------------------------------------------------------------------------------------------------------------------------------
// Global Types
//-------------------------------------------------------------------------------------------------------------------------------


struct FileRecord
{
    DWORD       Type;
    LONGLONG    Size;
    WCHAR       Path[MAX_PATH];
    WCHAR       Hook[32];
};


//-------------------------------------------------------------------------------------------------------------------------------
// Global Constants
//-------------------------------------------------------------------------------------------------------------------------------


constexpr DWORD PIPE_BUFFER_SIZE = 0x4000;


//-------------------------------------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------------------------------------


HANDLE gLogFile = INVALID_HANDLE_VALUE;
HMODULE gDll, gApp;
path gDllPath, gAppPath;
HANDLE gPipeRead, gPipeWrite;
list<FileRecord> gFileRecords;


//-------------------------------------------------------------------------------------------------------------------------------
// Common Utilities
//-------------------------------------------------------------------------------------------------------------------------------


class Logger
{
public:
    Logger()
    {
        // TODO: Add timestamp
    }

    ~Logger()
    {
        // Make sure the file is open.
        if (gLogFile == INVALID_HANDLE_VALUE)
        {
            gLogFile = CreateFileW(L"dev.log", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        }

        if (gLogFile != INVALID_HANDLE_VALUE)
        {
            // String buffer.
            Buf.flush();
            auto wstr = Buf.str();

            // do nothing.
            if (wstr.length() == 0)
                return;

            // Convert UCS2 to UTF8
            wstring_convert<codecvt_utf8<wchar_t>> cvt;
            auto u8str = cvt.to_bytes(wstr);

            // Write to file.
            SetFilePointer(gLogFile, 0, NULL, FILE_END);
            DWORD NumberOfBytesWritten;
            WriteFile(gLogFile, u8str.c_str(), u8str.length(), &NumberOfBytesWritten, NULL);
            FlushFileBuffers(gLogFile);
        }
    }

    template<class T>
    wstringstream& operator<<(const T& Val)
    {
        Buf << Val;
        return Buf;
    }

    wstringstream& operator<<(wstringstream&(*pfn)(wstringstream&))
    {
        return pfn(Buf);
    }

private:
    wstringstream Buf;
};


#define qDebug Logger()


BOOL HandlePathTest(HANDLE File)
{
    WCHAR Path[MAX_PATH] {};

    if (GetFinalPathNameByHandleW(File, Path, _countof(Path), FILE_NAME_NORMALIZED) == 0)
        return FALSE;

    return !!Path[0];
}


//-------------------------------------------------------------------------------------------------------------------------------
// Kernel Utilities
//-------------------------------------------------------------------------------------------------------------------------------


BOOLEAN NTAPI RtlDosPathNameToNtPathName_U(
    IN PCWSTR DosFileName,
    OUT PUNICODE_STRING NtFileName,
    OUT PWSTR* FilePart,
    OUT PVOID RelativeName);   // PRTL_RELATIVE_NAME_U


decltype(RtlInitUnicodeString)* pfnRtlInitUnicodeString;
decltype(RtlDosPathNameToNtPathName_U)* pfnRtlDosPathNameToNtPathName_U;
decltype(NtCreateFile)* pfnNtCreateFile;
decltype(NtClose)* pfnNtClose;


void InitNtdll()
{
    HMODULE NtDll = GetModuleHandleW(L"ntdll.dll");

    *(FARPROC*)&pfnRtlInitUnicodeString = GetProcAddress(NtDll, "RtlInitUnicodeString");
    *(FARPROC*)&pfnRtlDosPathNameToNtPathName_U = GetProcAddress(NtDll, "RtlDosPathNameToNtPathName_U");
    *(FARPROC*)&pfnNtCreateFile = GetProcAddress(NtDll, "NtCreateFile");
    *(FARPROC*)&pfnNtClose = GetProcAddress(NtDll, "NtClose");
}


void BackupRealNtCreateFile()
{
    // WIN10 X64
    constexpr BYTE OrgCode[] = { 0xB8, 0x55, 0x00, 0x00, 0x00, 0xBA, 0xD0, 0x60, 0x99, 0x77, 0xFF, 0xD2, 0xC2, 0x2C, 0x00, 0x90 };
    constexpr DWORD CodeSize = sizeof(OrgCode);

    // Temp store it
    PBYTE OldThunk = (PBYTE)pfnNtCreateFile;

    // Reset
    pfnNtCreateFile = nullptr;

    DWORD OldProtect;
    if (VirtualProtect(OldThunk, CodeSize, PAGE_EXECUTE_READWRITE, &OldProtect))
    {
        // Check if the function is hooked.
        if (OldThunk[0] != 0xE9)
        {
            LPVOID NewThunk = VirtualAlloc(NULL, CodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

            if (NewThunk)
            {
                // Copy code
                memcpy(NewThunk, OldThunk, CodeSize);

                // Store
                *(LPVOID*)&pfnNtCreateFile = NewThunk;

                qDebug << L"Backup >> NtCreateFile" << endl;
            }
        }

        // Restore protection
        VirtualProtect(OldThunk, CodeSize, OldProtect, &OldProtect);
    }
}


NTSTATUS NTAPI RealNtCreateFile(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer,
    IN ULONG EaLength)
{
    if (pfnNtCreateFile)
    {
        return pfnNtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock,
            AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
    }

    return STATUS_INVALID_HANDLE;
}


BOOL IsFileExists(LPCWSTR lpFileName)
{
    HANDLE            File;
    UNICODE_STRING    Name;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK   IoStatusBlock;

    if (pfnRtlDosPathNameToNtPathName_U(lpFileName, &Name, NULL, NULL) == FALSE)
        return FALSE;

    InitializeObjectAttributes(&ObjectAttributes, &Name, OBJ_CASE_INSENSITIVE, NULL, NULL);

    NTSTATUS Status = RealNtCreateFile(&File, FILE_GENERIC_READ, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_RANDOM_ACCESS | FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

    if (Status == STATUS_SUCCESS)
    {
        pfnNtClose(File);
        return TRUE;
    }

    if (Status == STATUS_SHARING_VIOLATION)
        return TRUE;

    //qDebug << L"NtCreateFile >> " << (PVOID)Status << endl;

    return FALSE;
}


//-------------------------------------------------------------------------------------------------------------------------------
// Hook Utilities
//-------------------------------------------------------------------------------------------------------------------------------


template<class T>
void InlineHook(T& OriginalFunction, T DetourFunction)
{
    DetourTransactionBegin();
    DetourAttach(&(PVOID&)OriginalFunction, (PVOID&)DetourFunction);
    DetourTransactionCommit();
}


template<class T>
void UnInlineHook(T& OriginalFunction, T DetourFunction)
{
    DetourTransactionBegin();
    DetourDetach(&(PVOID&)OriginalFunction, (PVOID&)DetourFunction);
    DetourTransactionCommit();
}


//-------------------------------------------------------------------------------------------------------------------------------
// Hook Variables
//-------------------------------------------------------------------------------------------------------------------------------


auto pfnCreateFileA = CreateFileA;
auto pfnCreateFileW = CreateFileW;


//-------------------------------------------------------------------------------------------------------------------------------
// Hook Codes
//-------------------------------------------------------------------------------------------------------------------------------


HANDLE WINAPI MineCreateFileA(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile)
{
    HANDLE File = pfnCreateFileA(lpFileName, dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

    if (File == INVALID_HANDLE_VALUE)
        return File;

    // Ignore the pipes and other drives. This will cause some crashes.
    if (!strncmp(lpFileName, "\\", 1))
        return File;

    if (GetFileType(File) != FILE_TYPE_DISK)
        return File;

    if (PathIsDirectoryA(lpFileName))
        return File;

    WCHAR Name[MAX_PATH] {};
    SHAnsiToUnicode(lpFileName, Name, _countof(Name));

    WCHAR Path[MAX_PATH] {};
    GetFullPathNameW(Name, _countof(Path), Path, NULL);

    LARGE_INTEGER Size {};
    GetFileSizeEx(File, &Size);

    FileRecord Record;

    Record.Type = is_directory(Path);
    Record.Size = Size.QuadPart;
    wcsncpy_s(Record.Path, Path, _TRUNCATE);
    wcscpy_s(Record.Hook, L"CreateFileA");

    gFileRecords.emplace_back(Record);

    WCHAR SizeText[32];
    StrFormatByteSizeW(Record.Size, SizeText, _countof(SizeText));

    if (IsFileExists(Path) == FALSE)
        qDebug << L"VirtualFile >> \"" << Path << L"\" " << SizeText << endl;

    return File;
}


HANDLE WINAPI MineCreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile)
{
    HANDLE File = pfnCreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

    if (File == INVALID_HANDLE_VALUE)
        return File;

    // Ignore the pipes and other drives. This will cause some crashes.
    if (!wcsncmp(lpFileName, L"\\", 1))
        return File;

    if (GetFileType(File) != FILE_TYPE_DISK)
        return File;

    if (PathIsDirectoryW(lpFileName))
        return File;

    WCHAR Path[MAX_PATH] {};
    GetFullPathNameW(lpFileName, _countof(Path), Path, NULL);

    LARGE_INTEGER Size {};
    GetFileSizeEx(File, &Size);

    FileRecord Record;

    Record.Type = is_directory(Path);
    Record.Size = Size.QuadPart;
    wcsncpy_s(Record.Path, Path, _TRUNCATE);
    wcscpy_s(Record.Hook, L"CreateFileW");

    gFileRecords.emplace_back(Record);

    WCHAR SizeText[32];
    StrFormatByteSizeW(Record.Size, SizeText, _countof(SizeText));

    if (IsFileExists(Path) == FALSE)
        qDebug << L"VirtualFile >> \"" << Path << L"\" " << SizeText << endl;

    return File;
}


//-------------------------------------------------------------------------------------------------------------------------------
// Worker Code
//-------------------------------------------------------------------------------------------------------------------------------


void Worker()
{
    qDebug << L"Worker is started." << endl;

    InlineHook(pfnCreateFileA, MineCreateFileA);
    InlineHook(pfnCreateFileW, MineCreateFileW);

    qDebug << L"Hook is installed." << endl;

    qDebug << L"Pipe is ready." << endl;

    while (true)
    {
        static BYTE Data[PIPE_BUFFER_SIZE];
        static BOOL Exit;

        DWORD NumberOfBytesRead;
        if (ReadFile(gPipeRead, Data, PIPE_BUFFER_SIZE, &NumberOfBytesRead, NULL) == FALSE)
        {
            qDebug << L"ERROR: Read pipe failed." << endl;
            continue;
        }

        if (NumberOfBytesRead <= sizeof(DWORD))
        {
            qDebug << L"WARN: Invalid data ignored." << endl;
            continue;
        }

        typedef struct {
            DWORD Cmd;
            PVOID Arg;
        } *PCMD;

        PCMD Cmd = (PCMD)Data;

        try
        {
            switch (Cmd->Cmd)
            {
                case 1:
                {
                    qDebug << L"Command >> IterateDirectory" << endl;

                    typedef struct {
                        WCHAR Path[MAX_PATH];
                    } *PARG;

                    PARG Arg = (PARG)&Cmd->Arg;

                    Arg->Path[_countof(Arg->Path) - 1] = 0;
                    path Path = Arg->Path;

                    typedef struct {
                        DWORD Cmd;
                        WCHAR Path[MAX_PATH];
                    } DATA;

                    unique_ptr<DATA> Data = make_unique<DATA>();
                    Data->Cmd = 1;

                    for (auto& Entry : directory_iterator(Path))
                    {
                        wcsncpy_s(Data->Path, Entry.path().wstring().c_str(), _TRUNCATE);

                        DWORD NumberOfBytesWritten;
                        WriteFile(gPipeWrite, Data.get(), sizeof(*Data), &NumberOfBytesWritten, NULL);
                    }

                    break;
                }
                case 2:
                {
                    qDebug << L"Command >> RecursiveIterateDirectory" << endl;

                    typedef struct {
                        WCHAR Path[MAX_PATH];
                    } *PARG;

                    PARG Arg = (PARG)&Cmd->Arg;

                    Arg->Path[_countof(Arg->Path) - 1] = 0;
                    path Path = Arg->Path;

                    typedef struct {
                        DWORD Cmd;
                        WCHAR Path[MAX_PATH];
                    } DATA;

                    unique_ptr<DATA> Data = make_unique<DATA>();
                    Data->Cmd = 2;

                    for (auto& Entry : recursive_directory_iterator(Path))
                    {
                        wcsncpy_s(Data->Path, Entry.path().wstring().c_str(), _TRUNCATE);

                        DWORD NumberOfBytesWritten;
                        WriteFile(gPipeWrite, Data.get(), sizeof(*Data), &NumberOfBytesWritten, NULL);
                    }

                    break;
                }
                case 3:
                {
                    qDebug << L"Command >> GetFileRecord" << endl;

                    typedef struct {
                        DWORD Cmd;
                        DWORD Type;
                        LONGLONG Size;
                        WCHAR Path[MAX_PATH];
                        WCHAR Hook[32];
                    } DATA;

                    unique_ptr<DATA> Data = make_unique<DATA>();
                    Data->Cmd = 3;

                    for (auto& Entry : gFileRecords)
                    {
                        Data->Type = Entry.Type;
                        Data->Size = Entry.Size;
                        wcsncpy_s(Data->Path, Entry.Path, _TRUNCATE);
                        wcsncpy_s(Data->Hook, Entry.Hook, _TRUNCATE);

                        DWORD NumberOfBytesWritten;
                        WriteFile(gPipeWrite, Data.get(), sizeof(*Data), &NumberOfBytesWritten, NULL);
                    }

                    break;
                }
                case 4:
                {
                    qDebug << L"Command >> DumpFile" << endl;

                    typedef struct {
                        WCHAR SrcPath[MAX_PATH];
                        WCHAR DstPath[MAX_PATH];
                    } *PARG;

                    PARG Arg = (PARG)&Cmd->Arg;

                    Arg->SrcPath[_countof(Arg->SrcPath) - 1] = 0;
                    path SrcPath = Arg->SrcPath;
                    Arg->DstPath[_countof(Arg->DstPath) - 1] = 0;
                    path DstPath = Arg->DstPath;

                    if (copy_file(SrcPath, DstPath) == false)
                        qDebug << L"ERROR: Dump " << SrcPath << " To " << DstPath << L" Failed" << endl;

                    break;
                }
                default:
                {
                    qDebug << L"ERROR: Invalid command." << endl;
                    break;
                }
            }
        }
        catch (const std::exception& e)
        {
            qDebug << L"ERROR: Command Execution ( " << e.what() << L" )" << endl;
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------------------
// Initialization Code
//-------------------------------------------------------------------------------------------------------------------------------


void Start()
{
    qDebug << L"Started." << endl;

    // Wait for a while to reduce the chance of collapse.
    sleep_for(seconds(1));

    if (CreatePipe(&gPipeRead, &gPipeWrite, NULL, PIPE_BUFFER_SIZE) == FALSE)
    {
        qDebug << L"ERROR: Create pipe failed." << endl;
        return;
    }

    qDebug << L"Pipe R/W = " << gPipeRead << L"/" << gPipeWrite << endl;

    WCHAR Path[MAX_PATH];

    if (GetModuleFileNameW(gDll, Path, _countof(Path)) == 0)
        return;
    gDllPath = path(Path).parent_path();

    if (GetModuleFileNameW(gApp, Path, _countof(Path)) == 0)
        return;
    gAppPath = path(Path).parent_path();

    qDebug << L"DLL_PATH = " << gDllPath.wstring() << endl;
    qDebug << L"APP_PATH = " << gAppPath.wstring() << endl;

    thread t(Worker);
    t.detach();
}


//-------------------------------------------------------------------------------------------------------------------------------
// DLL Entry
//-------------------------------------------------------------------------------------------------------------------------------


BOOL WINAPI DllMain(
    _In_ HMODULE hModule,
    _In_ DWORD fdwReason,
    _In_ LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // Delete old log file.
            remove(L"dev.log");

            qDebug << L"DLL Attached." << endl;

            gDll = hModule;
            gApp = GetModuleHandleW(NULL);

            InitNtdll();
            BackupRealNtCreateFile();

            thread t(Start);
            t.detach();

            break;
        }
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
        {
            qDebug << L"DLL Detached." << endl;
            break;
        }
    }

    return TRUE;
}
