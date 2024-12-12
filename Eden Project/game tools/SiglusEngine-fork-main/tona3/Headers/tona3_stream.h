#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

// ****************************************************************
// ストリーム基底クラス
// ================================================================
class C_stream_base
{
public:
	C_stream_base()	{	seek = 0;	}

	// ================================================================
	// バッファ操作
	// ----------------------------------------------------------------
	void	clear()				{	buffer.clear();	seek = 0;	}
	bool	empty()				{	return buffer.empty();		}
	int		size()				{	return (int)buffer.size();	}
	void	resize(int size)	{	buffer.resize(size);		}

	// シーク
	void	set_seek(int seek_ = 0)		{	seek = seek_;	}
	int		get_seek()					{	return seek;	}
	void	add_seek(int cnt)			{	seek += cnt;	}

	// バッファの取得
	BUFFER& get_buffer()		{	return buffer;				}
	BUFFER::iterator begin()	{	return buffer.begin();		}
	BUFFER::iterator end()		{	return buffer.end();		}
	BYTE* get()					{	return buffer.get();		}
	BYTE* get(int n)			{	return buffer.get() + n;	}
	BYTE* get_end()				{	return buffer.get_end();	}
	BYTE* get_cur_ptr()			{	return buffer.get() + seek;	}
	BYTE& operator [] (int n)	{	return buffer[n];			}

protected:
	BUFFER			buffer;
	int				seek;
};

// ****************************************************************
// ストリーム
// ================================================================
class C_stream : public C_stream_base
{
public:

	// ================================================================
	// データを追加
	// ----------------------------------------------------------------

	// データを追加
	template <typename TYPE>
	void push(TYPE data)
	{
		resize(seek + sizeof(TYPE));
		memcpy(buffer.get() + seek, &data, sizeof(data));
		seek += sizeof(TYPE);
	}

	// バイナリデータを追加
	void push(const void* data, int size)
	{
		if (size > 0)	{
			buffer.resize(seek + size);
			memcpy(buffer.get() + seek, data, size);
			seek += size;
		}
	}

	// 空データを追加
	void push_dirty(int size)
	{
		if (size > 0)	{
			buffer.resize(seek + size);
			seek += size;
		}
	}

	// 配列を追加
	template <typename TYPE>
	void push_array(const TYPE* data, int cnt)
	{
		push((const void *)data, sizeof(TYPE) * cnt); 
	}

	// 文字列を追加
	template <typename TYPE>
	void push_str(const TYPE& str)
	{
		typedef TYPE::value_type CHAR_TYPE;
		int size = (int)str.size() * sizeof(CHAR_TYPE);	// 書き込むサイズ
		push(str.c_str(), size);						// 書き込み
	}

	void push_str(const TCHAR* str)
	{
		push_str(TSTR(str));
	}

	// 文字列を追加（'\0' を追加）
	template <typename TYPE>
	void push_str_null(const TYPE& str)
	{
		push_str(str);					// 文字列を書き込む
		push<TYPE::value_type>(0);		// '\0' を書き込む
	}

	// 文字列を追加（長さを追加）
	template <typename TYPE>
	void push_str_len(const TYPE& str)
	{
		push<int>((int)str.size());		// 文字数を最初に積む
		push_str(str);					// 文字列を書き込む
	}

	// ================================================================
	// データを書き込む
	// ----------------------------------------------------------------
	void write(const void* data, int size)
	{
		if (size > 0)	{
			memcpy(buffer.get() + seek, data, size);
			seek += size;
		}
	}

	// ================================================================
	// データを取得
	// ----------------------------------------------------------------

	// データを取得（引数で取得）
	template <typename TYPE>
	void pop(TYPE& data)
	{
		data = *(TYPE *)(buffer.get() + seek);
		seek += sizeof(TYPE);
	}

	// データを取得（戻り値で取得）
	template <typename TYPE>
	TYPE pop_ret()
	{
		seek += sizeof(TYPE);
		return *(TYPE *)(buffer.get() + seek - sizeof(TYPE));
	}

	// バイナリデータを取得
	void pop(void* buf, int size)
	{
		if (size > 0)	{
			memcpy(buf, buffer.get() + seek, size);
			seek += size;
		}
	}

