#pragma		once

// ****************************************************************
// とな３
// ================================================================
namespace NT3
{
	class	C_d3d_texture;

	// ****************************************************************
	// D3D マスクパラメータ
	// ================================================================
	struct C_d3d_mask_param
	{
		int		x;
		int		y;
	};

	// ****************************************************************
	// D3D マスク
	// ================================================================
	class C_d3d_mask
	{
	public:
		C_d3d_mask();
		~C_d3d_mask();

		void	init();

		// パラメータ
		void	set_param_x(int x)		{	m_param.x = x;	}
		void	set_param_y(int y)		{	m_param.y = y;	}

		int		get_param_x()			{	return m_param.x;	}
		int		get_param_y()			{	return m_param.y;	}

		// テクスチャ
		void				set_texture(BSP<C_d3d_texture> texture)	{	m_texture = texture;	}
		BSP<C_d3d_texture>	get_texture()							{	return m_texture;		}

	private:
		C_d3d_mask_param	m_param;
		BSP<C_d3d_texture>	m_texture;
	};
}
