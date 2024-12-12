#ifndef _H_FUNCTIONDEF__
#define _H_FUNCTIONDEF__
#include <Windows.h>
#include <string>

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;


typedef  HFONT (WINAPI *ptr_CreateFontA)(
	int nHeight, // logical height of font height
	int nWidth, // logical average character width
	int nEscapement, // angle of escapement
	int nOrientation, // base-line orientation angle
	int fnWeight, // font weight
	DWORD fdwItalic, // italic attribute flag
	DWORD fdwUnderline, // underline attribute flag
	DWORD fdwStrikeOut, // strikeout attribute flag
	DWORD fdwCharSet, // character set identifier
	DWORD fdwOutputPrecision, // output precision
	DWORD fdwClipPrecision, // clipping precision
	DWORD fdwQuality, // output quality
	DWORD fdwPitchAndFamily, // pitch and family
	LPCSTR lpszFace // pointer to typeface name string
	);

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

typedef DWORD(WINAPI* ptr_ZwCreateFile)(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, DWORD ObjectAttributes, DWORD IoStatusBlock,
	PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions,
	PVOID EaBuffer, ULONG EaLength);

typedef DWORD(WINAPI* ptr_NtOpenFile)(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, DWORD ObjectAttributes, DWORD IoStatusBlock, ULONG ShareAccess, ULONG OpenOptions);



extern void make_console();
extern DWORD BaseAddr;         //exe»ùÖ·
extern void getFileNameAttr(std::wstring *fpath, std::wstring *fname, std::wstring *suffix, const WCHAR *base, int length);
extern void getFileNameAttrA(std::string *fpath, std::string *fname, std::string *suffix, const char *base, int length);
extern bool isAtWordDir(std::wstring *dstpath, PUNICODE_STRING path);
extern std::string getWorkDirA();
extern std::wstring getWorkDirW();
extern std::string UnicodeToANSI(UINT codepage, const WCHAR* ws, int wlen);
extern std::wstring ANSIToUnicode(UINT codepage, const char* s, int len);
extern std::string ANSItoANSI(UINT srcpage, const char *s, int len, UINT dstpage);
extern std::string replaceSrcDir(std::string *wdir, std::string *dstdir, const char *src);

extern ptr_CreateFontA old_CreateFontA;
extern ptr_CreateWindowExA old_CreateWindowExA;
extern ptr_MessageboxA old_MessageboxA;
extern ptr_CreateFileA old_CreateFileA;
extern ptr_ZwCreateFile old_ZwCreateFile;
extern ptr_FindFirstFileA old_FindFirstFileA;
extern ptr_NtOpenFile old_NtOpenFile;
extern ptr_AppendMenuA old_AppendMenuA;
extern ptr_SendMessageA old_SendMessageA;
extern ptr_GetGlyphOutlineA old_GetGlyphOutlineA;

#endif // !_H_FUNCTIONDEF__