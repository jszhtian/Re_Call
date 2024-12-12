#pragma		once

#include	"tona3_string.h"
#include	"tona3_array.h"

namespace NT3
{
	// ****************************************************************
	// �����R�[�h�Z�b�g �� ������
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
	// �t�H���g���c���[�h
	// ================================================================
	enum FONT_YOKOTATE_MODE
	{
		FONT_YOKOTATE_MODE_NONE,			// �ǂ���ł�����
		FONT_YOKOTATE_MODE_YOKO_ONLY,		// �������̂�
		FONT_YOKOTATE_MODE_TATE_ONLY,		// �c�����̂�
		FONT_YOKOTATE_MODE_BOTH,			// �c���̗����ɑΉ�
	};

	// ****************************************************************
	// �����t�F�C�X�^�C�v
	// ================================================================
	enum MOJI_FACE_TYPE
	{
		MOJI_FACE_TYPE_NONE,
		MOJI_FACE_TYPE_4,
		MOJI_FACE_TYPE_9,
		MOJI_FACE_TYPE_16,
	};

	// ****************************************************************
	// �t�H���g���w�肵�ĉ������擾
	// ================================================================
	inline int font_get_width(HWND h_wnd, CTSTR& str, HFONT h_font)
	{
		// font ���w�肳��Ă��Ȃ��ꍇ�̓f�t�H���g�� GUI �t�H���g���g��
		if (h_font == NULL)	{
			h_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		}

		HDC h_dc = GetDC(h_wnd);
		HFONT old_font = (HFONT)SelectObject(h_dc, h_font);

		C_size size;
		GetTextExtentPoint32(h_dc, str.c_str(), (int)str.size(), &size);
		SelectObject(h_dc, old_font);	// �t�H���g�����ɖ߂�
		ReleaseDC(h_wnd, h_dc);

		return size.cx;
	}

	// ****************************************************************
	// �t�H���g���X�g
	// ================================================================
	class C_font_name_list
	{
	public:
		bool	make_font_list(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode);				// �t�H���g���X�g�쐬
		void	free_font_list()	{ m_font_list.clear(); }																					// �t�H���g���X�g�J��
		bool	find_font(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode, TSTR font_name);	// �t�H���g����

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

		static LRESULT CALLBACK make_callback_func(ENUMLOGFONT *elf, NEWTEXTMETRIC *lpntm, int type, LPARAM lp);	// �t�H���g���X�g�쐬�R�[���o�b�N�֐�
	};

	// ****************************************************************
	// �t�H���g����r�֐�
	// ================================================================
	inline bool font_less_func(ENUMLOGFONT& lhs, ENUMLOGFONT& rhs)
	{
		return TSTR(lhs.elfLogFont.lfFaceName) < TSTR(rhs.elfLogFont.lfFaceName);
	}

	// ****************************************************************
	// �t�H���g����r�t�@���N�^
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
	// �t�H���g�A���t�@�����R�s�[
	// ================================================================
	class Cfont_copy
	{
	public:

		// �U�U�K���t�H���g���R�Q�r�b�g�ɃR�s�[�iGGO_GRAY8_BITMAP �� 32bitBMP�j
		void		gray8_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, C_argb color);

		// �t�H���g�A���t�@�����R�s�[
		void		alphablend_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y);

		// �t�H���g�A���t�@�����R�s�[�i�܂Ƃ��j
		void		alphablend_copy_face(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, MOJI_FACE_TYPE face_type);

	private:

		// �A���t�@�����e�[�u���쐬
		void		make_alphablend_table();

	};
};
