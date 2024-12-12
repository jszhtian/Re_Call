#include	"tonad3d3_pch.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_texture.h"
#include	"tonad3d3_sprite.h"

namespace NT3
{

	// ****************************************************************
	// D3D テクスチャ
	// ================================================================
	C_d3d_texture::C_d3d_texture()
	{
		// パラメータの初期化
		m_width = 0;
		m_height = 0;
		m_width_ex = 0;
		m_height_ex = 0;
		m_org_width = 0;
		m_org_height = 0;
		m_center = C_point(0, 0);
		m_blend = false;
		m_mipmap_enable = false;
		m_dirty = false;
	}

	C_d3d_texture::~C_d3d_texture()
	{
	}

	// ****************************************************************
	// テクスチャの初期化
	// ================================================================
	void C_d3d_texture::init()
	{
		// テクスチャを解放
		release();

		// パラメータの初期化
		m_width = 0;
		m_height = 0;
		m_width_ex = 0;
		m_height_ex = 0;
		m_org_width = 0;
		m_org_height = 0;
		m_center = C_point(0, 0);
		m_blend = false;
		m_mipmap_enable = false;
		m_dirty = false;
	}

	// ****************************************************************
	// テクスチャの解放
	// ================================================================
	void C_d3d_texture::release()
	{
		m_texture = NULL;
	}

