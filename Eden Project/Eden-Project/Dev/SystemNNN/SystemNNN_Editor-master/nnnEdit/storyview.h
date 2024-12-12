//
// storyview.h
//

#if !defined __TINYAN_NNNEDIT_STORYVIEW__
#define __TINYAN_NNNEDIT_STORYVIEW__

#include "scrollview.h"

#include "resource.h"


class CMyBitmap;

class CMyButton;
class CMyDIB;

class CStoryView : public CMyScrollView
{
public:
	CStoryView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CStoryView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

//	void ReCalcuScrollPara(void);

	static int m_markColorTable[];

	static int m_commandBackColorTable[];
	static int m_commandTextColorTable[];
	void CheckChangeMarkColor(int n,int cmd);

	BOOL MoveMouse(int x,int y,POINT screenPos);
	BOOL OnControlKey(WPARAM wParam,LPARAM lParam);


private:
	void OnClickLButton(WPARAM wParam, LPARAM lParam);

	void OnPaint(HWND hwnd,WPARAM wParam, LPARAM lParam);


//	void CommandToType(int cmd);
//	void CommandToPara(int cmd);



	int m_danrakuX;

	CMyButton* m_buttonNew;

	CMyButton* m_buttonChange;
	CMyButton* m_buttonEdit;

	CMyButton* m_buttonCut;
	CMyButton* m_buttonCopy;
	CMyButton* m_buttonPaste;
	CMyButton* m_buttonDelete;

	CMyButton* m_buttonUndo;
	CMyButton* m_buttonDo;

	CMyButton* m_buttonBack;
	CMyButton* m_buttonJump;
	CMyButton* m_buttonFoward;

	HBITMAP m_hBitmap;

	CMyDIB* m_dib;

	CMyBitmap* m_bitmap;
};


#endif
/*_*/

