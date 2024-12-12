//-------------------------------------------------------------------------------------------------------------------------------
// Written By MEGUMI
//-------------------------------------------------------------------------------------------------------------------------------

#include "pch.h"
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
#include "detours.h"
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>
#include <iomanip>
#include <map>
#include <fstream>
#include <unordered_map>
#include "MINT.h"


//-------------------------------------------------------------------------------------------------------------------------------
// Std Types
//-------------------------------------------------------------------------------------------------------------------------------


using std::chrono::seconds;
using std::endl;
using std::exception;
using std::filesystem::copy;
using std::filesystem::copy_file;
using std::filesystem::copy_options;
using std::filesystem::directory_entry;
using std::filesystem::directory_iterator;
using std::filesystem::file_size;
using std::filesystem::is_directory;
using std::filesystem::path;
using std::filesystem::recursive_directory_iterator;
using std::filesystem::relative;
using std::filesystem::remove;
using std::function;
using std::list;
using std::make_unique;
using std::string;
using std::this_thread::sleep_for;
using std::thread;
using std::transform;
using std::unique_ptr;
using std::wstring;
using std::wstringstream;


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
// Encoding Utilities
//-------------------------------------------------------------------------------------------------------------------------------


wstring AnsiToUnicode(const string& source)
{
    if (source.length() == 0)
        return wstring();
    int length = MultiByteToWideChar(CP_ACP, 0, source.data(), source.length(), nullptr, 0);
    if (length == 0)
        return wstring();
    wstring result(length, 0);
    if (MultiByteToWideChar(CP_ACP, 0, source.data(), source.length(), result.data(), result.length()) == 0)
        return wstring();
    return result;
}


wstring Utf8ToUnicode(const string& source)
{
    if (source.length() == 0)
        return wstring();
    int length = MultiByteToWideChar(CP_UTF8, 0, source.data(), source.length(), nullptr, 0);
    if (length == 0)
        return wstring();
    wstring result(length, 0);
    if (MultiByteToWideChar(CP_UTF8, 0, source.data(), source.length(), result.data(), result.length()) == 0)
        return wstring();
    return result;
}


string UnicodeToAnsi(const wstring& source)
{
    if (source.length() == 0)
        return string();
    int length = WideCharToMultiByte(CP_ACP, 0, source.data(), source.length(), nullptr, 0, nullptr, nullptr);
    if (length == 0)
        return string();
    string result(length, 0);
    if (WideCharToMultiByte(CP_ACP, 0, source.data(), source.length(), result.data(), result.length(), nullptr, nullptr) == 0)
        return string();
    return result;
}


string UnicodeToUtf8(const wstring& source)
{
    if (source.length() == 0)
        return string();
    int length = WideCharToMultiByte(CP_UTF8, 0, source.data(), source.length(), nullptr, 0, nullptr, nullptr);
    if (length == 0)
        return string();
    string result(length, 0);
    if (WideCharToMultiByte(CP_UTF8, 0, source.data(), source.length(), result.data(), result.length(), nullptr, nullptr) == 0)
        return string();
    return result;
}


//-------------------------------------------------------------------------------------------------------------------------------
// Common Utilities
//-------------------------------------------------------------------------------------------------------------------------------

static void make_console()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    std::cout << "VFS Dumper Ver 1.11\n" << endl;
}

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
            wstring wstr = Buf.str();

            // do nothing.
            if (wstr.length() == 0)
                return;

            // Convert UTF8 bytes.
            string u8str = UnicodeToUtf8(wstr);

            // Write to file.
            SetFilePointer(gLogFile, 0, NULL, FILE_END);
            DWORD NumberOfBytesWritten;
            WriteFile(gLogFile, u8str.c_str(), u8str.length(), &NumberOfBytesWritten, NULL);
            std::cout << u8str.c_str() << std::endl;
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


wstring GetErrorMsg()
{
    DWORD Code = GetLastError();
    wstring Message(512, 0);

    if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
        Code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), Message.data(), Message.length(), NULL) == 0)
    {
        return wstring();
    }

    wstring Result(Message.c_str());

    transform(Result.begin(), Result.end(), Result.begin(),
        [](auto Ch) { return (Ch == L'\r' || Ch == L'\n') ? L' ' : Ch; });

    return Result;
}


wstring What(const exception& e)
{
    return AnsiToUnicode(string(e.what()));
}


wstring SizeText(LONGLONG Size)
{
    WCHAR Text[64];
    StrFormatByteSizeW(Size, Text, _countof(Text));
    return wstring(Text);
}


BOOL HandlePathTest(HANDLE File)
{
    WCHAR Path[MAX_PATH] {};

    if (GetFinalPathNameByHandleW(File, Path, _countof(Path), FILE_NAME_NORMALIZED) == 0)
        return FALSE;

    return !!Path[0];
}


