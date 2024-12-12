// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

DWORD ImmAlloc = (BaseAddr + 0x18C910);
DWORD ImmFree = (BaseAddr + 0x18BADC);

__declspec(naked) PVOID CDECL HostAlloc(LONG Size)
{
    _asm
    {
        push dword ptr[esp + 4]
        call ImmAlloc
        pop ecx
        retn
    }
}


__declspec(naked) VOID CDECL HostFree(PVOID lpMem)
{
    _asm
    {
        push dword ptr[esp + 4]
        call ImmFree
        pop ecx
        retn
    }
}

WCHAR FilePath[0x1000] = { 0 };
wstring g_FileName;
#define READ

int(__cdecl* loadFile)(CHAR* FileName, void** FileBuff, size_t* FileSize, unsigned int* a4);
void* (__cdecl* loadImage)(BYTE* FileBuff, int FileSize);
#ifdef DUMP
int __cdecl HookLoadFile(CHAR* FileName, void** FileBuff, size_t* FileSize, unsigned int* a4)
{
    int ret = loadFile(FileName, FileBuff, FileSize, a4);
    //cout << FileName << endl;

    wstring fnm_w = wstring(ctow(FileName, 932));
    if (fnm_w.substr(fnm_w.length() - 3) == L"tlg" || fnm_w.substr(fnm_w.length() - 3) == L"tga")
    {
        wstring outName = FilePath + wstring(L"\\#output\\") + fnm_w;
        FixPath(outName);

        wstring outDir = GetDirectoryName(outName);
        if (!outDir.empty())
        {
            //cout << wtoc(outDir.c_str(), 936) << endl;
            SHCreateDirectory(NULL, outDir.c_str());
        }

        cout << "----------------------------------------------------" << endl;
        cout << "Dump:" << wtoc(outName.c_str(), 936) << endl;
        cout << "FileSize:0x" << hex << *FileSize << endl;

        FILE* fp = _wfopen(outName.c_str(), L"wb");
        if (fp)
        {
            fwrite(*FileBuff, *FileSize, 1, fp);
            fclose(fp);
            cout << "done" << endl;
        }
        else
            cout << "error to create file" << endl;
    }

    return ret;
}
#else
/*
int __cdecl HookLoadFile(CHAR* FileName, void** FileBuff, size_t* FileSize, unsigned int* a4)
{
    int ret = loadFile(FileName, FileBuff, FileSize, a4);
    //cout << FileName << endl;

    wstring fnm_w = wstring(ctow(FileName, 932));
    if (ret == 1 && fnm_w.substr(fnm_w.length() - 3) == L"tlg")
    {
        wstring outName = FilePath + wstring(L"\\#output\\") + fnm_w;
        FixPath(outName);

        FILE* fp = _wfopen(outName.c_str(), L"rb");
        if (fp)
        {
            cout << "Read:" << wtoc(outName.c_str(), 936) << endl;

            fseek(fp, 0, SEEK_END);
            DWORD FSize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            BYTE* FBuff = (BYTE*)HostAlloc(FSize);
            fread(FBuff, 1, FSize, fp);
            fclose(fp);

            *FileBuff = FBuff;
            *FileSize = FSize;
            //*a4 = 0x043a06;

            cout << "done" << endl;
        }
    }

    return ret;
}
*/

int __cdecl HookLoadFile(CHAR* FileName, void** FileBuff, size_t* FileSize, unsigned int* a4)
{
    int ret = loadFile(FileName, FileBuff, FileSize, a4);
    //cout << FileName << endl;

    wstring fnm_w = wstring(ctow(FileName, 932));
    if (ret == 1 && (fnm_w.substr(fnm_w.length() - 3) == L"tlg" || fnm_w.substr(fnm_w.length() - 3) == L"tga"))
    {
        wstring outName = FilePath + wstring(L"\\#output\\") + fnm_w;
        FixPath(outName);

        //cout << wtoc(outName.c_str(), 936) << endl;
        g_FileName = outName;
    }

    return ret;
}

void* __cdecl HookLoadImage(BYTE* FileBuff, int FileSize)
{
    if (g_FileName.size() != 0)
    {
        FILE* fp = _wfopen(g_FileName.c_str(), L"rb");
        if (fp)
        {
            cout << "Read:" << wtoc(g_FileName.c_str(), 936) << endl;

            fseek(fp, 0, SEEK_END);
            DWORD FSize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            BYTE* FBuff = (BYTE*)HostAlloc(FSize);
            fread(FBuff, 1, FSize, fp);
            fclose(fp);

            void* ret = loadImage(FBuff, FSize);

            //cout << "done" << endl;
            g_FileName.clear();

            return ret;
        }
        else
            return loadImage(FileBuff, FileSize);
    }
    else
        return loadImage(FileBuff, FileSize);
}

#endif

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
        L"黑体");
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    strcpy(lplf->lfFaceName, "黑体");

    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

ULONG __stdcall ansi_to_unicode(ULONG code)
{
    static UCHAR ch[3] = { 0 };
    static wchar_t wch = 0;

    if (code & 0xFF00) //sjis or gbk
    {
        ch[0] = (code >> 8) & 0xFF;
        ch[1] = code & 0xFF;
        ch[2] = '\0';
    }
    else // ascii
    {
        ch[0] = code & 0xFF;
        ch[1] = '\0';
    }

    MultiByteToWideChar(936, 0, (char*)ch, -1, &wch, 2);

    return wch;
}

