#include	"tona3_pch.h"
#include	"tona3_util.h"
#include	"tona3_font.h"

namespace NT3
{
	// ****************************************************************
	// フォントリスト作成
	// ================================================================
	bool C_font_name_list::make_font_list(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode)
	{
		// フォントリスト開放
		m_font_list.clear();

		// 処理変数設定
		work.truetype_only = truetype_only;
		work.toufuku_only = toufuku_only;
		work.char_set = char_set;
		work.yokotate_mode = yokotate_mode;

		// 列挙するフォントリスト
		LOGFONT logfont = { 0 };
		logfont.lfCharSet = DEFAULT_CHARSET;	// 全て列挙

		// リスト作成コールバック
		HDC hdc = GetDC(h_wnd);
		EnumFontFamiliesEx(hdc, &logfont, (FONTENUMPROC)&make_callback_func, (LPARAM)&work, 0);
		ReleaseDC(h_wnd, hdc);

		// 縦横の両方に対応のフォントリストを作成する
		if (work.yokotate_mode == FONT_YOKOTATE_MODE_BOTH)	{
			if (!work.yoko_gaki_font_list.empty() && !work.tate_gaki_font_list.empty())	{
				for (ARRAY<ENUMLOGFONT>::iterator yoko_itr = work.yoko_gaki_font_list.begin(); yoko_itr != work.yoko_gaki_font_list.end(); ++yoko_itr)	{
					TSTR comp_name = _T('@') + yoko_itr->elfLogFont.lfFaceName;
					for (ARRAY<ENUMLOGFONT>::iterator tate_itr = work.tate_gaki_font_list.begin(); tate_itr != work.tate_gaki_font_list.end(); ++tate_itr)	{
						if (comp_name == tate_itr->elfLogFont.lfFaceName)	{
							m_font_list.push_back(*yoko_itr);
							break;
						}
					}
				}
			}
		}
		else	{
			m_font_list = work.temp_font_list;		// パブリックのフォントリストにコピーする
		}

		// 名前順にソート
		std::sort(m_font_list.begin(), m_font_list.end(), font_less_func);

		// 指定された文字コードセットで絞り込み（上でソートしてること前提のアルゴリズムです）
		if (char_set != DEFAULT_CHARSET)	{

			// 作業領域と切り替え
			work.temp_font_list = m_font_list;
			m_font_list.clear();

			// ANSI の場合は特殊（デバッグのため SHIFT_JIS 優先）
			if (char_set == ANSI_CHARSET)	{
				ENUMLOGFONT last_font;
				for (ARRAY<ENUMLOGFONT>::iterator itr = work.temp_font_list.begin(); itr != work.temp_font_list.end(); ++itr)	{

					if (TSTR(itr->elfLogFont.lfFaceName) == TSTR(last_font.elfLogFont.lfFaceName))	{
						if (last_font.elfLogFont.lfCharSet == ANSI_CHARSET && itr->elfLogFont.lfCharSet == SHIFTJIS_CHARSET)	{
							m_font_list.pop_back();			// ANSI を削除して
							m_font_list.push_back(*itr);	// SHIFTJIS を追加
							last_font = *itr;
						}
					}
					else	{
						if (itr->elfLogFont.lfCharSet == ANSI_CHARSET || itr->elfLogFont.lfCharSet == SHIFTJIS_CHARSET)	{
							m_font_list.push_back(*itr);
							last_font = *itr;
						}
					}
				}
			}
			// ANSI 以外が指定された場合はそれのみ列挙
			else	{
				for (ARRAY<ENUMLOGFONT>::iterator itr = work.temp_font_list.begin(); itr != work.temp_font_list.end(); ++itr)	{
					if (itr->elfLogFont.lfCharSet == char_set)	{
						m_font_list.push_back(*itr);
					}
				}
			}
		}

		// 作業変数開放
		work.temp_font_list.clear();
		work.yoko_gaki_font_list.clear();
		work.tate_gaki_font_list.clear();

		// フォントがない
		if (m_font_list.empty())	{ return false; }

		return true;
	}

