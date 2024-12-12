// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

void _stdcall AymShowText(char* text)
{
    cout << wtocGBK(ctowJIS(text)) << endl;
    //strcpy(text, wtocGBK(ctowJIS(text)));
    /*
    if (!wcscmp(ctowJIS(text),L"■注意事項"))
        strcpy(text,"■注意事项");
    else if (!wcscmp(ctowJIS(text), L"■プロローグ"))
        strcpy(text, "■序章");
    else if (!wcscmp(ctowJIS(text), L"【誠悟】"))
        strcpy(text, "【诚悟】");
    else if (!wcscmp(ctowJIS(text), L"「はぁ……はぁ……っ、はぁ……」"))
        strcpy(text, "「哈……哈……唔、哈……」");
    else if (!wcscmp(ctowJIS(text), L"ドクン、ドクン、と五月蝿いくらいの心臓の音がしていた。"))
        strcpy(text, "扑通、扑通，心跳的声音吵得烦人。");
    else if (!wcscmp(ctowJIS(text), L"まるで耳の真横に心臓があるかのように、早鐘を打ち続ける。"))
        strcpy(text, "就像心脏在耳边持续拉响警报。");
    else if (!wcscmp(ctowJIS(text), L"「くっそ！　まさか、しくじるなんてよぉ…」"))
        strcpy(text, "「可恶！难道我失策了吗…」");
    else if (!wcscmp(ctowJIS(text), L"己の慢心を一人ごちながら、教卓の裏側で息を潜めていた。"))
        strcpy(text, "我饱尝自大带来的后果，屏息藏入课桌底下。");
    else if (!wcscmp(ctowJIS(text), L"だが、先程から五月蝿く鳴り喚く心音。"))
        strcpy(text, "从刚才起，心脏就一直狂跳着。");
    else if (!wcscmp(ctowJIS(text), L"聞こえるはずもない、と思いながらも、何処かで聞こえてしまうのではないか、という不安が俺の中を掻き毟っていく。"))
        strcpy(text, "我一边想着他们应该听不见这心跳声，一边又担心他们真的能听见。");
        */


}
char vis[260];
DWORD retaddr = 0;
DWORD len = 0;
void _stdcall AymShowTextW(char* text)
{
    //cout << wtocGBK(ctowJIS(text)) << endl;
    //strcpy(text, wtocGBK(ctowJIS(text)));
    len = strlen(text);
    memcpy(vis, text, len);
    //vis[strlen(text) + 1] = 0x1b;
    //vis[strlen(text) + 2] = 0x03;
    
}

PVOID pGetText = (PVOID)(BaseAddr + 0xd3acc);
__declspec(naked)void mGetName()
{
    __asm
    {
        pushad
        pushfd
        push esi
        call AymShowText
        popfd
        popad
    }
}

PVOID pGetText3 = (PVOID)(BaseAddr + 0xc0bce);
__declspec(naked)void mGetName3()
{
    __asm
    {
        pushad
        pushfd
        push eax
        call AymShowText
        popfd
        popad
        jmp pGetText3
    }
}

PVOID pGetText2 = (PVOID)(BaseAddr + 0x12b982);
__declspec(naked)void mGetName2()
{
    __asm
    {
        //mov straddr, esi;

        pushad;
        pushfd;
        push EBX;
        call AymShowTextW;
        popfd;
        popad;

        cmp len, 0x00;
        je END;

        lea ebx, vis;
        sub ebx, edx;
        mov esi, ebx;
        lea ebx, vis;
        mov len, 0x00;

    END:
        jmp pGetText2
    }
}

void InlineHook()
{
    DetourTransactionBegin();
    //DetourAttach((void**)&pGetText, mGetName);
    //DetourAttach((void**)&pGetText3, mGetName3);

    DetourAttach((void**)&pGetText2, mGetName2);
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
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

