#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_gdi.h"

namespace NT3
{
	C_font::C_font()
	{
		m_h_dc = NULL;
		m_h_font = NULL;
		m_h_old_font = NULL;
		m_font_size = 0;
		m_font_char_set = -1;
	}

	C_font::~C_font()
	{
		release();
	}

	void C_font::release()
	{
		if (m_h_old_font)
		{
			::SelectObject(m_h_dc, m_h_old_font);
			m_h_old_font = NULL;
		}

		if (m_h_font)
		{
			::DeleteObject(m_h_font);
			m_h_font = NULL;
		}

		if (m_h_dc)
		{
			::DeleteDC(m_h_dc);
			m_h_dc = NULL;
		}
	}

	void C_font::select_font(CTSTR& name, int char_set, int size)
	{
		// まだフォントがないか、フォントが変わった場合のみ作り直す
		if (!m_h_old_font || name != m_font_name || char_set != m_font_char_set || size != m_font_size)
		{
			release();

			// フォントを設定
			LOGFONT log_font = { 0 };
			log_font.lfCharSet = char_set;
			log_font.lfHeight = -size;	// マイナスを指定する！（メイリオ対策）
			_tcscpy_s(log_font.lfFaceName, sizeof(log_font.lfFaceName) / sizeof(TCHAR), name.c_str());

			// フォントを作成する
			m_h_dc = ::CreateCompatibleDC(0);
			m_h_font = ::CreateFontIndirect(&log_font);
			m_h_old_font = ::SelectObject(m_h_dc, (HGDIOBJ)m_h_font);

			// フォントの情報を記憶
			m_font_size = size;
			m_font_name = name;

#if 0
			// 以下、メイリオ対策
			// tm.tmInternalLeading が 0 でないフォントに対しての処理。
			// いくらなんでもかっこわるい…さらに 1px ずれる。少数の切り上げ下げの問題？
			// それ修正すると普通なフォントでバグりそうなのでもう無視する。ごめんねメイリオ。

			// SelectObject したので GetTextMetrics で情報が取得できる
			HFONT h_prev_font = m_h_font;	// １つ前のフォントを記憶しておく
			while (1)
			{
				TEXTMETRIC tm;
				GetTextMetrics(m_h_dc, &tm);
				int fixed_size = m_log_font.lfHeight - tm.tmInternalLeading;

				// 等しければ問題なし
				if (fixed_size == size)
				{
					break;
				}

				// 小さければ大きくしてみる
				else if (fixed_size < size)
				{
					release();

					m_log_font.lfHeight ++;
					m_h_dc = ::CreateCompatibleDC(0);
					m_h_font = ::CreateFontIndirect(&m_log_font);
					m_h_old_font = ::SelectObject(m_h_dc, (HGDIOBJ)m_h_font);
				}
				// 大きければ１個前のに戻して終了
				else if (fixed_size > size)
				{
					release();

					m_log_font.lfHeight --;
					m_h_dc = ::CreateCompatibleDC(0);
					m_h_font = ::CreateFontIndirect(&m_log_font);
					m_h_old_font = ::SelectObject(m_h_dc, (HGDIOBJ)m_h_font);

					break;
				}
			}
#endif

		}

	}

	void C_font::select_size(int size)
	{
		// まだフォントがないか、大きさが変わった場合のみ作り直す
		if (!m_h_old_font || size != m_font_size)
		{
			release();

			// フォントを設定
			LOGFONT log_font = { 0 };
			log_font.lfCharSet = m_font_char_set;
			log_font.lfHeight = -size;	// マイナスを指定する！（メイリオ対策）
			_tcscpy_s(log_font.lfFaceName, sizeof(log_font.lfFaceName) / sizeof(TCHAR), m_font_name.c_str());

			// フォントを作成する
			m_h_dc = ::CreateCompatibleDC(0);
			m_h_font = ::CreateFontIndirect(&log_font);
			m_h_old_font = ::SelectObject(m_h_dc, (HGDIOBJ)m_h_font);

			// フォントの情報を記憶
			m_font_size = size;
		}
	}
}
