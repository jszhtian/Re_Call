#pragma		once

// ****************************************************************
// ■とな３：アロケータ
// ================================================================
namespace NT3
{
	// ****************************************************************
	// とな３：アロケータ
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
		// 使えるバッファがなければバッファを確保
		if (m_enable_no_table.empty())	{

			// バッファ番号を指定
			int buffer_no = (int)m_buffer.size();	// old_size = buffer_no
			m_buffer.resize(buffer_no + 1);
			// 実際にバッファを確保
			void* p = new BYTE[t_buffer_size];
			m_buffer[buffer_no] = p;
			// 解放のためのマップを作成
			m_address_map.insert(std::pair<void*, int>(p, buffer_no));
			m_alloc_cnt ++;
			// バッファのアドレスを返す
			return m_buffer[buffer_no];
		}
		else	{
			// 使えるバッファ番号を取得
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

	// グローバルなメモリマネージャ
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

	// 適切なメモリマネージャからメモリを取得する
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
		T* p = (T *)mem_alloc(sizeof(T));	// メモリを確保して
		new((void *)p) T;					// コンストラクタを実行
		return p;
	}
	template <typename T>
	inline void delete_free(T* p)
	{
		p->~T();							// デストラクタを実行して
		mem_free(p, sizeof(T));				// メモリを解放する
	}

	// 適切なメモリマネージャからメモリを解放する
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

	// アロケータを作る
	template <class T>
	class C_allocator
	{
	public:
		// 型定義
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;

		// アロケータを U 型にバインドする
		template <class U>
		struct rebind
		{
			typedef C_allocator<U> other;	
		};

		// コンストラクタ
		C_allocator() throw()	{}
		C_allocator(const C_allocator&) throw()	{}
		template <class U> C_allocator(const C_allocator<U>&) throw()	{}

		// デストラクタ
		~C_allocator() throw()	{}

		// メモリを割り当てる
		pointer allocate(size_type num, const void* hint = 0)
		{
			size_type size = num * sizeof(T);
			return (pointer)mem_alloc(size);
		}
		// 割り当て済みの領域を初期化する
		void construct(pointer p, const_reference value)
		{
			new((void *)p) T(value);
		}
		// メモリを解放する
		void deallocate(pointer p, size_type num)
		{
			size_type size = num * sizeof(T);
			mem_free(p, size);
		}
		// 初期化済みの領域を削除する
		void destroy(pointer p)
		{
			p->~T();
		}

		// アドレスを返す
		pointer address(reference value) const				{	return &value;	}
		const_pointer address(const_reference value) const	{	return &value;	}

		// 割り当てることができる最大の要素数を返す
		size_type max_size() const throw()
		{
			return std::numeric_limits<size_t>::max() / sizeof(T);
		}
	};
}
