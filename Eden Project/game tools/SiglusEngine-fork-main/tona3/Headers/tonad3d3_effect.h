#pragma		once

namespace NT3
{

// ****************************************************************
// D3D エフェクト
// ================================================================
class C_d3d_effect
{
public:
	C_d3d_effect();
	C_d3d_effect(BIP<ID3DXEffect> effect);
	~C_d3d_effect();

	void	init();
	void	release();

	bool	load_effect_from_buffer(BYTE* buffer, int size);
	bool	load_effect_from_resource(LPCTSTR name, LPCTSTR type);

	ID3DXEffect*	body()			{	return m_effect.get();	}
	ID3DXEffect*	operator ->()	{	return m_effect.get();	}

protected:

	BIP<ID3DXEffect>	m_effect;
};

// ****************************************************************
// D3D スプライトのエフェクト
// ================================================================
class C_d3d_default_effect : public C_d3d_effect
{
public:
	bool	create_default_effect();
	bool	load_effect_from_buffer(BYTE* buffer, int size);
	bool	load_effect_from_resource(LPCTSTR name, LPCTSTR type);

	bool	on_lost_device();
	bool	on_reset_device();

	D3DXHANDLE		m_sprite_technique[2][2][2][2][2][2][2][2][2][3];
	D3DXHANDLE		m_mask_technique;
	D3DXHANDLE		m_shimi_technique;
	D3DXHANDLE		m_shimi_inv_technique;
	D3DXHANDLE		m_cross_raster_horizontal_technique;
	D3DXHANDLE		m_cross_raster_vertical_technique;
	D3DXHANDLE		m_raster_horizontal_technique;
	D3DXHANDLE		m_raster_vertical_technique;
	D3DXHANDLE		m_cross_explosion_blur_technique;
	D3DXHANDLE		m_explosion_blur_technique;
	D3DXHANDLE		m_cross_mosaic_technique;
	D3DXHANDLE		m_mosaic_technique;

	D3DXHANDLE		m_texture[4];
	D3DXHANDLE		m_float_array[4];

private:
	bool	set_effect_handle();
};


}

