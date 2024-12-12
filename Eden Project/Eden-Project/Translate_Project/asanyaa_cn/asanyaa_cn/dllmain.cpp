// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#define DEBUG_MODE
DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);
unordered_map<unsigned int, wstring>TextList;

/******************************* Install Patch *******************************/
TCHAR szBuffer[MAX_PATH] = { 0 };

void __stdcall SetDirPath(void* eax)
{
	string strOldName(wtoc(szBuffer, CP_ACP));
	string strDirName = strOldName.substr(0, strOldName.find_last_of("\\") + 1);
	memset(eax, 0, strlen(strDirName.c_str()) + 0x1);
	memcpy(eax, strDirName.c_str(), strlen(strDirName.c_str()));
}

PVOID Hook1 = NULL;
__declspec(naked) void ChangePath()
{
	_asm
	{
		pushad
		pushfd
		push eax
		call SetDirPath
		popfd
		popad
		jmp Hook1
	}
}

BOOL WINAPI BypassInstallPatch()
{
	GetModuleFileName((HMODULE)BaseAddr, szBuffer, sizeof(szBuffer) / sizeof(TCHAR) - 1);
	if (!szBuffer)
	{
		MessageBox(0, L"Can't Get FileName.", L"BypassInstallPatch", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	Hook1 = (PVOID)(BaseAddr + 0x5AA16);
	DetourTransactionBegin();
	DetourAttach((void**)&Hook1, ChangePath);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBox(NULL, L"Can't Set FileName.", L"BypassInstallPatch", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

/******************************* API Patch *******************************/
PVOID g_pOldSetWindowTextW = SetWindowTextW;
typedef bool (WINAPI* PfuncSetWindowTextW)(HWND hWnd, LPCWSTR lpString);
bool WINAPI HookSetWindowTextW(HWND hw, LPCWSTR lps)
{
	wchar_t newtitle[] = L"【鸽稽汉化组×早喵汉化组】对早上突然变成美少女的我有什么请求？真没办法 喵……可以哦 - v1.0";
	return ((PfuncSetWindowTextW)g_pOldSetWindowTextW)(hw, newtitle);
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef int (WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
int WINAPI HookCreateFontIndirectA(LOGFONTA* lplf)
{

	strcpy(lplf->lfFaceName, "黑体");
	lplf->lfWeight = FW_SEMIBOLD;
	lplf->lfCharSet = GB2312_CHARSET;

	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

BOOL WINAPI APIPatch()
{
	DetourTransactionBegin();
	DetourAttach(&g_pOldSetWindowTextW, HookSetWindowTextW);
	DetourAttach(&g_pOldCreateFontIndirectA, HookCreateFontIndirectA);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBox(NULL, L"APIHook Error.", L"APIHook", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

/******************************* Inline Patch *******************************/
void* (__fastcall *sub_444900)(char* a0, char* NotUse, void* a2, int a3);
void(__fastcall *sub_472930)(void* a1, int NotUse);
void* (__fastcall *sub_472990)(void* a1, int NotUse, LPCWSTR lpString);
int(__fastcall *sub_472C00)(int a1, int NotUse, int a2, int a3, void* Src, int a5, int a6);

wchar_t text[0x1000] = { 0 };
void* __fastcall GetTextByID(char* a0, char* NotUse, void* a2, int ID)
{
	int nullptr_1 = NULL;
	int nullptr_2 = NULL;
	int nullptr_3 = NULL;

	sub_472930(a2, nullptr_1);
	DWORD* v4 = (DWORD*)*((DWORD*)a0 + 15);
	DWORD* v5 = (DWORD*)*((DWORD*)a0 + 14);

	int v11[4];
	if (v5 != v4)
	{
		while (*v5 != ID)
		{
			v5 += 3;
			if (v5 == v4)
				return a2;
		}
		if (v5 != v4)
		{
#ifdef DEBUG_MODE
			cout << "ID:" << dec << ID;
#endif
			auto itr = TextList.find(ID);
			if (itr != TextList.end())
			{
				DWORD len = lstrlenW(itr->second.c_str());
				lstrcpyW(text, itr->second.c_str());
				sub_472990(v11, nullptr_2, text);
				sub_472C00((int)a2, nullptr_2, v11[0], v11[1], (void*)v11[2], v11[3], len);
#ifdef DEBUG_MODE
				cout << " copied" << endl;
#endif
			}
#ifdef DEBUG_MODE
			else
			{
				cout << " not found" << endl;
				return sub_444900(a0, NotUse, a2, ID);
			}
#endif
			/*
			if (ID == 1)
			{
				char* text = (char*)",　要跳过体验版的内容吗？中文测试测试测试超长测试测试测试";
				wchar_t* w_text = ctow(text, CP_ACP);
				sub_472990(v11, nullptr_2, w_text);
				sub_472C00((int)a2, nullptr_2, v11[0], v11[1], (void*)v11[2], v11[3], lstrlenW(w_text));
			}
			else
				return sub_444900(a0, NotUse, a2, ID);
			*/

		}
	}
	return a2;
}


/*
void* __fastcall GetTextByID(char* a0, char* NotUse, void* a2, int ID)
{
	void* ret = sub_444900(a0, NotUse, a2, ID);
#ifdef DEBUG_MODE
	cout << "ID:" << ID;
#endif
	auto itr = TextList.find(ID);
	if (itr != TextList.end())
	{
		DWORD len = lstrlenW(itr->second.c_str());
		wchar_t* text = new wchar_t[len];
		lstrcpyW(text, itr->second.c_str());
		*(DWORD*)((DWORD)ret + 8) = (DWORD)text;
		*(DWORD*)((DWORD)ret + 12) = (DWORD)(len / 2);
#ifdef DEBUG_MODE
		cout << " copied" << endl;
#endif
	}
#ifdef DEBUG_MODE
	else
		cout << endl;
#endif

	return ret;
}
*/

BOOL(__fastcall* sub_475E70)(int a1, int null, LPCSTR lpFileName);
int(__fastcall* sub_476780)(int a1, int null);
int(__fastcall* sub_476150)(int a1, int null, LPVOID lpBuffer, DWORD nNumberOfBytesToRead);

BOOL __fastcall CFPK::FPK_Open(int a1, int null, LPCSTR lpFileName)
{
	if (CFPK::fp)
		fclose(CFPK::fp);

	sprintf(CFPK::fnm, "_asanyaa_cn\\%s", lpFileName);
	CFPK::fp = fopen(CFPK::fnm, "rb");

	if (!CFPK::fp)
		return sub_475E70(a1, null, lpFileName);
#ifdef DEBUG_MODE
	cout << "ReadFile:" << CFPK::fnm << endl;
#endif
	return TRUE;
}

int __fastcall CFPK::FPK_GetFileSize(int a1, int null)
{
	if (CFPK::fp)
	{
		fseek(CFPK::fp, 0, SEEK_END);
		DWORD size = ftell(CFPK::fp);
		fseek(CFPK::fp, 0, SEEK_SET);
		return size;
	}
	return sub_476780(a1, null);
}

int __fastcall CFPK::FPK_Read(int a1, int null, LPVOID lpBuffer, DWORD nNumberOfBytesToRead)
{
	if (CFPK::fp)
	{
		fread(lpBuffer, nNumberOfBytesToRead, 1, CFPK::fp);
		return nNumberOfBytesToRead;
	}
	return sub_476150(a1, null, lpBuffer, nNumberOfBytesToRead);
}

BOOL WINAPI InlinePatch()
{
	//CFPK* FileFunc;
	// Text
	*(DWORD*)&sub_444900 = BaseAddr + 0x44900;
	*(DWORD*)&sub_472930 = BaseAddr + 0x72930;
	*(DWORD*)&sub_472990 = BaseAddr + 0x72990;
	*(DWORD*)&sub_472C00 = BaseAddr + 0x72C00;
	// File
	*(DWORD*)&sub_475E70 = BaseAddr + 0x75E70;
	*(DWORD*)&sub_476780 = BaseAddr + 0x76780;
	*(DWORD*)&sub_476150 = BaseAddr + 0x76150;
#ifdef DEBUG_MODE
	cout << "sub_444900:0x" << hex << (BaseAddr + 0x44900) << endl;
	cout << "sub_472930:0x" << hex << (BaseAddr + 0x72930) << endl;
	cout << "sub_472990:0x" << hex << (BaseAddr + 0x72990) << endl;
	cout << "sub_472C00:0x" << hex << (BaseAddr + 0x72C00) << endl;

	cout << "sub_475E70:0x" << hex << (BaseAddr + 0x75E70) << endl;
	cout << "sub_476780:0x" << hex << (BaseAddr + 0x76780) << endl;
	cout << "sub_476150:0x" << hex << (BaseAddr + 0x76150) << endl;
#endif
	DetourTransactionBegin();
	// Text
	DetourAttach((void**)&sub_444900, GetTextByID);
	// File
	DetourAttach((void**)&sub_475E70, CFPK::FPK_Open);
	DetourAttach((void**)&sub_476780, CFPK::FPK_GetFileSize);
	DetourAttach((void**)&sub_476150, CFPK::FPK_Read);
	if (DetourTransactionCommit() != NO_ERROR)
	{
		MessageBox(NULL, L"InlinePatch Error.", L"InlinePatch", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

/******************************* Global File Init *******************************/


unsigned int GetOffsetNum(wstring line)
{
	for (unsigned int c = 0; c < line.size(); c++)
	{
		if (line[c] == L'|')
			return c;
	}
	return 0;
}

BOOL InitTranslateText()
{
	UINT cnt = 0;
	ifstream txt("Script.txt");
	if (txt.is_open())
	{
		const int LineMax = 0x1000;
		char str[LineMax];
		while (txt.getline(str, LineMax))
		{
			if (str[0] == '0')
			{
				auto wtmp = ctow(str, CP_UTF8);
				wstring wline = wtmp;
				wstring off_str = wline.substr(0, 6);
				wstring text_str = wline.substr(6 + 1);
				unsigned int off = wcstoul(off_str.c_str(), NULL, 10);
				//cout << off << "||" << wtoc(text_str.c_str(), CP_ACP) << endl;
				//cout << wtoc(off_str.c_str(), CP_ACP) << "||" << wtoc(text_str.c_str(), CP_ACP) << endl;
				TextList.insert(pair<unsigned int, wstring>(off, text_str));
				cnt++;
			}
		}
		txt.close();
	}
	else
	{
		MessageBox(0, L"miss Script.txt.", L"InitTranslateText", MB_OK | MB_ICONERROR);
		return FALSE;
	}
#ifdef DEBUG_MODE
	cout << "LineCount:" << dec << cnt << endl;
#endif
	return TRUE;
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
		if (
			!BypassInstallPatch() || 
			!APIPatch() || 
			!InlinePatch() ||
			!InitTranslateText()
			)
			ExitProcess(-1);
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