	// ****************************************************************
	// テクスチャの作成
	// ================================================================
	bool C_d3d_texture::create(int width, int height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool)
	{
		HRESULT hr;

		// テクスチャを初期化
		init();

		// パラメータのコピー
		m_width = width;
		m_height = height;
		m_org_width = width;
		m_org_height = height;

		// 大きさを 2^n に調整する。2^n しか作れないグラボ対策。高速化の意味も。
		// 大きさ 1px だと GeForce9600 GT でおかしかった（白くなった）ので、最小 2px にします。
		if (!G_d3d.device.can_use_not_pow2_texture())	{
			for (m_width_ex = 2; m_width_ex < m_width; m_width_ex *= 2);
			for (m_height_ex = 2; m_height_ex < m_height; m_height_ex *= 2);
		}
		else	{
			m_width_ex = std::max(m_width, 2);
			m_height_ex = std::max(m_height, 2);
		}

		// 2048×2048 までしか作れない
		if (m_width_ex > 2048 || m_height_ex > 2048)	{
			set_last_error(_T("2048×2048 より大きなテクスチャは作れません。"), _T("C_d3d_texture::create"));
			return false;
		}

		// フォーマットと使用方法チェック
		if (usage & D3DUSAGE_AUTOGENMIPMAP)	{
			if (pool == D3DPOOL_SYSTEMMEM)	{
				set_last_error(_T("D3DUSAGE_AUTOGENMIPMAP と D3DPOOL_SYSTEMMEM は同時には指定できません。"), _T("C_d3d_texture::create"));
				return false;
			}
			else if (!G_d3d.device.can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())	{
				usage &= ~D3DUSAGE_AUTOGENMIPMAP;	// ミップマップの自動生成をオフにする
			}
		}
		if (usage & D3DUSAGE_DYNAMIC)	{
			if (pool == D3DPOOL_MANAGED)	{
				set_last_error(_T("D3DUSAGE_DYNAMIC と D3DPOOL_MANAGED は同時には指定できません。"), _T("D3DXCreateTexture"));
				return false;
			}
		}

		// ▲ミップマップは最大で１段階
		//mip_level = 1;

		// D3DX を利用してテクスチャを作成する
		IDirect3DTexture9* p_texture = NULL;
		hr = D3DXCreateTexture(G_d3d.device.body(), m_width_ex, m_height_ex, mip_level, usage, format, pool, &p_texture);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("CreateTexture"), hr);
			return false;
		}

		// 動的テクスチャでなければミップマップが作れる
		if (!(usage & D3DUSAGE_DYNAMIC))	{
			m_mipmap_enable = true;
		}

		// D3DXCreateTexture は参照カウンタを増やすので、false を指定します。
		m_texture = BIP<IDirect3DTexture9>(p_texture, false);

		return true;
	}

	bool C_d3d_texture::create(int width, int height)
	{
		return create(width, height, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);
	}

	// ****************************************************************
	// サーフェスを取得
	// ================================================================
	C_d3d_surface C_d3d_texture::get_surface(int level)
	{
		HRESULT hr;

		if (!m_texture)
			return C_d3d_surface();

		IDirect3DSurface9* p_surface = NULL;
		hr = m_texture->GetSurfaceLevel(level, &p_surface);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("GetSurfaceLevel"), hr);
			return C_d3d_surface();
		}

		// GetSurfaceLevel は内部で参照カウンタを上げています。BIP で管理しないとリークしてしまいます。
		return BIP<IDirect3DSurface9>(p_surface, false);
	}

	// ****************************************************************
	// テクスチャのデータをサーフェスに取得
	// ================================================================
	bool C_d3d_texture::get_render_target_data(int level, IDirect3DSurface9* p_dst_surface)
	{
		HRESULT hr;

		if (!m_texture)	{
			set_last_error(_T("テクスチャが準備されていません。"), _T("C_d3d_texture::get_render_target_data"));
			return false;
		}

		// サーフェスを取得
		IDirect3DSurface9* p_src_surface = NULL;
		hr = m_texture->GetSurfaceLevel(level, &p_src_surface);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("GetSurfaceLevel"), hr);
			return false;
		}

		// GetSurfaceLevel は内部で参照カウンタを上げています。BIP で管理しないとリークしてしまいます。
		BIP<IDirect3DSurface9> bip_surface(p_src_surface, false);

		// レンダーターゲットのデータを取得する
		hr = G_d3d.device->GetRenderTargetData(p_src_surface, p_dst_surface);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("GetRenderTargetData"), hr);
			return false;
		}

		return true;
	}

	// ****************************************************************
	// テクスチャのロック
	// ================================================================
	bool C_d3d_texture::lock_rect(UINT level, D3DLOCKED_RECT* texture_bit, C_rect* rect, DWORD flags)
	{
		HRESULT hr;

		if (!m_texture)	{
			set_last_error(_T("テクスチャが準備されていません。"), _T("LockRect"));
			return false;
		}

		hr = m_texture->LockRect(level, texture_bit, rect, flags);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("LockRect"), hr);
			return false;
		}

		return true;
	}

	bool C_d3d_texture::unlock_rect(UINT level)
	{
		HRESULT hr;

		if (!m_texture)	{
			set_last_error(_T("テクスチャが準備されていません。"), _T("UnlockRect"));
			return false;
		}

		hr = m_texture->UnlockRect(level);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("UnlockRect"), hr);
			return false;
		}

		return true;
	}

	// ****************************************************************
	// テクスチャの塗りつぶし
	// ================================================================
	bool C_d3d_texture::clear_color_ex(C_argb color)
	{
		// テクスチャにビットデータのコピー
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		for (int y = 0; y < m_height_ex; y++)	{
			BYTE* dst = (BYTE *)texture_bit.pBits + texture_bit.Pitch * y;
			for (int x = 0; x < m_width_ex; x++)	{
				*dst++ = color.b;
				*dst++ = color.g;
				*dst++ = color.r;
				*dst++ = color.a;
			}
		}

		if (!unlock_rect(0))
			return false;

		return true;
	}

	bool C_d3d_texture::clear_color(C_argb color)
	{
		// テクスチャにビットデータのコピー
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		for (int y = 0; y < m_height; y++)	{
			BYTE* dst = (BYTE *)texture_bit.pBits + texture_bit.Pitch * y;
			for (int x = 0; x < m_width; x++)	{
				*dst++ = color.b;
				*dst++ = color.g;
				*dst++ = color.r;
				*dst++ = color.a;
			}
		}

		if (!unlock_rect(0))
			return false;

		return true;
	}

	// ****************************************************************
	// 矩形を描画
	// ================================================================
	bool C_d3d_texture::draw_rect(C_rect rect, C_argb color)
	{
		// テクスチャにビットデータのコピー
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		for (int y = rect.top; y < rect.bottom; y++)	{
			BYTE* dst = (BYTE *)texture_bit.pBits + texture_bit.Pitch * y;
			for (int x = rect.left; x < rect.right; x++)	{
				*dst++ = color.b;
				*dst++ = color.g;
				*dst++ = color.r;
				*dst++ = color.a;
			}
		}

		if (!unlock_rect(0))
			return false;

		return true;
	}

	// ****************************************************************
	// 簡易に文字を読み込む（簡易指定）
	// ================================================================
	bool C_d3d_texture::load_moji(TCHAR moji, int moji_size, C_argb moji_color)
	{
		// フォントを指定する
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));
		lf.lfCharSet = DEFAULT_CHARSET;//SHIFTJIS_CHARSET;
		lf.lfHeight = moji_size;
		_tcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(TCHAR), _T("ＭＳ Ｐゴシック"));

		// 文字を読み込む
		return load_moji(moji, &lf, moji_color);
	}

	// ****************************************************************
	// 文字をテクスチャに読み込む（フォント指定）
	// ================================================================
	bool C_d3d_texture::load_moji(TCHAR moji, LOGFONT* lf, C_argb moji_color)
	{
		// デバイスコンテキスト
		HDC h_dc = CreateCompatibleDC(0);

		// フォント作成
		HFONT h_font = CreateFontIndirect(lf);

		// フォントを選択
		HGDIOBJ h_old_font = SelectObject(h_dc, (HGDIOBJ)h_font);

		// ハンドルを指定して文字を読み込む
		// サイズは lfHeight を渡します。（lfWidth だと 0 の可能性がある）
		if (!load_moji(moji, lf->lfHeight, moji_color, MOJI_FACE_TYPE_NONE, false, h_dc))
			return false;

		// ハンドルを解放
		SelectObject(h_dc, (HGDIOBJ)h_old_font);
		DeleteObject(h_font);
		DeleteDC(h_dc);

		return true;
	}

	// ****************************************************************
	// 文字をテクスチャに読み込む（デバイスコンテキスト指定）
	// ================================================================
	bool C_d3d_texture::load_moji(TCHAR moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_type, bool tategaki, HDC h_dc)
	{
		int rep_x = 0;		// x 座標がマイナスになったときの補正用
		int rep_y = 0;		// y 座標がマイナスになったときの補正用

		// フォントの情報を取得
		TEXTMETRIC tm = {0};
		GetTextMetrics(h_dc, &tm);

		// 縦書き／横書きのフォントによって行列が変わる
		MAT2 mat_yokogaki = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
		MAT2 mat_tategaki = { { 0, 0 }, { 0, -1 }, { 0, 1 }, { 0, 0 } };

		// フォントデータの取得
		GLYPHMETRICS gm = {0};
		MAT2 mat = tategaki ? mat_tategaki : mat_yokogaki;
		DWORD data_size = GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat);
		if (data_size == GDI_ERROR)
			return false;

		// 一部フォントで GGO_GRAY8_BITMAP を指定すると 0 が返ってくる。
		// その場合は GGO_BITMAP で読み込めばいいんだけど展開ルーチン作らないといけない。
		// でも Vista 以降だと TextOut ですら描画されないよ！！！というわけで残念ながら切り捨てます。

		// スペース
		if (data_size == 0)
		{
			// 文字タイプによってサイズを計算
			int need_size;
			switch (moji_type)	{
				case MOJI_FACE_TYPE_4:			need_size = moji_size + 1;	break;
				case MOJI_FACE_TYPE_9:			need_size = moji_size + 2;	break;
				case MOJI_FACE_TYPE_16:			need_size = moji_size + 3;	break;
				default:						need_size = moji_size;		break;
			}

			// 指定されたサイズのテクスチャを作成
			if (!create(need_size, need_size))
				return false;

			// テクスチャを作るのは大事です。大きさを当たり判定に使います。
		}
		// スペース以外
		else
		{
			int baseline_x, baseline_y, x, y, w, h;

			// フォントのビットデータ取得
			ARRAY<BYTE> font_data(data_size);
			GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, data_size, font_data.get(), &mat);

			// ベースラインの位置を決める
			// これはフォント情報からではなく自分で決める必要があります。
			// ＭＳ ゴシックが綺麗に入るように、文字サイズの 86% にしてみます。
			// この方法ならメイリオの internalLeading も関係ないです。
			if (tategaki)
			{
				baseline_x = moji_size - (int)(moji_size * 0.86 + 0.5);
				baseline_y = 0;
			}
			else
			{
				baseline_x = 0;
				baseline_y = (int)(moji_size * 0.86 + 0.5);
			}

			// ビットを出力する座標を計算
			if (tategaki)
			{
				x = baseline_x + gm.gmptGlyphOrigin.x;
				y = baseline_y - gm.gmptGlyphOrigin.y;
				w = (gm.gmBlackBoxX + 3) & 0xFFFC;		//  + 3) & 0xFFFC = アライメント調整
				h = gm.gmBlackBoxY;
			}
			else
			{
				x = baseline_x + gm.gmptGlyphOrigin.x;
				y = baseline_y - gm.gmptGlyphOrigin.y;
				w = (gm.gmBlackBoxX + 3) & 0xFFFC;		//  + 3) & 0xFFFC = アライメント調整
				h = gm.gmBlackBoxY;
			}

			// ★データサイズと gmBlackBox から割り出したサイズが合わないことがあります。
			// （例：フォント = "ＭＳ ゴシック"、サイズ = 11、文字 = 'ご'）
			// データサイズが大きい場合はそちらを使用し、その分文字を上にずらします。
			// これ何だろうフォントのバグ？かなぁ？

			// データサイズから高さを取り出してみる
			int real_h = data_size / w;
			if (real_h > h)
			{
				y -= real_h - h;
				h = real_h;
			}

			// 文字サイズを補正

			// マイナスの場合は補正（作成後に補正する）
			if (y < 0)
			{
				rep_y = - y;
				y = 0;
			}
			if (x < 0)
			{
				rep_x = - x;
				x = 0;
			}
			// 文字を大きくする。テクスチャサイズも大きくなります。
			moji_size = std::max(moji_size, y + h);

			// 文字タイプによってサイズを計算
			int need_size;
			switch (moji_type)
			{
			case MOJI_FACE_TYPE_4:			need_size = moji_size + 1;	break;
			case MOJI_FACE_TYPE_9:			need_size = moji_size + 2;	break;
			case MOJI_FACE_TYPE_16:			need_size = moji_size + 3;	break;
			default:						need_size = moji_size;		break;
			}

			// 指定されたサイズのテクスチャを作成
			if (!create(need_size, need_size))
				return false;

			// テクスチャをロック
			D3DLOCKED_RECT texture_bit;
			if (!lock_rect(0, &texture_bit, NULL, 0))
				return false;

			Cfont_copy fc;
			switch (moji_type)
			{
				case MOJI_FACE_TYPE_4:
				case MOJI_FACE_TYPE_9:
				case MOJI_FACE_TYPE_16:
					{
						ARRAY<BYTE> tmp_tc_font_data(moji_size * moji_size * 4);
						fc.gray8_copy(tmp_tc_font_data.get(), moji_size, moji_size, font_data.get(), w, h, x, y, moji_color);
						fc.alphablend_copy_face((BYTE *)texture_bit.pBits, get_width_ex(), get_height_ex(), tmp_tc_font_data.get(), moji_size, moji_size, 0, 0, moji_type);
					}
					break;
				default:					// 通常
					fc.gray8_copy((BYTE *)texture_bit.pBits, get_width_ex(), get_height_ex(), font_data.get(), w, h, x, y, moji_color);
					break;
			}

			// 中心を設定
			m_center.x = rep_x;
			m_center.y = rep_y;

			// テクスチャのアンロック
			if (!unlock_rect(0))
				return false;
		}

		// 文字はブレンドあり
		m_blend = true;

		return true;
	}

	// ****************************************************************
	// ファイルから D3D に対応した画像をテクスチャに読み込む
	// ================================================================
	bool C_d3d_texture::load_file(CTSTR& file_path)
	{
		HRESULT hr;

		// テクスチャのロード
		D3DXIMAGE_INFO image_info;
		IDirect3DTexture9* p_texture = NULL;
		hr = D3DXCreateTextureFromFileEx(G_d3d.device.body(), file_path.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &image_info, NULL, &p_texture);
		if (FAILED(hr))	{
			set_last_error_d3d(file_path + _T(" の読み込みに失敗しました。"), _T("CreateTexture"), hr);
			return false;
		}

		// D3DXCreateTextureFromFileEx は参照カウンタを増やすので、false を指定する
		m_texture = BIP<IDirect3DTexture9>(p_texture, false);

		// 情報を取得
		m_width  = image_info.Width;
		m_height = image_info.Height;
		m_org_width = image_info.Width;
		m_org_height = image_info.Height;

		// 大きさを 2^n に調整
		for (m_width_ex = 1; m_width_ex < m_width; m_width_ex *= 2);
		for (m_height_ex = 1; m_height_ex < m_height; m_height_ex *= 2);

		// 画像はブレンドあり
		m_blend = true;

		return true;
	}

	// ****************************************************************
	// g00 を読み込む
	// ================================================================
	bool C_d3d_texture::load_g00(CTSTR& g00_path)
	{
		// ブレンドフラグ
		m_blend = false;

		// g00 ファイルを読み込む
		BUFFER file_data;
		if (!C_file::read_full_data(g00_path, file_data))
			return false;

		// g00 を作成
		C_g00 g00;
		if (!g00.set_data(file_data.get(), file_data.size()))
			return false;

		// g00 の情報を取得
		S_g00_info g00_info;
		g00.get_info(&g00_info);

		if (g00_info.cut_cnt > 0)	{

			// カット０番を取得
			int cut_no = 0;
			C_g00_cut* g00_cut = g00.get_cut(cut_no);
			// カット０番の情報を取得
			S_g00_cut_info cut_info;
			g00_cut->get_info(&cut_info);
			// テクスチャを作成
			if (!create(cut_info.width, cut_info.height))
				return false;

			// テクスチャをロック
			D3DLOCKED_RECT texture_bit;
			if (!lock_rect(0, &texture_bit, NULL, 0))
				return false;

			// チップの処理
			for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

				// チップを取得
				C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
				// チップの情報を取得
				S_g00_chip_info chip_info;
				g00_chip->get_info(&chip_info);
				g00_chip->get_data((BYTE *)texture_bit.pBits + texture_bit.Pitch * chip_info.y + chip_info.x * 4, texture_bit.Pitch);

				// ブレンドする必要のあるチップがあったならばブレンド
				if (!m_blend && chip_info.sprite)
					m_blend = true;
			}
			// テクスチャをアンロック
			if (!unlock_rect(0))
				return false;
		}

		return true;
	}

	// ****************************************************************
	// g00 のカットを読み込む
	// ================================================================

	// ****************************************************************
	// アルファ合成テーブル作成
	// ================================================================

	// アルファ合成：透明度変換テーブル作成フラグ
	bool	Gv_va_graphics_alphablend_table_make_flag = false;

	// アルファ合成：透明度変換テーブル
	int		Gv_va_graphics_alphablend_table_tr[256][256];
	int		*Gv_va_graphics_alphablend_table_tr_adr[256];
	DWORD	Gv_va_graphics_alphablend_table_tr_adr_offset;

	// アルファ合成：処理テーブル１
	int		Gv_va_graphics_alphablend_table_work1[256][256];
	int		*Gv_va_graphics_alphablend_table_work1_adr[256];
	DWORD	Gv_va_graphics_alphablend_table_work1_adr_offset;

	// アルファ合成：処理テーブル２
	int		Gv_va_graphics_alphablend_table_work2[256][256];
	int		*Gv_va_graphics_alphablend_table_work2_adr[256];
	DWORD	Gv_va_graphics_alphablend_table_work2_adr_offset;

	// アルファ差分：透明度変換テーブル
	int		Gv_va_graphics_alphasub_table_tr[256][256];
	int		*Gv_va_graphics_alphasub_table_tr_adr[256];
	DWORD	Gv_va_graphics_alphasub_table_tr_adr_offset;

	// アルファ合成テーブル作成
	void f_make_alphablend_table()
	{
		int i, j;
		int *wp;
	
		// アルファ合成：透明度変換テーブル
		wp = Gv_va_graphics_alphablend_table_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = i + j - ((i * j) / 255);
			}
			Gv_va_graphics_alphablend_table_tr_adr[i] = Gv_va_graphics_alphablend_table_tr[i];
		}
	
		// アルファ合成：処理テーブル１
		wp = Gv_va_graphics_alphablend_table_work1[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = (255 - i) * j;
			}
			Gv_va_graphics_alphablend_table_work1_adr[i] = Gv_va_graphics_alphablend_table_work1[i];
		}
	
		// アルファ合成：処理テーブル２
		wp = Gv_va_graphics_alphablend_table_work2[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = 255 * i * j;
			}
			Gv_va_graphics_alphablend_table_work2_adr[i] = Gv_va_graphics_alphablend_table_work2[i];
		}

		// アルファ差分：透明度変換テーブル
		wp = Gv_va_graphics_alphasub_table_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = i * (255 - j) / 255;
			}
			Gv_va_graphics_alphasub_table_tr_adr[i] = Gv_va_graphics_alphasub_table_tr[i];
		}
	
		Gv_va_graphics_alphablend_table_tr_adr_offset = (DWORD)Gv_va_graphics_alphablend_table_tr_adr;	
		Gv_va_graphics_alphablend_table_work1_adr_offset = (DWORD)Gv_va_graphics_alphablend_table_work1_adr;
		Gv_va_graphics_alphablend_table_work2_adr_offset = (DWORD)Gv_va_graphics_alphablend_table_work2_adr;
		Gv_va_graphics_alphasub_table_tr_adr_offset = (DWORD)Gv_va_graphics_alphasub_table_tr_adr;	
	
		Gv_va_graphics_alphablend_table_make_flag = true;
	}

	// ****************************************************************
	// アルファ合成アセンブラ定義
	// ================================================================

