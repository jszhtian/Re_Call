#include "Clipbrd.hpp"

//class TClipboard : public System::TObject //TPersistent �l��TObject�� Persistent����
static TClipboard g_Clipboard;

//----------------------------------------------------------------------------
void TClipboard::Close(void)
{
}
//----------------------------------------------------------------------------
bool TClipboard::HasFormat(Word Format)
{
	return false;
}
//----------------------------------------------------------------------------
void TClipboard::Open(void)
{
	return;
}
//----------------------------------------------------------------------------
void TClipboard::SetAsHandle(Word Format, unsigned Value)
{
	return;
}
//----------------------------------------------------------------------------
//PROPERTY(AnsiString, AsText);//  __property AnsiString AsText = {read=GetAsText, write=SetAsText};
void TClipboard::setAsText(AnsiString val)
{
}
AnsiString TClipboard::getAsText() const
{
	return AnsiString();
}

//----------------------------------------------------------------------------
// �֐�
//----------------------------------------------------------------------------
TClipboard* Clipboard()
{
	return &g_Clipboard;
}
