// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

static DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
static LPWSTR ctow(char* str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(cp, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(cp, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static char* wtoc(LPCTSTR str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Open Console Success!" << endl;
}

UINT_PTR iTimerID;

void GetEIP()
{
	DWORD EIP = 0;
	_asm
	{
		call GetEIP;
	GetEIP:
		pop eax;
		mov EIP, eax
	}
	cout << hex << EIP << endl;
}

void CALLBACK TimerProc()
{
	GetEIP();
}

void Main()
{
	//iTimerID = SetTimer(NULL, 0, 1, (TIMERPROC)TimerProc);
	GetEIP();
}

void UnMain()
{
	KillTimer(NULL, iTimerID);
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
		Main();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		//UnMain();
        break;
    }
    return TRUE;
}