#if __USE_ASM_INTEL

	// テーブル定義
	#define		__TABLE_ALPHABLEND_TR		Gv_va_graphics_alphablend_table_tr_adr_offset
	#define		__TABLE_ALPHABLEND_WORK1	Gv_va_graphics_alphablend_table_work1_adr_offset
	#define		__TABLE_ALPHABLEND_WORK2	Gv_va_graphics_alphablend_table_work2_adr_offset

#endif

	// ****************************************************************
	// アルファ合成描画
	//		IN		BYTE *d_buf  : 転送先ポインタ
	//				int d_xl     : 転送先ＸＬ
	//				int d_yl     : 転送先ＹＬ
	//				BYTE *s_buf  : 転送元ポインタ
	//				int s_xl     : 転送元ＸＬ
	//				int s_yl     : 転送元ＹＬ
	//				int x        : 転送座標
	//				int y        : 転送座標
	// ================================================================
	void f_draw_alphablend(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, int blend_type)
	{
		const int _PIXEL_SIZE = 4;	// ▲固定

		if (s_buf == NULL || s_xl <= 0 || s_yl <= 0 || d_buf == NULL || d_xl <= 0 || d_yl <= 0)
			return;
	
		// アルファ合成テーブル作成
		if (!Gv_va_graphics_alphablend_table_make_flag)	{	f_make_alphablend_table();	}
	
		if (x >= d_xl || y >= d_yl)	{ return; }

		int cnt_x, cnt_y, d_free_x, d_free_y, s_free_x, s_free_y;

		if (x < 0) { s_free_x = -x; cnt_x = s_xl + x; d_free_x = 0; }
		else { s_free_x = 0; cnt_x = s_xl; d_free_x = x; }
		if ((d_xl - d_free_x) < cnt_x) { cnt_x = d_xl - d_free_x; }
		if (cnt_x <= 0)	{ return; }

		if (y < 0) { s_free_y = -y;	cnt_y = s_yl + y; d_free_y = 0; }
		else { s_free_y = 0; cnt_y = s_yl; d_free_y = y; }
		if ((d_yl - d_free_y) < cnt_y) { cnt_y = d_yl - d_free_y; }
		if (cnt_y <= 0)	{ return; }

		s_buf += (s_free_x + (s_free_y * s_xl)) * _PIXEL_SIZE;
		d_buf += (d_free_x + (d_free_y * d_xl)) * _PIXEL_SIZE;
		s_free_x = (s_xl - cnt_x) * _PIXEL_SIZE;
		d_free_x = (d_xl - cnt_x) * _PIXEL_SIZE;

#if __USE_ASM_INTEL
		_asm{
			mov		edi, d_buf
			mov		esi, s_buf
			mov		ecx, cnt_y
		__loop_y:
			push	ecx
			mov		ecx, cnt_x
		__loop_x:
		
			// 転送元の透明度が 0 の時は何もしない
			xor		ebx, ebx
			mov		bl, [esi + 3]
			and		ebx, ebx
			jnz		__step_draw
			jmp		__step_no_draw
		
		__step_draw:
		
			// 転送元の透明度が 255 の時はそのままコピー
			cmp		ebx, 255
			jne		__step_draw_check_dst_alpha
			movsd
			jmp		__step_next		// 次へ
		
		__step_draw_check_dst_alpha:
		
			// 転送先の透明度が 0 の時はそのままコピー
			xor		eax, eax
			mov		al, [edi + 3]
			and		eax, eax
			jnz		__step_draw_main
			movsd
			jmp		__step_next		// 次へ
		
		__step_draw_main:		
			push	ecx

			// 透明度を４倍にする
			shl		ebx, 2					// ebx = 転送元の透明度
			shl		eax, 2					// eax = 転送先の透明度
		
			// 透明度の合成（結果は ecx に格納）
			mov		ecx, ebx				// ebx = 転送元の透明度
			add		ecx, __TABLE_ALPHABLEND_TR
			mov		ecx, [ecx]
			add		ecx, eax				// eax = 転送先の透明度
			mov		ecx, [ecx]				// ecx = 合成された透明度
		
			// 合成された透明度が 0 の時は何もしない
			and		ecx, ecx
			jnz		__abcopy_alpha_check_ok
			jmp		__abcopy_alphablend_skip
		
		__abcopy_alpha_check_ok:
		
			// WORK1 の結果値を算出（結果は edx に格納）
			mov		edx, ebx				// ebx = 転送元の透明度
			add		edx, __TABLE_ALPHABLEND_WORK1
			mov		edx, [edx]
			add		edx, eax				// eax = 転送先の透明度
			mov		edx, [edx]				// edx = WORK1 の結果値

			// WORK2 の中間結果値を算出（結果は eax に格納）
			mov		eax, ebx				// ebx = 転送元の透明度
			add		eax, __TABLE_ALPHABLEND_WORK2
			mov		eax, [eax]				// eax = WORK2 の中間結果値

			// R
			xor		ebx, ebx
			mov		bl, [esi + 2]
			shl		ebx, 2
			add		ebx, eax				// eax = WORK2 の中間結果値
			mov		ebx, [ebx]
			push	ebx						// R push
		
			// G
			xor		ebx, ebx
			mov		bl, [esi + 1]
			shl		ebx, 2
			add		ebx, eax				// eax = WORK2 の中間結果値
			mov		ebx, [ebx]
			push	ebx						// G push
		
			// B
			xor		ebx, ebx
			mov		bl, [esi]
			shl		ebx, 2
			add		ebx, eax				// eax = WORK2 の中間結果値
			mov		ebx, [ebx]
			push	ebx						// B push

			// 後の mul 演算の都合上、edx の値が変化してしまうので、ebx に値を移す
			mov		ebx, edx				// edx = WORK1 の結果値
	
			// push / pop の関係があるので RGB の処理が逆順になる事に注意
		
			// B
			xor		eax, eax
			mov		al, [edi]
			mul		ebx						// ebx = WORK1 の結果値
			pop		edx						// B pop
			add		eax, edx
			shr		eax, 8
			xor		edx, edx				// div の前にクリア
			div		ecx						// ecx = 合成された透明度
			mov		[edi], al
		
			// G
			xor		eax, eax
			mov		al, [edi + 1]
			mul		ebx						// ebx = WORK1 の結果値
			pop		edx						// G pop
			add		eax, edx
			shr		eax, 8
			xor		edx, edx				// div の前にクリア
			div		ecx						// ecx = 合成された透明度
			mov		[edi + 1], al
		
			// R
			xor		eax, eax
			mov		al, [edi + 2]
			mul		ebx						// ebx = WORK1 の結果値
			pop		edx						// R pop
			add		eax, edx
			shr		eax, 8
			xor		edx, edx				// div の前にクリア
			div		ecx						// ecx = 合成された透明度
			mov		[edi + 2], al

			// A
			mov		[edi + 3], cl
		
		__abcopy_alphablend_skip:
			pop		ecx
		
		__step_no_draw:
			add		edi, _PIXEL_SIZE
			add		esi, _PIXEL_SIZE
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
#else

		// 参考 → http://misohena.jp/article/pixel_composite.html

		if (blend_type == E_d3d_blend_type_add)
		{
			BYTE *dp = d_buf;
			BYTE *sp = s_buf;
			int d_a, s_a, r_a;
			for (int i = 0; i < cnt_y; i++)	{
				for (int j = 0; j < cnt_x; j++)	{
					s_a = *(sp + 3);
					if (s_a)	{
						d_a = *(dp + 3);
						r_a = 255 - (255 - s_a) * (255 - d_a) / 255;
						for (int k = 0; k < 3; k++)	{
							int d_c = *(dp + k);
							int s_c = *(sp + k);
							int r_c = (s_a * d_a * std::min(s_c + d_c, 255) + s_a * (255 - d_a) * s_c + (255 - s_a) * d_a * d_c) / r_a / 255;
							*(dp + k) = r_c;
						}
						*(dp + 3) = r_a;
					}
					dp += _PIXEL_SIZE;
					sp += _PIXEL_SIZE;
				}
				dp += d_free_x;
				sp += s_free_x;
			}
		}
		else if (blend_type == E_d3d_blend_type_mul)
		{
			BYTE *dp = d_buf;
			BYTE *sp = s_buf;
			int d_a, s_a, r_a;
			for (int i = 0; i < cnt_y; i++)	{
				for (int j = 0; j < cnt_x; j++)	{
					s_a = *(sp + 3);
					if (s_a)	{
						d_a = *(dp + 3);
						r_a = 255 - (255 - s_a) * (255 - d_a) / 255;
						for (int k = 0; k < 3; k++)	{
							int d_c = *(dp + k);
							int s_c = *(sp + k);
							int r_c = (s_a * d_a * (s_c * d_c / 255) + s_a * (255 - d_a) * s_c + (255 - s_a) * d_a * d_c) / r_a / 255;
							*(dp + k) = r_c;
						}
						*(dp + 3) = r_a;
					}
					dp += _PIXEL_SIZE;
					sp += _PIXEL_SIZE;
				}
				dp += d_free_x;
				sp += s_free_x;
			}
		}
		else
		{
			BYTE *dp = d_buf;
			BYTE *sp = s_buf;
			int d_tr, s_tr, tr, work1;
			int *work2;
			for (int i = 0; i < cnt_y; i++)	{
				for (int j = 0; j < cnt_x; j++)	{
					s_tr = *(sp + 3);
					if (s_tr)	{
						if (s_tr == 255)	{
							// 転送元の透明度が 255 の時はそのままコピー
							*(DWORD *)dp = *(DWORD *)sp;
						}
						else	{
							d_tr = *(dp + 3);
							if (d_tr == 0)	{
								// 転送先の透明度が 0 の時はそのままコピー
								*(DWORD *)dp = *(DWORD *)sp;
							}
							else	{
								tr = *(*(Gv_va_graphics_alphablend_table_tr_adr + s_tr) + d_tr);
								if (tr)	{
									work1 = *(*(Gv_va_graphics_alphablend_table_work1_adr + s_tr) + d_tr);
									work2 = *(Gv_va_graphics_alphablend_table_work2_adr + s_tr);
									*dp = ((*(work2 + *sp) + (work1 * *dp)) >> 8) / tr;
									*(dp + 1) = ((*(work2 + *(sp + 1)) + (work1 * *(dp + 1))) >> 8) / tr;
									*(dp + 2) = ((*(work2 + *(sp + 2)) + (work1 * *(dp + 2))) >> 8) / tr;
									*(dp + 3) = (BYTE)tr;
								}
							}
						}
					}
					dp += _PIXEL_SIZE;
					sp += _PIXEL_SIZE;
				}
				dp += d_free_x;
				sp += s_free_x;
			}
		}
#endif
	}


	bool C_d3d_texture::load_g00_cut(C_g00_cut* g00_cut, bool futidori)
	{
		int BURE = 1;	// 縁取り作成時のブレの大きさ

		// ブレンドフラグ
		m_blend = true;

		// カットの情報を取得
		S_g00_cut_info cut_info;
		g00_cut->get_info(&cut_info);

		// テクスチャを作成（オリジナルサイズではなく最小矩形分だけ作成する）
		// 縁取る場合は 2px ずつ大きく取る
		if (futidori)	{
			if (!create(cut_info.disp_rect.right - cut_info.disp_rect.left + BURE * 2, cut_info.disp_rect.bottom - cut_info.disp_rect.top + BURE * 2))
				return false;
		}
		else	{
			if (!create(cut_info.disp_rect.right - cut_info.disp_rect.left, cut_info.disp_rect.bottom - cut_info.disp_rect.top))
				return false;
		}

		// オリジナルの大きさを設定
		m_org_width = cut_info.width;
		m_org_height = cut_info.height;

		// テクスチャをロック
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		// テクスチャを透明に塗りつぶす
		for (int y = 0; y < m_height_ex; y++)
			ZeroMemory((BYTE *)texture_bit.pBits + texture_bit.Pitch * y, m_width_ex * sizeof(C_argb));

		// チップの処理
		if (futidori)	{

			// 縁取る場合は、ダミーのビットマップに一度展開する
			// テクスチャに９方向に描画することで縁取りする
			// 結構重い処理です。

			// ダミーのビットマップを準備
			// m_width などを使ってはいけません。縁取り分のサイズが入っているからです。
			C_dib dib;
			dib.create(cut_info.disp_rect.right - cut_info.disp_rect.left, cut_info.disp_rect.bottom - cut_info.disp_rect.top, 32);

			// テクスチャに全チップを描画して、１枚のカットを構築する
			for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

				// チップを取得
				C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
				// チップの情報を取得
				S_g00_chip_info chip_info;
				g00_chip->get_info(&chip_info);
				g00_chip->get_data(dib.get_ptr(chip_info.x - cut_info.disp_rect.left, chip_info.y - cut_info.disp_rect.top), dib.get_width() * 4);
			}

			// テクスチャに９方向に描画することで縁取りを実現する
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), 0, 0, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 2, 0, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), 0, BURE * 2, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 2, BURE * 2, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 1, 0, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), 0, BURE * 1, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 2, BURE * 1, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 1, BURE * 2, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 1, BURE * 1, E_d3d_blend_type_alpha);

			// ブレンドする
			m_blend = true;
		}
		else	{

			// テクスチャに全チップを描画して、１枚のカットを構築する
			for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

				// チップを取得
				C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
				// チップの情報を取得
				S_g00_chip_info chip_info;
				g00_chip->get_info(&chip_info);
				g00_chip->get_data((BYTE *)texture_bit.pBits + texture_bit.Pitch * (chip_info.y - cut_info.disp_rect.top) + (chip_info.x - cut_info.disp_rect.left) * 4, texture_bit.Pitch);

				// エッジ処理（隣のピクセルに同じ色を置く処理）
				// 補完拡大したとき、隣に黒があると黒がにじんでしまうのを防ぐための処理です。
				add_edge_for_filterring(&texture_bit);

				// ブレンドする必要のあるチップがあったならばブレンド
				if (!m_blend && chip_info.sprite)
					m_blend = true;
			}
		}

		// テクスチャをアンロック
		if (!unlock_rect(0))
			return false;

		// 中心座標を設定
		set_center(C_point(cut_info.center) - C_point(cut_info.disp_rect.left, cut_info.disp_rect.top));

		return true;
	}

	// ****************************************************************
	// g00 カットを書き込み
	// ================================================================
	bool C_d3d_texture::draw_g00_cut(C_g00_cut* g00_cut, int x, int y, int blend_type)
	{
		// テクスチャをロック
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		// カットの情報を取得
		S_g00_cut_info cut_info;
		g00_cut->get_info(&cut_info);

		// ダミーのビットマップを準備
		C_dib dib;
		dib.create(cut_info.disp_rect.right - cut_info.disp_rect.left, cut_info.disp_rect.bottom - cut_info.disp_rect.top, 32);

		// テクスチャに全チップを描画して、１枚のカットを構築する
		for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

			// チップを取得
			C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
			// チップの情報を取得
			S_g00_chip_info chip_info;
			g00_chip->get_info(&chip_info);
			g00_chip->get_data(dib.get_ptr(chip_info.x - cut_info.disp_rect.left, chip_info.y - cut_info.disp_rect.top), dib.get_width() * 4);
		}

		// 中心補正
		x += m_center.x + cut_info.disp_rect.left - cut_info.center.x;
		y += m_center.y + cut_info.disp_rect.top - cut_info.center.y;

		// テクスチャに描画
		f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), x, y, blend_type);

		// テクスチャをアンロック
		if (!unlock_rect(0))
			return false;

		return true;
	}

	// ****************************************************************
	// フィルタリング処理のためにエッジを加える（ロックしない）
	// ================================================================
	bool C_d3d_texture::add_edge_for_filterring(D3DLOCKED_RECT* p_texture_bit)
	{
		// 下
		if (m_height_ex > m_height)	{
			BYTE* p_dst = (BYTE *)p_texture_bit->pBits + p_texture_bit->Pitch * m_height;
			BYTE* p_src = p_dst - p_texture_bit->Pitch;
			memcpy(p_dst, p_src, m_width * sizeof(C_argb));
		}
		// 右
		if (m_width_ex > m_width)	{
			BYTE* p_dst = (BYTE *)p_texture_bit->pBits + m_width * sizeof(C_argb);
			BYTE* p_src = p_dst - sizeof(C_argb);
			for (int y = 0; y < m_height; y++)	{
				*(C_argb *)p_dst = *(C_argb *)p_src;
				p_dst += p_texture_bit->Pitch;
				p_src += p_texture_bit->Pitch;
			}
			// 右下
			if (m_height_ex > m_height)	{
				*(C_argb *)p_dst = *(C_argb *)p_src;
			}
		}

		return true;
	}

	// ****************************************************************
	// ピクセルのカラーを取得
	// ================================================================
	C_argb C_d3d_texture::get_color(int x, int y)
	{
		// 中心分位置をずらす
		x += m_center.x;
		y += m_center.y;

		// 範囲チェック
		if (x < 0 || m_width <= x || y < 0 || m_height <= y)
			return C_argb(0, 0, 0, 0);

		// テクスチャをロック
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return C_argb(0, 0, 0, 0);

		C_argb color = *(C_argb *)(((BYTE *)texture_bit.pBits + texture_bit.Pitch * y) + x * sizeof(C_argb));

		// テクスチャをアンロック
		if (!unlock_rect(0))
			return C_argb(0, 0, 0, 0);

		return color;
	}
}
