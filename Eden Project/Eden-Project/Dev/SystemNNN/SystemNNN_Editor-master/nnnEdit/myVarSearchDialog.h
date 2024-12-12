//
// myinputdialog.h
//

#if !defined __TINYAN_NNNEDIT_MYVARSEARCHDIALOG__
#define __TINYAN_NNNEDIT_MYVARSEARCHDIALOG__

class CMyVarSearchDialog
{
public:
	CMyVarSearchDialog(HWND parentHWND, HINSTANCE hinstance, int maxBuffer = 64);
	~CMyVarSearchDialog();
	void End(void);

	int GetCommand(BOOL replaceFlag = FALSE,BOOL startFlag = FALSE);

	LPSTR GetSearchText(void);
	void SetSearchText(LPSTR text);
	LPSTR GetReplaceText(void);
	void SetReplaceText(LPSTR text);

	void SetDeaultRadio(int radio);
	void SetDefaultRadio2(int radio2);

	int GetRadio(void);
	int GetRadio2(void);

	static BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


	int m_maxText;
	char* m_textBuffer;

	int m_radio;
	int m_radio2;
	char m_searchText[1024];
	char m_replaceText[1024];

	HWND m_parentHWND;
	HINSTANCE m_hInstance;

//	BOOL m_allFlag;
	BOOL m_replaceFlag;
	BOOL m_startFlag;
//	BOOL m_endFlag;

	int m_windowX;
	int m_windowY;

	BOOL m_windowZahyoFlag;

private:
};


#endif
/*_*/
