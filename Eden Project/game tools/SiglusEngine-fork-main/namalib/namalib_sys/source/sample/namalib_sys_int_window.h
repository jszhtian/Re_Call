#pragma		once

// ****************************************************************
// 数値表示ウィンドウ
// ================================================================
class Cnamalib_num_window : public Cdialog
{

public:
				Cnamalib_num_window();										// コンストラクタ

	void		ready(CTSTR& window_title_str_, CTSTR& section_name_str_, int num_cnt_);	// 準備
	void		save_ini_last_state();										// 終了前の状態をＩＮＩに記憶
	bool		load_ini_last_state();										// 終了前の状態をＩＮＩから読み込む
	void		open_close(HWND parent_h_wnd, bool parent_focus);			// 開く／閉じる（作成／破棄）
	void		start_open(HWND parent_h_wnd);								// 起動時のオープン
	void		set_num(int index, int val);								// 数値設定
	void		update_proc();												// アップデートプロセス

	// 添え字付代入演算子
	int& operator [](int index)
	{
		if (0 > index || index >= (int)num_list.size()){	return dummy_num;	}
		return num_list[index];
	}

protected:
	LRESULT		window_proc(UINT msg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ

	TSTR		window_title_str;		// ウィンドウタイトル
	TSTR		section_name_str;		// セクション名
	ARRAY<int>	num_list;				// 数値変数リスト
	ARRAY<int>	num_list_last;			// 数値変数リスト（更新前の値）

	Cc_listview		_item_lstv_numlist;

private:
	int			dummy_num;
};

