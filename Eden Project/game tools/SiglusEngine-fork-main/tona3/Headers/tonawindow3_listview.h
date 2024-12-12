#pragma		once

#include	"tonawindow3_control.h"

namespace NT3
{

	// ****************************************************************
	// ���X�g�r���[�F�A�C�e���ҏW���b�Z�[�W
	// ================================================================
	//
	//	LVN_UPDATE_TEXT:
	//	�A�C�e����ҏW������A�A�C�e���̓��e��ύX����O�� WM_NOTIFY �ɂ�著���Ă��܂��B
	//	�ȉ��̗p�ɂ��ĕҏW���e��ύX���Ă��������B
	//
	//	NMHDR* nmhdr = (NMHDR *)lp;
	//	if (nmhdr->idFrom == ���X�g�r���[�̂h�c)	{
	//		if (nmhdr->code == LVN_UPDATE_TEXT)	{
	//			NMLVUPDATE* nmupdate = (NMLVUPDATE *)lp;
	//			nmupdate->new_text = �㏑������e�L�X�g;
	//		}
	//	}
	//
	// ================================================================
	//
	//	LVN_CHANGE_TEXT:
	//	�A�C�e���̓��e���ύX���ꂽ��� WM_NOTIFY �ɂ�著���Ă��܂��B
	//
	// ================================================================
	#define		LVN_UPDATE_TEXT			(LVN_FIRST - 90)
	#define		LVN_CHANGE_TEXT			(LVN_FIRST - 91)
	#define		LVN_EDIT_END_TEXT		(LVN_FIRST - 92)

	const int	LVN_EDIT_END_REASON_NONE = 0;
	const int	LVN_EDIT_END_REASON_RETURN = 1;
	const int	LVN_EDIT_END_REASON_ESCAPE = 2;
	const int	LVN_EDIT_END_REASON_UP = 3;
	const int	LVN_EDIT_END_REASON_DOWN = 4;

	// ���X�g�r���[�̃A�C�e�����X�V����
	typedef struct tagNMLVUPDATE
	{
		NMHDR	hdr;
		int		item_no;
		int		column_no;
		TSTR	old_text;		// �G�f�B�b�g�ŕҏW�����Ƃ��Ɏg�p����
		TSTR	new_text;		// �G�f�B�b�g�ŕҏW�����Ƃ��Ɏg�p����
		int		old_sel_no;		// �R���{�{�b�N�X�ŕҏW�����Ƃ��Ɏg�p����
		int		new_sel_no;		// �R���{�{�b�N�X�ŕҏW�����Ƃ��Ɏg�p����
	} NMLVUPDATE, *LPNMLVUPDATE;

	// ���X�g�r���[�̃A�C�e�����X�V����
	typedef struct tagNMLVCHANGE
	{
		NMHDR	hdr;
		int		item_no;
		int		column_no;
		TSTR	old_text;		// �G�f�B�b�g�ŕҏW�����Ƃ��Ɏg�p����
		TSTR	new_text;		// �G�f�B�b�g�ŕҏW�����Ƃ��Ɏg�p����
		int		old_sel_no;		// �R���{�{�b�N�X�ŕҏW�����Ƃ��Ɏg�p����
		int		new_sel_no;		// �R���{�{�b�N�X�ŕҏW�����Ƃ��Ɏg�p����
		int		end_reason;		// �I���������R
	} NMLVCHANGE, *LPNMLVCHANGE;

	// ���X�g�r���[�̃A�C�e���̕ҏW���I������
	typedef struct tagNMLVEDITEND
	{
		NMHDR	hdr;
		int		item_no;
		int		column_no;
		int		end_reason;		// �I���������R
	} NMLVEDITEND, *LPNMLVEDITEND;

	// ****************************************************************
	// ���X�g�r���[
	//		�E�B���h�E�X�^�C��
	//			LVS_ICON : �A�C�R��
	//			LVS_LIST : ���X�g�r���[
	//			LVS_REPORT : ���|�[�g
	// ================================================================
	class Cc_listview : public C_control
	{
	public:
		Cc_listview()	{ m_class_name = WC_LISTVIEW; }

