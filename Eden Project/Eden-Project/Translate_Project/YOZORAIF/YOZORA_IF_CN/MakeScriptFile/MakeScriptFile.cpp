// MakeScriptFile.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>
#include "zlib.h"
#pragma comment(lib, "zlib.lib")
using namespace std;

typedef struct AymHeader {
    char Magic[10]="AyamiKaze";
    DWORD Size;
};

void Func1(BYTE* src, DWORD len)
{
	__asm {
		mov		eax, src;
		mov		ebx, len;
	Tag:
		mov     dx, word ptr[eax];
		movzx   esi, dx;
		and dx, 0x5555;
		and esi, 0xAAAAAAAA;
		shr     esi, 1;
		add     edx, edx;
		or si, dx;
		mov     edx, esi;
		mov     word ptr[eax], dx;
		inc     ecx;
		add     eax, 0x2;
		cmp     ebx, ecx;
		ja      Tag;
	End:
	}
}

BYTE rol(int val, int n)
{
	n %= 8;
	return (val << n) | (val >> (8 - n));
}

BYTE ror(int val, int n)
{
	n %= 8;
	return (val >> n) | (val << (8 - n));
}

void cryption(BYTE* buff, DWORD size, int dec)
{
	if (dec == 0)
	{
		DWORD count = (size - 0x10) / 4;
		BYTE* p = buff + 0x10;

		DWORD key1 = 114514;
		DWORD key2 = 0x3AD023;
		DWORD c = 0x114;

		for (DWORD i = 0; i < count; i++)
		{
			DWORD* dp = (DWORD*)p;
			*dp ^= key2;
			*dp -= key1 ^ key2;
			*dp ^= key1;
			p = (BYTE*)dp;
			*p = ror(*p, c++);
			c &= 0xFF;
			p += 4;
		}

		Func1(buff, size);
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0x01010101;
			buff[i] += 0x12;
		}
		Func1(buff, size);
	}
	else if (dec == 1)
	{
		DWORD count = (size - 0x10) / 4;
		BYTE* p = buff + 0x10;

		DWORD key1 = 114514;
		DWORD key2 = 0x3AD023;
		DWORD c = 0x114;
		Func1(buff, size);
		for (int i = 0; i < size; i++)
		{
			buff[i] -= 0x12;
			buff[i] ^= 0x01010101;
		}
		Func1(buff, size);
		for (DWORD i = 0; i < count; i++)
		{
			*p = rol(*p, c++);
			//*p = CPP_ROL(*p, c++);
			c &= 0xFF;
			DWORD* dp = (DWORD*)p;
			*dp ^= key1;
			*dp += key1 ^ key2;
			*dp ^= key2;
			p = (BYTE*)dp + 4;

		}
	}
}


int main()
{
    FILE* fp = fopen("Script.txt", "rb");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
	BYTE* buff = (BYTE*)malloc(size);
    if (buff)
    {
        fread(buff, size, 1, fp);
        fclose(fp);
		cryption(buff, size, 0);
		DWORD comp_size;
		BYTE* comp_buff = (BYTE*)malloc(size);
		if (compress2(comp_buff, &comp_size, buff, size, 9) == Z_OK)
		{
			fp = fopen("Script.cpk", "wb");
			cryption(comp_buff, comp_size, 1);
			fwrite(comp_buff, comp_size, 1, fp);
			fclose(fp);
		}
    }
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
