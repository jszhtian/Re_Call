#pragma		once

#include	"tonagraphic3_pct.h"

namespace NT3
{

// ****************************************************************
// アルバム
//		複数のピクチャの集合
// ================================================================
class C_album
{
public:
	C_album();
	~C_album();

	bool	create(ARRAY< BSP<C_pct> > pct_list);	// アルバムを構築
	void	destroy();								// アルバムを破壊

	void	draw_to_dib(C_dib* dib, int pct_no, int x, int y, bool sprite);												// dib に描画
	void	draw_to_dib(C_dib* dib, int pct_no, S_disp_param* dp);														// dib に描画
	void	draw_to_dib(C_dib* dib, int pct_no, S_disp_param* dp, bool draw_flag, ARRAY<C_rect>* ret_draw_rect_list);	// dib に描画

	bool	expand_chips();		// チップを１枚絵に展開

	BSP<C_pct>	get_pct(int pct_no);		// ピクチャを取得（ピクチャ番号を指定）
	int			get_pct_cnt();				// ピクチャ数を取得
	int			get_width(int pct_no);		// 幅を取得
	int			get_height(int pct_no);		// 高さを取得

private:

	// ピクチャリスト
	typedef	ARRAY< BSP<C_pct> >	PCTLIST;
	PCTLIST	pct_list;
};

}
