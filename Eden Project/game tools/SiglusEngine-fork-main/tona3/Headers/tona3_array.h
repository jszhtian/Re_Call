#pragma		once

namespace NT3
{

// ****************************************************************
// ■可変長配列 ARRAY
// ================================================================
//		std::vector を継承。
//		色々とややこしいことをしてますが、std::vector との違いは、
//		get() メソッドでバッファへのポインタが取得できることだけです。
// ================================================================
//		ex. ARRAY<char> buf(256);	// サイズ 256 の char 型配列
//		ex. HogeAPI(buf.get());		// WinAPI などに char * を渡す
// ================================================================
#define	ARRAY	vector_ex
#define	BUFFER	ARRAY<BYTE>

template <typename data_type, class allocator_type = std::allocator<data_type> >
class vector_ex : public std::vector<data_type, allocator_type>
{
public:
	// 分かりやすくするため、型を定義しておきます。
	typedef std::vector<data_type, allocator_type>	base_type;		// 基底の型
	typedef vector_ex<data_type, allocator_type>	this_type;		// 自身の型

	// コンストラクタ
	// 基底クラスのコンストラクタ全てをオーバーライドする必要があります。
	// アロケータを指定できるコンストラクタはオーバーライドしていません。必要になったら作ります。
	vector_ex()	: vector()	{}
	explicit vector_ex(size_type _Count) : vector(_Count)	{}
		// 引数が１つのコンストラクタは、変換コンストラクタとして働き、暗黙の型変換を許してしまいます。
		// これを避けるために使っているのが explicit です。
	vector_ex(size_type _Count, const data_type& _Val) : vector(_Count, _Val)	{}
	vector_ex(const this_type& _Right) : vector(_Right)	{}
	vector_ex(size_type _Count, const data_type& _Val, const allocator_type& _Al) : vector(_Count, _Val, _Al)	{}

	// イテレータからのコンストラクタ
	template<class _Iter> vector_ex(_Iter _First, _Iter _Last) : vector(_First, _Last)	{}

	// バッファへのポインタの取得 get()
	// vector のバッファは連続していることが保証されています。
	// バッファが空の時は NULL を返します。
	data_type* get()	{
		return empty() ? NULL : &base_type::operator[](0);
	}
	const data_type* get() const	{
		return empty() ? NULL : &base_type::operator[](0);
	}
	data_type* get(int pos)	{
		return pos >= size() ? NULL : &base_type::operator[](pos);
	}
	const data_type* get(int pos) const	{
		return pos >= size() ? NULL : &base_type::operator[](pos);
	}
	data_type* get_end()	{
		return empty() ? NULL : &base_type::operator[](0) + size();
	}
	const data_type* get_end() const	{
		return empty() ? NULL : &base_type::operator[](0) + size();
	}
};


}
