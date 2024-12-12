#include	"pch.h"

// テーブル作成フラグ
bool	Gnamalib_grp_table_make_flag = false;

// 透明度変換テーブル
int		Gnamalib_grp_table_tr[256][511];
int		*Gnamalib_grp_table_tr_adr[256];

// モノクロ変換テーブル
int		Gnamalib_grp_table_mono_r[256];
int		Gnamalib_grp_table_mono_g[256];
int		Gnamalib_grp_table_mono_b[256];

// フィルタ変換テーブル作成
int		*Gnamalib_grp_table_flt_adr[511];
int		**Gnamalib_grp_table_flt_adr_neutral;

// 回転計算用テーブル
double	Gnamalib_grp_table_rotate_sin[3600];
double	Gnamalib_grp_table_rotate_cos[3600];

// アルファ合成：透明度変換テーブル
int		Gnamalib_grp_table_alphablend_tr[256][256];
int		*Gnamalib_grp_table_alphablend_tr_adr[256];

// アルファ合成：処理テーブル１
int		Gnamalib_grp_table_alphablend_work1[256][256];
int		*Gnamalib_grp_table_alphablend_work1_adr[256];

// アルファ合成：処理テーブル２
int		Gnamalib_grp_table_alphablend_work2[256][256];
int		*Gnamalib_grp_table_alphablend_work2_adr[256];

// ****************************************************************
// コンストラクタ
// ================================================================
Cnamalib_grp::Cnamalib_grp()
{
	int i, j;
	int *wp;
	int **adr;
	double r1, r2;

	// テーブル作成
	if (!Gnamalib_grp_table_make_flag)	{

		Gnamalib_grp_table_make_flag = true;

		// 透明度変換テーブル
		wp = Gnamalib_grp_table_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = -255; j <= 255; j++)	{
				*wp++ = (i * j) / 255;
			}
			Gnamalib_grp_table_tr_adr[i] = Gnamalib_grp_table_tr[i] + 255;			// + 255 = ニュートラル値（これ重要）
		}

		// モノクロ変換テーブル
		for (i = 0; i < 256; i++)	{	Gnamalib_grp_table_mono_r[i] = ((i * 30) << 8) / 100;	}
		for (i = 0; i < 256; i++)	{	Gnamalib_grp_table_mono_g[i] = ((i * 59) << 8) / 100;	}
		for (i = 0; i < 256; i++)	{	Gnamalib_grp_table_mono_b[i] = ((i * 11) << 8) / 100;	}

		// フィルタ変換テーブル作成
		adr = Gnamalib_grp_table_flt_adr;
		for (i = -255; i < 0; i++)	{	*adr = Gnamalib_grp_table_tr[255 + i] + 255;	adr++;	}
		Gnamalib_grp_table_flt_adr_neutral = adr;
		for (i = 0; i <= 255; i++)	{	*adr = Gnamalib_grp_table_tr[255 - i] + 255;	adr++;	}

		// 回転計算用テーブル作成
		r1 = 0.0;
		for (i = 0; i < 3600; i++)	{
			r2 = r1 * (3.141592 / 180);
			Gnamalib_grp_table_rotate_sin[i] = sin(r2);
			Gnamalib_grp_table_rotate_cos[i] = cos(r2);
			r1 += 0.1;
		}

		// アルファ合成：透明度変換テーブル
		wp = Gnamalib_grp_table_alphablend_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = i + j - ((i * j) / 255);
			}
			Gnamalib_grp_table_alphablend_tr_adr[i] = Gnamalib_grp_table_alphablend_tr[i];
		}

		// アルファ合成：処理テーブル１
		wp = Gnamalib_grp_table_alphablend_work1[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = (255 - i) * j;
			}
			Gnamalib_grp_table_alphablend_work1_adr[i] = Gnamalib_grp_table_alphablend_work1[i];
		}

		// アルファ合成：処理テーブル２
		wp = Gnamalib_grp_table_alphablend_work2[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = 255 * i * j;
			}
			Gnamalib_grp_table_alphablend_work2_adr[i] = Gnamalib_grp_table_alphablend_work2[i];
		}

	}
}

// ****************************************************************
// デストラクタ
// ================================================================
Cnamalib_grp::~Cnamalib_grp()
{
}



