#pragma		once

namespace NT3
{

// ****************************************************************
// 頂点バッファ
// ================================================================
class C_d3d_vertex_buffer
{
public:
	C_d3d_vertex_buffer();
	~C_d3d_vertex_buffer();

	void	init();
	void	release();

	bool	create_vertex_buffer(int vertex_cnt, DWORD FVF, int FVF_size, DWORD usage, D3DPOOL pool);
	bool	lock(int offset, int size, void** pp_buffer, DWORD flag);
	bool	unlock();

	int		get_vertex_count()	{	return m_vertex_cnt;				}
	DWORD	get_FVF()			{	return m_FVF;						}
	int		get_FVF_size()		{	return m_FVF_size;					}
	int		get_buffer_size()	{	return m_vertex_cnt * m_FVF_size;	}

	IDirect3DVertexBuffer9*	body()			{	return m_buffer.get();	}
	IDirect3DVertexBuffer9*	operator ->()	{	return m_buffer.get();	}

private:

	BIP<IDirect3DVertexBuffer9>		m_buffer;
	int								m_vertex_cnt;
	DWORD							m_FVF;
	int								m_FVF_size;
	DWORD							m_usage;
	D3DPOOL							m_pool;
};


}



