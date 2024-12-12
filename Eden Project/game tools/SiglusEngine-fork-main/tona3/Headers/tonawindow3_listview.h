#pragma		once

#include	"tonawindow3_control.h"

namespace NT3
{

	// ****************************************************************
	// リストビュー：アイテム編集メッセージ
	// ================================================================
	//
	//	LVN_UPDATE_TEXT:
	//	アイテムを編集した後、アイテムの内容を変更する前に WM_NOTIFY により送られてきます。
	//	以下の用にして編集内容を変更してください。
	//
	//	NMHDR* nmhdr = (NMHDR *)lp;
	//	if (nmhdr->idFrom == リストビューのＩＤ)	{
	//		if (nmhdr->code == LVN_UPDATE_TEXT)	{
	//			NMLVUPDATE* nmupdate = (NMLVUPDATE *)lp;
	//			nmupdate->new_text = 上書きするテキスト;
	//		}
	//	}
	//
	// ================================================================
	//
	//	LVN_CHANGE_TEXT:
	//	アイテムの内容が変更された後に WM_NOTIFY により送られてきます。
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

	// リストビューのアイテムを更新する
	typedef struct tagNMLVUPDATE
	{
		NMHDR	hdr;
		int		item_no;
		int		column_no;
		TSTR	old_text;		// エディットで編集したときに使用する
		TSTR	new_text;		// エディットで編集したときに使用する
		int		old_sel_no;		// コンボボックスで編集したときに使用する
		int		new_sel_no;		// コンボボックスで編集したときに使用する
	} NMLVUPDATE, *LPNMLVUPDATE;

	// リストビューのアイテムを更新した
	typedef struct tagNMLVCHANGE
	{
		NMHDR	hdr;
		int		item_no;
		int		column_no;
		TSTR	old_text;		// エディットで編集したときに使用する
		TSTR	new_text;		// エディットで編集したときに使用する
		int		old_sel_no;		// コンボボックスで編集したときに使用する
		int		new_sel_no;		// コンボボックスで編集したときに使用する
		int		end_reason;		// 終了した理由
	} NMLVCHANGE, *LPNMLVCHANGE;

	// リストビューのアイテムの編集を終了した
	typedef struct tagNMLVEDITEND
	{
		NMHDR	hdr;
		int		item_no;
		int		column_no;
		int		end_reason;		// 終了した理由
	} NMLVEDITEND, *LPNMLVEDITEND;

	// ****************************************************************
	// リストビュー
	//		ウィンドウスタイル
	//			LVS_ICON : アイコン
	//			LVS_LIST : リストビュー
	//			LVS_REPORT : レポート
	// ================================================================
	class Cc_listview : public C_control
	{
	public:
		Cc_listview()	{ m_class_name = WC_LISTVIEW; }

		// 設定
		void	clear();	// クリア
		void	easy_setting(bool is_full_row_select, bool is_grid_lines, bool is_check_boxes, bool is_header_drag_drop);	// 簡易設定

		// コラム関係
		int		get_column_cnt();												// コラム数を取得
		void	add_column(CTSTR& name, CTSTR& caption, int width, int align = LVCFMT_LEFT);
																				// コラムを追加（align = LVCFMT_LEFT / LVCFMT_CENTER / LVCFMT_RIGHT）
		void	insert_column(int column_no, CTSTR& name, CTSTR& caption, int width, int align = LVCFMT_LEFT);
																		 		// コラムを挿入（align = LVCFMT_LEFT / LVCFMT_CENTER / LVCFMT_RIGHT）
		void	delete_column(int column_no);									// コラムを削除
		void	delete_all_column();											// 全てのコラムを削除
		void	set_column_width(int column_no, int width);						// コラムの幅を設定
		void	set_column_width_just(int column_no, bool is_show_hsb);			// コラムの幅を設定（リストビューのサイズに合わせて指定のコラムのサイズを調整する）（水平スクロールバーの消去を指定できる：false = 消去）
		void	set_column_width_just(int column_no);							// コラムの幅を設定（リストビューのサイズに合わせて指定のコラムのサイズを調整する）
		int		get_column_width(int column_no);								// コラムの幅を取得

