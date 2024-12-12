// dllmain.cpp : 定义 DLL 应用程序的入口点。
#define RELEASE_MODE

#include "framework.h"

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    //cout << lplf->lfFaceName << endl;
    //if (!lstrcmpA(lplf->lfFaceName, "俵俽 僑僔僢僋"))
    lstrcpyA(lplf->lfFaceName, "SimHei");
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID g_pOldSetWindowTextA = SetWindowTextA;
typedef bool (WINAPI* PfuncSetWindowTextA)(HWND hWnd, LPCSTR lpString);
bool WINAPI HookSetWindowTextA(HWND hw, LPCSTR lpString)
{
    //cout << lpString << endl;
    if (lstrcmpA(lpString, "僌儕僓僀傾偺柪媨"))
        return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hw, lpString);
    wchar_t newtitle[] = L"【一姬应援会】灰色的迷宫 - v1.0 （禁止一切录播直播和商业行为）";
    return SetWindowTextW(hw, newtitle);
}

PVOID g_pOldMessageBoxA = MessageBoxA;
typedef int (WINAPI* PfuncMessageboxA)(
    _In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType
    );
int WINAPI HookMessageBoxA(
    _In_opt_ HWND    hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_     UINT    uType)
{
    wchar_t* lpText_W = ctow((char*)lpText, 932);
    wchar_t* lpCaption_W = ctow((char*)lpCaption, 932);
    //cout << wtoc(lpText_W, 936) << endl;
    if (!lstrcmpW(lpText_W, L"グリザイアの迷宮 を終了してよろしいですか？"))
        return MessageBoxW(hWnd, L"确认结束游戏吗？", L"提示", uType);
    return MessageBoxW(hWnd, lpText_W, lpCaption_W, uType);
}

PVOID g_pOldGetGlyphOutlineA = GetGlyphOutlineA;
typedef int(WINAPI* PfuncGetGlyphOutlineA)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2);
int WINAPI HookGetGlyphOutlineA(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
    //cout << hex << uChar << endl;
    if (uChar == '亂')
        uChar = '【';
    else if (uChar == '亃')
        uChar = '】';
    else if (uChar == '⑨')
    {
        LOGFONTW* lplf = (LOGFONTW*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(LOGFONTW));

        HFONT hOldFont = (HFONT)GetCurrentObject(hdc, OBJ_FONT);
        GetObject(hOldFont, sizeof(LOGFONTW), lplf);
        LOGFONTW Info = { 0 };

        Info.lfHeight = lplf->lfHeight;
        Info.lfWidth = lplf->lfWidth;
        Info.lfEscapement = lplf->lfEscapement;
        Info.lfOrientation = lplf->lfOrientation;
        Info.lfWeight = lplf->lfWeight;
        Info.lfItalic = lplf->lfItalic;
        Info.lfUnderline = lplf->lfUnderline;
        Info.lfStrikeOut = lplf->lfStrikeOut;
        Info.lfOutPrecision = lplf->lfOutPrecision;
        Info.lfClipPrecision = lplf->lfClipPrecision;
        Info.lfQuality = lplf->lfQuality;
        Info.lfPitchAndFamily = lplf->lfPitchAndFamily;
        lstrcpyW(Info.lfFaceName, L"MS Gothic");
        lplf->lfCharSet = SHIFTJIS_CHARSET;

        HFONT hFont = CreateFontIndirectW(&Info);

        hOldFont = (HFONT)SelectObject(hdc, hFont);
        //6A 26 
        DWORD Result = GetGlyphOutlineW(hdc, (UINT)0x266A, fuFormat,
            lpgm, cjBuffer, pvBuffer, lpmat2);

        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        HeapFree(GetProcessHeap(), 0, lplf);

        return Result;
    }
    return ((PfuncGetGlyphOutlineA)g_pOldGetGlyphOutlineA)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}


void WINAPI APIHook()
{
    DetourTransactionBegin();
    DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Hook CreateFontIndirectA Error", L"APIHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }

    DetourTransactionBegin();
    DetourAttach(&g_pOldSetWindowTextA, HookSetWindowTextA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Hook SetWindowTextA Error", L"APIHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }

    DetourTransactionBegin();
    DetourAttach(&g_pOldMessageBoxA, HookMessageBoxA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Hook MessageBoxA Error", L"APIHook", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }

    DetourTransactionBegin();
    DetourAttach(&g_pOldGetGlyphOutlineA, HookGetGlyphOutlineA);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"Hook GetGlyphOutlineA error", L"InitAPI", MB_OK | MB_ICONERROR);
        ExitProcess(-1);
    }
}

