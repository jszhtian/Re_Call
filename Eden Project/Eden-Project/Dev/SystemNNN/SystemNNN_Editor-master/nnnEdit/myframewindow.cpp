//
// myframewindow.cpp
//


#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <winuser.h>


#include "resource.h"

#include "windowlist.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "define.h"

#include "..\..\systemNNN\nnnUtillib\wheelmouse.h"

#include "case.h"
#include "messageData.h"

#include "myapplicationBase.h"


#include "myBitmap.h"

#include "myframewindow.h"
#include "myclientwindow.h"



CMyFrameWindow* CMyFrameWindow::m_this = NULL;
HWND CMyFrameWindow::m_clientHWND = NULL;


int CMyFrameWindow::m_windowButtonID[]=
{
	ID_BUTTON_MAINSCREEN,	MAINSCREEN_WINDOW,
	ID_BUTTON_CONTE,		CONTE_WINDOW,
	ID_BUTTON_FILM,			FILM_WINDOW,
	ID_BUTTON_LAYER,		LAYER_WINDOW,
	ID_BUTTON_ZAHYO,		ZAHYO_WINDOW,
	ID_BUTTON_GAMEMESSAGE,	GAMEMESSAGE_WINDOW,
	ID_BUTTON_CONTEMESSAGE,	CONTEMESSAGE_WINDOW,
	ID_BUTTON_EFFECT,		EFFECT_WINDOW,
	ID_BUTTON_STORY,		STORY_WINDOW,
	ID_BUTTON_CONTROL,		CONTROL_WINDOW,
	ID_BUTTON_FILMCASE,		FILMCASE_WINDOW,
	ID_BUTTON_VAR,			VAR_WINDOW,
	ID_BUTTON_PROGRAM,		PROGRAM_WINDOW,
	ID_BUTTON_STORYBOOK,	STORYBOOK_WINDOW,
	-1,-1,
};


int CMyFrameWindow::m_windowMenuID[]=
{
	ID_WINDOW_MAINSCREEN,	MAINSCREEN_WINDOW,
	ID_WINDOW_CONTE,		CONTE_WINDOW,
	ID_WINDOW_FILM,			FILM_WINDOW,
	ID_WINDOW_LAYER,		LAYER_WINDOW,
	ID_WINDOW_ZAHYO,		ZAHYO_WINDOW,
	ID_WINDOW_GAMEMESSAGE,	GAMEMESSAGE_WINDOW,
	ID_WINDOW_CONTEMESSAGE,	CONTEMESSAGE_WINDOW,
	ID_WINDOW_EFFECT,		EFFECT_WINDOW,
	ID_WINDOW_STORY,		STORY_WINDOW,
	ID_WINDOW_CONTROL,		CONTROL_WINDOW,
	ID_WINDOW_FILMCASE,		FILMCASE_WINDOW,
	ID_WINDOW_VAR,			VAR_WINDOW,
	ID_WINDOW_PROGRAM,		PROGRAM_WINDOW,
	ID_WINDOW_STORYBOOK,	STORYBOOK_WINDOW,
	-1,-1,
};




