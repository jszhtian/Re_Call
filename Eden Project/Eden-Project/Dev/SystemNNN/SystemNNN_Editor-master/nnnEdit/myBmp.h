//
// myBmp.h
//

#if !defined __TINYAN_NNNEDIT_MYBMP__
#define __TINYAN_NNNEDIT_MYBMP__

class CMyBMP
{
public:
	CMyBMP(HWND hwnd,HINSTANCE hinstance, LPCSTR name);
	CMyBMP(HWND hwnd,HINSTANCE hinstance, HBITMAP hbitmap);

	virtual ~CMyBMP();
	void End(void);

	void Put(HDC hdcDst, int putX, int putY, int srcX, int srcY, int sizeX, int sizeY);

protected:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HDC m_hdc;
	HBITMAP m_hBitmap;
	HGDIOBJ m_old;
};



#endif
/*_*/

