// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

DWORD CopyOff = 0;

PVOID g_pOldMultiByteToWideChar = MultiByteToWideChar;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
    _In_      UINT   CodePage,
    _In_      DWORD  dwFlags,
    _In_      LPCSTR lpMultiByteStr,
    _In_      int    cbMultiByte,
    _Out_opt_ LPWSTR lpWideCharStr,
    _In_      int    cchWideChar);
int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
    if (cp == 932 && cbMB == -1 || cbMB >= 2)
    {
        if ((unsigned char)lpMBS[0] == '^' && (unsigned char)lpMBS[1] == '8')
        {
            cp = 936;
            dwFg = 0;
            lpMBS += 2;
            cout << lpMBS << endl;
            if (cbMB != -1 && cbMB > 2)
            {
                cbMB -= 2;
            }
        }
    }
    return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);;
}

void APIHook()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(0, L"APIHook Error!", L"APIHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}





char CopyStr[0x1000] = { 0 };

PVOID pGetOff1 = (PVOID)(BaseAddr + 0xD3D8D);
__declspec(naked)void gGetOff1()
{
    _asm
    {
        mov CopyOff, eax;
        jmp pGetOff1
    }
}

DWORD TMP_REG = 0;
PVOID pGetOff2 = (PVOID)(BaseAddr + 0xD3E09);
__declspec(naked)void gGetOff2()
{
    _asm
    {
        mov TMP_REG, eax;
        mov eax, [esi];
        mov CopyOff, eax;
        mov eax, TMP_REG;
        jmp pGetOff2
    }
}

PVOID pGetOff3 = (PVOID)(BaseAddr + 0xD3E43);
__declspec(naked)void gGetOff3()
{
    _asm
    {
        mov CopyOff, eax;
        jmp pGetOff3
    }
}


DWORD AddrBack = BaseAddr + 0xD4230;
CHAR* m_ptr = nullptr;
DWORD OldLen = 0;
int  Hook_sub_4D4230(int a1, int a2, int a3, const CHAR a4, DWORD* a5, int a6, int a7)
{
    /*
    _asm
    {
        lea eax, a4;
        add eax, 0x4;
        mov eax, [eax];
        mov m_ptr, eax;
    }
    cout << "0x" << hex << CopyOff << m_ptr << endl;
    //*/
    /*
    cout << "0x" << hex << CopyOff << endl;
    if (CopyOff == 0x144)
    {
        memset(CopyStr, 0, 0x1000);
        _asm
        {
            lea eax, a4;
            add eax, 0x4;
            mov eax, [eax];
            mov m_ptr, eax;
        }
        string old_str = string(m_ptr);
        //cout << old_str << endl;
        string new_str = "^8";
        string line = "这是我的中文测试呀呀呀呀呀呀呀呀呀呀哎呀呀呀呀呀呀呀呀１２３ＡＢＣ";
        if (old_str.find("%LF") == 0)
            new_str += "%LF" + line;
        else if (old_str.length() >= 4 && old_str.substr(old_str.length() - 4) == "%K%P")
            new_str += line + "%K%P";
        else
        {
            cout << "mb err" << endl;
            new_str += line;
        }
        //cout << new_str << endl;
        lstrcpyA(CopyStr, new_str.c_str());
        char* ptr_CopyStr = CopyStr;
        _asm
        {
            lea eax, a4;
            add eax, 0x4;
            mov ebx, ptr_CopyStr;
            mov[eax], ebx;
        }
        OldLen = old_str.length();

    }
    else if (CopyOff == 0x131)
    {
        memset(CopyStr, 0, 0x1000);
        _asm
        {
            lea eax, a4;
            add eax, 0x4;
            mov eax, [eax];
            mov m_ptr, eax;
        }
        string old_str = string(m_ptr);
        //cout << old_str << endl;
        string new_str = "^8";
        string line = "人名测试";
        if (old_str.find("%LF") == 0)
            new_str += "%LF" + line;
        else if (old_str.length() >= 4 && old_str.substr(old_str.length() - 4) == "%K%P")
            new_str += line + "%K%P";
        else
        {
            cout << "mb err" << endl;
            new_str += line;
        }
        //cout << new_str << endl;
        lstrcpyA(CopyStr, new_str.c_str());
        char* ptr_CopyStr = CopyStr;
        _asm
        {
            lea eax, a4;
            add eax, 0x4;
            mov ebx, ptr_CopyStr;
            mov[eax], ebx;
        }
        OldLen = old_str.length();
    }

    //*/
    //cout << m_ptr << endl;
    return 0;
}

__declspec(naked)void ASM_sub_4D4230()
{
    _asm
    {
        //cmp DWORD PTR[esp], 0x4D3DA7;
        //jnz code_out;
        push ebp;
        push ecx;
        call Hook_sub_4D4230;
        pop ecx;
        pop ebp;
    //code_out:
        jmp AddrBack
    }
}

PVOID pChangeLen1 = (PVOID)(BaseAddr + 0xD3DA7);
__declspec(naked)void gChangeLen1()
{
    _asm
    {
        cmp OldLen, 0;
        je done;
        mov eax, OldLen;
        mov OldLen, 0
    done:
        jmp pChangeLen1
    }
}

PVOID pChangeLen2 = (PVOID)(BaseAddr + 0xD3E1C);
__declspec(naked)void gChangeLen2()
{
    _asm
    {
        cmp OldLen, 0;
        je done;
        mov eax, OldLen;
        mov OldLen, 0
    done:
        jmp pChangeLen2
    }
}

PVOID pChangeLen3 = (PVOID)(BaseAddr + 0xD3E5C);
__declspec(naked)void gChangeLen3()
{
    _asm
    {
        cmp OldLen, 0;
        je done;
        mov eax, OldLen;
        mov OldLen, 0
    done:
        jmp pChangeLen3
    }
}

char* SelPtr = nullptr;
void WINAPI GetSel(DWORD off, char* buff)
{
    //cout << "0x" << hex << off << "|" << (buff + off) << endl;
    if (off == 0x303b)
    {
        //SelPtr = (char*)"这是我的选项测试啦啦啦啦啦啦啦啦阿里阿巴巴";
        string old_str = string(buff);
        string new_str = "^8";
        string line = "这是我的选项测试啦啦啦啦啦啦啦啦阿里阿巴巴";
        new_str += line;
        SelPtr = (char*)new_str.c_str();
    }
    else
        SelPtr = buff;
}

DWORD ECX_TMP = 0;
PVOID pGetSel = (PVOID)(BaseAddr + 0xD3F58);
__declspec(naked)void gGetSel()
{
    _asm
    {
        pushad;
        pushfd;
        push ecx; // buff
        push[ebx]; // off
        call GetSel;
        popfd;
        popad;
        mov ecx, SelPtr;
        jmp pGetSel;
    }
}


void InlineHook()
{
    DetourTransactionBegin();
    DetourAttach((void**)&pGetOff1, gGetOff1);
    DetourAttach((void**)&pGetOff2, gGetOff2);
    DetourAttach((void**)&pGetOff3, gGetOff3);
    DetourAttach((void**)&AddrBack, ASM_sub_4D4230);
    DetourAttach((void**)&pChangeLen1, gChangeLen1);
    DetourAttach((void**)&pChangeLen2, gChangeLen2);
    DetourAttach((void**)&pChangeLen3, gChangeLen3);


    DetourAttach((void**)&pGetSel, gGetSel);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(0, L"InlineHook Error!", L"InlineHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
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
        APIHook();
        InlineHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

