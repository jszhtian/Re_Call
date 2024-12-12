//
// effectview.h
//

#if !defined __NNNEDIT_EFFECTWVIEW__
#define __NNNEDIT_EFFECTVIEW__

#include "myview.h"

#include "resource.h"

class CPrintMessage;
//class CEffectParaList;
class CEditSuuji;

class CMyBitmap;

class CEffectView : public CMyView
{
public:
	CEffectView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CEffectView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );



	static int m_deltaTable[];
//	int GetNowSelectNumber(void){return m_nowSelect;}

	void OnLeftKey(void);
	void OnRightKey(void);
	void OnUpKey(void);
	void OnDownKey(void);

	void OnEnterKey(void);

	BOOL MoveMouse(int x,int y,POINT screenPos);

private:
	CPrintMessage* m_message;

//	static int m_paraRange[][32][2];
//	static char m_paraName[][16];

//	static EFFECTPARAM m_effectParamTable[100];

	void ClickRoutineNormalMode(int x, int y);
//	void ClickRoutineSelectMode(int x, int y);

	void PrintNormalMode(HWND hwnd,WPARAM wParam,LPARAM lParam);
//	void PrintSelectMode(HDC hdc, HDC src);

//	CEffectParaList* m_effectParaList;

	HBITMAP m_hBitmapSel;
	HBITMAP m_hBitmapDelta;

	HBITMAP m_hBitmapFont;
	CEditSuuji* m_suuji;

//	int m_editKosuu;
//	int m_nowSelect;

	int m_deltaButtonPrintX;
	int m_deltaButtonPrintY;
	int m_deltaButtonNextX;
	int m_deltaButtonNextY;
	int m_deltaButtonSizeX;
	int m_deltaButtonSizeY;

	CMyBitmap* m_bitmapSel;
	CMyBitmap* m_bitmapDelta;
	int AdjustCenterX(int d);
	int AdjustCenterY(int d);

};


#endif
/*_*/

