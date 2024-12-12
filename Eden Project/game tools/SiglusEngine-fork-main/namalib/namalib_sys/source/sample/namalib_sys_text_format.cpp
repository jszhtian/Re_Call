#include	"pch.h"
#include	"namalib_sys_text_analyze.h"
#include	"namalib_sys_text_format.h"

// ****************************************************************
// コメント削除（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
bool Cnamalib_text_format::_tf_delete_comment(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list)
{
	TSTR new_text;
	TSTR::iterator itr;
	int line;
	bool ret = true;
	NAMALIB_SIMPLE_ERR_STRUCT err;

	// コメント削除
	line = 1;
	itr = text.begin();
	while (1)	{
		// 終了
		if (*itr == _T('\0'))		{
			new_text += *itr++; break;
		}
		// 改行
		else if (*itr == _T('\n'))	{
			new_text += *itr++; line++;
		}
		// 行コメント（;）
		else if (*itr == _T(';'))	{
			itr++;
			while (1)	{
				if (*itr == _T('\0'))			{	break;		}		// コメント終了
				else if (*itr == _T('\n'))		{	break;		}		// コメント終了
				else							{	itr++;		}		// コメント文字
			}
		}
		// 行コメント（//）
		else if (*itr == _T('/') && *(itr + 1) == _T('/'))	{
			itr += 2;
			while (1)	{
				if (*itr == _T('\0'))			{	break;		}		// コメント終了
				else if (*itr == _T('\n'))		{	break;		}		// コメント終了
				else							{	itr++;		}		// コメント文字
			}
		}
		// ブロックコメント（/* ～ */）
		else if (*itr == _T('/') && *(itr + 1) == _T('*'))	{
			itr += 2;
			err.line = line;
			while (1)				{
				// ブロックコメントエラー終了
				if (*itr == _T('\0'))	{
					if (err_list != NULL)	{
						err.msg = _T("ブロックコメントが閉じられていません。");
						err_list->push_back(err);
					}
					ret = false; break;
				}
				// ブロックコメント中の改行
				else if (*itr == _T('\n'))	{
					new_text += *itr++; line++;
				}
				// ブロックコメント終了
				else if (*itr == _T('*') && *(itr + 1) == _T('/'))	{
					itr += 2; break;
				}
				// ブロックコメント中の文字
				else	{
					itr++;
				}
			}
		}
		// 半角制御（" ～ "）
		else if (*itr == _T('"'))	{
			ret = _ta_copy_hankaku_control(itr, new_text, line, err_list, true);
		}
		// 有効な文字
		else						{
			new_text += *itr++;
		}

		// エラー終了
		if	(ret == false)			{
			break;
		}
	}

	if (ret)	{
		text = new_text;
	}

	return ret;
}

// ****************************************************************
// 大文字化（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
bool Cnamalib_text_format::_tf_change_to_upper(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list)
{
	TSTR new_text;
	TSTR::iterator itr;
	int line;
	bool ret = true;
	NAMALIB_SIMPLE_ERR_STRUCT err;

	// コメント削除
	line = 1;
	itr = text.begin();
	while (1)	{
		// 終了
		if (*itr == _T('\0'))		{
			new_text += *itr++; break;
		}
		// 改行
		else if (*itr == _T('\n'))	{
			new_text += *itr++; line++;
		}
		// 半角制御（" ～ "）
		else if (*itr == _T('"'))	{
			ret = _ta_copy_hankaku_control(itr, new_text, line, err_list, true);
		}
		// 小文字
		else if (is_small_alpha(*itr))	{
			new_text += *itr + LARGE_ALPHA_ADD; itr++;
		}
		// その他の文字
		else						{
			new_text += *itr++;
		}

		// エラー終了
		if	(ret == false)			{
			break;
		}
	}

	if (ret)	{
		text = new_text;
	}

	return ret;
}

// ****************************************************************
// 単純フォーマット（コメント削除／大文字化）（※送り込むテキストの最後に \0 を付加しておく事）
// ================================================================
bool Cnamalib_text_format::_tf_simple_format(TSTR& text, NAMALIB_SIMPLE_ERR_LIST *err_list)
{
	// コメント削除
	if (_tf_delete_comment(text, err_list) == false)		{
		return false;
	}

	// 大文字化
	if (_tf_change_to_upper(text, err_list) == false)	{
		return false;
	}

	return true;
}

