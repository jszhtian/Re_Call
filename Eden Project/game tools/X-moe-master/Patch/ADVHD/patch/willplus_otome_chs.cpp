

typedef INT_PTR(WINAPI* PfunDialogBoxParamW)(
	__in_opt HINSTANCE hInstance,
	__in     LPCWSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in     LPARAM dwInitParam);

PVOID pfOldDialogBoxParamW = NULL;

INT_PTR WINAPI HookDialogBoxParamW(
	__in_opt HINSTANCE hInstance,
	__in     LPCWSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in     LPARAM dwInitParam)
{
	if ((hInstance == GetModuleHandleW(NULL)) && ((ULONG_PTR)lpTemplateName == 0x66))
	{
		return ((PfunDialogBoxParamW)pfOldDialogBoxParamW)(GetSelfModule(),
			MAKEINTRESOURCEW(102), hWndParent, lpDialogFunc, dwInitParam);
	}
	else
	{
		return ((PfunDialogBoxParamW)pfOldDialogBoxParamW)(hInstance,
			lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	}
}



int WINAPI HookMultiByteToWideCharDll(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
	)
{

	auto Selector = [](UINT iCodePage)->UINT
	{ 
		switch (iCodePage)
		{
		case CP_ACP:
		case CP_OEMCP:
		case CP_THREAD_ACP:
			return 936;

		default:
			return iCodePage;
		}
	};

	return MultiByteToWideChar(
		Selector(CodePage),
		dwFlags, lpMultiByteStr, cbMultiByte,
		lpWideCharStr, cchWideChar);
}


//IAT
HMODULE WINAPI HookLoadLibraryW(
	_In_ LPCWSTR lpFileName
	)
{
	HMODULE hDll = LoadLibraryW(lpFileName);

	if (GetModuleHandleW(L"MSVCR90.DLL"))
	{
		PROC pfStubMultiByteToWideChar = (PROC)GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "MultiByteToWideChar");
		IATPatchDll(hDll, "Kernel32.dll", pfStubMultiByteToWideChar, (PROC)HookMultiByteToWideCharDll);
		return hDll;
	}
	return hDll;
}

/*
CPU Disasm
Address                       Hex dump                      Command                          Comments
004CF69D                      |.  897C24 14                 mov dword ptr [esp+14],edi
004CF6A1                      |.  894424 18                 mov dword ptr [esp+18],eax       ; UNICODE "SYS01.ogg"
004CF6A5                      |.  894C24 1C                 mov dword ptr [esp+1C],ecx
004CF6A9                      |.  391D F0AA5500             cmp dword ptr [55AAF0],ebx
*/
//Fix FileName

//__declspec(noreturn)
ULONG_PTR WINAPI FixFileNameInternal(PWCHAR lpStr, PWCHAR lpArcName)
{
	ULONG_PTR Result;
	ULONG     OriSize;
	CHAR      MBString[260];
	DWORD     OldProtect;

	__asm mov Result, eax;

	OriSize = lstrlenW(lpStr);

	RtlZeroMemory(MBString, sizeof(MBString));
	((PfunWideCharToMultiByte)pfWideCharToMultiByte)(936, 0, lpStr, lstrlenW(lpStr), 
		MBString, sizeof(MBString), 0, 0);

	VirtualProtect(lpStr, OriSize + 2, PAGE_EXECUTE_READWRITE, &OldProtect);
	((PfunMultiByteToWideChar)MultiByteToWideChar)(932, 0, MBString, lstrlenA(MBString), 
		lpStr, OriSize + 2);
	VirtualProtect(lpStr, OriSize + 2, OldProtect, &OldProtect);

	OutputStringW(L"Arc File Name :");
	OutputStringW(lpArcName);
	OutputStringW(L"\n");

	OutputStringW(lpStr);
	OutputStringW(L"\n");
	
	__asm mov eax, Result;

	return Result;
}

ULONG_PTR lpStart = 0x004CF69D;
ULONG_PTR lpEnd   = 0x004CF6A5;



//ebp is a pointer to arc file name
__declspec(naked)VOID FixFileName()
{
	__asm
	{
		mov dword ptr[esp + 14h], edi
		mov dword ptr[esp + 18h], eax
		pushad
		push ebp
		push eax
		call FixFileNameInternal
		popad
		jmp lpEnd
	}
}