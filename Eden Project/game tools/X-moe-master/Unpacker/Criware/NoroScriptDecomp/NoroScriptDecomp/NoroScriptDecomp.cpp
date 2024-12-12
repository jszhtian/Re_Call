#include <Windows.h>
#include <memory>

static BYTE Info[0xFEE] = { 0 };
static BYTE byte_6975F28[0xFEE] = { 0 };
PVOID pInfo = &Info[0];
PVOID pMemset = nullptr;

__declspec(naked) ULONG CDECL Decompress(PVOID OutBuffer, PVOID InBuffer)
{
	__asm
	{
		push ebp
		mov ebp, esp
		sub esp, 0xC
		push ebx
		push esi
		push edi
		mov edi, ecx
		mov ebx, dword ptr[edi]
		push 0xFEE
		mov esi, eax
		push 0
		add ebx, esi
		push Info
		add edi, 4
		mov dword ptr[ebp - 8], ebx
		call pMemset
		add esp, 0xC
		mov eax, 0xFEE
		xor edx, edx
		jmp loc_445F80

loc_445F74 :
		mov edx, dword ptr[ebp - 4]
		jmp loc_445F80
		lea esp, [esp]

loc_445F80:
		shr edx, 1
		mov dword ptr[ebp - 4], edx
		test edx, 0x00000100
		jnz loc_445F9A

		movzx edx, byte ptr[edi]
		inc edi
		or edx, 0x0000FF00
		mov dword ptr[ebp - 4], edx

loc_445F9A:
		movzx ecx, byte ptr[edi]
		test dl, 01
		jz loc_445FBD

		mov byte ptr[eax + Info], cl
		inc eax
		mov byte ptr[esi], cl
		inc esi
		inc edi
		and eax, 0x00000FFF
		cmp esi, ebx
		jb loc_445F80

loc_445FB6 :
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		retn

loc_445FBD :
		movzx edx, byte ptr[edi + 1]
		mov ebx, edx
		and ebx, 0x000000F0
		shl ebx, 4
		or ecx, ebx
		and edx, 0x0000000F
		lea ebx, [ecx + edx + 2]
		add edi, 2
		mov dword ptr[ebp - 0xC], ebx
		cmp ecx, ebx
		mov ebx, dword ptr[ebp - 8]
		mov edx, ecx
		jg loc_445F74
		jmp loc_445FF0

		lea esp, [esp]
		lea ecx, [ecx]

loc_445FF0:
		mov ecx, edx
		and ecx, 0x00000FFF
		movzx ecx, byte ptr[ecx + Info]
		mov byte ptr[eax + Info], cl
		inc eax
		mov byte ptr[esi], cl
		inc esi
		and eax, 0x00000FFF
		cmp esi, ebx
		jae loc_445FB6

		inc edx
		cmp edx, dword ptr[ebp - 0xC]
		jle loc_445FF0
		jmp loc_445F74
	}
}

typedef          __int64 ll;
typedef unsigned __int64 ull;

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;
typedef ll              int64;
typedef ll              sint64;
typedef ull             uint64;

template<class T> int8 __SETS__(T x)
{
	if (sizeof(T) == 1)
		return int8(x) < 0;
	if (sizeof(T) == 2)
		return int16(x) < 0;
	if (sizeof(T) == 4)
		return int32(x) < 0;
	return int64(x) < 0;
}

template<class T, class U> int8 __OFSUB__(T x, U y)
{
	if (sizeof(T) < sizeof(U))
	{
		U x2 = x;
		int8 sx = __SETS__(x2);
		return (sx ^ __SETS__(y)) & (sx ^ __SETS__(x2 - y));
	}
	else
	{
		T y2 = y;
		int8 sx = __SETS__(x);
		return (sx ^ __SETS__(y2)) & (sx ^ __SETS__(x - y2));
	}
}

int sub_445F40(unsigned int a1, int a2)
{
	unsigned int v2; // esi@1
	int v3; // ebx@1
	int v4; // edi@1
	int result; // eax@1
	int i; // edx@1
	int v7; // edx@4
	int v8; // ecx@5
	int v9; // ecx@8
	int v10; // ebx@8
	bool v11; // zf@8
	bool v12; // sf@8
	unsigned __int8 v13; // of@8
	int v14; // edx@8
	char v15; // cl@9
	int v16; // [sp+Ch] [bp-Ch]@8
	int v17; // [sp+10h] [bp-8h]@1
	int v18; // [sp+14h] [bp-4h]@3

	v2 = a1;
	v3 = a1 + *(DWORD *)a2;
	v4 = a2 + 4;
	v17 = a1 + *(DWORD *)a2;
	memset(byte_6975F28, 0, 0xFEEu);
	result = 4078;
	for (i = 0;; i = v18)
	{
		while (1)
		{
			i = (unsigned int)i >> 1;
			v18 = i;
			if (!(i & 0x100))
			{
				v7 = *(BYTE *)v4++;
				i = v7 | 0xFF00;
				v18 = i;
			}
			v8 = *(BYTE *)v4;
			if (!(i & 1))
				break;
			byte_6975F28[result] = v8;
			*(BYTE *)v2++ = v8;
			++v4;
			result = ((WORD)result + 1) & 0xFFF;
			if (v2 >= v3)
				return result;
		}
		v9 = 16 * (unsigned __int8)(*(BYTE *)(v4 + 1) & 0xF0) | v8;
		v10 = (*(BYTE *)(v4 + 1) & 0xF) + v9 + 2;
		v4 += 2;
		v16 = v10;
		v13 = __OFSUB__(v9, v10);
		v11 = v9 == v10;
		v12 = v9 - v10 < 0;
		v3 = v17;
		v14 = v9;
		if ((unsigned __int8)(v12 ^ v13) | v11)
			break;
	LABEL_2:
		;
	}
	while (1)
	{
		v15 = byte_6975F28[v14 & 0xFFF];
		byte_6975F28[result] = v15;
		*(BYTE *)v2++ = v15;
		result = ((WORD)result + 1) & 0xFFF;
		if (v2 >= v17)
			return result;
		++v14;
		if (v14 > v16)
			goto LABEL_2;
	}
}

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return 0;

	pMemset = memset;

	FILE* fin = nullptr, *fout = nullptr;
	fin = _wfopen(argv[1], L"rb");
	fseek(fin, 0, SEEK_END);
	unsigned InSize = ftell(fin);
	rewind(fin);
	PBYTE InBuffer = new BYTE[InSize];
	fread(InBuffer, 1, InSize, fin);
	fclose(fin);

	WCHAR lpFileName[MAX_PATH] = { 0 };
	wsprintfW(lpFileName, L"%s.out", argv[1]);

	unsigned OutSize = *(PULONG)InBuffer;
	PBYTE OutBuffer = new BYTE[OutSize];

	/*
	__try{
		Decompress(OutBuffer, InBuffer);
		
	}
	__except (1)
	{
		printf("%08x\n", OutBuffer);
	}
	*/

	sub_445F40((unsigned int)OutBuffer, (int)InBuffer);

	fout = _wfopen(lpFileName, L"wb");
	fwrite(OutBuffer, 1, OutSize, fout);
	fclose(fout);

	delete[] OutBuffer;
	delete[] InBuffer;
	return 0;
}

