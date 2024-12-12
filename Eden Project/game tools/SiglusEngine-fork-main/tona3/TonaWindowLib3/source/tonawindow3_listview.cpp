#include	"tonawindow3_pch.h"
#include	"tonawindow3_listview.h"
#include	"tonawindow3_menu.h"

namespace NT3
{
	// ****************************************************************
	// ���X�g�r���[�F�E�B���h�E�v���V�[�W��
	// ================================================================
	LRESULT Cc_listview::window_proc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
			case WM_COMMAND:

				if (false);
				// �_�~�[�R���{�{�b�N�X����̒ʒm���b�Z�[�W
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
	// ���X�g�r���[�F�j�󂳂ꂽ
	// ================================================================
	bool Cc_listview::on_destroy()
	{
		name_list.clear();

		return true;
	}

	// ****************************************************************
	// ���X�g�r���[�F�N���A
	// ================================================================
	void Cc_listview::clear()
	{
		send_message(LVM_DELETEALLITEMS, 0, 0);
	}

	// ****************************************************************
	// ���X�g�r���[�F�ȈՐݒ�
	//		is_full_row_select:		�s�S�̂�I��
	//		is_grid_lines:			�s�ɃO���b�h������
	//		is_check_boxes:			�`�F�b�N�{�b�N�X������
	//		is_header_drag_drop:	�w�b�_���h���b�O���h���b�v�ňړ��ł���悤�ɂ���
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
	// ���X�g�r���[�F�R�������̎擾
	// ================================================================
	int Cc_listview::get_column_cnt()
	{
		// ���X�g�r���[�̃w�b�_���擾
		HWND h_header = (HWND)send_message(LVM_GETHEADER, 0, 0);

		return (int)SendMessage(h_header, HDM_GETITEMCOUNT, 0, 0L);
	}

	// ****************************************************************
	// ���X�g�r���[�F�R�����̒ǉ�
	// ================================================================
	void Cc_listview::add_column(CTSTR& name, CTSTR& caption, int width, int align)
	{
		// �����ɃR������}��
		int column_no = get_column_cnt();
		insert_column(column_no, name, caption, width, align);
	}

	// ****************************************************************
	// ���X�g�r���[�F�R�����̑}��
	// ================================================================
	void Cc_listview::insert_column(int column_no, CTSTR& name, CTSTR& caption, int width, int align)
	{
		LV_COLUMN column;
		column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		column.fmt = align;
		column.cx = width;
		column.pszText = (LPTSTR)caption.c_str();
		send_message(LVM_INSERTCOLUMN, (WPARAM)column_no, (LPARAM)&column);

		// ���O���X�g�ɂ��ǉ�
		name_list.insert(name_list.begin() + column_no, name);
	}

	// ****************************************************************
	// ���X�g�r���[�F�R�������폜
	// ================================================================
	void Cc_listview::delete_column(int column_no)
	{
		send_message(LVM_DELETEALLITEMS, column_no, 0);

		// ���O���X�g������폜
		name_list.erase(name_list.begin() + column_no);
	}

	// ****************************************************************
	// ���X�g�r���[�F�S�ẴR�������폜
	// ================================================================
	void Cc_listview::delete_all_column()
	{
		while (1)	{
			if (send_message(LVM_DELETECOLUMN, 0, 0) == FALSE)
				break;
		}

		// ���O���X�g������폜
		name_list.clear();
	}

	// ****************************************************************
	// ���X�g�r���[�F�R�����̕���ݒ�
	// ================================================================
	void Cc_listview::set_column_width(int column_no, int width)
	{
		LV_COLUMN column;
		column.mask = LVCF_WIDTH;
		column.cx = width;
		send_message(LVM_SETCOLUMN, (WPARAM)column_no, (LPARAM)&column);
	}

	// ****************************************************************
	// ���X�g�r���[�F�R�����̕���ݒ�i���X�g�r���[�̃T�C�Y�ɍ��킹�Ďw��̃R�����̃T�C�Y�𒲐�����j
	// ================================================================

