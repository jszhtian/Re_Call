// ReadFileTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <tchar.h>
#include <iomanip>
#include <mutex>
#include <stdio.h>
#include <map>
#include <string>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <io.h>
#include "zlib.h"
#pragma comment(lib, "zlib.lib")
using namespace std;
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

#pragma pack(1)
//AymScriptVer1.00
struct TextStruct
{
	__int32 crc32val;
	wchar_t str[126];
};
typedef struct head
{
	char flag[17];
	DWORD Length;
	DWORD Zero;
	DWORD DEC_SIZE;
	DWORD COMP_SIZE;
}head_t;
static char* HeaderSig = (char*)"FileSystemVer1.00";
int main()
{
	FILE* fp = fopen("Exec.aym", "rb");
	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	fread(buff, size, 1, fp);
	fclose(fp);
	head_t header;
	memcpy(header.flag, "AymScriptVer1.00", 17);
	if (strcmp(header.flag, "AymScriptVer1.00") != 0)
	{
		MessageBox(0, L"Can't read Exec.aym [0x00000001]", L"Error", 0);
		return 0;
	}
	buff += 17;
	header.Length = *(DWORD*)buff;
	buff += 4;
	header.Zero = *(DWORD*)buff;
	buff += 4;
	header.DEC_SIZE = *(DWORD*)buff;
	buff += 4;
	header.COMP_SIZE = *(DWORD*)buff;
	buff += 4;
	cout << hex << header.Length << endl;
	cout << hex << header.DEC_SIZE << endl;
	cout << hex << header.COMP_SIZE << endl;
	DWORD key = 0x114514;
	unsigned char Key[4] = { 0x26,0x8B,0x7F,0x9C };
	DWORD uncomp_size = header.DEC_SIZE;
	char* uncomp_buff = (char*)malloc(header.DEC_SIZE);
	cryption((BYTE*)buff, header.COMP_SIZE, 0);
	if (uncompress((BYTE*)uncomp_buff, &uncomp_size, (BYTE*)buff, header.COMP_SIZE) == Z_OK)
	{
		if (uncomp_size != header.DEC_SIZE)
		{
			cout << "uncompress failed.Broken File." << endl;
			goto exit;
		}
		cryption((BYTE*)uncomp_buff, uncomp_size, 1);

		for (int i = 0; i < uncomp_size; i++)
		{
			key += i;
			uncomp_buff[i] -= key;
			uncomp_buff[i] ^= Key[i % 4];
		}
		fp = fopen("dec.bin", "wb");
		fwrite(uncomp_buff, uncomp_size, 1, fp);
		fclose(fp);
	}
	else
		cout << "uncompress failed.Can't uncompress." << endl;
exit:
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
