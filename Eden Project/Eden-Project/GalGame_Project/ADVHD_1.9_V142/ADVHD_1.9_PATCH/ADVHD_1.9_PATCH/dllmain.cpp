// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
CPKFileSystem CFPK;

PVOID g_pOldMultiByteToWideChar = MultiByteToWideChar;
typedef int(WINAPI* PfuncMultiByteToWideChar)(
    _In_      UINT   CodePage,
    _In_      DWORD  dwFlags,
    _In_      LPCSTR lpMultiByteStr,
    _In_      int    cbMultiByte,
    _Out_opt_ LPWSTR lpWideCharStr,
    _In_      int    cchWideChar);
int WINAPI HookMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
    if (cp == 932 && cbMB == -1 || cbMB >= 2)
    {
        if ((unsigned char)lpMBS[0] == '^' && (unsigned char)lpMBS[1] == '8')
        {
            cp = CP_UTF8;
            dwFg = 0;
            lpMBS += 2;
            //cout << lpMBS << endl;
            if (cbMB != -1 && cbMB > 2)
            {
                cbMB -= 2;
            }
        }
    }
    return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(cp, dwFg, lpMBS, cbMB, lpWCS, ccWC);;
}


PVOID g_pOldCreateFontIndirectW = CreateFontIndirectW;
typedef HFONT(WINAPI* PfuncCreateFontIndirectW)(LOGFONTW* lplf);
HFONT WINAPI HookCreateFontIndirectW(LOGFONTW* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    if (wcscmp(lplf->lfFaceName, L"MS UI Gothic") == 0 ||
        wcscmp(lplf->lfFaceName, L"ＭＳ ゴシック") == 0)
    {
        wcscpy_s(lplf->lfFaceName, L"SimHei");
    }
    return ((PfuncCreateFontIndirectW)g_pOldCreateFontIndirectW)(lplf);
}

