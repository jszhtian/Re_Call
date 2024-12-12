#include "pch.h"
#include <string>
#include <iostream>
#include <windowsx.h>
#include <synchapi.h>
static HHOOK hhk = NULL;
#define MOUSEEVENTF_FROMTOUCHPEN 0xFF515700
LRESULT CALLBACK keyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION && ((DWORD)lParam & 0x80000000) == 0)
    {
        auto msg = (MSG*)lParam;
        switch (msg->message)
        {
        case WM_LBUTTONDOWN:
            if ((GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCHPEN) == MOUSEEVENTF_FROMTOUCHPEN)
            {
                INPUT minput[2];
                ZeroMemory(&minput, sizeof(INPUT) * 2);
                minput[0].type = INPUT_MOUSE;
                minput[0].mi.dx = GET_X_LPARAM(msg->lParam);
                minput[0].mi.dy = GET_Y_LPARAM(msg->lParam);
                minput[0].mi.mouseData = 0;
                minput[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

                minput[1].type = INPUT_MOUSE;
                minput[1].mi.dx = GET_X_LPARAM(msg->lParam);
                minput[1].mi.dy = GET_Y_LPARAM(msg->lParam);
                minput[1].mi.mouseData = 0;
                minput[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

                UINT uRet = SendInput(1, &minput[0], sizeof(INPUT));
                Sleep(100);
                uRet = SendInput(1, &minput[1], sizeof(INPUT));
                return 1;
            }
            else
            {
                break;
            }
        case WM_LBUTTONUP:
            if ((GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCHPEN) == MOUSEEVENTF_FROMTOUCHPEN)
            {
                return 1;
            }
        case WM_RBUTTONDOWN:
            if ((GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCHPEN) == MOUSEEVENTF_FROMTOUCHPEN)
            {
                INPUT minput[2];
                ZeroMemory(&minput, sizeof(INPUT) * 2);
                minput[0].type = INPUT_MOUSE;
                minput[0].mi.dx = GET_X_LPARAM(msg->lParam);
                minput[0].mi.dy = GET_Y_LPARAM(msg->lParam);
                minput[0].mi.mouseData = 0;
                minput[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

                minput[1].type = INPUT_MOUSE;
                minput[1].mi.dx = GET_X_LPARAM(msg->lParam);
                minput[1].mi.dy = GET_Y_LPARAM(msg->lParam);
                minput[1].mi.mouseData = 0;
                minput[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

                UINT uRet = SendInput(1, &minput[0], sizeof(INPUT));
                Sleep(100);
                uRet = SendInput(1, &minput[1], sizeof(INPUT));
                return 1;
            }
            else
            {
                break;
            }
        case WM_RBUTTONUP:
            if ((GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCHPEN) == MOUSEEVENTF_FROMTOUCHPEN)
            {
                return 1;
            }
        default:
            break;
        }
    }
    return CallNextHookEx(hhk, code, wParam, lParam);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        hhk = SetWindowsHookEx(WH_GETMESSAGE, keyboardProc, hModule, GetCurrentThreadId());
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        UnhookWindowsHookEx(hhk);
    }
    return TRUE;
}