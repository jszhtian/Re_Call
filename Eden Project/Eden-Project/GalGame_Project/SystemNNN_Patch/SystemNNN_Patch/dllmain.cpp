// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
CHAR TextBox[0x1000] = { 0 };
unordered_map<DWORD, string> mesMap;
unordered_map<DWORD, string> strMap;

int(__fastcall* sub_425FA0)(DWORD* pThis, int null_ptr, int mesNum);
int __fastcall GetMessagePtr(DWORD* pThis, int null_ptr, int mesNum)
{
    /*
    int ret = pThis[0x242] + 4 * *(DWORD*)(pThis[0x242] + 4 * (mesNum + pThis[0x245]));
    cout << "mes: ";
    cout << dec << mesNum; //当前文本编号
    cout << " | 0x" << hex << 4 * *(DWORD*)(pThis[0x242] + 4 * (mesNum + pThis[0x245])); //当前文本offset
    cout << " | 0x" << hex << pThis[0x243]; // 当前脚本大小
    cout << " | " << wtoc(ctow((LPSTR)ret, 932), 936);
    cout << endl;
    */
    /*
    if (mesNum == 0)
    {
        //memset(TextBox, 0, 0x1000);
        lstrcpyA(TextBox, "？？？\r\n这是我的中文测试１２３ＡＢＣａｂｃ\r\n这是我的中文测试１２３ＡＢＣａｂｃ\r\n");
        return (int)&TextBox;
    }
    */
    for (auto it = mesMap.begin(); it != mesMap.end(); it++)
    {
        if (mesNum == it->first) 
        {
            lstrcpyA(TextBox, it->second.c_str());
            return (int)&TextBox;
        }
    }
    return sub_425FA0(pThis, null_ptr, mesNum);
}

int(__fastcall* sub_425FC0)(DWORD* pThis, int null_ptr, int strNum);
int __fastcall GetStringPtr(DWORD* pThis, int null_ptr, int strNum)
{
    /*
    int ret = pThis[0x242] + 4 * *(DWORD*)(pThis[0x242] + 4 * (strNum + pThis[0x246]));
    cout << "str: ";
    cout << dec << strNum; //当前文本编号
    cout << " | 0x" << hex << 4 * *(DWORD*)(pThis[0x242] + 4 * (strNum + pThis[0x246])); //当前文本offset
    cout << " | 0x" << hex << pThis[0x243]; // 当前脚本大小
    cout << " | " << wtoc(ctow((LPSTR)ret, 932), 936);
    cout << endl;
    */
    for (auto it = strMap.begin(); it != strMap.end(); it++)
    {
        if (strNum == it->first) 
        {
            lstrcpyA(TextBox, it->second.c_str());
            return (int)&TextBox;
        }
    }
    return sub_425FC0(pThis, null_ptr, strNum);
}

BOOL(__fastcall* sub_426000)(const char* pThis, int null_ptr, char* FileName);
BOOL __fastcall LoadScript(const char* pThis, int null_ptr, char* FileName)
{
    CHAR ReadScriptName[MAX_PATH] = { 0 };
    wsprintfA(ReadScriptName, "#Project\\%s.txt", FileName);
    ifstream fstm;
    fstm.open(ReadScriptName);
    if (fstm.is_open())
    {
        cout << ReadScriptName << endl;

        CHAR tmp[0x1000] = { 0 };
        vector<string> vec_str;

        mesMap.clear();
        strMap.clear();

        while (true)
        {
            fstm.getline(tmp, 0x1000);
            int outNum = fstm.gcount();
            //cout << outNum << endl;
            if (outNum == 0)
                break;
            vec_str.push_back(string(tmp));
        }
        for (int i = 0; i < vec_str.size(); i++)
        {
            string line = vec_str.at(i);
            if (line.size() > 0)
            {
                string header = line.substr(0, 2);
                //cout << header << endl;
                if (header == "cm")
                {
                    string num = line.substr(3, 4);
                    DWORD mesNum = stoi(num.c_str());

                    string str = line.substr(8);
                    string c_str = ReplaceString(str);
                    c_str = wtoc(ctow((CHAR*)c_str.c_str(), CP_UTF8), 936);
                    //cout << mesNum << "|" << c_str << endl;
                    mesMap.emplace(mesNum, c_str);
                }
                else if (header == "cs")
                {
                    string num = line.substr(3, 4);
                    DWORD strNum = stoi(num.c_str());

                    string str = line.substr(8);
                    string c_str = ReplaceString(str);
                    c_str = wtoc(ctow((CHAR*)c_str.c_str(), CP_UTF8), 936);
                    //cout << strNum << "|" << c_str << endl;
                    strMap.emplace(strNum, c_str);
                }
            }
        }
    }
    fstm.close();
    return sub_426000(pThis, null_ptr, FileName);
}