void* pstart_sjis_to_utf16 = (void*)(BaseAddr + 0x570F2);
void* pend_sjis_to_utf16 = (void*)(BaseAddr + 0x5710E);
ULONG wch;
__declspec(naked) void __stdcall _ansi_to_unicode()
{
    __asm
    {
        pushad
        push eax
        call ansi_to_unicode
        mov wch, eax
        popad
        mov eax, wch
        jmp pend_sjis_to_utf16
    }

}

const char* MenuText[] =
{
    "启动设置", // 起動設定
    "显示模式", //スクリーンモード 
    "窗口", // ウィンドウ
    "全屏", // フルスクリーン
    "16 : 9", // 16:9
    "4 : 3", // 4:3
    "分辨率", // スクリーン解像度
    "窗口大小适应屏幕分辨率。", // ウィンドウサイズをスクリーン解像度に合わせる
    "请注意，高分辨率要求PC具有比通常更高的性能。\nDBD(Dot By Dot)是在显示器上的一个点上显示图像的一个像素。", // 高解像度ではＰＣに通常より高いスペックが求められますので、予めご注意下さい。DBD(Dot By Dot)はディスプレイの１ドットに画像の１ピクセルを表示する事です。
    "高级设置", // 詳細設定
    "启动", // 起動
    "结束", // 終了
    "下次启动显示此对话框", // 次回起動時に起動設定画面を表示する

    "画面", // 画面
    "纵横比 16:9", // アスペクト比 16:9
    "纵横比 4:3", // アスペクト比  4:3
    "窗口大小", // ウィンドウサイズ
    "窗口位置", // ウィンドウ位置
    "窗口居中", // 画面の中央
    "保持坐标", // 座標を保持
    "窗口保持在最前", // 常に最前面で表示
    "全屏模式", // フルスクリーンモード
    "画面缩放插值模式", // 画面拡大縮小補間
    "垂直同步", // 垂直同期待ち
    "禁用", // 同期無効
    "固定大小(", // サイズ固定(
    "＋", // ＋
    "×", // ×
    "(等倍)", // (等倍)
};

void PatchText()
{
    WriteMemory((PVOID)(BaseAddr + 0x1BC444), (PVOID)MenuText[0], strlen(MenuText[0]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC02C), (PVOID)MenuText[1], strlen(MenuText[1]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC0D4), (PVOID)MenuText[1], strlen(MenuText[1]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC0C8), (PVOID)MenuText[2], strlen(MenuText[2]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC0B8), (PVOID)MenuText[3], strlen(MenuText[3]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC3DC), (PVOID)MenuText[4], strlen(MenuText[4]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC3D0), (PVOID)MenuText[5], strlen(MenuText[5]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC3BC), (PVOID)MenuText[6], strlen(MenuText[6]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC384), (PVOID)MenuText[7], strlen(MenuText[7]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC2E8), (PVOID)MenuText[8], strlen(MenuText[8]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC2DC), (PVOID)MenuText[9], strlen(MenuText[9]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC2D4), (PVOID)MenuText[10], strlen(MenuText[10]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC2CC), (PVOID)MenuText[11], strlen(MenuText[11]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC2A8), (PVOID)MenuText[12], strlen(MenuText[12]) + 1);

    WriteMemory((PVOID)(BaseAddr + 0x1BC0E8), (PVOID)MenuText[13], strlen(MenuText[13]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC0D4), (PVOID)MenuText[1], strlen(MenuText[1]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC0C8), (PVOID)MenuText[2], strlen(MenuText[2]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC0B8), (PVOID)MenuText[3], strlen(MenuText[3]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC0A4), (PVOID)MenuText[14], strlen(MenuText[14]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC090), (PVOID)MenuText[15], strlen(MenuText[15]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC07C), (PVOID)MenuText[16], strlen(MenuText[16]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC06C), (PVOID)MenuText[17], strlen(MenuText[17]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC060), (PVOID)MenuText[18], strlen(MenuText[18]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC054), (PVOID)MenuText[19], strlen(MenuText[19]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC040), (PVOID)MenuText[20], strlen(MenuText[20]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC028), (PVOID)MenuText[21], strlen(MenuText[21]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BBFE8), (PVOID)MenuText[22], strlen(MenuText[22]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BBF7C), (PVOID)MenuText[23], strlen(MenuText[23]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BBF70), (PVOID)MenuText[24], strlen(MenuText[24]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BBF44), (PVOID)MenuText[25], strlen(MenuText[25]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BBF50), (PVOID)MenuText[26], strlen(MenuText[26]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BBF54), (PVOID)MenuText[27], strlen(MenuText[27]) + 1);
    WriteMemory((PVOID)(BaseAddr + 0x1BC154), (PVOID)MenuText[28], strlen(MenuText[28]) + 1);


}

void Init()
{
    GetCurrentDirectoryW(0x1000, FilePath);

    *(DWORD*)&loadFile = BaseAddr + 0xBEFC0;
    Hook(loadFile, HookLoadFile);

#ifdef READ
    *(DWORD*)&loadImage = BaseAddr + 0xAECE0;
    Hook(loadImage, HookLoadImage);

    // API

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&pstart_sjis_to_utf16, _ansi_to_unicode);

    DetourAttach(&g_pOldCreateFontA, HookCreateFontA);
    DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
    if (DetourTransactionCommit() != NOERROR)
    {
        E(L"API Hook Error");
        ExitProcess(-1);
    }
#endif

    PatchText();
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

