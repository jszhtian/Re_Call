
#include "windows.h"
#include<iostream>

//TLS变量
__declspec(thread) char g_tlsszA[] = "TlsACallBack";
__declspec(thread) char g_tlsszB[] = "TlsBCallBack";
__declspec(thread) char g_tlsszM[] = "main";
__declspec(thread) char g_tlssz2[] = "TLS g_tlsNumb = 0x%p...\r\n";
__declspec(thread) int g_tlsNumb = 1111;

//TLS回调函数
void NTAPI t_TlsCallBack_A(PVOID DllHandle, DWORD Reason, PVOID Red)
{
	if (DLL_PROCESS_ATTACH == Reason)
	{
		MessageBoxA(0, "TLS回调函数A，DLL_PROCESS_ATTACH", g_tlsszA, 0);
	}
	else if (DLL_PROCESS_DETACH == Reason)
	{
		MessageBoxA(0, "TLS回调函数A，DLL_PROCESS_DETACH", g_tlsszA, 0);

	}
}
void NTAPI t_TlsCallBack_B(PVOID DllHandle, DWORD Reason, PVOID Red)
{
	if (DLL_PROCESS_ATTACH == Reason)
	{
		MessageBoxA(0, "TLS回调函数B，t_TlsCallBack_B", g_tlsszB, 0);
	}
}

//注册TLS回调函数
#pragma data_seg(".CRT$XLB")//".CRT$XLB" -> CRT表示使用C RunTime机制，X表示标识名随机，L表示TLS callback section，B可以是B-Y之间的任意一个字母
PIMAGE_TLS_CALLBACK p_thread_callback[] = {
	t_TlsCallBack_A,
	t_TlsCallBack_B,
	NULL
};
#pragma data_seg()

//线程回调函数
DWORD WINAPI MyThreadProc(
	_In_ LPVOID lpParameter
)
{
	printf("MyThreadProc -> Frist printf:");
	//printf(g_tlssz2, g_tlsNumb);
	//g_tlsNumb = 0x22222;
	//printf(g_tlssz2, g_tlsNumb);
	MessageBoxA(0, "线程回调函数MyThreadProc", "MyThreadProc", 0);
	return 0;
}

int main()
{
	//printf("main -> Tlstset is reuing...\r\n\r\n");
	MessageBoxA(0, "main函数", "main", 1);
	CreateThread(NULL, 0, MyThreadProc, NULL, 0, NULL);
	Sleep(100);
	CreateThread(NULL, 0, MyThreadProc, NULL, 0, NULL);

	system("pause");
	return 0;
}

