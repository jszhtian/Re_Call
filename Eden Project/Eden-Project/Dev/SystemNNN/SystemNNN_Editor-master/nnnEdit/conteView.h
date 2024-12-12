//
// Conteview.h
//

#if !defined __TINYAN_NNNEDIT_CONTEVIEW__
#define __TINYAN_NNNEDIT_CONTEVIEW__

#include "scrollview.h"

#include "resource.h"

#define PIC_KOSUU_MAX 16


//class CMyDirectX;
class CMyButton;

class CMiniPicCache;
class CMyFileOpen;
class CMyBitmap;

class CConteView : public CMyScrollView
{
public:
	CConteView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CConteView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	BOOL MoveMouse(int x,int y,POINT screenPos);
private:
	HFONT m_font;
	int m_fontSize;
	HBITMAP m_hBitmapFilm64x48;
	CMyBitmap* m_filmBitmap;


};


#endif
/*_*/

