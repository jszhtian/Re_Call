//
// commonDIB.h
//

#if !defined __TINYAN_NNNEDIT_COMMONDIB__
#define __TINYAN_NNNEDIT_COMMONDIB__


class CCommonDIB
{
public:
	CCommonDIB(int sizeX = 640,int sizeY = 480,HDC hdc = NULL,int bpp = 32);
	~CCommonDIB();
	void End(void);

	BOOL ReCreateDIB(int sizeX = 640, int sizeY = 480,HDC hdc = NULL);

	BOOL Put(HDC hdc, int x, int y, int sizeX ,int sizeY, int* lpData, int srcX = 0, int srcY = 0);
	HDC GetMemoryHDC(void);

	int* GetBuffer(void);
	void GetSize(SIZE* lpSize);

private:

	int m_sizeX;
	int m_sizeY;

	HDC m_memHDC;
	HGDIOBJ m_old;
	int* m_buffer;
	HBITMAP m_hBitmap;
	int m_bpp;
};



#endif
/*_*/

