#pragma		once

namespace NT3
{

#define		IDEX_STATUS_BAR00		50000

// ****************************************************************
// ダイアログステータスバー
// ================================================================
class C_dialog_statusbar
{

public:
	C_dialog_statusbar();											// コンストラクタ

	bool		create_sizegrip(HWND parent_h_wnd);					// サイズグリップ作成
	void		on_size(HWND parent_h_wnd, WPARAM wp, LPARAM lp);	// サイズ調整

protected:

	bool	create_flag;
	HWND	h_wnd;
};


}
