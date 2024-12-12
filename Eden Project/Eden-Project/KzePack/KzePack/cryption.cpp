#include "filesystem.h"

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

void cryption(BYTE* buff, DWORD size, BOOL dec)
{
	if (!dec)
	{
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0x01010101;
			buff[i] += 0x12;
		}
		Func1(buff, size);
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= EncryptionControlBlock[i % 4096];
			buff[i] -= EncryptionControlBlock[(i+2) % 4096];
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			buff[i] += EncryptionControlBlock[(i + 2) % 4096];
			buff[i] ^= EncryptionControlBlock[i % 4096];
		}
		Func1(buff, size);
		for (int i = 0; i < size; i++)
		{
			buff[i] += 0x12;
			buff[i] ^= 0x01010101;
		}
	}
}