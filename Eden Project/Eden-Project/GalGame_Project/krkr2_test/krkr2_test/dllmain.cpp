// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
//ofstream flog("SCR.txt");

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
    _In_      UINT   CodePage,
    _In_      DWORD  dwFlags,
    _In_      LPCSTR lpMultiByteStr,
    _In_      int    cbMultiByte,
    _Out_opt_ LPWSTR lpWideCharStr,
    _In_      int    cchWideChar);

int WINAPI HookMultiByteToWideChar(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
)
{
	int ret = MultiByteToWideChar(CodePage, dwFlags,
        lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
	switch (CodePage)
	{
	case CP_ACP:
        if (lpWideCharStr && (USHORT)*lpWideCharStr > 0x20)//c7aed6ff
        {
            CRC32 crc;
            DWORD strcrc = crc.Calc((char*)lpWideCharStr, ret);
            /*
            flog << "################## CRC:0x" << hex << strcrc << " ##################" << endl;
            flog << wtoc(lpWideCharStr,936) << endl;
            flog << "################## CRC:0x" << hex << strcrc << " ##################\r\n" << endl;
            */
            if (strcrc == 0xc7aed6ff)
            {
                FILE* fp = fopen("_Init.tjs", "rb");
                fseek(fp, 0, SEEK_END);
                DWORD size = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                char* buff = (char*)malloc(size);
                if (buff)
                {
                    fread(buff, size, 1, fp);
                    fclose(fp);
                    WCHAR* cpy_str = ctow(buff, CP_UTF8);
                    int cpy_len = wcslen(cpy_str);
                    wcscpy(lpWideCharStr, cpy_str);
                    cout << cpy_len << endl;
                    cout << ret << endl;
                    ret = cpy_len;
                }
            }
        }
	default:
        return ret;
	}
}

void Init()
{
    if (!IATPatch("Kernel32.dll", GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "MultiByteToWideChar"), (PROC)HookMultiByteToWideChar))
    {
        MessageBoxW(0, L"MultiByteToWideChar Hook Error。", L"IATHook Error", 0);
        return;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        make_console();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void) {
    return;
}