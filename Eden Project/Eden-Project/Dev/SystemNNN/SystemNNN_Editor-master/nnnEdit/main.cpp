//
// main.cpp
//


#include <windows.h>
#include <stdio.h>
#include <time.h>

//#include <msxml.h>

#include "resource.h"

#define __TINYANLIBDIR__ "d:\\tinyan\\systemNNNLib\\"
#define __LIBPNGDIR__ "d:\\libpng\\lib\\"
#define __TINYANINCLUDEDIR__ "d:\\tinyan\\systemNNNLib\\"


#include "d:\tinyan\systemNNNLib\\nyanLib\include\libinclude2017.h"


#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nnnUtilLib\mySaveFolder.h"

#include "define.h"


#include "case.h"
#include "messageData.h"

#include "myapplicationBase.h"
#include "myapplication.h"
//#include "wheelmouse.h"

//#include "systemfile.h"
//#include "game.h"

#include "..\..\systemNNN\nyanDirectXLib\mmx.h"

#include "main.h"

//#include "..\..\systemNNN\nyanlib\include\libinclude.h"

//#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
//#pragma comment(lib,"imm32.lib")
//#pragma comment(lib,"rpcrt4.lib")

//#pragma comment(lib,"dxguid.lib")
//#pragma comment(lib,"ddraw.lib")
//#pragma comment(lib,"dsound.lib")

//#pragma comment( lib, "msxml.lib" )

#if _MSC_VER >= 1700

	#if !defined __SYSTEMNNN_VER2__
		#if !defined _DEBUG
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditUtil2017.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditView2017.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditControl2017.lib")
		#else
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditUtil2017D.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditView2017D.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditControl2017D.lib")
		#endif
	#else
		#if !defined _DEBUG
		#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditUtil2017_ver2.lib")
		#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditView2017_ver2.lib")
		#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditControl2017_ver2.lib")
		#else
		#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditUtil2017_ver2D.lib")
		#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditView2017_ver2D.lib")
		#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditControl2017_ver2D.lib")
		#endif
	#endif
#else

	#if _MSC_VER >= 1500

		#if !defined _DEBUG
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditUtil2008.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditView2008.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditControl2008.lib")
		#else
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditUtil2008D.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditView2008D.lib")
			#pragma comment(lib,"..\\..\\systemNNN\\nyanLib\\lib\\nnnEditControl2008D.lib")
		#endif

	#endif

#endif

/*

#if !defined _DEBUG
#pragma comment(lib,"d:\\libpng\\lib\\zlib.lib")
#pragma comment(lib,"d:\\libpng\\lib\\libpng.lib")
#else
#pragma comment(lib,"d:\\libpng\\lib\\zlibD.lib")
#pragma comment(lib,"d:\\libpng\\lib\\libpngD.lib")
#endif

*/



LRESULT CALLBACK ChildWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

//MDICREATESTRUCT mdic;


//CGame* lpGame = NULL;
//CWheelMouse* lpCWheelMouse = NULL;


int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{


	m_app = NULL;

	if (CMMX::CheckMMX() == FALSE)
	{
		MessageBox(NULL,"MMX機能が無いと判断されました","エラー",MB_OK);
		return 1;
	}


	m_hInstance = hInstance;
	m_hInstancePrev = hPrevInstance;

	HWND hwnd0 = GetDesktopWindow();
	HDC hdc0 = GetDC(hwnd0);
//	int pln = GetDeviceCaps(hdc0,PLANES);
	m_dskTopColorBits = GetDeviceCaps(hdc0,BITSPIXEL);
	m_dskTopWindowSizeX = GetDeviceCaps(hdc0,HORZRES);
	m_dskTopWindowSizeY = GetDeviceCaps(hdc0,VERTRES);
	ReleaseDC(hwnd0,hdc0);

	if (m_dskTopColorBits < 16)
	{
		MessageBox(NULL,"16bitカラー以上に設定してください","Error",MB_OK);
		return 1;	//error
	}

	HANDLE mx = NULL;
	mx = CreateMutex(NULL,TRUE,"nnnEditByTinyan");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,"二重起動はできません","Error",MB_OK);
		return 1;
	}




