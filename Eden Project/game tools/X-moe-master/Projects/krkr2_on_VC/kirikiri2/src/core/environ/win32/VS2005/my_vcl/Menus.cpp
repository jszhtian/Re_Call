#include "Menus.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// �X�̃��j���[���ڂ𑀍삷��I�u�W�F�N�g
//class TMenuItem : public TComponent
TMenuItem::TMenuItem(TComponent* owner) : TComponent(owner)
{
}
TMenuItem::~TMenuItem()
{
}

/*	PROPERTY(int, AutoHotkeys);
	PROPERTY(AnsiString, Caption );
	PROPERTY(bool, Checked);
	PROPERTY(int, Count);
	PROPERTY(bool,Enabled);
	PROPERTY(int, GroupIndex);
	PROPERTY(HMENU, Handle);
	PROPERTY(int, MenuIndex);
	PROPERTY(TMenuItem*, Parent);
	PROPERTY(bool, RadioItem);
	PROPERTY(TShortCut, ShortCut);
	PROPERTY(bool,Visible);
	TMenuItem* Items[10];
*/

	// ���\�b�h
void TMenuItem::Add(TMenuItem*)
{
}
void TMenuItem::Click()
{
}
void TMenuItem::Delete(int index)
{
}
void TMenuItem::Insert(int index, TMenuItem*)
{
}
int  TMenuItem::IndexOf(TMenuItem*)
{
	return 0;
}

	// �C�x���g�B�N���[�W���[
//	boost::function1<void, System::TObject*> OnClick;
//};

//----------------------------------------------------------------------------
//	PROPERTY_ARY(TMenuItem*, Items);
void TMenuItem::setItems(int index, TMenuItem *)
{
}

TMenuItem* TMenuItem::getItems(int index) const
{
	return NULL;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//class TMenu : public TComponent
//{
//public:
TMenu::TMenu(TComponent* owner) : TComponent(owner)
{
}
TMenu::~TMenu()
{
}
//	PROPERTY_ARY(TMenuItem*, Items);
//	PROPERTY(bool, Visible);

//----------------------------------------------------------------------------
//WM_KEY ���b�Z�[�W�����j���[�̍��ڂɐݒ肵���V���[�g�J�b�g�L�[���܂ނƂ��A���j���[���ڂ� Click ���\�b�h�����s����
bool __fastcall TMenu::IsShortCut(Messages::TWMKey &Message)
{
	return false;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TMainMenu::TMainMenu(TComponent* owner) : TMenu(owner)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TPopupMenu::TPopupMenu(TComponent* owner) : TMenu(owner)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// �֐�
//----------------------------------------------------------------------------
//Menus.hpp
//�e�L�X�g�����񂩂� TShortCut �^��Ԃ�
TShortCut  TextToShortCut(const AnsiString&)
{
	return TShortCut(0);
}
//----------------------------------------------------------------------------
//Menus.hpp
//���j���[�V���[�g�J�b�g��������镶����ɕϊ�����
AnsiString ShortCutToText(TShortCut)
{
	return AnsiString();
}

//----------------------------------------------------------------------------
//Menus.hpp
//���j���[�V���[�g�J�b�g�̉��z�L�[�R�[�h�ƃV�t�g��Ԃ�Ԃ�
void ShortCutToKey( TShortCut ShortCut, Word & Key, TShiftState &Shift )
{
	Key = 0;
	Shift = TShiftState();
}
