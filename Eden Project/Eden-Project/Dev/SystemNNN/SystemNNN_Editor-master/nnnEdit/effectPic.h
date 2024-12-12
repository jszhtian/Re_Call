//
// effectPic.h
//

#if !defined __NNNEDIT_EFFECTPIC__
#define __NNNEDIT_EFFECTPIC__

//class CMyBMP;
#include "myBMP.h"

class CEffectPic : public CMyBMP
{
public:
	CEffectPic(HWND hwnd,HINSTANCE hinstance, LPCSTR name);
	CEffectPic(HWND hwnd,HINSTANCE hinstance, HBITMAP hbitmap);
	~CEffectPic();
	void End(void);

	void PutEffect(HDC hdcDst, int x, int y,int num);


protected:
	int m_sizeX;
	int m_sizeY;

};


#endif
/*_*/

