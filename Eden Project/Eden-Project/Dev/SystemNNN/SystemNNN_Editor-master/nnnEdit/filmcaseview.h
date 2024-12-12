//
// filmcaseview.h
//

#if !defined __TINYAN_NNNEDIT_FILMCASEVIEW__
#define __TINYAN_NNNEDIT_FILMCASEVIEW__

#include "scrollview.h"

#include "resource.h"

class CMyButton;
class CMyDIB;
class CMyBitmap;

class CFilmCaseView : public CMyScrollView
{
public:
	CFilmCaseView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CFilmCaseView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

//	void ReCalcuScrollPara(void);

//	void OnRButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

	BOOL MoveMouse(int x,int y,POINT screenPos);
	BOOL OnControlKey(WPARAM wParam,LPARAM lParam);

	static int m_filmColorTable[];
private:
	HBITMAP m_hBitmap1;
	HBITMAP m_hBitmap2;

	CMyButton* m_buttonNew;
	CMyButton* m_buttonOpen;
	CMyButton* m_buttonSave;
	CMyButton* m_buttonDelete;
	CMyButton* m_buttonUndo;
	CMyButton* m_buttonSearch;

	CMyButton* m_buttonNoClearEffect;

	CMyButton* m_buttonSpecialType;
	CMyButton* m_buttonConfigMask;
	CMyButton* m_buttonTaikenLevel;

	CMyButton* m_buttonCutin;
	CMyButton* m_buttonSkipFilm;

	CMyButton* m_buttonCut;
	CMyButton* m_buttonCopy;
	CMyButton* m_buttonPaste;

	CMyButton* m_buttonRenameLayer;

	CMyDIB* m_dib;
	void CheckChangeColor(int n,int cmd);

	CMyBitmap* m_typeTimeBitmap;
	CMyBitmap* m_configMaskBitmap;
	CMyBitmap* m_taikenLevelBitmap;

	CMyBitmap* m_cutinBitmap;
	CMyBitmap* m_skipFilmBitmap;

	CMyBitmap* m_renameLayerBitmap;


	void MyReleaseCapture(void);
	void SetMyCursor(WPARAM wParam,LPARAM lParam);

	BOOL m_captureFlag;
//	HCURSOR m_captureCursorCopy;
	HCURSOR m_captureCursorMoveAndInsert;
	HCURSOR m_defaultCursor;
	HCURSOR m_captureCursorBad;
	HCURSOR m_captureCursorExchange;

	int m_captureFilm;
	int m_captureCopyType;

	BOOL CheckOnFilmPic(int x,int y);
};


#endif
/*_*/

