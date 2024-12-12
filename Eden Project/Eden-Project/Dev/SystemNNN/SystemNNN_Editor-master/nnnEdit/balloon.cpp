//
// balloon.cpp
//

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
//#include "define.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "myBitmap.h"
#include "windowlist.h"

#include "balloon.h"

CBalloon* CBalloon::m_this = NULL;
char CBalloon::m_defaultHelpText[] = "HelpMessage";

CBalloon::NAMETYPELIST CBalloon::m_nameTypeList[]=
{
	{MAINSCREEN_WINDOW,"mainscreen"},
	{FILM_WINDOW,"film"},
	{LAYER_WINDOW,"layer"},
	{ZAHYO_WINDOW,"zahyo"},
	{GAMEMESSAGE_WINDOW,"message"},
	{EFFECT_WINDOW,"effect"},
	{STORY_WINDOW,"story"},
	{VAR_WINDOW,"var"},
	{PROGRAM_WINDOW,"program"},
	{FILMCASE_WINDOW,"filmcase"},
	{STORYBOOK_WINDOW,"storybook"},
	{CONTE_WINDOW,"conte"},
	{CONTEMESSAGE_WINDOW,"contemessage"},
	{CONTROL_WINDOW,"control"},

	{-1,""},
};



CBalloon::CBalloon(HWND clientHWND,HINSTANCE hinstance)
{
	m_clientHWND = clientHWND;
	m_hInstance = hinstance;

	m_helpText[0] = 0;

	m_waitTime = 20;
	m_waitCount = m_waitTime;

	m_typeCount = 0;
	for (int i=0;i<100;i++)
	{
		if (m_nameTypeList[i].type == -1) break;
		m_typeCount++;
	}

	for (int j=0;j<32;j++)
	{
		for (int i=0;i<32;i++)
		{
			m_listTable[j][i] = -1;
		}
	}

	m_helpList = new CNameList();
	m_helpList->LoadFile("nnndir\\setup\\list\\balloonhelp.txt");

	int n = m_helpList->GetNameKosuu();
	int n4 = n / 4;
	for (int i=0;i<n4;i++)
	{
		LPSTR typeName = m_helpList->GetName(i*4+0);
		int type = -1;

		if ((*typeName >= '0') && (*typeName <= '9'))
		{
			type = atoi(typeName);
		}
		else
		{
			type = SearchTypeName(typeName);
		}

		if ((type >= 0) && (type < 32))
		{
			int t = atoi(m_helpList->GetName(i*4+1));
			if ((t>=0) && (t<32))
			{
				m_listTable[type][t] = i;
				//$ to CR
				LPSTR name = m_helpList->GetName(i*4+3);
				int ln = strlen(name);
				for (int k=0;k<ln;k++)
				{
					if (*(name+k) == '$')
					{
						*(name+k) = 0x0d;
					}
				}
			}
		}
	}


//	m_bitmap = new CMyBitmap("nnndir\\setup\\bmp\\nekotitle.bmp");
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_TITLE));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_JOREMI));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_AEGI));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_BLACK));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_WA));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_NEKO));
//	m_hBitmap = LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_BURANE));

//	m_hBitmap = m_bitmap->GetHBitmap();

//	m_timerFlag = FALSE;

//	m_windowX = 0;
//	m_windowY = 0;
//	m_windowSizeX = 640;
//	m_windowSizeY = 480;

//	SetRect(&m_windowRect,0,0,64,64);
	m_windowSize.cx = 64;
	m_windowSize.cy = 64;
	m_windowPos.x = 0;
	m_windowPos.y = 0;

	m_hWnd = NULL;

	m_padding = 4;

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateSolidBrush(0x96cdea);
	wc.hCursor = NULL;
	wc.hIcon = NULL;
	wc.hInstance = m_hInstance;
	wc.lpfnWndProc = staticWndProc;
	wc.lpszClassName = "balloonclass";
	wc.lpszMenuName = NULL;
	wc.style = 0;

    RegisterClass(&wc);

	HWND hwnd0 = GetDesktopWindow();

	HDC hdc0 = GetDC(hwnd0);
	int dskX0 = GetDeviceCaps(hdc0,HORZRES);
	int dskY0 = GetDeviceCaps(hdc0,VERTRES);
	ReleaseDC(hwnd0,hdc0);


	RECT rc;
	GetWindowRect(clientHWND,&rc);

	int dskX = (rc.right - rc.left) / 2;
	int dskY = (rc.bottom - rc.top) / 2;

//	SIZE size = m_bitmap->GetSize();

	int sx = 64;
	int sy = 64;

	int wx = dskX0 / 2 - sx / 2;
	int wy = dskY0 / 2 - sy / 2;

	m_sizeCalcued = FALSE;
	m_calcuedHelpNumber = -2;

	m_windowPos.x = wx;
	m_windowPos.y = wy;
	m_windowSize.cx = sx;
	m_windowSize.cy = sy;

