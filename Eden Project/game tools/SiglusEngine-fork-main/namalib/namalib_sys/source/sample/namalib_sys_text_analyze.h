#pragma		once

// ****************************************************************
// テキスト解析
// ================================================================
class Cnamalib_text_analyze
{

public:

	// 半角制御コピー（※送り込むテキストの最後に \0 を付加しておく事）
	static	bool		_ta_copy_hankaku_control(TSTR::iterator& itr, TSTR& dst_text, int err_line, NAMALIB_SIMPLE_ERR_LIST *err_list, bool control_moji_copy);

	// 半角制御取得（※半角制御のエラー処理はしていない）
	static	TSTR		_ta_get_hankaku_control(TSTR::iterator& itr);

	// 行取得
	static	TSTR		_ta_get_line(TSTR::iterator& itr);

	// 区切り文字判定
	static	bool		_ta_check_kugiri(TCHAR c);

	// 区切りジャンプ（※送り込むテキストの最後に \0 を付加しておく事）
	static	void		_ta_jump_kugiri(TSTR::iterator& itr);

	// 行ジャンプ（※送り込むテキストの最後に \0 を付加しておく事）
	static	void		_ta_jump_line(TSTR::iterator& itr);

	// トークン取得（※送り込むテキストの最後に \0 を付加しておく事）
	static	TSTR		_ta_get_token(TSTR::iterator& itr, bool jump_kugiri_mod);

	// トークンをチェックする
	static	int			_ta_check_token(TSTR& token, int cnt, ...);
	// トークンを取得しチェックする（※送り込むテキストの最後に \0 を付加しておく事）
	static	int			_ta_get_check_token(TSTR::iterator& itr, bool jump_kugiri_mod, int cnt, ...);

	// トークンをリストチェックする
	static	int			_ta_find_token_list(TSTR& token, int list_cnt, TCHAR **top);
	// トークンを取得しリストチェックする（※送り込むテキストの最後に \0 を付加しておく事）
	static	int			_ta_get_find_token_list(TSTR::iterator& itr, bool jump_kugiri_mod, int list_cnt, TCHAR **top);

	// １文字チェック（※送り込むテキストの最後に \0 を付加しておく事）
	static	bool		_ta_check_moji(TSTR::iterator& itr, TCHAR moji, bool jump_kugiri_mod);

	// 指定の文字の後の「半角制御の文字」を１つ取得
	static	bool		_ta_get_hanakau_control(TSTR::iterator& start_itr, TCHAR moji, TSTR& str);

	// ＩＮＴ値を取得
	static	bool		_ta_get_int_num(TSTR::iterator& start_itr, int *num_);
	// 符号なしＩＮＴ値を取得
	static	bool		_ta_get_unsigned_int_num(TSTR::iterator& start_itr, int *num_);

	// 連続する「数値」の個数を取得
	static	int			_ta_get_nums_cnt(TSTR::iterator itr, TCHAR moji);
	// 連続する「符号なし数値」の個数を取得
	static	int			_ta_get_unsigned_nums_cnt(TSTR::iterator itr, TCHAR moji);

	// 連続する「数値」を指定数取得（※多くても少なくても失敗します）
	static	bool		_ta_get_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...);
	// 連続する「符号なし数値」を指定数取得（※多くても少なくても失敗します）
	static	bool		_ta_get_unsigned_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...);

	// 連続する「数値」を指定数取得（バッファ版）（※多くても少なくても失敗します）
	static	bool		_ta_get_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf);
	// 連続する「符号なし数値」を指定数取得（バッファ版）（※多くても少なくても失敗します）
	static	bool		_ta_get_unsigned_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf);

	// 連続する「数値」を指定数取得（可変配列版）（※多くても少なくても失敗します）
	static	bool		_ta_get_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag);
	// 連続する「符号なし数値」を指定数取得（可変配列版）（※多くても少なくても失敗します）
	static	bool		_ta_get_unsigned_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag);

protected:

	// 解析用の終了イテレータ
	TSTR::iterator		_ta_analyze_end_itr;
};


