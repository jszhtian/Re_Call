// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
vector<wstring>rawlist;
vector<wstring>newlist;

map<DWORD, wstring> SCRList;
map<DWORD, wstring> REPList;

HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    return CreateFontIndirectA(lplf);
}

HFONT WINAPI HookCreateFontA(int nHeight,
    int nWidth,
    int nEscapement,
    int nOrientation,
    int fnWeight,
    DWORD fdwltalic,
    DWORD fdwUnderline,
    DWORD fdwStrikeOut,
    DWORD fdwCharSet,
    DWORD fdwOutputPrecision,
    DWORD fdwClipPrecision,
    DWORD fdwQuality,
    DWORD fdwPitchAndFamily,
    LPCSTR lpszFace)
{
    return CreateFontA(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwltalic, fdwUnderline, fdwStrikeOut, GB2312_CHARSET,
        fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
}

PVOID g_pOldEnumFontFamiliesExA = EnumFontFamiliesExA;
typedef int (WINAPI* PfuncEnumFontFamiliesExA)(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags);
int WINAPI NewEnumFontFamiliesExA(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpEnumFontFamExProc, LPARAM lParam, DWORD dwFlags)
{
    lpLogfont->lfCharSet = GB2312_CHARSET;
    return ((PfuncEnumFontFamiliesExA)g_pOldEnumFontFamiliesExA)(hdc, lpLogfont, lpEnumFontFamExProc, lParam, dwFlags);
}

HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    const char* szWndName = "【枫笛汉化组】我们没有翅膀 - v1.0";
    HWND hwnd = CreateWindowExA(dwExStyle, lpClassName, szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    return hwnd;
}

void IATHook()
{
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectA"), (PROC)HookCreateFontIndirectA))
    {
        MessageBoxW(0, L"CreateFontIndirectA Hook Error。", L"IATHook Error", 0);
        return;
    }
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontA"), (PROC)HookCreateFontA))
    {
        MessageBoxW(0, L"CreateFontA Hook Error。", L"IATHook Error", 0);
        return;
    }
    if (!IATPatch("User32.dll", GetProcAddress(GetModuleHandleW(L"User32.dll"), "CreateWindowExA"), (PROC)HookCreateWindowExA))
    {
        MessageBoxW(0, L"CreateWindowExW Hook Error。", L"IATHook Error", 0);
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBoxW(0, L"EnumFontFamiliesExA Hook Error。", L"APIHook Error", 0);
        return;
    }
}

/*
name = BaseAddr + 0x55668 -> edx
text = BaseAddr + 0x556AD -> edx
title = BaseAddr + 0x21155 -> edi
System = BaseAddr + 0x9DF17 -> ebp
*/

bool IsMojika(char* text)
{
    if (strlen(text) == 0 || text == nullptr)
        return false;
    for (int a = 0; a < strlen(text); a++)
    {
        if ((unsigned int)text[a] > 0x80)
            return true;
    }
    return false;
}

void mstrcpy(char* a1, char* a2)
{
    memset(a1, 0, strlen(a1));
    memcpy(a1, a2, strlen(a2));
    memset(a1 + strlen(a2), 0, 0x2);
}

void __stdcall AymShowTextA(char* Text)
{
    if (IsMojika(Text))
    {
        CRC32 crc;
        wstring gbktext = ctowJIS(Text);
        DWORD strcrc = crc.Calc((char*)gbktext.c_str(), gbktext.length() * 2);
        auto scitr = REPList.find(strcrc);
        if (scitr != REPList.end())
        {
            char* tmp = wtoc((scitr->second).c_str());
            lstrcpyA(Text, tmp);
        }
        else
        {
            char* mText = wtocUTF(gbktext.c_str());
            cout << "MISS:" << "0x" << hex << strcrc << "|" << wtoc(gbktext.c_str()) << endl;
            FILE* fp = fopen("MissText.txt", "ab+");
            fwrite("-------------------------------------------------------", 55, 1, fp);
            fwrite("\r\n", 2, 1, fp);
            fwrite(mText, strlen(mText), 1, fp);
            fwrite("\r\n", 2, 1, fp);
            fwrite("-------------------------------------------------------", 55, 1, fp);
            fwrite("\r\n", 2, 1, fp);
            fclose(fp);
        }
        //lstrcpyA(Text, wtoc(ctowJIS(Text)));
    }
}
char* RetAddr = NULL;
void __stdcall AymShowText2A(char* Text)
{
    if (IsMojika(Text))
    {
        CRC32 crc;
        wstring gbktext = ctowJIS(Text);
        DWORD strcrc = crc.Calc((char*)gbktext.c_str(), gbktext.length() * 2);
        auto scitr = REPList.find(strcrc);
        if (scitr != REPList.end())
        {
            char* tmp = wtoc((scitr->second).c_str());
            //lstrcpyA(Text, tmp);
            RetAddr = tmp;
        }
        else
        {
            char* mText = wtocUTF(gbktext.c_str());
            cout << "MISS:" << "0x" << hex << strcrc << "|" << wtoc(gbktext.c_str()) << endl;
            FILE* fp = fopen("MissText.txt", "ab+");
            fwrite("-------------------------------------------------------", 55, 1, fp);
            fwrite("\r\n", 2, 1, fp);
            fwrite(mText, strlen(mText), 1, fp);
            fwrite("\r\n", 2, 1, fp);
            fwrite("-------------------------------------------------------", 55, 1, fp);
            fwrite("\r\n", 2, 1, fp);
            fclose(fp);
        }
        //lstrcpyA(Text, wtoc(ctowJIS(Text)));
    }
}

PVOID pGetText = (PVOID)(BaseAddr + 0x556B3);
__declspec(naked)void mGetText()
{
    __asm
    {
        pushad
        pushfd
        push edx
        call AymShowTextA
        popfd
        popad
        jmp pGetText
    }
}

PVOID pGetName = (PVOID)(BaseAddr + 0x5566D);
__declspec(naked)void mGetName()
{
    __asm
    {
        pushad
        pushfd
        push edx
        call AymShowTextA
        popfd
        popad
        jmp pGetName
    }
}

PVOID pGetTitle = (PVOID)(BaseAddr + 0x21155);
__declspec(naked)void mGetTitle()
{
    __asm
    {
        pushad
        pushfd
        push edi
        call AymShowText2A
        popfd
        popad

        cmp RetAddr, 0
        je have
        mov edi, RetAddr
        mov RetAddr, 0
        have:
        jmp pGetTitle
    }
}

PVOID pGetSystem = (PVOID)(BaseAddr + 0x9DF17);
__declspec(naked)void mGetSystem()
{
    __asm
    {
        pushad
        pushfd
        push ebp
        call AymShowTextA
        popfd
        popad
        jmp pGetSystem
    }
}

PVOID pGetSystem2 = (PVOID)(BaseAddr + 0x9A902);
__declspec(naked)void mGetSystem2()
{
    __asm
    {
        pushad
        pushfd
        push ecx
        call AymShowText2A
        popfd
        popad

        cmp RetAddr, 0
        je have
        mov ecx, RetAddr
        mov RetAddr, 0
        have:
        jmp pGetSystem2
    }
}

PVOID pAllocTextMem = (PVOID)(BaseAddr + 0x55687);
__declspec(naked)void mAllocTextMem()
{
    __asm
    {
        mov edx, 0x400
        jmp pAllocTextMem
    }
}

void InlineHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&pGetText, mGetText);
    DetourAttach(&pGetName, mGetName);
    DetourAttach(&pGetTitle, mGetTitle);
    DetourAttach(&pGetSystem, mGetSystem);
    DetourAttach(&pGetSystem2, mGetSystem2);
    DetourAttach(&pAllocTextMem, mAllocTextMem);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBoxW(0, L"InlineHook Hook Error。", L"InlineHook Error", 0);
        return;
    }
}

void InitText()
{
    ifstream fin("Script.txt");
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
        MessageBox(0, L"Can't open script file", L"InitText Error",0);
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
        //InitText();
        IATHook();
        //InlineHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI FengDi()
{
}