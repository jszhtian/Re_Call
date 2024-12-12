// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
bool HookOnce = false;

map<DWORD, wstring>TextList;

typedef enum _SIGNATURE
{
    TO_VOL = L'▲',//0x0708
    EOVOL = L'△',//0x0709
    STRUB = L'▼',//0x0A
    EOSTR = L'▽',//0x00
    EOPAR = L'◁',//0x06
    NXL = L'◆',//0x0704
    TO_RTN = L'◎',//0x0706
    TO_RUB = L'★',//0x0701
    NAME_SIG = L'※',//0x07
    UNKNOW_SIG = L'⊙',
    CH_HJN = L'∴',
};

wstring ProcessChsStr(WCHAR* str_in, unsigned int size)
{
    WORD v;
    static WORD list[0x005A] =
    {
           0x0001, 0x0001, 0x0002, 0x0002, 0x0003, 0x0003, 0x0004, 0x0004,
           0x0005, 0x0005, 0x0006, 0x002F, 0x0007, 0x0030, 0x0008, 0x0031,
           0x0009, 0x0032, 0x000A, 0x0033, 0x000B, 0x0034, 0x000C, 0x0035,
           0x000D, 0x0036, 0x000E, 0x0037, 0x000F, 0x0038, 0x0010, 0x0039,
           0x0011, 0x003A, 0x0012, 0x0012, 0x003B, 0x0013, 0x003C, 0x0014,
           0x003D, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x003E,
           0x0043, 0x001B, 0x003F, 0x0044, 0x001C, 0x0040, 0x0045, 0x001D,
           0x0041, 0x0046, 0x001E, 0x0042, 0x0047, 0x001F, 0x0020, 0x0021,
           0x0022, 0x0023, 0x0024, 0x0024, 0x0025, 0x0025, 0x0026, 0x0026,
           0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002C, 0x0302,
           0x0304, 0x002D, 0x002E, 0x0048, 0x0006, 0x0009, 0x4801, 0x4802,
           0x4804, 0x4805
           /*0x03xx>Wi,We,Wo 0x48xx>Vi,Ve,Vo*/
    };

    wstring chs = wstring(str_in, size);
    wstring ret(0x2000, 0);
    unsigned int len = 0;
    WORD ch;
    for (unsigned int idx = 0; idx < size; idx++)
    {
        switch (chs[idx])
        {
        case 0x000D:
        case EOPAR:
            break;
        case TO_VOL:
            ret[len++] = 0x0007;
            ret[len++] = 0x0008;
           // ret_len += 2;
            break;
        case EOVOL:
            ret[len++] = 0x0007;
            ret[len++] = 0x0009;
            //ret_len += 2;
            break;
        case STRUB:
            ret[len++] = 0x000A;
            //ret_len += 1;
            break;
        case EOSTR:
            ret[len++] = 0x0000;
            //ret_len += 1;
            break;
        case NXL:
            ret[len++] = 0x0007;
            ret[len++] = 0x0004;
            //ret_len += 2;
            break;
        case TO_RTN:
            ret[len++] = 0x0007;
            ret[len++] = 0x0006;
            //ret_len += 2;
            break;
        case TO_RUB:
            ret[len++] = 0x0007;
            ret[len++] = 0x0001;
            //ret_len += 2;
            break;
        case CH_HJN:
            ch = chs[idx++];
            ch = chs[idx];
            if (0xFF1F == ch)
            {
                ret[len++] = 0x0006;
                ret[len++] = 0x0000;
                //ret_len += 2;
                break;
            }
            if (0xFF01 == ch)
            {
                ret[len++] = 0x0006;
                ret[len++] = 0x0049;
                //ret_len += 2;
                break;
            }
            if (0x3041 <= ch && ch <= 0x3096)
                v = ch - 0x3041;/*Hiragana*/
            else if (0x30A1 <= ch && ch <= 0x30FA)
                v = ch - 0x30A1;/*Katakana*/
            else
            {
                ret[len++] = CH_HJN;/*as origin*/
                ret[len++] = ch;
                cout << "!!! WTF CHAR IS ORIGIN !!!" << endl;
                //ret_len += 2;
                break;
            };/*Non-Kana*/
            if (list[v] & 0xFF00)
            {
                ret[len++] = 0x0006;
                ret[len++] = list[v] >> 8;
                ret[len++] = 0x0006;
                ret[len++] = list[v] & 0x00FF;
                //ret_len += 4;
            }/*Dual Kana*/
            else
            {
                ret[len++] = 0x0006;
                ret[len++] = list[v];
                //ret_len += 2;
            }
            break;
        default:
            //ret[len++] = chs[idx];
            //ret_len += 1;
            if (chs[idx] == L'\\' && chs[idx + 1] == L'n')
            {
                ret[len++] = L'\n';
                idx += 1;
            }
            else
                ret[len++] = chs[idx];
        }
    }
    ret[len++] = 0x0000;
    return move(ret);
}

