// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
/*
2B1000:

0039B22D  |> \8BD6          mov edx,esi                              ;  esi = 心形文字
0039B22F  |.  8D4A 01       lea ecx,dword ptr ds:[edx+0x1]
0039B232  |>  8A02          /mov al,byte ptr ds:[edx]
0039B234  |.  42            |inc edx                                 ;  mojika_c.<ModuleEntryPoint>
0039B235  |.  84C0          |test al,al
0039B237  |.^ 75 F9         \jnz short mojika_c.0039B232
0039B239  |.  2BD1          sub edx,ecx                              ;  mojika_c.<ModuleEntryPoint>

0039B2CD  |> \8BD6          mov edx,esi                              ;  esi = dialog text
0039B2CF  |.  8D4A 01       lea ecx,dword ptr ds:[edx+0x1]
0039B2D2  |>  8A02          /mov al,byte ptr ds:[edx]
0039B2D4  |.  42            |inc edx                                 ;  mojika_c.<ModuleEntryPoint>
0039B2D5  |.  84C0          |test al,al
0039B2D7  |.^ 75 F9         \jnz short mojika_c.0039B2D2
0039B2D9  |.  2BD1          sub edx,ecx                              ;  mojika_c.<ModuleEntryPoint>
0039B2DB  |>  52            push edx                                 ;  mojika_c.<ModuleEntryPoint>
0039B2DC  |.  56            push esi                                 ;  mojika_c.<ModuleEntryPoint>
0039B2DD  |.  8D4D D8       lea ecx,[local.10]
0039B2E0  |.  E8 5B90F1FF   call mojika_c.002B4340
0039B2E5  |.  C745 FC 00000>mov [local.1],0x0
0039B2EC  |.  8D55 D8       lea edx,[local.10]
0039B2EF  |.  8B4F 6C       mov ecx,dword ptr ds:[edi+0x6C]          ;  setupapi.758B0C5D
0039B2F2  |.  52            push edx                                 ;  mojika_c.<ModuleEntryPoint>
*/

/*
void __stdcall replaceText(char* src)
{
	//cout << wtocGBK(ctowJIS(src)) << endl;
	cout << Utf8ToGbk(src) << endl;
	TXT << Utf8ToGbk(src) << endl;
	if (strcmp(Utf8ToGbk(src), "「ぁ……ぁ……ぁ……ぁ……ぁぁ……ぁ……」\r\n")==0)
		strcpy(src, GbkToUtf8((char*)"「啊……啊……啊啊……啊啊啊啊……啊啊……啊啊啊啊啊啊啊啊啊啊……」\r\n"));
}

static PVOID pGetText = NULL;
static PVOID pGetText2 = NULL;

__declspec(naked)void GetText()
{
	__asm
	{
		pushad;
		pushfd;
		push esi;
		call replaceText;
		popfd;
		popad;
		jmp pGetText;
	}
}

__declspec(naked)void GetText2()
{
	__asm
	{
		pushad;
		pushfd;
		push esi;
		call replaceText;
		popfd;
		popad;
		jmp pGetText2;
	}
}


void TextSet()
{
	pGetText = (PVOID)(BaseAddr + 0xeb22d);
	pGetText2 = (PVOID)(BaseAddr + 0xeb2cd);
	DetourTransactionBegin();
	DetourAttach((void**)&pGetText, GetText);
	DetourAttach((void**)&pGetText2, GetText2);
	DetourTransactionCommit();
}
*/
void __stdcall ShowFileName(char* FileName)
{
	cout << Utf8ToGbk(FileName) << endl;
}

static PVOID pGetFileNameFromMem = NULL;
__declspec(naked)void GetFileNameFromMem()
{
	__asm
	{
		pushad;
		pushfd;
		push [esi];
		call ShowFileName;
		popfd;
		popad;
		jmp pGetFileNameFromMem;
	}
}

void WINAPI InitFile()
{
	pGetFileNameFromMem = (PVOID)(BaseAddr + 0x3ADB3);
	DetourTransactionBegin();
	DetourAttach((void**)&pGetFileNameFromMem, GetFileNameFromMem);
	DetourTransactionCommit();
}

PVOID g_pOldCreateFontW= CreateFontW;
typedef HFONT(WINAPI* PfuncCreateFontW)(
	int     cHeight,
	int     cWidth,
	int     cEscapement,
	int     cOrientation,
	int     cWeight,
	DWORD   bItalic,
	DWORD   bUnderline,
	DWORD   bStrikeOut,
	DWORD   iCharSet,
	DWORD   iOutPrecision,
	DWORD   iClipPrecision,
	DWORD   iQuality,
	DWORD   iPitchAndFamily,
	LPCWSTR pszFaceName
);

