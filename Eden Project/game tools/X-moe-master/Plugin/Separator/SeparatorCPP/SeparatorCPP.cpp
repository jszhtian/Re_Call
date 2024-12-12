#include "SeparatorCPP.h"
#include <SDKDDKVer.h>
#include <Windows.h>
#include "resource.h"
#include "SeparatorHolder.h"
#include <ShlObj.h>
#include <OleIdl.h>
#include <assert.h>
#include <Shlwapi.h>
#include "TaskInfo.h"

#pragma comment(lib, "Shlwapi.lib")

INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT WINAPI SetupProcess(HWND hWnd);
HRESULT WINAPI LoadCore();
HRESULT WINAPI FreeCore();

HMODULE hCoreMod = nullptr;
HINSTANCE hGlobalInstance = nullptr;
INT_PTR CALLBACK TabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC StubTabProc = nullptr;

VOID WINAPI ScreenToClientSelf(HWND hwndPar, HWND hwndChild, RECT& rect)
{
	POINT pt;
	int ixPos, iyPos;

	UNREFERENCED_PARAMETER(ixPos);
	UNREFERENCED_PARAMETER(iyPos);

	GetWindowRect(hwndPar, &rect);
	pt.x = rect.left;
	pt.y = rect.top;
	ScreenToClient(hwndPar, &pt);
	rect.left = pt.x;
	rect.top = pt.y;
	pt.x = rect.right;
	pt.y = rect.bottom;
	ScreenToClient(hwndPar, &pt);
	rect.right = pt.x;
	rect.bottom = pt.y;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (SeparatorHolder::getInstance() == nullptr)
	{
		//????
		MessageBoxW(NULL, L"Failed to launch!", L"FATAL", MB_OK);
		return -1;
	}
	
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	SeparatorHolder::getInstance()->SetCPUCount(info.dwNumberOfProcessors);
	
	LoadCore();
	//ingored some error when fail to load this plugin
	LoadLibraryW(L"HighDPIPlugin.dll");
	hGlobalInstance = hInstance;

	HWND hWnd = NULL;
	hWnd = CreateDialogParamW(hInstance, MAKEINTRESOURCEW(ID_Separator_Main), /*GetDesktopWindow()*/ NULL, WndProc, WM_INITDIALOG);
	if (hWnd == nullptr)
	{
		MessageBoxW(NULL, L"Unable to Create main window!", L"FATAL", MB_OK);
		return -1;
	}
	
	DragAcceptFiles(hWnd, TRUE);

	StubTabProc = (WNDPROC)GetWindowLongW(GetDlgItem(hWnd, IDC_TAB1), GWLP_WNDPROC);
	SetWindowLongW(GetDlgItem(hWnd, IDC_TAB1), GWLP_WNDPROC, (LONG)TabWndProc);

	ULONG Style = GetWindowLong(hWnd, GWL_STYLE);
	SetWindowLongW(hWnd, GWL_STYLE, (Style | WS_MINIMIZEBOX));

	RECT MainRect = { 0 };
	GetWindowRect(hWnd, &MainRect);
	ULONG mDesktopX = GetSystemMetrics(SM_CXSCREEN);
	ULONG mDesktopY = GetSystemMetrics(SM_CYSCREEN);
	ULONG mStyle = GetWindowLongW(hWnd, GWL_EXSTYLE);
	MoveWindow(hWnd, mDesktopX / 2 - (MainRect.right - MainRect.left) / 2,
		mDesktopY / 2 - (MainRect.bottom - MainRect.top) / 2, MainRect.right - MainRect.left, MainRect.bottom - MainRect.top, FALSE);

	SetWindowLongW(hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, NULL, 225, LWA_ALPHA);
	SetLayeredWindowAttributes(GetDlgItem(hWnd, IDC_TAB1), RGB(236, 233, 216), 225, 1);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
#if 0
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);	
	}
#else
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		if (msg.message == WM_KEYDOWN)
		{
			SendMessage(hWnd, msg.message, msg.wParam, msg.lParam);
		}
		else if (!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
#endif

	FreeCore();
	return (int) msg.wParam;
}


wstring WINAPI GetFilePathName(wstring& Name)
{
	int Pos = Name.find_last_of(L"\\");
	if (Pos == wstring::npos)
	{
		Pos = Name.find_last_of(L"/");
	}
	if (Pos == wstring::npos)
	{
		return Name;
	}
	else
	{
		return Name.substr(Pos + 1, wstring::npos);
	}
}

