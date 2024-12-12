#include	"tona3_pch.h"
#include	"tona3_string.h"
#include	"tona3_array.h"
#include	"tona3_util.h"
#include	"tona3_math.h"

namespace NT3
{

// ****************************************************************
// 文字
// ================================================================
Cmoji::Cmoji()
{
	int size;
	char mb_temp[4];
	for (int i = 0; i < moji_count; i++)	{
		wctomb_s(&size, mb_temp, 4, (wchar_t)i);
		moji_size_table[i] = (size == 2 ? 2 : 1);	// 全角=2、その他=1
	}
}

// ****************************************************************
// テキストを行ごとに分割
// ================================================================
void separate_text_line(CTSTR& text, ARRAY<TSTR>& line_list)
{
	int s_pos = 0;
	while (1)	{
		int e_pos = (int)text.find(_T('\n'), s_pos);

		// 改行が見つからなかった場合は最後までをデータとして終了
		if (e_pos == TSTR::npos)	{
			line_list.push_back(text.substr(s_pos));
			break;
		}

		// 改行が見つかった場合は改行までをデータとする
		line_list.push_back(text.substr(s_pos, e_pos - s_pos));
		s_pos = e_pos + 1;
	}
}

// ****************************************************************
// 各種値を文字列に変換
// ================================================================
TSTR tostr(const type_info& type)
{
	return MBSTR_to_TSTR(type.name());
}

TSTR tostr(bool value)
{
	return value ? _T("true") : _T("false");
}

TSTR tostr(char value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%d"), (int)value);
	return t_str;
}

TSTR tostr(unsigned char value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%d"), (int)value);
	return t_str;
}

TSTR tostr(wchar_t value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%d"), (int)value);
	return t_str;
}

TSTR tostr(int value, int base)
{
	TCHAR t_str[16];
	switch (base)	{
		case 16:	_stprintf_s(t_str, 16, _T("%#I32x"), value);	break;
		default:	_stprintf_s(t_str, 16, _T("%I32d"), value);		break;
	}
	return t_str;
}

TSTR tostr(unsigned int value, int base)
{
	TCHAR t_str[16];
	switch (base)	{
		case 16:	_stprintf_s(t_str, 16, _T("%#I32x"), value);	break;
		default:	_stprintf_s(t_str, 16, _T("%I32u"), value);		break;
	}
	return t_str;
}

TSTR tostr(long value, int base)
{
	TCHAR t_str[16];
	switch (base)	{
		case 16:	_stprintf_s(t_str, 16, _T("%#lx"), value);	break;
		default:	_stprintf_s(t_str, 16, _T("%ld"), value);	break;
	}
	return t_str;
}

TSTR tostr(unsigned long value, int base)
{
	TCHAR t_str[16];
	switch (base)	{
		case 16:		_stprintf_s(t_str, 16, _T("%#lx"), value);	break;
		default:		_stprintf_s(t_str, 16, _T("%lu"), value);	break;
	}
	return t_str;
}

TSTR tostr(__int64 value, int base)
{
	TCHAR t_str[32];
	switch (base)	{
		case 16:		_stprintf_s(t_str, 32, _T("%#I64x"), value);	break;
		default:		_stprintf_s(t_str, 32, _T("%I64d"), value);		break;
	}
	return t_str;
}

TSTR tostr(unsigned __int64 value, int base)
{
	TCHAR t_str[32];
	switch (base)	{
		case 16:		_stprintf_s(t_str, 32, _T("%#I64x"), value);	break;
		default:		_stprintf_s(t_str, 32, _T("%I64u"), value);		break;
	}
	return t_str;
}

TSTR tostr(float value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%f"), value);
	return t_str;
}

TSTR tostr(double value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%g"), value);
	return t_str;
}

TSTR tostr(const TCHAR* value)
{
	return value;
}

TSTR tostr(CTSTR& value)
{
	return value;
}

TSTR tostr_moji(char value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%c"), value);
	return t_str;
}

TSTR tostr_moji(unsigned char value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%c"), value);
	return t_str;
}

TSTR tostr_moji(wchar_t value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%c"), value);
	return t_str;
}

TSTR tostr_BOOL(BOOL value)
{
	return value != 0 ? _T("TRUE") : _T("FALSE");
}

TSTR tostr_HANDLE(HANDLE value)
{
	TCHAR t_str[32];
	_stprintf_s(t_str, 32, _T("%#I64x"), (unsigned __int64)value);
	return t_str;
}

TSTR tostr(C_point value)
{
	return _T("(") + tostr(value.x) + _T(", ") + tostr(value.y) + _T(")");
}

TSTR tostr(C_point3 value)
{
	return _T("(") + tostr(value.x) + _T(", ") + tostr(value.y) + _T(", ") + tostr(value.z) + _T(")");
}

TSTR tostr(C_size value)
{
	return _T("(") + tostr(value.cx) + _T(", ") + tostr(value.cy) + _T(")");
}

TSTR tostr(C_rect value)
{
	return _T("(") + tostr(value.left) + _T(", ") + tostr(value.top) + _T(") - (") + tostr(value.right) + _T(", ") + tostr(value.bottom) + _T(")");
}

TSTR tostr(C_rgb value)
{
	return _T("(") + tostr(value.r) + _T(", ") + tostr(value.g) + _T(", ") + tostr(value.b) + _T(")");
}

TSTR tostr(C_argb value)
{
	return _T("(") + tostr(value.a) + _T(", ") + tostr(value.r) + _T(", ") + tostr(value.g) + _T(", ") + tostr(value.b) + _T(")");
}

// ****************************************************************
// 文字列を数値に変換
// ================================================================
bool str_to_int(CTSTR& str, int* num)
{
	return str_analize_get_integer(str.begin(), str.end(), num);
}

int str_to_int(CTSTR& str, int def)
{
	int num;
	if (str_to_int(str, &num))		return num;		// 成功時は取得できた値を返す
	else							return def;		// 失敗時はデフォルト値を返す
}

bool str_to_int64(CTSTR& str, __int64* num)
{
	return str_analize_get_integer64(str.begin(), str.end(), num);
}

__int64 str_to_int64(CTSTR& str, __int64 def)
{
	__int64 num;
	if (str_to_int64(str, &num))	return num;		// 成功時は取得できた値を返す
	else							return def;		// 失敗時はデフォルト値を返す
}

bool str_to_float(CTSTR& str, float* num)
{
	return str_analize_get_float(str.begin(), str.end(), num);
}

float str_to_float(CTSTR& str, float def)
{
	float num;
	if (str_to_float(str, &num))	return num;		// 成功時は取得できた値を返す
	else							return def;		// 失敗時はデフォルト値を返す
}

// ****************************************************************
// フォーマット
// ================================================================
TSTR str_format(TSTR format, ...)
{
	TCHAR buf[1024];

	va_list arg;
	va_start(arg, format);
	_vstprintf_s(buf, 1024, format.c_str(), arg);
	va_end(arg);

	return TSTR(buf);
}

// ****************************************************************
// 大文字に変換する
// ================================================================
void str_to_upper(TSTR& str)
{
	int len = (int)str.length();
	for (int p = 0; p < len; p++)	{
		if ('a' <= str[p] && str[p] <= 'z')
			str[p] += 'A' - 'a';
	}
}

// ****************************************************************
// 大文字に変換する（戻り値で取得）
// ================================================================
TSTR str_to_upper_ret(CTSTR& str)
{
	TSTR ret_str = str;
	str_to_upper(ret_str);
	return ret_str;
}

// ****************************************************************
// 小文字に変換する
// ================================================================
void str_to_lower(TSTR& str)
{
	int len = (int)str.length();
	for (int p = 0; p < len; p++)	{
		if ('A' <= str[p] && str[p] <= 'Z')
			str[p] += 'a' - 'A';
	}
}

// ****************************************************************
// 小文字に変換する（戻り値で取得）
// ================================================================
TSTR str_to_lower_ret(CTSTR& str)
{
	TSTR ret_str = str;
	str_to_lower(ret_str);
	return ret_str;
}

// ****************************************************************
// 全角に変換する
// ================================================================
void str_to_zenkaku(TSTR& str)
{
	str = str_to_zenkaku_ret(str);
}

// ****************************************************************
// 全角に変換する（戻り値で取得）
// ================================================================
TSTR str_to_zenkaku_ret(CTSTR& str)
{
	TSTR ret_str;

//		_T("!\"#$%&'()*+,-./")
//		_T("0123456789")
//		_T(":;<=>?@")
//		_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
//		_T("[\\]^_`")
//		_T("abcdefghijklmnopqrstuvwxyz")
//		_T("{|}~");

	static TCHAR* alpha = {
		_T("！”＃＄％＆’（）＊＋，－．／")
		_T("０１２３４５６７８９")
		_T("：；＜＝＞？＠")
		_T("ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ")
		_T("［￥］＾＿‘")
		_T("ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ")
		_T("｛｜｝～")
	};

//		_T("｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰ")
//		_T("ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉ")
//		_T("ﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ")

	static TCHAR* kana = {
		_T("。「」、・ヲァィゥェォャュョッー")
		_T("アイウエオカキクケコサシスセソタチツテトナニヌネノ")
		_T("ハヒフヘホマミムメモヤユヨラリルレロワン")
	};

//		_T("ｳﾞｶﾞｷﾞｸﾞｹﾞｺﾞｻﾞｼﾞｽﾞｾﾞｿﾞﾀﾞﾁﾞﾂﾞﾃﾞﾄﾞ")

	static TCHAR* kana_d = {
		_T("・・・・・・・・・・・・・・・・")
		_T("・・ヴ・・ガギグゲゴザジズゼゾダヂヅデド・・・・・")
		_T("バビブベボ・・・・・・・・・・・・・・・")
	};

//		_T("ﾊﾟﾋﾟﾌﾟﾍﾟﾎﾟ")

	static TCHAR* kana_h = {
		_T("・・・・・・・・・・・・・・・・")
		_T("・・・・・・・・・・・・・・・・・・・・・・・・・")
		_T("パピプペポ・・・・・・・・・・・・・・・")
	};

	for (TSTR::const_iterator itr = str.begin(); itr != str.end(); ++itr)	{

		if (false);

		else if (*itr == _T(' '))	{
			ret_str += _T('　');
		}
		else if (_T('!') <= *itr && *itr <= _T('~'))	{
			ret_str += *(alpha + *itr - _T('!'));
		}
		else if (_T('｡') <= *itr && *itr <= _T('ﾝ'))	{
			// 濁点／半濁点処理
			if (itr + 1 != str.end() && *(itr + 1) == _T('ﾞ'))	{
				ret_str += *(kana_d + *itr - _T('｡'));		++itr;
			}
			else if (itr + 1 != str.end() && *(itr + 1) == _T('ﾟ'))	{
				ret_str += *(kana_h + *itr - _T('｡'));		++itr;
			}
			else	{
				ret_str += *(kana + *itr - _T('｡'));
			}
		}
		else	{
			ret_str += *itr;
		}
	}

	return ret_str;
}

// ****************************************************************
// 終端に '\0' を追加
// ================================================================
void str_add_null(TSTR& str)
{
	str += _T('\0');
}

// ****************************************************************
// 終端に '\0' を追加（戻り値で取得）
// ================================================================
TSTR str_add_null_ret(CTSTR& str)
{
	TSTR ret_str = str;
	str_add_null(ret_str);
	return ret_str;
}

// ****************************************************************
// 指定の文字を取り除く
// ================================================================
void str_erase_moji(TSTR& str, TCHAR moji)
{
	str = str_erase_moji_ret(str, moji);
}

// ****************************************************************
// 指定の文字を取り除く（戻り値で取得）
// ================================================================
TSTR str_erase_moji_ret(CTSTR& str, TCHAR moji)
{
	TSTR ret_str;

	for (TSTR::const_iterator itr = str.begin(); itr != str.end(); ++itr)	{
		if (*itr != moji)
			ret_str += *itr;
	}

	return ret_str;
}

// ****************************************************************
// タブを取り除く
// ================================================================
void str_erase_tab(TSTR& str)
{
	str_erase_moji(str, _T('\t'));
}

// ****************************************************************
// タブを取り除く（戻り値で取得）
// ================================================================
TSTR str_erase_tab_ret(CTSTR& str)
{
	return str_erase_moji_ret(str, _T('\t'));
}

// ****************************************************************
// 両端の文字を取り除く
// ================================================================
void str_trim(TSTR& str, TCHAR moji)
{
	while (str.length() > 0 && str[0] == moji)
		str = str.substr(1);

	while (str.length() > 0 && str[str.length() - 1] == moji)
		str = str.substr(0, str.length() - 1);

}

// ****************************************************************
// 両端の文字を取り除く（戻り値で取得）
// ================================================================
TSTR str_trim_ret(TSTR str, TCHAR moji)
{
	str_trim(str, moji);
	return str;
}

// ****************************************************************
// 両端のダブルコーテーションを削除する
// ================================================================
void str_erase_double_quote_both_edge(TSTR& str)
{
	// 先頭の " を取り除く
	if (str.length() > 0)	{
		if (str[0] == '"')
			str = TSTR(str, 1);
	}
	// 末尾の " を取り除く
	if (str.length() > 0)	{
		if (str[str.length() - 1] == '"')
			str = TSTR(str, 0, str.length() - 1);
	}
}

// ****************************************************************
// 両端のダブルコーテーションを削除する（戻り値で取得）
// ================================================================
TSTR str_erase_double_quote_both_edge_ret(CTSTR& str)
{
	TSTR new_str = str;
	str_erase_double_quote_both_edge(new_str);
	return new_str;
}

// ****************************************************************
// 文字列を置換
// ================================================================
void str_replace(TSTR& str, CTSTR& find, CTSTR& replace)
{
	// find を検索し、見つかれば new_str に置換
	for (int pos = 0; (pos = (int)str.find(find, pos)) != TSTR::npos; )	{
		str = TSTR(str.substr(0, pos)) + replace + TSTR(str.substr(pos + find.length()));
		pos += (int)replace.length();
	}
}

// ****************************************************************
// 文字列を置換（戻り値で取得）
// ================================================================
TSTR str_replace_ret(CTSTR& str, CTSTR& find, CTSTR& replace)
{
	TSTR ret_str = str;
	str_replace(ret_str, find, replace);
	return ret_str;
}

// ****************************************************************
// WSTR を MBSTR に変換
// ================================================================
MBSTR WSTR_to_MBSTR(CWSTR& w_str, int code_page)
{
	if (w_str.empty())
		return "";	// マルチバイト文字列で空文字列を返す

	ARRAY<char> buf;
	MBSTR mb_str;

	// 必要なバッファサイズを取得
	int size = WideCharToMultiByte(code_page, 0, w_str.c_str(), -1, NULL, 0, NULL, NULL);
	if (size > 0)	{
		// 領域を確保して変換
		buf.resize(size + 1);
		WideCharToMultiByte(code_page, 0, w_str.c_str(), -1, buf.get(), size, NULL, NULL);
		buf[size] = '\0';	// '\0' をつけてから MBSTR に代入
		mb_str = buf.get();	// '\0' がないと、どこまでコピーすればいいか分からない
	}

	return mb_str;
}

// ****************************************************************
// MBSTR を WSTR に変換
// ================================================================
WSTR MBSTR_to_WSTR(CMBSTR& mb_str, int code_page)
{
	if (mb_str.empty())
		return L"";	// ワイド文字列で空文字列を返す

	ARRAY<wchar_t> buf;
	WSTR w_str;

	// 必要なバッファサイズを取得
	int size = MultiByteToWideChar(code_page, 0, mb_str.c_str(), -1, NULL, 0);
	if (size > 0)	{
		// 領域を確保して変換
		buf.resize(size + 1);
		MultiByteToWideChar(code_page, 0, mb_str.c_str(), -1, buf.get(), size);
		buf[size] = '\0';	// '\0' をつけてから WSTR に代入
		w_str = buf.get();	// '\0' がないと、どこまでコピーすればいいか分からない
	}

	return w_str;
}

// ****************************************************************
// WSTR を各種文字コードに変換
// ================================================================
MBSTR WSTR_to_MBSTR(CWSTR& w_str)		// Unicode -> Shift-JIS
{
	return WSTR_to_MBSTR(w_str, CP_ACP);
}

MBSTR WSTR_to_UTF8(CWSTR& w_str)		// Unicode -> UTF-8
{
	return WSTR_to_MBSTR(w_str, CP_UTF8);
}

// ****************************************************************
// MBSTR を各種文字コードに変換
// ================================================================
WSTR MBSTR_to_WSTR(CMBSTR& mb_str)		// Shift-JIS -> Unicode
{
	return MBSTR_to_WSTR(mb_str, CP_ACP);
}

MBSTR MBSTR_to_UTF8(CMBSTR& mb_str)		// Shift-JIS -> UTF-8
{
	return WSTR_to_MBSTR(MBSTR_to_WSTR(mb_str, CP_ACP), CP_UTF8);
}

// ****************************************************************
// UTF8 を各種文字コードに変換
// ================================================================
WSTR UTF8_to_WSTR(CMBSTR& utf8_str)		// UTF-8 -> Unicode
{
	return MBSTR_to_WSTR(utf8_str, CP_UTF8);
}

MBSTR UTF8_to_MBSTR(CMBSTR& utf8_str)	// UTF-8 -> Shift-JIS
{
	return WSTR_to_MBSTR(MBSTR_to_WSTR(utf8_str, CP_UTF8), CP_ACP);
}

// ****************************************************************
// TSTR を WSTR に変換
// ================================================================
WSTR TSTR_to_WSTR(CTSTR& t_str)
{
#if defined(_UNICODE) || defined(UNICODE)
	// ワイド文字列 → ワイド文字列
	return t_str;
#else
	// マルチバイト文字列 → ワイド文字列
	return MBTR_to_WSTR(t_str);
#endif
}

// ****************************************************************
// TSTR を MBSTR に変換
// ================================================================
MBSTR TSTR_to_MBSTR(CTSTR& t_str)
{
#if defined(_UNICODE) || defined(UNICODE)
	// ワイド文字列 → マルチバイト文字列
	return WSTR_to_MBSTR(t_str);
#else
	// マルチバイト文字列 → マルチバイト文字列
	return t_str;
#endif
}

// ****************************************************************
// TSTR を UTF-8 に変換
// ================================================================
MBSTR TSTR_to_UTF8(CTSTR& t_str)
{
#if defined(_UNICODE) || defined(UNICODE)
	// ワイド文字列 → マルチバイト文字列
	return WSTR_to_UTF8(t_str);
#else
	// マルチバイト文字列 → マルチバイト文字列
	return MBSTR_to_UTF8(t_str);
#endif
}

// ****************************************************************
// WSTR を TSTR に変換
// ================================================================
TSTR WSTR_to_TSTR(CWSTR& w_str)
{
#if defined(_UNICODE) || defined(UNICODE)
	// ワイド文字列 → ワイド文字列
	return w_str;
#else
	// ワイド文字列 → マルチバイト文字列
	return WSTR_to_MBSTR(w_str);
#endif
}

// ****************************************************************
// MBSTR を TSTR に変換
// ================================================================
TSTR MBSTR_to_TSTR(CMBSTR& mb_str)
{
#if defined(_UNICODE) || defined(UNICODE)
	// マルチバイト文字列 → ワイド文字列
	return MBSTR_to_WSTR(mb_str);
#else
	// マルチバイト文字列 → マルチバイト文字列
	return mb_str;
#endif
}

TSTR MBSTR_to_TSTR(const char* mb_str, int cnt)
{
	if (cnt <= 0)	return _T("");

	return MBSTR_to_TSTR(MBSTR(mb_str, cnt));
}

TSTR MBSTR_to_TSTR(const BYTE* mb_str, int cnt)
{
	if (cnt <= 0)	return _T("");

	return MBSTR_to_TSTR(MBSTR((const char *)mb_str, cnt));
}

// ****************************************************************
// UTF-8 を TSTR に変換
// ================================================================
TSTR UTF8_to_TSTR(CMBSTR& utf8_str)
{
#if defined(_UNICODE) || defined(UNICODE)
	// UTF-8 → Unicode
	return UTF8_to_WSTR(utf8_str);
#else
	// UTF-8 → Shift-JIS
	return UTF8_to_MBSTR(utf8_str);
#endif
}

TSTR UTF8_to_TSTR(const char* utf8_str, int cnt)
{
	if (cnt <= 0)	return _T("");

	return UTF8_to_TSTR(MBSTR(utf8_str, cnt));
}

TSTR UTF8_to_TSTR(const BYTE* utf8_str, int cnt)
{
	if (cnt <= 0)	return _T("");

	return UTF8_to_TSTR(MBSTR((const char *)utf8_str, cnt));
}

// ****************************************************************
// 文字列解析：セパレータをスキップ（削除予定）
// ================================================================
bool str_analize_skip_separator(TSTR::const_iterator& itr, TSTR::const_iterator end)
{
	return str_analize_skip_moji_list(itr, end, _T(" \t\n"));
}

// ****************************************************************
// 文字列解析：特定の文字をスキップ（削除予定）
// ================================================================
bool str_analize_skip_moji(TSTR::const_iterator& itr, TSTR::const_iterator end, TCHAR moji)
{
	for (; itr != end; ++itr)	{
		if (*itr != moji)
			break;
	}

	return itr != end;
}

// ****************************************************************
// 文字列解析：特定の文字リストをスキップ
// ================================================================
bool str_analize_skip_moji_list(TSTR::const_iterator& itr, TSTR::const_iterator end, CTSTR& moji_list)
{
	for (; itr != end; ++itr)	{
		if (moji_list.find(*itr) == TSTR::npos)
			break;
	}

	return itr != end;
}

// ****************************************************************
// 文字列解析：文字列から文字を判定
// ================================================================
bool str_analize_check_moji(TSTR::const_iterator& itr_, TSTR::const_iterator end, TCHAR moji)
{
	TSTR::const_iterator itr = itr_;

	// 存在判定
	if (itr == end)			return false;
	// 文字判定
	if (*itr++ != moji)		return false;

	itr_ = itr;
	return true;
}

// ****************************************************************
// 文字列解析：文字列から文字列を判定
// ================================================================
bool str_analize_check_str(TSTR::const_iterator& itr_, TSTR::const_iterator end, CTSTR& str, bool step)
{
	TSTR::const_iterator itr = itr_;
	TSTR::const_iterator itr_src = str.begin();

	// １文字ずつ比較していく（これが一番速かった）
	for (; itr_src != str.end(); ++itr_src, ++itr)	{
		if (itr == end || *itr != *itr_src)
			return false;
	}

	if (step)
		itr_ = itr;

	return true;
}

// ****************************************************************
// 文字列解析：文字列から文字を取得
// ================================================================
bool str_analize_get_moji(TSTR::const_iterator& itr_, TSTR::const_iterator end, TCHAR* moji)
{
	TSTR::const_iterator itr = itr_;

	// 初期値を設定
	if (moji)	*moji = 0;
	// 存在判定
	if (itr == end)		return false;
	// 文字の取得
	if (moji)	*moji = *itr++;

	itr_ = itr;
	return true;
}

// ****************************************************************
// 文字列解析：文字列から数値を取得
// ================================================================
bool str_analize_get_num(TSTR::const_iterator& itr_, TSTR::const_iterator end, int* num_)
{
	TSTR::const_iterator itr = itr_;

	// 成功判定
	if (itr == end || !is_num(*itr))	return false;

	// 数値判定
	int num = 0;
	for (; itr != end && is_num(*itr); ++itr)
		num = num * 10 + (*itr - _T('0'));

	// 成功！
	if (num_)		*num_ = num;
	itr_ = itr;		return true;
}
bool str_analize_get_num64(TSTR::const_iterator& itr_, TSTR::const_iterator end, __int64* num64_)
{
	TSTR::const_iterator itr = itr_;

	// 成功判定
	if (itr == end || !is_num(*itr))	return false;

	// 数値判定
	__int64 num64 = 0;
	for (; itr != end && is_num(*itr); ++itr)
		num64 = num64 * 10 + (*itr - _T('0'));

	// 成功！
	if (num64_)		*num64_ = num64;
	itr_ = itr;		return true;
}

// ****************************************************************
// 文字列解析：文字列から整数を取得
// ================================================================
bool str_analize_get_integer(TSTR::const_iterator& itr_, TSTR::const_iterator end, int* num_)
{
	TSTR::const_iterator itr = itr_;

	int sign = 1;
	int num = 0;

	// 符号
	if (false);
	else if (str_analize_check_moji(itr, end, _T('+')))		sign = 1;
	else if (str_analize_check_moji(itr, end, _T('-')))		sign = -1;
	// 数値
	if (!str_analize_get_num(itr, end, &num))	return false;

	// 成功！
	if (num_)		*num_ = num * sign;
	itr_ = itr;		return true;
}
bool str_analize_get_integer64(TSTR::const_iterator& itr_, TSTR::const_iterator end, __int64* num64_)
{
	TSTR::const_iterator itr = itr_;

	int sign = 1;
	__int64 num64 = 0;

	// 符号
	if (false);
	else if (str_analize_check_moji(itr, end, _T('+')))		sign = 1;
	else if (str_analize_check_moji(itr, end, _T('-')))		sign = -1;
	// 数値
	if (!str_analize_get_num64(itr, end, &num64))	return false;

	// 成功！
	if (num64_)		*num64_ = num64 * sign;
	itr_ = itr;		return true;
}

// ****************************************************************
// 文字列解析：文字列から小数を取得
// ================================================================
bool str_analize_get_float(TSTR::const_iterator& itr_, TSTR::const_iterator end, float* num_)
{
	TSTR::const_iterator itr = itr_;

	int sign = 1;
	int num_u = 0;			// 整数部分
	int num_d = 0;			// 小数部分（整数）
	int num_d_waru = 1;		// 小数部分の桁数^10
	float num_d_f = 0.0f;	// 小数部分（小数）

	// 符号
	if (false);
	else if (str_analize_check_moji(itr, end, _T('+')))		sign = 1;
	else if (str_analize_check_moji(itr, end, _T('-')))		sign = -1;

	// 整数部分
	if (!str_analize_get_num(itr, end, &num_u))
		return false;

	// 小数点
	if (itr == end || !str_analize_check_moji(itr, end, _T('.')))
		return false;

	// 成功判定
	if (itr == end || !is_num(*itr))
		return false;

	// 数値判定（桁数もカウントする。0001→とかは4桁なので注意！）
	for (; itr != end && is_num(*itr); ++itr)	{
		num_d = num_d * 10 + (*itr - _T('0'));
		num_d_waru *= 10;
		if (num_d_waru >= 1000000)	// これ以上は誤差なので解析しない（オーバーフローするし…）
			break;
	}
	num_d_f = (float)num_d / num_d_waru;

	// 成功！
	if (num_)		*num_ = num_d_f * sign;
	itr_ = itr;		return true;
}

// ****************************************************************
// 文字列解析：文字列から単語を取得
// ================================================================
bool str_analize_get_word(TSTR::const_iterator& itr_, TSTR::const_iterator end, TSTR& word)
{
	TSTR::const_iterator itr = itr_;

	// １文字目は英字または '_'
	if (itr == end || (!is_alpha(*itr) && *itr != _T('_')))
		return false;

	++itr;

	// ２文字目以降は英数字または '_'
	for (; itr != end; ++itr)	{
		if (!is_alpha(*itr) && !is_num(*itr) && *itr != _T('_'))
			break;
	}

	word = TSTR(itr_, itr);
	itr_ = itr;
	return true;
}

// ****************************************************************
// 文字列解析：文字列からコーテーション文字列を取得
// ================================================================
bool str_analize_get_dquote_str(TSTR::const_iterator& itr_, TSTR::const_iterator end, TSTR& str_, TCHAR escape)
{
	TSTR::const_iterator itr = itr_;

	TCHAR moji;

	// " を取得
	if (!str_analize_check_moji(itr, end, _T('"')))
		return false;

	enum STATE
	{
		STATE_NORMAL,
		STATE_ESCAPE,
	};
	STATE state = STATE_NORMAL;

	// 終端の " が来るまで文字列取得
	TSTR str;
	while (1)	{
		if (!str_analize_get_moji(itr, end, &moji))
			return false;

		if (state == STATE_NORMAL)	{
			if (moji == escape)	{
				state = STATE_ESCAPE;
			}
			else if (moji == _T('"'))	{
				break;
			}
			else	{
				str += moji;
			}
		}
		else if (state == STATE_ESCAPE)	{
			str += moji;
			state = STATE_NORMAL;
		}
	}

	itr_ = itr;
	str_ = str;
	return true;
}

// ****************************************************************
// 文字列解析：文字列から行を取得
// ================================================================
bool str_analize_get_line(TSTR::const_iterator& itr, TSTR::const_iterator end, TSTR& line)
{
	TSTR::const_iterator line_end = std::find(itr, end, _T('\n'));

	line = TSTR(itr, line_end);
	return true;
}




// ****************************************************************
// 文字列アナライザ：状態を保存・復元
// ================================================================
C_str_analizer::C_state C_str_analizer::save_state()
{
	C_state state;
	state.cur_itr = cur_itr;
	state.end_itr = end_itr;

	return state;
}

void C_str_analizer::load_state(C_state state)
{
	cur_itr = state.cur_itr;
	end_itr = state.end_itr;
}

bool C_str_analizer::FAIL(C_state state)
{
	load_state(state);
	return false;
}


void C_str_analizer::set_string(TSTR::const_iterator itr_begin, TSTR::const_iterator itr_end)
{
	cur_itr = itr_begin;
	end_itr = itr_end;
}

void C_str_analizer::set_separator_list(CTSTR& separator_list_)
{
	separator_list = separator_list_;
}

bool C_str_analizer::skip_separator()
{
	C_state state = save_state();

	if (!str_analize_skip_moji_list(cur_itr, end_itr, separator_list))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::is_end()
{
	C_state state = save_state();

	str_analize_skip_moji_list(cur_itr, end_itr, separator_list);
	if (cur_itr != end_itr)
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::check_moji(TCHAR moji)
{
	C_state state = save_state();

	skip_separator();
	if (!str_analize_check_moji(cur_itr, end_itr, moji))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::check_moji_no_skip(TCHAR moji)
{
	C_state state = save_state();

	if (!str_analize_check_moji(cur_itr, end_itr, moji))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::check_str(CTSTR &str)
{
	C_state state = save_state();

	skip_separator();
	if (!str_analize_check_str(cur_itr, end_itr, str))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::get_moji(TCHAR *moji)
{
	C_state state = save_state();

	skip_separator();
	if (!str_analize_get_moji(cur_itr, end_itr, moji))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::get_num(int* num)
{
	C_state state = save_state();

	skip_separator();
	if (!str_analize_get_num(cur_itr, end_itr, num))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::get_num64(__int64* num64)
{
	C_state state = save_state();

	skip_separator();
	if (!str_analize_get_num64(cur_itr, end_itr, num64))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::get_integer(int *integer)
{
	C_state state = save_state();

	int sign = 1;	// 符号（初期値はプラス）
	int num = 0;	// 数値

	// 符号
	if (check_moji(_T('+')))		sign = 1;	// プラス
	else if (check_moji(_T('-')))	sign = -1;	// マイナス

	// 数値
	if (!get_num(&num))
		return FAIL(state);

	// 成功！
	*integer = sign * num;
	return true;
}

bool C_str_analizer::get_integer64(__int64 *integer64)
{
	C_state state = save_state();

	int sign = 1;	// 符号（初期値はプラス）
	__int64 num64 = 0;	// 数値

	// 符号
	if (check_moji(_T('+')))		sign = 1;	// プラス
	else if (check_moji(_T('-')))	sign = -1;	// マイナス

	// 数値
	if (!get_num64(&num64))
		return FAIL(state);

	// 成功！
	*integer64 = sign * num64;
	return true;
}

bool C_str_analizer::get_word(TSTR& word)
{
	C_state state = save_state();

	skip_separator();
	if (!str_analize_get_word(cur_itr, end_itr, word))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::get_dquote_str(TSTR &str, TCHAR escape)
{
	C_state state = save_state();

	skip_separator();
	if (!str_analize_get_dquote_str(cur_itr, end_itr, str, escape))
		return FAIL(state);

	// 成功！
	return true;
}

bool C_str_analizer::get_line(TSTR& line)
{
	C_state state = save_state();

	if (!str_analize_get_line(cur_itr, end_itr, line))
		return FAIL(state);

	// 成功！
	return true;
}

// ****************************************************************
// 文字列解析：文字列から情報を format で取得
// ================================================================
bool C_str_analizer::get_format(CTSTR format, ...)
{
	C_state state = save_state();

	va_list arg;
	va_start(arg, format);

	int num;
	__int64 num64;
	TCHAR moji;
	TSTR str;

	// フォーマット文字列の解析
	C_str_analizer fmt(format.begin(), format.end(), _T(""));		// セパレータなし
	while (!fmt.is_end())	{
		
		if (fmt.get_moji(&moji))	{

			if (false);
			else if (moji == _T(' '))	{
				// セパレータをスキップ
				if (!skip_separator())		return FAIL(state);
			}
			else if (moji == _T('%'))	{
				if (fmt.get_moji(&moji))	{
					if (false);
					else if (moji == _T('d'))	{
						// 整数を取得
						if (!get_integer(&num))		return FAIL(state);
						int* p = va_arg(arg, int*);
						*p = num;
					}
					else if (moji == _T('s'))	{
						// 文字列を取得
						if (!get_dquote_str(str, _T('\\')))	return FAIL(state);
						*va_arg(arg, TSTR*) = str;
					}
					else if (moji == _T('6'))	{
						if (fmt.get_moji(&moji))	{
							if (false);
							else if (moji == _T('4'))	{
								if (fmt.get_moji(&moji))	{
									if (false);
									else if (moji == _T('d'))	{
										// 整数を取得
										if (!get_integer64(&num64))		return FAIL(state);
										__int64* p = va_arg(arg, __int64*);
										*p = num64;
									}
								}
							}
						}
					}
				}
			}
			else	{
				// 文字を判定
				if (!check_moji_no_skip(moji))	return FAIL(state);
			}
		}
	}

	va_end(arg);

	// 成功！
	return true;
}

}
