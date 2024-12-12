//
// mydib.h
//

#if !defined __TINYAN_NNNEDIT_MYDIB__
#define __TINYAN_NNNEDIT_MYDIB__

class CSelectObjectOnly;

class CMyDIB
{
public:
	CMyDIB(HWND hwnd, int x, int y, int bpp = 32);
	~CMyDIB();
	void End(void);

	void Put(int x, int y, HDC hdc, HDC src = NULL);
	void SetPic(LPVOID data);

private:
	HBITMAP m_hBitmap;
	LPVOID m_buffer;

	int m_sizeX;
	int m_sizeY;
	int m_bpp;

	CSelectObjectOnly* m_selObj;

};

#endif
/*_*/

