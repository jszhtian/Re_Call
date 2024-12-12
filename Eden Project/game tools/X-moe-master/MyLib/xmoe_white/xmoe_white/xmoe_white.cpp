// xmoe_white.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <Windows.h>

BOOL StartHook(LPCSTR szDllName, PROC pfnOrg, PROC pfnNew)
{
	HMODULE hmod;
	LPCSTR szLibName;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk;
	DWORD dwOldProtect, dwRVA;
	PBYTE pAddr;

	hmod = GetModuleHandle(NULL);
	pAddr= (PBYTE)hmod;
	pAddr += *((DWORD*) &pAddr[0x3C]);
	dwRVA = *((DWORD*) &pAddr[0x80]);

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hmod + dwRVA);

	for(; pImportDesc->Name; pImportDesc++)
	{
		szLibName = (LPCSTR)((DWORD)hmod + pImportDesc->Name);

		if(!stricmp(szLibName, szDllName))
		{
			pThunk = (PIMAGE_THUNK_DATA)((DWORD)hmod + pImportDesc->FirstThunk);

			for(; pThunk->u1.Function; pThunk++)
			{
				if(pThunk->u1.Function == (DWORD)pfnOrg)
				{
					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);

					pThunk->u1.Function = (DWORD) pfnNew;

					VirtualProtect((LPVOID)&pThunk->u1.Function, 4, dwOldProtect, &dwOldProtect);

					return TRUE;
				}

			}
		}
	}
	return FALSE;
}


/*
0045C541  /$ 55             PUSH EBP
0045C542  |. 8BEC           MOV EBP,ESP
0045C544  |. 53             PUSH EBX
0045C545  |. 56             PUSH ESI
0045C546  |. 68 23C54500    PUSH WhiteEte.0045C523                   ; /pModule = "KERNEL32.dll"
0045C54B  |. FF15 34D14500  CALL DWORD PTR DS:[<&KERNEL32.GetModuleH>; \GetModuleHandleA
0045C551  |. 68 31C54500    PUSH WhiteEte.0045C531                   ; /ProcNameOrOrdinal = "CompareStringA"
0045C556  |. 50             PUSH EAX                                 ; |hModule
0045C557  |. FF15 2CD14500  CALL DWORD PTR DS:[<&KERNEL32.GetProcAdd>; \GetProcAddress
0045C55D  |. 6A FF          PUSH -1
0045C55F  |. FF75 0C        PUSH DWORD PTR SS:[EBP+C]
0045C562  |. 6A FF          PUSH -1
0045C564  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]
0045C567  |. 6A 01          PUSH 1
0045C569  |. 68 11040000    PUSH 411
0045C56E  |. FFD0           CALL EAX
0045C570  |. 83C0 FE        ADD EAX,-2
0045C573  |. 5E             POP ESI
0045C574  |. 5B             POP EBX
0045C575  |. 5D             POP EBP
0045C576  \. C2 0800        RETN 8
*/

//lstrcmpiA
int __stdcall lstrcmpiEX(LPCSTR lpString1, LPCSTR lpString2)
{
	int ret = CompareStringA(0x411, 1, lpString1, -1, lpString2, -1);
	return ret - 2;
}


LRESULT __stdcall MySendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM IParam)
{
	LRESULT ret;
	if(Msg == WM_GETTEXT)
	{
		ret = SendMessageA(hWnd, Msg, 0, (LPARAM)"xmoe");
		SetWindowTextA(hWnd,  "xmoe");
	}
	else
		ret = SendMessageA(hWnd, Msg, wParam, IParam);

	return ret;
}


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
)
{
	char*    szGBKTitle;
	szGBKTitle = new char[MAX_PATH];
	memset(szGBKTitle, 0, MAX_PATH);
	CovtShiftJISToGB((const char*)lpWindowName, szGBKTitle, MAX_PATH);
	return (CreateWindowExA(dwExStyle, lpClassName, szGBKTitle, dwStyle, x,y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam));
}

bool CovtShiftJISToGB(const char* JISStr, char* GBBuf, int nSize) 
{ 
    static wchar_t wbuf[2048];
    int nLen; 

    nLen = strlen(JISStr)+1; 
    //if (wbuf==NULL) return false; 
    nLen = MultiByteToWideChar(932, 0, JISStr, 
        nLen, wbuf, nLen); 
    if (nLen > 0) 
        nLen = WideCharToMultiByte(936, 0, wbuf, 
        nLen, GBBuf, nSize, NULL, 0); 
    return nLen!=0; 
}