//	m_hWnd = CreateWindow("starttitleclass","",WS_POPUP | WS_VISIBLE,
	m_hWnd = CreateWindow("balloonclass","",WS_POPUP,
					wx,wy,
					sx,sy,

					m_clientHWND,
					//NULL,

					NULL,
					m_hInstance,NULL);

	m_this = this;
	m_showMode = FALSE;
	m_balloonNumber = -1;
	m_subType = -1;
	m_mousePoint.x = 0;
	m_mousePoint.y = 0;
	m_printZahyo.x = 0;
	m_printZahyo.y = 0;

}





CBalloon::~CBalloon()
{
	End();
}

void CBalloon::End(void)
{
//	if (m_timerFlag)
//	{
//		m_timerFlag = FALSE;
//		KillTimer(m_hWnd,555);
//	}

	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
//	DELETEGDIOBJECT(m_hBitmap);
//	ENDDELETECLASS(m_bitmap);
	ENDDELETECLASS(m_helpList);
}


LRESULT CALLBACK CBalloon::staticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_this != NULL)
	{
		return m_this->MyWndProc(hWnd,message,wParam,lParam);
	}



	return DefWindowProc(hWnd,message,wParam,lParam);
}

LRESULT CBalloon::MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

//char mes1[256] = "アドレス 0x02719029で不正なアクセスが発生しました。";
//char mes2[256] = "EAX=8855648 EBX=7342345 ECX=7410643 EDX=6298882";
//char mes3[256] = "CTRL + ALT + DELを押してマシンを再起動してください。";

	switch (message)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;
//		HDC src;
//		HGDIOBJ old;
		int oldMode;
		RECT rc;

		hdc = BeginPaint(hWnd,&ps);
		SetRect(&rc,0,0,m_windowSize.cx,m_windowSize.cy);

		HBRUSH br;

//		br = CreateSolidBrush(0x96cdea);
//		Rectangle(hdc,0,0,m_windowSize.cx,m_windowSize.cy);
//		DeleteObject(br);


		br = CreateSolidBrush(0);
		FrameRect(hdc,&rc,br);
		DeleteObject(br);



		oldMode = SetBkMode(hdc,TRANSPARENT);
		SetRect(&rc,m_padding,m_padding,m_windowSize.cx-m_padding,m_windowSize.cy-m_padding);
		DrawText(hdc,m_helpText,-1,&rc,DT_LEFT | DT_WORDBREAK);
		SetBkMode(hdc,oldMode);
/*
HBRUSH br;
br = CreateSolidBrush(0xff0000);
RECT rc;
SetRect(&rc,0,0,1600,1200);
FillRect(hdc,&rc,br);
DeleteObject(br);
*/

//		src = CreateCompatibleDC(hdc);
//		old = SelectObject(src,m_hBitmap);
//		BitBlt(hdc,0,0,430,270,src,0,0,SRCCOPY);
//		SelectObject(src,old);
//		DeleteDC(src);
/*
COLORREF col,bk;
int md;
col = GetTextColor(hdc);
bk = GetBkColor(hdc);
md = GetBkMode(hdc);

SetBkMode(hdc,TRANSPARENT);
SetTextColor(hdc,COLORREF(0xffffff));
SetBkColor(hdc,COLORREF(0));

TextOut(hdc,480,500,mes1,strlen(mes1));
TextOut(hdc,480,550,mes2,strlen(mes2));
TextOut(hdc,480,580,mes3,strlen(mes3));

SetBkMode(hdc,md);
SetTextColor(hdc,col);
SetBkColor(hdc,bk);
*/

		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
//		PostMessage(hWnd,WM_CLOSE,0,0);
//		if (m_timerFlag)
//		{
//			KillTimer(hWnd,555);
//			m_timerFlag = FALSE;
//		}
		break;
	case WM_TIMER:
//		m_timerFlag = FALSE;
//		KillTimer(hWnd,555);
//		PostMessage(hWnd,WM_CLOSE,0,0);
		break;
	case WM_DESTROY:
		m_hWnd = NULL;
		break;
//	case WM_CLOSE:
//		DestroyWindow(hWnd);
//		break;
	}

	return DefWindowProc(hWnd,message,wParam,lParam);
}