	// ****************************************************************
	// フォントリスト作成コールバック関数
	// ================================================================
	LRESULT CALLBACK C_font_name_list::make_callback_func(ENUMLOGFONT *elf, NEWTEXTMETRIC *lpntm, int type, LPARAM lp)
	{
		WORK *work = (WORK *)lp;

		// 日本語判定
		//if (work->japanese_only && elf->elfLogFont.lfCharSet != SHIFTJIS_CHARSET)	{
		//	return 1;
		//}

		// TrueType 判定
		if (work->truetype_only && !(type & TRUETYPE_FONTTYPE))	{
			return 1;
		}

		// 等幅判定
		if (work->toufuku_only && (elf->elfLogFont.lfPitchAndFamily & 0x03) == VARIABLE_PITCH)	{
			return 1;
		}

		// 名前へのポインタ
		TCHAR *name_p = &elf->elfLogFont.lfFaceName[0];

		// 横書き判定
		if (work->yokotate_mode == FONT_YOKOTATE_MODE_YOKO_ONLY && *name_p == '@')	{
			return 1;
		}

		// 縦書き判定
		if (work->yokotate_mode == FONT_YOKOTATE_MODE_TATE_ONLY && *name_p != '@')	{
			return 1;
		}

		// 縦横の両方に対応
		if (work->yokotate_mode == FONT_YOKOTATE_MODE_BOTH)
		{
			if (*name_p != '@')	{ work->yoko_gaki_font_list.push_back(*elf); }
			else				{ work->tate_gaki_font_list.push_back(*elf); }
		}
		// 縦横の片方に対応
		else
		{
			work->temp_font_list.push_back(*elf);
		}

		return 1;
	}

	// ****************************************************************
	// フォント検索
	// ================================================================
	bool C_font_name_list::find_font(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode, TSTR font_name)
	{
		// フォントリストがなかったら作成する
		bool font_list_make_flag = false;
		if (m_font_list.empty())	{
			make_font_list(h_wnd, truetype_only, toufuku_only, char_set, yokotate_mode);
			if (m_font_list.empty())	{
				return false;
			}
			font_list_make_flag = true;
		}

		// 検索
		bool find_flag = false;
		ARRAY<ENUMLOGFONT>::iterator itr = m_font_list.begin();
		for (; itr != m_font_list.end(); ++itr)	{
			if (itr->elfLogFont.lfFaceName == font_name)	{
				find_flag = true;
				break;
			}
		}

		// フォントリストを作成した場合は開放する
		if (font_list_make_flag)	{
			free_font_list();
		}

		return find_flag;
	}

