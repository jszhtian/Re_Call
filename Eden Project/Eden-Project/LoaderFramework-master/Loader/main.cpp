// main.cpp
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#include <detours.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    static WCHAR szDirPath[2048];
    static WCHAR szAppPath[2048];

    GetModuleFileNameW(hInstance, szDirPath, ARRAYSIZE(szDirPath));
    if (GetLastError() != ERROR_SUCCESS)
        return 1;

    PathRemoveFileSpecW(szDirPath);
    PathAddBackslashW(szDirPath);
    PathCombineW(szAppPath, szDirPath, L"run.exe");

    STARTUPINFOW startupInfo = { sizeof(startupInfo) };
    PROCESS_INFORMATION processInfo = {};
    LPCSTR rgDlls[1] = { "font_patch.dll" };

    if (DetourCreateProcessWithDllsW(szAppPath, NULL, NULL, NULL, FALSE, NULL, NULL, szDirPath, &startupInfo, &processInfo, ARRAYSIZE(rgDlls), rgDlls, NULL) != TRUE) {
        
        MessageBoxW(0, L"Æô¶¯Ê§°Ü", L"Æô¶¯", MB_OK | MB_ICONERROR);
        return 1;
    }
#if 0
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(processInfo.hProcess, &exitCode);

    return exitCode;
#else
    return 0;
#endif
}
