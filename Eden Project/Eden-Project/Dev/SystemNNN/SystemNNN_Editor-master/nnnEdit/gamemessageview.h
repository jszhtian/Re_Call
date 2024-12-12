//
// GameMessageview.h
//

#if !defined __NNNEDIT_GAMEMESSAGEVIEW__
#define __NNNEDIT_GAMEMESSAGEVIEW__

#include "scrollview.h"

#include "resource.h"

class CMyBitmap;

class CMyButton;
class CGameMessageView : public CMyScrollView
{
public:
	CGameMessageView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	virtual ~CGameMessageView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	void SetEditMode(BOOL flag = TRUE, int n = -1);
//	void ReCalcuScrollPara(void);

	static int m_messageColorTable[];

	BOOL MoveMouse(int x,int y,POINT screenPos);
	BOOL OnControlKey(WPARAM wParam,LPARAM lParam);

//	void OnEnterKey(void);
//	void OnDeleteKey(void);
private:
	HWND m_editHWND[2];

	BOOL m_editMode;
	int m_editNumber;	//???

	void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

	void OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);

	CMyButton* m_buttonNew;
	CMyButton* m_buttonCut;
	CMyButton* m_buttonCopy;
	CMyButton* m_buttonPaste;
	CMyButton* m_buttonDelete;
	CMyButton* m_buttonUndo;
	CMyButton* m_buttonWindow;
	CMyButton* m_buttonCommentTop;
	CMyButton* m_buttonCommentBottom;

	CMyButton* m_buttonExpStatus;
	CMyButton* m_buttonMessageEffect;
	CMyButton* m_buttonMessageEffectTime;

	int m_modeButtonPrintX;
	int m_modeButtonPrintY;
	int m_voiceButtonPrintX;
	int m_voiceButtonPrintY;
	int m_seButtonPrintX;
	int m_seButtonPrintY;

	int GetAreaNumber(int x, int y);
	BOOL CheckInButton(int x, int y, int bx, int by);

	void CheckChangeColor(int n,int cmd);

	int CalcuVoiceChannel(int x,int y);
	int CalcuSeChannel(int x, int y);

	void PutAddButtonPic(HDC hdc,HDC src,int putX,int putY,int addPicFlag);
	
	CMyBitmap* m_addButtonBitmap;
	HBITMAP m_addButtonHBitmap;
	CMyBitmap* m_expStatusBitmap;
	CMyBitmap* m_messageEffectBitmap;
	CMyBitmap* m_messageEffectTimeBitmap;



	CMyBitmap* m_messageFontSizeTypeBitmap;
	CMyButton* m_buttonMessageFontSizeType;

	CMyBitmap* m_faceBitmap;
	CMyButton* m_buttonFace;

	CMyBitmap* m_mustFaceBitmap;
	CMyButton* m_buttonMustFace;

	CMyBitmap* m_fixFaceBitmap;
	CMyButton* m_buttonFixFace;


	CMyBitmap* m_musicFadeBitmap;
	CMyButton* m_buttonMusicFade;


	int m_editorMessageGyo;
	int m_widthGuideLine;

	int GetMessageButtonParam(int x,int y);

};


#endif
/*_*/

