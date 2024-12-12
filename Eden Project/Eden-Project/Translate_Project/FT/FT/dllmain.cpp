// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

void __stdcall AymShowTextA(char* Text)
{
    char* text = wtocGBK(ctowJIS(Text));
    //cout << text << endl;
    if (!strcmp(text, "閉じる"))
        strcpy(Text, "关闭");
    if (!strcmp(text, "アリスとの思い出を辿りますか？"))
        strcpy(Text, "要追溯与爱丽丝的回忆吗？");
    if (!strcmp(text, "グレーテルとの思い出を辿りますか？"))
        strcpy(Text, "要追溯与葛雷特的回忆吗？");
    if (!strcmp(text, "オデット、オディールの思い出を辿りますか？"))
        strcpy(Text, "要追溯与奥杰塔，奥吉莉亚的回忆吗？");
    if (!strcmp(text, "ラプンツェルとの思い出を辿りますか？"))
        strcpy(Text, "要追溯与莴苣的回忆吗？");
    if (!strcmp(text, "ゲルダとの思い出を辿りますか？"))
        strcpy(Text, "要追溯与格尔达的回忆吗？");
    if (!strcmp(text, "はい"))
        strcpy(Text, "是");
    if (!strcmp(text, "いいえ"))
        strcpy(Text, "否");
}
/*○0027○閉じる
●0027●关闭

○0028○アリスとの思い出を辿りますか？
●0028●要追溯与爱丽丝的回忆吗？

○0031○グレーテルとの思い出を辿りますか？
●0031●要追溯与葛雷特的回忆吗？

○0034○オデット，オディールの思い出を辿りますか？
●0034●要追溯与奥杰塔，奥吉莉亚的回忆吗？

○0037○ラプンツェルとの思い出を辿りますか？
●0037●要追溯与莴苣的回忆吗？

○0040○ゲルダとの思い出を辿りますか？
●0040●要追溯与格尔达的回忆吗？
はい
いいえ
*/
PVOID pGetText = (PVOID)(BaseAddr + 0x4C52B);
__declspec(naked)void mGetText()
{
    __asm
    {
        pushad
        pushfd
        push eax
        call AymShowTextA
        popfd
        popad
        jmp pGetText
    }
}

void InlineHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&pGetText, mGetText);
    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBoxW(0, L"InlineHook Hook Error。", L"InlineHook Error", 0);
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
        //make_console();
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
