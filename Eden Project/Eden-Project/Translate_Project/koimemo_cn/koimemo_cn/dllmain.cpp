// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
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

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocUTF(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

LPWSTR ctowGBK(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(936, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(936, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(CP_ACP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

LPWSTR ctow(char* str)
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

void Error(const char* Text)
{
	MessageBoxA(0, Text, "Aym", 0);
}

char* g_xml = NULL;
DWORD g_xml_size = NULL;

bool AymFile(char* buffer, DWORD size, const char* FileName, const char* mode)
{
	if (strcmp(mode, "rb") == 0)
	{
		FILE* fp = fopen(FileName, "rb");
		if (!fp)
		{
			Error("Can't find File.");
			return false;
		}
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buffer = (char*)malloc(size);
		if (!buffer || !size)
		{
			Error("No Buffer or Size.");
			return false;
		}
		fread(buffer, size, 1, fp);
		fclose(fp);
		g_xml_size = size;
		g_xml = buffer;
		return true;
	}
	else if (strcmp(mode, "wb") == 0)
	{
		FILE* fp = fopen(FileName, "wb");
		if (!buffer || !size)
		{
			Error("No Buffer or Size.");
			return false;
		}
		fwrite(buffer, size, 1, fp);
		fclose(fp);
		return true;
	}
	else
	{
		Error("No mode suppost.");
		return false;
	}

}

PVOID g_pOldSetWindowTextW = NULL;
typedef int (WINAPI* PfuncSetWindowTextW)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextW(HWND hwnd, LPWSTR lpString)
{
	lstrcpy(lpString, L"【伊甸学院-6th】此花绽放季，与君重逢时 - v1.0");
	return ((PfuncSetWindowTextW)g_pOldSetWindowTextW)(hwnd, lpString);
}


/***********************************************************************/


void AymReplaceTextW(wchar_t* Text1, wchar_t* Text2, wchar_t* Text3)
{
	if (lstrcmp(Text1, Text2) == 0)
		lstrcpy(Text1, Text3);
}
void __stdcall GetText(wchar_t* Text)
{
	DWORD len = lstrlen(Text);
	if (Text[0] <= 0x40)
		return;
	//cout << wtoc(Text) << endl;
	//TXT << "------------------------------------------------" << endl;
	//TXT << wtoc(Text) << endl;
	AymReplaceTextW(Text, (wchar_t*)L"終了します。\nよろしいですか？", (wchar_t*)L"即将结束游戏，\n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 000 を上書きしますか？", (wchar_t*)L"即将覆盖No. 000 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 001 を上書きしますか？", (wchar_t*)L"即将覆盖No. 001 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 002 を上書きしますか？", (wchar_t*)L"即将覆盖No. 002 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 003 を上書きしますか？", (wchar_t*)L"即将覆盖No. 003 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 004 を上書きしますか？", (wchar_t*)L"即将覆盖No. 004 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 005 を上書きしますか？", (wchar_t*)L"即将覆盖No. 005 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 006 を上書きしますか？", (wchar_t*)L"即将覆盖No. 006 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 007 を上書きしますか？", (wchar_t*)L"即将覆盖No. 007 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 008 を上書きしますか？", (wchar_t*)L"即将覆盖No. 008 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 009 を上書きしますか？", (wchar_t*)L"即将覆盖No. 009 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 010 を上書きしますか？", (wchar_t*)L"即将覆盖No. 010 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 011 を上書きしますか？", (wchar_t*)L"即将覆盖No. 011 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 012 を上書きしますか？", (wchar_t*)L"即将覆盖No. 012 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 013 を上書きしますか？", (wchar_t*)L"即将覆盖No. 013 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 014 を上書きしますか？", (wchar_t*)L"即将覆盖No. 014 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 015 を上書きしますか？", (wchar_t*)L"即将覆盖No. 015 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 016 を上書きしますか？", (wchar_t*)L"即将覆盖No. 016 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 017 を上書きしますか？", (wchar_t*)L"即将覆盖No. 017 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 018 を上書きしますか？", (wchar_t*)L"即将覆盖No. 018 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 019 を上書きしますか？", (wchar_t*)L"即将覆盖No. 019 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 020 を上書きしますか？", (wchar_t*)L"即将覆盖No. 020 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 021 を上書きしますか？", (wchar_t*)L"即将覆盖No. 021 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 022 を上書きしますか？", (wchar_t*)L"即将覆盖No. 022 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 023 を上書きしますか？", (wchar_t*)L"即将覆盖No. 023 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 024 を上書きしますか？", (wchar_t*)L"即将覆盖No. 024 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 025 を上書きしますか？", (wchar_t*)L"即将覆盖No. 025 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 026 を上書きしますか？", (wchar_t*)L"即将覆盖No. 026 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 027 を上書きしますか？", (wchar_t*)L"即将覆盖No. 027 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 028 を上書きしますか？", (wchar_t*)L"即将覆盖No. 028 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 029 を上書きしますか？", (wchar_t*)L"即将覆盖No. 029 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 030 を上書きしますか？", (wchar_t*)L"即将覆盖No. 030 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 031 を上書きしますか？", (wchar_t*)L"即将覆盖No. 031 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 032 を上書きしますか？", (wchar_t*)L"即将覆盖No. 032 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 033 を上書きしますか？", (wchar_t*)L"即将覆盖No. 033 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 034 を上書きしますか？", (wchar_t*)L"即将覆盖No. 034 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 035 を上書きしますか？", (wchar_t*)L"即将覆盖No. 035 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 036 を上書きしますか？", (wchar_t*)L"即将覆盖No. 036 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 037 を上書きしますか？", (wchar_t*)L"即将覆盖No. 037 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 038 を上書きしますか？", (wchar_t*)L"即将覆盖No. 038 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 039 を上書きしますか？", (wchar_t*)L"即将覆盖No. 039 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 040 を上書きしますか？", (wchar_t*)L"即将覆盖No. 040 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 041 を上書きしますか？", (wchar_t*)L"即将覆盖No. 041 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 042 を上書きしますか？", (wchar_t*)L"即将覆盖No. 042 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 043 を上書きしますか？", (wchar_t*)L"即将覆盖No. 043 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 044 を上書きしますか？", (wchar_t*)L"即将覆盖No. 044 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 045 を上書きしますか？", (wchar_t*)L"即将覆盖No. 045 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 046 を上書きしますか？", (wchar_t*)L"即将覆盖No. 046 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 047 を上書きしますか？", (wchar_t*)L"即将覆盖No. 047 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 048 を上書きしますか？", (wchar_t*)L"即将覆盖No. 048 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 049 を上書きしますか？", (wchar_t*)L"即将覆盖No. 049 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 050 を上書きしますか？", (wchar_t*)L"即将覆盖No. 050 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 051 を上書きしますか？", (wchar_t*)L"即将覆盖No. 051 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 052 を上書きしますか？", (wchar_t*)L"即将覆盖No. 052 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 053 を上書きしますか？", (wchar_t*)L"即将覆盖No. 053 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 054 を上書きしますか？", (wchar_t*)L"即将覆盖No. 054 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 055 を上書きしますか？", (wchar_t*)L"即将覆盖No. 055 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 056 を上書きしますか？", (wchar_t*)L"即将覆盖No. 056 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 057 を上書きしますか？", (wchar_t*)L"即将覆盖No. 057 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 058 を上書きしますか？", (wchar_t*)L"即将覆盖No. 058 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 059 を上書きしますか？", (wchar_t*)L"即将覆盖No. 059 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 060 を上書きしますか？", (wchar_t*)L"即将覆盖No. 060 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 061 を上書きしますか？", (wchar_t*)L"即将覆盖No. 061 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 062 を上書きしますか？", (wchar_t*)L"即将覆盖No. 062 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 063 を上書きしますか？", (wchar_t*)L"即将覆盖No. 063 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 064 を上書きしますか？", (wchar_t*)L"即将覆盖No. 064 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 065 を上書きしますか？", (wchar_t*)L"即将覆盖No. 065 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 066 を上書きしますか？", (wchar_t*)L"即将覆盖No. 066 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 067 を上書きしますか？", (wchar_t*)L"即将覆盖No. 067 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 068 を上書きしますか？", (wchar_t*)L"即将覆盖No. 068 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 069 を上書きしますか？", (wchar_t*)L"即将覆盖No. 069 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 070 を上書きしますか？", (wchar_t*)L"即将覆盖No. 070 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 071 を上書きしますか？", (wchar_t*)L"即将覆盖No. 071 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 072 を上書きしますか？", (wchar_t*)L"即将覆盖No. 072 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 073 を上書きしますか？", (wchar_t*)L"即将覆盖No. 073 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 074 を上書きしますか？", (wchar_t*)L"即将覆盖No. 074 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 075 を上書きしますか？", (wchar_t*)L"即将覆盖No. 075 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 076 を上書きしますか？", (wchar_t*)L"即将覆盖No. 076 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 077 を上書きしますか？", (wchar_t*)L"即将覆盖No. 077 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 078 を上書きしますか？", (wchar_t*)L"即将覆盖No. 078 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 079 を上書きしますか？", (wchar_t*)L"即将覆盖No. 079 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 080 を上書きしますか？", (wchar_t*)L"即将覆盖No. 080 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 081 を上書きしますか？", (wchar_t*)L"即将覆盖No. 081 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 082 を上書きしますか？", (wchar_t*)L"即将覆盖No. 082 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 083 を上書きしますか？", (wchar_t*)L"即将覆盖No. 083 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 084 を上書きしますか？", (wchar_t*)L"即将覆盖No. 084 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 085 を上書きしますか？", (wchar_t*)L"即将覆盖No. 085 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 086 を上書きしますか？", (wchar_t*)L"即将覆盖No. 086 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 087 を上書きしますか？", (wchar_t*)L"即将覆盖No. 087 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 088 を上書きしますか？", (wchar_t*)L"即将覆盖No. 088 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 089 を上書きしますか？", (wchar_t*)L"即将覆盖No. 089 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 090 を上書きしますか？", (wchar_t*)L"即将覆盖No. 090 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 091 を上書きしますか？", (wchar_t*)L"即将覆盖No. 091 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 092 を上書きしますか？", (wchar_t*)L"即将覆盖No. 092 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 093 を上書きしますか？", (wchar_t*)L"即将覆盖No. 093 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 094 を上書きしますか？", (wchar_t*)L"即将覆盖No. 094 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 095 を上書きしますか？", (wchar_t*)L"即将覆盖No. 095 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 096 を上書きしますか？", (wchar_t*)L"即将覆盖No. 096 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 097 を上書きしますか？", (wchar_t*)L"即将覆盖No. 097 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 098 を上書きしますか？", (wchar_t*)L"即将覆盖No. 098 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"No. 099 を上書きしますか？", (wchar_t*)L"即将覆盖No. 099 \n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"コメントを入力して下さい。", (wchar_t*)L"请输入注释。");
	AymReplaceTextW(Text, (wchar_t*)L"セーブしました。", (wchar_t*)L"存档完成。");
	AymReplaceTextW(Text, (wchar_t*)L"クイックロードしますか？", (wchar_t*)L"要快速读档吗？");
	AymReplaceTextW(Text, (wchar_t*)L"次のシーンへ進みます。\nよろしいでしょうか？", (wchar_t*)L"即将跳转到下一事件，\n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"クイックジャンプしますか？", (wchar_t*)L"要跳转到指定位置吗？");
	AymReplaceTextW(Text, (wchar_t*)L"タイトルに戻ります。\nよろしいですか？", (wchar_t*)L"即将返回标题，\n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"オートセーブデータを初期化します。\nよろしいでしょうか？", (wchar_t*)L"即将重置自动存档数据，\n确认吗？");
	AymReplaceTextW(Text, (wchar_t*)L"クイックセーブデータを初期化します。\nよろしいでしょうか？", (wchar_t*)L"即将重置快速存档数据，\n确认吗？");
}

void __stdcall AymGetFileName(wchar_t* FileName)
{
	if (lstrcmp(FileName, L".\\data2.dat") == 0)
		lstrcpy(FileName, L".\\sonohana");
}

void __stdcall AymReplaceXml(wchar_t* Text)
{
	cout << "SavePath1 set." << endl;
	if (lstrcmp(Text, L"Campus\\その花が咲いたら、また僕は君に出逢う") == 0)
	{
		lstrcpy(Text, L"Campus\\此花绽放季，与君重逢时");
	}
}

void __stdcall AymReplaceXml2(wchar_t* Text)
{//00A8C580  朰媝幏帠偲楒偡傞偍忟條偺夞憐榐...............


	cout << "SavePath2 set." << endl;
	if (lstrcmp(Text, L"偦偺壴偑嶇偄偨傜丄傑偨杔偼孨偵弌埀偆") == 0)
	{
		lstrcpy(Text, L"此花绽放季，与君重逢时");
	}
}

void __stdcall AymGetMenuText(wchar_t* Text)
{
	//cout << wtoc(Text) << endl;
	//TXT << wtoc(Text) << endl;
	AymReplaceTextW(Text, (wchar_t*)L"クイックセーブ	F9", (wchar_t*)L"快速存档　　F9");
	AymReplaceTextW(Text, (wchar_t*)L"クイックロード	F10", (wchar_t*)L"快速读档　　F10");
	AymReplaceTextW(Text, (wchar_t*)L"クイックセーブデータ(&Q)", (wchar_t*)L"快速存档数据(&Q)");
	AymReplaceTextW(Text, (wchar_t*)L"オートセーブデータ(&A)", (wchar_t*)L"自动存档数据(&A)");
	AymReplaceTextW(Text, (wchar_t*)L"タイトルに戻る(&T)	T", (wchar_t*)L"返回标题画面(&T)	T");
	AymReplaceTextW(Text, (wchar_t*)L"画面切り替え(&W)	Alt+Enter", (wchar_t*)L"切换显示模式(&W)	Alt+Enter");
	AymReplaceTextW(Text, (wchar_t*)L"終了(&X)	Alt+F4", (wchar_t*)L"结束(&X)	Alt+F4");
	AymReplaceTextW(Text, (wchar_t*)L"ファイル(&F)", (wchar_t*)L"文件(&F)");
	AymReplaceTextW(Text, (wchar_t*)L"オートセーブ", (wchar_t*)L"自动存档");
	AymReplaceTextW(Text, (wchar_t*)L"ボイス再生時のセリフ表示", (wchar_t*)L"播放语音时显示对白");
	AymReplaceTextW(Text, (wchar_t*)L"オートモード", (wchar_t*)L"自动模式");
	AymReplaceTextW(Text, (wchar_t*)L"既読部分全て", (wchar_t*)L"已读部分");
	AymReplaceTextW(Text, (wchar_t*)L"全て", (wchar_t*)L"全部");
	AymReplaceTextW(Text, (wchar_t*)L"スキップ", (wchar_t*)L"快进");
	AymReplaceTextW(Text, (wchar_t*)L"クイックセーブデータの初期化", (wchar_t*)L"重置快速存档数据");
	AymReplaceTextW(Text, (wchar_t*)L"オートセーブデータの初期化", (wchar_t*)L"重置自动存档数据");
	AymReplaceTextW(Text, (wchar_t*)L"設定(&C)", (wchar_t*)L"设置(&C)");
	AymReplaceTextW(Text, (wchar_t*)L"バージョン情報(&V)...", (wchar_t*)L"版本信息(&V)...");
	AymReplaceTextW(Text, (wchar_t*)L"ヘルプ(&H)", (wchar_t*)L"帮助(&H)");
}

PVOID pGetName = NULL;
PVOID pGetFileName = NULL;//(PVOID)(0x10021C4D);
__declspec(naked)void GetName()
{
	__asm
	{
		pushad
		pushfd
		push dword ptr ss : [ebp + 0x10]
		call GetText
		popfd
		popad
		jmp pGetName
	}
}
__declspec(naked)void GetFileName()
{
	__asm
	{
		pushad
		pushfd
		push eax
		call AymGetFileName
		popfd
		popad
		jmp pGetFileName
	}
}
PVOID pGetXmlBuff = NULL;
PVOID pReplaceXmlBuff = NULL;
PVOID pGetMenuText = NULL;
PVOID pGetMenuText2 = NULL;
PVOID pGetNameText = NULL;
PVOID pGetNameText2 = NULL;
__declspec(naked)void GetXmlBuff()
{
	__asm
	{
		//MOV xmlbuff, 0xA8C580
		pushad
		pushfd
		push 0xA8C580
		call AymReplaceXml2
		popfd
		popad
		jmp pGetXmlBuff
	}
}

__declspec(naked)void ReplaceXmlBuff()
{
	__asm
	{
		pushad
		pushfd
		push eax
		call AymReplaceXml
		popfd
		popad
		jmp pReplaceXmlBuff
	}
}

__declspec(naked)void GetMenuText()
{
	__asm
	{
		pushad
		pushfd
		push eax
		call AymGetMenuText
		popfd
		popad
		jmp pGetMenuText
	}
}

__declspec(naked)void GetMenuText2()
{
	__asm
	{
		pushad
		pushfd
		push eax
		call AymGetMenuText
		popfd
		popad
		jmp pGetMenuText2
	}
}

void __stdcall AymGetNameText(wchar_t* Text)
{
	if (lstrlen(Text) == 0)
		return;
	//cout << wtoc(name) << endl;
	AymReplaceTextW(Text, (wchar_t*)L"雛乃", (wchar_t*)L"雏乃");
	AymReplaceTextW(Text, (wchar_t*)L"運転手", (wchar_t*)L"司机");
	AymReplaceTextW(Text, (wchar_t*)L"大家さん", (wchar_t*)L"房东");
	AymReplaceTextW(Text, (wchar_t*)L"メイド長", (wchar_t*)L"女仆长");
	AymReplaceTextW(Text, (wchar_t*)L"メイド", (wchar_t*)L"女仆");
	AymReplaceTextW(Text, (wchar_t*)L"シェフ", (wchar_t*)L"厨师长");
	AymReplaceTextW(Text, (wchar_t*)L"使用人Ａ", (wchar_t*)L"佣人Ａ");
	AymReplaceTextW(Text, (wchar_t*)L"使用人Ｂ", (wchar_t*)L"佣人Ｂ");
	AymReplaceTextW(Text, (wchar_t*)L"来賓", (wchar_t*)L"来宾");
	AymReplaceTextW(Text, (wchar_t*)L"酔っ払い", (wchar_t*)L"醉汉");
	AymReplaceTextW(Text, (wchar_t*)L"旦那様", (wchar_t*)L"老爷");
	AymReplaceTextW(Text, (wchar_t*)L"おばさんＡ", (wchar_t*)L"大婶Ａ");
	AymReplaceTextW(Text, (wchar_t*)L"おばさんＢ", (wchar_t*)L"大婶Ｂ");
	AymReplaceTextW(Text, (wchar_t*)L"来客", (wchar_t*)L"客人");
	AymReplaceTextW(Text, (wchar_t*)L"二人", (wchar_t*)L"两人");
	AymReplaceTextW(Text, (wchar_t*)L"店員", (wchar_t*)L"店员");
	AymReplaceTextW(Text, (wchar_t*)L"豊橋", (wchar_t*)L"丰桥");
}

__declspec(naked)void GetNameText()
{
	__asm
	{
		pushad
		pushfd
		push dword ptr ds : [eax + 0x26C]
		call AymGetNameText
		popfd
		popad
		jmp pGetNameText
	}
}

__declspec(naked)void GetNameText2()
{
	__asm
	{
		pushad
		pushfd
		push dword ptr ds : [ecx]
		call AymGetNameText
		popfd
		popad
		jmp pGetNameText2
	}
}
void Hook()
{
	DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
	pGetName = (PVOID)(BaseAddr + 0x2369FB);
	pGetFileName = (PVOID)(BaseAddr + 0x28AFB6);
	pGetXmlBuff = (PVOID)(BaseAddr + 0x234AB8);
	pReplaceXmlBuff = (PVOID)(BaseAddr + 0x2347D4);
	pGetMenuText = (PVOID)(BaseAddr + 0xEE836);
	pGetMenuText2 = (PVOID)(BaseAddr + 0xEE789);
	pGetNameText = (PVOID)(BaseAddr + 0x21CCA9);
	pGetNameText2 = (PVOID)(BaseAddr + 0x252E27);
	//pGetName = (PVOID)(BaseAddr + 0x261e3a);
	//pGetTextSize = (PVOID)(BaseAddr + 0x276311);
	//pGetText = (PVOID)(BaseAddr + 0x27631D);
	cout << "BaseAddress:0x" << hex << (PVOID)BaseAddr << endl;
	cout << "pGetName:0x" << hex << pGetName << endl;
	cout << "pGetFileName:0x" << hex << pGetFileName << endl;
	cout << "pGetXmlBuff:0x" << hex << pGetXmlBuff << endl;
	cout << "pReplaceXmlBuff:0x" << hex << pGetXmlBuff << endl;
	cout << "pGetMenuText:0x" << hex << pGetXmlBuff << endl;
	cout << "pGetMenuText2:0x" << hex << pGetXmlBuff << endl;
	g_pOldSetWindowTextW = DetourFindFunction("USER32.dll", "SetWindowTextW");
	DetourTransactionBegin();
	DetourAttach((void**)&pGetName, GetName);
	DetourAttach((void**)&pGetFileName, GetFileName);
	DetourAttach((void**)&pGetXmlBuff, GetXmlBuff);
	DetourAttach((void**)&pReplaceXmlBuff, ReplaceXmlBuff);
	DetourAttach((void**)&pGetMenuText, GetMenuText);
	DetourAttach((void**)&pGetMenuText2, GetMenuText2);
	//DetourAttach((void**)&pGetNameText, GetNameText);
	//DetourAttach((void**)&pGetNameText2, GetNameText2);

	DetourAttach(&g_pOldSetWindowTextW, NewSetWindowTextW);

	if(DetourTransactionCommit()!=NOERROR)
	{ 
		MessageBox(NULL, L"ERROR", L"AymKze", MB_OK);
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
		Hook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI Dummy()
{
}