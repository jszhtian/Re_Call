//
// layerview.h
//

#if !defined __NNNEDIT_LAYERVIEW__
#define __NNNEDIT_LAYERVIEW__


#include "myview.h"

#include "resource.h"

#define PIC_KOSUU_MAX 32

class CMyPopupMenu;
class CNameList;
class CMyBitmap;

//class CMyDirectX;
class CMyButton;

class CMiniPicCache;
class CMyFileOpen;

class CLayerView : public CMyView
{
public:
	CLayerView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CLayerView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	void UpdateWriteButton(int layer);
	void UpdateEyeButton(int layer);
	void UpdatePic(int layer);
//	void UpdatePic2(int layer);

	void SetMiniPic(int picNumber, int* picData);
//	void SetMiniPic2(int picNumber, int* picData);


	void ClearMiniPic(int n);
	void PictureToMini(int n, int* src, int x = 640, int y = 480);

//	static int m_commandToEffectTable[];



//	static UINT CALLBACK OFNHookProc(HWND hdlg, UINT uiMsg,  WPARAM wParam,  LPARAM lParam);

	virtual void OnLeftKey(void);
	virtual void OnRightKey(void);
	virtual void OnUpKey(void);
	virtual void OnDownKey(void);

	int OpenPopupLayerMenu(WPARAM wParam,LPARAM lParam);
	int OpenPopupEffectMenu(WPARAM wParam,LPARAM lParam);

	BOOL MoveMouse(int x,int y,POINT screenPos);
	BOOL OnControlKey(WPARAM wParam,LPARAM lParam);

	//virtual void OnDownKey(void);
private:
	void UpdateButtonArea(int layer, int nm);

	void SelectPic(int layer,int cmd);
//	void ChangeEffect(int layer,int cmd);

	BOOL m_captureFlag;
	HCURSOR m_captureCursorCopy;
	HCURSOR m_captureCursorMove;
	HCURSOR m_defaultCursor;
	HCURSOR m_captureCursorBad;
	HCURSOR m_captureCursorExchange;

	int m_captureLayerEffect;
	int m_captureLayer;
	int m_captureCopyType;


//	int CommandToEffect(int cmd);

	typedef struct _tagBUTTONZAHYO
	{
		int x,y;
		int nextX,nextY;
		int sizeX,sizeY;
	} BUTTONZAHYO;

	static BUTTONZAHYO m_buttonZahyo[];


//	CMyDirectX* m_directX;
//	HBITMAP m_hBitmapEye0;
//	HBITMAP m_hBitmapEye1;
//	HBITMAP m_hBitmapWrite0;
//	HBITMAP m_hBitmapWrite1;

	HBITMAP m_hBitmapDIB[PIC_KOSUU_MAX];
	HBITMAP m_hBitmapVarControl;

//	HBITMAP m_hBitmapDIB2[PIC_KOSUU_MAX];

	int* m_dibBuffer[PIC_KOSUU_MAX];
//	int* m_dibBuffer2[PIC_KOSUU_MAX];

	int m_lastPictureBufferNumber[PIC_KOSUU_MAX];

	void GetUpdateArea(RECT* lpRect, int layer, int nm);

	char* m_lpFileNameBuffer[PIC_KOSUU_MAX];


//	int m_writeButtonPrintX;
//	int m_writeButtonPrintY;
//	int m_writeButtonNextX;
//	int m_writeButtonNextY;
//	int m_writeButtonSizeX;
//	int m_writeButtonSizeY;

	HBITMAP m_hBitmap;

	HBITMAP m_hBitmapEffectMini;

	CMyButton* m_buttonOpen;
	CMyButton* m_buttonSave;
	CMyButton* m_buttonCut;
	CMyButton* m_buttonCopy;
	CMyButton* m_buttonPaste;
	CMyButton* m_buttonDelete;
	CMyButton* m_buttonUndo;

	CMyButton* m_buttonCopyPreEffect;
	CMyButton* m_buttonCopyPreEffect2;

	int GetAreaNumberX(int x, int y);
	int GetAreaNumberY(int x, int y);
//	HMENU m_popupMenu;

	CMiniPicCache* m_miniPic;
	CMyFileOpen* m_file;


	int m_picTypeKosuu;






	CMyPopupMenu* m_pictureMenu;
	CMyPopupMenu* m_effectMenu;
	CMyPopupMenu* m_layerMenu;

//	CNameList* m_picTypeList;
//	int m_picTypeKosuu;

	CMyBitmap* m_leftButtonPic;
	CMyBitmap* m_rightButtonPic;
	CMyBitmap* m_eyeButtonPic;
	CMyBitmap* m_effectMiniPic;

	CMyBitmap* m_bitmapVarControlMark;
	CMyBitmap* m_bitmapEffectMini;
	CMyBitmap* m_bitmap;

	CMyBitmap* m_copyPreEffectBitmap;
	CMyBitmap* m_copyPreEffect2Bitmap;

	int m_upperAreaSizeY;
	int m_leftPicSizeX;
	int m_leftPicSizeY;
	int m_leftButtonSizeX;
	int m_leftButtonSizeY;
	int m_eyeSizeX;
	int m_eyeSizeY;
	int m_rightButtonSizeX;
	int m_rightButtonSizeY;
	int m_effectSizeX;
	int m_effectSizeY;

	void PutLeftButton(HDC hdc,HDC src,int n,BOOL onFlag);
	void PutRightButton(HDC hdc,HDC src,int n,BOOL onFlag);
	void PutEyeButton(HDC hdc,HDC src, int n, BOOL onFlag);
	void PutMiniEffect(HDC hdc,HDC src,int n,int effect);

	void MyReleaseCapture(void);
	void SetMyCursor(WPARAM wParam,LPARAM lParam);

};


#endif
/*_*/

