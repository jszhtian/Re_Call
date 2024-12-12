//
// buttonPic.h
//

#if !defined __NNNEDIT_BUTTONPIC__
#define __NNNEDIT_BUTTONPIC__

//class CMyBMP;
#include "myBMP.h"

class CButtonPic : public CMyBMP
{
public:
	CButtonPic(HWND hwnd,HINSTANCE hinstance, LPCSTR name);
	CButtonPic(HWND hwnd,HINSTANCE hinstance, HBITMAP hbitmap);
	~CButtonPic();
	void End(void);

	void PutButton(HDC hdcDst, int x, int y,int buttonNum, int buttonType);


protected:
	int m_sizeX;
	int m_sizeY;

};


#endif
/*_*/

