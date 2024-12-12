#pragma		once

#include	"tona3_array.h"

namespace NT3
{

// ****************************************************************
// �i�e�X�g���B�B�B�V���O���g���쐬�⏕�N���X�B�j
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
// �����Ǘ��N���X
//		�V���O���g���쐬�⏕�N���X���g���Ă݂Ă܂��B
//		�C���X�^���X���������Ă��܂��܂����B���s�ł��B
//		�Ƃ����킯�ŕ������Ȃ��ł��������B��ֈĂ��������B�B�B
// ================================================================
class Cmoji : public C_singleton<Cmoji>
{
public:
	Cmoji();

	enum	{	moji_count = 65536	};
	BYTE	moji_size_table[moji_count];
};

// ****************************************************************
// ��������n�֐�
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

inline	bool	is_zenkaku_num(wchar_t c)			{	return (L'�O' <= c && c <= L'�X');	}
inline	bool	is_zenkaku_small_alpha(wchar_t c)	{	return (L'��' <= c && c <= L'��');	}
inline	bool	is_zenkaku_large_alpha(wchar_t c)	{	return (L'�`' <= c && c <= L'�y');	}
inline	bool	is_zenkaku_alpha(wchar_t c)			{	return (L'��' <= c && c <= L'��') || (L'�`' <= c && c <= L'�y');	}

// ****************************************************************
// ��������
// ================================================================
//		std::basic_string�istring, wstring�j���p���B
//		�����o�𑝂₷���߂ɍ�������ǁA���͉��������Ă܂���B
// ================================================================

template <typename char_type>
class basic_string_ex : public std::basic_string<char_type>
{
public:
	// ������₷�����邽�߁A�^���`���Ă����܂��B
	typedef std::basic_string<char_type>	base_type;		// ���̕�����^
	typedef basic_string_ex<char_type>		this_type;		// ���g�̕�����^

	// �R���X�g���N�^
	// ���N���X�̃R���X�g���N�^�S�Ă��I�[�o�[���C�h����K�v������܂��B
	basic_string_ex() : base_type()	{}
	basic_string_ex(const this_type& _Right) : base_type(_Right)	{}
	basic_string_ex(const this_type& _Right, size_type _Roff, size_type _Count = npos) : base_type(_Right, _Roff, _Count)	{}
	basic_string_ex(const char_type* _Ptr, size_type _Count) : base_type(_Ptr, _Count)	{}
	basic_string_ex(const char_type* _Ptr) : base_type(_Ptr)	{}
	basic_string_ex(size_type _Count, char_type _Ch) : base_type(_Count, _Ch)	{}
	// ����ɁA���̌^����̕ϊ����\�ɂ��܂��B
	basic_string_ex(const base_type& X, size_type pos = 0, size_type n = npos) : base_type(X, pos, n)	{}
	// �C�e���[�^����̃R���X�g���N�^�����܂��B
	template <typename _It>
	basic_string_ex(_It _First, _It _Last) : base_type(_First, _Last)	{}