DWORD RetOff = 0;
//WCHAR TextBuff[0x2000] = { 0 };

void WINAPI ProcessMessage(DWORD Text, DWORD Offset)
{
    RetOff = Offset;
    auto scitr = TextList.find(Offset);
    if (scitr != TextList.end())
    {
        //memset(TextBuff, 0, 0x2000);
        wstring chsStr = ProcessChsStr((WCHAR*)scitr->second.c_str(), scitr->second.length());
        //lstrcpyW(TextBuff, chsStr.c_str());
        RetOff = (DWORD)chsStr.c_str() - Text;
        cout << "Offset:" << Offset << "|str:" << wtoc(scitr->second.c_str(), 936) << endl;
    }
}

PVOID pASM_ProcessMessage = (PVOID)0x00421C3B;
__declspec(naked)void ASM_ProcessMessage()
{
    __asm
    {
        pushad;
        pushfd;
        push edi;
        push ecx
        call ProcessMessage;
        popfd;
        popad;
        mov edi, RetOff;
        jmp pASM_ProcessMessage
    }
}

PVOID pBypassFontCheck = (PVOID)0x00430361;
__declspec(naked)void BypassFontCheck()
{
    __asm
    {
        mov eax, ebx;
        jmp pBypassFontCheck;
    }
}

void InlinePatch()
{
    DetourTransactionBegin();

    DetourAttach(&pBypassFontCheck, BypassFontCheck);
    DetourAttach(&pASM_ProcessMessage, ASM_ProcessMessage);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Patch hook error", L"Patch", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}

PVOID g_pOldCreateFontIndirectW = CreateFontIndirectW;
typedef int (WINAPI* PfuncCreateFontIndirectW)(LOGFONTW* lplf);
int WINAPI NewCreateFontIndirectW(LOGFONTW* lplf)
{

    lplf->lfCharSet = GB2312_CHARSET;
    if (lplf->lfFaceName[0] == '@')
    {
        wcscpy_s(lplf->lfFaceName, L"@SimHei");
    }
    else
    {
        wcscpy_s(lplf->lfFaceName, L"SimHei");
    }
    if (!HookOnce)
    {
        InlinePatch();
        HookOnce = true;
    }
    return ((PfuncCreateFontIndirectW)g_pOldCreateFontIndirectW)(lplf);
}

void Init()
{
    //g_pOldSetWindowTextW = DetourFindFunction("USER32.dll", "SetWindowTextW");
    DetourTransactionBegin();
    //DetourAttach(&g_pOldSetWindowTextW, NewSetWindowTextW);
    DetourAttach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Init hook error", L"Init", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}

void SetMessageScript()
{
    ifstream fin("MalieMoji.txt");
    const int LineMax = 0x2000;
    CHAR str[LineMax];
    if (fin.is_open())
    {
        while (fin.getline(str, LineMax))
        {
            auto wtmp = ctow(str, CP_UTF8);
            wstring wline = wtmp;
            wstring OffVal = wline.substr(2, 8);
            wstring wstr = wline.substr(11);
            DWORD Off = wcstoul(OffVal.c_str(), NULL, 16);
            //cout << "LOAD:" << "0x" << hex << Off << wtoc(wstr.c_str(), 936) << endl;
            TextList.insert(pair<DWORD, wstring>(Off, wstr));
        }
    }
    else
    {
        MessageBox(0, L"File Open fail!", L"Error", MB_OK | MB_ICONERROR);
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
        SetMessageScript();
        Init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