	// 配列を取得
	template <typename TYPE>
	void pop_array(TYPE* buf, int read_cnt)
	{
		pop((void *)buf, sizeof(TYPE) * read_cnt);
	}

	// 文字列を取得（引数で取得）
	template <typename TYPE>
	void pop_str_null(TYPE& str)
	{
		str = pop_str_null<TYPE>();
	}

	// 文字列を取得（戻り値で取得）
	template <typename TYPE>
	TYPE pop_str_null_ret()
	{
		typedef TYPE::value_type CHAR_TYPE;

		// '\0' を探す
		int cnt = 0;
		CHAR_TYPE* p;
		for (p = (CHAR_TYPE *)(buffer.get() + seek); *p != _T('\0'); p++, cnt++);

		// 文字列を作成
		TYPE str((CHAR_TYPE *)(buffer.get() + seek), p);
		seek += (cnt + 1) * sizeof(CHAR_TYPE);		// +1: '\0' の分

		return str;
	}

	// 文字列を取得（引数で取得）
	template <typename TYPE>
	void pop_str_len(TYPE& str)
	{
		str = pop_str_len_ret<TYPE>();
	}

	// 文字列を取得（戻り値で取得）
	template <typename TYPE>
	TYPE pop_str_len_ret()
	{
		typedef TYPE::value_type CHAR_TYPE;

		int size = pop_ret<int>() * sizeof(CHAR_TYPE);		// データ長を取得

		seek += size;
		return TYPE((CHAR_TYPE *)(buffer.get() + seek - size), (CHAR_TYPE *)(buffer.get() + seek));
	}
};

// ****************************************************************
// スタック
// ================================================================
class Cstack : public C_stream_base
{
public:

	// データを追加
	template <typename TYPE>
	void push(TYPE data)
	{
		buffer.resize(seek + sizeof(TYPE));
		*(TYPE *)(buffer.get() + seek) = data;
		seek += sizeof(TYPE);
	}

	// 文字列を追加
	void push_str_len(CTSTR& str)
	{
		int size = (int)str.size() * sizeof(TCHAR);
		buffer.resize(seek + size);
		memcpy(buffer.get() + seek, str.c_str(), size);
		seek += (int)size;

		push((int)str.size());		// 文字数を最後に積む
	}

	// バイナリ書き込み
	void push(const void* data, int size)
	{
		buffer.resize(seek + size);
		memcpy(buffer.get() + seek, data, size);
		seek += size;
	}

	// データを取得（引数に取得）
	template <typename TYPE>
	void pop(TYPE* data)
	{
		seek -= sizeof(TYPE);
		*data = *(TYPE *)(buffer.get() + seek);
	}

	// データを取得（戻り値で取得）
	template <typename TYPE>
	TYPE pop_ret()
	{
		seek -= sizeof(TYPE);
		return *(TYPE *)(buffer.get() + seek);
	}

	// 文字列を取得（引数で取得）
	void pop_str_len(TSTR& str)
	{
		int size = pop_ret<int>() * sizeof(TCHAR);		// データ長を取得

		seek -= size;
		str = TSTR((TCHAR *)(buffer.get() + seek), (TCHAR *)(buffer.get() + seek + size));
	}

	// 文字列を取得（戻り値で取得）
	TSTR pop_str_len_ret()
	{
		int size = pop_ret<int>() * sizeof(TCHAR);		// データ長を取得

		seek -= size;
		return TSTR((TCHAR *)(buffer.get() + seek), (TCHAR *)(buffer.get() + seek + size));
	}

	// 最初の要素を取得
	template <typename TYPE>
	TYPE front()
	{
		return *(TYPE *)(buffer.get());
	}
	// 最後の要素を取得
	template <typename TYPE>
	TYPE back()
	{
		return *(TYPE *)(buffer.get() + seek - sizeof(TYPE));
	}
	// 最後の文字列を取得
	TSTR back_str()
	{
		int size = back<int>() * sizeof(TCHAR);		// データ長を取得
		return TSTR((TCHAR *)(buffer.get() + seek - size - sizeof(int)), (TCHAR *)(buffer.get() + seek - sizeof(int)));
	}
};

}
