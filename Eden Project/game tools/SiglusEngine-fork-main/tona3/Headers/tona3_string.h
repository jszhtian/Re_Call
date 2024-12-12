#pragma		once

#include	"tona3_array.h"

namespace NT3
{

// ****************************************************************
// （テスト中。。。シングルトン作成補助クラス。）
// ================================================================
template <typename TYPE>
class C_singleton
{
public:
	static TYPE& get()
	{
		static TYPE instance;
		return instance;
	}
};

// ****************************************************************
// 文字管理クラス
//		シングルトン作成補助クラスを使ってみてます。
//		インスタンスが複数作れてしまいました。失敗です。
//		というわけで複数作らないでください。代替案を検討中。。。
// ================================================================
class Cmoji : public C_singleton<Cmoji>
{
public:
	Cmoji();

	enum	{	moji_count = 65536	};
	BYTE	moji_size_table[moji_count];
};

// ****************************************************************
// 文字操作系関数
// ================================================================
inline	bool	is_hankaku(char c)			{	return ( (c < 0x80) || (0xa0 <= c && c <= 0xdf) || (0xfe <= c && c <= 0xff) );	}
inline	bool	is_zenkaku(char c)			{	return ( (0x80 <= c) && (c < 0xa0 || 0xdf < c) && (c < 0xfe || 0xff < c) );		}
inline	bool	is_hankaku(wchar_t c)		{	return Cmoji::get().moji_size_table[c] == 1;	}
inline	bool	is_zenkaku(wchar_t c)		{	return Cmoji::get().moji_size_table[c] == 2;	}
inline	bool	is_num(char c)				{	return '0' <= c && c <= '9';	}
inline	bool	is_num(wchar_t c)			{	return L'0' <= c && c <= L'9';	}
inline	bool	is_small_alpha(char c)		{	return 'a' <= c && c <= 'z';	}
inline	bool	is_small_alpha(wchar_t c)	{	return L'a' <= c && c <= L'z';	}
inline	bool	is_large_alpha(char c)		{	return 'A' <= c && c <= 'Z';	}
inline	bool	is_large_alpha(wchar_t c)	{	return L'A' <= c && c <= L'Z';	}
inline	bool	is_alpha(char c)			{	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');		}
inline	bool	is_alpha(wchar_t c)			{	return (L'a' <= c && c <= L'z') || (L'A' <= c && c <= L'Z');	}

inline	bool	is_zenkaku_num(wchar_t c)			{	return (L'０' <= c && c <= L'９');	}
inline	bool	is_zenkaku_small_alpha(wchar_t c)	{	return (L'ａ' <= c && c <= L'ｚ');	}
inline	bool	is_zenkaku_large_alpha(wchar_t c)	{	return (L'Ａ' <= c && c <= L'Ｚ');	}
inline	bool	is_zenkaku_alpha(wchar_t c)			{	return (L'ａ' <= c && c <= L'ｚ') || (L'Ａ' <= c && c <= L'Ｚ');	}

// ****************************************************************
// ■文字列
// ================================================================
//		std::basic_string（string, wstring）を継承。
//		メンバを増やすために作ったけど、実は何も増えてません。
// ================================================================

template <typename char_type>
class basic_string_ex : public std::basic_string<char_type>
{
public:
	// 分かりやすくするため、型を定義しておきます。
	typedef std::basic_string<char_type>	base_type;		// 基底の文字列型
	typedef basic_string_ex<char_type>		this_type;		// 自身の文字列型

	// コンストラクタ
	// 基底クラスのコンストラクタ全てをオーバーライドする必要があります。
	basic_string_ex() : base_type()	{}
	basic_string_ex(const this_type& _Right) : base_type(_Right)	{}
	basic_string_ex(const this_type& _Right, size_type _Roff, size_type _Count = npos) : base_type(_Right, _Roff, _Count)	{}
	basic_string_ex(const char_type* _Ptr, size_type _Count) : base_type(_Ptr, _Count)	{}
	basic_string_ex(const char_type* _Ptr) : base_type(_Ptr)	{}
	basic_string_ex(size_type _Count, char_type _Ch) : base_type(_Count, _Ch)	{}
	// さらに、基底の型からの変換も可能にします。
	basic_string_ex(const base_type& X, size_type pos = 0, size_type n = npos) : base_type(X, pos, n)	{}
	// イテレータからのコンストラクタも作ります。
	template <typename _It>
	basic_string_ex(_It _First, _It _Last) : base_type(_First, _Last)	{}

	// ベース文字列を得る
	base_type& base()				{	return *this;	}
	const base_type& base()	const	{	return *this;	}
};

// ****************************************************************
// ■文字列型
// ================================================================
//		普段は TSTR 型を使います。
//		明示的にマルチバイト文字やワイド文字を扱いたいときのみ
//		MBSTR や WSTR を使ってください。
// ================================================================
typedef	basic_string_ex<char>		MBSTR;	// マルチバイト文字列
typedef	basic_string_ex<wchar_t>	WSTR;	// ワイド文字列
typedef	basic_string_ex<TCHAR>		TSTR;	// 文字列

// const な文字列型
typedef const MBSTR		CMBSTR;
typedef const WSTR		CWSTR;
typedef const TSTR		CTSTR;

// ****************************************************************
// 文字列クラス演算子
//		TSTR のメンバとして解決できないものを定義
// ================================================================
inline TSTR operator + (CTSTR& lop, CTSTR& rop)
{
	return operator + (lop.base(), rop.base());
}

inline TSTR operator + (CTSTR& lop, TCHAR* rop)
{
	return operator + (lop.base(), (const TCHAR *)rop);
}

inline TSTR operator + (CTSTR& lop, const TCHAR* rop)
{
	return operator + (lop.base(), rop);
}

inline TSTR operator + (TCHAR* lop, CTSTR& rop)
{
	return operator + ((const TCHAR *)lop, rop.base());
}

inline TSTR operator + (const TCHAR* lop, CTSTR& rop)
{
	return operator + (lop, rop.base());
}

inline TSTR operator * (CTSTR& lop, int rop)
{
	TSTR str;
	for (int n = 0; n < rop; n++)
		str += lop;

	return str;
}

// ****************************************************************
// テキストを行ごとに分割
// ================================================================
void separate_text_line(CTSTR& text, ARRAY<TSTR>& buf);		// テキストを行ごとに分割

// ****************************************************************
// 各種値を文字列に変換
// ================================================================
TSTR	tostr(const type_info& type);					// type_info → TSTR
TSTR	tostr(bool value);								// bool → TSTR（"true" / "false"）
TSTR	tostr_BOOL(BOOL value);							// BOOL → TSTR（"TRUE" / "FALSE"）
TSTR	tostr(char value);								// char → TSTR（整数値を戻す）
TSTR	tostr_moji(char value);							// char → TSTR（文字を戻す）
TSTR	tostr(unsigned char value);						// uchar → TSTR（整数値を戻す）
TSTR	tostr_moji(unsigned char value);				// uchar → TSTR（文字を戻す）
TSTR	tostr(wchar_t value);							// wchar_t → TSTR（整数値を戻す）
TSTR	tostr_moji(wchar_t value);						// wchar_t → TSTR（文字を戻す）
TSTR	tostr(int value, int base = 10);				// int → TSTR
TSTR	tostr(unsigned int value, int base = 10);		// uint → TSTR
TSTR	tostr(long value, int base = 10);				// long → TSTR
TSTR	tostr(unsigned long value, int base = 10);		// ulong → TSTR
TSTR	tostr(__int64 value, int base = 10);			// int64 → TSTR
TSTR	tostr(unsigned __int64 value, int base = 10);	// uint64 → TSTR
TSTR	tostr(float value);								// float → TSTR
TSTR	tostr(double value);							// double → TSTR
TSTR	tostr(const TCHAR* value);						// TCHAR* → TSTR
TSTR	tostr(CTSTR& value);							// TSTR → TSTR
TSTR	tostr_HANDLE(HANDLE value);						// HANDLE → TSTR

struct	C_point;	TSTR	tostr(C_point value);		// POINT → TSTR "(x,y)"
struct	C_point3;	TSTR	tostr(C_point3 value);		// POINT3 → TSTR "(x,y,z)"
struct	C_size;		TSTR	tostr(C_size value);		// SIZE → TSTR "(w,h)"
struct	C_rect;		TSTR	tostr(C_rect value);		// RECT → TSTR "(l,t)-(r,b)"
struct	C_rgb;		TSTR	tostr(C_rgb value);			// RGB → TSTR "(r,g,b)"
struct	C_argb;		TSTR	tostr(C_argb value);		// ARGB → TSTR "(a,r,g,b)"

template<typename TYPE>
TSTR tostr(ARRAY<TYPE>& list)						// ARRAY<TYPE> → TSTR "(1,2,3,...)"
{
	bool first = true;
	TSTR str = _T("(");
	for (ARRAY<TYPE>::iterator itr = list.begin(); itr != list.end(); ++itr)	{
		if (!first)	str += _T(", ");
		str += tostr(*itr);
		first = false;
	}
	str += _T(")");
	return str;
}

// ****************************************************************
// 文字列を各種値に変換
// ================================================================
bool	str_to_int(CTSTR& str, int* num);			// 引数で取得
int		str_to_int(CTSTR& str, int def);			// 初期値を与える
bool	str_to_int64(CTSTR& str, __int64* num);		// 引数で取得
__int64	str_to_int64(CTSTR& str, __int64 def);		// 初期値を与える
bool	str_to_float(CTSTR& str, float* num);		// 引数で取得
float	str_to_float(CTSTR& str, float def);		// 初期値を与える
TSTR	str_format(TSTR format, ...);

// ****************************************************************
// 文字列変換
// ================================================================
void	str_to_upper(TSTR& str);				// 大文字に変換する
TSTR	str_to_upper_ret(CTSTR& str);			// 大文字に変換する（戻り値で取得）
void	str_to_lower(TSTR& str);				// 小文字に変換する
TSTR	str_to_lower_ret(CTSTR& str);			// 大文字に変換する（戻り値で取得）
void	str_to_zenkaku(TSTR& str);				// 全角に変換する
TSTR	str_to_zenkaku_ret(CTSTR& str);			// 全角に変換する（戻り値で取得）
void	str_to_hankaku(TSTR& str);				// （※未実装）半角に変換する
TSTR	str_to_hankaku_ret(CTSTR& str);			// （※未実装）半角に変換する（戻り値で取得）

void	str_add_null(TSTR& str);										// 終端に '\0' を追加する
TSTR	str_add_null_ret(CTSTR& str);									// 終端に '\0' を追加する（戻り値で取得）
void	str_erase_moji(TSTR& str, TCHAR moji);							// 指定の文字を取り除く
TSTR	str_erase_moji_ret(CTSTR& str, TCHAR moji);						// 指定の文字を取り除く（戻り値で取得）
void	str_erase_tab(TSTR& str);										// タブを取り除く
TSTR	str_erase_tab_ret(CTSTR& str);									// タブを取り除く（戻り値で取得）
void	str_trim(TSTR& str, TCHAR moji);								// 両端の文字を取り除く
TSTR	str_trim_ret(TSTR str, TCHAR moji);								// 両端の文字を取り除く（戻り値で取得）
void	str_erase_double_quote_both_edge(TSTR& str);					// 両端のダブルコーテーションを削除
TSTR	str_erase_double_quote_both_edge_ret(CTSTR& str);				// 両端のダブルコーテーションを削除（戻り値で取得）
void	str_replace(TSTR& str, CTSTR& find, CTSTR& replace);			// 一部を置換する
TSTR	str_replace_ret(CTSTR& str, CTSTR& find, CTSTR& replace);		// 一部を置換する（戻り値で取得）

// ****************************************************************
// 文字コード変換
// ================================================================
MBSTR	WSTR_to_MBSTR(CWSTR& w_str);					// Unicode → Shift-JIS
MBSTR	WSTR_to_UTF8(CWSTR& w_str);						// Unicode → UTF-8
WSTR	MBSTR_to_WSTR(CMBSTR& mb_str);					// Shift-JIS → Unicode
MBSTR	MBSTR_to_UTF8(CMBSTR& mb_str);					// Shift-JIS → UTF-8
WSTR	UTF8_to_WSTR(CMBSTR& utf8_str);					// UTF-8 → Unicode
MBSTR	UTF8_to_MBSTR(CMBSTR& utf8_str);				// UTF-8 → Shift-JIS

WSTR	TSTR_to_WSTR(CTSTR& t_str);						// Ｔ文字列 → Unicode
MBSTR	TSTR_to_MBSTR(CTSTR& t_str);					// Ｔ文字列 → Shift-JIS
MBSTR	TSTR_to_UTF8(CTSTR& t_str);						// Ｔ文字列 → UTF-8
TSTR	WSTR_to_TSTR(CWSTR& w_str);						// Unicode → Ｔ文字列
TSTR	MBSTR_to_TSTR(CMBSTR& mb_str);					// Shift-JIS → Ｔ文字列
TSTR	MBSTR_to_TSTR(const char* mb_str, int cnt);		// Shift-JIS → Ｔ文字列
TSTR	MBSTR_to_TSTR(const BYTE* mb_str, int cnt);		// Shift-JIS → Ｔ文字列
TSTR	UTF8_to_TSTR(CMBSTR& utf8_str);					// UTF-8 → Ｔ文字列
TSTR	UTF8_to_TSTR(const char* utf8_str, int cnt);	// UTF-8 → Ｔ文字列
TSTR	UTF8_to_TSTR(const BYTE* utf8_str, int cnt);	// UTF-8 → Ｔ文字列

// ****************************************************************
// 文字列解析
// ================================================================
bool	str_analize_skip_separator(TSTR::const_iterator& itr, TSTR::const_iterator end);					// セパレータをスキップ
bool	str_analize_skip_moji(TSTR::const_iterator& itr, TSTR::const_iterator end, TCHAR moji);				// 指定の文字をスキップ
bool	str_analize_skip_moji_list(TSTR::const_iterator& itr, TSTR::const_iterator end, CTSTR& moji_list);	// 指定の文字をスキップ（複数指定）
bool	str_analize_check_moji(TSTR::const_iterator& itr, TSTR::const_iterator end, TCHAR moji);			// 文字を判定
bool	str_analize_check_str(TSTR::const_iterator& itr, TSTR::const_iterator end, CTSTR& str, bool step = true);				// 文字列を判定
bool	str_analize_get_moji(TSTR::const_iterator& itr, TSTR::const_iterator end, TCHAR* moji);				// 文字を取得
bool	str_analize_get_num(TSTR::const_iterator& itr, TSTR::const_iterator end, int* num);					// 直値を取得（符号を含まない）
bool	str_analize_get_num64(TSTR::const_iterator& itr, TSTR::const_iterator end, __int64* num);			// 直値を取得（符号を含まない）
bool	str_analize_get_integer(TSTR::const_iterator& itr, TSTR::const_iterator end, int* num);				// 整数を取得（符号を含む）
bool	str_analize_get_integer64(TSTR::const_iterator& itr, TSTR::const_iterator end, __int64* num64);		// 整数を取得（符号を含む）
bool	str_analize_get_float(TSTR::const_iterator& itr, TSTR::const_iterator end, float* num);				// 小数を取得（符号を含む）
bool	str_analize_get_word(TSTR::const_iterator& itr, TSTR::const_iterator end, TSTR& word);				// 単語を取得（_ または [A-Za-z] ではじまる、_ または [A-Za-z0-9] かならる語）
bool	str_analize_get_dquote_str(TSTR::const_iterator& itr, TSTR::const_iterator end, TSTR& str, TCHAR escape);	// ダブルコーテーションで囲まれた文字列を取得
bool	str_analize_get_line(TSTR::const_iterator& itr, TSTR::const_iterator end, TSTR& line);						// 行を取得
//bool	str_analize_get_format(TSTR::const_iterator& itr_, TSTR::const_iterator end, CTSTR format, ...);			// フォーマット指定で取得（%d, %s）

class C_str_analizer
{
public:
	C_str_analizer()		{}
	C_str_analizer(TSTR::const_iterator itr_begin, TSTR::const_iterator itr_end, CTSTR& separator_list = _T(" \t\n"))
	{
		set_string(itr_begin, itr_end);
		set_separator_list(separator_list);
	}

	// 設定
	void	set_string(TSTR::const_iterator itr_begin, TSTR::const_iterator itr_end);	// 解析する開始位置と終了位置を設定
	void	set_separator_list(CTSTR& separator_list);									// セパレータを設定（例：_T(" \t\n")）

	// 取得
	TSTR::const_iterator	get_cur()		{	return cur_itr;					}
	TSTR::const_iterator	get_end()		{	return end_itr;					}
	TSTR					get_nokori()	{	return TSTR(cur_itr, end_itr);	}

	// 解析
	bool	is_end();
	bool	skip_separator();
	bool	skip_moji(TCHAR moji);
	bool	skip_moji_list(CTSTR& moji_list);
	bool	check_moji(TCHAR moji);
	bool	check_moji_no_skip(TCHAR moji);
	bool	check_str(CTSTR& str);
	bool	get_moji(TCHAR* moji);
	bool	get_num(int* num);
	bool	get_num64(__int64* num64);
	bool	get_integer(int* num);
	bool	get_integer64(__int64* num64);
	bool	get_word(TSTR& word);
	bool	get_dquote_str(TSTR& str, TCHAR escape);
	bool	get_format(CTSTR format, ...);
	bool	get_line(TSTR& line);

protected:

	TSTR::const_iterator	cur_itr;
	TSTR::const_iterator	end_itr;
	TSTR					separator_list;

	// 状態保存
	struct C_state
	{
		TSTR::const_iterator	cur_itr;
		TSTR::const_iterator	end_itr;
	};
	C_state	save_state();
	void	load_state(C_state state);
	bool	FAIL(C_state state);
};

}
