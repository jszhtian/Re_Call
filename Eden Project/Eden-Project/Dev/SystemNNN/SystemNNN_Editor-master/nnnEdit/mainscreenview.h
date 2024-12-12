//
// mainscreenview.h
//

#if !defined __TINYAN_NNNEDIT_MAINSCREENVIEW__
#define __TINYAN_NNNEDIT_MAINSCREENVIEW__

#include "myview.h"

class CMyDocument;
class CMyDirectDraw;
class CMMX;

class CPicture;
class CMyMessage;

class CMessageSetup;


class CMainScreenView : public CMyView
{
public:
	CMainScreenView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CMainScreenView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	void PrintLayerHint(int btn);
	void PrintEffectHint(int effectLayer);

	void PrintPlayIcon(int playMode,int storyMode = 0);

	void PrintFrameTime(int nowFrame,int setFrame,int printPlace = 0);
	void PrintShakin(int shakin,int skip,int totalSkip);

	void FlipToScreen(void);
	void Moved(WPARAM wParam,LPARAM lParam);

	static int m_gazeData[][4];

	static int m_arrowDeltaTable[10];
	void SetMessageSetup(CMessageSetup* lpMessageSetup){m_messageSetup = lpMessageSetup;}

	BOOL MoveMouse(int x,int y,POINT screenPos);

private:
	CMyDirectDraw* m_directX;
	CMMX* m_mmx;

	int m_bpp;

	int m_mouseXMae;
	int m_mouseYMae;

	int m_shiftMode;

	CPicture* m_gazePic[32];
	CPicture* m_markPic;
	CPicture* m_upperButtonPic;

	CPicture* m_outerArrowPic;

	CPicture* m_speedTypePic;

	CPicture* m_centerTargetPic;
	CPicture* m_startTargetPic;
	CPicture* m_sizeTargetPic;

	CPicture* m_transButtonPic;
	CPicture* m_transButton3Pic;

	CPicture* m_leftButtonPic;
	CPicture* m_rightButtonPic;

	CPicture* m_onoffButtonPic;

	CPicture* m_frameTypePic;
	CPicture* m_frameGazePic;
	CPicture* m_frameMarkerPic;
	CPicture* m_frameNumberPic;


	CPicture* m_playModeIconPic;

	void GetEffectControlAndPara(void);

	int m_effectTableOffset[256];
	int* m_controlWork;

	void CheckGazeControl(int mouseX,int mouseY,BOOL dragFlag = FALSE,WPARAM wParam = 0);

	int m_captureGaze;
	BOOL m_captureDrag;

	CMyMessage* m_message;
	void PutUpperButton(int x, int y, int n, int md);

	CPicture* m_messageWindowPic;
	CPicture* m_nameWindowPic;

	CPicture* m_suujiPic;

	void PrintSuuji(int x,int y, int d,int keta = 3);
	void PrintMiniMessage(int x, int y, LPSTR mes);
	void PutChara(int x, int y, int ch);

	int CalcuParamByGaze(CEffectParam* effectParam,int n,int d,int dv);
	int CheckAndAdjustParam(CEffectParam* effectParam,int n,int d);
	//int CalcuAdjustParam(int n,int delta);

	BOOL m_captureWithY;


	void OnClickRButton(WPARAM wParam,LPARAM lParam);

	POINT m_effectMenuPrintZahyo[2];
	POINT m_effectBarPrintZahyo[2];
	POINT m_effectButtonPrintZahyo[2];
	POINT m_effectDescPrintZahyo[2];
	POINT m_effectParamDescPrintZahyo[2];
	POINT m_effectLeftButtonPrintZahyo[2];
	POINT m_effectRightButtonPrintZahyo[2];

	POINT m_effectFrameZahyo[2];
	int m_buttonPrintDX[7];

	void PutLeftButton(POINT pt,int md);
	void PutRightButton(POINT pt,int md);

	POINT GetBarBaseZahyo(int effect,int n);
	POINT GetButtonBaseZahyo(int effect,int n);

	POINT GetEffectFrameZahyo(int updown);

	int m_leftRight;
	int m_topBottom;

	void PutSpeedTypePic(POINT pt,int d,int md);
	void PutButtonPic(int type,POINT pt,int d,int md);

	void PutBar(int type,POINT pt,int d,int md);
	void PutParamName(POINT pt,LPSTR name,int md);

	POINT GetStartPointXY(int effectLayer,int n,CEffectParam* effectParam,CKomaData* pKoma);
	int GetStartPointY(int effectLayer,int n,CEffectParam* effectParam,CKomaData* pKoma);

	void PutFrameGaze(POINT pt,int type);
	void PutFrameType(POINT pt,int type);
	void PutFrameMarker(POINT pt,int d);
	void PrintFrameSuuji(POINT pt,int d);


	POINT GetEffectTypeZahyo(int updown);
	POINT GetEFfectGazeZahyo(int updown);
	int GetOnEffectGaze(int mouseX,int mouseY,int updown);
	BOOL CheckOnEffectType(int mouseX,int mouseY,int updown);

	int GetOnGaze(int mouseX,int mouseY,int effect,int n);

	int GetOnArrowButton(int mouseX,int mouseY,int effect,int n);
	POINT GetLeftArrowButtonZahyo(int effect,int n);
	POINT GetRightArrowButtonZahyo(int effect,int n);
	POINT GetParamNameZahyo(int effect,int n);

	BOOL CheckOnParamName(int mouseX,int mouseY,int effect,int n);
	BOOL CheckOnStatusButton(int mouseX,int mouseY,int effect,int n);


	CPicture* m_mesWinPic;
	CPicture* m_nameWinPic;


	CMessageSetup* m_messageSetup;

	void PrintLine3(int sx,int sy,int ex,int ey,int r,int g,int b);

	int m_waitVSync;
};

#endif
/*_*/
