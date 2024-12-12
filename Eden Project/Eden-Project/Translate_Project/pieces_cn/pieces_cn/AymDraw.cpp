#include "framework.h"
/*
typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

typedef struct TextOutA_Replace_s
{
	unsigned char OldlpString[MAX_PATH];
	unsigned char NewlpString[MAX_PATH];
	struct TextOutA_Replace_s* next;
}NodeTextOutA_Replace, * LinkTextOutA_Replace;
LinkTextOutA_Replace TextOutA_Replace;

DWORD CheckString(wchar_t* buff)
{
	if (wcsncmp(buff, L"0x", 2) == 0 || wcsncmp(buff, L"0X", 2) == 0)
		return wcstol(buff + 2, NULL, 16);
	else
		return wcstol(buff, NULL, 10);
}

PVOID g_pOldTextOutA = NULL;
typedef int(WINAPI* PfuncTextOutA)(HDC hdc, int x, int y, LPCSTR lpString, int c);
int WINAPI AymTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
	unsigned short uChar = 0;
	memcpy((char*)& uChar + 1, lpString, 1);
	memcpy((char*)& uChar, lpString + 1, 1);
	if (uChar >= 0x8140 && uChar <= 0xE000)
	{


	}
	LPSTR String[MAX_PATH];
	strncpy((unit8*)String, lpString, c);
	NodeTextOutA_Replace* q = TextOutA_Replace;
	while (q->next != NULL)
	{
		q = q->next;
		if (strncmp((unit8*)String, q->OldlpString, c) == 0)
		{
			strncpy((unit8*)String, q->NewlpString, c);
			break;
		}
	}
	return ((PfuncTextOutA)g_pOldTextOutA)(hdc, x, y, (LPCSTR)String, c);
}
*/