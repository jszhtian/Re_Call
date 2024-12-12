//
// suuji.h
//

#if !defined __TINYAN_NNNEDIT_SUUJI__
#define __TINYAN_NNNEDIT_SUUJI__


class CEditSuuji
{
public:
	CEditSuuji(HBITMAP hbitmap,int sizeX = 12,int sizeY = 12);
	~CEditSuuji();
	void End(void);

	void PrintSuuji(HDC hdc, int x, int y, int d, int keta = 4, HDC src = NULL);
	void PrintMessage(HDC hdc, int x, int y, LPSTR mes, HDC src = NULL);




private:
	HBITMAP m_hBitmap;
	void PutChara(HDC hdc, HDC src, int x, int y, int c);
	int m_sizeX;
	int m_sizeY;
};

#endif
/*_*/

