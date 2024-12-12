#include	"pch.h"
#include	"namalib_sys_text_analyze.h"
#include	"namalib_sys_text_format.h"

namespace Nnama
{

// ****************************************************************
// コメント削除
// ================================================================

// 変換（半角制御のエラーチェックも兼ねている）
bool _ntf_delete_comment(TSTR& text, int* err_line, TSTR& err_msg)
{
	TSTR::iterator itr = text.begin();
	int line_no = 1;
	TSTR new_text;

	while (1)	{
		// 終了
		if (*itr == _T('\0'))		{
			new_text += *itr++;
			text = new_text;
			return true;
		}

		// 改行
		else if (*itr == _T('\n'))	{
			new_text += *itr++;		line_no++;
		}

		// 行コメント（;）
		else if (*itr == _T(';'))	{
			_nta_jump_line(itr);
		}

		// 行コメント（//）
		else if (*itr == _T('/') && *(itr + 1) == _T('/'))	{
			_nta_jump_line(itr);
		}

		// ブロックコメント（/* ～ */）
		else if (*itr == _T('/') && *(itr + 1) == _T('*'))	{
			itr += 2;
			*err_line = line_no;
			while (1)				{
				// ブロックコメントエラー
				if (*itr == _T('\0'))	{
					err_msg = _T("ブロックコメントが閉じられていません。");
					return false;
				}
				// ブロックコメント中の改行
				else if (*itr == _T('\n'))	{
					new_text += *itr++;		line_no++;
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

		// 有効な文字
		else						{
			new_text += *itr++;
		}
	}
}

// 変換（//* = ssa）（半角制御のエラーチェックも兼ねている）
bool _ntf_delete_comment_ssa(TSTR& text, int* err_line, TSTR& err_msg)
{
	TSTR::iterator itr = text.begin();
	int line_no = 1;
	TSTR new_text;

	while (1)	{
		// 終了
		if (*itr == _T('\0'))		{
			new_text += *itr++;
			text = new_text;
			return true;
		}

		// 改行
		else if (*itr == _T('\n'))	{
			new_text += *itr++;		line_no++;
		}

		// 行コメント（//*）
		else if (*itr == _T('/') && *(itr + 1) == _T('/') && *(itr + 2) == _T('*'))	{
			_nta_jump_line(itr);
		}

		// 有効な文字
		else						{
			new_text += *itr++;
		}
	}
}

// ****************************************************************
// 大文字化
// ================================================================

// 取得
TSTR _ntf_ret_change_to_upper(CTSTR& text)
{
	TSTR::const_iterator itr = text.begin();
	TSTR new_text;

	while (1)	{
		// 終了
		if (*itr == _T('\0'))	{	new_text += *itr++;		return new_text;	}
		// 小文字
		else if (is_small_alpha(*itr))	{	new_text += *itr++ + LARGE_ALPHA_ADD;	}
		// その他の文字
		else	{	new_text += *itr++;		}
	}
}

// 変換
void _ntf_change_to_upper(TSTR& text)
{
	text = _ntf_ret_change_to_upper(text);
}

// ****************************************************************
// 行末の不要な空白文字を削除
// ================================================================

// 取得
TSTR _ntf_ret_delete_line_end_space(TSTR& text)
{
	TSTR new_text;
	TSTR::iterator itr = text.begin();
	while (1)	{
		// 終了
		if (*itr == _T('\0'))		{	new_text += *itr++;	return new_text;	}
		// 改行
		else if (*itr == _T('\n'))	{	new_text += *itr++;	}
		// 空白文字
		else if (_nta_check_space(*itr))	{
			TSTR::iterator backup_itr = itr;
			while (1)	{
				// 空白ジャンプ
				_nta_jump_space(itr);
				// 終了
				if (_nta_check_end(*itr))	{	break;	}
				// その他
				else	{	new_text += TSTR(backup_itr, itr);	break;	}
			}
		}
		// その他
		else	{	new_text += *itr++;	}
	}
}

// 変換
void _ntf_delete_line_end_space(TSTR& text)
{
	text = _ntf_ret_delete_line_end_space(text);
}

// ****************************************************************
// 後方の不要な空白行を削除
// ================================================================

// 取得
TSTR _ntf_ret_delete_backward_space_line(TSTR& text)
{
	TSTR new_text;
	TSTR::iterator itr = text.begin();
	while (1)	{
		// 終了
		if (*itr == _T('\0'))		{	new_text += *itr++;	return new_text;	}
		// 改行もしくは空白文字
		else if (*itr == _T('\n') || _nta_check_space(*itr))	{
			TSTR::iterator backup_itr = itr;
			while (1)	{
				// 空白ジャンプ
				_nta_jump_space(itr);
				// 終了
				if (*itr == _T('\0'))	{	break;	}
				// 改行
				else if (*itr == _T('\n'))	{	itr++;	}
				// その他
				else	{	new_text += TSTR(backup_itr, itr);	break;	}
			}
		}
		// その他
		else	{
			new_text += _nta_get_line(itr);
			if (*itr == _T('\n'))	{	new_text += *itr++;	}
		}
	}
}

// 変換
void _ntf_delete_backward_space_line(TSTR& text)
{
	text = _ntf_ret_delete_backward_space_line(text);
}

// ****************************************************************
// インデント挿入
// ================================================================

// 取得
TSTR _ntf_ret_insert_indent(CTSTR& text, bool is_first_line, bool is_last_line, CTSTR& indent)
{
	TSTR::const_iterator itr = text.begin();
	TSTR new_text;

	while (1)	{
		if (is_first_line)	{
			if (is_last_line)	{	new_text += indent;	}
			else if (*itr != _T('\0'))	{	new_text += indent;	}
		}
		while (1)	{
			// 終了
			if (*itr == _T('\0'))	{	new_text += *itr++;	return new_text;	}
			// 改行
			else if (*itr == _T('\n'))	{
				new_text += *itr++;
				// 連続する改行はインデントしない
				while (1)	{
					if (*itr == _T('\n'))	{	new_text += *itr++;	}
					else	{	break;	}
				}
				break;
			}
			// 終了／改行
			else if (_nta_check_end(*itr))	{	break;	}
			// その他の文字
			else	{	new_text += *itr++;		}
		}
		is_first_line = true;
	}
}

// 変換
void _ntf_insert_indent(TSTR& text, bool is_first_line, bool is_last_line, CTSTR& indent)
{
	text = _ntf_ret_insert_indent(text, is_first_line, is_last_line, indent);
}

// ****************************************************************
// １行化
// ================================================================

// 取得
TSTR _ntf_ret_change_to_line(CTSTR& text)
{
	TSTR::const_iterator itr = text.begin();
	TSTR new_text;
	bool hankaku_space_insert = true;

	while (1)	{
		// 行頭のタブ及び半角スペースを１つの半角スペースにする
		if (_nta_check_space(*itr))	{
			itr++;	_nta_jump_space(itr);
			// 半角スペース置き換え
			if (hankaku_space_insert == false && !_nta_check_end(*itr))	{
				new_text += _T(' ');
				hankaku_space_insert = true;
			}
		}

		while (1)	{
			// 終了
			if (*itr == _T('\0'))	{
				// 最後に追加した文字が半角スペースなら削除する
				if (!new_text.empty())	{
					if (*(new_text.end() - 1) == _T(' '))	{	new_text.erase(new_text.end() - 1, new_text.end());	}
				}
				new_text += *itr++;
				return new_text;
			}
			// 改行
			else if (*itr == _T('\n'))	{
				itr++;
				// 半角スペース置き換え
				if (hankaku_space_insert == false)	{
					new_text += _T(' ');
					hankaku_space_insert = true;
				}
				break;
			}
			else	{
				// 行末のタブ及び半角スペースを削除する
				if (_nta_check_space(*itr))	{
					TSTR space_str = _nta_get_space(itr);
					// 行末でないならそのまま残す
					if (!_nta_check_end(*itr))	{
						new_text += space_str;
					}
				}
				// その他の文字
				else	{	new_text += *itr++;		}

				hankaku_space_insert = false;
			}
		}
	}
}

// 変換
void _ntf_change_to_line(TSTR& text)
{
	text = _ntf_ret_change_to_line(text);
}

}

