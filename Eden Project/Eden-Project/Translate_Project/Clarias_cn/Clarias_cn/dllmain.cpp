// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

DWORD WINAPI HookGetGlyphOutlineA(
	_In_        HDC            hdc,
	_In_        UINT           uChar,
	_In_        UINT           uFormat,
	_Out_       LPGLYPHMETRICS lpgm,
	_In_        DWORD          cbBuffer,
	_Out_       LPVOID         lpvBuffer,
	_In_  const MAT2* lpmat2
)
{
	int len;
	char mbchs[2];
	UINT cp = 936;
	if (IsDBCSLeadByteEx(cp, uChar >> 8))
	{
		len = 2;
		mbchs[0] = (uChar & 0xff00) >> 8;
		mbchs[1] = (uChar & 0xff);
	}
	else
	{
		len = 1;
		mbchs[0] = (uChar & 0xff);
	}
	uChar = 0;
	MultiByteToWideChar(cp, 0, mbchs, len, (LPWSTR)&uChar, 1);

	DWORD Result = 0;

	LOGFONTW* lplf = (LOGFONTW*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(LOGFONTW));

	HFONT hOldFont = (HFONT)GetCurrentObject(hdc, OBJ_FONT);
	GetObject(hOldFont, sizeof(LOGFONTW), lplf);
	LOGFONTW Info = { 0 };
	Info.lfHeight = lplf->lfHeight;
	Info.lfWidth = lplf->lfWidth;
	Info.lfEscapement = lplf->lfEscapement;
	Info.lfOrientation = lplf->lfOrientation;
	Info.lfWeight = lplf->lfWeight;
	Info.lfItalic = lplf->lfItalic;
	Info.lfUnderline = lplf->lfUnderline;
	Info.lfStrikeOut = lplf->lfStrikeOut;
	Info.lfOutPrecision = lplf->lfOutPrecision;
	Info.lfClipPrecision = lplf->lfClipPrecision;
	Info.lfQuality = lplf->lfQuality;
	Info.lfPitchAndFamily = lplf->lfPitchAndFamily;
	// 0019FB88   00B87188  \FaceName = "尮恀僑僔僢僋摍暆 Normal"

	if(!lstrcmpW(lplf->lfFaceName, L"尮恀僑僔僢僋摍暆 Normal"))
		lstrcpyW(Info.lfFaceName, L"微软雅黑");
	else
		lstrcpyW(Info.lfFaceName, lplf->lfFaceName);
	Info.lfCharSet = GB2312_CHARSET;

	HFONT hFont = CreateFontIndirectW(&Info);
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	Result = GetGlyphOutlineW(hdc, uChar, uFormat,
		lpgm, cbBuffer, lpvBuffer, lpmat2);

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);
	HeapFree(GetProcessHeap(), 0, lplf);
	return Result;
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
    const char* szWndName = "【春雪如星汉化组】闪钢的Clarias - v1.0";

    return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void InitAPI()
{
	if(!IATPatch("GDI32.dll", (PROC)GetGlyphOutlineA, (PROC)HookGetGlyphOutlineA))
	{
		MessageBoxW(NULL, L"HookGetGlyphOutlineA error", L"InitAPI", MB_OK | MB_ICONERROR);
		return;
	}
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"HookCreateWindowExA error", L"InitAPI", MB_OK);
        return;
    }
}
/*************************************************************************************************************/
char* mbuff = NULL;
char* Buff = NULL;
DWORD msize = 0;
char* Name = NULL;

void __stdcall AymDumpFile(char* buff, DWORD size)
{
    if (Name && mbuff && msize != 0)
    {
        memset(buff, 0x00, size);
        memcpy(buff, mbuff, msize);
        _asm {
            mov eax, msize;
        }
        cout << "successful" << endl;
        free(mbuff);
        mbuff = NULL;
        msize = 0;
        Name = NULL;
    }
}
void __stdcall AymGetFileName(char* name)
{
    char FileName[MAX_PATH] = { 0 };
    sprintf(FileName, "Clarias_cn\\%s", wtoc(ctowJIS(name)));

    cout << FileName <<  endl;

    FILE* fp = fopen(FileName, "rb");
    if (fp)
    {
        cout << "Read" << endl;
        fseek(fp, 0, SEEK_END);
        msize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        mbuff = (char*)malloc(msize);
        if (mbuff && fread(mbuff, msize, 1, fp))
            Name = name;
        fclose(fp);
    }

}

static PVOID pGetFileName = (PVOID)(BaseAddr + 0x55574D);
__declspec(naked) void GetFileName()
{
    __asm
    {
        pushad
        pushfd
        push eax
        call AymGetFileName
        popfd
        popad
        jmp pGetFileName
    }
}

static PVOID pGetFileBuff = (PVOID)(BaseAddr + 0x555B5B);
__declspec(naked) void GetFileBuff()
{
    __asm
    {
        pushad
        pushfd
        MOV Buff, ECX;
        popfd
        popad
        jmp pGetFileBuff
    }
}
static PVOID pGetFileSize = (PVOID)(BaseAddr + 0x555B60);
__declspec(naked) void mGetFileSize()
{
    __asm
    {
        pushad
        pushfd
        push eax
        push Buff
        call AymDumpFile
        popfd
        popad
        jmp pGetFileSize
    }
}

static PVOID pAllocMemory = (PVOID)(BaseAddr + 0x541232);
__declspec(naked) void mAllocMemory()
{
    __asm
    {
        pushad;
        pushfd;
        cmp msize, 0x00;
        je have;
        mov eax, msize;
        mov dword ptr ds : [esi + 0x20] , eax;
    have:
        popfd;
        popad;
        jmp pAllocMemory;
    }
}

void  InitHook()
{
    
    DetourTransactionBegin();
    //FileSystem Set

    DetourAttach((void**)&pGetFileName, GetFileName);
    //DetourAttach((void**)&pGetFileBuff, GetFileBuff);
    //DetourAttach((void**)&pGetFileSize, mGetFileSize);
    //DetourAttach((void**)&pAllocMemory, mAllocMemory);

    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"ERROR", L"AymKze", MB_OK);
    }

    InitAPI();
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
        InitHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

