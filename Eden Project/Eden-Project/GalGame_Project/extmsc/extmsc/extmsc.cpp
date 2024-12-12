// extmsc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include<assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include  <Windows.h>
using namespace std;
#define MAX_LOADSTRING 100

#define u8(x) *(pM+(x))
#define u32(x) *(int*)(pM+(x))
#define val(x) *(int*)(pM+(x)+1),*(pM+(x))

DWORD Msc2Txt(char* lpMsc, int nSizeM, char* lpTxt, int nSizeT)
{
	char* pM = lpMsc;
	char* pT = lpTxt;
	FILE* f = fopen("text.txt", "wb");
	while (pM < lpMsc + nSizeM)
	{
		(*pM++) ^= 0x77;
	}
	pM = lpMsc;

	strcpy(pT, "Table1:\r\n");
	pT += strlen("Table1:\r\n");
	int hdrsize = *(int*)(lpMsc + 2);
	int t1size = *(int*)(lpMsc + 6);
	int t2size = hdrsize - t1size - 10;
	pM += 11;
	while ((pM - lpMsc - 11) < t1size)
	{
		int a = wsprintf(LPSTR(pT), _T("Index:%d\tOffset:%X\r\n"), *(int*)pM, *(int*)(pM + 4));
		pT += a;
		pM += 9;
	}
	strcpy(pT, "Table2:\r\n");
	pT += strlen("Table2:\r\n");
	pM += 4;
	while (pM < lpMsc + hdrsize)
	{
		int a = wsprintf(LPSTR(pT), _T("Index:%d\tOffset:%X\r\n"), *(int*)pM, *(int*)(pM + 4));
		pT += a;
		pM += 9;
	}

	pM = lpMsc + hdrsize;
	char str[512];
	while (pM < lpMsc + nSizeM)
	{
		char cc1tl1 = *pM++;
		char ctl2 = *pM++;
		switch (cc1tl1)
		{
		case 0:
			strcpy(pT, "P- ");
			pT += 3;
			switch (ctl2)
			{
			case 0:
				pT += wsprintf((LPSTR)pT, _T("i0(%d,%d,%d,%dm(%d))\r\n"), u8(0), u32(1), u8(5), val(6));
				pM += 11;
				break;
			case 1:
				pT += wsprintf((LPSTR)pT, _T("i1(%dm(%d),%d,%dm(%d),%d,%dm(%d))\r\n"), val(0), u8(5), val(6), u8(11), val(12));
				pM += 17;
				break;
			case 2:
				pT += wsprintf((LPSTR)pT, _T("i2(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 3:
				pT += wsprintf((LPSTR)pT, _T("CallOffsetTable(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 4:
				strcpy(pT, "i4()\r\n");
				pT += strlen("i4()\r\n");
				break;
			case 5:
				pT += wsprintf((LPSTR)pT, _T("i5(%dm(%d),%d)\r\n"), val(0), u8(5));
				pM += 6;
				break;
			case 6:
				strcpy(pT, "return()\r\n");
				pT += strlen("return()\r\n");
				break;
			case 7:
				pT += wsprintf((LPSTR)pT, _T("LoadScriptNo(%d)\r\n"), u8(0));
				pM++;
				break;
			case 8:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("ProcessScript(\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			case 9:
				pT += wsprintf((LPSTR)pT, _T("YesNoDialog()\r\n"));
				break;
			case 0xa:
				pT += wsprintf((LPSTR)pT, _T("YesNoDialog2(%d)\r\n"), u8(0));
				pM++;
				break;
			case 0xc:
				pT += wsprintf((LPSTR)pT, _T("HideMsgWnd()\r\n"));
				break;
			case 0xd:
				pT += wsprintf((LPSTR)pT, _T("EnterAutoMode()\r\n"));
				break;
			case 0xe:
				pT += wsprintf((LPSTR)pT, _T("CtrlSkip()\r\n"));
				break;
			case 0x10:
				pT += wsprintf((LPSTR)pT, _T("SaveDialog(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x11:
				pT += wsprintf((LPSTR)pT, _T("LoadPressed(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x16:
				pT += wsprintf((LPSTR)pT, _T("i22(%d)\r\n"), u8(0));
				pM++;
				break;
			case 0x17:
				pT += wsprintf((LPSTR)pT, _T("i23(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x1d:
				pT += wsprintf((LPSTR)pT, _T("DestroyWnd(%d,%dm(%d),%dm(%d))\r\n"), u8(0), val(1), val(6));
				pM += 11;
				break;
			case 0x20:
				pT += wsprintf((LPSTR)pT, _T("i32(%dm(%d),%dm(%d),%d,%d)\r\n"), val(0), val(5), u8(10), u8(11));
				pM += 12;
				break;
			case 0x30:
				pT += wsprintf((LPSTR)pT, _T("JumpPressed()\r\n"));
				break;
			case 0x33:
				pT += wsprintf((LPSTR)pT, _T("i51(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x34:
				pT += wsprintf((LPSTR)pT, _T("i54(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x36:
				pT += wsprintf((LPSTR)pT, _T("i54(%d)\r\n"), u8(0));
				pM++;
				break;
			case 0x37:
				pT += wsprintf((LPSTR)pT, _T("i55(%d,%dm(%d),%dm(%d))\r\n"), u8(0), val(1), val(6));
				pM += 11;
				break;
			default:
				pT += wsprintf((LPSTR)pT, _T("i%d()\r\n"), ctl2);
			}
			break;
		case 1:
			strcpy(pT, "S- ");
			pT += 3;
			switch (ctl2)
			{
			case 0:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("SetWindowCaption(\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				if (strlen(str) != 0)
				{
					fwrite(str, strlen(str), 1, f);
					fwrite("\r\n", strlen("\r\n"), 1, f);
				}
				break;
			case 1:
				strncpy(str, pM + 5, u32(1));
				str[u32(1)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("JumpSetScriptNo(%d,\"%s\")\r\n"), u8(0), str);
				pM += u32(1) + 5;
				break;
			case 2:
				pT += wsprintf((LPSTR)pT, _T("i2(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 3:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("AddNameInfo(\"%s\","), str);
				pM += u32(0) + 4;
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("\"%s\","), str);
				pM += u32(0) + 4;
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			case 4:
				strncpy(str, pM + 5, u32(1));
				str[u32(1)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("LoadOGV(%d,\"%s\")\r\n"), u8(0), str);
				pM += u32(1) + 5;
				break;
			case 5:
				pT += wsprintf((LPSTR)pT, _T("i5(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			case 6:
				pT += wsprintf((LPSTR)pT, _T("i6(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			case 7:
				pT += wsprintf((LPSTR)pT, _T("i7(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			case 8:
				pT += wsprintf((LPSTR)pT, _T("i8(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10), val(15), val(20), val(25), val(30));
				pM += 35;
				break;
			case 9:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i9(\"%s\","), str);
				pM += u32(0) + 4;
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("\"%s\","), str);
				pM += u32(0) + 4;
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("\"%s\","), str);
				pM += u32(0) + 4;
				pT += wsprintf((LPSTR)pT, _T("%d)\r\n"), u8(0));
				pM++;
				break;
			case 0xa:
				pT += wsprintf((LPSTR)pT, _T("i10(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			case 0xb:
				strncpy(str, pM + 14, u32(10));
				str[u32(10)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i11(%dm(%d),%dm(%d),\"%s\")\r\n"), val(0), val(5), str);
				pM += u32(10) + 14;
				break;
			case 0xC:
				pT += wsprintf((LPSTR)pT, _T("i12(%d,%dm(%d))\r\n"), u8(0), val(1));
				pM += 6;
				break;
			case 0xd:
				strncpy(str, pM + 9, u32(5));
				str[u32(5)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i13(%dm(%d),\"%s\""), val(0), str);
				pM += u32(5) + 9;
				if (strlen(str) != 0)
				{
					fwrite(str, strlen(str), 1, f);
					fwrite("\r\n", strlen("\r\n"), 1, f);
				}
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			case 0xe:
				pT += wsprintf((LPSTR)pT, _T("LoadInMenu(%d,%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), u8(0), val(1), val(6), val(11), val(16));
				pM += 21;
				break;
			case 0xf:
				pT += wsprintf((LPSTR)pT, _T("i15(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x10:
				pT += wsprintf((LPSTR)pT, _T("i16(%d,i15(%dm(%d),%dm(%d),%dm(%d))\r\n"), u8(0), val(1), val(6), val(11));
				pM += 16;
				break;
			case 0x11:
				pT += wsprintf((LPSTR)pT, _T("i17(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x12:
				pT += wsprintf((LPSTR)pT, _T("EnterHistroy(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10), val(15), val(20), val(25), val(30));
				pM += 35;
				break;
			case 0x13:
				pT += wsprintf((LPSTR)pT, _T("i19(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10), val(15), val(20));
				pM += 25;
				break;
			case 0x14:
				pT += wsprintf((LPSTR)pT, _T("HistoryPrev(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0x15:
				strncpy(str, pM + 5, u32(1));
				str[u32(1)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("SysDirSetup(%d,\"%s\")\r\n"), u8(0), str);
				pM += u32(1) + 5;
				break;
			case 0x17:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("AddPackIfDirNotExists(\"%s\","), str);
				pM += u32(0) + 4;
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			case 0x18:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i24(\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			case 0x1a:
				pT += wsprintf((LPSTR)pT, _T("i26(%d,%d)\r\n"), u8(0), u8(1));
				pM += 2;
				break;
			case 0x1b:
				pT += wsprintf((LPSTR)pT, _T("i27(%d,%dm(%d))\r\n"), u8(0), val(1));
				pM += 6;
				break;
			case 0x1c:
				pT += wsprintf((LPSTR)pT, _T("OKPressed(%d,%dm(%d),%dm(%d))\r\n"), u8(0), val(1), val(6));
				pM += 11;
				break;
			case 0x1d:
				pT += wsprintf((LPSTR)pT, _T("i29(%d,%dm(%d),%dm(%d))\r\n"), u8(0), val(1), val(6));
				pM += 11;
				break;
			case 0x1e:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i30(\"%s\","), str);
				pM += u32(0) + 4;
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			default:
				pT += wsprintf((LPSTR)pT, _T("i%d()\r\n"), ctl2);
			}
			break;
		case 2:
			strcpy(pT, "M- ");
			pT += 3;
			switch (ctl2)
			{
			case 0:
				strncpy(str, pM + 9, u32(5));
				str[u32(5)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("SetFileNo(%dm(%d),\"%s\")\r\n"), val(0), str);
				pM += u32(5) + 9;
				break;
			case 1:
				pT += wsprintf((LPSTR)pT, _T("SetXY(%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10));
				pM += 15;
				break;
			case 5:
			case 0xa:
			case 0x10:
			case 0x11:
				pT += wsprintf((LPSTR)pT, _T("i%d(%dm(%d),%dm(%d),%dm(%d))\r\n"), ctl2, val(0), val(5), val(10));
				pM += 15;
				break;
			case 2:
			case 0x17:
				pT += wsprintf((LPSTR)pT, _T("i%d(%dm(%d),%d)\r\n"), ctl2, val(0), u8(5));
				pM += 6;
				break;
			case 3:
				pT += wsprintf((LPSTR)pT, _T("i%d(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), ctl2, val(0), val(5), val(10), val(15), val(20));
				pM += 25;
				break;
			case 4:
			case 6:
			case 7:
			case 0xd:
			case 0xE:
			case 0xf:
			case 0x13:
				pT += wsprintf((LPSTR)pT, _T("i%d(%dm(%d),%dm(%d))\r\n"), ctl2, val(0), val(5));
				pM += 10;
				break;
			case 8:
			case 0xb:
			case 0x15:
				pT += wsprintf((LPSTR)pT, _T("i%d(%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), ctl2, val(0), val(5), val(10), val(15));
				pM += 20;
				break;
			case 9:
				pT += wsprintf((LPSTR)pT, _T("i%d(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), ctl2, val(0), val(5), val(10), val(15), val(20), val(25));
				pM += 30;
				break;
			case 0xc:
				pT += wsprintf((LPSTR)pT, _T("i%d(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%d)\r\n"), ctl2, val(0), val(5), val(10), val(15), u8(20));
				pM += 21;
				break;
			case 0x12:
				strncpy(str, pM + 29, u32(25));
				str[u32(25)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i18(...,\"%s\")\r\n"), str);
				pM += u32(25) + 29;
				break;
			case 0x14:
				strncpy(str, pM + 9, u32(5));
				str[u32(5)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i20(%dm(%d),\"%s\","), val(0), str);
				pM += u32(5) + 9;
				pT += wsprintf((LPSTR)pT, _T("%dm(%d),%dm(%d),%dm(%d),%d)\r\n"), val(0), val(5), val(10), u8(15));
				pM += 16;
				break;
			case 0x16:
				strncpy(str, pM + 9, u32(5));
				str[u32(5)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i22(%dm(%d),\"%s\","), val(0), str);
				pM += u32(5) + 9;
				pT += wsprintf((LPSTR)pT, _T("%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10), val(15));
				pM += 20;
				break;
			default:
				__asm int 3
			}
			break;
		case 3:
			strcpy(pT, "D- ");
			pT += 3;
			switch (ctl2)
			{
			case 0:
				if (u32(5) >= 512)
					__asm int 3
				strncpy(str, pM + 9, u32(5));
				str[u32(5)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("ReadFileNo(%d,\"%s\","), val(0), str);
				pM += u32(5) + 9;
				pT += wsprintf((LPSTR)pT, _T("%dm(%d),%d)\r\n"), val(0), u8(5));
				pM += 6;
				break;
			case 1:
				pT += wsprintf((LPSTR)pT, _T("i1(%dm(%d),%dm(%d),%d)\r\n"), val(0), val(5), u8(10));
				pM += 11;
				break;
			case 2:
				pT += wsprintf((LPSTR)pT, _T("i2(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 3:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("PlayVideo(\"%s\","), str);
				pM += u32(0) + 4;
				pT += wsprintf((LPSTR)pT, _T("%d)\r\n"), u8(0));
				pM++;
				break;
			default:
				__asm int 3
			}
			break;
		case 4:
			strcpy(pT, "4- ");
			pT += 3;
			switch (ctl2)
			{
			case 0:
				pT += wsprintf((LPSTR)pT, _T("i0(%dm(%d),%d,%dm(%d))\r\n"), val(0), u8(5), val(6));
				pM += 11;
				break;
			case 1:
				pT += wsprintf((LPSTR)pT, _T("i1(%dm(%d),%d)\r\n"), val(0), u8(5));
				pM += 6;
				break;
			case 2:
				pT += wsprintf((LPSTR)pT, _T("i2(%dm(%d),%d)\r\n"), val(0), u8(5));
				pM += 6;
				break;
			case 3:
				pT += wsprintf((LPSTR)pT, _T("i3(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			case 4:
				pT += wsprintf((LPSTR)pT, _T("i4(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			default:
				__asm int 3
			}
			break;
		case 5:
			strcpy(pT, "T- ");
			pT += 3;
			switch (ctl2)
			{
			case 0:
				strncpy(str, pM + 9, u32(5));
				str[u32(5)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("ShowText(%dm(%d),\"%s\")\r\n"), val(0), str);
				pM += u32(5) + 9;
				if (strlen(str) != 0)
				{
					fwrite(str, strlen(str), 1, f);
					fwrite("\r\n", strlen("\r\n"), 1, f);
				}
				break;
			case 1:
				pT += wsprintf((LPSTR)pT, _T("i1(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10), val(15), val(20), val(25), val(30), val(35));
				pM += 40;
				break;
			case 2:
				pT += wsprintf((LPSTR)pT, _T("i2(%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10), val(15), val(20), val(25), val(30));
				pM += 35;
				break;
			case 3:
				pT += wsprintf((LPSTR)pT, _T("i3(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			case 4:
				pT += wsprintf((LPSTR)pT, _T("i4(%dm(%d),%dm(%d))\r\n"), val(0), val(5));
				pM += 10;
				break;
			case 5:
				pT += wsprintf((LPSTR)pT, _T("i5(%d)\r\n"), u8(0));
				pM++;
				break;
			case 6:
				pT += wsprintf((LPSTR)pT, _T("i6()\r\n"));
				break;
			case 7:
				pT += wsprintf((LPSTR)pT, _T("i7(%d,%d)\r\n"), u8(0), u8(1));
				pM += 2;
				break;
			default:
				__asm int 3
			}
			break;
		case 6:
			strcpy(pT, "V- ");
			pT += 3;
			switch (ctl2)
			{
			case 0:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i0(\"%s\","), str);
				pM += u32(0) + 4;
				pT += wsprintf((LPSTR)pT, _T("%d,%dm(%d))\r\n"), u8(0), val(1));
				pM += 6;
				break;
			case 1:
				pT += wsprintf((LPSTR)pT, _T("i1(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 2:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("PlayVoice(\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			case 3:
				pT += wsprintf((LPSTR)pT, _T("i3()\r\n"));
				break;
			case 4:
				strncpy(str, pM + 9, u32(5));
				str[u32(5)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("PlaySE(%dm(%d),\"%s\")\r\n"), val(0), str);
				pM += u32(5) + 9;
				break;
			case 5:
				pT += wsprintf((LPSTR)pT, _T("i5(%dm(%d),%d)\r\n"), val(0), u8(5));
				pM += 6;
				break;
			case 6:
				pT += wsprintf((LPSTR)pT, _T("i6(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 7:
				pT += wsprintf((LPSTR)pT, _T("ReplayVoice()\r\n"));
				break;
			case 8:
				pT += wsprintf((LPSTR)pT, _T("ReplayHistoryVoice(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 9:
				pT += wsprintf((LPSTR)pT, _T("i9(%dm(%d))\r\n"), val(0));
				pM += 5;
				break;
			case 0xa:
				strncpy(str, pM + 4, u32(0));
				str[u32(0)] = '\0';
				pT += wsprintf((LPSTR)pT, _T("i10(\"%s\")\r\n"), str);
				pM += u32(0) + 4;
				break;
			case 0xb:
				pT += wsprintf((LPSTR)pT, _T("i11(%d,%dm(%d),%dm(%d))\r\n"), u8(0), val(1), val(6));
				pM += 11;
				break;
			case 0xc:
				pT += wsprintf((LPSTR)pT, _T("i12(%dm(%d),%dm(%d),%dm(%d),%dm(%d))\r\n"), val(0), val(5), val(10), val(15));
				pM += 20;
				break;
			case 0xD:
				pT += wsprintf((LPSTR)pT, _T("i13()\r\n"));
				break;
			default:
				__asm int 3;
			}
			break;
		default:
			pT += wsprintf((LPSTR)pT, _T("Unk VMCode:0x%x\r\n"), cc1tl1);
			break;
			//__asm int 3
		}
	}
	fclose(f);
	return (pT - lpTxt);
}

void Exec(char* FileName)
{
	FILE* fp = fopen(FileName, "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	DWORD Wsize = (size * 20 > 100000) ? size * 20 : 100000;
	char* Wbuff = (char*)malloc(Wsize);
	Wsize = Msc2Txt(buff, size, Wbuff, Wsize);
	cout << hex << Wsize << endl;
	//strcat(FileName, ".txt");
	fp = fopen("disasm.txt", "wb");
	fwrite(Wbuff, Wsize, 1, fp);
	fclose(fp);
	free(buff);
	free(Wbuff);
	/*
	for (int i = 0; i < size; i++)
		buff[i] ^= 0x77;
	int v26 = *(DWORD*)(buff + 2);
	unsigned int v13 = *(DWORD*)(buff + 6);
	int v14 = size - v26;
	unsigned int v27 = *(DWORD*)(buff + v13 + 0xA);
	signed int VMSize = v13 / 9;//这个可能为0或者更小
	int v26 = size - v26;
	int v28 = *(DWORD*)(buff + 6);
	DWORD v16 = *(DWORD*)(buff + 0xF);
	while (1) 
	{
		unsigned int index = v16 - 1;
		if (index < 0x1000)
			break;
		unsigned int unk = *(DWORD*)index;
		index += 0x9;
		int i = 0;
		i++;
		if (i >= VMSize)
			goto pr;
	}
pr:
	char* pos = buff + v13 + 0x13;
	strcat(FileName, ".dec");
	fp = fopen(FileName, "wb");
	fwrite(buff, size, 1, fp);
	cout << hex << size << endl;
	fclose(fp);
	free(buff);
	*/
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		//cout << "Usage:" << argv[0] << " File" << endl;
		MessageBox(0, "exCD.exe <input cd file>", "AyamiKaze", 0);
		//return -1;
	}
	char* FILENAME = argv[1];
	//char* FILENAME = (char*)"ma01_001.msc";
	//Cryption(FILENAME);
	Exec(FILENAME);
	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
