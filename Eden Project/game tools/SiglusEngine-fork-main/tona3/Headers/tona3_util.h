#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

	// ****************************************************************
	// メッセージボックス
	// ================================================================

	// メッセージボックス
	int message_box(HWND h_wnd, CTSTR& msg, CTSTR& caption, UINT type);
	int message_box(CTSTR& msg, CTSTR& caption, UINT type);

	// エラーボックス
	int error_box(CTSTR& err_msg, UINT type = MB_OK);

	// 警告ボックス
	int warning_box(CTSTR& msg, UINT type = MB_OK);
	int warning_box(CTSTR& msg, CTSTR& caption, UINT type = MB_OK);

	// 情報ボックス
	int info_box(CTSTR& msg, UINT type = MB_OK);
	int info_box(CTSTR& msg, CTSTR& caption, UINT type = MB_OK);

	// ****************************************************************
	// ＴＰＣ暗号
	// ================================================================
	void tpc_angou_encode(BYTE* data, int size);						// ＴＰＣ暗号の暗号化
	void tpc_angou_decode(BYTE* data, int size);						// ＴＰＣ暗号の復号化

	// ****************************************************************
	// エンコード
	// ================================================================
	TSTR url_encode(CTSTR& value);
	TSTR base64_encode(ARRAY<BYTE>& buf);
	TSTR hmac_base64_encode(CTSTR& key, CTSTR& data);

	// ****************************************************************
	// コンピュータＩＤを取得
	// ================================================================
	DWORD get_cpu_id();
	DWORD get_computer_id();

	// ****************************************************************
	// ＯＳのバージョンを文字列で取得
	// ================================================================
	TSTR get_os_version_str();

	// ****************************************************************
	// 現在の日付を文字列で取得
	// ================================================================
	TSTR get_sys_time_str();

	// ****************************************************************
	// UUID を文字列で取得
	// ================================================================
	TSTR get_uuid();

	// ****************************************************************
	// ミューテックス
	// ================================================================
	class C_mutex
	{
	public:
		C_mutex()
		{
			m_h_mutex = NULL;
		}

		~C_mutex()
		{
			release_mutex();
		}

		bool create_mutex(CTSTR& name)
		{
			release_mutex();

			m_h_mutex = ::CreateMutex(NULL, TRUE, name.c_str());
			if (!m_h_mutex)
				return false;

			return true;
		}

		void release_mutex()
		{
			if (m_h_mutex)	{
				::CloseHandle(m_h_mutex);
				m_h_mutex = NULL;
			}
		}

	private:
		HANDLE	m_h_mutex;
	};

	// ****************************************************************
	// bool型構造体
	// ================================================================
	struct S_bool
	{
		bool value;
	};

	// ****************************************************************
	// 型変換
	// ================================================================
	int		bool_to_int(bool value);

	// ****************************************************************
	// C 配列のサイズを取得
	// ================================================================
	#define		sizeof_array(list)		(sizeof(list) / sizeof(list[0]))

	// ****************************************************************
	// メモリ操作
	// ================================================================
	void	fill_memory(void* buf, int cnt, BYTE data);					// メモリを指定した値で埋める（1 byte 単位）
	void	fill_memory_2(void* buf, int cnt, WORD data);				// メモリを指定した値で埋める（2 byte 単位）
	void	fill_memory_4(void* buf, int cnt, DWORD data);				// メモリを指定した値で埋める（4 byte 単位）
	void	fill_memory_x(void* buf, int size, int cnt, void* data);	// メモリを指定した値で埋める（x byte 単位）

	// ****************************************************************
	// ビット操作
	// ================================================================
	template <typename TYPE>
	class C_bit
	{
	public:
		C_bit()	{}

		C_bit(TYPE* src, int pos, int cnt)
		{
			assign(src, pos, cnt);
		}

		// アドレスをセット
		void assign(TYPE* src_, int pos_, int cnt_)
		{
			src = src_;
			size = sizeof(TYPE) * 8;
			l_cnt = pos_;
			r_cnt = size - pos_ - cnt_;
			cnt = cnt_;
			max = (1 << cnt_) - 1;
		}

		// int 型として値を取り出す
		operator TYPE() const
		{
			return (TYPE)((*src) << l_cnt) >> (l_cnt + r_cnt);
		}

		// 0クリア
		void clear()
		{
			TYPE left = ((1 << l_cnt) - 1) << (cnt + r_cnt);
			TYPE right = (1 << r_cnt) - 1;
			*src &= left | right;
		}

		// 1フィル
		void fill()
		{
			*src |= ((1 << cnt) - 1) << r_cnt;
		}

		// 値を代入
		C_bit& operator = (TYPE rhs)
		{
			clear();						// 0クリア
			*src |= (rhs & max) << r_cnt;	// 代入
			return *this;
		}

		// 演算
		TYPE operator + (TYPE rhs)		{	return ((TYPE)(*this) + rhs) & max;	}
		TYPE operator - (TYPE rhs)		{	return ((TYPE)(*this) - rhs) & max;	}
		TYPE operator * (TYPE rhs)		{	return ((TYPE)(*this) * rhs) & max;	}
		TYPE operator / (TYPE rhs)		{	return ((TYPE)(*this) / rhs) & max;	}
		TYPE operator % (TYPE rhs)		{	return ((TYPE)(*this) % rhs) & max;	}
		C_bit& operator += (TYPE rhs)	{	return (*this = *this + rhs);		}
		C_bit& operator -= (TYPE rhs)	{	return (*this = *this - rhs);		}
		C_bit& operator *= (TYPE rhs)	{	return (*this = *this * rhs);		}
		C_bit& operator /= (TYPE rhs)	{	return (*this = *this / rhs);		}
		C_bit& operator %= (TYPE rhs)	{	return (*this = *this % rhs);		}

		// 値を文字列で返す
		TSTR	str()
		{
			TSTR s;
			TYPE value = (TYPE)(*this);
			for (int i = 0; i < cnt; i++)	{
				s = ((value & 1) ? _T("1") : _T("0")) + s;
				value >>= 1;
			}
			return s;
		}

	private:
		TYPE*	src;
		int		size;
		int		cnt;	// 使用するビット数
		int		l_cnt;	// 左側の未使用のビット数
		int		r_cnt;	// 右側の未使用のビット数
		int		max;	// 最大値
	};


	// ****************************************************************
	// POD 構造体の比較
	//		POD 構造体 - (Plane Old Data）
	//		int などの基本型のみをメンバに持つ、Cと互換性のある構造体
	// ================================================================
	template <typename TYPE>
	TYPE& pod_assign(TYPE* lhs, const TYPE* rhs)
	{
		memcpy(lhs, rhs, sizeof(TYPE));
		return *lhs;
	}

	template <typename TYPE>
	bool pod_equal(const TYPE* lhs, const TYPE* rhs)
	{
		return memcmp(lhs, rhs, sizeof(TYPE)) == 0;
	}

	template <typename TYPE>
	bool pod_array_equal(const ARRAY<TYPE>& lhs, const ARRAY<TYPE>& rhs)
	{
		return (lhs.empty() && rhs.empty())
			|| (lhs.size() == rhs.size() && memcmp(&lhs[0], &rhs[0], lhs.size() * sizeof(TYPE)) == 0);
	}

	// ****************************************************************
	// イテレータの参照先の比較演算子
	// ================================================================
	template <typename T>
	bool iterator_comp(T lhs, T rhs)
	{
		if (false);
		else if (lhs == NULL)	return false;
		else if (rhs == NULL)	return true;

		return *lhs < *rhs;
	}

	template <typename T>
	bool PTR_comp(T* lhs, T* rhs)
	{
		return *lhs < *rhs;
	}

	template <typename T>
	bool BSP_comp(BSP<T>& lhs, BSP<T>& rhs)
	{
		return *lhs < *rhs;
	}

	template <typename T>
	bool WP_comp(BWP<T>& lhs, BWP<T>& rhs)
	{
		return *lhs.lock() < *rhs.lock();
	}

	// ****************************************************************
	// インデックス構造体
	// ================================================================
	struct C_index
	{
		int		offset;
		int		size;

		// コンストラクタ
		C_index()						{	offset = 0;			size = 0;		}
		C_index(int offset_, int size_)	{	offset = offset_;	size = size_;	}

		// 基本演算子
		bool operator == (const C_index& rhs) const	{	return offset == rhs.offset && size == rhs.size;	}
		bool operator != (const C_index& rhs) const	{	return offset != rhs.offset || size != rhs.size;	}
	};

	// ****************************************************************
	// ポイント構造体
	// ================================================================
	struct C_size;
	struct C_size3;

	struct C_point : public POINT
	{
		// コンストラクタ
		C_point()				{	x = 0;			y = 0;			}
		C_point(int x_, int y_)	{	x = x_;			y = y_;			}
		C_point(POINT& point)	{	x = point.x;	y = point.y;	}

		// 基本演算子
		C_point		operator - () const						{	return C_point(- x, - y);				}
		bool		operator == (const C_point& rhs) const	{	return x == rhs.x && y == rhs.y;		}
		bool		operator != (const C_point& rhs) const	{	return x != rhs.x || y != rhs.y;		}
		C_point		operator + (const C_point& rhs) const	{	return C_point(x + rhs.x, y + rhs.y);	}
		C_point		operator - (const C_point& rhs) const	{	return C_point(x - rhs.x, y - rhs.y);	}
		C_point&	operator += (const C_point& rhs)		{	x += rhs.x;	y += rhs.y;	return *this;	}
		C_point&	operator -= (const C_point& rhs)		{	x -= rhs.x;	y -= rhs.y;	return *this;	}

		operator C_size();

		// コラムとローで指定
		LONG&		column()	{ return x; }
		LONG&		row()		{ return y; }
	};

	struct C_point3
	{
		int x;
		int y;
		int z;

		// コンストラクタ
		C_point3()							{	x = 0;	y = 0;	z = 0;	}
		C_point3(int x_, int y_, int z_)	{	x = x_;	y = y_;	z = z_;	}

		// 基本演算子
		bool		operator == (const C_point3& rhs) const	{	return x == rhs.x && y == rhs.y && z == rhs.z;		}
		bool		operator != (const C_point3& rhs) const	{	return x != rhs.x || y != rhs.y || z != rhs.z;		}
		C_point3	operator + (const C_point3& rhs) const	{	return C_point3(x + rhs.x, y + rhs.y, z + rhs.z);	}
		C_point3	operator - (const C_point3& rhs) const	{	return C_point3(x - rhs.x, y - rhs.y, z - rhs.z);	}
		C_point3&	operator += (const C_point3& rhs)		{	x += rhs.x;	y += rhs.y;	z += rhs.z;	return *this;	}
		C_point3&	operator -= (const C_point3& rhs)		{	x -= rhs.x;	y -= rhs.y;	z -= rhs.z;	return *this;	}

		operator C_size3();
	};

	// ****************************************************************
	// サイズ構造体
	// ================================================================
	struct C_size : public SIZE
	{
		// コンストラクタ
		C_size()					{	cx = 0;		cy = 0;		}
		C_size(int cx_, int cy_)	{	cx = cx_;	cy = cy_;	}

		// 基本演算子
		C_size	operator - () const				{	return C_size(- cx, - cy);				}
		bool	operator == (const C_size& rhs)	{	return cx == rhs.cx && cy == rhs.cy;	}
		bool	operator != (const C_size& rhs)	{	return cx != rhs.cx || cy != rhs.cy;	}
		C_size	operator + (const C_size& rhs)	{	return C_size(cx + rhs.cx, cy + rhs.cy);	}
		C_size	operator - (const C_size& rhs)	{	return C_size(cx - rhs.cx, cy - rhs.cy);	}
		C_size	operator * (const int& rhs)		{	return C_size(cx * rhs, cy * rhs);		}
		C_size	operator / (const int & rhs)	{	return C_size(cx / rhs, cy / rhs);		}

		operator C_point();

		// コラムとローで指定
		LONG&		column()	{ return cx; }
		LONG&		row()		{ return cy; }

	};

	struct C_size3
	{
		int cx;
		int cy;
		int cz;

		// コンストラクタ
		C_size3()							{	cz = 0;		cy = 0;		cz = 0;		}
		C_size3(int cx_, int cy_, int cz_)	{	cz = cx_;	cy = cy_;	cz = cz_;	}

		// 基本演算子
		C_size3	operator - () const					{	return C_size3(- cx, - cy, - cz);						}
		bool	operator == (const C_size3& rhs)	{	return cx == rhs.cx && cy == rhs.cy && cz == rhs.cz;	}
		bool	operator != (const C_size3& rhs)	{	return cx != rhs.cx || cy != rhs.cy || cz != rhs.cz;	}
		C_size3	operator + (const C_size3& rhs)		{	return C_size3(cx + rhs.cx, cy + rhs.cy, cz + rhs.cz);	}
		C_size3	operator - (const C_size3& rhs)		{	return C_size3(cx - rhs.cx, cy - rhs.cy, cz - rhs.cz);	}
		C_size3	operator * (const C_size3& rhs)		{	return C_size3(cx * rhs.cx, cy * rhs.cy, cz * rhs.cz);	}

		operator C_point3();
	};

	// ****************************************************************
	// ポイント - サイズ変換
	// ================================================================
	inline C_point::operator C_size()	{	return C_size(x, y);			}
	inline C_point3::operator C_size3()	{	return C_size3(x, y, z);		}
	inline C_size::operator C_point()	{	return C_point(cx, cy);			}
	inline C_size3::operator C_point3()	{	return C_point3(cx, cy, cz);	}

	// ****************************************************************
	// レクト構造体
	// ================================================================
	struct C_rect : public RECT
	{
		// コンストラクタ
		C_rect()							{	left = 0;			top = 0;		right = 0;			bottom = 0;				}
		C_rect(int l, int t, int r, int b)	{	left = l;			top = t;		right = r;			bottom = b;				}
		C_rect(RECT& rect)					{	left = rect.left;	top = rect.top;	right = rect.right;	bottom = rect.bottom;	}
		C_rect(C_point lt, C_point rb)		{	left = lt.x;		top = lt.y;		right = rb.x;		bottom = rb.y;			}

		// コンストラクトヘルパー
		static	C_rect	by_size(int l, int t, int w, int h)		{	return C_rect(l, t, l + w, t + h);	}
		static	C_rect	by_size(C_point pos, C_size size)		{	return C_rect(pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);	}

		// 基本演算子
		bool	operator == (const C_rect& rhs) const		{	return left == rhs.left && top == rhs.top && right == rhs.right && bottom == rhs.bottom;	}
		bool	operator != (const C_rect& rhs) const		{	return left != rhs.left || top != rhs.top || right != rhs.right || bottom != rhs.bottom;	}
		C_rect	operator + (const C_point point) const		{	return C_rect(left + point.x, top + point.y, right + point.x, bottom + point.y);	}
		C_rect	operator - (const C_point point) const		{	return C_rect(left - point.x, top - point.y, right - point.x, bottom - point.y);	}
		C_rect&	operator += (const C_point point)			{	*this = *this + point;		return *this;	}
		C_rect&	operator -= (const C_point point)			{	*this = *this - point;		return *this;	}

		// マージン
		C_rect	reduce_margin(C_rect margin)					{	return C_rect(left + margin.left, top + margin.top, right - margin.right, bottom - margin.bottom);	}


		int		width() const					{	return right - left;				}
		int		height() const					{	return bottom - top;				}
		C_point	left_top() const				{	return C_point(left, top);			}
		C_point	right_top() const				{	return C_point(right, top);			}
		C_point	left_bottom() const				{	return C_point(left, bottom);		}
		C_point	right_bottom() const			{	return C_point(right, bottom);		}
		C_size	size() const					{	return C_size(width(), height());	}

		void	set_left_top(int x, int y)		{	left = x;		top = y;							}
		void	set_left_top(C_point pos)		{	left = pos.x;	top = pos.y;						}
		void	set_width(int width)			{	right = left + width;								}
		void	set_height(int height)			{	bottom = top + height;								}
		void	set_size(int width, int height)	{	right = left + width;	bottom = top + height;		}
		void	set_size(C_size size)			{	right = left + size.cx;	bottom = top + size.cy;		}

		void	move_x(int x)					{	left += x;	right += x;								}
		void	move_y(int y)					{	top += y;	bottom += y;							}
		void	move(int x, int y)				{	left += x;	top += y;	right += x;	bottom += y;	}

		bool	is_in(int x, int y)  const		{	return (left <= x && x < right) && (top <= y && y < bottom);	}
		bool	is_in(C_point point)  const		{	return is_in(point.x, point.y);									}
	};

	// ****************************************************************
	// 色構造体
	// ================================================================
	struct C_rgb
	{
		BYTE	b;
		BYTE	g;
		BYTE	r;

		C_rgb()								{	r = 0;  g = 0;  b = 0;	}
		C_rgb(BYTE r_, BYTE g_, BYTE b_)	{	r = r_;	g = g_; b = b_;	}

		// 基本演算子
		bool operator == (const C_rgb& rhs) const	{	return r == rhs.r && g == rhs.g && b == rhs.b;	}
		bool operator != (const C_rgb& rhs) const	{	return r != rhs.r || g != rhs.g || b != rhs.b;	}
	};

	struct C_argb
	{
		BYTE	b;
		BYTE	g;
		BYTE	r;
		BYTE	a;

		C_argb()									{	a = 0;  r = 0;  g = 0;  b = 0;				}
		C_argb(BYTE a_, BYTE r_, BYTE g_, BYTE b_)	{	a = a_; r = r_; g = g_; b = b_;				}
		C_argb(BYTE a_, C_rgb rgb)					{	a = a_; r = rgb.r; g = rgb.g; b = rgb.b;	}

		// 変換演算子
		operator const DWORD() const	{	return (a << 24) + (r << 16) + (g << 8) + b;	}

		// 基本演算子
		bool operator == (const C_argb& rhs) const	{	return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;	}
		bool operator != (const C_argb& rhs) const	{	return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a;	}
		bool operator <  (const C_argb& rhs) const	{	return r < rhs.r || (r == rhs.r && (g < rhs.g || (g == rhs.g && (b < rhs.b || (b == rhs.b && a < rhs.a)))));	}

		C_rgb	rgb()	{	return C_rgb(r, g, b);	}
	};

	// ****************************************************************
	// 計測
	//		TIME_START ～ TIME_END 間を cnt 回実行し
	//		かかった時間を表示します。
	//		デバッグ計測用に使います。
	// ================================================================
	//		TIME_START(cnt)	{
	//			// 処理
	//		} TIME_END;
	// ================================================================
	#define		TIME_START(cnt)	{	DWORD start = ::GetTickCount();	for (int i = 0; i < cnt; i++)		
	#define		TIME_END			DWORD end = ::GetTickCount();	info_box(tostr(end - start));		}



};
