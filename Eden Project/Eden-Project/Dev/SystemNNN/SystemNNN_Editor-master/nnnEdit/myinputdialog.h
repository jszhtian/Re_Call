//
// myinputdialog.h
//

#if !defined __TINYAN_NNNEDIT_MYINPUTDIALOG__
#define __TINYAN_NNNEDIT_MYINPUTDIALOG__

class CMyInputDialog
{
public:
	CMyInputDialog(HWND parentHWND, HINSTANCE hinstance, int maxBuffer = 64);
	~CMyInputDialog();
	void End(void);

	LPSTR GetText(LPSTR oldText,BOOL suujiFlag = FALSE,LPSTR captionName = NULL);
	LPSTR GetText2(LPSTR oldText,LPSTR captionName = NULL);
	BOOL GetNumber(int n,int* lpNumber,LPSTR captionName = NULL);
//	BOOL GetNumber2(int n, int* lpNumber);

	BOOL GetPoint(POINT pt,POINT* lpPoint,LPSTR captionName = NULL);
	BOOL GetSize(SIZE sz,SIZE* lpSize,LPSTR captionName = NULL);
	BOOL GetRect(RECT rc,RECT* lpRect,LPSTR captionName = NULL);

	BOOL GetAutoFlag(void);
	BOOL GetFullFlag(void){return m_fullFlag;}
	BOOL GetAutoCRFlag(void){return m_autoCRFlag;}
	static BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	typedef struct _tagMYTEXTSTRUCT
	{
		int type;
		int maxLength;
		LPSTR buff;
		BOOL suujiFlag;
		BOOL autoFlag;
		BOOL fullFlag;
		BOOL autoCRFlag;
		LPSTR caption;
		POINT pt;
		SIZE sz;
	} MYTEXTSTRUCT, *LPMYTEXTSTRUCT;

	int TextToSuuji(void);

	static char m_textCaption[];
	static char m_text2Caption[];
	static char m_numberCaption[];

	static char m_pointCaption[];
	static char m_sizeCaption[];
	static char m_rectCaption[];


private:

	int m_maxText;
//	char m_textBuffer[16384];
	char* m_textBuffer;
	HWND m_parentHWND;
	HINSTANCE m_hInstance;

	BOOL m_autoFlag;
	BOOL m_fullFlag;
	BOOL m_autoCRFlag;

};

#endif
/*_*/
