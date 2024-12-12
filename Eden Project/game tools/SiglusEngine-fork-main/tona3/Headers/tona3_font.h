#pragma		once

#include	"tona3_string.h"
#include	"tona3_array.h"

namespace NT3
{
	// ****************************************************************
	// 文字コードセット → 文字列
	// ================================================================
	inline TSTR tostr_CHARSET(BYTE char_set)
	{
		switch (char_set)	{
		case ANSI_CHARSET:			return _T("ANSI");
		case DEFAULT_CHARSET:		return _T("DEFAULT");
		case SYMBOL_CHARSET:		return _T("SYMBOL");
		case SHIFTJIS_CHARSET:		return _T("SHIFTJIS");
		case HANGEUL_CHARSET:		return _T("HANGEUL");
		case GB2312_CHARSET:		return _T("GB2312");
		case CHINESEBIG5_CHARSET:	return _T("CHINESEBIG5");
		case OEM_CHARSET:			return _T("OEM");
		case JOHAB_CHARSET:			return _T("JOHAB");
		case HEBREW_CHARSET:		return _T("HEBREW");
		case ARABIC_CHARSET:		return _T("ARABIC");
		case GREEK_CHARSET:			return _T("GREEK");
		case TURKISH_CHARSET:		return _T("TURKISH");
		case VIETNAMESE_CHARSET:	return _T("VIETNAMESE");
		case THAI_CHARSET:			return _T("THAI");
		case EASTEUROPE_CHARSET:	return _T("EASTEUROPE");
		case RUSSIAN_CHARSET:		return _T("RUSSIAN");
		case MAC_CHARSET:			return _T("MAC");
		case BALTIC_CHARSET:		return _T("BALTIC");
		default:					return _T("???");
		}
	}

	// ****************************************************************
	// フォント横縦モード
	// ================================================================
	enum FONT_YOKOTATE_MODE
	{
		FONT_YOKOTATE_MODE_NONE,			// どちらでもいい
		FONT_YOKOTATE_MODE_YOKO_ONLY,		// 横書きのみ
		FONT_YOKOTATE_MODE_TATE_ONLY,		// 縦書きのみ
		FONT_YOKOTATE_MODE_BOTH,			// 縦横の両方に対応
	};

	// ****************************************************************
	// 文字フェイスタイプ
	// ================================================================
	enum MOJI_FACE_TYPE
	{
		MOJI_FACE_TYPE_NONE,
		MOJI_FACE_TYPE_4,
		MOJI_FACE_TYPE_9,
		MOJI_FACE_TYPE_16,
	};

	// ****************************************************************
	// フォントを指定して横幅を取得
	// ================================================================
	inline int font_get_width(HWND h_wnd, CTSTR& str, HFONT h_font)
	{
		// font が指定されていない場合はデフォルトの GUI フォントを使う
		if (h_font == NULL)	{
			h_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		}

		HDC h_dc = GetDC(h_wnd);
		HFONT old_font = (HFONT)SelectObject(h_dc, h_font);

		C_size size;
		GetTextExtentPoint32(h_dc, str.c_str(), (int)str.size(), &size);
		SelectObject(h_dc, old_font);	// フォントを元に戻す
		ReleaseDC(h_wnd, h_dc);

		return size.cx;
	}

	// ****************************************************************
	// フォントリスト
	// ================================================================
	class C_font_name_list
	{
	public:
		bool	make_font_list(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode);				// フォントリスト作成
		void	free_font_list()	{ m_font_list.clear(); }																					// フォントリスト開放
		bool	find_font(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode, TSTR font_name);	// フォント検索

		ARRAY<ENUMLOGFONT>		m_font_list;

	private:

		struct WORK
		{
			bool				truetype_only;
			bool				toufuku_only;
			DWORD				char_set;
			FONT_YOKOTATE_MODE	yokotate_mode;
			ARRAY<ENUMLOGFONT>	yoko_gaki_font_list;
			ARRAY<ENUMLOGFONT>	tate_gaki_font_list;
			ARRAY<ENUMLOGFONT>	temp_font_list;
		}	work;

		static LRESULT CALLBACK make_callback_func(ENUMLOGFONT *elf, NEWTEXTMETRIC *lpntm, int type, LPARAM lp);	// フォントリスト作成コールバック関数
	};

	// ****************************************************************
	// フォント名比較関数
	// ================================================================
	inline bool font_less_func(ENUMLOGFONT& lhs, ENUMLOGFONT& rhs)
	{
		return TSTR(lhs.elfLogFont.lfFaceName) < TSTR(rhs.elfLogFont.lfFaceName);
	}

	// ****************************************************************
	// フォント名比較ファンクタ
	// ================================================================
	class C_font_name_functor
	{
	public:

		C_font_name_functor(CTSTR& font_name)
		{
			m_font_name = font_name;
		}

		bool operator()(ENUMLOGFONT& font)
		{
			return font.elfLogFont.lfFaceName == m_font_name;
		}

	private:
		TSTR	m_font_name;
	};

	// ****************************************************************
	// フォントアルファ合成コピー
	// ================================================================
	class Cfont_copy
	{
	public:

		// ６６階調フォントを３２ビットにコピー（GGO_GRAY8_BITMAP → 32bitBMP）
		void		gray8_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, C_argb color);

		// フォントアルファ合成コピー
		void		alphablend_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y);

		// フォントアルファ合成コピー（袋とじ）
		void		alphablend_copy_face(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, MOJI_FACE_TYPE face_type);

	private:

		// アルファ合成テーブル作成
		void		make_alphablend_table();

	};
};