HFONT WINAPI HookCreateFontW(
	int     cHeight,
	int     cWidth,
	int     cEscapement,
	int     cOrientation,
	int     cWeight,
	DWORD   bItalic,
	DWORD   bUnderline,
	DWORD   bStrikeOut,
	DWORD   iCharSet,
	DWORD   iOutPrecision,
	DWORD   iClipPrecision,
	DWORD   iQuality,
	DWORD   iPitchAndFamily,
	LPCWSTR pszFaceName
)
{
	iCharSet = GB2312_CHARSET;
	///*
	//cout << wtoc(pszFaceName) << endl;
	if (!lstrcmpW(pszFaceName, L"FOT-セザンヌ Pro DB")) //FOT-セザンヌ Pro M
		return ((PfuncCreateFontW)g_pOldCreateFontW)(
			cHeight,
			cWidth,
			cEscapement,
			cOrientation,
			cWeight,
			bItalic,
			bUnderline,
			bStrikeOut,
			iCharSet,
			iOutPrecision,
			iClipPrecision,
			iQuality,
			iPitchAndFamily,
			L"思源黑体 CN Medium"
			);


	else if (!lstrcmpW(pszFaceName, L"FOT-セザンヌ Pro M"))
		return ((PfuncCreateFontW)g_pOldCreateFontW)(
			cHeight,
			cWidth,
			cEscapement,
			cOrientation,
			cWeight,
			bItalic,
			bUnderline,
			bStrikeOut,
			iCharSet,
			iOutPrecision,
			iClipPrecision,
			iQuality,
			iPitchAndFamily,
			L"思源黑体 CN Medium"
			);

	else if (!lstrcmpW(pszFaceName, L"FOT-ユトリロ Pro M"))
		return ((PfuncCreateFontW)g_pOldCreateFontW)(
			cHeight,
			cWidth,
			cEscapement,
			cOrientation,
			cWeight,
			bItalic,
			bUnderline,
			bStrikeOut,
			iCharSet,
			iOutPrecision,
			iClipPrecision,
			iQuality,
			iPitchAndFamily,
			L"FOTユトリロ&方正萤雪"
			);

	else if (!lstrcmpW(pszFaceName, L"@FOT-ユトリロ Pro M"))
		return ((PfuncCreateFontW)g_pOldCreateFontW)(
			cHeight,
			cWidth,
			cEscapement,
			cOrientation,
			cWeight,
			bItalic,
			bUnderline,
			bStrikeOut,
			iCharSet,
			iOutPrecision,
			iClipPrecision,
			iQuality,
			iPitchAndFamily,
			L"@FOTユトリロ&方正萤雪"
			);

	else
		return ((PfuncCreateFontW)g_pOldCreateFontW)(
			cHeight,
			cWidth,
			cEscapement,
			cOrientation,
			cWeight,
			bItalic,
			bUnderline,
			bStrikeOut,
			iCharSet,
			iOutPrecision,
			iClipPrecision,
			iQuality,
			iPitchAndFamily,
			pszFaceName
			);
	//*/
	//cout << wtoc(pszFaceName) << endl;
}

const char* szWndName = "丑小鸭与文字祸 | Minikui Mojika no Ko";
PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
	DWORD dwExStyle,
	LPCTSTR lpClassName,
	LPCTSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam);
HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	//00F7F4 046B27A8  "みにくいモジカの子"
	if(!lstrcmpA((CHAR*)lpWindowName, "みにくいモジカの子"))
		return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	else
		return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldSetWindowTextW = SetWindowTextW;
typedef bool (WINAPI* PfuncSetWindowTextW)(HWND hWnd, LPCWSTR lpString);
bool WINAPI HookSetWindowTextW(HWND hw, LPCWSTR lpString)
{
	if(wcsstr(lpString, L"みにくいモジカの子"))
		return ((PfuncSetWindowTextW)g_pOldSetWindowTextW)(hw, L"丑小鸭与文字祸 | Minikui Mojika no Ko");
	return ((PfuncSetWindowTextW)g_pOldSetWindowTextW)(hw, lpString);
}

void WINAPI APIHook()
{
	DetourTransactionBegin();
	DetourAttach(&g_pOldCreateFontW, HookCreateFontW);
	
	DetourAttach(&g_pOldCreateWindowExA, HookCreateWindowExA);
	DetourAttach(&g_pOldSetWindowTextW, HookSetWindowTextW);
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
	{
		//make_console();
		//InitFile();
		APIHook();
	}
		break;
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
