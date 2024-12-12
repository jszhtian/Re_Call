//
// myframewindow.h
//

#if !defined __NNNEDIT_MYFRAMEWINDOW__
#define __NNNEDIT_MYFRAMEWINDOW__

class CWheelMouse;
class CMyApplicationBase;

class CMyClientWindow;

class CMyBitmap;
class CMyFrameWindow
{
public:
	CMyFrameWindow(CMyApplicationBase* app);
	~CMyFrameWindow();
	void End(void);

	static CMyFrameWindow* m_this;
	static LRESULT CALLBACK staticFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	static HWND m_clientHWND;

	static TBBUTTON m_tbb[];

	static int m_windowButtonID[];
	static int m_windowMenuID[];

	LRESULT FrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	HWND GetHWND(void);
	HWND GetMyClientHWND(void);

	void SetCheckMenuAndButton(int windowNumber, BOOL md = TRUE);
	
	void SetCheckMenu(int menuID,BOOL flg = TRUE);

	void CheckWindow(int windowNumber, BOOL flg = TRUE);

	void CheckButtonAndMenu(int buttonID, int menuID, BOOL flg = TRUE);

	void SetStatusMessage(LPSTR mes);

	void ToTop(HWND hwnd);
	void ActiveChildWindow(int windowNumber);
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	CMyApplicationBase* m_app;

	void CheckWindowButton(WPARAM wParam);
	void CheckWindowMenu(WPARAM wParam);

	HWND m_hToolBar;
	HWND m_statusBar;

	CWheelMouse* m_wheelMouse;

	HANDLE m_hBitmapBG;
	CMyClientWindow* m_client;
	static long m_oldClientProc;

	CMyBitmap* m_toolBarBitmap;
	HBITMAP m_hToolBarBitmap;

};

#endif
/*_*/
