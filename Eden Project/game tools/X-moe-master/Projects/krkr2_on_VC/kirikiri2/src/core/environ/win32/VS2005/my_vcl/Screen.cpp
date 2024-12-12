
#include "Screen.h"

// main�֐��Ȃ��ɋ����ɉ�����������g���b�N
TScreen* Screen;
static __THogeInstance<TScreen, &Screen> __TScreenInstance;

// �A�v���P�[�V�������s���̃X�N���[���̏�Ԃ�\���I�u�W�F�N�g
/*
class TScreen
{
private:
	std::vector<TForm*> m_Forms;
	std::vector<HCURSOR> m_Cursors;
public:
	PROPERTY_R(int, Width);
	PROPERTY_R(int, Height);
	PROPERTY_R(int, FormCount);
	PROPERTY(TCursor, Cursor);

	PROPERTY_ARY_R(TForm*, Forms);
	PROPERTY_ARY(HCURSOR, Cursors);
};
*/

//----------------------------------------------------------------------------
//	PROPERTY_R(int, Width);
// �X�N���[���̕���Ԃ�
int TScreen::getWidth() const
{
	return wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
}

//----------------------------------------------------------------------------
//	PROPERTY_R(int, Height);
// �X�N���[���̍�����Ԃ�
int TScreen::getHeight() const
{
	return wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
}

//----------------------------------------------------------------------------
//	PROPERTY(int, FormCount);
int TScreen::getFormCount() const
{
	return m_Forms.size();
}
//----------------------------------------------------------------------------
//	PROPERTY(TCursor, Cursor);
// �A�v���P�[�V�����S�̂ɑ΂���}�E�X�J�[�\����ݒ�E�擾����

//----------------------------------------------------------------------------
//	PROPERTY_ARY_R(TForm*, Forms);
// �A�v���P�[�V�������ł��łɍ쐬���ꂽ���ׂẴt�H�[���̃��X�g��Ԃ�
TForm* TScreen::getForms( int index ) const
{
	return m_Forms[index];
}

//----------------------------------------------------------------------------
//	PROPERTY_ARY(HCURSOR, Cursors);
// �A�v���P�[�V�������Ŏg�p�\�ȃJ�[�\���̃��X�g��ݒ�E�擾����
HCURSOR TScreen::getCursors( int index ) const
{
	return m_Cursors[index];
}
void TScreen::setCursors(int index, HCURSOR h)
{
}
