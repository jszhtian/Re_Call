#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include "detours.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <mutex>
#include <vector>
#include <functional>
#pragma comment(lib, "detours.lib")
using namespace std;
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
//ofstream TXT("Text.txt");

typedef enum _SIGNATURE
{
    TO_VOL = L'▲',
    EOVOL = L'△',
    STRUB = L'▼',
    EOSTR = L'▽',
    EOPAR = L'◁',
    NXL = L'◆',
    TO_RTN = L'◎',
    TO_RUB = L'★',
    NAME_SIG = L'※',
    UNKNOW_SIG = L'⊙',
    CH_HJN = L'∴',
};

static const WORD Jindai2Kana[0x004A] = {
      0xFF1F, 0x30A2, 0x30A4, 0x30A6, 0x30A8, 0x30AA, 0x30AB, 0x30AD,
      /*?!  A   I   U   E   O   KA  KI*/
      0x30AF, 0x30B1, 0x30B3, 0x30B5, 0x30B7, 0x30B9, 0x30BB, 0x30BD,
      /*KU  KE  KO  SA  SHI SU  SE  SO*/
      0x30BF, 0x30C1, 0x30C4, 0x30C6, 0x30C8, 0x30CA, 0x30CB, 0x30CC,
      /*TA  CHI TSU TE  TO  NA  NI  NU*/
      0x30CD, 0x30CE, 0x30CF, 0x30D2, 0x30D5, 0x30D8, 0x30DB, 0x30DE,
      /*NE  NO  HA  HI  FU  HE  HO  MA*/
      0x30DF, 0x30E0, 0x30E1, 0x30E2, 0x30E4, 0x30E6, 0x30E8, 0x30E9,
      /*MI  MU  ME  MO  YA  YU  YO  RA*/
      0x30EA, 0x30EB, 0x30EC, 0x30ED, 0x30EF, 0x30F2, 0x30F3, 0x30AC,
      /*RI  RU  RE  RO  WA  WO  N   GA*/
      0x30AE, 0x30B0, 0x30B2, 0x30B4, 0x30B6, 0x30B8, 0x30BA, 0x30BC,
      /*GI  GU  GE  GO  ZA  JI  ZU  ZE*/
      0x30BE, 0x30C0, 0x30C2, 0x30C5, 0x30C7, 0x30C9, 0x30D0, 0x30D3,
      /*ZO  DA  DJI DZU DE  DO  BA  BI*/
      0x30D6, 0x30D9, 0x30DC, 0x30D1, 0x30D4, 0x30D7, 0x30DA, 0x30DD,
      /*BU  BE  BO  PA  PI  PU  PE  PO*/
      0x30F4, 0xFF01
      /*VU  !!*/
};

class MalieString
{
public:
	MalieString(const std::wstring& str) :str_(str) {}
	bool init();
	//std::vector<std::wstring> export_str();
	//std::wstring import_str(std::function<std::wstring(void)> get_str);

private:
	enum class State {
		Init,
		Voice,
		Ruby,
		Text,
	};

	typedef std::pair<int, int> StrPos;

private:
	std::wstring str_;
	std::vector<StrPos> str_pos_;
};

static char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static LPWSTR ctowGBK(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(936, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(936, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

static LPWSTR ctow(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_ACP, 0, str, -1, out, dwMinSize);//转换
	return out;
}

static void make_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Console\n" << endl;
}

static void Error(const char* Text)
{
	MessageBoxA(0, Text, "Aym", 0);
}

bool MalieString::init() {
    State state = State::Init;
    int line_start = -1;
    for (uint32_t idx = 0; idx < str_.size(); ++idx) {
        bool state_changed = true;
        int lastIdx = idx;
        switch (str_[idx]) {
        case 0:
            if (state == State::Ruby) {
                state_changed = false;
            }
            else {
                state = State::Init;
            }
            break;
        case 1:
            state = State::Init;
            idx += 4;
            break;
        case 2:
            state = State::Init;
            ++idx;
            break;
        case 3:
            state = State::Init;
            idx += 2;
            break;
        case 4:
            state = State::Init;
            ++idx;
            break;
        case 5:
            state = State::Init;
            idx += 2;
            break;
        case 6:
            state = State::Init;
            idx += 2;
            break;
        case 0xA:
            if (state == State::Ruby) {
                state_changed = false;
            }
            else {
                state = State::Init;
            }
            break;
        case 7:
            switch (str_[++idx]) {
            case 1://递归调用文字读取，然后继续处理（包含注释的文字）
                if (state == State::Init) {
                    //如果行首就是ruby的话，因为先改掉了state会导致下面代码不认为这是行首，所以需要先设置好
                    //真特么蛋疼，这半吊子的状态机写得
                    line_start = idx - 1;
                }
                state = State::Ruby;
                state_changed = false;
                break;
            case 4://下一句自动出来
                state = State::Init;
                break;
            case 6://代表本句结束
                state = State::Init;
                break;
            case 7://递归调用文字读取然后wcslen，跳过不处理。应该是用于注释
                state = State::Init;
                ++idx;
                idx += wcslen(&str_[idx]);
                break;
            case 8://LoadVoice 后面是Voice名
                state = State::Voice;
                break;
            case 9://LoadVoice结束
                state = State::Init;
                break;
            default:
                state = State::Init;
                break;
            }
            break;
        default:
            if (state != State::Text && state != State::Ruby) {
                state = State::Text;
                line_start = idx;
            }
            else {
                state_changed = false;
            }
        }
        if (state_changed && line_start != -1 && line_start != idx) {
            str_pos_.push_back(StrPos(line_start, lastIdx));
            line_start = -1;
        }
    }
    return true;
}