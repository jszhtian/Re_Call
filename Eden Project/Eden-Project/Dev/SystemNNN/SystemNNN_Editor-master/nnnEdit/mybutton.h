//
// mybutton.h
//

#if !defined __NNNEDIT_MYBUTTON__
#define __NNNEDIT_MYBUTTON__

#define ID_USER_LBUTTONDOWN 1500
#define ID_USER_LBUTTONUP 1501
#define ID_USER_LBUTTONDBLCLK 1502

#define ID_USER_RBUTTONDOWN 1510
#define ID_USER_RBUTTONUP 1511
#define ID_USER_RBUTTONDBLCLK 1512

class CMyButton
{
public:
	CMyButton(int n, HWND parentHWND, HBITMAP hbitmap, int printX, int printY, int sizeX = 24, int sizeY = 24);
	virtual ~CMyButton();
	void End(void);

	void Print(HDC hdc, HDC src = NULL,int md = -1, RECT* lpRect = NULL);

	//calcu

	void CalcuLButtonDown(WPARAM wParam, LPARAM lParam);
	BOOL CheckOn(int x,int y);
	int GetNumber(void);

protected:
	int m_number;
	HWND m_parentHWND;

	int m_printX;
	int m_printY;
	int m_sizeX;
	int m_sizeY;

	int m_mode;
private:
	HBITMAP m_hBitmap;


};


#endif
/*_*/