TBBUTTON CMyFrameWindow::m_tbb[] = {

    {0,  ID_BUTTON_NEW,		TBSTATE_ENABLED, TBSTYLE_BUTTON , 0, 0},
    {1,  ID_BUTTON_OPEN,	TBSTATE_ENABLED, TBSTYLE_BUTTON , 0, 0},
    {2,  ID_BUTTON_SAVE,	TBSTATE_ENABLED, TBSTYLE_BUTTON , 0, 0},

	{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},

    {3, ID_BUTTON_FILMMODE, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP, 0, 0},
    {4, ID_BUTTON_STORYMODE, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECKGROUP, 0, 0},

	{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},

    {5, ID_BUTTON_CONFIG, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

	{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},


    {6,  ID_BUTTON_MAINSCREEN,	TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {7,  ID_BUTTON_CONTE,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {8,  ID_BUTTON_FILM,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {9,  ID_BUTTON_LAYER,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {10, ID_BUTTON_ZAHYO,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {11, ID_BUTTON_GAMEMESSAGE,	TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {12, ID_BUTTON_CONTEMESSAGE,TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},

    {13, ID_BUTTON_EFFECT,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {14, ID_BUTTON_STORY,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {15, ID_BUTTON_VAR,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {16, ID_BUTTON_CONTROL,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {17, ID_BUTTON_FILMCASE,	TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {18, ID_BUTTON_PROGRAM,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},
    {19, ID_BUTTON_STORYBOOK,	TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK, 0, 0},

	{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},

    {20, ID_BUTTON_PROJECT_BUILD_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {21, ID_BUTTON_PROJECT_BUILD_ALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {22, ID_BUTTON_PROJECT_ADD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {23, ID_BUTTON_PROJECT_EDIT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

	{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},

    {24, ID_BUTTON_COMPILE_ALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {25, ID_BUTTON_COMPILE_STORY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {26, ID_BUTTON_COMPILE_COMMAND, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {27, ID_BUTTON_BUILD_SPT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {28, ID_BUTTON_BUILD_TXT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

	{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},

    {29, ID_BUTTON_UTIL_COPYEFFECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {30, ID_BUTTON_UTIL_COPYEFFECT2, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

	{0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},

    {31, ID_BUTTON_HELP,		TBSTATE_ENABLED, TBSTYLE_BUTTON , 0, 0},
};


CMyFrameWindow::CMyFrameWindow(CMyApplicationBase* app)
{
	m_this = this;
	m_app = app;
	m_client = NULL;
	m_clientHWND = NULL;


	m_hInstance = m_app->GetMyInstance();
	m_hWnd = NULL;

//	int* aaa = new int[16];
//	aaa[16] = 0;	//for overrun test



//	m_hBitmapBG = LoadImage(NULL,"nnndir\\bmp\\back.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	m_wheelMouse = new CWheelMouse();

	RECT rc;
	m_app->GetFrameWindowRect(&rc);
	if ( (rc.right<=0) || (rc.bottom <= 0))
	{
		rc.left = 0;
		rc.top = 16;
		rc.right = 1280 + 2;
		rc.bottom = 1024 - 32;
	}



	m_toolBarBitmap = new CMyBitmap("nnndir\\setup\\bmp\\toolbar1.bmp");
	m_hToolBarBitmap = m_toolBarBitmap->GetHBitmap();


	m_hWnd = CreateWindow(MYCLASSNAME,NULL,WS_OVERLAPPEDWINDOW,
//						  CW_USEDEFAULT,CW_USEDEFAULT,
//						  CW_USEDEFAULT,CW_USEDEFAULT,
							rc.left,rc.top,
							rc.right,rc.bottom,
						  //NULL,hMenuInit,m_hInstance,NULL);
						  NULL,NULL,m_hInstance,NULL);





}


CMyFrameWindow::~CMyFrameWindow()
{
	End();
}


void CMyFrameWindow::End(void)
{
	ENDDELETECLASS(m_toolBarBitmap);

	if (m_client != NULL)
	{
//		SetWindowLong(m_clientHWND,GWL_WNDPROC,m_oldClientProc);
		ENDDELETECLASS(m_client);
	}


	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
	ENDDELETECLASS(m_wheelMouse);
//	DELETEGDIOBJECT(m_hBitmapBG);
}


HWND CMyFrameWindow::GetHWND(void)
{
	return m_hWnd;
}


LRESULT CALLBACK CMyFrameWindow::staticFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (m_this != NULL)
	{
		return m_this->FrameWndProc(hWnd,message,wParam,lParam);
	}

    return DefFrameProc(hWnd,m_clientHWND,message,wParam,lParam);


//	return DefWindowProc(hWnd,message,wParam,lParam);
}


LRESULT CMyFrameWindow::FrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	static HWND hToolBar;
	if (m_wheelMouse != NULL)
	{
		message = m_wheelMouse->TranslateMessage(message);
	}

	if (message == WM_LBUTTONDOWN)
	{
	//	OutputDebugString("L");
	}

	if (message == WM_MOUSEWHEEL)
	{
		//OutputDebugString("wheel");
	}


//	if (message == WM_COMMAND)
//	{
//		switch (wParam)
//		{
//		}
//
//		if (wParam == IDM_DUMMY)
//		{
//			m_app->dmy();
//		}
//
//		
//	}


//static char tmptext[] = "abcde";

	if (message == WM_CLOSE)
	{
		if (m_app->CheckModify())
		{
			int rt = MessageBox(m_hWnd,"変更されています。\n保存しますか","警告",MB_YESNOCANCEL | MB_APPLMODAL | MB_ICONEXCLAMATION );
			if (rt == IDCANCEL) return 0;
			if (rt == IDYES) m_app->Save();
		}

		RECT rc;
		GetWindowRect(hWnd,&rc);
		rc.right -= rc.left;
		rc.bottom -= rc.top;

		m_app->SetFrameWindowRect(&rc);

		PostQuitMessage(0);
	}

	if (message == WM_CREATE)
	{

		
//	static HWND hToolBar;
//	TBBUTTON tb = {0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0};
	InitCommonControls();



	m_hToolBar = CreateToolbarEx(


				hWnd, //親ウィンドウ
//				m_clientHWND, //親ウィンドウ
                WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS,//| CCS_NOPARENTALIGN |CCS_NOMOVEY | CCS_NODIVIDER|CCS_NORESIZE, //ウィンドウスタイル
//                IDR_TOOLBAR1, // コントロールID
                667, // コントロールID
                31, //イメージの数
//                m_hInstance,
//				(HINSTANCE)GetWindowLong(m_clientHWND, GWL_HINSTANCE),
  //              IDR_TOOLBAR1,
				NULL,(UINT)m_hToolBarBitmap,
                m_tbb,
                39, // ボタンの数
                0, //ボタンの幅
                0, //ボタンの高さ
                16, //イメージの幅
                15, //イメージの高さ
                sizeof(TBBUTTON));
	
//	SendMessage(hToolBar, TB_INSERTBUTTON, 2, (LPARAM)&tb);
		


//char szBuf[64];
//   LoadString(m_hInstance, ID_BUTTON_STORYBOOK, szBuf, sizeof(szBuf) - 1);
//    m_tbb[20].iString = SendMessage(m_hToolBar, TB_ADDSTRING, 0, (LPARAM)szBuf);







    m_statusBar = CreateWindowEx(
        0,        //拡張スタイル
        STATUSCLASSNAME,    //ウィンドウクラス
        NULL,    //タイトル
        WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE,    //ウィンドウスタイル
                //WS_VISIBLEを指定しないときは必ずShowWindow関数を呼ぶ
        0,    //Ｘ座標
        0,    //Ｙ座標
        0,    //ウィンドウ幅
        0,    //高さ
        hWnd,    //親ウィンドウ
        (HMENU)1300,//ID_STATUS,    //ステータスバーのＩＤ	てきとうにゃ
        m_hInstance,    //インスタンスハンドル
        NULL);
    SendMessage(m_statusBar, SB_SIMPLE, TRUE, 0L);










		CLIENTCREATESTRUCT ccs;

		ccs.hWindowMenu  = NULL;
		ccs.idFirstChild = ID_MDICHILD;
/*
		m_clientHWND = CreateWindow("MDICLIENT",NULL,
							  WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|
							  WS_HSCROLL|WS_VSCROLL
							  | MDIS_ALLCHILDSTYLES ,
//							  0,0,0,0,hWnd,lpcs->hMenu,
							  0,0,0,0,
							  hWnd,NULL,
							  m_hInstance,(LPVOID)&ccs);
*/

		m_client = new CMyClientWindow(hWnd,m_hInstance);
		m_clientHWND = m_client->GetHWND();

//		m_oldClientProc =  GetWindowLong(m_clientHWND,GWL_WNDPROC);
//		SetWindowLong(m_clientHWND,GWL_WNDPROC,(long)(CMyClientWindow::staticClientWndProc));


	}

	if (message == WM_PAINT)
	{
//		SendMessage(m_clientHWND,message,wParam,lParam);
//		PAINTSTRUCT ps;
//		BeginPaint(hWnd,&ps);
//		TextOut(ps.hdc,100,100,"あいうえお",10);
//		EndPaint(hWnd,&ps);
	}


	if (message == WM_ERASEBKGND)
	{
//		if (hWnd == m_hWnd)
//		{
//			SendMessage(m_clientHWND,message,wParam,lParam);
//		}
//
//		if (hWnd == m_clientHWND)
//		{
//			int hhh = 0;
//			hhh++;
//		}

//		SendMessage(m_clientHWND,message,wParam,lParam);
	}


	switch (message)
	{
//	case WM_ERASEBKGND:
//		return TRUE;
//		break;

	case WM_MOVE:
		m_app->OnMove(wParam,lParam);
		break;

//	case WM_CHAR:
//		char mes[256];
//		sprintf_s(mes,"[frame window key=%d]",wParam);
//		OutputDebugString(mes);
//		break;


/*
	case WM_ERASEBKGND:
OutputDebugString("Erase,");
		if (m_hBitmapBG != NULL)
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd,&ps);

			HDC hdcErase = (HDC)wParam;
			HDC src = CreateCompatibleDC(hdcErase);
			HGDIOBJ old = SelectObject(src,m_hBitmapBG);
			BitBlt(hdcErase,0,0,1600,1200,src,0,0,SRCCOPY);
			SelectObject(src,old);
			DeleteDC(src);

			EndPaint(hWnd,&ps);

OutputDebugString("back");
			return 0;
		}
*/

/*
	case WM_PAINT:
		if (m_hBitmapBG != NULL)
		{
			PAINTSTRUCT ps;
			RECT rc;
			if (GetUpdateRect(hWnd,&rc,TRUE))
			{
				return 0;

				BeginPaint(hWnd,&ps);
OutputDebugString("WM_PAINT ");
				HDC hdcErase = ps.hdc;
				HDC src = CreateCompatibleDC(hdcErase);
				HGDIOBJ old = SelectObject(src,m_hBitmapBG);
		//		BitBlt(hdcErase,0,0,1600,1200,src,0,0,SRCCOPY);
				SelectObject(src,old);
				DeleteDC(src);

				EndPaint(hWnd,&ps);
				return 0;
			}
		}

		break;
*/
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case TTN_NEEDTEXT:
			LPTOOLTIPTEXT lptip;
			lptip = (LPTOOLTIPTEXT)lParam;

//			if (lptip->uFlags & TTF_IDISHWND)
//			{
//				OutputDebugString("hwnd ");
//			}
//			else
//			{
//				OutputDebugString("id ");
//			}

/*
TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst = AfxGetResourceHandle();
            return(TRUE);
        }
    }
    return(FALSE);

*/

//break;

			UINT nID = ((LPNMHDR)lParam)->idFrom;
    
			if (lptip->uFlags & TTF_IDISHWND)
			{
				// idFrom is actually the HWND of the tool
				nID = GetDlgCtrlID((HWND)nID);
			}

			if(nID)
			{
				switch (lptip->hdr.idFrom)
				{
				case ID_BUTTON_FILMMODE:
				case ID_BUTTON_STORYMODE:
	//			case ID_BUTTON_CCC:

				case ID_BUTTON_NEW:
				case ID_BUTTON_OPEN:
				case ID_BUTTON_SAVE:

				case ID_BUTTON_CONFIG:

				case ID_BUTTON_MAINSCREEN:
				case ID_BUTTON_CONTE:
				case ID_BUTTON_FILM:
				case ID_BUTTON_LAYER:
				case ID_BUTTON_ZAHYO:
				case ID_BUTTON_EFFECT:
				case ID_BUTTON_GAMEMESSAGE:
				case ID_BUTTON_STORY:
				case ID_BUTTON_VAR:
				case ID_BUTTON_CONTROL:
				case ID_BUTTON_FILMCASE:
				case ID_BUTTON_PROGRAM:
				case ID_BUTTON_STORYBOOK:

				case ID_BUTTON_PROJECT_BUILD_NEW:
				case ID_BUTTON_PROJECT_BUILD_ALL:
				case ID_BUTTON_PROJECT_ADD:
				case ID_BUTTON_PROJECT_EDIT:

				case ID_BUTTON_COMPILE_ALL:
				case ID_BUTTON_COMPILE_STORY:
				case ID_BUTTON_COMPILE_COMMAND:
				case ID_BUTTON_BUILD_SPT:
				case ID_BUTTON_BUILD_TXT:

				case ID_BUTTON_UTIL_COPYEFFECT:
				case ID_BUTTON_UTIL_COPYEFFECT2:

				case ID_BUTTON_HELP:

				case ID_BUTTON_CONTEMESSAGE:

					//lptip->lpszText = MAKEINTRESOURCE(lptip->hdr.idFrom);
					lptip->lpszText = MAKEINTRESOURCE(nID);
//static char aaa[] = "あいうえお";

//SetStatusMessage("あいうえお");

					
//					lptip->lpszText = aaa;
//					lptip->hinst = NULL;

					lptip->hinst = m_hInstance;
//					return (TRUE);
					break;
				}

			}

//			default:
//OutputDebugString("NO ");
//				break;

			break;
		}

//		return 0;
		break;



	case WM_COMMAND:
		CheckWindowButton(wParam);
		CheckWindowMenu(wParam);


		m_app->OnCommand(wParam,lParam);

//		if (wParam == ID_BUTTON_CCC) m_app->dmy();

//		if (wParam == ID_BUTTON_BBB)
//		{
//			int dat = SendMessage(m_hToolBar,TB_GETSTATE,ID_BUTTON_BBB,0);
//			char mes[256];
//			sprintf(mes,"%d %d %d \n",wParam,lParam,dat);
//			if (dat & TBSTATE_CHECKED)
//			{
//				OutputDebugString("Pressed\n");
//			}
//			else
//			{
//				OutputDebugString("Released\n");
//			}
//
//			OutputDebugString(mes);
////			MessageBox(NULL,"あああ","あああ",MB_OK);
//	/	}
//		switch(wParam)
//		{
//		case ID_WINDOW_MAINSCREEN:
//			break;
//		}
		break;
	}


	if (message == WM_DESTROY)
	{
		DELETECLASS(m_client);
//		if (m_clientHWND != NULL) DestroyWindow(m_clientHWND);
		m_clientHWND = NULL;
		m_hWnd = NULL;
	}

	if (message == WM_SIZE)
//	if (
	{

    SendMessage(m_hToolBar, TB_AUTOSIZE, 0, 0);
	SendMessage(m_statusBar, WM_SIZE, wParam, lParam);



	RECT rcToolBar;
    GetWindowRect(m_hToolBar, &rcToolBar);

	RECT rcStatusBar;
	GetWindowRect(m_statusBar,&rcStatusBar);

	if (m_clientHWND != NULL)
	{
		MoveWindow(
			m_clientHWND,   // MDIクライアントのハンドル
			0, 
			rcToolBar.bottom - rcToolBar.top,
			LOWORD(lParam), 
			HIWORD(lParam) - (rcToolBar.bottom - rcToolBar.top) - (rcStatusBar.bottom - rcStatusBar.top), 
			TRUE);
	}

	return 0;
/*
//		RECT   rcClient;
		RECT toolRect;

		GetClientRect(hToolBar,&toolRect);
		SetWindowPos(hToolBar,0, 0,0, LOWORD(lParam),toolRect.bottom,SWP_NOMOVE);
		GetWindowRect(hToolBar,&toolRect);
		SetWindowPos(m_clientHWND,0, 0,toolRect.bottom - toolRect.top,LOWORD(lParam),HIWORD(lParam),0);
 */

//		SendMessage(hToolBar,message,wParam,lParam);
	}



//	if (hWnd != m_hWnd)return DefWindowProc(hWnd,message,wParam,lParam);

//	return DefFrameProc(hWnd,m_clientHWND,message,wParam,lParam);

    return DefFrameProc(hWnd,m_clientHWND,message,wParam,lParam);

//	return DefWindowProc(hWnd,message,wParam,lParam);
}

HWND CMyFrameWindow::GetMyClientHWND(void)
{
	return m_clientHWND;
}


void CMyFrameWindow::CheckWindowButton(WPARAM wParam)
{
	int n = -1;
	for (int i=0;i<33;i++)
	{
		int id = m_windowButtonID[i*2];
		if (id == -1) break;

		if ((int)wParam == id)
		{
			n = m_windowButtonID[i*2+1];
			break;
		}
	}

	if (n == -1) return;

	int dat = SendMessage(m_hToolBar,TB_GETSTATE,wParam,0);
	if (dat & TBSTATE_CHECKED)
	{
		m_app->ChangeShowHideWindow(n);
	}
	else
	{
		m_app->ChangeShowHideWindow(n);
	}
}


void CMyFrameWindow::CheckWindowMenu(WPARAM wParam)
{
	int n = -1;
	for (int i=0;i<33;i++)
	{
		int id = m_windowMenuID[i*2];
		if (id == -1) break;

		if ((int)wParam == id)
		{
			n = m_windowMenuID[i*2+1];
			break;
		}
	}

	if (n == -1) return;
	m_app->ChangeShowHideWindow(n);
}


void CMyFrameWindow::SetCheckMenuAndButton(int windowNumber, BOOL md)
{
	int id = -1;
	int i;
	for (i=0;i<33;i++)
	{
		int n = m_windowButtonID[i*2+1];
		if (n == -1) break;

		if (windowNumber == n)
		{
			id = m_windowButtonID[i*2];
			break;
		}
	}

	if (id != -1)
	{
		//set button status
	}



	id = -1;
	for (i=0;i<33;i++)
	{
		int n = m_windowMenuID[i*2+1];
		if (n == -1) break;

		if (windowNumber == n)
		{
			id = m_windowMenuID[i*2];
			break;
		}
	}

	if (id != -1)
	{
		//set menu status
	}

}


void CMyFrameWindow::CheckWindow(int windowNumber, BOOL flg)
{
	int bid = -1;

	int i;
	for (i=0;i<14;i++)
	{
		if (m_windowButtonID[i*2+1] == windowNumber)
		{
			bid = m_windowButtonID[i*2];
			break;
		}
	}

	int mid = -1;
	for (i=0;i<14;i++)
	{
		if (m_windowMenuID[i*2+1] == windowNumber)
		{
			mid = m_windowMenuID[i*2];
			break;
		}
	}

	if ((bid != -1) && (mid != -1))
	{
		CheckButtonAndMenu(bid,mid,flg);
	}


}

void CMyFrameWindow::SetCheckMenu(int menuID, BOOL flg)
{
	HMENU menu = GetMenu(m_hWnd);
	MENUITEMINFO mi;
	
	ZeroMemory(&mi,sizeof(mi));

	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_STATE;// | MIIM_ID;

	if (flg)
	{
		mi.fState = MFS_CHECKED;
	}
	else
	{
		mi.fState = MFS_UNCHECKED;
	}

	SetMenuItemInfo(menu,menuID,FALSE,&mi);
}


void CMyFrameWindow::CheckButtonAndMenu(int buttonID, int menuID, BOOL flg)
{
	HMENU menu = GetMenu(m_hWnd);
	MENUITEMINFO mi;
	
	ZeroMemory(&mi,sizeof(mi));

	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_STATE;// | MIIM_ID;

	if (flg)
	{
		mi.fState = MFS_CHECKED;
	}
	else
	{
		mi.fState = MFS_UNCHECKED;
	}

	SetMenuItemInfo(menu,menuID,FALSE,&mi);

	WPARAM wParam = (WPARAM)buttonID;
	LPARAM lParam;
	if (flg)
	{
		lParam = (LPARAM)MAKELONG(TBSTATE_ENABLED | TBSTATE_CHECKED,0);
	}
	else
	{
		lParam = (LPARAM)MAKELONG(TBSTATE_ENABLED,0);
	}

	SendMessage(m_hToolBar,TB_SETSTATE,wParam,lParam);

}

void CMyFrameWindow::SetStatusMessage(LPSTR mes)
{
	if (mes == NULL) return;

	char buf[128];
	int ln = strlen(mes);
	if (ln>126) ln = 126;
	memcpy(buf,mes,ln);
	buf[ln] = 0;
	buf[ln+1] = 0;

	SendMessage(m_statusBar,SB_SETTEXT,(WPARAM)(255 | 0 ),(LPARAM)buf);
}


void CMyFrameWindow::ToTop(HWND hwnd)
{
	m_client->ToTop(hwnd);
}


void CMyFrameWindow::ActiveChildWindow(int windowNumber)
{

}