	// �x�[�X������𓾂�
	base_type& base()				{	return *this;	}
	const base_type& base()	const	{	return *this;	}
};

// ****************************************************************
// ��������^
// ================================================================
//		���i�� TSTR �^���g���܂��B
//		�����I�Ƀ}���`�o�C�g�����⃏�C�h���������������Ƃ��̂�
//		MBSTR �� WSTR ���g���Ă��������B
// ================================================================
typedef	basic_string_ex<char>		MBSTR;	// �}���`�o�C�g������
typedef	basic_string_ex<wchar_t>	WSTR;	// ���C�h������
typedef	basic_string_ex<TCHAR>		TSTR;	// ������

// const �ȕ�����^
typedef const MBSTR		CMBSTR;
typedef const WSTR		CWSTR;
typedef const TSTR		CTSTR;

// ****************************************************************
// ������N���X���Z�q
//		TSTR �̃����o�Ƃ��ĉ����ł��Ȃ����̂��`
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
// �e�L�X�g���s���Ƃɕ���
// ================================================================
void separate_text_line(CTSTR& text, ARRAY<TSTR>& buf);		// �e�L�X�g���s���Ƃɕ���

// ****************************************************************
// �e��l�𕶎���ɕϊ�
// ================================================================
TSTR	tostr(const type_info& type);					// type_info �� TSTR
TSTR	tostr(bool value);								// bool �� TSTR�i"true" / "false"�j
TSTR	tostr_BOOL(BOOL value);							// BOOL �� TSTR�i"TRUE" / "FALSE"�j
TSTR	tostr(char value);								// char �� TSTR�i�����l��߂��j
TSTR	tostr_moji(char value);							// char �� TSTR�i������߂��j
TSTR	tostr(unsigned char value);						// uchar �� TSTR�i�����l��߂��j
TSTR	tostr_moji(unsigned char value);				// uchar �� TSTR�i������߂��j
TSTR	tostr(wchar_t value);							// wchar_t �� TSTR�i�����l��߂��j
TSTR	tostr_moji(wchar_t value);						// wchar_t �� TSTR�i������߂��j
TSTR	tostr(int value, int base = 10);				// int �� TSTR
TSTR	tostr(unsigned int value, int base = 10);		// uint �� TSTR
TSTR	tostr(long value, int base = 10);				// long �� TSTR
TSTR	tostr(unsigned long value, int base = 10);		// ulong �� TSTR
TSTR	tostr(__int64 value, int base = 10);			// int64 �� TSTR
TSTR	tostr(unsigned __int64 value, int base = 10);	// uint64 �� TSTR
TSTR	tostr(float value);								// float �� TSTR
TSTR	tostr(double value);							// double �� TSTR
TSTR	tostr(const TCHAR* value);						// TCHAR* �� TSTR
TSTR	tostr(CTSTR& value);							// TSTR �� TSTR
TSTR	tostr_HANDLE(HANDLE value);						// HANDLE �� TSTR

struct	C_point;	TSTR	tostr(C_point value);		// POINT �� TSTR "(x,y)"
struct	C_point3;	TSTR	tostr(C_point3 value);		// POINT3 �� TSTR "(x,y,z)"
struct	C_size;		TSTR	tostr(C_size value);		// SIZE �� TSTR "(w,h)"
struct	C_rect;		TSTR	tostr(C_rect value);		// RECT �� TSTR "(l,t)-(r,b)"
struct	C_rgb;		TSTR	tostr(C_rgb value);			// RGB �� TSTR "(r,g,b)"
struct	C_argb;		TSTR	tostr(C_argb value);		// ARGB �� TSTR "(a,r,g,b)"

template<typename TYPE>
TSTR tostr(ARRAY<TYPE>& list)						// ARRAY<TYPE> �� TSTR "(1,2,3,...)"
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
// ��������e��l�ɕϊ�
// ================================================================
bool	str_to_int(CTSTR& str, int* num);			// �����Ŏ擾
int		str_to_int(CTSTR& str, int def);			// �����l��^����
bool	str_to_int64(CTSTR& str, __int64* num);		// �����Ŏ擾
__int64	str_to_int64(CTSTR& str, __int64 def);		// �����l��^����
bool	str_to_float(CTSTR& str, float* num);		// �����Ŏ擾
float	str_to_float(CTSTR& str, float def);		// �����l��^����
TSTR	str_format(TSTR format, ...);

// ****************************************************************
// ������ϊ�
// ================================================================
void	str_to_upper(TSTR& str);				// �啶���ɕϊ�����
TSTR	str_to_upper_ret(CTSTR& str);			// �啶���ɕϊ�����i�߂�l�Ŏ擾�j
void	str_to_lower(TSTR& str);				// �������ɕϊ�����
TSTR	str_to_lower_ret(CTSTR& str);			// �啶���ɕϊ�����i�߂�l�Ŏ擾�j
void	str_to_zenkaku(TSTR& str);				// �S�p�ɕϊ�����
TSTR	str_to_zenkaku_ret(CTSTR& str);			// �S�p�ɕϊ�����i�߂�l�Ŏ擾�j
void	str_to_hankaku(TSTR& str);				// �i���������j���p�ɕϊ�����
TSTR	str_to_hankaku_ret(CTSTR& str);			// �i���������j���p�ɕϊ�����i�߂�l�Ŏ擾�j

void	str_add_null(TSTR& str);										// �I�[�� '\0' ��ǉ�����
TSTR	str_add_null_ret(CTSTR& str);									// �I�[�� '\0' ��ǉ�����i�߂�l�Ŏ擾�j
void	str_erase_moji(TSTR& str, TCHAR moji);							// �w��̕�������菜��
TSTR	str_erase_moji_ret(CTSTR& str, TCHAR moji);						// �w��̕�������菜���i�߂�l�Ŏ擾�j
void	str_erase_tab(TSTR& str);										// �^�u����菜��
TSTR	str_erase_tab_ret(CTSTR& str);									// �^�u����菜���i�߂�l�Ŏ擾�j
void	str_trim(TSTR& str, TCHAR moji);								// ���[�̕�������菜��
TSTR	str_trim_ret(TSTR str, TCHAR moji);								// ���[�̕�������菜���i�߂�l�Ŏ擾�j
void	str_erase_double_quote_both_edge(TSTR& str);					// ���[�̃_�u���R�[�e�[�V�������폜
TSTR	str_erase_double_quote_both_edge_ret(CTSTR& str);				// ���[�̃_�u���R�[�e�[�V�������폜�i�߂�l�Ŏ擾�j
void	str_replace(TSTR& str, CTSTR& find, CTSTR& replace);			// �ꕔ��u������
TSTR	str_replace_ret(CTSTR& str, CTSTR& find, CTSTR& replace);		// �ꕔ��u������i�߂�l�Ŏ擾�j

// ****************************************************************
// �����R�[�h�ϊ�
// ================================================================
MBSTR	WSTR_to_MBSTR(CWSTR& w_str);					// Unicode �� Shift-JIS
MBSTR	WSTR_to_UTF8(CWSTR& w_str);						// Unicode �� UTF-8
WSTR	MBSTR_to_WSTR(CMBSTR& mb_str);					// Shift-JIS �� Unicode
MBSTR	MBSTR_to_UTF8(CMBSTR& mb_str);					// Shift-JIS �� UTF-8
WSTR	UTF8_to_WSTR(CMBSTR& utf8_str);					// UTF-8 �� Unicode
MBSTR	UTF8_to_MBSTR(CMBSTR& utf8_str);				// UTF-8 �� Shift-JIS

WSTR	TSTR_to_WSTR(CTSTR& t_str);						// �s������ �� Unicode
MBSTR	TSTR_to_MBSTR(CTSTR& t_str);					// �s������ �� Shift-JIS
MBSTR	TSTR_to_UTF8(CTSTR& t_str);						// �s������ �� UTF-8
TSTR	WSTR_to_TSTR(CWSTR& w_str);						// Unicode �� �s������
TSTR	MBSTR_to_TSTR(CMBSTR& mb_str);					// Shift-JIS �� �s������
TSTR	MBSTR_to_TSTR(const char* mb_str, int cnt);		// Shift-JIS �� �s������
TSTR	MBSTR_to_TSTR(const BYTE* mb_str, int cnt);		// Shift-JIS �� �s������
TSTR	UTF8_to_TSTR(CMBSTR& utf8_str);					// UTF-8 �� �s������
TSTR	UTF8_to_TSTR(const char* utf8_str, int cnt);	// UTF-8 �� �s������
TSTR	UTF8_to_TSTR(const BYTE* utf8_str, int cnt);	// UTF-8 �� �s������

// ****************************************************************
// ��������
// ================================================================
bool	str_analize_skip_separator(TSTR::const_iterator& itr, TSTR::const_iterator end);					// �Z�p���[�^���X�L�b�v
bool	str_analize_skip_moji(TSTR::const_iterator& itr, TSTR::const_iterator end, TCHAR moji);				// �w��̕������X�L�b�v
bool	str_analize_skip_moji_list(TSTR::const_iterator& itr, TSTR::const_iterator end, CTSTR& moji_list);	// �w��̕������X�L�b�v�i�����w��j
bool	str_analize_check_moji(TSTR::const_iterator& itr, TSTR::const_iterator end, TCHAR moji);			// �����𔻒�
bool	str_analize_check_str(TSTR::const_iterator& itr, TSTR::const_iterator end, CTSTR& str, bool step = true);				// ������𔻒�
bool	str_analize_get_moji(TSTR::const_iterator& itr, TSTR::const_iterator end, TCHAR* moji);				// �������擾
bool	str_analize_get_num(TSTR::const_iterator& itr, TSTR::const_iterator end, int* num);					// ���l���擾�i�������܂܂Ȃ��j
bool	str_analize_get_num64(TSTR::const_iterator& itr, TSTR::const_iterator end, __int64* num);			// ���l���擾�i�������܂܂Ȃ��j
bool	str_analize_get_integer(TSTR::const_iterator& itr, TSTR::const_iterator end, int* num);				// �������擾�i�������܂ށj
bool	str_analize_get_integer64(TSTR::const_iterator& itr, TSTR::const_iterator end, __int64* num64);		// �������擾�i�������܂ށj
bool	str_analize_get_float(TSTR::const_iterator& itr, TSTR::const_iterator end, float* num);				// �������擾�i�������܂ށj
bool	str_analize_get_word(TSTR::const_iterator& itr, TSTR::const_iterator end, TSTR& word);				// �P����擾�i_ �܂��� [A-Za-z] �ł͂��܂�A_ �܂��� [A-Za-z0-9] ���Ȃ���j
bool	str_analize_get_dquote_str(TSTR::const_iterator& itr, TSTR::const_iterator end, TSTR& str, TCHAR escape);	// �_�u���R�[�e�[�V�����ň͂܂ꂽ��������擾
bool	str_analize_get_line(TSTR::const_iterator& itr, TSTR::const_iterator end, TSTR& line);						// �s���擾
//bool	str_analize_get_format(TSTR::const_iterator& itr_, TSTR::const_iterator end, CTSTR format, ...);			// �t�H�[�}�b�g�w��Ŏ擾�i%d, %s�j

class C_str_analizer
{
public:
	C_str_analizer()		{}
	C_str_analizer(TSTR::const_iterator itr_begin, TSTR::const_iterator itr_end, CTSTR& separator_list = _T(" \t\n"))
	{
		set_string(itr_begin, itr_end);
		set_separator_list(separator_list);
	}

	// �ݒ�
	void	set_string(TSTR::const_iterator itr_begin, TSTR::const_iterator itr_end);	// ��͂���J�n�ʒu�ƏI���ʒu��ݒ�
	void	set_separator_list(CTSTR& separator_list);									// �Z�p���[�^��ݒ�i��F_T(" \t\n")�j

	// �擾
	TSTR::const_iterator	get_cur()		{	return cur_itr;					}
	TSTR::const_iterator	get_end()		{	return end_itr;					}
	TSTR					get_nokori()	{	return TSTR(cur_itr, end_itr);	}

	// ���
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

	// ��ԕۑ�
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