		// �ݒ�
		void	clear();	// �N���A
		void	easy_setting(bool is_full_row_select, bool is_grid_lines, bool is_check_boxes, bool is_header_drag_drop);	// �ȈՐݒ�

		// �R�����֌W
		int		get_column_cnt();												// �R���������擾
		void	add_column(CTSTR& name, CTSTR& caption, int width, int align = LVCFMT_LEFT);
																				// �R������ǉ��ialign = LVCFMT_LEFT / LVCFMT_CENTER / LVCFMT_RIGHT�j
		void	insert_column(int column_no, CTSTR& name, CTSTR& caption, int width, int align = LVCFMT_LEFT);
																		 		// �R������}���ialign = LVCFMT_LEFT / LVCFMT_CENTER / LVCFMT_RIGHT�j
		void	delete_column(int column_no);									// �R�������폜
		void	delete_all_column();											// �S�ẴR�������폜
		void	set_column_width(int column_no, int width);						// �R�����̕���ݒ�
		void	set_column_width_just(int column_no, bool is_show_hsb);			// �R�����̕���ݒ�i���X�g�r���[�̃T�C�Y�ɍ��킹�Ďw��̃R�����̃T�C�Y�𒲐�����j�i�����X�N���[���o�[�̏������w��ł���Ffalse = �����j
		void	set_column_width_just(int column_no);							// �R�����̕���ݒ�i���X�g�r���[�̃T�C�Y�ɍ��킹�Ďw��̃R�����̃T�C�Y�𒲐�����j
		int		get_column_width(int column_no);								// �R�����̕����擾

		// �A�C�e���֌W
		int		add_item();														// �A�C�e����ǉ�
		int		add_item(int column_no, CTSTR& str);							// �A�C�e����ǉ�
		int		add_item_visible(int column_no, CTSTR& str);					// �A�C�e����ǉ��A���͈͂ɗ���悤�ɃX�N���[��
		int		insert_item(int item_no);										// �A�C�e����}��
		int		insert_item(int item_no, int column_no, CTSTR& str);			// �A�C�e����}��
		int		insert_item_visible(int item_no, int column_no, CTSTR& str);	// �A�C�e����}���A���͈͂ɗ���悤�ɃX�N���[��
		void	delete_item(int item_no);										// �A�C�e�����폜
		void	reduce_item(int item_cnt);										// �A�C�e�����팸
		void	set_item_cnt(int item_cnt);										// �A�C�e������ݒ�i���₵���茸�炵���肷��j
		int		get_item_cnt();													// �A�C�e�������擾
		int		get_item_top_no();												// ���X�g�r���[�F�擪�̃A�C�e���ԍ����擾����
		void	set_item_visible(int item_no);									// �A�C�e�������͈͂ɗ���悤�ɃX�N���[��
		void	set_item_visible_top(int item_no);								// �A�C�e�������͈͂̐擪�ɗ���悤�ɃX�N���[���iitem_no = -1 : �Ō�̍��ڂ����͈͂ɂȂ�悤�ɂ���j

		// �A�C�e���t�H�[�J�X�֌W
		void	set_item_focus(int item_no, bool is_focus);						// �A�C�e���̃t�H�[�J�X��Ԃ�ݒ�
		int		get_item_focus(int item_no);									// �A�C�e���̃t�H�[�J�X��Ԃ��擾
		int		get_focused_item_no();											// �t�H�[�J�X�̂���A�C�e���ԍ����擾

		// �A�C�e���Z���N�g�֌W
		void	set_item_select(int item_no, bool is_select);					// �A�C�e���̑I����Ԃ�ݒ�
		void	set_item_select_all(bool is_select);							// �S�ẴA�C�e���̑I����Ԃ�ݒ�
		void	set_item_selection_mark(int item_no);							// �I���J�n�ʒu��ݒ肷��iShith + Click �őI������Ƃ��̃X�^�[�g�ʒu�j
		bool	get_item_select(int item_no);									// �A�C�e�����I������Ă��邩����
		int		get_selected_item_no();											// �I������Ă���A�C�e���ԍ����擾
		int		get_selected_item_cnt();										// �I������Ă���A�C�e���̌����擾