void WINAPI PrintMsg(UINT Width, UINT Height)
{
    cout << dec << Width << " | " << Height << endl;
}

#define IMAGE_CRC_INIT (0L)

char szHG3Name[MAX_PATH] = { 0 };
unsigned int szHG3IndexName = NULL;

bool WINAPI ReadPngFile(const std::string& strName, int nOffset, PNG_DATA* pPngData, int* chn)
{
    if (!pPngData || strName.empty())
    {
        return false;
    }

    FILE* fPng = fopen(strName.c_str(), "rb");
    if (!fPng)
    {
        return false;
    }

    fseek(fPng, nOffset, SEEK_SET);

    if (!PngFile::ReadPngFile(fPng, pPngData, chn))
    {
        fclose(fPng);
        if (pPngData->pRgba)
        {
            free(pPngData->pRgba);
        }

        MessageBoxA(NULL, strName.c_str(), "PNG Error", MB_OK);
        return false;
    }

    fclose(fPng);
    return true;
}

PNG_DATA oPngData = { 0 };

void WINAPI ReplaceImage(BYTE* pBuffer, unsigned int nWidth, unsigned int nHeight, char* FileName)
{
    if (!pBuffer)
    {
        return;
    }
    int chn = 0;
    cout << "start" << endl;
    if (ReadPngFile(FileName, 0, &oPngData, &chn) && oPngData.pRgba)
    {
        if (chn == 0)
            return;
        cout << chn << endl;
        unsigned int nImageSize = nWidth * nHeight * chn;

        //memcpy(pBuffer, oPngData.pRgba, nImageSize);
        free(oPngData.pRgba);
        cout << "copied" << endl;
        memset(&oPngData, 0, sizeof(PNG_DATA));
    }
}

void WriteImage(BYTE* pBuffer, unsigned int nWidth, unsigned int nHeight)
{
    unsigned int nImageSize = nWidth * nHeight * 4;

    PNG_DATA pic = { 0 };
    pic.eFlag = HAVE_ALPHA;
    pic.nWidth = nWidth;
    pic.nHeight = nHeight;
    pic.pRgba = pBuffer;

    unsigned int nCrc = crc32(IMAGE_CRC_INIT, pBuffer, nImageSize);
    char szImageName[MAX_PATH] = { 0 };
    sprintf(szImageName, "image_cn\\%08X.png", nCrc);
    std::string strName(szImageName);
    PngFile::WritePngFile(strName.c_str(), &pic);
}

void BMP_TO_DIB(PBYTE data, int width, int height, int BitCount)
{
    BYTE* TempBuffer = nullptr;
    int i = 0;
    int widthlen = 0;
    int nAlignWidth = 0;
    size_t BufferSize = 0;
    nAlignWidth = (width * 32 + 31) / 32;
    BufferSize = 4 * nAlignWidth * height;
    TempBuffer = (BYTE*)malloc(BufferSize);

    //反转图片,修正图片信息
    widthlen = width * (BitCount / 8); //对齐宽度大小
    for (i = 0; i < height; i++) {
        memcpy(&TempBuffer[(((height - i) - 1) * widthlen)], &data[widthlen * i], widthlen);
    }

    memcpy(data, TempBuffer, BufferSize);

    free(TempBuffer);
}

