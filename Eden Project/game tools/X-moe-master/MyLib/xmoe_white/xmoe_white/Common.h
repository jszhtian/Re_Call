#ifndef __Common_H__
#define __Common_H__

#include <windows.h>

BOOL StartHook(LPCSTR szProcName, PROC pfnOrg, PROC pfnNew);

LRESULT __stdcall MySendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM IParam);

int __stdcall lstrcmpiEX(LPCSTR lpString1, LPCSTR lpString2);

bool CovtShiftJISToGB(const char* JISStr, char* GBBuf, int nSize);

HWND __stdcall MyCreateWindowExA(
DWORD dwExStyle,//���ڵ���չ���
LPCSTR lpClassName,//ָ��ע��������ָ��
LPCSTR lpWindowName,//ָ�򴰿����Ƶ�ָ��
DWORD dwStyle,//���ڷ��
int x,//���ڵ�ˮƽλ��
int y,//���ڵĴ�ֱλ��
int nWidth,//���ڵĿ��
int nHeight,//���ڵĸ߶�
HWND hWndParent,//�����ڵľ��
HMENU hMenu,//�˵��ľ�������Ӵ��ڵı�ʶ��
HINSTANCE hInstance,//Ӧ�ó���ʵ���ľ��
LPVOID lpParam//ָ�򴰿ڵĴ�������
);
 
#endif