path GetDesktopPath()
{
    path Path = "C:/";

    PWSTR pszPath;
    if (SHGetKnownFolderPath(FOLDERID_Desktop, KF_FLAG_DEFAULT, NULL, &pszPath) == S_OK)
    {
        Path = pszPath;
        CoTaskMemFree(pszPath);
    }

    return Path;
}


bool SHCreateDirectory(const path& Path)
{
    return SHCreateDirectory(NULL, Path.c_str()) == ERROR_SUCCESS;
}


bool SHCopyFile(const path& Src, const path& Dest)
{
    return !!CopyFileW(Src.c_str(), Dest.c_str(), FALSE);
}


//-------------------------------------------------------------------------------------------------------------------------------
// Kernel Utilities
//-------------------------------------------------------------------------------------------------------------------------------


decltype(RtlInitUnicodeString)* pfnRtlInitUnicodeString = nullptr;
decltype(RtlDosPathNameToNtPathName_U)* pfnRtlDosPathNameToNtPathName_U = nullptr;
decltype(NtCreateFile)* pfnNtCreateFile = nullptr;
decltype(NtClose)* pfnNtClose = nullptr;


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

    if (pfnNtCreateFile == nullptr)
        return;

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

                qDebug << L"Backup: NtCreateFile" << endl;
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

    //qDebug << L"NtCreateFile: " << (PVOID)Status << endl;

    return FALSE;
}

BOOL IsFileExists(const path& Path)
{
    return IsFileExists(Path.c_str());
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

    // Here we cannot determine whether the file is a virtual file,
    //   because the virtual file system can overwrite physical files.
    //     so we need to dump it and calculate the MD5 value.
    qDebug << L"File: " << path(Path) << L" " << SizeText(Record.Size) << endl;

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

    // Here we cannot determine whether the file is a virtual file,
    //   because the virtual file system can overwrite physical files.
    //     so we need to dump it and calculate the MD5 value.
    qDebug << L"File: " << path(Path) << L" " << SizeText(Record.Size) << endl;

    return File;
}


//-------------------------------------------------------------------------------------------------------------------------------
// Enumeration Code
//-------------------------------------------------------------------------------------------------------------------------------


#if 0 // BUG CODE
void EnumerateDirectory(const path& Path, bool Recursive)
{
    auto Func = [&](const directory_entry& Entry)
    {
        qDebug << L"EnumFile: " << Entry.path() << endl;
    };

    try
    {
        if (Recursive)
        {
            for (auto& Entry : recursive_directory_iterator(Path))
                Func(Entry);
        }
        else
        {
            for (auto& Entry : directory_iterator(Path))
                Func(Entry);
        }
    }
    catch (const exception& e)
    {
        qDebug << L"EnumDir: " << What(e) << endl;
    }
}
#endif


void RecursiveIterateDirectory(const path& DirPath, const function<void(const path&, bool)>& Func)
{
    WIN32_FIND_DATAW Data;
    HANDLE Handle = FindFirstFileW((DirPath / L"*").c_str(), &Data);

    if (Handle == INVALID_HANDLE_VALUE)
    {
        qDebug << L"IterateDirectory: Failed. " << GetErrorMsg() << endl;
        return;
    }

    do
    {
        path Path = DirPath / Data.cFileName;

        if (Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (Data.cFileName[0] == L'.')
                continue;

            Func(Path, true);
            RecursiveIterateDirectory(Path, Func);
        }
        else
        {
            Func(Path, false);
        }
    }
    while (FindNextFileW(Handle, &Data));

    if (GetLastError() != ERROR_NO_MORE_FILES)
        qDebug << L"IterateDirectory: Error. " << GetErrorMsg() << endl;

    FindClose(Handle);
}


void EnumerateDirectory(const path& DirPath, bool DumpFile)
{
    path BasePath = DirPath;
    path OutPath = GetDesktopPath() / L"dumpfiles";

    auto Func = [&](const path& Path, bool IsDir)
    {
        if (IsDir)
            return;

        if (Path.filename() == L"dev.log")
            return;

        qDebug << L"EnumFile: " << Path << endl;

        if (DumpFile)
        {
            path DestPath = OutPath / Path.lexically_relative(BasePath);

            SHCreateDirectory(DestPath.parent_path());

            if (SHCopyFile(Path, DestPath) == false)
                qDebug << L"Copy failed. " << GetErrorMsg() << endl;
            else
                qDebug << L"File copied." << endl;
        }
    };

    RecursiveIterateDirectory(DirPath, Func);
}


//-------------------------------------------------------------------------------------------------------------------------------
// Worker Code
//-------------------------------------------------------------------------------------------------------------------------------


//#define PIPE_ENABLE
#define HOOK_ENABLE
//#define ENUM_ENABLE


