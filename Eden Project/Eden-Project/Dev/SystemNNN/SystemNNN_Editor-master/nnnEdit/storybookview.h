//
// storybookview.h
//

#if !defined __NNNEDIT_STORYBOOKVIEW__
#define __NNNEDIT_STORYBOOKVIEW__

class CMyScrollView;
#include "scrollview.h"

//#include "resource.h"

class CMyButton;
class CStoryBookView : public CMyScrollView
{
public:
	CStoryBookView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CStoryBookView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	BOOL MoveMouse(int x,int y,POINT screenPos);
	BOOL OnControlKey(WPARAM wParam,LPARAM lParam);

//	void ReCalcuScrollPara(void);
private:
	CMyButton* m_buttonNew;
	CMyButton* m_buttonOpen;
	CMyButton* m_buttonSave;
	CMyButton* m_buttonDelete;
	CMyButton* m_buttonUndo;
	CMyButton* m_buttonSearch;

};


#endif
/*_*/