/*
	WNDCLASSEX wc;


	// フレームウィンドウのクラスの登録
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hInstance;
	wc.hIcon         = NULL;//LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MAIN));
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = "DUMMY_MENU";
	wc.lpszClassName = className;
	wc.hIconSm       = NULL;//LoadImage(hInstance,MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!RegisterClassEx(&wc))
	{
		//mutexend
		return -1;
	}
*/



/*

              // Ｈｅｌｌｏ　ＭＤＩ子ウィンドウのクラスの登録
	wc.lpfnWndProc   = (WNDPROC)ChildWndProc;
	wc.cbWndExtra    = sizeof(LONG);
	wc.hIcon         = NULL;//LoadIcon(hInstance,MAKEINTRESOURCE(IDI_HELLO));
	wc.lpszClassName = childClassName;//pHelloClass;
	wc.hIconSm       = NULL;//LoadImage(hInstance,MAKEINTRESOURCE(IDI_HELLO),  IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!RegisterClassEx(&wc))
	{
		//mutexend
		return -2;
	}
*/


	// フレームウィンドウの作成
//	hMenuInit  = LoadMenu(hInstance,"ApMdisys");
//	hMenuHello = LoadMenu(hInstance,"ApMdiHello");
//	hMenuRedit = LoadMenu(hInstance,"ApMdiRedit");
//	hAccel = LoadAccelerators(hInstance,pAppName);

/*
OutputDebugString("CreateWindow()\n");

	m_hWnd = CreateWindow(className,NULL,WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT,CW_USEDEFAULT,
						  CW_USEDEFAULT,CW_USEDEFAULT,
						  //NULL,hMenuInit,m_hInstance,NULL);
						  NULL,NULL,m_hInstance,NULL);


OutputDebugString("End CreateWindow()\n");

	if (m_hWnd == NULL)
	{
		//mutexend
		return -3;
	}
*/

//	SetWindowText(m_hWnd,"あいうえお");
//	ShowWindow(m_hWnd,SW_SHOW);
//	UpdateWindow(m_hWnd);



	CoInitialize(NULL);

	
	CMySaveFolder::ChangeMyFolder("bugnekosoft","nnnedit",MYFOLDER_TYPE_MYDOCUMENT);
	CMySaveFolder::ChangeSaveFolder(NULL,TRUE);
//	CMySaveFolder::CreateSubFolder();



	TIMECAPS	tc ;
	timeGetDevCaps( &tc , sizeof(TIMECAPS) );

	/* マルチメディアタイマーのサービス精度を最大に */
	timeBeginPeriod( tc.wPeriodMin );


	m_app = new CMyApplication(m_hInstance);

	int oldTime = (int)timeGetTime();
	int oldTime0 = oldTime;

	while(1)
	{
		MSG msg;
		if ( PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) )
		{
			if ( !GetMessage(&msg,NULL,0,0) )
			{
				break;
			}

//			if (!TranslateAccelerator(hWndMain,hAccel,&msg) &&
//								!TranslateMDISysAccel(hWndClient,&msg))

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_MOUSEMOVE)
			{
			//	OutputDebugString("M");
				m_app->MainMouseMove(msg);
			}
		}
		else
		{
			int tm = (int)timeGetTime();
			int frame = m_app->GetFrameTime();

			if ((tm - oldTime) > frame - 5)			
			{
				if (m_app != NULL)
				{
					m_app->OnTimer(tm-oldTime);
				}
//					if (lpGame != NULL)
//					{
//						lpGame->MainLoop(tm - oldTime);
//					}
				int ps = tm - oldTime;
				if (ps<0) ps = 0;

				oldTime0 = tm;
				if (ps >= frame)
				{
					oldTime = tm;
				}
				else
				{
					oldTime = tm;
					oldTime += (frame - ps);
				}

			}
			else
			{
//					MsgWaitForMultipleObjectsEx(0,NULL,1,QS_POSTMESSAGE | QS_SENDMESSAGE,0);
//					MsgWaitForMultipleObjects(0,NULL,FALSE,1,QS_POSTMESSAGE | QS_SENDMESSAGE);
				Sleep(1);
			}
		}
	}

	ENDDELETECLASS(m_app);

	timeEndPeriod( tc.wPeriodMin );


