// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>
#include <filesystem>
#include <thread>
using namespace std::filesystem;
using namespace std::chrono;

void DumpAllFiles(const path& src_dir, const path& dst_dir)
{
	copy_options op = copy_options::overwrite_existing | copy_options::recursive;
	copy(src_dir, dst_dir, op);
}

void WINAPI Init2()
{
    /*
    std::this_thread::sleep_for(seconds(1));
	TCHAR file_name[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), file_name, ARRAYSIZE(file_name));
	path src = path(file_name).parent_path();
	cout << file_name << endl;
	//path dst = "C:/Users/AyamiKaze/Desktop/dump";
	path dst = "D:/A_Workspace/Eden-Project/dump";
	DumpAllFiles(src, dst);
    */
    FILE* fp = fopen("dmm.dll", "rb");
    if (fp) 
    {
        fseek(fp, 0, SEEK_END);
        DWORD size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* buff = (char*)malloc(size);
        fread(buff, size, 1, fp);
        fclose(fp);

        fp = fopen("dmm.dmp.dll", "wb");
        fwrite(buff, size, 1, fp);
        fclose(fp);
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
    {
        /*
        make_console();
        std::thread t(Init2);
        t.detach();
        break;
        */
    }
    case DLL_THREAD_ATTACH:
		Init2();
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

