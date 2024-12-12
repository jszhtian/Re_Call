
#include "windows.h"
#include<iostream>

//TLS����
__declspec(thread) char g_tlsszA[] = "TlsACallBack";
__declspec(thread) char g_tlsszB[] = "TlsBCallBack";
__declspec(thread) char g_tlsszM[] = "main";
__declspec(thread) char g_tlssz2[] = "TLS g_tlsNumb = 0x%p...\r\n";
__declspec(thread) int g_tlsNumb = 1111;

//TLS�ص�����
void NTAPI t_TlsCallBack_A(PVOID DllHandle, DWORD Reason, PVOID Red)
{
	if (DLL_PROCESS_ATTACH == Reason)
	{
		MessageBoxA(0, "TLS�ص�����A��DLL_PROCESS_ATTACH", g_tlsszA, 0);
	}
	else if (DLL_PROCESS_DETACH == Reason)
	{
		MessageBoxA(0, "TLS�ص�����A��DLL_PROCESS_DETACH", g_tlsszA, 0);

	}
}
void NTAPI t_TlsCallBack_B(PVOID DllHandle, DWORD Reason, PVOID Red)
{
	if (DLL_PROCESS_ATTACH == Reason)
	{
		MessageBoxA(0, "TLS�ص�����B��t_TlsCallBack_B", g_tlsszB, 0);
	}
}

//ע��TLS�ص�����
#pragma data_seg(".CRT$XLB")//".CRT$XLB" -> CRT��ʾʹ��C RunTime���ƣ�X��ʾ��ʶ�������L��ʾTLS callback section��B������B-Y֮�������һ����ĸ
PIMAGE_TLS_CALLBACK p_thread_callback[] = {
	t_TlsCallBack_A,
	t_TlsCallBack_B,
	NULL
};
#pragma data_seg()

//�̻߳ص�����
DWORD WINAPI MyThreadProc(
	_In_ LPVOID lpParameter
)
{
	printf("MyThreadProc -> Frist printf:");
	//printf(g_tlssz2, g_tlsNumb);
	//g_tlsNumb = 0x22222;
	//printf(g_tlssz2, g_tlsNumb);
	MessageBoxA(0, "�̻߳ص�����MyThreadProc", "MyThreadProc", 0);
	return 0;
}

int main()
{
	//printf("main -> Tlstset is reuing...\r\n\r\n");
	MessageBoxA(0, "main����", "main", 1);
	CreateThread(NULL, 0, MyThreadProc, NULL, 0, NULL);
	Sleep(100);
	CreateThread(NULL, 0, MyThreadProc, NULL, 0, NULL);

	system("pause");
	return 0;
}

