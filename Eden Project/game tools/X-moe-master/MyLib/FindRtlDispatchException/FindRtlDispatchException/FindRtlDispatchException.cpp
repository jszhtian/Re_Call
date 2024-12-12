#include <stdio.h>
#include <Windows.h>

ULONG FindCallRtlDispatchExcepation_NoPass()
{
	PBYTE pAddr = (BYTE *)::GetProcAddress(::GetModuleHandleA("ntdll"), "KiUserExceptionDispatcher");
	if (pAddr)
	{
		while (*pAddr != 0xE8)pAddr++;
		return (ULONG)((*(DWORD *)(pAddr + 1)) + 5 + (DWORD)pAddr);
	}
	return NULL;
}




int wmain(int argc, WCHAR* argv[])
{
	printf("%08x\n", FindCallRtlDispatchExcepation_NoPass());
	getchar();
	return 0;
}

