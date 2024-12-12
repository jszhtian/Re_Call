// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include "resource.h"

#include "../3rd/Base64.h"
#include "../3rd/xxhash.h"
#include "../3rd/xxhash.cpp"
using namespace std;

void HelloWorld()
{
    cout << "HelloWorld" << endl;
}

#pragma code_seg(".JunckCode")
#pragma optimize( "g", off )
void JunkCode1(PVOID Call1)
{
    _asm {
        mov eax, ecx;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        pushad;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        popad;
        sub eax, 0x11;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        pushad;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        popad;
        _emit 0x90;
        add eax, 0x14;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        pushad;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        popad;
        mov eax, ecx;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        pushad;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        _emit 0x90;
        _emit 0x90;
        popad;
        push ecx;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        _emit 0x90;
        jmp fin;
        _emit 0x90;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
fin:
        pop eax;
        pop ecx;
        push ecx;
        push ebx;
        push eax;
        pop eax;
        pop ebx;
        pop ecx;
        _emit 0x90;
        push esi;
        xor esi, esi;
        test esi, esi;
        pushad;
        mov eax, ebx;
        mov ebx, eax;
        mov ebx, ecx;
        mov edx, ebx;
        mov ecx, eax;
        jnz junk_code;
        jz run;
    junk_code:
        pop esi;
        pushad;
        pushfd;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        mov eax, ebx;
        mov ebx, ecx;
        mov ecx, edx;
        mov edx, eax;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        mov eax, ebx;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        mov ebx, ecx;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        mov ecx, edx;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        mov edx, eax;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;

        popfd;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        popad;
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
        _emit 0x45;

    run:
        popad;
        pop esi;
        push ebx;
        xor ebx, ebx;
        test ebx, ebx;
        jnz junk_code2;
        jz run2;
    junk_code2:
        _emit 0x11;
        _emit 0x45;
        _emit 0x14;
        _emit 0x19;
        _emit 0x19;
        _emit 0x81;
        _emit 0x00;
    run2:
        pop ebx;
        call fin2;
        _emit 0x07;
    fin2:
        call Call1;
        add dword ptr ss : [esp] , 8;
        ret;

    }
}
#pragma optimize( "g", on )
#pragma code_seg()
#pragma comment(linker, "/SECTION:.JunckCode,ERW")

void Executecpuid(DWORD veax, DWORD* Ret)
{
    DWORD deax;
    DWORD debx;
    DWORD decx;
    DWORD dedx;

    __asm
    {
        mov eax, veax;
        cpuid;
        mov deax, eax;
        mov debx, ebx
            mov decx, ecx
            mov dedx, edx
    }

    Ret[0] = deax;
    Ret[1] = debx;
    Ret[2] = decx;
    Ret[3] = dedx;
}

std::string GetBrand()
{
    const DWORD BRANDID = 0x80000002;
    char cBrand[52];
    memset(cBrand, 0, 52);

    DWORD Ret[4] = { 0 };

    for (DWORD i = 0; i < 3; i++)
    {
        Executecpuid(BRANDID + i, Ret);
        memcpy(cBrand + i * 16, &Ret, 16);
    }
    return std::string(cBrand);
}

std::string GetUserNameSelf()
{
    CHAR Name[MAX_PATH] = { 0 };
    DWORD cbSize = sizeof(Name);
    GetUserNameA(Name, &cbSize);

    return std::string(Name, cbSize);
}

#define DLL_EXPORT __declspec( dllexport )

std::string getSysInfo()
{
    DWORD key = 0x00134967;
    auto getHwid = GetBrand();
    auto getUN = GetUserNameSelf();
    unsigned long long HWID = XXH64(getHwid.c_str(), getHwid.length(), 0x114514);
    unsigned long long UN = XXH64(getUN.c_str(), getUN.length(), 0x1919810);
    HWID ^= UN;

    string bs64 = base64_encode((unsigned char*)&HWID, sizeof(unsigned long long));
    cout << bs64 << endl;
    system("pause");
    return bs64;
}


