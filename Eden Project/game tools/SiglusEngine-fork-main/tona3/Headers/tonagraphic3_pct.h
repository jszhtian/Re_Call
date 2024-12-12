#pragma		once

#include	"tonagraphic3_dib.h"

namespace NT3
{

// ****************************************************************
// dib チップ
//		dib に加えて、描画位置などの情報を持つ
//		主にピクチャ内部で使用する
// ================================================================
struct C_dib_chip : public C_dib
{
	C_dib_chip()
	{
		x = 0;
		y = 0;
		sprite = false;
	}

	int		x;
	int		y;
	bool	sprite;		// 透過あり
};

// ****************************************************************
// ピクチャ
//		複数の dib チップの集合
// ================================================================
class C_pct
{
public:
	C_pct();
	~C_pct();

	void	destroy();	// 破壊

	bool	create(ARRAY< BSP<C_dib_chip> > chip_list, int width, int height, C_point center_, C_rect disp_rect);	// ピクチャの作成
	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf);				// 文字を読み込む
	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf, MAT2 mat);		// 文字を読み込む
	bool	load_bmp(CTSTR& file_path);															// bmp を読み込む
	bool	load_png(CTSTR& file_path);															// png を読み込む
	bool	load_jpg(CTSTR& file_path);															// jpg を読み込む

	BSP<C_dib_chip>	expand_chips();			// チップを１枚に展開
	bool			magnify(int scale);		// ピクチャを拡大
	bool			rotate_90();			// 右に９０度回転

	void	draw_to_dib(C_dib* dib, int x, int y, bool sprite);												// ピクチャを dib に描画
	void	draw_to_dib(C_dib* dib, S_disp_param* dp);														// ピクチャを dib に描画
	void	draw_to_dib(C_dib* dib, S_disp_param* dp, bool draw_flag, ARRAY<C_rect>* ret_draw_rect_list);	// ピクチャを dib に描画

	int		get_width()		{	return width;					}	// 幅を取得
	int		get_height()	{	return height;					}	// 高さを取得
	C_point	get_center()	{	return center;					}	// 中心座標を取得
	C_rect	get_disp_rect()	{	return disp_rect - center;		}	// 描画矩形を取得
	int		get_chip_cnt()	{	return (int)chip_list.size();	}	// チップ数を取得

private:

	// チップリスト
	typedef ARRAY< BSP<C_dib_chip> >	CHIPLIST;
	CHIPLIST							chip_list;

	int		width;		// 幅
	int		height;		// 高さ
	C_point	center;		// 中心座標
	C_rect	disp_rect;	// 描画矩形

	// ================================================================
	// disp_rect は、その外に透明 (a = 0) データ以外がないことを保証する矩形です。
	// 合成方法が Edisp_blend_type_copy 以外のときは、disp_rect 以外を描画する必要はありません。
	// 合成方法が Edisp_blend_type_copy のときは、disp_rect を使わずに、width - height 全体を描画する必要があります。
	// ----------------------------------------------------------------
};

}
