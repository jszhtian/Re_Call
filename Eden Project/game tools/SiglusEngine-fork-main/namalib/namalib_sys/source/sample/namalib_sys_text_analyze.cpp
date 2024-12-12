#include	"pch.h"
#include	"namalib_sys_text_analyze.h"

// ****************************************************************
// 半角制御コピー（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
bool Cnamalib_text_analyze::_ta_copy_hankaku_control(TSTR::iterator& itr, TSTR& dst_text, int err_line, NAMALIB_SIMPLE_ERR_LIST *err_list, bool control_moji_copy)
{
	bool ret = true;
	NAMALIB_SIMPLE_ERR_STRUCT err;

	// 半角制御の始まりの記号
	if (control_moji_copy)	{	dst_text += *itr;	}
	itr++;

	while (1)	{
		if (_is_line_end(*itr))	{		// エラー終了
			if (err_list != NULL)	{
				err.line = err_line;
				err.msg = _T("半角制御が閉じられていません。もしくは、半角制御の途中で改行していませんか？");
				err_list->push_back(err);
			}
			ret = false; break;
		}
		else if (*itr == _T('"'))	{		// 半角制御終了
			if (control_moji_copy)	{	dst_text += *itr;	}
			itr++; break;
		}
		else						{		// 半角制御文字
			dst_text += *itr++;
		}
	}

	return ret;
}

// ****************************************************************
// 半角制御取得（※半角制御のエラー処理はしていない）
// ================================================================
TSTR Cnamalib_text_analyze::_ta_get_hankaku_control(TSTR::iterator& itr)
{
	TSTR str;

	itr++;		// 半角制御の始まりの記号

	while (1)	{
		if (*itr == _T('"'))	{		// 半角制御終了
			itr++; break;
		}
		else					{		// 半角制御文字
			str += *itr++;
		}
	}

	return str;
}

// ****************************************************************
// 行取得
// ================================================================
TSTR Cnamalib_text_analyze::_ta_get_line(TSTR::iterator& itr)
{
	TSTR str;

	while (1)	{
		// 終了／改行
		if (_is_line_end(*itr))	{
			break;
		}
		// その他の文字
		else	{
			str += *itr++;
		}
	}

	return str;
}

// ****************************************************************
// 区切り文字判定
// ================================================================
bool Cnamalib_text_analyze::_ta_check_kugiri(TCHAR c)
{
	if ((c == _T(' ')) || (c == _T('\t')) || (c == _T(',')))	{	return true;	}
	return false;
}

// ****************************************************************
// 区切りジャンプ（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
void Cnamalib_text_analyze::_ta_jump_kugiri(TSTR::iterator& itr)
{
	while (1)	{
		// 区切り文字
		if ((*itr == _T(' ')) || (*itr == _T('\t')) || (*itr == _T(',')))	{	itr++;	}
		// その他の文字
		else																{	break;	}
	}
}

// ****************************************************************
// 行ジャンプ（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
void Cnamalib_text_analyze::_ta_jump_line(TSTR::iterator& itr)
{
	while (1)	{
		// 終了／改行
		if (_is_line_end(*itr))	{	break;	}
		// その他の文字
		else					{	itr++;	}
	}
}

// ****************************************************************
// トークン取得（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
TSTR Cnamalib_text_analyze::_ta_get_token(TSTR::iterator& itr, bool jump_kugiri_mod)
{
	TSTR token;
	bool moji_get = false;

	// 区切りジャンプ
	if (jump_kugiri_mod)	{	_ta_jump_kugiri(itr);	}

	while (1)	{
		// トークン文字
		if (is_alpha(*itr) || *itr == _T('_') || *itr == _T('@'))	{
			token += *itr++;
			moji_get = true;
		}
		// 先頭に使えないトークン文字
		else if (is_num(*itr))	{
			if (moji_get)	{	token += *itr++;	}
			else			{	break;				}
		}
		// その他の文字
		else	{	break;	}
	}

	return token;
}

// ****************************************************************
// トークンをチェックする
// ================================================================
int Cnamalib_text_analyze::_ta_check_token(TSTR& token, int cnt, ...)
{
	TCHAR *str;
	if (token.empty())	{	return -1;	}
	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		str = va_arg(arg, TCHAR *);
		if (str_get_upper(token) == str)	{	return i;	}
	}
	va_end(arg);
	return -1;
}