std::string getTimeInfo()
{
    SYSTEMTIME STM;
    GetLocalTime(&STM);
    unsigned long long TimeHash = XXH64(&STM, sizeof(SYSTEMTIME), 0x11456783);
    unsigned long long TimeHash2 = XXH64(&TimeHash, sizeof(unsigned long long), 0x88754467);

    string bs64 = base64_encode((unsigned char*)&TimeHash2, sizeof(unsigned long long));
    cout << bs64 << endl;
    system("pause");
    return bs64;
}

#pragma code_seg(".Key")

void Decrypt()
{
    // 加密读取本地的CPU和用户信息+key文件里的时间戳
    auto bsTime = getTimeInfo();
    // 加密
    FILE* fp = fopen("AQUA.enc.exe", "rb");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* buff = new BYTE[size];
    fread(buff, size, 1, fp);
    fclose(fp);

    // key
    fp = fopen("Key.otm", "rb");
    if (!fp)
    {
        MessageBox(0, L"缺少文件", L"OTOMEDrm", MB_ICONERROR | MB_OK);
        return;

    }
    fseek(fp, 0, SEEK_END);
    DWORD Ksize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* Kbuff = new BYTE[Ksize];
    fread(Kbuff, Ksize, 1, fp);
    fclose(fp);

    auto s_hwid = getSysInfo();
    auto bsHwid = base64_decode(s_hwid);
    for (int i = 0; i < size; i++)
        buff[i] ^= bsHwid.c_str()[i % bsHwid.length()];

    string s_timebase = string((char*)Kbuff, Ksize);
    string bsDTime = base64_decode(s_timebase);
    for (int i = 0; i < size; i++)
        buff[i] ^= bsDTime.c_str()[i % bsDTime.length()];
    fp = fopen("aqua.dec.exe", "wb");
    fwrite(buff, size, 1, fp);
    fclose(fp);

    fp = fopen("Key.otm", "wb");
    // Enc
    fwrite(bsTime.c_str(), bsTime.length(), 1, fp);
    fclose(fp);
}

void Encrypt()
{
    FILE* fp = fopen("AQUA.exe", "rb");
    fseek(fp, 0, SEEK_END);
    DWORD size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* buff = new BYTE[size];
    fread(buff, size, 1, fp);
    fclose(fp);

    auto bsTime = getTimeInfo();
    auto s_hwid = getSysInfo();

    fp = fopen("Key.otm", "wb");
    fwrite(bsTime.c_str(), bsTime.length(), 1, fp);
    fclose(fp);

    auto bsDTime = base64_decode(bsTime);
    for (int i = 0; i < size; i++)
        buff[i] ^= bsDTime.c_str()[i % bsDTime.length()];

    auto bsHwid = base64_decode(s_hwid);
    for (int i = 0; i < size; i++)
        buff[i] ^= bsHwid.c_str()[i % bsHwid.length()];

    fp = fopen("AQUA.enc.exe", "wb");
    fwrite(buff, size, 1, fp);
    fclose(fp);
}
#pragma code_seg()
#pragma comment(linker, "/SECTION:.Key,ERW")


typedef void (*JCode)(PVOID);
int main()
{
    //void* a1 = (char*)JunkCode1;
    //FILE* fp = fopen("JunkCode.bin", "wb");
    //fwrite(JunkCode1, 0x2B7, 1, fp);
    //fclose(fp);

    FILE* JC = fopen("Atom", "rb");
    fseek(JC, 0, SEEK_END);
    DWORD JCSize = ftell(JC);
    fseek(JC, 0, SEEK_SET);
    //BYTE* JCBuff = new BYTE[JCSize];
    void* JCBuff = VirtualAlloc(0, JCSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    fread(JCBuff, JCSize, 1, JC);
    fclose(JC);
    JCode JunkCodeFunc = (JCode)JCBuff;
    JunkCodeFunc((PVOID)&HelloWorld);
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
