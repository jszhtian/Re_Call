//
// controlview.h
//

#if !defined __TINYAN_NNNEDIT_CONTROLVIEW__
#define __TINYAN_NNNEDIT_CONTROLVIEW__

#include "myview.h"

#include "resource.h"

class CMyBitmap;

class CControlView : public CMyView
{
public:
	CControlView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CControlView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	static int m_buttonTable[];

	virtual void OnLeftKey(void);
	virtual void OnRightKey(void);
	virtual void OnUpKey(void);
	virtual void OnDownKey(void);
	BOOL MoveMouse(int x,int y,POINT screenPos);

private:
	HBITMAP m_hBitmap;

	void CheckButton(int x, int y);
	CMyBitmap* m_buttonBitmap;
};


#endif
/*_*/