char FileName[MAX_PATH] = { 0 };
void WINAPI ProcessImage(BYTE* pBuffer, unsigned int nWidth, unsigned int nHeight)
{
    if (lstrlenA(szHG3Name) != 0)
    {
#ifdef DEBUG_MODE
        cout << "-----------------------------Get image info-----------------------------" << endl;
#endif
        wsprintfA(FileName, "_pic\\%s\\%s_%d_000.png", szHG3Name, szHG3Name, szHG3IndexName);
#ifdef DEBUG_MODE
        cout << FileName << endl;
        cout << "Width:" << nWidth << endl;
        cout << "Height:" << nHeight << endl;
#endif
        szHG3IndexName = NULL;
        //return;
        //WriteImage(pBuffer, nWidth, nHeight);
        //ReplaceImage(pBuffer, nWidth, nHeight, FileName);

        ///*
        FILE* fp = nullptr;
        auto err = fopen_s(&fp, FileName, "rb");
        if (!fp)
            return;
        fseek(fp, 0, SEEK_END);
        DWORD size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        BYTE* buff = new BYTE[size];
        fread(buff, size, 1, fp);
        fclose(fp);

        int width, height, bit_depth;
        NakedMemory dib;
        if (!ReadPngToBmp(buff, size, &width, &height, &bit_depth, dib))
        {
            delete[] buff;
            return;
        }
        delete[] buff;

        if (width != nWidth || height != nHeight)
        {
            //MessageBox(NULL, L"图片信息不匹配", L"Info", MB_OK | MB_ICONERROR);
#ifdef DEBUG_MODE
            cout << "警告！图片信息不匹配" << endl;
            cout << "should:" << endl;
            cout << "\twidth:" << dec << nWidth << endl;
            cout << "\theight:" << dec << nHeight << endl;
            cout << "but:" << endl;
            cout << "\twidth:" << dec << width << endl;
            cout << "\theight:" << dec << height << endl;
            cout << "此错误可能不准确，请根据实际情况来判断。" << endl;
#endif
            return;
        }

        BYTE* Output = NULL;
        ULONG OutputLength;
#ifdef DEBUG_MODE
        cout << "bit_depth:" << bit_depth << endl;
#endif
        /*
        if (bit_depth == 24)
        {
            MessageBox(NULL, L"bit_depth = 24", L"bit_depth", MB_OK | MB_ICONERROR);
            return;
        }
        */
        BMP_TO_DIB((PBYTE)dib.Get(), width, height, bit_depth);

        memcpy(pBuffer, dib.Get(), dib.GetSize());
#ifdef DEBUG_MODE
        cout << "draw fin" << endl;
        cout << "-----------------------------Get image info End-----------------------------" << endl;
#endif
        memset(FileName, 0, MAX_PATH);
    }
}

void WINAPI pGetHG3Name(char* buffer)
{
    //cout << "Load1:" << buffer << endl;
    lstrcpyA(szHG3Name, buffer);
}

void WINAPI pGetHG3Name2(char* buffer)
{
    ///*
    string fnm = string(buffer);
    if (fnm.substr(fnm.length() - 3) == "hg3")
    {
        fnm = fnm.substr(0, fnm.length() - 4);
        //cout << fnm << endl;
        //memset(szHG3Name, 0, MAX_PATH);
        //memcpy(szHG3Name, fnm.c_str(), lstrlenA(fnm.c_str()));
        //cout << "Load2:" << fnm << endl;
        lstrcpyA(szHG3Name, fnm.c_str());
    }
    //*/
}

//DWORD tmp = 0;
void* g_GetHG3Name = (void*)(BaseAddr + 0x41E5B);
void __declspec(naked) GetHG3Name()
{
    __asm
    {
        pushad;
        pushfd;
        push esi;
        call pGetHG3Name;
        popfd;
        popad;
        jmp g_GetHG3Name;
    }
}

void* g_GetHG3Name2 = (void*)(BaseAddr + 0x1060A7);
void __declspec(naked) GetHG3Name2()
{
    __asm
    {
        pushad;
        pushfd;
        push ecx;
        call pGetHG3Name2;
        popfd;
        popad;
        jmp g_GetHG3Name2;
    }
}
void* g_GetHG3IndexName = (void*)(BaseAddr + 0x141832);
void __declspec(naked) GetHG3IndexName()
{
    __asm
    {
        mov edx, dword ptr ss : [esp + 0xE4]
        mov szHG3IndexName, edx
        jmp g_GetHG3IndexName
    }
}

void* g_p_copy_image = (void*)(BaseAddr + 0x141e4e);
void __declspec(naked) copy_image()
{
    __asm
    {
        pushad
        pushfd

        mov eax, dword ptr[esp + 0x38]
        mov	ebx, dword ptr[eax + 0x4]  //ebx = height
        mov eax, dword ptr[esp + 0x78]
        add edx, eax  //ecx = buffer
        shr eax, 2  //eax = width

        push ebx;
        push eax;
        push edx
        call ProcessImage

        popfd
        popad
        jmp g_p_copy_image
    }
}

void InlineHoook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&g_p_copy_image, copy_image);
    DetourAttach(&g_GetHG3IndexName, GetHG3IndexName);
    //DetourAttach(&g_GetHG3Name, GetHG3Name);
    DetourAttach(&g_GetHG3Name2, GetHG3Name2);

    if (DetourTransactionCommit() != NO_ERROR)
    {
        MessageBoxW(NULL, L"Hook Error.", L"InlineHoook", MB_OK | MB_ICONERROR);
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
#ifdef DEBUG_MODE
        make_console();
#endif
        APIHook();
        InlineHoook();
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