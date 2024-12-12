#pragma		once

#include	"tonawindow3_wbase.h"

namespace NT3
{

// ****************************************************************
// ウィンドウ
// ================================================================
class C_window : public C_window_base
{
public:
	C_window()	{}
	~C_window()	{}

	bool	create(CTSTR& name, CTSTR& class_name, CTSTR& caption
		, int x, int y, int client_w, int client_h
		, int icon_id, int menu_id, int brush_type, DWORD style, DWORD ex_style, bool is_limit_min, bool is_attach
		, HWND h_parent_wnd = NULL, int adjust_way = 0);

protected:
	LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);

private:
	TSTR	m_class_name;

	bool	on_destroy();
};

};
