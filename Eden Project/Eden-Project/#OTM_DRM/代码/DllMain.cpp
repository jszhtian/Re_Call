#include "../pack/Share.h"
#include"../aplib/aplib.h"
#include <Windows.h>
#pragma comment(lib, "../aplib/aplib.lib")
// �� dll �����ν����˺ϲ�����������������Ϊ[��дִ��]
#pragma comment(linker, "/merge:.data=.text") 
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/section:.text,RWE")

#define TH32CS_SNAPTHREAD   0x00000004
struct TypeOffset
{
	WORD Offset : 12;
	WORD Type : 4;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����߳����API
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagTHREADENTRY32
{
	DWORD   dwSize;
	DWORD   cntUsage;
	DWORD   th32ThreadID;       // this thread
	DWORD   th32OwnerProcessID; // Process this thread is associated with
	LONG    tpBasePri;
	LONG    tpDeltaPri;
	DWORD   dwFlags;
} THREADENTRY32;
typedef THREADENTRY32 *  PTHREADENTRY32;
typedef THREADENTRY32 *  LPTHREADENTRY32;
typedef struct _CLIENT_ID {
	DWORD   UniqueProcess;
	DWORD   UniqueThread;
} CLIENT_ID;
typedef   CLIENT_ID   *PCLIENT_ID;
typedef enum MY_THREAD_INFORMATION_CLASS {
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
} MYTHREAD_INFORMATION_CLASS, *PMYTHREAD_INFORMATION_CLASS;
typedef ULONG KPRIORITY;
typedef struct _THREAD_BASIC_INFORMATION {
	NTSTATUS                ExitStatus;
	PVOID                   TebBaseAddress;
	CLIENT_ID               ClientId;
	KAFFINITY               AffinityMask;
	KPRIORITY               Priority;
	KPRIORITY               BasePriority;
	
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef HANDLE (WINAPI* MYCreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID);
typedef BOOL (WINAPI* MYThread32First)(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
typedef BOOL (WINAPI* MYThread32Next)(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
typedef HANDLE (WINAPI* MYOpenThread)(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ DWORD dwThreadId);
typedef LONG(__stdcall *MYNtQueryInformationThread)(IN  HANDLE ThreadHandle, IN  MYTHREAD_INFORMATION_CLASS ThreadInformationClass, OUT PVOID ThreadInformation, IN  ULONG ThreadInformationLength, OUT PULONG ReturnLength OPTIONAL);

MYCreateToolhelp32Snapshot MyCreateToolhelp32Snapshot = 0;
MYThread32First MyThread32First = 0;
MYThread32Next MyThread32Next = 0;
MYOpenThread MyOpenThread = 0;
MYNtQueryInformationThread MyNtQueryInformationThread = 0;


typedef BOOL (WINAPI* MYIsDebuggerPresent)(VOID);
MYIsDebuggerPresent MyIsDebuggerPresent =0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����API
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FARPROC(WINAPI* MYGetProcAddress)(HMODULE hModule, LPCSTR  lpProcName);
typedef int(WINAPI* MYMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
typedef HMODULE (WINAPI* MYLoadLibraryExA)(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
typedef BOOL (WINAPI* MYVirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
typedef LPVOID (WINAPI* MYVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef void* (__cdecl* MYmemcpy)(void* _Dst, void const* _Src, size_t _Size);
typedef HANDLE(WINAPI* MYCreateThread)(_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes, _In_ SIZE_T dwStackSize, _In_ LPTHREAD_START_ROUTINE lpStartAddress, _In_opt_ __drv_aliasesMem LPVOID lpParameter, _In_ DWORD dwCreationFlags, _Out_opt_ LPDWORD lpThreadId);
typedef BOOL(WINAPI* MYSetThreadContext)(_In_ HANDLE hThread, _In_ CONST CONTEXT * lpContext);
typedef BOOL (WINAPI* MYGetThreadContext)(_In_ HANDLE hThread, _Inout_ LPCONTEXT lpContext);
typedef HANDLE (WINAPI* MYGetCurrentThread)(VOID);
typedef int(__cdecl* MYatexit)(void(__cdecl*)(void));
typedef BOOL (WINAPI* MYTerminateThread)(_In_ HANDLE hThread, _In_ DWORD dwExitCode);
typedef NTSTATUS (WINAPI* MYDbgkpSendApiMessage)(PVOID ApiMsg,	PVOID Port,	BOOLEAN SuspendProcess);

MYGetProcAddress MyGetProcAddress =0;
MYMessageBoxA MyMessageBoxA = 0;
MYLoadLibraryExA MyLoadLibraryExA;
MYVirtualProtect MyVirtualProtect =0;
MYVirtualAlloc MyVirtualAlloc;
MYmemcpy Mymemcpy = 0;
MYCreateThread MyCreateThread = 0;
MYSetThreadContext MySetThreadContext = 0;
MYGetThreadContext MyGetThreadContext = 0;
MYGetCurrentThread MyGetCurrentThread = 0;
MYatexit Myatexit = 0;
MYTerminateThread MyTerminateThread = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���ڲ���API
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef ATOM(WINAPI* MYRegisterClassA)(CONST WNDCLASSA *lpWndClass);
typedef HWND(WINAPI* MYCreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
typedef HMODULE(WINAPI* MYGetModuleHandleA)(LPCSTR lpModuleName);
typedef BOOL(WINAPI* MYShowWindow)(HWND hWnd, int nCmdShow);
typedef BOOL(WINAPI* MYUpdateWindow)(HWND hWnd);
typedef BOOL(WINAPI* MYGetMessageA)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
typedef BOOL(WINAPI* MYTranslateMessage)(CONST MSG *lpMsg);
typedef LRESULT(WINAPI* MYDispatchMessageA)(CONST MSG *lpMsg);
typedef LRESULT(WINAPI* MYDefWindowProcA)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
typedef VOID(WINAPI* MYPostQuitMessage)(int nExitCode);
typedef BOOL(WINAPI* MYGetClientRect)(HWND hWnd, LPRECT lpRect);
typedef int (WINAPI* MYGetWindowTextA)(HWND hWnd, _Out_writes_(nMaxCount) LPSTR lpString,  int nMaxCount);
typedef HWND(WINAPI* MYGetDlgItem)(HWND hDlg, int nIDDlgItem);
typedef int(__cdecl* MYmemcmp)(_In_reads_bytes_(_Size) void const* _Buf1, _In_reads_bytes_(_Size) void const* _Buf2, size_t _Size);
typedef VOID(WINAPI* MYExitProcess)(UINT uExitCode);
typedef LRESULT(WINAPI* MYSendMessageA)(HWND hWnd, UINT Msg, _Pre_maybenull_ _Post_valid_ WPARAM wParam, _Pre_maybenull_ _Post_valid_ LPARAM lParam);

MYCreateWindowExA	MyCreateWindowExA;
MYRegisterClassA	 MyRegisterClassA;
MYGetModuleHandleA MyGetModuleHandleA;
MYShowWindow		MyShowWindow;
MYUpdateWindow		MyUpdateWindow;
MYGetMessageA		MyGetMessageA;
MYTranslateMessage MyTranslateMessage;
MYDispatchMessageA MyDispatchMessageA;
MYDefWindowProcA   MyDefWindowProcA;
MYPostQuitMessage MyPostQuitMessage;
MYGetClientRect MyGetClientRect;
MYGetWindowTextA MyGetWindowTextA;
MYGetDlgItem MyGetDlgItem;
MYmemcmp Mymemcmp;
MYExitProcess MyExitProcess;
MYSendMessageA MySendMessageA;


HMODULE gKernelBase = 0;
HMODULE gUser32Base = 0;
DWORD gImageBase;
HWND gParentWnd = 0;

PIMAGE_DOS_HEADER pDosHead(DWORD Base) {
	return (PIMAGE_DOS_HEADER)Base;
}
PIMAGE_NT_HEADERS pNtHead(DWORD Base) {
	return (PIMAGE_NT_HEADERS)(pDosHead(Base)->e_lfanew + Base);
}
PIMAGE_FILE_HEADER pFileHead(DWORD Base) {
	return &(pNtHead(Base)->FileHeader);
}
PIMAGE_OPTIONAL_HEADER pOptionHead(DWORD Base) {
	return &(pNtHead(Base)->OptionalHeader);
}
PIMAGE_SECTION_HEADER  GetSecHeadAddr(DWORD ModuleBase, LPCSTR Name) {
	DWORD NumberOfSec = pFileHead(ModuleBase)->NumberOfSections;
	auto FristSec = IMAGE_FIRST_SECTION(pNtHead(ModuleBase));
	for (size_t i = 0; i < NumberOfSec; i++)
	{
		if (!memcmp(FristSec[i].Name, Name, strlen(Name)+1)) {
			return &FristSec[i];
		}
	}
	return 0;
}

void GetFunAdd() {
	DWORD Ent;
	DWORD Eot;
	DWORD Eat;
	CHAR Buff[] = { 'G','e','t','P','r','o','c','A','d','d','r' ,'e','s','s','\0' };
	__asm 
	{
		pushad
		mov eax,fs:[0x30]
		mov eax,[eax+0xc]
		mov eax,[eax+0x1c]		//��һ��ģ��
		mov eax,[eax]			//kernelģ��
		mov eax,[eax+0x8]		//kernel��ַ
		mov gKernelBase,eax		
		mov edx,[eax+0x3c]
		add edx,eax
		mov edx,[edx+0x78]
		add edx,eax				//������
		mov ebx,[edx+0x1c]
		add ebx,eax
		mov Eat,ebx				//��ַ��
		mov ebx,[edx+0x20]
		add ebx,eax
		mov Ent,ebx				//���Ʊ�
		mov ebx,[edx+0x24]
		add ebx,eax
		mov Eot,ebx				//��ű�

		xor ebx, ebx
		jmp tag_FristCmp
	tag_CmpFunNameLoop:
		inc ebx
	tag_FristCmp:
		mov esi, Ent
		mov esi, [ebx * 0x4 + esi]					//�������Ʊ�			
		lea esi, [esi + eax]						//��������VA
		lea edi, dword ptr Buff						//GetProAddress�ַ�����ַ
		mov ecx, 0xE								//GetProAddress����
		cld											//�������λ
		repe cmpsb									//ѭ���Ƚ��ַ�
		jne tag_cmpFunNameLoop

		mov esi,Eot									
		xor edi, edi
		mov di, [ebx * 2 + esi]						//���
		//ʹ������ڵ�ַ�����ҵ�������ַ
		mov edx, Eat
		mov esi, [edx + edi * 4]					//�õ�������ַRVA
		lea eax, [esi + eax]						//������ַ
		mov MyGetProcAddress,eax
		popad
	}

}

void GetApi() {
	MyVirtualProtect = (MYVirtualProtect)MyGetProcAddress(gKernelBase, "VirtualProtect");
	MyLoadLibraryExA = (MYLoadLibraryExA)MyGetProcAddress(gKernelBase, "LoadLibraryExA");
	
	gUser32Base = MyLoadLibraryExA("User32.dll", NULL, NULL);
	MyMessageBoxA = (MYMessageBoxA) MyGetProcAddress(gUser32Base, "MessageBoxA");

	MyVirtualAlloc = (MYVirtualAlloc)MyGetProcAddress(gKernelBase, "VirtualAlloc");

	HMODULE hVcruntime = MyLoadLibraryExA("vcruntime140.dll", NULL, NULL);
	Myatexit = (MYatexit)MyGetProcAddress(hVcruntime, "atexit");
	Mymemcpy = (MYmemcpy)MyGetProcAddress(hVcruntime, "memcpy");
	MyRegisterClassA = (MYRegisterClassA)MyGetProcAddress(gUser32Base, "RegisterClassA");
	MyCreateWindowExA = (MYCreateWindowExA)MyGetProcAddress(gUser32Base, "CreateWindowExA");
	MyGetModuleHandleA = (MYGetModuleHandleA)MyGetProcAddress(gKernelBase, "GetModuleHandleA");
	MyShowWindow = (MYShowWindow)MyGetProcAddress(gUser32Base, "ShowWindow");
	MyUpdateWindow = (MYUpdateWindow)MyGetProcAddress(gUser32Base, "UpdateWindow");
	MyGetMessageA = (MYGetMessageA)MyGetProcAddress(gUser32Base, "GetMessageA");
	MyTranslateMessage = (MYTranslateMessage)MyGetProcAddress(gUser32Base, "TranslateMessage");
	MyDispatchMessageA = (MYDispatchMessageA)MyGetProcAddress(gUser32Base, "DispatchMessageA");
	MyDefWindowProcA = (MYDefWindowProcA)MyGetProcAddress(gUser32Base, "DefWindowProcA");
	MyPostQuitMessage = (MYPostQuitMessage)MyGetProcAddress(gUser32Base, "PostQuitMessage");
	MyGetClientRect  = (MYGetClientRect)MyGetProcAddress(gUser32Base, "GetClientRect");
	MyGetWindowTextA = (MYGetWindowTextA)MyGetProcAddress(gUser32Base, "GetWindowTextA");
	MyGetDlgItem = (MYGetDlgItem)MyGetProcAddress(gUser32Base, "GetDlgItem");
	Mymemcmp = (MYmemcmp)MyGetProcAddress(hVcruntime, "memcmp");
	MyExitProcess = (MYExitProcess)MyGetProcAddress(gKernelBase, "ExitProcess");
	MySendMessageA = (MYSendMessageA)MyGetProcAddress(gUser32Base, "SendMessageA");
	MyCreateThread = (MYCreateThread)MyGetProcAddress(gKernelBase, "CreateThread");
	MyGetThreadContext = (MYGetThreadContext)MyGetProcAddress(gKernelBase, "GetThreadContext");
	MyGetCurrentThread = (MYGetCurrentThread)MyGetProcAddress(gKernelBase, "GetCurrentThread");
	MyTerminateThread = (MYTerminateThread)MyGetProcAddress(gKernelBase, "TerminateThread");

	HMODULE hKernel32 = MyLoadLibraryExA("Kernel32.dll", NULL, NULL);
	HMODULE hNtdll = MyLoadLibraryExA("ntdll.dll", NULL, NULL);
	MYCreateToolhelp32Snapshot MyCreateToolhelp32Snapshot = (MYCreateToolhelp32Snapshot)MyGetProcAddress(hKernel32, "CreateToolhelp32Snapshot");
	if (!MyCreateToolhelp32Snapshot)
		MyMessageBoxA(0, "MyCreateToolhelp32Snapshot", 0, 0);
	MyThread32First = (MYThread32First)MyGetProcAddress(hKernel32, "Thread32First");
	if (!MyThread32First)
		MyMessageBoxA(0, "MyThread32First", 0, 0);
	MyThread32Next = (MYThread32Next)MyGetProcAddress(hKernel32, "Thread32Next");
	MyOpenThread = (MYOpenThread)MyGetProcAddress(hKernel32, "OpenThread");
	MyNtQueryInformationThread = (MYNtQueryInformationThread)MyGetProcAddress(hNtdll, "NtQueryInformationThread");

	MyIsDebuggerPresent = (MYIsDebuggerPresent)MyGetProcAddress(hKernel32, "IsDebuggerPresent");


}

//
HANDLE gThread = 0;
VOID CloseThread() {
	MyTerminateThread(gThread, 0);
	}
//���ܴ���
void DecSection() {
	
	DWORD OldProtect;
	//��ȡ.text����
	auto SectionHead = GetSecHeadAddr(gImageBase, ".text");
	DWORD Size = SectionHead->Misc.VirtualSize;
	CHAR* Address = (CHAR*)(SectionHead->VirtualAddress + gImageBase);
	MyVirtualProtect(Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
	//ѭ������
	while (Size) {
		*Address = *Address ^ 0x15;
		Address++;
		Size--;
	}
	MyVirtualProtect(Address, Size, OldProtect, &OldProtect);
}

//�ص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CHAR Buff[0x11] = {0};
	HWND h1000 = 0;
	DWORD Len = 0;
	switch (message)
	{
	case WM_CREATE:
		MyCreateWindowExA(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 200, 30, hWnd, (HMENU)0x1000, NULL, NULL);
		MyCreateWindowExA(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("��֤����"), WS_CHILD | WS_VISIBLE, 10, 40, 80, 30, hWnd, (HMENU)0x1001, NULL, NULL);
		MyCreateWindowExA(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("ȡ������"), WS_CHILD | WS_VISIBLE, 100, 40, 80, 30, hWnd, (HMENU)0x1002, NULL, NULL);
	}
	if (hWnd == gParentWnd)
	{
		switch (message)
		{
			case WM_CLOSE:
				MyPostQuitMessage(0);
				if (lParam != 100)
				{
					MyExitProcess(0);
				}
				break;
			case WM_COMMAND:
			{
				int nId = LOWORD(wParam);
				switch (nId)
				{
					case 0x1001:
						h1000 = MyGetDlgItem(gParentWnd, 0x1000);
						Len = MyGetWindowTextA(h1000, Buff, 0x10);
						if (Len ==0)
						{
							MyMessageBoxA(0, TEXT("���벻��Ϊ��"), TEXT("��ʾ"), 0);
						}
						else if (!Mymemcmp("1234", Buff, Len)) {
							MyMessageBoxA(0, TEXT("������ȷ"), TEXT("��ʾ"), 0);							
							MySendMessageA(gParentWnd, WM_CLOSE, 0, 100);
						}
						else {
							MyMessageBoxA(0, TEXT("�������"), TEXT("����"), 0);
						}
						break;
					case 0x1002:
						MyExitProcess(0);						
						break;
					default:
						break;
				}
			}
		}
	}
	return  MyDefWindowProcA(hWnd, message, wParam, lParam);
	};

//���뵯��
void CallShllFun() {
	//__asm int 3
	WNDCLASS wc = { 0 };
	HMODULE hInstance = MyGetModuleHandleA(NULL);
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//����
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;//ʵ�����������˳���
	wc.hIcon = 0;
	wc.hCursor = 0;
	wc.hbrBackground = 0;//BRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;//�˵�
	wc.lpszClassName = ("���ܿ�");
	MyRegisterClassA(&wc);
	gParentWnd = MyCreateWindowExA(
		0,
		"���ܿ�",//����
		"C++д�Ŀ�",//������
		WS_OVERLAPPEDWINDOW,//�ص����ڵķ��   ��Ҫ�Ĳ���----���ڷ��
		100, 200, 300, 130, //λ�úʹ�С
		NULL,               //�����ھ��
		NULL,               //�˵������û�в˵�
		hInstance,          //ʵ�����
		NULL                //������Ϣ
	);
	//��ʾ�����´���
	MyShowWindow(gParentWnd, SW_SHOW);
	MyUpdateWindow(gParentWnd);
	MSG msg = { 0 };
	while (MyGetMessageA(&msg, 0, 0, 0))
	{
		MyTranslateMessage(&msg);
		MyDispatchMessageA(&msg);
	}
	return ;
}

//�޸��ض�λ
void FixReloca(SHARE ShareData) {
	auto Relocs = (PIMAGE_BASE_RELOCATION)(ShareData.dwRelocRva + gImageBase);
	DWORD OldProtect = 0;
	while (Relocs->VirtualAddress)
	{
		DWORD OldProtect = 0;
		MyVirtualProtect((LPVOID)(gImageBase + Relocs->VirtualAddress), 0x1000, PAGE_READWRITE, &OldProtect);

		TypeOffset* items = (TypeOffset*)(Relocs + 1);
		//�����ض�λ��
		int count = (Relocs->SizeOfBlock - 8) / 2;
		for (int i = 0; i < count; ++i)
		{
			if (items[i].Type == 3)
			{
				DWORD Temp = 0;
				// �����ÿһ����Ҫ�ض�λ���������ڵĵ�ַ
				DWORD* item = (DWORD*)(gImageBase + Relocs->VirtualAddress + items[i].Offset);
				// �������������Ҫ�ض�λ�����ݣ�ͨ���Ǵ���Σ�����д��
				*item = *item + gImageBase - ShareData.OldImageBase;
			}
		}
		MyVirtualProtect((LPVOID)(gImageBase + Relocs->VirtualAddress), 0x1000, OldProtect, &OldProtect);
		// ��һ���ض�λ��
		Relocs = (PIMAGE_BASE_RELOCATION)(Relocs->SizeOfBlock + (DWORD)Relocs);
	}
}

//�޸�������IAT
void DecIat(SHARE ShareData) {
	auto ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)(gImageBase + ShareData.ImpTableVA);
	while (ImportTable->Name)
	{
		//��ȡDLL����
		CHAR* Name = (CHAR*)(ImportTable->Name+gImageBase);
		HMODULE Module = MyLoadLibraryExA(Name, NULL, NULL);
		auto Iat = (int*)(ImportTable->FirstThunk + gImageBase);
		for (size_t i = 0; Iat[i]!=0; i++)
		{
			DWORD FunAddr = 0;
			DWORD OldProtect;
			BYTE* DecCode = (BYTE*)MyVirtualAlloc(0, 0x50, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			//���ܴ���
			BYTE OpCode[] = {
			0xE8, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x58, 0x90, 0xB8,		
			0xA4, 0xA3, 0xA2, 0xA1,			//����							
			0xEB, 0x04, 0xE8, 0xBF, 0x78, 0xCC, 0x35, 0x59, 0x10, 0x40, 
			0x00, 0xEB, 0x01, 0xE9, 0x50, 0xEB, 0x02, 0x61, 0xE8, 0x90,	
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xEB, 0x05, 0xE8, 0x5B, 
			0xC3, 0xE8, 0x88, 0x90, 0xC3, 0xE8, 0x72, 0x6B, 0x77, 0x88	
			};			
			MyVirtualProtect(&Iat[i], 0x50, PAGE_EXECUTE_READWRITE, &OldProtect);
			//���λΪ1����ŵ���
			if ((DWORD)Iat[i] & 0x80000000) 
				FunAddr = (DWORD)MyGetProcAddress(Module, (LPCSTR)(Iat[i] & 0xffff));			
			else{
				auto AddressOfData = (PIMAGE_IMPORT_BY_NAME)(Iat[i] + gImageBase);
				FunAddr = (DWORD)MyGetProcAddress(Module, AddressOfData->Name);//���ǰ77216210
			}
			//�����ܺ�ĺ�����ַд��ѿռ�Ľ��ܴ���
			FunAddr ^= 0x00401059;
			OpCode[9] = FunAddr;
			OpCode[10] = FunAddr >> 0x8;
			OpCode[11] = FunAddr >> 0x10;
			OpCode[12] = FunAddr >> 0x18;
			Mymemcpy(DecCode, OpCode, 0x50);
			Iat[i] = (int)DecCode;
			MyVirtualProtect(&Iat[i], 0x50, OldProtect, &OldProtect);
		}
		ImportTable++;
	}

}

//�ֶ�����Tls
void CallTls(SHARE ShareData) {
	//�������TLS��
	if (ShareData.TlsVirtualAddress)
	{
		DWORD OldProtect = 0;
		MyVirtualProtect(&(pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress), 0x1000, PAGE_EXECUTE_READWRITE, &OldProtect);
		//�ָ�Tls����
		pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress = ShareData.TlsVirtualAddress;
		MyVirtualProtect(&(pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress), 0x1000, OldProtect, &OldProtect);	
		auto TlsTable = (PIMAGE_TLS_DIRECTORY)(pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress + gImageBase);
		
		//�ֶ�����TLS�ص�����
		auto CallBackTable = (PIMAGE_TLS_CALLBACK*)(ShareData.TlsCallBackTableVa - ShareData.OldImageBase + gImageBase);
		while (*CallBackTable)
		{
			(*CallBackTable)((PVOID)gImageBase, DLL_PROCESS_ATTACH, NULL);
			CallBackTable++;
		}
	}
}

//�����Իص�����----Ӳ���ϵ�
DWORD WINAPI ThreadProc1(_In_ LPVOID lpParameter) {
	while (true){
		CONTEXT ConText;
		DWORD  Temp = CONTEXT_DEBUG_REGISTERS;
		Mymemcpy(&ConText, &Temp, sizeof(CONTEXT));
		//��ȡ���ԼĴ���
		MyGetThreadContext(MyGetCurrentThread(), &ConText);	
		//�жϵ��ԼĴ���
		if (ConText.Dr0 || ConText.Dr1 || ConText.Dr2 || ConText.Dr3)
		{
			MyMessageBoxA(0, "��⵽Ӳ���ϵ�", 0, 0);
		}
	}
}
//�����Իص�����---TEB+0xF24
DWORD WINAPI ThreadProc2(_In_ LPVOID lpParameter) {
	while (true)
	{
		THREADENTRY32 entryThread ;
		DWORD  Temp = 28;
		Mymemcpy(&entryThread, &Temp, sizeof(THREADENTRY32));
		//�߳̿���
		HANDLE hSnapshot = MyCreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return 0;
		if (!MyThread32First(hSnapshot, &entryThread))
			return 0;
		//�����߳̿���
		do{
			DWORD ThreadId = entryThread.th32OwnerProcessID;
			HANDLE hThread = MyOpenThread(THREAD_ALL_ACCESS, NULL, ThreadId);
			THREAD_BASIC_INFORMATION threadBasicInfo;
			//��ѯTEB��ַ
			LONG status = MyNtQueryInformationThread(hThread, ThreadBasicInformation, &threadBasicInfo, sizeof(threadBasicInfo), NULL);
			DWORD* DbgSsReserved = (DWORD*)((DWORD)(threadBasicInfo.TebBaseAddress) + 0xF20);
			if (*(DbgSsReserved + 1))
				MyMessageBoxA(0, "�߳�TEB+0xF24��Ϊ�գ����߳����ڵ��Գ���", 0, 0);
		
		} while (MyThread32Next(hSnapshot, &entryThread));
	}
}

//������֮ ���������̵߳�TEB���鿴TEB+0xF24�����TEB��0xF24��ֵ����ô����߳̾��ǵ�����
void SeeTeb_F24() {
	gThread = MyCreateThread(NULL, NULL, ThreadProc2, NULL, 0, NULL);
}

//������֮ ����Ӳ���ϵ�
void GetDr0(){
	gThread = MyCreateThread(NULL, NULL, ThreadProc1,NULL, 0, NULL);
}

//������֮ PEB+0x68
void CheckNtGlobalFlag() {
	int NtGlobalFlag = 0;
	__asm {		
		mov eax, dword ptr fs : [0x30]// ͨ�� TEB ƫ��Ϊ 0x30 �ҵ� PEB �ṹ		
		mov eax, dword ptr[eax + 0x68]// ͨ�� PEB ƫ��Ϊ 0x68 �ĵط��ҵ� NtGlobalFlag		
		mov NtGlobalFlag, eax		  // �����ǰ�Ľ��̱����ԣ�����ľ��� 0x70
	}
	if(NtGlobalFlag==0x70)
		MyMessageBoxA(0,"PEB+0x68==0x70,��ǰ������\n",0,0);
}

//������
void Debugging() {
	GetDr0();
	//SeeTeb_F24();
	if (MyIsDebuggerPresent())
		MyMessageBoxA(0,"��ǰ����[��]����״̬\n",0,0);
	CheckNtGlobalFlag();
}

//��ѹ������
void UnCompression(SHARE ShareData){
	// 1.��ȡ����ͷ�׵�ַ	
	auto SecTextHead = GetSecHeadAddr(gImageBase, ".text");

	// 2.��ѹѹ������

	// �ڴ��ַ
	PCHAR lpPacked = ((PCHAR)gImageBase + ShareData.TextRVA);
	// ��ȡ��ѹ��Ĵ�С
	DWORD dwPackedSize = aPsafe_get_orig_size(lpPacked);
	// �����ڴ�
	PCHAR lpBuffer = (PCHAR)MyVirtualAlloc(NULL, dwPackedSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	// ��ѹ
	aPsafe_depack(lpPacked, ShareData.TextCompressSize, lpBuffer, dwPackedSize);
	//3. ��ԭ���Σ���.text���ε�����д��ԭ��ַ
	DWORD OldProtect = 0;
	MyVirtualProtect(lpPacked, dwPackedSize, PAGE_EXECUTE_READWRITE, &OldProtect);
	Mymemcpy(lpPacked, lpBuffer, SecTextHead->Misc.VirtualSize);
	MyVirtualProtect(lpPacked, dwPackedSize, OldProtect, &OldProtect);
}




// ȥ�����е��������͵������������Ʒ���
extern "C"
{
	__declspec(dllexport) SHARE ShareData;
	__declspec(dllexport) __declspec(naked) void start()
	{
		// ��
		_asm
		{
			push - 1
			push 0
			push 0
			mov eax, dword ptr fs : [0]
			push eax
			mov dword ptr fs : [0], esp
			sub esp, 0x68
			push ebx
			push esi
			push edi
			pop eax
			pop eax
			pop eax
			add esp, 0x68
			pop eax
			mov dword ptr fs : [0], eax
			pop eax
			pop eax
			pop eax
			pop eax
			mov ebp, eax	
			push - 1
			push 0
			push 0
			mov eax, dword ptr fs : [0]
			push eax
			mov dword ptr fs : [0], esp
			sub esp, 0x68
			push ebx
			push esi
			push edi
			pop eax
			pop eax
			pop eax
			add esp, 0x68
			pop eax
			mov dword ptr fs : [0], eax
			pop eax
			pop eax
			pop eax
			pop eax
			mov ebp, eax

			mov eax, fs:[0x30]
			mov eax, [eax + 0x8]		//��ȡ��ǰ����gImageBase
			mov gImageBase, eax
		}
		GetFunAdd();					//��ȡGetProcAddress
		GetApi();						//��ȡAPI
		CallShllFun();					//���뵯��
		UnCompression(ShareData);		//��ѹ
		DecSection();					//���ܴ����
		DecIat(ShareData);				//����IAT
		FixReloca(ShareData);			//�޸��ض�λ
		CallTls(ShareData);				//����TLS		
		Debugging();					//������
		__asm
		{
			mov eax, gImageBase				//��ȡ��ǰ����gImageBase
			mov ebx, ShareData.DestOEP		
			add eax,ebx
			jmp eax
		}
	}
}
