#ifndef __Common_H__
#define __Common_H__

#include <windows.h>

BOOL StartHook(LPCSTR szProcName, PROC pfnOrg, PROC pfnNew);

LRESULT __stdcall MySendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM IParam);

int __stdcall lstrcmpiEX(LPCSTR lpString1, LPCSTR lpString2);

bool CovtShiftJISToGB(const char* JISStr, char* GBBuf, int nSize);

HWND __stdcall MyCreateWindowExA(
DWORD dwExStyle,//窗口的扩展风格
LPCSTR lpClassName,//指向注册类名的指针
LPCSTR lpWindowName,//指向窗口名称的指针
DWORD dwStyle,//窗口风格
int x,//窗口的水平位置
int y,//窗口的垂直位置
int nWidth,//窗口的宽度
int nHeight,//窗口的高度
HWND hWndParent,//父窗口的句柄
HMENU hMenu,//菜单的句柄或是子窗口的标识符
HINSTANCE hInstance,//应用程序实例的句柄
LPVOID lpParam//指向窗口的创建数据
);
 
#endif