	void Cc_listview::set_column_width_just(int column_no, bool is_horizon_scroll_bar)
	{
		C_rect rect;
		int column_cnt, column_width, change_width;

		GetClientRect(m_h_wnd, &rect);		// ���X�g�r���[�̋�`���擾

		column_cnt = get_column_cnt();		// �R���������擾

		// -1 �̏ꍇ�A�Ō�̃R�����̕���ݒ�
		if (column_no == -1)
			column_no = column_cnt - 1;

		// �R�����̕����擾
		column_width = 0;
		for (int i = 0; i < column_cnt; i++)	{
			if (i != column_no)				// �^�[�Q�b�g�̃R�����̕��͌v�Z���Ȃ�
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
		set_column_width_just(column_no, true);		// true = �����X�N���[���o�[�������Ȃ�
	}

	// ****************************************************************
	// ���X�g�r���[�F�R�����̕����擾
	// ================================================================
	int Cc_listview::get_column_width(int column_no)
	{
		return (int)send_message(LVM_GETCOLUMNWIDTH, (WPARAM)column_no, (LPARAM)0);
	}

	// ****************************************************************
	// ���X�g�r���[�F�A�C�e���̒ǉ�
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
	// ���X�g�r���[�F�A�C�e���̑}��
	// ================================================================
	int	Cc_listview::insert_item(int item_no)
	{
		LV_ITEM item;
		item.mask     = 0;				// ���ڂ��w�肵�Ȃ�
		item.iItem    = item_no;		// �}���ʒu
		item.iSubItem = 0;				// �T�u�A�C�e���ԍ�

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
	// ���X�g�r���[�F�A�C�e�����̏k��
	// ================================================================
	void Cc_listview::reduce_item(int item_cnt)
	{
		int item_no = item_cnt;
		while (send_message(LVM_DELETEITEM, item_no, 0));
	}

	// ****************************************************************
	// ���X�g�r���[�F�A�C�e���̍폜
	// ================================================================
	void Cc_listview::delete_item(int item_no)
	{
		send_message(LVM_DELETEITEM, item_no, 0);
	}

	// ****************************************************************
	// ���X�g�r���[�F�擪�̃A�C�e���ԍ����擾����
	// ================================================================
	int Cc_listview::get_item_top_no()
	{
		if (m_h_wnd == NULL)	{ return 0; }
		return (int)send_message(LVM_GETTOPINDEX, 0, 0);
	}

	// ****************************************************************
	// ���X�g�r���[�F�A�C�e�����擪�ɗ���悤�ɉ��͈͂��ړ��iitem_no : -1 = �Ō�̍��ڂ����͈͂ɂȂ�悤�ɂ���j
	// ================================================================
	void Cc_listview::set_item_visible_top(int item_no)
	{
		// -1 �̏ꍇ�͍Ō�̃A�C�e��
		if (item_no < 0)	
			item_no = get_item_cnt() - 1;

		// ���͈͂̐擪�A�C�e�����擾
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
	// ���X�g�r���[�F�A�C�e�����̐ݒ�
	// ================================================================
	void Cc_listview::set_item_cnt(int item_cnt)
	{
		int item_cnt_old = get_item_cnt();

		// �A�C�e���������Ȃ��Ƃ�
		if (item_cnt_old < item_cnt)	{
			for (int i = 0; i < item_cnt - item_cnt_old; i++)	{
				add_item();
			}
		}
		// �A�C�e�����������Ƃ�
		else if (item_cnt_old > item_cnt)	{
			reduce_item(item_cnt);
		}
	}

	// ****************************************************************
	// ���X�g�r���[�F�A�C�e�����̎擾
	// ================================================================
	int Cc_listview::get_item_cnt()
	{
		return (int)send_message(LVM_GETITEMCOUNT, 0, 0);
	}

	// ****************************************************************
	// ���X�g�r���[�F�t�H�[�J�X���Z�b�g
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
	// ���X�g�r���[�F�t�H�[�J�X�̂���A�C�e�����擾
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
	// ���X�g�r���[�F�I��
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
	// ���X�g�r���[�F���ׂđI��
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
	// ���X�g�r���[�F���ׂđI��
	// ================================================================
	void Cc_listview::set_item_selection_mark(int item_no)
	{
		send_message(LVM_SETSELECTIONMARK, 0, item_no);
	}

	// ****************************************************************
	// ���X�g�r���[�F�I������Ă��邩�ǂ�������
	// ================================================================
	bool Cc_listview::get_item_select(int item_no)
	{
		return (send_message(LVM_GETITEMSTATE, item_no, LVIS_SELECTED) & LVIS_SELECTED) > 0;
	}

	// ****************************************************************
	// ���X�g�r���[�F�I������Ă���A�C�e���ԍ����擾
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
	// ���X�g�r���[�F�I������Ă���A�C�e�������擾
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
	// ���X�g�r���[�F�C���[�W���X�g�̐ݒ�
	// ================================================================
	void Cc_listview::set_image_list(HIMAGELIST h_image_list)
	{
		send_message(LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)h_image_list);
	}

	// ****************************************************************
	// ���X�g�r���[�F�C���[�W�̐ݒ�
	// ================================================================
	void Cc_listview::set_item_image(int item_no, int image_no)
	{
		LV_ITEM	item;
		item.iItem      = item_no;
		item.iSubItem   = 0;
		item.mask       = LVIF_IMAGE;
		item.iImage     = image_no;
		send_message(LVM_SETITEM, 0, (LPARAM)&item);
		// �C���[�W�͂Ȃ��� LVM_SETITEMSTATE ���g���Ȃ��̂� LVM_SETITEM �ŁB
	}

	// ****************************************************************
	// ���X�g�r���[�F�`�F�b�N��Ԃ�ݒ�
	// ================================================================
	void Cc_listview::set_item_check(int item_no, bool check)
	{
		// �C���[�W�}�X�N�̎擾
		int state = (int)send_message(LVM_GETITEMSTATE, item_no, LVIS_STATEIMAGEMASK);

		// �`�F�b�N�Ɋւ��镔���� 0 �ɏ�����
		state &= 0xCFFF;			// (b)1100111111111111

		// �`�F�b�N�Ɋւ��镔����ݒ�
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
	// ���X�g�r���[�F�`�F�b�N��Ԃ��擾
	// ================================================================
	bool Cc_listview::get_item_check(int item_no)
	{
		// �C���[�W�}�X�N�̎擾
		int state = (int)send_message(LVM_GETITEMSTATE, item_no, LVIS_STATEIMAGEMASK);

		return (state >> 12) - 1 > 0;
		// ���ȉ��̂悤�ɂ������񂾂��ǁA�O�̂��� commctrl.h �Ɠ��������ɂ��Ă݂Ă܂��B
		// return state & 0x2000;	// (b)0010000000000000
	}

	// ****************************************************************
	// ���X�g�r���[�F�`�F�b�N����Ă���A�C�e�������擾
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
	// ���X�g�r���[�F���͈͂ɓ���悤�ɃX�N���[��
	// ================================================================
	void Cc_listview::set_item_visible(int item_no)
	{
		send_message(LVM_ENSUREVISIBLE, item_no, TRUE);
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g��ݒ�
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
	// ���X�g�r���[�F�e�L�X�g��ݒ肵�ĉ��͈͂ɃX�N���[��
	// ================================================================
	void Cc_listview::set_cell_text_visible(int item_no, int column_no, CTSTR& str)
	{
		set_cell_text(item_no, column_no, str);
		set_item_visible(item_no);
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�̎擾
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
	// ���X�g�r���[�F�Z���̋�`���擾
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
	// ���X�g�r���[�F���W�i���X�g�r���[�̃N���C�A���g��ԁj����Z���ԍ����擾
	// ================================================================
	bool Cc_listview::get_cell_by_point(int x, int y, int* item_no, int* column_no)
	{
		*item_no = -1;
		*column_no = -1;

		// �R�����O�Ԃ͎擾�ł��Ȃ��I
		for (int i = 0; i < get_item_cnt(); i++)	{
			for (int c = 1; c < get_column_cnt(); c++)	{
				if (get_cell_rect(i, c).is_in(x, y))	{
					*item_no = i;
					*column_no = c;
					return true;
				}
			}
		}

		// �R�����O�Ԃ͂x���݂̂Ŕ��肷��
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
	// ���X�g�r���[�F�e�L�X�g��ҏW�i�G�f�B�b�g�F�i�������n���o�[�W�����j�j
	// ================================================================
	void Cc_listview::edit_item_text_edit_with_str(int item_no, int column_no, TSTR str)
	{
		editing = true;
		editing_item_no = item_no;
		editing_column_no = column_no;
		editing_num = false;

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		// �A�C�e���̋�`���擾
		C_rect item_rect = get_cell_rect(item_no, column_no);

		// �_�~�[�̃G�f�B�b�g���ڂ���
		dummy_edit.create(m_window_name + _T("_DUMMY_EDIT"), _T(""), item_rect.left, item_rect.top, item_rect.width(), item_rect.height(), false, WS_CHILD, 0, false, false, m_h_wnd);
		SendMessage(dummy_edit.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
		dummy_edit.listview = this;			// ���X�g�r���[�ւ̃|�C���^
		dummy_edit.spin = &dummy_spin;		// �A���X�s���ւ̃|�C���^
		dummy_edit.set_text(str);			// ���݂̃e�L�X�g�������l�Ƃ��Đݒ�
		dummy_edit.set_sel_all();			// �e�L�X�g��I����Ԃɂ���
		dummy_edit.set_show(true);			// �\��
		dummy_edit.set_focus();				// �t�H�[�J�X�𓖂Ă�

		// �����l�̋L��
		nmlv_update.old_text = str;
		nmlv_change.old_text = str;
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g��ҏW�i�G�f�B�b�g�F���݂̃Z���̕������ύX����j
	// ================================================================
	void Cc_listview::edit_item_text_edit(int item_no, int column_no)
	{
		edit_item_text_edit_with_str(item_no, column_no, get_cell_text(item_no, column_no));
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g��ҏW�i���l�G�f�B�b�g�F���݂̃Z���̐��l��ύX����j
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

		// �A�C�e���̋�`���擾
		C_rect item_rect = get_cell_rect(item_no, column_no);

		// �_�~�[�̃X�s�����ڂ���
		dummy_spin.create(m_window_name + _T("_DUMMY_SPIN"), _T(""), item_rect.right, item_rect.top, 16, item_rect.height(), false, WS_CHILD | UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_NOTHOUSANDS, 0, false, false, m_h_wnd);
		dummy_spin.listview = this;								// ���X�g�r���[�ւ̃|�C���^
		dummy_spin.edit = &dummy_edit;							// �A���G�f�B�b�g�ւ̃|�C���^
		dummy_spin.set_show(true);								// �\��
		dummy_spin.set_edit(dummy_edit.get_handle());			// �A������G�f�B�b�g�̃n���h�����w��
		dummy_spin.set_edit_range(min, max);					// �l�͈̔͂��w��

		// ���l��
		int val = str_to_int(get_cell_text(item_no, column_no), def);
		dummy_edit.set_text(tostr(val));	// ���݂̃e�L�X�g�������l�Ƃ��Đݒ�
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g��ҏW�i�R���{�{�b�N�X�j
	// ================================================================
	void Cc_listview::edit_item_text_combobox(int item_no, int column_no, TCHAR** text_list, int text_cnt, int sel_no)
	{
		editing = true;
		editing_item_no = item_no;
		editing_column_no = column_no;
		editing_num = false;

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		// �A�C�e���̋�`���擾
		C_rect item_rect = get_cell_rect(item_no, column_no);

		// �_�~�[�̃R���{�{�b�N�X���ڂ���
		dummy_combo.create(m_window_name + _T("_DUMMY_COMBOBOX"), _T(""), item_rect.left, item_rect.top, item_rect.width(), item_rect.height() * 8, false, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 0, false, false, m_h_wnd);
		SendMessage(dummy_combo.get_handle(), WM_SETFONT, (WPARAM)font, (LPARAM)MAKELPARAM(TRUE, 0));
		dummy_combo.listview = this;	// ���X�g�r���[�ւ̃|�C���^

		for (int i = 0; i < text_cnt; i++)	{
			dummy_combo.insert_item_to_bottom(text_list[i], false);
		}

		dummy_combo.set_sel_no(sel_no);	// �I��
		dummy_combo.set_show(true);		// �\��
		dummy_combo.set_focus();		// �t�H�[�J�X�𓖂Ă�

		// �����l�̋L��
		nmlv_update.old_sel_no = sel_no;
		nmlv_change.old_sel_no = sel_no;
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�̕ҏW���I������i�G�f�B�b�g�j
	// ================================================================
	void Cc_listview::end_edit_item_edit(bool change, int end_reason)
	{
		editing = false;

		// �ύX����ꍇ�̂�
		if (change)	{

			// �ύX���܂��惁�b�Z�[�W
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

			// �e�L�X�g��ύX
			set_cell_text(editing_item_no, editing_column_no, nmlv_update.new_text);
		
			// �ύX�����惁�b�Z�[�W
			nmlv_change.hdr.hwndFrom = m_h_wnd;
			nmlv_change.hdr.idFrom = get_id();
			nmlv_change.hdr.code = LVN_CHANGE_TEXT;
			nmlv_change.item_no = editing_item_no;
			nmlv_change.column_no = editing_column_no;
			nmlv_change.new_text = nmlv_update.new_text;
			PostMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_change);
		}

		// �ҏW�������b�Z�[�W
		nmlv_edit_end.hdr.hwndFrom = m_h_wnd;
		nmlv_edit_end.hdr.idFrom = get_id();
		nmlv_edit_end.hdr.code = LVN_EDIT_END_TEXT;
		nmlv_edit_end.item_no = editing_item_no;
		nmlv_edit_end.column_no = editing_column_no;
		nmlv_edit_end.end_reason = end_reason;
		PostMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_edit_end);

		// �_�~�[�̃R���g���[����j��
		dummy_edit.destroy();
		dummy_spin.destroy();
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�̕ҏW���I������i�R���{�{�b�N�X�j
	// ================================================================
	void Cc_listview::end_edit_item_combobox(bool change)
	{
		editing = false;

		// �ύX����ꍇ�̂�
		if (change)	{

			// �ύX���܂��惁�b�Z�[�W
			nmlv_update.hdr.hwndFrom = m_h_wnd;
			nmlv_update.hdr.idFrom = get_id();
			nmlv_update.hdr.code = LVN_UPDATE_TEXT;
			nmlv_update.item_no = editing_item_no;
			nmlv_update.column_no = editing_column_no;
			nmlv_update.new_sel_no = dummy_combo.get_sel_no();
			SendMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_update);

			// �e�L�X�g��ύX
			set_cell_text(editing_item_no, editing_column_no, dummy_combo.get_item_text(nmlv_update.new_sel_no));
		
			// �ύX�����惁�b�Z�[�W
			nmlv_change.hdr.hwndFrom = m_h_wnd;
			nmlv_change.hdr.idFrom = get_id();
			nmlv_change.hdr.code = LVN_CHANGE_TEXT;
			nmlv_change.item_no = editing_item_no;
			nmlv_change.column_no = editing_column_no;
			nmlv_change.new_sel_no = nmlv_update.new_sel_no;
			PostMessage(get_parent_handle(), WM_NOTIFY, (WPARAM)get_id(), (LPARAM)&nmlv_change);
		}

		// �_�~�[�̃R���g���[����j��
		dummy_combo.destroy();
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�ҏW�p�̃G�f�B�b�g
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
				// �t�H�[�J�X���ړ������ꍇ�͕ҏW�I��
				// �������A�����Ă���X�s���Ɉڂ����ꍇ�͏���
				if (listview->editing && !(spin && ::GetFocus() == spin->get_handle()))	{
					listview->end_edit_item_edit(true, LVN_EDIT_END_REASON_NONE);
				}
				break;
			case WM_MOUSEWHEEL:
				return TRUE;		// �e�ɏ������񂳂Ȃ�
		}

		return Cc_edit::window_proc(msg, wp, lp);
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�ҏW�p�̃G�f�B�b�g�̃X�s��
	// ================================================================
	LRESULT Cc_listview::Citem_spin::window_proc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)	{
			case WM_KILLFOCUS:
				// �t�H�[�J�X���ړ������ꍇ�͕ҏW�I��
				// �������A�����Ă���G�f�B�b�g�Ɉڂ����ꍇ�͏���
				if (listview->editing && !(edit && ::GetFocus() == edit->get_handle()))	{
					listview->end_edit_item_edit(true, LVN_EDIT_END_REASON_NONE);
				}
		}

		return Cc_spin::window_proc(msg, wp, lp);
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�ҏW�p�̃R���{�{�b�N�X
	// ================================================================
	LRESULT Cc_listview::Citem_combo::window_proc(UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)	{
			case WM_KILLFOCUS:
				// �t�H�[�J�X���ړ������ꍇ�͕ҏW�I��
				if (listview->editing)	{
					listview->end_edit_item_combobox(false);
				}
		}

		return Cc_combobox::window_proc(msg, wp, lp);
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�ҏW�p�̃G�f�B�b�g�̃v���V�[�W���ݒ�
	// ================================================================
	void Cc_listview::set_edit_proc(WNDPROC proc)
	{
		dummy_edit.set_proc(proc);
	}

	// ****************************************************************
	// ���X�g�r���[�F�e�L�X�g�ҏW�p�̃R���{�{�b�N�X�̃v���V�[�W���ݒ�
	// ================================================================
	void Cc_listview::set_combobox_proc(WNDPROC proc)
	{
		dummy_combo.set_proc(proc);
	}

	// ****************************************************************
	// �|�b�v�A�b�v���j���[
	// ================================================================
	void Cc_listview::create_popup_menu()
	{
		C_popup_menu menu;

		menu.create();
		menu.add_item(1, _T("csv �ŕۑ�"), true, false);

		// ���j���[���J���I���J�n
		C_point mp = get_mouse_pos();	// ���������͓Ǝ��Ń}�E�X���W�����o��
		int syscom_id = menu.select(get_handle(), mp.x, mp.y);

		if (syscom_id == 1)
		{
			// csv �ŕۑ�����I�I
			C_select_file_dialog sfd;
			sfd.add_filter(_T("CSV �t�@�C��"), _T("*.csv"));
			TSTR file_path = sfd.select_save_file(_T("CSV �`���ŕۑ�"), _T("�V���� CSV �t�@�C��.csv"));
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
	// ini �ۑ�
	// ================================================================
	void Cc_listview::save_ini()
	{
		int column_cnt = get_column_cnt();

		// �R�������̕ۑ�
		for (int c = 0; c < column_cnt; c++)	{
			int column_width = get_column_width(c);
			save_ini_int(_T("header_") + name_list[c] + _T("_width"), column_width);
		}

		// �R���������̕ۑ�
		int order[256];
		HWND h_header = (HWND)send_message(LVM_GETHEADER, 0, 0);
		SendMessage(h_header, HDM_GETORDERARRAY, column_cnt, (LPARAM)order);
		for (int c = 0; c < column_cnt; c++)
			save_ini_int(_T("header_") + name_list[c] + _T("_order"), order[c]);
	}

	// ****************************************************************
	// ini �Ǎ�
	// ================================================================
	void Cc_listview::load_ini()
	{
		int column_cnt = get_column_cnt();

		// �R�������̓Ǎ�
		for (int c = 0; c < column_cnt; c++)	{
			int column_width = load_ini_int(_T("header_") + name_list[c] + _T("_width"), get_column_width(c));
			set_column_width(c, column_width);
		}

		// �R���������̓Ǎ�
		int order[256];
		for (int c = 0; c < column_cnt; c++)
			order[c] = load_ini_int(_T("header_") + name_list[c] + _T("_order"), c);

		HWND h_header = (HWND)send_message(LVM_GETHEADER, 0, 0);
		SendMessage(h_header, HDM_SETORDERARRAY, column_cnt, (LPARAM)order);
	}

	void Cc_listview::load_ini_column_width()
	{
		int column_cnt = get_column_cnt();

		// �R�������̓Ǎ�
		for (int c = 0; c < column_cnt; c++)	{
			int column_width = load_ini_int(_T("header_") + name_list[c] + _T("_width"), get_column_width(c));
			set_column_width(c, column_width);
		}
	}
}
