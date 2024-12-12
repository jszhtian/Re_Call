#pragma		once

#define		___USE_TG___			0	// 作りかけのとなグラフィック。通常は使わない。
#define		___USE_NG___			0	// 旧なまグラフィック。通常は使わない。
#define		___USE_NAMALIB_GRP___	1	// 新なまグラフィック。通常はこれを使う。

// ****************************************************************
// TG / NG / namalib
// ================================================================

#if ___USE_TG___

namespace Ntona
{
void tc_draw_box_copy(
		BYTE* dst, int dst_w, int dst_h, 
		int x, int y, int w, int h,			// ボックス矩形
		BYTE a, BYTE r, BYTE g, BYTE b,		// ボックス色
		int blend_type,												// 合成方法
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// 不透明度、モノクロ度、反転度、明暗度
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// ＲＧＢ変換
		int rep_color_r, int rep_color_g, int rep_color_b			// ＲＧＢ補正
	);

void tc_draw_dib_copy(
		BYTE* dst, int dst_w, int dst_h,	// 描画先
		BYTE* src, int src_w, int src_h,	// 描画元
		int x, int y,						// 描画位置
		int blend_type,													// 合成方法
		BYTE alpha, BYTE mono, BYTE reverse, int bright,				// 不透明度、モノクロ度、反転度、明暗度
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,		// ＲＧＢ変換
		int rep_color_r, int rep_color_g, int rep_color_b,				// ＲＧＢ補正
		int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// 描画矩形
	);

void tc_draw_box_blend(
		BYTE* dst, int dst_w, int dst_h, 
		int x, int y, int w, int h,			// ボックス矩形
		BYTE a, BYTE r, BYTE g, BYTE b,		// ボックス色
		int blend_type,												// 合成方法
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// 不透明度、モノクロ度、反転度、明暗度
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// ＲＧＢ変換
		int rep_color_r, int rep_color_g, int rep_color_b			// ＲＧＢ補正
	);

void tc_draw_dib_blend(
		BYTE* dst, int dst_w, int dst_h,	// 描画先
		BYTE* src, int src_w, int src_h,	// 描画元
		int x, int y,						// 描画位置
		int blend_type,													// 合成方法
		BYTE alpha, BYTE mono, BYTE reverse, int bright,				// 不透明度、モノクロ度、反転度、明暗度
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,		// ＲＧＢ変換
		int rep_color_r, int rep_color_g, int rep_color_b,				// ＲＧＢ補正
		int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// 描画矩形
	);
};

#elif ___USE_NG___

#define		SC		signed char				// 1ﾊﾞｲﾄ
#define		SS		signed short int		// 2ﾊﾞｲﾄ
#define		SI		signed int				// 4ﾊﾞｲﾄ
#define		SL		signed long int			// 4ﾊﾞｲﾄ
#define		SD		signed double			// 8ﾊﾞｲﾄ

#define		UC		unsigned char			// 1ﾊﾞｲﾄ
#define		US		unsigned short int		// 2ﾊﾞｲﾄ
#define		UI		unsigned int			// 4ﾊﾞｲﾄ
#define		UL		unsigned long int		// 4ﾊﾞｲﾄ
#define		UD		unsigned double			// 8ﾊﾞｲﾄ

#include	<np_palt.h>
#include	<ng_grp.h>

#undef		SC
#undef		SS
#undef		SI
#undef		SL
#undef		SD

#undef		UC
#undef		US
#undef		UI
#undef		UL
#undef		UD

#pragma comment(lib, "ng_grp_ga.lib")

#elif ___USE_NAMALIB_GRP___

#include	<namalib_grp.h>

#endif

namespace NT3
{

	// ****************************************************************
	// フォント構造体
	// ================================================================
	class C_log_font : public LOGFONT
	{
	public:

		C_log_font()	
		{
			// デフォルト値を設定する
			ZeroMemory(this, sizeof(*this));
			lfCharSet = DEFAULT_CHARSET;//SHIFTJIS_CHARSET;
			lfHeight = 16;
			_tcscpy_s(lfFaceName, LF_FACESIZE, _T("ＭＳ Ｐゴシック"));
		}
	};

