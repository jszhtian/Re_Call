//
// starttitle.h
//

#if !defined __TINYAN_NNNEDIT_STARTTITLE__
#define __TINYAN_NNNEDIT_STARTTITLE__

class CMyBitmap;
class CStartTitle
{
public:
	CStartTitle(HWND clientHWND, HINSTANCE hinstance);
	~CStartTitle();
	void End(void);

	static LRESULT CALLBACK staticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	static CStartTitle* m_this;
	void Show(void);
	void Start(void);

private:
	HWND m_clientHWND;
	HINSTANCE m_hInstance;

	HWND m_hWnd;

	CMyBitmap* m_bitmap;
	HBITMAP m_hBitmap;

	BOOL m_timerFlag;
};


#endif
/*_*/

