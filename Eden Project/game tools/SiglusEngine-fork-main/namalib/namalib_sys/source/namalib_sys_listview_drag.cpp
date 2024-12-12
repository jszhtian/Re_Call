#include	"pch.h"
#include	"namalib_sys_listview_drag.h"

namespace Nnama
{

// ****************************************************************
// コンストラクタ
// ================================================================
Cnamalib_listview_drag::Cnamalib_listview_drag()
{
	lstv = NULL;
}

// ****************************************************************
// 開始
// ※１
// 　リストビューがフォーカスされていない場合でもドラッグが開始される事がある。
// 　その場合、フォーカスが外れた場合の処理に引っかかる為、ドラッグ処理がすぐにキャンセルされてしまう。
// 　なので、フォーカスが来てなかった場合は、強制的にフォーカスする。
// ※２
// 　ドラッグが開始された後、かなり遅れてアイテムへのフォーカスが来る事がある。
// 　自分で行ったリストのスクロール処理より遅れて来た場合、
// 　フォーカスされたアイテムが可視範囲に入るようリスト位置が調整されるので都合が悪い。
// 　なので、ドラッグ開始時にフォーカスされたアイテムが無い状態にする。
// ================================================================
void Cnamalib_listview_drag::start(Cc_listview* _lstv)
{
	lstv = _lstv;
	target_listview = _lstv;
	if (lstv->get_handle() != GetFocus())	{	SetFocus(lstv->get_handle());	}		// ※１
	lstv->set_item_focus(lstv->get_focused_item_no(), false);						// ※２
	drag_line.create(lstv->get_name() + _T("_drag_line"), _T(""), 10, 10, 10, 10, false, WS_CHILD | SS_GRAYRECT | WS_BORDER, 0, false, false, lstv->get_handle());
	drag_line_target_item_no = -1;
	lstv_scroll_update = false;
	lstv_top_item_no = (int)lstv->send_message(LVM_GETTOPINDEX, 0, 0);
	scroll_time = GetTickCount();
}

// ****************************************************************
// 終了
// ================================================================
void Cnamalib_listview_drag::end()
{
	lstv = NULL;
	drag_line.destroy();
}

// ****************************************************************
// プロセス
// ================================================================
int Cnamalib_listview_drag::proc()
{
	if (lstv == NULL)	{	return -2;	}

	if (lstv->get_handle() != GetFocus())	{	end();	return -2;	}

	if (GetAsyncKeyState((int)VK_ESCAPE) & 0x8000)	{	end();	return -2;	}

	if (!(GetAsyncKeyState((int)VK_LBUTTON) & 0x8000))	{
		end();
		if (drag_line_target_item_no == -1)	{	return -2;	}
		else	{	return drag_line_target_item_no;	}
	}

	set_drag_line();	// ドラッグライン設定

	return -1;
}

// ****************************************************************
// ターゲットリストビュー判定
// ================================================================
bool Cnamalib_listview_drag::check_target_listview(Cc_listview* _lstv)
{
	return (target_listview == _lstv) ? true : false;
}

// ****************************************************************
// ドラッグライン設定
// ================================================================
void Cnamalib_listview_drag::set_drag_line()
{
	int no = set_drag_line_func();

	if (drag_line_target_item_no != no)	{
		if (no != -1)	{
			set_drag_line_draw(no);
			lstv_scroll_update = true;
			drag_line.set_show(true);
		}
		else	{
			drag_line.set_show(false);
		}
		drag_line_target_item_no = no;
	}
}

int Cnamalib_listview_drag::set_drag_line_func()
{
	int item_cnt = lstv->get_item_cnt();
	if (item_cnt <= 0)	{	return -1;	}

	int top_item_no = (int)lstv->send_message(LVM_GETTOPINDEX, 0, 0);
	if (top_item_no < 0)	{	return -1;	}

	// リストビュー更新（この処理がないと、ドラッグラインの表示がおかしくなる）
	if (lstv_top_item_no != top_item_no && lstv_scroll_update)	{
		lstv_top_item_no = top_item_no;
		lstv->invalidate_rect(NULL, TRUE);
		lstv_scroll_update = false;
	}

	C_point mpos;
	GetCursorPos(&mpos);
	ScreenToClient(lstv->get_handle(), &mpos);

	C_rect crect = lstv->get_client_rect();

	C_rect rect = lstv->get_cell_rect(top_item_no, 0);
	int item_height = rect.bottom - rect.top;	// 項目１つ分の高さ
	if (item_height <= 0)	{	return -1;	}
	crect.top = rect.top;

	if (!(mpos.x >= crect.left && mpos.x <= crect.right && mpos.y >= crect.top && mpos.y <= crect.bottom))	{
		// スクロール処理
		if (mpos.y < crect.top || mpos.y > crect.bottom)	{
			int time = GetTickCount();
			int alpha, scroll_item_no;
			int scroll_step = 1;
			if (mpos.y < crect.top)	{
				alpha = 40 - ((crect.top - mpos.y) / 2);
			}
			else	{
				alpha = 40 - ((mpos.y - crect.bottom) / 2);
			}
			if (alpha < 0)	{
				scroll_step += ((-alpha) / 4);	// スクロール項目数を大きくする
				alpha = 0;
			}
			DWORD wait = 2 * alpha;
			if ((DWORD)(time - scroll_time) > wait)	{
				if (mpos.y < crect.top)	{
					scroll_item_no = top_item_no - scroll_step;
					if (scroll_item_no < 0)	{	scroll_item_no = 0;	}
				}
				else	{
					scroll_item_no = top_item_no + scroll_step;
					if (scroll_item_no >= item_cnt)	{	scroll_item_no = item_cnt - 1;	}	
				}
				lstv->set_item_visible_top(scroll_item_no);
				scroll_time = GetTickCount();
			}
		}
		return -1;
	}

	int target_item_no = ((mpos.y - crect.top) / item_height) + top_item_no;
	if (target_item_no > item_cnt)	{	target_item_no = item_cnt;	}

	return target_item_no;
}

void Cnamalib_listview_drag::set_drag_line_draw(int item_no)
{
	C_rect crect = lstv->get_client_rect();
	int item_cnt = lstv->get_item_cnt();
	if (item_no < item_cnt)	{
		C_rect rect = lstv->get_cell_rect(item_no, 0);
		crect.top = rect.top - 2;
	}
	else	{
		C_rect rect = lstv->get_cell_rect(item_no - 1, 0);
		crect.top = rect.bottom - 1;
	}
	crect.bottom = crect.top + 3;
	drag_line.set_window_rect(crect);
}

}