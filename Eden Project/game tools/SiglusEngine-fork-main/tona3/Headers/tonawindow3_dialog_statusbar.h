#pragma		once

namespace NT3
{

#define		IDEX_STATUS_BAR00		50000

// ****************************************************************
// �_�C�A���O�X�e�[�^�X�o�[
// ================================================================
class C_dialog_statusbar
{

public:
	C_dialog_statusbar();											// �R���X�g���N�^

	bool		create_sizegrip(HWND parent_h_wnd);					// �T�C�Y�O���b�v�쐬
	void		on_size(HWND parent_h_wnd, WPARAM wp, LPARAM lp);	// �T�C�Y����

protected:

	bool	create_flag;
	HWND	h_wnd;
};


}
