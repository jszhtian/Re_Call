#include"pch.h"
#include <windows.h>
#include <D3D9.h>

#include "ilhook.h"
#include "FuncHelper.h"
#include "enc.h"
#include "entis.h"

using namespace std;

void HOOKFUNC MyCFI(LPLOGFONTA lfi)
{
	lfi->lfCharSet = 0x86;
    strcpy(lfi->lfFaceName, "SimHei");
    lfi->lfHeight *= 0.65;
}

BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
    if (_Reason == DLL_PROCESS_ATTACH)
    {
        static const HookPointStruct points[] = 
        {
            { nullptr, 0x3f5374, on_dec_ready, (char*)"a", 0, 0 },
            { nullptr, 0x3f538f, on_dec_complete, (char*)"r", 0, 0 }
        };

        if (!HookFunctions(points))
        {
            MessageBox(0, L"Hook 失败！", 0, 0);
        }
        static const HookPointStructWithName points2[] =
        {{ (char*)"gdi32.dll", (char*)"CreateFontIndirectA", MyCFI, (char*)"1", 0, 0 }};
        if (!HookFunctions(points2))
        {
            MessageBox(0, L"Hook2 失败！", 0, 0);
        }
    }
    return TRUE;
}