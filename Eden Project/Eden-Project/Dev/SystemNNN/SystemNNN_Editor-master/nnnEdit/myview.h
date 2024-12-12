//
// myview.h
//

#if !defined __NNNEDIT_MYVIEW__
#define __NNNEDIT_MYVIEW__

#define VIEW_KOSUU_MAX 100

class CMyDocument;
class CMyToolBar;
class CMyButton;

class CMyView
{
public:
	CMyView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	virtual ~CMyView();
	virtual void End(void);


//	virtual void ReCalcuScrollPara(void);

	static int m_viewKosuu;
	static HWND m_viewHWNDTable[VIEW_KOSUU_MAX];
	static CMyView* m_viewTable[VIEW_KOSUU_MAX];

	static LRESULT CALLBACK staticViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
//	LRESULT FrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	virtual LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;

	HWND GetHWND(void);

	void SetHWND(HWND hwnd);

	BOOL SetViewWindowText(LPSTR text);
	BOOL MoveViewWindow(int wx, int wy, int sizeX , int sizeY );
	BOOL MoveViewWindow(RECT* lpRect);
	void WindowIsMoved(int x, int y);
	void WindowSizeIsChanged(int sizeX, int sizeY);

	BOOL Show(BOOL bFlag = TRUE);

	CMyDocument* GetMyDocument(void);

	void GetWindowZahyo(RECT* lpRect);

	void MyInvalidateRect(RECT* lpRect = NULL, BOOL bErase = TRUE);

	void OnToolButton(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void SetWindowName(LPSTR name);

	virtual void ReCalcuScrollPara(void);
	virtual void SetScrollArea(int startX, int startY, int endX, int endY);
	virtual void SetScrollPara(int pos, int posMax = -1, int page = -1, int vh = -1);

	virtual int GetBlockKosuu(int vh = -1);
	int m_number;

	int AdjustWindowSizeX(int x);
	int AdjustWindowSizeY(int y);


	void SetResizeFlag(BOOL flg = TRUE);
	void SetBlockKosuuMin(int nx, int ny);
	void SetBlockKosuuMax(int nx, int ny);
	void SetBlockKosuu(int nx, int ny);
	void SetScrollFlag(BOOL hFlag, BOOL vFlag);

	void SetNonBlockArea(int left,int top, int right ,int bottom);

	void SetWindowStyle(void);

	void SetFreeSizeFlag(int vh, BOOL flg = TRUE);

	void InitWindowZahyo(void);

	LPSTR GetMyViewWindowName(void);

	virtual void OnLeftKey(void);
	virtual void OnRightKey(void);
	virtual void OnUpKey(void);
	virtual void OnDownKey(void);
	virtual void OnPageUp(void);
	virtual void OnPageDown(void);

	virtual void OnEnterKey(void);
	virtual void OnDeleteKey(void);
	virtual void OnInsertKey(void);
	virtual void OnSpaceKey(void);
	virtual void OnEscapeKey(void);
	virtual void OnTabKey(int shift = 0);

	virtual void OnCtrlXKey(void) {};
	virtual void OnCtrlCKey(void) {};
	virtual void OnCtrlVKey(void) {};

	virtual void OnFunctionKey(int n);
	int OpenPopupMenu(WPARAM wParam, LPARAM lParam, int nPos1 = 0, int nPos2 = -1, int n = 0);

	virtual BOOL MoveMouse(int x,int y,POINT screenPos);
	void EraseBalloon(void);

	CMyButton* m_buttonForCheck[64];
	int m_buttonForCheckNumber;
	virtual BOOL OnControlKey(WPARAM wParam,LPARAM lParam);
	

protected:
	CMyDocument* m_document;
	HWND m_clientHWND;
	HINSTANCE m_hInstance;

//	HWND GetHWND(void);

	HWND m_hWnd;

	BOOL m_showFlag;

	BOOL m_captureFlag;
	CMyToolBar* m_myToolBar;

	int m_windowX;
	int m_windowY;
	int m_windowSizeX;
	int m_windowSizeY;

//	int m_clientX;
//	int m_clientY;
//	int m_clientSizeX;
//	int m_clientSizeY;


	HMENU m_popupMenu[10];


	HBITMAP GetCommonBitmap(int n);

	void EraseSplit(HDC hdc, int col = -1);
	void EraseUpper(HDC hdc, int col = -1);
	void EraseLower(HDC hdc, int col = -1);


	BOOL m_resizeFlag;
	BOOL m_freeSizeFlag[2];
	BOOL m_scrollFlag[2];

	int m_blockKosuuMin[2];
	int m_blockKosuuMax[2];

	int m_blockKosuu[2];
	int m_blockSizeX;
	int m_blockSizeY;

	int m_topArea;
	int m_bottomArea;
	int m_leftArea;
	int m_rightArea;



	void SetBlockSize(int sizeX,int sizeY);

	SIZE CalcuWindowSize(int nx = 1, int ny = 1);

	void AdjustWindowSize(WPARAM fwFlag, LPRECT lpRect);

	char m_windowName[64];
	void SetMyViewWindowName(LPSTR name);

	int CheckOnBalloonButton(int x,int y);
	void AddBalloonCheckButton(CMyButton* button);
	void SetBalloonCheckButton(int n,CMyButton* button);
private:

};


#endif
/*_*/
