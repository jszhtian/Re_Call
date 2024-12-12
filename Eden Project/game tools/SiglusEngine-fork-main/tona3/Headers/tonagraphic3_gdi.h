#pragma		once

namespace NT3
{

	class C_font
	{
	public:
		C_font();
		~C_font();

		void	release();
		void	select_font(CTSTR& name, int char_set, int size);
		void	select_size(int size);

		HDC		get_h_dc()	{	return m_h_dc;	}

	private:
		HDC		m_h_dc;
		HFONT	m_h_font;
		HGDIOBJ	m_h_old_font;

		int		m_font_size;
		int		m_font_char_set;
		TSTR	m_font_name;

		// コピー禁止！
		C_font(C_font&);
		C_font& operator = (C_font&);
	};

}
