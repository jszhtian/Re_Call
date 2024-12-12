// MyBitmapButton.h: interface for the CMyBitmapButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBITMAPBUTTON_H__29BE38F0_703E_4AD7_AD80_4E92A3150530__INCLUDED_)
#define AFX_MYBITMAPBUTTON_H__29BE38F0_703E_4AD7_AD80_4E92A3150530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyBitmapButton : public CBitmapButton  
{
public:
	void SetBmpRgn(UINT bitmapResource,COLORREF clrRemove);
	CMyBitmapButton();
	virtual ~CMyBitmapButton();

protected:
	HRGN m_rgnFinal;


};

#endif // !defined(AFX_MYBITMAPBUTTON_H__29BE38F0_703E_4AD7_AD80_4E92A3150530__INCLUDED_)
