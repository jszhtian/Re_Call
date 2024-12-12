#include "framework.h"
#include "detours.h"
#include <iostream>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

////////////�����ַ���////////////////////////////////////////////////////////

#define GBK_CODE_PAGE 936

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
	lplf->lfCharSet = ANSI_CHARSET;
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}


//ò��Win10�� CreateFontA ����ͨ��CreateFontIndirectAʵ�֣����Ե���Hook
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
	fdwCharSet = ANSI_CHARSET;
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
HWND WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	const char* szWndName = "������Ļ��ǰ��ʫ�����������ִκ����顿 gkd.galkey.cc";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

HANDLE WINAPI NewCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	string strOldName(lpFileName);
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	string strPazName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string strNewName;

	if (strPazName == "scr.paz")
	{
		strNewName = strDirName + "cnscr.paz";
	}
	else if (strPazName == "sys.paz")
	{
		strNewName = strDirName + "cnsys.paz";
	}
	else
	{
		strNewName = strOldName;
	}
	return CreateFileA(
		strNewName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

PVOID pCreateFileA = (PVOID)(BaseAddr + 0x94BE);
PVOID retnp = (PVOID)(BaseAddr + 0x94C4);
__declspec(naked) void _CreateFileA()
{
	__asm
	{
		call NewCreateFileA
		jmp retnp
	}
}

void APIHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
	DetourAttach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourAttach(&pCreateFileA, _CreateFileA);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"����ʧ��", L"����ʧ��", MB_OK);
}

void __stdcall LumShowMessage(char* Text)
{
	//cout << Text << endl;
	//02FF7304  97 44 81 40 90 6C                                �D�@�l...


	if (!strcmp(Text, "�i�@��"))
		strcpy(Text, "����Զ");
	else if (!strcmp(Text, "�@�I�@"))
		strcpy(Text, "���ա�");
	else if (!strcmp(Text, "�͂邩"))
		strcpy(Text, "��ң��");
	else if (!strcmp(Text, "�^�@��"))
		strcpy(Text, "�桡��");
	else if (!strcmp(Text, "�H�H�H"))
		strcpy(Text, "������");
	else
		return;
}

PVOID pShowMessage = NULL;
__declspec(naked) void mShowMessage()
{
	_asm
	{
		pushad
		pushfd
		push edi
		call LumShowMessage
		popfd
		popad
		jmp pShowMessage
	}
}

void MessageShowInitFunction()
{
	pShowMessage = (PVOID)(BaseAddr + 0x2060A);
	DetourTransactionBegin();
	DetourAttach((void**)&pShowMessage, mShowMessage);
	if (DetourTransactionCommit() != NOERROR)
		MessageBox(NULL, L"����ʧ��", L"����ʧ��", MB_OK);
}
static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//make_console();
		APIHook();
		MessageShowInitFunction();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void)
{
	return;
}