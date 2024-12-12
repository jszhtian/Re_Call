// KrkrDumpLoader.cpp

#include <windows.h>
#include <detours.h>
#include "path.h"
#include "util.h"

#define HIDE_CONSOLE

int wmain(int argc, wchar_t* argv[])
{

#ifdef HIDE_CONSOLE
	HWND hWnd = GetConsoleWindow();  // 获取控制台窗口句柄
	ShowWindow(hWnd, SW_HIDE);  // 隐藏控制台窗口
#endif

	std::wstring gamePath = Path::GetFullPath(L"AQUA.EXE");
	std::wstring gameDirPath = Path::GetDirectoryName(gamePath);

	std::wstring commandLine;

	for (int i = 2; i < argc; i++)
	{
		commandLine += argv[i];
		commandLine += L' ';
	}

	std::string dllPath = "AQUA_CHS.SUO";

	STARTUPINFO startupInfo{};
	PROCESS_INFORMATION processInfo{};

	startupInfo.cb = sizeof(startupInfo);

	if (DetourCreateProcessWithDllW(gamePath.c_str(), const_cast<std::wstring::pointer>(commandLine.c_str()),
		NULL, NULL, FALSE, 0, NULL, gameDirPath.c_str(), &startupInfo, &processInfo, dllPath.c_str(), NULL) == FALSE)
	{
		auto msg = Util::GetLastErrorMessageA();
		printf("CreateProcess failed : %s\n", msg.c_str());
		return 1;
	}

	WaitForSingleObject(processInfo.hProcess, INFINITE);

	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	return 0;
}
