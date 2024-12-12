// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

wstring ProcessStr(WCHAR* str_in, size_t sz)
{
    wstring str(str_in, sz);
    //size_t sz = str.size();
    bool fl_rub = 0, fl_vol = 0;
    wstring ret(sz + 3, 0);
    size_t len = 0;
    WORD pch;
    for (size_t idx = 0; idx < sz; idx++)
    {
        switch (str[idx])
        {
        case 0:
            if (fl_rub || fl_vol) fl_rub = fl_vol = 0;
            ret[len++] = EOSTR;
            break;
        case 1:
            idx += 4;
            break;
        case 2:
            ++idx;
            break;
        case 3:
            idx += 2;
            break;
        case 4:
            ++idx;
            break;
        case 5:
            idx += 2;
            break;
        case 6:
            ret[len++] = CH_HJN;
            pch = str[++idx];
            if (pch == 155) pch = 15;/*fix Mr.Masada's bug*/
            if (pch == 100) pch = 16;/*fix Mr.Masada's bug*/
            ret[len++] = Jindai2Kana[pch];
            break;
        case 7:
            switch (str[++idx])
            {
            case 0x0001://递归调用文字读取，然后继续处理（包含注释的文字）
                ret[len++] = TO_RUB;
                fl_rub = 1;
                break;
            case 0x0004://下一句自动出来
                ret[len++] = NXL;
                break;
            case 0x0006://代表本句结束
                ret[len++] = TO_RTN;
                break;
            case 0x0007://递归调用文字读取然后wcslen，跳过不处理。应该是用于注释
                ++idx;
                idx += wcslen(&str[idx]);
                break;
            case 0x0008://LoadVoice 后面是Voice名
                ret[len++] = TO_VOL;
                fl_vol = 1;
                break;
            case 0x0009://LoadVoice结束
                ret[len++] = EOVOL;
                break;
            default:
                ret[len++] = UNKNOW_SIG;
                break;
            }
            break;
        default:
            ret[len++] = str[idx];
        }
    }
    ret[len++] = EOPAR;
    return ret;
}

void _stdcall HookPushStr(unsigned int VM_CODE, unsigned int offset, unsigned int size)
{
    /*
    FILE* fp = fopen("log.txt", "ab+");
    wstring ret = ProcessStr(str, sz / 2);
    char* writeBuff = wtocUTF(ret.c_str());
    fwrite(writeBuff, strlen(writeBuff), 1, fp);
    fwrite("\r\n", 2, 1, fp);
    fclose(fp);
    //MalieString m_str(wstring(str, sz));
    //cout << "ReadTxt:" << wtocGBK(ProcessStr(str ,sz / 2).c_str()) << endl;
    cout << "ReadIdx:0x" << hex << idx << endl;
    cout << "ReadSz:0x" << hex << sz / 2 << endl;
    */
    WCHAR* str = (WCHAR*)(VM_CODE + offset);

    wstring ptr = ProcessStr(str, size / 2);
    cout << "offset:0x" << hex << offset;
    cout << "|" << wtocGBK(ptr.c_str()) << endl;
}

PVOID pHookPushStr = (PVOID)0x525E07;//(PVOID)(0x10021C4D);
__declspec(naked)void gHookPushStr()
{
    __asm
    {
        pushad;
        pushfd;
        push ecx; // size
        push ebx; // offset
        push esi; // str
        call HookPushStr;
        popfd;
        popad;
        jmp pHookPushStr;
    }
}

void _stdcall HookPushStr2(unsigned int VM_DATA, unsigned int offset)
{
    WCHAR* str = (WCHAR*)(VM_DATA + offset);
    cout << "offset:0x" << hex << offset;
    cout << "|" << wtocGBK(str) << endl;
}

PVOID pHookPushStr2 = (PVOID)0x5326D6;//(PVOID)(0x10021C4D);
__declspec(naked)void gHookPushStr2()
{
    __asm
    {
        pushad;
        pushfd;
        push eax; // offset
        push edx; // str
        call HookPushStr2;
        popfd;
        popad;
        jmp pHookPushStr2;
    }
}

void Patch()
{
    DetourTransactionBegin();

    DetourAttach(&pHookPushStr, gHookPushStr);
    DetourAttach(&pHookPushStr2, gHookPushStr2);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Patch hook error", L"Patch", MB_OK);
        ExitProcess(-1);
    }
}

bool IsOnce = false;
PVOID g_pOldSetWindowTextW = NULL;
typedef int (WINAPI* PfuncSetWindowTextW)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextW(HWND hwnd, LPWSTR lpString)
{
    if (!lstrcmp(lpString, L"恄欘恄埿恄妝 弻擵岝")) 
    {
        lstrcpy(lpString, L"神咒神威神楽 曙之光 - v1.0");
        if (!IsOnce)
        {
            IsOnce = true;
            Patch();
        }
    }
    return ((PfuncSetWindowTextW)g_pOldSetWindowTextW)(hwnd, lpString);
}

PVOID g_pOldCreateFontIndirectW = NULL;
typedef int (WINAPI* PfuncCreateFontIndirectW)(LOGFONTW* lplf);
int WINAPI NewCreateFontIndirectW(LOGFONTW* lplf)
{

    if (wcscmp(lplf->lfFaceName, L"MS UI Gothic") == 0)
    {
        wcscpy_s(lplf->lfFaceName, L"SimHei");
    }

    return ((PfuncCreateFontIndirectW)g_pOldCreateFontIndirectW)(lplf);
}

void Init()
{
    g_pOldSetWindowTextW = DetourFindFunction("USER32.dll", "SetWindowTextW");
    g_pOldCreateFontIndirectW = DetourFindFunction("GDI32.dll", "CreateFontIndirectW");
    DetourTransactionBegin();
    DetourAttach(&g_pOldSetWindowTextW, NewSetWindowTextW);
    DetourAttach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Init hook error", L"Init", MB_OK);
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
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

