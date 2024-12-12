//
// main.h
//

#if !defined __TINYAN_WD2_MAIN__
#define __TINYAN_WD2_MAIN__


class CGame;
class CWheelMouse;
class CCommonSystemFile;

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

CGame* m_game;
//CWheelMouse* m_wheelMouse;
//CCommonSystemFile* m_systemFile;
CMainControl* m_mainControl;

//BOOL m_coInitFlag;
//int ReturnFinish(void);

#endif

/*_*/

