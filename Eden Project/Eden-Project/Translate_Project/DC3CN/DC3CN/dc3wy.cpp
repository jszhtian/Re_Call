#include "framework.h"
#include "LoliHook/LoliHook.h"


#ifdef DC3WY1

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

typedef HFONT(WINAPI* ptr_CreateFontIndirectA)(const LOGFONT* lpLogFont);

typedef HWND(WINAPI* ptr_CreateWindowExA)(
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

typedef int (WINAPI* ptr_MessageboxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

typedef HANDLE(WINAPI* ptr_CreateFileA)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);


typedef HANDLE(WINAPI* ptr_FindFirstFileA)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);

typedef BOOL(WINAPI* ptr_AppendMenuA) (HMENU hMenu, UINT uFlags, UINT_PTR uIDNewltem, LPCTSTR lpNewltem);

typedef LRESULT(WINAPI* ptr_SendMessageA)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM IParam);
typedef DWORD(WINAPI* ptr_GetGlyphOutlineA)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, const MAT2 *lpmat2);

ptr_CreateFontIndirectA old_CreateFontIndirectA = NULL;
ptr_CreateWindowExA old_CreateWindowExA = NULL;
ptr_MessageboxA old_MessageboxA = NULL;
ptr_CreateFileA old_CreateFileA = NULL;
ptr_FindFirstFileA old_FindFirstFileA = NULL;
ptr_AppendMenuA old_AppendMenuA = NULL;
ptr_SendMessageA old_SendMessageA = NULL;
ptr_GetGlyphOutlineA old_GetGlyphOutlineA = NULL;
HFONT jisFont;
HFONT chsFont;

int ccount = 0;
const char *ffname = "黑体";
HFONT WINAPI new_CreateFontIndirectA(const LOGFONT* lpLogFont)
{
	LOGFONTA nft = *(LOGFONTA*)lpLogFont;
	memset(nft.lfFaceName, 0, 32);
	memcpy(nft.lfFaceName, ffname, strlen(ffname));
	nft.lfCharSet = 0x86;
	//lpLogFont->lfCharSet = 0x86;
	//printf("%x, %s\n",lpLogFont->lfCharSet, lpLogFont->lfFaceName);
	HFONT tmp = old_CreateFontIndirectA((const LOGFONT*)&nft);
	return tmp;
}


HWND WINAPI new_CreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	const char* szWndName = "【-COKEZIGE汉化组-】Da Capo Ⅲ With You v0.51";

	return old_CreateWindowExA(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}


int WINAPI new_MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	if (strcmp(Pstr, "終了しますか？") == 0) {
		strcpy(Pstr, "结束游戏吗？");
	}
	return old_MessageboxA(hWnd, Pstr, "提示", uType);
}


//======================================
void ReplacePath(string *path) {
	string nname = "wy_cn\\" + path->substr(path->find_last_of("\\") + 1);
	if (GetFileAttributesA(nname.c_str()) != INVALID_FILE_ATTRIBUTES) {
		printf("ReadFileFromKaze:%s\n", nname.c_str());
		path->assign(nname);
	}
}

HANDLE WINAPI new_CreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	string newName(lpFileName);
	ReplacePath(&newName);

	return old_CreateFileA(
		newName.c_str(),
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

HANDLE WINAPI new_FindFirstFileA(
	LPCSTR lpFileName,
	LPWIN32_FIND_DATAA lpFindFileData) {

	string newName(lpFileName);
	ReplacePath(&newName);

	return old_FindFirstFileA(newName.c_str(), lpFindFileData);
}

BOOL WINAPI new_AppendMenuA(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewltem, LPCTSTR lpNewltem) {
	//printf("%s", lpNewltem);
	if (strcmp((const char*)lpNewltem, "僶乕僕儑儞忣曬") == 0) {
		return old_AppendMenuA(hMenu, uFlags, uIDNewltem, (LPCTSTR)"版本信息");
	}
	else return old_AppendMenuA(hMenu, uFlags, uIDNewltem, lpNewltem);
}

LRESULT WINAPI new_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM IParam) {
	const char *oldptr = (const char*)IParam;
	if (oldptr && strlen(oldptr) == 77) {
		if (oldptr[0] == 0x44 && oldptr[1] == 0x2e && oldptr[2] == 0x43) {
			oldptr = "～初音岛Ⅲ·白金伙伴～ Ver.1.01\n资源版本 Ver.1.01";
			IParam = (LPARAM)oldptr;
		}
	}
	return old_SendMessageA(hWnd, Msg, wParam, IParam);
}