PVOID g_pOldSetWindowTextW = SetWindowTextW;
typedef BOOL (WINAPI* pfuncSetWindowTextW)(
    HWND    hWnd,
    LPCWSTR lpString
);
BOOL WINAPI HookSetWindowTextW(
    HWND    hWnd,
    LPCWSTR lpString
)
{
    //cout << wtoc(lpString, 936) << endl;
    if(!lstrcmpW(lpString, L"虜ノ旋律 －refrain－ 調律される処女たちと女教師-Settings"))
        return ((pfuncSetWindowTextW)g_pOldSetWindowTextW)(hWnd, L"Game Start Settings");
    return ((pfuncSetWindowTextW)g_pOldSetWindowTextW)(hWnd, lpString);
}
PVOID g_pOldCreateWindowExW = CreateWindowExW;
typedef HWND (WINAPI* pfuncCreateWindowExW)(
    DWORD     dwExStyle,
    LPCWSTR   lpClassName,
    LPCWSTR   lpWindowName,
    DWORD     dwStyle,
    int       X,
    int       Y,
    int       nWidth,
    int       nHeight,
    HWND      hWndParent,
    HMENU     hMenu,
    HINSTANCE hInstance,
    LPVOID    lpParam
);
HWND WINAPI HookCreateWindowExW(
    DWORD     dwExStyle,
    LPCWSTR   lpClassName,
    LPCWSTR   lpWindowName,
    DWORD     dwStyle,
    int       X,
    int       Y,
    int       nWidth,
    int       nHeight,
    HWND      hWndParent,
    HMENU     hMenu,
    HINSTANCE hInstance,
    LPVOID    lpParam
)
{
    //cout << wtoc(lpWindowName, 936) << endl;
    if(!lstrcmpW(lpWindowName, L"虜ノ旋律 －refrain－ 調律される処女たちと女教師"))
        return ((pfuncCreateWindowExW)g_pOldCreateWindowExW)(
            dwExStyle,
            lpClassName,
            L"【一青二白汉化组】虏之旋律－refrain－ 被调音的处女们和女教师",
            dwStyle,
            X,
            Y,
            nWidth,
            nHeight,
            hWndParent,
            hMenu,
            hInstance,
            lpParam
            );
    return ((pfuncCreateWindowExW)g_pOldCreateWindowExW)(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
        );
}
void APIHook()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldMultiByteToWideChar, HookMultiByteToWideChar);
    DetourAttach(&g_pOldCreateFontIndirectW, HookCreateFontIndirectW);
    DetourAttach(&g_pOldSetWindowTextW, HookSetWindowTextW);
    DetourAttach(&g_pOldCreateWindowExW, HookCreateWindowExW);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(0, L"APIHook Error!", L"APIHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}

BYTE* Txt_buff = nullptr;
BYTE* Idx_buff = nullptr;

// 替换文本
PVOID Addr_sub_4D4230 = NULL;
int sub_4D4230(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int retVal = 0;
    _asm
    {
        push a7;
        push a6;
        push a5;
        push a4;
        push a3;
        mov ecx, a1;
        mov ebp, a2;
        call Addr_sub_4D4230;
        //add esp, 0x14;
        mov retVal, eax;
    }
    return retVal;
}
//int Hook_sub_4D4230(int a1, int a2, int a3, const CHAR* a4, DWORD* a5, int a6, int a7)
int sub_4D4230_ebpVal = 0;
int sub_4D4230_ret = 0;
int Hook_sub_4D4230(Registers* regs)
{
    _asm mov sub_4D4230_ebpVal, ebp;
    auto a7 = *(int*)(regs->esp + 0x14);
    auto a6 = *(int*)(regs->esp + 0x10);
    auto a5 = *(int*)(regs->esp + 0xC);
    auto a4 = *(int*)(regs->esp + 0x8);
    auto a3 = *(int*)(regs->esp + 0x4);
    auto a2 = (int)regs->ebp;
    auto a1 = (int)regs->ecx;

    if (*(int*)regs->esp != 0x4D3DA7)
        goto code_ret;
    if (Txt_buff)
    {
        auto off = *(int*)regs->esi;
        /*
        * esi -> cur buff
        * esi + 4 -> start buff
        * esi + 8 -> end buff
        */
        auto scitr = TextList.find(off);
        if (scitr != TextList.end())
        {
            string old_str = string((char*)a4);
            //cout << old_str << endl;
            string new_str = "^8";
            string line = scitr->second;
            if (old_str.find("%LF") == 0)
                new_str += "%LF" + line;
            else if (old_str.length() >= 4 && old_str.substr(old_str.length() - 4) == "%K%P")
                new_str += line + "%K%P";
            else
            {
                //cout << "mb err" << endl;
                new_str += line;
            }
            //cout << wtoc(ctow((CHAR*)new_str.c_str(), CP_UTF8), 936) << endl;
            sub_4D4230_ret = sub_4D4230(a1, a2, a3, (int)new_str.c_str(), a5, a6, a7);
            _asm mov ebp, sub_4D4230_ebpVal;
            return old_str.length();
        }
    }
code_ret:
    sub_4D4230_ret = sub_4D4230(a1, a2, a3, a4, a5, a6, a7);
    _asm mov ebp, sub_4D4230_ebpVal;
    return sub_4D4230_ret;
}

__declspec(naked)void ASM_sub_4D4230()
{
    _asm
    {
        pushad;
        pushfd;
        mov ebp, esp;
        push ebp;
        call Hook_sub_4D4230;
        add esp, 0x4;
        mov[esp + 0x20], eax
        popfd;
        popad;
        retn 0x14;
    }
}

// 替换选项
void Hook_4D3F58(Registers* regs)
{
    int off = *(int*)regs->ebx;
    //cout << "Seloff:0x" << hex << off << endl;
    if (Txt_buff)
    {
        auto scitr = TextList.find(off);
        if (scitr != TextList.end())
        {
            auto& line = scitr->second;
            static char sel[200];
            strcpy_s(sel, "^8");
            if(line.find("Sel:") == 0)
                strcat_s(sel, &line[4]);
            else
                strcat_s(sel, line.c_str());
            //strcat_s(sel, "\0");
            regs->ecx = (DWORD)sel;
        }
    }
}

PVOID Addr_4D3F58 = NULL;
__declspec(naked)void ASM_4D3F58()
{
    _asm
    {
        pushad;
        pushfd;
        mov ebp, esp;
        push ebp;
        call Hook_4D3F58;
        add esp, 0x4;
        popfd;
        popad;
        jmp Addr_4D3F58;
    }
}


// 读取
vector<string> SplitTxtA(BYTE* txt, DWORD size)
{
    auto buff = (uint8_t*)txt;
    uint32_t i = 0;
    uint32_t last = 0;
    auto ls = vector<string>();
    while (true)
    {
        while (i < size && buff[i] != '\r') 
            i++;
        ls.push_back(string((char*)buff + last, i - last));
        if (i + 2 > size) 
            break;
        i += 2;
        last = i;
    }
    return ls;
}

vector<UINT> SplitIdx(BYTE* buff, DWORD size)
{
    auto ls = vector<UINT>();
    for (int i = 0; i < size; i+=4)
        ls.push_back(*(DWORD*)(buff + i));
    return ls;
}

void ReadFileFromDir(wstring FileName)
{
    if (Txt_buff)
    {
        delete[] Txt_buff;
        delete[] Idx_buff;
        Txt_buff = nullptr;
        Idx_buff = nullptr;
        TextList.clear();
    }
    wstring fnm = FileName.substr(0, FileName.length() - 4);
    //FileNameToLower(fnm);
    wstring txt = L"#Project\\txt\\" + fnm + L".txt";
    wstring idx = L"#Project\\idx\\" + fnm + L".idx";

    FILE* fp = nullptr;
    // txt
    _wfopen_s(&fp, txt.c_str(), L"rb");
    if (!fp)
        return;
    fseek(fp, 0, SEEK_END);
    DWORD Txt_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    Txt_buff = new BYTE[Txt_size];
    fread(Txt_buff, 1, Txt_size, fp);
    fclose(fp);

    // idx
    _wfopen_s(&fp, idx.c_str(), L"rb");
    if (!fp)
        return;
    fseek(fp, 0, SEEK_END);
    DWORD Idx_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    Idx_buff = new BYTE[Idx_size];
    fread(Idx_buff, 1, Idx_size, fp);
    fclose(fp);

    auto txt_line = SplitTxtA(Txt_buff + 3, Txt_size - 3);// 跳过BOM
    auto idx_line = SplitIdx(Idx_buff + 4, Idx_size - 4); // 跳过头部CRC

    if (txt_line.size() != idx_line.size())
    {
        WCHAR err_msg[MAX_PATH] = { 0 };
        wsprintfW(err_msg, L"%s行数不匹配。", fnm.c_str());
        MessageBox(0, err_msg, L"错误", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }

    for (int i = 0; i < txt_line.size(); i++)
    {
        //cout << hex << idx_line[i] << "|" << wtoc(ctow((CHAR*)txt_line[i].c_str(), CP_UTF8), 936) << endl;

        TextList.insert(pair<UINT, string>(idx_line[i], txt_line[i]));
    }
    cout << "read:" <<  wtoc((WCHAR*)txt.c_str(), CP_ACP) << endl;
}

#if 0
void ReadFileFromPack(wstring FileName)
{
    if (Txt_buff)
    {
        delete[] Txt_buff;
        delete[] Idx_buff;
        Txt_buff = nullptr;
        Idx_buff = nullptr;
        TextList.clear();
    }
    wstring fnm = FileName.substr(0, FileName.length() - 4);
    FileNameToLower(fnm);
    wstring txt = L"txt\\" + fnm + L".txt";
    wstring idx = L"idx\\" + fnm + L".idx";
    DWORD Txt_size = 0;
    DWORD Idx_size = 0;
    if (!CFPK.GetFileByName((WCHAR*)txt.c_str(), &Txt_buff, &Txt_size))
    {
        return;
    }
    if (!CFPK.GetFileByName((WCHAR*)idx.c_str(), &Idx_buff, &Idx_size))
    {
        return;
    }

    auto txt_line = SplitTxtA(Txt_buff + 3, Txt_size - 3);// 跳过BOM
    auto idx_line = SplitIdx(Idx_buff + 4, Idx_size - 4); // 跳过头部CRC

    if (txt_line.size() != idx_line.size())
    {
        WCHAR err_msg[MAX_PATH] = { 0 };
        wsprintfW(err_msg, L"%s行数不匹配。", fnm.c_str());
        MessageBox(0, err_msg, L"错误", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }

    for (int i = 0; i < txt_line.size(); i++)
    {
        //cout << hex << idx_line[i] << "|" << wtoc(ctow((CHAR*)txt_line[i].c_str(), CP_UTF8), 936) << endl;

        TextList.insert(pair<UINT, string>(idx_line[i], txt_line[i]));
    }
    //cout << "read:" << wtoc((WCHAR*)fnm.c_str(), CP_ACP) << endl;
}
#endif

PVOID Addr_sub_409690 = NULL;
int sub_409690_ebpVal = 0;
void Hook_sub_409690(Registers* regs)
{
    // wchar_t
    WCHAR* FileName = (WCHAR*) * (int*)(regs->esp + 0x8);
    WCHAR* ArcName = (WCHAR*)*(int*)(regs->esp + 0x4);
    if (ArcName && FileName)
    {
       // cout << "ReadFile: " << wtoc((WCHAR*)FileName, CP_ACP) << " from " << wtoc((WCHAR*)ArcName, CP_ACP) << endl;
        wstring fnm = wstring((WCHAR*)FileName);
        //FileNameToLower(fnm);
        if (fnm.length() > 3 && fnm.substr(fnm.length() - 3) == L"ws2")
        {
            //cout << "read:" << wtoc((WCHAR*)fnm.c_str(), CP_ACP) << endl;
            ReadFileFromDir(fnm);
            //ReadFileFromPack(fnm);
        }
    }
}

__declspec(naked)void ASM_sub_409690()
{
    _asm
    {
        pushad;
        pushfd;
        mov ebp, esp;
        push ebp;
        call Hook_sub_409690;
        add esp, 0x4;
        popfd;
        popad;
        jmp Addr_sub_409690;
    }
}

void InlineHook()
{
    Addr_sub_4D4230 = (PVOID)(BaseAddr + 0xD4230);
    Addr_4D3F58 = (PVOID)(BaseAddr + 0xD3F58);
    Addr_sub_409690 = (PVOID)(BaseAddr + 0x9690);
    DetourTransactionBegin();
    //DetourAttach((void**)&pProcessStr, gProcessStr);
    DetourAttach((void**)&Addr_sub_4D4230, ASM_sub_4D4230);
    DetourAttach((void**)&Addr_4D3F58, ASM_4D3F58);
    DetourAttach((void**)&Addr_sub_409690, ASM_sub_409690);
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
        //if(!CFPK.InitFileSystem())
        //    ExitProcess(-1);
        APIHook();
        InlineHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI AyamiKaze()
{
}