HRESULT WINAPI LoadCore()
{
	if (hCoreMod == nullptr)
	{
		hCoreMod = LoadLibraryW(L"SeparatorCore.dll");
	}
	return hCoreMod ? S_OK : S_FALSE;
}

HRESULT WINAPI FreeCore()
{
	BOOL Result;
	if (hCoreMod)
	{
		Result = FreeLibrary(hCoreMod);
		hCoreMod = nullptr;
	}
	return Result ? S_OK : S_FALSE;
}

typedef HRESULT(WINAPI* SeparatorEntryProc)(int TaskInfo, const WCHAR* Name, const WCHAR* PluginName,
	const WCHAR* Args, ULONG CPULogicCount);

HRESULT WINAPI SetupProcess(HWND hWnd)
{
	if (hCoreMod == nullptr)
	{
		MessageBoxW(hWnd, L"Cannot load SeparatorCore.dll", L"Error", MB_OK);
		return S_FALSE;
	}
	SeparatorEntryProc SeparatorEntry = nullptr;
	SeparatorEntry = (SeparatorEntryProc)GetProcAddress(hCoreMod, "SeparatorEntry");
	if (SeparatorEntry == nullptr)
	{
		MessageBoxW(hWnd, L"Invalid SeparatorCore.dll", L"Error", MB_OK);
		return S_FALSE;
	}
	SeparatorHolder* holder = SeparatorHolder::getInstance();
	BOOL bCheckFile, bCheckFolder, bCheckScript;
	holder->GetCheckFile(&bCheckFile);
	holder->GetCheckFolder(&bCheckFolder);
	holder->GetCheckScript(&bCheckScript);

	ULONG Cores = 1;
	wstring PluginName, ArgsName;
	holder->GetEditPlugin(PluginName);
	holder->GetEditArgs(ArgsName);
	holder->GetCPUUsingCores(&Cores);
	if (bCheckFile)
	{
		wstring PathName;
		holder->GetEditFile(PathName);
		if (PathName.length())
		{
			EnableWindow(hWnd, FALSE);
			SeparatorEntry(TaskInfo::FileTask, PathName.c_str(), PluginName.c_str(),
				ArgsName.c_str(), Cores);
			EnableWindow(hWnd, TRUE);
		}
		else
		{
			MessageBoxW(hWnd, L"File Task : File name is emtry", L"Error", MB_OK);
			return S_FALSE;
		}
	}
	else if (bCheckFolder)
	{
		wstring PathName;
		holder->GetEditFolder(PathName);
		if (PathName.length())
		{
			EnableWindow(hWnd, FALSE);
			SeparatorEntry(TaskInfo::FileTask, PathName.c_str(), PluginName.c_str(),
				ArgsName.c_str(), Cores);
			EnableWindow(hWnd, TRUE);
		}
		else
		{
			MessageBoxW(hWnd, L"Folder task : Folder name is empty", L"Error", MB_OK);
			return S_FALSE;
		}
	}
	else if (bCheckScript)
	{
		wstring PathName;
		holder->GetEditScript(PathName);
		if (PathName.length())
		{
			EnableWindow(hWnd, FALSE);
			SeparatorEntry(TaskInfo::FileTask, PathName.c_str(), PluginName.c_str(),
				ArgsName.c_str(), Cores);
			EnableWindow(hWnd, TRUE);
		}
		else
		{
			MessageBoxW(hWnd, L"Script task : Script name is empty", L"Error", MB_OK);
			return S_FALSE;
		}
	}
	return S_OK;
}


INT_PTR CALLBACK TabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//for tab transparent
	if (message == WM_ERASEBKGND)
	{
		SetBkColor((HDC)wParam, RGB(0, 0, 0));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR)GetStockObject(HOLLOW_BRUSH);
	}
	else if (message == WM_CTLCOLORBTN)
	{
		SetBkColor((HDC)wParam, RGB(0, 0, 0));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR)GetStockObject(HOLLOW_BRUSH);
	}
	return CallWindowProcW(StubTabProc, hWnd, message, wParam, lParam);
}


