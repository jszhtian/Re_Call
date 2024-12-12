//
// mydocument.h
//

#if !defined __NNNEDIT_MYDOCUMENT__
#define __NNNEDIT_MYDOCUMENT__

class CMyApplicationBase;
class CMyFrameWindow;
class CMyClientWindow;
class CMyView;
class CKomaData;
class CMessageData;
class CCommandData;
class CFilmData;
class CStoryData;
class CCase;

class CMyDocument
{
public:
//	CMyDocument(HWND hWndFrame, HWND hWndClient , HINSTANCE hInstance);
	CMyDocument(CMyApplicationBase* application);
	virtual ~CMyDocument();
	void End(void);

//	HWND GetClientHWND(void);
//	HINSTANCE GetMyInstance(void);


//	BOOL Show(BOOL bFlag = TRUE);

	virtual void OnCloseButton(void);
	CMyApplicationBase* GetApp(void);

	CMyView* GetView(void);

	HWND GetViewHWND(void);
	void MyInvalidateRect(RECT* lpRect = NULL, BOOL bErase = TRUE);
	void UpdateMyWindow(int windowNumber = -1);

	HBITMAP GetCommonBitmap(int n);

	CFilmData* GetNowSelectFilm(void);
	CStoryData* GetNowSelectStory(void);

	CKomaData* GetNowSelectKoma(void);
	CMessageData* GetNowSelectMessage(void);
	CCommandData* GetNowSelectCommand(void);

	virtual CCase* GetNowSelectCaseObject(void){return NULL;}

	int GetNowSelectPictureLayer(void);
	int GetNowSelectEffectLayer(void);


//	int GetNowSelectLayer(void);
	void SetWindowName(LPSTR name);

	void WindowIsMoved(int x, int y);
	void WindowSizeIsChanged(int sizeX,int sizeY);

	HGDIOBJ SelObjOnly(HDC hdc, HGDIOBJ obj);

	void GetInitWindowZahyo(LPSTR windowName,RECT* lpRect, BOOL* lpFlag);

	virtual int GetScrollObjectKosuu(int vh = -1);
	virtual int GetNowSelectNumber(void);

	SIZE GetDesktopSize(void);

//	HWND GetFrameHWND(void);
//	HINSTANCE GetMyInstance(void);

	LPSTR SelectFile(LPSTR defaultName = NULL,LPSTR defaultDirName = NULL);

	virtual BOOL DeleteChildObject(int n,int sub = -1);


	void PreClickMouse(int windowNumber);

	virtual int GetBlockKosuu(void);

	virtual void OnLeftKey(void){}
	virtual void OnRightKey(void){}
	virtual void OnUpKey(void){}
	virtual void OnDownKey(void){}
	virtual void OnPageUp(void){}
	virtual void OnPageDown(void){}

	virtual void OnEnterKey(void){}
	virtual void OnDeleteKey(void){}
	virtual void OnInsertKey(void){}
	virtual void OnSpaceKey(void){}
	virtual void OnEscapeKey(void){}
	virtual void OnTabKey(int shift);

	virtual void SelectPrevObject(int prev = -1);
	virtual void SelectNextObject(int next = 1);

	virtual void OnSelectNumber(int n){}

	virtual void ScrollToSelect(void){}

	virtual void OnFunctionKey(int n);

	int GetWindowNumber(void){return m_windowNumber;}
	virtual BOOL OnUndo(int n = -1){return FALSE;}

protected:
	CMyApplicationBase* m_app;

	HINSTANCE m_hInstance;

	CMyFrameWindow* m_frame;
	CMyClientWindow* m_client;
	CMyView* m_view;

	HWND m_frameHWND;
	HWND m_clientHWND;
	HWND m_viewHWND;

	int m_windowNumber;


	virtual CCase* GetChildObject(int n);


};

#endif
/*_*/