// トークンを取得しチェックする（※送り込むテキストの最後に \0 を付加しておく事）
int Cnamalib_text_analyze::_ta_get_check_token(TSTR::iterator& itr, bool jump_kugiri_mod, int cnt, ...)
{
	TCHAR *str;
	TSTR token = _ta_get_token(itr, jump_kugiri_mod);
	if (token.empty())	{	return -1;	}
	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		str = va_arg(arg, TCHAR *);
		if (str_get_upper(token) == str)	{	return i;	}
	}
	va_end(arg);
	return -1;
}

// ****************************************************************
// トークンをリストチェックする
// ================================================================
int Cnamalib_text_analyze::_ta_find_token_list(TSTR& token, int list_cnt, TCHAR **top)
{
	if (token.empty())	{	return -1;	}
	TCHAR **end = top + list_cnt;
	TCHAR **res = std::find(top, end, str_get_upper(token));
	if (res == end)	{
		return -1;
	}
	return (int)((PTR)res - (PTR)top) / sizeof(TCHAR **);
}

// トークンを取得しリストチェックする（※送り込むテキストの最後に \0 を付加しておく事）
int Cnamalib_text_analyze::_ta_get_find_token_list(TSTR::iterator& itr, bool jump_kugiri_mod, int list_cnt, TCHAR **top)
{
	TSTR token = _ta_get_token(itr, jump_kugiri_mod);
	if (token.empty())	{	return -1;	}
	return _ta_find_token_list(token, list_cnt, top);
}

// ****************************************************************
// １文字チェック（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
bool Cnamalib_text_analyze::_ta_check_moji(TSTR::iterator& itr, TCHAR moji, bool jump_kugiri_mod)
{
	TCHAR target_moji;

	// 区切りジャンプ
	if (jump_kugiri_mod)	{	_ta_jump_kugiri(itr);	}

	// 大文字化
	if (is_small_alpha(moji))	{	moji += LARGE_ALPHA_ADD;	}

	// 大文字化
	target_moji = *itr;
	if (is_small_alpha(target_moji))	{	target_moji += LARGE_ALPHA_ADD;		}

	// 比較
	if (target_moji != moji)	{	return false;	}

	// 文字を飛ばす
	itr++;

	return true;
}

// ****************************************************************
// 指定の文字の後の「半角制御の文字」を１つ取得
// ================================================================
bool Cnamalib_text_analyze::_ta_get_hanakau_control(TSTR::iterator& start_itr, TCHAR moji, TSTR& str)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}

	_ta_jump_kugiri(itr);
	if (*itr != _T('"'))	{	return false;	}

	str = _ta_get_hankaku_control(itr);

	// 返却
	start_itr = itr;

	return true;
}

// ****************************************************************
// ＩＮＴ値を取得
// ================================================================
bool Cnamalib_text_analyze::_ta_get_int_num(TSTR::iterator& start_itr, int *num_)
{
	TSTR::iterator itr = start_itr;

	// 符号
	int sign = 1;

	// 符号判定
	if (*itr == _T('+'))		{				itr++;	_ta_jump_kugiri(itr);	}
	else if (*itr == _T('-'))	{	sign = -1;	itr++;	_ta_jump_kugiri(itr);	}

	// 数値判定
	bool ok = false;
	int num = 0;
	while (1)	{
		if (is_num(*itr))	{	num = num * 10 + (*itr - _T('0'));	ok = true;	itr++;	}
		else				{	break;	}
	}

	// 数値を取得していない
	if (!ok)	{	return false;	}

	// 符号をつける
	num *= sign;

	// 返却
	start_itr = itr;
	if (num_)	{	*num_ = num;	}

	return true;
}

// 符号なしＩＮＴ値を取得
bool Cnamalib_text_analyze::_ta_get_unsigned_int_num(TSTR::iterator& start_itr, int *num_)
{
	TSTR::iterator itr = start_itr;

	// 数値判定
	bool ok = false;
	int num = 0;
	while (1)	{
		if (is_num(*itr))	{	num = num * 10 + (*itr - _T('0'));	ok = true;	itr++;	}
		else				{	break;	}
	}

	// 数値を取得していない
	if (!ok)	{	return false;	}

	// 返却
	start_itr = itr;
	if (num_)	{	*num_ = num;	}

	return true;
}

