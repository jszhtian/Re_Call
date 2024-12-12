#include	"pch.h"
#include	"namalib_sys_text_analyze.h"

namespace Nnama
{

// ****************************************************************
// 行制御
// ================================================================

// 取得
TSTR _nta_get_line(TSTR::const_iterator& itr)
{
	TSTR str;
	while (1)	{
		if (_nta_check_end(*itr))	{	break;				}		// 終了／改行
		else						{	str += *itr++;		}		// その他の文字
	}
	return str;
}

// 取得のみ（イテレータを進めない）
TSTR _nta_get_only_line(TSTR::const_iterator& start_itr)
{
	TSTR::const_iterator itr = start_itr;
	return _nta_get_line(itr);
}

// ジャンプ
void _nta_jump_line(TSTR::const_iterator& itr)
{
	while (1)	{
		if (_nta_check_end(*itr))	{	break;	}		// 終了／改行
		else						{	itr++;	}		// その他の文字
	}
}

// ****************************************************************
// 空白文字制御
// ================================================================

#define		SPACE_CHECK(c)			(c == _T(' ') || c == _T('\t'))

// 判定
bool _nta_check_space(TCHAR moji)
{
	if (SPACE_CHECK(moji))	{	return true;	}
	return false;
}

// 取得
TSTR _nta_get_space(TSTR::const_iterator& itr)
{
	TSTR str;
	while (1)	{
		if (SPACE_CHECK(*itr))	{	str += *itr++;	}	// 空白文字
		else					{	break;			}	// その他の文字
	}
	return str;
}

// 取得のみ（イテレータを進めない）
TSTR _nta_get_only_space(TSTR::const_iterator& start_itr)
{
	TSTR::const_iterator itr = start_itr;
	return _nta_get_space(itr);
}

// ジャンプ
void _nta_jump_space(TSTR::const_iterator& itr)
{
	while (1)	{
		if (SPACE_CHECK(*itr))	{	itr++;	}	// 空白文字
		else					{	break;	}	// その他の文字
	}
}

// 現在位置より「前」の空白文を取得（現在位置の文字は判定しない）
TSTR _nta_get_space_forward(CTSTR& str, TSTR::const_iterator itr)
{
	if (str.empty())	{	return TSTR();	}
	TSTR::const_iterator top = str.begin();
	if (itr == top)		{	return TSTR();	}
	itr--;
	TSTR::const_iterator space_itr = str.end();
	while (1)	{
		if (_nta_check_end(*itr))			{	break;	}	// 終了／改行
		else if (_nta_check_space(*itr))	{				// 空白文字
			space_itr = itr;	if (itr == top)	{	break;	}	itr--;
		}
		else								{	break;	}	// その他
	}

	if (space_itr != str.end())	{	return _nta_get_space(space_itr);	}
	return TSTR();
}

// 後ろから空白文を削除
void _nta_delete_space_backward(TSTR& str)
{
	if (str.empty())	{	return;	}
	TSTR::iterator itr = str.end() - 1;
	TSTR::iterator top = str.begin();
	TSTR::iterator space_itr = str.end();
	while (1)	{
		if (_nta_check_space(*itr))	{						// 空白文字
			space_itr = itr;	if (itr == top)	{	break;	}	itr--;
		}
		else									{	break;	}	// その他
	}
	if (space_itr != str.end())	{	str.erase(space_itr, str.end());	}
}

// ****************************************************************
// 区切り文字制御
// ================================================================

#define		KUGIRI_CHECK(c)			(c == _T(' ') || c == _T('\t') || c == _T('\n') || c == _T('\0'))

// 判定
bool _nta_check_kugiri(TCHAR moji)
{
	if (KUGIRI_CHECK(moji))	{	return true;	}
	return false;
}

// ****************************************************************
// 終端文字制御
// ================================================================

#define		END_CHECK(c)			(c == _T('\n') || c == _T('\0'))

// 判定
bool _nta_check_end(TCHAR moji)
{
	if (END_CHECK(moji))	{	return true;	}
	return false;
}

// ****************************************************************
// トークン制御
// ================================================================

#define		HANKAKU_TOKEN_CHECK(c)			(is_num(c) || is_alpha(c) || c == _T('@') || c == _T('_') || c == _T('*'))
#define		ZENKAKU_TOKEN_CHECK(c)			(is_zenkaku(c))

// 取得（半角のみ）
TSTR _nta_get_token_hankaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (HANKAKU_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// トークン文字
		else							{	break;				}	// その他の文字
	}
	return token;
}

