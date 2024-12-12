//
// varview.h
//

#if !defined __NNNEDIT_VARVIEW__
#define __NNNEDIT_VARVIEW__

#include "scrollview.h"

#include "resource.h"

class CMyBitmap;

class CMyButton;
//class CVarInitData;

class CVarView : public CMyScrollView
{
public:
	CVarView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CVarView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

//	void ReCalcuScrollPara(void);

	static int m_markColorTable[];

	static int m_textColorTable[];
	static int m_backColorTable[];

	void VarBlockIsChanged(void);

	void OnLeftKey(void);
	void OnRightKey(void);
	BOOL MoveMouse(int x,int y,POINT screenPos);
	BOOL OnControlKey(WPARAM wParam,LPARAM lParam);


private:
	HBITMAP m_hBitmap;
	CMyButton* m_buttonSave;
	CMyButton* m_buttonCut;
	CMyButton* m_buttonCopy;
	CMyButton* m_buttonPaste;
	CMyButton* m_buttonSearch;

	CMyBitmap* m_bitmap;
	CMyBitmap* m_searchBitmap;

	void CheckChangeMarkColor(int n,int cmd);

	int m_varBlockKosuu;
	int m_modeButtonPrintX;


//	CVarInitData* m_varInitData;

};


#endif
/*_*/

