#if !defined __NNNEDIT_BALLOON__
#define __NNNEDIT_BALLOON__


class CNameList;
class CBalloon
{
public:
	CBalloon(HWND clientHWND, HINSTANCE hinstance);
	~CBalloon();
	void End(void);

	void OnArea(int n,POINT pt,int subType,POINT screenPos);

	static LRESULT CALLBACK staticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	static CBalloon* m_this;
	void Show(void);
//	void Start(void);

	typedef struct NAMETYPELIST
	{
		int type;
		char name[32];
	}NAMETYPELIST;

	static NAMETYPELIST m_nameTypeList[];
	static char m_defaultHelpText[];
	void OnTimer(void);

private:
	HWND m_clientHWND;
	HINSTANCE m_hInstance;

	HWND m_hWnd;
	BOOL m_showMode;
	int m_balloonNumber;
	int m_subType;
	POINT m_mousePoint;
	POINT m_printZahyo;

//	RECT m_windowRect;
	BOOL m_sizeCalcued;

	SIZE m_windowSize;
	POINT m_windowPos;

	int m_padding;

	CNameList* m_helpList;

//	CMyBitmap* m_bitmap;
//	HBITMAP m_hBitmap;

//	BOOL m_timerFlag;
	int m_calcuedHelpNumber;

	int m_listTable[32][32];
	char m_helpText[4096];

	int SearchTypeName(LPSTR name);
	int m_typeCount;

	int m_waitTime;
	int m_waitCount;
	POINT m_mousePos;
	POINT m_screenPos;

};

#endif
/*_*/