DWORD WINAPI new_GetGlyphOutlineA(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, const MAT2 *lpmat2) {
	if (uChar == 0xA1EC) {
		uChar = 0x81F4;
		SelectObject(hdc, jisFont);
		DWORD t = old_GetGlyphOutlineA(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
		SelectObject(hdc, chsFont);
		return t;
	}
	return old_GetGlyphOutlineA(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}



bool Init()
{
	old_CreateFileA = (ptr_CreateFileA)LoliHook::ApiHook("kernel32.dll", "CreateFileA", &new_CreateFileA);
	old_FindFirstFileA = (ptr_FindFirstFileA)LoliHook::ApiHook("kernel32.dll", "FindFirstFileA", &new_FindFirstFileA);
	old_CreateFontIndirectA = (ptr_CreateFontIndirectA)LoliHook::ApiHook("gdi32.dll", "CreateFontIndirectA", &new_CreateFontIndirectA);
	old_CreateWindowExA = (ptr_CreateWindowExA)LoliHook::ApiHook("user32.dll", "CreateWindowExA", &new_CreateWindowExA);
	old_AppendMenuA = (ptr_AppendMenuA)LoliHook::ApiHook("user32.dll", "AppendMenuA", &new_AppendMenuA);
	old_SendMessageA = (ptr_SendMessageA)LoliHook::ApiHook("user32.dll", "SendMessageA", &new_SendMessageA);
	old_MessageboxA = (ptr_MessageboxA)LoliHook::ApiHook("user32.dll", "MessageBoxA", &new_MessageBoxA);
	old_GetGlyphOutlineA = (ptr_GetGlyphOutlineA)LoliHook::ApiHook("gdi32.dll", "GetGlyphOutlineA", &new_GetGlyphOutlineA);

	if (old_CreateFileA == NULL || old_FindFirstFileA == NULL || old_CreateFontIndirectA == NULL || old_MessageboxA == NULL ||
		old_CreateWindowExA == NULL || old_AppendMenuA == NULL || old_SendMessageA == NULL || old_GetGlyphOutlineA == NULL) {
		MessageBoxA(NULL, "api hook 失败！", "error", MB_OK);
		return false;
	}

	MessageBoxW(NULL, L"本补丁由【-COKEZIGE汉化组-】制作\n禁止一切录播直播和商业行为", L"警告", MB_OK | MB_ICONINFORMATION);
	return true;

	//ApiHook("user32.dll", "CreateWindowExA", (intptr_t)(&NewCreateWindowExA));
	//ApiHook("kernel32.dll", "CreateFileA", (intptr_t)(&NewCreateFileA));
	//ApiHook("kernel32.dll", "FindFirstFileA", (intptr_t)(&NewFindFirstFileA));
	//ApiHook("user32.dll", "AppendMenuA", (intptr_t)(&NewAppendMenuA));
	//ApiHook("user32.dll", "SendMessageA", (intptr_t)(&NewSendMessageA));
	//ApiHook("user32.dll", "MessageBoxA", (intptr_t)(&MessageBoxAEx));
	/*
	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");

	DetourTransactionBegin();
	DetourAttach((void**)& CreateFontAOLD, CreateFontAEx);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	DetourAttach(&g_pOldFindFirstFileA, NewFindFirstFileA);
	DetourAttach(&g_pOldAppendMenuA, NewAppendMenuA);
	DetourAttach(&g_pOldSendMessageA, NewSendMessageA);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);

	if (DetourTransactionCommit() != NOERROR) { MessageBox(NULL, L"INIT FUNCTION ERROR", L"System", MB_OK | MB_ICONERROR); }
	else { MessageBox(NULL, L"本补丁由【-COKEZIGE汉化组-】制作\n禁止一切录播直播和商业行为", L"警告", MB_OK | MB_ICONINFORMATION); }
	*/
}


/**********************************************************************************************************************/

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		make_console();
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


#endif // 