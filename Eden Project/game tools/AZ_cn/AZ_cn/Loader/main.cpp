#include <windows.h>

BOOL CreateAndInject(TCHAR* appName, TCHAR* dllName);

#pragma comment(linker, "/entry:main3")

int main2()
{
    wchar_t exeName[]=L"ch.exe";
    if(!CreateAndInject(exeName,L"AZ_cn.dll"))
    {
        MessageBox(0,L"�޷�������ע�����",0,0);
        return 0;
    }

    return 0;
}

void main3()
{
    ExitProcess(main2());
}