//	lpCWheelMouse = new CWheelMouse();

//	int wx = lpSystemFile->m_systemdata.windowX;
//	int wy = lpSystemFile->m_systemdata.windowY;

	int wx = 0;
	int wy = 0;

	int sx = 640 + 2*GetSystemMetrics(SM_CXFIXEDFRAME );
	int sy = 480 + 2*GetSystemMetrics(SM_CYFIXEDFRAME ) + GetSystemMetrics(SM_CYMENU);

	if ((wx+sx) > m_dskTopWindowSizeX) wx = m_dskTopWindowSizeX - sx;
	if (wx<0) wx = 0;
	if ((wy+sy) > m_dskTopWindowSizeY) wy = m_dskTopWindowSizeY - sy;
	if (wy<0) wy = 0;

//	lpSystemFile->m_systemdata.windowX = wx;
//	lpSystemFile->m_systemdata.windowY = wy;
//	lpSystemFile->m_systemdata.colorBits = bpp;





//	srand((unsigned int)time(NULL));

//	mciSendCommand(0,0,0,0);

	//font



  //  ShowWindow( m_hWnd, SW_SHOW );
//	SetFocus(m_hWnd);

	
//	lpGame = new CGame(hWnd,hInstance,lpSystemFile,bpp);

	BOOL endFlag = FALSE;
//	if (lpGame->CheckInitDirectX() == FALSE)
//	{
//		int rt2 = MessageBox(hWnd,"DirectXがインストールされていない可能性があります\nこのまま続けますか","警告",MB_OKCANCEL);
//		if (rt2 ==IDCANCEL)
//		{
//			endFlag = TRUE;
//		}
//	}



//	if (lpSystemFile->Save() == FALSE)
//	{
//		MessageBox(NULL,"システムファイル : sav\\sysfile.sav がセーブできませんでした","エラー",MB_OK);
//	}


//	ENDDELETECLASS(lpGame);
//	ENDDELETECLASS(lpCWheelMouse);
//	ENDDELETECLASS(lpSystemFile);

//    ShowWindow( m_hWnd, SW_SHOW );

	if (mx != NULL)
	{
		//ReleaseMutex( mx );
		CloseHandle(mx);
	}

	CoUninitialize();
	return 0;
}





LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
#if defined __TINYAN_WHEELMOUSE__
	if (lpCWheelMouse != NULL)
	{
		message = lpCWheelMouse->TranslateMessage(message);
	}
#endif

	switch (message)
	{
	case WM_COMMAND:
//		if (wParam == IDM_DUMMY)
//		{
	//		m_app->dmy();

//			MDICREATESTRUCT mdic;
//
//			mdic.szClass = MYCHILDCLASSNAME;
//			mdic.szTitle = "MDI SUB WINDOW";
//			mdic.hOwner  = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
//			mdic.x  = mdic.y  = CW_USEDEFAULT;
//			mdic.cx = mdic.cy = CW_USEDEFAULT;
//			mdic.style   = 0;
//			mdic.lParam  = 0;
//			SendMessage(m_hWndClient,WM_MDICREATE ,0,(LPARAM)&mdic);
//		}
		break;

		case WM_CREATE:         // メインウィンドウの新規作成

//utputDebugString("WM_CREATE\n");

			LPCREATESTRUCT lpcs;
			CLIENTCREATESTRUCT ccs;
			HINSTANCE hInst;
//			LOGFONT   lf;

//			InitCommonControls();
//			hRtfLib = LoadLibrary("RICHED32.DLL");
			hInst = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);


			lpcs  = (LPCREATESTRUCT)lParam;

