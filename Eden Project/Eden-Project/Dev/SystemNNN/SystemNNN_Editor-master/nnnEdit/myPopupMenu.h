//
//
//

#if !defined __NNNEDIT_MYPOPUPMENU__
#define __NNNEDIT_MYPOPUPMENU__


class CNameList;
class CMyBitmap;
class CMyPopupMenu
{
public:
	CMyPopupMenu(LPSTR fileName,int menuCommandNumber);
	virtual ~CMyPopupMenu();
	void End(void);

	void SetMyMenu(HWND hwnd);
	int OpenMenu(POINT pt,HWND hwnd = NULL);

protected:
	CNameList* m_nameList;
	HMENU m_menu;
	HWND m_hWnd;
private:
	int m_menuCommandNumber;
	CMyBitmap** m_bitmap;
	int m_bitmapKosuu;

	void DestroySubMenu(HMENU menu);
};






#endif
/*_*/

