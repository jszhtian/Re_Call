#include	"tonawindow3_pch.h"
#include	"tonawindow3_listview.h"
#include	"tonawindow3_menu.h"

namespace NT3
{
	// ****************************************************************
	// リストビュー：ウィンドウプロシージャ
	// ================================================================
	LRESULT Cc_listview::window_proc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
			case WM_COMMAND:

				if (false);
				// ダミーコンボボックスからの通知メッセージ
				else if ((HWND)lp == dummy_combo.get_handle())
				{
					if (HIWORD(wp) == CBN_SELENDOK && editing)
					{
						end_edit_item_combobox(true);
					}
					if (HIWORD(wp) == CBN_SELENDCANCEL && editing)
					{
						end_edit_item_combobox(false);
					}
				}
				break;

			case WM_DESTROY:	if (!on_destroy())	return FALSE;	break;

			case WM_CONTEXTMENU:
				create_popup_menu();
				break;

		}

		return C_control::window_proc(msg, wp, lp);
	}

	// ****************************************************************
	// リストビュー：破壊された
	// ================================================================
	bool Cc_listview::on_destroy()
	{
		name_list.clear();

		return true;
	}

	// ****************************************************************
	// リストビュー：クリア
	// ================================================================
	void Cc_listview::clear()
	{
		send_message(LVM_DELETEALLITEMS, 0, 0);
	}

	// ****************************************************************
	// リストビュー：簡易設定
	//		is_full_row_select:		行全体を選択
	//		is_grid_lines:			行にグリッドをつける
	//		is_check_boxes:			チェックボックスをつける
	//		is_header_drag_drop:	ヘッダをドラッグ＆ドロップで移動できるようにする
	// ================================================================
	void Cc_listview::easy_setting(bool is_full_row_select, bool is_grid_lines, bool is_check_boxes, bool is_header_drag_drop)
	{
		DWORD style =
				( is_full_row_select ? LVS_EX_FULLROWSELECT : 0 )
			  | ( is_grid_lines ? LVS_EX_GRIDLINES : 0 )
			  | ( is_check_boxes ? LVS_EX_CHECKBOXES : 0 )
			  | ( is_header_drag_drop ? LVS_EX_HEADERDRAGDROP : 0 )
			  | (DWORD)send_message(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

		send_message(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, style);
	}

	// ****************************************************************
	// リストビュー：コラム数の取得
	// ================================================================
	int Cc_listview::get_column_cnt()
	{
		// リストビューのヘッダを取得
		HWND h_header = (HWND)send_message(LVM_GETHEADER, 0, 0);

		return (int)SendMessage(h_header, HDM_GETITEMCOUNT, 0, 0L);
	}

	// ****************************************************************
	// リストビュー：コラムの追加
	// ================================================================
	void Cc_listview::add_column(CTSTR& name, CTSTR& caption, int width, int align)
	{
		// 末尾にコラムを挿入
		int column_no = get_column_cnt();
		insert_column(column_no, name, caption, width, align);
	}

	// ****************************************************************
	// リストビュー：コラムの挿入
	// ================================================================
	void Cc_listview::insert_column(int column_no, CTSTR& name, CTSTR& caption, int width, int align)
	{
		LV_COLUMN column;
		column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		column.fmt = align;
		column.cx = width;
		column.pszText = (LPTSTR)caption.c_str();
		send_message(LVM_INSERTCOLUMN, (WPARAM)column_no, (LPARAM)&column);

		// 名前リストにも追加
		name_list.insert(name_list.begin() + column_no, name);
	}

	// ****************************************************************
	// リストビュー：コラムを削除
	// ================================================================
	void Cc_listview::delete_column(int column_no)
	{
		send_message(LVM_DELETEALLITEMS, column_no, 0);

		// 名前リストからも削除
		name_list.erase(name_list.begin() + column_no);
	}

	// ****************************************************************
	// リストビュー：全てのコラムを削除
	// ================================================================
	void Cc_listview::delete_all_column()
	{
		while (1)	{
			if (send_message(LVM_DELETECOLUMN, 0, 0) == FALSE)
				break;
		}

		// 名前リストからも削除
		name_list.clear();
	}

	// ****************************************************************
	// リストビュー：コラムの幅を設定
	// ================================================================
	void Cc_listview::set_column_width(int column_no, int width)
	{
		LV_COLUMN column;
		column.mask = LVCF_WIDTH;
		column.cx = width;
		send_message(LVM_SETCOLUMN, (WPARAM)column_no, (LPARAM)&column);
	}

	// ****************************************************************
	// リストビュー：コラムの幅を設定（リストビューのサイズに合わせて指定のコラムのサイズを調整する）
	// ================================================================

	void Cc_listview::set_column_width_just(int column_no, bool is_horizon_scroll_bar)
	{
		C_rect rect;
		int column_cnt, column_width, change_width;

		GetClientRect(m_h_wnd, &rect);		// リストビューの矩形を取得

		column_cnt = get_column_cnt();		// コラム数を取得

		// -1 の場合、最後のコラムの幅を設定
		if (column_no == -1)
			column_no = column_cnt - 1;

		// コラムの幅を取得
		column_width = 0;
		for (int i = 0; i < column_cnt; i++)	{
			if (i != column_no)				// ターゲットのコラムの幅は計算しない
				column_width += get_column_width(i);
		}

		change_width = rect.width() - column_width;
		if (change_width < 0)
			change_width = 0;

		set_column_width(column_no, change_width);

		if (is_horizon_scroll_bar == false)
			ShowScrollBar(m_h_wnd, SB_HORZ, FALSE);
	}

	void Cc_listview::set_column_width_just(int column_no)
	{
		set_column_width_just(column_no, true);		// true = 水平スクロールバーを消さない
	}

	// ****************************************************************
	// リストビュー：コラムの幅を取得
	// ================================================================
	int Cc_listview::get_column_width(int column_no)
	{
		return (int)send_message(LVM_GETCOLUMNWIDTH, (WPARAM)column_no, (LPARAM)0);
	}

	// ****************************************************************
	// リストビュー：アイテムの追加
	// ================================================================
	int	Cc_listview::add_item()
	{
		return insert_item(get_item_cnt());
	}

	int	Cc_listview::add_item(int column_no, CTSTR& str)
	{
		int item_no = add_item();
		set_cell_text(item_no, column_no, str);

		return item_no;
	}

	int	Cc_listview::add_item_visible(int column_no, CTSTR& str)
	{
		int item_no = add_item();
		set_cell_text_visible(item_no, column_no, str);

		return item_no;
	}

	// ****************************************************************
	// リストビュー：アイテムの挿入
	// ================================================================
	int	Cc_listview::insert_item(int item_no)
	{
		LV_ITEM item;
		item.mask     = 0;				// 項目を指定しない
		item.iItem    = item_no;		// 挿入位置
		item.iSubItem = 0;				// サブアイテム番号

		return (int)send_message(LVM_INSERTITEM, 0, (LPARAM)&item);
	}

	int	Cc_listview::insert_item(int item_no, int column_no, CTSTR& str)
	{
		item_no = insert_item(item_no);
		set_cell_text(item_no, column_no, str);

		return item_no;
	}

	int	Cc_listview::insert_item_visible(int item_no, int column_no, CTSTR& str)
	{
		item_no = insert_item(item_no);
		set_cell_text_visible(item_no, column_no, str);

		return item_no;
	}

	// ****************************************************************
	// リストビュー：アイテム数の縮小
	// ================================================================
	void Cc_listview::reduce_item(int item_cnt)
	{
		int item_no = item_cnt;
		while (send_message(LVM_DELETEITEM, item_no, 0));
	}

	// ****************************************************************
	// リストビュー：アイテムの削除
	// ================================================================
	void Cc_listview::delete_item(int item_no)
	{
		send_message(LVM_DELETEITEM, item_no, 0);
	}

	// ****************************************************************
	// リストビュー：先頭のアイテム番号を取得する
	// ================================================================
	int Cc_listview::get_item_top_no()
	{
		if (m_h_wnd == NULL)	{ return 0; }
		return (int)send_message(LVM_GETTOPINDEX, 0, 0);
	}

	// ****************************************************************
	// リストビュー：アイテムが先頭に来るように可視範囲を移動（item_no : -1 = 最後の項目が可視範囲になるようにする）
	// ================================================================
	void Cc_listview::set_item_visible_top(int item_no)
	{
		// -1 の場合は最後のアイテム
		if (item_no < 0)	
			item_no = get_item_cnt() - 1;

		// 可視範囲の先頭アイテムを取得
		int top_item_no = (int)send_message(LVM_GETTOPINDEX, 0, 0);
		if (item_no == top_item_no)
			return;

		C_rect rect;
		rect.left = LVIR_LABEL;
		send_message(LVM_GETITEMRECT, 0, (LPARAM)&rect);
		int item_height = rect.bottom - rect.top;

		send_message(LVM_SCROLL, 0, (item_height * (item_no - top_item_no)));
	}

	// ****************************************************************
	// リストビュー：アイテム数の設定
	// ================================================================
	void Cc_listview::set_item_cnt(int item_cnt)
	{
		int item_cnt_old = get_item_cnt();

		// アイテム数が少ないとき
		if (item_cnt_old < item_cnt)	{
			for (int i = 0; i < item_cnt - item_cnt_old; i++)	{
				add_item();
			}
		}
		// アイテム数が多いとき
		else if (item_cnt_old > item_cnt)	{
			reduce_item(item_cnt);
		}
	}

	// ****************************************************************
	// リストビュー：アイテム数の取得
	// ================================================================
	int Cc_listview::get_item_cnt()
	{
		return (int)send_message(LVM_GETITEMCOUNT, 0, 0);
	}

	// ****************************************************************
	// リストビュー：フォーカスをセット
	// ================================================================
	void Cc_listview::set_item_focus(int item_no, bool is_focus)
	{
		LV_ITEM item;
		item.mask = LVIF_STATE;
		item.state = is_focus ? LVIS_FOCUSED : 0;
		item.stateMask = LVIS_FOCUSED;
		send_message(LVM_SETITEMSTATE, item_no, (LPARAM)&item);
	}

	// ****************************************************************
	// リストビュー：フォーカスのあるアイテムを取得
	// ================================================================
	int Cc_listview::get_focused_item_no()
	{
		int item_cnt = get_item_cnt();
		for (int i = 0; i < item_cnt; i++)	{
			if (send_message(LVM_GETITEMSTATE, i, LVIS_FOCUSED) & LVIS_FOCUSED)
				return i;
		}

		return -1;
	}

	// ****************************************************************
	// リストビュー：選択
	// ================================================================
	void Cc_listview::set_item_select(int item_no, bool is_select)
	{
		LV_ITEM item;
		item.mask = LVIF_STATE;
		item.state = is_select ? LVIS_SELECTED : 0;
		item.stateMask = LVIS_SELECTED;
		send_message(LVM_SETITEMSTATE, item_no, (LPARAM)&item);
	}

	// ****************************************************************
	// リストビュー：すべて選択
	// ================================================================
	void Cc_listview::set_item_select_all(bool is_select)
	{
		int item_cnt = get_item_cnt();
		for (int i = 0; i < item_cnt; i++)	{
			LV_ITEM item;
			item.mask = LVIF_STATE;
			item.state = is_select ? LVIS_SELECTED : 0;
			item.stateMask = LVIS_SELECTED;
			send_message(LVM_SETITEMSTATE, i, (LPARAM)&item);
		}
	}

	// ****************************************************************
	// リストビュー：すべて選択
	// ================================================================
	void Cc_listview::set_item_selection_mark(int item_no)
	{
		send_message(LVM_SETSELECTIONMARK, 0, item_no);
	}

	// ****************************************************************
	// リストビュー：選択されているかどうか判定
	// ================================================================
	bool Cc_listview::get_item_select(int item_no)
	{
		return (send_message(LVM_GETITEMSTATE, item_no, LVIS_SELECTED) & LVIS_SELECTED) > 0;
	}

	// ****************************************************************
	// リストビュー：選択されているアイテム番号を取得
	// ================================================================
	int Cc_listview::get_selected_item_no()
	{
		int item_cnt = get_item_cnt();
		int select_cnt = 0;

		for (int i = 0; i < item_cnt; i++)	{
			if (send_message(LVM_GETITEMSTATE, i, LVIS_SELECTED) & LVIS_SELECTED)
				return i;
		}

		return -1;
	}

	// ****************************************************************
	// リストビュー：選択されているアイテム数を取得
	// ================================================================
	int Cc_listview::get_selected_item_cnt()
	{
		int item_cnt = get_item_cnt();
		int select_cnt = 0;

		for (int i = 0; i < item_cnt; i++)	{
			if (send_message(LVM_GETITEMSTATE, i, LVIS_SELECTED) & LVIS_SELECTED)
				select_cnt++;
		}

		return select_cnt;
	}

	// ****************************************************************
	// リストビュー：イメージリストの設定
	// ================================================================
	void Cc_listview::set_image_list(HIMAGELIST h_image_list)
	{
		send_message(LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)h_image_list);
	}

	// ****************************************************************
	// リストビュー：イメージの設定
	// ================================================================
	void Cc_listview::set_item_image(int item_no, int image_no)
	{
		LV_ITEM	item;
		item.iItem      = item_no;
		item.iSubItem   = 0;
		item.mask       = LVIF_IMAGE;
		item.iImage     = image_no;
		send_message(LVM_SETITEM, 0, (LPARAM)&item);
		// イメージはなぜか LVM_SETITEMSTATE が使えないので LVM_SETITEM で。
	}

	// ****************************************************************
	// リストビュー：チェック状態を設定
	// ================================================================
	void Cc_listview::set_item_check(int item_no, bool check)
	{
		// イメージマスクの取得
		int state = (int)send_message(LVM_GETITEMSTATE, item_no, LVIS_STATEIMAGEMASK);

		// チェックに関する部分を 0 に初期化
		state &= 0xCFFF;			// (b)1100111111111111

		// チェックに関する部分を設定
		state |= check ? 0x2000		// (b)0010000000000000
					   : 0x1000;	// (b)0001000000000000

		LV_ITEM	item;
		ZeroMemory(&item, sizeof(LV_ITEM));
		item.mask       = LVIF_STATE;
		item.state      = state;
		item.stateMask  = LVIS_STATEIMAGEMASK;
		send_message(LVM_SETITEMSTATE, item_no, (LPARAM)&item);
	}

	// ****************************************************************
	// リストビュー：チェック状態を取得
	// ================================================================
	bool Cc_listview::get_item_check(int item_no)
	{
		// イメージマスクの取得
		int state = (int)send_message(LVM_GETITEMSTATE, item_no, LVIS_STATEIMAGEMASK);

		return (state >> 12) - 1 > 0;
		// ↓以下のようにしたいんだけど、念のため commctrl.h と同じ書式にしてみてます。
		// return state & 0x2000;	// (b)0010000000000000
	}

	// ****************************************************************
	// リストビュー：チェックされているアイテム数を取得
	// ================================================================
	int Cc_listview::get_checked_item_cnt()
	{
		int item_cnt = get_item_cnt();
		int check_cnt = 0;
		for (int i = 0; i < item_cnt; i++)	{
			if (get_item_check(i))
				check_cnt ++;
		}

		return check_cnt;
	}

	// ****************************************************************
	// リストビュー：可視範囲に入るようにスクロール
	// ================================================================
	void Cc_listview::set_item_visible(int item_no)
	{
		send_message(LVM_ENSUREVISIBLE, item_no, TRUE);
	}

	// ****************************************************************
	// リストビュー：テキストを設定
	// ================================================================
	void Cc_listview::set_cell_text(int item_no, int column_no, CTSTR& str)
	{
		if (get_cell_text(item_no, column_no) == str)	{
			return;
		}
		LV_ITEM	item;
		item.mask     = LVIF_TEXT;
		item.iSubItem = column_no;
		item.pszText  = (LPTSTR)str.c_str();
		send_message(LVM_SETITEMTEXT, item_no, (LPARAM)&item);
	}

	// ****************************************************************
	// リストビュー：テキストを設定して可視範囲にスクロール
	// ================================================================
	void Cc_listview::set_cell_text_visible(int item_no, int column_no, CTSTR& str)
	{
		set_cell_text(item_no, column_no, str);
		set_item_visible(item_no);
	}

	// ****************************************************************
	// リストビュー：テキストの取得
	// ================================================================
	TSTR Cc_listview::get_cell_text(int item_no, int column_no)
	{
		TCHAR t_str[MAX_PATH];

		LV_ITEM item;
		item.mask = LVIF_TEXT | LVIF_PARAM;
		item.iSubItem   = column_no;
		item.pszText    = t_str;
		item.cchTextMax = MAX_PATH;
		send_message(LVM_GETITEMTEXT, item_no, (LPARAM)&item);

		return t_str;
	}

	// ****************************************************************
	// リストビュー：セルの矩形を取得
	// ================================================================
	C_rect Cc_listview::get_cell_rect(int item_no, int column_no)
	{
		C_rect rect;
		rect.top = column_no;
		rect.left = 0;

		send_message(LVM_GETSUBITEMRECT, item_no, (LPARAM)&rect);

		return rect;
	}

	// ****************************************************************
	// リストビュー：座標（リストビューのクライアント空間）からセル番号を取得
	// ================================================================
	bool Cc_listview::get_cell_by_point(int x, int y, int* item_no, int* column_no)
	{
		*item_no = -1;
		*column_no = -1;

		// コラム０番は取得できない！
		for (int i = 0; i < get_item_cnt(); i++)	{
			for (int c = 1; c < get_column_cnt(); c++)	{
				if (get_cell_rect(i, c).is_in(x, y))	{
					*item_no = i;
					*column_no = c;
					return true;
				}
			}
		}

		// コラム０番はＹ軸のみで判定する
		C_rect rect;
		for (int i = 0; i < get_item_cnt(); i++)	{
			rect = get_cell_rect(i, 0);
			if (rect.top <= y && y < rect.bottom)	{
				*item_no = i;
				*column_no = 0;
				return true;
			}
		}

		return false;
	}

	// ****************************************************************
	// リストビュー：テキストを編集（エディット：（文字列を渡すバージョン））
	// ================================================================
	void Cc_listview::edit_item_text_edit_with_str(int item_no, int column_no, TSTR str)
	{
		editing = true;
		editing_item_no = item_no;
		editing_column_no = column_no;
		editing_num = false;

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		// アイテムの矩形を取得
		C_rect item_rect = get_cell_rect(item_no, column_no);

		// ダミーのエディットを載せる
		dummy_edit.create(m_window_name + _T("_DUMMY_EDIT"), _T(""), item_rect.left, item_rect.top, item_rect.width(), item_rect.height(), false, WS_CHILD, 0, false, false, m_h_wnd);
		SendMessage(dummy_edit.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
		dummy_edit.listview = this;			// リストビューへのポインタ
		dummy_edit.spin = &dummy_spin;		// 連動スピンへのポインタ
		dummy_edit.set_text(str);			// 現在のテキストを初期値として設定
		dummy_edit.set_sel_all();			// テキストを選択状態にする
		dummy_edit.set_show(true);			// 表示
		dummy_edit.set_focus();				// フォーカスを当てる

		// 初期値の記憶
		nmlv_update.old_text = str;
		nmlv_change.old_text = str;
	}

	// ****************************************************************
	// リストビュー：テキストを編集（エディット：現在のセルの文字列を変更する）
	// ================================================================
	void Cc_listview::edit_item_text_edit(int item_no, int column_no)
	{
		edit_item_text_edit_with_str(item_no, column_no, get_cell_text(item_no, column_no));
	}

	// ****************************************************************
	// リストビュー：テキストを編集（数値エディット：現在のセルの数値を変更する）
	// ================================================================
	void Cc_listview::edit_item_text_edit_for_num(int item_no, int column_no, int min, int max, int def)
	{
		TSTR str = get_cell_text(item_no, column_no);
		int num = str_to_int(str, def);
		num = limit(min, num, max);
		str = tostr(num);
		edit_item_text_edit_with_str(item_no, column_no, str);
		editing_num = true;
		editing_num_min = min;
		editing_num_max = max;
		editing_num_def = def;

		// アイテムの矩形を取得
		C_rect item_rect = get_cell_rect(item_no, column_no);

		// ダミーのスピンを載せる
		dummy_spin.create(m_window_name + _T("_DUMMY_SPIN"), _T(""), item_rect.right, item_rect.top, 16, item_rect.height(), false, WS_CHILD | UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_NOTHOUSANDS, 0, false, false, m_h_wnd);
		dummy_spin.listview = this;								// リストビューへのポインタ
		dummy_spin.edit = &dummy_edit;							// 連動エディットへのポインタ
		dummy_spin.set_show(true);								// 表示
		dummy_spin.set_edit(dummy_edit.get_handle());			// 連動するエディットのハンドルを指定
		dummy_spin.set_edit_range(min, max);					// 値の範囲を指定

		// 数値化
		int val = str_to_int(get_cell_text(item_no, column_no), def);
		dummy_edit.set_text(tostr(val));	// 現在のテキストを初期値として設定
	}

	// ****************************************************************
	// リストビュー：テキストを編集（コンボボックス）
	// ================================================================
	void Cc_listview::edit_item_text_combobox(int item_no, int column_no, TCHAR** text_list, int text_cnt, int sel_no)
	{
		editing = true;
		editing_item_no = item_no;
		editing_column_no = column_no;
		editing_num = false;

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		// アイテムの矩形を取得
		C_rect item_rect = get_cell_rect(item_no, column_no);

		// ダミーのコンボボックスを載せる
		dummy_combo.create(m_window_name + _T("_DUMMY_COMBOBOX"), _T(""), item_rect.left, item_rect.top, item_rect.width(), item_rect.height() * 8, false, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 0, false, false, m_h_wnd);
		SendMessage(dummy_combo.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
		dummy_combo.listview = this;	// リストビューへのポインタ

		for (int i = 0; i < text_cnt; i++)	{
			dummy_combo.insert_item_to_bottom(text_list[i], false);
		}

		dummy_combo.set_sel_no(sel_no);	// 選択
		dummy_combo.set_show(true);		// 表示
		dummy_combo.set_focus();		// フォーカスを当てる

		// 初期値の記憶
		nmlv_update.old_sel_no = sel_no;
		nmlv_change.old_sel_no = sel_no;
	}

	// ****************************************************************
	// リストビュー：テキストの編集を終了する（エディット）
	// ================================================================
	void Cc_listview::end_edit_item_edit(bool change, int end_reason)
	{
		editing = false;

		// 変更する場合のみ
		if (change)	{

			// 変更しますよメッセージ
			nmlv_update.hdr.hwndFrom = m_h_wnd;
			nmlv_update.hdr.idFrom = get_id();
			nmlv_update.hdr.code = LVN_UPDATE_TEXT;
			nmlv_update.item_no = editing_item_no;
			nmlv_update.column_no = editing_column_no;
			if (editing_num)	{
				TSTR str = dummy_edit.get_text();
				int num = str_to_int(str, editing_num_def);
				num = limit(editing_num_min, num, editing_num_max);
				nmlv_update.new_text = tostr(num);
			}
			else	{
				nmlv_update.new_text = dummy_edit.get_text();
			}
			SendMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_update);

			// テキストを変更
			set_cell_text(editing_item_no, editing_column_no, nmlv_update.new_text);
		
			// 変更したよメッセージ
			nmlv_change.hdr.hwndFrom = m_h_wnd;
			nmlv_change.hdr.idFrom = get_id();
			nmlv_change.hdr.code = LVN_CHANGE_TEXT;
			nmlv_change.item_no = editing_item_no;
			nmlv_change.column_no = editing_column_no;
			nmlv_change.new_text = nmlv_update.new_text;
			PostMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_change);
		}

		// 編集完了メッセージ
		nmlv_edit_end.hdr.hwndFrom = m_h_wnd;
		nmlv_edit_end.hdr.idFrom = get_id();
		nmlv_edit_end.hdr.code = LVN_EDIT_END_TEXT;
		nmlv_edit_end.item_no = editing_item_no;
		nmlv_edit_end.column_no = editing_column_no;
		nmlv_edit_end.end_reason = end_reason;
		PostMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_edit_end);

		// ダミーのコントロールを破壊
		dummy_edit.destroy();
		dummy_spin.destroy();
	}

	// ****************************************************************
	// リストビュー：テキストの編集を終了する（コンボボックス）
	// ================================================================
	void Cc_listview::end_edit_item_combobox(bool change)
	{
		editing = false;

		// 変更する場合のみ
		if (change)	{

			// 変更しますよメッセージ
			nmlv_update.hdr.hwndFrom = m_h_wnd;
			nmlv_update.hdr.idFrom = get_id();
			nmlv_update.hdr.code = LVN_UPDATE_TEXT;
			nmlv_update.item_no = editing_item_no;
			nmlv_update.column_no = editing_column_no;
			nmlv_update.new_sel_no = dummy_combo.get_sel_no();
			SendMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_update);

			// テキストを変更
			set_cell_text(editing_item_no, editing_column_no, dummy_combo.get_item_text(nmlv_update.new_sel_no));
		
			// 変更したよメッセージ
			nmlv_change.hdr.hwndFrom = m_h_wnd;
			nmlv_change.hdr.idFrom = get_id();
			nmlv_change.hdr.code = LVN_CHANGE_TEXT;
			nmlv_change.item_no = editing_item_no;
			nmlv_change.column_no = editing_column_no;
			nmlv_change.new_sel_no = nmlv_update.new_sel_no;
			PostMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_change);
		}

		// ダミーのコントロールを破壊
		dummy_combo.destroy();
	}

	// ****************************************************************
	// リストビュー：テキスト編集用のエディット
	// ================================================================
	LRESULT Cc_listview::Citem_edit::window_proc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)	{
			case WM_KEYDOWN:
				if (wp == VK_RETURN)	{
					listview->end_edit_item_edit(true, LVN_EDIT_END_REASON_RETURN);
				}
				if (wp == VK_UP)	{
					listview->end_edit_item_edit(true, LVN_EDIT_END_REASON_UP);
				}
				if (wp == VK_DOWN)	{
					listview->end_edit_item_edit(true, LVN_EDIT_END_REASON_DOWN);
				}
				if (wp == VK_ESCAPE)	{
					listview->end_edit_item_edit(false, LVN_EDIT_END_REASON_ESCAPE);
				}
				break;
			case WM_KILLFOCUS:
				// フォーカスが移動した場合は編集終了
				// ただし連動しているスピンに移った場合は除く
				if (listview->editing && !(spin && ::GetFocus() == spin->get_handle()))	{
					listview->end_edit_item_edit(true, LVN_EDIT_END_REASON_NONE);
				}
				break;
			case WM_MOUSEWHEEL:
				return TRUE;		// 親に処理を回さない
		}

		return Cc_edit::window_proc(msg, wp, lp);
	}

	// ****************************************************************
	// リストビュー：テキスト編集用のエディットのスピン
	// ================================================================
	LRESULT Cc_listview::Citem_spin::window_proc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)	{
			case WM_KILLFOCUS:
				// フォーカスが移動した場合は編集終了
				// ただし連動しているエディットに移った場合は除く
				if (listview->editing && !(edit && ::GetFocus() == edit->get_handle()))	{
					listview->end_edit_item_edit(true, LVN_EDIT_END_REASON_NONE);
				}
		}

		return Cc_spin::window_proc(msg, wp, lp);
	}

	// ****************************************************************
	// リストビュー：テキスト編集用のコンボボックス
	// ================================================================
	LRESULT Cc_listview::Citem_combo::window_proc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)	{
			case WM_KILLFOCUS:
				// フォーカスが移動した場合は編集終了
				if (listview->editing)	{
					listview->end_edit_item_combobox(false);
				}
		}

		return Cc_combobox::window_proc(msg, wp, lp);
	}

	// ****************************************************************
	// リストビュー：テキスト編集用のエディットのプロシージャ設定
	// ================================================================
	void Cc_listview::set_edit_proc(WNDPROC proc)
	{
		dummy_edit.set_proc(proc);
	}

	// ****************************************************************
	// リストビュー：テキスト編集用のコンボボックスのプロシージャ設定
	// ================================================================
	void Cc_listview::set_combobox_proc(WNDPROC proc)
	{
		dummy_combo.set_proc(proc);
	}

	// ****************************************************************
	// ポップアップメニュー
	// ================================================================
	void Cc_listview::create_popup_menu()
	{
		C_popup_menu menu;

		menu.create();
		menu.add_item(1, _T("csv で保存"), true, false);

		// メニューを開き選択開始
		C_point mp = get_mouse_pos();	// ここだけは独自でマウス座標を取り出す
		int syscom_id = menu.select(get_handle(), mp.x, mp.y);

		if (syscom_id == 1)
		{
			// csv で保存する！！
			C_select_file_dialog sfd;
			sfd.add_filter(_T("CSV ファイル"), _T("*.csv"));
			TSTR file_path = sfd.select_save_file(_T("CSV 形式で保存"), _T("新しい CSV ファイル.csv"));
			if (!file_path.empty())
			{
				ARRAY<TSTR> csv_data;

				int column_cnt = this->get_column_cnt();
				int row_cnt = this->get_item_cnt();
				for (int j = 0; j < row_cnt; j++)
				{
					TSTR line_data;
					for (int i = 0; i < column_cnt; i++)
					{
						TSTR text;
						text = this->get_cell_text(j, i);
						text = str_replace_ret(text, _T("\""), _T("\"\""));
						if (text.find(_T('"')) != TSTR::npos || text.find(_T(',')) != TSTR::npos)
						{
							text = _T("\"") + text + _T("\"");
						}
						if (i > 0)
						{
							text = _T(",") + text;
						}

						line_data += text;
					}

					csv_data.push_back(line_data);
				}

				C_file::write_full_text_in_MB(file_path, csv_data);
			}
		}
	}

	// ****************************************************************
	// ini 保存
	// ================================================================
	void Cc_listview::save_ini()
	{
		int column_cnt = get_column_cnt();

		// コラム幅の保存
		for (int c = 0; c < column_cnt; c++)	{
			int column_width = get_column_width(c);
			save_ini_int(_T("header_") + name_list[c] + _T("_width"), column_width);
		}

		// コラム順序の保存
		int order[256];
		HWND h_header = (HWND)send_message(LVM_GETHEADER, 0, 0);
		SendMessage(h_header, HDM_GETORDERARRAY, column_cnt, (LPARAM)order);
		for (int c = 0; c < column_cnt; c++)
			save_ini_int(_T("header_") + name_list[c] + _T("_order"), order[c]);
	}

	// ****************************************************************
	// ini 読込
	// ================================================================
	void Cc_listview::load_ini()
	{
		int column_cnt = get_column_cnt();

		// コラム幅の読込
		for (int c = 0; c < column_cnt; c++)	{
			int column_width = load_ini_int(_T("header_") + name_list[c] + _T("_width"), get_column_width(c));
			set_column_width(c, column_width);
		}

		// コラム順序の読込
		int order[256];
		for (int c = 0; c < column_cnt; c++)
			order[c] = load_ini_int(_T("header_") + name_list[c] + _T("_order"), c);

		HWND h_header = (HWND)send_message(LVM_GETHEADER, 0, 0);
		SendMessage(h_header, HDM_SETORDERARRAY, column_cnt, (LPARAM)order);
	}

	void Cc_listview::load_ini_column_width()
	{
		int column_cnt = get_column_cnt();

		// コラム幅の読込
		for (int c = 0; c < column_cnt; c++)	{
			int column_width = load_ini_int(_T("header_") + name_list[c] + _T("_width"), get_column_width(c));
			set_column_width(c, column_width);
		}
	}
}