//			ccs.hWindowMenu  = GetSubMenu(lpcs->hMenu,IDMC_WINDOW);
			ccs.hWindowMenu  = NULL;
			ccs.idFirstChild = ID_MDICHILD;

			m_hWndClient = CreateWindow("MDICLIENT",NULL,
							  WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|
							  WS_HSCROLL|WS_VSCROLL,
							  0,0,0,0,hWnd,lpcs->hMenu,
							  hInst,(LPVOID)&ccs);

		
			break;


		case WM_DESTROY:        // ウィンドウを破棄する
			if (m_hWndClient) DestroyWindow(m_hWndClient);
//			if (hFont) DeleteObject(hFont);
//			if (hRtfLib) FreeLibrary(hRtfLib);
			PostQuitMessage(0);
			return 0;
			break;
	}



//	if (message == WM_DESTROY)
//	{
//		ShowWindow(hWnd,SW_HIDE);
//
//		PostQuitMessage(0);
//		return 0;
//	}

//	if (lpGame != NULL)
//	{
//		return lpGame->GameProc(hWnd,message,wParam,lParam);
//	}

    return DefFrameProc(hWnd,m_hWndClient,message,wParam,lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);

} /* MainWndproc */



LRESULT CALLBACK ChildWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_CREATE:
//		SetWindowLong(hWnd,0,IDM_CENTER);
		return 0;
		break;

	case WM_PAINT:
		HDC         hdc;
		PAINTSTRUCT ps;
		RECT        rc;
		UINT        w;

		hdc = BeginPaint(hWnd,&ps);
		if (hdc)
		{
		  GetClientRect(hWnd,&rc);
		  SetBkMode(hdc,TRANSPARENT);
		  SetTextColor(hdc,RGB(0,0,255));
//		  SelectObject(hdc,hFont);
//		  switch ((UINT)GetWindowLong(hWnd,0))
//		  {
//			  case IDM_LEFT:   w = DT_LEFT;   break;
//			  case IDM_CENTER: w = DT_CENTER; break;
//			  case IDM_RIGHT:  w = DT_RIGHT;  break;
//		  }
		  w = DT_LEFT;
		  DrawText(hdc,"Hello",strlen("Hello") ,&rc , DT_SINGLELINE|DT_VCENTER|w);
		}
		EndPaint(hWnd,&ps);
		return 0;
		break;


		case WM_MDIACTIVATE:

//			HMENU hMenu;

			if ((HWND)lParam == hWnd)
			{
//				hMenu = GetSubMenu(hMenuHello,IDMC_WINDOW+1);
//				SendMessage(hWndClient,WM_MDISETMENU  ,(WPARAM)hMenuHello,(LPARAM)hMenu);
			}
			else
			{
//			  hMenu = GetSubMenu(hMenuInit,IDMC_WINDOW);
//			  SendMessage(hWndClient,WM_MDISETMENU    ,(WPARAM)hMenuInit,(LPARAM)hMenu);
			}
		
//			DrawMenuBar(hWndMain);
			return 0;
			break;
		}

	return DefMDIChildProc(hWnd,message,wParam,lParam);
}


/*
          static BOOL CALLBACK CloseEnumProc(HWND hWnd,LONG lParam)
          {
              if (GetParent(hWnd) != hWndClient) return 1;
              if (lParam == 0)          // Close
              {
                  if (!SendMessage(hWnd,WM_QUERYENDSESSION,0,0L)) return 1;
                  SendMessage(hWndClient,WM_MDIDESTROY,(WPARAM)hWnd,0L);
              }
              else if (lParam == 1)     // Icon
                  ShowWindow(hWnd,SW_MINIMIZE);
              else if (lParam == 2)     // Restore
                  SendMessage(hWndClient,WM_MDIRESTORE,(WPARAM)hWnd,0L);
              return 1;
          }
*/