BOOL(__fastcall* sub_409900)(int pThis, int null_ptr, LPSTR filename, BOOL b256Flag, int a4);
BOOL __fastcall LoadDWQ(int pThis, int null_ptr, LPSTR filename, BOOL b256Flag, int a4)
{
    int m_pic = pThis + 8 * 4;
    BOOL ret = sub_409900(pThis, null_ptr, filename, b256Flag, a4);
    if (ret) 
    {
        string PicFileName = "#Project\\dwq\\" + string(filename) + ".png";

        DWORD m_maskFlag = *(DWORD*)(pThis + 92);
        //cout << m_maskFlag << endl;

        FILE* fp = fopen(PicFileName.c_str(), "rb");
        if (fp)
        {
            //cout << PicFileName << endl;
            fseek(fp, 0, SEEK_END);
            DWORD size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            BYTE* buff = new BYTE[size];
            fread(buff, size, 1, fp);
            fclose(fp);

            int width, height, bit_depth;
            NakedMemory dib;
            if (!ReadPngToBmp(buff, size, &width, &height, &bit_depth, dib, 0))
            {
                delete[] buff;
                return ret;
            }
            delete[] buff;
            if (bit_depth == 24)
            {
                dib = Dib24To32(width, height, dib);
            }
            BMP_TO_DIB((PBYTE)dib.Get(), width, height, 32);

            //cout << "start copy" << endl;
            memcpy((BYTE*)*(int*)m_pic, dib.Get(), width * height * 4);
            //cout << "end" << endl;

            if (m_maskFlag == 1 && bit_depth == 32)
            {
                int m_maskPic = pThis + 40;
                //cout << hex << m_maskPic << endl;
                dib = Dib2Mask(width, height, dib);
                //cout << "start copy 2" << endl;
                memcpy((BYTE*)*(int*)m_maskPic, dib.Get(), width * height);
                //cout << "end 2" << endl;
            }
        }
    }
    return ret;
}

void InlineHook()
{
    *(DWORD*)&sub_425FA0 = BaseAddr + 0x25FA0;
    *(DWORD*)&sub_425FC0 = BaseAddr + 0x25FC0;

    *(DWORD*)&sub_426000 = BaseAddr + 0x26000;
    /* 方法1&2
    (DWORD*)&sub_40BB00 = BaseAddr + 0xBB00;
    */
    *(DWORD*)&sub_409900 = BaseAddr + 0x9900;
    DetourTransactionBegin();

    // Text
    DetourAttach((void**)&sub_426000, LoadScript);
    DetourAttach((void**)&sub_425FA0, GetMessagePtr);
    DetourAttach((void**)&sub_425FC0, GetStringPtr);

    // Pic
    /* 方法1&2
    DetourAttach((void**)&sub_40BB00, OpenDWQFile);
    DetourAttach(&pGetDibAddr, GetDibAddr);
    DetourAttach(&pDoCopy, DoCopy);

    DetourAttach(&pGetDibAddr2, GetDibAddr2);
    DetourAttach(&pDoCopy2, DoCopy2);
    */
    // 方法3
    //DetourAttach((void**)&sub_4090D0, ReadDWQHeader);
    DetourAttach((void**)&sub_409900, LoadDWQ);

    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBox(NULL, L"InlinePatch Error.", L"InlinePatch", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}

PVOID g_pOldCreateFontA = CreateFontA;
typedef HFONT(WINAPI* PfuncCreateFontA)(int nHeight,
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
    LPCTSTR lpszFace);
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
    LPCTSTR lpszFace)
{
    return CreateFontW(nHeight,
        nWidth,
        nEscapement,
        nOrientation,
        fnWeight,
        fdwltalic,
        fdwUnderline,
        fdwStrikeOut,
        GB2312_CHARSET,
        fdwOutputPrecision,
        fdwClipPrecision,
        fdwQuality,
        fdwPitchAndFamily,
        L"SimHei");
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
HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    const char* szWndName = "【穗见学园汉化组】梦幻廻廊 - v1.0 [BuildTime: 20220705]";
    return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void Init()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontA, HookCreateFontA);
    DetourAttach(&g_pOldCreateWindowExA, HookCreateWindowExA);
    DetourTransactionCommit();
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
        InlineHook();
        Init();
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