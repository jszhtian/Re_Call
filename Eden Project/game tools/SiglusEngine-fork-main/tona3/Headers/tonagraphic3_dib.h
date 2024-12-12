#pragma		once

#include	"tonagraphic3_graphic.h"

namespace NT3
{

// ****************************************************************
// ディブセクション
// ================================================================
//		単一のビットマップ（矩形画像）を管理するクラスです。
//		クラス名はディブセクションとなってますが、実体はただのビットデータで扱っています。
// ================================================================
//		true で初期化することで、DIBSECTION を作成することができます。
// ================================================================
class C_dib
{
public:
	C_dib();
	~C_dib();

	explicit C_dib(bool is_API);		// DIBSECTION を利用するなら true を指定

	void	init();
	bool	create(int width, int height, int depth);						// ディブを構築
	bool	create(int width, int height, int depth, C_point center);		// ディブを構築
	void	destroy();														// ディブを破壊

	bool	is_init()				{ return is_API ? h_bitmap != NULL : !bit_data.empty(); }			// データの有無を調べる
	HBITMAP	get_handle()			{ return is_API ? h_bitmap : NULL; }								// ハンドルを取得
	BYTE*	get_ptr()				{ return is_API ? bit_data_API : bit_data.get(); }					// 実データへのポインタを取得
	BYTE*	get_ptr(int x, int y)	{ return get_ptr() + y * bytes_per_line + x * bytes_per_pixel; }	// 実データへのポインタを取得
	int		get_size()				{ return is_API ? width * height * depth / 8 : bit_data.size(); }	// サイズを取得
	int		get_width()				{ return width; }	// 幅を取得
	int		get_height()			{ return height; }	// 高さを取得

	void	clear_color(C_argb color);	// 指定色でクリア

	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, int* off_x, int* off_y);				// 文字をロード
	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, int* off_x, int* off_y, MAT2 mat);		// 文字をロード
	bool	load_bmp(CTSTR& file_path);																// bmp をロード
	bool	load_png(CTSTR& file_path);																// png をロード
	bool	load_jpg(CTSTR& file_path);																// jpg をロード

	bool	create_magnified_dib_scale(C_dib* src, int scale_x, int scale_y);		// 拡大（倍率指定）
	bool	create_magnified_dib_size(C_dib* src, int dst_w, int dst_h);			// 拡大（サイズ指定）
	bool	create_rotate_90_dib(C_dib* p_src_dib);									// 右に９０度回転

	void	draw_point(int x, int y, C_argb color, bool sprite);														// 点を描画
	void	draw_rect(int x, int y, int w, int h, C_argb color, bool fill, bool sprite);								// 矩形を描画（簡易）
	void	draw_rect(S_disp_param* dp);																				// 矩形を描画（フルパラメータ）
	void	draw_rect(S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect);				// 矩形を描画（フルパラメータ＋描画矩形取得）
	void	draw_dib(C_dib* dib, int x, int y, bool sprite);															// dib を描画（簡易）
	void	draw_dib(C_dib* dib, S_disp_param* dp);																		// dib を描画（フルパラメータ）
	void	draw_dib(C_dib* dib, S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect);	// dib を描画（フルパラメータ＋描画矩形取得）
	void	draw_dib_easy(C_dib* dib, int x, int y);																	// dib を描画（簡易版：グラフィックライブラリを使用しません）

	void	transform_draw(S_disp_param* dp);	// 変換描画
	void	transform_to_gray_scale();			// グレースケールに変換

	bool	display_to_screen(HDC h_dst_dc);	// スクリーンに描画

private:

	// ビットマップヘッダ構造体
	struct HEADER	{
		BITMAPINFOHEADER	info;
		DWORD				BitField[3];
	};
	
	// ビットマップヘッダの設定
	HEADER	header;

	// ビットマップ情報
	bool	is_API;				// API を使っているかどうか
	BUFFER	bit_data;			// ビットデータ
	BYTE*	bit_data_API;		// ビットデータ（APIの場合）
	HBITMAP	h_bitmap;			// ビットマップハンドル（APIの場合）
	int		bytes_per_line;		// １行あたりのバイト数
	int		bytes_per_pixel;	// １ピクセルあたりのバイト数
	int		bytes;				// ビットマップ全体のバイト数
	int		width;				// 幅
	int		height;				// 高さ
	int		depth;				// 色数
	C_point	center;				// 中心

	bool	create_ready(int width, int height, int depth, C_point center);
	void	draw_glyph_outline(BYTE* font, int pitch, C_argb color);

	// コピーを禁止にするため、コピーコンストラクタと代入演算子を隠蔽
	C_dib(C_dib&);
	C_dib& operator = (C_dib& rop);
};

}
