#pragma		once

// ****************************************************************
// ���ƂȂR�F�A���P�[�^
// ================================================================
namespace NT3
{
	// ****************************************************************
	// �ƂȂR�F�A���P�[�^
	// ================================================================
	template <int t_buffer_size>
	class C_mem_manager
	{
	public:
		C_mem_manager();
		void* alloc();
		void free(void* p);
		void release();

	private:
		ARRAY<void*>			m_buffer;
		ARRAY<int>				m_enable_no_table;
		std::map<void*, int>	m_address_map;
		int						m_alloc_cnt;
	};

	template <int t_buffer_size>
	C_mem_manager<t_buffer_size>::C_mem_manager()
	{
		m_alloc_cnt = 0;
	}

	template <int t_buffer_size>
	void* C_mem_manager<t_buffer_size>::alloc()
	{
		// �g����o�b�t�@���Ȃ���΃o�b�t�@���m��
		if (m_enable_no_table.empty())	{

			// �o�b�t�@�ԍ����w��
			int buffer_no = (int)m_buffer.size();	// old_size = buffer_no
			m_buffer.resize(buffer_no + 1);
			// ���ۂɃo�b�t�@���m��
			void* p = new BYTE[t_buffer_size];
			m_buffer[buffer_no] = p;
			// ����̂��߂̃}�b�v���쐬
			m_address_map.insert(std::pair<void*, int>(p, buffer_no));
			m_alloc_cnt ++;
			// �o�b�t�@�̃A�h���X��Ԃ�
			return m_buffer[buffer_no];
		}
		else	{
			// �g����o�b�t�@�ԍ����擾
			int buffer_no = m_enable_no_table.back();
			m_enable_no_table.pop_back();
			m_alloc_cnt --;
			return m_buffer[buffer_no];
		}
	}

	template <int t_buffer_size>
	void C_mem_manager<t_buffer_size>::free(void* p)
	{
		std::map<void*, int>::iterator itr = m_address_map.find(p);
		if (itr != m_address_map.end())	{
			m_enable_no_table.push_back(itr->second);
		}
	}

	template <int t_buffer_size>
	void C_mem_manager<t_buffer_size>::release()
	{
		size_t buffer_cnt = m_buffer.size();
		for (size_t i = 0; i < buffer_cnt; i++)	{
			delete[] m_buffer[i];
		}
	}

	// �O���[�o���ȃ������}�l�[�W��
	extern	C_mem_manager<32>					G_mem_mng_32;
	extern	C_mem_manager<128>					G_mem_mng_128;
	extern	C_mem_manager<1024>					G_mem_mng_1KB;
	extern	C_mem_manager<1024 * 4>				G_mem_mng_4KB;
	extern	C_mem_manager<1024 * 16>			G_mem_mng_16KB;
	extern	C_mem_manager<1024 * 64>			G_mem_mng_64KB;
	extern	C_mem_manager<1024 * 256>			G_mem_mng_256KB;
	extern	C_mem_manager<1024 * 1024>			G_mem_mng_1MB;
	extern	C_mem_manager<1024 * 1024 * 4>		G_mem_mng_4MB;
	extern	C_mem_manager<1024 * 1024 * 16>		G_mem_mng_16MB;
	extern	C_mem_manager<1024 * 1024 * 64>		G_mem_mng_64MB;
	extern	C_mem_manager<1024 * 1024 * 256>	G_mem_mng_256MB;