void Worker()
{
    qDebug << L"Worker is started." << endl;

#ifdef ENUM_ENABLE

    qDebug << L"Enumerating Directory..." << endl;

    EnumerateDirectory(gAppPath, true);

#endif

#ifdef HOOK_ENABLE

    InlineHook(pfnCreateFileA, MineCreateFileA);
    InlineHook(pfnCreateFileW, MineCreateFileW);

    qDebug << L"Hook is installed." << endl;

#endif

#ifdef PIPE_ENABLE

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
                    qDebug << L"Command: IterateDirectory" << endl;

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
                    qDebug << L"Command: RecursiveIterateDirectory" << endl;

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
                    qDebug << L"Command: GetFileRecord" << endl;

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
                    qDebug << L"Command: DumpFile" << endl;

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
        catch (const exception& e)
        {
            qDebug << L"ERROR: Command Execution ( " << What(e) << L" )" << endl;
        }
    }

#endif

}


//-------------------------------------------------------------------------------------------------------------------------------
// Initialization Code
//-------------------------------------------------------------------------------------------------------------------------------


void Start()
{
    qDebug << L"Started." << endl;

    // Wait for a while to reduce the chance of collapse.
    sleep_for(seconds(2));

#ifdef PIPE_ENABLE

    if (CreatePipe(&gPipeRead, &gPipeWrite, NULL, PIPE_BUFFER_SIZE) == FALSE)
    {
        qDebug << L"ERROR: Create pipe failed." << endl;
        return;
    }

    qDebug << L"Pipe R/W = " << gPipeRead << L"/" << gPipeWrite << endl;

#endif

    WCHAR Path[MAX_PATH];

    if (GetModuleFileNameW(gDll, Path, _countof(Path)) == 0)
        return;
    gDllPath = path(Path).parent_path();

    if (GetModuleFileNameW(gApp, Path, _countof(Path)) == 0)
        return;
    gAppPath = path(Path).parent_path();

    qDebug << L"DLL_PATH = " << gDllPath << endl;
    qDebug << L"APP_PATH = " << gAppPath << endl;

    thread t(Worker);
    t.detach();
}


//-------------------------------------------------------------------------------------------------------------------------------
// DLL Entry
//-------------------------------------------------------------------------------------------------------------------------------
#define FOPEN_MODE
//#define HOOK_MODE

#ifdef HOOK_MODE
BOOL WINAPI DllMain(
    _In_ HMODULE hModule,
    _In_ DWORD fdwReason,
    _In_ LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            make_console();
            try
            {
                DeleteFileW(L"dev.log");

                qDebug << L"DLL Attached." << endl;

                gDll = hModule;
                gApp = GetModuleHandleW(NULL);

                InitNtdll();
                BackupRealNtCreateFile();

                thread t(Start);
                t.detach();
            }
            catch (const exception& e)
            {
                qDebug << L"Exception ( " << What(e) << L" )" << endl;
            }

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
#endif

#ifdef FOPEN_MODE
std::vector<wstring>FileList;

static LPWSTR ctowUTF(char* str)
{
    static wchar_t result[1024];
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, result, len);
    result[len] = L'\0';
    return result;
}

void InitFileListFile()
{
    std::ifstream fin("FileList.txt");
    const int LineMax = 0x1000;
    char str[LineMax];
    if (fin.is_open())
    {
        int counter = 0;
        while (fin.getline(str, LineMax))
        {
            auto wtmp = ctowUTF(str);
            FileList.push_back(wtmp);
        }
        fin.close();
    }
    else
    {
        fin.close();
        MessageBox(0, L"Can't open script file", L"InitText Error", 0);
        return;
    }
}

static char* wtoc(LPCTSTR str)
{
    static char result[1024];
    int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, 0);
    WideCharToMultiByte(CP_ACP, 0, str, -1, result, len, NULL, 0);
    result[len] = '\0';
    return result;
}
#include <io.h>
#include <direct.h>
#include <string>
bool IsOnce = false;
BOOL WINAPI DllMain(
    _In_ HMODULE hModule,
    _In_ DWORD fdwReason,
    _In_ LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        make_console();
        InitFileListFile();
        break;
    }
    case DLL_THREAD_ATTACH:
    {
        if (!IsOnce) 
        {
            for (auto f : FileList)
            {
                char* fn = wtoc(f.c_str());
                FILE* fp = fopen(fn, "rb");
                if (fp)
                {
                    fseek(fp, 0, SEEK_END);
                    DWORD size = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    auto buff = malloc(size);
                    if (buff)
                    {
                        fread(buff, size, 1, fp);
                        fclose(fp);
                        if (buff)
                        {
                            std::cout << "dump:" << fn << endl;
                            string fnm(fn);
                            fnm = "dump\\" + fnm;
                            string strDirName = fnm.substr(0, fnm.find_last_of("\\") + 1);

                            if (_access(strDirName.c_str(), 0) == -1)
                                _mkdir(strDirName.c_str());

                            fopen((fnm).c_str(), "wb");
                            fwrite(buff, size, 1, fp);
                            fclose(fp);
                            IsOnce = true;
                        }
                    }
                }
            }
        }
    }
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
    {
        break;
    }
    }

    return TRUE;
}
#endif