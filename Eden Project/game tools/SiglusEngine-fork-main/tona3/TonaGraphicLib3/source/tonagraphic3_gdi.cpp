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
		// �܂��t�H���g���Ȃ����A�t�H���g���ς�����ꍇ�̂ݍ�蒼��
		if (!m_h_old_font || name != m_font_name || char_set != m_font_char_set || size != m_font_size)
		{
			release();

			// �t�H���g��ݒ�
			LOGFONT log_font = { 0 };
			log_font.lfCharSet = char_set;
			log_font.lfHeight = -size;	// �}�C�i�X���w�肷��I�i���C���I�΍�j
			_tcscpy_s(log_font.lfFaceName, sizeof(log_font.lfFaceName) / sizeof(TCHAR), name.c_str());

			// �t�H���g���쐬����
			m_h_dc = ::CreateCompatibleDC(0);
			m_h_font = ::CreateFontIndirect(&log_font);
			m_h_old_font = ::SelectObject(m_h_dc, (HGDIOBJ)m_h_font);

			// �t�H���g�̏����L��
			m_font_size = size;
			m_font_name = name;

#if 0
			// �ȉ��A���C���I�΍�
			// tm.tmInternalLeading �� 0 �łȂ��t�H���g�ɑ΂��Ă̏����B
			// ������Ȃ�ł���������邢�c����� 1px �����B�����̐؂�グ�����̖��H
			// ����C������ƕ��ʂȃt�H���g�Ńo�O�肻���Ȃ̂ł�����������B���߂�˃��C���I�B

			// SelectObject �����̂� GetTextMetrics �ŏ�񂪎擾�ł���
			HFONT h_prev_font = m_h_font;	// �P�O�̃t�H���g���L�����Ă���
			while (1)
			{
				TEXTMETRIC tm;
				GetTextMetrics(m_h_dc, &tm);
				int fixed_size = m_log_font.lfHeight - tm.tmInternalLeading;

				// ��������Ζ��Ȃ�
				if (fixed_size == size)
				{
					break;
				}

				// ��������Α傫�����Ă݂�
				else if (fixed_size < size)
				{
					release();

					m_log_font.lfHeight ++;
					m_h_dc = ::CreateCompatibleDC(0);
					m_h_font = ::CreateFontIndirect(&m_log_font);
					m_h_old_font = ::SelectObject(m_h_dc, (HGDIOBJ)m_h_font);
				}
				// �傫����΂P�O�̂ɖ߂��ďI��
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
		// �܂��t�H���g���Ȃ����A�傫�����ς�����ꍇ�̂ݍ�蒼��
		if (!m_h_old_font || size != m_font_size)
		{
			release();

			// �t�H���g��ݒ�
			LOGFONT log_font = { 0 };
			log_font.lfCharSet = m_font_char_set;
			log_font.lfHeight = -size;	// �}�C�i�X���w�肷��I�i���C���I�΍�j
			_tcscpy_s(log_font.lfFaceName, sizeof(log_font.lfFaceName) / sizeof(TCHAR), m_font_name.c_str());

			// �t�H���g���쐬����
			m_h_dc = ::CreateCompatibleDC(0);
			m_h_font = ::CreateFontIndirect(&log_font);
			m_h_old_font = ::SelectObject(m_h_dc, (HGDIOBJ)m_h_font);

			// �t�H���g�̏����L��
			m_font_size = size;
		}
	}
}
