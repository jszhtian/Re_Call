#include <Windows.h>

#define _TAG4(s) ( \
                (((s) >> 24) & 0xFF)       | \
                (((s) >> 8 ) & 0xFF00)     | \
                (((s) << 24) & 0xFF000000) | \
                (((s) << 8 ) & 0x00FF0000) \
                )
#define TAG4(s) _TAG4((DWORD)(s))

#define TAG8(left, right) (((UINT64)TAG4(right) << 32) | TAG4(left))


typedef struct SHELL_DATA32
{
	//CrystallizerStat
	ULONG64 Maigc1;
	ULONG64 Magic2;
	DWORD   OriOEP;
}SHELL_DATA32, *PSHELL_DATA32;
