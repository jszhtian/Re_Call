#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

	// ****************************************************************
	// ���b�Z�[�W�{�b�N�X
	// ================================================================

	// ���b�Z�[�W�{�b�N�X
	int message_box(HWND h_wnd, CTSTR& msg, CTSTR& caption, UINT type);
	int message_box(CTSTR& msg, CTSTR& caption, UINT type);

	// �G���[�{�b�N�X
	int error_box(CTSTR& err_msg, UINT type = MB_OK);

	// �x���{�b�N�X
	int warning_box(CTSTR& msg, UINT type = MB_OK);
	int warning_box(CTSTR& msg, CTSTR& caption, UINT type = MB_OK);

	// ���{�b�N�X
	int info_box(CTSTR& msg, UINT type = MB_OK);
	int info_box(CTSTR& msg, CTSTR& caption, UINT type = MB_OK);

	// ****************************************************************
	// �s�o�b�Í�
	// ================================================================
	void tpc_angou_encode(BYTE* data, int size);						// �s�o�b�Í��̈Í���
	void tpc_angou_decode(BYTE* data, int size);						// �s�o�b�Í��̕�����

	// ****************************************************************
	// �G���R�[�h
	// ================================================================
	TSTR url_encode(CTSTR& value);
	TSTR base64_encode(ARRAY<BYTE>& buf);
	TSTR hmac_base64_encode(CTSTR& key, CTSTR& data);

	// ****************************************************************
	// �R���s���[�^�h�c���擾
	// ================================================================
	DWORD get_cpu_id();
	DWORD get_computer_id();

	// ****************************************************************
	// �n�r�̃o�[�W�����𕶎���Ŏ擾
	// ================================================================
	TSTR get_os_version_str();

	// ****************************************************************
	// ���݂̓��t�𕶎���Ŏ擾
	// ================================================================
	TSTR get_sys_time_str();

	// ****************************************************************
	// UUID �𕶎���Ŏ擾
	// ================================================================
	TSTR get_uuid();

	// ****************************************************************
	// �~���[�e�b�N�X
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
	// bool�^�\����
	// ================================================================
	struct S_bool
	{
		bool value;
	};

	// ****************************************************************
	// �^�ϊ�
	// ================================================================
	int		bool_to_int(bool value);

	// ****************************************************************
	// C �z��̃T�C�Y���擾
	// ================================================================
	#define		sizeof_array(list)		(sizeof(list) / sizeof(list[0]))

	// ****************************************************************
	// ����������
	// ================================================================
	void	fill_memory(void* buf, int cnt, BYTE data);					// ���������w�肵���l�Ŗ��߂�i1 byte �P�ʁj
	void	fill_memory_2(void* buf, int cnt, WORD data);				// ���������w�肵���l�Ŗ��߂�i2 byte �P�ʁj
	void	fill_memory_4(void* buf, int cnt, DWORD data);				// ���������w�肵���l�Ŗ��߂�i4 byte �P�ʁj
	void	fill_memory_x(void* buf, int size, int cnt, void* data);	// ���������w�肵���l�Ŗ��߂�ix byte �P�ʁj

	// ****************************************************************
	// �r�b�g����
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

		// �A�h���X���Z�b�g
		void assign(TYPE* src_, int pos_, int cnt_)
		{
			src = src_;
			size = sizeof(TYPE) * 8;
			l_cnt = pos_;
			r_cnt = size - pos_ - cnt_;
			cnt = cnt_;
			max = (1 << cnt_) - 1;
		}

		// int �^�Ƃ��Ēl�����o��
		operator TYPE() const
		{
			return (TYPE)((*src) << l_cnt) >> (l_cnt + r_cnt);
		}

		// 0�N���A
		void clear()
		{
			TYPE left = ((1 << l_cnt) - 1) << (cnt + r_cnt);
			TYPE right = (1 << r_cnt) - 1;
			*src &= left | right;
		}

		// 1�t�B��
		void fill()
		{
			*src |= ((1 << cnt) - 1) << r_cnt;
		}

		// �l����
		C_bit& operator = (TYPE rhs)
		{
			clear();						// 0�N���A
			*src |= (rhs & max) << r_cnt;	// ���
			return *this;
		}

		// ���Z
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

		// �l�𕶎���ŕԂ�
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
		int		cnt;	// �g�p����r�b�g��
		int		l_cnt;	// �����̖��g�p�̃r�b�g��
		int		r_cnt;	// �E���̖��g�p�̃r�b�g��
		int		max;	// �ő�l
	};


	// ****************************************************************
	// POD �\���̂̔�r
	//		POD �\���� - (Plane Old Data�j
	//		int �Ȃǂ̊�{�^�݂̂������o�Ɏ��AC�ƌ݊����̂���\����
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
	// �C�e���[�^�̎Q�Ɛ�̔�r���Z�q
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
	// �C���f�b�N�X�\����
	// ================================================================
	struct C_index
	{
		int		offset;
		int		size;

		// �R���X�g���N�^
		C_index()						{	offset = 0;			size = 0;		}
		C_index(int offset_, int size_)	{	offset = offset_;	size = size_;	}

		// ��{���Z�q
		bool operator == (const C_index& rhs) const	{	return offset == rhs.offset && size == rhs.size;	}
		bool operator != (const C_index& rhs) const	{	return offset != rhs.offset || size != rhs.size;	}
	};

	// ****************************************************************
	// �|�C���g�\����
	// ================================================================
	struct C_size;
	struct C_size3;

	struct C_point : public POINT
	{
		// �R���X�g���N�^
		C_point()				{	x = 0;			y = 0;			}
		C_point(int x_, int y_)	{	x = x_;			y = y_;			}
		C_point(POINT& point)	{	x = point.x;	y = point.y;	}

		// ��{���Z�q
		C_point		operator - () const						{	return C_point(- x, - y);				}
		bool		operator == (const C_point& rhs) const	{	return x == rhs.x && y == rhs.y;		}
		bool		operator != (const C_point& rhs) const	{	return x != rhs.x || y != rhs.y;		}
		C_point		operator + (const C_point& rhs) const	{	return C_point(x + rhs.x, y + rhs.y);	}
		C_point		operator - (const C_point& rhs) const	{	return C_point(x - rhs.x, y - rhs.y);	}
		C_point&	operator += (const C_point& rhs)		{	x += rhs.x;	y += rhs.y;	return *this;	}
		C_point&	operator -= (const C_point& rhs)		{	x -= rhs.x;	y -= rhs.y;	return *this;	}

		operator C_size();

		// �R�����ƃ��[�Ŏw��
		LONG&		column()	{ return x; }
		LONG&		row()		{ return y; }
	};

	struct C_point3
	{
		int x;
		int y;
		int z;

		// �R���X�g���N�^
		C_point3()							{	x = 0;	y = 0;	z = 0;	}
		C_point3(int x_, int y_, int z_)	{	x = x_;	y = y_;	z = z_;	}

		// ��{���Z�q
		bool		operator == (const C_point3& rhs) const	{	return x == rhs.x && y == rhs.y && z == rhs.z;		}
		bool		operator != (const C_point3& rhs) const	{	return x != rhs.x || y != rhs.y || z != rhs.z;		}
		C_point3	operator + (const C_point3& rhs) const	{	return C_point3(x + rhs.x, y + rhs.y, z + rhs.z);	}
		C_point3	operator - (const C_point3& rhs) const	{	return C_point3(x - rhs.x, y - rhs.y, z - rhs.z);	}
		C_point3&	operator += (const C_point3& rhs)		{	x += rhs.x;	y += rhs.y;	z += rhs.z;	return *this;	}
		C_point3&	operator -= (const C_point3& rhs)		{	x -= rhs.x;	y -= rhs.y;	z -= rhs.z;	return *this;	}

		operator C_size3();
	};

	// ****************************************************************
	// �T�C�Y�\����
	// ================================================================
	struct C_size : public SIZE
	{
		// �R���X�g���N�^
		C_size()					{	cx = 0;		cy = 0;		}
		C_size(int cx_, int cy_)	{	cx = cx_;	cy = cy_;	}

		// ��{���Z�q
		C_size	operator - () const				{	return C_size(- cx, - cy);				}
		bool	operator == (const C_size& rhs)	{	return cx == rhs.cx && cy == rhs.cy;	}
		bool	operator != (const C_size& rhs)	{	return cx != rhs.cx || cy != rhs.cy;	}
		C_size	operator + (const C_size& rhs)	{	return C_size(cx + rhs.cx, cy + rhs.cy);	}
		C_size	operator - (const C_size& rhs)	{	return C_size(cx - rhs.cx, cy - rhs.cy);	}
		C_size	operator * (const int& rhs)		{	return C_size(cx * rhs, cy * rhs);		}
		C_size	operator / (const int & rhs)	{	return C_size(cx / rhs, cy / rhs);		}

		operator C_point();

		// �R�����ƃ��[�Ŏw��
		LONG&		column()	{ return cx; }
		LONG&		row()		{ return cy; }

	};

	struct C_size3
	{
		int cx;
		int cy;
		int cz;

		// �R���X�g���N�^
		C_size3()							{	cz = 0;		cy = 0;		cz = 0;		}
		C_size3(int cx_, int cy_, int cz_)	{	cz = cx_;	cy = cy_;	cz = cz_;	}

		// ��{���Z�q
		C_size3	operator - () const					{	return C_size3(- cx, - cy, - cz);						}
		bool	operator == (const C_size3& rhs)	{	return cx == rhs.cx && cy == rhs.cy && cz == rhs.cz;	}
		bool	operator != (const C_size3& rhs)	{	return cx != rhs.cx || cy != rhs.cy || cz != rhs.cz;	}
		C_size3	operator + (const C_size3& rhs)		{	return C_size3(cx + rhs.cx, cy + rhs.cy, cz + rhs.cz);	}
		C_size3	operator - (const C_size3& rhs)		{	return C_size3(cx - rhs.cx, cy - rhs.cy, cz - rhs.cz);	}
		C_size3	operator * (const C_size3& rhs)		{	return C_size3(cx * rhs.cx, cy * rhs.cy, cz * rhs.cz);	}

		operator C_point3();
	};

	// ****************************************************************
	// �|�C���g - �T�C�Y�ϊ�
	// ================================================================
	inline C_point::operator C_size()	{	return C_size(x, y);			}
	inline C_point3::operator C_size3()	{	return C_size3(x, y, z);		}
	inline C_size::operator C_point()	{	return C_point(cx, cy);			}
	inline C_size3::operator C_point3()	{	return C_point3(cx, cy, cz);	}

	// ****************************************************************
	// ���N�g�\����
	// ================================================================
	struct C_rect : public RECT
	{
		// �R���X�g���N�^
		C_rect()							{	left = 0;			top = 0;		right = 0;			bottom = 0;				}
		C_rect(int l, int t, int r, int b)	{	left = l;			top = t;		right = r;			bottom = b;				}
		C_rect(RECT& rect)					{	left = rect.left;	top = rect.top;	right = rect.right;	bottom = rect.bottom;	}
		C_rect(C_point lt, C_point rb)		{	left = lt.x;		top = lt.y;		right = rb.x;		bottom = rb.y;			}

		// �R���X�g���N�g�w���p�[
		static	C_rect	by_size(int l, int t, int w, int h)		{	return C_rect(l, t, l + w, t + h);	}
		static	C_rect	by_size(C_point pos, C_size size)		{	return C_rect(pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);	}

		// ��{���Z�q
		bool	operator == (const C_rect& rhs) const		{	return left == rhs.left && top == rhs.top && right == rhs.right && bottom == rhs.bottom;	}
		bool	operator != (const C_rect& rhs) const		{	return left != rhs.left || top != rhs.top || right != rhs.right || bottom != rhs.bottom;	}
		C_rect	operator + (const C_point point) const		{	return C_rect(left + point.x, top + point.y, right + point.x, bottom + point.y);	}
		C_rect	operator - (const C_point point) const		{	return C_rect(left - point.x, top - point.y, right - point.x, bottom - point.y);	}
		C_rect&	operator += (const C_point point)			{	*this = *this + point;		return *this;	}
		C_rect&	operator -= (const C_point point)			{	*this = *this - point;		return *this;	}

		// �}�[�W��
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
	// �F�\����
	// ================================================================
	struct C_rgb
	{
		BYTE	b;
		BYTE	g;
		BYTE	r;

		C_rgb()								{	r = 0;  g = 0;  b = 0;	}
		C_rgb(BYTE r_, BYTE g_, BYTE b_)	{	r = r_;	g = g_; b = b_;	}

		// ��{���Z�q
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

		// �ϊ����Z�q
		operator const DWORD() const	{	return (a << 24) + (r << 16) + (g << 8) + b;	}

		// ��{���Z�q
		bool operator == (const C_argb& rhs) const	{	return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;	}
		bool operator != (const C_argb& rhs) const	{	return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a;	}
		bool operator <  (const C_argb& rhs) const	{	return r < rhs.r || (r == rhs.r && (g < rhs.g || (g == rhs.g && (b < rhs.b || (b == rhs.b && a < rhs.a)))));	}

		C_rgb	rgb()	{	return C_rgb(r, g, b);	}
	};

	// ****************************************************************
	// �v��
	//		TIME_START �` TIME_END �Ԃ� cnt ����s��
	//		�����������Ԃ�\�����܂��B
	//		�f�o�b�O�v���p�Ɏg���܂��B
	// ================================================================
	//		TIME_START(cnt)	{
	//			// ����
	//		} TIME_END;
	// ================================================================
	#define		TIME_START(cnt)	{	DWORD start = ::GetTickCount();	for (int i = 0; i < cnt; i++)		
	#define		TIME_END			DWORD end = ::GetTickCount();	info_box(tostr(end - start));		}



};