// 取得（全角のみ）
TSTR _nta_get_token_zenkaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// トークン文字
		else							{	break;				}	// その他の文字
	}
	return token;
}

// 取得
TSTR _nta_get_token(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_TOKEN_CHECK(*itr) || HANKAKU_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// トークン文字
		else														{	break;				}	// その他の文字
	}
	return token;
}

// 取得（数値用）
TSTR _nta_get_token_for_num(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (HANKAKU_TOKEN_CHECK(*itr) || *itr == _T('-'))	{	token += *itr++;	}	// トークン文字
		else												{	break;				}	// その他の文字
	}
	return token;
}

// 判定
bool _nta_check_token(TSTR::const_iterator& start_itr, CTSTR token)
{
	if (token.empty())	{	return false;	}

	TSTR::const_iterator itr = start_itr;
	for (TSTR::const_iterator token_itr = token.begin(); token_itr != token.end(); ++token_itr)	{
		if (*itr != *token_itr)	{	return false;	}
		itr++;
	}

	start_itr = itr;	// イテレータを進める
	return true;
}

// 判定のみ（イテレータを進めない）
bool _nta_check_only_token(TSTR::const_iterator& start_itr, CTSTR token)
{
	if (token.empty())	{	return false;	}
	TSTR::const_iterator itr = start_itr;
	for (TSTR::const_iterator token_itr = token.begin(); token_itr != token.end(); ++token_itr)	{
		if (*itr != *token_itr)	{	return false;	}
		itr++;
	}
	return true;
}

// ****************************************************************
// ＠トークン制御
// ================================================================

#define		HANKAKU_AT_TOKEN_CHECK(c)			(is_num(c) || is_alpha(c) || c == _T('_'))
#define		ZENKAKU_AT_TOKEN_CHECK(c)			(c != _T('＠') && is_zenkaku(c))

// 取得（半角のみ）
TSTR _nta_get_at_token_hankaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (HANKAKU_AT_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// トークン文字
		else								{	break;				}	// その他の文字
	}
	return token;
}

// 取得（全角のみ）
TSTR _nta_get_at_token_zenkaku_only(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_AT_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// トークン文字
		else								{	break;				}	// その他の文字
	}
	return token;
}

// 取得
TSTR _nta_get_at_token(TSTR::const_iterator& itr)
{
	TSTR token;
	while (1)	{
		if (ZENKAKU_AT_TOKEN_CHECK(*itr) || HANKAKU_AT_TOKEN_CHECK(*itr))	{	token += *itr++;	}	// トークン文字
		else																{	break;				}	// その他の文字
	}
	return token;
}

// ****************************************************************
// セパレート制御
// ================================================================

// 判定
bool _nta_check_separate(TCHAR moji)
{
	if (ZENKAKU_TOKEN_CHECK(moji) || HANKAKU_TOKEN_CHECK(moji))	{	return false;	}	// トークン文字
	return true;
}

// 文字列の最初の文字を判定
bool _nta_check_separate_first_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_separate(*(str.begin())))	{	return true;	}
	return false;
}

// 文字列の最後の文字を判定
bool _nta_check_separate_last_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_separate(*(str.end() - 1)))	{	return true;	}
	return false;
}

// ****************************************************************
// ＠セパレート制御
// ================================================================

// 判定
bool _nta_check_at_separate(TCHAR moji)
{
	if (ZENKAKU_AT_TOKEN_CHECK(moji) || HANKAKU_AT_TOKEN_CHECK(moji))	{	return false;	}	// トークン文字
	return true;
}

// 文字列の最初の文字を判定
bool _nta_check_at_separate_first_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_at_separate(*(str.begin())))	{	return true;	}
	return false;
}

