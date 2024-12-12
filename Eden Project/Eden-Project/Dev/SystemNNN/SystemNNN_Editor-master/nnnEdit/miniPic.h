//
//
//

#if !defined __NNNEDIT_MINIPIC__
#define __NNNEDIT_MINIPIC__


class CMiniPic
{
public:
	CMiniPic(HWND hwnd);
	~CMiniPic();
	void End(void);

	void SetData(int* ptr);
	int* GetBuffer(void){return m_buffer;}
	HBITMAP GetHBitmap(void){return m_hBitmap;}

	BOOL GetFlag(void){return m_flag;}
	void SetFlag(BOOL flg = TRUE){m_flag = FALSE;}
private:
	HWND m_hWnd;
	HBITMAP m_hBitmap;
	int* m_buffer;
	BOOL m_flag;
};

#endif
/*_*/