		// �A�C�e���`�F�b�N�֌W
		void	set_item_check(int item_no, bool check);						// �A�C�e���̃`�F�b�N��Ԃ�ݒ�
		bool	get_item_check(int item_no);									// �A�C�e���̃`�F�b�N��Ԃ��擾
		int		get_checked_item_cnt();											// �`�F�b�N�̂��Ă���A�C�e�������擾

		// �C���[�W�֌W
		void	set_image_list(HIMAGELIST h_image_list);						// �C���[�W���X�g��ݒ�
		void	set_item_image(int item_no, int image_no);						// �C���[�W��ݒ�

		// // // // //
		// ���X�g�r���[�̃A�C�R���X�y�[�X�̃T�C�Y��ݒ�
		// lv_main.send_message(LVM_SETICONSPACING, 0, MAKELONG(Gdata.thumb_width + SPACE_WIDTH, Gdata.thumb_height + SPACE_HEIGHT));

		// �Z���֌W
		void	set_cell_text(int item_no, int column_no, CTSTR& str);			// �e�L�X�g��ݒ�
		void	set_cell_text_visible(int item_no, int column_no, CTSTR& str);	// �e�L�X�g��ݒ肵�ăA�C�e�������͈͂Ɉړ�
		TSTR	get_cell_text(int item_no, int column_no);						// �e�L�X�g���擾
		C_rect	get_cell_rect(int item_no, int column_no);						// �A�C�e���̋�`���擾
		bool	get_cell_by_point(int x, int y, int* item_no, int* column_no);	// ���W�i���X�g�r���[�̃N���C�A���g��ԁj����Z���ԍ����擾

		// �e�L�X�g�̕ҏW
		void	edit_item_text_edit(int item_no, int column_no);													// ���݂̃Z���̕������ύX����
		void	edit_item_text_edit_with_str(int item_no, int column_no, CTSTR str);								// �i�������n���o�[�W�����j
		void	edit_item_text_edit_for_num(int item_no, int column_no, int min, int max, int def);					// ���݂̃Z���̐��l��ύX����
		void	edit_item_text_combobox(int item_no, int column_no, TCHAR** text_list, int text_cnt, int sel_no);	// �R���{�{�b�N�X�őI������
		void	end_edit_item_edit(bool change, int end_reason);
		void	end_edit_item_combobox(bool change);
		void	set_edit_proc(WNDPROC proc);
		void	set_combobox_proc(WNDPROC proc);

		// ini �֘A
		void	save_ini();
		void	load_ini();
		void	load_ini_column_width();

		// �E�N���b�N���j���[
		void	create_popup_menu();

	protected:
		LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);

	private:
		bool			on_destroy();
		ARRAY<TSTR>		name_list;

	protected:

		// ****************************************************************
		// �A�C�e���ҏW�p�̃G�f�B�b�g
		// ================================================================
		class Citem_edit : public Cc_edit
		{
		public:
			Cc_listview*	listview;
			Cc_spin*		spin;

		protected:
			LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
		};

		// ****************************************************************
		// �A�C�e���ҏW�p�̃X�s��
		// ================================================================
		class Citem_spin : public Cc_spin
		{
		public:
			Cc_listview*	listview;
			Cc_edit*		edit;

		protected:
			LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
		};

		// ****************************************************************
		// �A�C�e���ҏW�p�̃R���{
		// ================================================================
		class Citem_combo : public Cc_combobox
		{
		public:
			Cc_listview*	listview;

		protected:
			LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
		};

		// ****************************************************************
		// �A�C�e���ҏW�p
		// ================================================================
		Citem_edit	dummy_edit;
		Citem_spin	dummy_spin;
		Citem_combo	dummy_combo;
		bool		editing;
		int			editing_item_no;
		int			editing_column_no;
		bool		editing_num;
		int			editing_num_min;
		int			editing_num_max;
		int			editing_num_def;
		NMLVUPDATE	nmlv_update;
		NMLVCHANGE	nmlv_change;
		NMLVEDITEND	nmlv_edit_end;

	};
}