// 文字列の最後の文字を判定
bool _nta_check_at_separate_last_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	else if (_nta_check_at_separate(*(str.end() - 1)))	{	return true;	}
	return false;
}

// ****************************************************************
// 文字列制御
// ================================================================

// 取得：ダブルクオーテーション
TSTR _nta_get_str_double_quotation(TSTR::const_iterator& itr, TCHAR escape_moji)
{
	TSTR token;
	if (*itr == _T('\"'))	{
		itr++;
		while (1)	{
			if (END_CHECK(*itr))			{	break;	}
			else if (*itr == _T('\"'))		{	itr++;	break;	}
			else if (*itr == escape_moji)	{	itr++;	token += *itr++;	}
			else							{	token += *itr++;	}
		}
	}
	return token;
}

// 取得：指定の文字が見つかるまで
TSTR _nta_get_str_find_moji(TSTR::const_iterator& itr, TCHAR target_moji, TCHAR escape_moji)
{
	TSTR token;
	while (1)	{
		if (END_CHECK(*itr))			{	break;	}
		else if (*itr == target_moji)	{	break;	}
		else if (*itr == escape_moji)	{	itr++;	token += *itr++;	}
		else							{	token += *itr++;	}
	}
	return token;
}

// ****************************************************************
// 連続する指定文字の制御
// ================================================================

// 指定文字の数を取得
int _nta_get_moji_cnt(TSTR::const_iterator itr, TCHAR moji)
{
	int moji_cnt = 0;
	while (1)	{
		if (*itr != moji)	{	break;	}
		itr++;	moji_cnt++;
	}
	return moji_cnt;
}

// 指定文字を指定数ジャンプする
void _nta_jump_moji(TSTR::const_iterator& itr, TCHAR moji, int cnt)
{
	if (cnt <= 0)	{	return;	}
	int moji_cnt = 0;
	while (1)	{
		if (*itr != moji)	{	break;	}
		itr++;	moji_cnt++;
		if (moji_cnt == cnt)	{	break;	}
	}
}

// ****************************************************************
// 空白行制御
// ================================================================

// 判定
bool _nta_check_space_line(TSTR::const_iterator itr)
{
	while (1)	{
		if (_nta_check_end(*itr))			{	return true;	}	// 終了／改行
		else if (_nta_check_space(*itr))	{	itr++;			}	// 空白文字
		else								{	return false;	}	// その他の文字
	}
}

// 現在位置より「前」は空白行か判定（現在位置の文字は判定しない）
bool _nta_check_space_line_forward(CTSTR& str, TSTR::const_iterator itr)
{
	if (str.empty())	{	return true;	}
	TSTR::const_iterator top = str.begin();
	if (itr == top)		{	return true;	}
	itr--;
	while (1)	{
		if (_nta_check_end(*itr))			{	return true;	}	// 終了／改行
		else if (_nta_check_space(*itr))	{						// 空白文字
			if (itr == top)	{	return true;	}	itr--;
		}
		else								{	return false;	}	// その他
	}
	return true;
}

// 現在位置より「後」は空白行か判定（現在位置の文字も判定する）
bool _nta_check_space_line_backward(CTSTR& str, TSTR::const_iterator itr)
{
	if (str.empty())	{	return true;	}
	TSTR::const_iterator end = str.end();
	while (itr != end)	{
		if (_nta_check_end(*itr))			{	return true;	}	// 終了／改行
		else if (_nta_check_space(*itr))	{	itr++;			}	// 空白文字
		else								{	return false;	}	// その他
	}
	return true;
}

// ****************************************************************
// 改行制御
// ================================================================

// 判定
bool _nta_check_return_contain(CTSTR& str)
{
	if (str.empty())	{	return false;	}
	TSTR::const_iterator itr = str.begin();
	TSTR::const_iterator end = str.end();
	while (itr != end)	{
		if (*itr == _T('\n'))	{	return true;	}	// 改行
		itr++;
	}
	return false;
}

// 最後の文字が改行か判定
bool _nta_check_return_last_moji(CTSTR& str)
{
	if (str.empty())	{	return true;	}
	TSTR::const_iterator itr = str.end() - 1;
	if (*itr == _T('\n'))	{	return true;	}
	return false;
}


}