	// �K�؂ȃ������}�l�[�W�����烁�������擾����
	inline void* mem_alloc(size_t size)
	{
		if (size <= 32)	{
			return G_mem_mng_32.alloc();
		}
		else if (size <= 128)	{
			return G_mem_mng_128.alloc();
		}
		else if (size <= 1024)	{
			return G_mem_mng_1KB.alloc();
		}
		else if (size <= 1024 * 4)	{
			return G_mem_mng_4KB.alloc();
		}
		else if (size <= 1024 * 16)	{
			return G_mem_mng_16KB.alloc();
		}
		else if (size <= 1024 * 64)	{
			return G_mem_mng_64KB.alloc();
		}
		else if (size <= 1024 * 256)	{
			return G_mem_mng_256KB.alloc();
		}
		else if (size <= 1024 * 1024)	{
			return G_mem_mng_1MB.alloc();
		}
		else if (size <= 1024 * 1024 * 4)	{
			return G_mem_mng_4MB.alloc();
		}
		else if (size <= 1024 * 1024 * 16)	{
			return G_mem_mng_16MB.alloc();
		}
		else if (size <= 1024 * 1024 * 64)	{
			return G_mem_mng_64MB.alloc();
		}
		else	{
			return G_mem_mng_256MB.alloc();
		}
	}

	template <typename T>
	inline T* new_alloc()
	{
		T* p = (T *)mem_alloc(sizeof(T));	// ���������m�ۂ���
		new((void *)p) T;					// �R���X�g���N�^�����s
		return p;
	}
	template <typename T>
	inline void delete_free(T* p)
	{
		p->~T();							// �f�X�g���N�^�����s����
		mem_free(p, sizeof(T));				// ���������������
	}

	// �K�؂ȃ������}�l�[�W�����烁�������������
	inline void mem_free(void* p, size_t size)
	{
		if (size <= 32)	{
			G_mem_mng_32.free(p);
		}
		else if (size <= 128)	{
			G_mem_mng_128.free(p);
		}
		else if (size <= 1024)	{
			G_mem_mng_1KB.free(p);
		}
		else if (size <= 1024 * 4)	{
			G_mem_mng_4KB.free(p);
		}
		else if (size <= 1024 * 16)	{
			G_mem_mng_16KB.free(p);
		}
		else if (size <= 1024 * 64)	{
			G_mem_mng_64KB.free(p);
		}
		else if (size <= 1024 * 256)	{
			G_mem_mng_256KB.free(p);
		}
		else if (size <= 1024 * 1024)	{
			G_mem_mng_1MB.free(p);
		}
		else if (size <= 1024 * 1024 * 4)	{
			G_mem_mng_4MB.free(p);
		}
		else if (size <= 1024 * 1024 * 16)	{
			G_mem_mng_16MB.free(p);
		}
		else if (size <= 1024 * 1024 * 64)	{
			G_mem_mng_64MB.free(p);
		}
		else	{
			G_mem_mng_256MB.free(p);
		}
	}

	// �A���P�[�^�����
	template <class T>
	class C_allocator
	{
	public:
		// �^��`
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;

		// �A���P�[�^�� U �^�Ƀo�C���h����
		template <class U>
		struct rebind
		{
			typedef C_allocator<U> other;	
		};

		// �R���X�g���N�^
		C_allocator() throw()	{}
		C_allocator(const C_allocator&) throw()	{}
		template <class U> C_allocator(const C_allocator<U>&) throw()	{}

		// �f�X�g���N�^
		~C_allocator() throw()	{}

		// �����������蓖�Ă�
		pointer allocate(size_type num, const void* hint = 0)
		{
			size_type size = num * sizeof(T);
			return (pointer)mem_alloc(size);
		}
		// ���蓖�čς݂̗̈������������
		void construct(pointer p, const_reference value)
		{
			new((void *)p) T(value);
		}
		// ���������������
		void deallocate(pointer p, size_type num)
		{
			size_type size = num * sizeof(T);
			mem_free(p, size);
		}
		// �������ς݂̗̈���폜����
		void destroy(pointer p)
		{
			p->~T();
		}

		// �A�h���X��Ԃ�
		pointer address(reference value) const				{	return &value;	}
		const_pointer address(const_reference value) const	{	return &value;	}

		// ���蓖�Ă邱�Ƃ��ł���ő�̗v�f����Ԃ�
		size_type max_size() const throw()
		{
			return std::numeric_limits<size_t>::max() / sizeof(T);
		}
	};
}
