// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

char* mbuff = NULL;
char* Buff = NULL;
DWORD msize = 0;
char* Name = NULL;
vector<wstring>rawlist;
vector<wstring>newlist;

map<DWORD, wstring> SCRList;
map<DWORD, wstring> REPList;
void __stdcall AymDumpFile(char* buff, DWORD size)
{
    if (Name && mbuff && msize != 0)
    {
        memset(buff, 0x00, size);
        memcpy(buff, mbuff, msize);
        _asm {
            mov eax, msize;
        }
        cout << "successful" << endl;
        free(mbuff);
        mbuff = NULL;
        msize = 0;
        Name = NULL;
    }
}
void __stdcall AymGetFileName(char* name)
{
    string fnm(name);
    string Dir = "Oniama_cn\\";
    string OutName = Dir + fnm;
    FILE* fp = fopen(OutName.c_str(), "rb");
    if (fp)
    {
        cout << OutName.c_str() << endl;
        fseek(fp, 0, SEEK_END);
        msize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        mbuff = (char*)malloc(msize);
        if (fread(mbuff, msize, 1, fp))
        {
            fclose(fp);
            Name = name;
        }
    }
}

static PVOID pGetFileName = (PVOID)(BaseAddr + 0x3386B1);
__declspec(naked) void GetFileName()
{
    __asm
    {
        pushad
        pushfd
        push edx
        call AymGetFileName
        //mov Name, edx
        popfd
        popad
        jmp pGetFileName
    }
}

DWORD TMP = 0;
static PVOID pGetFileBuff = (PVOID)(BaseAddr + 0x338a5e);
static PVOID pGetFileBuff2 = (PVOID)(BaseAddr + 0x338abc);
__declspec(naked) void GetFileBuff()
{
    __asm
    {
        pushad
        pushfd
        MOV TMP, EAX;
        MOV EAX, dword ptr ds : [eax] ;
        MOV Buff, eax;
        mov eax, TMP;
        mov TMP,0x00
        popfd
        popad
        jmp pGetFileBuff
    }
}
__declspec(naked) void GetFileBuff2()
{
    __asm
    {
        pushad
        pushfd
        MOV TMP, EAX;
        MOV EAX, dword ptr ds : [eax] ;
        MOV Buff, eax;
        mov eax, TMP;
        mov TMP, 0x00;
        popfd
        popad
        jmp pGetFileBuff2
    }
}

static PVOID pGetFileSize = (PVOID)(BaseAddr + 0x338a63);
static PVOID pGetFileSize2 = (PVOID)(BaseAddr + 0x338AC1);
__declspec(naked) void mGetFileSize()
{
    __asm
    {
        pushad
        pushfd
        push eax
        push Buff
        call AymDumpFile
        popfd
        popad
        jmp pGetFileSize
    }
}

__declspec(naked) void mGetFileSize2()
{
    __asm
    {
        pushad
        pushfd
        push eax
        push Buff
        call AymDumpFile
        popfd
        popad
        jmp pGetFileSize2
    }
}

void __stdcall AymShowFileCompressModeNumber(DWORD num)
{
    cout << num << endl;
}
static PVOID pShowFileCompressModeNumber = (PVOID)(BaseAddr + 0x338963);
__declspec(naked) void ShowFileCompressModeNumber()
{
    __asm
    {
        pushad
        pushfd
        push eax
        call AymShowFileCompressModeNumber
        popfd
        popad
        jmp pShowFileCompressModeNumber
    }
}

static PVOID pAllocMemory = (PVOID)(BaseAddr + 0x2F8712);
__declspec(naked) void mAllocMemory()
{
    __asm
    {
        pushad;
        pushfd;
        cmp msize, 0x00;
        je have;
        mov eax, msize;
        mov dword ptr ds : [esi + 0x20] , eax;
    have:
        popfd;
        popad;
        jmp pAllocMemory;
    }
}

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
    DWORD dwExStyle,
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam);
HWND WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    const char* szWndName = "【白井木学园】尽情撒娇 ～哥哥，请更多地依赖我吧～ - v1.0";

    return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

typedef HFONT(WINAPI* fnCreateFontA)(
    int nHeight, // logical height of font height
    int nWidth, // logical average character width
    int nEscapement, // angle of escapement
    int nOrientation, // base-line orientation angle
    int fnWeight, // font weight
    DWORD fdwItalic, // italic attribute flag
    DWORD fdwUnderline, // underline attribute flag
    DWORD fdwStrikeOut, // strikeout attribute flag
    DWORD fdwCharSet, // character set identifier
    DWORD fdwOutputPrecision, // output precision
    DWORD fdwClipPrecision, // clipping precision
    DWORD fdwQuality, // output quality
    DWORD fdwPitchAndFamily, // pitch and family
    LPCSTR lpszFace // pointer to typeface name string
    );
fnCreateFontA CreateFontAOLD;
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
    return CreateFontA(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, GB2312_CHARSET, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
}

HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    LOGFONTA lf;
    memcpy(&lf, lplf, sizeof(LOGFONTA));
    //strcpy(lf.lfFaceName, "SimHei");
    lf.lfCharSet = GB2312_CHARSET;
    return CreateFontIndirectA(&lf);
}

