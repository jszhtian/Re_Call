#include "../pack/Share.h"
#include"../aplib/aplib.h"
#include <Windows.h>
#pragma comment(lib, "../aplib/aplib.lib")
// 对 dll 的区段进行了合并，并设置区段属性为[读写执行]
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
//遍历线程相关API
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
//其他API
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
//窗口操作API
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
		mov eax,[eax+0x1c]		//第一个模块
		mov eax,[eax]			//kernel模块
		mov eax,[eax+0x8]		//kernel基址
		mov gKernelBase,eax		
		mov edx,[eax+0x3c]
		add edx,eax
		mov edx,[edx+0x78]
		add edx,eax				//导出表
		mov ebx,[edx+0x1c]
		add ebx,eax
		mov Eat,ebx				//地址表
		mov ebx,[edx+0x20]
		add ebx,eax
		mov Ent,ebx				//名称表
		mov ebx,[edx+0x24]
		add ebx,eax
		mov Eot,ebx				//序号表

		xor ebx, ebx
		jmp tag_FristCmp
	tag_CmpFunNameLoop:
		inc ebx
	tag_FristCmp:
		mov esi, Ent
		mov esi, [ebx * 0x4 + esi]					//遍历名称表			
		lea esi, [esi + eax]						//函数名称VA
		lea edi, dword ptr Buff						//GetProAddress字符串地址
		mov ecx, 0xE								//GetProAddress长度
		cld											//清除方向位
		repe cmpsb									//循环比较字符
		jne tag_cmpFunNameLoop

		mov esi,Eot									
		xor edi, edi
		mov di, [ebx * 2 + esi]						//序号
		//使用序号在地址表中找到函数地址
		mov edx, Eat
		mov esi, [edx + edi * 4]					//得到函数地址RVA
		lea eax, [esi + eax]						//函数地址
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
//解密代码
void DecSection() {
	
	DWORD OldProtect;
	//获取.text区段
	auto SectionHead = GetSecHeadAddr(gImageBase, ".text");
	DWORD Size = SectionHead->Misc.VirtualSize;
	CHAR* Address = (CHAR*)(SectionHead->VirtualAddress + gImageBase);
	MyVirtualProtect(Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
	//循环解密
	while (Size) {
		*Address = *Address ^ 0x15;
		Address++;
		Size--;
	}
	MyVirtualProtect(Address, Size, OldProtect, &OldProtect);
}

//回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CHAR Buff[0x11] = {0};
	HWND h1000 = 0;
	DWORD Len = 0;
	switch (message)
	{
	case WM_CREATE:
		MyCreateWindowExA(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 200, 30, hWnd, (HMENU)0x1000, NULL, NULL);
		MyCreateWindowExA(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("验证密码"), WS_CHILD | WS_VISIBLE, 10, 40, 80, 30, hWnd, (HMENU)0x1001, NULL, NULL);
		MyCreateWindowExA(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("取消启动"), WS_CHILD | WS_VISIBLE, 100, 40, 80, 30, hWnd, (HMENU)0x1002, NULL, NULL);
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
							MyMessageBoxA(0, TEXT("密码不能为空"), TEXT("提示"), 0);
						}
						else if (!Mymemcmp("1234", Buff, Len)) {
							MyMessageBoxA(0, TEXT("密码正确"), TEXT("提示"), 0);							
							MySendMessageA(gParentWnd, WM_CLOSE, 0, 100);
						}
						else {
							MyMessageBoxA(0, TEXT("密码错误"), TEXT("错误"), 0);
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

//密码弹框
void CallShllFun() {
	//__asm int 3
	WNDCLASS wc = { 0 };
	HMODULE hInstance = MyGetModuleHandleA(NULL);
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//类风格
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;//实例句柄，代表此程序
	wc.hIcon = 0;
	wc.hCursor = 0;
	wc.hbrBackground = 0;//BRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;//菜单
	wc.lpszClassName = ("加密壳");
	MyRegisterClassA(&wc);
	gParentWnd = MyCreateWindowExA(
		0,
		"加密壳",//类名
		"C++写的壳",//窗口名
		WS_OVERLAPPEDWINDOW,//重叠窗口的风格   重要的参数----窗口风格
		100, 200, 300, 130, //位置和大小
		NULL,               //父窗口句柄
		NULL,               //菜单句柄，没有菜单
		hInstance,          //实例句柄
		NULL                //附加信息
	);
	//显示、更新窗口
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

//修复重定位
void FixReloca(SHARE ShareData) {
	auto Relocs = (PIMAGE_BASE_RELOCATION)(ShareData.dwRelocRva + gImageBase);
	DWORD OldProtect = 0;
	while (Relocs->VirtualAddress)
	{
		DWORD OldProtect = 0;
		MyVirtualProtect((LPVOID)(gImageBase + Relocs->VirtualAddress), 0x1000, PAGE_READWRITE, &OldProtect);

		TypeOffset* items = (TypeOffset*)(Relocs + 1);
		//遍历重定位项
		int count = (Relocs->SizeOfBlock - 8) / 2;
		for (int i = 0; i < count; ++i)
		{
			if (items[i].Type == 3)
			{
				DWORD Temp = 0;
				// 计算出每一个需要重定位的数据所在的地址
				DWORD* item = (DWORD*)(gImageBase + Relocs->VirtualAddress + items[i].Offset);
				// 这里操作的是需要重定位的数据，通常是代码段（不可写）
				*item = *item + gImageBase - ShareData.OldImageBase;
			}
		}
		MyVirtualProtect((LPVOID)(gImageBase + Relocs->VirtualAddress), 0x1000, OldProtect, &OldProtect);
		// 下一个重定位块
		Relocs = (PIMAGE_BASE_RELOCATION)(Relocs->SizeOfBlock + (DWORD)Relocs);
	}
}

//修复并加密IAT
void DecIat(SHARE ShareData) {
	auto ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)(gImageBase + ShareData.ImpTableVA);
	while (ImportTable->Name)
	{
		//获取DLL名称
		CHAR* Name = (CHAR*)(ImportTable->Name+gImageBase);
		HMODULE Module = MyLoadLibraryExA(Name, NULL, NULL);
		auto Iat = (int*)(ImportTable->FirstThunk + gImageBase);
		for (size_t i = 0; Iat[i]!=0; i++)
		{
			DWORD FunAddr = 0;
			DWORD OldProtect;
			BYTE* DecCode = (BYTE*)MyVirtualAlloc(0, 0x50, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			//解密代码
			BYTE OpCode[] = {
			0xE8, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x58, 0x90, 0xB8,		
			0xA4, 0xA3, 0xA2, 0xA1,			//加密							
			0xEB, 0x04, 0xE8, 0xBF, 0x78, 0xCC, 0x35, 0x59, 0x10, 0x40, 
			0x00, 0xEB, 0x01, 0xE9, 0x50, 0xEB, 0x02, 0x61, 0xE8, 0x90,	
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xEB, 0x05, 0xE8, 0x5B, 
			0xC3, 0xE8, 0x88, 0x90, 0xC3, 0xE8, 0x72, 0x6B, 0x77, 0x88	
			};			
			MyVirtualProtect(&Iat[i], 0x50, PAGE_EXECUTE_READWRITE, &OldProtect);
			//最高位为1，序号导入
			if ((DWORD)Iat[i] & 0x80000000) 
				FunAddr = (DWORD)MyGetProcAddress(Module, (LPCSTR)(Iat[i] & 0xffff));			
			else{
				auto AddressOfData = (PIMAGE_IMPORT_BY_NAME)(Iat[i] + gImageBase);
				FunAddr = (DWORD)MyGetProcAddress(Module, AddressOfData->Name);//异或前77216210
			}
			//将加密后的函数地址写入堆空间的解密代码
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

//手动调用Tls
void CallTls(SHARE ShareData) {
	//如果存在TLS表
	if (ShareData.TlsVirtualAddress)
	{
		DWORD OldProtect = 0;
		MyVirtualProtect(&(pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress), 0x1000, PAGE_EXECUTE_READWRITE, &OldProtect);
		//恢复Tls数据
		pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress = ShareData.TlsVirtualAddress;
		MyVirtualProtect(&(pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress), 0x1000, OldProtect, &OldProtect);	
		auto TlsTable = (PIMAGE_TLS_DIRECTORY)(pOptionHead(gImageBase)->DataDirectory[9].VirtualAddress + gImageBase);
		
		//手动调用TLS回调函数
		auto CallBackTable = (PIMAGE_TLS_CALLBACK*)(ShareData.TlsCallBackTableVa - ShareData.OldImageBase + gImageBase);
		while (*CallBackTable)
		{
			(*CallBackTable)((PVOID)gImageBase, DLL_PROCESS_ATTACH, NULL);
			CallBackTable++;
		}
	}
}

//反调试回调函数----硬件断点
DWORD WINAPI ThreadProc1(_In_ LPVOID lpParameter) {
	while (true){
		CONTEXT ConText;
		DWORD  Temp = CONTEXT_DEBUG_REGISTERS;
		Mymemcpy(&ConText, &Temp, sizeof(CONTEXT));
		//获取调试寄存器
		MyGetThreadContext(MyGetCurrentThread(), &ConText);	
		//判断调试寄存器
		if (ConText.Dr0 || ConText.Dr1 || ConText.Dr2 || ConText.Dr3)
		{
			MyMessageBoxA(0, "检测到硬件断点", 0, 0);
		}
	}
}
//反调试回调函数---TEB+0xF24
DWORD WINAPI ThreadProc2(_In_ LPVOID lpParameter) {
	while (true)
	{
		THREADENTRY32 entryThread ;
		DWORD  Temp = 28;
		Mymemcpy(&entryThread, &Temp, sizeof(THREADENTRY32));
		//线程快照
		HANDLE hSnapshot = MyCreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return 0;
		if (!MyThread32First(hSnapshot, &entryThread))
			return 0;
		//遍历线程快照
		do{
			DWORD ThreadId = entryThread.th32OwnerProcessID;
			HANDLE hThread = MyOpenThread(THREAD_ALL_ACCESS, NULL, ThreadId);
			THREAD_BASIC_INFORMATION threadBasicInfo;
			//查询TEB地址
			LONG status = MyNtQueryInformationThread(hThread, ThreadBasicInformation, &threadBasicInfo, sizeof(threadBasicInfo), NULL);
			DWORD* DbgSsReserved = (DWORD*)((DWORD)(threadBasicInfo.TebBaseAddress) + 0xF20);
			if (*(DbgSsReserved + 1))
				MyMessageBoxA(0, "线程TEB+0xF24不为空，该线程正在调试程序", 0, 0);
		
		} while (MyThread32Next(hSnapshot, &entryThread));
	}
}

//反调试之 遍历所有线程的TEB，查看TEB+0xF24，如果TEB的0xF24有值，那么这个线程就是调试器
void SeeTeb_F24() {
	gThread = MyCreateThread(NULL, NULL, ThreadProc2, NULL, 0, NULL);
}

//反调试之 设置硬件断点
void GetDr0(){
	gThread = MyCreateThread(NULL, NULL, ThreadProc1,NULL, 0, NULL);
}

//反调试之 PEB+0x68
void CheckNtGlobalFlag() {
	int NtGlobalFlag = 0;
	__asm {		
		mov eax, dword ptr fs : [0x30]// 通过 TEB 偏移为 0x30 找到 PEB 结构		
		mov eax, dword ptr[eax + 0x68]// 通过 PEB 偏移为 0x68 的地方找到 NtGlobalFlag		
		mov NtGlobalFlag, eax		  // 如果当前的进程被调试，保存的就是 0x70
	}
	if(NtGlobalFlag==0x70)
		MyMessageBoxA(0,"PEB+0x68==0x70,当前被调试\n",0,0);
}

//反调试
void Debugging() {
	GetDr0();
	//SeeTeb_F24();
	if (MyIsDebuggerPresent())
		MyMessageBoxA(0,"当前处于[被]调试状态\n",0,0);
	CheckNtGlobalFlag();
}

//解压缩函数
void UnCompression(SHARE ShareData){
	// 1.获取节区头首地址	
	auto SecTextHead = GetSecHeadAddr(gImageBase, ".text");

	// 2.解压压缩区段

	// 内存地址
	PCHAR lpPacked = ((PCHAR)gImageBase + ShareData.TextRVA);
	// 获取解压后的大小
	DWORD dwPackedSize = aPsafe_get_orig_size(lpPacked);
	// 申请内存
	PCHAR lpBuffer = (PCHAR)MyVirtualAlloc(NULL, dwPackedSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	// 解压
	aPsafe_depack(lpPacked, ShareData.TextCompressSize, lpBuffer, dwPackedSize);
	//3. 还原区段，将.text区段的内容写入原地址
	DWORD OldProtect = 0;
	MyVirtualProtect(lpPacked, dwPackedSize, PAGE_EXECUTE_READWRITE, &OldProtect);
	Mymemcpy(lpPacked, lpBuffer, SecTextHead->Misc.VirtualSize);
	MyVirtualProtect(lpPacked, dwPackedSize, OldProtect, &OldProtect);
}




// 去除所有导出函数和导出变量的名称粉碎
extern "C"
{
	__declspec(dllexport) SHARE ShareData;
	__declspec(dllexport) __declspec(naked) void start()
	{
		// 花
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
			mov eax, [eax + 0x8]		//获取当前进程gImageBase
			mov gImageBase, eax
		}
		GetFunAdd();					//获取GetProcAddress
		GetApi();						//获取API
		CallShllFun();					//密码弹窗
		UnCompression(ShareData);		//解压
		DecSection();					//解密代码段
		DecIat(ShareData);				//加密IAT
		FixReloca(ShareData);			//修复重定位
		CallTls(ShareData);				//处理TLS		
		Debugging();					//反调试
		__asm
		{
			mov eax, gImageBase				//获取当前进程gImageBase
			mov ebx, ShareData.DestOEP		
			add eax,ebx
			jmp eax
		}
	}
}
