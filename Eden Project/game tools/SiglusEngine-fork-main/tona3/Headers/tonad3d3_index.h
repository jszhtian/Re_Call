#pragma		once

namespace NT3
{

// ****************************************************************
// インデックスバッファ
// ================================================================
class C_d3d_index_buffer
{
public:
	C_d3d_index_buffer();
	C_d3d_index_buffer(BIP<IDirect3DIndexBuffer9> buffer);
	~C_d3d_index_buffer();

	void	init();
	void	release();

	bool	create_index_buffer(int index_cnt, DWORD usage, D3DPOOL pool);
	bool	lock(int offset, int size, void** pp_buffer, DWORD flag);
	bool	unlock();

	int		get_index_cnt()		{	return m_index_cnt;					}
	int		get_buffer_size()	{	return m_index_cnt * sizeof(WORD);	}

	IDirect3DIndexBuffer9*	body()			{	return m_buffer.get();	}
	IDirect3DIndexBuffer9*	operator ->()	{	return m_buffer.get();	}

private:

	BIP<IDirect3DIndexBuffer9>		m_buffer;
	int								m_index_cnt;
	DWORD							m_usage;
	D3DPOOL							m_pool;
};


}



