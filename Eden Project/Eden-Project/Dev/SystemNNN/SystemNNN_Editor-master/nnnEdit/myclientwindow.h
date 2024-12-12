//
// myclientwindow.h
//

#if !defined __TINYAN_NNNEDIT_MYCLIENTWINDOW__
#define __TINYAN_NNNEDIT_MYCLIENTWINDOW__


class CMyClientWindow
{
public:
	CMyClientWindow(HWND hwndParent, HINSTANCE hinstance);
	~CMyClientWindow();
	void End(void);

	HWND GetHWND(void);

	static LRESULT CALLBACK staticClientWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	void ToTop(HWND hwnd);
private:
	static CMyClientWindow* m_this;
	LRESULT ClientWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );


	HWND m_hWndParent;
	HWND m_hWnd;

	HINSTANCE m_hInstance;

	static long m_oldClientProc;


};



#endif
/*_*/

