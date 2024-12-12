// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#define GBK_CODE_PAGE 932

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = DEFAULT_CHARSET;
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}


//貌似Win10中 CreateFontA 不再通过CreateFontIndirectA实现，所以单独Hook
PVOID g_pOldCreateFontA = CreateFontA;
typedef int (WINAPI* PfuncCreateFontA)(int nHeight,
	int nWidth,
	int nEscapement,
	int nOrientation,
	int fnWeight,
	DWORD fdwltalic,
	DWORD fdwUnderline,
	DWORD fdwStrikeOut,
	DWORD fdwCharSet,
	DWORD fdwOutputPrecision,
	DWORD fdwClipPrecision,
	DWORD fdwQuality,
	DWORD fdwPitchAndFamily,
	LPCTSTR lpszFace);
int WINAPI NewCreateFontA(int nHeight,
	int nWidth,
	int nEscapement,
	int nOrientation,
	int fnWeight,
	DWORD fdwltalic,
	DWORD fdwUnderline,
	DWORD fdwStrikeOut,
	DWORD fdwCharSet,
	DWORD fdwOutputPrecision,
	DWORD fdwClipPrecision,
	DWORD fdwQuality,
	DWORD fdwPitchAndFamily,
	LPCTSTR lpszFace)
{
	fdwCharSet = DEFAULT_CHARSET;
	return ((PfuncCreateFontA)g_pOldCreateFontA)(nHeight,
		nWidth,
		nEscapement,
		nOrientation,
		fnWeight,
		fdwltalic,
		fdwUnderline,
		fdwStrikeOut,
		fdwCharSet,
		fdwOutputPrecision,
		fdwClipPrecision,
		fdwQuality,
		fdwPitchAndFamily,
		lpszFace);
}

PVOID g_pOldEnumFontFamiliesExA = EnumFontFamiliesExA;
typedef int (WINAPI* PfuncEnumFontFamiliesExA)(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags);
int WINAPI NewEnumFontFamiliesExA(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags)
{
	lpLogfont->lfCharSet = GB2312_CHARSET;
	lpLogfont->lfFaceName[0] = '\0';
	return ((PfuncEnumFontFamiliesExA)g_pOldEnumFontFamiliesExA)(hdc, lpLogfont, lpEnumFontFamExProc, lParam, dwFlags);
}

PVOID HookAddr_FakeCharProcessFunc = (PVOID)0x4C2691;
PVOID TODO_COPY = (PVOID)0x4C25F8;
__declspec(naked) void FakeCharProcessFunc()
{
	__asm
	{
		lea ebx, dword ptr ds : [eax] ;

		mov edx, ebx;
		shr edx, 0x8;
		mov byte ptr ss : [ebp + 0xC] , dl;
		mov byte ptr ss : [ebp + 0xD] , bl;// 不调整顺序

		jmp TODO_COPY;
	}
}

PVOID HookAddr_FakeCharProcessFunc2 = (PVOID)0x50E366;
PVOID JmpOut = (PVOID)0x50E36B;
DWORD IsDBStr = 0;
DWORD tmp_reg = 0;
__declspec(naked) void FakeCharProcessFunc2()
{
	__asm
	{
		cmp [eax], 0x80;
		jb do_exit;
		mov IsDBStr, 0x1;
		/*
		movzx ecx, word ptr ds : [eax] ;
		mov dword ptr ss : [ebp - 0x10] , ecx;
		movzx ecx, byte ptr ds : [eax] ;// 复原
		test ecx, ecx;
		jmp JmpOut;
		*/
		movzx ecx, word ptr ds : [eax] ;
		mov byte ptr ss : [ebp - 0xF] , cl;
		shr ecx, 0x8;
		mov byte ptr ss : [ebp - 0x10] , cl;
		movzx ecx, byte ptr ds : [eax] ;// 复原
		test ecx, ecx;
		jmp JmpOut;
	do_exit:
		mov IsDBStr, 0x0;
		jmp HookAddr_FakeCharProcessFunc2;
	}
}

PVOID HookAddr_FixCharLen = (PVOID)0x50E371;
__declspec(naked) void FixCharLen()
{
	__asm
	{
		cmp IsDBStr, 0;
		je do_exit;
		inc eax;// 如果是双字节，这里还要加1，避免文字计算错位。
	do_exit:
		jmp HookAddr_FixCharLen;
	}
}



PVOID HookAddr_FixFontLen = (PVOID)0x50E171;
PVOID JmpOut2 = (PVOID)0x50E177;
__declspec(naked) void FixFontLen()
{
	__asm
	{
		cmp [edi], 0x80;
		jb do_exit;
		mov ax, word ptr ds : [edi] ;
		mov[ebp - 0x10], ax;
		add edi, 0x2;
		test ax, ax;
		jmp JmpOut2;
	do_exit:
		jmp HookAddr_FixFontLen;
	}
}

PVOID HookAddr_FixFontLen2 = (PVOID)0x50E218;
PVOID JmpOut3 = (PVOID)0x50E21E;

__declspec(naked) void FixFontLen2()
{
	__asm
	{
		cmp[edi], 0x80;
		jb do_exit;
		mov ax, word ptr ds : [edi] ;
		mov[ebp - 0x10], ax;
		add edi, 0x2;
		jmp JmpOut3;
	do_exit:
		jmp HookAddr_FixFontLen2;
	}
}
void Init()
{
	DetourTransactionBegin();

	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
	DetourAttach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
	
	DetourAttach(&HookAddr_FakeCharProcessFunc, FakeCharProcessFunc);
	DetourAttach(&HookAddr_FakeCharProcessFunc2, FakeCharProcessFunc2);

	DetourAttach(&HookAddr_FixCharLen, FixCharLen);
//	DetourAttach(&HookAddr_FixFontLen, FixFontLen);
//	DetourAttach(&HookAddr_FixFontLen2, FixFontLen2);
	
	DetourTransactionCommit();
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


__declspec(dllexport)void WINAPI AyamiKaze()
{
}