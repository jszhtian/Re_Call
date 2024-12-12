//
//
//

#if !defined __NNNEDIT_MYBITMAP__
#define __NNNEDIT_MYBITMAP__



class CMyBitmap
{
public:
	CMyBitmap(LPSTR filename,HDC hdc = NULL);
	virtual ~CMyBitmap();
	void End(void);

	HBITMAP GetHBitmap(void){return m_bitmap;}

	void Blt(HDC hdc,POINT dstPoint,POINT srcPoint,SIZE putSize,HDC src = NULL,int bltType = 0);
	void Blt(HDC hdc,int dstX,int dstY,int srcX,int srcY,int sizeX,int sizeY,HDC src = NULL,int bltType = 0);

	SIZE GetSize(void){return m_size;}
private:
	HBITMAP m_bitmap;
	SIZE m_size;
};





#endif
/*_*/

