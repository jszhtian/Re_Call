// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
/*
00C113F0      0FBE0430         movsx eax,byte ptr ds:[eax+esi]
00C113F4      25 FF000000      and eax,0xFF
00C113F9   .  894424 10        mov dword ptr ss:[esp+0x10],eax
*/
//static std::mutex mtx;
static char* ret2;
char* __stdcall replaceText(char* src)
{

	__asm {
		pushad
		pushfd
	}
	//auto TmpWstr = ctowJIS(src);
	//auto TmpStr = wtocGBK(TmpWstr);
	//char teast[] = "中文TEST2";
	cout << wtocGBK(ctowJIS(src)) << endl;
	if (strcmp(wtocGBK(ctowJIS(src)), "「あ、そうそう」") == 0)
	{
		strcpy(src, "「啊，对了对了」");
	}
	//strcat(dst, teast);
	__asm {
		popfd
		popad
	}
	return src;
}

static PVOID pGetText = NULL;
static PVOID dGetText = NULL;
//static PVOID jmp1 = NULL;
static PVOID jmp2 = NULL;
__declspec(naked)void GetText()
{
	__asm
	{
		cmp dword ptr ds : [edi + 0x14] , 0x10
		jb CodeFix
		mov eax, dword ptr ds : [edi]
		push eax
		call replaceText
		jmp jmp2

	CodeFix:
		mov eax,edi
		jmp jmp2
		//movsx eax, byte ptr ds : [eax + esi]
		//and eax, 0xFF
		//mov dword ptr ss : [esp + 0x10] , eax
		//mov dword ptr ds : [ebx + 0x388] , ecx
		//cmp eax, 0x9
		//jmp dGetText
	}
}

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
	//cout << Pstr << endl;
	return MessageBoxAOLD(hWnd, Pstr, "提示", uType);
}

void TextSet()
{
	jmp2 = (PVOID)(BaseAddr + 0x713f0);
	pGetText = (PVOID)(BaseAddr + 0x713e4);
	dGetText = (PVOID)(BaseAddr + 0x71403);
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
	cout << hex << (BaseAddr) << " | " << hex << (pGetText) << " | " << hex << (jmp2) << endl;
	DetourTransactionBegin();
	//DetourAttach(&pGetText, GetText);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);
	DetourTransactionCommit();
	//MEM_SetNopCode(((BYTE*)pGetText), 0x12);
	JMPHelper(pGetText, GetText);
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
		TextSet();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

