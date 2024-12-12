//
// ConteMessageView.h
//

#if !defined __TINYAN_NNNEDIT_CONTEMESSAGEVIEW__
#define __TINYAN_NNNEDIT_CONTEMESSAGEVIEW__

#include "myview.h"

#include "resource.h"



class CConteMessageView : public CMyView
{
public:
	CConteMessageView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CConteMessageView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	BOOL MoveMouse(int x,int y,POINT screenPos);

private:
	HFONT m_font;

	void OnPaint(HWND hwnd, HDC hdc, RECT rc);

	int m_fontSize;

};


#endif
/*_*/

