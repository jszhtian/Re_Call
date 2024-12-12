#pragma		once

namespace NT3
{

// ****************************************************************
// D3D サーフェス
// ================================================================
class C_d3d_surface
{
public:
	C_d3d_surface();
	C_d3d_surface(BIP<IDirect3DSurface9> surface);
	~C_d3d_surface();

	void	init();
	void	release();

	bool	create_surface(int width, int height);
	bool	create_surface(int width, int height, D3DFORMAT format, D3DPOOL pool);
	bool	create_dpst_buffer(int width, int height);

	bool	lock_rect(D3DLOCKED_RECT* texture_bit, C_rect* rect, DWORD flags);	// ロック
	bool	unlock_rect();														// アンロック

	IDirect3DSurface9*	body()			{	return m_surface.get();	}
	IDirect3DSurface9*	operator ->()	{	return m_surface.get();	}
	C_size				get_size()		{	return C_size(m_width, m_height);		}
	C_size				get_size_ex()	{	return C_size(m_width_ex, m_height_ex);	}			

private:

	BIP<IDirect3DSurface9>	m_surface;
	int						m_width;
	int						m_height;
	int						m_width_ex;
	int						m_height_ex;
};

// ****************************************************************
// D3D バッファ
// ================================================================
class Cd3d_buffer
{
public:
	Cd3d_buffer();
	Cd3d_buffer(BIP<ID3DXBuffer> buffer);
	~Cd3d_buffer();

	void	init();
	void	release();

	ID3DXBuffer*	body()			{	return m_buffer.get();	}
	ID3DXBuffer*	operator ->()	{	return m_buffer.get();	}

private:

	BIP<ID3DXBuffer>	m_buffer;
};


}