	// ****************************************************************
	// ６６階調フォントを３２ビットにコピー（GGO_GRAY8_BITMAP → 32bitBMP）
	//		IN		BYTE *d_buf  : 転送先ポインタ
	//				int d_xl     : 転送先ＸＬ
	//				int d_yl     : 転送先ＹＬ
	//				BYTE *s_buf  : 転送元ポインタ
	//				int s_xl     : 転送元ＸＬ
	//				int s_yl     : 転送元ＹＬ
	//				int x        : 転送座標
	//				int y        : 転送座標
	//				C_argb color  : カラー
	// ================================================================
	void Cfont_copy::gray8_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, C_argb color)
	{
		// カラーテーブル作成
		C_argb rgba[66];
		fill_memory_4(rgba, 66, color);
		BYTE alpha = color.a;
		BYTE *p_rgba = (BYTE *)rgba + 3;
		for (int i = 0; i < 66; i++)	{	*p_rgba = (alpha * i) / 65;	p_rgba += 4;	}
		p_rgba = (BYTE *)rgba;

		int cnt_x, cnt_y, d_free_x, d_free_y, s_free_x, s_free_y;

		if (x >= d_xl || y >= d_yl)	{	return;	}

		if (x < 0)	{	s_free_x = -x;	cnt_x = s_xl + x;	d_free_x = 0;	}
		else		{	s_free_x = 0;	cnt_x = s_xl;		d_free_x = x;	}
		if ((d_xl - d_free_x) < cnt_x)	{	cnt_x = d_xl - d_free_x;	}
		if (cnt_x <= 0)	{	return;	}

		if (y < 0)	{	s_free_y = -y;	cnt_y = s_yl + y;	d_free_y = 0;	}
		else		{	s_free_y = 0;	cnt_y = s_yl;		d_free_y = y;	}
		if ((d_yl - d_free_y) < cnt_y)	{	cnt_y = d_yl - d_free_y;	}
		if (cnt_y <= 0)	{	return;	}

		s_buf += (s_free_x + (s_free_y * s_xl));
		d_buf += (d_free_x + (d_free_y * d_xl)) * 4;
		s_free_x = (s_xl - cnt_x);
		d_free_x = (d_xl - cnt_x) * 4;

		_asm{
			mov		edi, d_buf
			mov		esi, s_buf
			mov		edx, p_rgba
			mov		ecx, cnt_y

		__loop_y:
			push	ecx
			mov		ecx, cnt_x

		__loop_x:
			mov		ebx, edx
			xor		eax, eax
			lodsb
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			stosd
			loop	__loop_x

			add		edi, d_free_x
			add		esi, s_free_x
			pop		ecx
			loop	__loop_y
		}
	}

	// ****************************************************************
	// アルファ合成テーブル作成
	// ================================================================

	// アルファ合成：透明度変換テーブル作成フラグ
	bool	Gfont_alphablend_table_make_flag = false;

	// アルファ合成：透明度変換テーブル
	int		Gfont_alphablend_table_tr[256][256];
	int		*Gfont_alphablend_table_tr_adr[256];

	// アルファ合成：処理テーブル１
	int		Gfont_alphablend_table_work1[256][256];
	int		*Gfont_alphablend_table_work1_adr[256];

	// アルファ合成：処理テーブル２
	int		Gfont_alphablend_table_work2[256][256];
	int		*Gfont_alphablend_table_work2_adr[256];

	// アルファ合成テーブル作成
	void Cfont_copy::make_alphablend_table()
	{
		int i, j;
		int *wp;

		// アルファ合成：透明度変換テーブル
		wp = Gfont_alphablend_table_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = i + j - ((i * j) / 255);
			}
			Gfont_alphablend_table_tr_adr[i] = Gfont_alphablend_table_tr[i];
		}

		// アルファ合成：処理テーブル１
		wp = Gfont_alphablend_table_work1[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = (255 - i) * j;
			}
			Gfont_alphablend_table_work1_adr[i] = Gfont_alphablend_table_work1[i];
		}

		// アルファ合成：処理テーブル２
		wp = Gfont_alphablend_table_work2[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = 255 * i * j;
			}
			Gfont_alphablend_table_work2_adr[i] = Gfont_alphablend_table_work2[i];
		}

		Gfont_alphablend_table_make_flag = true;
	}

	// ****************************************************************
	// アルファ合成アセンブラ定義
	// ================================================================

	// テーブル定義
	#define		__TABLE_ALPHABLEND_TR		OFFSET Gfont_alphablend_table_tr_adr
	#define		__TABLE_ALPHABLEND_WORK1	OFFSET Gfont_alphablend_table_work1_adr
	#define		__TABLE_ALPHABLEND_WORK2	OFFSET Gfont_alphablend_table_work2_adr

	// ================================================================

	// 描画元ピクセルＲを取得（ＥＢＸレジスタに取得）
	#define		__GET_SRC_R_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi+2]

	// 描画元ピクセルＧを取得（ＥＢＸレジスタに取得）
	#define		__GET_SRC_G_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi+1]

	// 描画元ピクセルＢを取得（ＥＢＸレジスタに取得）
	#define		__GET_SRC_B_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi]

	// 描画元ピクセルＡを取得（ＥＢＸレジスタに取得）
	#define		__GET_SRC_A_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi+3]

	// ================================================================

	// 描画元ピクセルＲを取得（ＥＡＸレジスタに取得）
	#define		__GET_SRC_R_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi+2]

	// 描画元ピクセルＧを取得（ＥＡＸレジスタに取得）
	#define		__GET_SRC_G_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi+1]

	// 描画元ピクセルＢを取得（ＥＡＸレジスタに取得）
	#define		__GET_SRC_B_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi]

	// 描画元ピクセルＡを取得（ＥＡＸレジスタに取得）
	#define		__GET_SRC_A_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi+3]

	// ================================================================

	// 描画先ピクセルＲを取得（ＥＢＸレジスタに取得）
	#define		__GET_DST_R_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi+2]

	// 描画先ピクセルＧを取得（ＥＢＸレジスタに取得）
	#define		__GET_DST_G_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi+1]

	// 描画先ピクセルＢを取得（ＥＢＸレジスタに取得）
	#define		__GET_DST_B_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi]

	// 描画先ピクセルＡを取得（ＥＢＸレジスタに取得）
	#define		__GET_DST_A_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi+3]

	// ================================================================

	// 描画先ピクセルＲを取得（ＥＡＸレジスタに取得）
	#define		__GET_DST_R_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi+2]

	// 描画先ピクセルＧを取得（ＥＡＸレジスタに取得）
	#define		__GET_DST_G_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi+1]

	// 描画先ピクセルＢを取得（ＥＡＸレジスタに取得）
	#define		__GET_DST_B_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi]

	// 描画先ピクセルＡを取得（ＥＡＸレジスタに取得）
	#define		__GET_DST_A_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi+3]

	// ================================================================

	// 描画先ピクセルＲを設定（ＥＡＸレジスタを設定）
	#define		__SET_DST_R_EAX \
	__asm				mov		[edi+2], al

	// 描画先ピクセルＧを設定（ＥＡＸレジスタを設定）
	#define		__SET_DST_G_EAX \
	__asm				mov		[edi+1], al

	// 描画先ピクセルＢを設定（ＥＡＸレジスタを設定）
	#define		__SET_DST_B_EAX \
	__asm				mov		[edi], al

	// 描画先ピクセルＡを設定（ＥＡＸレジスタを設定）
	#define		__SET_DST_A_EAX \
	__asm				mov		[edi+3], al

	// ================================================================

	// 描画先ピクセルＲを設定（ＥＢＸレジスタを設定）
	#define		__SET_DST_R_EBX \
	__asm				mov		[edi+2], bl

	// 描画先ピクセルＧを設定（ＥＢＸレジスタを設定）
	#define		__SET_DST_G_EBX \
	__asm				mov		[edi+1], bl

	// 描画先ピクセルＢを設定（ＥＢＸレジスタを設定）
	#define		__SET_DST_B_EBX \
	__asm				mov		[edi], bl

	// 描画先ピクセルＡを設定（ＥＢＸレジスタを設定）
	#define		__SET_DST_A_EBX \
	__asm				mov		[edi+3], bl

	// ================================================================

	// アルファ値を２５５に設定
	#define		__SET_DST_A_255 \
	__asm				mov		[edi+3], 255

	// ================================================================

	static int		alphablend_a;		// アルファ合成用：透明度
	static int		alphablend_r;		// アルファ合成用：Ｒ
	static int		alphablend_g;		// アルファ合成用：Ｇ
	static int		alphablend_b;		// アルファ合成用：Ｂ

	#define		__ALPHABLEND_A		alphablend_a
	#define		__ALPHABLEND_R		alphablend_r
	#define		__ALPHABLEND_G		alphablend_g
	#define		__ALPHABLEND_B		alphablend_b

	// ================================================================

	// アルファ合成
	#define		__SET_R_BLD			__SET_DST_R_EAX
	#define		__SET_G_BLD			__SET_DST_G_EAX
	#define		__SET_B_BLD			__SET_DST_B_EAX

	// （透明度）
	#define		__SET_A_BLD \
						__GET_SRC_A_EAX \
						__SET_DST_A_EAX

	// 透明度 アルファ合成
	#define		__SET_BLD_TR_FUNC(COLOR) \
	__asm				mov		__ALPHABLEND_##COLOR, ebx \

	#define		__SET_R_BLD_TR		__SET_BLD_TR_FUNC(R)
	#define		__SET_G_BLD_TR		__SET_BLD_TR_FUNC(G)
	#define		__SET_B_BLD_TR		__SET_BLD_TR_FUNC(B)

	// アルファ合成準備
	#define		__READY_ALPHABLEND(COLOR) \
	__asm				mov		ebx, __ALPHABLEND_##COLOR \
	__asm				shl		ebx, 2 \
	__asm				add		ebx, eax \
	__asm				mov		ebx, [ebx] \
	__asm				mov		__ALPHABLEND_##COLOR, ebx

	// アルファ合成描画
	#define		__DRAW_ALPHABLEND(COLOR) \
						__GET_DST_##COLOR##_EAX \
	__asm				mul		ebx \
	__asm				add		eax, __ALPHABLEND_##COLOR \
	__asm				shr		eax, 8 \
	__asm				div		ecx \
						__SET_DST_##COLOR##_EAX \

	// アルファ合成なし描画
	#define		__DRAW_ALPHABLENDLESS \
	__asm				mov		ebx, __ALPHABLEND_R \
						__SET_DST_R_EBX \
	__asm				mov		ebx, __ALPHABLEND_G \
						__SET_DST_G_EBX \
	__asm				mov		ebx, __ALPHABLEND_B \
						__SET_DST_B_EBX \
						__SET_DST_A_EAX

	// アルファ前処理
	#define		__BEFOR_ALPHABLEND(fm) \
	__asm				push	ecx \
	__asm				mov		ecx, __ALPHABLEND_A \
	__asm				shl		ecx, 2 \
	__asm				mov		edx, ecx \
	__asm				add		ecx, __TABLE_ALPHABLEND_TR \
	__asm				mov		ecx, [ecx] \
						__GET_DST_A_EAX \
	__asm				shl		eax, 2 \
	__asm				add		ecx, eax \
	__asm				mov		ecx, [ecx] \
	__asm				and		ecx, ecx \
	__asm				jnz		__##fm##_befor_a_bld_tr_end \
	__asm				jmp		__##fm##_alphablend_skip \
	__asm			__##fm##_befor_a_bld_tr_end: \
	__asm				add		edx, __TABLE_ALPHABLEND_WORK1 \
	__asm				mov		edx, [edx] \
	__asm				add		edx, eax \
	__asm				mov		edx, [edx]

	// アルファ後処理
	#define		__AFTER_ALPHABLEND(fm) \
	__asm				mov		eax, __ALPHABLEND_A \
	__asm				mov		ebx, eax \
	__asm				xor		ebx, 0xff \
	__asm				jnz		__##fm##_after_alphablend_next \
						__DRAW_ALPHABLENDLESS \
	__asm				jmp		__##fm##_alphablend_skip \
	__asm			__##fm##_after_alphablend_next: \
						__GET_DST_A_EBX \
	__asm				and		ebx, ebx \
	__asm				jnz		__##fm##_after_alphablend_draw \
						__DRAW_ALPHABLENDLESS \
	__asm				jmp		__##fm##_alphablend_skip \
	__asm			__##fm##_after_alphablend_draw: \
	__asm				shl		eax, 2 \
	__asm				add		eax, __TABLE_ALPHABLEND_WORK2 \
	__asm				mov		eax, [eax] \
						__READY_ALPHABLEND(R) \
						__READY_ALPHABLEND(G) \
						__READY_ALPHABLEND(B) \
	__asm				mov		ebx, edx \
						__DRAW_ALPHABLEND(R) \
						__DRAW_ALPHABLEND(G) \
						__DRAW_ALPHABLEND(B) \
	__asm				mov		[edi+3], cl \
	__asm			__##fm##_alphablend_skip: \
	__asm				pop		ecx

	// ****************************************************************
	// フォントアルファ合成コピー
	//		IN		BYTE *d_buf  : 転送先ポインタ
	//				int d_xl     : 転送先ＸＬ
	//				int d_yl     : 転送先ＹＬ
	//				BYTE *s_buf  : 転送元ポインタ
	//				int s_xl     : 転送元ＸＬ
	//				int s_yl     : 転送元ＹＬ
	//				int x        : 転送座標
	//				int y        : 転送座標
	// ================================================================
	void Cfont_copy::alphablend_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y)
	{
		// アルファ合成テーブル作成
		if (!Gfont_alphablend_table_make_flag)	{	make_alphablend_table();	}

		int cnt_x, cnt_y, d_free_x, d_free_y, s_free_x, s_free_y;

		if (x >= d_xl || y >= d_yl)	{	return;	}

		if (x < 0)	{	s_free_x = -x;	cnt_x = s_xl + x;	d_free_x = 0;	}
		else		{	s_free_x = 0;	cnt_x = s_xl;		d_free_x = x;	}
		if ((d_xl - d_free_x) < cnt_x)	{	cnt_x = d_xl - d_free_x;	}
		if (cnt_x <= 0)	{	return;	}

		if (y < 0)	{	s_free_y = -y;	cnt_y = s_yl + y;	d_free_y = 0;	}
		else		{	s_free_y = 0;	cnt_y = s_yl;		d_free_y = y;	}
		if ((d_yl - d_free_y) < cnt_y)	{	cnt_y = d_yl - d_free_y;	}
		if (cnt_y <= 0)	{	return;	}

		s_buf += (s_free_x + (s_free_y * s_xl)) * 4;
		d_buf += (d_free_x + (d_free_y * d_xl)) * 4;
		s_free_x = (s_xl - cnt_x) * 4;
		d_free_x = (d_xl - cnt_x) * 4;

		_asm{
			mov		edi, d_buf
			mov		esi, s_buf
			mov		ecx, cnt_y
		__loop_y:
			push	ecx
			mov		ecx, cnt_x
		__loop_x:
			xor		eax, eax
			mov		al, [esi+3]
			and		al, al
			jnz		__step_draw
			jmp		__step_no_draw			// 転送元の透明度が０なのでジャンプ

		__step_draw:

			cmp		al, 255
			jne		__step_draw_else

			// 転送元の透明度が 255 の時はそのままコピー
			movsd
			jmp		__step_next

		__step_draw_else:

			// 転送先の透明度取得
			mov		bl, [edi+3]
			and		bl, bl
			jnz		__step_draw_main

			// 転送先の透明度が 0 の時はそのままコピー
			movsd
			jmp		__step_next

		__step_draw_main:

			xor		ebx, ebx
			mov		bl, [esi+3]
			mov		__ALPHABLEND_A, ebx
			__BEFOR_ALPHABLEND(fm2)
			__GET_SRC_R_EBX __SET_R_BLD_TR
			__GET_SRC_G_EBX __SET_G_BLD_TR
			__GET_SRC_B_EBX __SET_B_BLD_TR
			__AFTER_ALPHABLEND(fm2)

		__step_no_draw:
			add		edi, 4
			add		esi, 4
		__step_next:
			dec		ecx
			jz		__loop_x_end
			jmp		__loop_x
		__loop_x_end:
			add		edi, d_free_x
			add		esi, s_free_x
			pop		ecx
			dec		ecx
			jz		__loop_y_end
			jmp		__loop_y
		__loop_y_end:
		}
	}

	// ****************************************************************
	// フォントアルファ合成コピー（袋とじ）
	//		IN		BYTE *d_buf  : 転送先ポインタ
	//				int d_xl     : 転送先ＸＬ
	//				int d_yl     : 転送先ＹＬ
	//				BYTE *s_buf  : 転送元ポインタ
	//				int s_xl     : 転送元ＸＬ
	//				int s_yl     : 転送元ＹＬ
	//				int x        : 転送座標
	//				int y        : 転送座標
	// ================================================================
	void Cfont_copy::alphablend_copy_face(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, MOJI_FACE_TYPE face_type)
	{
		switch (face_type)	{
			case MOJI_FACE_TYPE_NONE:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				break;
			case MOJI_FACE_TYPE_4:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 1);
				break;
			case MOJI_FACE_TYPE_9:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 2);
				break;
			case MOJI_FACE_TYPE_16:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 3);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 3);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 3);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y + 3);
				break;
		}
	}
};