void CBalloon::OnArea(int n,POINT pt,int subType,POINT screenPos)
{
	if (n < 0)
	{
		if (m_showMode)
		{
			ShowWindow(m_hWnd,SW_HIDE);
		//	SetWindowPos(m_hWnd,NULL,0,0,512,256,SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
			m_showMode = FALSE;
		}
		m_balloonNumber = -1;
		m_waitCount = m_waitTime;

		if (n == -2)
		{
			m_calcuedHelpNumber = -2;
		}
		if (n == -1)
		{
			m_calcuedHelpNumber = -2;
		}
	}
	else
	{
//		char mes[256];
//		sprintf_s(mes,256,"[%d %d:(%d,%d)]\x00d\x00a",n,subType,pt.x,pt.y);
//		OutputDebugString(mes);


		int helpNumber = -1;
		if ((n >= 0) && (n<32) && (subType >= 0) && (subType < 32)) helpNumber = m_listTable[n][subType];

		m_mousePos = pt;
		m_screenPos = screenPos;

		int oldBallonNumber = m_balloonNumber;
		m_balloonNumber = helpNumber;
		if (m_calcuedHelpNumber != helpNumber)
		{
			if (m_showMode)
			{
				ShowWindow(m_hWnd,SW_HIDE);
		//		SetWindowPos(m_hWnd,NULL,0,0,512,256,SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
				m_showMode = FALSE;
			}

			if (oldBallonNumber < 0)
			{
				m_waitCount = m_waitTime;
			}
			else
			{
				m_waitCount = 2;
			}
		}


		/*

		LPSTR helpText = m_defaultHelpText;
		if (helpNumber >= 0)
		{
			helpText = m_helpList->GetName(helpNumber*4+3);
		}
		int ln = strlen(helpText);
		memcpy(m_helpText,helpText,ln+1);

		int oldBallonNumber = m_balloonNumber;
		m_balloonNumber = helpNumber;

		if (m_calcuedHelpNumber != helpNumber)
		{
			if (oldBallonNumber < 0)
			{
				m_waitCount = m_waitTime;
			}
			else
			{
				m_waitCount = 2;
			}

			HDC hdc = GetDC(m_hWnd);
			//HDC hdc = GetDC(NULL);
			RECT rc;
			SetRect(&rc,0,0,512,256);
			DrawText(hdc,m_helpText,-1,&rc,DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
			ReleaseDC(m_hWnd,hdc);
			m_windowSize.cx = rc.right - rc.left + m_padding*2;
			m_windowSize.cy = rc.bottom - rc.top + m_padding*2;

			m_calcuedHelpNumber = helpNumber;
			m_sizeCalcued = TRUE;

		//	RECT rc;
		//	GetWindowRect(m_hWnd,&rc);
		//	m_windowPos.x = rc.left;
		//	m_windowPos.y = rc.top;

			m_windowPos.x = pt.x + 50;
			m_windowPos.y = pt.y + 50;

			
			sprintf_s(mes,256,"[setpos:(%d,%d)]\x00d\x00a",pt.x,pt.y);
			OutputDebugString(mes);

			if (m_showMode)
			{
				ShowWindow(m_hWnd,SW_HIDE);
				m_showMode = FALSE;
			}

		//	SetWindowPos(m_hWnd,NULL,m_windowPos.x,m_windowPos.y,m_windowSize.cx+1,m_windowSize.cy+1,SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
			SetWindowPos(m_hWnd,NULL,m_windowPos.x,m_windowPos.y,m_windowSize.cx,m_windowSize.cy,SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
//			MoveWindow(m_hWnd,m_windowPos.x,m_windowPos.y,m_windowSize.cx,m_windowSize.cy,FALSE);



		}
	*/

	}
}

void CBalloon::OnTimer(void)
{
	if (m_balloonNumber < 0) return;

	if (m_waitCount > 0)
	{
		m_waitCount--;
		if (m_waitCount == 0)
		{
			if (m_calcuedHelpNumber != m_balloonNumber)
			{

				LPSTR helpText = m_defaultHelpText;
				if (m_balloonNumber >= 0)
				{
					helpText = m_helpList->GetName(m_balloonNumber*4+3);
				}

				int ln = strlen(helpText);
				memcpy(m_helpText,helpText,ln+1);


				HDC hdc = GetDC(m_hWnd);
				RECT rc;

				SetRect(&rc,0,0,512,256);
				DrawText(hdc,m_helpText,-1,&rc,DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
				ReleaseDC(m_hWnd,hdc);
				m_windowSize.cx = rc.right - rc.left + m_padding*2;
				m_windowSize.cy = rc.bottom - rc.top + m_padding*2;

				m_calcuedHelpNumber = m_balloonNumber;
				m_sizeCalcued = TRUE;


				m_windowPos.x = m_screenPos.x + 16;
				m_windowPos.y = m_screenPos.y + 24;


		//	SetWindowPos(m_hWnd,NULL,m_windowPos.x,m_windowPos.y,m_windowSize.cx+1,m_windowSize.cy+1,SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
				SetWindowPos(m_hWnd,NULL,m_windowPos.x,m_windowPos.y,m_windowSize.cx,m_windowSize.cy,SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
//			MoveWindow(m_hWnd,m_windowPos.x,m_windowPos.y,m_windowSize.cx,m_windowSize.cy,FALSE);




		
			}
		


			if (m_showMode == FALSE)
			{
				ShowWindow(m_hWnd,SW_SHOWNA);
				MoveWindow(m_hWnd,m_windowPos.x,m_windowPos.y,m_windowSize.cx,m_windowSize.cy,TRUE);

				m_showMode = TRUE;
			}
		}
	}
}

void CBalloon::Show(void)
{
	ShowWindow(m_hWnd,SW_SHOW);
}


int CBalloon::SearchTypeName(LPSTR name)
{
	for (int i=0;i<m_typeCount;i++)
	{
		if (_stricmp(name,m_nameTypeList[i].name) == 0)
		{
			return m_nameTypeList[i].type;
		}
	}

	return -1;
}

/*_*/

