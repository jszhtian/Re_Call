#include "stdafx.h"
#include "TVPHook.h"
#include "shlwapi.h"
#include "tp_stub.h"
#include "detours.h"
#include <tlhelp32.h>
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "Shlwapi.lib")
TVPHook* TVPHook::handle = NULL;
CPKFileSystem CPKFS;




char* wtoc(LPCTSTR str, UINT cp)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char *out = new char[dwMinSize];
	WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	//MessageBoxW(0, L"Hooked!", L"Info", 0);
	auto obj = GetobjTVPhook();
	UINT outcp;
	switch (cp)
	{
	case CP_ACP:
	case CP_OEMCP:
	case CP_THREAD_ACP:
		outcp = 932;
		break;

	default:
		break;
	}
	return((PfuncMultiByteToWideChar)obj->g_pOldMultiByteToWideChar)(outcp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}

HMODULE WINAPI HookLoadLibraryW(LPCWSTR lpLibFileName)
{
	HMODULE       Module;
	TVPHook*  hookobj;


	hookobj = GetobjTVPhook();
	Module = ((PfuncLoadLibraryW)hookobj->g_pOldLoadLibraryW)(lpLibFileName);
	hookobj->initKrkrHook(lpLibFileName, Module);
	return Module;
}
HWND hwnd2;

PVOID g_pOldCreateWindowExW = CreateWindowExW;
typedef HWND(WINAPI* pfincCreateWindowExW)(
	DWORD     dwExStyle,
	LPCWSTR   lpClassName,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
	);
HWND WINAPI HookCreateWindowExW(
	DWORD     dwExStyle,
	LPCWSTR   lpClassName,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
)
{
	//0118E20C   055DAF80  |WindowName = "俺の姿が?透明に! 不可視の薬と数奇な運命"
	hwnd2 = ((pfincCreateWindowExW)g_pOldCreateWindowExW)(dwExStyle, lpClassName,
		L"【希月学园&红发小女孩】不可视之药与坎坷的命运～请勿在公共平台录播与直播",
		dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	return hwnd2;
}

TVPHook::TVPHook()
{
	g_POldV2Link = NULL;
	g_POldCreateStream = NULL;
	g_pOldMultiByteToWideChar = NULL;
	g_pOldLoadLibraryW = NULL;
	inited = false;
}


TVPHook::~TVPHook()
{
}

COLORREF rgb;
HWND ghWnd;
HBITMAP hBitmap;
int CALLBACK TimerProc()
{
	static int wndAlp = 0;
	static int flag = 0;

	if (flag)
	{
		if (flag == 1)
		{
			Sleep(5000);
			flag = 2;
		}
		wndAlp -= 3;
		if (wndAlp == 0)
			DestroyWindow(ghWnd);
		SetLayeredWindowAttributes(ghWnd, -1, wndAlp, LWA_ALPHA);
	}
	else
	{
		wndAlp += 5;
		if (wndAlp == 255)
			flag = 1;
		SetLayeredWindowAttributes(ghWnd, -1, wndAlp, LWA_ALPHA);
	}
	return 0;
}
void DrawBmp(HDC hDC, HBITMAP bitmap, int nWidth, int nHeight)
{
	BITMAP			bm;
	HDC hdcImage;
	HDC hdcMEM;
	hdcMEM = CreateCompatibleDC(hDC);
	hdcImage = CreateCompatibleDC(hDC);
	HBITMAP bmp = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	GetObject(bitmap, sizeof(bm), (LPSTR)&bm);
	SelectObject(hdcMEM, bmp);
	SelectObject(hdcImage, bitmap);
	StretchBlt(hdcMEM, 0, 0, nWidth, nHeight, hdcImage, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	StretchBlt(hDC, 0, 0, nWidth, nHeight, hdcMEM, 0, 0, nWidth, nHeight, SRCCOPY);

	DeleteObject(hdcImage);
	DeleteDC(hdcImage);
	DeleteDC(hdcMEM);
}
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	static HDC compDC = 0;
	static RECT rect;
	if (uMsg == WM_CREATE)
	{
		ghWnd = hwnd;
		SetLayeredWindowAttributes(hwnd, -1, 0, LWA_ALPHA);
		SetTimer(hwnd, 5003, 1, (TIMERPROC)TimerProc);

		int scrWidth, scrHeight;

		scrWidth = GetSystemMetrics(SM_CXSCREEN);
		scrHeight = GetSystemMetrics(SM_CYSCREEN);
		GetWindowRect(hwnd, &rect);
		rect.left = (scrWidth - rect.right) / 2;
		rect.top = (scrHeight - rect.bottom) / 2;
		SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

		DrawBmp(GetDC(hwnd), hBitmap, 1280, 720);
	}
	if (uMsg == WM_PAINT)
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);

	}
	if (uMsg == WM_CLOSE)
	{
		DestroyWindow(hwnd);
	}
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
HINSTANCE hInst, hInst2;

