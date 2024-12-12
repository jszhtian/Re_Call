#pragma		once

#include	"tona3_string.h"
#include	"tona3_array.h"

namespace NT3
{
	// ****************************************************************
	// CSV ƒtƒ@ƒCƒ‹
	// ================================================================
	class C_csv
	{
	public:
		C_csv();
		~C_csv();

		bool	load_csv(CTSTR& file_path);
		TSTR	get_item(int row_no, int column_no);
		int		get_row_cnt()		{	return m_row_cnt;		}
		int		get_column_cnt()	{	return m_column_cnt;	}

	private:
		ARRAY< ARRAY<TSTR> >	m_csv_data;
		int						m_row_cnt;
		int						m_column_cnt;
	};
};