	// ****************************************************************
	// テキストフォーマット構造体
	// ================================================================
	struct C_text_format
	{
		C_text_format() : width(16), height(16)	{}

		int width;
		int height;
	};

	// ****************************************************************
	// パレット
	// ================================================================
	class C_palette
	{
		C_palette();

	public:
		static	C_palette&	get_instance()
		{
			static C_palette instance;
			return instance;
		}

		BYTE*	p_ggo_gray8;

	private:
		BYTE	ggo_gray8[256];
	};

	// ****************************************************************
	// 描画：プレーンタイプ
	// ================================================================
	enum E_disp_plane_type
	{
		E_disp_plane_type_line = 0,
		E_disp_plane_type_box = 1,
		E_disp_plane_type_box_fill = 2,
		E_disp_plane_type_max = 3,
	};

	// ****************************************************************
	// 描画：描画構造体
	// ================================================================
	struct S_disp_param_plane
	{
		C_rect				rect;		// 矩形
		C_argb				color;		// 色
		E_disp_plane_type	type;		// タイプ（E_disp_plane_type のうち１つを指定）
	};

	struct S_disp_param_filter
	{
		int		r;		// Ｒ
		int		g;		// Ｇ
		int		b;		// Ｂ
		BYTE	a;		// Ａ
	};

	struct S_disp_param_tile
	{
		BYTE*	ptr;		// タイル：データポインタ
		C_size	size;		// タイル：サイズ
		C_point	center;		// タイル：中心座標
		bool	reverse;	// タイル：反転
		BYTE	shift;		// タイル：シフト（強度 0 ～ 8）
		int		add;		// タイル：加算（波紋処理）
	};

	struct S_disp_param
	{
		S_disp_param();
		void	init();

		// 基本パラメータ
		C_point	pos;			// 描画座標
		C_point	center;			// 中心座標
		bool	sprite;			// 透過（true / false）
		TRUE_COLOR_GRP_DRAW_MOD		blend_type;		// 合成方法
		BYTE	alpha;			// 不透明度（0～255）
		BYTE	mono;			// モノクロ度（0～255）
		BYTE	reverse;		// 反転度（0～255）
		int		bright;			// 明暗度（-255～255）
		C_rgb	color;			// ＲＧＢ変換
		BYTE	color_rate;		// ＲＧＢ変換度（0～255）
		int		rep_color_r;	// Ｒ修正度（-255～255）
		int		rep_color_g;	// Ｒ修正度（-255～255）
		int		rep_color_b;	// Ｒ修正度（-255～255）

		bool	dst_clip_use;	// クリップフラグ
		C_rect	dst_clip;		// クリップ
		bool	src_rect_use;	// 描画範囲フラグ
		C_rect	src_rect;		// 描画範囲

		// タイルパラメータ
		S_disp_param_tile	tile;
		// プレーンパラメータ
		S_disp_param_plane	plane;
		// フィルタパラメータ
		S_disp_param_filter	filter;
		// 拡張パラメータ
		C_size	scale;			// 倍率（当倍 = 1000）
		int		rotate;			// 回転（1° = 10）

		S_disp_param& operator = (const S_disp_param& rhs)	{ return pod_assign(this, &rhs); }
		bool operator == (const S_disp_param& rhs)			{ return pod_equal(this, &rhs); }
		bool operator != (const S_disp_param& rhs)			{ return !pod_equal(this, &rhs); }
	};

	// ****************************************************************
	// 描画関数
	// ================================================================
	void tc_draw_plane(BYTE* dst, int dst_w, int dst_h, C_point dst_center, S_disp_param* dp);
	void tc_draw_plane(BYTE* dst, int dst_w, int dst_h, C_point dst_center, S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* draw_rect);
	void tc_draw_dib(BYTE* dst, int dst_w, int dst_h, C_point dst_center, BYTE* src, int src_w, int src_h, S_disp_param* dp);
	void tc_draw_dib(BYTE* dst, int dst_w, int dst_h, C_point dst_center, BYTE* src, int src_w, int src_h, S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* draw_rect);

	void tc_draw_dib_easy(BYTE* dst, int dst_w, int dst_h, BYTE* src, int src_w, int src_h, int x, int y);
}
