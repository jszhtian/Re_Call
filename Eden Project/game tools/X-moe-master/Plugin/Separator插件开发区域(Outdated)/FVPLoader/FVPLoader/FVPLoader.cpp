// FVPLoader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib,"Advapi32.lib")

struct ProcessWindow
{
	DWORD dwProcessId;
	HWND hwndWindow;
};

BOOL SetPrivilege(HANDLE hToken,LPCTSTR lpszPrivilege,BOOL bEnablePrivilege)   
{  
    TOKEN_PRIVILEGES tp;  
    LUID luid;  
  
    if ( !LookupPrivilegeValue(   
        NULL,            // lookup privilege on local system   
        lpszPrivilege,   // privilege to lookup    
        &luid ) )        // receives LUID of privilege   
    {  
        //printf("LookupPrivilegeValue error: %u\n", GetLastError() );   
        return FALSE;   
    }  
  
    tp.PrivilegeCount = 1;  
    tp.Privileges[0].Luid = luid;  
    if (bEnablePrivilege)  
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
    else  
        tp.Privileges[0].Attributes = 0;  
  
    // Enable the privilege or disable all privileges.   
  
    if ( !AdjustTokenPrivileges(  
        hToken,   
        FALSE,   
        &tp,   
        sizeof(TOKEN_PRIVILEGES),   
        (PTOKEN_PRIVILEGES) NULL,   
        (PDWORD) NULL) )  
    {   
        //printf("AdjustTokenPrivileges error: %u\n", GetLastError() );   
        return FALSE;   
    }

	return TRUE;
}

// 查找进程主窗口的回调函数
BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{
	ProcessWindow *pProcessWindow = (ProcessWindow *)lParam;

	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	// 判断是否是指定进程的主窗口
	if (pProcessWindow->dwProcessId == dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL)
	{
		pProcessWindow->hwndWindow = hWnd;

		return FALSE;
	}

	return TRUE;
}


BOOL InjectDLL(char *Cmdstr)
{
	BOOL isWINXP = FALSE;
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize=sizeof(os);
	if(!GetVersionEx((OSVERSIONINFO *)&os))
	{
		return FALSE;
	}
	switch(os.dwMajorVersion)
	{
		case 5:
			isWINXP = TRUE;
		break;

		default:
		break;
	}

	char* szDllPath = "nt.dll";
	HANDLE hProcess = NULL;
	HMODULE hMod = NULL;
	LPVOID  pBuf = NULL;
	
	STARTUPINFOA si = {sizeof(si)};
	//si.lpTitle = "Debug";
	PROCESS_INFORMATION pi;
	CreateProcessA(NULL, Cmdstr, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);

	if(isWINXP == TRUE)
	{
		HANDLE hToken;  
		BOOL bRet = OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken);  
		SetPrivilege(hToken,SE_DEBUG_NAME,TRUE);
	}
	//查询pID 
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);

	if(hProcess == NULL)
	{
		MessageBoxA(NULL, "Failed to Get Handle!\nX'moe [xmoe.project@gmail.com]","FVPLoader",NULL);
		return FALSE;
	}
	DWORD dwBufSize = (DWORD)((strlen("xmoe_white.dll") + 1) * sizeof(char));
	
	pBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pBuf, (LPVOID)szDllPath, dwBufSize, NULL);


	//HMODULE hMod = NULL;
	hMod = GetModuleHandleA("kernel32.dll");
	HANDLE hThread = NULL;
	LPTHREAD_START_ROUTINE pThreadProc;

	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryA");
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pBuf, 0, NULL);

	ResumeThread(pi.hThread);
	WaitForSingleObject(hThread, INFINITE);
		

	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

int main(int argc, char** argv)
{
	if(!argv[1])
	{
		MessageBoxA(NULL, "Usage : <fvp_engine>\nX'moe[xmoe.project@gmail.com]", "FVPLoader", MB_OK);
		return -1;
	}
	FILE *fout = fopen("FVPLoader.txt","wb");
	fprintf(fout,"FVPLoader 0.2\r\nX'moe [xmoe.project@gmail.com]\r\nIf you have any problems, please send a mail to me\r\n");
	fclose(fout);
	//printf("%s", argv[0]);
	InjectDLL(argv[1]);
	return 0;
}
