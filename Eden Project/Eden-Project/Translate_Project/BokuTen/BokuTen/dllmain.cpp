// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "detours.h"
#include "crc32.h"
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);

vector<wstring>rawlist;
vector<wstring>newlist;

map<DWORD, wstring> SCRList;
map<DWORD, wstring> REPList;

ofstream LOG("LOG.txt");
static void make_console() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    std::cout << "Open Console Success!" << std::endl;
}
char* wtoc(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE);
    char* bbb = new char[dwMinSize];
    WideCharToMultiByte(CP_ACP, NULL, str, -1, bbb, dwMinSize, NULL, FALSE);
    return bbb;
}
char* wtocUTF(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

char* wtocGBK(LPCTSTR str)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

LPWSTR ctowUTF(char* str)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
    return out;
}

LPWSTR ctowJIS(char* str)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
    LPWSTR out = new wchar_t[dwMinSize];
    MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
    return out;
}
/*
00466907  |.  E8 E4720200   |call RealLive.0048DBF0
0046690C  |>  83C4 04       |add esp,0x4
0046690F  |.  89AF 5C220000 |mov dword ptr ds:[edi+0x225C],ebp
00466915  |>  8B4B 1C       |mov ecx,dword ptr ds:[ebx+0x1C]         ;  VM Struct Here
00466918  |.  8D6B 1C       |lea ebp,dword ptr ds:[ebx+0x1C]
0046691B  |.  83F9 FF       |cmp ecx,-0x1
0046691E  |.  0F84 19050000 |je RealLive.00466E3D
00466924  |.  8A01          |mov al,byte ptr ds:[ecx]
00466926  |.  84C0          |test al,al
00466928  |.  0F84 1C050000 |je RealLive.00466E4A
0046692E  |.  8B97 F4160000 |mov edx,dword ptr ds:[edi+0x16F4]
00466934  |.  85D2          |test edx,edx                            ;  RealLive.005FE4B0
*/

/*
char* NewVMCode = (char*)malloc(TestTextLen + 0x20);
//Build Code
memcpy(NewVMCode, TestText, TestTextLen);
memcpy(NewVMCode + TestTextLen, code, 0x20);
//Replace
memcpy(Text, NewVMCode, TestTextLen + 0x20);
cout << "copy fin" << endl;
*/

char* RetText;
char OldVMCode[MAX_PATH];
char* OldAddr;

typedef struct MyMemInfo {
    char info[MAX_PATH];
    DWORD size;
}MyMemInfo_t;
MyMemInfo_t MMI;

bool IsJis(char* text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        if ((unsigned int)text[i] < 0x20)
            return false;
        if ((unsigned int)text[i] == '?')
            return false;
        if ((unsigned int)text[i] > 0x80)
            return true;
    }
    return false;
}
bool IsOnce = true;
void __stdcall AyamiPrintText(char* Text)
{
    //if (strstr(Text, "#"))//Text 
    //{
        DWORD offset = 0x4;
        if (OldAddr != 0 && (unsigned int)Text[2] == '#' || strlen(Text) == 3 && !IsOnce)
        {
            RetText = OldAddr;
            memcpy(RetText, Text, 0x3);
            IsOnce = true;
            OldAddr = 0;
            return;
        }
        if (IsJis(Text) && IsOnce)
        {
            /*
            char* text = wtocGBK(ctowJIS(Text));
            cout << text << endl;
            char* TestText = text;
            DWORD TestTextLen = strlen(TestText);
            if (!strcmp(text, text))
            {
                char* NewVMCode = (char*)malloc(TestTextLen);
                //Build Code
                memcpy(NewVMCode, TestText, TestTextLen);
                RetText = NewVMCode;
                OldAddr = Text + strlen(Text) - 0x3;
                IsOnce = false;
            }
            */
            CRC32 crc;
            wstring gbktext = ctowJIS(Text);
            DWORD strcrc = crc.Calc((char*)gbktext.c_str(), gbktext.length() * 2);
            auto scitr = REPList.find(strcrc);
            if (scitr != REPList.end())
            {
                char* tmp = wtoc((scitr->second).c_str());
                DWORD len = strlen(tmp);
                char* NewVMCode = (char*)malloc(len);
                memcpy(NewVMCode, tmp, len);
                RetText = NewVMCode;
                OldAddr = Text + strlen(Text) - 0x3;
                IsOnce = false;

                //cout << "REPLACE:" << "0x" << hex << scitr->first << "|" << tmp << endl;
            }
            else
            {
                cout << "MISS:" << "0x" << hex << strcrc << "|" << wtoc(gbktext.c_str()) << endl;
            }
        }
    //}
}

PVOID pGetText = (PVOID)(BaseAddr + 0x66915);
PVOID dGetText = (PVOID)(BaseAddr + 0x6691B);
__declspec(naked)void mGetText()
{
    __asm
    {
        pushad
        pushfd
        push[ebx + 0x1C]
        call AyamiPrintText
        popfd
        popad
        ////////////////////////
        cmp RetText, 0
        je have
        mov ecx, RetText
        mov RetText,0
        lea ebp, [ebx + 0x1C]
        jmp dGetText
        have:
        jmp pGetText
    }
}

PVOID pGetText2 = (PVOID)(BaseAddr + 0x65b3e);
PVOID dGetText2 = (PVOID)(BaseAddr + 0x6623F);
__declspec(naked)void mGetText2()
{
    __asm
    {
        pushad
        pushfd
        push ebp
        call AyamiPrintText
        popfd
        popad

        cmp RetText, 0
        je have
        mov ebp, RetText
        mov RetText, 0
        have:
        jmp pGetText2
    }
}

void Init()
{
    make_console();
    cout << "0x" << hex << pGetText2 << endl;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    //DetourAttach((void**)&pGetText, mGetText);
    DetourAttach((void**)&pGetText2, mGetText2);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(0, L"Hook Error.", L"Attention", 0);
    }
}

void InitText()
{
    ifstream fin("mix.txt");
    const int LineMax = 0x1000;
    char str[LineMax];
    if (fin.is_open())
    {
        int counter = 0;
        while (fin.getline(str, LineMax))
        {
            auto wtmp = ctowUTF(str);
            wstring wline = wtmp;
            if (wline[0] == (wchar_t)L'/' && wline[1] == (wchar_t)L'/')
            {
                wline = wline.substr(2);
                rawlist.push_back(wline);
            }
            else
            {
                newlist.push_back(wline);
            }
        }
        fin.close();
        size_t index = 0;
        CRC32 crc;
        for (index; index < rawlist.size(); ++index)
        {
            crc.Init();
            DWORD crcval = crc.Calc((char*)rawlist.at(index).c_str(), rawlist.at(index).length() * 2);
            REPList.insert(pair<DWORD, wstring>(crcval, newlist.at(index).c_str()));
            //cout << "0x" << hex << crcval << "|" << wtoc(newlist.at(index).c_str()) << endl;
        }
    }
    else
    {
        fin.close();
        MessageBox(0, L"Can't open script file", L"InitText Error", 0);
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
        Init();
        InitText();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

