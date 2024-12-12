//
// main.h
//

#if !defined __TINYAN_NNNEDIT_MAIN__
#define __TINYAN_NNNEDIT_MAIN__

class CMyApplication;

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

int m_dskTopWindowSizeX;
int m_dskTopWindowSizeY;
int m_dskTopColorBits;

HINSTANCE m_hInstance;
HINSTANCE m_hInstancePrev;

HWND m_hWnd;
HWND m_hWndClient;


CMyApplication* m_app;

#endif

/*_*/

