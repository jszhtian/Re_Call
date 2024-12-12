// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
/*
00595121  01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01  
00595131  01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 00  .
00595141  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00595151  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00595161  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00595171  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01  ...............
00595181  01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01  
00595191  01 01 01 01 01 01 01 01 01 01 01 01 00 00 00     ...


007E3994  79 73 62 69 6E 5C 79 73 74 25 30 35 64 2E 79 62  ysbin\yst%05d.yb
007E39A4  6E                                               n

00B3F0A8   03518665  \Text = "偊傠偪傖傫偡両丂乣僄儘偄偙偲偑偟偨偔偰偨傑傜側偄壌偺挧愴乣"




void memcopy(void* dest, void* src, size_t size)
{
	DWORD oldProtect;
	VirtualProtect(dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, src, size);
}

void MyCharSet()
{
	BYTE Patch1[] = { 0x01 };
	BYTE Patch2[] = { 0x4D };
	BYTE Patch3[] = { 0x79 };
	BYTE Patch4[] = { 0x53 };
	BYTE Patch5[] = { 0x69 };
	BYTE Patch6[] = { 0x75 };

	//BYTE Patch7[] = { 0x86 };

	memcopy((void*)(BaseAddr + 0x195140), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195141), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195142), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195143), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195144), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195145), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195146), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195147), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195148), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195149), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19514A), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19514B), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19514C), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19514D), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19514E), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19514F), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195150), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195151), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195152), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195153), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195154), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195155), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195156), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195157), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195158), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195159), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19515A), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19515B), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19515C), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19515D), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19515E), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19515F), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195160), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195161), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195162), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195163), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195164), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195165), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195166), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195167), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195168), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195169), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19516A), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19516B), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19516C), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19516D), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19516E), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19516F), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195170), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195171), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195172), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195173), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195174), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195175), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195176), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195177), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195178), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x195179), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19517A), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19517B), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19517C), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19517D), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19517E), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19517F), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19519D), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19519E), Patch1, sizeof(Patch1));
	memcopy((void*)(BaseAddr + 0x19519F), Patch1, sizeof(Patch1));

	memcopy((void*)(BaseAddr + 0X3E3994), Patch2, sizeof(Patch2));
	memcopy((void*)(BaseAddr + 0X3E3995), Patch3, sizeof(Patch3));
	memcopy((void*)(BaseAddr + 0X3E3996), Patch4, sizeof(Patch4));
	memcopy((void*)(BaseAddr + 0X3E3997), Patch5, sizeof(Patch5));
	memcopy((void*)(BaseAddr + 0X3E3998), Patch6, sizeof(Patch6));

}
char* TitleChange(char* Old)
{
	_asm {
		pushad
		pushfd
	}
	if (strcmp("偊傠偪傖傫偡両丂乣僄儘偄偙偲偑偟偨偔偰偨傑傜側偄壌偺挧愴乣", Old) == 0)
	{
		strcpy(Old, "【白梦汉化组】えろちゃんす！ - 简体中文化补丁V1.00");
	}
	_asm {
		popfd
		popad
	}
	return Old;
}

static PVOID TEXT_JMPHookPoint = (PVOID)(BaseAddr + 0x4F784);
static PVOID TEXT_JMPHookPointNext = (PVOID)(BaseAddr + 0x4F78C);
__declspec(naked) void Title()
{
	_asm
	{
		lea eax, dword ptr ds : [edx + 0x1D]
		push eax
		call TitleChange
		push eax
		push 0
		push 0xC
		jmp TEXT_JMPHookPointNext
	}
}

void JMPHelper(PVOID HookPoint, PVOID JMP_FUNC)
{
	DWORD oldProtect = 0;
	VirtualProtect(HookPoint, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	DWORD offset = (DWORD)JMP_FUNC - (DWORD)HookPoint - 5;
	char JMPOp[] = { 0xE9 };
	memcpy(HookPoint, &JMPOp, 1);
	memcpy((char*)HookPoint + 1, &offset, sizeof(DWORD));
	VirtualProtect(HookPoint, 5, oldProtect, &oldProtect);
}*/

LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	strcpy(lplf->lfFaceName, "黑体");

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

wchar_t szTitle[] = L"确认";
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	if (strcmp(Pstr, "ゲームを終了します。よろしいですか？") == 0)
	{
		strcpy(Pstr, "即将结束游戏。确认吗？");

	}
	/*
	if (strcmp(Pstr, "こちらでよろしいですか？（途中で変更は出来ません）") == 0)
	{
		strcpy(Pstr, "这样可以吗？（中途无法再改变）");

	}
	*/
	return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}

void Init()
{
	DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	//DetourAttach(&g_pOldTextOutA, NewTextOutA);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"启动失败", L"启动失败", MB_OK);
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
extern "C" __declspec(dllexport) void dummy(void) {
	return;
}
