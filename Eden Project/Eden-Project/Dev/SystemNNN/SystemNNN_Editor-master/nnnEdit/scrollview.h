//
// scrollview.h
//

#if !defined __NNNEDIT_MYSCROLLVIEW__
#define __NNNEDIT_MYSCROLLVIEW__

#include "myview.h"

class CMyScrollView : public CMyView
{
public:
	CMyScrollView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	virtual ~CMyScrollView();
	void End(void);

	virtual LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;

	virtual void ReCalcuScrollPara(void);

	virtual void ScrollTo(int n);

	virtual void OnLeftKey(void);
	virtual void OnRightKey(void);
	virtual void OnUpKey(void);
	virtual void OnDownKey(void);
	virtual void OnPageUp(void);
	virtual void OnPageDown(void);

protected:
	void SetScrollArea(int startX, int startY, int endX, int endY);
	void SetScrollPara(int pos, int posMax = -1, int page = -1, int vh = -1);
	void OnScroll(UINT message, WPARAM wParam, LPARAM lParam);

	void OnWheelScroll(UINT message, WPARAM wParam, LPARAM lParam);

	int GetMyScrollPos(int vh = -1);
	int GetBlockKosuu(int vh = -1);

	int GetContentsNumber(int x, int y);


	int m_scrollPos[2];
	int m_scrollMax[2];
	int m_scrollPage[2];


	int m_scrollAreaStartX;
	int m_scrollAreaStartY;
	int m_scrollAreaEndX;
	int m_scrollAreaEndY;

	void CheckInWindow(UINT message, WPARAM wParam, LPARAM lParam);

	int m_oldSelect;
	void UpdateBlock(int n);

private:

};


#endif
/*_*/
