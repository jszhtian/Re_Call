//
// zahyoview.h
//

#if !defined __NNNEDIT_ZAHYOVIEW__
#define __NNNEDIT_ZAHYOVIEW__

#include "myview.h"

class CMyBitmap;
class CEditSuuji;
class CZahyoView : public CMyView
{
public:
	CZahyoView(CMyDocument* pDocument, HWND clientHWND, HINSTANCE hinstance);
	~CZahyoView();
	void End(void);

	LRESULT ViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	static int m_switchXY[];
	static int m_switchXY2[];
	static int m_presetZahyo[][32];
//	BOOL m_switchFlag[16];
	BOOL MoveMouse(int x,int y,POINT screenPos);

private:
	HBITMAP m_hFontBitmap;
	CMyBitmap* m_fontBitmap;
	CEditSuuji* m_suuji;

	HBITMAP m_hBitmap;
	CMyBitmap* m_bitmap;

	HBITMAP m_hPresetBitmap;
	CMyBitmap* m_presetBitmap;


//	int m_switchKosuu;
//	int m_nowSelectSwitch;
	int GetOnPresetButton(int x,int y);

	void OnClickLButton(WPARAM wParam, LPARAM lParam);
	void PrintPresetButton(HDC hdc,HDC src,int x,int y, int dx,int dy,int type);

	void CheckAndCopyZahyo(int n);
	void CheckAndPresetZahyo(int n);

	void AdjustCenter(RECT* lpRect);

};


#endif
/*_*/