wchar_t szTitle[] = L"提示";
typedef int (WINAPI* fnMessageboxA)(
    _In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType
    );
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType)
{
    char* Pstr = wtocGBK(ctowJIS((char*)lpText));
    //cout << lpText << endl;
    //TXT << Pstr << endl;
    //cout << Pstr << endl;
    if (!lstrcmpA(Pstr, "ゲームを終了しますか？"))
        lstrcpyA(Pstr, "结束游戏吗？");
    return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}

void IATHook()
{
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontA"), (PROC)CreateFontAEx))
    {
        MessageBoxW(0, L"CreateFontA Hook Error。", L"IATHook Error", 0);
        return;
    }
    
    if (!IATPatch("Gdi32.dll", GetProcAddress(GetModuleHandleW(L"Gdi32.dll"), "CreateFontIndirectA"), (PROC)HookCreateFontIndirectA))
    {
        MessageBoxW(0, L"CreateFontIndirectA Hook Error。", L"IATHook Error", 0);
        return;
    }
    
}

void  InitHook()
{
    DetourTransactionBegin();
    //FileSystem Set
    DetourAttach((void**)&pGetFileName, GetFileName);
    DetourAttach((void**)&pGetFileBuff, GetFileBuff);
    DetourAttach((void**)&pGetFileBuff2, GetFileBuff2);
    DetourAttach((void**)&pGetFileSize, mGetFileSize);
    DetourAttach((void**)&pGetFileSize2, mGetFileSize2);
    DetourAttach((void**)&pAllocMemory, mAllocMemory);

    MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");
    DetourAttach((void**)&MessageBoxAOLD, MessageBoxAEx);
    DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
    //DetourAttach((void**)&pShowFileCompressModeNumber, ShowFileCompressModeNumber);//if you need to check compress mode, add on it.
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"ERROR", L"AymKze", MB_OK);
    }
    IATHook();
}


void strcpyA(char* old, const char* cpy)
{
    memset(old, 0, strlen(old));
    memcpy(old, cpy, strlen(cpy));
}

#define RELEASE
void __stdcall AyamiKazeShowTextA(DWORD type, char* text, DWORD len)
{
    char* dst = nullptr;
    if (type >= 0x10)
    {
        _asm
        {
            mov eax, dword ptr ds : [edi + 0xB4] ;
            mov dst, eax;
        }
#ifdef RELEASE
        CRC32 crc;
        wstring gbktext = ctow(dst);
        DWORD strcrc = crc.Calc((char*)gbktext.c_str(), gbktext.length() * 2);
        auto scitr = REPList.find(strcrc);
        if (scitr != REPList.end())
        {
            char* tmp = wtoc((scitr->second).c_str());
            strcpyA(dst, tmp);
            DWORD Len = lstrlenA(tmp) - 0x1;
            _asm
            {
                mov eax, Len;
                mov dword ptr ds : [edi + 0xC4] , eax;
            }
        }
        else 
        {
            cout << "GetStringType:0x" << hex << type << endl;
            cout << "strlen:0x" << hex << len << endl;
            cout << "miss text:" << dst << endl;
        }
#else
        cout << "----------------------------------------------------------------------------------" << endl;
        cout << "GetStringType:0x" << hex << type << endl;
        cout << "strlen:0x" << hex << len << endl;
        cout << "miss text:" << dst << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
#endif
    }
    else
    {
#ifdef RELEASE
        CRC32 crc;
        wstring gbktext = ctow(text);
        DWORD strcrc = crc.Calc((char*)gbktext.c_str(), gbktext.length() * 2);
        auto scitr = REPList.find(strcrc);
        if (scitr != REPList.end())
        {
            char* tmp = wtoc((scitr->second).c_str());
            strcpyA(text, tmp);
            DWORD Len = strlen(tmp) - 0x1;
            _asm
            {
                mov eax, Len;
                mov dword ptr ds : [edi + 0xC4] , eax;
            }
        }
        else
        {
            cout << "GetStringType:0x" << hex << type << endl;
            cout << "strlen:0x" << hex << len << endl;
            cout << "miss text:" << text << endl;
        }
#else
        cout << "----------------------------------------------------------------------------------" << endl;
        cout << "GetStringType:0x" << hex << type << endl;
        cout << "strlen:0x" << hex << len << endl;
        cout << "miss text:" << text << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
#endif
    }
}


static PVOID pGetText = (PVOID)(BaseAddr + 0x2024e0);
__declspec(naked) void mGetText()
{
    __asm
    {
        pushad;
        pushfd;
        
        push dword ptr ds : [edi + 0xC4] ;      //strlen
        lea esi, dword ptr ds : [edi + 0xB4] ;  //str
        mov ebx, dword ptr ds : [esi + 0x14] ;  //type
        push esi;
        push ebx;
        call AyamiKazeShowTextA

        popfd;
        popad;
        jmp pGetText;
    }
}

ofstream LOG("log.txt");

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
            //LOG << "0x" << hex << crcval << "|" << wtoc(newlist.at(index).c_str()) << endl;
        }
        cout << hex << pGetText << endl;
        DetourTransactionBegin();
        DetourAttach((void**)&pGetText, mGetText);
        if (DetourTransactionCommit() != NOERROR)
        {
            MessageBox(NULL, L"ERROR", L"AymKze", MB_OK);
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
        make_console();
        InitHook();
        InitText();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

