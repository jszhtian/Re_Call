// nnnExe2019.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include <Windows.h>
//#include "framework.h"
#include "nnnExe2019.h"


#define __USE_XAUDIO2__ 1
#if !defined __TINYANLIBDIR__
#define __TINYANLIBDIR__ "d:\\tinyan\\systemNNNLib\\"
#endif


#include "d:\tinyan\systemNNNLib\\nyanLib\include\libinclude2019.h"














#include "d:\tinyan\systemNNN\nyanLib\include\commonMacro.h"
#include "d:\tinyan\systemNNN\nnnUtilLib\scriptcommand.h"
#include "d:\tinyan\systemNNN\nnnUtilLib\commonGameVersion.h"
#include "d:\tinyan\systemNNN\nnnUtilLib\nameList.h"

#include "d:\tinyan\systemNNNLib\\nnnUtilLib\currentDirControl.h"


#include "d:\tinyan\systemNNN\nyanLib\INCLUDE\Picture.h"

#include "d:\tinyan\systemNNNLib\nnnLib\mainControl.h"
#include "d:\tinyan\systemNNNLib\nnnLib\gameCallBack.h"

#include "game.h"


// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	CCurrentDirControl::ChangeCurrentToExe();

	
	




	  

	


	//
	// 必要ならここで体験版フラグなどをたてる
	//
//	CCommonGameVersion::SetTaikenVersion();
//	CCommonGameVersion::SetLoadOk();
//	CCommonGameVersion::SetSaveOk();
	CCommonGameVersion::SetAllOnOk();

	//
	// 必要ならここで独自のwheelMouse,systemfileを作成する
	//
	m_game = NULL;
	//	m_wheelMouse = new CWheelMouse();
	//	m_systemFile = new CCommonSystemFile();
	//	m_systemFile->Load(TRUE);

	
	m_mainControl = new CMainControl();	//コンストラクタでwheelMouse,systemfileを指定できる 指定なし、またはNULLだと内部で標準のを作成する

	if (m_mainControl->CheckSystem())
	{
//		HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		HICON icon = NULL;
		HWND hWnd = m_mainControl->CreateWindowRoutine(hInstance, icon);//hinstance,icon,proc    proc == NULLだとCMainControl内部の標準callBack

		CCommonSystemFile* lpSystemFile = m_mainControl->GetSystemFile();
		SIZE sz = m_mainControl->GetWindowSize();
		int bpp = m_mainControl->GetBpp();
		m_game = new CGame(hWnd, hInstance, lpSystemFile, sz.cx, sz.cy, bpp);
		m_mainControl->Game(m_game);
	}

	ENDDELETECLASS(m_game);
	ENDDELETECLASS(m_mainControl);

	CCurrentDirControl::RestoreCurrent();











	return 0;
}