bool __stdcall LogoShow()
{
	if (!fopen("_media\\background-bmp.bmp", "rb"))
	{
		MessageBox(0, L"no img.", 0, 0);
		return false;
	}
	hBitmap = (HBITMAP)LoadImageA(NULL, "_media\\background-bmp.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	WNDCLASSEXA wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInst;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName = "About";
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.cbWndExtra = DLGWINDOWEXTRA;
	rgb = 0xFFFFFFFF;
	RegisterClassExA(&wcex);
	HWND hWnd = CreateWindowExA(WS_EX_LAYERED | WS_EX_TOPMOST, "About", "About", WS_POPUP | WS_SYSMENU | WS_SIZEBOX, 0, 0, 1280, 730, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

bool TVPHook::init()
{
	if (!CPKFS.InitFileSystem())
		ExitProcess(-1);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	g_pOldLoadLibraryW= DetourFindFunction("Kernel32.dll", "LoadLibraryW");
	DetourAttach(&g_pOldLoadLibraryW, HookLoadLibraryW);
	DetourAttach(&g_pOldCreateWindowExW, HookCreateWindowExW);
	DetourTransactionCommit();
	bool ret = false;
	ret = LogoShow();
	return ret;
}


PVOID GetTVPCreateStreamAddress()
{
	auto baseAddr = (DWORD)GetModuleHandle(NULL);
	std::cout << "BASE:0x" << std::hex << (DWORD)baseAddr << std::endl;
	//PVOID CallIStreamStub = (PVOID)(baseAddr + 0x42BA0);
	PVOID CallIStreamStub = (PVOID)(baseAddr + 0x2AAC0);
	return CallIStreamStub;
}

class tTJSCriticalSection
{
	CRITICAL_SECTION CS;
public:
	tTJSCriticalSection() { InitializeCriticalSection(&CS); }
	~tTJSCriticalSection() { DeleteCriticalSection(&CS); }

	void Enter() { EnterCriticalSection(&CS); }
	void Leave() { LeaveCriticalSection(&CS); }
};

class tTJSCriticalSectionHolder
{
	tTJSCriticalSection *Section;
public:
	tTJSCriticalSectionHolder(tTJSCriticalSection &cs)
	{
		Section = &cs;
		Section->Enter();
	}

	~tTJSCriticalSectionHolder()
	{
		Section->Leave();
	}

};

static tTJSCriticalSection LocalCreateStreamCS;

std::wstring GetKrkrFileName(LPCWSTR Name)
{
	std::wstring Info(Name);

	if (Info.find_last_of(L">") != std::wstring::npos)
		Info = Info.substr(Info.find_last_of(L">") + 1, std::wstring::npos);

	if (Info.find_last_of(L"/") != std::wstring::npos)
		Info = Info.substr(Info.find_last_of(L"/") + 1, std::wstring::npos);

	return Info;
}

/*
void FileNameToLower(std::wstring& FileName)
{
	for (auto& Item : FileName)
	{
		if (Item <= L'Z' && Item >= L'A')
			Item += towlower(Item);
	}
}
*/

static void FileNameToLower(std::wstring& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

#define NONE
#ifdef TEST

IStream * TVPHook::CreateLocalStream(LPCWSTR lpFileName)
{
	tTJSCriticalSectionHolder CSHolder(LocalCreateStreamCS);
	std::wstring FileName;
	FileName = GetKrkrFileName(lpFileName);
	FileNameToLower(FileName);
	std::wstring NewPath = L"_media\\" + FileName;
	std::cout << "NewPath:" << wtoc(FileName.c_str(), 936) << std::endl;
	IStream* *Stream;
	auto hr = SHCreateStreamOnFileEx(NewPath.c_str(), STGM_READ, 0, FALSE, NULL, Stream);
	if (SUCCEEDED(hr))
	{
		//std::cout << "NewPath:" << wtoc(FileName.c_str(),936) << std::endl;
		return *Stream;
	}
	else
	{

		return NULL;
	}
	
}

#else
IStream* TVPHook::CreateLocalStream(LPCWSTR lpFileName)
{
	tTJSCriticalSectionHolder CSHolder(LocalCreateStreamCS);
	std::wstring FileName;
	FileName = GetKrkrFileName(lpFileName);

	BYTE* FileBuff = nullptr;
	DWORD FileSize = 0;

	FileNameToLower(FileName);
	char* fnm = wtoc(FileName.c_str(), 936);
	CPKFS.GetFileByName(fnm, &FileBuff, &FileSize);
	std::cout << "NewPath:" << fnm << std::endl;

	if (FileSize != 0)
	{
		//std::cout << "NewPath:" << wtoc(FileName.c_str()) << std::endl;
		IStream* pStream = SHCreateMemStream(NULL, NULL);
		pStream = SHCreateMemStream(NULL, NULL);
		pStream->Write(FileBuff, FileSize, NULL);
		LARGE_INTEGER pos;
		pos.LowPart = 0; pos.HighPart = 0;
		pStream->Seek(pos, STREAM_SEEK_SET, NULL);
		delete[] FileBuff;
		FileSize = 0;
		return pStream;
	}
	else
	{
		std::wstring NewPath = L"_media\\" + FileName;
		IStream** Stream;
		auto hr = SHCreateStreamOnFileEx(NewPath.c_str(), STGM_READ, 0, FALSE, NULL, Stream);
		if (SUCCEEDED(hr))
			return *Stream;
		//cout << "miss" << endl;
		return NULL;
	}

}

#endif
tTJSBinaryStream* __fastcall HookTVPCreateStream(const ttstr & _name, tjs_uint32 flags)
{
	TVPHook*      objhook;
	tTJSBinaryStream*  Stream;
	IStream*           IStream;

	objhook = GetobjTVPhook();
	
	IStream = objhook->CreateLocalStream(_name.c_str());
	if (IStream)
	{
		Stream = TVPCreateBinaryStreamAdapter(IStream);
		if (!Stream)
		{
			Stream = ((PfuncCreateStream)objhook->g_POldCreateStream)(_name, flags);
		}
	}
	else
	{
		Stream = ((PfuncCreateStream)objhook->g_POldCreateStream)(_name, flags);
	}
	return Stream;
}

HRESULT __stdcall HookV2Link(iTVPFunctionExporter *exporter)
{
	TVPHook* objTVPhook = GetobjTVPhook();
	if (!objTVPhook->inited)
	{
		TVPInitImportStub(exporter);
		objTVPhook->TVPFunctionExporter = exporter;
		//std::cout << "KRKR Function List:0x" << std::hex << (DWORD)exporter << std::endl;
		static char funcname[] = "IStream * ::TVPCreateIStream(const ttstr &,tjs_uint32)";
		auto CallIStreamStub = TVPGetImportFuncPtr(funcname);
		auto baseAddr = (DWORD)GetModuleHandle(NULL);
		//std::cout << "BASE:0x" << std::hex << (baseAddr) << std::endl;
		//std::cout << "IStream * ::TVPCreateIStream:0x" << std::hex << ((DWORD)CallIStreamStub) << std::endl;
		auto res=GetTVPCreateStreamAddress();
		objTVPhook->g_POldCreateStream = res;
		//std::cout << "TVPCreateStream:0x" << std::hex << (DWORD)res << std::endl;
		if (res!=NULL)
		{
			DetourTransactionBegin();
			DetourAttach(&objTVPhook->g_POldCreateStream, HookTVPCreateStream);
			if (DetourTransactionCommit() != NO_ERROR)
			{
				MessageBoxW(NULL, L"Hook目标函数失败", L"严重错误", MB_OK | MB_ICONWARNING);
			}
		}
		objTVPhook->inited = true;
	}
	return ((PfuncV2Link)objTVPhook->g_POldV2Link)(exporter);
}

void TVPHook::initKrkrHook(LPCWSTR lpFileName, HMODULE Module)
{
	auto extStr = PathFindExtensionW(lpFileName);
	TVPHook* objTVPhook = GetobjTVPhook();
	if (StrCmpW(extStr,L".dll")==0|| StrCmpW(extStr, L".tpm") == 0)
	{
		auto pV2Link = GetProcAddress(Module, "V2Link");
		objTVPhook->g_POldV2Link = pV2Link;
		if (pV2Link!=NULL&&!inited)
		{
			DetourTransactionBegin();
			DetourAttach(&objTVPhook->g_POldV2Link, HookV2Link);
			DetourTransactionCommit();
		}
	}
}


TVPHook * GetobjTVPhook()
{
	if (TVPHook::handle==NULL)
	{
		TVPHook::handle = new TVPHook;	
	}
	return TVPHook::handle;
}

bool TVPHook::uninit()
{
	DetourDetach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourDetach(&g_pOldLoadLibraryW, HookLoadLibraryW);
	DetourDetach(&g_POldV2Link, HookV2Link);
	g_POldV2Link = NULL;
	g_POldCreateStream = NULL;
	g_pOldMultiByteToWideChar = NULL;
	g_pOldLoadLibraryW = NULL;
	return false;
}