		// アイテム関係
		int		add_item();														// アイテムを追加
		int		add_item(int column_no, CTSTR& str);							// アイテムを追加
		int		add_item_visible(int column_no, CTSTR& str);					// アイテムを追加、可視範囲に来るようにスクロール
		int		insert_item(int item_no);										// アイテムを挿入
		int		insert_item(int item_no, int column_no, CTSTR& str);			// アイテムを挿入
		int		insert_item_visible(int item_no, int column_no, CTSTR& str);	// アイテムを挿入、可視範囲に来るようにスクロール
		void	delete_item(int item_no);										// アイテムを削除
		void	reduce_item(int item_cnt);										// アイテムを削減
		void	set_item_cnt(int item_cnt);										// アイテム数を設定（増やしたり減らしたりする）
		int		get_item_cnt();													// アイテム数を取得
		int		get_item_top_no();												// リストビュー：先頭のアイテム番号を取得する
		void	set_item_visible(int item_no);									// アイテムが可視範囲に来るようにスクロール
		void	set_item_visible_top(int item_no);								// アイテムが可視範囲の先頭に来るようにスクロール（item_no = -1 : 最後の項目が可視範囲になるようにする）

		// アイテムフォーカス関係
		void	set_item_focus(int item_no, bool is_focus);						// アイテムのフォーカス状態を設定
		int		get_item_focus(int item_no);									// アイテムのフォーカス状態を取得
		int		get_focused_item_no();											// フォーカスのあるアイテム番号を取得

		// アイテムセレクト関係
		void	set_item_select(int item_no, bool is_select);					// アイテムの選択状態を設定
		void	set_item_select_all(bool is_select);							// 全てのアイテムの選択状態を設定
		void	set_item_selection_mark(int item_no);							// 選択開始位置を設定する（Shith + Click で選択するときのスタート位置）
		bool	get_item_select(int item_no);									// アイテムが選択されているか判定
		int		get_selected_item_no();											// 選択されているアイテム番号を取得
		int		get_selected_item_cnt();										// 選択されているアイテムの個数を取得

		// アイテムチェック関係
		void	set_item_check(int item_no, bool check);						// アイテムのチェック状態を設定
		bool	get_item_check(int item_no);									// アイテムのチェック状態を取得
		int		get_checked_item_cnt();											// チェックのついているアイテム数を取得

		// イメージ関係
		void	set_image_list(HIMAGELIST h_image_list);						// イメージリストを設定
		void	set_item_image(int item_no, int image_no);						// イメージを設定

		// // // // //
		// リストビューのアイコンスペースのサイズを設定
		// lv_main.send_message(LVM_SETICONSPACING, 0, MAKELONG(Gdata.thumb_width + SPACE_WIDTH, Gdata.thumb_height + SPACE_HEIGHT));

		// セル関係
		void	set_cell_text(int item_no, int column_no, CTSTR& str);			// テキストを設定
		void	set_cell_text_visible(int item_no, int column_no, CTSTR& str);	// テキストを設定してアイテムを可視範囲に移動
		TSTR	get_cell_text(int item_no, int column_no);						// テキストを取得
		C_rect	get_cell_rect(int item_no, int column_no);						// アイテムの矩形を取得
		bool	get_cell_by_point(int x, int y, int* item_no, int* column_no);	// 座標（リストビューのクライアント空間）からセル番号を取得

		// テキストの編集
		void	edit_item_text_edit(int item_no, int column_no);													// 現在のセルの文字列を変更する
		void	edit_item_text_edit_with_str(int item_no, int column_no, CTSTR str);								// （文字列を渡すバージョン）
		void	edit_item_text_edit_for_num(int item_no, int column_no, int min, int max, int def);					// 現在のセルの数値を変更する
		void	edit_item_text_combobox(int item_no, int column_no, TCHAR** text_list, int text_cnt, int sel_no);	// コンボボックスで選択する
		void	end_edit_item_edit(bool change, int end_reason);
		void	end_edit_item_combobox(bool change);
		void	set_edit_proc(WNDPROC proc);
		void	set_combobox_proc(WNDPROC proc);

		// ini 関連
		void	save_ini();
		void	load_ini();
		void	load_ini_column_width();

		// 右クリックメニュー
		void	create_popup_menu();

	protected:
		LRESULT	window_proc(UINT msg, WPARAM wp, LPARAM lp);

	private:
		bool			on_destroy();
		ARRAY<TSTR>		name_list;

	protected:

		// ****************************************************************
		// アイテム編集用のエディット
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
		// アイテム編集用のスピン
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
		// アイテム編集用のコンボ
		// ================================================================
		class Citem_combo : public Cc_combobox
		{
		public:
			Cc_listview*	listview;

		protected:
			LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
		};

		// ****************************************************************
		// アイテム編集用
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