static HBITMAP BGImage = nullptr;
static BITMAP  BGObject;
INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT Painter;

	switch (message)
	{
	case WM_CREATE:
	{
		
	}
	break;

	case WM_INITDIALOG:
	{
		SendMessageW(hWnd, WM_SETICON, ICON_SMALL,
			(LPARAM)LoadIconW(hGlobalInstance, MAKEINTRESOURCEW(IDI_SEPARATORCPP)));
		BGImage = LoadBitmapW(hGlobalInstance, MAKEINTRESOURCEW(IDB_BITMAP1));
		GetObjectW(BGImage, sizeof(BGObject), &BGObject);

		ShowWindow(GetDlgItem(hWnd, IDC_STATIC4), FALSE);

		TCITEMW TabPage1;
		TCITEMW TabPage2;
		TabPage1.pszText = L"Input";
		TabPage2.pszText = L"About";
		TabPage1.iImage = -1;
		TabPage2.iImage = -1;
		TabPage1.mask = TabPage2.mask = TCIF_TEXT;
		HWND hTab = GetDlgItem(hWnd, IDC_TAB1);
		TabCtrl_InsertItem(hTab, 0, &TabPage1);
		TabCtrl_InsertItem(hTab, 1, &TabPage2);
		TabCtrl_SetCurSel(GetDlgItem(hWnd, IDC_TAB1), 0);

		SendMessageW(GetDlgItem(hWnd, IDC_TAB1), WM_CTLCOLORDLG, 0, 0);

		//=============================
		HWND FileButton = GetDlgItem(hWnd, IDC_BUTTON1);
		HWND FileEdit = GetDlgItem(hWnd, IDC_EDIT1);
		HWND FolderButton = GetDlgItem(hWnd, IDC_BUTTON2);
		HWND FolderEdit = GetDlgItem(hWnd, IDC_EDIT2);
		HWND ScriptButton = GetDlgItem(hWnd, IDC_BUTTON3);
		HWND ScriptEdit = GetDlgItem(hWnd, IDC_EDIT3);
		HWND PluginEdit = GetDlgItem(hWnd, IDC_EDIT4);
		HWND ArgsEdit = GetDlgItem(hWnd, IDC_EDIT5);

		HWND FileCheck = GetDlgItem(hWnd, IDC_CHECK1);
		HWND FolderCheck = GetDlgItem(hWnd, IDC_CHECK2);
		HWND ScriptCheck = GetDlgItem(hWnd, IDC_CHECK3);
		HWND PluginCheck = GetDlgItem(hWnd, IDC_CHECK4);
		HWND ArgsCheck = GetDlgItem(hWnd, IDC_CHECK5);

		EnableWindow(FileButton, FALSE);
		EnableWindow(FileEdit, FALSE);
		EnableWindow(FolderButton, FALSE);
		EnableWindow(FolderEdit, FALSE);
		EnableWindow(ScriptButton, FALSE);
		EnableWindow(PluginEdit, FALSE);
		EnableWindow(ArgsEdit, FALSE);
		EnableWindow(ScriptEdit, FALSE);

		DragAcceptFiles(FileEdit, TRUE);
		DragAcceptFiles(FolderEdit, TRUE);
		DragAcceptFiles(ScriptEdit, TRUE);

		SendMessageW(ScriptCheck, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessageW(FileCheck, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessageW(FolderCheck, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessageW(ArgsCheck, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessageW(PluginCheck, BM_SETCHECK, BST_UNCHECKED, 0);

		ULONG CPUCores;
		SeparatorHolder::getInstance()->GetCPUCount(&CPUCores);
		HWND hSubWnd = GetDlgItem(hWnd, IDC_COMBO1);
		for (ULONG i = 1; i <= min(CPUCores, 32); i++)
		{
			WCHAR Info[MAX_PATH] = { 0 };
			if (i == 1)
			{
				wsprintfW(Info, L"%d Core", i);
			}
			else
			{
				wsprintfW(Info, L"%d Cores", i);
			}
			SendMessageW(hSubWnd, CB_INSERTSTRING, i - 1, (LPARAM)Info);
		}
		ULONG Heigth = 0;
		ULONG Width = 0;
		Heigth = SendMessageW(hSubWnd, CB_GETITEMHEIGHT, 0, 0);
		Width = SendMessageW(hSubWnd, CB_GETDROPPEDWIDTH, 0, 0);
		SendMessageW(hSubWnd, CB_SETCURSEL, 0, 0);
	}
	break;

	case WM_MOUSEACTIVATE:
	{
		SetLayeredWindowAttributes(hWnd, NULL, 225, LWA_ALPHA);
	}
	break;

	case WM_MOUSELEAVE:
	{
		SetLayeredWindowAttributes(hWnd, NULL, 180, LWA_ALPHA);
	}
	break;

	case WM_PAINT:
	{
		if (IsIconic(hWnd))
		{
		}
		else
		{
			RECT tabRect, dlgRect;
			GetWindowRect(GetDlgItem(hWnd, IDC_TAB1), &tabRect);
			GetWindowRect(hWnd, &dlgRect);
			HDC hTabDC = GetDC(GetDlgItem(hWnd, IDC_TAB1));
			HDC hDC = BeginPaint(hWnd, &Painter);
			HDC mDC = CreateCompatibleDC(hDC);
			SelectObject(mDC, BGImage);
			BitBlt(hDC, 0, 0, BGObject.bmWidth, BGObject.bmHeight, mDC, 0, 0, SRCCOPY);
			DeleteDC(mDC);
			EndPaint(hWnd, &Painter);
		}
	}
	break;

	

	//"Cores"
	case WM_CTLCOLORSTATIC:
	{
		ULONG IndexId = GetWindowLongW((HWND)lParam, GWL_ID);
		if (IndexId == IDC_STATIC4 || IndexId == IDC_STATIC3 || IndexId == IDC_STATIC1 || IndexId == IDC_STATIC2)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (INT_PTR)GetStockObject(HOLLOW_BRUSH);
		}

		if (IndexId == IDC_CHECK1 || IndexId == IDC_CHECK2 || IndexId == IDC_CHECK3 ||
			IndexId == IDC_CHECK4 || IndexId == IDC_CHECK5)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (INT_PTR)GetStockObject(HOLLOW_BRUSH);
		}
	}
	break;

	case WM_CTLCOLORDLG:
	{
		ULONG IndexId = GetWindowLongW((HWND)lParam, GWL_ID);
		//if (IndexId == IDC_TAB1)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (INT_PTR)GetStockObject(HOLLOW_BRUSH);
		}
	}
	break;

	case WM_SYSCOMMAND:
	{
		WORD wmIdsys = LOWORD(wParam);
		WORD wmEventsys = HIWORD(wParam);
		switch (wmIdsys)
		{
		case SC_CLOSE:
			DestroyWindow(hWnd);
			break;

		case SC_MINIMIZE:
			break;

		case SC_RESTORE:
		{
		}
		break;

		default:
			//return DefWindowProcW(hWnd, message, wParam, lParam);
			break;
		}
	}
	break;
	
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		//select a file
		case IDC_EDIT1:
		{
			switch (wmEvent)
			{
			case EN_CHANGE:
			{
				HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT1);
				WCHAR lpString[1024] = { 0 };
				GetWindowTextW(hSubWnd, lpString, 1024);
				SeparatorHolder::getInstance()->SetEditFile(wstring(lpString));
			}
			break;
			default:
				break;
			}
		}
		break;

		//select a folder
		case IDC_EDIT2:
		{
			switch (wmEvent)
			{
			case EN_CHANGE:
			{
				HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT2);
				WCHAR lpString[1024] = { 0 };
				GetWindowTextW(hSubWnd, lpString, 1024);
				SeparatorHolder::getInstance()->SetEditFolder(wstring(lpString));
			}
			break;
			default:
				break;
			}
		}
		break;

		//select a script
		case IDC_EDIT3:
		{
			switch (wmEvent)
			{
			case EN_CHANGE:
			{
				HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT3);
				WCHAR lpString[1024] = { 0 };
				GetWindowTextW(hSubWnd, lpString, 1024);
				SeparatorHolder::getInstance()->SetEditScript(wstring(lpString));
			}
			break;
			default:
				break;
			}
		}
		break;

		//select plugin
		case IDC_EDIT4:
		{
			switch (wmEvent)
			{
			case EN_CHANGE:
			{
				HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT4);
				WCHAR lpString[1024] = { 0 };
				GetWindowTextW(hSubWnd, lpString, 1024);
				SeparatorHolder::getInstance()->SetEditPlugin(wstring(lpString));
			}
			break;
			default:
				break;
			}
		}
		break;

		//args
		case IDC_EDIT5:
		{
			switch (wmEvent)
			{
			case EN_CHANGE:
			{
				HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT5);
				WCHAR lpString[1024] = { 0 };
				GetWindowTextW(hSubWnd, lpString, 1024);
				SeparatorHolder::getInstance()->SetEditArgs(wstring(lpString));
			}
			break;
			default:
				break;
			}
		}
		break;

		//select a file
		case IDC_CHECK1:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				BOOL Result;
				HWND FileCheck = GetDlgItem(hWnd, IDC_CHECK1);
				//SeparatorHolder::getInstance()->GetCheckFile(&Result);
				Result = SendMessageW(FileCheck, BM_GETCHECK, 0, 0);
				if (Result == FALSE)
				{
					HWND FileButton = GetDlgItem(hWnd, IDC_BUTTON1);
					HWND FileEdit = GetDlgItem(hWnd, IDC_EDIT1);
					EnableWindow(FileButton, FALSE);
					EnableWindow(FileEdit, FALSE);
					SeparatorHolder::getInstance()->SetCheckFile(FALSE);
				}
				else
				{
					HWND FolderCheck = GetDlgItem(hWnd, IDC_CHECK2);
					HWND ScriptCheck = GetDlgItem(hWnd, IDC_CHECK3);

					HWND FileButton = GetDlgItem(hWnd, IDC_BUTTON1);
					HWND FileEdit = GetDlgItem(hWnd, IDC_EDIT1);
					HWND FolderButton = GetDlgItem(hWnd, IDC_BUTTON2);
					HWND FolderEdit = GetDlgItem(hWnd, IDC_EDIT2);
					HWND ScriptButton = GetDlgItem(hWnd, IDC_BUTTON3);
					HWND ScriptEdit = GetDlgItem(hWnd, IDC_EDIT3);

					EnableWindow(FileButton, TRUE);
					EnableWindow(FileEdit, TRUE);
					EnableWindow(FolderButton, FALSE);
					EnableWindow(FolderEdit, FALSE);
					EnableWindow(ScriptButton, FALSE);
					EnableWindow(ScriptEdit, FALSE);

					SendMessageW(FileCheck, BM_SETCHECK, BST_CHECKED, 0);
					BOOL FolderResult, ScriptResult;
					FolderResult = SendMessageW(FolderButton, BM_GETCHECK, 0, 0);
					ScriptResult = SendMessageW(ScriptButton, BM_GETCHECK, 0, 0);
					if (1) //(FolderResult)
					{
						SendMessageW(FolderCheck, BM_SETCHECK, BST_UNCHECKED, 0);
						SeparatorHolder::getInstance()->SetCheckFolder(FALSE);
					}
					if (1) //(ScriptResult)
					{
						SendMessageW(ScriptCheck, BM_SETCHECK, BST_UNCHECKED, 0);
						SeparatorHolder::getInstance()->SetCheckScript(FALSE);
					}
					SeparatorHolder::getInstance()->SetCheckFile(TRUE);
				}
			}
			break;
			default:
				break;
			}
		}
		break;

		//select a folder
		case IDC_CHECK2:
		{
				switch (wmEvent)
				{
				case BN_CLICKED:
				{
					BOOL Result;
					HWND FolderCheck = GetDlgItem(hWnd, IDC_CHECK2);
					//SeparatorHolder::getInstance()->GetCheckFile(&Result);
					Result = SendMessageW(FolderCheck, BM_GETCHECK, 0, 0);
					if (Result == FALSE)
					{
						HWND FolderButton = GetDlgItem(hWnd, IDC_BUTTON2);
						HWND FolderEdit = GetDlgItem(hWnd, IDC_EDIT2);
						EnableWindow(FolderButton, FALSE);
						EnableWindow(FolderEdit, FALSE);
						SeparatorHolder::getInstance()->SetCheckFolder(FALSE);
					}
					else
					{
						HWND FileCheck = GetDlgItem(hWnd, IDC_CHECK1);
						HWND ScriptCheck = GetDlgItem(hWnd, IDC_CHECK3);

						HWND FileButton = GetDlgItem(hWnd, IDC_BUTTON1);
						HWND FileEdit = GetDlgItem(hWnd, IDC_EDIT1);
						HWND FolderButton = GetDlgItem(hWnd, IDC_BUTTON2);
						HWND FolderEdit = GetDlgItem(hWnd, IDC_EDIT2);
						HWND ScriptButton = GetDlgItem(hWnd, IDC_BUTTON3);
						HWND ScriptEdit = GetDlgItem(hWnd, IDC_EDIT3);

						EnableWindow(FolderButton, TRUE);
						EnableWindow(FolderEdit, TRUE);
						EnableWindow(FileButton, FALSE);
						EnableWindow(FileEdit, FALSE);
						EnableWindow(ScriptButton, FALSE);
						EnableWindow(ScriptEdit, FALSE);

						SendMessageW(FolderCheck, BM_SETCHECK, BST_CHECKED, 0);
						BOOL FileResult, ScriptResult;
						FileResult = SendMessageW(FileButton, BM_GETCHECK, 0, 0);
						ScriptResult = SendMessageW(ScriptButton, BM_GETCHECK, 0, 0);
						if (1) //(FileResult)
						{
							SendMessageW(FileCheck, BM_SETCHECK, BST_UNCHECKED, 0);
							SeparatorHolder::getInstance()->SetCheckFile(FALSE);
						}
						if (1) //( ScriptResult)
						{
							SendMessageW(ScriptCheck, BM_SETCHECK, BST_UNCHECKED, 0);
							SeparatorHolder::getInstance()->SetCheckScript(FALSE);
						}
						SeparatorHolder::getInstance()->SetCheckFolder(TRUE);
					}
				}
				break;
			default:
				break;
			}
		}
		break;

		//select a script
		case IDC_CHECK3:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				BOOL Result;
				HWND ScriptCheck = GetDlgItem(hWnd, IDC_CHECK3);
				Result = SendMessageW(ScriptCheck, BM_GETCHECK, 0, 0);
				if (Result == FALSE)
				{
					HWND ScriptButton = GetDlgItem(hWnd, IDC_BUTTON3);
					HWND ScriptEdit = GetDlgItem(hWnd, IDC_EDIT3);
					EnableWindow(ScriptButton, FALSE);
					EnableWindow(ScriptEdit, FALSE);
					SeparatorHolder::getInstance()->SetCheckScript(FALSE);
				}
				else
				{
					HWND FileCheck = GetDlgItem(hWnd, IDC_CHECK1);
					HWND FolderCheck = GetDlgItem(hWnd, IDC_CHECK2);

					HWND FileButton = GetDlgItem(hWnd, IDC_BUTTON1);
					HWND FileEdit = GetDlgItem(hWnd, IDC_EDIT1);
					HWND FolderButton = GetDlgItem(hWnd, IDC_BUTTON2);
					HWND FolderEdit = GetDlgItem(hWnd, IDC_EDIT2);
					HWND ScriptButton = GetDlgItem(hWnd, IDC_BUTTON3);
					HWND ScriptEdit = GetDlgItem(hWnd, IDC_EDIT3);

					EnableWindow(FolderButton, FALSE);
					EnableWindow(FolderEdit, FALSE);
					EnableWindow(FileButton, FALSE);
					EnableWindow(FileEdit, FALSE);
					EnableWindow(ScriptButton, TRUE);
					EnableWindow(ScriptEdit, TRUE);

					SendMessageW(ScriptCheck, BM_SETCHECK, BST_CHECKED, 0);
					BOOL FileResult, FolderResult;
					FileResult = SendMessageW(FileButton, BM_GETCHECK, 0, 0);
					FolderResult = SendMessageW(FolderButton, BM_GETCHECK, 0, 0);
					if (1)// (FileResult)
					{
						SendMessageW(FileCheck, BM_SETCHECK, BST_UNCHECKED, 0);
						SeparatorHolder::getInstance()->SetCheckFile(FALSE);
					}
					if (1)//(FolderResult)
					{
						SendMessageW(FolderCheck, BM_SETCHECK, BST_UNCHECKED, 0);
						SeparatorHolder::getInstance()->SetCheckFolder(FALSE);
					}
					SeparatorHolder::getInstance()->SetCheckScript(TRUE);
				}
			}
			break;
			default:
				break;
			}
		}
		break;

		//select plugin
		case IDC_CHECK4:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				BOOL Result;
				HWND PluginCheck = GetDlgItem(hWnd, IDC_CHECK4);
				Result = SendMessageW(PluginCheck, BM_GETCHECK, 0, 0);
				//SeparatorHolder::getInstance()->GetCheckPlugin(&Result);
				if (Result == FALSE)
				{
					HWND PluginEdit = GetDlgItem(hWnd, IDC_EDIT4);
					EnableWindow(PluginEdit, FALSE);
					SeparatorHolder::getInstance()->SetCheckPlugin(FALSE);
				}
				else
				{
					HWND PluginEdit = GetDlgItem(hWnd, IDC_EDIT4);
					EnableWindow(PluginEdit, TRUE);
					SeparatorHolder::getInstance()->SetCheckPlugin(TRUE);
				}
			}
			break;
			default:
				break;
			}
		}
		break;

		//args
		case IDC_CHECK5:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				BOOL Result;
				HWND ArgsCheck = GetDlgItem(hWnd, IDC_CHECK5);
				Result = SendMessageW(ArgsCheck, BM_GETCHECK, 0, 0);
				//SeparatorHolder::getInstance()->GetCheckArgs(&Result);
				if (Result == FALSE)
				{
					HWND ArgsCheck = GetDlgItem(hWnd, IDC_EDIT5);
					EnableWindow(ArgsCheck, FALSE);
					SeparatorHolder::getInstance()->SetCheckArgs(FALSE);
				}
				else
				{
					HWND ArgsCheck = GetDlgItem(hWnd, IDC_EDIT5);
					EnableWindow(ArgsCheck, TRUE);
					SeparatorHolder::getInstance()->SetCheckArgs(TRUE);
				}
			}
			break;
			default:
				break;
			}
		}
		break;

		//select a file
		case IDC_BUTTON1:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				WCHAR lpString[MAX_PATH] = { 0 };
				WCHAR CurDir[MAX_PATH] = { 0 };
				OPENFILENAMEW OpenInfo = { { sizeof(OPENFILENAMEW) } };
				OpenInfo.hwndOwner = hWnd;
				OpenInfo.lpstrFilter = L"All File\0*.*";
				OpenInfo.lpstrFile = lpString;
				OpenInfo.nMaxFile = sizeof(lpString);
				OpenInfo.lpstrTitle = L"Select a file";
				
				GetCurrentDirectoryW(MAX_PATH, CurDir);
				OpenInfo.lpstrInitialDir = CurDir;
				OpenInfo.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
				BOOL Result = GetOpenFileNameW(&OpenInfo);
				if (Result)
				{
					SeparatorHolder::getInstance()->SetEditFile(wstring(lpString));
					HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT1);
					SetWindowTextW(hSubWnd, lpString);
				}
			}
			break;
			default:
				break;
			}
		}
		break;

		//select a folder
		case IDC_BUTTON2:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				WCHAR lpString[MAX_PATH];
				BROWSEINFOW FolderInfo = { 0 };
				FolderInfo.hwndOwner = hWnd;
				FolderInfo.lpszTitle = L"Select a folder";
				FolderInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_UAHINT | BIF_NONEWFOLDERBUTTON;
				LPITEMIDLIST Dlist;
				Dlist = SHBrowseForFolderW(&FolderInfo);
				if (Dlist != nullptr)
				{
					SHGetPathFromIDListW(Dlist, lpString);
					SeparatorHolder::getInstance()->SetEditFolder(wstring(lpString));
					HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT2);
					SetWindowTextW(hWnd, lpString);
				}
			}
			break;
			default:
				break;
			}
		}
		break;

		//select a script
		case IDC_BUTTON3:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				WCHAR lpString[MAX_PATH] = { 0 };
				WCHAR CurDir[MAX_PATH] = { 0 };
				OPENFILENAMEW OpenInfo = { { sizeof(OPENFILENAMEW) } };
				OpenInfo.hwndOwner = hWnd;
				OpenInfo.lpstrFilter = L"Anz-X'moe Script File\0*.axs";
				OpenInfo.lpstrFile = lpString;
				OpenInfo.nMaxFile = sizeof(lpString);
				OpenInfo.lpstrTitle = L"Select a script";

				GetCurrentDirectoryW(MAX_PATH, CurDir);
				OpenInfo.lpstrInitialDir = CurDir;
				OpenInfo.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
				BOOL Result = GetOpenFileNameW(&OpenInfo);
				if (Result)
				{
					SeparatorHolder::getInstance()->SetEditFile(wstring(lpString));
					HWND hSubWnd = GetDlgItem(hWnd, IDC_EDIT3);
					SetWindowTextW(hSubWnd, lpString);
				}
			}
			break;
			default:
				break;
			}
		}
		break;

		//launch
		case IDC_TASK_OK:
		{
			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				SetupProcess(hWnd);
			}
			break;
			default:
				break;
			}
		}
		break;

		//cpu cores
		case IDC_COMBO1:
		{
			switch (wmEvent)
			{
			case CBN_SELCHANGE:
			{
				
				HWND hSubWnd = GetDlgItem(hWnd, IDC_COMBO1);
				ULONG SelectCur = SendMessageW(hSubWnd, CB_GETCURSEL, 0, 0);
				SeparatorHolder::getInstance()->SetCPUUsingCores(SelectCur + 1);
			}
			break;
			default:
				break;
			}
		}
		break;

		default:
			//return DefWindowProcW(hWnd, message, wParam, lParam);
			break;
		}
	break;

	case WM_NOTIFY:
	{
		WORD wmId2 = LOWORD(wParam);
		WORD wmEvent2 = HIWORD(wParam);
		NMHDR* pInfo = nullptr;
		pInfo = (NMHDR*)lParam;
		switch (wParam)
		{
		case IDC_TAB1:
		{
			switch (pInfo->code)
			{
			case TCN_SELCHANGE:
			{
				ULONG Index = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_TAB1));
				if (Index == 0)
				{
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK1), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK2), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK3), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK4), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK5), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT1), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT2), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT3), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT4), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT5), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_BUTTON1), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_BUTTON2), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_BUTTON3), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_STATIC1), TRUE);
					ShowWindow(GetDlgItem(hWnd, IDC_STATIC2), TRUE);

					ShowWindow(GetDlgItem(hWnd, IDC_STATIC4), FALSE);
				}
				else
				{
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK1), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK2), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK3), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK4), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_CHECK5), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT1), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT2), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT3), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT4), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_EDIT5), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_BUTTON1), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_BUTTON2), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_BUTTON3), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_STATIC1), FALSE);
					ShowWindow(GetDlgItem(hWnd, IDC_STATIC2), FALSE);

					ShowWindow(GetDlgItem(hWnd, IDC_STATIC4), TRUE);
					
				}
			}
			break;
			default:
				break;
			}
		}
		break;
		default:
			break;
		}
	}
	break;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT DrawInfo = (LPDRAWITEMSTRUCT)lParam;
		if (DrawInfo->hwndItem == GetDlgItem(hWnd, IDC_TAB1))
		{
			if (DrawInfo->itemID == 0 || DrawInfo->itemID == 1)
			{
				FillRect(DrawInfo->hDC, &(DrawInfo->rcItem), (HBRUSH)GetStockObject(HOLLOW_BRUSH));
				SetBkColor(DrawInfo->hDC, RGB(0, 0, 0));
				SetBkMode(DrawInfo->hDC, TRANSPARENT);
			}
		}
	}
	break;

	case WM_DROPFILES:
	{
		WCHAR FilePath[MAX_PATH] = { 0 };
		HDROP hDrop = (HDROP)wParam;
		UINT nFileNum = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
		if (nFileNum == 1)
		{
			InvalidateRect(hWnd, NULL, TRUE);
			DragQueryFileW(hDrop, 0, FilePath, MAX_PATH);
			DragFinish(hDrop);
			BOOL isFolder = PathIsDirectoryW(FilePath);
			if (isFolder)
			{
				SetWindowTextW(GetDlgItem(hWnd, IDC_EDIT2), FilePath);
				SeparatorHolder::getInstance()->SetEditFolder(wstring(FilePath));
			}
			else
			{
				WCHAR* pExtName = PathFindExtensionW(FilePath);
				if (!lstrcmpiW(pExtName, L"axs"))
				{
					SetWindowTextW(GetDlgItem(hWnd, IDC_EDIT3), FilePath);
					SeparatorHolder::getInstance()->SetEditPlugin(wstring(FilePath));
				}
				else if (!lstrcmpiW(pExtName, L"axm"))
				{
					wstring FilePathName = GetFilePathName(wstring(FilePath));
					SetWindowTextW(GetDlgItem(hWnd, IDC_EDIT4), (L"axm\\" + FilePathName).c_str());
					SeparatorHolder::getInstance()->SetEditScript((L"axm\\" + FilePathName));
				}
				else
				{
					SetWindowTextW(GetDlgItem(hWnd, IDC_EDIT1), FilePath);
					SeparatorHolder::getInstance()->SetEditFile(wstring(FilePath));
				}
			}
		}
		else
		{
			MessageBoxW(hWnd, L"Please Drop only one file at the same time", L"Error", MB_OK);
			DragFinish(hDrop);
		}
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		//return DefWindowProcW(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