// ****************************************************************
// 連続する「数値」の個数を取得
// ================================================================
int Cnamalib_text_analyze::_ta_get_nums_cnt(TSTR::iterator itr, TCHAR moji)
{
	int cnt;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return 0;	}

	cnt = 0;
	while (1)	{
		_ta_jump_kugiri(itr);
		if (_ta_get_int_num(itr, NULL) == false)	{	break;	}
		cnt++;
	}

	return cnt;
}

// 連続する「符号なし数値」の個数を取得
int Cnamalib_text_analyze::_ta_get_unsigned_nums_cnt(TSTR::iterator itr, TCHAR moji)
{
	int cnt;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return 0;	}

	cnt = 0;
	while (1)	{
		_ta_jump_kugiri(itr);
		if (_ta_get_unsigned_int_num(itr, NULL) == false)	{	break;	}
		cnt++;
	}

	return cnt;
}


// ****************************************************************
// 連続する「数値」を指定数取得（※多くても少なくても失敗します）
// ================================================================
bool Cnamalib_text_analyze::_ta_get_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_nums_cnt(itr, _T('\0')) != cnt)							{	return false;	}

	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_int_num(itr, va_arg(arg, int *));
	}
	va_end(arg);

	// 返却
	start_itr = itr;

	return true;
}

// 連続する「符号なし数値」を指定数取得（※多くても少なくても失敗します）
bool Cnamalib_text_analyze::_ta_get_unsigned_nums(TSTR::iterator& start_itr, TCHAR moji, int cnt, ...)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_unsigned_nums_cnt(itr, _T('\0')) != cnt)				{	return false;	}

	va_list arg;
	va_start(arg, cnt);
	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_unsigned_int_num(itr, va_arg(arg, int *));
	}
	va_end(arg);

	// 返却
	start_itr = itr;

	return true;
}

// ****************************************************************
// 連続する「数値」を指定数取得（バッファ版）（※多くても少なくても失敗します）
// ================================================================
bool Cnamalib_text_analyze::_ta_get_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_nums_cnt(itr, _T('\0')) != cnt)							{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_int_num(itr, nums_buf);
		nums_buf++;
	}

	// 返却
	start_itr = itr;

	return true;
}

// 連続する「符号なし数値」を指定数取得（バッファ版）（※多くても少なくても失敗します）
bool Cnamalib_text_analyze::_ta_get_unsigned_nums_buf(TSTR::iterator& start_itr, TCHAR moji, int cnt, int *nums_buf)
{
	TSTR::iterator itr = start_itr;

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_unsigned_nums_cnt(itr, _T('\0')) != cnt)				{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_unsigned_int_num(itr, nums_buf);
		nums_buf++;
	}

	// 返却
	start_itr = itr;

	return true;
}

// ****************************************************************
// 連続する「数値」を指定数取得（可変配列版）（※多くても少なくても失敗します）
// ================================================================
bool Cnamalib_text_analyze::_ta_get_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag)
{
	TSTR::iterator itr = start_itr;
	int num;

	if (clear_flag)	{	num_array.clear();	}

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_nums_cnt(itr, _T('\0')) != cnt)							{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_int_num(itr, &num);
		num_array.push_back(num);
	}

	// 返却
	start_itr = itr;

	return true;
}

// 連続する「符号なし数値」を指定数取得（可変配列版）（※多くても少なくても失敗します）
bool Cnamalib_text_analyze::_ta_get_unsigned_nums_array(TSTR::iterator& start_itr, TCHAR moji, int cnt, ARRAY<int>& num_array, bool clear_flag)
{
	TSTR::iterator itr = start_itr;
	int num;

	if (clear_flag)	{	num_array.clear();	}

	if (moji != _T('\0') && _ta_check_moji(itr, moji, true) == false)	{	return false;	}
	if (_ta_get_unsigned_nums_cnt(itr, _T('\0')) != cnt)				{	return false;	}

	for (int i = 0; i < cnt; i++)	{
		_ta_jump_kugiri(itr);
		_ta_get_unsigned_int_num(itr, &num);
		num_array.push_back(num);
	}

	// 返却
	start_itr = itr;

	return true;
}



