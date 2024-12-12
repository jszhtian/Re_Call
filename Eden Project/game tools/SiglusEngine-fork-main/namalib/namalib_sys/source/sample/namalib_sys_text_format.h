#pragma		once

// ****************************************************************
// テキスト整形
// ================================================================
class Cnamalib_text_format : public Cnamalib_text_analyze
{

public:

	// コメント削除（※送り込むテキストの最後に \0 を付加しておく事）
	static	bool		_tf_delete_comment(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list);

	// 大文字化（※送り込むテキストの最後に \0 を付加しておく事）
	static	bool		_tf_change_to_upper(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list);

	// 単純フォーマット（コメント削除／大文字化）（※送り込むテキストの最後に \0 を付加しておく事）
	static	bool		_tf_simple_format(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list);
};


