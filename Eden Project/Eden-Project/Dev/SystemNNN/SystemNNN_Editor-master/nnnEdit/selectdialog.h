//
// selectdialog.h
//

#if !defined __TINYAN_NNNEDIT_SELECTDIALOG__
#define __TINYAN_NNNEDIT_SELECTDIALOG__

class CNameList;

//class CMyApplication;
class CGameMessageDoc;
class CFilmDoc;


class CSelectDialog
{
public:
	CSelectDialog(HWND parentHWND,HINSTANCE hinstance);
	~CSelectDialog();
	void End(void);

	static BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

	int GetSelect1(CNameList* lpNameList,int oldSelect = -1,CGameMessageDoc* lpDoc=NULL,CFilmDoc* lpDoc2 = NULL);
	int GetSelect(CNameList* lpNameList,int oldSelect = -1,CGameMessageDoc* lpDoc=NULL,CFilmDoc* lpDoc2 = NULL);
	int GetSelect(LPSTR* pNamePtr,int nameKosuu,int oldSelect = -1,CGameMessageDoc* lpDoc=NULL,CFilmDoc* lpDoc2 = NULL);

	void SetCaption(LPSTR caption);
	static char m_selectCaption[];

	typedef struct _tagMYSELECTDIALOGSTRUCT
	{
		CNameList* list;
		int oldSelect;
		int nameType;
		LPSTR* pNamePtr;
		int nameKosuu;
		CGameMessageDoc* doc;
		CFilmDoc* doc2;
		LPSTR caption;
	} MYSELECTDIALOGSTRUCT;

private:
	HWND m_parentHWND;
	HINSTANCE m_hInstance;
	BOOL m_playFlag;
	BOOL m_captionExistFlag;
	LPSTR m_caption;

//	CGameMessageDoc* m_mesDoc;
};



#endif
/*_*/

