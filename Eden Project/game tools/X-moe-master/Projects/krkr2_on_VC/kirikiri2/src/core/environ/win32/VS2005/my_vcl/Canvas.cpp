#include "vcl.h"
#include "Graphics.hpp"
#include "Canvas.h"
//----------------------------------------------------------------------------
//class TCanvas : public System::TObject
//{
//private:
//	wxDC* m_wxDC;
//	HDC hdc;
//public:
//	PROPERTY_VAR_R(HDC, Handle);
//	PROPERTY(TFont*, Font);
//	PROPERTY(TRect, ClipRect);

//	TPen   * Pen;
//	TBrush * Brush;
//----------------------------------------------------------------------------
TCanvas::TCanvas(wxDC * p, bool bCanDelete)
{
	Font  = NULL;
	Pen   = NULL;
	Brush = NULL;

	Font  = new TFont(p);
	Pen   = new TPen;
	Brush = new TBrush;

	m_wxDC = p;
	m_Handle = (HDC) p->GetHDC();
	m_bCanDelete = bCanDelete;
}

//----------------------------------------------------------------------------
TCanvas::~TCanvas()
{
	if (m_bCanDelete && m_wxDC)
		delete m_wxDC;
}

//----------------------------------------------------------------------------
//PROPERTY(TRect, ClipRect);
void TCanvas::setClipRect(TRect v)
{
	m_wxDC->SetClippingRegion( wxRect(v.left, v.top, v.right-v.left, v.bottom-v.top) );
}

TRect TCanvas::getClipRect() const
{
	wxCoord x, y, width, height;
	m_wxDC->GetClippingBox(&x, &y, &width, &height);
	m_wxDC->GetSize(&width, &height);

	return TRect(x, y, x+width, y+height);
}

//----------------------------------------------------------------------------
void TCanvas::Draw( int X, int Y, TGraphic* Graphic )
{
	m_wxDC->DrawBitmap(*(Graphic->m_wxBitmap), X, Y, false);
}

//----------------------------------------------------------------------------
//�w��̓_�����Ԓ�����A���I�ɕ`�悷��
void TCanvas::Polyline(TPoint* pt, int num)
{

}

//----------------------------------------------------------------------------
//�L�����o�X��̎w��̋�`�̓�����h��Ԃ�
void TCanvas::FillRect(TRect)
{
}

//----------------------------------------------------------------------------
//���݂̃t�H���g�ŕ`�悵���ꍇ�̕�����̍�����Ԃ�
int TCanvas::TextHeight(const AnsiString & Text)
{
	wxCoord w, h;

	m_wxDC->GetTextExtent(wxString(Text.c_str()), &w, &h );
	return (int)h;
}

//----------------------------------------------------------------------------
//�������`�悷��
void TCanvas::TextOut(int x, int y, const AnsiString& Text)
{
	//	TPen   * Pen;
	if ( Font != NULL )
	{
		m_wxDC->SetTextForeground( wxColour( Font->m_Color ) );
		m_wxDC->SetFont( *(Font->m_wxFont) );
	}
	m_wxDC->DrawText( Text.c_str(), x, y );
}
