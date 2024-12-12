//
// filmview.h
//

#if !defined __TINYAN_NNNEDIT_FILMVIEW__
#define __TINYAN_NNNEDIT_FILMVIEW__

//#include "myview.h"
#include "scrollview.h"

//#define FILM_PIC_KOSUU_MAX 64

class CMyButton;
class CMyBitmap;


class CFilmView : public CMyScrollView
{
public:
	CFilmView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CFilmView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	BOOL MoveMouse(int x,int y,POINT screenPos);
	BOOL OnControlKey(WPARAM wParam,LPARAM lParam);

//	void ReCalcuScrollPara(void);

private:
//	HBITMAP m_hBitmapDIB[FILM_PIC_KOSUU_MAX];
//	int* m_dibBuffer[FILM_PIC_KOSUU_MAX];

	int m_filmPicKosuu;
	HBITMAP* m_lpHBitmapDIB;
	int** m_lplpDIBBuffer;
	int* m_lpDIBNumberTable;

	HBITMAP m_hBitmapFilm64x48;

	int m_komaPrintX;
	int m_komaPrintY;
	int m_komaNextX;
	int m_komaNextY;

	int m_printKomaKosuu;

	CMyButton* m_buttonNew;
	CMyButton* m_buttonCut;
	CMyButton* m_buttonCopy;
	CMyButton* m_buttonPaste;
	CMyButton* m_buttonDelete;
	CMyButton* m_buttonUndo;
	CMyButton* m_buttonOverrap;
	CMyButton* m_buttonOverrapType;
	CMyButton* m_buttonSetCG;
	CMyButton* m_buttonNoClearEffect;
	CMyButton* m_buttonBGM;
	CMyButton* m_buttonFrame;
	CMyButton* m_buttonWindowOff;
	CMyButton* m_buttonDemo;
	CMyButton* m_buttonWindowNumber;
	CMyButton* m_buttonCursorNumber;
	CMyButton* m_buttonMouseNumber;
	CMyButton* m_buttonAutoMessage;
	CMyButton* m_buttonCannotClick;
	CMyButton* m_buttonCannotSkip;
	CMyButton* m_buttonOptionOff;
	CMyButton* m_buttonCutin;
	CMyButton* m_buttonPreload;
	CMyButton* m_buttonExpStatus;
	CMyButton* m_buttonVarControl;


	BOOL AddFilmPicBuffer(int n);

	CMyBitmap* m_filmBitmap;

	CMyBitmap* m_filmPartsBitmap;

	CMyBitmap* m_overrapBitmap;

	CMyBitmap* m_fontBitmap;

	CMyBitmap* m_meswinMiniBitmap;
	CMyBitmap* m_onpuBitmap;
	CMyBitmap* m_kyufuBitmap;

	CMyBitmap* m_buttonDemoBitmap;
	HBITMAP m_hBitmapFilmParts;

	CMyBitmap* m_buttonWindowNumberBitmap;
	CMyBitmap* m_buttonCursorNumberBitmap;
	CMyBitmap* m_buttonMouseNumberBitmap;


	CMyBitmap* m_buttonAutoMessageBitmap;
	CMyBitmap* m_buttonCannotClickBitmap;
	CMyBitmap* m_buttonCannotSkipBitmap;

	CMyBitmap* m_buttonOptionOffBitmap;
	CMyBitmap* m_cutinBitmap;
	CMyBitmap* m_preloadBitmap;
	CMyBitmap* m_expStatusBitmap;
	CMyBitmap* m_varControlBitmap;


	void OnPaint(HWND hwnd,WPARAM wParam,LPARAM lParam);

	void PutFilmParts(HDC hdc,int x, int y,int n,HDC src = NULL);

	int m_overrapButtonPrintX;

	void PrintMiniSuuji(HDC hdc,int x,int y,int d);

};


#endif
/*_*/

