// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include <filesystem>
using namespace std::filesystem;


void DumpAllFiles(const path& src_dir, const path& dst_dir)
{
	copy_options op = copy_options::overwrite_existing | copy_options::recursive;
	copy(src_dir, dst_dir, op);
}

void WINAPI Init2()
{
	TCHAR file_name[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), file_name, ARRAYSIZE(file_name));
	path src = path(file_name).parent_path();
    cout << file_name << endl;
	//path dst = "C:/Users/AyamiKaze/Desktop/dump";
	path dst = "D:/A_Workspace/Eden-Project/dump";
	DumpAllFiles(src, dst);
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
		Init2();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
__declspec(dllexport)void WINAPI Dummy()
{
}
